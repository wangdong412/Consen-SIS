/* lld.c - Source Code for Spansion's Low Level Driver */
/**************************************************************************
* Copyright (C)2011 Spansion LLC. All Rights Reserved .
*
* This software is owned and published by:
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming Spansion's
* Flash memory components. This software is licensed by Spansion to be adapted only
* for use in systems utilizing Spansion's Flash memories. Spansion is not be
* responsible for misuse or illegal use of this software for devices not
* supported herein.  Spansion is providing this source code "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the source code herein.
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY,
* FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  SPANSION WILL
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE,
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS,
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Copyright notice must be included with
* this software, whether used in part or whole, at all times.
*/
#include "lld.h"
#ifdef S29GLxxxP
#include "S29GLxxxP.h"
#define LLD_BUFFER_SIZE 32
#define LLD_SECTOR_SIZE (0x10000) //WORD
#define LLD_SUSP_RESUME_DELAY  30
#endif

int nor_flash_errno = 0;
#ifndef LLD_STATUS_REG
FLSTATE FLSTATEOP = FLSTATE_NOT_BUSY;
unsigned char g_suspend_state = FLRESUME;
ADDRESS g_suspend_sector, g_writebuffer_sector;

#define DQ1_MASK   (0x02 * LLD_DEV_MULTIPLIER)  /* DQ1 mask for all interleave devices */
#define DQ2_MASK   (0x04 * LLD_DEV_MULTIPLIER)  /* DQ2 mask for all interleave devices */
#define DQ3_MASK   (0x08 * LLD_DEV_MULTIPLIER)  /* DQ3 mask for all interleave devices */
#define DQ5_MASK   (0x20 * LLD_DEV_MULTIPLIER)  /* DQ5 mask for all interleave devices */
#define DQ6_MASK   (0x40 * LLD_DEV_MULTIPLIER)  /* DQ6 mask for all interleave devices */
#define DQ6_TGL_DQ1_MASK (dq6_toggles >> 5) 	/* Mask for DQ1 when device DQ6 toggling */
#define DQ6_TGL_DQ5_MASK (dq6_toggles >> 1) 	/* Mask for DQ5 when device DQ6 toggling */
#endif

extern int intLock(void);
extern void intUnlock(unsigned int);

#define LLD_LOCK    unsigned int lldlock;   \
                    int x1 = __LINE__;\
                    lldlock = lld_lock(x1); \
                    if (lldlock == 0xffffffff) return -7;
                    

#define LLD_UNLOCK  x1 = __LINE__;\
                    unsigned int lldunlock = lld_unlock(lldlock,x1); \
                    if (lldunlock == 0xffffffff) return -8;

static int testlld = 0;

/* private functions */
static unsigned int lld_lock(int xx)
{
    if (testlld !=0) {
        /*printf("lock a locked %d\n",xx);
        while(1);*/
        return 0xffffffff;
    }
    testlld = 1;

    return intLock();
}

static int lld_unlock(unsigned int locker,int xx)
{
    if (testlld !=1) {
/*        printf("unlock a unlocked %d\n",xx);
        while(1);*/
        return 0xffffffff;
    }
    testlld = 0;

    intUnlock(locker);
    return 0;
}
/* Public Functions  */
/******************************************************************************
*
* lld_GetVersion - Get LLD Version Number in string format.
*
*  PARAMETERS:  LLD_CHAR[] versionStr
*               Pointer to an empty char array. The size of the array has to be at
*               least 12 in order to avoid buffer overflow.
*  RETURNS: version number in string returned in versionStr
*
* ERRNO:
*/
void lld_GetVersion( LLD_CHAR versionStr[])
{
    LLD_CHAR*  pVer = (LLD_CHAR*)LLD_VERSION;

    if (versionStr) {
        while (*pVer) *versionStr++ = *pVer++;
        *versionStr = 0;
    }
}

/******************************************************************************
*
* lld_InitCmd - Initialize LLD
*
*
* RETURNS: void
*
* ERRNO:
*/
void lld_InitCmd
(
    FLASHDATA * base_addr			/* device base address in system */
)
{
    (void) base_addr;
#ifdef S29GLxxxP
    FLSTATEOP = FLSTATE_NOT_BUSY;
#endif
}

/******************************************************************************
*
* lld_ResetCmd - Writes a Software Reset command to the flash device
*
*
* RETURNS: void
*
* ERRNO:
*/

void lld_ResetCmd
(
    FLASHDATA * base_addr   /* device base address in system */
)
{
#ifdef LLD_RESET_CMD
    /* Write Software RESET command */
    FLASH_WR(base_addr, 0, NOR_RESET_CMD);
    lld_InitCmd(base_addr);
#endif
}

#ifdef LLD_SECTOR_ERASE_CMD_1
/******************************************************************************
*
* lld_SectorEraseCmd - Writes a Sector Erase Command to Flash Device
*
* This function only issues the Sector Erase Command sequence.
* Erase status polling is not implemented in this function.
*
* RETURNS: void
*
*/

void lld_SectorEraseCmd
(
    FLASHDATA * base_addr,                   /* device base address in system */
    ADDRESS offset                           /* address offset from base address */
)
{

    /* Issue unlock sequence command */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_ERASE_SETUP_CMD);

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    /* Write Sector Erase Command to Offset */
    FLASH_WR(base_addr, offset, NOR_SECTOR_ERASE_CMD);

#ifdef S29GLxxxP
    FLSTATEOP = FLSTATE_ERASE;
#endif
}
#endif // LLD_SECTOR_ERASE_CMD_1

#ifdef LLD_CHIP_ERASE_CMD_1
/******************************************************************************
*
* lld_ChipEraseCmd - Writes a Chip Erase Command to Flash Device
*
* This function only issues the Chip Erase Command sequence.
* Erase status polling is not implemented in this function.
*
* RETURNS: void
*
*/
void lld_ChipEraseCmd
(
    FLASHDATA * base_addr    /* device base address in system */
)
{
    /* Issue inlock sequence command */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_ERASE_SETUP_CMD);

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    /* Write Chip Erase Command to Base Address */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_CHIP_ERASE_CMD);
}
#endif //LLD_CHIP_ERASE_CMD_1

#ifdef LLD_PROGRAM_CMD_1
/******************************************************************************
*
* lld_ProgramCmd - Writes a Program Command to Flash Device
*
* This function only issues the Program Command sequence.
* Program status polling is not implemented in this function.
*
* RETURNS: void
*
*/
void lld_ProgramCmd
(
    FLASHDATA * base_addr,               /* device base address in system */
    ADDRESS offset,                  /* address offset from base address */
    FLASHDATA *pgm_data_ptr          /* variable containing data to program */
)
{
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    /* Write Program Command */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_PROGRAM_CMD);
    /* Write Data */
    FLASH_WR(base_addr, offset, *pgm_data_ptr);
}
#endif // LLD_PROGRAM_CMD_1

#ifdef LLD_WRITE_BUFFER_CMD_1
/******************************************************************************
*
* lld_WriteToBufferCmd - Writes "Write to Buffer Pgm" Command sequence to Flash
*
* RETURNS: void
*
*/
void lld_WriteToBufferCmd
(
    FLASHDATA * base_addr,               /* device base address in system */
    ADDRESS offset                       /* address offset from base address */
)
{
    /* Issue unlock command sequence */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    /* Write Write Buffer Load Command */
    FLASH_WR(base_addr, offset, NOR_WRITE_BUFFER_LOAD_CMD);

#ifdef S29GLxxxP
    FLSTATEOP = FLSTATE_WRITEBUFFER;
    g_writebuffer_sector = offset / LLD_SECTOR_SIZE;
#endif
}
/******************************************************************************
*
* lld_ProgramBufferToFlashCmd - Writes "Pgm Buffer To Flash" Cmd sequence to Flash
*
* RETURNS: void
*
*/
void lld_ProgramBufferToFlashCmd
(
    FLASHDATA * base_addr,               /* device base address in system */
    ADDRESS offset                       /* address offset from base address */
)
{
    /* Transfer Buffer to Flash Command */
    FLASH_WR(base_addr, offset, NOR_WRITE_BUFFER_PGM_CONFIRM_CMD);
}
/******************************************************************************
*
* lld_WriteBufferAbortResetCmd - Writes "Write To Buffer Abort" Reset to Flash
*
* RETURNS: void
*
*/
void lld_WriteBufferAbortResetCmd
(
    FLASHDATA * base_addr        /* device base address in system */
)
{

    /* Issue Write Buffer Abort Reset Command Sequence */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    /* Write to Buffer Abort Reset Command */
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_WRITE_BUFFER_ABORT_RESET_CMD);
}
#endif // LLD_WRITE_BUFFER_CMD_1

#ifdef LLD_SUSP_RESUME_CMD_1
/******************************************************************************
*
* lld_ProgramSuspendCmd - Writes Suspend Command to Flash
*
* RETURNS: void
*
*/
void lld_ProgramSuspendCmd
(
    FLASHDATA * base_addr,         /* device base address in system    */
    ADDRESS offset                 /* address offset from base address */
)
{
    /* Write Suspend Command */
    FLASH_WR(base_addr, offset, NOR_SUSPEND_CMD);
}

/******************************************************************************
*
* lld_EraseSuspendCmd - Writes Suspend Command to Flash
*
* RETURNS: void
*
*/
void lld_EraseSuspendCmd
(
    FLASHDATA * base_addr,       /* device base address in system    */
    ADDRESS offset               /* address offset from base address */
)
{

#ifdef PAUSE_BETWEEN_ERASE_SUSPENDS
    DelayMilliseconds(5);
#endif

#ifdef S29GLxxxP
    FLASHDATA dq6_toggles;
    FLASHDATA status_read_1, status_read_2, status_read_3;

    status_read_1 = FLASH_RD(base_addr, offset);
    status_read_2 = FLASH_RD(base_addr, offset);
    status_read_3 = FLASH_RD(base_addr, offset);

    /* Any DQ6 toggles */
    dq6_toggles = ((status_read_1 ^ status_read_2) &        /* Toggles between read1 and read2 */
                   (status_read_2 ^ status_read_3) &        /* Toggles between read2 and read3 */
                   DQ6_MASK );                              /* Check for DQ6 only */

    if (dq6_toggles && (g_suspend_state == FLRESUME)) {
        g_suspend_state = FLSUSPEND;
        g_suspend_sector = offset / LLD_SECTOR_SIZE;
    }
#endif

    /* Write Suspend Command */
    FLASH_WR(base_addr, offset, NOR_SUSPEND_CMD);
}

/******************************************************************************
*
* lld_EraseResumeCmd - Writes Resume Command to Flash
*
* RETURNS: void
*
*/
void lld_EraseResumeCmd
(
    FLASHDATA * base_addr,       /* device base address in system    */
    ADDRESS offset               /* address offset from base address */
)
{

    /* Write Resume Command */
    FLASH_WR(base_addr, offset, NOR_RESUME_CMD);

#ifdef S29GLxxxP
    g_suspend_state = FLRESUME;
#endif
}

/******************************************************************************
*
* lld_ProgramResumeCmd - Writes Resume Command to Flash
*
* RETURNS: void
*
*/
void lld_ProgramResumeCmd
(
    FLASHDATA * base_addr,       /* device base address in system    */
    ADDRESS offset               /* address offset from base address */
)
{

    /* Write Resume Command */
    FLASH_WR(base_addr, offset, NOR_RESUME_CMD);

}
#endif /* LLD_SUSP_RESUME_CMD_1 */


#ifdef LLD_SUSP_RESUME_OP_1
/******************************************************************************
*
* lld_EraseSuspendOp - Performs Erase Suspend Operation
*
* Function performs erase suspend operation.
* Function issues erase suspend commands and will poll for completion.
*
* RETURNS: DEVSTATUS
*          DEV_ERASE_SUSPEND: Suspension OP is successful.
*          DEV_ERASE_SUSPEND_ERROR; Suspension OP fails. If Erase OP is complete,
*                           The function will return DEV_ERASE_SUSPEND_ERROR.
*/
DEVSTATUS lld_EraseSuspendOp
(
    FLASHDATA * base_addr,		/* device base address is system */
    ADDRESS offset              /* address offset from base address */
)
{
    FLASHDATA    status_reg;
#ifdef LLD_POLL_TOGGLE
#ifndef LLD_POLL_STATUS
    unsigned long	polling_counter = 0xFFFFFFFF;
#endif
#endif
#ifdef LLD_POLL_STATUS
    lld_EraseSuspendCmd(base_addr);			/* issue erase suspend command */
    status_reg = lld_Poll(base_addr, 0);		/* wait for device done */
    if( (status_reg & DEV_ERASE_SUSP_MASK) == DEV_ERASE_SUSP_MASK )
        return( DEV_ERASE_SUSPEND  );	       /* Erase suspend  */
    return( DEV_ERASE_SUSPEND_ERROR );       /* Erase suspend error */
#else
#ifdef LLD_POLL_TOGGLE
    LLD_LOCK;
    lld_EraseSuspendCmd(base_addr, offset);	/* issue erase suspend command */
    do {
        polling_counter--;
        status_reg = lld_StatusGetReg(base_addr, offset);
    } while((status_reg == DEV_BUSY) && polling_counter);

    if( (status_reg & DEV_ERASE_SUSP_MASK) == DEV_ERASE_SUSP_MASK ) {
        LLD_UNLOCK;
        return( DEV_ERASE_SUSPEND  );	       /* Erase suspend  */
    }
    LLD_UNLOCK;
    return( DEV_ERASE_SUSPEND_ERROR );       /* Erase suspend error */
#else
    DelayMicroseconds(LLD_SUSP_RESUME_DELAY);
    return( DEV_ERASE_SUSPEND  );	       /* Erase suspend  */
#endif
#endif
}

/******************************************************************************
*
* lld_ProgramSuspendOp - Performs Program Suspend Operation
*
* Function pergorm program suspend operation.
* Function issues program suspend commands and will poll for completion.
*
* RETURNS: DEVSTATUS
           DEV_PROGRAM_SUSPEND: Program Suspension OP is successful.
           DEV_PROGRAM_SUSPEND_ERROR: Program Suspension OP fails.
                If Program OP is complete, this function will return
                DEV_PROGRAM_SUSPEND_ERROR.
*/
DEVSTATUS lld_ProgramSuspendOp
(
    FLASHDATA * base_addr,		/* Device base address is system */
    ADDRESS offset              /* address offset from base address */
)
{
    FLASHDATA    status_reg;

#ifdef LLD_POLL_TOGGLE
#ifndef LLD_POLL_STATUS
    unsigned long	polling_counter = 0xFFFFFFFF;
#endif
#endif

#ifdef LLD_POLL_STATUS
    lld_ProgramSuspendCmd(base_addr);			/* Issue program suspend command */
    status_reg = lld_Poll( base_addr, 0 );
    if( (status_reg & DEV_PROGRAM_SUSP_MASK) == DEV_PROGRAM_SUSP_MASK )
        return( DEV_PROGRAM_SUSPEND );	    /* Program suspend */
    return( DEV_PROGRAM_SUSPEND_ERROR  );	    /* Program suspend error */
#else
#ifdef LLD_POLL_TOGGLE
    LLD_LOCK;
    lld_ProgramSuspendCmd(base_addr, offset);	/* Issue program suspend command */
    do {
        polling_counter--;
        status_reg = lld_StatusGetReg(base_addr, offset);
    } while((status_reg == DEV_BUSY) && polling_counter);

    if( (status_reg & DEV_PROGRAM_SUSP_MASK) == DEV_PROGRAM_SUSP_MASK ) {
        LLD_UNLOCK;
        return( DEV_PROGRAM_SUSPEND );	    /* Program suspend */
    }
    LLD_UNLOCK;
    return( DEV_PROGRAM_SUSPEND_ERROR  );	    /* Program suspend error */
#else
    DelayMicroseconds(LLD_SUSP_RESUME_DELAY);
    return( DEV_PROGRAM_SUSPEND  );	       /* Erase suspend  */
#endif
#endif
}
#endif /* LLD_SUSP_RESUME_OP_1 */


#ifdef LLD_GET_ID_CMD_1
/******************************************************************************
*
* lld_GetDeviceId - Get device ID operation
*
* RETURNS: three byte ID in a single int
*
*/
unsigned int lld_GetDeviceId
(
    FLASHDATA * base_addr   /* device base address in system */
)
{
    unsigned int id;


    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_AUTOSELECT_CMD);

#ifndef LLD_CONFIGURATION_X8X16_AS_X8
#ifndef LLD_CONFIGURATION_X8_AS_X8
    id  = (unsigned int)(FLASH_RD(base_addr, 0x0001) & 0x000000FF) << 16;
    id |= (unsigned int)(FLASH_RD(base_addr, 0x000E) & 0x000000FF) <<  8;
    id |= (unsigned int)(FLASH_RD(base_addr, 0x000F) & 0x000000FF)      ;
#else
    id  = (unsigned int)(FLASH_RD(base_addr, 0x0002) & 0x000000FF) << 16;
    id |= (unsigned int)(FLASH_RD(base_addr, 0x001C) & 0x000000FF) <<  8;
    id |= (unsigned int)(FLASH_RD(base_addr, 0x001E) & 0x000000FF)      ;
#endif
#else
    id  = (unsigned int)(FLASH_RD(base_addr, 0x0002) & 0x000000FF) << 16;
    id |= (unsigned int)(FLASH_RD(base_addr, 0x001C) & 0x000000FF) <<  8;
    id |= (unsigned int)(FLASH_RD(base_addr, 0x001E) & 0x000000FF)      ;
#endif
    /* Write Software RESET command */
    FLASH_WR(base_addr, 0, NOR_RESET_CMD);

    return(id);
}
#endif /* LLD_GET_ID_CMD_1 */


#ifdef LLD_CONFIG_REG_CMD_3 /* For WS-P device Configuration Reg. 1 */
/******************************************************************************
*
* lld_SetConfigRegCmd - Set configuration register command for WS-P device.
* WS-P type device uses config. reg. 1 to set wait state.
* RETURNS: n/a
*/

void lld_SetConfigRegCmd
(
    FLASHDATA *   base_addr,	/* device base address in system */
    FLASHDATA value,			/* Configuration Register 0 value */
    FLASHDATA value1			/* Configuration Register 1 value */
)
{

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_SET_CONFIG_CMD);

    FLASH_WR(base_addr, 0, value);		/* configuration register 0 */
    FLASH_WR(base_addr, 1, value1);       /* configuration register 1 */

}

/******************************************************************************
*
* lld_ReadConfigRegCmd - Read configuration register command.
*
* RETURNS: config register
*
*/
FLASHDATA lld_ReadConfigRegCmd
(
    FLASHDATA *   base_addr,	/* device base address in system */
    FLASHDATA offset			/* configuration reg. offset 0/1 */
)
{

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_UNLOCK_DATA1);
    FLASH_WR(base_addr, LLD_UNLOCK_ADDR2, NOR_UNLOCK_DATA2);

    FLASH_WR(base_addr, LLD_UNLOCK_ADDR1, NOR_READ_CONFIG_CMD);
    return(FLASH_RD(base_addr, offset));

}
#endif /* LLD_CONFIG_REG_CMD_3 */

#ifdef LLD_POLL_TOGGLE
#ifdef LLD_DEV_FLASH
/*************************************************************************
*
* lld_Poll - Polls flash device for embedded operation completion
*
* Function polls the Flash device to determine when an embedded
*  operation has finished.
*  Function performs a verify on the polling address if the device
*  returns from status routine in a non-busy/non-error state.
*
* <polling_type> variable is used to determine type of polling to perform.
*  This variable tells the status routine what mode the device is in.
*  Future versions of this function will also use the <polling_type>
*  variable to properly initialize and implement watchdog timers.
*  Acceptable values for <polling_type> are:
*   LLD_P_POLL_PGM
*   LLD_P_POLL_WRT_BUF_PGM
*   LLD_P_POLL_SEC_ERS
*   LLD_P_POLL_CHIP_ERS
*
* RETURNS: DEVSTATUS
*
*/
DEVSTATUS lld_Poll
(
    FLASHDATA * base_addr,          /* device base address in system */
    ADDRESS offset,                 /* address offset from base address */
    FLASHDATA *exp_data_ptr,        /* expect data */
    FLASHDATA *act_data_ptr,        /* actual data */
    POLLING_TYPE polling_type       /* type of polling to perform */
)
{
    DEVSTATUS       dev_status;
    unsigned long   polling_counter = 0xFFFFFFFF;


    do {
        polling_counter--;
        dev_status = lld_StatusGet(base_addr, offset);
    } while((dev_status == DEV_BUSY) && polling_counter);

    /* if device returns status other than "not busy" then we
       have a polling error state. */
    if(dev_status != DEV_NOT_BUSY) {
#ifdef LLD_WRITE_BUFFER_OP
        if(dev_status == DEV_WRITE_BUFFER_ABORT) {
            lld_WriteBufferAbortResetCmd(base_addr);
        } else {
            /* Write Software RESET command */
            FLASH_WR(base_addr, 0, NOR_RESET_CMD);
        }
#endif
        /* indicate to caller that there was an error */
        return(dev_status);
    } else {
        LLD_LOCK;
        /* read the actual data */
        *act_data_ptr = FLASH_RD(base_addr, offset);

        /* Check that polling location verifies correctly */
        if((*exp_data_ptr & LLD_DB_READ_MASK) == (*act_data_ptr & LLD_DB_READ_MASK)) {
            dev_status = DEV_NOT_BUSY;
            LLD_UNLOCK;
            return(dev_status);
        } else {
            dev_status = DEV_VERIFY_ERROR;
            LLD_UNLOCK;
            return(dev_status);
        }
    }
}
#endif /* LLD_DEV_FLASH */
#endif /* LLD_POLL_TOGGLE */


#ifdef LLD_STATUS_GET
/******************************************************************************
*
* lld_StatusGet - Determines Flash operation status
*
*
* RETURNS: DEVSTATUS
*
*/

DEVSTATUS lld_StatusGet
(
    FLASHDATA *  base_addr,      /* device base address in system */
    ADDRESS      offset          /* address offset from base address */
)
{

    FLASHDATA dq2_toggles;
    FLASHDATA dq6_toggles;
    FLASHDATA status_read_1;
    FLASHDATA status_read_2;
    FLASHDATA status_read_3;

    status_read_1 = FLASH_RD(base_addr, offset);
    status_read_2 = FLASH_RD(base_addr, offset);
    status_read_3 = FLASH_RD(base_addr, offset);

    /* Any DQ6 toggles */
    dq6_toggles = ((status_read_1 ^ status_read_2) &        /* Toggles between read1 and read2 */
                   (status_read_2 ^ status_read_3) &        /* Toggles between read2 and read3 */
                   DQ6_MASK );                              /* Check for DQ6 only */

    /* Any DQ2 toggles */
    dq2_toggles = ((status_read_1 ^ status_read_2) &        /* Toggles between read1 and read2 */
                   (status_read_2 ^ status_read_3) &        /* Toggles between read2 and read3 */
                   DQ2_MASK );                              /* Check for DQ6 only */

    if (dq6_toggles) {
        /* Checking WriteBuffer Abort condition:
           Check for all devices that have DQ6 toggling also have Write Buffer Abort DQ1 set */
        if ((!dq2_toggles) &&
            ((DQ6_TGL_DQ1_MASK & status_read_1) == DQ6_TGL_DQ1_MASK)
#ifdef S29GLxxxP
            && (FLSTATEOP == FLSTATE_WRITEBUFFER)
            && (g_writebuffer_sector == (offset / LLD_SECTOR_SIZE))
#endif
           )
            return DEV_WRITE_BUFFER_ABORT;

        /* Checking Timeout condition:
           Check for all devices that have DQ6 toggling also have Time Out DQ5 set. */
        if ((DQ6_TGL_DQ5_MASK & status_read_1) == DQ6_TGL_DQ5_MASK )
            return DEV_EXCEEDED_TIME_LIMITS;

        /* No timeout, no WB error */
        return DEV_BUSY;
    } else { /* no DQ6 toggles on all devices */
        /* Checking Erase Suspend condition */
        status_read_1 = FLASH_RD(base_addr, offset);
        status_read_2 = FLASH_RD(base_addr, offset);

        /* Checking Erase Suspend condition */
        if ((((status_read_1 ^ status_read_2) & DQ2_MASK) == 0)
#ifdef S29GLxxxP
            && (g_suspend_state == FLRESUME)
#endif
           )
            return DEV_NOT_BUSY;         /* All devices DQ2 not toggling */
        else {						// at least one device is suspended
#ifdef S29GLxxxP
            if ( (g_suspend_state == FLSUSPEND) &&
                 (g_suspend_sector != (offset / LLD_SECTOR_SIZE))
               )
                return DEV_NOT_BUSY;     /* At least some devices toggle DQ2 */
            else
#endif
                return DEV_SUSPEND;          /* At least some devices toggle DQ2 */
        }
    }
}

/******************************************************************************
*
* lld_StatusClear - Clears the flash status
*
*
* RETURNS: void
*
*/
void lld_StatusClear
(
    FLASHDATA *  base_addr,      /* device base address in system */
    ADDRESS      offset          /* address offset from base address */
)
{
    FLASHDATA status_reg;

    status_reg = lld_StatusGetReg(base_addr, offset);

    /*WBA happened */
    if( ((status_reg & DEV_RDY_MASK) == DEV_RDY_MASK)  &&          /* device is ready */
        ((status_reg & DEV_PROGRAM_MASK) == DEV_PROGRAM_MASK) && /* program error */
        ((status_reg & DEV_ERASE_MASK) != DEV_ERASE_MASK)        /* no erase error */
      ) {
#ifdef LLD_WRITE_BUFFER_CMD_1
        lld_WriteBufferAbortResetCmd(base_addr);
#endif
    } else {
        lld_ResetCmd(base_addr);
    }
}


/******************************************************************************
*
* lld_StatusGetReg - Gets the flash status register bits
*
*
* RETURNS: FLASHDATA
*
*/
FLASHDATA lld_StatusGetReg
(
    FLASHDATA *  base_addr,      /* device base address in system */
    ADDRESS      offset          /* address offset from base address */
)
{
    FLASHDATA sector_protected = 0x0;
    FLASHDATA status_reg = 0x0000;
    DEVSTATUS status;

    status = lld_StatusGet(base_addr, offset);

    if(status == DEV_NOT_BUSY) {
        status_reg |= DEV_RDY_MASK;
    } else if(status == DEV_WRITE_BUFFER_ABORT) { /* Can happen only because of failed program op*/
        status_reg |= DEV_RDY_MASK;
        status_reg |= DEV_PROGRAM_MASK;
    } else if(status == DEV_EXCEEDED_TIME_LIMITS) { /* Can happen either by failed program or erase op */
        status_reg |= DEV_RDY_MASK;
        status_reg |= DEV_PROGRAM_MASK;
        status_reg |= DEV_ERASE_MASK;
    } else if(status == DEV_SUSPEND) { /* Only erase suspend can be detected by DQ polling */
        status_reg |= DEV_RDY_MASK;
        status_reg |= DEV_ERASE_SUSP_MASK;
    }

    if((status != DEV_BUSY) && (status != DEV_WRITE_BUFFER_ABORT)) {
#ifdef LLD_AUTOSELECT_CMD
        /* find if sectior protected by reading autoselct offset 0x02*/
        lld_AutoselectEntryCmd(base_addr);

        offset &= ~(0xFF);
#ifndef LLD_CONFIGURATION_X8X16_AS_X8
#ifndef LLD_CONFIGURATION_X8_AS_X8
        offset |= 0x02;
#else
        offset |= 0x04;
#endif
#else
        offset |= 0x04;
#endif
        sector_protected = (FLASH_RD(base_addr, offset));

        lld_AutoselectExitCmd(base_addr);
#endif
    }

    if(sector_protected == 0x1) {
        status_reg |= DEV_SEC_LOCK_MASK;
    }

    return status_reg;
}

#endif /// LLD_STATUS_GET 

#ifdef LLD_READ_OP
/******************************************************************************
*
* lld_ReadOp - Read memory array operation
*
* RETURNS: data read
*
*/
FLASHDATA lld_ReadOp
(
    FLASHDATA * base_addr,    /* device base address is system */
    ADDRESS offset        /* address offset from base address */
)
{
    FLASHDATA data;

    LLD_LOCK;
    data = FLASH_RD(base_addr, offset);
    LLD_UNLOCK;

    return(data);
}
#endif /* LLD_READ_OP */

#ifdef LLD_WRITE_BUFFER_OP
/******************************************************************************
*
* lld_WriteBufferProgramOp - Performs a Write Buffer Programming Operation.
*
* Function programs a write-buffer overlay of addresses to data
* passed via <data_buf>.
* Function issues all required commands and polls for completion.
*
* There are 4 main sections to the function:
*  Set-up and write command sequence
*  Determine number of locations to program and load buffer
*  Start operation with "Program Buffer to Flash" command
*  Poll for completion
*
* REQUIREMENTS:
*  Data in the buffer MUST be properly aligned with the Flash bus width.
*  No parameter checking is performed.
*  The <word_count> variable must be properly initialized.
*  Valid <byte_cnt> values:
*   min = 1 byte (only valid when used with a single x8 Flash)
*   max = write buffer size in bytes * number of devices in parallel
      (e.g. 32-byte buffer per device, 2 x16 devices = 64 bytes)
*
* RETURNS: DEVSTATUS
*/
#ifdef LLD_STATUS_REG

#else	/* toggle loop */
DEVSTATUS lld_WriteBufferProgramOp
(
    FLASHDATA *   base_addr,  /* device base address in system     */
    ADDRESS   offset,     /* address offset from base address  */
    WORDCOUNT word_count, /* number of words to program        */
    FLASHDATA *data_buf   /* buffer containing data to program */
)
{
    DEVSTATUS status;
    FLASHDATA write_data = 0;
    FLASHDATA read_data = 0;
    ADDRESS   last_loaded_addr;
    ADDRESS   current_offset;
    ADDRESS   end_offset;
    FLASHDATA wcount;

    /* Initialize variables */
    current_offset   = offset;
    end_offset       = offset + word_count - 1;
    last_loaded_addr = offset;

    /* don't try with a count of zero */
    if (!word_count) {
        return(DEV_NOT_BUSY);
    }

    LLD_LOCK;
    /* Issue Load Write Buffer Command Sequence */
    lld_WriteToBufferCmd(base_addr, offset);

    /* Write # of locations to program */
    wcount = (FLASHDATA)word_count - 1;
    wcount *= LLD_DEV_MULTIPLIER;

    FLASH_WR(base_addr, offset, wcount);

    /* Load Data into Buffer */
    while(current_offset <= end_offset) {
        /* Store last loaded address & data value (for polling) */
        last_loaded_addr = current_offset;
        write_data = *data_buf;

        /* Write Data */
        FLASH_WR(base_addr, current_offset++, *data_buf++);
    }

    /* Issue Program Buffer to Flash command */
    lld_ProgramBufferToFlashCmd(base_addr, last_loaded_addr);
    LLD_UNLOCK;

    status = lld_Poll(base_addr, last_loaded_addr, &write_data,
                      &read_data, LLD_P_POLL_WRT_BUF_PGM);
    return(status);
}
#endif /* LLD_STATUS_REG */
#endif /* LLD_WRITE_BUFFER_OP */

#ifdef LLD_PROGRAM_OP
/******************************************************************************
*
* lld_ProgramOp - Performs a standard Programming Operation.
*
* Function programs a single location to the specified data.
* Function issues all required commands and polls for completion.
*
*
* RETURNS: DEVSTATUS
*/
#ifdef LLD_STATUS_REG
#else
DEVSTATUS lld_ProgramOp
(
    FLASHDATA * base_addr,      /* device base address is system */
    ADDRESS offset,         /* address offset from base address */
    FLASHDATA write_data    /* variable containing data to program */
)
{
    FLASHDATA read_data = 0;
    DEVSTATUS status;

    LLD_LOCK;
    lld_ProgramCmd(base_addr, offset, &write_data);
    LLD_UNLOCK;
    status = lld_Poll(base_addr, offset, &write_data, &read_data, LLD_P_POLL_PGM);
    return(status);
}
#endif
#endif /* LLD_PROGRAM_OP */

#ifdef LLD_SECTOR_ERASE_OP
/******************************************************************************
*
* lld_SectorEraseOp - Performs a Sector Erase Operation
*
* Function erases the sector containing <base_addr> + <offset>.
* Function issues all required commands and polls for completion.
*
*
* RETURNS: DEVSTATUS
*
* ERRNO:
*/
#ifdef LLD_STATUS_REG
#else
#ifdef LLD_DEV_FLASH
DEVSTATUS lld_SectorEraseOp
(
    FLASHDATA * base_addr,    /* device base address is system */
    ADDRESS offset        /* address offset from base address */
)
{
    FLASHDATA         expect_data = (FLASHDATA)0xFFFFFFFF;
    FLASHDATA         actual_data = 0;
    DEVSTATUS         status;

    LLD_LOCK;
    lld_SectorEraseCmd(base_addr, offset);
    LLD_UNLOCK;

    status = lld_Poll(base_addr, offset, &expect_data, &actual_data, LLD_P_POLL_SEC_ERS);
    return(status);
}
#endif /* LLD_DEV_FLASH */
#endif /* LLD_STATUS_REG */

#endif /* LLD_SECTOR_ERASE_OP */

#ifdef LLD_CHIP_ERASE_OP
/******************************************************************************
*
* lld_ChipEraseOp - Performs a Chip Erase Operation
*
* Function erases entire device located at <base_addr>.
* Function issues all required commands and polls for completion.
*
*
* RETURNS: DEVSTATUS
*/
#ifdef LLD_STATUS_REG
#else
DEVSTATUS lld_ChipEraseOp
(
    FLASHDATA * base_addr   /* device base address in system */
)
{

    DEVSTATUS status;
    FLASHDATA expect_data = (FLASHDATA)0xFFFFFFFF;
    FLASHDATA actual_data = 0;

    LLD_LOCK;
    lld_ChipEraseCmd(base_addr);
    LLD_UNLOCK;
    status = lld_Poll(base_addr, 0, &expect_data, &actual_data, LLD_P_POLL_CHIP_ERS);

    /* if an error during polling, write RESET command to device */
    if(status != DEV_NOT_BUSY)
        /* Write Software RESET command */
        FLASH_WR(base_addr, 0, NOR_RESET_CMD);
    return(status);
}

#endif
#endif /* LLD_CHIP_ERASE_OP */

#ifdef LLD_MEMCPY_OP
/******************************************************************************
*
* lld_memcpy   This function attempts to mimic the standard memcpy
*              function for flash.  It segments the source data
*              into page size chunks for use by Write Buffer Programming.
*
* RETURNS: DEVSTATUS
*
*/
DEVSTATUS lld_memcpy
(
    FLASHDATA * base_addr,    /* device base address is system */
    ADDRESS offset,           /* address offset from base address */
    WORDCOUNT word_cnt,       /* number of words to program */
    FLASHDATA *data_buf       /* buffer containing data to program */
)
{
    ADDRESS mask = LLD_BUFFER_SIZE - 1;
    WORDCOUNT intwc = word_cnt;
    DEVSTATUS status = DEV_NOT_BUSY;

    if (offset & mask) {
        /* program only as much as necessary, so pick the lower of the two numbers */
        if (word_cnt < (LLD_BUFFER_SIZE - (offset & mask)) )
            intwc = word_cnt;
        else
            intwc = LLD_BUFFER_SIZE - (offset & mask);

        /* program the first few to get write buffer aligned */
        status = lld_WriteBufferProgramOp(base_addr, offset, intwc, data_buf);
        if (status != DEV_NOT_BUSY) {
            return(status);
        }

        offset   += intwc; /* adjust pointers and counter */
        word_cnt -= intwc;
        data_buf += intwc;
        if (word_cnt == 0) {
            return(status);
        }
    }

    while(word_cnt >= LLD_BUFFER_SIZE) { /* while big chunks to do */
        status = lld_WriteBufferProgramOp(base_addr, offset, LLD_BUFFER_SIZE, data_buf);
        if (status != DEV_NOT_BUSY) {
            return(status);
        }

        offset   += LLD_BUFFER_SIZE; /* adjust pointers and counter */
        word_cnt -= LLD_BUFFER_SIZE;
        data_buf += LLD_BUFFER_SIZE;
    }
    if (word_cnt == 0) {
        return(status);
    }

    status = lld_WriteBufferProgramOp(base_addr, offset, word_cnt, data_buf);
    return(status);
}
#endif /* LLD_MEMCPY_OP */

static unsigned char flash_read8(ADDRESS addr)
{
    FLASHDATA *base_addr = (FLASHDATA *)FLASH_BASE_ADDR;
    ADDRESS offset;
    FLASHDATA data;
    unsigned char cda;
    ulong wp ;

    wp = (addr & ~(LLD_PORT_WIDTH - 1));
    offset = ((FLASHDATA *)wp - base_addr);

    data = lld_ReadOp(base_addr,offset);

    if (offset & (LLD_PORT_WIDTH - 1)) {
        cda =(unsigned char ) data;
    } else {
        cda = (unsigned char )(data>>8) ;
    }
    return cda;
}

static void flash_add_byte (FLASHDATA *cword, uchar c)
{
    *cword = (*cword << 8) | c;
}

/*******************************************************************************
*
* lld_write - write data to flash
*
* RETURNS: actual wrote data length.or return LLD_FAIL if the operation is failed.
*
*/
int lld_write ( ADDRESS addr, uchar *src, unsigned int cnt)
{
    FLASHDATA *base_addr = (FLASHDATA *)FLASH_BASE_ADDR;
    DEVSTATUS status;
    FLASHDATA cword;
    WORDCOUNT wcnt;
    int align, i;
    uchar portwidth = LLD_PORT_WIDTH;
    unsigned int wp;
    unsigned int cp;
    unsigned int cnt_bak = cnt;
    ADDRESS offset = 0;
    FLASHDATA write_data = 0;
    unsigned char cdata = 255;

    /* handle unaligned start */
    wp = addr & ~(portwidth - 1);
    align = addr - wp;
    if (0 != align ) {
        cword = 0;
        cp = wp;
       
        for (i = 0; i < align; ++i, ++cp) {
            cdata = flash_read8(cp);
            flash_add_byte(&cword, cdata);
        }

        for (; ((i < portwidth) && (cnt > 0)); i++) {
            flash_add_byte(&cword, *src);
            src++;
            cnt--;
        }

#if 0
        /*for portwidth > 2*/
        for (; (cnt == 0) && (i < portwidth); ++i, ++cp) {
            cdata = flash_read8(cp);
            flash_add_byte(&cword, cdata);
        }
#endif

        offset = (FLASHDATA *)wp - base_addr;

        status = lld_ProgramOp(base_addr, offset, cword);
        if (DEV_NOT_BUSY != status) {
            nor_flash_errno = status;
            return (cnt-cnt_bak);
        }
        if (0 == cnt)
            return cnt_bak;
    }
    /*align the start addr*/
    addr = (addr + portwidth - 1) & ~(portwidth - 1); 
    offset = (FLASHDATA *)addr - base_addr;
    wcnt = cnt / portwidth;

    status = lld_memcpy(base_addr, offset, wcnt, (FLASHDATA *)src);
    if (DEV_NOT_BUSY != status) {
        nor_flash_errno = status;
        return (cnt_bak-cnt);
    }

    cnt %= portwidth;
    if (0 != cnt) {
        wp = addr + (wcnt * portwidth);
        offset += wcnt;
        src += (wcnt * portwidth);
        write_data = 0;
        for(i = 0; ((i < cnt) && (cnt > 0)); i++) {
            flash_add_byte(&write_data, *src);
            src++;
            cnt--;
        }
        for (; i < portwidth; i++) {
            cdata = flash_read8(wp + i);
            flash_add_byte(&write_data, cdata);
        }
        status = lld_ProgramOp(base_addr, offset, write_data);
        if (DEV_NOT_BUSY != status) {
            nor_flash_errno = status;
            return (cnt_bak-cnt);
        }
    }
    return cnt_bak-cnt;
}


/*******************************************************************************
*
* lld_read - read data from flash
*
* RETURNS: actual read data length.or return LLD_FAIL if the operation is failed.
*
*/
int lld_read ( ADDRESS addr , uchar* buf, unsigned int cnt )
{
    FLASHDATA * base_addr = (FLASHDATA *)FLASH_BASE_ADDR;
    FLASHDATA cword;
    ADDRESS offset;
    int aln, i,cnt_bak,cnt2;
    unsigned char portwidth = LLD_PORT_WIDTH;
    unsigned int wp;
    unsigned int wcnt = 0;

    cnt_bak = cnt;

    wp = (addr & ~(portwidth - 1));
    aln = addr - wp;

    if (0 != aln) {
        for(i = aln; ((i < portwidth) && (cnt > 0)); i++) {
            *buf = flash_read8(addr);
            addr++;
            buf++;
            cnt--;
        }
        if (cnt == 0)
            return cnt_bak;
    }
    wcnt = cnt / portwidth;
    offset = (FLASHDATA *)addr - base_addr;

    for (i = 0; i < wcnt; i++) {
        cword = lld_ReadOp(base_addr, offset);
        offset++;
        *((FLASHDATA *)buf) = cword;
        buf += portwidth;
        cnt-=portwidth;
    }

    addr += (wcnt * portwidth);
    cnt2 = cnt % portwidth;

    if (cnt2 != 0)
        for(i = 0; i<cnt2; i++) {
            *buf++ = flash_read8(addr+i);
            cnt--;
        }
    return cnt_bak-cnt;
}

/*******************************************************************************
*
* lld_erase - erase flash
*
* erase flash from "no" and erase "num" sectors
*
* RETURNS: actual read data length.or return LLD_FAIL if the operation is failed.
*
*/
int lld_erase(int no, int num)
{
    DEVSTATUS ret;
    int i;
    if (no <0 || no>SECTOR_NUM|| (no+num)>SECTOR_NUM)
        return -1;

    if(num < SECTOR_NUM) {
        for( i=0; i<num; i++ ) {
            ret = lld_SectorEraseOp((FLASHDATA *)FLASH_BASE_ADDR, no*LLD_SECTOR_SIZE);
            if (ret != DEV_NOT_BUSY)
                return -2;
        }
    } else if(num == SECTOR_NUM) {
        ret = lld_ChipEraseOp((FLASHDATA *)FLASH_BASE_ADDR);
        if (ret != DEV_NOT_BUSY)
            return -3;
    }
    return 0;
}
