/* sysLaw.c - P2020DS Law driver source file */

/*
modification history
--------------------
01a,31may14,Jeff.Lau  written
*/


/* includes */


#include <libc/cagboot.h>
#include <config.h>
#include <p1020rdb.h>
#include <libc/stdio.h>
#include <libc/access.h>
#include <libc/system.h>
#include <libc/string.h>
#include <ppc/ppc85xx.h>
#include <ppc/mmuE500Lib.h>
#include <ppc/toolPpc.h>


#define QORIQ_TGTID_DPAA        0x0E
#define QORIQ_TGTID_PCIE1       0x02    /* PCI Express1 */
#define QORIQ_TGTID_PCIE2       0x01    /* PCI Express2 */
#define QORIQ_TGTID_PCIE3       0x03    /* PCI Express3 */
#define QORIQ_TGTID_ELBC        0x04
#define QORIQ_TGTID_DDR         0x0F
#define QORIQ_LAWAR_ENABLE      0x80000000 /* Window enabled */
#define QORIQ_LAWAR_TGTID       0x01F00000 /* Target ID */

#define  M85XX_LAW_CNT          12
#define  M85XX_LAW_SIZE         0x20
#define  LAWBAR_ADRS_SHIFT      12

/* LBC CS0 - flash 0 - default for bootrom */

#define FLASH_BASE_PART0_P1023       0xFC000000
#define FLASH_BASE_PART0_DEFAULT     0xFF000000
#define FLASH_BASE_PART1             0xFF800000
#define FLASH_SIZE_P1023             0x04000000
#define FLASH_SIZE_DEFAULT           0x01000000
#define FLASH_SIZE_PART0_P1023       0x03000000
#define FLASH_SIZE_PART0_DEFAULT     0x00600000
#define FLASH_SIZE_PART1             0x00800000
#define ROM_SIZE                     0x00100000

/* macro to compose 64-bit PHYS_ADDRs */

/*
 * The DPAA's window must be configured as an 8 Mbyte window at address
 * 0xFF00_0000. It is required that this LAW is overlapped by the default
 * CCSR space in the most significant 2 Mbytes. The NOR flash is addressed
 * at 0xFE00_0000. The DPAA and CCSR's windows overlap the window of the
 * NOR flash. If two or more LAWs overlap, the lower numbered window takes
 * precedence. Therefore, we set access LAWs for DPAA and CCSR before NOR
 * flash.
 */

UINT32 lawRegisterTable [][2] = {
#if 0
    {
        _CAG_CONFIG_FLASH_BASE_ADRS>> 12,
        (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_64MB | QORIQ_LAWAR_ENABLE
    },

    {
        _CAG_CONFIG_SRAM_ADDR>> 12,
        (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_1MB | QORIQ_LAWAR_ENABLE
    },

    {
        _CAG_CONFIG_FPGA_ADDR >> 12,
        (QORIQ_TGTID_ELBC << 20) | LAWAR_SIZE_64KB | QORIQ_LAWAR_ENABLE
    },
#endif
    {0x000ec000,0x80400019},/*nor*/
    {0x000ff600,0x80400013},/*sram*/
    {0x000ff700,0x8040000f},/*fpga*/
    {0x00080000,0x8020001a},/*pcie1 outbound*/
    {0x000ffc00,0x8020000f},/*?*/
    {0x000a0000,0x8010001a},/*pcie2 outbound*/
};

int lawNum = NELEMENTS(lawRegisterTable);

/*******************************************************************************
*
* coreLawBarSet - set LAWs for devices
*
* This routine sets local access window registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void coreLawBarSet (void)
{
    int i;
    int lawIndex;
    UINT32 tgtID[M85XX_LAW_CNT][2];
    VUINT32 * lawBARn;
    VUINT32 * lawARn;

    lawIndex = 0;
    bzero(tgtID, sizeof(tgtID));

    for (i = 0; i < M85XX_LAW_CNT; i++ ) {
        lawARn = (VUINT32 *)((UINT32)M85XX_LAWAR0(_CAG_CONFIG_CCSBAR) + i * M85XX_LAW_SIZE);
        lawBARn = (UINT32 *)((UINT32)M85XX_LAWBAR0(_CAG_CONFIG_CCSBAR) + i * M85XX_LAW_SIZE);
        if (*lawARn & QORIQ_LAWAR_ENABLE)
            tgtID[i][0] = *lawBARn;
        tgtID[i][1] = *lawARn & QORIQ_LAWAR_TGTID;
    }

entry:
    for (i = 0; i < M85XX_LAW_CNT; i++ ) {

        if ( tgtID[i][0] == lawRegisterTable[lawIndex][0] &&
             (tgtID[i][1] == (lawRegisterTable[lawIndex][1] & QORIQ_LAWAR_TGTID))) {
            lawIndex++;
        }
        if (lawIndex == lawNum)
            return;
    }

    for (i = 0; i < M85XX_LAW_CNT; i++ ) {
        lawARn = (VUINT32 *)((UINT32)M85XX_LAWAR0(_CAG_CONFIG_CCSBAR) + i * M85XX_LAW_SIZE);
        lawBARn = (VUINT32 *)((UINT32)M85XX_LAWBAR0(_CAG_CONFIG_CCSBAR) + i * M85XX_LAW_SIZE);

        if (*lawARn & QORIQ_LAWAR_ENABLE)
            continue;

        *lawBARn = lawRegisterTable[lawIndex][0];
        *lawARn = lawRegisterTable[lawIndex][1];
        lawIndex++;
        if (lawIndex < lawNum)
            goto entry;
        else
            return;
    }

}


/*******************************************************************************
*
* coreLbcSet - set LBC for devices
*
* This routine sets local bus controller registers.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void coreLbcSet (void)
{
    /* Nor Flash */

    * (VINT32 *) M85XX_BR0(_CAG_CONFIG_CCSBAR) = 0xEC001001;
    * (VINT32 *) M85XX_OR0(_CAG_CONFIG_CCSBAR) = 0xFC000FF7;

    /* sram*/

    * (VINT32 *) M85XX_BR1(_CAG_CONFIG_CCSBAR) = 0xFF601001;
    * (VINT32 *) M85XX_OR1(_CAG_CONFIG_CCSBAR) = 0xFFF00C40;

    /* fpga */

    * (VINT32 *) M85XX_BR2(_CAG_CONFIG_CCSBAR) = 0xFF701001;
    * (VINT32 *) M85XX_OR2(_CAG_CONFIG_CCSBAR) = 0xFFF00FF6;

    return;
}

/*******************************************************************************
*
* sysHwInit - initialize the system hardware
*
* This routine initializes various feature of the P10XXRDB board. It sets up
* the control registers, initializes various devices if they are present.
*
* NOTE: This routine should not be called directly by the user.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void sysHwInit (void)
{
    coreLawBarSet();
    coreLbcSet();
}
