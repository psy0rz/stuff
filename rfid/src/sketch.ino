#include <LiquidCrystal.h>

//byte BLINK_PINS[]={PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};


//byte BLINK_PINS[]={17,1 8,15,16,13,14,11,12};
//byte BLINK_PINS[]={PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};

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
*/
#define PD1 0
#define PD1 1
#define PD2 2
#define PD3 3
#define PD4 4  
#define PD5 5  
#define PD6 6  
#define PD7 7  

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
//20?
//21?
//22?

 int p= 0;

byte BLINK_PINS[]={PB0, 14};



/*
Analog 0  Buttons 
Digital 4 DB4
Digital 5 DB5
Digital 6 DB6
Digital 7 DB7
Digital 8 RS (Data or Signal Display Selection)
Digital 9 Enable
Digital 10  Backlit Control
*/
// initialize the library with the numbers of the interface pins
//LiquidCrystal(rs, enable, d4, d5, d6, d7) 
//LiquidCrystal lcd(PB4, PB5, PB0, PB1, PB2, PB3);
//LiquidCrystal lcd(PC4, PC5, PC0, PC1, PC2, PC3);


void setup() {                 
  while(1)
  {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    delay(500);
    digitalWrite(p, HIGH);
    delay(500);
    p++;

  }


  // set up the LCD's number of columns and rows: 
  //lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");

  for(byte pin_index=0; pin_index<sizeof(BLINK_PINS); pin_index++)
  {
    pinMode(BLINK_PINS[pin_index], OUTPUT);
  }

  pinMode(PC0, OUTPUT);
  pinMode(PC1, OUTPUT);
  pinMode(PC2, OUTPUT);
  pinMode(PC3, OUTPUT);
  pinMode(PC4, OUTPUT);
  pinMode(PC5, OUTPUT);
}

void loop() {
    static unsigned long blink_last_time=0;
    static byte blink_count=0;
    if (millis()-blink_last_time> 100)
    {

      digitalWrite(BLINK_PINS[blink_count], LOW);
      blink_count=(blink_count+1)%sizeof(BLINK_PINS);
      digitalWrite(BLINK_PINS[blink_count], HIGH);
      blink_last_time=millis();
    }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);
//  delay(100);

}







/** LFSR 
void loop() {


    static unsigned long blink_last_time=0;
    //static byte blink_count=0;
    static word blink_bits=1;

    if (millis()-blink_last_time> 100)
    {

      for (word bit=0; bit<sizeof(BLINK_PINS); bit++)
      {
        word drop_bit  = ((blink_bits >> 0) ^ (blink_bits >> 2) ^ (blink_bits >> 3) ^ (blink_bits >> 5) ) & 1;
        blink_bits =  (blink_bits >> 1) | (drop_bit << sizeof(BLINK_PINS));
      }

      //convert bits to actual output
      for (word bit=0; bit<sizeof(BLINK_PINS); bit++)
      {
        if (blink_bits & (1<<bit))
          digitalWrite(BLINK_PINS[bit], HIGH);
        else
          digitalWrite(BLINK_PINS[bit], LOW);
      }
      blink_last_time=millis();
  
    }
}
*/