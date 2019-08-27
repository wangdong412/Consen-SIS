/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SHA_H
#define _SHA_H

#include <jr.h>

/* number of bytes in the SHA256-256 digest */
#define SHA256_DIGEST_SIZE 32

/*
 * number of words in the digest - Digest is kept internally
 * as 8 32-bit words
 */
#define _SHA256_DIGEST_LENGTH 8

/*
 * block length - A block, treated as a sequence of
 * 32-bit words
 */
#define SHA256_BLOCK_LENGTH 16

/* number of bytes in the block */
#define SHA256_DATA_SIZE 64

#define MAX_SG		12

/*
 * Scatter Gather Entry - Speicifies the the Scatter Gather Format
 * related information
 */
#ifdef CONFIG_SYS_FSL_SEC_LE
struct sg_entry {
	uint32_t addr_lo;	/* Memory Address - lo */
	uint16_t addr_hi;	/* Memory Address of the start of the
				 * buffer - hi
				 */
	uint16_t reserved_zero;
	unsigned int length:30;	/* Length of the data in the frame */
	unsigned int final:1;
	unsigned int extension:1;
	unsigned int offset:13;
	unsigned int reserved_offset:3;
	uint8_t bpid;		/* Buffer Pool Id */
	uint8_t reserved_zero2;
};
#else
struct sg_entry {
	uint16_t reserved_zero;
	uint16_t addr_hi;	/* Memory Address of the start of the
				 * buffer - hi
				 */
	uint32_t addr_lo;	/* Memory Address - lo */
	unsigned int extension:1;
	unsigned int final:1;
	unsigned int length:30;	/* Length of the data in the frame */
	uint8_t reserved_zero2;
	uint8_t bpid;		/* Buffer Pool Id */
	unsigned int reserved_offset:3;
	unsigned int offset:13;
};
#endif

/*
 * SHA256-256 context
 * contain the following fields
 * State
 * count low
 * count high
 * block data buffer
 * index to the buffer
 */
struct sha_ctx {
	struct sg_entry sg_tbl[MAX_SG];
	uint32_t sha_desc[64];
	u8 hash[SHA256_DIGEST_SIZE];
	struct result op;
	uint32_t sg_num;
	uint32_t len;
	struct jobring *jr;
};

void sha_init(struct sha_ctx *ctx, struct jobring *jr);
void sha_update(struct sha_ctx *ctx, uint8_t *data, uint32_t length);
int sha_final(struct sha_ctx *ctx);
int sha_digest(struct sha_ctx *ctx, uint8_t *digest);

#endif
