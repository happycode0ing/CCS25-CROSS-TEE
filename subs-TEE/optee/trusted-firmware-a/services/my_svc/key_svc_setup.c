/*
 * Copyright (c) 2014-2023, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
/* add a key-related runtime service setup */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <common/debug.h>
#include <common/runtime_svc.h>
#include <lib/el3_runtime/cpu_data.h>
#include <lib/pmf/pmf.h>
#include <lib/psci/psci.h>
#include <lib/runtime_instr.h>
#include <services/my_svc.h>
#include <smccc_helpers.h>
#include <tools_share/uuid.h>

/* It needs a l2tee_key_table in fact, (level2-tee, key), which is special in Center-TEE. 
** key_update() function is to be defined. Before a key is updated, it must to be informed to the correct l2tee, avoiding key-inconsistency.
*/
// a key keeps to be valid for 1 hour at most, 60*60*1=3600
#define MAX_TIME 3600

static uint32_t l2tee_key_table[3];
static time_t used_time[3];

static void key_update(unsigned int l2tee_type)
{
	uint32_t new_key;
	uint32_t old_key = l2tee_key_table[l2tee_type];
	time_t current_time;
	time(&current_time);
	unsigned long passed_time = difftime(current_time, used_time[l2tee_type]);
	if (passed_time < MAX_TIME)
	{
		/* key is in valid time, don't need to be updated */
		return ;
	}
	uint32_t lowbits = passed_time & 0xffffffff;
	uint32_t highbits = (passed_time >> 32) & 0xffffffff;
	used_time[l2tee_type] = current_time;
	new_key = (old_key & lowbits) ^ highbits;
	l2tee_key_table[l2tee_type] = new_key;
	return ;
}

/* Setup Key Service, OEN = 9 */
static int32_t key_svc_setup(void)
{
	int ret = 0;
	/* initialize key value & time used for every key */
	l2tee_key_table[SUBF_TEE] = 0xffffffff; /* subf-tee communication needs no encryption because of real-time needs */
	l2tee_key_table[SUBS_TEE] = 0x11223311;
	l2tee_key_table[SUBT_TEE] = 0x11223322;
	// time(&used_time[SUBF_TEE]);
	time(&used_time[SUBS_TEE]);
	time(&used_time[SUBT_TEE]);
	
	// key_update(SUBF_TEE);
	key_update(SUBS_TEE);
	key_update(SUBT_TEE);
	
	return ret;
}

/* Key Service SMC Handler */
static uintptr_t key_svc_smc_handler(uint32_t smc_fid,
			     u_register_t x1,
			     u_register_t x2,
			     u_register_t x3,
			     u_register_t x4,
			     void *cookie,
			     void *handle,
			     u_register_t flags)
{
	uint32_t result = -1;
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
		case KEY_SVC_GET:
			/* return the correct key value */
			// assume that x1 records the type of l2tee.
			if (x1==SUBF_TEE)
			{
				// key_update(SUBF_TEE);
				result = l2tee_key_table[SUBF_TEE];
			}
			else if(x1 == SUBS_TEE)
			{
				key_update(SUBS_TEE);
				result = l2tee_key_table[SUBS_TEE];
			}
			else if(x1 == SUBT_TEE)
			{
				key_update(SUBT_TEE);
				result = l2tee_key_table[SUBT_TEE];
			}
			SMC_RET1(handle, result);
		default:
			VERBOSE("Unimplemented Key Service Call: 0x%x \n", smc_fid);
			SMC_RET1(handle, SMC_UNK);
	}
}

/* Register Key Service Calls as runtime service */
DECLARE_RT_SVC(
		key_svc,
		OEN_KEY_START,
		OEN_KEY_END,
		SMC_TYPE_FAST,
		key_svc_setup,
		key_svc_smc_handler
);
