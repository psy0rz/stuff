# bare minimum to initalize webrepl and start meowton

# disable debug
# import esp
# esp.osdebug(1)

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

# import utelnetserver
# utelnetserver.start()


import temp
temp.run()



# import machine 
# from machine import Pin
# from pins import * 
# import time

# strip=machine.SPI(-1,sck=Pin(D1, Pin.OUT), mosi=Pin(D2,Pin.OUT), miso=Pin(D8), firstbit=0, polarity=1, phase=1, baudrate=1000000);

# a=bytearray(3)
# a[0]=200
# a[1]=200
# a[2]=200

# #reset

# numLEDs=32

# #reset
# time.sleep(1)
# strip.write(bytearray(b'\x00'))

# #time.sleep(0.001)
# #strip.write(bytearray(b'\x00'))

# #strip.write(bytearray(b'\x00'))

# # time.sleep(1)
# # strip.write(bytearray(b'\x00'))
# # time.sleep(1)
# # strip.write(bytearray(b'\x00'))
# # time.sleep(1)
# # strip.write(bytearray(b'\x00'))



