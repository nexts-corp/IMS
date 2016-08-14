#include "wifi_api.h"
#include "print_debug.h"
#include <EEPROM.h>


extern WiFiClient client;
extern PrintDebug debug;
extern uint8_t print_debug_mode;

WIFI_API::WIFI_API(){}

uint8_t WIFI_API::get_dhcp(){
  EEPROM.get( ARD_DHCP, w_dhcp );
  return w_dhcp;
}
uint8_t* WIFI_API::get_ip(){
  int i = 0;
//  for(i=0;i<4;i++){
//    EEPROM.get( ARD_IP + i, w_ip[i] );
//  }
  for(i=0;i<4;i++){
    w_ip[i] = EEPROM.read( ARD_IP + i);
  }
  return w_ip;
}
uint8_t* WIFI_API::get_sn(){
  int i = 0;
//  for(i=0;i<4;i++){
//    EEPROM.get( ARD_SN + i, w_sn[i] );
//  }
  for(i=0;i<4;i++){
    w_sn[i] = EEPROM.read( ARD_SN + i);
  }
  return w_sn;
}
uint8_t* WIFI_API::get_gw(){
  int i = 0;
//  for(i=0;i<4;i++){
//    EEPROM.get( ARD_GW + i, w_gw[i] );
//  }
  for(i=0;i<4;i++){
    w_gw[i] = EEPROM.read( ARD_GW + i);
  }
  return w_gw;
}
uint8_t* WIFI_API::get_mac(){
  int i = 0;
//  for(i=0;i<6;i++){
//    EEPROM.get( ARD_MAC + i, w_mac[i] );
//  }
  for(i=0;i<6;i++){
    w_mac[i] = EEPROM.read( ARD_MAC + i);
  }
  return w_mac;
}
uint8_t* WIFI_API::get_dns_ip(){
  int i = 0;
//  for(i=0;i<4;i++){
//    EEPROM.get( ARD_DNS_IP + i, w_dns_ip[i] );
//  }
  for(i=0;i<4;i++){
    w_dns_ip[i] = EEPROM.read( ARD_DNS_IP + i);
  }
  return w_dns_ip;
}
char* WIFI_API::get_dns_name(){
  int i = 0;
//  for(i=0;i<32;i++){
//    EEPROM.get( ARD_DNS_NAME + i, w_dns_name[i] );
//  }
  for(i=0;i<32;i++){
    w_dns_name[i] = EEPROM.read( ARD_DNS_NAME + i);
  }
  return w_dns_name;
}
char* WIFI_API::get_ssid(){
  int i = 0;
//  for(i=0;i<32;i++){
//    EEPROM.get( ARD_SSID + i, w_ssid[i] );
//  }
  debug.print(0,"get -> w_ssid-1\n");
  debug.print_payload(0,(uint8_t*)w_ssid,32);
  for(i=0;i<32;i++){
    w_ssid[i] = EEPROM.read( ARD_SSID + i);
  }
  debug.print(0,"get -> w_ssid-2\n");
  debug.print_payload(0,(uint8_t*)w_ssid,32);
  return w_ssid;
}
char* WIFI_API::get_pwd(){
  int i = 0;
//  for(i=0;i<32;i++){
//    EEPROM.get( ARD_PWD + i, w_pwd[i] );
//  }
  debug.print(0,"get -> w_pwd-1\n");
  debug.print_payload(0,(uint8_t*)w_pwd,32);
  for(i=0;i<32;i++){
    w_pwd[i] = EEPROM.read( ARD_PWD + i);
  }
  debug.print(0,"get -> w_pwd-2\n");
  debug.print_payload(0,(uint8_t*)w_pwd,32);
//  EEPROM.get( ARD_PWD, w_pwd );
  return w_pwd;
}

uint8_t* WIFI_API::get_serv_ip(){
  int i = 0;
//  for(i=0;i<4;i++){
//    EEPROM.get( ARD_SERV_IP + i, w_serv_ip[i] );
//  }
  for(i=0;i<4;i++){
    w_serv_ip[i] = EEPROM.read( ARD_SERV_IP + i);
  }
  return w_serv_ip;
}
char* WIFI_API::get_serv_name(){
  int i = 0;
//  for(i=0;i<32;i++){
//    EEPROM.get( ARD_SERV_NAME + i, w_serv_name[i] );
//  }
  for(i=0;i<32;i++){
    w_serv_name[i] = EEPROM.read( ARD_SERV_NAME + i);
  }
  return w_serv_name;
}
uint16_t WIFI_API::get_serv_port(){
  EEPROM.get( ARD_SERV_PORT , w_serv_port);
  return w_serv_port;
}



void WIFI_API::set_dhcp(uint8_t data){
  w_dhcp = data;
  EEPROM.put( ARD_DHCP, w_dhcp );
  EEPROM.commit();
}
void WIFI_API::set_ip(uint8_t *data){
  int i = 0;
  
  memcpy(w_ip,data,4);
//  for(i=0;i<4;i++){
//    EEPROM.put( ARD_IP + i, w_ip[i] );
//  }
  for(i=0;i<4;i++){
     EEPROM.write( ARD_IP + i, w_ip[i]);
  }
  EEPROM.commit();
}
void WIFI_API::set_sn(uint8_t *data){
  int i = 0;
  memcpy(w_sn,data,4);
//  for(i=0;i<4;i++){
//    EEPROM.put( ARD_SN + i, w_sn[i] );
//  }
  for(i=0;i<4;i++){
     EEPROM.write( ARD_SN + i,w_sn[i]);
  }
  EEPROM.commit();
}
void WIFI_API::set_gw(uint8_t *data){
  int i = 0;
  memcpy(w_gw,data,4);
//  for(i=0;i<4;i++){
//    EEPROM.put( ARD_GW + i, w_gw[i] );
//  }
  for(i=0;i<4;i++){
     EEPROM.write( ARD_GW + i, w_gw[i]);
  }
  EEPROM.commit();
}
void WIFI_API::set_mac(uint8_t *data){
  int i = 0;
  memcpy(w_mac,data,6);
//  for(i=0;i<6;i++){
//    EEPROM.put( ARD_MAC + i, w_mac[i] );
//  }
  for(i=0;i<6;i++){
     EEPROM.write( ARD_MAC + i, w_mac[i]);
  }
  EEPROM.commit();
}
void WIFI_API::set_dns_ip(uint8_t *data){
  int i = 0;
  memcpy(w_dns_ip,data,4);
//  for(i=0;i<4;i++){
//    EEPROM.put( ARD_DNS_IP + i, w_dns_ip[i] );
//  }
  for(i=0;i<4;i++){
     EEPROM.write( ARD_DNS_IP + i, w_dns_ip[i]);
  }
  EEPROM.commit();
}
void WIFI_API::set_dns_name(char *data){
  int i = 0;
  memset(w_dns_name,0,sizeof(w_dns_name));
  memcpy(w_dns_name,data,32);
//  for(i=0;i<32;i++){
//    EEPROM.put( ARD_DNS_NAME + i, w_dns_name[i] );
//  }
  for(i=0;i<32;i++){
     EEPROM.write( ARD_DNS_NAME + i, w_dns_name[i]);
  }
  EEPROM.commit();
}

void WIFI_API::set_ssid(char* data){
  int i = 0;
  memset(w_ssid,0,sizeof(w_ssid));
  memcpy(w_ssid,data,32);
  debug.print(0,"w_ssid-1(data)\n");
  debug.print_payload(0,(uint8_t*)data,32);
  debug.print(0,"w_ssid-2(w_pwd)\n");
  debug.print_payload(0,(uint8_t*)w_ssid,32);
//  for(i=0;i<32;i++){
//    EEPROM.put( ARD_SSID + i, w_ssid[i] );
//  }
  for(i=0;i<32;i++){
    EEPROM.write( ARD_SSID + i, w_ssid[i] );
  }
  EEPROM.commit();
}
void WIFI_API::set_pwd(char* data){
  int i = 0;
  memset(w_pwd,0,sizeof(w_pwd));
  memcpy(w_pwd,data,32);

  debug.print(0,"set_pwd-1(data)\n");
  debug.print_payload(0,(uint8_t*)data,32);
  debug.print(0,"set_pwd-2(w_pwd)\n");
  debug.print_payload(0,(uint8_t*)w_pwd,32);
//  for(i=0;i<32;i++){
//    EEPROM.put( ARD_PWD + i, w_pwd[i] );
//  }
  for(i=0;i<32;i++){
    EEPROM.write( ARD_PWD + i, w_pwd[i] );
  }
  //EEPROM.put( ARD_PWD, w_pwd);
  EEPROM.commit();
}
void WIFI_API::set_serv_ip(uint8_t *data){
  int i = 0;
  memcpy(w_serv_ip,data,4);
//  for(i=0;i<4;i++){
//    EEPROM.put( ARD_SERV_IP + i, w_serv_ip[i] );
//  }
  for(i=0;i<4;i++){
     EEPROM.write( ARD_SERV_IP + i, w_serv_ip[i]);
  }
  EEPROM.commit();
}
void WIFI_API::set_serv_name(char* data){
  int i = 0;
  memset(w_serv_name,0,sizeof(w_serv_name));
  memcpy(w_serv_name,data,32);
//  for(i=0;i<32;i++){
//    EEPROM.put( ARD_SERV_NAME + i, w_serv_name[i] );
//  }
  for(i=0;i<32;i++){
    EEPROM.write( ARD_SERV_NAME + i, w_serv_name[i] );
  }
  EEPROM.commit();
}
void WIFI_API::set_serv_port(uint16_t data){
  w_serv_port = data;
  EEPROM.put( ARD_SERV_PORT, w_serv_port );
  EEPROM.commit();
}

void WIFI_API::send_status(uint8_t data){
  uint8_t buff[20];
  uint16_t len;
  uint16_t idx_w = 0;
  uint8_t status = data;
  uint16_t pkt_length = 0;
  uint8_t pkt_chsum = 0;
  
  //pkt_length = Frame Type(1) + CMD + Payload(1)
  pkt_length = (1 + 1 + 1);

  buff[idx_w++] = 0x7e;
  buff[idx_w++] = (pkt_length >> 8) & 0xFF;
  buff[idx_w++] = pkt_length & 0xFF;
    
  buff[idx_w++] = (uint8_t)W_STATUS; //Frame Type
  buff[idx_w++] = (uint8_t)W_CONNECT_SERV;  //CMD
  buff[idx_w++] = (uint8_t)data;  //Data
  pkt_chsum = checksum((char*)&buff[3],pkt_length);
  buff[idx_w++] = pkt_chsum;

  Serial.write((byte*)buff,idx_w);  
}
void WIFI_API::send_uart_data(uint8_t *data, uint16_t len){
  uint8_t buff[20];
  uint16_t idx_w = 0;
  uint16_t pkt_length = 0;
  uint8_t pkt_chsum = 0;

  //pkt_length = Frame Type(1) + Payload(1)
  pkt_length = (1 + len);
  
  buff[idx_w++] = 0x7e;
  buff[idx_w++] = (pkt_length >> 8) & 0xFF;
  buff[idx_w++] = pkt_length & 0xFF;
  buff[idx_w++] = (uint8_t)W_DATA_TRASNSMIT; //Frame Type

  pkt_chsum = checksum((char*)&buff[3],pkt_length);

  Serial.write((byte*)buff,idx_w); 
  Serial.write((byte*)data,len); 
  Serial.write((byte*)&pkt_chsum,1); 
  
}
void WIFI_API::send_event(uint8_t data){
  
}

int8_t WIFI_API::process(uint8_t *value, uint16_t len){
  int offset = 0;
  char *ret = NULL;
  uint16_t len_buff;
  int ptc_ret;
  uint8_t frame_type;
  uint8_t cmd;
  int size_pkt = 0x0000;
  int total = 0;
  char st = 0x7e;
  char * pch;
  int str_offset = 0;
  uint16_t idx_r = 0;


  do{
    if(value[idx_r] == 0x7e){
      offset = idx_r;
      if(offset==len){
        debug.print(0,"invalid coordinator packet\n");                                   // not found Start Delimiter 0x7E
        ptc_ret = -1;
        return ptc_ret;
      }
      size_pkt = (value[offset+1] & 0xffff) << 8;                                       // check packet length (MSB)
      size_pkt |= value[offset+2];                                                      // check packet length (LSB)
      debug.print(0,"\r\nlen=%d(%04x)\r\n", size_pkt, size_pkt);
      //Serial.print();
      total += size_pkt;                                                                     // update total analyse 
      if (total > len){                                                                //check length validation
          offset++;
          total = offset;                                                               // roll back total analyse                          
          debug.print(0,"\r\ninvalid length!!\r\n");
          //continue;
      }                             
        
      if(checksum((char*)&value[offset+3], size_pkt) != value[offset+3+size_pkt]){        // checksum error detection                  
          debug.print(0,(char*)"checksum error\r\n");
          offset++;
          total = offset;                                                               // roll back total analyse          
      }
      else{                                                                             // got a valid packet
        debug.print_payload(0,&value[offset+3], size_pkt);
        frame_type = value[offset+3]; 
        if(frame_type == W_DATA_RECEIV){
          debug.print(0,"W_DATA_RECEIV\n");
          send_wifi_data(&value[offset+3], (size_pkt-1));   //send wifi data
        }else if(frame_type == W_CONFIG_RECEIV){
          debug.print(0,"W_CONFIG_RECEIV\n");
          cmd = value[offset+3+1];
          response_config(cmd, &value[offset+3], size_pkt);
        }else{
          debug.print(0,"Unknow cmd.\n");
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
  
  return ptc_ret;
}

void WIFI_API::send_wifi_data(uint8_t *data, uint16_t len){
  debug.print_payload(0,&data[1], len);
  client.write((byte*)&data[1],len);
}

uint8_t  WIFI_API::checksum(char *buf, uint16_t len){
  int i = 0;
  char sum = 0;                                          
  //print_payload(buf,len);
  for (i = 0; i < len; i++) {
      sum += buf[i];
  }                 
  return (0xff - (sum & 0xff));
}

void protocol_proc(){
  
}

void WIFI_API::response_config(int8_t cmd , uint8_t *data, uint16_t len){
  uint8_t buf[20];
  char str_buf[40];
  uint16_t idx_w = 0;
  uint16_t pkt_length = 0;
  uint8_t pkt_chsum = 0;
  uint8_t ip_buf[4];
  uint8_t mac_buf[6];
  
  if(cmd == W_GET_DHCP){
    uint8_t dhcp = get_dhcp();
    debug.print(0,"DHCP: %02X\n",get_dhcp());

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_DHCP;     //CMD

    memcpy(&buf[idx_w],&dhcp,1);
    idx_w += 4;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_GET_IP){
    memcpy(ip_buf,get_ip(),4);
    debug.print(0,(const char*)"IP: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+4);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_IP;       //CMD

    memcpy(&buf[idx_w],get_ip(),4);
    idx_w += 4;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_GET_SN){
    memcpy(ip_buf,get_sn(),4);
    debug.print(0,(const char*)"SN: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    pkt_length = (1+1+4);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_SN;       //CMD

    memcpy(&buf[idx_w],get_sn(),4);
    idx_w += 4;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_GET_GW){
    memcpy(ip_buf,get_gw(),4);
    debug.print(0,(const char*)"GW: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    pkt_length = (1+1+4);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_GW;       //CMD

    memcpy(&buf[idx_w],get_gw(),4);
    idx_w += 4;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
    
  }else if(cmd == W_GET_MAC){
    memset(mac_buf,0,sizeof(str_buf));
    memcpy(mac_buf,get_mac(),6);
    debug.print(0,(const char*)"MAC: %02X:%02X:%02X:%02X\n",mac_buf[0], mac_buf[1],mac_buf[2],mac_buf[3],mac_buf[4],mac_buf[5]);

    pkt_length = (1+1+6);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_MAC;       //CMD

    memcpy(&buf[idx_w],mac_buf,6);
    idx_w += 6;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
    
  }else if(cmd == W_GET_SERV_IP){
    memcpy(ip_buf,get_serv_ip(),4);
    debug.print(0,(const char*)"Server IP: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    pkt_length = (1+1+4);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_SERV_IP;       //CMD

    memcpy(&buf[idx_w],get_serv_ip(),4);
    idx_w += 4;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
    
  }else if(cmd == W_GET_SERV_NAME){
    memset(str_buf,0,sizeof(str_buf));
    memcpy(str_buf,get_serv_name(),32);
    debug.print(0,(const char*)"Server Name: %s\n",str_buf);

    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_SERV_NAME;       //CMD

    memcpy(&buf[idx_w],str_buf,32);
    idx_w += 32;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
    
  }else if(cmd == W_GET_SERV_PORT){
    uint16_t serv_port = get_serv_port();
    debug.print(0,(const char*)"Server Port: %d\n",serv_port);

    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_SERV_PORT;       //CMD

    memcpy(&buf[idx_w],&serv_port,1);
    idx_w += 4;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
    
  }else if(cmd == W_GET_SSID){
    memset(str_buf,0,sizeof(str_buf));
    memcpy(str_buf,get_ssid(),32);
    debug.print(0,(const char*)"SSID: %s\n",str_buf);

    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_SSID;       //CMD

    memcpy(&buf[idx_w],str_buf,32);
    idx_w += 32;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
      
  }else if(cmd == W_GET_PWD){
    memset(str_buf,0,sizeof(str_buf));
    memcpy(str_buf,get_pwd(),32);
    debug.print(0,(const char*)"PWD: %s\n",str_buf);

    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_GET_PWD;       //CMD

    memcpy(&buf[idx_w],str_buf,32);
    idx_w += 32;
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
      
  }

  if(cmd == W_RESTART){
    while(1);      
  }else if(cmd == W_CLR_EEP){
    clear_eep();
    while(1);
  }

  if(cmd == W_SET_DHCP){
    
    uint8_t dhcp = data[2];
    set_dhcp(dhcp);
    debug.print(0,"DHCP Set: %02X\n",dhcp);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_DHCP;     //CMD
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_IP){
    memcpy(ip_buf,&data[2],4);
    
    set_ip(ip_buf);
    debug.print(0,(const char*)"IP Set: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_IP;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_SN){
    memcpy(ip_buf,&data[2],4);
    
    set_ip(ip_buf);
    debug.print(0,(const char*)"SN Set: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_SN;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_GW){
    memcpy(ip_buf,&data[2],4);
    
    set_ip(ip_buf);
    debug.print(0,(const char*)"GW Set: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_GW;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_MAC){
    memset(mac_buf,0,sizeof(str_buf));
    memcpy(mac_buf,&data[2],4);
    
    set_mac(mac_buf);
    debug.print(0,(const char*)"MAC Set: %02X:%02X:%02X:%02X\n",mac_buf[0], mac_buf[1],mac_buf[2],mac_buf[3],mac_buf[4],mac_buf[5]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_MAC;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_DNS_IP){
    memcpy(ip_buf,&data[2],4);
    
    set_ip(ip_buf);
    debug.print(0,(const char*)"DNS IP Set: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_DNS_IP;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_DNS_NAME){
    memcpy(str_buf,&data[2],32);
    
    set_dns_name(str_buf);
    debug.print(0,(const char*)"DNS Name Set: %s\n",str_buf);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_DNS_NAME;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_SERV_IP){
    memcpy(ip_buf,&data[2],4);
    
    set_serv_ip(ip_buf);
    debug.print(0,(const char*)"Serv IP Set: %d.%d.%d.%d\n",ip_buf[0], ip_buf[1],ip_buf[2],ip_buf[3]);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_SERV_IP;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_SERV_NAME){
    memcpy(str_buf,&data[2],32);
    
    set_serv_name(str_buf);
    debug.print(0,(const char*)"Serv Name Set: %s\n",str_buf);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_SERV_NAME;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_SERV_PORT){
    uint16_t serv_port;
    memcpy(&serv_port,&data[2],2);
    
    set_serv_port(serv_port);
    debug.print(0,(const char*)"Serv Port Set: %d\n",serv_port);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_SERV_PORT;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }else if(cmd == W_SET_SSID){
    memcpy(str_buf,&data[2],32);
    
    set_ssid(str_buf);
    debug.print(0,(const char*)"SSID Set: %s\n",str_buf);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_SSID;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }
  else if(cmd == W_SET_PWD){
    memset(str_buf,0,sizeof(str_buf));
    memcpy(str_buf,&data[2],32);
    
    set_pwd(str_buf);
    debug.print(0,(const char*)"PWD Set: %s\n",str_buf);

    //Frame Type + CMD + Payload
    pkt_length = (1+1+32);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_SET_PWD;     //CMD
    
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;

    Serial.write((byte*)buf,idx_w);  
  }
  
}

void set_debug_mode(uint8_t data){
  uint8_t buf[20];
  uint16_t idx_w = 0;
  uint16_t pkt_length = 0;
  uint8_t pkt_chsum = 0;
  
  uint8_t deg_mode = data;

  if(deg_mode == W_DEBUG_USE){
    print_debug_mode = 1;
    debug.print(0,"Debug Mode Set: %02X\n",deg_mode);
  
    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_DEBUG_MODE;     //CMD
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;
  
    Serial.write((byte*)buf,idx_w); 
  }else if(deg_mode == W_DEBUG_NOT_USE){
    print_debug_mode = 0;
    debug.print(0,"Debug Mode Set: %02X\n",deg_mode);
  
    //Frame Type + CMD + Payload
    pkt_length = (1+1+1);
    buf[idx_w++] = 0x7e;
    buf[idx_w++] = (pkt_length >> 8) & 0xFF;
    buf[idx_w++] = pkt_length & 0xFF;
    buf[idx_w++] = (uint8_t)W_CONFIG_RESP;  //Fram Type
    buf[idx_w++] = (uint8_t)W_DEBUG_MODE;     //CMD
    buf[idx_w++] = (uint8_t)0x00;     //Set Success
    pkt_chsum = checksum((char*)&buf[3],pkt_length);
    buf[idx_w++] = pkt_chsum;
  
    Serial.write((byte*)buf,idx_w); 
  }
}




