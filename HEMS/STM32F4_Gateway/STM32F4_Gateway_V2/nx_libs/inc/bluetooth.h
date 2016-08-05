#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "main.h"

int8_t bt_write(uint8_t *value, uint16_t len);
int8_t bt_read(uint8_t *value, uint16_t *len);

#endif 