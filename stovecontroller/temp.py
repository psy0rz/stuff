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

last_send=0
req_data=""

pid = PID(1, 0.1, 0.05, setpoint=180, sample_time=0.1, proportional_on_measurement=True, output_limits=((27,130)) )

def measure(t):
    global last_send
    global req_data
    temp=max.read()
    if temp==1024:
        #error, dont waist graph
        temp=-1
        return

    servosmall=int(pid(temp))
    print("T={}, servo={}".format(temp, servosmall))

    # print(temp)
    req_data=req_data+'temps temp={},servosmall={}\n'.format(temp, servosmall)
    if time.time()-last_send>=4:
        store(req_data)
        req_data=""
        last_send=time.time()

global servosmall
def ctrl():
    import machine
    import sys
    global servosmall
    #SERVO
    s = machine.PWM(machine.Pin(5), freq=50)
    min=27
    max=130
    middle=77
    servosmall=middle

    while True:
        c=sys.stdin.read(1)
        if c=='q':
            servosmall=servosmall+1
        elif c=='a':
            servosmall=servosmall-1
        elif c=='w':
            servosmall=servosmall+10
        elif c=='s':
            servosmall=servosmall-10
        elif c=='e':
            servosmall=max
        elif c=='d':
            servosmall=min

        if servosmall<min:
            servosmall=min

        if servosmall>max:
            servosmall=max


        print("servosmall={}".format(servosmall))
        s.duty(servosmall)

        if c=='z':
            print("SLEEP")
            s.duty(0)


tim = Timer(-1)
tim.init(period=1000, mode=Timer.PERIODIC, callback=measure)

ctrl()
