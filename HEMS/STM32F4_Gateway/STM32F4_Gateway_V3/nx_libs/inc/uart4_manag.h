#ifndef UART4_MANAG_H
#define UART4_MANAG_H

#include "main.h"

#define MAX_BUFF_UART4	512

void uart4_callback();
int8_t uart4_read(uint8_t *value,uint16_t *len,uint16_t size);
int8_t uart4_write(uint8_t *value,uint16_t len);
void uart4_flush();

extern void uart4_callback();



#endif 