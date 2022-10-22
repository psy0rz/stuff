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
# import sparta #D1 en D2
sck = Pin(D5, Pin.OUT)  #
cs = Pin(D6, Pin.OUT)
so = Pin(D7, Pin.IN)    #
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
temp=None

# pid = PID(1, 0.1, 0.05, setpoint=180, sample_time=1, proportional_on_measurement=True, output_limits=((27,130)) )


# pid = PID(4, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=True, output_limits=((27,130)) )
# pid = PID(4, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=False, output_limits=((27,130)) )
servo_max=130
servo_min=27
pid = PID(8, 0.2, 0, setpoint=config.setpoint, sample_time=0, proportional_on_measurement=False, output_limits=((servo_min,servo_max)) )
# pid = PID(8, 0.8, 0, setpoint=config.setpoint, sample_time=0, proportional_on_measurement=False, output_limits=((servo_min,servo_max)) )
# pid = PID(8, 0.4, 0, setpoint=config.setpoint, sample_time=0, proportional_on_measurement=False, output_limits=((servo_min,servo_max)) )

servosmall=120

start_time=-config.coldstart_time
last_temp=0


def calc_factor(pwm):
    return (pwm-servo_min)/(servo_max-servo_min)

def measure_loop():
    global last_send
    global req_data
    global temp
    global servosmall
    global start_time
    global last_temp


    last_diff=0
    

    ledcount=0

    while True:
        try:
            while not sens_pipe.ready():
                pass

            temp_read=sens_pipe.read()
            if temp_read>=1000:
                #error, dont waist graph
                print("TEMP READ ERROR")
                continue

            if temp==None:
                temp=temp_read

            temp=temp*0.9+ temp_read*0.1

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

            #prevent 1 step jitter because of rounding errors
            diff=servosmall_want-servosmall
            if diff!=0:
                #it needs to be either a big step (>1), OR a step in the same direction.
                if abs(diff)>1 or (diff>0 and last_diff>0) or (diff<0 and last_diff<0):
                    if diff>0:
                        servosmall=servosmall+1
                    else:
                        servosmall=servosmall-1

                    last_diff=diff
                    servosmall_pwm.duty(servosmall)
            else:
                servosmall_pwm.duty(0)



            # oxygene=sparta.read_oxygene()
            oxygene=0


            print("{:12} {:5.1f}°C  {:0.0%} (P={:3.0%} I={:3.0%} D={:3.0%} SERV={:d})".format(
                status, 
                temp, 
                # (servosmall_want-servo_min)/(servo_max-servo_min), 
                calc_factor(servosmall),
                (pid._proportional/(servo_max-servo_min)), 
                calc_factor(pid._error_sum),
                (pid._differential/(servo_max-servo_min)),
                servosmall
                
            ))

            #led feedback for servo amount
            ledcount=(ledcount+1)
            if ledcount>=(1-calc_factor(servosmall))*10:
                ledcount=0
                led.value(not led.value())




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



def run():
    event_loop=uasyncio.get_event_loop()
    event_loop.create_task(measure_loop())
   
    # # start webinterface?
    # if config.run_webserver:
    #     webserver.run()
    # else:
    event_loop.run_forever()
