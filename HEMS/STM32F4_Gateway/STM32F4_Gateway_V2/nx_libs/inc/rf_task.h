#ifndef ETH_TASK_H
#define ETH_TASK_H

#include "main.h"
#include "print_debug.h"

void rf_task(void const * argument);
void rf_transmit();
void rf_receiv();

extern void rf_task(void const * argument);

#endif