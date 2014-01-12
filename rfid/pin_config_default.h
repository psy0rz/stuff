#ifndef pin_config_h
#define pin_config_h

<<<<<<< Updated upstream
////////////////////////////// Pin outs
//                          //  
//arduino pin               // description     MFRC522 rfid pin  RF24 pin                      
//GND                       // GND             gnd(orange)       1 (gnd,brown)  
//3v3                       // 3v3             3.3v(brown)       2 (vcc,red) 3(ce,orange)
//11                        // SPI MOSI        MOSI(blue)        6 (mosi,blue)
//12                        // SPI MISO        MISO(green)       7 (miso,violet)
//13                        // SPI SCK         SCK(violet)       5 (sck,green)
#define CS_RF24_PIN 10      // CS RF24                           4 (csn,yellow)
#define CS_MFRC522_PIN 8    // CS MFRC522      SDA(?)       
#define RST_PIN 6           // RST             RST(red)
                            // IRQ                               8 (irq,gray)
#define RFID_LOCK_PIN 9     // lock pin        
#define RFID_MANUAL_PIN 7   // manual pin      
#define RFID_LED_PIN 4      // led pin         
=======
description     uno pin    MFRC522 rfid pin  RF24 pin                      
GND             GND        gnd(orange)       1 (gnd,brown)                2
3v3             VCC        3.3v(brown)       2 (vcc,red) 3(ce,orange)     1 4
SPI MOSI        D11        MOSI(blue)        6 (mosi,blue)				  5
SPI MISO        D12        MISO(green)       7 (miso,violet)              8
SPI SCK         D13        SCK(violet)       5 (sck,green)                6
CS RF24         D10                          4 (csn,yellow)               3         
CS MFRC522      D8         SDA(grey)
RST             D6         RST(red)
IRQ             D2                           8 (irq,gray)				  7	
lock pin        D9
manual pin      D7
led pin         D4
>>>>>>> Stashed changes



//lock powersaving stuff
#define RFID_LOCK_DUTY_ON      100    //full on time in ms when locking
#define RFID_LOCK_DUTY_TOTAL   10000  //total time of pwm duty cycle 
//the pwm value is stored in config.lock_pwm


#endif
