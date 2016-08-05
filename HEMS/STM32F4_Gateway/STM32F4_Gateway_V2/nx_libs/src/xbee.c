#include "xbee.h"
#include "print_debug.h"


/*
xbee_api_proc std. => xbee api process
*/
void xbee_api_proc(){

}

void xbee_at_cmd(){

}

int8_t xbee_read(uint8_t *value, uint16_t *len){
	int8_t ret = 0;
	ret = uart3_read(value,len,255);
//	if(len > 0){
//		HAL_UART_Transmit(&huart3,value,*len,10);
//	}
	
	return ret;
}

int8_t xbee_write(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	ret = uart3_write(value, len);
	return ret;
}