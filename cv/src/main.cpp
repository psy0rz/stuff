#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>


///////////////////////////////////////////////////// instellingen

// D1 = SCL van display
// D2 = SDA van display
// display op 5 volt

#define PIN_RESET_LAMP D5 //lamp met storings signaal
#define PIN_RESET_KNOP D3 //reset knop

#define PIN_HEATER D6 //olie verwarmings element
#define PIN_TEMP_SENSOR D7 // olie temperatuur sensor
#define PIN_TEMP_SETTING A0 //temperatuur instelling potmeter
#define TEMP_MIN 20 //min/max bereik temperatuur potmeter
#define TEMP_MAX 60

#define RESET_SNEL 10 //seconden
#define RESET_LANG 3600*2 //seconden
#define MAX_RESETS 3 //max aantal snelle resets

///////////////////////////////////////////////////// begin programma

LiquidCrystal_I2C lcd(0x27,16,2);  

OneWire oneWire(PIN_TEMP_SENSOR); //olie temp sensor
DallasTemperature temps(&oneWire);




//initialisatie van alles
void setup() {
  Serial.begin(115200); 
  pinMode(PIN_RESET_LAMP, INPUT_PULLUP);
  pinMode(PIN_RESET_KNOP,OUTPUT);
  pinMode(PIN_HEATER,OUTPUT);

  digitalWrite(PIN_RESET_KNOP,0);
  digitalWrite(PIN_HEATER,0);

  temps.begin();

  lcd.init();
  lcd.setBacklight(1);
  lcd.print("GME ketel 1.0");
  delay(1000);


}

//druk reset knop in
void reset_ketel()
{
  digitalWrite(PIN_RESET_KNOP,1);
  delay(500);
  digitalWrite(PIN_RESET_KNOP,0);

}

//is er een ketel storing?
bool ketel_storing()
{
  //als input "knippert" met 50hz is er storing
  bool begin_status=digitalRead(PIN_RESET_LAMP);
  for (int i=0; i<10; i++)
  {
    if (digitalRead(PIN_RESET_LAMP)!=begin_status)
      return(true);
      delay(10);
  }
  return(false);

  
}


bool verwarmen=0;

void regel_temperatuur()
{
  yield();

  //lees olie temperatuur
  temps.requestTemperatures();

  float gemeten_temp=temps.getTempCByIndex(0);
  if (gemeten_temp==-127)
    return;

  float gewenste_temp=TEMP_MIN+ (float(analogRead(PIN_TEMP_SETTING))*(TEMP_MAX-TEMP_MIN)/1024);


  //verwarmen
  if (gemeten_temp<=gewenste_temp-0.1)
  {
    verwarmen=1;
  }

  //koelen
  if (gemeten_temp>=gewenste_temp+0.1)
  {
    verwarmen=0;
  }
  digitalWrite(PIN_HEATER,verwarmen);


  //display update
  lcd.setCursor(0,1);
  if (verwarmen)
  {
    lcd.printf("%.1f (%.1f) * ", gemeten_temp, gewenste_temp);
  }
  else
  {
    lcd.printf("%.1f (%.1f)   ", gemeten_temp, gewenste_temp);    
  }


  
}




//wacht aantal seconden en blijf temperatuur regellen.
void wacht_en_regel(unsigned long seconden)
{
  unsigned long start_time=millis();
  while(millis()-start_time < seconden*1000)
  {
    yield();
    regel_temperatuur();
  }
}

int resets_over=MAX_RESETS;
unsigned long tijd_laatste_reset_verhoging=millis(); //in mS
int totaal_resets=0;

void loop() {

  lcd.setCursor(0,0);

  //reset teller verhogen
  unsigned long seconden_sinds_laatste_verhoging=(millis()-tijd_laatste_reset_verhoging)/1000;
  if (seconden_sinds_laatste_verhoging >= RESET_LANG)
  {
    if (resets_over<MAX_RESETS)
      resets_over++;
    
    tijd_laatste_reset_verhoging=millis();

    //reset display voor de zekerheid
    lcd.begin(16,2);
  }

  //storing?
  if (ketel_storing())
  {
    //mag resetten?
    if (resets_over>0)
    {
      lcd.print("RESETTEN        ");
      wacht_en_regel(RESET_SNEL);
      reset_ketel();
      resets_over--;
      totaal_resets++;
    }
    else
    //kk de kk wachten
    {
      lcd.printf("WACHT %ds    ", int(RESET_LANG-seconden_sinds_laatste_verhoging));
    }    
  }
  else
  {
    lcd.printf("OK R=%d (%d)            ", totaal_resets, resets_over);
  }
  

  regel_temperatuur();


}

