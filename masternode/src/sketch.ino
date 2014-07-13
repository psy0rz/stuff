/** Master node, doesnt do anything except relay messages between serial port and nrf24 network
*/

#include <EEPROM.h>

#include "pin_config.h"
#include "messaging.h"
#include "eeprom_config.h"
#include "utils.h"

#include "printf.h"

//called when receiving an event that was received via network or serial 
void Msg::handle(uint16_t from, char * event,  char * par)
{
  //when debugging parsing issues
  // Serial.println("from: ");
  // Serial.println(from);
  // Serial.println("event: ");
  // Serial.println(event);
  // Serial.println("par: ");
  // Serial.println(par);

  
  // if (strcmp_P(event, PSTR("some.event"))==0)
  // {
  //   ...do stuff
  //   return;
  // }


}


void setup() 
{
  config_read();

  Serial.begin(115200);
  #ifdef SERIAL_DEBUG
  printf_begin();
  #endif

  msg.begin();

  //pin 9 and 10 different pwm freq.  122.5hz
//  TCCR1B = TCCR1B & 0b11111000 | 4;


  
}




void loop()
{
  
  //send/receive network and serial messages
  msg.loop(); 

}

