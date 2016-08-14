#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

//void serialEvent();
uint8_t  checksum(char *buf, uint16_t len);
void clear_eep();

//extern void serialEventRun(void) __attribute__((weak));

#endif 
