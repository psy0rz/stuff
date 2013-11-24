/* RFID reader and unlocker, using minimal memory. Can learn up to 100 tags. 
 * (C)2013 Edwin Eefting edwin@datux.nl
 * Released under GPL
 * 
 * Using basic RDM630 reader: http://dx.com/p/125k-rfid-card-reader-module-rdm630-series-non-contact-rf-id-card-module-for-arduino-green-red-206725
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
 * 
 * Serial API input:
 * rfid_admin XXXX    reset admin tag to this value
 * rfid_clear XXXX    reset clear tag to this value
*/

//#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

//rfid config
SoftwareSerial rfid(2,3); 	//rfid reader RX pin
#define RFID_LEN 5  		//rfid id length in bytes
#define RFID_IDS 100 		//number of ids to store in eeprom

//init
#define RFID_STR_LEN (RFID_LEN*2)+2+1
char rfid_buf[RFID_STR_LEN]; //rfid read buffer (we get them in ascii hex strings, so we need twice the space)
char rfid_pos=0;


void setup() {                 

  //rfid reader
  rfid.begin(9600);  
  rfid_pos=0;
  rfid_buf[(RFID_LEN*2)+1]=0;
  

  //serial output
  Serial.begin(9600);
  Serial.println(F("rfid locker v1.0 booted"));
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
  

//state machine that checks what to do with a scanned tag
typedef enum {
  NORMAL,      //normal base state
  ADD //next scanned tag will be added as normal tag
}  rfid_states;

void rfid_check(unsigned char check_id[])
{
  //static variables for state machine
  static unsigned char last_id[RFID_LEN];
  static rfid_states state=NORMAL;
  static unsigned long last_time;

  //ignore zero ids
  if (rfid_is_zero(check_id))
    return;
  
  //after a few seconds of no data forget everything and reset to normal state
  if (millis()-last_time > 3000)
  {
    state=NORMAL;
    rfid_zero(last_id);
  }
  last_time=millis();
  
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
    else //normal match
    {
      rfid_print_status(F("rfid_ok"), check_id);
    }
  }
}

//reset and clear global rfid_buf buffer (for security)
void rfid_reset_buf()
{
  rfid_pos=0;
  for (int i=0; i< RFID_STR_LEN; i++)
    rfid_buf[i]=0;
}

//call this periodicly
void rfid_loop()
{
  if (Serial.available())
  {
    int r=Serial.readBytesUntil(' ', rfid_buf, RFID_STR_LEN-1);
      
    if (r==0)
      return;
    
    rfid_buf[r]=0;
    
    if (strcmp(rfid_buf, ("rfid_admin"))==0)
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
    else if (strcmp(rfid_buf, ("rfid_clear"))==0)
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


  }


  if (rfid.available())
  {
    char c=rfid.read();
    if (c==2) //start of data
    {
      rfid_reset_buf();
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
      {
        //scanned data is ok, now check what to do with it
        rfid_check((unsigned char *)rfid_buf);
      }
      
      rfid_reset_buf();
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
}


void loop() {
  rfid_loop();

}






