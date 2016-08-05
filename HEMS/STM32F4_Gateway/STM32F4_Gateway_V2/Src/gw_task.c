#include "gw_task.h"
#include "xbee.h"
#include "uart3_manag.h"

extern recv_uart_t recv_uart_3;
extern data_t data_gw_xbee; 

/* edv_manag_task function std. => End Device Manage

*/
void gw_task(void const * argument)
{
  /* USER CODE BEGIN edv_manag_task */
	uint8_t ret = 0;
	osStatus os_state;
	
	print_debug_semph(0,"[INFO]gw_task is running ...\n");
	print_debug_semph(0,"[INFO]Module XBee\n");
	memset(&recv_uart_3,0,sizeof(recv_uart_t));
	memset(&data_gw_xbee,0,sizeof(data_t));
  /* Infinite loop */
  for(;;)
  {
		memcpy(data_gw_xbee.value,(const void*)"Hi XBee.\n",9);
		data_gw_xbee.length = 9;
		gw_transmit(data_gw_xbee.value,data_gw_xbee.length);
		//osDelay(500);
		if((ret = gw_receiv(data_gw_xbee.value, &data_gw_xbee.length))==1){
			//print_payload(0,data_gw_xbee.value,data_gw_xbee.length);
			if(data_gw_xbee.length > 0){
				HAL_UART_Transmit(&huart3,data_gw_xbee.value,data_gw_xbee.length,10);
			}
		}
    osDelay(500);
  }
  /* USER CODE END rf_task */
}


int8_t gw_transmit(uint8_t *value,uint16_t len){
	uint8_t ret = 0;
	ret = xbee_write(value, len);
	
	return ret;
}

int8_t gw_receiv(uint8_t *value,uint16_t *len){
	uint8_t ret = 0;
	ret = xbee_read(value, len);
	return ret;
}
