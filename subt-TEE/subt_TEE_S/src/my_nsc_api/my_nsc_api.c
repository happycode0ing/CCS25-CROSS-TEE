/*
 * my_nsc_api.c
 *
 *  Created on: 2024年9月2日
 *      Author: 27951
 */

#include "secure_modules/encryption/encryption.h"
#include "my_nsc_api.h"
#include "canfd/bsp_canfd0.h"
#include "canfd/bsp_canfd1.h"

#define MAXREGIONS 3

volatile uint8_t buffer[MAXREGIONS][1024] = {"secret data1111 ", "secret data2222 ", "secret data3333 "};

// 初始化secure_function模块
BSP_CMSE_NONSECURE_ENTRY void My_nsc_api_Init(void)
{
    for(int i = 0; i < MAXREGIONS; i++)
    {
        if(i == 0)
        {
            for(int j = 0; j < 1024; )
            {
                memcpy(buffer[i]+j, "secret data1111 ", 16);
                j += 16;
            }
        }
        else if(i == 1)
        {
            for(int j = 0; j < 1024; )
            {
                memcpy(buffer[i]+j, "secret data2222 ", 16);
                j += 16;
            }
        }
        else if(i == 2)
        {
            for(int j = 0; j < 1024; )
            {
                memcpy(buffer[i]+j, "secret data3333 ", 16);
                j += 16;
            }
        }
    }
    return ;
}

BSP_CMSE_NONSECURE_ENTRY int My_NSC_API_ReadSecureData(ciphertext_t *ret, int region_num, unsigned int offset, unsigned int length)
{
    if (region_num > MAXREGIONS-1 || region_num < 0 || offset+length > 1024)
    {
        return -1;
    }
    encrypt_data(ret->data.byte, ret->length, buffer[region_num]+offset, length);
    return 0;
}
