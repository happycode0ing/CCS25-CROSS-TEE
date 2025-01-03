/*
 * housekeeper.c
 *
 *  Created on: 2024年11月14日
 *      Author: 27951
 */
#include "housekeeper.h"
#include "secure_modules/house_owner.h"

extern TrustedHouse trustedhouse[];

void housekeeper1(void)
{
    int ret;
    // 可访问空间限制，若一个housekeeper管理多个tapp，则由housekeeper分割trustedhouse的buffer为多个片段
    // 当前假设一个housekeeper暂时只有一个tapp
    char * buffer = trustedhouse[0].buffer;     // house1的序号是0
    unsigned long int length = trustedhouse[0].buffer_length;
    ret = tapp1(buffer, length); // 运行该trusted house内对应的tapp
    if (ret)
    {
        printf("tapp execution error!\r\n");
    }
    return ;
}
