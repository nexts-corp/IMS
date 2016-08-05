#ifndef LAN_H
#define LAN_H

#include "main.h"

int8_t lan_write(uint8_t *value, uint16_t len);
int8_t lan_read(uint8_t *value, uint16_t *len);

#endif 