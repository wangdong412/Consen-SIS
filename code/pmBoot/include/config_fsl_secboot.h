/*
 * Copyright 2010-2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_FSL_SECBOOT_H
#define __CONFIG_FSL_SECBOOT_H

#ifdef CONFIG_SECURE_BOOT

#define CONFIG_CMD_ESBC_VALIDATE

/*
 * Control should not reach back to uboot after validation of images
 * for secure boot flow and therefore bootscript should have
 * the bootm command. If control reaches back to uboot anyhow
 * after validating images, core should just spin.
 */
#ifdef CONFIG_BOOTSCRIPT_KEY_HASH
#define CONFIG_SECBOOT \
	"setenv bs_hdraddr " __stringify(CONFIG_BOOTSCRIPT_HDR_ADDR)";" \
	"setenv bootargs \'root=/dev/ram rw console=ttyS0,115200 "	\
	"ramdisk_size=600000\';"	\
	"esbc_validate $bs_hdraddr " \
	  __stringify(CONFIG_BOOTSCRIPT_KEY_HASH)";" \
	"source $img_addr;"					\
	"esbc_halt\0"
#else
#define CONFIG_SECBOOT \
	"setenv bs_hdraddr " __stringify(CONFIG_BOOTSCRIPT_HDR_ADDR)";"	 \
	"setenv bootargs \'root=/dev/ram rw console=ttyS0,115200 "	\
	"ramdisk_size=600000\';"	\
	"esbc_validate $bs_hdraddr;"		\
	"source $img_addr;"				\
	"esbc_halt\0"
#endif

/* For secure boot flow, default environment used will be used */
#if defined(CONFIG_SYS_RAMBOOT)
#if defined(CONFIG_RAMBOOT_SPIFLASH)
#undef CONFIG_ENV_IS_IN_SPI_FLASH
#elif defined(CONFIG_RAMBOOT_NAND)
#undef CONFIG_ENV_IS_IN_NAND
#elif defined(CONFIG_RAMBOOT_SDCARD)
#undef CONFIG_ENV_IS_IN_MMC
#endif
#else /*CONFIG_SYS_RAMBOOT*/
#undef CONFIG_ENV_IS_IN_FLASH
#endif

#define CONFIG_ENV_IS_NOWHERE

/*
 * We don't want boot delay for secure boot flow
 * before autoboot starts
 */
#undef CONFIG_BOOTDELAY
#define CONFIG_BOOTDELAY	0
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND		CONFIG_SECBOOT

/*
 * CONFIG_ZERO_BOOTDELAY_CHECK should not be defined for
 * secure boot flow as defining this would enable a user to
 * reach uboot prompt by pressing some key before start of
 * autoboot
 */
#undef CONFIG_ZERO_BOOTDELAY_CHECK

#endif
#endif
