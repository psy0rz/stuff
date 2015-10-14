Zabbix nmap port monitior
=========================

This program uses nmap to scan a IP block. It creates a zabbix alert for each open port. 
When the port is closed again, the trigger status will be OK again. After this the trigger and items will be removed in 14 days.

We use this to do a complete scan of a network with more than 200 hosts every night.

Ports that should be open can be acknowledged or disabled in zabbix.

Usage:
------
./zabbix-nmap --server my.zabbixserver.com --port 10052 --host nmapchecker 21.22.23.0/24

You should create a cronjob that runs this script.
