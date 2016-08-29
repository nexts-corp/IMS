#ifndef NETWORK_H
#define NETWORK_H

#include "main.h"

//Network Protocol
#define NET_MQTT_PTC		(int8_t)0x10
#define NET_HTTP_PTC		(int8_t)0x11
#define NET_TCP_PTC			(int8_t)0x12
#define NET_UDP_PTC			(int8_t)0x13

//Network Module
#define NET_WIFI				(int8_t)0x20
#define NET_LAN					(int8_t)0x21
#define NET_RF					(int8_t)0x22
#define NET_GSM					(int8_t)0x23

//Network Status
#define NET_ENABLE			(int8_t)0x30
#define NET_DISABLE			(int8_t)0x31

typedef struct{
	uint8_t length;
	uint8_t value[256];
}network_data_t;

typedef struct{
	int8_t enable;
	int8_t protocol;
	int8_t module;
}network_t;




void net_enable(int8_t enable);

void net_protocol(int8_t protocol_type);

void net_module(int8_t module);

int8_t net_connect();
void net_callback();
void net_mqtt_callback(uint8_t *topic_val, uint16_t topic_len, uint8_t *payload_val, uint16_t payload_len, uint16_t packet_id);

#endif 