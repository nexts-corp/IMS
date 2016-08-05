#include "eth_task.h"
#include "lan.h"
#include "uart6_manag.h"

extern recv_uart_t recv_uart_6;
extern data_t data_lan; 

/* eth_task function std. => ethernet Device Manage

*/
void eth_task(void const * argument)
{
  /* USER CODE BEGIN edv_manag_task */
	uint8_t ret = 0;
	osStatus os_state;
	
	print_debug_semph(0,"eth_task is running ...\n");
	print_debug_semph(0,"[INFO]Module Lan, Wifi\n");
	memset(&recv_uart_6,0,sizeof(recv_uart_t));
	memset(&data_lan,0,sizeof(data_t));
  /* Infinite loop */
  for(;;)
  {
		memcpy(data_lan.value,(const void*)"Hi Lan.\n",8);
		data_lan.length = 8;
		eth_transmit(data_lan.value,data_lan.length);
		//osDelay(500);
		if((ret = eth_receiv(data_lan.value, &data_lan.length))==1){
			//print_payload(0,data_lan.value,data_lan.length);
			if(data_lan.length > 0){
				HAL_UART_Transmit(&huart6,data_lan.value,data_lan.length,10);
				print_payload(0,data_lan.value,data_lan.length);
			}
		}
    osDelay(500);
  }
  /* USER CODE END rf_task */
}


int8_t eth_transmit(uint8_t *value,uint16_t len){
	uint8_t ret = 0;
	ret = lan_write(value, len);
	
	return ret;
}

int8_t eth_receiv(uint8_t *value,uint16_t *len){
	uint8_t ret = 0;
	ret = lan_read(value, len);
	return ret;
}
