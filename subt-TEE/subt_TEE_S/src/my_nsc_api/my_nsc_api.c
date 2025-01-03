/*
 * my_nsc_api.c
 *
 *  Created on: 2024年9月2日
 *      Author: 27951
 */

#include <secure_modules/key_management.h>
#include "my_nsc_api.h"
#include "canfd/bsp_canfd0.h"
#include "canfd/bsp_canfd1.h"

// 初始化secure_function模块
BSP_CMSE_NONSECURE_ENTRY void My_nsc_api_Init(void)
{
    return ;
}


BSP_CMSE_NONSECURE_ENTRY uint32_t get_key1_from_sec(void)
{
    uint32_t temp = secure_get_key1_from_sec(); // key1
    return temp;
}

BSP_CMSE_NONSECURE_ENTRY uint32_t get_key2_from_sec(void)
{
    uint32_t temp = secure_get_key2_from_sec(); // key2
    return temp;
}

BSP_CMSE_NONSECURE_ENTRY uint32_t get_key3_from_sec(void)
{
    uint32_t temp = secure_get_key3_from_sec(); // key3
    return temp;
}

BSP_CMSE_NONSECURE_ENTRY uint32_t get_key4_from_sec(void)
{
    uint32_t temp = secure_get_key4_from_sec(); // key4
    return temp;
}

