#ifndef pin_config_h
#define pin_config_h

////////////////////////////// Pin outs
//                          //  
//arduino pin               // description     MFRC522 rfid pin  RF24 pin                      
//GND                       // GND             gnd(orange)       1 (gnd,brown)  
//3v3                       // 3v3             3.3v(brown)       2 (vcc,red) 
//11                        // SPI MOSI        MOSI(blue)        6 (mosi,blue)
//12                        // SPI MISO        MISO(green)       7 (miso,violet)
//13                        // SPI SCK         SCK(violet)       5 (sck,green)
#define CE_RF24_PIN A0      // CE RF24                           3 (ce,orange)
#define CS_RF24_PIN 10      // CS RF24                           4 (csn,yellow)
#define CS_MFRC522_PIN 8    // CS MFRC522      SDA(gray)       
#define RST_PIN 6           // RST             RST(red)
                            // IRQ                               8 (irq,gray)
#define RFID_LOCK_PIN 9     // lock pin        
#define RFID_MANUAL_PIN 7   // manual pin      
#define RFID_LED_PIN 4      // led pin         



//lock powersaving stuff
#define RFID_LOCK_INITIAL	   120000 //initial full power time (so that you dont have to wait)
#define RFID_LOCK_DUTY_ON      100    //full on time in ms when locking
#define RFID_LOCK_DUTY_TOTAL   10000  //total time of pwm duty cycle 
//the pwm value is stored in config.lock_pwm


#endif
