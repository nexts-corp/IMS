#include "xbee.h"
#include "print_debug.h"
#include "gw_ptc.h"



/*
xbee_api_proc std. => xbee api process
*/
void xbee_api_proc(){

}

void xbee_at_cmd(){

}

int8_t xbee_read(uint8_t *value, uint16_t *len){
	int8_t ret = 0;
	ret = uart3_read(value,len,255);
//	if(len > 0){
//		HAL_UART_Transmit(&huart3,value,*len,10);
//	}
	
	return ret;
}

int8_t xbee_write(uint8_t *value, uint16_t len){
	int8_t ret = 0;
	//print_payload(0,value,len);
	ret = uart3_write(value, len);
	return ret;
}

/* ================================================================================= */ 
// Select Hadler Function                       
int xbee_processPacket(uint8_t *buf, int len) {                       

    int8_t xbee_frame_type;   
    int res, i;  
    
    xbee_frame_type = buf[0];
	
    
    switch(xbee_frame_type) { 
        case ZRPKT:{                                                                     // ZigBee Receive packet  
            print_debug_semph(0,"[INFO]ZRPKT(%02X)\n", (int8_t)ZRPKT);     
						
						if(len < 12) {                                  
                // shoudn't reach here since checksum valid
                return -1;
            }   

						res = gw_ims_ptc(&buf[0], len);
						//res = gw_proc_zig_receiv((char *)&buf[0], len);                                         // TNT_COMM_PROTOCOL PACKET
            if (res < 0){   return -1;  }                 
            
            break; 
        
				}                    
        default:{                      
            print_debug_semph(0,"\r\nUnknown XBee Frame Type ( %02x )!!\r\n", xbee_frame_type);                
            return -1;
            break;  
        }
    }     
    return res;        
}
