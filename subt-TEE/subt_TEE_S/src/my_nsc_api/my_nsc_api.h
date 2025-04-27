/*
 * my_nsc_api.h
 *
 *  Created on: 2024年9月2日
 *      Author: 27951
 */

#ifndef MY_NSC_API_MY_NSC_API_H_
#define MY_NSC_API_MY_NSC_API_H_
#include "hal_data.h"

typedef struct{
    uint8_t byte[16];
}uint128_t;

typedef struct{
    unsigned long long length[1];
    uint128_t data;
}ciphertext_t;

// 自定义的NSC函数
BSP_CMSE_NONSECURE_ENTRY void My_nsc_api_Init(void);
BSP_CMSE_NONSECURE_ENTRY int My_NSC_API_ReadSecureData(ciphertext_t *ret, int region_num, unsigned int offset, unsigned int length);

#endif /* MY_NSC_API_MY_NSC_API_H_ */
