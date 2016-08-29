#include "uart4_manag.h"

extern recv_uart_t recv_uart_4;
uint8_t uart_4_buff[5];

void uart4_callback(){
	if(HAL_UART_Receive_IT(&huart4,uart_4_buff,1)==HAL_OK){
		if(recv_uart_4.size <= MAX_BUFF_UART4){
			recv_uart_4.value[recv_uart_4.idx_w++] = uart_4_buff[0];
			recv_uart_4.size++;
			if(recv_uart_4.idx_r > MAX_BUFF_UART4){
				//HAL_UART_Transmit(&huart4,(uint8_t*)"[ERROR]Uart4 Read Over flow\n",28,100);
				print_debug_semph(0,"[ERROR]Uart4 Read Over flow");
				recv_uart_4.idx_w = 0;
				recv_uart_4.idx_r = 0;
				recv_uart_4.size = 0;
			}
		}else{
			//HAL_UART_Transmit(&huart4,(uint8_t*)"[ERROR]Uart4 Read Over flow\n",28,100);
			print_debug_semph(0,"[ERROR]Uart4 Read Over flow");
			recv_uart_4.idx_w = 0;
			recv_uart_4.idx_r = 0;
			recv_uart_4.size = 0;
		}
	}
}

int8_t uart4_read(uint8_t *value,uint16_t *len,uint16_t size){
	osStatus os_state;
	int8_t ret = 0;
	
	if((os_state=osSemaphoreWait(myBinarySem01Handle,2000))==0){
		if(recv_uart_4.size>0){
			if((size > 0) && (size <= recv_uart_4.size)){
				memcpy(value,&recv_uart_4.value[recv_uart_4.idx_r],size);
				memcpy(len,&recv_uart_4.size,sizeof(uint16_t));
				recv_uart_4.idx_r += recv_uart_4.size;
				recv_uart_4.size -= recv_uart_4.size;
				ret = 1;
				if(recv_uart_4.idx_r > MAX_BUFF_UART4){
					//HAL_UART_Transmit(&huart4,(uint8_t*)"[ERROR]Uart4 Read Over flow\n",28,100);
					print_debug_semph(0,"[ERROR]Uart4 Read Over flow");
					recv_uart_4.idx_w = 0;
					recv_uart_4.idx_r = 0;
					recv_uart_4.size = 0;
					ret = 0;
				}
			}else{
				//HAL_UART_Transmit(&huart4,&recv_uart_4.value[recv_uart_4.idx_r],recv_uart_4.size,10);
				memcpy(value,&recv_uart_4.value[recv_uart_4.idx_r],recv_uart_4.size);
				memcpy(len,&recv_uart_4.size,sizeof(uint16_t));
				recv_uart_4.idx_r += recv_uart_4.size;
				recv_uart_4.size -= recv_uart_4.size;
				ret = 1;
				if(recv_uart_4.idx_r > MAX_BUFF_UART4){
					//HAL_UART_Transmit(&huart4,(uint8_t*)"[ERROR]Uart4 Read Over flow\n",28,100);
					print_debug_semph(0,"[ERROR]Uart4 Read Over flow");
					recv_uart_4.idx_w = 0;
					recv_uart_4.idx_r = 0;
					recv_uart_4.size = 0;
					ret = 0;
				}					
			}

		}
		osSemaphoreRelease(myBinarySem01Handle);
	}
	return ret;
}

int8_t uart4_write(uint8_t *value,uint16_t len){
	osStatus os_state;
	int8_t ret = 0;
	if((os_state=osSemaphoreWait(myBinarySem01Handle,2000))==0){
		HAL_UART_Transmit(&huart4,value,len,100);
		
		osSemaphoreRelease(myBinarySem01Handle);
		ret = 1;
	}
	return ret;
}

void uart4_flush(){
	recv_uart_4.idx_w = 0;
	recv_uart_4.idx_r = 0;
	recv_uart_4.size = 0;
}