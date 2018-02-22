/*
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX5...............................................................................................................................
XXXXXXXXXXX7........................................................................................................................................................................................................................
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX0..............................................................................................................................................
*/

const int steps_per_minute=6;
const int sense_steps_hour=45;
const int sense_steps_12=630;
int sense_steps=0; //currently sensed steps (to detect where the hour and minute arms are)
// int step; //current step of minute arm

void step(byte pin1,byte pin2, byte step)
{
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);

  if (step==0)
  {
    digitalWrite(pin1, 0);
    digitalWrite(pin2, 1);
  }
  else if (step==1)
  {
    digitalWrite(pin1, 0);
    digitalWrite(pin2, 0);

  }
  else if (step==2)
  {
    digitalWrite(pin1, 1);
    digitalWrite(pin2, 0);
  }
  else if (step==3)
  {
    digitalWrite(pin1, 1);
    digitalWrite(pin2, 1);
  }

  // delay(150);
  // digitalWrite(pin1, 1);
  // digitalWrite(pin2, 1);
}

byte s=0;
void step_s()
{
    step(D1,D2,s);
    s=(s+2)%4;
}


byte m=0;
void step_m()
{
    step(D5,D6,m);
    m=(m+1)%4;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\nWARPCLOCK boot");
  pinMode(D7,INPUT);


}




boolean triggered=false;

int d=15;
void loop()
{
step_s();
delay(1000);
return;


//   step_m();
//   delay(d);
  int r=Serial.read();
//
if (r=='u')
{
  d=d+1;
  Serial.println(d);
}
if (r=='d')
{
  d=d-1;
  Serial.println(d);
}
// return;


  if (digitalRead(D7))
  {
    if (triggered)
    {
      triggered=false;
      while(Serial.read()==-1 ) yield();
    }
    Serial.print(".");
    step_m();
    delay(d);
  }
  else
  {
    if (!triggered)
    {
      triggered=true;
      Serial.println();
    }
    Serial.print("X");
    step_m();
    // delay(250/2);
    delay(d);

  }


  if (r=='m')
  {
//    for (int i=0;i<5;i++)
    {
      step_m();
  //    delay(100);
    }
  }

  if (r=='s')
  {
    step_s();
  }



}
