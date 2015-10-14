Zabbix blacklist monitor 

This program checks ip adresses against blacklists creates a zabbix alert for each blacklisted ip. 

Its fast and does the checks in parallel. It has a rate-limiter to prevent too much flooding of the DNS servers.

Usage:

./zabbix-bl --zabbix_server 1.2.3.4 --host blacklistchecker --blacklists blacklists.txt --ips ips.txt 

You should create a cronjob that runs this script.

