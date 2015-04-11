#include "LedControl.h"

static const int DATA_PIN = 20;
static const int CLK_PIN  = 5;
static const int CS_PIN   = 21;

static const int lowPin = 11;             /* ground pin for the buton ;-) */
static const int buttonPin = 9;           /* choose the input pin for the pushbutton */

static const int INTENSITY = 5;


LedControl lc=LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1);

void setup()
{
    /*
       The MAX72XX is in power-saving mode on startup,
       we have to do a wakeup call
     */
    lc.shutdown(0,false);
    /* Set the brightness to a medium values */
    lc.setIntensity(0,INTENSITY);
    /* and clear the display */
    lc.clearDisplay(0);
    /* setup pins */
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(lowPin, OUTPUT);
    digitalWrite(lowPin, LOW);
}

void loop()
{
  int bird_max=1000;
  int bird_y=bird_max/2;
  int bird_x=3;
  int bird_speed=0;
  int bird_gravity=7;
  int bird_jump_speed=70; 


  int start_time=millis();
  int frame_time=25;

  bool pressed=false;

  while(1)
  {
    start_time=millis();


    if (!digitalRead(buttonPin) && !pressed)
    {
        pressed=true;
        bird_speed=bird_jump_speed;
    }
    else
    {
        pressed=false;
        bird_speed=bird_speed-bird_gravity;
    }

    bird_y=bird_y-bird_speed;

    if (bird_y<0)
      bird_y=0;

    if (bird_y>bird_max)
      bird_y=bird_max;

    //downscale birdheight to 8 pixels :P
    lc.setRow(0, bird_x, (1 << ((bird_y*7) / bird_max)));

    //wait for next frame
    while( (millis()-start_time) < frame_time){
      ;
    }
  }
}
