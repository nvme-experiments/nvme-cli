/* SPDX-License-Identifier: GPL-2.0-or-later */
#ifndef TYPES_H
#define TYPES_H

#include "nvme/types.h"

/**
 * struct nvme_identify_args - Arguments for the NVMe Identify command
 * @nsid:		Namespace identifier, if applicable
 * @cntid:		The Controller Identifier, if applicable
 * @cns:		The Controller or Namespace structure, see @enum nvme_identify_cns
 * @csi:		Command Set Identifier
 * @cnssid:	    Identifier that is required for a particular CNS value
 * @uidx:		UUID Index if controller supports this id selection method
 * @data:		User space destination address to transfer the data
 * @result:		The command completion result from CQE dword0
 */
struct nvme_identify_args {
	__u32 nsid;
	__u16 cntid;
	enum nvme_identify_cns cns;
	enum nvme_csi csi;
	__u16 cnssid;
	__u8 uidx;
	void *data;
	__u32 *result;
};

#endif
