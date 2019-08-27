/* heap.c - bmalloc/free handling facilities */

/*
 *------------------------------------------------------------
 * Project:	cagOS
 * Goal:
 * Copyright (c) 2013, 2014 CAG Systems, Inc.
 *------------------------------------------------------------
 *
 *    @@@@     @@@      @@@@@       ####      ######
 *  @@       @@  @@    @@         ##    ##   ##
 * @@       @@    @@  @@   @@@@  ##      ## ######
 * @@      @@@@@@@@@@ @@    @@    ##    ##      ##
 *  @@@@@ @@@      @@@  @@@@@@      ####  #######
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable CAG license agreement.
 *
 * 1 tab == 4 spaces!
 *------------------------------------------------------------
 * modification history
 * --------------------
 * 03jun15,Jeff created
 *
 * DESCRIPTION
 * This file contains
 */

/*
 * A sample implementation of bmalloc() and bfree() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 */

#include <config.h>
#include <libc/cagboot.h>
#include <libc/stdio.h>
#include <libc/stddef.h>
#include <libc/assert.h>
#include <kernel/sysOs.h>

/* Block sizes must not get too small. */
#define Heap_BLOCK_SIZE	( ( size_t ) ( HeapStructSize * 2 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE		( ( size_t ) 8 )
#define TOTAL_HEAP_SIZE           ( ( size_t ) ( 1024 * 1024 ) )
#define BYTE_ALIGNMENT_MASK ( 0x0007 )
#define BYTE_ALIGNMENT 8

/* Allocate the memory for the heap. */
#if( APPLICATION_ALLOCATED_HEAP == 1 )
/* The application writer has already defined the array used for the RTOS
heap - probably so it can be placed in a special segment or address. */
extern UINT8 mHeap[ TOTAL_HEAP_SIZE ];
#else
static UINT8 mHeap[ TOTAL_HEAP_SIZE ];
#endif /* APPLICATION_ALLOCATED_HEAP */

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct A_BLOCK_LINK {
    struct A_BLOCK_LINK *pNextFreeBlock;	/*<< The next free block in the list. */
    size_t BlockSize;						/*<< The size of the free block. */
} BlockLink_t;

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvtBlockFreeList( BlockLink_t *pBlockToInsert );

/*
 * Called automatically to setup the required heap structures the first time
 * bmalloc() is called.
 */
static void HeapInit( void );

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t HeapStructSize	= ( ( sizeof( BlockLink_t ) + ( ( ( size_t ) BYTE_ALIGNMENT_MASK ) - ( size_t ) 1 ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK ) );

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t xStart, *pxEnd = NULL;

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t FreeBytesRemaining = 0U;
static size_t MinimumEverFreeBytesRemaining = 0U;

/* Gets set to the top bit of an size_t type.  When this bit in the BlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t BlockAllocatedBit = 0;

/*-----------------------------------------------------------*/

void *bmalloc( size_t WantedSize )
{
    BlockLink_t *pBlock, *pPreviousBlock, *pNewBlockLink;
    void *pvReturn = NULL;
    OS_ERR  err;

    OSSchedLock (&err);
    {
        /* If this is the first call to bmalloc then the heap will require
        initialisation to setup the list of free blocks. */
        if( pxEnd == NULL ) {
            HeapInit();
        } else {
        }

        /* Check the requested block size is not so large that the top bit is
        set.  The top bit of the block size member of the BlockLink_t structure
        is used to determine who owns the block - the application or the
        kernel, so it must be free. */
        if( ( WantedSize & BlockAllocatedBit ) == 0 ) {
            /* The wanted size is increased so it can contain a BlockLink_t
            structure in addition to the requested amount of bytes. */
            if( WantedSize > 0 ) {
                WantedSize += HeapStructSize;

                /* Ensure that blocks are always aligned to the required number
                of bytes. */
                if( ( WantedSize & BYTE_ALIGNMENT_MASK ) != 0x00 ) {
                    /* Byte alignment required. */
                    WantedSize += ( BYTE_ALIGNMENT - ( WantedSize & BYTE_ALIGNMENT_MASK ) );
                    assert ( WantedSize & BYTE_ALIGNMENT_MASK );
                } else {
                }
            } else {
            }

            if( ( WantedSize > 0 ) && ( WantedSize <= FreeBytesRemaining ) ) {
                /* Traverse the list from the start	(lowest address) block until
                one	of adequate size is found. */
                pPreviousBlock = &xStart;
                pBlock = xStart.pNextFreeBlock;
                while( ( pBlock->BlockSize < WantedSize ) && ( pBlock->pNextFreeBlock != NULL ) ) {
                    pPreviousBlock = pBlock;
                    pBlock = pBlock->pNextFreeBlock;
                }

                /* If the end marker was reached then a block of adequate size
                was	not found. */
                if( pBlock != pxEnd ) {
                    /* Return the memory space pointed to - jumping over the
                    BlockLink_t structure at its start. */
                    pvReturn = ( void * ) ( ( ( UINT8 * ) pPreviousBlock->pNextFreeBlock ) + HeapStructSize );

                    /* This block is being returned for use so must be taken out
                    of the list of free blocks. */
                    pPreviousBlock->pNextFreeBlock = pBlock->pNextFreeBlock;

                    /* If the block is larger than required it can be split into
                    two. */
                    if( ( pBlock->BlockSize - WantedSize ) > Heap_BLOCK_SIZE ) {
                        /* This block is to be split into two.  Create a new
                        block following the number of bytes requested. The void
                        cast is used to prevent byte alignment warnings from the
                        compiler. */
                        pNewBlockLink = ( void * ) ( ( ( UINT8 * ) pBlock ) + WantedSize );
                        assert ( ( ( UINT32 ) pNewBlockLink ) & BYTE_ALIGNMENT_MASK ) ;

                        /* Calculate the sizes of two blocks split from the
                        single block. */
                        pNewBlockLink->BlockSize = pBlock->BlockSize - WantedSize;
                        pBlock->BlockSize = WantedSize;

                        /* Insert the new block into the list of free blocks. */
                        prvtBlockFreeList( ( pNewBlockLink ) );
                    } else {
                    }

                    FreeBytesRemaining -= pBlock->BlockSize;

                    if( FreeBytesRemaining < MinimumEverFreeBytesRemaining ) {
                        MinimumEverFreeBytesRemaining = FreeBytesRemaining;
                    } else {
                    }

                    /* The block is being returned - it is allocated and owned
                    by the application and has no "next" block. */
                    pBlock->BlockSize |= BlockAllocatedBit;
                    pBlock->pNextFreeBlock = NULL;
                } else {
                }
            } else {
            }
        } else {
        }

    }
    OSSchedUnlock (&err);

#if( configUSE_MALLOC_FAILED_HOOK == 1 )
    {
        if( pvReturn == NULL ) {
            extern void vApplicationMallocFailedHook( void );
            vApplicationMallocFailedHook();
        } else {
        }
    }
#endif

    assert ( ( ( UINT32 ) pvReturn ) & BYTE_ALIGNMENT_MASK );
    return pvReturn;
}


void bfree( void *pv )
{
    UINT8 *puc = ( UINT8 * ) pv;
    BlockLink_t *pLink;
    OS_ERR  err;


    if( pv != NULL ) {
        /* The memory being freed will have an BlockLink_t structure immediately
        before it. */
        puc -= HeapStructSize;

        /* This casting is to keep the compiler from issuing warnings. */
        pLink = ( void * ) puc;

        /* Check the block is actually allocated. */
        assert ( pLink->BlockSize & BlockAllocatedBit );
        assert( pLink->pNextFreeBlock );

        if( ( pLink->BlockSize & BlockAllocatedBit ) != 0 ) {
            if( pLink->pNextFreeBlock == NULL ) {
                /* The block is being returned to the heap - it is no longer
                allocated. */
                pLink->BlockSize &= ~BlockAllocatedBit;

                OSSchedLock (&err);
                {
                    /* Add this block to the list of free blocks. */
                    FreeBytesRemaining += pLink->BlockSize;
                    prvtBlockFreeList( ( ( BlockLink_t * ) pLink ) );
                }
                OSSchedUnlock (&err);
            } else {
            }
        } else {
        }
    }
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize( void )
{
    return FreeBytesRemaining;
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize( void )
{
    return MinimumEverFreeBytesRemaining;
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks( void )
{
    /* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

static void HeapInit( void )
{
    BlockLink_t *pFirstFreeBlock;
    UINT8 *pucAlignedHeap;
    UINT32 uAddress;
    size_t xTotalHeapSize = TOTAL_HEAP_SIZE;

    /* Ensure the heap starts on a correctly aligned boundary. */
    uAddress = ( UINT32 ) mHeap;

    if( ( uAddress & BYTE_ALIGNMENT_MASK ) != 0 ) {
        uAddress += ( BYTE_ALIGNMENT - 1 );
        uAddress &= ~( ( UINT32 ) BYTE_ALIGNMENT_MASK );
        xTotalHeapSize -= uAddress - ( UINT32 ) mHeap;
    }

    pucAlignedHeap = ( UINT8 * ) uAddress;

    /* xStart is used to hold a pointer to the first item in the list of free
    blocks.  The void cast is used to prevent compiler warnings. */
    xStart.pNextFreeBlock = ( void * ) pucAlignedHeap;
    xStart.BlockSize = ( size_t ) 0;

    /* pxEnd is used to mark the end of the list of free blocks and is inserted
    at the end of the heap space. */
    uAddress = ( ( UINT32 ) pucAlignedHeap ) + xTotalHeapSize;
    uAddress -= HeapStructSize;
    uAddress &= ~( ( UINT32 ) BYTE_ALIGNMENT_MASK );
    pxEnd = ( void * ) uAddress;
    pxEnd->BlockSize = 0;
    pxEnd->pNextFreeBlock = NULL;

    /* To start with there is a single free block that is sized to take up the
    entire heap space, minus the space taken by pxEnd. */
    pFirstFreeBlock = ( void * ) pucAlignedHeap;
    pFirstFreeBlock->BlockSize = uAddress - ( UINT32 ) pFirstFreeBlock;
    pFirstFreeBlock->pNextFreeBlock = pxEnd;

    /* Only one block exists - and it covers the entire usable heap space. */
    MinimumEverFreeBytesRemaining = pFirstFreeBlock->BlockSize;
    FreeBytesRemaining = pFirstFreeBlock->BlockSize;

    /* Work out the position of the top bit in a size_t variable. */
    BlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void prvtBlockFreeList( BlockLink_t *pBlockToInsert )
{
    BlockLink_t *pIterator;
    UINT8 *puc;

    /* Iterate through the list until a block is found that has a higher address
    than the block being inserted. */
    for( pIterator = &xStart; pIterator->pNextFreeBlock < pBlockToInsert; pIterator = pIterator->pNextFreeBlock ) {
        /* Nothing to do here, just iterate to the right position. */
    }

    /* Do the block being inserted, and the block it is being inserted after
    make a contiguous block of memory? */
    puc = ( UINT8 * ) pIterator;
    if( ( puc + pIterator->BlockSize ) == ( UINT8 * ) pBlockToInsert ) {
        pIterator->BlockSize += pBlockToInsert->BlockSize;
        pBlockToInsert = pIterator;
    } else {
    }

    /* Do the block being inserted, and the block it is being inserted before
    make a contiguous block of memory? */
    puc = ( UINT8 * ) pBlockToInsert;
    if( ( puc + pBlockToInsert->BlockSize ) == ( UINT8 * ) pIterator->pNextFreeBlock ) {
        if( pIterator->pNextFreeBlock != pxEnd ) {
            /* Form one big block from the two blocks. */
            pBlockToInsert->BlockSize += pIterator->pNextFreeBlock->BlockSize;
            pBlockToInsert->pNextFreeBlock = pIterator->pNextFreeBlock->pNextFreeBlock;
        } else {
            pBlockToInsert->pNextFreeBlock = pxEnd;
        }
    } else {
        pBlockToInsert->pNextFreeBlock = pIterator->pNextFreeBlock;
    }

    /* If the block being inserted plugged a gab, so was merged with the block
    before and the block after, then it's pNextFreeBlock pointer will have
    already been set, and should not be set here as that would make it point
    to itself. */
    if( pIterator != pBlockToInsert ) {
        pIterator->pNextFreeBlock = pBlockToInsert;
    } else {
    }
}

