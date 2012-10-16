#include "Arduino.h"


#define MAX_TIME 10000 
#define PREAMBLE 10
#define MAX_DATA 1000

//universal definition of a 433mhz 'wave'.
//we assume a few things here:
//-all data is encoded with 'long' and 'short' pulses.
//-there is always a preamble to stablelize the signal
struct Wave
{
  //time of a 1
  unsigned int high_time;
  //time of a 0
  unsigned int low_time;
  //number of data bits
  unsigned int data_count;
  //data bits
  boolean data[MAX_DATA];
};


