/* RFID reader and unlocker, using minimal memory. Can learn up to 100 tags. 
 * (C)2013 Edwin Eefting edwin@datux.nl
 * Released under GPL
 * 
 * Using basic RDM630 reader: http://dx.com/p/125k-rfid-card-reader-module-rdm630-series-non-contact-rf-id-card-module-for-arduino-green-red-206725
 *
 * Not anymore, now using AT125 http://dx.com/p/125k-rfid-card-reader-module-non-contact-rf-id-card-module-for-arduino-green-236289
 * 
 * Doesnt require interrupts, so should work with softserial as well.
 * 
 * Serial API output:
 * rfid_raw XXXXX     Raw received rfid text-data (including checksum)
 * rfid_ok XXXXX      Scanned known normal tag
 * rfid_nok XXXXX     Scanned unknown tag
 * rfid_admin XXXXX   Scanned admin tag (index 0), next to be scanned will be added.
 * rfid_clear XXXXX   Scanned clear all tag (index 1). clears all normal tags 
 * rfid_add XXXX      Added XXX to db
 * rfid_del XXXX      Deleted XXX from db
 * rfid_ack           Acknowledgement of serial api input commands below
 * rfid_msg           Messages and debugging
 * 
 * Serial API input:
 * rfid_admin XXXX    reset admin tag to this value
 * rfid_clear XXXX    reset clear tag to this value


 * NFRC522 13mhz card stuff:
 *
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               52                MOSI
 * SPI MISO   12               51                MISO
 * SPI SCK    13               50                SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 */

//choose one, or both
#define ENABLE_MFRC522
#define ENABLE_125KHZ

//mfc stuff
#ifdef ENABLE_MFRC522
#include <MFRC522.h>
#include <SPI.h>

//mfrc522 config
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
#endif

//125 khz stuff
#ifdef ENABLE_125KHZ
#include <SoftwareSerial.h>
//rfid 125khz config
SoftwareSerial rfid(2,3);   //rfid reader RX pin
#endif

#define RFID_LEN 5      //rfid id length in bytes
#define RFID_IDS 100    //number of ids to store in eeprom

#include <EEPROM.h>


//#define RFID_LED 11             //feedback led
//#define RFID_LOCK 9            //lock output
//#define RFID_MANUAL 10          //manual open by switch pin

#define RFID_LED 4             //feedback led
#define RFID_LOCK 7            //lock output
#define RFID_MANUAL 6          //manual open by switch pin
#define RFID_MANUAL_LEVEL 1    //level to manual open door (1 or 0)

//init
#define RFID_STR_LEN 16 //(RFID_LEN*2)+2+1
char rfid_buf[RFID_STR_LEN]; //rfid read buffer (we get them in ascii hex strings, so we need twice the space)
char rfid_pos=0;


void setup() {                 
  Serial.begin(9600);
  Serial.println(F("rfid_msg initializing"));

#ifdef ENABLE_125KHZ
  //125khz rfid reader
  rfid.begin(9600);  
  rfid_pos=0;
  rfid_buf[(RFID_LEN*2)+1]=0;
#endif

#ifdef ENABLE_MFRC522
  SPI.begin();  
  mfrc522.PCD_Init(); 
#endif 

  pinMode(RFID_LED, OUTPUT);     
  pinMode(RFID_LOCK, OUTPUT);
  pinMode(RFID_MANUAL, INPUT_PULLUP);
  
  
  
  Serial.println(F("rfid_msg rfid locker v1.0 booted"));
  
  
}

//print rfid status message and id to serial
void rfid_print_status(const __FlashStringHelper *status, unsigned char id[])
{
    Serial.print(status);
    Serial.print(" ");
    char buf[3];
    for (int i=0; i< RFID_LEN; i++)
    {
      sprintf(buf, "%02hhX", id[i]);
      buf[2]=0;
      Serial.print(buf);
    }
    Serial.println();
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
      rfid_print_status(F("rfid_del"), id);
      rfid_zero(id);
      rfid_eeprom_put(i, id);
    }
  }
}


//state machine that checks what to do with a scanned tag
typedef enum {
  NORMAL,      //normal base state
  ADD //next scanned tag will be added as normal tag
}  rfid_states;

static rfid_states state=NORMAL;
unsigned long rfid_unlocked=0; //time the door was unlocked. 0 means door is locked
unsigned long last_time;
unsigned char last_id[RFID_LEN];

void rfid_check(unsigned char check_id[])
{


  //ignore zero ids
  if (rfid_is_zero(check_id))
    return;
    
  //ignore repeated ids
  if (rfid_match(check_id, last_id))
    return;

  rfid_copy(last_id, check_id);
  
  //find out which key matches
  int id_index=rfid_find(check_id);
  
  if (state==ADD)
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
        rfid_print_status(F("rfid_add"), check_id);
      }
      else
      {
        Serial.println(F("rfid_msg out of key storage"));
      }
      state=NORMAL;
    }
  }
  else if (state==NORMAL)
  {
    if (id_index==-1) //unknown
    {
      rfid_print_status(F("rfid_nok"), check_id);
    }
    else if (id_index==0) //admin match
    {
      //change to add-state
      rfid_print_status(F("rfid_admin"), check_id);
      state=ADD; 
    }
    else if (id_index==1) //clear all match
    {
      rfid_print_status(F("rfid_clear"), check_id);
      //delete all nonzero normal keys
      rfid_clear();
    }
    else //normal match
    {
      rfid_print_status(F("rfid_ok"), check_id);
      if (rfid_unlocked)
        rfid_unlocked=0;
      else
        rfid_unlocked=millis();
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
      Serial.print(F("rfid_raw "));
      Serial.println(rfid_buf);

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

      Serial.print(F("rfid_raw "));
      Serial.println(rfid_buf);

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

void loop()
{
  static bool led=true;
  static unsigned long led_time=0;
  
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
    //scanned data is ok, now check what to do with it
    digitalWrite(RFID_LED, !led);
    delay(10);
    digitalWrite(RFID_LED, led);

    rfid_check((unsigned char *)rfid_buf);
    last_time=millis();
  }
  
  //manual opening of door
  if (digitalRead(RFID_MANUAL)==RFID_MANUAL_LEVEL)
  {
    rfid_unlocked=millis();
  }
  

  //after a few seconds of no data forget everything and reset to normal state
  if (millis()-last_time > 1000)
  {
    state=NORMAL;
    rfid_zero(last_id);
  }

  if (state==ADD)
  {
    //show a "waiting for input" pattern
    if (millis()-led_time > 100)
    {
      led=!led;
      digitalWrite(RFID_LED, led);
      led_time=millis();
    }
  }
  else if (state==NORMAL)
  {
    //unlocked
    if (rfid_unlocked)
    {
      digitalWrite(RFID_LOCK, LOW); 
      if (millis()-rfid_unlocked > 4000)
      {
          rfid_unlocked=0;
      }
      
      //heartbeat (mostly off)
      if (led && millis()-led_time > 50)
      {
        led=0;
        digitalWrite(RFID_LED, led);
        led_time=millis();
      }
      if (!led && millis()-led_time > 2000)
      {
        led=1;
        digitalWrite(RFID_LED, led);
        led_time=millis();
      }

    }
    //locked
    else
    {
      digitalWrite(RFID_LOCK, HIGH); 
//       delay(100);
//       analogWrite(RFID_LOCK, 64);
//       delay(1000);
      
      //glowing heartbeat (mostly on)
      if (led && millis()-led_time > 1000)
      {
        led=0;
        digitalWrite(RFID_LED, led);
        led_time=millis();
      }
      if (!led && millis()-led_time > 50)
      {
        led=1;
        digitalWrite(RFID_LED, led);
        led_time=millis();
      }
    }
  }
  
  
  //TODO: cleanup
  //serial stuff to add the admin and clear-id's for the first time.
  if (Serial.available())
  {
    int r=Serial.readBytesUntil(' ', rfid_buf, RFID_STR_LEN-1);
      
    if (r==0)
      return;
    
    rfid_buf[r]=0;
    
    if (strcmp(rfid_buf, ("rfid_set_admin"))==0)
    {
      //read hex id
      int r=Serial.readBytesUntil('\n', rfid_buf, RFID_STR_LEN-1);
      
      if (r==0)
        return;
      
      //convert ascii hex to bytes
      for (int i=0; i< RFID_LEN; i++)
        sscanf(&rfid_buf[i*2], "%2hhx", &rfid_buf[i]); 
      
      //store as admin
      rfid_eeprom_put(0,(unsigned char*)rfid_buf);
      rfid_print_status(F("rfid_ack"), (unsigned char*)rfid_buf);
      
    }
    else if (strcmp(rfid_buf, ("rfid_set_clear"))==0)
    {
      //read hex id
      int r=Serial.readBytesUntil('\n', rfid_buf, RFID_STR_LEN-1);

      //convert ascii hex to bytes
      for (int i=0; i< RFID_LEN; i++)
        sscanf(&rfid_buf[i*2], "%2hhx", &rfid_buf[i]); 

      if (r==0)
        return;
      
      //store as clear id
      rfid_eeprom_put(1,(unsigned char*)rfid_buf);
      rfid_print_status(F("rfid_ack"), (unsigned char*)rfid_buf);
    }
    else if (strcmp(rfid_buf, ("rfid_clear"))==0)
    {
      rfid_clear();
    }


  }

  
}

