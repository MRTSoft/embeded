#include <inavr.h>
#include <iom16.h>
#include "usart.h"

#define _FILL_FACTOR 95 // in procente (%)
#define _SIGNAL_FREQ 20000 // Hz
#define _CLOCK_FREQ 4000000 // Hz

int cnt;//Why?
int direction;
#define _UP 1
#define _DOWN 2
int realFillFactor;
int fillFactorStep;
int changeInOverflows;//Nr de overflow-uri pana se schimba directia

#pragma vector=TIMER1_OVF_vect
__interrupt void T1int(){
}

#pragma vector=TIMER0_COMP_vect
__interrupt void changeFillFactor(){
  switch(direction){
  case _UP:
    OCR1A += fillFactorStep;
    realFillFactor++;
    break;
  case _DOWN:
    OCR1A -= fillFactorStep;
    realFillFactor--;
    break;
  default:
    direction = _UP;
    realFillFactor = 5;
    OCR1A = 5 * fillFactorStep;
  }
  if (realFillFactor <= 5){
    direction = _UP;
    realFillFactor = 5;
    OCR1A = 5 * fillFactorStep;
  }
  
  if (realFillFactor >= 95){
    direction = _DOWN;
    realFillFactor = 95;
    OCR1A = 95 * fillFactorStep;
  }
}

void configureTimer1(){
  
  //Seteaza pin-ul de output
  //PD5 -> OC1A
  //PD4 -> OC1B
  
  DDRD |= (1<<PD5);
  PORTD &= (1<<PD5);
  
  //WGM13 : 10 = 1110 - Fast PWN mode
  TCCR1B |= (1<<WGM13) | (1<<WGM12);
  TCCR1A |= (1<<WGM11);
  
  //Fara prescaler CS12:10 = 001
  //     CS1 2.1.0  
  //    x8 - 0.1.0
  //   x64 - 0.1.1
  //  x256 - 1.0.0
  // x1024 - 1.0.1
  TCCR1B |= (1<<CS10);
  unsigned long _PRESCALER = 1;
  //Compare output mode
  // COM1A/B1:0
  // 10 - normal PWM mode
  // 11 - inverting mode
  TCCR1A |= (1 << COM1A1); // Foloseste canalul A
  //TCCR1A |= (1<<COM1B1); // Foloseste canalul B
  
  //Enable overflow interrupt - But why?
  //TIMSK |= (1<<TOIE1);
  //__enable_interrupt();
  
  //Setare frecventa semnal
  //Counter-ul se reseteaza cand ajunge la ICR1
  long long icr_val;
  icr_val =  _CLOCK_FREQ / (_SIGNAL_FREQ * _PRESCALER);
  ICR1 = icr_val;
  
  // Seteaza fill factor-ul
  long long ocr_val;
  ocr_val = ((long long)_FILL_FACTOR * (long long)ICR1) / 100; //Foloseste canalul A
  OCR1A = ocr_val;
  fillFactorStep = ((long long) ICR1) / 100;
  realFillFactor = _FILL_FACTOR;
  direction = _UP;
  //OCR1B = _FILL_FACTOR * ICR1 / 100; //Foloseste canalul B  
}

void configureTimer0(){
  // Configure timer 1 to measure 1/90 of a second
  // Execute a compare interrupt when this happens
  
  // The prescaler should be at least 256
  TCCR0 |= (1<<CS02);
  
  // Execution mode is clear timer on compare
  // WGM01:0 = 10
  TCCR0 |= (1<<WGM01);
  
  TIMSK |= (1<<OCIE0);
  OCR0 = 174; // aprox 0.01 s
  __enable_interrupt();
}

void sendFillFactor(){
  char buff[15];
  sprintf(buff, "<FU-%02d%%>\r\n", realFillFactor);
  USART_transmit_string(buff);
  if (realFillFactor == 5 || realFillFactor == 95){
   sprintf(buff, "<- - - - \r\n");
   USART_transmit_string(buff);
  }
}

void main(void){
 configureTimer1();
 configureTimer0();
 USART_initialize(BAUD_RATE);
 int oldFillFactor = 0;
 while(1){
   if (oldFillFactor != realFillFactor){
    sendFillFactor();
    oldFillFactor = realFillFactor;
   }
 }
}