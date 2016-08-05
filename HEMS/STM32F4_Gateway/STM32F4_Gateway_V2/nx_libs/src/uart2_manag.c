#include "uart2_manag.h"


uart_t uart2_var;


void uart2_update(UART_HandleTypeDef *huart_ints){
	uint8_t ch;
	while(HAL_UART_Receive_IT(huart_ints,&ch,1) == HAL_OK){
		uart2_var.rx[uart2_var.idx_write++] = ch;
		uart2_var.curr_size++;
	}
	print_debug_semph(0,"u2-curr_size(%d)\n",uart2_var.curr_size);
}

uint16_t uart2_read(uint8_t * buff,uint16_t max_read){
	uint16_t size;
	
	if((size=uart2_avaiable())>0){

		if(size<=MORMAL_BUFF_UART2){
			memcpy(buff,&uart2_var.rx[uart2_var.idx_read],size);
			uart2_var.idx_read -= size;
			uart2_var.curr_size -= size;
			return size;
		}
	}
}

void uart2_write(UART_HandleTypeDef *huart_ints,uint8_t * buff,uint16_t len){
	uint16_t size,i;
	for(i=0;i<len;i++){
		HAL_UART_Transmit(huart_ints,&buff[i],1,100);
	}
}

uint16_t uart2_avaiable(){
	if(uart2_var.curr_size==0){
		return 0;
	}
	print_debug_semph(0,"u2-curr_size(%d)\n",uart2_var.curr_size);
	return uart2_var.curr_size;
}