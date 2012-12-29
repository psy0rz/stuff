#include <EEPROM.h>

#define RELAY_ON LOW
#define RELAY_OFF HIGH


//push button inputs
const int button_a=12;
const int button_b=11;

//feedback lights for the buttons
const int button_a_lamp_out=8;
const int button_b_lamp_out=9;

//count down signal (currently an alarm bell thats turned on and off during count down)
const int countdown_out=10;

//firing relays
//the inverted fire-relais, is in series with the normal one for extra security: 
//we had some issues with the Arduino uno board R2 (not R3).
//sometimes the board didnt do a good power on reset and all the outputs where high!
const int fire_out=7;
const int fire_out_inv=6; 

//how long should both buttons be pressed before starting?
const int start_delay=1000;

//default countdown time (no its not in seconds or mS ;)
const long countdown_time=15000;

//how long should we 'fire'. this is programmable and stored in nvram, by holding the fire buttons
int fire_delay;

void setup() {
  Serial.begin(9600);
  Serial.println("ultracarribitjannifier 1.0 starting");

  pinMode(button_a,  INPUT_PULLUP);
  pinMode(button_b,  INPUT_PULLUP);


  //make sure important outputs are high (relais off), before enabling outputs
  digitalWrite(button_a_lamp_out, RELAY_ON);
  digitalWrite(button_b_lamp_out, RELAY_ON);
  digitalWrite(fire_out, RELAY_OFF);
  digitalWrite(fire_out_inv, RELAY_ON);
  digitalWrite(countdown_out, RELAY_OFF);

// delay(1000); //wait for powersupplies to settle, before enabling stuff
// (not needed, arduino uno r3 startup delay is long enough)

  pinMode(countdown_out, OUTPUT);
  pinMode(fire_out, OUTPUT);
  pinMode(fire_out_inv, OUTPUT);
  pinMode(button_a_lamp_out,  OUTPUT);
  pinMode(button_b_lamp_out,  OUTPUT);
  
  fire_delay=EEPROM.read(0); 

}


void loop() {

  //reset fire and countdown outputs
  digitalWrite(fire_out, RELAY_OFF);
  digitalWrite(fire_out_inv, RELAY_ON);
  digitalWrite(countdown_out, RELAY_OFF);

  Serial.println("standby...");

  //wait for buttons to be pressed at the same time
  while (digitalRead(button_a)==1 || digitalRead(button_b)==1)
  {
      if ((millis() % 1000)==0)
      {
        digitalWrite(button_a_lamp_out, RELAY_ON);
        digitalWrite(button_b_lamp_out, RELAY_ON);
        delay(100);
        digitalWrite(button_a_lamp_out, RELAY_ON);
        digitalWrite(button_b_lamp_out, RELAY_ON);
      }     
  }
  Serial.println("push");

  //wait until buttons are pressed long enough to start count down
  unsigned long start_time=millis();
  while ( millis()-start_time < start_delay)
  {
    if (digitalRead(button_a)==1 || digitalRead(button_b)==1)
    {
      Serial.println("no push");
      return;
    }
  }

  //count down  
  Serial.println("count down started");
  for (long i=countdown_time; i>0; i+=-1000)
  {
        digitalWrite(button_a_lamp_out, RELAY_OFF);
        digitalWrite(button_b_lamp_out, RELAY_ON);
        digitalWrite(countdown_out, RELAY_ON);
        delay(i/20);
        digitalWrite(button_a_lamp_out, RELAY_ON);
        digitalWrite(button_b_lamp_out, RELAY_OFF);
        digitalWrite(countdown_out, RELAY_OFF);
        delay(i/20);
  }  
  

  //DESTROYY!@(#)!@(#!
  Serial.println("FIRING!");
  digitalWrite(countdown_out,  RELAY_OFF);
  digitalWrite(fire_out, RELAY_ON);
  digitalWrite(fire_out_inv, RELAY_OFF);
  start_time=millis();
  
  // While buttons pressed learn new fire time
  bool rewrite=false;
  while (digitalRead(button_a)==0 && digitalRead(button_b)==0)
  {
    rewrite=true;
  }

  
  if (rewrite) 
  {
    Serial.println("storing new time:");
    fire_delay=(millis()-start_time)/1000;
    Serial.println(fire_delay);
    EEPROM.write(0, fire_delay);

  } else {
    while (millis()-start_time < fire_delay*1000) 
    {
        digitalWrite(button_a_lamp_out, RELAY_OFF);
        digitalWrite(button_b_lamp_out, RELAY_OFF);
        delay(100);
        digitalWrite(button_a_lamp_out, RELAY_ON);
        digitalWrite(button_b_lamp_out, RELAY_ON);
        delay(100);
    }
  }
  
}









