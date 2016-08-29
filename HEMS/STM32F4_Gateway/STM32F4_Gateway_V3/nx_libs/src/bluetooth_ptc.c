#include "bluetooth_ptc.h"


extern osSemaphoreId binarySemQueueHandle;
extern osSemaphoreId binarySemLocalTaskQueueHandle;
extern osMessageQId q_control_end_dvHandle;



uint8_t parser_buff[10];

int8_t bt_parser(uint8_t *val, uint16_t length){
	int8_t ret = 0;
	int8_t i = 0;
	char *token = NULL;
	ctl_dv_t ctl_dv;
	osStatus os_state;
	char buff;
	
	if((length <= 0) || (length >= 25)){
		ret = 0;
		return ret;
	}
	
	
	print_debug_semph(0,"[INFO]{BT}BT_DATA_RECEIV\n");
	
	token = strstr((char *)&val[0],"*");
	if(token==NULL){
		print_debug_semph(0,"[ERROR]{BT}Packet Parser is fail1.\n");
		ret = 0;
		return ret;
	}
	print_debug_semph(0,"token %s\n",token);
	//parser_buff[0] = (uint8_t)(*(token+1));
	
	buff = (uint8_t)(*(token+1));
	ret = convert_char2uint(buff, &ctl_dv.dv_id);
	if(ret!=1){
		print_debug_semph(0,"[ERROR]{BT}Convert ID is error.\n");
		ret = 0;
		return ret;
	}
	print_debug_semph(0,"ID %d\n",ctl_dv.dv_id);
	
	
	token = strstr((char *)(token+1),"/");
	buff = (uint8_t)(*(token+1));
	ret = convert_char2uint(buff, &ctl_dv.value);
	if(ret!=1){
		print_debug_semph(0,"[ERROR]{BT}Convert Value is error.\n");
		ret = 0;
		return ret;
	}
	print_debug_semph(0,"Status %d\n",ctl_dv.value);
	
	
	if(token==NULL){
		print_debug_semph(0,"[ERROR]{BT}Packet Parser is fail2.\n");
		ret = 0;
		return ret;
	}
	
	token = strstr((char *)(token+1),"#");
	if(token==NULL){
		print_debug_semph(0,"[ERROR]{BT}Packet Parser is fail3.\n");
		ret = 0;
		return ret;
	}
	
	print_debug_semph(0,"[INFO]{BT}Packet Parser is complete.\n");
	
	
	//Queue
	if(q_control_end_dvHandle!=NULL){
		//////buffer queue
		print_debug_semph(0,"[IONFO]{Queue}q_control_end_dvHandle have address.\n");	
		if((os_state=osSemaphoreWait(binarySemLocalTaskQueueHandle,2000))==0){
			if(xQueueSend(q_control_end_dvHandle,&ctl_dv,1000)==pdTRUE){
				print_debug_semph(0,"[IONFO]{Queue}q_control_end_dvHandle can send.\n");	
				//print_payload(0,wifi_queue_rx.value, wifi_queue_rx.length);
			}
			osSemaphoreRelease(binarySemLocalTaskQueueHandle);
		}						
	}
	
	return ret;
}

int8_t convert_char2uint(char val , uint8_t *out_val){
	int8_t ret = 0;
	uint8_t i = 0;
	char num_hex = (char)0x30;
	uint8_t num_dec = 0;
	
	print_debug_semph(0,"[INFO]{BT}val %c.\n",val);
	for(i=0;i<10;i++){
		if(val == num_hex++){
			memcpy(out_val,&i,1);
			
			
			//print_debug_semph(0,"[INFO]{BT}out_val %d, i %d.\n",(*out_val),i);
			ret = 1;
			return ret;
		}
	}
	
	ret = 0;
	return ret;
}