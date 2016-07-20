#define PA        DDRA
#define PB        DDRB 
#define PC        DDRC 
#define PD        DDRD 
#define PE        DDRE
#define PF        DDRF
#define PG        DDRG
#define PH        DDRH
#define PJ        DDRJ
#define PK        DDRK
#define PL        DDRL

#define A0        0
#define A1        1
#define A2        2
#define A3        3
#define A4        4
#define A5        5        
#define A6        6
#define A7        7
#define A8        8
#define A9        9
#define A10       10
#define A11       11
#define A12       12
#define A13       13
#define A14       14
#define A15       15




#define init_bitout(port,pin) (port)=(1<<pin)
#define init_bitin(port,pin) (port)=(0<<pin)

#define init_portout(port) (port)=(0xff)
#define init_portin(port) (port)=(0x00)

#define Set_bit(port,pin) (port) |= (1 << (pin))
#define Reset_bit(port,pin) (port) &= ~(1 << (pin))

#define Get_bit(PINX,pin) ((PINX)&(1<<pin))

#define Toggle_bit(port,pin) (port) ^= (1 << (pin))

/* 
/////////////////////////////////////////////////////
//Read the ADC conversion result                   //
// Analog 0-1024 Port PF0 - PF8 (A0-A7)            //
// Analog (-512)-512 Port PK0 - PK8 (A8-A15)       //
// example                                         //
// init_analogpin(A0);                             //
// analog_read(A0);                                //
/////////////////////////////////////////////////////
*/
unsigned int analog_read(unsigned char adc_input);
void init_analogpin(unsigned int pin);
/* 
/////////////////////////////////////////////////////
// Output PWM 0-255                                //
// CH1--------->PB7                                //
// CH2--------->PB4                                //
// example                                         //
// Set_PWM(1 ,255);                                //
/////////////////////////////////////////////////////
*/
void Set_PWM(char ch ,int duty );
