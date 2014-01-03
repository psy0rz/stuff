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



//choose one, or both
#define ENABLE_MFRC522
//#define ENABLE_125KHZ //OBSOLETE

//mfc stuff
#ifdef ENABLE_MFRC522
#include <MFRC522.h>
#include <SPI.h>
MFRC522 mfrc522(CS_MFRC522_PIN, RST_PIN); // Create MFRC522 instance.
#endif

//125 khz stuff (deprecated)
#ifdef ENABLE_125KHZ
#include <SoftwareSerial.h>
//rfid 125khz config
SoftwareSerial rfid(RFID_125KHZ_PIN,3);   //rfid reader RX pin + dummy pin (you can use it for other stuff)
#endif

#define RFID_LEN 5      //rfid id length in bytes
#define RFID_IDS 100    //number of ids to store in eeprom

//#define RFID_LED_PIN 11             //feedback led
//#define RFID_LOCK_PIN 9            //lock output
//#define RFID_MANUAL 10          //manual open by switch pin


#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <MemoryFree.h>

#include "sketch.h"
#include "messaging.h"
#include "eeprom_config.h"

void(* reboot) (void) = 0;


#define RFID_STR_LEN 16 //(RFID_LEN*2)+2+1
char rfid_buf[RFID_STR_LEN]; //rfid read buffer (we get them in ascii hex strings, so we need twice the space)
char rfid_pos=0;


//dutycycle function to make stuff blink without using memory 
//returns true during 'on' ms, with total periode time 'total' ms.
bool duty_cycle(unsigned long on, unsigned long total, unsigned long starttime=0)
{
  if (!starttime)
    return((millis()%total)<on);
  else
    return(((millis()-starttime)%total)<on);
}




//convert ascii hex rfid to bytes
void rfid_hex_to_bytes(char * id_str, unsigned char id[])
{ 
  for (int i=0; i<= RFID_LEN; i++)
    sscanf(&id_str[i*2], "%2hhx", &id[i]); 
}


//parse and handle 0 terminated message, as well as forward it to rs232
bool Msg::handle(uint16_t from, char * event,  char * par)
{
  Serial.print('0');
  Serial.print(from,OCT);
  Serial.print(' ');
  Serial.print(event);
  Serial.print(' ');
  Serial.println(par);

  //when debugging parsing issues
  // Serial.println("from: ");
  // Serial.println(from);
  // Serial.println("event: ");
  // Serial.println(event);
  // Serial.println("par: ");
  // Serial.println(par);

  //change node id
  if (strcmp_P(event, PSTR("node.id"))==0)
  {
    //update eeprom
    sscanf(par,"%u", &config.node_id);
    config_update();
    delay(1000);
    reboot();
  }
  else if (strcmp_P(event, PSTR("rfid.set_adm"))==0)
  {
    unsigned char id[RFID_LEN];
    rfid_hex_to_bytes(par, id);

    //store as admin key
    rfid_eeprom_put(0,id);
    send_master_rfid(PSTR("rfid.adm_set"), id);
  }
  else if (strcmp_P(event, PSTR("rfid.set_clr"))==0)
  {
    unsigned char id[RFID_LEN];
    rfid_hex_to_bytes(par, id);

    //store as clear key
    rfid_eeprom_put(1,id);
    send_master_rfid(PSTR("rfid.clr_set"), id);
  }
  else if (strcmp_P(event, PSTR("rfid.clr"))==0)
  {
    rfid_clear();
  }
}



//send a message to the master with a rfid-id as parameter (will be converted to hex string)
bool send_master_rfid(const char * event, unsigned char id[])
{
    char msg_buf[MAX_MSG]; 

    strcpy_P(msg_buf, event);
    strcat(msg_buf," ");

    char buf[3];
    for (int i=0; i< RFID_LEN; i++)
    {
      sprintf(buf, "%02hhX", id[i]);
      buf[2]=0;
      strcat(msg_buf,buf);
    }
    msg.send_line(msg_buf);
}




void setup() 
{
  config_read();

  Serial.begin(9600);
  SPI.begin();  

#ifdef ENABLE_125KHZ
  //125khz rfid reader
  rfid.begin(9600);  
  rfid_pos=0;
  rfid_buf[(RFID_LEN*2)+1]=0;
#endif

#ifdef ENABLE_MFRC522
  mfrc522.PCD_Init(); 
#endif 


  msg.begin();

  //pin 9 and 10 different pwm freq.  122.5hz
  TCCR1B = TCCR1B & 0b11111000 | 4;

  pinMode(RFID_LED_PIN, OUTPUT);     
  pinMode(RFID_LOCK_PIN, OUTPUT);
  pinMode(RFID_MANUAL_PIN, INPUT_PULLUP);

  char par[MAX_MSG];
  sprintf_P(par, PSTR("%i free"), freeMemory());
  msg.send(PSTR("node.boot"), par);
  
}


//stores id at specified nr in eeprom
void rfid_eeprom_put(int index, unsigned char id[])
{
  int eeprom_offset=index*RFID_LEN;
  for (int i=0; i< RFID_LEN; i++)
  {
    EEPROM.write(i+eeprom_offset, id[i]);
  }  
}

//get id at specified index from eeprom
void rfid_eeprom_get(int index, unsigned char id[])
{
  int eeprom_offset=index*RFID_LEN;
  for (int i=0; i< RFID_LEN; i++)
  {
    id[i]=EEPROM.read(i+eeprom_offset);
  }  
}


//checks if id is zero (e.g. empty/clear)
bool rfid_is_zero(unsigned char id[])
{
  for (int i=0; i< RFID_LEN; i++)
    if (id[i]!=0)
      return(false);
    
  return(true);
}

//clears id to zero
void rfid_zero(unsigned char id[])
{
  for (int i=0; i< RFID_LEN; i++)
    id[i]=0;
}


//compares 2 ids
bool rfid_match(unsigned char ida[], unsigned char idb[])
{
  for (int i=0; i< RFID_LEN; i++)
    if (ida[i]!=idb[i])
      return(false);
    
  return(true);
}

//copy id
void rfid_copy(unsigned char dst[], unsigned char src[])
{
  memcpy(dst, src, RFID_LEN);
}


//find key in eeprom and return id.
//returns -1 if nonexisting
int rfid_find(unsigned char id[])
{
  unsigned char buf[RFID_LEN];

  for (int i=0; i<RFID_IDS; i++)
  {
    rfid_eeprom_get(i, buf);
    if (rfid_match(buf, id))
    {
      return(i);
    }
  }
  return(-1);
}

//clear all normal keys from eeprom (do this the first time to init eeprom as well)
void rfid_clear()
{
  for (int i=2; i<RFID_IDS; i++)
  {
    unsigned char id[RFID_LEN];
    rfid_eeprom_get(i, id);
    if (!rfid_is_zero(id))
    {
      send_master_rfid(PSTR("rfid.del"), id);
      rfid_zero(id);
      rfid_eeprom_put(i, id);
    }
  }
}



rfid_states rfid_state=rfid_state_locked;
unsigned long rfid_state_started=0;
word rfid_state_duration=0;

void rfid_change_state(rfid_states new_state, word duration)
{
  if (new_state==rfid_state)
    return;

  rfid_state=new_state;

  if (rfid_state==rfid_state_add)
    msg.send(PSTR("rfid.state add"));
  else if (rfid_state==rfid_state_locked)
    msg.send(PSTR("rfid.state locked"));
  else if (rfid_state==rfid_state_unlocked)
    msg.send(PSTR("rfid.state unlocked"));

  rfid_state_started=millis();
  rfid_state_duration=duration;
}

rfid_states rfid_get_state()
{
  if (rfid_state!=rfid_state_locked)
  {
    if (rfid_state_duration)
    {
      if ( (millis()-rfid_state_started)/1000 > rfid_state_duration)
      {
        rfid_change_state(rfid_state_locked,0);
      }
    }
  }
  return(rfid_state);
}


void rfid_check(unsigned char check_id[])
{
  //ignore zero ids
  if (rfid_is_zero(check_id))
    return;
    
  //find out which key matches
  int id_index=rfid_find(check_id);
  
  if (rfid_state==rfid_state_add)
  {
    if (id_index==-1) //key is still unkown
    {
      //find a zero-key spot
      unsigned char id[RFID_LEN];
      rfid_zero(id);
      id_index=rfid_find(id);
      if (id_index!=-1)
      {
        //store it
        rfid_eeprom_put(id_index, check_id);
        send_master_rfid(PSTR("rfid.add"), check_id);
      }
      else
      {
        msg.send(PSTR("rfid.msg out of key storage"));
      }
    }
    rfid_change_state(rfid_state_locked,0);
  }
  else if (rfid_state==rfid_state_locked || rfid_state==rfid_state_unlocked)
  {
    if (id_index==-1) //unknown
    {
      send_master_rfid(PSTR("rfid.nok"), check_id);
    }
    else if (id_index==0) //admin match
    {
      //change to add-state
      send_master_rfid(PSTR("rfid.adm"), check_id);
      rfid_change_state(rfid_state_add,10);
    }
    else if (id_index==1) //clear all match
    {
      send_master_rfid(PSTR("rfid.clr"), check_id);
      //delete all nonzero normal keys
      rfid_clear();
    }
    else //normal match
    {
      send_master_rfid(PSTR("rfid.ok"), check_id);
      if (rfid_state==rfid_state_locked)
        rfid_change_state(rfid_state_unlocked,RFID_UNLOCK_TIME);
      else
        rfid_change_state(rfid_state_locked,0);
    }
  }
}

//reset and clear global rfid_buf buffer (for security)
void rfid_reset_buf(char *rfid_buf)
{
  rfid_pos=0;
  for (int i=0; i< RFID_STR_LEN; i++)
    rfid_buf[i]=0;
}

#ifdef ENABLE_125KHZ

//call this periodically to read RDM630 modules
bool rfid_read_RDM630(char *rfid_buf)
{

  if (rfid.available())
  {
    char c=rfid.read();

    if (c==2) //start of data
    {
      rfid_reset_buf(rfid_buf);
    }
    else if (c==3) //end of data
    {    
      //Serial.print(F("rfid_raw "));
      //Serial.println(rfid_buf);

      //convert ascii hex to bytes and generate checksum
      unsigned char checksum=0;
      for (int i=0; i< RFID_LEN+1; i++)
      {
        sscanf(&rfid_buf[i*2], "%2hhx", &rfid_buf[i]); 
        checksum=checksum^rfid_buf[i];
      }
    
      if (checksum==0)
        return(true);
      
      rfid_reset_buf(rfid_buf);
    }
    else //data
    {
      if (rfid_pos<RFID_STR_LEN)
      {
        rfid_buf[rfid_pos]=c;
        rfid_pos++;       
      }
    }
  }
  return(false);
}


//call this periodically to read ATS125 modules
bool rfid_read_ATS125(char *rfid_buf)
{

  if (rfid.available())
  {
    char c=rfid.read();

    if (c==0xa) //end of data
    {
      if (rfid_pos<=3)
        return(false);

      rfid_buf[rfid_pos-2]=0; //strip the crlf

      //Serial.print(F("rfid_raw "));
      //Serial.println(rfid_buf);

      //convert ascii decimal number to bytes
      char rfid_bytes[RFID_STR_LEN]; 
      rfid_reset_buf(rfid_bytes);
      sscanf(rfid_buf, "%lu", rfid_bytes); 
      rfid_copy((unsigned char*)rfid_buf, (unsigned char*)rfid_bytes);


      return(true);
    }
    else //data
    {
      //strip leading zeroes
      if (rfid_pos==0 && c=='0')
        return(false);

      if (rfid_pos<RFID_STR_LEN)
      {
        rfid_buf[rfid_pos]=c;
        rfid_pos++;       
      }
    }
  }
  return(false);
}

#endif


#ifdef ENABLE_MFRC522

bool rfid_read_mfrc522(char *rfid_buf)
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return(false);
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return(false);
  }

  rfid_reset_buf(rfid_buf);
  memcpy(rfid_buf, mfrc522.uid.uidByte, 4); //for now we only support 4 bytes. this is default 
  return(true);
}
#endif 


void rfid_loop()
{
// you can use one or both :)
  if (
#ifdef ENABLE_125KHZ
    rfid_read_ATS125(rfid_buf)
#else
    0
#endif 
    || 
#ifdef ENABLE_MFRC522
    rfid_read_mfrc522(rfid_buf)
#else
    0
#endif
    )
  {
    static unsigned long last_scan=0;
    //ignore during quick repeats
    if (millis()-last_scan > 500)
    {
      //scanned data is ok, now check what to do with it
      rfid_check((unsigned char *)rfid_buf);
    }
    last_scan=millis();
  }
}


void loop()
{
  
  //send/receive network and serial messages
  msg.loop(); 

  //handle rfid scans
  rfid_loop();

  //manual opening of door
  if (digitalRead(RFID_MANUAL_PIN)==RFID_MANUAL_LEVEL)
  {
    rfid_change_state(rfid_state_unlocked, RFID_UNLOCK_TIME);
  }

  switch(rfid_get_state())  
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

