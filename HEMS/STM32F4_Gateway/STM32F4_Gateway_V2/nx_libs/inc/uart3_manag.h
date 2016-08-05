#ifndef UART3_MANAG_H
#define UART3_MANAG_H

#include "main.h"

#define MAX_BUFF_UART3	512

void uart3_callback();
int8_t uart3_read(uint8_t *value,uint16_t *len,uint16_t size);
int8_t uart3_write(uint8_t *value,uint16_t len);

extern void uart3_callback();



#endif 