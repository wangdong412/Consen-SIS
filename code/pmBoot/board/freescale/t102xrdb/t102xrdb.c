/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <i2c.h>
#include <netdev.h>
#include <linux/compiler.h>
#include <asm/mmu.h>
#include <asm/processor.h>
#include <asm/immap_85xx.h>
#include <asm/fsl_law.h>
#include <asm/fsl_serdes.h>
#include <asm/fsl_portals.h>
#include <asm/fsl_liodn.h>
#include <asm/mpc85xx_gpio.h>
#include <fm_eth.h>
#include "t102xrdb.h"
#include "cpld.h"

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	struct cpu_type *cpu = gd->arch.cpu;
	static const char *freq[3] = {"100.00MHZ", "125.00MHz", "156.25MHZ"};

	printf("Board: %sRDB, ", cpu->name);
	printf("Board rev: 0x%02x CPLD ver: 0x%02x, boot from ",
	       CPLD_READ(hw_ver), CPLD_READ(sw_ver));

#ifdef CONFIG_SDCARD
	puts("SD/MMC\n");
#elif CONFIG_SPIFLASH
	puts("SPI\n");
#else
	u8 reg;

	reg = CPLD_READ(flash_csr);

	if (reg & CPLD_BOOT_SEL) {
		puts("NAND\n");
	} else {
		reg = ((reg & CPLD_LBMAP_MASK) >> CPLD_LBMAP_SHIFT);
		printf("NOR vBank%d\n", reg);
	}
#endif

	puts("SERDES Reference Clocks:\n");
	printf("SD1_CLK1=%s, SD1_CLK2=%s\n", freq[2], freq[0]);

	return 0;
}

int board_early_init_r(void)
{
	u32 reg;

#ifdef CONFIG_SYS_FLASH_BASE
	const unsigned int flashbase = CONFIG_SYS_FLASH_BASE;
	int flash_esel = find_tlb_idx((void *)flashbase, 1);
	/*
	 * Remap Boot flash region to caching-inhibited
	 * so that flash can be erased properly.
	 */

	/* Flush d-cache and invalidate i-cache of any FLASH data */
	flush_dcache();
	invalidate_icache();
	if (flash_esel == -1) {
		/* very unlikely unless something is messed up */
		puts("Error: Could not find TLB for FLASH BASE\n");
		flash_esel = 2;	/* give our best effort to continue */
	} else {
		/* invalidate existing TLB entry for flash + promjet */
		disable_tlb(flash_esel);
	}

	set_tlb(1, flashbase, CONFIG_SYS_FLASH_BASE_PHYS,
		MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		0, flash_esel, BOOKE_PAGESZ_256M, 1);
#endif

	set_liodns();
#ifdef CONFIG_SYS_DPAA_QBMAN
	setup_portals();
#endif

	/* Increase IO drive strength to avoid FCS error on RGMII */
#define IODSECR1_LVDD_VAL  (0x7 << 26)
#define IODSECR1_L1VDD_VAL (0x7 << 23)
	reg = in_be32((unsigned *)CONFIG_SYS_FSL_SCFG_IODSECR1_ADDR);
	reg |= IODSECR1_LVDD_VAL | IODSECR1_L1VDD_VAL;
	out_be32((unsigned *)CONFIG_SYS_FSL_SCFG_IODSECR1_ADDR, reg);

	return 0;
}

unsigned long get_board_sys_clk(void)
{
	return CONFIG_SYS_CLK_FREQ;
}

unsigned long get_board_ddr_clk(void)
{
	return CONFIG_DDR_CLK_FREQ;
}

int misc_init_r(void)
{
	return 0;
}

void ft_board_setup(void *blob, bd_t *bd)
{
	phys_addr_t base;
	phys_size_t size;

	ft_cpu_setup(blob, bd);

	base = getenv_bootm_low();
	size = getenv_bootm_size();

	fdt_fixup_memory(blob, (u64)base, (u64)size);

#ifdef CONFIG_PCI
	pci_of_setup(blob, bd);
#endif

	fdt_fixup_liodn(blob);
	fdt_fixup_dr_usb(blob, bd);

#ifdef CONFIG_SYS_DPAA_FMAN
	fdt_fixup_fman_ethernet(blob);
	fdt_fixup_board_enet(blob);
#endif
}

#ifdef CONFIG_DEEP_SLEEP
void board_mem_sleep_setup(void)
{
	/* does not provide HW signals for power management */
	CPLD_WRITE(misc_ctl_status, (CPLD_READ(misc_ctl_status) & ~0x40));
	/* Disable MCKE isolation */
	gpio_set_value(2, 0);
	udelay(1);
}
#endif

#ifdef CONFIG_FSL_DEEP_SLEEP
/* determine if it is a warm boot */
bool is_warm_boot(void)
{
#define DCFG_CCSR_CRSTSR_WDRFR	(1 << 3)
	struct ccsr_gur __iomem *gur = (void *)CONFIG_SYS_MPC85xx_GUTS_ADDR;

	if (in_be32(&gur->scrtsr[0]) & DCFG_CCSR_CRSTSR_WDRFR)
		return 1;

	return 0;
}

void fsl_dp_mem_setup(void)
{
	/* does not provide HW signals for power management */
	CPLD_WRITE(misc_ctl_status, (CPLD_READ(misc_ctl_status) & ~0x40));
	/* Disable MCKE isolation */
	gpio_set_value(2, 0);
	udelay(1);
}

void fsl_dp_ddr_restore(void)
{
#define DDR_BUFF_LEN	128
	volatile u64 *src, *dst;
	int i;
	struct ccsr_scfg __iomem *scfg = (void *)CONFIG_SYS_MPC85xx_SCFG;

	if (!is_warm_boot())
		return;

	/* get the address of ddr date from SPARECR3 */
	src = (u64 *)in_be32(&scfg->sparecr[2]);
	dst = (u64 *)CONFIG_SYS_SDRAM_BASE;
	for (i = 0; i < DDR_BUFF_LEN / 8; i++)
		*dst++ = *src++;

	flush_dcache();
}

int fsl_dp_resume(void)
{
	u32 start_addr;
	void (*kernel_resume)(void);
	struct ccsr_scfg __iomem *scfg = (void *)CONFIG_SYS_MPC85xx_SCFG;

	if (!is_warm_boot())
		return 0;

	fsl_dp_ddr_restore();

	l2cache_init();
#if defined(CONFIG_RAMBOOT_PBL)
	disable_cpc_sram();
#endif
	enable_cpc();

	/* Get the entry address and jump to kernel */
	start_addr = in_be32(&scfg->sparecr[1]);
	debug("Entry address is 0x%08x\n", start_addr);
	kernel_resume = (void (*)(void))start_addr;
	kernel_resume();

	return 0;
}
#endif
