#ifndef _ra_h
#define _ra_h

//Rfid authorize class. takes care of all rfid reading, matching and authorizing


// //choose one, or both
// #define ENABLE_MFRC522
// //#define ENABLE_125KHZ //OBSOLETE

#include <MFRC522.h>
#include <SPI.h>
#include <avr/pgmspace.h>

// //125 khz stuff (deprecated)
// #ifdef ENABLE_125KHZ
// #include <SoftwareSerial.h>
// //rfid 125khz config
// SoftwareSerial rfid(RFID_125KHZ_PIN,3);   //rfid reader RX pin + dummy pin (you can use it for other stuff)
// #endif

#define RFID_LEN 5      //rfid id length in bytes
#define RFID_IDS 100    //number of ids to store in eeprom

//#define RFID_LED_PIN 11             //feedback led
//#define RFID_LOCK_PIN 9            //lock output
//#define RFID_MANUAL 10          //manual open by switch pin


#define RFID_STR_LEN 16 //(RFID_LEN*2)+2+1
char buf[RFID_STR_LEN]; //rfid read buffer (we get them in ascii hex strings, so we need twice the space)
char pos=0;

class Ra
{
  public:
  MFRC522 mfrc522;

  unsigned long last_scan;

  //state machine that checks what to do with a scanned tag
  typedef enum {
    state_locked,
    state_unlocked,
    state_add
  } state_t;
  unsigned long state_started=0;
  state_t state=state_locked;
  word state_duration=0;

  Ra() : mfrc522(CS_MFRC522_PIN, RST_PIN)
  {
    last_scan=0;
  }

  void begin()
  {
    SPI.begin();

    // #ifdef ENABLE_125KHZ
    //   //125khz rfid reader
    //   rfid.begin(9600);  
    //   rfid_pos=0;
    //   rfid_buf[(RFID_LEN*2)+1]=0;
    // #endif

    mfrc522.PCD_Init(); 

  }

  //convert ascii hex rfid to bytes
  void hex_to_bytes(char * id_str, unsigned char id[])
  { 
    for (int i=0; i<= RFID_LEN; i++)
      sscanf(&id_str[i*2], "%2hhx", &id[i]); 
  }


  //send a message to the master with a rfid-id as parameter (will be converted to hex string)
  bool send(const char * event, unsigned char id[])
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

  //stores id at specified nr in eeprom
  void eeprom_put(int index, unsigned char id[])
  {
    int eeprom_offset=index*RFID_LEN;
    for (int i=0; i< RFID_LEN; i++)
    {
      EEPROM.write(i+eeprom_offset, id[i]);
    }  
  }

  //get id at specified index from eeprom
  void eeprom_get(int index, unsigned char id[])
  {
    int eeprom_offset=index*RFID_LEN;
    for (int i=0; i< RFID_LEN; i++)
    {
      id[i]=EEPROM.read(i+eeprom_offset);
    }  
  }


  //checks if id is zero (e.g. empty/clear)
  bool is_zero(unsigned char id[])
  {
    for (int i=0; i< RFID_LEN; i++)
      if (id[i]!=0)
        return(false);
      
    return(true);
  }

  //clears id to zero
  void zero(unsigned char id[])
  {
    for (int i=0; i< RFID_LEN; i++)
      id[i]=0;
  }


  //compares 2 ids
  bool match(unsigned char ida[], unsigned char idb[])
  {
    for (int i=0; i< RFID_LEN; i++)
      if (ida[i]!=idb[i])
        return(false);
      
    return(true);
  }

  //copy id
  void copy(unsigned char dst[], unsigned char src[])
  {
    memcpy(dst, src, RFID_LEN);
  }


  //find key in eeprom and return id.
  //returns -1 if nonexisting
  int find(unsigned char id[])
  {
    unsigned char buf[RFID_LEN];

    for (int i=0; i<RFID_IDS; i++)
    {
      eeprom_get(i, buf);
      if (match(buf, id))
      {
        return(i);
      }
    }
    return(-1);
  }

  //clear all normal keys from eeprom (do this the first time to init eeprom as well)
  void clear()
  {
    for (int i=2; i<RFID_IDS; i++)
    {
      unsigned char id[RFID_LEN];
      eeprom_get(i, id);
      if (!is_zero(id))
      {
        send(PSTR("rfid.del"), id);
        zero(id);
        eeprom_put(i, id);
      }
    }
  }




  void change_state(state_t new_state, word duration)
  {
    if (new_state==state)
      return;

    state=new_state;

    if (state==state_add)
      msg.send(PSTR("rfid.state add"));
    else if (state==state_locked)
      msg.send(PSTR("rfid.state locked"));
    else if (state==state_unlocked)
      msg.send(PSTR("rfid.state unlocked"));

    state_started=millis();
    state_duration=duration;
  }

  state_t get_state()
  {
    if (state!=state_locked)
    {
      if (state_duration)
      {
        if ( (millis()-state_started)/1000 > state_duration)
        {
          change_state(state_locked,0);
        }
      }
    }
    return(state);
  }


  void check(unsigned char check_id[])
  {
    //ignore zero ids
    if (is_zero(check_id))
      return;
      
    //find out which key matches
    int id_index=find(check_id);
    
    if (state==state_add)
    {
      if (id_index==-1) //key is still unkown
      {
        //find a zero-key spot
        unsigned char id[RFID_LEN];
        zero(id);
        id_index=find(id);
        if (id_index!=-1)
        {
          //store it
          eeprom_put(id_index, check_id);
          send(PSTR("rfid.add"), check_id);
        }
        else
        {
          msg.send(PSTR("rfid.msg out of key storage"));
        }
      }
      change_state(state_locked,0);
    }
    else if (state==state_locked || state==state_unlocked)
    {
      if (id_index==-1) //unknown
      {
        send(PSTR("rfid.nok"), check_id);
      }
      else if (id_index==0) //admin match
      {
        //change to add-state
        send(PSTR("rfid.adm"), check_id);
        change_state(state_add,10);
      }
      else if (id_index==1) //clear all match
      {
        send(PSTR("rfid.clr"), check_id);
        //delete all nonzero normal keys
        clear();
      }
      else //normal match
      {
        send(PSTR("rfid.ok"), check_id);
        if (state==state_locked)
          change_state(state_unlocked,RFID_UNLOCK_TIME);
        else
          change_state(state_locked,0);
      }
    }
  }

  //reset and clear global buf buffer (for security)
  void reset_buf(char *buf)
  {
    pos=0;
    for (int i=0; i< RFID_STR_LEN; i++)
      buf[i]=0;
  }

  // #ifdef ENABLE_125KHZ

  // //call this periodically to read RDM630 modules
  // bool read_RDM630(char *buf)
  // {

  //   if (rfid.available())
  //   {
  //     char c=rfid.read();

  //     if (c==2) //start of data
  //     {
  //       reset_buf(buf);
  //     }
  //     else if (c==3) //end of data
  //     {    
  //       //Serial.print(F("raw "));
  //       //Serial.println(buf);

  //       //convert ascii hex to bytes and generate checksum
  //       unsigned char checksum=0;
  //       for (int i=0; i< RFID_LEN+1; i++)
  //       {
  //         sscanf(&buf[i*2], "%2hhx", &buf[i]); 
  //         checksum=checksum^buf[i];
  //       }
      
  //       if (checksum==0)
  //         return(true);
        
  //       reset_buf(buf);
  //     }
  //     else //data
  //     {
  //       if (pos<RFID_STR_LEN)
  //       {
  //         buf[pos]=c;
  //         pos++;       
  //       }
  //     }
  //   }
  //   return(false);
  // }


  // //call this periodically to read ATS125 modules
  // bool read_ATS125(char *buf)
  // {

  //   if (rfid.available())
  //   {
  //     char c=rfid.read();

  //     if (c==0xa) //end of data
  //     {
  //       if (pos<=3)
  //         return(false);

  //       buf[pos-2]=0; //strip the crlf

  //       //Serial.print(F("raw "));
  //       //Serial.println(buf);

  //       //convert ascii decimal number to bytes
  //       char bytes[RFID_STR_LEN]; 
  //       reset_buf(bytes);
  //       sscanf(buf, "%lu", bytes); 
  //       copy((unsigned char*)buf, (unsigned char*)bytes);


  //       return(true);
  //     }
  //     else //data
  //     {
  //       //strip leading zeroes
  //       if (pos==0 && c=='0')
  //         return(false);

  //       if (pos<RFID_STR_LEN)
  //       {
  //         buf[pos]=c;
  //         pos++;       
  //       }
  //     }
  //   }
  //   return(false);
  // }

  // #endif


  bool read_mfrc522(char *buf)
  {
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return(false);
    }

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return(false);
    }

    reset_buf(buf);
    memcpy(buf, mfrc522.uid.uidByte, 4); //for now we only support 4 bytes. this is default 
    return(true);
  }


  void loop()
  {
    if (read_mfrc522(buf))
    {
      //ignore during quick repeats
      if (millis()-last_scan > 500)
      {
        //scanned data is ok, now check what to do with it
        check((unsigned char *)buf);
      }
      last_scan=millis();
    }
  }


  //handle network events (called from main loop with messages from Msg
  bool handle(uint16_t from, char * event,  char * par)
  {
    if (strcmp_P(event, PSTR("rfid.set_adm"))==0)
    {
      unsigned char id[RFID_LEN];
      hex_to_bytes(par, id);

      //store as admin key
      eeprom_put(0,id);
      send(PSTR("rfid.adm_set"), id);
      return(true);
    }
    
    if (strcmp_P(event, PSTR("rfid.set_clr"))==0)
    {
      unsigned char id[RFID_LEN];
      hex_to_bytes(par, id);

      //store as clear key
      eeprom_put(1,id);
      send(PSTR("rfid.clr_set"), id);
      return(true);
    }

    if (strcmp_P(event, PSTR("rfid.clr"))==0)
    {
      clear();
      return(true);
    }

    return(false);
  }


};

Ra ra; // :))



#endif