/*
 * key_management.h
 *
 *  Created on: 2024年11月14日
 *      Author: 27951
 */

#ifndef SECURE_MODULES_KEY_MANAGEMENT_H_
#define SECURE_MODULES_KEY_MANAGEMENT_H_
#include "hal_data.h"

uint32_t secure_get_key1_from_sec(void);
uint32_t secure_get_key2_from_sec(void);
uint32_t secure_get_key3_from_sec(void);
uint32_t secure_get_key4_from_sec(void);
void update_key(void);

#endif /* SECURE_MODULES_KEY_MANAGEMENT_H_ */
