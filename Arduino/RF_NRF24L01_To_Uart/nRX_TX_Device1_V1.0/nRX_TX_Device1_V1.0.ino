#include <SPI.h>
#include <nRF24L01p.h>
#include <string.h>


#define RF_MAX_BUFFER 255
#define UART_MAX_BUFFER 255


int led_built_in = 13;
byte byte_data[3];
unsigned int i = 0;

nRF24L01p receiver(7, 8); //CSN,CE
int iteration = 0;
char vcCode[3];


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
byte uart_Byte[UART_MAX_BUFFER];
unsigned int uart_index_read = 0;
unsigned int uart_index_write = 0;
unsigned int uart_index_count = 0;

byte rf_Byte[UART_MAX_BUFFER];
unsigned int rf_index_read = 0;
unsigned int rf_index_write = 0;
unsigned int rf_index_count = 0;


void rfUpdate();
void setup() {
  delay(150);
  Serial.begin(9600);

  pinMode(led_built_in, OUTPUT);
  digitalWrite(led_built_in, LOW);

  SPI.begin();
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);
  receiver.TXaddress("ALL");
  receiver.RXaddress("ALL");
  //receiver.dataRate(250);
  receiver.init();
  delay(1000);
  Serial.println("RF RDY.");
}

void loop() {
  rfUpdate();
  if (rf_index_count > 0) {
    Serial.write(rf_Byte[rf_index_read++]);
    rf_index_count--;
    if (rf_index_read == rf_index_write || (rf_index_count <= 0)) {
      rf_index_write = 0;
      rf_index_read = 0;
    }
  }

  if (uart_index_count > 0) {
    receiver.txPL(uart_Byte[uart_index_read++]);
    receiver.send(SLOW);
    uart_index_count--;
    if (uart_index_read == uart_index_write || (uart_index_count <= 0)) {
      uart_index_write = 0;
      uart_index_read = 0;
    }
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {

  while (Serial.available()) {
    // get the new byte:
    uart_Byte[uart_index_write++] = (byte)Serial.read();
    uart_index_count++;
    if (uart_index_write >= UART_MAX_BUFFER) {
      uart_index_read = 0;
      uart_index_write = 0;
    }

    if (uart_index_read >= UART_MAX_BUFFER) {
      uart_index_read = 0;
      uart_index_write = 0;
    }
  }
}


void rfUpdate() {
  while (receiver.available()) {
    receiver.read();
    receiver.rxPL(byte_data, 1);
    rf_Byte[rf_index_write++] = byte_data[0];
    rf_index_count++;
    if (rf_index_write >= RF_MAX_BUFFER) {
      rf_index_read = 0;
      rf_index_write = 0;
    }

    if (rf_index_read >= RF_MAX_BUFFER) {
      rf_index_read = 0;
      rf_index_write = 0;
    }
  }
}





