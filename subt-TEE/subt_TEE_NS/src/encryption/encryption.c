/*
 * encryption.c
 *
 *  Created on: 2024年9月21日
 *      Author: 27951
 */
#include "encryption.h"
#include "aes.h"

#include <string.h>

// CBC mode

#if defined(AES256)
    uint8_t key[32] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
#elif defined(AES192)
    uint8_t key[24] = { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
                        0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b };
#elif defined(AES128)
//    uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t key[16];
#endif

    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

void get_key(void)
{
    // 获取key
    uint32_t key1 =  get_key1_from_sec();
    uint32_t key2 =  get_key2_from_sec();
    uint32_t key3 =  get_key3_from_sec();
    uint32_t key4 =  get_key4_from_sec();
    *key = (key1 & 0xff000000) >> 24;
    *(key + 1) = (key1 & 0x00ff0000) >> 16;
    *(key + 2) = (key1 & 0x0000ff00) >> 8;
    *(key + 3) = (key1 & 0x000000ff) >> 0;
    *(key + 4) = (key2 & 0xff000000) >> 24;
    *(key + 5) = (key2 & 0x00ff0000) >> 16;
    *(key + 6) = (key2 & 0x0000ff00) >> 8;
    *(key + 7) = (key2 & 0x000000ff) >> 0;
    *(key + 8) = (key3 & 0xff000000) >> 24;
    *(key + 9) = (key3 & 0x00ff0000) >> 16;
    *(key + 10) = (key3 & 0x0000ff00) >> 8;
    *(key + 11) = (key3 & 0x000000ff) >> 0;
    *(key + 12) = (key4 & 0xff000000) >> 24;
    *(key + 13) = (key4 & 0x00ff0000) >> 16;
    *(key + 14) = (key4 & 0x0000ff00) >> 8;
    *(key + 15) = (key4 & 0x000000ff) >> 0;
}

void encrypt_data(uint8_t * buffer, unsigned int length)
{
    struct AES_ctx ctx;
    // 获取key
    get_key();
    // 初始化 IV
    AES_init_ctx_iv(&ctx, key, iv);
    // 清除key
    memset(key, 0, 16);
    // 填充buffer长度为16Bytes的整数倍，AES128=16*8 mode
    unsigned int old_len = length;
    length = (length / 16) * 16 + ((length%16) ? 16: 0);
    for(unsigned int padi = old_len; padi < length; padi++)
    {
        *(buffer+padi) = 0x00;
    }
    // 加密
    AES_CBC_encrypt_buffer(&ctx, (uint8_t *)buffer, length);
    return ;
}


void decrypt_data(uint8_t * buffer, unsigned int length)
{
    struct AES_ctx ctx;
    // 获取key
    get_key();
    // 初始化 IV
    AES_init_ctx_iv(&ctx, key, iv);
    // 清除key
    memset(key, 0, 16);
    // 填充长度为16Bytes的整数倍
    length = (length / 16) * 16 + ((length%16) ? 16: 0);
    // 解密
    AES_CBC_decrypt_buffer(&ctx, (uint8_t *)buffer, length);

    return ;
}

