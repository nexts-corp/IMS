#ifndef UART6_MANAG_H
#define UART6_MANAG_H

#include "main.h"

#define MAX_BUFF_UART6	512

void uart6_callback();
int8_t uart6_read(uint8_t *value,uint16_t *len,uint16_t size);
int8_t uart6_write(uint8_t *value,uint16_t len);

extern void uart6_callback();



#endif 