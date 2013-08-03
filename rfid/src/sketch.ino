
byte BLINK_PINS[]={PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7};


void setup() {                 
    for(byte pin_index=0; pin_index<sizeof(BLINK_PINS); pin_index++)
    {
      pinMode(BLINK_PINS[pin_index], OUTPUT);
    }
}


void loop() {
    static unsigned long blink_last_time=0;
    //static byte blink_count=0;
    static word blink_bits=1;

    if (millis()-blink_last_time> 100)
    {

      for (word bit=0; bit<sizeof(BLINK_PINS); bit++)
      {
        word drop_bit  = ((blink_bits >> 0) ^ (blink_bits >> 2) ^ (blink_bits >> 3) ^ (blink_bits >> 5) ) & 1;
        blink_bits =  (blink_bits >> 1) | (drop_bit << sizeof(BLINK_PINS));
      }

      //convert bits to actual output
      for (word bit=0; bit<sizeof(BLINK_PINS); bit++)
      {
        if (blink_bits & (1<<bit))
          digitalWrite(BLINK_PINS[bit], HIGH);
        else
          digitalWrite(BLINK_PINS[bit], LOW);
      }
      blink_last_time=millis();
    }
}
