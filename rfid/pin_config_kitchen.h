#ifndef pin_config_h
#define pin_config_h
/*
Pin outs

description     uno pin    MFRC522 rfid pin  RF24 pin                      
GND             GND        gnd(orange)       1 (gnd,brown)  
3v3             VCC        3.3v(brown)       2 (vcc,red) 3(ce,orange)
SPI MOSI        D11        MOSI(blue)        6 (mosi,blue)
SPI MISO        D12        MISO(green)       7 (miso,violet)
SPI SCK         D13        SCK(violet)       5 (sck,green)
CS RF24         D10                          4 (csn,yellow)
CS MFRC522      D8   
RST             D6         RST(red)
IRQ             D2                           8 (irq,gray)
lock pin        D9
manual pin      D7
led pin         D4

*/

//chip select for RF24 SPI
#define CS_RF24_PIN 10

//chip select MFRC522 SPI
#define CS_MFRC522_PIN 8

 //reset pin for all SPI devices
#define RST_PIN 6

#define RFID_LED_PIN 4             //feedback led


//lock powersaving stuff
#define RFID_LOCK_PIN 9            //lock output
#define RFID_LOCK_DUTY_ON      100    //full on time in ms when locking
#define RFID_LOCK_DUTY_TOTAL   10000  //total time of pwm duty cycle 
//the pwm value is stored in config.lock_pwm

#define RFID_MANUAL_PIN 7          //manual open by switch pin
//#define RFID_125KHZ_PIN 5 //rx pin for 125khz module



#endif
