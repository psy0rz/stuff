#!/usr/bin/env python3
import time
import urllib.request

macs=[ "04:d6:aa:0f:e8:e9" ]
offline_delay=600
last_online_time=time.time()
online=True
url="http://192.168.13.25/json.htm?type=command&param=switchlight&idx=393&switchcmd="
online_url =url+"Off"
offline_url=url+"On"

from pyunifi.controller import Controller


c = Controller("wifi.kantoor2.datux.nl", "admin", "kutje123", 8443, "v5" , "default", ssl_verify=False)



while True:
    # get aps and client list
    aps = c.get_aps()
    ap_names = dict([(ap['mac'], ap.get('name')) for ap in aps])
    clients = c.get_clients()
    clients.sort(key=lambda x: -x['rssi'])


    FORMAT = '%-16s  %18s  %-12s  %4s  %4s  %3s  %3s %s'
    print()
    print(FORMAT % ('NAME', 'MAC', 'AP', 'CHAN', 'RSSI', 'RX', 'TX', 'online'))

    for client in clients:
        ap_name = ap_names[client['ap_mac']]
        name = client.get('hostname') or client.get('ip', 'Unknown')
        rssi = client['rssi']
        mac = client['mac']
        rx = int(client['rx_rate'] / 1000)
        tx = int(client['tx_rate'] / 1000)
        channel = client['channel']


        if mac in macs:
            detected="*"
            last_online_time=time.time()
            if not online:
                print("SWITCHED TO ONLINE")
                online=True
                urllib.request.urlopen(online_url)

        else:
            detected=""

        print(FORMAT % (name, mac, ap_name, channel, rssi, rx, tx, detected))


    # print(time.time()-last_online_time)
    if time.time()-last_online_time>offline_delay and online:
        online=False
        print("SWITCHED TO OFFLINE")
        urllib.request.urlopen(offline_url)


    time.sleep(60)
