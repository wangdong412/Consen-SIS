/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ARM_NONSECURE_H_
#define _ARM_NONSECURE_H_

extern ulong non_sec_start;         /* start of image for smp spin table */
extern ulong non_sec_end;         /* end of image for smp spin table */
int fdt_add_non_sec_mem_rsv(void *blob);

#endif /* _ARM_NONSECURE_H_ */
