#include "gw_ptc.h"
#include "gw_task.h"
#include "xbee.h"
#include "print_debug.h"
#include "conf_def.h"

extern osMessageQId que_gw_to_ethHandle;
extern osMessageQId q_gw_stat_to_ethHandle;

// Sender/Receiver Address put in INT_COMM_PROTOCOL Heder
char RECEIVER_ADDRESS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char SENDER_ADDRESS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  
DATASET Read_DataReport[4];


client_tb_t client_tb;
client_stat_tb_t client_stat_tb;

data_t gw_send_buf;

uint16_t gw_packet_len = 0;
uint16_t idx_send_w = 0;

void gw_proc_xbee_api(uint8_t *value, uint16_t len){
	char *ret = NULL;
	uint16_t len_buff;
	int ptc_ret;
	uint8_t frame_type;
	int offset = 0;
	int size_pkt = 0x0000;
	int total = 0;
	
	do{
		offset += strcspn((const char*)&value[offset], "\x7E");
		if(offset==len){
			print_debug_semph(0,"invalid coordinator packet");                                   // not found Start Delimiter 0x7E
		}
		size_pkt = (value[offset+1] & 0xffff) << 8;                                       // check packet length (MSB)
		size_pkt |= value[offset+2];                                                      // check packet length (LSB)
		print_debug_semph(0,"\r\nlen=%d(%04x)\r\n", size_pkt, size_pkt);
		total += size_pkt;                                                                     // update total analyse 
		if (total > len){                                                                //check length validation
				offset++;
				total = offset;                                                               // roll back total analyse                          
				print_debug_semph(0,"\r\nXBEE> invalid length!!\r\n");
				continue;
		}                             
			
		if(xbee_checksum((char*)&value[offset+3], size_pkt) != value[offset+3+size_pkt]){        // checksum error detection                  
				print_debug_semph(0,"XBEE> checksum error\r\n");
				offset++;
				total = offset;                                                               // roll back total analyse          
		}
		else{                                                                             // got a valid packet 
				//printDebug("checksum correct\r\n");
				//printDebug("\r\nXBee Receive Packet ---\r\n");   
				print_payload(0,&value[offset+3], size_pkt);  
				//printDebug("-------\r\n");        
				//xbee_processPacket(&recvPacket[offset+3], len);                               // analyse API-specific Structure 
			xbee_processPacket((uint8_t*)&value[offset+3], size_pkt);
			offset += 3+len;
		}  
	}while(offset<len);
	//ret = strstr((const char*)value,(const char*)0x7e);
	//ret = strtok((char*)value,(const char*)0x7e);
	
	
	
	
//	offset += strcspn((const char*)&value[offset], "\x7E");
//	if(offset==len){
//		print_debug_semph(0,"invalid coordinator packet");                                   // not found Start Delimiter 0x7E
//	}
//	size_pkt = (value[offset+1] & 0xffff) << 8;                                       // check packet length (MSB)
//	size_pkt |= value[offset+2];                                                      // check packet length (LSB)
//	print_debug_semph(0,"\r\nlen=%d(%04x)\r\n", size_pkt, size_pkt);
//	total += size_pkt;                                                                     // update total analyse 
//	if (total > len){                                                                //check length validation
//			offset++;
//			total = offset;                                                               // roll back total analyse                          
//			print_debug_semph(0,"\r\nXBEE> invalid length!!\r\n");
//			//continue;
//	}                             
//		
//	if(xbee_checksum((char*)&value[offset+3], size_pkt) != value[offset+3+size_pkt]){        // checksum error detection                  
//			print_debug_semph(0,"XBEE> checksum error\r\n");
//			offset++;
//			total = offset;                                                               // roll back total analyse          
//	}
//	else{                                                                             // got a valid packet 
//			//printDebug("checksum correct\r\n");
//			//printDebug("\r\nXBee Receive Packet ---\r\n");   
//			print_payload(0,&value[offset+3], size_pkt);  
//			//printDebug("-------\r\n");        
//			//xbee_processPacket(&recvPacket[offset+3], len);                               // analyse API-specific Structure 
//		xbee_processPacket((uint8_t*)&value[offset+3], size_pkt);
//		offset += 3+len;
//	}  
	
	
	
	
//	
//	
//	memcpy(&len_buff,(ret+2),sizeof(uint16_t));
//	print_debug_semph(0,"len_buff(%d, %04X)\n",len_buff,len_buff);
//	
//	if(len_buff>0){
//		ptc_ret = xbee_checksum((char *)ret+4,len_buff);
//		print_debug_semph(0,"[INFO]xbee_checksum(%d, %02X)\n",ptc_ret,ptc_ret);
//	}
//	
//	
//	//Frame Type
//	frame_type = *(ret+4);
//	gw_proc_pkt(frame_type,(uint8_t *)(ret+3),len_buff);
//	
//	//SID
//	print_debug_semph(0,"[INFO]Sender ID\n");
//	print_payload(0,&value[4],8);
//	
//	
	//
	
	//strstr();
}

void gw_proc_pkt(uint8_t frame_type,uint8_t *value, uint16_t len){
	if(frame_type==0x90){
		print_debug_semph(0,"[INFO]Frame Type(90)=> xbee receive packet\n");
	}
}

void gw_ping_ack(){
	
}

int xbee_checksum(char buf[],int len) {

    int i;
    char sum = 0;                                          
    //print_payload(buf,len);
    for (i = 0; i < len; i++) {
        sum += buf[i];
    }                 
    return (0xff - (sum & 0xff));

}

int gw_ims_ptc(uint8_t *value,uint16_t len){
	int8_t ims_cmd;
	int8_t ims_frame_type;
	int8_t ret;
	
	ims_frame_type = value[30];
	ims_cmd = value[31];
	print_debug_semph(0,"[INFO]IMS-Frame Type(%02X)\n", ims_frame_type); 
	print_debug_semph(0,"[INFO]CMD(%02X)\n", ims_cmd); 
	
	if(ims_frame_type==NETMAINC){
		//End device req join
		if(ims_cmd==JOINREQ){
			//gw_Getaddress(value);
//			ret=gw_add_edv(&value[32]);
//			if(ret > 0){
//				print_debug_semph(0,"[INFO]S/N add\n");
//				print_payload(0,xbee_table.xbee_dev[ret-1].end_sn,8);
//			}else if(ret == (-2)){
//				print_debug_semph(0,"[INFO]S/N added\n");
//			}else{
//				print_debug_semph(0,"[INFO]End Device Full.\n");
//			}
			
			//send
			//gw_join_ack(xbee_table.xbee_dev[ret-1].end_sn);
			
			//gw_join_ack(xbee_table.xbee_dev[ret-1].end_sn);
			print_debug_semph(0,"[INFO]Join Req(%02X)\n", ims_cmd); 
			gw_join_ack(&value[32],len-32);
			//gw_transmit(data_gw_xbee.value,data_gw_xbee.length);
		}
		else if(ims_cmd==HEARTBEAT)
		{	
			gw_Heart_ack(value);
		}
	}
	else if(ims_frame_type==DATAREPC)
	{
		//gw_Read_DataReport(value);
		gw_dat_report_parser(value);
	}
	else if(ims_frame_type==STATREPC)
	{
		//gw_Read_Status(value);
		gw_status_parser(value);
	}
	else if(ims_frame_type==EVENREPC)
	{
		gw_event_parser(value);
	}
	else if(ims_frame_type==(int8_t)RMDCTRLA)	
	{
		//0x8B
		//gw_event_parser(value);
		gw_remote_parser(value);
	}
	
	
	
	
	
	
	
	
	
	return 0;
}

int gw_add_edv(uint8_t *sn){
	int i=0,j=0;
	int ret = 0;
	if(xbee_table.dev_count==0){
		memcpy(xbee_table.xbee_dev[xbee_table.dev_count++].end_sn,&sn[0],8);
		return xbee_table.dev_count;
	}else if(xbee_table.dev_count<=2){
		ret = gw_chk_dup_edv(sn);
		if(ret==(-1)){
			memcpy(xbee_table.xbee_dev[xbee_table.dev_count++].end_sn,&sn[0],8);
			return xbee_table.dev_count;
		}else if(ret == 1){
			print_debug_semph(0,"[INFO]End Device S/N Dup.\n");
			return -2;
		}
	}else{
		return -3;//Device member full
	}
	
	return -1; //
}

int gw_chk_dup_edv(uint8_t *sn){
	int i=0,j=0;
	int ret = 0;
	for(i=0;i<2;i++){
		ret = 0;
		for(j=0;j<8;j++){
			if(xbee_table.xbee_dev[i].end_sn[j] == sn[j]){
				++ret;
			}
		}
		if(ret==8){
			return 1; //dup
		}
	}
	return -1;
}

int gw_select_edv(uint8_t *sn){
	int i=0,j=0;
	int ret = 0;
	for(i=0;i<2;i++){
		ret = 0;
		for(j=0;j<8;j++){
			if(xbee_table.xbee_dev[i].end_sn[j] == sn[j]){
				++ret;
				if(ret==8){
					return i;
				}
			}
		}
	}
	return -1;
}

/* ================================================================================= */
/*************************************************************************************/
/************************** Process INT_COMM_PROTOCAL PACKET *************************/
/*************************************************************************************/
/* ================================================================================= */ 
int gw_proc_zig_receiv(char *buf, int len) {

    // Zigbee Protocol Variable
    unsigned char srcAddr[9];
    unsigned int srcNetAddr = 0x0000;
    //unsigned char recvOption;
    unsigned int dataLen;           // TNT_COMM_PROTOCOL PACKET Length
    
    // INT_COMM_PROTOCOL Variable
    //unsigned char protoVersion;
    unsigned char senderAddr[8];
    unsigned char recvAddr[8];
    unsigned char frameType;
    unsigned char frameID;
    int8_t res;
    // - Status Request Cmd
    //unsigned int interval;
    //unsigned char accm;
    //unsigned char listID;
    // - Remote Device Control
    char command;
    
    // Define Zigbee Protocol Variable
    memcpy(srcAddr, &buf[1], 8); 
    srcNetAddr  = (buf[9]&0xFFFF) << 8;                   
    srcNetAddr |= buf[10];  
    //recvOption  = buf[11];
      
    // Define INT_COMM_PROTOCOL Variable
    memcpy(senderAddr, &buf[13], 8);        // Sender Address in Header of INT_COMM_PROTOCAL
    memcpy(recvAddr, &buf[21], 8);          // Receiver Address in Header of INT_COMM_PROTOCAL
    //protoVersion    = buf[12];
    frameID         = buf[29]; 
    frameType       = buf[30];
    
    // Define data length of INT_COMM_PROTOCOL PACKET
    dataLen = len-1-8-2-1;              // Data Lenght = Len- FrameType- SrcAddr- SrcNetAddr- ReceiveOption     Byte.
    
//    //----- Save address of coordinator in first times ----//
//    if(!_flag_saveRecvAddr) {
//        _flag_saveRecvAddr = 1;
//        memcpy(_receiveAddress, senderAddr, 8);
//        //printDebug("SAVED RECEIVER_ADDRESS\r\n");
//    }
     
     switch(frameType) { 
        /*------------------ Command Packet ------------------*/
        case STATREQC :                                                       // -- 0x1A Status Request COMMAND 
            if(dataLen < 30){                                    
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }  
            print_debug_semph(0,">>> STATUS_REQUEST_CMD : 0x%X\r\n", STATREQC);
            //accm = buf[39];
            //interval = (buf[40]&0xFFFF) << 8;
            //interval |= buf[41];
            //listID = accm & 0x0F;
            
            
            break;
        case NETMAINC :                                                       // -- 0x7A Network Maintenance COMMAND
            if(dataLen < 20){                                    
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }  
            print_debug_semph(0,">>> NETWORK_MAINTENANCE_CMD : 0x%X\r\n", NETMAINC);
						
            break;
        
        case STATREPA :                                                        // -- 0x2B Status Report ACKNOWLEDGE
            if(dataLen < 23){                                    
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }  
            print_debug_semph(0,">>> STATUS_REPORT_ACK : 0x%X\r\n", STATREPA);
            //INT_removeWaitQueue(frameID);
            //INT_setTimeSync(&buf[31]);
            break;
        case EVENREPA :                                                        // -- 0x3B Event Report ACKNOWLEDGE
            if(dataLen < 19){                                    
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }  
            print_debug_semph(0,">>> EVENT_REPORT_ACK : 0x%X\r\n", EVENREPA);
            //INT_removeWaitQueue(frameID);
            break;
        case DATAREPA :                                                        // -- 0x5B Data Report ACKNOWLEDGE
            if(dataLen < 19){                                    
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }  
            print_debug_semph(0,">>> DATA_REPORT_ACK : 0x%X\r\n", DATAREPA);
            //INT_removeWaitQueue(frameID);
            break;  
        case NETMAINA :                                                        // -- 0x7B Network Maintenance ACKNOWLEDGE       
            if(dataLen < 21){            
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }        
            print_debug_semph(0,">>> NETWORK_MAINTENANCE_ACK : 0x%X\r\n", NETMAINA);
            if(buf[31] == HEARTBEAT || buf[31] == JOINREQ) {
                if(buf[32] == 0x00) {            // Ack OK
                    //memcpy(&RECEIVER_ADDRESS[0], &senderAddr[0], 8);
                    //printDebug("SAVED RECEIVER_ADDRESS\r\n");    
                }
            }
            
            //res = funcProcessNETMAINA(&buf[29], dataLen);
            if(res < 0) {
                return -1;
            }                                 
            break;
        case PROTOERROR :                                                      // -- 0xEB Protocol Error Feedback        
            if(dataLen < 20){            
                // shoudn't reach here since checksum valid
                gw_sendProtocolErrorFeedbackAck(frameID, INVALID_LEN);
                return -1;
            }        
            print_debug_semph(0,"\r\n>>> PROTOCOL_ERROR_FEEDBACK_ACK : FRAME ID[0x%02X] ERROR ID[0x%02X]\r\n\r\n", frameID, buf[31] );                                 
            break;
        
        default:                      
            print_debug_semph(0,"\r\n>>> RECEIVE INT_COMM> Frame Type not define yet (0x%02X)!!\r\n\r\n", frameType);
            gw_sendProtocolErrorFeedbackAck(frameID, INVALID_FTYPE);                
            return -1;
            break;
     }
                       
    /*printDebug("ZigBee Receive Packet\r\n");
    printDebug("Source Addr : "); 
    for(i = 1; i < 9; i++) {
          printDebug("%02X ", buf[i]);            
    }    
    printDebug("\r\nNetwork Addr =(%04X)\r\n", srcNetAddr);    
    printDebug("Receive Optiobs =(%02X)\r\n", recvOption);
    printDebug("Data : ");  
    for(i = 0; i < dataLen; i++) {
          printDebug("%02X ", data[i]);            
    }  
    printDebug("\n");*/ 
    return 0;     

}

int gw_sendProtocolErrorFeedbackAck(char frameID, char errorID) {

    char frameData[255];
    int8_t res;
    int frameLen;
    int index = 0;
    
    frameLen = 1+1+1;    // frameID+ frameType+ ErrorID    Byte.
    
    
    frameData[index++] = frameID;                                                 // Frame ID
    frameData[index++] = PROTOERROR;                                              // Frame Type
    frameData[index++] = errorID;                                                 // Error ID
    
    print_debug_semph(0,"<<< [INT_sendProtocolErrorFeedbackAck] Sending..\r\n");
    //print_payload(frameData, index);
    res = gw_sendPacket(RECEIVER_ADDRESS, frameData, frameLen, 0);     
    if(res < 0) {                
        return -1;
    }
    return 0;
    
}

// Put INT_COMM_PROTOCOL Packet in Xbee Tx Packet and Send its.
int8_t gw_sendPacket(char *recvAddr, char *data, int dataLen, int timeout) {       
                                                 
    char coordinatorAddr[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    char destNetAddr[] = {0xFF, 0xFE};
    char sendPacket[255];
    int packetLen;
    char frameID;
    char frameLen;
    int res;
    int index = 0;
    
    // Start+Len+FrameType+FrameID+destAddr+destNetAddr+BroadcastRadius+Options+  ProtoVer+SendAddr+RecvAddr+  RFData+CheckSum   Byte.
    packetLen = 1+2+1+1+8+2+1+1+1+8+8+dataLen+1;    
    frameLen = packetLen - 4;
    frameID = data[0];
   
    
    sendPacket[index++] = 0x7E;                                                            // Start Delimeter
    sendPacket[index++] = (frameLen >> 8) & 0xFF;                                          // Xbee Packet Length  - msb
    sendPacket[index++] = frameLen & 0xFF;                                                 //                     - lsb
    sendPacket[index++] = 0x10;                                                            // Frame Type   - ZigBee Transmit Request
    sendPacket[index++] = frameID;                                                         // Frame ID
    memcpy(&sendPacket[index], coordinatorAddr, 8);                                        // 64 bit Destination Address
    index += 8;
    memcpy(&sendPacket[index], destNetAddr, 2);                                            // 16 bit Destination Address
    index += 2;
    sendPacket[index++] = 0x00;                                                            // BroadcastRadius
    sendPacket[index++] = 0x00;                                                            // Options
    /* ---------- Part of INT_COMM_PROTOCOL ---------- */
    sendPacket[index++] = PROTO_VERSION;                                                   // Protocal Version
    memcpy(&sendPacket[index], SENDER_ADDRESS, 8);                                         // Sender Address
    index += 8;
    memcpy(&sendPacket[index], recvAddr, 8);                                               // Receiver Address
    index += 8;
    memcpy(&sendPacket[index], data, dataLen);                                             // Data
    index += dataLen;
    /* ----------------------------------------------- */
    sendPacket[index] = xbee_checksum(&sendPacket[3], packetLen-3);                         // Checksum
    
    
    //printDebug("\r\nPay ---\r\n");   
    //print_payload(sendPacket, packetLen);  
    //printDebug("-------\r\n");
    
    //write_uart1(sendPacket, packetLen);       

    return 0;
    
}

int gw_Heart_ack(uint8_t *edv_adr)
{	
	
		uint8_t gw_tx_buff[255];
		uint16_t packetLen=0;
		int i=0;
		
		gw_tx_buff[packetLen]=0x7e;
		gw_tx_buff[1]=0x00;
		packetLen=packetLen+3;
		gw_tx_buff[packetLen++]=0x10;
		gw_tx_buff[packetLen++]=0x01;
		
		for(i=0;i<8;i++)
		{
			gw_tx_buff[packetLen++]=edv_adr[i+1];
		}
		gw_tx_buff[packetLen++]=0xff;
		gw_tx_buff[packetLen++]=0xfe;
		gw_tx_buff[packetLen++]=0x00;
		gw_tx_buff[packetLen++]=0x00;
		gw_tx_buff[packetLen++]=0x30;
		for(i=0;i<8;i++)
		{
			gw_tx_buff[packetLen++]=0x00;
		}
		for(i=0;i<8;i++)
		{
			gw_tx_buff[packetLen++]=edv_adr[i+1];
		}
		gw_tx_buff[packetLen++]=0xff;
		gw_tx_buff[packetLen++]=0x7b;
		gw_tx_buff[packetLen++]=0x11;
		gw_tx_buff[packetLen++]=0x00;
		//packetLen++;
		gw_tx_buff[packetLen]=xbee_checksum((char*)&gw_tx_buff[3],packetLen-3);
		gw_tx_buff[2]=packetLen-3;
		packetLen++;
		
		
		print_debug_semph(0,"[INFO] Heart beat ACK...\n");
		print_payload(0,gw_tx_buff,packetLen);
		gw_transmit(gw_tx_buff,packetLen);
		
	
	
}

//int gw_join_ack(uint8_t *edv_adr){
//	uint8_t gw_tx_buff[255];
//	uint16_t idx_w = 0;
//	//uint16_t packetLen = 0;
//	uint16_t rf_data_len;
//	uint16_t frame_len;
//	uint8_t frame_id;
//	
//	// Start+Len+FrameType+FrameID+destAddr+destNetAddr+BroadcastRadius+Options+  ProtoVer+SendAddr+RecvAddr+  RFData+CheckSum   Byte.
//	rf_data_len = 3;
//	gw_packet_len = 1+2+1+1+8+2+1+1+1+8+8+rf_data_len+1;
//	frame_len = gw_packet_len-4;
//	frame_id = (uint8_t)0xff;
//	
//	gw_tx_buff[idx_w++] = (uint8_t)0x7e;						// Start Delimeter
//	gw_tx_buff[idx_w++] = (frame_len >> 8) & 0xFF;	// Xbee Packet Length  - msb
//	gw_tx_buff[idx_w++] = frame_len & 0xFF;					//                     - lsb
//	gw_tx_buff[idx_w++] = (uint8_t)0x10;						// Frame Type   - ZigBee Transmit Request
//	gw_tx_buff[idx_w++] = frame_id;									// Frame ID
//	memcpy(&gw_tx_buff[idx_w],edv_adr,8);						// 64 bit Destination Address
//	idx_w += 8;
//	gw_tx_buff[idx_w++] = (uint8_t)0xff;						// 16 bit Destination Address
//	gw_tx_buff[idx_w++] = (uint8_t)0xfe;						// 16 bit Destination Address
//	
//	gw_tx_buff[idx_w++] = (uint8_t)0x00;						// BroadcastRadius
//	gw_tx_buff[idx_w++] = (uint8_t)0x00;						// Options
//	
//	/* ---------- Part of INT_COMM_PROTOCOL ---------- */
//	gw_tx_buff[idx_w++] = (uint8_t)PROTO_VERSION;
//	memcpy(&gw_tx_buff[idx_w], SENDER_ADDRESS, 8);                                         	// Sender Address
//	idx_w += 8;
//	memcpy(&gw_tx_buff[idx_w],edv_adr,8);                                               		// Receiver Address
//	idx_w += 8;	
//	
//	//Data
//	gw_tx_buff[idx_w++] = (uint8_t)NETMAINA;
//	gw_tx_buff[idx_w++] = (uint8_t)JOINREQ;
//	gw_tx_buff[idx_w++] = (uint8_t)0x00;
//	
//	//gw_tx_buff[idx_w++] = xbee_checksum((char*)&gw_tx_buff[3], packetLen-3);                         // Checksum
//	print_debug_semph(0,"packetLen(%d), idx_w(%d)",gw_packet_len,idx_w);
//	gw_tx_buff[idx_w] = xbee_checksum((char*)&gw_tx_buff[3], idx_w-3);                         // Checksum
//	
//	print_debug_semph(0,"[INFO] Join ack\n");
//	print_payload(0,gw_tx_buff, (const uint16_t)gw_packet_len); 
//	gw_transmit(gw_tx_buff,gw_packet_len);
//	return 1;
//}

int8_t gw_join_ack(uint8_t *val , uint16_t len){
	//uint8_t gw_tx_buff[255];
	
	//uint16_t packetLen = 0;
	uint16_t rf_data_len = 0;
	uint16_t frame_len;
	uint8_t frame_id;
	
	// Start+Len+FrameType+FrameID+destAddr+destNetAddr+BroadcastRadius+Options+  ProtoVer+SendAddr+RecvAddr+  RFData+CheckSum   Byte.
	rf_data_len = 5;
	gw_packet_len = 1+2+1+1+8+2+1+1+1+8+8+rf_data_len+1;
	frame_len = gw_packet_len-4;
	frame_id = (uint8_t)0xff;
	
	memset(&gw_send_buf,0,sizeof(data_t));
	idx_send_w = 0;
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x7e;						// Start Delimeter
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x00;	// Xbee Packet Length  - msb
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x00;					//                     - lsb
	
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x10;						// Frame Type   - ZigBee Transmit Request
	gw_send_buf.value[idx_send_w++] = frame_id;									// Frame ID
	memcpy(&gw_send_buf.value[idx_send_w],&val[0],8);						// 64 bit Destination Address
	idx_send_w += 8;
	gw_send_buf.value[idx_send_w++] = (uint8_t)0xff;						// 16 bit Destination Address
	gw_send_buf.value[idx_send_w++] = (uint8_t)0xfe;						// 16 bit Destination Address
	
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x00;						// BroadcastRadius
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x00;						// Options
	
	/* ---------- Part of INT_COMM_PROTOCOL ---------- */
	gw_send_buf.value[idx_send_w++] = (uint8_t)PROTO_VERSION;
	memcpy(&gw_send_buf.value[idx_send_w], SENDER_ADDRESS, 8);                                         	// Sender Address
	idx_send_w += 8;
	memcpy(&gw_send_buf.value[idx_send_w],&val[0],8);                                               		// Receiver Address
	idx_send_w += 8;	
	
	//Frame ID
	gw_send_buf.value[idx_send_w++] = (uint8_t)0xFF;
	//Data
	gw_send_buf.value[idx_send_w++] = (uint8_t)NETMAINA;
	gw_send_buf.value[idx_send_w++] = (uint8_t)JOINREQ;
	gw_send_buf.value[idx_send_w++] = (uint8_t)0x00;
	
	//Update Length
	gw_send_buf.value[1] = ((idx_send_w-3) >> 8) & 0xFF;	// Xbee Packet Length    - msb
	gw_send_buf.value[2] = (idx_send_w-3) & 0xFF;					//                     - lsb
	
	
	//gw_tx_buff[idx_w++] = xbee_checksum((char*)&gw_tx_buff[3], packetLen-3);                         // Checksum
	print_debug_semph(0,"packetLen(%d), idx_w(%d) , idx_w-3(%d)\n",gw_packet_len,idx_send_w,(idx_send_w-3));
	
	print_payload(0,&gw_send_buf.value[3], (idx_send_w-3)); 
	gw_send_buf.value[idx_send_w] = xbee_checksum((char*)&gw_send_buf.value[3], (idx_send_w-3));                         // Checksum
	print_debug_semph(0,"[INFO] Join ack(cs=%02X)\n",gw_send_buf.value[idx_send_w]);
	
	
	idx_send_w += 1;
	
	
	
	//print_debug_semph(0,"[INFO] Join ack\n");
	print_payload(0,gw_send_buf.value, (const uint16_t)idx_send_w); 
	gw_transmit(&gw_send_buf.value[0],idx_send_w);
	return 1;
}

int gw_send_net_main_ack(){
	uint8_t gw_tx_buff[255];
	uint16_t idx_w = 0;
	uint16_t packetLen;
	uint16_t rf_data_len;
	uint16_t frame_len;
	uint8_t frame_id;
	
	// Start+Len+FrameType+FrameID+destAddr+destNetAddr+BroadcastRadius+Options+  ProtoVer+SendAddr+RecvAddr+  RFData+CheckSum   Byte.
	rf_data_len = 4;
	packetLen = 1+2+1+1+8+2+1+1+1+8+8+rf_data_len+1;
	frame_len = packetLen-4;
	frame_id = (uint8_t)0xff;
	
	gw_tx_buff[idx_w++] = (uint8_t)0x7e;
	gw_tx_buff[idx_w++] = (frame_len >> 8) & 0xFF;
	gw_tx_buff[idx_w++] = frame_len & 0xFF;
	gw_tx_buff[idx_w++] = (uint8_t)0x10;
	gw_tx_buff[idx_w++] = frame_id;
}
int gw_Read_DataReport(uint8_t *edv_adr)
{
		//DATASET Read_DataReport[4];
	uint8_t SID[8];
	uint8_t *datafloat;
	int i;
	int NUM_D;
	for(i=0;i<8;i++)
	{
		 SID[i]=edv_adr[i+1];
	}
	print_debug_semph(0,"[INFO]------------------------------Data Report--------------------------------- \r\n");
	print_debug_semph(0,"[INFO] SID DataReport :\r\n");
	print_payload(0,SID,8); 
	NUM_D=edv_adr[44];
	
	switch(NUM_D) 
	{
     case 1:
						Read_DataReport[0].dataID = edv_adr[45];
						Read_DataReport[0].dataType=edv_adr[46];
						memcpy(&Read_DataReport[0].value,&edv_adr[47],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
						
						break;
     case 2:
						Read_DataReport[0].dataID = edv_adr[45];
						Read_DataReport[0].dataType=edv_adr[46];
						memcpy(&Read_DataReport[0].value,&edv_adr[47],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
		 
						Read_DataReport[1].dataID = edv_adr[51];
						Read_DataReport[1].dataType=edv_adr[52];
						memcpy(&Read_DataReport[1].value,&edv_adr[53],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data2----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[1].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[1].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[1].value);
						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
						break;
		 case 3:
						Read_DataReport[0].dataID = edv_adr[45];
						Read_DataReport[0].dataType=edv_adr[46];
						memcpy(&Read_DataReport[0].value,&edv_adr[47],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
		 
						Read_DataReport[1].dataID = edv_adr[51];
						Read_DataReport[1].dataType=edv_adr[52];
						memcpy(&Read_DataReport[1].value,&edv_adr[53],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data2----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[1].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[1].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[1].value);
		 
						Read_DataReport[2].dataID = edv_adr[57];
						Read_DataReport[2].dataType=edv_adr[58];
						memcpy(&Read_DataReport[2].value,&edv_adr[59],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data3----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[2].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[2].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[2].value);
						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
						break;
		 case 4:
						Read_DataReport[0].dataID = edv_adr[45];
						Read_DataReport[0].dataType=edv_adr[46];
						memcpy(&Read_DataReport[0].value,&edv_adr[47],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
		 
						Read_DataReport[1].dataID = edv_adr[51];
						Read_DataReport[1].dataType=edv_adr[52];
						memcpy(&Read_DataReport[1].value,&edv_adr[53],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data2----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[1].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[1].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[1].value);
		 
						Read_DataReport[2].dataID = edv_adr[57];
						Read_DataReport[2].dataType=edv_adr[58];
						memcpy(&Read_DataReport[2].value,&edv_adr[59],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data3----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[2].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[2].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[2].value);
						
						Read_DataReport[3].dataID = edv_adr[63];
						Read_DataReport[3].dataType=edv_adr[64];
						memcpy(&Read_DataReport[3].value,&edv_adr[65],4);;
						print_debug_semph(0,"[INFO]----------------------------------Data4----------------------------------- \r\n");
						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[3].dataID);
						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[3].dataType);
						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[3].value);
						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
		 
						break;
		 default :
            break;
	}
	
	
}

int8_t gw_dat_report_parser(uint8_t *val)
{
		//DATASET Read_DataReport[4];
	uint8_t client_id[8];
	uint8_t *datafloat;
	int i;
	int NUM_D;
	uint16_t idx_w = 0;
	uint16_t len_of_chsum = 0;
	uint8_t data_num_set;
	osStatus os_state;
	
	for(i=0;i<8;i++)
	{
		 client_id[i]=val[i+1];
	}
	print_debug_semph(0,"[INFO]------------------------------Data Report--------------------------------- \r\n");
	print_debug_semph(0,"[INFO] SID DataReport :\r\n");
	print_payload(0,client_id,8); 
	NUM_D=val[44];
	
	memset(&client_tb,0,sizeof(client_tb_t));
	
	idx_w = 1;
	memcpy(client_tb.client_dv[0].address,&val[idx_w],8);
	
	data_num_set = val[44];
	idx_w = 45;
	if(data_num_set > 0){
		for(i=0;i<data_num_set;i++){
			client_tb.client_dv[0].data_set[i].data_id = val[idx_w++];
			client_tb.client_dv[0].data_set[i].data_type = val[idx_w++];
			memcpy(&client_tb.client_dv[0].data_set[i].value,&val[idx_w],4);
			idx_w += 4;
		}
		
		print_debug_semph(0,"[INFO]{data report}\n");
		print_debug_semph(0,"client id: ");
		//Print Adr of client
		for(i=0;i<8;i++){
			print_debug_semph(0,"%02X ",client_tb.client_dv[0].address[i]);
		}
		print_debug_semph(0,"\n");
			
		print_debug_semph(0,"payload(%d set): \n",data_num_set);
		for(i=0;i<data_num_set;i++){
			if(i==1){
				print_payload(0,(const uint8_t*)&client_tb.client_dv[0].data_set[i],6);
				print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",client_tb.client_dv[0].data_set[i].data_id);
				print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",client_tb.client_dv[0].data_set[i].data_type);
				client_tb.client_dv[0].data_set[i].value = client_tb.client_dv[0].data_set[i].value/1000;
				print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",client_tb.client_dv[0].data_set[i].value);
			}else{
				print_payload(0,(const uint8_t*)&client_tb.client_dv[0].data_set[i],6);
				print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",client_tb.client_dv[0].data_set[i].data_id);
				print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",client_tb.client_dv[0].data_set[i].data_type);
				print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",client_tb.client_dv[0].data_set[i].value);
			}
			
		}
		print_debug_semph(0,"\n");
	}
	
	
	
	if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
		if(xQueueSend(que_gw_to_ethHandle,&client_tb.client_dv[0],1000)==pdTRUE){
			print_debug_semph(0,"[IONFO]{Queue}que_gw_to_ethHandle can send.\n");	
		}
		osSemaphoreRelease(binarySemQueueHandle);
	}	
	
	/////////////////////// Data Report Ack /////////////////////////
	idx_w = 0;
	gw_send_buf.value[idx_w++]=(uint8_t)0x7e;

	//Length Remaining
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;

	//Frame Type(Send:0x10)
	gw_send_buf.value[idx_w++]=(uint8_t)0x10;

	gw_send_buf.value[idx_w++]=(uint8_t)0xFF;
	
	memcpy(&gw_send_buf.value[idx_w],&val[1],8);
	idx_w += 8;

	gw_send_buf.value[idx_w++]=(uint8_t)0xff;
	gw_send_buf.value[idx_w++]=(uint8_t)0xfe;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	
	////////IMS Protocol Structure
	//Protocol Version 
	gw_send_buf.value[idx_w++]=(uint8_t)0x30;
	
	//Sender Address
	for(i=0;i<8;i++)
	{
		gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	}
	
	//Receiver Address
	for(i=0;i<8;i++)
	{
		gw_send_buf.value[idx_w++]=(uint8_t)val[i+1];
	}
	
	//Frame ID
	gw_send_buf.value[idx_w++]=(uint8_t)0x03;
	
	//Frame Type
	gw_send_buf.value[idx_w++]=(uint8_t)0x5B;
	
		
	//Update Length Remianing
	gw_send_buf.value[1] = ((idx_w-3) >> 8) & 0xFF;	// Xbee Packet Length    - msb
	gw_send_buf.value[2] = (idx_w-3) & 0xFF;					//                     - lsb
	
	
	//Checksum
	len_of_chsum = idx_w-3;
	gw_send_buf.value[idx_w++]=xbee_checksum((char*)&gw_send_buf.value[3],len_of_chsum);
	
	
	
	print_debug_semph(0,"[INFO] Data Report ACK...\n");
	print_payload(0,gw_send_buf.value,idx_w);
	gw_transmit(gw_send_buf.value,idx_w);
	
	
//	switch(NUM_D) 
//	{
//     case 1:
//						Read_DataReport[0].dataID = val[45];
//						Read_DataReport[0].dataType=val[46];
//						memcpy(&Read_DataReport[0].value,&val[47],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
//						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
//						
//						break;
//     case 2:
//						Read_DataReport[0].dataID = val[45];
//						Read_DataReport[0].dataType=val[46];
//						memcpy(&Read_DataReport[0].value,&val[47],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
//		 
//						Read_DataReport[1].dataID = val[51];
//						Read_DataReport[1].dataType=val[52];
//						memcpy(&Read_DataReport[1].value,&val[53],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data2----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[1].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[1].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[1].value);
//						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
//						break;
//		 case 3:
//						Read_DataReport[0].dataID = val[45];
//						Read_DataReport[0].dataType=val[46];
//						memcpy(&Read_DataReport[0].value,&val[47],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
//		 
//						Read_DataReport[1].dataID = val[51];
//						Read_DataReport[1].dataType=val[52];
//						memcpy(&Read_DataReport[1].value,&val[53],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data2----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[1].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[1].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[1].value);
//		 
//						Read_DataReport[2].dataID = val[57];
//						Read_DataReport[2].dataType=val[58];
//						memcpy(&Read_DataReport[2].value,&val[59],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data3----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[2].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[2].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[2].value);
//						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
//						break;
//		 case 4:
//						Read_DataReport[0].dataID = val[45];
//						Read_DataReport[0].dataType=val[46];
//						memcpy(&Read_DataReport[0].value,&val[47],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data1----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[0].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[0].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[0].value);
//		 
//						Read_DataReport[1].dataID = val[51];
//						Read_DataReport[1].dataType=val[52];
//						memcpy(&Read_DataReport[1].value,&val[53],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data2----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[1].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[1].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[1].value);
//		 
//						Read_DataReport[2].dataID = val[57];
//						Read_DataReport[2].dataType=val[58];
//						memcpy(&Read_DataReport[2].value,&val[59],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data3----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[2].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[2].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[2].value);
//						
//						Read_DataReport[3].dataID = val[63];
//						Read_DataReport[3].dataType=val[64];
//						memcpy(&Read_DataReport[3].value,&val[65],4);;
//						print_debug_semph(0,"[INFO]----------------------------------Data4----------------------------------- \r\n");
//						print_debug_semph(0,"[INFO] Data ID : Sensor %d \r\n",Read_DataReport[3].dataID);
//						print_debug_semph(0,"[INFO] Data Type :  0x%x \r\n",Read_DataReport[3].dataType);
//						print_debug_semph(0,"[INFO] Value :  %0.3f \r\n",Read_DataReport[3].value);
//						print_debug_semph(0,"[INFO]----------------------- -----END Data Report------------------------------ \r\n");
//		 
//						break;
//		 default :
//            break;
//	}
	
	
}

//int gw_Read_Status(uint8_t *edv_adr)
//{
//	uint8_t SID[8];
//	float dataV;
//	float dataA;
//	int i;
//	uint8_t Status_Relay;
//	for(i=0;i<8;i++)
//	{
//		 SID[i]=edv_adr[i+1];
//	}
//	print_debug_semph(0,"[INFO]------------------------------Status Report--------------------------------- \r\n");
//	print_debug_semph(0,"[INFO] SID Status : \r\n");
//	print_payload(0,SID,8); 
//	Status_Relay=edv_adr[44];
//	print_debug_semph(0,"[INFO] Relay Status : 0x0%x\r\n",Status_Relay);
//	memcpy(&dataV,&edv_adr[49],4);
//	print_debug_semph(0,"[INFO] Voltage Status : %0.3f V.\r\n",dataV);
//	memcpy(&dataA,&edv_adr[53],4);
//	print_debug_semph(0,"[INFO] Current Status : %0.3f A.\r\n",dataA);
//	print_debug_semph(0,"[INFO]-----------------------------END Status Report------------------------------- \r\n");
//	
//}

int8_t gw_status_parser(uint8_t *val)
{
	uint8_t SID[8];
	uint16_t idx_w = 0;
	float dataV;
	uint16_t len_of_chsum = 0;
	float dataA;
	int i;
	uint8_t Status_Relay;
	osStatus os_state;
	
	for(i=0;i<8;i++)
	{
		 SID[i]=val[i+1];
	}
	print_debug_semph(0,"[INFO]------------------------------Status Report--------------------------------- \r\n");
	print_debug_semph(0,"[INFO] SID Status : \r\n");
	print_payload(0,SID,8); 
	Status_Relay=val[44];
	print_debug_semph(0,"[INFO] Relay Status : 0x0%x\r\n",Status_Relay);
	memcpy(&dataV,&val[49],4);
	print_debug_semph(0,"[INFO] Voltage Status : %0.3f V.\r\n",dataV);
	memcpy(&dataA,&val[53],4);
	print_debug_semph(0,"[INFO] Current Status : %0.3f A.\r\n",dataA);
	print_debug_semph(0,"[INFO]-----------------------------END Status Report------------------------------- \r\n");
	
	//client_stat_tb
	
	memset(&client_stat_tb,0,sizeof(client_stat_tb_t));
	
	idx_w = 1;
	memcpy(client_stat_tb.status_dv[0].address,&val[idx_w],8);
	
	idx_w = 44;
	client_stat_tb.status_dv[0].status_set.dat_switch = val[idx_w];
	
	idx_w = 49;
	memcpy(&client_stat_tb.status_dv[0].status_set.dat_volt,&val[idx_w],4);
	idx_w += 4;
	memcpy(&client_stat_tb.status_dv[0].status_set.dat_amp,&val[idx_w],4);
	
	print_debug_semph(0,"[INFO]{status report}\n");
	print_debug_semph(0,"client id: ");
	//Print Adr of client
	for(i=0;i<8;i++){
		print_debug_semph(0,"%02X ",client_stat_tb.status_dv[0].address[i]);
	}
	print_debug_semph(0,"\n");
		
	print_debug_semph(0,"payload: \n");
	print_debug_semph(0,"[INFO] Relay Status :  %02X \r\n",client_stat_tb.status_dv[0].status_set.dat_switch);
	print_debug_semph(0,"[INFO] Volt :  %0.3f V\r\n",client_stat_tb.status_dv[0].status_set.dat_volt);
	print_debug_semph(0,"[INFO] Amp :  %0.3f A\r\n",client_stat_tb.status_dv[0].status_set.dat_amp);
	print_debug_semph(0,"\n");
	
	if((os_state=osSemaphoreWait(binarySemQueueHandle,2000))==0){
		if(xQueueSend(q_gw_stat_to_ethHandle,&client_stat_tb.status_dv[0],1000)==pdTRUE){
			print_debug_semph(0,"[IONFO]{Queue}q_gw_stat_to_ethHandle can send.\n");	
		}
		osSemaphoreRelease(binarySemQueueHandle);
	}	
	
	
	
	/////////////////////// Status Report Ack /////////////////////////
	idx_w = 0;
	gw_send_buf.value[idx_w++]=(uint8_t)0x7e;

	//Length Remaining
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;

	//Frame Type(Send:0x10)
	gw_send_buf.value[idx_w++]=(uint8_t)0x10;

	gw_send_buf.value[idx_w++]=(uint8_t)0xFF;
	
	memcpy(&gw_send_buf.value[idx_w],&val[1],8);
	idx_w += 8;

	gw_send_buf.value[idx_w++]=(uint8_t)0xff;
	gw_send_buf.value[idx_w++]=(uint8_t)0xfe;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	
	////////IMS Protocol Structure
	//Protocol Version 
	gw_send_buf.value[idx_w++]=(uint8_t)0x30;
	
	//Sender Address
	for(i=0;i<8;i++)
	{
		gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	}
	
	//Receiver Address
	for(i=0;i<8;i++)
	{
		gw_send_buf.value[idx_w++]=(uint8_t)val[i+1];
	}
	
	//Frame ID
	gw_send_buf.value[idx_w++]=(uint8_t)0x03;
	
	//Frame Type
	gw_send_buf.value[idx_w++]=(uint8_t)0x2B;
	
		//Timestamp
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	
	//Update Length Remianing
	gw_send_buf.value[1] = ((idx_w-3) >> 8) & 0xFF;	// Xbee Packet Length    - msb
	gw_send_buf.value[2] = (idx_w-3) & 0xFF;					//                     - lsb
	
	
	//Checksum
	len_of_chsum = idx_w-3;
	gw_send_buf.value[idx_w++]=xbee_checksum((char*)&gw_send_buf.value[3],len_of_chsum);
	
	
	
	print_debug_semph(0,"[INFO] Status ACK...\n");
	print_payload(0,gw_send_buf.value,idx_w);
	gw_transmit(gw_send_buf.value,idx_w);
}

int8_t gw_event_parser(uint8_t *val)
{
	uint8_t SID[8];
	uint16_t idx_w = 0;
	float dataV;
	float dataA;
	uint16_t len_of_chsum = 0;
	int i;
	uint8_t Status_Relay;
	for(i=0;i<8;i++)
	{
		 SID[i]=val[i+1];
	}
	
	//val[45] = Event ID
	
	print_debug_semph(0,"\n\n[INFO]{event report}----------------\n");
	print_debug_semph(0,"client id: ");
	//Print Adr of client
	for(i=0;i<8;i++){
		print_debug_semph(0,"%02X ",SID[i]);
	}
	print_debug_semph(0,"\n");
	
	
	print_debug_semph(0,"event id(%02X).\n",val[44]);
	
	if(val[44] == (uint8_t)0xA0){
		print_debug_semph(0,"switch press event: ",val[44]);
		if(val[45] == (uint8_t)0x00){
			print_debug_semph(0,"OFF(%02X).\n",val[45]);
		}else if(val[45] == (uint8_t)0x01){
			print_debug_semph(0,"ON(%02X).\n",val[45]);
		}
	}
	print_debug_semph(0,"\n");
	
	
	idx_w = 0;
	gw_send_buf.value[idx_w++]=(uint8_t)0x7e;

	//Length Remaining
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;

	//Frame Type(Send:0x10)
	gw_send_buf.value[idx_w++]=(uint8_t)0x10;

	gw_send_buf.value[idx_w++]=(uint8_t)0xFF;
	
	memcpy(&gw_send_buf.value[idx_w],&val[1],8);
	idx_w += 8;

	gw_send_buf.value[idx_w++]=(uint8_t)0xff;
	gw_send_buf.value[idx_w++]=(uint8_t)0xfe;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	
	////////IMS Protocol Structure
	//Protocol Version 
	gw_send_buf.value[idx_w++]=(uint8_t)0x30;
	
	//Sender Address
	for(i=0;i<8;i++)
	{
		gw_send_buf.value[idx_w++]=(uint8_t)0x00;
	}
	
	//Receiver Address
	for(i=0;i<8;i++)
	{
		gw_send_buf.value[idx_w++]=(uint8_t)val[i+1];
	}
	
	//Frame ID
	gw_send_buf.value[idx_w++]=(uint8_t)0x03;
	
	//Frame Type
	gw_send_buf.value[idx_w++]=(uint8_t)0x3B;
	
		
	//Update Length Remianing
	gw_send_buf.value[1] = ((idx_w-3) >> 8) & 0xFF;	// Xbee Packet Length    - msb
	gw_send_buf.value[2] = (idx_w-3) & 0xFF;					//                     - lsb
	
	
	//Checksum
	len_of_chsum = idx_w-3;
	gw_send_buf.value[idx_w++]=xbee_checksum((char*)&gw_send_buf.value[3],len_of_chsum);
	
	
	
	print_debug_semph(0,"[INFO] Event ACK...\n");
	print_payload(0,gw_send_buf.value,idx_w);
	gw_transmit(gw_send_buf.value,idx_w);
}

int8_t gw_remote_parser(uint8_t *val)
{
	uint8_t SID[8];
	uint16_t idx_w = 0;
	float dataV;
	float dataA;
	uint16_t len_of_chsum = 0;
	int i;
	uint8_t Status_Relay;
	for(i=0;i<8;i++)
	{
		 SID[i]=val[i+1];
	}
	
	//val[45] = Event ID
	
	print_debug_semph(0,"\n\n[INFO]{event report}----------------\n");
	print_debug_semph(0,"client id: ");
	//Print Adr of client
	for(i=0;i<8;i++){
		print_debug_semph(0,"%02X ",SID[i]);
	}
	print_debug_semph(0,"\n");
	
	
	print_debug_semph(0,"channel id(%02X).\n",val[31]);
	
	if(val[32] == (uint8_t)0x00){
		print_debug_semph(0,"success(%02X)",val[32]);
	}else if(val[32] == (uint8_t)0x00){
		print_debug_semph(0,"error(%02X)",val[32]);
	}
	print_debug_semph(0,"\n");
	
	print_debug_semph(0,"[INFO] Remote ACK...\n");
}

