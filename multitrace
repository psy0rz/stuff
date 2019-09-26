#!/usr/bin/env python
# -*- coding: utf8 -*-

from __future__ import print_function
import os
import sys
import re
import traceback
import subprocess
import pprint
import cStringIO
import time
from scapy.all import *

###### parse arguments
import argparse
parser = argparse.ArgumentParser(description='Multitrace 1.1 - multi protocol traceroute network analyser')
parser.add_argument('--max-hops', default=32, help='Max number of hops, default %(default)s')
parser.add_argument('--interval', type=float, default=0, help='Interval between packets, default %(default)s')
parser.add_argument('--timeout', type=float, default=1, help='Timeout waiting for answers, default %(default)s')
parser.add_argument('--retry', type=float, default=3, help='Retry amount after timeout, default %(default)s')
parser.add_argument('host', help='Host to trace')
# parser.add_argument('--verbose', action='store_true', help='verbose output')
# parser.add_argument('--debug', action='store_true', help='debug output (shows commands that are executed)')
args = parser.parse_args()


complete=False
for ttl in range(1,args.max_hops+1):
    print("Hop ", ttl, ": ")

    packets=[
            IP(dst=args.host, ttl=ttl)/UDP(dport=44443+ttl),
            IP(dst=args.host, ttl=ttl)/UDP(dport=53),
            IP(dst=args.host, ttl=ttl)/TCP(dport=44443+ttl, flags="S"),
            IP(dst=args.host, ttl=ttl)/TCP(dport=80, flags="S"),
            IP(dst=args.host, ttl=ttl)/TCP(dport=135, flags="S"),
            IP(dst=args.host, ttl=ttl)/TCP(dport=25, flags="S"),
            IP(dst=args.host, ttl=ttl)/ICMP(),
    ]

    packets[0].desc="UDP random"
    packets[0].id=1

    packets[1].desc="UDP 53"
    packets[1].id=2

    packets[2].desc="TCP random"
    packets[2].id=3

    packets[3].desc="TCP 80"
    packets[3].id=4

    packets[4].desc="TCP 135"
    packets[4].id=5

    packets[5].desc="TCP 25"
    packets[5].id=6

    packets[6].desc="ICMP"
    packets[6].id=7

    answers,unans=sr(packets, timeout=args.timeout, retry=args.retry, inter=args.interval, verbose=0)


    for packet in packets:
        found=False
        for snd, recv in answers:
            if snd.id==packet.id:
                print(" {: <10} : {}".format(packet.desc, recv.src))
                if snd.dst==recv.src:
                    complete=True
                found=True
                break
        if not found:
                print(" {: <10} : {}".format(packet.desc, "TIMEOUT"))



    if complete:
        break
