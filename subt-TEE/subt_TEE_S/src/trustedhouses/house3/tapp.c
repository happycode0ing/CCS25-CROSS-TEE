/*
 * tapp.c
 *
 *  Created on: 2024年11月14日
 *      Author: 27951
 */
#include "housekeeper.h"

int tapp3(char * buf, unsigned long int len)
{
    char * accessible_buffer;
    unsigned long int buffer_length;
    // 获得当前tapp的可访问空间参数
    accessible_buffer = buf;
    buffer_length = len;
    printf("house 3: hello world\r\n");

    // write some data into buffer
    for(int i = 0; i < 10; i++)
    {
        accessible_buffer[i] = '3';
    }

    // read some data from buffer
    for(int i = 0; i < 10; i++)
    {
        printf("%c", accessible_buffer[i]);
    }
    printf("\n");

    return 0;
}
