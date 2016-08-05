#ifndef UART_MANAG_H
#define UART_MANAG_H


#include "main.h"
#include "print_debug.h"

#define MINI_BUFF_UART2		256
#define MORMAL_BUFF_UART2	512
#define MAX_BUFF_UART2		1024


typedef struct{
	uint16_t idx_read;	//index read
	uint16_t idx_write;	//index write
	uint16_t curr_size;	//current size
	uint8_t rx[MORMAL_BUFF_UART2];
	uint8_t tx[MORMAL_BUFF_UART2];
}uart_t;

void uart2_update(UART_HandleTypeDef *huart_ints);
uint16_t uart2_read(uint8_t * buff,uint16_t len);
void uart2_write(UART_HandleTypeDef *huart_ints,uint8_t * buff,uint16_t len);
uint16_t uart2_avaiable();

extern uart_t uart2_var;

#endif 