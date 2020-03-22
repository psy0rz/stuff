#Module to read Spartan 2 OEM from https://www.14point7.com/products/spartan-2-oem
#NOTE: dont forget to add 2 pullup resistors of 4k7!

#messy, because i ported it from a C example.

import machine
from machine import Pin
from pins import *

SPARTA_ADDR=1


Pump_Current16_to_Lambda_Start=0 # To get index for Pump_Current16_to_Lambda Array for we take Pump_Current16 and subtract Pump_Current16_to_Lambda_Start
# Pump_Current16_to_Lambda_Size=409 #size of Pump_Current16_to_Lambda Array
# Ri_to_Temperature_C_Start=113 #To get index for Ri_to_Temperature_C Array for we take Ri and subtract Ri_to_Temperature_C_Start
# Ri_to_Temperature_C_Size=75 #size of Ri_to_Temperature_C Array

#lookup table to convert Pump_Current16 to Lambda
Pump_Current16_to_Lambda=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,29,44,59,74,89,104,119,134,148,163,178,193,207,222,237,252,266,281,296,310,325,339,354,368,383,397,412,426,441,455,470,484,498,513,527,541,556,570,584,598,613,627,641,655,669,683,697,711,726,740,754,768,782,796,809,823,837,851,865,879,893,907,920,934,948,962,975,989,1003,1016,1030,1044,1057,1071,1084,1098,1111,1125,1138,1152,1165,1179,1192,1206,1219,1232,1246,1259,1272,1286,1299,1312,1325,1339,1352,1365,1378,1391,1404,1417,1430,1443,1457,1470,1483,1496,1508,1521,1534,1547,1560,1573,1586,1599,1611,1624,1637,1650,1662,1675,1688,1701,1713,1726,1738,1751,1764,1776,1789,1801,1814,1826,1839,1851,1864,1876,1888,1901,1913,1925,1938,1950,1962,1975,1987,1999,2011,2023,2036,2048,2060]

#lookup table to convert Ri to Sensor Temperature[C]
# Ri_to_Temperature_C=[820,818,817,815,814,813,811,810,808,807,805,804,803,801,800,799,798,796,795,794,792,791,790,789,788,786,785,784,783,782,781,780,779,777,776,775,774,773,772,771,770,769,768,767,766,765,764,763,762,761,760,759,758,758,757,756,755,754,753,752,751,751,750,749,748,747,746,745,745,744,743,742,741,741,740]



sparta = machine.I2C(scl=machine.Pin(D1, Pin.OUT), sda=machine.Pin(D2, Pin.IN), freq=400000)


def read_oxygene():
    """read and return oxygene level"""

    #This will tell Spartan that OEM that we want to start a read from Memory Address 0
    sparta.writeto(SPARTA_ADDR, b'0')
    sparta_data=sparta.readfrom(SPARTA_ADDR, 6)

    #   I2C_Data.I2C_Addr=Val_Array[0];
    read_addr=sparta_data[0]
    
    #   I2C_Data.ID8=Val_Array[1];
    read_id8=sparta_data[1]

    #   I2C_Data.Pump_Current16=(Val_Array[2]<<8) + Val_Array[3];
    read_pump_current16=(sparta_data[2]<<8) + sparta_data[3]

    #   I2C_Data.Ri=Val_Array[4];
    read_ri=sparta_data[4]

    #   I2C_Data.Status8=Val_Array[5];
    read_status8=sparta_data[5]
    
    #   Processed_Data.I2C_Addr=I2C_Data.I2C_Addr;
    #   Processed_Data.ID=I2C_Data.ID8;
    #   i=I2C_Data.Pump_Current16-Pump_Current16_to_Lambda_Start;
    lambda_index=read_pump_current16-Pump_Current16_to_Lambda_Start

    #   // make sure array index is within array bounds
    #   if (i<0)
    #   {
    #       i=0;
    #   }
    #   if (i>(Pump_Current16_to_Lambda_Size-1))
    #   {
    #       i=Pump_Current16_to_Lambda_Size-1;
    #   }
    # print(lambda_index)
    lambda_index=min(max(0,lambda_index), len(Pump_Current16_to_Lambda)-1)

    #   Processed_Data.Lambda=((float)pgm_read_word_near(Pump_Current16_to_Lambda+i))/100; //use Pump_Current16 as index to Pump_Current16_to_Lambda lookup table and then Divide by 1000 to get actual Lambda
    oxygene=Pump_Current16_to_Lambda[lambda_index]/100
     
    #   i=I2C_Data.Ri-Ri_to_Temperature_C_Start;// Index has to be justified to the right by Ri_to_Temperature_C_Start
    #   // make sure array index is within array bounds
    #   if (i<0)
    #   {
    #       i=0;
    #   }
    #   if (i>(Ri_to_Temperature_C_Size-1))
    #   {
    #       i=Ri_to_Temperature_C_Size-1;
    #   }
    #   Processed_Data.Temperature_C=pgm_read_word_near(Ri_to_Temperature_C+i); //Use the Justified Ri as index to Ri_to_Temaperature_C lookup table to get get Sensor Temperature[C]
    #   Processed_Data.Status=I2C_Data.Status8;
      
    #   if (No_I2C_Response_Flag==1)
    #   {
    #     Serial.println("I2C no response");  
    #   }
    #   if (No_I2C_Response_Flag==0)
    #   {
    #     // Serial.print("Communicating using I2C Address ");
    #     // Serial.print(I2C_Address);
    #     // Serial.print("\n");
    #     // Serial.print("Spartan 2 onboard I2C Address is set to "); 
    #     // Serial.print(Processed_Data.I2C_Addr); 
    #     // Serial.print("\n");
    #     // Serial.print("ID="); 
    #     // Serial.print(Processed_Data.ID); 
    #     // Serial.print("\n");
    #     // Serial.print("Lambda=");
    #     Serial.print("P=");
    #     Serial.print(I2C_Data.Pump_Current16);  
    #     Serial.print(" ");
    #     Serial.print(Processed_Data.Lambda); 
    #     Serial.print("% T=");
    #     // Serial.print("Temperature[C]="); 
    #     Serial.print(Processed_Data.Temperature_C); 
    #     Serial.print("\n");   
    #     // Serial.print("Status="); 
    #     // Serial.print(Processed_Data.Status); 
    #     // Serial.print("\n");
    #     // Serial.print("\n");
    #   }

    return(oxygene)
