// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2023 Solidigm.
 *
 * Authors: leonardo.da.cunha@solidigm.com
 * Hardeep.Dhillon@solidigm.com
 */

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include "common.h"
#include "nvme.h"
#include "libnvme.h"
#include "plugin.h"
#include "nvme-print.h"
#include "solidigm-util.h"

#define MARKET_LOG_LID 0xDD
#define MARKET_LOG_MAX_SIZE 512

int sldgm_get_market_log(int argc, char **argv, struct command *command,
				struct plugin *plugin)
{
	const char *desc = "Get Solidigm Marketing Name log and show it.";
	const char *raw = "dump output in binary format";
	_cleanup_nvme_root_ nvme_root_t r = NULL;
	_cleanup_nvme_link_ nvme_link_t l = NULL;
	char log[MARKET_LOG_MAX_SIZE];
	int err;
	__u8 uuid_idx;
	bool  raw_binary = false;

	OPT_ARGS(opts) = {
		OPT_FLAG("raw-binary", 'b', &raw_binary, raw),
		OPT_INCR("verbose", 'v', &nvme_cfg.verbose, verbose),
		OPT_END()
	};

	err = parse_and_open(&r, &l, argc, argv, desc, opts);
	if (err)
		return err;

	sldgm_get_uuid_index(l, &uuid_idx);

	struct nvme_get_log_args args = {
		.nsid	= NVME_NSID_ALL,
		.rae	= false,
		.lsp	= NVME_LOG_LSP_NONE,
		.lid	= MARKET_LOG_LID,
		.lsi	= NVME_LOG_LSI_NONE,
		.csi	= NVME_CSI_NVM,
		.ot	= false,
		.uidx	= uuid_idx,
		.lpo	= 0,
		.log	= log,
		.len	= sizeof(log),
		.result = NULL,
	};

	err = nvme_get_log(l, args.nsid, args.rae, args.lsp, args.lid, args.lsi, args.csi, args.ot,
					   args.uidx, args.lpo, args.log, args.len, NVME_LOG_PAGE_PDU_SIZE,
					   args.result);
	if (err) {
		nvme_show_status(err);
		return err;
	}
	if (!raw_binary)
		printf("Solidigm Marketing Name Log:\n%s\n", log);
	else
		d_raw((unsigned char *)&log, sizeof(log));

	return err;
}
