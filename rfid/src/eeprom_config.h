#ifndef eeprom_config_h
#define eeprom_config_h
#include <avr/eeprom.h>

//put all dynamic config in here.
//NOTE: we could save memory by making some macros that wrap a eeprom-read in realtime.

struct Config
{
  uint16_t node_id;
  word ra_unlock_time; //time to unlock door in seconds, after scanning a tag
} config;


void config_read()
{
  eeprom_read_block((void*)&config, (void*)512, sizeof(config));
}

void config_update()
{
  eeprom_update_block((const void*)&config, (void*)512, sizeof(config));
}


#endif
