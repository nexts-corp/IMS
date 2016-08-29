#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

//void serialEvent();
uint8_t  checksum(char *buf, uint16_t len);
void clear_eep();
uint8_t connected_check();
uint8_t connect_server();
void uart_flush();
void wifi_flush();

//extern void serialEventRun(void) __attribute__((weak));

#endif 
