#include "eth_task.h"
#include "network.h"
#include "wifi.h"
#include "uart4_manag.h"

extern recv_uart_t recv_uart_4;
extern data_t data_eth; 

extern osMessageQId que_gw_to_ethHandle;

extern osSemaphoreId binarySemQueueHandle;



///data_t 

/* eth_task function std. => ethernet Device Manage

*/
void eth_task(void const * argument)
{
  /* USER CODE BEGIN edv_manag_task */
	uint8_t ret = 0;
	osStatus os_state;
	/* USER CODE BEGIN StartReceiver */
	osEvent retvalue;
	uint16_t queue_len = 0;
	uint8_t i = 0;
	
	print_debug_semph(0,"eth_task is running ...\n");
	print_debug_semph(0,"[INFO]Module Lan, Wifi\n");
	memset(&recv_uart_4,0,sizeof(recv_uart_t));
	memset(&data_eth,0,sizeof(data_t));
	
//	wifi_debug(W_DEBUG_MODE);
//	wifi_dns((uint8_t)0x01);
//	//wifi_connect_ap((const char*)"BK-Wifi",(const char*)"pass123456");
//	wifi_connect_ap((const char*)"@Next-Wifi-2G",(const char*)"mestnics");
//	//wifi_connect((const char*)"www.google.com",80);
//	wifi_connect((const char*)"io.adafruit.com",1883);
	//io.adafruit.com:1883
  /* Infinite loop */
	
	//wifi_debug(W_DEBUG_MODE);
	//wifi_debug(W_DEBUG_MODE);
	
	net_enable(NET_ENABLE);
	net_protocol(NET_MQTT_PTC);
	net_module(NET_WIFI);
	
  for(;;)
  {
		
//		
//		if((os_state=osSemaphoreWait(binarySemQueueHandle,3000))==0){
//			
//			osSemaphoreRelease(binarySemQueueHandle);
//		}
		//xQueueReceive
//		if(que_gw_to_ethHandle!=NULL){
//			print_debug_semph(0,"[INFO]{Queue}get.\n");
//			//retvalue = osMessageGet(que_gw_to_ethHandle,2000);
////			queue_len = ((data_t*)retvalue.value.p)->length;
////			if( queue_len > 0){
////				print_debug_semph(0,"[INFO]{QUEUE}Therer is que_gw_to_eth(len(%d)).\n",queue_len);
////				
////				print_payload(0,((data_t*)retvalue.value.p)->value,((data_t*)retvalue.value.p)->length);
////			}
////				print_debug_semph(0,"[INFO]{Queue}receive.\n");
//		}
//		
		
		
//		if(que_gw_to_ethHandle!=NULL){
//			//queue_eth_data.length = 0;
//			if (xQueueReceive(que_gw_to_ethHandle, (client_dv_t*)&queue_eth_data, 100) == pdTRUE) {
////				if( queue_eth_data.length > 0){
////					print_debug_semph(0,"[INFO]{QUEUE}Therer is que_gw_to_eth(len(%d)).\n",queue_eth_data.length);
////					
////					//print_payload(0,queue_eth_data.,queue_eth_data.length);
////				}
//				
//				print_debug_semph(0,"[INFO]{data report from queue}\n");
//				print_debug_semph(0,"client id: ");
//				//Print Adr of client
//				for(i=0;i<8;i++){
//					print_debug_semph(0,"%02X ",queue_eth_data.address[i]);
//				}
//				print_debug_semph(0,"\n");
//				
//				for(i=0;i<4;i++){
//					print_debug_semph(0,"[INFO] Data ID : Sensor %d \n",queue_eth_data.data_set[i].data_id);
//					print_debug_semph(0,"[INFO] Data Type :  0x%x \n",queue_eth_data.data_set[i].data_type);
//					print_debug_semph(0,"[INFO] Value :  %0.3f \n",queue_eth_data.data_set[i].value);
//				}
//			}
//		}
		
		

		
		
		net_connect();
//		if(wifi_connected()){
//			print_debug_semph(0,"[INFO]wifi connectd with server.\n");
//		}else{
//			print_debug_semph(0,"[INFO]wifi not connectd with server.\n");
//		}
//		memcpy(data_eth.value,(const void*)"Hi Lan.\n",8);
//		data_eth.length = 8;
//		eth_transmit(data_eth.value,data_eth.length);
		//osDelay(500);
//		if((ret = eth_receiv(data_eth.value, &data_eth.length))==1){
//			
//			if(data_eth.length > 0){
//				print_payload(0,data_eth.value,data_eth.length);
//				//HAL_UART_Transmit(&huart4,data_eth.value,data_eth.length,10);
//				//print_payload(0,data_eth.value,data_eth.length);
//			}
//		}
    osDelay(1000);
  }
  /* USER CODE END rf_task */
}


int8_t eth_transmit(uint8_t *value,uint16_t len){
	uint8_t ret = 0;
	ret = wifi_write(value, len);
	
	return ret;
}

int8_t eth_receiv(uint8_t *value,uint16_t *len){
	uint8_t ret = 0;
	ret = wifi_read(value, len);
	return ret;
}
