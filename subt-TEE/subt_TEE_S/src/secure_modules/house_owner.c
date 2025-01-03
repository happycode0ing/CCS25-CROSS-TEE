/*
 * house_owner.c
 *
 *  Created on: 2024年11月14日
 *      Author: 27951
 */
#include <secure_modules/house_owner.h>
#include "trustedhouses/house1/housekeeper.h"
#include "trustedhouses/house2/housekeeper.h"
#include "trustedhouses/house3/housekeeper.h"

TrustedHouse trustedhouse[TRUSTEDHOUSE_MAXN];
static uint8_t already_hkid;
static uint8_t already_taid;
static uint8_t house_num;

void trustedhouse_init(void)
{
    already_taid = 0x00;
    already_hkid = 0x00;
    house_num = 0x00;
    return ;
}

TrustedHouse trustedhouse_create(void)
{
    TrustedHouse th;
    th.valid = 1;
    th.tapp_id = already_taid++;
    th.housekeeper_id = already_hkid++;
    th.buffer_length = SPACE_LENGTH; // default 256B
    th.buffer = (char *)malloc(th.buffer_length * sizeof(char));
    if (th.buffer == NULL)
    {
        printf("malloc failed!\r\n");
        th.valid = 0;
        // 回收id
        already_taid--;
        already_hkid--;
        return th;
    }
    trustedhouse[house_num++] = th;
    return th;
}

void trustedhouse_destroy(uint8_t id)
{
    trustedhouse[id].tapp_id = 0xff;
    trustedhouse[id].housekeeper_id = 0xff;
    free(trustedhouse[id].buffer);
    trustedhouse[id].buffer = NULL;
    trustedhouse[id].buffer_length = 0;
    trustedhouse[id].valid = 0;
}

void trustedhouse_enter(uint8_t id)
{
    switch(id)
    {
        case HOUSEKEEPER_1:
            if (trustedhouse[id].valid)
            {
                housekeeper1();
            }
            else
            {
                printf("trusted house does not exist!\r\n");
            }
            break;
        case HOUSEKEEPER_2:
            if (trustedhouse[id].valid)
            {
                housekeeper2();
            }
            else
            {
                printf("trusted house does not exist!\r\n");
            }
            break;
        case HOUSEKEEPER_3:
            if (trustedhouse[id].valid)
            {
                housekeeper3();
            }
            else
            {
                printf("trusted house does not exist!\r\n");
            }
            break;
        default:
            break;
    }
}
