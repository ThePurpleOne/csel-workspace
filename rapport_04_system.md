Top of the original silly: CPU1 is at 100%

```
top - 00:22:33 up 22 min,  1 user,  load average: 0.38, 0.13, 0.04
Tasks:  95 total,   2 running,  93 sleeping,   0 stopped,   0 zombie
%Cpu0  :   0.0/0.0     0[          ]     %Cpu1  : 100.0/0.0   100[||||||||||]
%Cpu2  :   0.0/0.0     0[          ]     %Cpu3  :   0.7/0.0     1[          ]
GiB Mem :  8.5/0.5      [          ]
GiB Swap:  0.0/0.0      [          ]

  PID USER      PR  NI    VIRT    RES  %CPU  %MEM     TIME+ S COMMAND           
    1 root      20   0    2.6m   0.3m   0.0   0.1   0:01.78 S init              
  159 root      20   0    2.6m   0.3m   0.0   0.1   0:00.00 S  `- /sbin/syslog+ 
  163 root      20   0    2.6m   0.3m   0.0   0.1   0:00.00 S  `- /sbin/klogd + 
  175 root      20   0    3.0m   2.2m   0.0   0.5   0:00.15 S  `- /sbin/mdev -+ 
  246 root      20   0    6.0m   2.0m   0.0   0.4   0:00.00 S  `- sshd: /usr/s+ 
  249 root      20   0    2.6m   1.8m   0.0   0.4   0:00.07 S  `- -sh           
  265 root      20   0    1.7m   0.2m  99.3   0.0   0:23.92 R      `- ./silly_+ 
  266 root      20   0    3.0m   1.6m   0.7   0.3   0:00.27 R      `- top       
    2 root      20   0    0.0m   0.0m   0.0   0.0   0:00.01 S [kthreadd]        
    3 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [rcu_gp]      
    4 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [rcu_par_gp]  
    5 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [slub_flushw+ 
    6 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [netns]       
    8 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [kworker/0:0+ 
   10 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [mm_percpu_w+ 
   11 root      20   0    0.0m   0.0m   0.0   0.0   0:00.00 S  `- [rcu_tasks_k+ 
```



After modifications : 
```
# tail -f /var/log/messages
Jan  1 01:05:01 csel user.info silly_led[283]: started, freq=2 Hz
Jan  1 01:05:07 csel user.info silly_led[283]: k1 pressed: freq=3 Hz
Jan  1 01:05:09 csel user.info silly_led[283]: k1 pressed: freq=4 Hz
Jan  1 01:05:10 csel user.info silly_led[283]: k1 pressed: freq=5 Hz
Jan  1 01:05:10 csel user.info silly_led[283]: k1 pressed: freq=6 Hz
Jan  1 01:05:10 csel user.info silly_led[283]: k1 pressed: freq=7 Hz
Jan  1 01:05:10 csel user.info silly_led[283]: k1 pressed: freq=8 Hz
Jan  1 01:05:11 csel user.info silly_led[283]: k1 pressed: freq=9 Hz
Jan  1 01:05:11 csel user.info silly_led[283]: k1 pressed: freq=10 Hz
Jan  1 01:05:11 csel user.info silly_led[283]: k1 pressed: freq=11 Hz
Jan  1 01:05:13 csel user.info silly_led[283]: k2 pressed: freq=2 Hz
Jan  1 01:05:14 csel user.info silly_led[283]: k3 pressed: freq=1 Hz
Jan  1 01:05:16 csel user.info silly_led[283]: k1 pressed: freq=2 Hz
Jan  1 01:05:16 csel user.info silly_led[283]: k1 pressed: freq=3 Hz
Jan  1 01:05:16 csel user.info silly_led[283]: k1 pressed: freq=4 Hz
Jan  1 01:05:16 csel user.info silly_led[283]: k1 pressed: freq=5 Hz
Jan  1 01:05:16 csel user.info silly_led[283]: k1 pressed: freq=6 Hz
Jan  1 01:05:17 csel user.info silly_led[283]: k3 pressed: freq=5 Hz
Jan  1 01:05:17 csel user.info silly_led[283]: k3 pressed: freq=4 Hz
Jan  1 01:05:18 csel user.info silly_led[283]: k3 pressed: freq=3 Hz
Jan  1 01:05:18 csel user.info silly_led[283]: k3 pressed: freq=2 Hz
Jan  1 01:05:18 csel user.info silly_led[283]: k3 pressed: freq=1 Hz
Jan  1 01:05:19 csel user.info silly_led[283]: k2 pressed: freq=2 Hz

```

CPU is not at 100% with the timers

```
top - 01:07:15 up  1:07,  1 user,  load average: 0.01, 0.00, 0.06
Tasks:  95 total,   1 running,  94 sleeping,   0 stopped,   0 zombie
%Cpu0  :   0.0/0.0     0[          ]     %Cpu1  :   0.0/0.0     0[          ]
%Cpu2  :   0.7/0.7     1[          ]     %Cpu3  :   0.0/0.0     0[          ]
GiB Mem :  8.6/0.5      [          ]
GiB Swap:  0.0/0.0      [          ]

  PID USER      PR  NI    VIRT    RES  %CPU  %MEM     TIME+ S COMMAND           
    1 root      20   0    2.6m   0.3m   0.0   0.1   0:01.78 S init              
  159 root      20   0    2.6m   0.3m   0.0   0.1   0:00.01 S  `- /sbin/syslog+ 
  163 root      20   0    2.6m   0.3m   0.0   0.1   0:00.00 S  `- /sbin/klogd + 
  175 root      20   0    3.0m   2.2m   0.0   0.5   0:00.15 S  `- /sbin/mdev -+ 
  246 root      20   0    6.0m   2.0m   0.0   0.4   0:00.00 S  `- sshd: /usr/s+ 
  249 root      20   0    2.6m   1.8m   0.0   0.4   0:00.12 S  `- -sh           
  283 root      20   0    1.8m   0.2m   0.0   0.0   0:00.01 S      `- ./silly_+ 
  285 root      20   0    3.0m   1.6m   0.7   0.3   0:00.11 R      `- top       
    2 root      20   0    0.0m   0.0m   0.0   0.0   0:00.01 S [kthreadd]        
    3 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [rcu_gp]      
    4 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [rcu_par_gp]  
    5 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [slub_flushw+ 
    6 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [netns]       
    8 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [kworker/0:0+ 
   10 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [mm_percpu_w+ 
   11 root      20   0    0.0m   0.0m   0.0   0.0   0:00.00 S  `- [rcu_tasks_k+ 

```


# Exercice 1 

Check the running processes 

```
# ps aux
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.0   2688   272 ?        Ss   00:00   0:01 init
...
root       286  0.0  0.0      0     0 ?        I    01:25   0:00 [kworker/u8:1-e
root       287  0.0  0.0      0     0 ?        I    01:30   0:00 [kworker/u8:2-e
root       290  0.0  0.0   1868   196 ttyS0    S+   01:32   0:00 ./exercice01
root       291  0.0  0.0   1868    80 ttyS0    S+   01:32   0:00 ./exercice01
root       292  1.8  1.0   6548  4976 ?        Ss   01:32   0:00 sshd: root@pts/
root       294  0.2  0.3   2688  1724 pts/0    Ss   01:32   0:00 -sh
root       298  0.0  0.0   2724   348 pts/0    R+   01:33   0:00 ps aux

```

```
# ./exercice01 
signal 15 received, ignoring
signal 15 received, ignoring
signal 2 received, ignoring
signal 2 received, ignoring
Hello World
[parent] received: Hello World
asd
[parent] received: asd
asd
[parent] received: asd
as
[parent] received: as
d
[parent] received: d
exit
[parent] received: exit
# 

```

to send a signal to a process : 

```
# ps aux | grep exer
root       352  0.0  0.0   1736   200 ttyS0    S+   01:43   0:00 ./exercice01
root       353  0.0  0.0   1736    84 ttyS0    S+   01:43   0:00 ./exercice01
root       355  0.0  0.0   2688   320 pts/0    S+   01:43   0:00 grep exer
# kill -SIGTERM 352
# kill -SIGTERM 353
# kill -SIGINT 353
# kill -SIGINT 352
```
