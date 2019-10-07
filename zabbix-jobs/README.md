# Zabbix job monitor 

## Usage 

```
Usage: /bin/zabbix-job-status <jobname> <interval> <exitcode> [ <ignorecode>... ] 
Interval is used to generate alerts if the job is not run for a specific time. (with margins)
 hourly  : job should report every hour
 daily   : job should report every day
 weekly  : job should report every week
 montly  : job should report every month
 manual  : job is run manually, not at a specific interval.

Specify ignorecodes for exitcodes that you want to 'ignore' and report 0 instead. Usefull for ignoring exit code 24 in rsync jobs for example.
```

Import and enable the template first. You dont have to create the jobs manually, the script will do that automaticly for you.

## Example 

Example backup script:
```
rsync -avx server01:/home /backup02/server01
zabbix-job-status backup_server01 daily $? 24
```

Call this script from a cronjob every day. 

If it fails you'll get a trigger with Average level, like this:
```
Job backup_server01 on backup02 has exit code 1

```

If it doesnt send any report for 2 days it will also trigger:
```
Last daily report of backup_server01 on backup02 too long ago.
```

If the last succesfull job is longer than a week ago it will give a high alert:
```
Last succesfull daily job backup_server01 on backup02 was longer than a week ago.
```

For hourly, weekly and monthly backups there are other timeperiodes, look in the template.

