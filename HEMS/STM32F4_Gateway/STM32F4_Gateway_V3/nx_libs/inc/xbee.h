#ifndef XBEE_H
#define XBEE_H

#include "main.h"

#define ATCMD       (int8_t)0x08
#define ATCMQ       (int8_t)0x09
#define ZTR         (int8_t)0x10
#define EAZCF       (int8_t)0x11
#define RCR         (int8_t)0x17
#define CSR         (int8_t)0x21
#define ATCMR       (int8_t)0x88
#define MDS         (int8_t)0x8A
#define ZTS         (int8_t)0x8B
#define ZRPKT       (int8_t)0x90	//ZigBee Receive Packet
#define ZERI        (int8_t)0x91
#define ZIORXI      (int8_t)0x92
#define NI_IND      (int8_t)0x95    
#define RCRES       (int8_t)0x97

#define XBEE_TIMEOUT 0  

int8_t xbee_write(uint8_t *value, uint16_t len);
int8_t xbee_read(uint8_t *value, uint16_t *len);

int xbee_processPacket(uint8_t *buf, int len);

#endif 