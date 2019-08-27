/*
 * CAAM Error Reporting
 *
 * Copyright 2009-2012 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <jr.h>
#include <malloc.h>

#define JRSTA_SSRC_SHIFT            28
#define JRSTA_CCBERR_CHAID_MASK     0x00f0
#define JRSTA_CCBERR_CHAID_SHIFT    4
#define JRSTA_CCBERR_ERRID_MASK     0x000

#define JRSTA_DECOERR_JUMP          0x08000000
#define JRSTA_DECOERR_INDEX_SHIFT   8
#define JRSTA_DECOERR_INDEX_MASK    0xff00
#define JRSTA_DECOERR_ERROR_MASK    0x00ff

#define SPRINTFCAT(str, format, param, max_alloc)		\
{								\
	char *tmp;						\
								\
	tmp = malloc(sizeof(format) + max_alloc);		\
	sprintf(tmp, format, param);				\
	strcat(str, tmp);					\
	free(tmp);						\
}

static void report_jump_idx(u32 status, char *outstr)
{
	u8 idx = (status & JRSTA_DECOERR_INDEX_MASK) >>
		  JRSTA_DECOERR_INDEX_SHIFT;

	if (status & JRSTA_DECOERR_JUMP)
		strcat(outstr, "jump tgt desc idx ");
	else
		strcat(outstr, "desc idx ");

	SPRINTFCAT(outstr, "%d: ", idx, sizeof("255"));
}

static void report_ccb_status(u32 status, char *outstr)
{
	char *cha_id_list[] = {
		"",
		"AES",
		"DES, 3DES",
		"ARC4",
		"MD5, SHA-1, SH-224, SHA-256, SHA-384, SHA-512",
		"RNG",
		"SNOW f8",
		"Kasumi f8, f9",
		"All Public Key Algorithms",
		"CRC",
		"SNOW f9",
	};
	char *err_id_list[] = {
		"None. No error.",
		"Mode error.",
		"Data size error.",
		"Key size error.",
		"PKHA A memory size error.",
		"PKHA B memory size error.",
		"Data arrived out of sequence error.",
		"PKHA divide-by-zero error.",
		"PKHA modulus even error.",
		"DES key parity error.",
		"ICV check failed.",
		"Hardware error.",
		"Unsupported CCM AAD size.",
		"Class 1 CHA is not reset",
		"Invalid CHA combination was selected",
		"Invalid CHA selected.",
	};
	u8 cha_id = (status & JRSTA_CCBERR_CHAID_MASK) >>
		    JRSTA_CCBERR_CHAID_SHIFT;
	u8 err_id = status & JRSTA_CCBERR_ERRID_MASK;

	report_jump_idx(status, outstr);

	if (cha_id < ARRAY_SIZE(cha_id_list)) {
		SPRINTFCAT(outstr, "%s: ", cha_id_list[cha_id],
			   strlen(cha_id_list[cha_id]));
	} else {
		SPRINTFCAT(outstr, "unidentified cha_id value 0x%02x: ",
			   cha_id, sizeof("ff"));
	}

	if (err_id < ARRAY_SIZE(err_id_list)) {
		SPRINTFCAT(outstr, "%s", err_id_list[err_id],
			   strlen(err_id_list[err_id]));
	} else {
		SPRINTFCAT(outstr, "unidentified err_id value 0x%02x",
			   err_id, sizeof("ff"));
	}
}

static void report_jump_status(u32 status, char *outstr)
{
	SPRINTFCAT(outstr, "%s() not implemented", __func__, sizeof(__func__));
}

static void report_deco_status(u32 status, char *outstr)
{
	const struct {
		u8 value;
		char *error_text;
	} desc_error_list[] = {
		{ 0x00, "None. No error." },
		{ 0x01, "SGT Length Error."},
		{ 0x02, "Reserved." },
		{ 0x03, "Job Ring Control Err. Bad value in JR Control reg." },
		{ 0x04, "Invalid Descriptor Cmd. The Desc Cmd fld invalid." },
		{ 0x05, "Reserved." },
		{ 0x06, "Invalid KEY Command" },
		{ 0x07, "Invalid LOAD Command" },
		{ 0x08, "Invalid STORE Command" },
		{ 0x09, "Invalid OPERATION Command" },
		{ 0x0A, "Invalid FIFO LOAD Command" },
		{ 0x0B, "Invalid FIFO STORE Command" },
		{ 0x0C, "Invalid MOVE Command" },
		{ 0x0D, "Invalid JUMP Command."},
		{ 0x0E, "Invalid MATH Command" },
		{ 0x0F, "Invalid SIGNATURE Command" },
		{ 0x10, "Invalid Sequence Command."},
		{ 0x11, "Skip data type invalid. The type must be 0xE or 0xF."},
		{ 0x12, "Shared Descriptor Header Error" },
		{ 0x13, "Header Error." },
		{ 0x14, "Burster Error." },
		{ 0x15, "Context Register Length Error."},
		{ 0x16, "DMA Error" },
		{ 0x17, "Reserved." },
		{ 0x1A, "Job failed due to JR reset" },
		{ 0x1B, "Job failed due to Fail Mode" },
		{ 0x1C, "DECO Watchdog timer timeout error" },
		{ 0x1D, "DECO Error" },
		{ 0x1E, "DECO tried to copy from DECO having Desc error" },
		{ 0x1F, "LIODN error" },
		{ 0x20, "DECO completed a reset initiated via DRR register" },
		{ 0x21, "Nonce error." },
		{ 0x22, "Meta data is too large (> 511 bytes) for TLS decap " },
		{ 0x80, "DNR (do not run) error" },
		{ 0x81, "undefined protocol command" },
		{ 0x82, "invalid setting in PDB" },
		{ 0x83, "Anti-replay LATE error" },
		{ 0x84, "Anti-replay REPLAY error" },
		{ 0x85, "Sequence number overflow" },
		{ 0x86, "Sigver invalid signature" },
		{ 0x87, "DSA Sign Illegal test descriptor" },
		{ 0x88, "Protocol Format Error" },
		{ 0x89, "Protocol Size Error" },
		{ 0xC1, "Blob Command error: Undefined mode" },
		{ 0xC2, "Blob Command error: Secure Memory Blob mode error" },
		{ 0xC4, "Blob Command error: Black Blob key or input sz err" },
		{ 0xC5, "Blob Command error: Invalid key destination" },
		{ 0xC8, "Blob Command error: Trusted/Secure mode error" },
		{ 0xF0, "IPsec TTL or hop limit field error" },
		{ 0xF1, "3GPP HFN matches or exceeds the Threshold" },
	};
	u8 desc_error = status & JRSTA_DECOERR_ERROR_MASK;
	int i;

	report_jump_idx(status, outstr);

	for (i = 0; i < ARRAY_SIZE(desc_error_list); i++)
		if (desc_error_list[i].value == desc_error)
			break;

	if (i != ARRAY_SIZE(desc_error_list) && desc_error_list[i].error_text) {
		SPRINTFCAT(outstr, "%s", desc_error_list[i].error_text,
			   strlen(desc_error_list[i].error_text));
	} else {
		SPRINTFCAT(outstr, "unidentified error value 0x%02x",
			   desc_error, sizeof("ff"));
	}
}

static void report_jr_status(u32 status, char *outstr)
{
}

static void report_cond_code_status(u32 status, char *outstr)
{
	SPRINTFCAT(outstr, "%s() not implemented", __func__, sizeof(__func__));
}

int caam_jr_strstatus(char *outstr, u32 status)
{
	int ret = 0;
	struct stat_src {
		void (*report_ssed)(u32 status, char *outstr);
		char *error;
	} status_src[] = {
		{ NULL, "No error" },
		{ NULL, NULL },
		{ report_ccb_status, "CCB" },
		{ report_jump_status, "Jump" },
		{ report_deco_status, "DECO" },
		{ NULL, NULL },
		{ report_jr_status, "Job Ring" },
		{ report_cond_code_status, "Condition Code" },
	};
	u32 ssrc = status >> JRSTA_SSRC_SHIFT;

	sprintf(outstr, "%s: ", status_src[ssrc].error);

	if (status_src[ssrc].report_ssed) {
		status_src[ssrc].report_ssed(status, outstr);
		ret = -1;
	} else {
		ret = 0;
	}

	return ret;
}
