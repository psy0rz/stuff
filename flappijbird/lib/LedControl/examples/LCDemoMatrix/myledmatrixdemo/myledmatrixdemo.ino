
//Vcc=5v grd=grd din=12 cs=10 clk=11
#include "LedControl.h"

LedControl lc=LedControl(12,11,10,1);

unsigned long delaytime=500;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00111110,B00010000,B00100000,B00100000,B00010000};
  byte d[5]={B00011100,B00100010,B00100010,B00010010,B11111110};
  byte u[5]={B00111100,B00000010,B00000010,B00000100,B00111110};
  byte i[5]={B00000000,B00100010,B10111110,B00000010,B00000000};
  byte n[5]={B00111110,B00010000,B00100000,B00100000,B00011110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

  /* now display them one by one with a small delay */
  lc.setColumn(0,0,a[0]);
  lc.setColumn(0,1,a[1]);
  lc.setColumn(0,2,a[2]);
  lc.setColumn(0,3,a[3]);
  lc.setColumn(0,4,a[4]);
  delay(delaytime);
  lc.setColumn(0,0,r[0]);
  lc.setColumn(0,1,r[1]);
  lc.setColumn(0,2,r[2]);
  lc.setColumn(0,3,r[3]);
  lc.setColumn(0,4,r[4]);
  delay(delaytime);
  lc.setColumn(0,0,d[0]);
  lc.setColumn(0,1,d[1]);
  lc.setColumn(0,2,d[2]);
  lc.setColumn(0,3,d[3]);
  lc.setColumn(0,4,d[4]);
  delay(delaytime);
  lc.setColumn(0,0,u[0]);
  lc.setColumn(0,1,u[1]);
  lc.setColumn(0,2,u[2]);
  lc.setColumn(0,3,u[3]);
  lc.setColumn(0,4,u[4]);
  delay(delaytime);
  lc.setColumn(0,0,i[0]);
  lc.setColumn(0,1,i[1]);
  lc.setColumn(0,2,i[2]);
  lc.setColumn(0,3,i[3]);
  lc.setColumn(0,4,i[4]);
  delay(delaytime);
  lc.setColumn(0,0,n[0]);
  lc.setColumn(0,1,n[1]);
  lc.setColumn(0,2,n[2]);
  lc.setColumn(0,3,n[3]);
  lc.setColumn(0,4,n[4]);
  delay(delaytime);
  lc.setColumn(0,0,o[0]);
  lc.setColumn(0,1,o[1]);
  lc.setColumn(0,2,o[2]);
  lc.setColumn(0,3,o[3]);
  lc.setColumn(0,4,o[4]);
  delay(delaytime);
  lc.setColumn(0,0,0);
  lc.setColumn(0,1,0);
  lc.setColumn(0,2,0);
  lc.setColumn(0,3,0);
  lc.setColumn(0,4,0);
  delay(delaytime);
}
void loop() { 
  writeArduinoOnMatrix();
}

