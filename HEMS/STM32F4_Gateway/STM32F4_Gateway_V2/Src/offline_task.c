#include "offline_task.h"
#include "bluetooth.h"
#include "uart4_manag.h"

extern recv_uart_t recv_uart_4;
extern data_t data_bt; 

/* edv_manag_task function std. => End Device Manage

*/
void offline_task(void const * argument)
{
  /* USER CODE BEGIN edv_manag_task */
	uint8_t ret = 0;
	osStatus os_state;
	
	print_debug_semph(0,"[INFO]offline_task is running ...\n");
	print_debug_semph(0,"[INFO]Module Bluetooth\n");
	memset(&recv_uart_4,0,sizeof(recv_uart_t));
	memset(&data_bt,0,sizeof(data_t));
  /* Infinite loop */
  for(;;)
  {
		memcpy(data_bt.value,(const void*)"Hi Bluetooth.\n",14);
		data_bt.length = 14;
		offline_transmit(data_bt.value,data_bt.length);
		//osDelay(500);
		if((ret = offline_receiv(data_bt.value, &data_bt.length))==1){
			//print_payload(0,data_bt.value,data_bt.length);
			if(data_bt.length > 0){
				HAL_UART_Transmit(&huart4,data_bt.value,data_bt.length,10);
			}
		}
    osDelay(500);
  }
  /* USER CODE END rf_task */
}


int8_t offline_transmit(uint8_t *value,uint16_t len){
	uint8_t ret = 0;
	ret = bt_write(value, len);
	
	return ret;
}

int8_t offline_receiv(uint8_t *value,uint16_t *len){
	uint8_t ret = 0;
	ret = bt_read(value, len);
	return ret;
}
