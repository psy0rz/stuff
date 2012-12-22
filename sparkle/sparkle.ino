#include <SPI.h>

#define R 0
#define G 1
#define B 2

#define LED_COUNT 160
//some strips have different color ordering??
#define COLOR_BYTE0 B
#define COLOR_BYTE1 R
#define COLOR_BYTE2 G

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


//yellow and red 'flames'
void do_fire()
{
   byte led;

  if (random(5)==0)
  {
    led=random(LED_COUNT);
    led_fade_to(led, 127, 20, 0, -2);

    led=random(LED_COUNT);
    led_fade_to(led, 127, 0, 0, -2);
  }

  //let flames dimm slowly
  led=random(LED_COUNT);
  led_fade_to(led, 0,0, 0,2);

}



//give idle leds a glowy effect 
void do_glowy()
{
  if (random(1)==0)
  {
    byte led=random(LED_COUNT);
    if (fade_speed[led]==0)
        led_fade_from(led, want_rgb[led][0]+random(5) , want_rgb[led][1]+random(5), want_rgb[led][2]+random(5)   ,25);
  }
}

//sparkle a random led sometimes
void do_sparkle()
{
  if (random(10)==0)
  {
    byte led=random(LED_COUNT);
    led_fade_from(led, 127,127, 127, -10);
  }
}  



//a 'kit radar'
//default its configured to look like it, but usually you will reconfigure it completely different. 
//it can be usefull for many different effects.
void do_radar(
  byte r=127, byte g=0, byte b=0, // color
  word spd=64, //speed. (skips this many updates), 
  char fade_min=2, char fade_max=2, //minimum and maximum fade length (randomizes  between this for smoother effect)
  word start_led=0, word end_led=7, //start and end lednr
  byte dir=2 //direction: 0 left, 1 right, 2 both
  )
{
  if (fade_step%spd == 0)
  {
     word led;
     word our_step=(fade_step/spd);
     
     //left
     if (dir==0)
     {
        led=(
         our_step
         %(end_led-start_led+1) //keep it in our range
        )+start_led;
     }
     //right
     else if (dir==1)
     {
        led=end_led-(
         our_step
         %(end_led-start_led+1) //keep it in our range
        );
     }
     //both
     else
     {
        word range=(end_led-start_led+1);
        led=(our_step%(range*2));
        if (led>=range)
          led=range-led+range-1;
        
//        0 1 2 3 4 5  
        
//        0 1 2 2 1 0
        
        led=led+start_led;
     }
      
      
     led_fade_from(led, r,g,b, random(fade_min, fade_max));
  }
}


unsigned long last_micros=0;

void loop() {

//  do_fire();
    do_radar( 
      127,0,0, //color
      64, //speed. (skips this many updates)
      10, 10, //minimum and maximum fade speed
      0,30, //start and end lednr
      2 //direction
    );


  do_sparkle();
//  do_glowy();  

  //////////////// adjust some parameters via serial on the fly
/*  if (Serial.available()) {
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
*/
 
 
  //////////////// below is the fade and send code, just leave it be.
  //(we could put it in a function but that costs performance)
 
  //update all the current and wanted values  
  for (word led = 0; led < LED_COUNT; led++) {    
    //FIRST send the current values, then do the fades
    SPI.transfer(0x80 | (curr_rgb[led][COLOR_BYTE0]));
    SPI.transfer(0x80 | (curr_rgb[led][COLOR_BYTE1]));
    SPI.transfer(0x80 | (curr_rgb[led][COLOR_BYTE2]));

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

  fade_step++;

  //limit the max number of 'frames' per second
  while (micros()-last_micros < 1000) ;
  last_micros=micros();
 
}
