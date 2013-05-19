#!/usr/bin/env python2

#(C)2013 Edwin Eefting edwin@datux.nl
# based on http://open.syn3.nl/syn3/trac/default/browser/trunk/projects/movingsign-m500N-7x80rg2/writemarquee

# a small python server that can show status messages on a marquee and make alert sounds if something is wrong.

#example: 
#    http://server:8080/zabbix/alert/somehost
#    http://server:8080/airco/ok/23


import bottle
import re
import traceback
import sys
import os.path
import urllib
import serial
import collections

#Set comport with stty -F /dev/ttyS0 2400
#(8,n,1)


### init
marquee_macros = {
    "X0" : "\xef\x60",
    "X1" : "\xef\x61",
    "X2" : "\xef\x62",
    "X3" : "\xef\x63",
    "X4" : "\xef\x64",
    "X5" : "\xef\x65",
    "X6" : "\xef\x66",
    "X7" : "\xef\x67",
    "X8" : "\xef\x68",
    "X9" : "\xef\x69",
    "XA" : "\xef\x6a",
    "XB" : "\xef\x6b",
    "XC" : "\xef\x6c",
    "XD" : "\xef\x6d",
    "XE" : "\xef\x6e",
    "XF" : "\xef\x6f",
    "XG" : "\xef\x70",
    "XH" : "\xef\x71",
    "XI" : "\xef\x72",
    "XJ" : "\xef\x73",
    "XK" : "\xef\x74",
    "XL" : "\xef\x75",
    "XM" : "\xef\x76",
    "XN" : "\xef\x77",
    "XO" : "\xef\x78",
    "XP" : "\xef\x79",
    "XQ" : "\xef\x7a",
    "XR" : "\xef\x7b",
    "XS" : "\xef\x7c",
    "XT" : "\xef\x7d",
    "F0" : "\xef\xa0",
    "F1" : "\xef\xa1",
    "F2" : "\xef\xa2",
    "F3" : "\xef\xa3",
    "F4" : "\xef\xa4",
    "F5" : "\xef\xa5",
    "C0" : "\xef\xb0",        # deepred
    "C1" : "\xef\xb1",        # red
    "C2" : "\xef\xb2",        # dark orange
    "C3" : "\xef\xb3",        # yellow
    "C4" : "\xef\xb4",        # orange
    "C5" : "\xef\xb5",        # deep orange
    "C6" : "\xef\xb6",        # green
    "C7" : "\xef\xb7",        # light green
    "C8" : "\xef\xb8",        # rainbow
    "C9" : "\xef\xb9",        # mixed green down
    "CA" : "\xef\xba",        # red/orange
    "CB" : "\xef\xbb",        # green/red
    "CC" : "\xef\xbc",        # red background green char.
    "CD" : "\xef\xbd",        # green backround red char.
    "CE" : "\xef\xbe",        # red background orange char.
    "CF" : "\xef\xbf",        # green background orange char.
    "A0" : "\xef\x90",
    "A1" : "\xef\x91",
    "A2" : "\xef\x92",
    "A3" : "\xef\x93",
    "A4" : "\xef\x94",
    "A5" : "\xef\x95",
    "A6" : "\xef\x96",
    "A7" : "\xef\x97",
    "S0" : "\xef\xc0",
    "S1" : "\xef\xc1",
    "S2" : "\xef\xc2",
    "S3" : "\xef\xc3",
    "S4" : "\xef\xc4",
    "S5" : "\xef\xc5",
    "S6" : "\xef\xc6",
    "S7" : "\xef\xc7",
    "P0" : "\xef\xc8",
    "P1" : "\xef\xc9",
    "P2" : "\xef\xca",
    "P3" : "\xef\xcb",
    "P4" : "\xef\xcc",
    "P5" : "\xef\xcd",
    "P6" : "\xef\xce",
    "P7" : "\xef\xcf",
    "B0" : "\xef\xe0",
    "B1" : "\xef\xe1",
    "B2" : "\xef\xe2",

    "M0" : "\xff\x01", # inmediate/plain/no effect
    "M1" : "\xff\x02", # right to left
    "M2" : "\xff\x03", # left to right
    "M3" : "\xff\x04", # in to out
    "M4" : "\xff\x05", # out to in
    "M5" : "\xff\x06", # in to out still
    "M6" : "\xff\x07", # middle right left
    "M7" : "\xff\x08", # middle left right
    "M8" : "\xff\x09", # middle out to in
    "M9" : "\xff\x0a", # middle in to out
    "MA" : "\xff\x0b", # scroll up
    "MB" : "\xff\x0c", # scroll down
    "MC" : "\xff\x0d", # merge out to in
    "MD" : "\xff\x0e", # merge middle
    "ME" : "\xff\x0f", # appear from middle down
    "MF" : "\xff\x10", # appear from middle up
    "MG" : "\xff\x11", # scanner
    "MH" : "\xff\x12", # fireworks
    "MI" : "\xff\x13", # pac-man
    "MJ" : "\xff\x14", # stack 
    "MK" : "\xff\x15", # shoot from right
    "ML" : "\xff\x16", # flash
    "MM" : "\xff\x17", # shoot in midle
    "MN" : "\xff\x18", # char. by char. from right
    "MO" : "\xff\x19", # random
}

marquee_port=serial.Serial("/dev/ttyS0",2400)

def write_marquee(msg):
    for (macro,cmd) in marquee_macros.items():
        msg=re.sub("\$"+macro,cmd,msg)

    #header
    marquee_port.write("\x00\xff\xff\x00\x0b\x00\xff\x01\x30\x31\x02") 
    marquee_port.write(msg)
    #trailer
    marquee_port.write("\xff\xff\x00")

write_marquee("(restarted)")

status=collections.OrderedDict()

@bottle.route('/<params:path>')
def update(params):
    (key, prio, msg)=params.split("/")
    new_status={
            'prio':prio,
            'msg': urllib.unquote(msg)
        }

    if prio=="del":
        status.pop(key)
    else:
        if (key in status):
            # no changes ?
            if (status[key]==new_status):
                return 

            #deleted it so it gets added to the end
            status.pop(key)

        status[key]=new_status


    #create a nice marquee string of current status
    status_str=""
    for (key, state) in reversed(status.items()):
        if status_str!="":
            status_str+=" "

        #either use msg or key, to safe space        
        if state['msg']!="":
            txt=state['msg']
        else:
            txt=key

        if state['prio']=="ok":
            status_str+="$C7"+txt
        elif state['prio']=="caution":
            status_str+="$C3$B2"+txt+"$P7"
        elif state['prio']=="alert":
            status_str+="$C1$B0"+txt+"$P7"

    write_marquee(status_str)



if __name__ == '__main__':
    #bottle.debug(True)
    bottle.run(reloader=True, host='0.0.0.0', port=8080)


