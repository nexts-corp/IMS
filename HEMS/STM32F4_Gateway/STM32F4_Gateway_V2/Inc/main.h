#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "uart2_manag.h"
#include "uart3_manag.h"
#include "uart4_manag.h"
#include "uart6_manag.h"

typedef struct{
	uint16_t length;
	uint8_t value[255];
}data_t;

typedef struct{
	uint16_t idx_r;	//index read
	uint16_t idx_w;	//index write
	uint16_t size;	//size of data
	uint8_t value[512];
}recv_uart_t;

void uart2_callback();
void uart3_callback();

extern osSemaphoreId myBinarySem01Handle;
extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

extern void uart2_callback();
extern void uart3_callback();
extern void uart4_callback();
extern void uart6_callback();

extern recv_uart_t recv_uart_2,recv_uart_3,recv_uart_6;
extern data_t data_gw_xbee;
extern data_t data_bt;

#endif