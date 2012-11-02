
//#include "rfreader.h"

/*
avg_time: 264
High time :624
Low time  :184


scope:
1 high time: 450uS
0 high time: 470uS
1 low time: 140uS
0 low time: 170uS

9x (high+low)=5600uS 

(high+low)=622uS

460+160us?



*/


////////////////////// setup

//max time we can go without state-change. after this we assume the signal has ended.
#define MAX_TIME 2000

//number of samples to use for moving average
#define AVG_N 30

//the timestamp when we last changed state
unsigned long state_timestamp=0;

//received bit nr since last reset
unsigned int bit_nr=0;

//actual stored data bits
#define MAX_BITS 128
byte bits[MAX_BITS/8];

float output_comp_time=0; //the send seems to need some compensation to be able to reproduce the same signal.
//average times (moving averages)
float avg_high_time=460-output_comp_time;
float avg_low_time=160+output_comp_time;
float avg_time=(avg_high_time+avg_low_time)/2;


int pause_time=0;

bool listening=true;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  
  Serial.println("starting..");
  
  attachInterrupt(0, state_isr, CHANGE);

}

////////////////////// state of the input pin changed
void state_isr()
{
  if (!listening)
    return;

  unsigned long timestamp=micros();

  //amount of time that has passed since last state change:
  unsigned long state_time=timestamp-state_timestamp;

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

int wave_count=0;

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
        Serial.println(avg_high_time);
        Serial.println(avg_low_time);
        Serial.println(pause_time);
        
        if (micros()-timestamp > pause_time)
          Serial.println("printing is too slow!"); 
          
        wave_count++;
        if (wave_count>5)
        {
          wave_count=0;
          //repeat the whole signal
          listening=false;
          Serial.println("sending..");
          for (int i=0; i<5; i++)
          {
            delay(1000);
  
             //repeat bursts          
            for (int burst=0; burst<10; burst++)
            {
              bool high=false;
              //traverse the signal
              unsigned long end_time=micros();
              for (int b=0; b<bit_nr; b++)
              {
                if (high)
                {
                   digitalWrite(3, LOW);
                   high=false;
                }
                else
                {
                   digitalWrite(3, HIGH);
                   high=true;
                }
                
                if (bitRead(bits[b/8], 7-(b%8)))
                  end_time=end_time+avg_high_time;
                else
                  end_time=end_time+avg_low_time;
                  
                while(micros() < end_time)
                {
                  ;
                }
              }
              digitalWrite(3, LOW);
              delayMicroseconds(pause_time);
            }
          }
          Serial.println("done sending.");
        }
        listening=true;
   

      }
      bit_nr=0;
  }
  

}


