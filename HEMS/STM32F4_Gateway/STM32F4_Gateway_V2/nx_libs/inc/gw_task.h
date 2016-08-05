#ifndef GW_TASK_H
#define GW_TASK_H

#include "main.h"

void gw_task(void const * argument);
int8_t gw_transmit(uint8_t *value,uint16_t len);
int8_t gw_receiv(uint8_t *value,uint16_t *len);

extern void gw_task(void const * argument);





#endif 