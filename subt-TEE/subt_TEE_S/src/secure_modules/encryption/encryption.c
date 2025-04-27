/*
 * encryption.c
 *
 *  Created on: 2024年9月21日
 *      Author: 27951
 */
#include "encryption.h"
#include "key_management.h"
//#include "aes.h"
#include "ascon.h"
#include "crypto_aead.h"
#include <string.h>

extern uint32_t key1;
extern uint32_t key2;
extern uint32_t key3;
extern uint32_t key4;

const unsigned char npub[] = "\xb5\xe1\xb1\x77\x18\x6e\x8b\x9b\xc4\x2e\x5e\xf1\xe5\x6f\x9f\x39";  // nonce
//    uint8_t key[] = { 0x16, 0x8d, 0xd8, 0x28, 0x58, 0x60, 0x0b, 0x08, 0x4d, 0x4d, 0x0b, 0xc1, 0xed, 0x2a, 0xd9, 0x65 };
volatile   uint8_t key[16];

void init_key(void)
{
    *key = (key1 & 0xff000000) >> 24;
    *(key+1) = (key1 & 0x00ff0000) >> 16;
    *(key+2) = (key1 & 0x0000ff00) >> 8;
    *(key+3) = (key1 & 0x000000ff);
    *(key+4) = (key2 & 0xff000000) >> 24;
    *(key+5) = (key2 & 0x00ff0000) >> 16;
    *(key+6) = (key2 & 0x0000ff00) >> 8;
    *(key+7) = (key2 & 0x000000ff);
    *(key+8) = (key3 & 0xff000000) >> 24;
    *(key+9) = (key3 & 0x00ff0000) >> 16;
    *(key+10) = (key3 & 0x0000ff00) >> 8;
    *(key+11) = (key3 & 0x000000ff);
    *(key+12) = (key4 & 0xff000000) >> 24;
    *(key+13) = (key4 & 0x00ff0000) >> 16;
    *(key+14) = (key4 & 0x0000ff00) >> 8;
    *(key+15) = (key4 & 0x000000ff);
}


void encrypt_data(uint8_t * result, unsigned long long *retlen, uint8_t * buffer, unsigned int length)
{
//    unsigned char *c;
//    unsigned long long *clen;
//    const unsigned char *m;
//    unsigned long long mlen;
    const unsigned char ad[] = "lss";
    unsigned long long adlen=3;
//    const unsigned char *nsec;
//    const unsigned char npub[] = "\xb5\xe1\xb1\x77\x18\x6e\x8b\x9b\xc4\x2e\x5e\xf1\xe5\x6f\x9f\x39";  // nonce
//    const unsigned char *k;
    // initialize key
    init_key();
    int ret = crypto_aead_encrypt((unsigned char *)result, (unsigned long long *)retlen,
                                  (const unsigned char *)buffer, (unsigned long long)length,
                                  (const unsigned char*)ad, (unsigned long long)adlen,
                                  (const unsigned char*)NULL, (const unsigned char*)npub,
                                  (const unsigned char*)key);
    if(!ret)
    {
//        printf("crypto_aead_encrypt success!\r\n");
    }
    return ;
}


void decrypt_data(uint8_t * result, unsigned long long *retlen, uint8_t * buffer, unsigned int length)
{
//    unsigned char *c;
//    unsigned long long *clen;
//    const unsigned char *m;
//    unsigned long long mlen;
    const unsigned char *ad = "lss";
    unsigned long long adlen=3;
//    const unsigned char *nsec;
//    const unsigned char npub[] = "\xb5\xe1\xb1\x77\x18\x6e\x8b\x9b\xc4\x2e\x5e\xf1\xe5\x6f\x9f\x39";  // nonce
//    const unsigned char *k;

    int ret = crypto_aead_decrypt((unsigned char*)result, (unsigned long long*)retlen,
                                  (unsigned char*)NULL, (const unsigned char*)buffer,
                                  (unsigned long long)length, (const unsigned char*)ad,
                                  (unsigned long long)adlen, (const unsigned char*)npub,
                                  (const unsigned char*) key);
    if(!ret)
    {
//        printf("crypto_aead_decrypt success!\r\n");
    }

    return ;
}

