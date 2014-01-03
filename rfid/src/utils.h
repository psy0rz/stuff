#ifndef _utils_h
#define _utils_h

void(* reboot) (void) = 0;




//dutycycle function to make stuff blink without using memory 
//returns true during 'on' ms, with total periode time 'total' ms.
bool duty_cycle(unsigned long on, unsigned long total, unsigned long starttime=0)
{
  if (!starttime)
    return((millis()%total)<on);
  else
    return(((millis()-starttime)%total)<on);
}


#endif