from machine import Pin
import time
import urequests
import config
from machine import Timer
import machine
import sys


led = Pin(2, Pin.OUT)


def store(data):
    try:
        led.off()
        # print("HTTP posting to {}".format(config.db))

        resp=urequests.post(config.db, data=data, headers={ 'Content-Type': 'text/plain' })


        if resp.status_code!=204:
            print('HTTP response "{}", text: {} '.format(resp.reason,resp.text))
            print("DB ERROR")
        else:
            led.on()

        return(True)

    except Exception as e:
        print('HTTP error: {}'.format(e))
        return(False)

import o
dat = machine.Pin(13)
ow=o.OneWire(dat)
ds = o.DS18X20(ow)


while len(ds.roms)!=4:
    print("scanning...")
    ds = o.DS18X20(ow)

for r in ds.roms:
    ds.start_conversion(r)

def measure(bla=None):

    data='temps '

    for r in ds.roms:
        hexaddr = ''.join([str(hex((i)))[2:4] for i in r]);
        temp=ds.read_temp_async(r)
        print(temp)
        if temp>-5 and temp<110:
            data=data+"temp_{}={},".format(hexaddr,temp)

    for r in ds.roms:
        ds.start_conversion(r)

    print("---")

    store(data[:-1])
    pass

# tim = Timer(-1)
# tim.init(period=1000, mode=Timer.PERIODIC, callback=measure)
while True:
    time.sleep(5)
    measure()
