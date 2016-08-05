#include "lan.h"
#include "print_debug.h"


/*
lan_api_proc std. => LAN api process
*/
void lan_api_proc(){

}

void lan_at_cmd(){

}

int8_t lan_read(uint8_t *value, uint16_t *len){
	int8_t ret = 0;
	ret = uart6_read(value,len,255);
//	if(len > 0){
//		HAL_UART_Transmit(&huart3,value,*len,10);
//	}
	
	return ret;
}

int8_t lan_write(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	ret = uart6_write(value, len);
	return ret;
}