#ifndef DEBUG_H
#define DEBUG_H

#include "uart.h"
                                            
extern unsigned int hwCalId;

void debug_req(void);

void printDebug(flash char *fmtstr, ...);                  
void print_payload(const unsigned char *payload, int len);  
void iGetSensorValue();

#endif 
   