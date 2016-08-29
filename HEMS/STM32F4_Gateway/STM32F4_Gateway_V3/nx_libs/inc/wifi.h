#ifndef WIFI_H
#define WIFI_H

#include "main.h"

//Status
#define STATUS_RESTART            0x10
#define STATUS_CONNECTED_OK       0x20
#define STATUS_CONNECTED_FAIL     0x30
#define STATUS_CONN_AP_OK         0x70
#define STATUS_CONN_AP_FAIL       0x71
#define STATUS_CONN_SERV_OK       0x72
#define STATUS_CONN_SERV_FAIL     0x73

//Frame Type
#define W_DATA_RECEIV           0x90
#define W_DATA_TRASNSMIT        0x10
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
#define W_GET_CONNECTED_SERV 0x1C

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
#define W_SET_CONNECTED_SERV 0x2C
#define W_SET_CONNECT_SERV 0x2D



#define W_RESTART         0x30
#define W_OPEN            0x31
#define W_CLOSE           0x32

//Config response
#define W_CONNECT_SERV        0x40
#define W_DIS_CONNECT_SERV    0x41

#define W_CLR_EEP             0x50
#define W_DEBUG_MODE          0x51
#define W_DNS_IP_MODE         0x52

#define W_SUCCESS             0x00
#define W_ERROR               0x01
#define W_DEBUG_USE           0x02
#define W_DEBUG_NOT_USE       0x03

int8_t wifi_write(uint8_t *value, uint16_t len);
int8_t wifi_read(uint8_t *value, uint16_t *len);

int8_t wifi_transmit(uint8_t *value, uint16_t len);

int8_t wifi_config(uint8_t cmd, uint8_t *value, uint16_t len);
int8_t wifi_connect(const char * serv_name,uint16_t port);
int8_t wifi_connect_ip(uint8_t * serv_ip,uint16_t port);
int8_t wifi_connect_ap(const char * ssid,const char * pwd);
int8_t wifi_connected();
int8_t wifi_connect_serv();
int8_t wifi_debug(uint8_t data);
int8_t wifi_dns(uint8_t data);
int8_t wifi_clear_mem(uint8_t data);
void wifi_data_flush();

int8_t wifi_api_packer(uint8_t frame_type, uint8_t cmd, uint8_t *value, uint16_t len);
int8_t wifi_api_parser(uint8_t *value, uint16_t len);
int8_t wifi_api_parser_output(uint8_t *value, uint16_t len, uint8_t *val_out, uint16_t *len_out);
uint8_t  wifi_checksum(char *buf, uint16_t len);

void wifi_mqtt_callback(uint8_t *topic_val, uint16_t topic_len, uint8_t *payload_val, uint16_t payload_len, uint16_t packet_id);

#endif 