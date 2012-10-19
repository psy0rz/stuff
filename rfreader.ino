
#include "rfreader.h"

void wave_dump(Wave & wave)
{
//  Serial.print("High time :");
//  Serial.println(wave.high_time);
//  Serial.print("Low time  :");
//  Serial.println(wave.low_time);

  Serial.print("Data      :");
    Serial.print(wave.data);
  Serial.println("");
}


void wave_reset(Wave & wave)
{
  wave.high_time=0;
  wave.low_time=0;
  wave.data=0;
}

//returns true if waves match
boolean wave_compare(Wave & wave_a, Wave & wave_b)
{
  return(wave_a.data==wave_b.data);
}

////////////////////// setup
//current state of the signal (high or low)
boolean current_state=false;

//the timestamp when we got this state
unsigned long state_timestamp=0;

//the number of state changes since last reset
unsigned int state_changes=0;

//length of previous state change
//int prev_state_time=0;
boolean prev_data_bit=false;

//average time (moving average)
int avg_time=0;
int avg_high_time=0;
int avg_low_time=0;

boolean preamble_done=false;

//the new wave we're currently collecting
Wave new_wave;

//the stored waves
unsigned int waves_count=0;
#define WAVES_MAX 10
Wave waves[WAVES_MAX];

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  
  Serial.println("starting..");
  
  wave_reset(new_wave);

  attachInterrupt(0, state_isr, CHANGE);

}



////////////////////// state of the input pin changed
void state_isr()
{
  state_changes++;

  int timestamp=micros();

  //amount of time that has passed since last state change:
  int state_time=timestamp-state_timestamp;

  //keep a moving average of the state-change-time. 
  avg_time=((AVG_N-1)*avg_time + state_time)/AVG_N;

  //determine if the databit is high or low
  boolean data_bit;
  if (state_time>avg_time)
  {
    data_bit=true;
    avg_high_time=((AVG_N-1)*avg_high_time + state_time)/AVG_N;
  }
  else
  {
    data_bit=false;
    avg_low_time=((AVG_N-1)*avg_low_time + state_time)/AVG_N;
  }
    
 
 //we're still in preamble mode.
  //we wait for the avg_time to smooth out, and until a certain amount of preamble bits where seen
  if (state_changes<MIN_PREAMBLE)
  {
    //we've seen double bits too early, assume garbage or out-of-sync
    if (data_bit==prev_data_bit)
    {
      state_changes=0;
    }
    preamble_done=false;
  }

  //we seem to be in sync, and the minimum preamble was seen
  else
  {
    //wait for the preamble to finish
    if (!preamble_done)
    {
      //the preamble finishes when we see double bits
      if (data_bit==prev_data_bit)
      {
        preamble_done=true;
        new_wave.high_time=avg_high_time;
        new_wave.low_time=avg_low_time;
      }
    }
    
    //preamble is done, store databit
    if (preamble_done)
    {
      new_wave.data=new_wave.data<<1;
      bitWrite(new_wave.data, 0, data_bit);
    }
  }
  
  //store new state and data_bit
  state_timestamp=timestamp;
  prev_data_bit=data_bit;

}


///////////////////// main loop
void loop() {
//  int new_state = digitalRead(2);
int new_state=0;

  int timestamp=micros();
  //amount of time that has passed since last state change:
  int state_time=timestamp-state_timestamp;

  //timeout, end of wave.
  if ((
        (state_time>MAX_TIME) ||     //generic timeout
        (state_time>avg_time*3)   //current wave probably has just ended
       ) && state_changes)
  {
      //got enough data?
      if (new_wave.data>bit(7))
      {

        //show it
        wave_dump(new_wave);
        
        //try to find existing wave or store it
        boolean found=false;
        for (int waves_nr=0; waves_nr<waves_count; waves_nr++)
        {
          if (wave_compare(new_wave, waves[waves_nr]))
          {
            Serial.print("MATCHED: ");
            Serial.println(waves_nr);
            found=true;
            break;
          }
        }

        //not found, store wave in memory
//        if (!found)
//        {
//          if (waves_count<WAVES_MAX)
//          {
//            waves_count++;
//            waves[waves_count]=new_wave;
//          }
//        }
        
        
      }
      
      //reset everything
      state_changes=0;
      current_state=new_state;
      wave_reset(new_wave);
  }

}


