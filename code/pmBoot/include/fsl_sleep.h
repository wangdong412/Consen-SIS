/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __DEEP_SLEEP_H
#define __DEEP_SLEEP_H

/*
 * Functions prototype for sleep.
 * Each platform that supports deep sleep needs to
 * implement them on their own.
 */

/* determine if it is a wakeup from deep sleep */
bool is_warm_boot(void);

/* board specific memory setup, like CKE isolation */
void fsl_dp_mem_setup(void);

/* clean up everything and jump to kernel */
int fsl_dp_resume(void);

/*
 * When wakeup from deep sleep, the first 128 bytes space
 * will be used to do DDR training which corrupts the data
 * in there. This function will restore them.
 */
void fsl_dp_ddr_restore(void);

#endif
