#ifndef LOCAL_TASK_H
#define LOCAL_TASK_H

#include "main.h"

typedef struct{
	uint8_t dv_id;
	uint8_t dv_adr[8];
}map_dv_t;

void local_task(void const * argument);
extern void local_task(void const * argument);


int8_t local_transmit(uint8_t *value,uint16_t len);
int8_t local_receiv(uint8_t *value,uint16_t *len);
int8_t local_control_transmit(uint8_t *adr,uint8_t ch, uint8_t val);
int local_xbee_checksum(char buf[],int len);


#endif 