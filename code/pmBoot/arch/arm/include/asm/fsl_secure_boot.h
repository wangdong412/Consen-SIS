/*
 * Copyright 2010-2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __FSL_SECURE_BOOT_H
#define __FSL_SECURE_BOOT_H

#ifdef CONFIG_SECURE_BOOT

/* The address needs to be modified according to NOR memory map */
#define CONFIG_BOOTSCRIPT_HDR_ADDR	0x600a0000
#define CONFIG_KEY_REVOCATION
#define CONFIG_FSL_ISBC_KEY_EXT

#include <config_fsl_secboot.h>
#endif

#endif
