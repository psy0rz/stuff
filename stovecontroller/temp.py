from max6675 import MAX6675
from machine import Pin
import time
import urequests
import config
from machine import Timer

sck = Pin(14, Pin.OUT)
cs = Pin(12, Pin.OUT)
so = Pin(13, Pin.IN)
led = Pin(2, Pin.OUT)

max = MAX6675(sck, cs , so)


#SERVO
#s = machine.PWM(machine.Pin(5), freq=50)
# 30/77/130


def store(data):
    try:
        led.off()
        # print("HTTP posting to {}".format(config.db))

        resp=urequests.post(config.db, data=data, headers={ 'Content-Type': 'text/plain' })


        if resp.status_code!=204:
            print('HTTP response "{}", text: {} '.format(resp.reason,resp.text))
            print("DB ERROR")

        # print("OK")
        led.on()
        return(True)

    except Exception as e:
        print('HTTP error: {}'.format(e))
        return(False)

def measure(t):
    req_data=""
    temp=max.read()
    # print(temp)
    req_data=req_data+'temps,sens=pijp temp={}\n'.format(temp)
    store(req_data)
    req_data=""



tim = Timer(-1)
tim.init(period=2000, mode=Timer.PERIODIC, callback=measure)
