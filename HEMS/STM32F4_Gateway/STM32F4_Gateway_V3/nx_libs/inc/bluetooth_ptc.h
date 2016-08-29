#ifndef BLUETOOTH_PTC_H
#define BLUETOOTH_PTC_H

#include "main.h"

int8_t bt_parser(uint8_t *val, uint16_t length);
int8_t convert_char2uint(char val , uint8_t *out_val);

#endif