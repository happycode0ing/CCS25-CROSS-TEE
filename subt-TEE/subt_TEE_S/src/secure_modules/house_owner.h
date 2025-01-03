/*
 * house_owner.h
 *
 *  Created on: 2024年11月14日
 *      Author: 27951
 */

#ifndef SECURE_MODULES_HOUSE_OWNER_H_
#define SECURE_MODULES_HOUSE_OWNER_H_
#include "hal_data.h"
#include "stdlib.h"

/* 系统里默认有3个 空trusted house 待使用，优先使用这三个house */

#define SPACE_LENGTH    0x100
#define TRUSTEDHOUSE_MAXN   256

#define HOUSEKEEPER_1   0x00
#define HOUSEKEEPER_2   0x01
#define HOUSEKEEPER_3   0x02

typedef struct TrustedHouse
{
    uint8_t tapp_id;                   /* id_length: 8 bit */
    uint8_t housekeeper_id;            /* 理论上最大数量: 2^8=256 */
    char * buffer;                      /* 可访问空间的开始地址 */
    unsigned long int buffer_length;     /* 可访问空间的长度 */
    int valid;                          /* 是否有效, 1:有效，0:无效 */
}TrustedHouse;

void trustedhouse_init(void);               // 初始化trusted house需使用的全局变量
TrustedHouse trustedhouse_create(void);     // 成功返回创建成功的trusted house
void trustedhouse_destroy(uint8_t id);      // 销毁Trusted House
void trustedhouse_enter(uint8_t id);              // 将控制权转移给对应的housekeeper

#endif /* SECURE_MODULES_HOUSE_OWNER_H_ */
