#include "wifi.h"
#include "network.h"
#include "mqtt_ptc.h"
#include "print_debug.h"

extern osSemaphoreId binarySemQueueHandle;
extern osMessageQId q_eth_stream_to_parserHandle;

extern network_t network;
extern data_t data_eth;
//mqtt_t mqtt_buf;
//data_t data_wifi_buff;
/*
lan_api_proc std. => LAN api process
*/


int8_t wifi_read(uint8_t *value, uint16_t *len){
	int8_t ret = 0;
	ret = uart4_read(value,len,255);
	wifi_data_flush();
//	if(len > 0){
//		HAL_UART_Transmit(&huart3,value,*len,10);
//	}
	
	return ret;
}

int8_t wifi_write(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	ret = uart4_write(value, len);
	return ret;
}

int8_t wifi_transmit(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	
	if(len>0){
		//print_payload(0,(uint8_t*)value,len);
//		memset(buf,0,sizeof(buf));
//		memcpy(buf,(uint8_t*)value,len);
	}else{
		ret = 0;
		return ret;
	}
	
	ret = wifi_api_packer(W_DATA_TRASNSMIT,NULL,value,len);
	if(ret!=1){
		print_debug_semph(0,"W_DATA_TRASNSMIT is fail.\n");
	}
	
	
	
	
//	osDelay(1000);
//	memset(pkt_buf,0,sizeof(pkt_buf));
//	data_eth.length = 0;
//	wifi_read(data_eth.value,&data_eth.length);
//	
//	if(data_eth.length > 0){
//		print_payload(0,data_eth.value,data_eth.length);
//		wifi_api_parser(data_eth.value,data_eth.length);
//		ret = 1;
//	}
	return ret;
}

int8_t wifi_config(uint8_t cmd, uint8_t *value, uint16_t len){
	int8_t ret = 0;
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	
	if(len>0){
//		print_payload(0,(uint8_t*)value,len);
		memset(buf,0,sizeof(buf));
		memcpy(buf,(uint8_t*)value,len);
	}
	
	if(cmd==W_SET_SERV_NAME){
		wifi_api_packer(W_CONFIG_RECEIV,W_SET_SERV_NAME,buf,32);
	}else if(cmd==W_SET_SERV_PORT){
		wifi_api_packer(W_CONFIG_RECEIV,W_SET_SERV_PORT,buf,2);
	}else if(cmd==W_SET_SSID){
		wifi_api_packer(W_CONFIG_RECEIV,W_SET_SSID,buf,32);
	}else if(cmd==W_SET_PWD){
		wifi_api_packer(W_CONFIG_RECEIV,W_SET_PWD,buf,32);
	}else if(cmd==W_GET_CONNECTED_SERV){
		wifi_api_packer(W_CONFIG_RECEIV,W_GET_CONNECTED_SERV,buf,0);
	}else if(cmd==W_DEBUG_MODE){
		wifi_api_packer(W_CONFIG_RECEIV,W_DEBUG_MODE,buf,1);
	}else if(cmd==W_DNS_IP_MODE){
		wifi_api_packer(W_CONFIG_RECEIV,W_DNS_IP_MODE,buf,1);
	}else if(cmd==W_SET_SERV_IP){
		wifi_api_packer(W_CONFIG_RECEIV,W_SET_SERV_IP,buf,4);
	}else if(cmd==W_CLR_EEP){
		wifi_api_packer(W_CONFIG_RECEIV,W_CLR_EEP,NULL,0);
	}else if(cmd==W_SET_CONNECT_SERV){
		wifi_api_packer(W_CONFIG_RECEIV,W_SET_CONNECT_SERV,NULL,0);
		osDelay(2000);
	}
	
	
	
	
	osDelay(500);
	memset(pkt_buf,0,sizeof(pkt_buf));
	data_eth.length = 0;
	wifi_read(data_eth.value,&data_eth.length);
	
	if(data_eth.length > 0){
//		print_payload(0,data_eth.value,data_eth.length);
		ret = wifi_api_parser(data_eth.value,data_eth.length);
//		return ret;
//		ret = 1;
	}else{
		ret = 0;
	}
	return ret;
}

int8_t wifi_debug(uint8_t data){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	uint8_t debug_use = 0x00;
	
	
	ret = wifi_config(data, &debug_use, 1);
	if(ret != 1){
		print_debug_semph(0,"W_DEBUG_USE set error.\n");
		ret = 0;
	}else if(ret == 1){
		print_debug_semph(0,"W_DEBUG_USE set OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_clear_mem(uint8_t data){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	
	
	ret = wifi_config(data, NULL, 1);
	if(ret != 1){
		print_debug_semph(0,"W_CLR_EEP set error.\n");
		ret = 0;
	}else if(ret == 1){
		print_debug_semph(0,"W_CLR_EEP set OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_dns(uint8_t data){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	uint8_t debug_use = 0x01;
	
	if(data == 0x00){
		debug_use = 0x00;	//not use,but ip
		ret = wifi_config(W_DNS_IP_MODE, &debug_use, 1);
	}else if(data == 0x01){
		debug_use = 0x01;	//use
		ret = wifi_config(W_DNS_IP_MODE, &debug_use, 1);
	}
	
	
	
	if(ret != 1){
		print_debug_semph(0,"W_DNS_USE set error.\n");
		ret = 0;
	}else if(ret == 1){
		print_debug_semph(0,"W_DNS_USE set OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_connected(){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	
	
	ret = wifi_config(W_GET_CONNECTED_SERV, (uint8_t*)"", 0);
	if(ret != 1){
		print_debug_semph(0,"W_GET_CONNECTED_SERV is error.\n");
		ret = 0;
	}else if(ret == 1){
		print_debug_semph(0,"W_GET_CONNECTED_SERV is OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_connect_serv(){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	
	
	ret = wifi_config(W_SET_CONNECT_SERV, (uint8_t*)"", 0);
	if(ret != 1){
		print_debug_semph(0,"W_SET_CONNECT_SERV is error.\n");
		ret = 0;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_CONNECT_SERV is OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_connect_ap(const char * ssid,const char * pwd){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint8_t pkt_buf2[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	int8_t ssid_len = strlen(ssid);
	int8_t pwd_len = strlen(pwd);
	
	memset(pkt_buf,0,sizeof(pkt_buf));
	memcpy(pkt_buf,ssid,ssid_len);
	memset(pkt_buf2,0,sizeof(pkt_buf2));
	memcpy(pkt_buf2,pwd,pwd_len);
	
	ret = wifi_config(W_SET_SSID, (uint8_t*)pkt_buf, ssid_len);
	if(ret != 1){
		print_debug_semph(0,"W_SET_SSID set error.\n");
		return ret;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_SSID set OK.\n");
		ret = 1;
	}
	
	osDelay(1000);
	
	
	ret = wifi_config(W_SET_PWD, (uint8_t*)pkt_buf2, pwd_len);
	if(ret != 1){
		print_debug_semph(0,"W_SET_PWD set error.\n");
		return ret;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_PWD set OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_connect(const char * serv_name,uint16_t port){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	
	ret = wifi_config(W_SET_SERV_NAME, (uint8_t*)serv_name, strlen(serv_name));
	if(ret != 1){
		print_debug_semph(0,"W_SET_SERV_NAME set error.\n");
		
		return ret;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_SERV_NAME set OK.\n");
		ret = 1;
	}
	osDelay(1000);
	ret = wifi_config(W_SET_SERV_PORT, (uint8_t*)&port, 2);
	if(ret != 1){
		print_debug_semph(0,"W_SET_SERV_PORT set error.\n");
		return 0;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_SERV_PORT set OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_connect_ip(uint8_t * serv_ip,uint16_t port){
	uint8_t buf[32];
	uint8_t pkt_buf[50];
	uint16_t pkt_len = 0;
	int8_t ret = 0;
	
	ret = wifi_config(W_SET_SERV_IP, (uint8_t*)serv_ip, 4);
	if(ret != 1){
		print_debug_semph(0,"W_SET_SERV_IP set error.\n");
		return ret;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_SERV_IP set OK.\n");
		ret = 1;
	}
	osDelay(1000);
	ret = wifi_config(W_SET_SERV_PORT, (uint8_t*)&port, 2);
	if(ret != 1){
		print_debug_semph(0,"W_SET_SERV_PORT set error.\n");
		return ret;
	}else if(ret == 1){
		print_debug_semph(0,"W_SET_SERV_PORT set OK.\n");
		ret = 1;
	}
	return ret;
}

int8_t wifi_api_packer(uint8_t frame_type, uint8_t cmd, uint8_t *value, uint16_t len){
	int8_t ret = 0;
	//uint8_t pkt_buf[256];
	uint16_t pkt_len = 0;
	uint8_t pkt_chsum = 0;	//packet checksum
	uint16_t idx_w = 0;
	
	//data_eth.value, &data_eth.length
	memset(&data_eth,0,sizeof(data_eth));
	
	
	if(frame_type == W_DATA_TRASNSMIT){
		//Frame Type + Payload
		pkt_len = (1+len);
	}else{
		//Frame Type + CMD + Payload
		pkt_len = (1+1+len);
	}
	
	
	
	data_eth.value[idx_w++] = (uint8_t)0x7E;
	data_eth.value[idx_w++] = (pkt_len >> 8) & 0xFF;
  data_eth.value[idx_w++] = pkt_len & 0xFF;
	//print_payload(0,pkt_buf,idx_w);
	if(frame_type == W_CONFIG_RECEIV){
		data_eth.value[idx_w++] = (uint8_t)W_CONFIG_RECEIV;  //Fram Type
		if(cmd == W_SET_SERV_NAME){
			data_eth.value[idx_w++] = (uint8_t)W_SET_SERV_NAME;     //CMD
		}else if(cmd == W_SET_SERV_PORT){
			data_eth.value[idx_w++] = (uint8_t)W_SET_SERV_PORT;     //CMD
		}else if(cmd == W_SET_SSID){
			data_eth.value[idx_w++] = (uint8_t)W_SET_SSID;     //CMD
		}else if(cmd == W_SET_PWD){
			data_eth.value[idx_w++] = (uint8_t)W_SET_PWD;     //CMD
		}else if(cmd == W_GET_CONNECTED_SERV){
			data_eth.value[idx_w++] = (uint8_t)W_GET_CONNECTED_SERV;     //CMD
		}else if(cmd == W_DEBUG_MODE){
			data_eth.value[idx_w++] = (uint8_t)W_DEBUG_MODE;     //CMD
		}else if(cmd == W_DNS_IP_MODE){
			data_eth.value[idx_w++] = (uint8_t)W_DNS_IP_MODE;     //CMD
		}else if(cmd == W_SET_SERV_IP){
			data_eth.value[idx_w++] = (uint8_t)W_SET_SERV_IP;     //CMD
		}else if(cmd == W_SET_SERV_IP){
			data_eth.value[idx_w++] = (uint8_t)W_CLR_EEP;     //CMD
		}else if(cmd == W_SET_CONNECT_SERV){
			data_eth.value[idx_w++] = (uint8_t)W_SET_CONNECT_SERV;     //CMD
		}
		
		
		
	}else if(frame_type == W_DATA_TRASNSMIT){
		data_eth.value[idx_w++] = (uint8_t)W_DATA_TRASNSMIT;  //Fram Type
	}else{
		print_debug_semph(0,"[ERROR]frame type is unknow(%02X)\n",frame_type);
		return ret;
	}
	
	if(len>0){
		memcpy(&data_eth.value[idx_w],value,len);
		idx_w += len;
	}
	
	
	pkt_chsum = wifi_checksum((char*)&data_eth.value[3],pkt_len);
	print_debug_semph(0,"[INFO]wifi_checksum(%02X)(len:(%d))\n",pkt_chsum,pkt_len);
	print_debug_semph(0,"[INFO]Wifi Send\n");
	//print_payload(0,&data_eth.value[3],pkt_len);
	data_eth.value[idx_w++] = pkt_chsum;
	
	print_payload(0,data_eth.value,idx_w);
	wifi_write(data_eth.value,idx_w);
	
	ret = 1;
	return ret;
}

int8_t wifi_api_parser(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	uint8_t pkt_buf[100];
	uint16_t pkt_len = 0;
	uint8_t pkt_chsum = 0;	//packet checksum
	uint16_t idx_w = 0;
	uint16_t idx_r = 0;
	uint16_t offset = 0;
	int8_t ptc_ret;
	uint16_t size_pkt = 0;
	uint16_t total = 0;
	uint8_t frame_type;
  uint8_t cmd;
	uint8_t resp_val;
	
	do{
    if(value[idx_r] == 0x7e){
      offset = idx_r;
      if(offset==len){
        print_debug_semph(0,"invalid coordinator packet\n");                                   // not found Start Delimiter 0x7E
        ptc_ret = -1;
        return ptc_ret;
      }
      size_pkt = (value[offset+1] & 0xffff) << 8;                                       // check packet length (MSB)
      size_pkt |= value[offset+2];                                                      // check packet length (LSB)
      print_debug_semph(0,"\r\nlen=%d(%04x)\r\n", size_pkt, size_pkt);
      //Serial.print();
      total += size_pkt;                                                                     // update total analyse 
      if (total > len){                                                                //check length validation
          offset++;
          total = offset;                                                               // roll back total analyse                          
          print_debug_semph(0,"\r\ninvalid length!!\r\n");
          continue;
      }                             
        
      if(wifi_checksum((char*)&value[offset+3], size_pkt) != value[offset+3+size_pkt]){        // checksum error detection                  
          print_debug_semph(0,(char*)"checksum error\r\n");
          offset++;
          total = offset;                                                               // roll back total analyse          
      }
      else{           
//				// got a valid packet
//        print_payload(0,&value[offset+3], size_pkt);
        frame_type = value[offset+3]; 
        if(frame_type == W_DATA_RECEIV){
          print_debug_semph(0,"W_DATA_RECEIV\n");
          //send_wifi_data(&value[offset+3], (size_pkt-1));   //send wifi data
        }else if(frame_type == W_CONFIG_RECEIV){
          print_debug_semph(0,"W_CONFIG_RECEIV\n");
          cmd = value[offset+3+1];
          //response_config(cmd, &value[offset+3], size_pkt);
        }else if(frame_type == W_CONFIG_RESP){
          print_debug_semph(0,"W_CONFIG_RESP\n");
          cmd = value[offset+3+1];
					resp_val = value[offset+3+1+1];
					print_debug_semph(0,"res_cmd(%02X), res_value(%02X) is OK.\n",cmd, resp_val);
					if(cmd == W_GET_CONNECTED_SERV){
						if(resp_val == 0x01){	
							//0x01=connected
							return 1;
						}else{
							//0x01=connect error
							return 0;
						}
					}else if((cmd == W_SET_DHCP) || (cmd == W_SET_IP) || (cmd == W_SET_SN) ||(cmd == W_SET_GW) ||
					(cmd == W_SET_MAC) ||(cmd == W_SET_DNS_IP) ||(cmd == W_SET_SERV_NAME) ||(cmd == W_SET_SERV_IP) 
					||(cmd == W_SET_SERV_NAME)||(cmd == W_SET_SERV_PORT)||(cmd == W_SET_SSID)||(cmd == W_SET_PWD)
					||(cmd == W_CLR_EEP)||(cmd == W_DEBUG_MODE)||(cmd == W_DNS_IP_MODE) ||(cmd == W_SET_CONNECT_SERV)){
						
						if(resp_val == 0x00){
							return 1;
						}else{
							return 0;
						}
					}
					
          //response_config(cmd, &value[offset+3], size_pkt);
        }else if(frame_type == W_STATUS){
					cmd = value[offset+3+1];
					print_debug_semph(0,"W_STATUS(%02X).\n",cmd);
					resp_val = value[offset+3+1+1];
          if(cmd == W_CONNECT_SERV){
						if(resp_val == STATUS_CONNECTED_OK){
							print_debug_semph(0,"Connect to server is OK(%02X).\n",resp_val);
							return 1;
						}else if(resp_val == STATUS_CONNECTED_FAIL){
							print_debug_semph(0,"Connect to server is fail(%02X).\n",resp_val);
							return 0;
						}else{
							print_debug_semph(0,"Connect unknow(%02X).\n",resp_val);
							return 0;
						}
					}else{
						return 0;
					}
					
          //response_config(cmd, &value[offset+3], size_pkt);
        }else{
          print_debug_semph(0,"Unknow cmd.\n");
        } 
          //printDebug("checksum correct\r\n");
          //printDebug("\r\nXBee Receive Packet ---\r\n");   
         // print_payload(0,&value[offset+3], size_pkt);  
          //printDebug("-------\r\n");        
          //xbee_processPacket(&recvPacket[offset+3], len);                               // analyse API-specific Structure 
        //xbee_processPacket((uint8_t*)&value[offset+3], size_pkt);
        offset += 3+len;
      }  
    }
    if(idx_r==len){
      break;
    }
    
  }while(value[idx_r++] != 0x7e);
	return ret;
}
int8_t wifi_api_parser_output(uint8_t *value, uint16_t len, uint8_t *val_out, uint16_t *len_out){
	int8_t ret = 0;
	uint8_t pkt_buf[100];
	uint16_t pkt_len = 0;
	uint8_t pkt_chsum = 0;	//packet checksum
	uint16_t idx_w = 0;
	uint16_t idx_r = 0;
	uint16_t offset = 0;
	int8_t ptc_ret;
	uint16_t size_pkt = 0;
	uint16_t total = 0;
	uint8_t frame_type;
  uint8_t cmd;
	uint16_t size_out = 0;
	uint8_t status_val;
	data_t wifi_queue_rx;
	
	osStatus os_state;
	
	do{
    if(value[idx_r] == 0x7e){
      offset = idx_r;
      if(offset==len){
        print_debug_semph(0,"invalid coordinator packet\n");                                   // not found Start Delimiter 0x7E
        ptc_ret = -1;
        return ptc_ret;
      }
      size_pkt = (value[offset+1] & 0xffff) << 8;                                       // check packet length (MSB)
      size_pkt |= value[offset+2];                                                      // check packet length (LSB)
      print_debug_semph(0,"\r\nlen=%d(%04x)\r\n", size_pkt, size_pkt);
      //Serial.print();
      total += size_pkt;                                                                     // update total analyse 
      if (total > len){                                                                //check length validation
          offset++;
          total = offset;                                                               // roll back total analyse                          
          print_debug_semph(0,"\r\ninvalid length!!\r\n");
          continue;
      }                             
        
      if(wifi_checksum((char*)&value[offset+3], size_pkt) != value[offset+3+size_pkt]){        // checksum error detection                  
          print_debug_semph(0,(char*)"checksum error\r\n");
          offset++;
          total = offset;                                                               // roll back total analyse          
      }
      else{                                                                             // got a valid packet
//        print_payload(0,&value[offset+3], size_pkt);
        frame_type = value[offset+3]; 
				memcpy(val_out,&value[offset+3+1],size_pkt-1);
				size_out = size_pkt-1;
				memcpy(len_out,&size_out,sizeof(uint16_t));
				
				
				
        if(frame_type == W_DATA_RECEIV){
          print_debug_semph(0,"W_DATA_RECEIV\n");
					////Queue
					if(q_eth_stream_to_parserHandle!=NULL){
						//////buffer queue
						memcpy(wifi_queue_rx.value,&value[offset+3+1],size_pkt-1);
						wifi_queue_rx.length = size_pkt-1;
						
						if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
							if(xQueueSend(q_eth_stream_to_parserHandle,&wifi_queue_rx,1000)==pdTRUE){
								print_debug_semph(0,"[IONFO]{Queue}q_eth_stream_to_parserHandle can send.\n");	
								print_payload(0,wifi_queue_rx.value, wifi_queue_rx.length);
							}
							osSemaphoreRelease(binarySemQueueHandle);
						}						
					}
					
					return 1;
          //send_wifi_data(&value[offset+3], (size_pkt-1));   //send wifi data
        }else if(frame_type == W_CONFIG_RECEIV){
          print_debug_semph(0,"W_CONFIG_RECEIV\n");
          cmd = value[offset+3+1];
					return 1;
          //response_config(cmd, &value[offset+3], size_pkt);
        }else if(frame_type == W_CONFIG_RESP){
          print_debug_semph(0,"W_CONFIG_RESP\n");
          cmd = value[offset+3+1];
					if(value[offset+3+1+1] == 0x00){
						print_debug_semph(0,"W_CONFIG_RESP->cmd(%02X) is OK.\n",cmd);
						return 1;
					}else{
						return 0;
					}
          //response_config(cmd, &value[offset+3], size_pkt);
        }else if(frame_type == W_STATUS){
					cmd = value[offset+3+1];
					print_debug_semph(0,"W_STATUS(%02X).\n",cmd);
					status_val = value[offset+3+1+1];
          if(cmd == W_CONNECT_SERV){
						if(status_val == STATUS_CONNECTED_OK){
							print_debug_semph(0,"Connect to server is OK(%02X).\n",status_val);
							return 1;
						}else if(status_val == STATUS_CONNECTED_FAIL){
							print_debug_semph(0,"Connect to server is fail(%02X).\n",status_val);
							return 0;
						}else{
							print_debug_semph(0,"Connect unknow(%02X).\n",status_val);
							return 0;
						}
					}else{
						return 0;
					}
					
          //response_config(cmd, &value[offset+3], size_pkt);
        }else{
          print_debug_semph(0,"Unknow cmd.\n");
        } 
          //printDebug("checksum correct\r\n");
          //printDebug("\r\nXBee Receive Packet ---\r\n");   
         // print_payload(0,&value[offset+3], size_pkt);  
          //printDebug("-------\r\n");        
          //xbee_processPacket(&recvPacket[offset+3], len);                               // analyse API-specific Structure 
        //xbee_processPacket((uint8_t*)&value[offset+3], size_pkt);
        offset += 3+len;
      }  
    }
    if(idx_r==len){
      break;
    }
    
  }while(value[idx_r++] != 0x7e);
	return ret;
}

uint8_t  wifi_checksum(char *buf, uint16_t len){
  int i = 0;
  char sum = 0;                                          
  //print_payload(buf,len);
  for (i = 0; i < len; i++) {
      sum += buf[i];
  }                 
  return (0xff - (sum & 0xff));
}

uint8_t  wifi_checksum2_buf(char *buf1, uint16_t len1, char *buf2, uint16_t len2){
  int i = 0;
  char sum = 0;                                          
  //print_payload(buf,len);
  for (i = 0; i < len1; i++) {
      sum += buf1[i];
  } 
	for (i = 0; i < len2; i++) {
      sum += buf2[i];
  }  
  return (0xff - (sum & 0xff));
}

void wifi_mqtt_callback(uint8_t *topic_val, uint16_t topic_len, uint8_t *payload_val, uint16_t payload_len, uint16_t packet_id){
//	print_debug_semph(0,"[INFO]Topic Len(%d)\n",topic_len);
//	print_payload(0,topic_val,topic_len);
//	
//	print_debug_semph(0,"[INFO]Payload Len(%d)\n",payload_len);
//	print_payload(0,payload_val,payload_len);
	
	net_mqtt_callback(topic_val, topic_len, payload_val, payload_len, packet_id);
	//wifi_data_flush();
}

void wifi_data_flush(){
	uart4_flush();
	//print_debug_semph(0,"Wifi Flush Buffer.\n");
}