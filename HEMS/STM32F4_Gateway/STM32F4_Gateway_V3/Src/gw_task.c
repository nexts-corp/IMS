#include "gw_task.h"
#include "xbee.h"
#include "uart3_manag.h"
#include "gw_ptc.h"

extern recv_uart_t recv_uart_3;
extern data_t data_gw_xbee; 

extern osMessageQId que_gw_to_ethHandle;

extern osSemaphoreId binarySemQueueHandle;

data_t mail;

//uint8_t ID[]={0x00,0x13,0xa2,0x00,0x40,0x9f,0x3a,0x2d};
uint16_t IDtest =1;
tableID table[20];
int numID = 0;
int CS = 0;
/* edv_manag_task function std. => End Device Manage

*/
void gw_task(void const * argument)
{
  /* USER CODE BEGIN edv_manag_task */
	uint8_t ret = 0;
	osStatus os_state;
	uint8_t count = 0;
	
	
	print_debug_semph(0,"[INFO]gw_task is running ...\n");
	print_debug_semph(0,"[INFO]Module XBee\n");
	memset(&recv_uart_3,0,sizeof(recv_uart_t));
	memset(&data_gw_xbee,0,sizeof(data_t));
	
  /* Infinite loop */
  for(;;)
  {
//		if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
//			
//			
//			osSemaphoreRelease(binarySemQueueHandle);
//		}
//		if(++count>200){
//			count = 0;
//		}
//		sprintf((void*)mail.value,"Hello(%03d)",count);
//		mail.length = 10;
//			if(xQueueSend(que_gw_to_ethHandle,&mail,1000)==pdTRUE){
//				print_debug_semph(0,"[IONFO]{Queue}can send.\n");	
//			}
		
		
//		os_state = osMessagePut(que_gw_to_ethHandle,(uint32_t)&mail,200);
//		print_debug_semph(0,"[IONFO]{Queue}os_state(%02X).\n",os_state);	
		
		
		
//		memcpy(data_gw_xbee.value,(const void*)"Hi XBee.\n",9);
//		data_gw_xbee.length = 9;
//		gw_transmit(data_gw_xbee.value,data_gw_xbee.length);
		//osDelay(500);
		if((ret = gw_receiv(data_gw_xbee.value, &data_gw_xbee.length))==1){
			//print_payload(0,data_gw_xbee.value,data_gw_xbee.length);
			if(data_gw_xbee.length > 0){
				//HAL_UART_Transmit(&huart3,data_gw_xbee.value,data_gw_xbee.length,10);
				print_payload(0,data_gw_xbee.value,data_gw_xbee.length);
				gw_Getaddress(data_gw_xbee.value);
				gw_proc_xbee_api(data_gw_xbee.value,data_gw_xbee.length);
				data_gw_xbee.length = 0;
			}
			
		}
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==0)
		{
			osDelay(20);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==0)
			{
				if(CS==0)
				{
					gw_remote(1,ch1,low);
					CS++;
				}
				else if(CS==1)
				{
					gw_remote(1,ch1,high);
					CS--;
				}
				osDelay(100);
				
			}
		}
		
    osDelay(100);
		//gw_remote(ID,ch1,high);
  }
  /* USER CODE END rf_task */
}


int8_t gw_transmit(uint8_t *value,uint16_t len){
	uint8_t ret = 0;
	
	//print_payload(0,value,len);
	ret = xbee_write(value, len);
	
	return ret;
}

int8_t gw_receiv(uint8_t *value,uint16_t *len){
	uint8_t ret = 0;
	ret = xbee_read(value, len);
	return ret;
}

int8_t gw_remote(uint16_t indexID,char ch,char value)
{
	uint8_t data[100];
	int i=0;
	int len=0;
	/*------ Xbee header --------*/
	data[0]=0x7e;
	data[1]=0x00;
	data[3]=0x10;
	data[4]=0x01;
	for(i=0;i<8;i++)
	{
		data[i+5]= table[indexID].address[i];
	}
	data[13]=0xff;
	data[14]=0xfe;
	data[15]=0x00;
	data[16]=0x00;
	/*------ End Xbee header --------*/
	len=14;	
	data[17]=0x30;
	len++;
	for(i=0;i<8;i++)
	{
		data[i+18]= 0x00;
		len++;
	}	
	for(i=0;i<8;i++)
	{
		data[i+26]= table[indexID].address[i];
		len++;
	}
	data[34]=0x30;
	data[35]=0x8a;
	for(i=0;i<8;i++)
	{
		data[i+36]= table[indexID].address[i];
		len++;
	}
	
	data[44]=ch;
	data[45]=value;
	data[46]=0x00;
	data[47]=0x00;
	data[48]=0x00;
	len=len+7;
	data[2]=len;
	
	data[49]=xbee_checksum((char*)&data[3],len);
	//data[49]=0xf5;
	//xbee_checksum((char*)&gw_tx_buff[3], idx_w-3);
	//data[41]=0xF0;
	print_payload(0,data,len+4);
	gw_transmit(data,len+4);
	print_debug_semph(0,"[INFO]Remote To ID: %d \r\n",indexID);
	
}

uint8_t gw_Getaddress(uint8_t* dataAll)
{
	uint8_t CID[8];
	int i=0;
	int j=0;
	if(numID<20)
	{
			if(dataAll[3]==0x90)
			{
					for(i=0;i<8;i++)
					{
						CID[i]=dataAll[i+4];
					}
					for(i=0;i<20;i++)
					{
						for(j=0;j<8;j++)
						{
							if(CID[j]!=table[i].address[j])
							{
								break;
							}
						}
						if(j==8)
						{
							break;
						}
							
					}
					if(i==20)
					{
						numID++;
						table[numID].index=numID;
						memcpy(table[numID].address,CID,8);
						print_debug_semph(0,"[INFO]No. ID : %d \r\n",table[numID].index);
						print_payload(0,CID,8);
					}

			}
			
	}
			
}

