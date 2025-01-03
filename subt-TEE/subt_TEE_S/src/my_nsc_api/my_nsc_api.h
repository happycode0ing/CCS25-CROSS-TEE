/*
 * my_nsc_api.h
 *
 *  Created on: 2024年9月2日
 *      Author: 27951
 */

#ifndef MY_NSC_API_MY_NSC_API_H_
#define MY_NSC_API_MY_NSC_API_H_
#include "hal_data.h"

// 自定义的NSC函数
BSP_CMSE_NONSECURE_ENTRY void My_nsc_api_Init(void);
BSP_CMSE_NONSECURE_ENTRY uint32_t get_key1_from_sec(void);
BSP_CMSE_NONSECURE_ENTRY uint32_t get_key2_from_sec(void);
BSP_CMSE_NONSECURE_ENTRY uint32_t get_key3_from_sec(void);
BSP_CMSE_NONSECURE_ENTRY uint32_t get_key4_from_sec(void);

#endif /* MY_NSC_API_MY_NSC_API_H_ */
