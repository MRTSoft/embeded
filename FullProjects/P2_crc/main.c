#include <inavr.h>
#include <iom16.h>

#include "crc.h"
#include "usart.h"

void main(){
  unsigned int my_crc_t = crc16wtable(0,0, 0x4000-2);
//  unsigned int my_crc = crc16(
  int i;
  while(1){
    i++;
    i--;
  }
}