#ifndef XBEE_H
#define XBEE_H

#include "main.h"

int8_t xbee_write(uint8_t *value, uint16_t len);
int8_t xbee_read(uint8_t *value, uint16_t *len);

#endif 