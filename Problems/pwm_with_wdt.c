#include <iom16.h>
#include <inavr.h>

__no_init int state;

int main(void){
  //Set watchdog prescale to 000 -> 0.52 s time-out
  // See Table 17
  WDTCR |= (1<<WDE) | (0<<WDP2) | (0<<WDP1) | (0<<WDP0);
  DDRD |= (1<<PD5);
  //PORTD &= (1<<PD5);
  switch(state){
  case 0:
    //set PD5;
    PORTD |= (1<<PD5);
    state = 1;
    break;
  case 1:
    //reset PD5;
    PORTD &= ~(1<<PD5);
    state = 2;
    break;
  case 2:
    //reset PD5;
    PORTD &= ~(1<<PD5);
    state = 0;
    break;
  default:
    state = 0;
  }
  while(1){ 
    ;
  }
}