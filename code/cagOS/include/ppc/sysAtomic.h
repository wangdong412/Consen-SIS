#ifdef __SYSATOM_H__
#define __SYSATOM_H__

#include <libc/cagboot.h>
#define TRUE 1
#define FALSE 0

/* atomically clear a memory location */
UINT32 atomicClear
   (
   UINT32 *target /* memory location to clear */
   );

/* atomically set a memory location */
   /*return the old value of target*/
UINT32 atomic32Set
   (
   UINT32 *target,    /* memory location to set */
   UINT32 value    /* set with this value */
   );

   /* Get the value of a shared memory atomically */
UINT32 atomic32Get
    (
    UINT32 * target    /* address of atom to be retrieved */
    );


    /* atomically add a value to a memory location */
UINT32 atomic32Add
   (
   UINT32 *target,    /* memory location to add to */
   UINT32 value    /* value to add */
   );

   /* atomically subtract a value from a memory location */
UINT32 atomic32Sub
   (
   UINT32 *target,    /* memory location to subtract from */
   UINT32 value    /* value to subtract */
   );

   /* atomically perform a bitwise AND on a memory location */
UINT32 atomic32And
   (
   UINT32 *target,    /* memory location to AND */
   UINT32 value    /* AND with this value */
   );

   /* atomically perform a bitwise OR on memory location */
UINT32 atomic32Or
   (
   UINT32 *target,    /* memory location to OR */
   UINT32 value    /* OR with this value */
   );

   /* atomically perform a bitwise XOR on a memory location */
UINT32 atomic32Xor
   (
   UINT32 *target,    /* memory location to XOR */
   UINT32 value    /* XOR with this value */
   );

   /* atomically compare-and-swap the contents of a memory location */
BOOL atomic32Cas
    (
    UINT32 *target,	        /* memory location to compare-and-swap */
    UINT32 ldValue,	/* compare to this value */
    UINT32 newValue	/* swap with this value */
    );
#endif
