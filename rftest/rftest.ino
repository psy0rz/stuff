
//#include "rfreader.h"

/*
avg_time: 264
High time :624
Low time  :184
*/


////////////////////// setup

//max time we can go without state-change. after this we assume the signal has ended.
#define MAX_TIME 2000

//number of samples to use for moving average
#define AVG_N 4

//the timestamp when we last changed state
unsigned long state_timestamp=0;

//received bit nr since last reset
unsigned int bit_nr=0;

//actual stored data bits
#define MAX_BITS 128
byte bits[MAX_BITS/8];

//average times (moving averages)
int avg_time=0;
int avg_high_time=0;
int avg_low_time=0;

int pause_time=0;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  
  Serial.println("starting..");
  
  attachInterrupt(0, state_isr, CHANGE);

}

////////////////////// state of the input pin changed
void state_isr()
{

  unsigned long timestamp=micros();

  //amount of time that has passed since last state change:
  int state_time=timestamp-state_timestamp;

  //was the change fast enough to be considered part of this signal?
  if (state_time<MAX_TIME)
  {
  
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

    //store it in bits array:
    bitWrite(bits[bit_nr/8], 7-(bit_nr%8), data_bit);
    bit_nr++;

  }
  else
  {
    //store the pause between 2 signals
    pause_time=state_time;
    bit_nr=0;
  }
  
  //store timestamp
  state_timestamp=timestamp;

}


///////////////////// main loop
void loop() {
  unsigned long timestamp=micros();

  //amount of time that has passed since last state change:
  int state_time=timestamp-state_timestamp;

  //timeout, end of wave.
  if ((state_time>MAX_TIME) && bit_nr)
  {
      if (bit_nr>=32)
      {
/*
        Serial.print("pause_time: ");
        Serial.println(pause_time);

        Serial.print("avg_time: ");
        Serial.println(avg_time);

        Serial.print("avg_high_time: ");
        Serial.println(avg_high_time);

        Serial.print("avg_low_time: ");
        Serial.println(avg_low_time);

        Serial.print("bit_nr: ");
        Serial.println(bit_nr);
  */      
        //show it
        Serial.print("data: ");
        char buf[MAX_BITS+1];
        buf[0]=0;
        for (int b=0; b<bit_nr; b++)
        {
          if (bitRead(bits[b/8], 7-(b%8)))
            strcat(buf,"1");
          else
            strcat(buf,"0");
        }
        Serial.println(buf);
        
        if (micros()-timestamp > pause_time)
          Serial.println("printing is too slow!");
      }
      bit_nr=0;
  }
}


