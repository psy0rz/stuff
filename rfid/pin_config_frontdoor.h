#ifndef pin_config_h
#define pin_config_h

////////////////////////////// Pin outs
//                          //
//arduino pin               // description     MFRC522 rfid pin     RF24 pin                      
//GND                       // GND             gnd(green)           1 (gnd,brown)  
//3v3                       // 3v3             3.3v(orange)         2 (vcc,red) 3(ce,orange)
//11                        // SPI MOSI        MOSI(gray)           6 (mosi,blue)
//12                        // SPI MISO        MISO(violet)         7 (miso,violet)
//13                        // SPI SCK         SCK(white)           5 (sck,green)
#define CS_RF24_PIN 7       // CS RF24                              4 (csn,yellow)
#define CS_MFRC522_PIN 10   // CS MFRC522      SDA(black)   
#define RST_PIN 8           // RST             RST(yellow)
                            // IRQ                                  8 (irq,gray)
#define RFID_LOCK_PIN 9     // lock pin        
#define RFID_MANUAL_PIN 6   // manual pin      
#define RFID_LED_PIN 4      // led pin         



//lock powersaving stuff
#define RFID_LOCK_DUTY_ON      100    //full on time in ms when locking
#define RFID_LOCK_DUTY_TOTAL   10000  //total time of pwm duty cycle 
//the pwm value is stored in config.lock_pwm


#endif
