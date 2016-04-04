Zabbix blacklist monitor
========================

(C)Datux 2015 - Sponsorred by Spothost

This program checks ip adresses against blacklists and creates a zabbix alert for each blacklisted ip entry. When the blacklisting is gone, the trigger status will be OK again. After this the trigger and items will be removed in 14 days.

Its fast and does the checks in parallel. It has a rate-limiter to prevent too much flooding of the DNS servers. We use it to check more than 700 servers against 70 blacklists.

Installing
----------

1. import the zbx_pl template into zabbix.
2. create a host in zabbix that is linked to this template. call this host "blacklistchecker".
3. create a file ips.txt that contain the ip adresses you want to check. you can also use CIDR notation in this file, for example: 1.2.3.0/24
4. try running the script manually to see if it works:
 ./zabbix-bl --zabbix_server 1.2.3.4 --host blacklistchecker --blacklists blacklists.txt --ips ips.txt 
 
5. if it works, create a cronjob that will run the script automaticly.




