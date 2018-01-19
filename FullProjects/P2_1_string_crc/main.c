#include <inavr.h>
#include <iom16.h>

#include "crc.h"
#include "usart.h"
unsigned int crc;
unsigned char rxData = 0;
unsigned int index;
char dataBuff[40];


#pragma vector = USART_RXC_vect
__interrupt void USART_Receive(void){
  rxData = UDR;
  dataBuff[index++] = rxData;
  UDR = rxData;
}

void main(){
  USART_initialize(BAUD_RATE);
  __enable_interrupt();
  USART_transmit_string("Ready!\n\r\0");
  while (0xBEEF) {
    if (rxData == '@'){
      index--;//Remove @ from input buffer
      crc = crc16wtable2(0, (unsigned int)dataBuff, sizeof(char)*index);
      index = 0;
      rxData = 0;
      char buff[20];
      sprintf(buff, "\r\nCRC-%04X\r\n", crc); 
      USART_transmit_string(buff);
    }
  }
}