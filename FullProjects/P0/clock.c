#include <inavr.h>
#include <iom16.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <inavr.h>
#include <iom16.h>

#define F_OSC 4000000
#define BAUD 19200
#define BAUD_RATE (F_OSC/16/BAUD - 1)

void USART_initialize(unsigned short int baud_rate);
void USART_transmit(unsigned char data);
unsigned char USART_Receive( void );
int mode;
#define _CLOCK 1
#define _SET 2
char inBuff[9];
int buffPoz;
int dataReady = 0;

typedef struct _timp {
  unsigned char hh;
  unsigned char mm;
  unsigned char ss;
  unsigned int nnnn;
} Timp;

Timp globalClock;


#pragma vector = USART_RXC_vect

__interrupt void interrupt_routine_USART_RXC(void){
  char aux = UDR;
  //mode = _SET;
  //asdasd
  //UDR = aux;
  inBuff[buffPoz] = aux;
  buffPoz++;
  
  //if (buffPoz >= 8){
   //seteaza ceas 
  // buffPoz = 0;
   //mode = _CLOCK;

  //}
  if (aux == '\n'){
   buffPoz = 0;
   //mode = _CLOCK;
  }
}


void USART_initialize(unsigned short int baud_rate){
  
  UBRRH = (unsigned char)(baud_rate >> 8);
  UBRRL = (unsigned char)(baud_rate & 0xFF);
  
  UCSRB = (1 << RXEN) | (1 << TXEN); 
  
  DDRD |= (1 << PD1);
  
  DDRD &= ~(1 << PD0);
  
  UCSRB |= (1 << RXCIE);
}

void USART_transmit(unsigned char data)
{
  
  while (!( UCSRA & (1 << UDRE)));
  UDR = data;
}

void sendTime(){
  char * buff;
  buff = (char *)malloc(sizeof(char) * 11);
  sprintf(buff, "%02d:%02d:%02d\n\r", globalClock.hh, globalClock.mm, globalClock.ss);
  unsigned i, n;
  n = strlen(buff);
  for(i = 0; i < n; ++i) {
    USART_transmit((unsigned char)buff[i]); 
  }
  free(buff);
}

void incrementClock(){
  globalClock.ss++;
  if (globalClock.ss >= 60){
    globalClock.mm++;
    globalClock.ss = 0;
    if (globalClock.mm >= 60){
      globalClock.hh = (globalClock.hh + 1) % 24; // Data loss
      globalClock.mm = 0;
    }
  }
}

void setClock(unsigned char hh, unsigned char mm, unsigned char ss, int nnnn)
{
  globalClock.hh = hh;
  globalClock.mm = mm;
  globalClock.ss = ss;
  globalClock.nnnn = nnnn;
  
}

#pragma vector = TIMER1_OVF_vect
__interrupt void onEachSecond()
{
  //1 Reset the timer
  TCNT1 = 3035;
  //2 Increment the clock
  //incrementClock();
  //USART_transmit('*');
  //3 Send the data - NOT HERE!!!
  dataReady = 1;
}

void updateTime(){
 unsigned int h,m,s;
 sscanf(inBuff, "%u:%u:%u", &h, &m, &s);
 //USART_transmit((unsigned char) h + 'A');
 setClock(h,m,s,0);
}

void main() {
  USART_initialize(BAUD_RATE);
  setClock(0,0,0,0);
  TCNT1 = 3035;
  TIMSK |= (1<<TOIE1); // Enable timer 1 overflow interrupt
  TCCR1B |= (1<<CS11) | (1<<CS10);
  dataReady = 0;
  mode = _CLOCK;
  __enable_interrupt();

  while(1){
    if (dataReady == 1){
      incrementClock();
      //if (mode == _CLOCK){
      sendTime();
      //}
      if (buffPoz != 0){
        USART_transmit('>');
        for(int i=0; i<buffPoz; ++i){
         USART_transmit(inBuff[i]);
        }
        USART_transmit('\n');
        USART_transmit('\r');
      }
      if (buffPoz >= 8){
       updateTime(); 
       buffPoz = 0;
      }
      dataReady = 0;
    }
  }  
}