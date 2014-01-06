/* RFID reader and unlocker, using minimal memory. Can learn up to 100 tags. 
 * (C)2013 Edwin Eefting edwin@datux.nl
 * Released under GPL
 * 
 * Using basic RDM630 reader: http://dx.com/p/125k-rfid-card-reader-module-rdm630-series-non-contact-rf-id-card-module-for-arduino-green-red-206725
 *
 * Not anymore, now using AT125 http://dx.com/p/125k-rfid-card-reader-module-non-contact-rf-id-card-module-for-arduino-green-236289
 *
 * Using NRF24 modules for communication.
 *
 * Look in pin_config for pinouts
 * 
 * API command input
 * node.id 0XXX         set rf24Network node id. (and restarts)
 * rfid.set_adm XXXX    reset admin tag to this value. responds with rfid.adm_set to master
 * rfid.set_clr XXXX    reset admin tag to this value. responds with rfid.clr_set to master
 * rfid.set_ut X        set unlock time, after scanning a tag
 * rfid.unlock X        unlock for X seconds
 * rfid.lock            lock 
 * lock.set_pwm         lock pwm value
 
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


#include <EEPROM.h>

#include "pin_config.h"
#include "messaging.h"
#include "eeprom_config.h"
#include "utils.h"
#include "ra.h"

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

  if (ra.handle(from,event,par))
  {
    return;
  }

  if (strcmp_P(event, PSTR("lock.set_pwm"))==0)
  {
    config.lock_pwm=atoi(par);
    config_update();
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

  Serial.begin(115200);
  msg.begin();
  ra.begin();

  //pin 9 and 10 different pwm freq.  122.5hz
  TCCR1B = TCCR1B & 0b11111000 | 4;

  pinMode(RFID_LED_PIN, OUTPUT);     
  pinMode(RFID_LOCK_PIN, OUTPUT);
  pinMode(RFID_MANUAL_PIN, INPUT_PULLUP);

  
}




void loop()
{
  
  //send/receive network and serial messages
  msg.loop(); 

  //handle rfid scans
  ra.loop();

  //manual opening of door
  static byte manual_last=0;
  if (digitalRead(RFID_MANUAL_PIN)!=manual_last)
  {
    ra.change_state(Ra::state_unlocked, config.ra_unlock_time);
    manual_last=digitalRead(RFID_MANUAL_PIN);
  }

  switch(ra.get_state()) 
  {

    case Ra::state_add:
      digitalWrite(RFID_LED_PIN, duty_cycle(100,200));
      break;

    case Ra::state_locked:
      //lock powersaving: only use short pulses to power the lock, and then fallback to pwm mode.
      if (duty_cycle(RFID_LOCK_DUTY_ON, RFID_LOCK_DUTY_TOTAL) || (millis()-ra.state_started < 1000) )
        analogWrite(RFID_LOCK_PIN, 255);
      else
        analogWrite(RFID_LOCK_PIN, config.lock_pwm);

      digitalWrite(RFID_LED_PIN, duty_cycle(1900,2000));
      break;

    case Ra::state_unlocked:
      analogWrite(RFID_LOCK_PIN, 0);
      digitalWrite(RFID_LED_PIN, duty_cycle(100,2000));
      break;
  }

}

