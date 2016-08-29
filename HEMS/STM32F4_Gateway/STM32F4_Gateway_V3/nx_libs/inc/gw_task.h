#ifndef GW_TASK_H
#define GW_TASK_H

/*-------Remote Channel Digital Output Drive-------*/
#define ch1 0x20
#define ch2 0x21
#define ch3 0x22
#define ch4 0x23
#define ch5 0x24
#define ch6 0x25
#define ch7 0x26
#define ch8 0x27
#define ch9 0x28
#define ch10 0x29
#define ALL 0x2f
/* Value */
#define low 0x00
#define high 0x01

/*-------Channel Digital Output Drive-------*/

#include "main.h"

typedef struct{
	uint16_t index;
	uint8_t address[8];
}tableID;

void gw_task(void const * argument);
int8_t gw_transmit(uint8_t *value,uint16_t len);
int8_t gw_receiv(uint8_t *value,uint16_t *len);
int8_t gw_remote(uint16_t index,char ch,char value);
uint8_t gw_Getaddress(uint8_t* dataAll);


extern void gw_task(void const * argument);





#endif 