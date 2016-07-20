#include <mega2560.h>
#include <stdlib.h>
#include <string.h>
#include <delay.h>
#include <stdio.h>
#include "nx_io.h"

#define ADC_VREF_TYPE 0x00

// Read the ADC conversion result
// Analog 0-1024 Port PF0 - PF8 (A0-A7)
// Analog (-512)-512 Port PK0 - PK8 (A8-A15) 
// example
// init_analogpin(A0);
unsigned int analog_read(unsigned char adc_input)
{
      ADMUX = (0<<REFS1)|(1<<REFS0)|adc_input;
      ADCSRA |=(1<<ADSC);
      while(!(ADCSRA&(1<<ADIF)));
      return ADCW;
}

void init_analogpin(unsigned int pin)
{
      
      ACSR=0x80;
      ADCSRB=0x00;
      DIDR1=0x00;
      DIDR0=0x00;
      DIDR2=0x00;
      ADMUX=ADC_VREF_TYPE & 0xff;
      ADCSRA=0x83;
}

void Set_PWM(char ch ,int duty )
{
    switch (ch)
    {
        case 1: DDRB |= (1 << DDB7);
                //OCR0A = 50;
                TCCR0A |= (1 << COM0A1);
                // set non-inverting mode
                TCCR0A |= (1 << WGM01) | (1 << WGM00);
                // set 10bit phase corrected PWM Mode
                TCCR0B |= (1 << CS01);
                // set prescaler to 8 and starts PWM
                OCR0A = duty;
                break;
        case 2: 
                DDRB |= (1 << DDB4);
                 //OCR0A = 50;
                TCCR2A |= (1 << COM2A1);
                // set non-inverting mode
                TCCR2A |= (1 << WGM21) | (1 << WGM20);
                // set 10bit phase corrected PWM Mode
                TCCR2B |= (1 << CS21);
                OCR2A = duty;
                // set prescaler to 8 and starts PWM 
                break;
    }
   
}

