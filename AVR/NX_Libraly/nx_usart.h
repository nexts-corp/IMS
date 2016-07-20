#ifndef UART_H
#define UART_H

#define RXB8 1
#define TXB8 0

#define UPE 2

#define DOR 3

#define FE 4

#define UDRE 5

#define RXC 7

#define FRAMING_ERROR           (1<<FE)
#define PARITY_ERROR            (1<<UPE)
#define DATA_OVERRUN            (1<<DOR)
#define DATA_REGISTER_EMPTY     (1<<UDRE)
#define RX_COMPLETE             (1<<RXC)


void init_uart(char port , int Baudrate);
void putchar0(char c);
void printDebug(flash char *fmtstr, ...);
void send_uart(char port,char *buffer);


#endif