#include "local_task.h"
#include "gw_task.h"
#include "bluetooth.h"
#include "uart1_manag.h"
#include "bluetooth_ptc.h"
#include "print_debug.h"

extern osSemaphoreId binarySemQueueHandle;
extern osSemaphoreId binarySemLocalTaskQueueHandle;
extern osMessageQId q_control_end_dvHandle;

extern data_t data_bt;

map_dv_t map_dv[4] = {
{1,{0x00,0x13,0xA2,0x00,0x40,0x9F,0x39,0x9E}},
{2,{0x00,0x13,0xA2,0x00,0x40,0x9F,0x37,0x0F}},
{3,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
{4,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
};


void local_task(void const * argument){
	uint8_t ret = 0;
	osStatus os_state;
	uint8_t i = 0;
	uint8_t q_ret = 0;
	ctl_dv_t ctl_dv;
	uint8_t channel = (uint8_t)0x20; //channel1 = 0x20
	
	memset(&data_bt,0,sizeof(data_bt));
	print_debug_semph(0,"[INFO]{Task}local_task running...\n");
	for(;;){
		//print_debug_semph(0,"[INFO]{Task}local_task running...\n");
		if((ret = local_receiv(data_bt.value, &data_bt.length))==1){
			//print_payload(0,data_gw_xbee.value,data_gw_xbee.length);
			if(data_bt.length > 0){
				//HAL_UART_Transmit(&huart3,data_gw_xbee.value,data_gw_xbee.length,10);
				print_payload(0,data_bt.value,data_bt.length);
				bt_parser(data_bt.value,data_bt.length);
				data_bt.length = 0;
			}
			
		}
		
		
		if(q_control_end_dvHandle!=NULL){
			if((os_state=osSemaphoreWait(binarySemLocalTaskQueueHandle,2000))==0){
				q_ret = xQueueReceive(q_control_end_dvHandle, (ctl_dv_t*)&ctl_dv, 2000);
				osSemaphoreRelease(binarySemLocalTaskQueueHandle);
				
				if (q_ret == pdTRUE) {
					print_debug_semph(0,"[INFO]{control device from queue}\n");
					print_debug_semph(0,"client id: ");
					//Print Adr of client
					for(i=0;i<8;i++){
						print_debug_semph(0,"%02X ",map_dv[ctl_dv.dv_id-1].dv_adr[i]);
					}
					print_debug_semph(0,"\n");
					
					print_debug_semph(0,"[INFO] Control Relay(%d)\n",ctl_dv.dv_id);
					if(ctl_dv.value == (uint8_t)0x00){
						print_debug_semph(0,"Status: OFF\n");
						local_control_transmit(map_dv[ctl_dv.dv_id-1].dv_adr,channel, low);
					}else if(ctl_dv.value == (uint8_t)0x01){
						print_debug_semph(0,"Status: ON\n");
						local_control_transmit(map_dv[ctl_dv.dv_id-1].dv_adr,channel, high);
					}
				}
			}
		}
			
		osDelay(100);
	}
}

int8_t local_transmit(uint8_t *value,uint16_t len){
	uint8_t ret = 0;
	ret = bt_write(value, len);
	
	return ret;
}

int8_t local_receiv(uint8_t *value,uint16_t *len){
	uint8_t ret = 0;
	ret = bt_read(value, len);
	return ret;
}

int8_t local_control_transmit(uint8_t *adr,uint8_t ch, uint8_t val)
{
	uint8_t data[100];
	int i=0;
	int len=0;
	/*------ Xbee header --------*/
	data[0]=0x7e;
	data[1]=0x00;
	data[3]=0x10;
	data[4]=0x01;
	
	///////// xbee address dest
	for(i=0;i<8;i++)
	{
		data[i+5]= adr[i];
	}
	data[13]=0xff;
	data[14]=0xfe;
	data[15]=0x00;
	data[16]=0x00;
	/*------ End Xbee header --------*/
	len=14;	
	
	////// IMS  Protocol Structure ////////
	data[17]=0x30;
	len++;
	
	///// Sender ID ////////
	for(i=0;i<8;i++)
	{
		data[i+18]= 0x00;
		len++;
	}	
	
	///// Receiver ID ////////
	for(i=0;i<8;i++)
	{
		data[i+26]= adr[i];
		len++;
	}
	
	//// Frame ID
	data[34]=0x30;
	
	//// Trame Type
	data[35]=0x8a;
	
	// Target ID
	for(i=0;i<8;i++)
	{
		data[i+36]= adr[i];
		len++;
	}
	
	data[44]=ch;
	data[45]=val;
	data[46]=0x00;
	data[47]=0x00;
	data[48]=0x00;
	len=len+7;
	data[2]=len;
	
	data[49]=local_xbee_checksum((char*)&data[3],len);
	//data[49]=0xf5;
	//xbee_checksum((char*)&gw_tx_buff[3], idx_w-3);
	//data[41]=0xF0;
	print_payload(0,data,len+4);
	gw_transmit(data,len+4);
	//print_debug_semph(0,"[INFO]Remote To ID: %d \r\n",indexID);
	
}

int local_xbee_checksum(char buf[],int len) {

    int i;
    char sum = 0;                                          
    //print_payload(buf,len);
    for (i = 0; i < len; i++) {
        sum += buf[i];
    }                 
    return (0xff - (sum & 0xff));

}