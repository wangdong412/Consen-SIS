/*
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <rsa_sec.h>
#include <jobdesc.h>
#include <fsl_secboot_err.h>

void rsa_done(uint32_t desc, uint32_t status, void *arg)
{
	struct result *x = arg;
	x->status = status;
#ifdef DEBUG
	x->err = caam_jr_strstatus(x->outstr, status);
#else
	if (status)
		x->err = -1;
#endif

	x->done = 1;
}

/* This functionw ould return teh status returned by SEC .
 * If non zero , means there was some error reported by SEC */
int rsa_public_verif_sec(unsigned char *sign, uint8_t *to, uint8_t *rsa_pub_key,
			int klen, struct rsa_context *ctx, struct jobring *jr)
{
	unsigned long long timeval;
	unsigned long long timeout;
	int ret = 0;
	memset(ctx, 0, sizeof(struct rsa_context));

	ctx->pkin.a = sign;
	ctx->pkin.a_siz = klen;
	ctx->pkin.n = rsa_pub_key;
	ctx->pkin.n_siz = klen;
	ctx->pkin.e = rsa_pub_key + klen;
	ctx->pkin.e_siz = klen;

	inline_cnstr_jobdesc_pkha_rsaexp(ctx->rsa_desc,
					 &ctx->pkin, to, klen);

	ret = jr_enqueue(jr, ctx->rsa_desc, rsa_done, &ctx->op);
	if (ret)
		return ret;

	timeval = get_ticks();
	timeout = usec2ticks(CONFIG_SEC_DEQ_TIMEOUT);

	while (ctx->op.done != 1) {
		if (jr_dequeue(jr))
			return ERROR_ESBC_SEC_DEQ;

		if ((get_ticks() - timeval) > timeout) {
			printf("SEC Dequeue timed out\n");
			return ERROR_ESBC_SEC_DEQ_TO;
		}
	}

	if (ctx->op.err < 0)
		return ctx->op.status;

	return 0;
}
