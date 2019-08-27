/*
 * Copyright 2014 Freescale Semiconductor
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/ls102xa_stream_id.h>

void ls102xa_config_smmu_stream_id(struct smmu_stream_id *id, uint32_t num)
{
	uint32_t *scfg = (uint32_t *)CONFIG_SYS_FSL_SCFG_ADDR;
	int i;

	for (i = 0; i < num; i++)
		out_be32(scfg + id[i].offset, id[i].stream_id);
}

void ls1021x_config_smmu3(uint32_t liodn)
{
	uint32_t *addr;
	u32 smr, s2cr, nscr;

	addr = (uint32_t *)(CONFIG_SYS_SMMU3_ADDR + CONFIG_SMMU_NSCR_OFFSET);
       /* SMMU NSCR configuration */
	nscr = in_le32(addr);

	nscr = nscr  & ~(1 << SMMU_NSCR_CLIENTPD_SHIFT |
			 1 << SMMU_NSCR_MTCFG_SHIFT);
	out_le32(addr, nscr);

	/* SMMU SMR configuration */
	addr = (uint32_t *)(CONFIG_SYS_SMMU3_ADDR + CONFIG_SMMU_SMR_OFFSET);

	smr = 0;
	smr = smr & (~(SMR_ID_MASK << SMR_MASK_SHIFT));
	smr = smr | (1 << SMR_SMR_VALID_SHIFT) | liodn;

	out_le32(addr, smr);

	/* SMMU S2CR configuration */
	addr = (uint32_t *)(CONFIG_SYS_SMMU3_ADDR + CONFIG_SMMU_S2CR_OFFSET);
	s2cr = (S2CR_WACFG_WRITE_ALLOCATE << S2CR_WACFG_SHIFT) |
		(S2CR_RACFG_READ_ALLOCATE << S2CR_RACFG_SHIFT) |
		(S2CR_TYPE_BYPASS << S2CR_TYPE_SHIFT) |
		(S2CR_MEM_ATTR_CACHEABLE << S2CR_MEM_ATTR_SHIFT) |
		S2CR_MTCFG |
		(S2CR_SHCFG_OUTER_CACHEABLE << S2CR_SHCFG_SHIFT);

	out_le32(addr, s2cr);
}

void ls1021x_config_caam_stream_id(struct liodn_id_table *tbl, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		u32 liodn;
		if (tbl[i].num_ids == 2)
			liodn = (tbl[i].id[0] << 16) | tbl[i].id[1];
		else
			liodn = tbl[i].id[0];

		out_le32((uint32_t *)(tbl[i].reg_offset), liodn);
	}
}
