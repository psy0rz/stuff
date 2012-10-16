
#include "rfreader.h"

void wave_dump(Wave & wave)
{
  Serial.print("High time :");
  Serial.println(wave.high_time);
  Serial.print("Low time  :");
  Serial.println(wave.low_time);
  Serial.print("Data count:");
  Serial.println(wave.data_count);

  Serial.print("Data      :");
  for (int data_nr=0; data_nr<wave.data_count; data_nr++)
    Serial.print(wave.data[data_nr]);
  
  Serial.println("");
  Serial.println("");
}


void wave_reset(Wave & wave)
{
  wave.high_time=0;
  wave.low_time=0;
  wave.data_count=0;
}


////////////////////// setup
//current state of the signal (high or low)
boolean current_state=false;

//the timestamp when we got this state
unsigned long state_timestamp=0;

//the number of state changes since last reset
unsigned int state_changes=0;

//length of previous state change
int prev_state_time=0;

//current border time that is used to determine if a bit is high or low.
//this time is automaticly determined at the end of the preamble
int border_time=0;

//the new wave we're currently collecting
Wave new_wave;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  
  Serial.println("starting..");
  
  wave_reset(new_wave);

}


///////////////////// main loop

void loop() {
  int new_state = digitalRead(2);
  int timestamp=micros();
  
  //amount of time that has passed since last state change:
  int state_time=timestamp-state_timestamp;

  //timeout, reset everything
  if (((state_time>MAX_TIME) || (state_time>border_time*3)) && state_changes)
  {
      
      if (new_wave.data_count)
        wave_dump(new_wave);

      state_changes=0;
      border_time=MAX_TIME;
      current_state=new_state;
      wave_reset(new_wave);
  }

  //input state changed?
  if (new_state!=current_state)
  {
    state_changes++;
    
    //preamble just finished..signal should be stable now:
    if (state_changes==PREAMBLE)
    {
      //determine high and low times
      if (state_time>prev_state_time)
      {
        new_wave.high_time=state_time;
        new_wave.low_time=prev_state_time;
      }
      else
      {
        new_wave.high_time=prev_state_time;
        new_wave.low_time=state_time;
      }
      border_time=((new_wave.high_time-new_wave.low_time)/2) + new_wave.low_time;
    }

    //preamble is done, we're collecting actual data:
    if (state_changes>=PREAMBLE)
    {
      //databuffer is not full yet?
      if (new_wave.data_count < MAX_DATA)
      {
        if (state_time>border_time)
        {
          new_wave.data[new_wave.data_count]=true;
        }
        else
        {
          new_wave.data[new_wave.data_count]=false;
        }
        new_wave.data_count++;
      }
    }
    
    //store new state and the timestamp
    current_state=new_state;
    state_timestamp=timestamp;
    prev_state_time=state_time;
  }
}


