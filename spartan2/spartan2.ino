/*
 * This Code example has been tested with Arduino Uno and Arduino Diecimila
 * SDA and SLC lines require 4.7k pullups
 * Connect Arduino SDA to Spartam-2-OEM-I2C SDA
 * Connect Arduino SLC to Spartam-2-OEM-I2C SCL
 * Data from Spartam-2-OEM-I2C is diplayed on the Arduino IDE Serial Monitor
 * If you need to force/reset Spartam-2-OEM-I2C's I2C Address, please read the last page of the Spartam_2_OEM_I2C Manual 
 */

#include <Wire.h>
#include <avr/pgmspace.h>
void setup() {
  Wire.begin(); // Initialize I2C
  Serial.begin(9600); //Initialize Serial Port
  Serial.println("Spartan 2 OEM I2C Arduino Example");
  Serial.println(); 
}
//#define I2C_Address 0 // This is the I2C Address of Spartan 2 OEM
#define Pump_Current16_to_Lambda_Start 0 // To get index for Pump_Current16_to_Lambda Array for we take Pump_Current16 and subtract Pump_Current16_to_Lambda_Start
#define Pump_Current16_to_Lambda_Size 409 //size of Pump_Current16_to_Lambda Array
#define Ri_to_Temperature_C_Start 113 // To get index for Ri_to_Temperature_C Array for we take Ri and subtract Ri_to_Temperature_C_Start
#define Ri_to_Temperature_C_Size 75 //size of Ri_to_Temperature_C Array
//lookup table to convert Pump_Current16 to Lambda
// const PROGMEM unsigned int Pump_Current16_to_Lambda[]={376,379,381,384,387,390,393,395,398,401,403,406,409,411,414,417,419,422,424,427,430,432,435,437,440,442,445,447,450,452,455,457,460,462,464,467,469,472,474,476,479,481,483,486,488,490,493,495,497,499,502,504,506,508,511,513,515,517,520,522,524,526,528,530,533,535,537,539,541,543,546,548,550,552,554,556,558,560,562,564,567,569,571,573,575,577,579,581,583,585,587,589,591,593,595,598,600,602,604,606,608,610,612,614,616,618,620,622,624,626,628,630,632,634,636,638,640,642,644,646,648,650,653,655,657,659,661,663,665,667,669,671,673,675,677,679,681,684,686,688,690,692,694,696,698,700,703,705,707,709,711,713,715,718,720,722,724,726,729,731,733,735,737,740,742,744,746,749,751,753,756,758,760,763,765,767,770,772,774,777,779,781,784,786,789,791,794,796,799,801,804,806,809,811,814,816,819,821,824,827,829,832,834,837,840,842,845,848,851,853,856,859,862,864,867,870,873,876,879,881,884,887,890,893,896,899,902,905,908,911,914,917,920,923,927,930,933,936,939,942,946,949,952,955,959,962,965,969,972,976,979,982,986,989,993,996,1000,1007,1015,1023,1031,1039,1047,1056,1064,1073,1082,1090,1099,1108,1118,1127,1136,1146,1156,1166,1176,1186,1196,1207,1217,1228,1239,1250,1262,1273,1285,1297,1309,1321,1334,1346,1359,1372,1386,1399,1413,1427,1442,1456,1471,1486,1502,1517,1533,1550,1566,1583,1600,1618,1636,1654,1673,1692,1711,1731,1752,1772,1794,1815,1837,1860,1883,1907,1931,1956,1981,2007,2033,2061,2088,2117,2146,2176,2207,2239,2271,2305,2339,2374,2411,2448,2486,2526,2567,2608,2652,2696,2742,2790,2839,2889,2942,2996,3052,3110,3171,3233,3298,3365,3435,3508,3584,3663,3745,3831,3920,4014,4112,4215,4323,4436,4555,4680,4812,4952,5099,5255,5420,5596,5783,5983,6196,6425,6670,6935,7220,7530,7866,8232,8634,9076,9564,10106,10713,11394,12167,13050,14068,15256,16659,18342,20399,22967,26268,30664,36811,46012,61299};
const PROGMEM unsigned int Pump_Current16_to_Lambda[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,29,44,59,74,89,104,119,134,148,163,178,193,207,222,237,252,266,281,296,310,325,339,354,368,383,397,412,426,441,455,470,484,498,513,527,541,556,570,584,598,613,627,641,655,669,683,697,711,726,740,754,768,782,796,809,823,837,851,865,879,893,907,920,934,948,962,975,989,1003,1016,1030,1044,1057,1071,1084,1098,1111,1125,1138,1152,1165,1179,1192,1206,1219,1232,1246,1259,1272,1286,1299,1312,1325,1339,1352,1365,1378,1391,1404,1417,1430,1443,1457,1470,1483,1496,1508,1521,1534,1547,1560,1573,1586,1599,1611,1624,1637,1650,1662,1675,1688,1701,1713,1726,1738,1751,1764,1776,1789,1801,1814,1826,1839,1851,1864,1876,1888,1901,1913,1925,1938,1950,1962,1975,1987,1999,2011,2023,2036,2048,2060};
//lookup table to convert Ri to Sensor Temperature[C]
const PROGMEM unsigned int Ri_to_Temperature_C[]={820,818,817,815,814,813,811,810,808,807,805,804,803,801,800,799,798,796,795,794,792,791,790,789,788,786,785,784,783,782,781,780,779,777,776,775,774,773,772,771,770,769,768,767,766,765,764,763,762,761,760,759,758,758,757,756,755,754,753,752,751,751,750,749,748,747,746,745,745,744,743,742,741,741,740};
byte Val_Array[10];
unsigned int i=0;
byte No_I2C_Response_Flag=1;
byte I2C_Address;
byte temp;
struct I2C_Struct{
  byte I2C_Addr;
  byte ID8;
  unsigned int Pump_Current16;
  byte Ri;
  byte Status8;
};
I2C_Struct I2C_Data; //This Struct holds the I2C data in the same form that it is on Spartan 2

struct Processed_Data_Struct{
  byte I2C_Addr;
  byte ID;
  float Lambda;
  unsigned int Temperature_C;
  byte Status;
};
Processed_Data_Struct Processed_Data; //This Struct hold the final Processed Data
void loop() {
  Serial.println("Enter I2C Address");  
  Serial.println(); 
  while (Serial.available()==0);
  I2C_Address=Serial.parseInt();
  Serial.println("Enter Option");
  Serial.println("[1]=datalog to screen");  
  Serial.println("[2]=Write New I2C Address to Spartan 2 OEM"); 
  Serial.println(); 
  while (Serial.available()==0);
  temp=Serial.parseInt();
  if (temp==1)
  {
    while(1)
    {
      Wire.beginTransmission(I2C_Address); //Setup communication with device @ I2C Address = 1 
      Wire.write(0); //This will tell Spartan that OEM that we want to start a read from Memory Address 0
      Wire.endTransmission();     // end transmission
      Wire.requestFrom(I2C_Address, 6); //Read 6 Bytes from Device with I2C Address = I2C_Address
      i=0;
      No_I2C_Response_Flag=1;
      while (Wire.available()) //Dump the I2C data into an Array
      {
        Val_Array[i] = Wire.read();
        No_I2C_Response_Flag=0;
        i++;
      }
      
      I2C_Data.I2C_Addr=Val_Array[0];
      I2C_Data.ID8=Val_Array[1];
      I2C_Data.Pump_Current16=(Val_Array[2]<<8) + Val_Array[3];
      I2C_Data.Ri=Val_Array[4];
      I2C_Data.Status8=Val_Array[5];
    
      Processed_Data.I2C_Addr=I2C_Data.I2C_Addr;
      Processed_Data.ID=I2C_Data.ID8;
      i=I2C_Data.Pump_Current16-Pump_Current16_to_Lambda_Start;
      // make sure array index is within array bounds
      if (i<0)
      {
          i=0;
      }
      if (i>(Pump_Current16_to_Lambda_Size-1))
      {
          i=Pump_Current16_to_Lambda_Size-1;
      }
      Processed_Data.Lambda=((float)pgm_read_word_near(Pump_Current16_to_Lambda+i))/1000; //use Pump_Current16 as index to Pump_Current16_to_Lambda lookup table and then Divide by 1000 to get actual Lambda
     
      i=I2C_Data.Ri-Ri_to_Temperature_C_Start;// Index has to be justified to the right by Ri_to_Temperature_C_Start
      // make sure array index is within array bounds
      if (i<0)
      {
          i=0;
      }
      if (i>(Ri_to_Temperature_C_Size-1))
      {
          i=Ri_to_Temperature_C_Size-1;
      }
      Processed_Data.Temperature_C=pgm_read_word_near(Ri_to_Temperature_C+i); //Use the Justified Ri as index to Ri_to_Temaperature_C lookup table to get get Sensor Temperature[C]
      Processed_Data.Status=I2C_Data.Status8;
      
      if (No_I2C_Response_Flag==1)
      {
        Serial.println("I2C no response");  
      }
      if (No_I2C_Response_Flag==0)
      {
        Serial.print("Communicating using I2C Address ");
        Serial.print(I2C_Address);
        Serial.print("\n");
        Serial.print("Spartan 2 onboard I2C Address is set to "); 
        Serial.print(Processed_Data.I2C_Addr); 
        Serial.print("\n");
        Serial.print("ID="); 
        Serial.print(Processed_Data.ID); 
        Serial.print("\n");
        Serial.print("Lambda=");
        //Serial.print(I2C_Data.Pump_Current16);  
        Serial.print(Processed_Data.Lambda); 
        Serial.print("\n");
        Serial.print("Temperature[C]="); 
        Serial.print(Processed_Data.Temperature_C); 
        Serial.print("\n");   
        Serial.print("Status="); 
        Serial.print(Processed_Data.Status); 
        Serial.print("\n");
        Serial.print("\n");
      }
      delay(500); //Change this to increase or decrease serial update rate 
    }
  }
  if (temp==2)
  {
    Serial.println("Enter New I2C Address to be written to Spartan 2 OEM");
    Serial.println(); 
    while (Serial.available()==0);
    temp=Serial.parseInt();
    Wire.beginTransmission(I2C_Address); //Setup communication with device @ I2C Address = 1 
    Wire.write(0); //This will tell Spartan that OEM that we want to start a read from Memory Address 0
    Wire.write(temp); //write New address to Spartan 2 OEM
    Wire.endTransmission(); 
    Serial.println("Done");
    Serial.println("Please Restart Console");
  }
 while(1);
<<<<<<< HEAD
}
=======
}
>>>>>>> ea9600e1ad988a96785c9ffef9b578a0c50dae40
