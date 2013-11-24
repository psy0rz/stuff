#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

/*
Defines for dealextreme dx.com jy_mcu jy_mega16/32 board with atmega32 at 16mhz

add this to boards.txt:
atmega32.name=Atmega32 board
atmega32.upload.protocol=stk500
atmega32.upload.maximum_size=30720
atmega32.upload.speed=19200
atmega32.bootloader.low_fuses=0xFF
atmega32.bootloader.high_fuses=0xDA
atmega32.bootloader.path=stk500
atmega32.bootloader.file=ATmegaBOOT_168_atmega32.hex
atmega32.bootloader.unlock_bits=0x3F
atmega32.bootloader.lock_bits=0x0F
atmega32.build.mcu=atmega32
atmega32.build.f_cpu=16000000L
atmega32.build.core=arduino:arduino
atmega32.build.variant=standard

#define PD0 0 //(used for programming RX)
#define PD1 1 //(used for programming TX)
#define PD2 2
#define PD3 3
#define PD4 4  //S1
#define PD5 5  //S2
#define PD6 6  //S3
#define PD7 7  //S4 (used to enable programming)

#define PB0 8  //LED D8
#define PB1 9  //LED D7
#define PB2 10 //LED D6
#define PB3 11 //LED D5
#define PB4 12 //LED D4
#define PB5 13 //LED D3

#define PC0 14
#define PC1 15
#define PC2 16
#define PC3 17
#define PC4 18
#define PC5 19
*/



//rfid config
SoftwareSerial rfid(2,3); 	//rfid reader RX pin
#define RFID_LEN 5  		//rfid id length in bytes
#define RFID_IDS 10 		//number of ids to store in eeprom

//init
#define RFID_BUF_LEN (RFID_LEN*2)+2+1
char rfid_buf[RFID_BUF_LEN]; //rfid read buffer (we get them in hex, so we need twice the space)
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

//print a rfid id thats stored in bytes to serial. (uses global rfid_buf for temporary storage)
void rfid_print(unsigned char id[])
{
    char buf[3];
    for (int i=0; i< RFID_LEN; i++)
    {
      sprintf(buf, "%02hhX", id[i]);
      buf[2]=0;
      Serial.print(buf);
    }
}

//reset and clear buffer (for security)
void rfid_reset()
{
  rfid_pos=0;
  for (int i=0; i< RFID_BUF_LEN; i++)
    rfid_buf[i]=0;
}

//call this periodicly
void rfid_loop()
{
  if (rfid.available())
  {
    char c=rfid.read();
    if (c==2) //start of data
    {
      rfid_reset();
    }
    else if (c==3) //end of data
    {    
      Serial.print(F("rfidraw "));
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
        //scanned data is ok
        Serial.print(F("rfid "));
        rfid_print((unsigned char *)rfid_buf);
        Serial.println();
        
      }
      
       rfid_reset();
    }
    else //data
    {
      if (rfid_pos<RFID_BUF_LEN)
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






