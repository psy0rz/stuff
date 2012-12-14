#include <SPI.h>

#define LED_COUNT 150

const byte
  redFrequency = 12,
  greenFrequency = 15,
  blueFrequency = 19,
  intervalRed = 16,
  intervalGreen = 12,
  intervalBlue = 8;

int index;
long currentMillis, nextRed, nextGreen, nextBlue;
boolean
  enableRed = true,
  enableGreen = true,
  enableBlue = true;

//current rgbvalues
byte curr_rgb[LED_COUNT][3];
byte want_rgb[LED_COUNT][3]; 
char fade_speed[LED_COUNT]; //we use the char just as an 'signed byte' ;)


int fade_step=0;
void setup() {
  // Start SPI communication for the LEDstrip
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.transfer(0); // 'Prime' the SPI bus with initial latch (no wait)
//  Serial.begin(9600);

}


void loop() {
  fade_step++;

//  currentMillis = millis();
/*  if (Serial.available()) {
    switch (Serial.read()) {
      case 'r':
        enableRed = !enableRed;
        break;
      case 'g':
        enableGreen = !enableGreen;
        break;
      case 'b':
        enableBlue = !enableBlue;
        break;
    }
  }
  // Send three color bytes for all LEDs on the strip.
  for (index = 0; index < LED_COUNT; index++) {
    SPI.transfer(0x80 | ((enableBlue)  ? blueWave(index)  : 0));
    SPI.transfer(0x80 | ((enableRed)   ? redWave(index)   : 0));
    SPI.transfer(0x80 | ((enableGreen) ? greenWave(index) : 0));
  }*/

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

  //update all the current and wanted values  
  for (index = 0; index < LED_COUNT; index++) {
    char s=fade_speed[index];

    //FIRST send the current values, then do the fades
    SPI.transfer(0x80 | (curr_rgb[index][0]));
    SPI.transfer(0x80 | (curr_rgb[index][1]));
    SPI.transfer(0x80 | (curr_rgb[index][2]));

    if (s!=0)
    {
      //change by 1 , every s steps:
      if (s>0)
     {
        if ((fade_step % s) == 0)
        {
          for (byte color=0; color<3; color++)
          {
            if (want_rgb[index][color]>curr_rgb[index][color])
              curr_rgb[index][color]++;
            else if (want_rgb[index][color]<curr_rgb[index][color])
              curr_rgb[index][color]--;
          }
        }
      } 
      //change by s, every step
      else 
      {
        for (byte color=0; color<3; color++)
        {
          char diff=want_rgb[index][color]-curr_rgb[index][color];
          //step size is greater than difference, just jump to it at once
          if (-s > abs(diff))
            curr_rgb[index][color]=want_rgb[index][color];
          else
          {
            //we need to increase current value (note that s is negative)
            if (diff>0)
              curr_rgb[index][color]-=s;
            //we need to decrease current value
            else
              curr_rgb[index][color]+=s;           
          }
        }
      }
    }
  }
   
  // Sending a byte with a MSB of zero enables the output
  // Also resets the index at which new commands start.
  SPI.transfer(0);
  

  

}

byte redWave(byte index) {
  static byte counter = 0;
  static byte pos = 0;
  if (currentMillis >= nextRed) {
    nextRed = currentMillis + intervalRed;
    counter = (counter + 3) % 30;
    if (counter == 0) {
      pos = ++pos % redFrequency; // pos++; // run red lights in forward direction
    }
  }
  switch ((index + pos) % redFrequency) {
    case 0:
      return counter / 2;
    case 1:
      return 15 + counter;
    case 2:
      return 45 + counter * 2;
    case 3:
      return 105 - counter * 2;
    case 4:
      return 45 - counter;
    case 5:
      return 15 - counter / 2;
    default:
      return 0;
  }
}

byte blueWave(byte index) {
  static byte counter = 0;
  static byte pos = 0;
  if (currentMillis >= nextBlue) {
    nextBlue = currentMillis + intervalBlue;
    counter = (counter + 3) % 30;
    if (counter == 0) {
      pos = ++pos % blueFrequency;
    }
  }
  switch ((index + pos) % blueFrequency) {
    case 0:
      return counter / 2;
    case 1:
      return 15 + counter;
    case 2:
      return 45 + counter * 2;
    case 3:
      return 105 - counter * 2;
    case 4:
      return 45 - counter;
    case 5:
      return 15 - counter / 2;
    default:
      return 0;
  }
}

byte greenWave(byte index) {
  static byte counter = 0;
  static byte pos = 0;
  if (currentMillis >= nextGreen) {
    nextGreen = currentMillis + intervalGreen;
    counter = (counter + 3) % 30;
    if (counter == 0) {
      if (pos == 0)
        pos = greenFrequency - 1;
      else
        --pos; // run green lights in opposite direction
    }
  }
  switch ((index + pos) % greenFrequency) {
    case 0:
      return 15 - counter / 2;
    case 1:
      return 45 - counter;
    case 2:
      return 105 - counter * 2;
    case 3:
      return 45 + counter * 2;
    case 4:
      return 15 + counter;
    case 5:
      return counter / 2;
    default:
      return 0;
  }
}
