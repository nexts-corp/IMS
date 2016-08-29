#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "fatfs.h"
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
	uint8_t value[400]; //512
}recv_uart_t;

////////////////////////// Client Data Report Struct ////////////////
typedef struct {
    uint8_t data_id;
    uint8_t data_type;
    float value;
}data_set_t;

typedef struct{
	uint8_t address[8];
	data_set_t data_set[4];
}client_dv_t;

typedef struct{
	uint8_t size;
	client_dv_t client_dv[10];
}client_tb_t;
/////////////////////////////////////////////////////////

////////////////////////// Client Status Report Struct ////////////////
typedef struct {
    uint8_t dat_switch;//0x00=OFF,0x01=ON
    float dat_volt;
		float dat_amp;
}status_set_t;

typedef struct{
	uint8_t address[8];
	status_set_t status_set;
}status_dv_t;

typedef struct{
	uint8_t size;
	status_dv_t status_dv[10];
}client_stat_tb_t;
/////////////////////////////////////////////////////////

/////////////////////// Control End Device ////////////////////
typedef struct{
	uint8_t dv_id;
	uint8_t value;	//ON, OFF
}ctl_dv_t;

///////////////////////////////////////////////////////////////

////Extern the semaphore
extern osSemaphoreId myBinarySem01Handle;
extern osSemaphoreId binarySemQueueHandle;
extern osSemaphoreId binarySemLocalTaskQueueHandle;

////Extern the queue
extern osMessageQId que_gw_to_ethHandle;
extern osMessageQId q_gw_stat_to_ethHandle;
extern osMessageQId q_eth_stream_to_parserHandle;
extern osMessageQId q_control_end_dvHandle;

////Extern the I2C
extern I2C_HandleTypeDef hi2c1;

////Extern the uart
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;

////Extern the uart callback function
extern void uart1_callback();
extern void uart2_callback();
extern void uart3_callback();
extern void uart4_callback();


//////Extern the data buffer
extern recv_uart_t recv_uart_1,recv_uart_2,recv_uart_3,recv_uart_4;
extern data_t data_gw_xbee,data_bt,data_lan;

#endif