/*
 * key_management.c
 *
 *  Created on: 2025年3月19日
 *      Author: 27951
 */

#include <secure_modules/encryption/key_management.h>
#include "SysTick/bsp_SysTick.h"

// 1 hour=3600s=3600000ms, 2000ms for test
#define VALIDTIME_MAX 2000

// 初始密钥 0x16, 0x8d, 0xd8, 0x28, 0x58, 0x60, 0x0b, 0x08, 0x4d, 0x4d, 0x0b, 0xc1, 0xed, 0x2a, 0xd9, 0x65
uint32_t key1 = 0x168dd828; //
uint32_t key2 = 0x58600b08; //
uint32_t key3 = 0x4d4d0bc1; //
uint32_t key4 = 0xed2ad965; //

volatile uint64_t already_time = 0;

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
