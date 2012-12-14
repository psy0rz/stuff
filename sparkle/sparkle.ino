#include <SPI.h>

#define LED_COUNT 150

//current rgbvalues
byte curr_rgb[LED_COUNT][3];
//target rbgvalues
byte want_rgb[LED_COUNT][3]; 
//fade speed to reach the target with
char fade_speed[LED_COUNT]; //we use the char just as an 'signed byte' ;)

int fade_step=0;


//sets the led to specified value on next update. 
//no fading and stuff
void led_set(led, r,g,b)
{
  want_rgb[led][0]=r;  
  want_rgb[led][1]=g;  
  want_rgb[led][2]=b;  
  curr_rgb[led][0]=r;  
  curr_rgb[led][1]=g;  
  curr_rgb[led][2]=b;
  fade_speed[led]=0;
}

void setup() {
  // Start SPI communication for the LEDstrip
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.transfer(0); // 'Prime' the SPI bus with initial latch (no wait)
  
  for (int i; i<LED_COUNT; i++)
  {
    led_set(i, 0,0,0);
  }  
}

void loop() {
  fade_step++;
  
  //random modify   
  if (random(100)==0)
  {
    byte l=random(LED_COUNT);
//    byte c=random(3);
    curr_rgb[l][0]=127;
    curr_rgb[l][1]=127;
    curr_rgb[l][2]=127;
    want_rgb[l][0]=0;
    want_rgb[l][1]=0;
    want_rgb[l][2]=random(25,50);
    fade_speed[l]=-1;
  }
 
 
 
  //////////////// below is the fade and send code, just leave it be.
  //(we could put it in a function but that costs performance)
 
  //update all the current and wanted values  
  for (byte led = 0; led < LED_COUNT; led++) {    
    //FIRST send the current values, then do the fades
    SPI.transfer(0x80 | (curr_rgb[led][0]));
    SPI.transfer(0x80 | (curr_rgb[led][1]));
    SPI.transfer(0x80 | (curr_rgb[led][2]));

    //now do all the fades
    char s=fade_speed[led];
    if (s!=0)
    {
      //change by 1 , every s steps:
      if (s>0)
      {
        if ((fade_step % s) == 0)
        {
          for (byte color=0; color<3; color++)
          {
            if (want_rgb[led][color]>curr_rgb[led][color])
              curr_rgb[led][color]++;
            else if (want_rgb[led][color]<curr_rgb[led][color])
              curr_rgb[led][color]--;
          }
        }
      } 
      //change by s, every step
      else 
      {
        for (byte color=0; color<3; color++)
        {
          char diff=want_rgb[led][color]-curr_rgb[led][color];
          //step size is greater than difference, just jump to it at once
          if (-s > abs(diff))
          {
            curr_rgb[led][color]=want_rgb[led][color];
            fade_speed[led]=0;
          }
          else
          {
            //we need to increase current value (note that s is negative)
            if (diff>0)
              curr_rgb[led][color]-=s;
            //we need to decrease current value
            else
              curr_rgb[led][color]+=s;           
          }
        }
      }
    }
  }
   
  // Sending a byte with a MSB of zero enables the output
  // Also resets the led at which new commands start.
  SPI.transfer(0);
 
}
