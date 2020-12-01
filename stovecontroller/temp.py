from max6675 import MAX6675
from machine import Pin
import time
import urequests
import config
from machine import Timer
from PID import PID
import machine
import sys
import uasyncio
from pins import *

#pin assignments
import sparta #D1 en D2
sck = Pin(D5, Pin.OUT)
cs = Pin(D6, Pin.OUT)
so = Pin(D7, Pin.IN)
led = Pin(D4, Pin.OUT) 
servosmall_pwm = machine.PWM(machine.Pin(D3), freq=50)


sens_pipe = MAX6675(sck, cs , so)

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
temp=0

# pid = PID(1, 0.1, 0.05, setpoint=180, sample_time=1, proportional_on_measurement=True, output_limits=((27,130)) )


# pid = PID(4, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=True, output_limits=((27,130)) )
# pid = PID(4, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=False, output_limits=((27,130)) )
servo_max=130
servo_min=27
pid = PID(8, 0.2, 1, setpoint=config.setpoint, sample_time=0, proportional_on_measurement=False, output_limits=((servo_min,servo_max)) )

servosmall=None

start_time=-config.coldstart_time
last_temp=0

def measure_loop():
    global last_send
    global req_data
    global temp
    global servosmall
    global start_time
    global last_temp

    while True:
        try:
            temp_total=0
            temps=0
            # while temps<3:
            #     while not sens_pipe.ready():
            #         pass

            #     cur=sens_pipe.read()
            #     #skip errornous measurements
            #     if temp!=0 and abs(cur-temp)>20:
            #         print("SKIP {}".format(cur))
            #         temp=cur
            #     else:
            #         temp_total=temp_total+cur
            #         temps=temps+1

            # temp=temp_total/temps
            while not sens_pipe.ready():
                pass

            temp_read=sens_pipe.read()
            if temp_read>=1000:
                #error, dont waist graph
                print("TEMP READ ERROR")
                continue

            temp=temp*0.9+ temp_read*0.1


            # oxygene=sparta.read_oxygene()
            oxygene=0

            #dont regulate when we're still starting up the stove (extra air outlet is open)
            if temp<config.coldstart_temp:
                start_time=time.time()

            if time.time()-start_time<config.coldstart_time:
                status="coldstarting"
                servosmall_want=servo_max
            if temp>config.bypass_open:
                status="bypass open"
                servosmall_want=servo_max
            else:
                status="regulating"
                #do the pid magic
                servosmall_want=int(pid(temp))

            last_temp=temp

            if servosmall==None:
                servosmall=servosmall_want

            #servo is slow, so do gradual steps or else it locks up
            maxstep=1000
            step=max(min(maxstep, servosmall_want-servosmall), -maxstep)
            if step:
                servosmall=servosmall+step
            servosmall_pwm.duty(servosmall)
            # else:
            #     #turn off, to prevent annoying noise
            #     servosmall_pwm.duty(0)

            print("{:12} {:5.1f}°C  {:0.0%} ({:0.0%})".format(
                status, 
                temp, 
                (servosmall_want-servo_min)/(servo_max-servo_min), 
                (servosmall-servo_min)/(servo_max-servo_min)
            ))


            # print("temp={}, setpoint={}, servo={}".format(temp, pid.setpoint, servosmall))


            req_data=req_data+'temps temp={},servosmall={},setpoint={},P={},I={},D={},oxygene={}\n'.format(temp, servosmall,pid.setpoint, pid._proportional, pid._error_sum, pid._differential, oxygene)
            # if time.time()-last_send>=0:
            # store(req_data)
            req_data=""
            last_send=time.time()
        except Exception as e:
            raise
            print(str(e))

        # await uasyncio.sleep_ms(1000)


# def ctrl():
#
#
#     while True:
#         c=sys.stdin.read(1)
#         # if c=='q':
#         #     servosmall=servosmall+1
#         # elif c=='a':
#         #     servosmall=servosmall-1
#         # elif c=='w':
#         #     servosmall=servosmpid
#         # elif c=='s':
#         #     servosmall=servosmall-10
#         # elif c=='e':
#         #     servosmall=max
#         # elif c=='d':
#         #     servosmall=min
#         if c=='t':
#             pid.setpoint=pid.setpoint+1
#         elif c=='g':
#             pid.setpoint=pid.setpoint-1
#
#         print(pid.setpoint)
#
#         # if servosmall<min:
#         #     servosmall=min
#         #
#         # if servosmall>max:
#         #     servosmall=max
#
#
#
#         # print("servosmall={}".format(servosmall))
#         # s.duty(servosmall)
#
#         # if c=='z':
#         #     print("SLEEP")
#         #     s.duty(0)

def run():
    event_loop=uasyncio.get_event_loop()
    event_loop.create_task(measure_loop())
   
    # # start webinterface?
    # if config.run_webserver:
    #     webserver.run()
    # else:
    event_loop.run_forever()
