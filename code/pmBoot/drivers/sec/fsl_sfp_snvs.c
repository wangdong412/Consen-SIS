/*
 * Copyright 2010-2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <fsl_sec.h>

int change_sec_mon_state(u32 initial_state, u32 final_state)
{
	struct ccsr_snvs_regs *snvs_regs = (void *)(CONFIG_SYS_SNVS_ADDR);
	u32 sts = snvs_in32(&snvs_regs->hp_stat);
	int timeout = 10;

	if ((sts & HPSR_SSM_ST_MASK) != initial_state)
		return -1;

	if (initial_state == HPSR_SSM_ST_TRUST) {
		switch (final_state) {
		case HPSR_SSM_ST_NON_SECURE:
			printf("SNVS state transitioning to Soft Fail.\n");
			snvs_setbits32(&snvs_regs->hp_com, HPCOMR_SW_SV);

			/*
			 * poll till SNVS is in
			 * Soft Fail state
			 */
			while (((sts & HPSR_SSM_ST_MASK) !=
				HPSR_SSM_ST_SOFT_FAIL)) {
				while (timeout) {
					sts = snvs_in32(&snvs_regs->hp_stat);

					if ((sts & HPSR_SSM_ST_MASK) ==
						HPSR_SSM_ST_SOFT_FAIL)
						break;

					udelay(10);
					timeout--;
				}
			}

			if (timeout == 0) {
				printf("SNVS state transition timeout.\n");
				return -1;
			}

			timeout = 10;

			printf("SNVS state transitioning to Non Secure.\n");
			snvs_setbits32(&snvs_regs->hp_com, HPCOMR_SSM_ST);

			/*
			 * poll till SNVS is in
			 * Non Secure state
			 */
			while (((sts & HPSR_SSM_ST_MASK) !=
				HPSR_SSM_ST_NON_SECURE)) {
				while (timeout) {
					sts = snvs_in32(&snvs_regs->hp_stat);

					if ((sts & HPSR_SSM_ST_MASK) ==
						HPSR_SSM_ST_NON_SECURE)
						break;

					udelay(10);
					timeout--;
				}
			}

			if (timeout == 0) {
				printf("SNVS state transition timeout.\n");
				return -1;
			}
			break;
		case HPSR_SSM_ST_SOFT_FAIL:
			printf("SNVS state transitioning to Soft Fail.\n");
			snvs_setbits32(&snvs_regs->hp_com, HPCOMR_SW_FSV);

			/*
			 * polling loop till SNVS is in
			 * Soft Fail state
			 */
			while (((sts & HPSR_SSM_ST_MASK) !=
				HPSR_SSM_ST_SOFT_FAIL)) {
				while (timeout) {
					sts = snvs_in32(&snvs_regs->hp_stat);

					if ((sts & HPSR_SSM_ST_MASK) ==
						HPSR_SSM_ST_SOFT_FAIL)
						break;

					udelay(10);
					timeout--;
				}
			}

			if (timeout == 0) {
				printf("SNVS state transition timeout.\n");
				return -1;
			}
			break;
		default:
			return -1;
		}
	} else if (initial_state == HPSR_SSM_ST_NON_SECURE) {
		switch (final_state) {
		case HPSR_SSM_ST_SOFT_FAIL:
			printf("SNVS state transitioning to Soft Fail.\n");
			snvs_setbits32(&snvs_regs->hp_com, HPCOMR_SW_FSV);

			/*
			 * polling loop till SNVS is in
			 * Soft Fail state
			 */
			while (((sts & HPSR_SSM_ST_MASK) !=
				HPSR_SSM_ST_SOFT_FAIL)) {
				while (timeout) {
					sts = snvs_in32(&snvs_regs->hp_stat);

					if ((sts & HPSR_SSM_ST_MASK) ==
						HPSR_SSM_ST_SOFT_FAIL)
						break;

					udelay(10);
					timeout--;
				}
			}

			if (timeout == 0) {
				printf("SNVS state transition timeout.\n");
				return -1;
			}
			break;
		default:
			return -1;
		}
	}

	return 0;
}
