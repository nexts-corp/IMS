#ifndef OFFLINE_H
#define OFFLINE_H

#include "main.h"

void offline_task(void const * argument);
int8_t offline_transmit(uint8_t *value,uint16_t len);
int8_t offline_receiv(uint8_t *value,uint16_t *len);

extern void offline_task(void const * argument);


#endif 