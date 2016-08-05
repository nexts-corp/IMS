#ifndef OFFLINE_H
#define OFFLINE_H

#include "main.h"

void eth_task(void const * argument);
int8_t eth_transmit(uint8_t *value,uint16_t len);
int8_t eth_receiv(uint8_t *value,uint16_t *len);

extern void eth_task(void const * argument);


#endif 