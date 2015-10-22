Zabbix blacklist monitor
========================

(C)Datux 2015 - Sponsorred by Spothost

This program checks ip adresses against blacklists and creates a zabbix alert for each blacklisted ip entry. When the blacklisting is gone, the trigger status will be OK again. After this the trigger and items will be removed in 14 days.

Its fast and does the checks in parallel. It has a rate-limiter to prevent too much flooding of the DNS servers. We use it to check more than 700 servers against 70 blacklists.

Usage:
------

./zabbix-bl --zabbix_server 1.2.3.4 --host blacklistchecker --blacklists blacklists.txt --ips ips.txt 

You should create a cronjob that runs this script.

