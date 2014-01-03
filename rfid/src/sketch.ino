/* RFID reader and unlocker, using minimal memory. Can learn up to 100 tags. 
 * (C)2013 Edwin Eefting edwin@datux.nl
 * Released under GPL
 * 
 * Using basic RDM630 reader: http://dx.com/p/125k-rfid-card-reader-module-rdm630-series-non-contact-rf-id-card-module-for-arduino-green-red-206725
 *
 * Not anymore, now using AT125 http://dx.com/p/125k-rfid-card-reader-module-non-contact-rf-id-card-module-for-arduino-green-236289
 *
 * Using NRF24 modules for communication 
 * 
 * API command input
 * node.id 0XXX         set rf24Network node id. (and restarts)
 * rfid.set_adm XXXX  reset admin tag to this value. responds with rfid.adm_set to master
 * rfid.set_clr XXXX  reset admin tag to this value. responds with rfid.clr_set to master
 * rfid.unlock X        unlock door for X seconds
 
 * API events output, send to master:
 * node.boot  XXX       Node has (re)-booted. (prints bytes free)
 * node.ping            Registering and monitoring of node/network online status
 * rfid.ok XXXXX        Scanned known normal tag
 * rfid.nok XXXXX       Scanned unknown tag
 * rfid.adm   XXXXX     Scanned admin tag (index 0), next to be scanned will be added.
 * rfid.clr   XXXXX     Scanned clear all tag (index 1). clears all normal tags 
 * rfid.add XXXX        Added XXX to db
 * rfid.del XXXX        Deleted XXX from db
 * rfid.msg XXX         Messages and debugging
 * 
*/

#include "pin_config.h"




#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <MemoryFree.h>

#include "sketch.h"
#include "messaging.h"
#include "eeprom_config.h"
#include "utils.h"



void Msg::handle(uint16_t from, char * event,  char * par)
{
  //when debugging parsing issues
  // Serial.println("from: ");
  // Serial.println(from);
  // Serial.println("event: ");
  // Serial.println(event);
  // Serial.println("par: ");
  // Serial.println(par);


  if (ra.handle(from,event,par))
  {
    return;
  }
  
  // if (strcmp_P(event, PSTR("some.event"))==0)
  // {
  //   ...do stuff
  //   return;
  // }


}





void setup() 
{
  config_read();

  Serial.begin(9600);
  msg.begin();
  ra.begin();

  //pin 9 and 10 different pwm freq.  122.5hz
  TCCR1B = TCCR1B & 0b11111000 | 4;

  pinMode(RFID_LED_PIN, OUTPUT);     
  pinMode(RFID_LOCK_PIN, OUTPUT);
  pinMode(RFID_MANUAL_PIN, INPUT_PULLUP);

  char par[MAX_MSG];
  sprintf_P(par, PSTR("%i free"), freeMemory());
  msg.send(PSTR("node.boot"), par);
  
}




void loop()
{
  
  //send/receive network and serial messages
  msg.loop(); 

  //handle rfid scans
  ra.loop();

  //manual opening of door
  if (digitalRead(RFID_MANUAL_PIN)==RFID_MANUAL_LEVEL)
  {
    ra.change_state(ra::state_unlocked, RFID_UNLOCK_TIME);
  }

  switch(ra.get_state()) 
  {

    case rfid_state_add:
      digitalWrite(RFID_LED_PIN, duty_cycle(100,200));
      break;

    case rfid_state_locked:
      //lock powersaving: only use short pulses to power the lock, and then fallback to pwm mode.
      if (duty_cycle(RFID_LOCK_DUTY_ON, RFID_LOCK_DUTY_TOTAL) || (millis()-rfid_state_started < 1000) )
        analogWrite(RFID_LOCK_PIN, 255);
      else
        analogWrite(RFID_LOCK_PIN, RFID_LOCK_PWM);

      digitalWrite(RFID_LED_PIN, duty_cycle(1900,2000));
      break;

    case rfid_state_unlocked:
      analogWrite(RFID_LOCK_PIN, 0);
      digitalWrite(RFID_LED_PIN, duty_cycle(100,2000));
      break;
  }

}

