#ifndef __SERIALA_H__
#define __SERIALA_H__


#include <inavr.h>
#include <iom16.h>
#include <string.h>
#include <stdio.h>
//pentru functia sprintf / sscanf
#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1)

#define _USART_INT // Intrerupere pentru receptia datelor

void USART_initialize(unsigned short int baud_rate){
  // seteazã baud rate 
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  // activeazã transmisia si receptia la iesire
  UCSRB = (1 << RXEN) | (1 << TXEN); 
  /* seteazã pinul TXD: ie.ire */
  DDRD |= (1 << PD1);
  /* seteazã pinul RXD: intrare */
  DDRD &= ~(1 << PD0);
#ifdef _USART_INT
  // activeazã întreruperea 
  UCSRB |= (1 << RXCIE); 
  __enable_interrupt();
#endif
}
void USART_transmit(unsigned char data){
  {
    // asteapta pâna ce se termina de transmis toate datele 
    //si dupa trece la urmatoarele informatii 
    while (!( UCSRA & (1 << UDRE)))
    {
      ;
    }
    UDR = data;
  } 
}


#ifndef _USART_INT
unsigned char USART_Receive( void ){
  /* A.teapta recep.ionarea datelor */
  while ( !(UCSRA & (1<<RXC)) )  {
    ;  
  }
  // Preia si returneaza datele receptionate din buffer
  return UDR;
}
#else

#pragma vector = USART_RXC_vect
__interrupt void USART_Receive(void);

#endif

void USART_transmit_string(const char * string){
  //send a string
  int len = strlen(string);
  int i;
  for(i = 0; i<len; ++i){
    USART_transmit((unsigned char) string[i]);
  }
}

#endif