#include <mega2560.h>  
#include <stdio.h>           
#include <string.h>
#include <ctype.h>         
#include <delay.h>


#include "nx_usart.h"
/*============================================================*/
/* Baudrate set 9600 14400 38400                              */
/*============================================================*/
void init_uart(char port , int Baudrate)
{     switch (port)
      {
            case 0:
                  if(Baudrate==9600)
                  {
                        UCSR0A=0x00;
                        UCSR0B=0x98;
                        UCSR0C=0x06;
                        UBRR0H=0x00;
                        UBRR0L=0x33;  
                  }
                  else if(Baudrate==14400)
                  {
                        UCSR0A=0x00;
                        UCSR0B=0x98;
                        UCSR0C=0x06;
                        UBRR0H=0x00;
                        UBRR0L=0x22;
                  }
                  else if(Baudrate==38400)
                  {
                      UCSR0A=0x00;
                      UCSR0B=0x98;
                      UCSR0C=0x06;
                      UBRR0H=0x00;
                      UBRR0L=0x0C;  
                  }
                  break;

            case 1:
                  if(Baudrate==9600)
                  {
                      UCSR1A=0x00;
                      UCSR1B=0x98;
                      UCSR1C=0x06;
                      UBRR1H=0x00;
                      UBRR1L=0x33;  
                  }
                  else if(Baudrate==14400)
                  {
                        UCSR1A=0x00;
                        UCSR1B=0x98;
                        UCSR1C=0x06;
                        UBRR1H=0x00;
                        UBRR1L=0x22;
                  }
                  else if(Baudrate==38400)
                  {
                      UCSR1A=0x00;
                      UCSR1B=0x98;
                      UCSR1C=0x06;
                      UBRR1H=0x00;
                      UBRR1L=0x0C; 
                  }
                  break;

            case 2:
                   if(Baudrate==9600)
                  {
                      UCSR2A=0x00;
                      UCSR2B=0x98;
                      UCSR2C=0x06;
                      UBRR2H=0x00;
                      UBRR2L=0x33;  
                  }
                  else if(Baudrate==14400)
                  {
                        UCSR2A=0x00;
                        UCSR2B=0x98;
                        UCSR2C=0x06;
                        UBRR2H=0x00;
                        UBRR2L=0x22;
                  }
                  else if(Baudrate==38400)
                  {
                      UCSR2A=0x00;
                      UCSR2B=0x98;
                      UCSR2C=0x06;
                      UBRR2H=0x00;
                      UBRR2L=0x0C; 
                  } 
                  break;

            case 3:
                  if(Baudrate==9600)
                  {
                      UCSR3A=0x00;
                      UCSR3B=0x98;
                      UCSR3C=0x06;
                      UBRR3H=0x00;
                      UBRR3L=0x33;  
                  }
                  else if(Baudrate==14400)
                  {
                        UCSR3A=0x00;
                        UCSR3B=0x98;
                        UCSR3C=0x06;
                        UBRR3H=0x00;
                        UBRR3L=0x22;
                  }
                  else if(Baudrate==38400)
                  {
                      UCSR3A=0x00;
                      UCSR3B=0x98;
                      UCSR3C=0x06;
                      UBRR3H=0x00;
                      UBRR3L=0x0C; 
                  } 
                  break;  
      }
      
}

/*============================================================*/
#pragma used+
void putchar0(char c)
{
    while ((UCSR0A & DATA_REGISTER_EMPTY)==0);
    UDR0=c;
}
#pragma used-
#pragma used+
void putchar1(char c)
{
    while ((UCSR1A & DATA_REGISTER_EMPTY)==0);
    UDR1=c;
}
#pragma used-
#pragma used+
void putchar2(char c)
{
    while ((UCSR2A & DATA_REGISTER_EMPTY)==0);
    UDR2=c;
}
#pragma used-
#pragma used+
void putchar3(char c)
{
    while ((UCSR3A & DATA_REGISTER_EMPTY)==0);
    UDR3=c;
}
#pragma used-

/*============================================================*/
void printDebug(flash char *fmtstr, ...)
{
    char textBuffer[256];
    int num=0;
    va_list argptr; 
    
    va_start(argptr, fmtstr);
    vsprintf(textBuffer,fmtstr,argptr); 
    
    send_uart(1, textBuffer); 
      
    va_end(argptr);        
    
    return;        
}
/*============================================================*/

/*============================================================*/
void send_uart(char port,char *buffer)
{
    char i = 0;
    
    switch (port)
    {
        case 0: while(buffer[i] != 0)    
                {
                    putchar0(buffer[i]);
                    i++;
                }
                break;

        case 1: while(buffer[i] != 0)    
                {
                    putchar1(buffer[i]);
                    i++;
                }
                break;

        case 2: while(buffer[i] != 0)    
                {
                    putchar2(buffer[i]);
                    i++;
                }
                break;

        case 3: while(buffer[i] != 0)    
                {
                    putchar3(buffer[i]);
                    i++;
                }
                break;
    }    
}
/*============================================================*/