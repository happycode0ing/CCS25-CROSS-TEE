/*
 * Copyright (c) 2014-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
/* add a new runtime service setup */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <common/debug.h>
#include <common/runtime_svc.h>
#include <lib/el3_runtime/cpu_data.h>
#include <lib/pmf/pmf.h>
#include <lib/psci/psci.h>
#include <lib/runtime_instr.h>
#include <services/my_svc.h>
#include <smccc_helpers.h>
#include <tools_share/uuid.h>
#include <lib/xlat_tables/xlat_tables.h>

/* CAN Protector runtime service */

static int entity_type; // the type of level2-tee communicated with: SUBF_TEE, SUBS_TEE, SUBT_TEE
static int caller_state; // secure 0 / non-secure 1
static uintptr_t  address_start;
static unsigned long length;
static uint8_t * message;

static int security_check()
{
	int ret = 0;
	// check if type is valid
	if (entity_type != SUBF_TEE && entity_type != SUBS_TEE && entity_type != SUBT_TEE)
	{
		return -1;
	}
	// check if address accessd from is legal and permitted, namely, if the security state of caller and address is same.
	if ((address_start & (U(1) << MT_SEC_SHIFT)) >> MT_SEC_SHIFT != caller_state 
		|| ((address_start + length) & (U(1) << MT_SEC_SHIFT)) >> MT_SEC_SHIFT != caller_state)
	{
		return -1;
	}
	memcpy(message, (void *)address_start, length);
	return ret;
}

static int forward_to_can_driver()
{
	int ret = 0;
	// forward instructions/message to can driver in trusted OS
	
	return ret;
}

/* Setup CAN Protecor Service, OEN = 10 */
static int32_t cp_svc_setup(void)
{
	int ret = 0;
	
	return ret;
}

/* CAN Protector Service SMC Handler */
/* x1: tee_type, x2: address_start, x3: length */
static uintptr_t cp_svc_smc_handler(uint32_t smc_fid,
			     u_register_t x1,
			     u_register_t x2,
			     u_register_t x3,
			     u_register_t x4,
			     void *cookie,
			     void *handle,
			     u_register_t flags)
{
	uint32_t result;
	if (((smc_fid >> FUNCID_CC_SHIFT) & FUNCID_CC_MASK) == SMC_32)
	{
		/* 32-bit SMC function, clear top parameter bits */
		x1 &= UINT32_MAX;
		x2 &= UINT32_MAX;
		x3 &= UINT32_MAX;
		x4 &= UINT32_MAX;
	}
	switch (smc_fid)
	{
		case CP_SVC_INVOKE:
			/* return invoke result */
			entity_type = x1;
			address_start = x2;
			length = x3;
			caller_state = is_caller_non_secure(flags);
			result = security_check();
			if (!result)
			{
				forward_to_can_driver();
				SMC_RET1(handle, SMC_OK);
			}
			SMC_RET1(handle, SMC_UNK);
		default:
			VERBOSE("Unimplemented CP Service Call: 0x%x \n", smc_fid);
			SMC_RET1(handle, SMC_UNK);
	}
}

/* Register CP Service Calls as runtime service */
DECLARE_RT_SVC(
		cp_svc,
		OEN_CP_START,
		OEN_CP_END,
		SMC_TYPE_FAST,
		cp_svc_setup,
		cp_svc_smc_handler
);
