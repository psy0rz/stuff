
byte BLINK_PINS[]={PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};


void setup() {                 
    for(byte pin_index=0; pin_index<sizeof(BLINK_PINS); pin_index++)
    {
      pinMode(BLINK_PINS[pin_index], OUTPUT);
    }
}



void loop() {
    static unsigned long blink_last_time=0;
    static byte blink_count=0;
    if (millis()-blink_last_time> (((sizeof(BLINK_PINS)-blink_count)*20)+20))
    {

      digitalWrite(BLINK_PINS[blink_count], LOW);
      blink_count=(blink_count+1)%sizeof(BLINK_PINS);
      digitalWrite(BLINK_PINS[blink_count], HIGH);
      blink_last_time=millis();
    }
}
