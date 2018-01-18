#include <inavr.h>
#include <iom16.h>

#define _FILL_FACTOR 25 // in procente (%)
#define _SIGNAL_FREQ 2000 // Hz
#define _CLOCK_FREQ 4000000 // Hz

//int cnt = 0;//Why?

#pragma vector=TIMER1_OVF_vect
__interrupt void T1int(){
// ++cnt; 
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
  //OCR1B = _FILL_FACTOR * ICR1 / 100; //Foloseste canalul B  
}

void main(){
 configureTimer1();
 while(1){
  //Do nothing 
 }
}