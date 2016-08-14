#ifndef WIFI_API_H
#define WIFI_API_H

#include "main.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* qsort, bsearch, NULL */
}
#endif

#include <EEPROM.h>

//Status
#define STATUS_RESTART            0x10
#define STATUS_CONNECTED_OK       0x20
#define STATUS_CONNECTED_FAIL     0x30
#define STATUS_CONN_AP_OK         0x70
#define STATUS_CONN_AP_FAIL       0x71
#define STATUS_CONN_SERV_OK       0x72
#define STATUS_CONN_SERV_FAIL     0x73

//Frame Type
#define W_DATA_RECEIV           0x10
#define W_DATA_TRASNSMIT        0x90
#define W_CONFIG_RECEIV         0x09
#define W_CONFIG_RESP           0x88
#define W_STATUS                0x8A


//Command (CMD)
#define W_GET_DHCP        0x10
#define W_GET_IP          0x11
#define W_GET_SN          0x12
#define W_GET_GW          0x13
#define W_GET_MAC         0x14
#define W_GET_DNS_IP      0x15
#define W_GET_DNS_NAME    0x16
#define W_GET_SERV_IP     0x17
#define W_GET_SERV_NAME   0x18
#define W_GET_SERV_PORT   0x19
#define W_GET_SSID        0x1A
#define W_GET_PWD         0x1B

#define W_SET_DHCP        0x20
#define W_SET_IP          0x21
#define W_SET_SN          0x22
#define W_SET_GW          0x23
#define W_SET_MAC         0x24
#define W_SET_DNS_IP      0x25
#define W_SET_DNS_NAME    0x26
#define W_SET_SERV_IP     0x27
#define W_SET_SERV_NAME   0x28
#define W_SET_SERV_PORT   0x29
#define W_SET_SSID        0x2A
#define W_SET_PWD         0x2B



#define W_RESTART         0x30
#define W_OPEN            0x31
#define W_CLOSE           0x32

//Config response
#define W_CONNECT_SERV        0x40
#define W_DIS_CONNECT_SERV    0x41


#define W_SUCCESS             0x00
#define W_ERROR               0x01
#define W_DEBUG_USE           0x02
#define W_DEBUG_NOT_USE       0x03


#define W_CLR_EEP            0x50
#define W_DEBUG_MODE          0x51


//#define ARD_DHCP        0 //(1 byte)
//#define ARD_IP          (ARD_DHCP + 1) //1 <=> 4 (4 bytes)
//#define ARD_SN          (ARD_DHCP + ARD_IP + 3) //5 <=> 8 (4 bytes)
//#define ARD_GW          (ARD_DHCP + ARD_IP + ARD_SN + 3) //9 <=> 12 (4 bytes)
//#define ARD_MAC         (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + 5) //13 <=> 17 (4 bytes)
//#define ARD_DNS_IP      (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + 3) //18 <=> 21 (4 bytes)
//#define ARD_DNS_NAME    (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + 32) //22 <=> 53 (32 bytes)
//#define ARD_SERV_IP     (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + 3) //54 <=> 57 (4 bytes)
//#define ARD_SERV_NAME   (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + 32) //58 <=> 89 (32 bytes)
//#define ARD_SERV_PORT   (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + ARD_SERV_NAME + 1) //90 <=> 91 (2 bytes)
//#define ARD_SSID        (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + ARD_SERV_NAME + ARD_SERV_PORT + 32) //92 <=> 123 (32 bytes)
//#define ARD_PWD         (ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + ARD_SERV_PORT + ARD_SERV_PORT + ARD_SSID + 32) //124 <=> 155 (32 bytes)

#define ARD_DHCP        (uint16_t)0 //(1 byte)
#define ARD_IP          (uint16_t)1 //(ARD_DHCP + 1) //1 <=> 4 (4 bytes)
#define ARD_SN          (uint16_t)5 //(ARD_DHCP + ARD_IP + 3) //5 <=> 8 (4 bytes)
#define ARD_GW          (uint16_t)9 //(ARD_DHCP + ARD_IP + ARD_SN + 3) //9 <=> 12 (4 bytes)
#define ARD_MAC         (uint16_t)13 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + 5) //13 <=> 17 (4 bytes)
#define ARD_DNS_IP      (uint16_t)18 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + 3) //18 <=> 21 (4 bytes)
#define ARD_DNS_NAME    (uint16_t)22 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + 32) //22 <=> 53 (32 bytes)
#define ARD_SERV_IP     (uint16_t)54 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + 3) //54 <=> 57 (4 bytes)
#define ARD_SERV_NAME   (uint16_t)58 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + 32) //58 <=> 89 (32 bytes)
#define ARD_SERV_PORT   (uint16_t)90 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + ARD_SERV_NAME + 1) //90 <=> 91 (2 bytes)
#define ARD_SSID        (uint16_t)92 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + ARD_SERV_NAME + ARD_SERV_PORT + 32) //92 <=> 123 (32 bytes)
#define ARD_PWD         (uint16_t)124 //(ARD_DHCP + ARD_IP + ARD_SN + ARD_GW + ARD_MAC + ARD_DNS_IP + ARD_DNS_NAME + ARD_SERV_IP + ARD_SERV_PORT + ARD_SERV_PORT + ARD_SSID + 32) //124 <=> 155 (32 bytes)


void protocol_proc();

class WIFI_API{
  private:

  
  
  public:

  char w_module_name[32];
  char w_firmware_version[32];
  char w_ssid[32];
  char w_pwd[32];//password

  /////My
  uint8_t w_dhcp;
  uint8_t w_ip[4];
  uint8_t w_sn[4];
  uint8_t w_gw[4];
  uint8_t w_mac[6];
  uint8_t w_dns_ip[4];
  char w_dns_name[33];
  /////Server
  uint8_t w_serv_ip[4];
  char w_serv_name[33];
  uint16_t w_serv_port;
  
  WIFI_API();
  uint8_t get_dhcp();
  uint8_t* get_ip();
  uint8_t* get_sn();
  uint8_t* get_gw();
  uint8_t* get_mac();
  uint8_t* get_dns_ip();
  char* get_dns_name();

  char* get_ssid();
  char* get_pwd();

  uint8_t* get_serv_ip();
  char* get_serv_name();
  uint16_t get_serv_port();

  void set_dhcp(uint8_t data);
  void set_ip(uint8_t *data);
  void set_sn(uint8_t *data);
  void set_gw(uint8_t *data);
  void set_mac(uint8_t *data);
  void set_dns_ip(uint8_t *data);
  void set_dns_name(char *data);

  void set_ssid(char* data);
  void set_pwd(char* data);

  void set_serv_ip(uint8_t *data);
  void set_serv_name(char* data);
  void set_serv_port(uint16_t data);


  
  void send_status(uint8_t data);
  void send_uart_data(uint8_t *data, uint16_t len);
  void send_wifi_data(uint8_t *data, uint16_t len);
  void send_event(uint8_t data);

  void response_config(int8_t cmd , uint8_t *data, uint16_t len);

  int8_t process(uint8_t *value, uint16_t len);

  uint8_t  checksum(char *buf, uint16_t len);
  
};

void set_debug_mode(uint8_t data);

#endif 
