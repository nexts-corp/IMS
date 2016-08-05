#include "rf_task.h"
#include "xbee.h"
#include "uart3_manag.h"

extern recv_uart_t recv_uart_3;

/* rf_task function */
void rf_task(void const * argument)
{
  /* USER CODE BEGIN rf_task */
	uint8_t i = 0;
	osStatus os_state;
	
	print_debug_semph(0,"rf_task is running ...\n");
	print_debug_semph(0,"[INFO]Module XBee, Bluetooth and Wifi\n");
	memset(&recv_uart_3,0,sizeof(recv_uart_t));
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END rf_task */
}


void rf_transmit(){

}

void rf_receiv(){

}





