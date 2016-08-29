#include "network.h"
#include "mqtt_ptc.h"
#include "wifi.h"
#include "stdlib.h"
#include "bluetooth_ptc.h"



extern osMessageQId que_gw_to_ethHandle;
extern osMessageQId q_gw_stat_to_ethHandle;
extern osMessageQId q_eth_stream_to_parserHandle;
extern osMessageQId q_control_end_dvHandle;

extern osSemaphoreId binarySemQueueHandle;

network_t network;
network_data_t network_data;

client_dv_t queue_eth_data;
status_dv_t queue_eth_status;


//0x00,0x13,0xA2,0x00,0x40,0x9F,0x39,0x9E

void net_enable(int8_t enable){
	network.enable = enable;
}

void net_protocol(int8_t protocol_type){
	network.protocol = protocol_type;
}

void net_module(int8_t module){
	network.module = module;
}

int8_t net_connect(){
	int8_t ret = 0;
	uint8_t i = 0;
	uint8_t q_ret = 0;
	
	uint8_t t_count = 0;
	
	float mqtt_volt = 0;
	float mqtt_amp = 0;
	float mqtt_watt = 0;
	uint8_t mqtt_relay1 = 0;
	int16_t client_id = 0;
	
	
	uint8_t mqtt_str[20];
	uint16_t mqtt_str_len = 0;
	
	uint8_t mqtt_user[32] = "bongkot";
	//uint8_t mqtt_aio_key[32] = "1823f794ebf44ca5885bf69e31b7c4e8";
	uint8_t mqtt_aio_key[32] = "25353f7437f14595a69bee0f970cf62d";	//new gen
	uint8_t mqtt_client_id[32] = "nx-smart-device";
	uint8_t serv_ip[4] = {(uint8_t)192,(uint8_t)168,(uint8_t)10,(uint8_t)106};
	
	osStatus os_state;
	
	
	if(network.protocol==NET_MQTT_PTC){
		if(network.module==NET_WIFI){

			wifi_data_flush();
			osDelay(100);
			ret = wifi_debug(W_DEBUG_MODE);
			if(ret == 0){
				return ret;
			}
			
			//dns use
			ret = wifi_dns((uint8_t)0x01);
			if(ret == 0){
				return ret;
			}
			//wifi_dns((uint8_t)0x00);
			//wifi_connect_ap((const char*)"BK-Wifi",(const char*)"pass123456");
			ret = wifi_connect_ap((const char*)"@Next-Wifi-2G",(const char*)"mestnics");
			if(ret == 0){
				return ret;
			}
			//wifi_connect((const char*)"www.google.com",80);
			//wifi_connect((const char*)"io.adafruit.com",1883);
			ret = wifi_connect((const char*)"io.adafruit.com",1883);
			if(ret == 0){
				return ret;
			}
			
			ret = wifi_connect_serv();
			print_debug_semph(0,"[NET]connect with server(%d).\n",ret);
			
			//wifi_connect_ip(serv_ip,5000);	
			while(network.enable == NET_ENABLE){
				if(wifi_connected()){
					print_debug_semph(0,"[NET]wifi connectd with server.\n");
					if(mqtt_connected()!=MQTT_CONNECTED){
						if(mqtt_connect(mqtt_user, 7, mqtt_aio_key, 32, mqtt_client_id, 15)){
							print_debug_semph(0,"[NET]MQTT connnect is OK.\n");
						}else{
							print_debug_semph(0,"[NET]MQTT connnect is fail.\n");
						}
					}else{
						print_debug_semph(0,"[NET]MQTT connnected to host.\n");
//						ret = mqtt_ping();
//						if(ret == 0){
//							break;
//						}
						//osDelay(100);
						//mqtt_callback();
						
						//////////////// Subscribe Area////////////////
						if(mqtt_connected() == MQTT_CONNECTED){
							mqtt_subscribe((uint8_t*)"bongkot/feeds/relay_1", (uint16_t)21, (uint8_t)MQTT_QOS1);
							osDelay(1000);
							mqtt_callback();
							mqtt_subscribe((uint8_t*)"bongkot/feeds/relay_1", (uint16_t)21, (uint8_t)MQTT_QOS1);
							osDelay(1000);
							mqtt_callback();
							
							mqtt_subscribe((uint8_t*)"bongkot/feeds/pwm", (uint16_t)17, (uint8_t)MQTT_QOS1);
							osDelay(1000);
							mqtt_callback();
							mqtt_subscribe((uint8_t*)"bongkot/feeds/pwm", (uint16_t)17, (uint8_t)MQTT_QOS1);
							osDelay(1000);
							mqtt_callback();
						}
						
						
						//wifi_data_flush();
						while(mqtt_connected() == MQTT_CONNECTED){
							if(que_gw_to_ethHandle!=NULL){
								if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
									q_ret = xQueueReceive(que_gw_to_ethHandle, (client_dv_t*)&queue_eth_data, 2000);
									osSemaphoreRelease(binarySemQueueHandle);
									if (q_ret == pdTRUE) {
										
										print_debug_semph(0,"[INFO]{data report from queue}\n");
										print_debug_semph(0,"client id: ");
										//Print Adr of client
										for(i=0;i<8;i++){
											print_debug_semph(0,"%02X ",queue_eth_data.address[i]);
										}
										print_debug_semph(0,"\n");
										
										sprintf((void*)mqtt_str,"%02X%02X%02X%02X%02X%02X%02X%02X",queue_eth_data.address[0],
											queue_eth_data.address[1],queue_eth_data.address[2],queue_eth_data.address[3],
											queue_eth_data.address[4],queue_eth_data.address[5],queue_eth_data.address[6],
											queue_eth_data.address[7]);
										
										mqtt_str_len = 16;
										print_debug_semph(0,"[NET]Client ID(%s).\n",mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/client_id", (uint16_t)23, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
										
										for(i=0;i<4;i++){
											print_debug_semph(0,"[INFO] Data ID : Sensor %d \n",queue_eth_data.data_set[i].data_id);
											print_debug_semph(0,"[INFO] Data Type :  0x%x \n",queue_eth_data.data_set[i].data_type);
											print_debug_semph(0,"[INFO] Value :  %0.3f \n",queue_eth_data.data_set[i].value);
																					
										}
										
										sprintf((void*)mqtt_str,"%3.2f",queue_eth_data.data_set[0].value);
										mqtt_str_len = 6;
										print_debug_semph(0,"[NET]Volt(%f), Str(%s).\n",mqtt_volt,mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
										
										sprintf((void*)mqtt_str,"%2.3f",queue_eth_data.data_set[1].value);
										mqtt_str_len = 6;
										print_debug_semph(0,"[NET]Amp(%f), Str(%s).\n",mqtt_amp,mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
										
										sprintf((void*)mqtt_str,"%4.2f",queue_eth_data.data_set[2].value);
										mqtt_str_len = 7;
										print_debug_semph(0,"[NET]Watt(%f), Str(%s).\n",mqtt_watt,mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/watt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
										
										sprintf((void*)mqtt_str,"%4.2f",queue_eth_data.data_set[3].value);
										mqtt_str_len = 7;
										print_debug_semph(0,"[NET]Watt/Hr(%f), Str(%s).\n",mqtt_watt,mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/watt_phr", (uint16_t)22, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
									}
									
								}
							}
								
							
							//////////////////// Status Report ///////////////////
							
							if(q_gw_stat_to_ethHandle!=NULL){
								if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
									q_ret = xQueueReceive(q_gw_stat_to_ethHandle, (status_dv_t*)&queue_eth_status, 2000);
									if (q_ret == pdTRUE) {
										
										print_debug_semph(0,"[INFO]{status report from queue}\n");
										print_debug_semph(0,"client id: ");
										//Print Adr of client
										for(i=0;i<8;i++){
											print_debug_semph(0,"%02X ",queue_eth_status.address[i]);
										}
										print_debug_semph(0,"\n");
										
										sprintf((void*)mqtt_str,"%02X%02X%02X%02X%02X%02X%02X%02X",queue_eth_status.address[0],
											queue_eth_status.address[1],queue_eth_status.address[2],queue_eth_status.address[3],
											queue_eth_status.address[4],queue_eth_status.address[5],queue_eth_status.address[6],
											queue_eth_status.address[7]);
										
										mqtt_str_len = 16;
										print_debug_semph(0,"[NET]Client ID(%s).\n",mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/client_id", (uint16_t)23, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
										
										
										print_debug_semph(0,"[INFO] Relay Status : %02X \n",queue_eth_status.status_set.dat_switch);
										print_debug_semph(0,"[INFO] Amp :  %0.3f A \n",queue_eth_status.status_set.dat_volt);
										print_debug_semph(0,"[INFO] Value :  %0.3f V \n",queue_eth_status.status_set.dat_amp);
										
										
	//									sprintf((void*)mqtt_str,"%3.2f",queue_eth_status.status_set.dat_volt);
	//									mqtt_str_len = 6;
	//									print_debug_semph(0,"[NET]Volt(%f), Str(%s).\n",mqtt_volt,mqtt_str);
	//									mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
	//									osDelay(500);
	//									mqtt_callback();
	//									
	//									sprintf((void*)mqtt_str,"%2.3f",queue_eth_status.status_set.dat_amp);
	//									mqtt_str_len = 6;
	//									print_debug_semph(0,"[NET]Amp(%f), Str(%s).\n",mqtt_amp,mqtt_str);
	//									mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
	//									osDelay(500);
	//									mqtt_callback();
										
										
										//Relay Status
										print_debug_semph(0,"[NET]Relay Status(%02X).\n",(uint8_t)queue_eth_status.status_set.dat_switch);
										mqtt_relay1 = (uint8_t)queue_eth_status.status_set.dat_switch;
										if(mqtt_relay1 == 1){
											mqtt_relay1 = 1;
											sprintf((void*)mqtt_str,"ON");
											mqtt_str_len = 2;
										}else if(mqtt_relay1 == 0){
											mqtt_relay1 = 0;
											sprintf((void*)mqtt_str,"OFF");
											mqtt_str_len = 3;
										}
										print_debug_semph(0,"[NET]Relay(%d), Str(%s).\n",mqtt_relay1,mqtt_str);
										mqtt_publish((uint8_t*)"bongkot/feeds/relay_status", (uint16_t)26, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
										osDelay(500);
										mqtt_callback();
										
									}
									osSemaphoreRelease(binarySemQueueHandle);
								}
							}	
								
							
							
							mqtt_callback();
							osDelay(100);
							
							//ping every 15 sec
							if((++t_count) > 150){
								
								ret = mqtt_ping();
								
								///////////////Publish Area //////////////////
								
								
								
								
//								client_id = (rand() % 10000);
//								sprintf((void*)mqtt_str,"%06d",client_id);
//								mqtt_str_len = 6;
//								print_debug_semph(0,"[NET]Client ID(%06d), Str(%s).\n",client_id,mqtt_str);
//								//mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)"250.60", (uint16_t)6, (uint8_t)MQTT_QOS1, 0);
//								mqtt_publish((uint8_t*)"bongkot/feeds/client_id", (uint16_t)23, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//										
//								//Volt
//								do{
//									mqtt_volt = (float)((rand() % 240) + ((rand() % 90)*0.1));
//								}while(mqtt_volt < 210.0f);
//								sprintf((void*)mqtt_str,"%3.2f",mqtt_volt);
//								mqtt_str_len = 6;
//								
//								print_debug_semph(0,"[NET]Volt(%f), Str(%s).\n",mqtt_volt,mqtt_str);
//								//mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)"250.60", (uint16_t)6, (uint8_t)MQTT_QOS1, 0);
//								mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//								
//								//Amp
//								do{
//									mqtt_amp = (float)((rand() % 10) + ((rand() % 90)*0.1));
//								}while(mqtt_volt < 0.9f);
//								sprintf((void*)mqtt_str,"%2.2f",mqtt_amp);
//								mqtt_str_len = 5;
//								print_debug_semph(0,"[NET]Amp(%f), Str(%s).\n",mqtt_amp,mqtt_str);
//								//mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)"7.45", (uint16_t)4, (uint8_t)MQTT_QOS1, 0);
//								mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//								
//								//Watt
//								mqtt_watt = mqtt_volt*mqtt_amp;
//								sprintf((void*)mqtt_str,"%4.2f",mqtt_watt);
//								mqtt_str_len = 7;
//								print_debug_semph(0,"[NET]Watt(%f), Str(%s).\n",mqtt_watt,mqtt_str);
//								mqtt_publish((uint8_t*)"bongkot/feeds/watt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
								
								
								t_count = 0;
								if(ret == 0){
									break;
								}
							}
						}
						
					}
				}else{
					print_debug_semph(0,"[NET]wifi dose't connected with server.\n");
					ret = wifi_connect_serv();
					print_debug_semph(0,"[NET]connect with server(%d).\n",ret);
				}
			}
		}
	}else if(network.protocol==NET_HTTP_PTC){
	
	}else if(network.protocol==NET_TCP_PTC){
	
	}else if(network.protocol==NET_UDP_PTC){
	
	}
}

//int8_t net_connect(){
//	int8_t ret = 0;
//	
//	uint8_t t_count = 0;
//	
//	float mqtt_volt = 0;
//	float mqtt_amp = 0;
//	float mqtt_watt = 0;
//	uint8_t mqtt_relay1 = 0;
//	int16_t client_id = 0;
//	
//	
//	uint8_t mqtt_str[20];
//	uint16_t mqtt_str_len = 0;
//	
//	uint8_t mqtt_user[32] = "bongkot";
//	uint8_t mqtt_aio_key[32] = "1823f794ebf44ca5885bf69e31b7c4e8";
//	uint8_t mqtt_client_id[32] = "nx-smart-device";
//	uint8_t serv_ip[4] = {(uint8_t)192,(uint8_t)168,(uint8_t)10,(uint8_t)106};
//	if(network.protocol==NET_MQTT_PTC){
//		if(network.module==NET_WIFI){
//			
////			ret = wifi_clear_mem(W_CLR_EEP);
////			if(ret == 0){
////				return ret;
////			}
//			wifi_data_flush();
//			osDelay(100);
//			ret = wifi_debug(W_DEBUG_MODE);
//			if(ret == 0){
//				return ret;
//			}
//			ret = wifi_dns((uint8_t)0x01);
//			if(ret == 0){
//				return ret;
//			}
//			//wifi_dns((uint8_t)0x00);
//			//wifi_connect_ap((const char*)"BK-Wifi",(const char*)"pass123456");
//			ret = wifi_connect_ap((const char*)"@Next-Wifi-2G",(const char*)"mestnics");
//			if(ret == 0){
//				return ret;
//			}
//			//wifi_connect((const char*)"www.google.com",80);
//			//wifi_connect((const char*)"io.adafruit.com",1883);
//			ret = wifi_connect((const char*)"io.adafruit.com",1883);
//			if(ret == 0){
//				return ret;
//			}
//			
//			ret = wifi_connect_serv();
//			print_debug_semph(0,"[NET]connect with server(%d).\n",ret);
//			
//			//wifi_connect_ip(serv_ip,5000);	
//			while(network.enable == NET_ENABLE){
//				if(wifi_connected()){
//					print_debug_semph(0,"[NET]wifi connectd with server.\n");
//					if(mqtt_connected()!=MQTT_CONNECTED){
//						if(mqtt_connect(mqtt_user, 7, mqtt_aio_key, 32, mqtt_client_id, 15)){
//							print_debug_semph(0,"[NET]MQTT connnect is OK.\n");
//						}else{
//							print_debug_semph(0,"[NET]MQTT connnect is fail.\n");
//						}
//					}else{
//						print_debug_semph(0,"[NET]MQTT connnected to host.\n");
////						ret = mqtt_ping();
////						if(ret == 0){
////							break;
////						}
//						//osDelay(100);
//						//mqtt_callback();
//						
//						
//						///////////////Publish Area //////////////////
//						client_id = (rand() % 10000);
//						sprintf((void*)mqtt_str,"%06d",client_id);
//						mqtt_str_len = 6;
//						print_debug_semph(0,"[NET]Client ID(%06d), Str(%s).\n",client_id,mqtt_str);
//						//mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)"250.60", (uint16_t)6, (uint8_t)MQTT_QOS1, 0);
//						mqtt_publish((uint8_t*)"bongkot/feeds/client_id", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//						osDelay(1000);
//						mqtt_callback();
//						
//						//Volt
//						do{
//							mqtt_volt = (float)((rand() % 240) + ((rand() % 90)*0.1));
//						}while(mqtt_volt < 200.0f);
//						sprintf((void*)mqtt_str,"%3.2f",mqtt_volt);
//						mqtt_str_len = 6;
//						
//						print_debug_semph(0,"[NET]Volt(%f), Str(%s).\n",mqtt_volt,mqtt_str);
//						//mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)"250.60", (uint16_t)6, (uint8_t)MQTT_QOS1, 0);
//						mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//						osDelay(1000);
//						mqtt_callback();
//						
//						//Amp
//						do{
//							mqtt_amp = (float)((rand() % 10) + ((rand() % 90)*0.1));
//						}while(mqtt_volt < 0.9f);
//						sprintf((void*)mqtt_str,"%2.2f",mqtt_amp);
//						mqtt_str_len = 5;
//						print_debug_semph(0,"[NET]Amp(%f), Str(%s).\n",mqtt_amp,mqtt_str);
//						//mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)"7.45", (uint16_t)4, (uint8_t)MQTT_QOS1, 0);
//						mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//						osDelay(1000);
//						mqtt_callback();
//						
//						//Watt
//						mqtt_watt = mqtt_volt*mqtt_amp;
//						sprintf((void*)mqtt_str,"%4.2f",mqtt_watt);
//						mqtt_str_len = 7;
//						print_debug_semph(0,"[NET]Watt(%f), Str(%s).\n",mqtt_watt,mqtt_str);
//						mqtt_publish((uint8_t*)"bongkot/feeds/watt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//						osDelay(1000);
//						mqtt_callback();
//						
////						//Relay1
////						if(mqtt_relay1 == 0){
////							mqtt_relay1 = 1;
////							sprintf((void*)mqtt_str,"ON");
////							mqtt_str_len = 2;
////						}else if(mqtt_relay1 == 1){
////							mqtt_relay1 = 0;
////							sprintf((void*)mqtt_str,"OFF");
////							mqtt_str_len = 3;
////						}
////						print_debug_semph(0,"[NET]Relay(%d), Str(%s).\n",mqtt_relay1,mqtt_str);
////						//mqtt_publish((uint8_t*)"bongkot/feeds/relay1", (uint16_t)20, (uint8_t*)"ON", (uint16_t)2, (uint8_t)MQTT_QOS1, 0);
////						mqtt_publish((uint8_t*)"bongkot/feeds/relay1", (uint16_t)20, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
////						
////						osDelay(1000);
////						mqtt_callback();
//						
//						
//						//////////////// Subscribe Area////////////////
//						
//						mqtt_subscribe((uint8_t*)"bongkot/feeds/relay1", (uint16_t)20, (uint8_t)MQTT_QOS1);
//						osDelay(1000);
//						mqtt_callback();
//						mqtt_subscribe((uint8_t*)"bongkot/feeds/relay1", (uint16_t)20, (uint8_t)MQTT_QOS1);
//						osDelay(1000);
//						mqtt_callback();
//						
//						mqtt_subscribe((uint8_t*)"bongkot/feeds/pwm", (uint16_t)17, (uint8_t)MQTT_QOS1);
//						osDelay(1000);
//						mqtt_callback();
//						mqtt_subscribe((uint8_t*)"bongkot/feeds/pwm", (uint16_t)17, (uint8_t)MQTT_QOS1);
//						osDelay(1000);
//						mqtt_callback();
//						
//						//wifi_data_flush();
//						while(mqtt_connected() == MQTT_CONNECTED){
//							
//								
//							mqtt_callback();
//							osDelay(100);
//							
//							//ping every 15 sec
//							if((++t_count) > 150){
//								
//								ret = mqtt_ping();
//								
//								///////////////Publish Area //////////////////
//								client_id = (rand() % 10000);
//								sprintf((void*)mqtt_str,"%06d",client_id);
//								mqtt_str_len = 6;
//								print_debug_semph(0,"[NET]Client ID(%06d), Str(%s).\n",client_id,mqtt_str);
//								//mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)"250.60", (uint16_t)6, (uint8_t)MQTT_QOS1, 0);
//								mqtt_publish((uint8_t*)"bongkot/feeds/client_id", (uint16_t)23, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//										
//								//Volt
//								do{
//									mqtt_volt = (float)((rand() % 240) + ((rand() % 90)*0.1));
//								}while(mqtt_volt < 210.0f);
//								sprintf((void*)mqtt_str,"%3.2f",mqtt_volt);
//								mqtt_str_len = 6;
//								
//								print_debug_semph(0,"[NET]Volt(%f), Str(%s).\n",mqtt_volt,mqtt_str);
//								//mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)"250.60", (uint16_t)6, (uint8_t)MQTT_QOS1, 0);
//								mqtt_publish((uint8_t*)"bongkot/feeds/volt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//								
//								//Amp
//								do{
//									mqtt_amp = (float)((rand() % 10) + ((rand() % 90)*0.1));
//								}while(mqtt_volt < 0.9f);
//								sprintf((void*)mqtt_str,"%2.2f",mqtt_amp);
//								mqtt_str_len = 5;
//								print_debug_semph(0,"[NET]Amp(%f), Str(%s).\n",mqtt_amp,mqtt_str);
//								//mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)"7.45", (uint16_t)4, (uint8_t)MQTT_QOS1, 0);
//								mqtt_publish((uint8_t*)"bongkot/feeds/amp", (uint16_t)17, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//								
//								//Watt
//								mqtt_watt = mqtt_volt*mqtt_amp;
//								sprintf((void*)mqtt_str,"%4.2f",mqtt_watt);
//								mqtt_str_len = 7;
//								print_debug_semph(0,"[NET]Watt(%f), Str(%s).\n",mqtt_watt,mqtt_str);
//								mqtt_publish((uint8_t*)"bongkot/feeds/watt", (uint16_t)18, (uint8_t*)mqtt_str, (uint16_t)mqtt_str_len, (uint8_t)MQTT_QOS1, 0);
//								osDelay(1000);
//								mqtt_callback();
//								
//								
//								t_count = 0;
//								if(ret == 0){
//									break;
//								}
//							}
//						}
//						
//					}
//					//mqtt_callback();
//					//mqtt_loop();
//					
////					osDelay(1000);
////					net_callback();
//				}else{
//					print_debug_semph(0,"[NET]wifi dose't connected with server.\n");
//					ret = wifi_connect_serv();
//					print_debug_semph(0,"[NET]connect with server(%d).\n",ret);
//					
//				}
//			}
//		}
//	}else if(network.protocol==NET_HTTP_PTC){
//	
//	}else if(network.protocol==NET_TCP_PTC){
//	
//	}else if(network.protocol==NET_UDP_PTC){
//	
//	}
//}

void net_callback(){
	uint8_t ret = 0;
	
	
	if(network.protocol==NET_MQTT_PTC){
		
		//ret = mqtt_receive();
	}
//	data_eth.length = 0;
//	wifi_read(data_eth.value,&data_eth.length);
//	
//	if(data_eth.length > 0){
//		print_payload(0,data_eth.value,data_eth.length);
//		wifi_api_parser(data_eth.value,data_eth.length);
//		ret = 1;
//	}
}

void net_mqtt_callback(uint8_t *topic_val, uint16_t topic_len, uint8_t *payload_val, uint16_t payload_len, uint16_t packet_id){
	osStatus os_state;
	ctl_dv_t ctl_dv;
	uint8_t q_ret = 0;
	uint8_t i = 0;
	int8_t ret = 0;
	char *token = NULL;
	char buff;
	
	print_debug_semph(0,"[INFO]Network Topic Len(%d)\n",topic_len);
	print_payload(0,topic_val,topic_len);
	
	print_debug_semph(0,"[INFO]Network Payload Len(%d)\n",payload_len);
	print_payload(0,payload_val,payload_len);
	
	
	////////////////// Topic Parser ////////////////
	token = strstr((char *)&topic_val[0],"relay_");
	if(token==NULL){
		print_debug_semph(0,"[ERROR]{CB}Topic Packet Parser is fail1.\n");
		ret = 0;
		return;
	}else{
		print_debug_semph(0,"topic token %s\n",token);
	}
	
	token = strstr((char *)&topic_val[0],"_");
	if(token==NULL){
		print_debug_semph(0,"[ERROR]{CB}Topic Packet Parser is fail1.\n");
		ret = 0;
		return;
	}else{
		print_debug_semph(0,"id %s\n",token);
		buff = (uint8_t)(*(token+1));
		ret = convert_char2uint(buff, &ctl_dv.dv_id);
		if(ret!=1){
			print_debug_semph(0,"[ERROR]{CB}Convert ID is error.\n");
			ret = 0;
			return;
		}
		print_debug_semph(0,"ID %d\n",ctl_dv.dv_id);
		
		if(strncmp((char*)payload_val,"ON",2) == 0){//0:the contents of both string are equal
			print_debug_semph(0,"Remote Status: ON\n");
			ctl_dv.value = (uint8_t)0x01;
			if(q_control_end_dvHandle!=NULL){
				if((os_state=osSemaphoreWait(binarySemLocalTaskQueueHandle,2000))==0){
					if(xQueueSend(q_control_end_dvHandle,&ctl_dv,1000)==pdTRUE){
						print_debug_semph(0,"[INFO]{Queue}q_control_end_dvHandle can send.\n");	
					}
					osSemaphoreRelease(binarySemLocalTaskQueueHandle);
				}						
			}
			
		}else if(strncmp((char*)payload_val,"OFF",3) == 0){//0:the contents of both string are equal
			print_debug_semph(0,"Remote Status: OFF\n");
			ctl_dv.value = (uint8_t)0x00;
			if(q_control_end_dvHandle!=NULL){
				if((os_state=osSemaphoreWait(binarySemLocalTaskQueueHandle,2000))==0){
					if(xQueueSend(q_control_end_dvHandle,&ctl_dv,1000)==pdTRUE){
						print_debug_semph(0,"[INFO]{Queue}q_control_end_dvHandle can send.\n");	
					}
					osSemaphoreRelease(binarySemLocalTaskQueueHandle);
				}						
			}
		}
		
		
	}
	
//	if(strncmp((char*)topic_val,"relay/1",7) == 0){//0:the contents of both string are equal
//		if(strncmp((char*)payload_val,"1",2) == 0){
//			ret = convert_char2uint(&payload_val[0], &ctl_dv.dv_id);
//			if(ret!=1){
//				print_debug_semph(0,"[ERROR]{BT}Convert ID is error.\n");
//				ret = 0;
//			}else if(ret == 1){
//				print_debug_semph(0,"ID %d\n",ctl_dv.dv_id);
//			}
//			
//		}else if(strncmp((char*)payload_val,"OFF",3) == 0){
//			//Queue
//			if(q_control_end_dvHandle!=NULL){
//				if((os_state=osSemaphoreWait(binarySemLocalTaskQueueHandle,2000))==0){
//					if(xQueueSend(q_control_end_dvHandle,&ctl_dv,1000)==pdTRUE){
//						print_debug_semph(0,"[IONFO]{Queue}q_control_end_dvHandle can send.\n");	
//					}
//					osSemaphoreRelease(binarySemLocalTaskQueueHandle);
//				}						
//			}
//		}
//	}
//	if(q_control_end_dvHandle!=NULL){
//		if((os_state=osSemaphoreWait(binarySemLocalTaskQueueHandle,2000))==0){
////			q_ret = xQueueReceive(q_control_end_dvHandle, (ctl_dv_t*)&ctl_dv, 2000);
////			osSemaphoreRelease(binarySemLocalTaskQueueHandle);
////			
////			if (q_ret == pdTRUE) {
////				print_debug_semph(0,"[INFO]{control device from queue}\n");
////				print_debug_semph(0,"client id: ");
////				//Print Adr of client
////				for(i=0;i<8;i++){
////					print_debug_semph(0,"%02X ",map_dv[ctl_dv.dv_id-1].dv_adr[i]);
////				}
////				print_debug_semph(0,"\n");
////				
////				print_debug_semph(0,"[INFO] Control Relay(%d)\n",ctl_dv.dv_id);
////				if(ctl_dv.value == (uint8_t)0x00){
////					print_debug_semph(0,"Status: OFF\n");
////					local_control_transmit(map_dv[ctl_dv.dv_id-1].dv_adr,channel, low);
////				}else if(ctl_dv.value == (uint8_t)0x01){
////					print_debug_semph(0,"Status: ON\n");
////					local_control_transmit(map_dv[ctl_dv.dv_id-1].dv_adr,channel, high);
////				}
////			}
//		}
//	}
}
