#include "uart3_manag.h"

extern recv_uart_t recv_uart_3;
uint8_t uart_3_buff[5];

void uart3_callback(){
	if(HAL_UART_Receive_IT(&huart3,uart_3_buff,1)==HAL_OK){
		if(recv_uart_3.size < MAX_BUFF_UART3){
			recv_uart_3.value[recv_uart_3.idx_w++] = uart_3_buff[0];
			recv_uart_3.size++;
		}else{
			//HAL_UART_Transmit(&huart3,(uint8_t*)"Uart3 Over flow\n",16,100);
			print_debug_semph(0,"Uart3 Over flow\n");
			recv_uart_3.idx_w = 0;
			recv_uart_3.idx_r = 0;
			recv_uart_3.size = 0;
		}
		
	}
}

int8_t uart3_read(uint8_t *value,uint16_t *len,uint16_t size){
	osStatus os_state;
	int8_t ret = 0;
	
	if((os_state=osSemaphoreWait(myBinarySem01Handle,2000))==0){
		if(recv_uart_3.size>0){
			if((size > 0) && (size <= recv_uart_3.size)){
				memcpy(value,&recv_uart_3.value[recv_uart_3.idx_r],size);
				memcpy(len,&recv_uart_3.size,sizeof(uint16_t));
				recv_uart_3.idx_r += recv_uart_3.size;
				recv_uart_3.size -= recv_uart_3.size;
				ret = 1;
				if(recv_uart_3.idx_r > MAX_BUFF_UART3){
					//HAL_UART_Transmit(&huart3,(uint8_t*)"Uart3 Read Over flow\n",16,100);
					print_debug_semph(0,"Uart3 Over flow\n");
					recv_uart_3.idx_w = 0;
					recv_uart_3.idx_r = 0;
					recv_uart_3.size = 0;
					ret = 0;
				}
			}else{
				//HAL_UART_Transmit(&huart3,&recv_uart_3.value[recv_uart_3.idx_r],recv_uart_3.size,10);
				memcpy(value,&recv_uart_3.value[recv_uart_3.idx_r],recv_uart_3.size);
				memcpy(len,&recv_uart_3.size,sizeof(uint16_t));
				recv_uart_3.idx_r += recv_uart_3.size;
				recv_uart_3.size -= recv_uart_3.size;
				ret = 1;
				if(recv_uart_3.idx_r > MAX_BUFF_UART3){
					//HAL_UART_Transmit(&huart3,(uint8_t*)"Uart3 Read Over flow\n",16,100);
					print_debug_semph(0,"Uart3 Over flow\n");
					recv_uart_3.idx_w = 0;
					recv_uart_3.idx_r = 0;
					recv_uart_3.size = 0;
					ret = 0;
				}					
			}

		}
		osSemaphoreRelease(myBinarySem01Handle);
	}
	return ret;
}

int8_t uart3_write(uint8_t *value,uint16_t len){
	osStatus os_state;
	int8_t ret = 0;
	if((os_state=osSemaphoreWait(myBinarySem01Handle,2000))==0){
		HAL_UART_Transmit(&huart3,value,len,100);
		
		osSemaphoreRelease(myBinarySem01Handle);
		ret = 1;
	}
	return ret;
}