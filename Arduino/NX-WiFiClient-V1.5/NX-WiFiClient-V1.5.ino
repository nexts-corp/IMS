/* 
Company : Nexts-Corp
Devper : Hardware Team
Firmware Ver : NX-WiFiClient-V1.3
Arduino IDE : Sketch Arduino 1.6.7
3Party Lib : ESP8266WiFi, EEPROM
Description : Wifi to Uart (WIFI Stream <==> UART API)

Module : NodeMCU V2
Config
       -> Board:NodeMCUV2 (ESP8266 ESP-12E Module)
       -> Upload Using:Serial
       -> CPU Frequency:80 MHz
       -> Flash Size:4M(3M SPIFFS) 
       -> Upload Speed:115200
*/

#include "main.h"
#include "wifi_api.h"
#include "print_debug.h"

//const char* ssid     = "@Next-Wifi-2G";
//const char* password = "mestnics";

const char* ssid     = "BK-Wifi";
const char* password = "pass123456";

int flag_frist_boot = 0;


WiFiClient client;
//the IP address is dependent on your network
IPAddress serv_ip(192, 168, 0, 101);
uint16_t  serv_port = 5000; 
WIFI_API wifi_api_data;
int flag_connect = 0;

extern PrintDebug debug;

typedef struct{
  uint16_t idx_r;
  uint16_t idx_w;
  uint16_t size;
  uint16_t len;
  uint8_t value[512];
}data_t;

data_t data_wifi;
uint16_t pkt_length = 0;
uint8_t pkt_chsum = 0;

uint8_t  checksum(uint8_t *buf, uint16_t len);
void serialEvent();

//#define D0 16
//#define D1 5 // I2C Bus SCL (clock)
//#define D2 4 // I2C Bus SDA (data)
//#define D3 0
//#define D4 2 // Same as "LED_BUILTIN", but inverted logic
//#define D5 14 // SPI Bus SCK (clock)
//#define D6 12 // SPI Bus MISO 
//#define D7 13 // SPI Bus MOSI
//#define D8 15 // SPI Bus SS (CS)
//#define D9 3 // RX0 (Serial console)
//#define D10 1 // TX0 (Serial console)


int w_connect_ap(){
  int connect_count = 10;

  //get data form eeprom
  wifi_api_data.get_dhcp();
  wifi_api_data.get_ip();
  wifi_api_data.get_sn();
  wifi_api_data.get_gw();
  wifi_api_data.get_mac();
  wifi_api_data.get_dns_ip();
  wifi_api_data.get_dns_name();
  wifi_api_data.get_ssid();
  wifi_api_data.get_pwd();
//  wifi_api_data.get_serv_ip();
//  wifi_api_data.get_serv_name();
//  wifi_api_data.get_serv_port();

 // WiFi.begin(ssid, password);
  WiFi.begin(wifi_api_data.w_ssid, wifi_api_data.w_pwd);

  debug.print(0,"Connecting to ");
  do{
    if (WiFi.status() == WL_CONNECTED) {
      flag_connect = 1;
      break;
    }
    debug.print(0,".");
    delay(500);
  }while(--connect_count);
  
  if(flag_connect==1){
    debug.print(0,"module can connect ap.\n");
    wifi_api_data.send_status(STATUS_CONN_AP_OK);
  }else if(flag_connect==0){
    debug.print(0,"module can't connect ap.\n");
    wifi_api_data.send_status(STATUS_CONN_AP_FAIL);
  }
  return flag_connect;
}

void set_config(){
  
  WiFi.begin(wifi_api_data.w_ssid, wifi_api_data.w_pwd);

  flag_connect = w_connect_ap();
  if(flag_connect==1){
    debug.print(0,"module can connect ap.\n");
  }else{
    debug.print(0,"module can't connect ap.\n");
  }
  

  serv_ip[0] = wifi_api_data.w_serv_ip[0];
  serv_ip[1] = wifi_api_data.w_serv_ip[1];
  serv_ip[2] = wifi_api_data.w_serv_ip[2];
  serv_ip[3] = wifi_api_data.w_serv_ip[3];
  
  if (!client.connect(serv_ip, wifi_api_data.w_serv_port)) {
    //Serial.println("connection failed.");
    wifi_api_data.send_status(STATUS_CONN_SERV_FAIL);
    while(1);
    return;
  }else{
    //Serial.println("connected.");  
    wifi_api_data.send_status(STATUS_CONN_SERV_OK);
  }
}

void clear_eep(){
  char buffClear[512];

  /*
   * Clear EEPROM 512 bytes from 0-500 address.
   */
  memset(buffClear,0,sizeof(buffClear));
  EEPROM.put(0, buffClear);
  EEPROM.commit();
  debug.print(0,"clear_eep.\n");
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);

//  if(flag_frist_boot == 0){
//    clear_eep();
//    flag_frist_boot = 1;
//  }
//  if(EEPROM.read(0)==0x00){
//    clear_eep();
//    EEPROM.write(0,0x01);
//    EEPROM.commit();
//  }
  

  
  
  //debug.print(0,"EEPROM.length(%d)\n",EEPROM.length());

  
  
  //Serial.setDebugOutput(true);
  delay(10);

  wifi_api_data.send_status(STATUS_RESTART);

  debug.print(0,"\n");
  debug.print(0,"Company : Nexts-Corp\n");
  debug.print(0,"Modele : NodeMCU V2\n");
  debug.print(0,"Firmware Ver. : NX-WiFiClient-V1.1\n");

  
  
  if(w_connect_ap() == 1){
    debug.print(0,"\n\nConnecting to \n");
    debug.print(0,"SSID: %s\n",ssid);
    
    
//    Serial.println("");
//    Serial.println("WiFi connected");  
//    Serial.print("IP address: ");
//    Serial.println(WiFi.localIP());
//    Serial.print("SN address: ");
//    Serial.println(WiFi.subnetMask());
//    Serial.print("GW address: ");
//    Serial.println(WiFi.gatewayIP());

    uint8_t ip_set[4];
    debug.print(0,"WiFi connected");
    memcpy(&ip_set[0],(uint8_t*)&WiFi.localIP()[0],4);
    debug.print(0,"IP: %d.%d.%d.%d\n",ip_set[0],ip_set[1],ip_set[2],ip_set[3]);
    memcpy(&ip_set[0],(uint8_t*)&WiFi.subnetMask()[0],4);
    debug.print(0,"SN: %d.%d.%d.%d\n",ip_set[0],ip_set[1],ip_set[2],ip_set[3]);
    memcpy(&ip_set[0],(uint8_t*)&WiFi.gatewayIP()[0],4);
    debug.print(0,"GW: %d.%d.%d.%d\n",ip_set[0],ip_set[1],ip_set[2],ip_set[3]);
    
  //  Serial.print("DNS-IP address: ");
  //  Serial.println(WiFi.dnsServerIP());
  
    
    debug.print_payload(0,&WiFi.localIP()[0],4);
    debug.print_payload(0,&WiFi.subnetMask()[0],4);
    debug.print_payload(0,&WiFi.gatewayIP()[0],4);
    //debug.print_payload(0,&WiFi.dnsServerIP()[0],4);
  
//    wifi_api_data.set_ssid((const char*)"");
//    wifi_api_data.set_pwd((const char*)"");
    
    wifi_api_data.set_ip(&WiFi.localIP()[0]);
    wifi_api_data.set_sn(&WiFi.subnetMask()[0]);
    wifi_api_data.set_gw(&WiFi.gatewayIP()[0]);
    //wifi_api_data.set_dns_ip(&WiFi.dnsServerIP()[0]);
    

    uint8_t s_ip[4];
    memcpy(s_ip,wifi_api_data.get_serv_ip(),4);
    serv_ip[0] = s_ip[0];
    serv_ip[1] = s_ip[1];
    serv_ip[2] = s_ip[2];
    serv_ip[3] = s_ip[3];

    serv_port = wifi_api_data.get_serv_port();

    debug.print(0,"s_ip: %d.%d.%d.%d\n",s_ip[0],s_ip[1],s_ip[2],s_ip[3]);
    debug.print(0,"serv_ip: %d.%d.%d.%d\n",serv_ip[0],serv_ip[1],serv_ip[2],serv_ip[3]);
    debug.print(0,"serv_portv_ip: %d\n",serv_port);
    
    if (!client.connect(serv_ip, serv_port)) {
      debug.print(0,"Open Socket failed.\n");
      wifi_api_data.send_status(STATUS_CONNECTED_FAIL);
      //while(1);
      //return;
    }else{
      debug.print(0,"Open Socket Ok.\n");  
      wifi_api_data.send_status(STATUS_CONNECTED_OK);
    }
    
  }

  //memset(&data_wifi,0,sizeof(data_wifi));
}

int value = 0;

void loop() {

  serialEvent();
  //Serial.print("connecting to ");
  
  // This will send the request to the server
  //client.print("Hi Server.\n");
  //wifi_data.send_status(STATUS_RESTART);
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    //Serial.write((byte)client.read());
    data_wifi.value[data_wifi.idx_w++] = client.read();
    ++data_wifi.size;
    if(data_wifi.size>512){
      Serial.println("Buffer Over Flow.");
      data_wifi.size = 0;
      data_wifi.idx_r = 0;
      data_wifi.idx_w = 0;
    }
    delay(1);
  }

  if (!client.connected()) {
    Serial.println();
    debug.print(0,"disconnecting.\n");
    client.stop();
    
    uint8_t s_ip[4];
    memcpy(s_ip,wifi_api_data.get_serv_ip(),4);
    serv_ip[0] = s_ip[0];
    serv_ip[1] = s_ip[1];
    serv_ip[2] = s_ip[2];
    serv_ip[3] = s_ip[3];

    serv_port = wifi_api_data.get_serv_port();

    debug.print(0,"s_ip: %d.%d.%d.%d\n",s_ip[0],s_ip[1],s_ip[2],s_ip[3]);
    debug.print(0,"serv_ip: %d.%d.%d.%d\n",serv_ip[0],serv_ip[1],serv_ip[2],serv_ip[3]);
    debug.print(0,"serv_portv_ip: %d\n",serv_port);
    if (!client.connect(serv_ip, serv_port)) {
      Serial.println("Open Socket failed.");
      wifi_api_data.send_status(STATUS_CONNECTED_FAIL);
      //while(1);
      //return;
    }else{
      Serial.println("Open Socket Ok.");  
      wifi_api_data.send_status(STATUS_CONNECTED_OK);
    }
  }

  
//  int i = 0;
//  if(data_wifi.size>0){
//    pkt_length = (data_wifi.size + 1);
//    pkt_chsum = checksum((char*)&data_wifi.value[data_wifi.idx_r],data_wifi.size);
//    Serial.write(0x7e);
//    Serial.write((pkt_length >> 8) & 0xFF);
//    Serial.write(pkt_length & 0xFF);
//    Serial.write(0x90);
//    Serial.write((byte*)&data_wifi.value[data_wifi.idx_r],data_wifi.size);
//    Serial.write(pkt_chsum);
//
//    
//    
//    data_wifi.idx_r += data_wifi.size;
//    data_wifi.size -= data_wifi.size;
//    if(data_wifi.idx_r == data_wifi.idx_w || (data_wifi.idx_r > 512)){
//      data_wifi.size = 0;
//      data_wifi.idx_r = 0;
//      data_wifi.idx_w = 0;
//    }
//  }

  if(data_wifi.size>0){
    wifi_api_data.send_uart_data(&data_wifi.value[0],data_wifi.size);
    data_wifi.idx_r += data_wifi.size;
    data_wifi.size -= data_wifi.size;
    if(data_wifi.idx_r == data_wifi.idx_w || (data_wifi.idx_r > 512)){
      data_wifi.size = 0;
      data_wifi.idx_r = 0;
      data_wifi.idx_w = 0;
    }
  }

  
}

uint8_t  checksum(char *buf, uint16_t len){
  int i = 0;
  char sum = 0;                                          
  //print_payload(buf,len);
  for (i = 0; i < len; i++) {
      sum += buf[i];
  }                 
  return (0xff - (sum & 0xff));
}

void wifi_api_status(){
  
}

//void serialEventRun(void) {
//  if (Serial.available()) serialEvent();
//}
void serialEvent() {
  uint16_t len = 0;
  uint8_t buf[255] ;
  
  while (Serial.available()){
    //Serial.print((char)Serial.read());
    buf[len++]  = (uint8_t)Serial.read();
  }
  if(len>0){
    wifi_api_data.process(buf,len);
  }
  
}

////Listen for serial input from the console
//void serialEvent() {
//  while (Serial.available()) {
//    char inChar = (char)Serial.read();
//    inputString += inChar;
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}


