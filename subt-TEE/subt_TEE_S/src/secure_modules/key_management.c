/*
 * key_management.c
 *
 *  Created on: 2024年11月14日
 *      Author: 27951
 */
#include <secure_modules/key_management.h>
#include "SysTick/bsp_SysTick.h"

// 1 hour=3600s=3600000ms, 2000ms for test
#define VALIDTIME_MAX 2000

volatile uint64_t already_time = 0;

// 初始密钥
uint32_t key1 = 0x2b7e1516; //
uint32_t key2 = 0x28aed2a6; //
uint32_t key3 = 0xabf71588; //
uint32_t key4 = 0x09cf4f3c; //

// key1
uint32_t secure_get_key1_from_sec(void)
{
    update_key();
    return key1;
}

// key2
uint32_t secure_get_key2_from_sec(void)
{
    update_key();
    return key2;
}

// key3
uint32_t secure_get_key3_from_sec(void)
{
    update_key();
    return key3;
}

// key4
uint32_t secure_get_key4_from_sec(void)
{
    update_key();
    return key4;
}

void update_key(void)
{
    uint64_t now;
    now = system_get_time() / 1000000000; // time passed
    if (already_time > VALIDTIME_MAX)
    {
        key1 = (key1 | (now >> 32))  & (now & 0xffffffff);
        key2 = (key2 | (now >> 32))  & (now & 0xffffffff);
        key3 = (key3 | (now >> 32))  & (now & 0xffffffff);
        key4 = (key4 | (now >> 32))  & (now & 0xffffffff);
        already_time = 0;
    }
}
