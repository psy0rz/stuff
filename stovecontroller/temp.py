from max6675 import MAX6675
from machine import Pin
import time
import urequests
import config
from machine import Timer
from PID import PID
import machine
import sys

sck = Pin(14, Pin.OUT)
cs = Pin(12, Pin.OUT)
so = Pin(13, Pin.IN)
led = Pin(2, Pin.OUT)

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

servosmall_pwm = machine.PWM(machine.Pin(5), freq=50)
# pid = PID(1, 0.1, 0.05, setpoint=180, sample_time=1, proportional_on_measurement=True, output_limits=((27,130)) )


# pid = PID(4, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=True, output_limits=((27,130)) )
# pid = PID(4, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=False, output_limits=((27,130)) )
pid = PID(8, 0.2, 1, setpoint=config.setpoint, sample_time=1, proportional_on_measurement=False, output_limits=((27,130)) )

servosmall=int((130-27)/2)

def measure(t):
    global last_send
    global req_data
    global temp
    global servosmall

    temp_total=0
    temps=0
    while temps<3:
        while not sens_pipe.ready():
            pass

        cur=sens_pipe.read()
        #skip errornous measurements
        if temp!=0 and abs(cur-temp)>20:
            print("SKIP {}".format(cur))
        else:
            temp_total=temp_total+cur
            temps=temps+1

    temp=temp_total/temps

    if temp>=1000:
        #error, dont waist graph
        print("READ ERROR")
        temp=-1
        return

    #do the pid magic
    servosmall_want=int(pid(temp))

    #servo is slow, so do gradual steps or else it locks up
    maxstep=10
    step=max(min(maxstep, servosmall_want-servosmall), -maxstep)
    if step:
        servosmall=servosmall+step
        servosmall_pwm.duty(servosmall)
    else:
        #turn off, to prevent annoying noise
        servosmall_pwm.duty(0)



    # print("temp={}, setpoint={}, servo={}".format(temp, pid.setpoint, servosmall))


    req_data=req_data+'temps temp={},servosmall={},setpoint={},P={},I={},D={}\n'.format(temp, servosmall,pid.setpoint, pid._proportional, pid._error_sum, pid._differential)
    # if time.time()-last_send>=0:
    store(req_data)
    req_data=""
    last_send=time.time()

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


tim = Timer(-1)
tim.init(period=1000, mode=Timer.PERIODIC, callback=measure)

#ctrl()
