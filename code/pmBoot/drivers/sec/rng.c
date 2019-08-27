/*
 * RNG initialization
 *
 * Derived from linux caam driver ctrl.c
 *
 * Copyright 2013 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <jr.h>
#include <desc_constr.h>
#include <desc.h>
#include <fsl_sec.h>

u8 get_rng_vid(void)
{
	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	u32 cha_vid = sec_in32(&sec->chavid_ls);

	return (cha_vid & SEC_CHAVID_RNG_LS_MASK) >> SEC_CHAVID_LS_RNG_SHIFT;
}

/*
 * Descriptor to instantiate RNG State Handle 0 in normal mode and
 * load the JDKEK, TDKEK and TDSK registers
 */
static void build_instantiation_desc(u32 *desc)
{
	u32 *jump_cmd;

	init_job_desc(desc, 0);

	/* INIT RNG in non-test mode */
	append_operation(desc, OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG |
			 OP_ALG_AS_INIT);

	/* wait for done */
	jump_cmd = append_jump(desc, JUMP_CLASS_CLASS1);
	set_jump_tgt_here(desc, jump_cmd);

	/*
	 * load 1 to clear written reg:
	 * resets the done interrrupt and returns the RNG to idle.
	 */
	append_load_imm_u32(desc, 1, LDST_SRCDST_WORD_CLRW);

	/* generate secure keys (non-test) */
	append_operation(desc, OP_TYPE_CLASS1_ALG | OP_ALG_ALGSEL_RNG |
			 OP_ALG_RNG4_SK);
}

void rng4_init_done(uint32_t desc, uint32_t status, void *arg)
{
	struct result *instantiation = arg;
	instantiation->status = status;
	instantiation->err = caam_jr_strstatus(instantiation->outstr, status);
	instantiation->done = 1;
}

static int instantiate_rng(struct jobring *jr)
{
	struct result op;
	u32 *desc;
	u32 rdsta_val;
	int ret = 0;
	unsigned long long timeval;
	unsigned long long timeout;
	ccsr_sec_t __iomem *sec =
			(ccsr_sec_t __iomem *)CONFIG_SYS_FSL_SEC_ADDR;
	struct rng4tst __iomem *rng =
			(struct rng4tst __iomem *)&sec->rng;

	memset(&op, 0, sizeof(struct result));

	desc = malloc(CAAM_CMD_SZ * 6);
	if (!desc) {
		printf("cannot allocate RNG init descriptor memory\n");
		return -1;
	}

	build_instantiation_desc(desc);
	ret = jr_enqueue(jr, desc, rng4_init_done, &op);
	if (ret)
		return -1;

	timeval = get_ticks();
	timeout = usec2ticks(CONFIG_SEC_DEQ_TIMEOUT);

	while (op.done != 1) {
		if (jr_dequeue(jr)) {
			printf("RNG Instantiation :deq error\n");
			return -1;
		}

		if ((get_ticks() - timeval) > timeout) {
			printf("RNG Instantiation : SEC Dequeue timed out\n");
			return -1;
		}
	}

	if (op.status)
		printf("RNG: Instantiation failed with error %x\n", op.status);

	rdsta_val = sec_in32(&rng->rdsta);
	if (op.status || !(rdsta_val & RNG_STATE0_HANDLE_INSTANTIATED))
		return -1;

	return ret;
}

/*
 * By default, the TRNG runs for 200 clocks per sample;
 * 1200 clocks per sample generates better entropy.
 */
static void kick_trng(int ent_delay)
{
	ccsr_sec_t __iomem *sec =
			(ccsr_sec_t __iomem *)CONFIG_SYS_FSL_SEC_ADDR;
	struct rng4tst __iomem *rng =
			(struct rng4tst __iomem *)&sec->rng;
	u32 val;

	/* put RNG4 into program mode */
	setbits_be32(&rng->rtmctl, RTMCTL_PRGM);
	/* rtsdctl bits 0-15 contain "Entropy Delay, which defines the
	 * length (in system clocks) of each Entropy sample taken
	 * */
	val = sec_in32(&rng->rtsdctl);
	val = (val & ~RTSDCTL_ENT_DLY_MASK) |
	      (ent_delay << RTSDCTL_ENT_DLY_SHIFT);
	sec_out32(&rng->rtsdctl, val);
	/* min. freq. count, equal to 1/4 of the entropy sample length */
	sec_out32(&rng->rtfreqmin, ent_delay >> 2);
	/* max. freq. count, equal to 8 times the entropy sample length */
	sec_out32(&rng->rtfreqmax, ent_delay << 3);
	/* put RNG4 into run mode */
	clrbits_be32(&rng->rtmctl, RTMCTL_PRGM);
}

int rng_init(struct jobring *jr)
{
	int ret, ent_delay = RTSDCTL_ENT_DLY_MIN;
	ccsr_sec_t __iomem *sec =
			(ccsr_sec_t __iomem *)CONFIG_SYS_FSL_SEC_ADDR;
	struct rng4tst __iomem *rng =
			(struct rng4tst __iomem *)&sec->rng;

	u32 rdsta = sec_in32(&rng->rdsta);

	/* Check if RNG state 0 handler is already instantiated */
	if (rdsta & RNG_STATE0_HANDLE_INSTANTIATED)
		return 0;

	do {
		/*
		 * If either of the SH's were instantiated by somebody else
		 * then it is assumed that the entropy
		 * parameters are properly set and thus the function
		 * setting these (kick_trng(...)) is skipped.
		 * Also, if a handle was instantiated, do not change
		 * the TRNG parameters.
		 */
		kick_trng(ent_delay);
		ent_delay += 400;
		/*
		 * if instantiate_rng(...) fails, the loop will rerun
		 * and the kick_trng(...) function will modfiy the
		 * upper and lower limits of the entropy sampling
		 * interval, leading to a sucessful initialization of
		 * the RNG.
		 */
		ret = instantiate_rng(jr);
	} while ((ret == -1) && (ent_delay < RTSDCTL_ENT_DLY_MAX));
	if (ret) {
		printf("RNG: Failed to instantiate RNG\n");
		return ret;
	}

	 /* Enable RDB bit so that RNG works faster */
	setbits_be32(&sec->scfgr, SEC_SCFGR_RDBENABLE);

	return ret;
}
