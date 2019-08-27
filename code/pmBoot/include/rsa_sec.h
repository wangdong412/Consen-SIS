/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __RSA_SEC_H
#define __RSA_SEC_H

#include <common.h>
#include <jr.h>

struct pk_in_params {
	uint8_t *e;
	uint32_t e_siz;
	uint8_t *n;
	uint32_t n_siz;
	uint8_t *a;
	uint32_t a_siz;
	uint8_t *b;
	uint32_t b_siz;
};

struct rsa_context {
	struct pk_in_params pkin;
	uint32_t rsa_desc[64];
	struct result op;
};

int rsa_public_verif_sec(unsigned char *sign, uint8_t *to, uint8_t *rsa_pub_key,
			int klen, struct rsa_context *ctx, struct jobring *jr);

#endif
