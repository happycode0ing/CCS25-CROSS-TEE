/*
 * Copyright (c) 2014-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
/* test to add a new runtime service setup */
#include <assert.h>
#include <stdint.h>

#include <common/debug.h>
#include <common/runtime_svc.h>
#include <lib/el3_runtime/cpu_data.h>
#include <lib/pmf/pmf.h>
#include <lib/psci/psci.h>
#include <lib/runtime_instr.h>
#include <services/my_svc.h>
#include <smccc_helpers.h>
#include <tools_share/uuid.h>

/* Setup Test Service, OEN = 8 */
static int32_t test_svc_setup(void)
{
	int ret = 0;
	
	return ret;
}

/* Test Service SMC Handler */
static uintptr_t test_svc_smc_handler(uint32_t smc_fid,
			     u_register_t x1,
			     u_register_t x2,
			     u_register_t x3,
			     u_register_t x4,
			     void *cookie,
			     void *handle,
			     u_register_t flags)
{
	int result;
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
		case TEST_SVC_ADD:
			/* return add result */
			result = 0;
			SMC_RET1(handle, result);
		case TEST_SVC_MUL:
			/* return mul result */
			result = 1;
			SMC_RET1(handle, result);
		default:
			VERBOSE("Unimplemented Test Service Call: 0x%x \n", smc_fid);
			SMC_RET1(handle, SMC_UNK);
	}
}

/* Register Test Service Calls as runtime service */
DECLARE_RT_SVC(
		test_svc,
		OEN_RSV_START,
		OEN_RSV_START,
		SMC_TYPE_FAST,
		test_svc_setup,
		test_svc_smc_handler
);
