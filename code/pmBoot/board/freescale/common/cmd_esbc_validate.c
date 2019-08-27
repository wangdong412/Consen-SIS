/*
 * Copyright 2010-2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <fsl_validate.h>

static int do_esbc_validate(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	if (argc < 2)
		return cmd_usage(cmdtp);

	return fsl_secboot_validate(cmdtp, flag, argc, argv);
}

static int do_esbc_blob_encap(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	if (argc < 5)
		return cmd_usage(cmdtp);
	return fsl_secboot_blob_encap(cmdtp, flag, argc, argv);
}

static int do_esbc_blob_decap(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	if (argc < 5)
		return cmd_usage(cmdtp);
	return fsl_secboot_blob_decap(cmdtp, flag, argc, argv);
}

U_BOOT_CMD(
	esbc_validate,	3,	0,	do_esbc_validate,
	"Validates signature on a given image using RSA verification",
	"<hdr_addr> <hash_val>"
);

static int do_esbc_halt(cmd_tbl_t *cmdtp, int flag, int argc,
				char * const argv[])
{
	printf("Core is entering spin loop.\n");
loop:
	goto loop;

	return 0;
}

U_BOOT_CMD(
	esbc_halt,	1,	0,	do_esbc_halt,
	"Put the core in spin loop ",
	""
);

U_BOOT_CMD(
	esbc_blob_encap,	5,	0,	do_esbc_blob_encap,
	"Creates a Cryptographic blob using a Blob Key",
	"<src addr> <dest addr > <size of data> <128 bit key idnfr>"
);

U_BOOT_CMD(
	esbc_blob_decap,	5,	0,	do_esbc_blob_decap,
	"Decapsulates the cryptgraphic blob",
	"<blob pointer> <dest addr> <size of decap blob> <128 bit key idnfr>"
);
