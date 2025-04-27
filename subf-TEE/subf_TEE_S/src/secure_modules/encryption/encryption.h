/*
 * encryption.h
 *
 *  Created on: 2024年9月21日
 *      Author: 27951
 */

#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_
#include <stdlib.h>
#include <stdint.h>

void encrypt_data(uint8_t * result, unsigned long long *retlen, uint8_t * buffer, unsigned int length);
//void encrypt_data(uint8_t * buffer, unsigned int length);
void decrypt_data(uint8_t * result, unsigned long long *retlen, uint8_t * buffer, unsigned int length);
//void decrypt_data(uint8_t * buffer, unsigned int length);

#endif /* ENCRYPTION_H_ */
