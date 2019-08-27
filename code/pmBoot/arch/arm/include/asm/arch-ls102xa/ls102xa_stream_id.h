/*
 * Copyright 2014 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __FSL_LS102XA_STREAM_ID_H_
#define __FSL_LS102XA_STREAM_ID_H_

#include <fsl_sec.h>

#define CONFIG_SMMU_NSCR_OFFSET		0x400
#define CONFIG_SMMU_SMR_OFFSET		0x800
#define CONFIG_SMMU_S2CR_OFFSET		0xc00

#define SMMU_NSCR_CLIENTPD_SHIFT	0
#define SMMU_NSCR_MTCFG_SHIFT		20

#define SMR_SMR_VALID_SHIFT		31
#define SMR_ID_MASK			0x7fff
#define SMR_MASK_SHIFT			16

#define S2CR_WACFG_SHIFT		22
#define S2CR_WACFG_MASK			0x3
#define S2CR_WACFG_WRITE_ALLOCATE	0x2

#define S2CR_RACFG_SHIFT		20
#define S2CR_RACFG_MASK			0x3
#define S2CR_RACFG_READ_ALLOCATE	0x2

#define S2CR_TYPE_SHIFT			16
#define S2CR_TYPE_MASK			0x3
#define S2CR_TYPE_BYPASS		0x01

#define S2CR_MEM_ATTR_SHIFT		12
#define S2CR_MEM_ATTR_MASK		0xf
#define S2CR_MEM_ATTR_CACHEABLE		0xa

#define S2CR_MTCFG			0x00000800

#define S2CR_SHCFG_SHIFT		8
#define S2CR_SHCFG_MASK			0x3
#define S2CR_SHCFG_OUTER_CACHEABLE	0x1
#define S2CR_SHCFG_INNER_CACHEABLE	0x2

#define SET_LIODN_ENTRY_1(name, idA, off, compatoff) \
	{ .compat = name, \
	  .id = { idA }, .num_ids = 1, \
	  .reg_offset = off + CONFIG_SYS_IMMR, \
	  .compat_offset = compatoff + CONFIG_SYS_CCSRBAR_PHYS, \
	}

#define SET_LIODN_ENTRY_2(name, idA, idB, off, compatoff) \
	{ .compat = name, \
	  .id = { idA, idB }, .num_ids = 2, \
	  .reg_offset = off + CONFIG_SYS_IMMR, \
	  .compat_offset = compatoff + CONFIG_SYS_CCSRBAR_PHYS, \
	}

/*
 * handle both old and new versioned SEC properties:
 * "fsl,secX.Y" became "fsl,sec-vX.Y" during development
 */
#define SET_SEC_JR_LIODN_ENTRY(jrnum, liodnA, liodnB) \
	SET_LIODN_ENTRY_2("fsl,sec4.0-job-ring", liodnA, liodnB, \
		offsetof(ccsr_sec_t, jrliodnr[jrnum].ls) + \
		CONFIG_SYS_FSL_SEC_OFFSET, \
		CONFIG_SYS_FSL_SEC_OFFSET + 0x1000 + 0x1000 * jrnum), \
	SET_LIODN_ENTRY_2("fsl,sec-v4.0-job-ring", liodnA, liodnB,\
		offsetof(ccsr_sec_t, jrliodnr[jrnum].ls) + \
		CONFIG_SYS_FSL_SEC_OFFSET, \
		CONFIG_SYS_FSL_SEC_OFFSET + 0x1000 + 0x1000 * jrnum)

/* This is a bit evil since we treat rtic param as both a string & hex value */
#define SET_SEC_RTIC_LIODN_ENTRY(rtic, liodnA) \
	SET_LIODN_ENTRY_1("fsl,sec4.0-rtic-memory", \
		liodnA,	\
		offsetof(ccsr_sec_t, rticliodnr[0x##rtic-0xa].ls) + \
		CONFIG_SYS_FSL_SEC_OFFSET, \
		CONFIG_SYS_FSL_SEC_OFFSET + 0x6100 + 0x20 * (0x##rtic-0xa)), \
	SET_LIODN_ENTRY_1("fsl,sec-v4.0-rtic-memory", \
		liodnA,	\
		offsetof(ccsr_sec_t, rticliodnr[0x##rtic-0xa].ls) + \
		CONFIG_SYS_FSL_SEC_OFFSET, \
		CONFIG_SYS_FSL_SEC_OFFSET + 0x6100 + 0x20 * (0x##rtic-0xa))

#define SET_SEC_DECO_LIODN_ENTRY(num, liodnA, liodnB) \
	SET_LIODN_ENTRY_2(NULL, liodnA, liodnB, \
		offsetof(ccsr_sec_t, decoliodnr[num].ls) + \
		CONFIG_SYS_FSL_SEC_OFFSET, 0)

struct liodn_id_table {
	const char *compat;
	u32 id[2];
	u8 num_ids;
	phys_addr_t compat_offset;
	unsigned long reg_offset;
};

struct smmu_stream_id {
	uint16_t offset;
	uint16_t stream_id;
	char dev_name[32];
};

void ls1021x_config_caam_stream_id(struct liodn_id_table *tbl, int size);
void ls102xa_config_smmu_stream_id(struct smmu_stream_id *id, uint32_t num);
void ls1021x_config_smmu3(uint32_t liodn);
#endif
