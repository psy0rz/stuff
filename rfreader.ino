

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  
  Serial.println("starting..");

}

int oldState=0;
unsigned long oldTime=0;
  
#define MAX_TIMES 300
#define MAX_DIFF 10000

int times[MAX_TIMES];
int timenr=0;

//dump data to serial port and reset buffer
void dump()
{
  //first is rubish
  for(int i=1; i<timenr;i++)
  {
    Serial.println(times[i]);
  }
  Serial.println();
  timenr=0;
}

void loop() {
  int state = digitalRead(2);
  int diff=micros()-oldTime;

  //timeout, ignore stuff we received so far and reset
  if (diff>MAX_DIFF && timenr)
  {
    if (timenr>10)
    {
      Serial.println("timeout");
      dump();
    }
    else
    {
      //prevent noise, ignore it
      timenr=0;
    }
  }

  //input state changed
  if (state!=oldState)
  {
    oldState=state;

    times[timenr]=diff;
    timenr++;
    if (timenr>=MAX_TIMES)
    {
      Serial.println("buffer full");
      dump();
    }
    oldTime=micros();
  }
}

