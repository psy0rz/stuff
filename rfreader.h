#include "Arduino.h"

//use this many samples for the moving average to determine the average timing of the signal
#define AVG_N 4
//minimum number of preamble bits to 'match' before we consider ourselfs synced.
#define MIN_PREAMBLE 5

//max idle time, after this time we assume the wave has ended.
#define MAX_TIME 2000 

//minimum number of data bits before a wave is considered valid
#define MIN_DATA 16



#define MAX_DATA 100

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
  unsigned long data;
};


