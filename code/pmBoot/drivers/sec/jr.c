/*
 * Copyright 2011, 2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <fsl_sec.h>
#include <jr.h>
#ifdef CONFIG_FSL_CORENET
#include <asm/fsl_pamu.h>
#endif
#include <fsl_secboot_err.h>

#define CIRC_CNT(head, tail, size)	(((head) - (tail)) & (size - 1))
#define CIRC_SPACE(head, tail, size)	CIRC_CNT((tail), (head) + 1, (size))

static inline void start_jr0(void)
{
	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	u32 ctpr_ms = sec_in32(&sec->ctpr_ms);
	u32 scfgr = sec_in32(&sec->scfgr);

	if (ctpr_ms & SEC_CTPR_MS_VIRT_EN_INCL) {
		/* VIRT_EN_INCL = 1 & VIRT_EN_POR = 1 or
		 * VIRT_EN_INCL = 1 & VIRT_EN_POR = 0 & SEC_SCFGR_VIRT_EN = 1
		 */
		if ((ctpr_ms & SEC_CTPR_MS_VIRT_EN_POR) ||
		    (!(ctpr_ms & SEC_CTPR_MS_VIRT_EN_POR) &&
					(scfgr & SEC_SCFGR_VIRT_EN)))
			sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR0);
	} else {
		/* VIRT_EN_INCL = 0 && VIRT_EN_POR_VALUE = 1 */
		if (ctpr_ms & SEC_CTPR_MS_VIRT_EN_POR)
			sec_out32(&sec->jrstartr, CONFIG_JRSTARTR_JR0);
	}
}

static inline void jr_reset_liodn(void)
{
	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	sec_out32(&sec->jrliodnr[0].ls, 0);
}

static inline void jr_disable_irq(void)
{
	struct jr_regs *regs = (struct jr_regs *)CONFIG_SYS_FSL_JR0_ADDR;
	uint32_t jrcfg = sec_in32(&regs->jrcfg1);

	jrcfg = jrcfg | JR_INTMASK;

	sec_out32(&regs->jrcfg1, jrcfg);
}

static void jr_initregs(struct jobring *jr)
{
	struct jr_regs *regs = (struct jr_regs *)CONFIG_SYS_FSL_JR0_ADDR;
	phys_addr_t ip_base = virt_to_phys((void *)jr->input_ring);
	phys_addr_t op_base = virt_to_phys((void *)jr->output_ring);

#ifdef CONFIG_PHYS_64BIT
	sec_out32(&regs->irba_h, ip_base >> 32);
#else
	sec_out32(&regs->irba_h, 0x0);
#endif
	sec_out32(&regs->irba_l, (uint32_t)ip_base);
#ifdef CONFIG_PHYS_64BIT
	sec_out32(&regs->orba_h, op_base >> 32);
#else
	sec_out32(&regs->orba_h, 0x0);
#endif
	sec_out32(&regs->orba_l, (uint32_t)op_base);
	sec_out32(&regs->ors, JR_SIZE);
	sec_out32(&regs->irs, JR_SIZE);

	if (!jr->irq)
		jr_disable_irq();
}

int jr_init(struct jobring *jr)
{
	memset(jr, 0, sizeof(struct jobring));

	jr->jq_id = DEFAULT_JR_ID;
	jr->irq = DEFAULT_IRQ;

#ifdef CONFIG_FSL_CORENET
	jr->liodn = DEFAULT_JR_LIODN;
#endif
	jr->size = JR_SIZE;
	jr->input_ring = (dma_addr_t *)malloc(JR_SIZE * sizeof(dma_addr_t));
	if (!jr->input_ring)
		return -1;
	jr->output_ring =
	    (struct op_ring *)malloc(JR_SIZE * sizeof(struct op_ring));
	if (!jr->output_ring)
		return -1;

	memset(jr->input_ring, 0, JR_SIZE * sizeof(dma_addr_t));
	memset(jr->output_ring, 0, JR_SIZE * sizeof(struct op_ring));

	start_jr0();

	jr_initregs(jr);

	return 0;
}

int jr_sw_cleanup(struct jobring *jr)
{
	jr->head = 0;
	jr->tail = 0;
	jr->read_idx = 0;
	jr->write_idx = 0;
	memset(jr->info, 0, sizeof(jr->info));
	memset(jr->input_ring, 0, jr->size * sizeof(dma_addr_t));
	memset(jr->output_ring, 0, jr->size * sizeof(struct op_ring));

	return 0;
}

int sec_init(struct jobring *jr)
{
	int ret = 0;

#ifdef CONFIG_FSL_CORENET
	uint32_t liodnr;
	uint32_t liodn_ns;
	uint32_t liodn_s;
#endif

#ifdef CONFIG_PHYS_64BIT
	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	uint32_t mcr = sec_in32(&sec->mcfgr);

	sec_out32(&sec->mcfgr, mcr | 1 << MCFGR_PS_SHIFT);
#endif

#ifdef CONFIG_FSL_CORENET
	liodnr = sec_in32(&sec->jrliodnr[0].ls);
	liodn_ns = (liodnr & JRNSLIODN_MASK) >> JRNSLIODN_SHIFT;
	liodn_s = (liodnr & JRSLIODN_MASK) >> JRSLIODN_SHIFT;
#endif
	ret = jr_init(jr);
	if (ret < 0)
		return -1;

#ifdef CONFIG_FSL_CORENET
	ret = sec_config_pamu_table(liodn_ns, liodn_s);
	if (ret < 0)
		return -1;

#ifdef CONFIG_FSL_CORENET
	pamu_enable();
#endif

#endif

	return ret;
}

int jr_hw_reset(void)
{
	struct jr_regs *regs = (struct jr_regs *)CONFIG_SYS_FSL_JR0_ADDR;
	uint32_t timeout = 100000;
	uint32_t jrint, jrcr;

	sec_out32(&regs->jrcr, JRCR_RESET);
	do {
		jrint = sec_in32(&regs->jrint);
	} while (((jrint & JRINT_ERR_HALT_MASK) ==
		  JRINT_ERR_HALT_INPROGRESS) && --timeout);

	jrint = sec_in32(&regs->jrint);
	if (((jrint & JRINT_ERR_HALT_MASK) !=
	     JRINT_ERR_HALT_INPROGRESS) && timeout == 0)
		return -1;

	timeout = 100000;
	sec_out32(&regs->jrcr, JRCR_RESET);
	do {
		jrcr = sec_in32(&regs->jrcr);
	} while ((jrcr & JRCR_RESET) && --timeout);

	if (timeout == 0)
		return -1;

	return 0;
}

int jr_reset(struct jobring *jr)
{
	if (jr_hw_reset() < 0)
		return -1;

	/* Clean up the jobring structure maintained by software */
	jr_sw_cleanup(jr);

	return 0;
}

int sec_reset(void)
{
	ccsr_sec_t *sec = (void *)CONFIG_SYS_FSL_SEC_ADDR;
	uint32_t mcfgr = sec_in32(&sec->mcfgr);
	uint32_t timeout = 100000;

	mcfgr |= MCFGR_SWRST;
	sec_out32(&sec->mcfgr, mcfgr);

	mcfgr |= MCFGR_DMA_RST;
	sec_out32(&sec->mcfgr, mcfgr);
	do {
		mcfgr = sec_in32(&sec->mcfgr);
	} while ((mcfgr & MCFGR_DMA_RST) == MCFGR_DMA_RST && --timeout);

	if (timeout == 0)
		return -1;

	timeout = 100000;
	do {
		mcfgr = sec_in32(&sec->mcfgr);
	} while ((mcfgr & MCFGR_SWRST) == MCFGR_SWRST && --timeout);

	if (timeout == 0)
		return -1;

	return 0;
}

/* -1 --- error, can't enqueue -- no space available */
int jr_enqueue(struct jobring *jr, uint32_t *desc_addr,
	       void (*callback)(uint32_t desc, uint32_t status, void *arg),
	       void *arg)
{
	struct jr_regs *regs = (struct jr_regs *)CONFIG_SYS_FSL_JR0_ADDR;
	int head = jr->head;
	dma_addr_t desc_phys_addr = virt_to_phys(desc_addr);

	if (sec_in32(&regs->irsa) == 0 ||
	    CIRC_SPACE(jr->head, jr->tail, jr->size) <= 0)
		return ERROR_ESBC_SEC_ENQ;

	jr->input_ring[head] = desc_phys_addr;
	jr->info[head].desc_phys_addr = desc_phys_addr;
	jr->info[head].desc_addr = (uint32_t)desc_addr;
	jr->info[head].callback = (void *)callback;
	jr->info[head].arg = arg;
	jr->info[head].op_done = 0;

	jr->head = (head + 1) & (jr->size - 1);

	sec_out32(&regs->irja, 1);

	return 0;
}

int jr_dequeue(struct jobring *jr)
{
	struct jr_regs *regs = (struct jr_regs *)CONFIG_SYS_FSL_JR0_ADDR;
	int head = jr->head;
	int tail = jr->tail;
	int idx, i, found;
	void (*callback)(uint32_t desc, uint32_t status, void *arg);
	void *arg = NULL;

	while (sec_in32(&regs->orsf) &&
	       CIRC_CNT(jr->head, jr->tail, jr->size)) {
		found = 0;

		dma_addr_t op_desc = jr->output_ring[jr->tail].desc;
		uint32_t status = jr->output_ring[jr->tail].status;
		uint32_t desc_virt;

		for (i = 0; CIRC_CNT(head, tail + i, jr->size) >= 1; i++) {
			idx = (tail + i) & (jr->size - 1);
			if (op_desc == jr->info[idx].desc_phys_addr) {
				desc_virt = jr->info[idx].desc_addr;
				found = 1;
				break;
			}
		}

		/* Error condition if match not found */
		if (!found)
			return ERROR_ESBC_SEC_DEQ;

		jr->info[idx].op_done = 1;
		callback = (void *)jr->info[idx].callback;
		arg = jr->info[idx].arg;

		/* When the job on tail idx gets done, increment
		 * tail till the point where job completed out of oredr has
		 * been taken into account
		 */
		if (idx == tail)
			do {
				tail = (tail + 1) & (jr->size - 1);
			} while (jr->info[tail].op_done);

		jr->tail = tail;
		jr->read_idx = (jr->read_idx + 1) & (jr->size - 1);

		sec_out32(&regs->orjr, 1);
		jr->info[idx].op_done = 0;

		callback(desc_virt, status, arg);
	}

	return 0;
}
