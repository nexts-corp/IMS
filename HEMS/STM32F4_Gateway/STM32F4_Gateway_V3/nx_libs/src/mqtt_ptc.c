#include "mqtt_ptc.h"
#include "network.h"
#include "wifi.h"

extern network_t network;
mqtt_t mqtt_data_rx;
//data_t data_mqtt;
uint8_t mqtt_buf[256];
uint16_t mqtt_rx_len = 0;
uint8_t mqtt_buf_rx[256];
uint8_t wifi_buf[256];
uint16_t mqtt_msg_id = 0;
uint16_t mqtt_sub_buf[256];
uint16_t mqtt_sub_len = 0;

int8_t mqtt_host_connected = MQTT_CONNECT_FAILED;

int8_t mqtt_connect(uint8_t *user_val, uint16_t user_len, uint8_t *pwd_val, uint16_t pwd_len, uint8_t *client_id_val, uint16_t client_id_len){

	char *token;
	uint16_t receiv_len = 0;
	uint8_t i = 0;
	int8_t ret = 0;
	uint16_t idx_w = 0;
	char * will_Topic = NULL;
	//
	// Leave room in the buffer for header and variable length field
  uint16_t length = 2;
	
	//unsigned int length = 0;
	uint8_t v;	
	
#if MQTT_VERSION == MQTT_VERSION_3_1
            char d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            char d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif

	memset(mqtt_buf,0,sizeof(mqtt_buf));
	

						
	mqtt_buf[idx_w++] = 0x10;
	idx_w += 1;//reserv len 1 byte
	for (i = 0;i<MQTT_HEADER_VERSION_LENGTH;i++) {
			mqtt_buf[idx_w++] = d[i];
	}
	
	//MQTT_Conncect->will_topic = 0x00; //test null data
	if (will_Topic) {
			v = 0x06|(MQTT_QOS1 << 3)|(MQTT_RETAIN <<5);
	} else {
			v = 0x02;
	}
	
	if(user_len>0) {
			v = v|0x80;

			if(pwd_len>0) {
					v = v|(0x80>>1);
			}
			//v = v&0xFD;
	}
	v = 0xC2;
	
	mqtt_buf[idx_w++] = v;
	mqtt_buf[idx_w++] = ((MQTT_KEEPALIVE) >> 8);
	mqtt_buf[idx_w++] = ((MQTT_KEEPALIVE) & 0xFF);
	
	//Client ID Len
	mqtt_buf[idx_w++] = ((client_id_len) >> 8);
	mqtt_buf[idx_w++] = ((client_id_len) & 0xFF);
	
	//Client ID
	memcpy(&mqtt_buf[idx_w],client_id_val,client_id_len);
	idx_w += client_id_len;
	
	
	//User name len
	mqtt_buf[idx_w++] = ((user_len) >> 8);
	mqtt_buf[idx_w++] = ((user_len) & 0xFF);
	//User Name
	memcpy(&mqtt_buf[idx_w],user_val,user_len);
	idx_w += user_len;
	
	//Password len
	mqtt_buf[idx_w++] = ((pwd_len) >> 8);
	mqtt_buf[idx_w++] = ((pwd_len) & 0xFF);
	//Password
	memcpy(&mqtt_buf[idx_w],pwd_val,pwd_len);
	idx_w += pwd_len;
	
	mqtt_buf[1] = (idx_w - 2);
	
	
	print_debug_semph(0,"[INFO]MQTT Packet Len(%d):\r\n",idx_w);
	print_payload(0,(const unsigned char*)mqtt_buf, idx_w);
	print_debug_semph(0,"\n");
	
	
	if(network.module == NET_WIFI){
		wifi_transmit(mqtt_buf,idx_w);
		
	}
	osDelay(3000);
	if(mqtt_receive(mqtt_buf,&mqtt_rx_len)){
		if(mqtt_rx_len > 0){
			print_payload(0,mqtt_buf, mqtt_rx_len);
			ret = mqtt_parser(mqtt_buf, mqtt_rx_len);
			if(ret == 1){
				mqtt_host_connected = MQTT_CONNECTED;
			}else{
				mqtt_host_connected = MQTT_CONNECT_FAILED;
			}
			
		}else{
			mqtt_host_connected = MQTT_CONNECT_FAILED;
			ret = 0;
			return ret;
		}
	}
	
	
	
//	
//	
//	switch(MQTT_Client.selectModule){
//		case NET_GSM_MODULE:{
//				gsm_return = GSM_UC20.open_tcp();
//			 printDebug("[MQTT_INFO]Connecting(0X%02x).....\r\n",gsm_return);
//				if(((uint8_t)gsm_return)==((uint8_t)GSM_TCP_OPENED)){//GSM_TCP_OPENED
//					printDebug("[MQTT_INFO]88888888888888888888888888.\r\n");
//					
//					
//					printDebug("[INFO]MQTT Packet Len(%d)---------:\r\n",MQTT_Pkt_Tx.length);
//	print_payload((const unsigned char*)MQTT_Pkt_Tx.value, MQTT_Pkt_Tx.length);
//	printDebug("\r\n");
//					
//					//if(GSM_UC20.write((const uint8_t *)MQTT_Pkt_Tx.value, MQTT_Pkt_Tx.length)==GSM_OK){
//						GSM_UC20.write((const uint8_t *)MQTT_Pkt_Tx.value, MQTT_Pkt_Tx.length);
//						mqtt_return = GSM_Read_Loop(MQTT_Pkt_Rx.value,&MQTT_Pkt_Rx.length);
//						
//						if(MQTT_Pkt_Rx.length>0){
//							if((MQTT_Pkt_Rx.value[0]&0xF0)==0x20){
//								if(MQTT_Pkt_Rx.value[1]==0x02){
//									if((MQTT_Pkt_Rx.value[2]==0x00) && (MQTT_Pkt_Rx.value[3]==0x00)){
//										printDebug("[MQTT_INFO]Connect is OK.\r\n");
//										MQTT_Client.connectStatus = MQTT_CONNECTED;
//										return MQTT_OK;
//									}else{
//										MQTT_Client.connectStatus = MQTT_DISCONNECTED;
//										return MQTT_ERROR;
//									}
//								}else{
//									MQTT_Client.connectStatus = MQTT_DISCONNECTED;
//									return MQTT_ERROR;
//								}
//							}else{
//								MQTT_Client.connectStatus = MQTT_DISCONNECTED;
//								return MQTT_ERROR;
//							}
//						}else{
//							MQTT_Client.connectStatus = MQTT_DISCONNECTED;
//							return MQTT_ERROR;
//						}
//				}else if((gsm_return==GSM_TCP_CLOSED) || (gsm_return==GSM_TCP_CLOSED_ERROR)){
//					//GSM_UC20.open_tcp()==GSM_TCP_OPENED
//					printDebug("[MQTT_INFO]Connect is Close(GSM is fail).\r\n");
//					MQTT_Client.connectStatus = MQTT_NOT_CONNECT;
//					return MQTT_Client.connectStatus;
//				}
//			break;
//		}
//		break;
//	}
	return ret;
}

int8_t mqtt_disconnect(){
	int8_t ret;
	
	
	return ret;
}

int8_t mqtt_connected(){

	return mqtt_host_connected;
}

int8_t mqtt_ping(){
	int8_t ret;
	uint16_t idx_w = 0;
	uint16_t receiv_len = 0;
	
	mqtt_buf[idx_w++] = (uint8_t)MQTT_PINGREQ;
	mqtt_buf[idx_w++] = (uint8_t)0x00;
	
	print_debug_semph(0,"[MQTT]PING Packet (Len=%d):\r\n",idx_w);
	print_payload(0,(const unsigned char*)mqtt_buf, idx_w);
	print_debug_semph(0,"\n");
	
	
	if(network.module == NET_WIFI){
		wifi_transmit(mqtt_buf,idx_w);
	}
	osDelay(1000);
//	if(mqtt_receive(mqtt_buf,&receiv_len)){
//		if(receiv_len > 0){
//			print_payload(0,mqtt_buf, receiv_len);
//			
//			
//			
//			ret = 1;
//			mqtt_host_connected = MQTT_CONNECTED;
//		}else{
//			mqtt_host_connected = MQTT_CONNECT_FAILED;
//			ret = 0;
//			return ret;
//		}
//	}
	ret = mqtt_callback();
	if(ret == 1){
		mqtt_host_connected = MQTT_CONNECTED;
		print_debug_semph(0,"[MQTT]PING is OK\n");
	}else{
		mqtt_host_connected = MQTT_CONNECT_FAILED;
		print_debug_semph(0,"[MQTT]PING is fail\n");
	}
	return ret;
}

int8_t mqtt_read(){
	int8_t ret;
	
	return ret;
}

int8_t mqtt_write(){
	int8_t ret;
	
	return ret;
}

int8_t mqtt_publish(uint8_t *topic_value, uint16_t topic_len, uint8_t *payload_value, uint16_t payload_len, uint8_t qos, uint8_t retain){
	int8_t ret;
	uint16_t idx_w = 0;
	
	mqtt_buf[idx_w++] = MQTT_PUBLISH|qos;
	//Retain
	if (retain) {
		 mqtt_buf[0] |= 1;
	}
	
	//Length Remain
	mqtt_buf[idx_w++] = 0;
	
	//Topic len
	mqtt_buf[idx_w++] = ((topic_len) >> 8);
	mqtt_buf[idx_w++] = ((topic_len) & 0xFF);
	
	
	//Topic value
	memcpy(&mqtt_buf[idx_w],topic_value,topic_len);
	idx_w += topic_len;
	
	//Message ID or Packet ID
	mqtt_buf[idx_w++] = ((mqtt_msg_id) >> 8);
	mqtt_buf[idx_w++] = ((mqtt_msg_id) & 0xFF);
	mqtt_msg_id++;
	if(mqtt_msg_id>=10000){
		 mqtt_msg_id = 0;
	}
	
	//Payload
	memcpy(&mqtt_buf[idx_w],payload_value,payload_len);
	idx_w += payload_len;
	
	//Update Length Remaining
	mqtt_buf[1] = (idx_w - 2);
	
	print_debug_semph(0,"[INFO]MQTT Publish Packet Len(%d):\n",idx_w);
	print_payload(0,(const unsigned char*)mqtt_buf, idx_w);
	print_debug_semph(0,"\n");
	
	
	if(network.module == NET_WIFI){
		wifi_transmit(mqtt_buf,idx_w);
		
	}
//	osDelay(1000);
//	if(mqtt_receive(mqtt_buf_rx,&mqtt_rx_len)){
//		if(mqtt_rx_len > 0){
//			//There are data
//			ret = 1;
//			print_payload(0,mqtt_buf_rx, mqtt_rx_len);
//		}else{
//			//There are not data
//			mqtt_host_connected = MQTT_CONNECT_FAILED;
//			ret = 0;
//		}
//	}
	ret = 1;
	return ret;
}

int8_t mqtt_subscribe(uint8_t *topic_value, uint16_t topic_len, uint8_t qos){
	int8_t ret;
	uint16_t idx_w = 0;
	
	
	//Control Packet
	mqtt_buf[idx_w++] = MQTT_SUBSCRIBE|0x02;
		
	mqtt_buf[idx_w++] = 0;//Remaining Length
		
	//Message ID or Packet ID
	mqtt_buf[idx_w++] = ((mqtt_msg_id) >> 8);
	mqtt_buf[idx_w++] = ((mqtt_msg_id) & 0xFF);
	mqtt_msg_id++;
	if(mqtt_msg_id>=10000){
		 mqtt_msg_id = 0;
	}
	
	//Topic len
	mqtt_buf[idx_w++] = ((topic_len) >> 8);
	mqtt_buf[idx_w++] = ((topic_len) & 0xFF);
	
	
	//Topic value
	memcpy(&mqtt_buf[idx_w],topic_value,topic_len);
	idx_w += topic_len;
	
	//QOS Request
	if(qos == (uint8_t)MQTT_QOS0){
		mqtt_buf[idx_w++] = (uint8_t)0x00;
	}else if(qos == (uint8_t)MQTT_QOS1){
		mqtt_buf[idx_w++] = (uint8_t)0x01;
	}else if(qos == (uint8_t)MQTT_QOS2){
		mqtt_buf[idx_w++] = (uint8_t)0x02;
	}
	
	//Update Length Remaining
	mqtt_buf[1] = (idx_w - 2);
	
	print_debug_semph(0,"[INFO]MQTT Subscribe Packet Len(%d):\n",idx_w);
	print_payload(0,(const unsigned char*)mqtt_buf, idx_w);
	print_debug_semph(0,"\n");
	
	
	if(network.module == NET_WIFI){
		wifi_transmit(mqtt_buf,idx_w);
		
	}
	
	return ret;
}

int8_t mqtt_receive(uint8_t *buf, uint16_t *len){
	int8_t ret = 0;
	uint16_t receive_len = 0;
	uint16_t receive_len2 = 0;
	//uint16_t len = 0;
	//data_mqtt.
	ret = wifi_read(wifi_buf, &receive_len);
	//print_debug_semph(0,"[mqtt_receive]ret(%d).\n",ret);
	if(ret==0){
		return ret;
	}
	
	ret = wifi_api_parser_output(wifi_buf, receive_len, buf, len);
	print_debug_semph(0,"[wifi_api_parser_output]ret(%d).\n",ret);
	memcpy(&receive_len2,len,sizeof(uint16_t));
	print_debug_semph(0,"[INFO]MQTT receive Len(%d).\n",receive_len2);
//	if(receive_len2 > 0){
//		ret = 1;
//		print_payload(0,buf, receive_len2);
//		
//	}
//	
//	if(len>0){
//		ret = 1;
//		print_payload(0,mqtt_buf, len);
//		
//		
//	}
	
	return ret;
}


int8_t mqtt_set_callback(mqtt_cb_t mqtt_cb){
	int8_t ret;
	
	osDelay(5000);
	if(mqtt_receive(mqtt_buf,&mqtt_rx_len)){
		if(mqtt_rx_len > 0){
			print_payload(0,mqtt_buf, mqtt_rx_len);
			ret = 1;
			mqtt_host_connected = MQTT_CONNECTED;
		}else{
			mqtt_host_connected = MQTT_CONNECT_FAILED;
			ret = 0;
			return ret;
		}
	}
	return ret;
}

int8_t mqtt_callback(){
	int8_t ret = 0;
	data_t mqtt_cb_dat;
	osStatus os_state;
	uint8_t q_ret = 0;
	
	if(q_eth_stream_to_parserHandle!=NULL){
		if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
			q_ret = xQueueReceive(q_eth_stream_to_parserHandle, (data_t*)&mqtt_cb_dat, 4000);
			osSemaphoreRelease(binarySemQueueHandle);
			
			if(q_ret==pdTRUE){
				print_debug_semph(0,"[IONFO]{Queue}q_eth_stream_to_parserHandle can receive.\n");	
				print_payload(0,mqtt_cb_dat.value, mqtt_cb_dat.length);
				if((mqtt_cb_dat.length > 0) && (mqtt_cb_dat.length < 400)){
					//mqtt_parser(mqtt_cb_dat.value, mqtt_cb_dat.length);
				}
			}
		}	
		
	}
	
	ret = mqtt_receive(mqtt_buf_rx,&mqtt_rx_len);
	//print_debug_semph(0,"[MQTT]ret(%d).\n",ret);
	if(ret == 1){
		
		if(mqtt_rx_len > 0){
			//There are data
			ret = 1;
			print_debug_semph(0,"[MQTT]mqtt_rx_len > 0.\n");
			print_payload(0,mqtt_buf_rx, mqtt_rx_len);
			mqtt_parser(mqtt_buf_rx, mqtt_rx_len);
		}else{
			print_debug_semph(0,"[MQTT]mqtt_rx_len = 0.\n");
			//There are not data
			ret = 0;
		}
	}
	return ret;
}

int8_t mqtt_parser(uint8_t *val, uint16_t len){
	int8_t ret = 0;
	uint8_t header_control = 0;
	uint8_t len_remain = 0;
	uint8_t ret_code1 = 0;
	uint8_t ret_code2 = 0;
	uint16_t ret_pack_id = 0;
	uint8_t buf_ack[5];
	uint16_t cal_all_len = 0;
	
	
	print_debug_semph(0,"[MQTT_CALLBACK]len(%d)\n",len);
	if(len <= (uint16_t)0){
		print_debug_semph(0,"[MQTT_CALLBACK]error len(%d)\n",len);
		return ret;
	}
	
	
	header_control = (val[0] & 0xF0); 
	print_debug_semph(0,"[MQTT_CALLBACK]header conrtol(%02X)\n",header_control);
	if(header_control == (uint8_t)0x20){
		len_remain = val[1];
		print_debug_semph(0,"[MQTT_CALLBACK]CONACK Len remaining(%d)\n",len_remain);
		if(len_remain==2){
			ret_code1 = val[2];
			ret_code2 = val[3];
			if((ret_code1==0x00) && (ret_code2==0x00) ){
				print_debug_semph(0,"[MQTT_CALLBACK]MQTT Connect OK.\n");
				ret = 1;
			}else{
				print_debug_semph(0,"[MQTT_CALLBACK]MQTT Connect Fail.\n");
				ret = 0;
			}
		}
	}else if(header_control == (uint8_t)0x30){
		len_remain = val[1];
		print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH Len remaining(%d)\n",len_remain);
		mqtt_data_rx.topic.length = (uint16_t)(val[2] << 8) | val[3];
		print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH Topic Len(%d)\n",mqtt_data_rx.topic.length);
		memcpy(mqtt_data_rx.topic.value,&val[4],mqtt_data_rx.topic.length);
		print_payload(0,mqtt_data_rx.topic.value,mqtt_data_rx.topic.length);
		
//		cal_all_len += mqtt_data_rx.topic.length;
//		cal_all_len += (len_remain - mqtt_data_rx.topic.length);//Len of Payload
		
		mqtt_data_rx.payload.length = (len_remain - mqtt_data_rx.topic.length - 2);//2=len position of topic
		print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH Payload Len(%d)\n",mqtt_data_rx.payload.length);
		memcpy(mqtt_data_rx.payload.value,&val[4+mqtt_data_rx.topic.length],mqtt_data_rx.topic.length);
		print_payload(0,mqtt_data_rx.payload.value,mqtt_data_rx.payload.length);
		
		//Check QoS > 0 Only
		if((val[0] & 0x06) == (uint8_t)MQTT_QOS0){
			print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH QoS(%d)\n",0);
			//ret_pack_id = 
		}else if((val[0] & 0x06) == (uint8_t)MQTT_QOS1){
			print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH QoS(%d)\n",1);
			//ret_pack_id = 
		}else if((val[0] & 0x06) == (uint8_t)MQTT_QOS2){
			print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH QoS(%d)\n",2);
			//ret_pack_id = 
		}
		wifi_mqtt_callback(mqtt_data_rx.topic.value, mqtt_data_rx.topic.length, mqtt_data_rx.payload.value, mqtt_data_rx.payload.length, (uint16_t)0);
		ret = 1;
//		if(len_remain == 2){
//			ret_pack_id = (uint16_t)(val[2] << 8) | val[3];
//			print_debug_semph(0,"[MQTT_CALLBACK]PUBLISH Packet ID(%d)\n",ret_pack_id);
//			ret = 1;
//		}else{
//			ret = 0;
//		}
	}else if(header_control == (uint8_t)0x40){
		len_remain = val[1];
		if(len_remain == 2){
			ret_pack_id = (uint16_t)(val[2] << 8) | val[3];
			print_debug_semph(0,"[MQTT_CALLBACK]PUBACK Packet ID(%d)\n",ret_pack_id);
			ret = 1;
		}else{
			ret = 0;
		}
	}else if(header_control == (uint8_t)0x90){
		print_debug_semph(0,"[MQTT_CALLBACK]SUBACK(0x90) Packet ID(%d)\n",ret_pack_id);
		len_remain = val[1];
		if(len_remain == 3){
			ret_pack_id = (uint16_t)(val[2] << 8) | val[3];
			print_debug_semph(0,"[MQTT_CALLBACK]SUBACK Packet ID(%d)\n",ret_pack_id);
			ret_code1 = val[4];
			if((ret_code1 >= 0) || (ret_code1 < 3)){
				ret = 1;
			}else{
				ret = 0;
			}
		}else{
			ret = 0;
		}
	}else if(header_control == (uint8_t)0xC0){
		buf_ack[0] = (uint8_t)0xD0;
		buf_ack[1] = (uint8_t)0x00;
		print_debug_semph(0,"[MQTT_CALLBACK]PINGREQ(0xC0) Packet ID(%d)\n",ret_pack_id);
		wifi_write(buf_ack,2);
		ret = 1;
	}else if(header_control == (uint8_t)0xD0){
		len_remain = val[1];
		if(len_remain == 0){
			print_debug_semph(0,"[MQTT_CALLBACK]PINGRESP(0xD0).\n");
			ret = 1;
		}else{
			ret = 0;
		}
	}
	////
	return ret;
}

int8_t mqtt_loop(){
	int8_t ret = 0;
	
	if(wifi_connected()){
		ret = mqtt_receive(mqtt_buf_rx,&mqtt_rx_len);
		print_debug_semph(0,"[MQTT]ret(%d).\n",ret);
		if(ret == 1){
			
			if(mqtt_rx_len > 0){
				//There are data
				ret = 1;
				print_debug_semph(0,"[MQTT]mqtt_rx_len > 0.\n");
				print_payload(0,mqtt_buf_rx, mqtt_rx_len);
				mqtt_parser(mqtt_buf_rx, mqtt_rx_len);
			}else{
				print_debug_semph(0,"[MQTT]mqtt_rx_len = 0.\n");
				//There are not data
				ret = 0;
			}
		}
		ret = 1;
	}else{
		mqtt_host_connected = MQTT_CONNECT_FAILED;
		ret = 0;
	}
	
	
	return ret;
}