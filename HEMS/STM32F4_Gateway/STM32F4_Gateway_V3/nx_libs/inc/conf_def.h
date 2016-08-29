#ifndef CONF_DEF_H
#define CONF_DEF_H

#include "main.h"

//Module
#define M_LAN						0x11
#define M_WIFI					0x12

#define METH_STATIC			0x21
#define METH_DHCP				0x22

#define PTC_MQTT				0x31
#define PTC_HTTP				0x32
#define PTC_SOCKET			0x33

//Wifi Security
#define W_WPA						0x41
#define W_WPA2					0x42

//Wifi Encryp
#define W_TKIP					0x43
#define W_AES						0x44

typedef struct{
	uint8_t ip[4];
	uint8_t sn[4];
	uint8_t gw[4];
	uint8_t dns_ip[4];
	uint8_t dns_name[32];
}ip_adr_t;

typedef struct{
	uint8_t user_len;
	uint8_t pwd_len;
	uint8_t user;
	uint8_t pwd;
}authen_t;

typedef struct{
	uint8_t module;
	uint8_t method;
	ip_adr_t my_ip;
	ip_adr_t serv_ip;
	authen_t wifi_authen;
}eth_t;

//device
typedef struct{
	uint8_t end_sn[8]; //end device serial number
	uint8_t join_stat;
}xbee_dev_t;


typedef struct{
	uint8_t dev_count;
	xbee_dev_t xbee_dev[10];
}xbee_table_t;


extern eth_t eth_value;
extern xbee_table_t xbee_table;


#endif 