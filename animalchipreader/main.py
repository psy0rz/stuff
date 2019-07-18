
# disable debug
import esp
esp.osdebug(None)

### network stuff
# import config
# import network
# from network import WLAN
# wlan = WLAN(network.STA_IF) # get current object, without changing the mode
# wlan.active(True)
# # wlan.ifconfig(config.network)
# wlan.connect(config.wifi_essid, config.wifi_password)

# import webrepl
# webrepl.start()
from machine import UART

uart = UART(1, baudrate=9600, bits=8, parity=None, stop=2, rx=32)


#test read: b'\x02FC8977B6007E30010000000000\x00\xff\x03'
#datasheet: https://allinbest.com/blog/1342k-animal-tag-fdxb-iso11784-reader-module/

from machine import Pin
import time
import struct
# p = Pin(32, Pin.IN)
r = Pin(33, Pin.OUT)


lastreset=0

prev=0
count=0
while True:

    if time.time()!=lastreset:
        r.value(0)
        r.value(1)
        lastreset=time.time()

    # v=p.value()
    # if v!=prev:
    #     prev=v
    #     count=count+1
    #     print(count)
    line=uart.read()
    if line:
        count=count+1
        print(count)
        # print(line)
        card_str=line[1:11].decode()
        country_str=line[11:15].decode()

        #swap lsb msb:
        card_str_swapped=list(card_str)
        card_str_swapped.reverse()
        card_str_swapped="".join(card_str_swapped)
        card=int(card_str_swapped,16)

        country_str_swapped=list(country_str)
        country_str_swapped.reverse()
        country_str_swapped="".join(country_str_swapped)
        country=int(country_str_swapped,16)

        #official format:
        print("{}{:012}".format(country, card))
