#include <EEPROM.h>

// this constant won't change:
const int button_a=12;
const int button_b=11;
const int countdown_out=10;
const int fire_out=7;
const int button_a_lamp_out=8;
const int button_b_lamp_out=9;

const int start_delay=1000;

const long countdown_time=15000;
int fire_delay;

void setup() {
  pinMode(button_a,  INPUT_PULLUP);
  pinMode(button_b,  INPUT_PULLUP);

  pinMode(countdown_out, OUTPUT);
  pinMode(fire_out, OUTPUT);
  pinMode(button_a_lamp_out,  OUTPUT);
  pinMode(button_b_lamp_out,  OUTPUT);

  digitalWrite(button_a_lamp_out, HIGH);
  digitalWrite(button_b_lamp_out, HIGH);
  digitalWrite(fire_out, HIGH);
  digitalWrite(countdown_out, HIGH);
  
  fire_delay=EEPROM.read(0); 

  Serial.begin(9600);
  Serial.println("ultracarribitjannifier 1.0 stared");
}

void check_buttons_pressed() 
{
  
}

void loop() {

  //wait for buttons to be pressed at the same time
  while (digitalRead(button_a)==1 || digitalRead(button_b)==1)
  {
      if ((millis() % 1000)==0)
      {
        digitalWrite(button_a_lamp_out, HIGH);
        digitalWrite(button_b_lamp_out, HIGH);
        delay(100);
        digitalWrite(button_a_lamp_out, LOW);
        digitalWrite(button_b_lamp_out, LOW);
      }     
  }
  Serial.println("push detected");

  //wait until buttons are pressed long enough to start count down
  unsigned long start_time=millis();
  while ( millis()-start_time < start_delay)
  {
    if (digitalRead(button_a)==1 || digitalRead(button_b)==1)
    {
      Serial.println("push cancelled");
      return;
    }
  }

  //count down  
  digitalWrite(countdown_out, LOW);
  Serial.println("count down started");
  for (long i=countdown_time; i>0; i+=-1000)
  {
        digitalWrite(button_a_lamp_out, HIGH);
        digitalWrite(button_b_lamp_out, LOW);
        digitalWrite(countdown_out, LOW);
        delay(i/20);
        digitalWrite(button_a_lamp_out, LOW);
        digitalWrite(button_b_lamp_out, HIGH);
        digitalWrite(countdown_out, HIGH);
        delay(i/20);
  }  
  

  //DESTROYY!@(#)!@(#!
  digitalWrite(countdown_out, HIGH);
  digitalWrite(fire_out, LOW);
  start_time=millis();
  
  // While buttons pressed learn new fire time
  bool rewrite=false;
  while (digitalRead(button_a)==0 && digitalRead(button_b)==0)
  {
    rewrite=true;
  }

  
  if (rewrite) 
  {
    fire_delay=(millis()-start_time)/1000;
    EEPROM.write(0, fire_delay);

  } else {
    while (millis()-start_time < fire_delay*1000) 
    {
        digitalWrite(button_a_lamp_out, HIGH);
        digitalWrite(button_b_lamp_out, HIGH);
        delay(100);
        digitalWrite(button_a_lamp_out, LOW);
        digitalWrite(button_b_lamp_out, LOW);
        delay(100);
    }    
  }
  
  digitalWrite(fire_out, HIGH);
  
}









