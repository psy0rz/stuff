#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


// D1 = SCL van display
// D2 = SDA van display
// display op 5 volt

#define PIN_RESET_LAMP D5
#define PIN_RESET_KNOP D6 //relay board

LiquidCrystal_I2C lcd(0x27,16,2);  


#define RESET_SNEL 3 //seconden
#define RESET_LANG 3 //seconden

//druk reset knop in
void reset_ketel()
{
  delay(500);
  
  pinMode(PIN_RESET_KNOP,OUTPUT);
  digitalWrite(PIN_RESET_KNOP,0);
  delay(500);
  digitalWrite(PIN_RESET_KNOP,1);
  delay(1000);

}

void setup() {
  Serial.begin(115200); 
  pinMode(PIN_RESET_LAMP, INPUT_PULLUP);

  lcd.init();
  lcd.setBacklight(1);
  lcd.print("GME ketel 1.0");
  delay(1000);

}



//is er een ketel storing?
bool ketel_storing()
{
  //als input "knippert" met 50hz is er storing
  bool begin_status=digitalRead(PIN_RESET_LAMP);
  for (int i=0; i<100; i++)
  {
    if (digitalRead(PIN_RESET_LAMP)!=begin_status)
      return(true);
  }
  return(false);

  
}


int storing_count=0;
unsigned long storing_start_time=0;


void loop() {

  //wacht op ketel OK
  lcd.clear();
  lcd.print("Wachten op OK");
  while(ketel_storing()) yield();


  //wachten zolang er geen storing is
  lcd.clear();
  lcd.print("Ketel OK");
  while(!ketel_storing()) yield();
  
  //storing, wacht kort
  lcd.clear();
  lcd.print("Wacht kort...");
  delay(RESET_SNEL*1000);

  lcd.clear();
  lcd.print("Resetten");
  reset_ketel();

  //wachten zolang er geen storing is
  lcd.clear();
  lcd.print("Ketel OK");
  while(!ketel_storing()) yield();

  //storing, wacht lang
  lcd.clear();
  lcd.print("Wacht lang...");
  delay(RESET_LANG*1000);

  lcd.clear();
  lcd.print("Resetten");
  reset_ketel();


  //wachten zolang er geen storing is
  lcd.clear();
  lcd.print("Ketel OK");
  while(!ketel_storing())  yield();

  lcd.clear();
  lcd.print("You're fucked");
  while(1)  yield();

}

