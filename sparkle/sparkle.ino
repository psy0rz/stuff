#include <SPI.h>

#define LED_COUNT 160

//current rgbvalues
byte curr_rgb[LED_COUNT][3];
//target rbgvalues
byte want_rgb[LED_COUNT][3]; 
//fade speed to reach the target with
char fade_speed[LED_COUNT]; //we use the char just as an 'signed byte' ;)

word fade_step=0;


//sets the led to specified value on next update. 
void led_set(word led, byte r, byte g, byte b)
{
  want_rgb[led][0]=r;  
  want_rgb[led][1]=g;  
  want_rgb[led][2]=b;  
  curr_rgb[led][0]=r;  
  curr_rgb[led][1]=g;  
  curr_rgb[led][2]=b;
  fade_speed[led]=0;
}

//fades from current value to target value.
void led_fade_to(word led, byte r, byte g, byte b, char new_speed)
{
  want_rgb[led][0]=r;  
  want_rgb[led][1]=g;  
  want_rgb[led][2]=b;  
  fade_speed[led]=new_speed;
}

//instantly sets the led to the specified value, and fades back to the current value.
//(nice for sparkles :))
void led_fade_from(word led, byte r, byte g, byte b, char new_speed)
{
  curr_rgb[led][0]=r;  
  curr_rgb[led][1]=g;  
  curr_rgb[led][2]=b;  
  fade_speed[led]=new_speed;
}

void setup() {
  // Start SPI communication for the LEDstrip
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.transfer(0); // 'Prime' the SPI bus with initial latch (no wait)

  Serial.begin(9600);

  //initialize led array 
  for (word led; led<LED_COUNT; led++)
  {
    led_set(led, 0,0,0);
  }  
}

void loop() {
  fade_step++;
  
  //do a sparkle glide every time fade_step loops: 
  //(hence when initalizing the first time)
  static byte glide_delay=40;
  static word
  glide_led=0;
  static char glide_tail_min=3;
  static char glide_tail_max=5;
  if (fade_step%glide_delay == 0)
  {
     led_set(glide_led, 0, 10, 0);
     led_fade_from(glide_led, 0,127,0, random(glide_tail_min, glide_tail_max));    

     led_set(LED_COUNT-glide_led, 10, 0, 0);
     led_fade_from(LED_COUNT-glide_led, 127,0,0, random(glide_tail_min, glide_tail_max));    

     glide_led++;
     if (glide_led>=LED_COUNT)
       glide_led=0;
  }


  //sparkle a random led sometimes
  static byte sparkles=100;
  static byte sparkle_max=1;
  if (random(100)==0)
  {
    byte led=random(LED_COUNT);
    led_fade_from(led, 0,0, 127, random(sparkle_max)+1);    
  }

  //////////////// adjust some parameters via serial on the fly
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'a':   glide_delay++;      break;
      case 'z':   glide_delay--;      break;
      case 's':   glide_tail_max++;   break;
      case 'x':   glide_tail_max--;   break;
      case 'd':   glide_tail_min++;   break;
      case 'c':   glide_tail_min--;   break;
      case 'f':   sparkles++;         break;
      case 'v':   sparkles--;         break;
      case 'g':   sparkle_max++;      break;
      case 'b':   sparkle_max--;      break;
    }
  }

 
 
  //////////////// below is the fade and send code, just leave it be.
  //(we could put it in a function but that costs performance)
 
  //update all the current and wanted values  
  for (word led = 0; led < LED_COUNT; led++) {    
    //FIRST send the current values, then do the fades
    SPI.transfer(0x80 | (curr_rgb[led][1]));
    SPI.transfer(0x80 | (curr_rgb[led][0]));
    SPI.transfer(0x80 | (curr_rgb[led][2]));

    //now do all the fades
    char s=fade_speed[led];
    byte done=0;
    
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
            else
              done++;
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
            done++;
          }
          else
          {
            //we need to increase current value (note that s is negative)
            if (diff>0)
              curr_rgb[led][color]-=s;
            //we need to decrease current value
            else if (diff<0)
              curr_rgb[led][color]+=s;
            else 
              done++;
          }
        }
      }
      if (done==3)
        fade_speed[led]=0; //done fading, saves performance
    }
  }
   
  // Sending a byte with a MSB of zero enables the output
  // Also resets the led at which new commands start.
  SPI.transfer(0);
 
}
