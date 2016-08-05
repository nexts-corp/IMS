#include "uart6_manag.h"

extern recv_uart_t recv_uart_6;
uint8_t uart_6_buff[5];

void uart6_callback(){
	if(HAL_UART_Receive_IT(&huart6,uart_6_buff,1)==HAL_OK){
		if(recv_uart_6.size < MAX_BUFF_UART6){
			recv_uart_6.value[recv_uart_6.idx_w++] = uart_6_buff[0];
			recv_uart_6.size++;
		}else{
			HAL_UART_Transmit(&huart6,(uint8_t*)"[ERROR]Uart6 Read Over flow\n",28,100);
			recv_uart_6.idx_w = 0;
			recv_uart_6.idx_r = 0;
			recv_uart_6.size = 0;
		}
	}
}

int8_t uart6_read(uint8_t *value,uint16_t *len,uint16_t size){
	osStatus os_state;
	int8_t ret = 0;
	
	if((os_state=osSemaphoreWait(myBinarySem01Handle,2000))==0){
		if(recv_uart_6.size>0){
			if((size > 0) && (size <= recv_uart_6.size)){
				memcpy(value,&recv_uart_6.value[recv_uart_6.idx_r],size);
				memcpy(len,&recv_uart_6.size,sizeof(uint16_t));
				recv_uart_6.idx_r += recv_uart_6.size;
				recv_uart_6.size -= recv_uart_6.size;
				ret = 1;
				if(recv_uart_6.idx_r > MAX_BUFF_UART6){
					HAL_UART_Transmit(&huart6,(uint8_t*)"[ERROR]Uart6 Read Over flow\n",28,100);
					recv_uart_6.idx_w = 0;
					recv_uart_6.idx_r = 0;
					recv_uart_6.size = 0;
					ret = 0;
				}
			}else{
				//HAL_UART_Transmit(&huart6,&recv_uart_6.value[recv_uart_6.idx_r],recv_uart_6.size,10);
				memcpy(value,&recv_uart_6.value[recv_uart_6.idx_r],recv_uart_6.size);
				memcpy(len,&recv_uart_6.size,sizeof(uint16_t));
				recv_uart_6.idx_r += recv_uart_6.size;
				recv_uart_6.size -= recv_uart_6.size;
				ret = 1;
				if(recv_uart_6.idx_r > MAX_BUFF_UART6){
					HAL_UART_Transmit(&huart6,(uint8_t*)"[ERROR]Uart6 Read Over flow\n",28,100);
					recv_uart_6.idx_w = 0;
					recv_uart_6.idx_r = 0;
					recv_uart_6.size = 0;
					ret = 0;
				}					
			}

		}
		osSemaphoreRelease(myBinarySem01Handle);
	}
	return ret;
}

int8_t uart6_write(uint8_t *value,uint16_t len){
	osStatus os_state;
	int8_t ret = 0;
	if((os_state=osSemaphoreWait(myBinarySem01Handle,2000))==0){
		HAL_UART_Transmit(&huart6,value,len,100);
		
		osSemaphoreRelease(myBinarySem01Handle);
		ret = 1;
	}
	return ret;
}