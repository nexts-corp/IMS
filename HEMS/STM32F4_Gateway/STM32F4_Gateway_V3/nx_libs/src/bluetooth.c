#include "bluetooth.h"
#include "print_debug.h"


/*
bt_api_proc std. => bluetooth api process
*/


int8_t bt_read(uint8_t *value, uint16_t *len){
	int8_t ret = 0;
	ret = uart1_read(value,len,255);
//	if(len > 0){
//		HAL_UART_Transmit(&huart3,value,*len,10);
//	}
	
	return ret;
}

int8_t bt_write(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	ret = uart1_write(value, len);
	return ret;
}