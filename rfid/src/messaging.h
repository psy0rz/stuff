#ifndef messaging_h
#define messaging_h

#include <Arduino.h>
#include <RF24Network.h>
#include <RF24.h>
#include <avr/pgmspace.h>
#include "eeprom_config.h"
#include "utils.h"
#include <MemoryFree.h>


#define MAX_MSG (32-sizeof(RF24NetworkHeader))
#define MASTER_NODE 0 //node to send all our events to. (this doenst have to be the rootnode)


// nRF24L01(+) radio using the Getting Started board

class Msg
{

  public:
  RF24 radio;
  RF24Network network;

  Msg() : radio(CE_RF24_PIN, CS_RF24_PIN), network(radio)
  {

  }

  void begin()
  {
    SPI.begin();
    radio.begin();
    network.begin(100, config.node_id);

    char par[MAX_MSG];
    sprintf_P(par, PSTR("%i free"), freeMemory());
    send(PSTR("node.boot"), par);

  }

  //send a raw message-line to a node (and do serial echoing and error checking)
  bool send_line(const char * msg_buf, uint16_t to_node=MASTER_NODE)
  {
      RF24NetworkHeader header(to_node, 'l');

      //hoeft niet?
      //if we're not the target, print sended messages on serial as well
      //this way we can use the serial api also when there is no network connection.
      // if (config.node_id!=to_node)
      // {
      //   Serial.print('0');
      //   Serial.print(config.node_id,OCT);
      //   Serial.print(' ');
      //   Serial.println(msg_buf);
      // }

      if (!network.write(header,msg_buf,strlen(msg_buf)+1))
      {
        //this is mostly for debugging:
        Serial.print('0');
        Serial.print(config.node_id,OCT);
        Serial.print(F(" net.error "));
        Serial.println(msg_buf);
        return(false);
      }

      return(true);

  }

  //send message with string parameter to master
  //NOTE: this should always be a PSTR()
  bool send(const char * event, char *par=NULL)
  {
      char msg_buf[MAX_MSG]; 

      strncpy_P(msg_buf, event, sizeof(msg_buf));
      msg_buf[MAX_MSG-1]=0;

      int left=MAX_MSG-1-strlen(msg_buf);
      if (par!=NULL && left>2)
      {
        ;
        strcat(msg_buf," ");
        strncat(msg_buf, par,left-1);
      }
      return(send_line(msg_buf));
  }

  //should be implemented in our main sketch and call other handlers and/or do stuff
  void handle(uint16_t from, char * event,  char * par);


  bool internal_handle(uint16_t from, char * event,  char * par)
  {
    //echo received messages to host computer
    Serial.print('0');
    Serial.print(from,OCT);
    Serial.print(' ');
    Serial.print(event);
    Serial.print(' ');
    Serial.println(par);

    //change node id
    if (strcmp_P(event, PSTR("node.id"))==0)
    {
      //update eeprom
      sscanf(par,"%u", &config.node_id);
      config_update();
      delay(1000);
      reboot();
      return(true); //never reached
    }
    
    return(false);
  }


  //check if there are messages to receive from the network, or send from serial.
  void loop()
  {
    char msg_buf[MAX_MSG]; 
    static unsigned long last_ping=-60000;
    //Pump the network regularly
    network.update();

    //network message available?
    while ( network.available() )
    {
      // If so, take a look at it 
      RF24NetworkHeader header;
//      network.peek(header);

      network.read(header,msg_buf, sizeof(msg_buf));

      //correct message type? 
      if (header.type=='l')
      {
        msg_buf[sizeof(msg_buf)-1]=0; //ensure 0 termination

        char * par;
        par=strchr(msg_buf,' ');
        if (par!=NULL)
        {
          (*par)=0; //0 terminate event-name
          par++; //par starts at next byte
        }

        if (!internal_handle(header.from_node, msg_buf, par))
          handle(header.from_node, msg_buf, par);
      }
    }

    //serial message available?
    if (Serial.available())
    {

      //to node?
      uint16_t to_node;
      Serial.readBytesUntil(' ', msg_buf, sizeof(msg_buf));
      sscanf(msg_buf,"%o",&to_node);

      //the rest will become one 0 terminated string:
      byte len=Serial.readBytesUntil('\n', msg_buf, sizeof(msg_buf)-1);
      msg_buf[len]=0;
      send_line(msg_buf, to_node);
    }

    //ping master node
    if ( millis()-last_ping >= 60000)
    {
      send(PSTR("node.ping"));
      last_ping=millis();
    }

  }

};

Msg msg;

#endif