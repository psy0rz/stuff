#!/bin/bash

# Openvpn iptables rules based on certificate name.
# (C)2018 SYN-3 edwin@datux.nl

OPERATION=$1
IP=$2
CN=$3

CHAIN="openvpn_$IP"

log()
{
 logger -t learn-address "$1"
}

allow_tcp()
{
    log "Allowing tcp to $1 port $2"
    iptables -I $CHAIN -d $1 -p tcp --dport $2 -jACCEPT
}

# always cleanup first
log "Deleting $IP"
iptables -D FORWARD -s $IP -j $CHAIN
iptables -F $CHAIN
iptables -X $CHAIN


if [ "$OPERATION" == "add" ] || [ "$OPERATION" == "update" ]; then
    # add chain
    log "Adding $IP for $CN"
    iptables -N $CHAIN
    iptables -I FORWARD -s $IP -j $CHAIN

    # add rules
    if [ -e /etc/openvpn/rules/$CN ]; then
        source /etc/openvpn/rules/$CN
    fi

fi


exit 0
