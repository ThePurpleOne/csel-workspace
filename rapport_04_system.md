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


# Exercice 2

First create a file that allocates and zeroes the memory : [file](src/04_system/exercice02/exercice02.c)

test it without CGroups :

```
# ./exercice02 
block 01 allocated and zeroed (1 MiB total)
block 02 allocated and zeroed (2 MiB total)
...
block 49 allocated and zeroed (49 MiB total)
block 50 allocated and zeroed (50 MiB total)
all 50 blocks allocated successfully
# 

```

Everything is allocated correctly. Now let's try with cgroups : 

```
# mount -t tmpfs none /sys/fs/cgroup
# mkdir /sys/fs/cgroup/memory
# mount -t cgroup -o memory memory /sys/fs/cgroup/memory
# mkdir /sys/fs/cgroup/memory/mem
# echo $$ > /sys/fs/cgroup/memory/mem/tasks
# echo 20M > /sys/fs/cgroup/memory/mem/memory.limit_in_bytes
# ./exercice02 
block 01 allocated and zeroed (1 MiB total)
block 02 allocated and zeroed (2 MiB total)
block 03 allocated and zeroed (3 MiB total)
block 04 allocated and zeroed (4 MiB total)
block 05 allocated and zeroed (5 MiB total)
block 06 allocated and zeroed (6 MiB total)
block 07 allocated and zeroed (7 MiB total)
block 08 allocated and zeroed (8 MiB total)
block 09 allocated and zeroed (9 MiB total)
block 10 allocated and zeroed (10 MiB total)
block 11 allocated and zeroed (11 MiB total)
block 12 allocated and zeroed (12 MiB total)
block 13 allocated and zeroed (13 MiB total)
block 14 allocated and zeroed (14 MiB total)
block 15 allocated and zeroed (15 MiB total)
block 16 allocated and zeroed (16 MiB total)
block 17 allocated and zeroed (17 MiB total)
block 18 allocated and zeroed (18 MiB total)
block 19 allocated and zeroed (19 MiB total)
Killed
# echo $?
137
```

we can see in the kernel logs that the process got killed by the oom-killer :

```
[ 1638.817242] exercice02 invoked oom-killer: gfp_mask=0xcc0(GFP_KERNEL), order=0, oom_score_adj=0
[ 1638.826030] CPU: 3 PID: 279 Comm: exercice02 Not tainted 5.15.148 #1
[ 1638.832385] Hardware name: FriendlyARM NanoPi NEO Plus2 (DT)
[ 1638.838039] Call trace:
[ 1638.840482]  dump_backtrace+0x0/0x1b0
[ 1638.844165]  show_stack+0x18/0x40
[ 1638.847483]  dump_stack_lvl+0x68/0x84
[ 1638.851152]  dump_stack+0x18/0x34
[ 1638.854469]  dump_header+0x48/0x1f4
[ 1638.857959]  oom_kill_process+0x224/0x230
[ 1638.861977]  out_of_memory+0x1ec/0x550
[ 1638.865728]  mem_cgroup_out_of_memory+0x138/0x150
[ 1638.870437]  try_charge_memcg+0x580/0x630
[ 1638.874450]  charge_memcg+0x54/0xc0
[ 1638.877942]  __mem_cgroup_charge+0x38/0x80
[ 1638.882036]  __handle_mm_fault+0x5bc/0xfe0
[ 1638.886136]  handle_mm_fault+0xe8/0x280
[ 1638.889972]  do_page_fault+0x150/0x3c0
[ 1638.893725]  do_translation_fault+0x7c/0xd0
[ 1638.897909]  do_mem_abort+0x40/0xb0
[ 1638.901398]  el0_da+0x24/0x60
[ 1638.904372]  el0t_64_sync_handler+0xe8/0x130
[ 1638.908641]  el0t_64_sync+0x1a0/0x1a4
[ 1638.912358] memory: usage 20480kB, limit 20480kB, failcnt 49
[ 1638.918032] memory+swap: usage 20480kB, limit 9007199254740988kB, failcnt 0
[ 1638.925024] kmem: usage 120kB, limit 9007199254740988kB, failcnt 0
[ 1638.931215] Memory cgroup stats for /mem:
[ 1638.931290] anon 20815872
[ 1638.931290] file 12288
[ 1638.931290] kernel_stack 16384
[ 1638.931290] pagetables 73728
[ 1638.931290] percpu 0
[ 1638.931290] sock 0
[ 1638.931290] shmem 0
[ 1638.931290] file_mapped 4096
[ 1638.931290] file_dirty 0
[ 1638.931290] file_writeback 0
[ 1638.931290] swapcached 0
[ 1638.931290] anon_thp 0
[ 1638.931290] file_thp 0
[ 1638.931290] shmem_thp 0
[ 1638.931290] inactive_anon 20795392
[ 1638.931290] active_anon 4096
[ 1638.931290] inactive_file 4096
[ 1638.931290] active_file 0
[ 1638.931290] unevictable 0
[ 1638.931290] slab_reclaimable 2816
[ 1638.931290] slab_unreclaimable 14392
[ 1638.931290] slab 17208
[ 1638.931290] workingset_refault_anon 0
[ 1638.931290] workingset_refault_file 0
[ 1638.931290] workingset_activate_anon 0
[ 1638.931290] workingset_activate_file 0
[ 1638.931290] workingset_restore_anon 0
[ 1638.931290] workingset_restore_file 0
[ 1639.024566] Tasks state (memory values in pages):
[ 1639.029300] [  pid  ]   uid  tgid total_vm      rss pgtables_bytes swapents oom_score_adj name
[ 1639.037938] [    255]     0   255      672      437    45056        0             0 sh
[ 1639.045872] [    279]     0   279     5606     5318    81920        0             0 exercice02
[ 1639.054494] oom-kill:constraint=CONSTRAINT_MEMCG,nodemask=(null),cpuset=/,mems_allowed=0,oom_memcg=/mem,task_memcg=/mem,task=exercice02,pid=279,uid=0
[ 1639.067940] Memory cgroup out of memory: Killed process 279 (exercice02) total-vm:22424kB, anon-rss:20304kB, file-rss:968kB, shmem-rss:0kB, UID:0 pgtables:80kB oom_score_adj:0
```

## Quel effet a la commande echo $$ > ... sur les cgroups ?

echo $$ print le PID du process courant. Dans ce cas la nous ajoutons le pid de notre process au cgroup.

## Quel est le comportement du sous-système memory lorsque le quota de mémoire est épuisé ? Pourrait-on le modifier ? Si oui, comment ?

Par défaut, l'OOM killer est déclenché et tue le processus. 

TODO, flemme de faire

## Est-il possible de surveiller/vérifier l’état actuel de la mémoire ? Si oui, comment ?

oui via le sysfs il est possible de voir l'état actuel de la mémoire : 

```
# cat /sys/fs/cgroup/memory/mem/memory.usage_in_bytes
1769472
# cat /sys/fs/cgroup/memory/mem/memory.stat
cache 1208320
rss 372736
rss_huge 0
shmem 0
mapped_file 589824
dirty 0
writeback 0
swap 0
pgpgin 6784
pgpgout 6398
pgfault 6222
pgmajfault 12
inactive_anon 311296
active_anon 4096
inactive_file 1196032
active_file 8192
unevictable 0
hierarchical_memory_limit 20971520     <- LIMITE DE 20M
hierarchical_memsw_limit 9223372036854771712
total_cache 1208320
total_rss 372736
total_rss_huge 0
total_shmem 0
total_mapped_file 589824
total_dirty 0
total_writeback 0
total_swap 0
total_pgpgin 6784
total_pgpgout 6398
total_pgfault 6222
total_pgmajfault 12
total_inactive_anon 311296
total_active_anon 4096
total_inactive_file 1196032
total_active_file 8192
total_unevictable 0
```


# Exercice 3

Programme composé d'un parent qui fork 2 workers, chacun faisant une boucle infinie pour saturer son coeur : [file](src/04_system/exercice03/exercice03.c)

```
top - 00:56:16 up 56 min,  2 users,  load average: 0.68, 0.16, 0.05
Tasks: 102 total,   3 running,  99 sleeping,   0 stopped,   0 zombie
%Cpu0  :  10.5/10.5   21[||||             ]     %Cpu1  : 100.0/0.0   100[|||||||||||||||||]
%Cpu2  :   0.0/0.0     0[                 ]     %Cpu3  : 100.0/0.0   100[|||||||||||||||||]
GiB Mem :  9.0/0.5      [                 ]
GiB Swap:  0.0/0.0      [                 ]

  PID USER      PR  NI    VIRT    RES  %CPU  %MEM     TIME+ S COMMAND                         
    1 root      20   0    2.6m   0.3m   0.0   0.1   0:01.79 S init                            
  245 root      20   0    6.0m   2.7m   0.0   0.6   0:00.00 S  `- sshd: /usr/sbin/sshd [list+ 
  253 root      20   0    6.4m   4.9m   0.0   1.0   0:00.25 S      `- sshd: root@pts/0        
  255 root      20   0    2.6m   1.7m   0.0   0.4   0:00.13 S          `- -sh                 
  308 root      20   0    1.7m   0.2m   0.0   0.0   0:00.00 S              `- ./exercice03    
  309 root      20   0    1.8m   0.1m  88.9   0.0   0:25.95 R                  `- ./exercice03 
  310 root      20   0    1.8m   0.1m  88.9   0.0   0:25.96 R                  `- ./exercice03 
  311 root      20   0    6.4m   4.9m   0.0   1.0   0:00.08 S      `- sshd: root@pts/1        
  313 root      20   0    2.6m   1.7m   0.0   0.4   0:00.01 S          `- -sh                 
  318 root      20   0    3.1m   1.9m  11.1   0.4   0:00.04 R              `- top             
```

cpu1 et cpu3 sont à 100%. Si on relance le process, les 2 cpus utilisés changent, c'est pas forcement le 1 et le 3

## Les 4 dernières lignes sont-elles obligatoires ?

Oui. Le sous-système `cpuset` exige que `cpuset.cpus` et `cpuset.mems` soient explicitement définis avant de pouvoir ajouter un processus à un cgroup. Sans ces deux attributs, le cgroup n'a pas de CPU ni de noeud mémoire configuré et toute écriture dans `tasks` échoue avec une erreur.

## Comportement attendu avec high/low

Le processus placé dans `high` est épinglé sur CPU3, celui dans `low` sur CPU2. Chaque process doit consommer 100% de son coeur assigné sans interférer avec l'autre. On peut le vérifier avec `top` : CPU2 et CPU3 doivent tous les deux afficher 100%.

```
top - 02:01:16 up  2:01,  3 users,  load average: 1.52, 0.48, 0.61
Tasks: 108 total,   5 running, 103 sleeping,   0 stopped,   0 zombie
%Cpu0  :   0.0/0.0     0[                                                                 ]     %Cpu1  :   0.6/1.3     2[|                                                                ]
%Cpu2  : 100.0/0.0   100[|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||]     %Cpu3  : 100.0/0.0   100[|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||]
GiB Mem :  9.5/0.5      [                                                                 ]
GiB Swap:  0.0/0.0      [                                                                 ]

  PID USER      PR  NI    VIRT    RES  %CPU  %MEM     TIME+ S COMMAND
    1 root      20   0    2.6m   0.3m   0.0   0.1   0:01.80 S init
  159 root      20   0    2.6m   0.3m   0.0   0.1   0:00.02 S  `- /sbin/syslogd -n
  163 root      20   0    2.6m   0.3m   0.0   0.1   0:00.02 S  `- /sbin/klogd -n
  175 root      20   0    3.0m   2.1m   0.0   0.4   0:00.14 S  `- /sbin/mdev -df
  245 root      20   0    6.0m   2.7m   0.0   0.6   0:00.02 S  `- sshd: /usr/sbin/sshd [listener] 0 of 10-100 startups
  253 root      20   0    6.4m   4.9m   0.0   1.0   0:00.39 S      `- sshd: root@pts/0
  255 root      20   0    2.6m   1.8m   0.0   0.4   0:00.23 S          `- -sh
  431 root      20   0    1.7m   0.2m   0.0   0.0   0:00.00 S              `- ./exercice03
  432 root      20   0    1.8m   0.1m  49.7   0.0   0:14.70 R                  `- ./exercice03
  433 root      20   0    1.8m   0.1m  50.3   0.0   0:14.71 R                  `- ./exercice03
  375 root      20   0    6.4m   4.9m   0.0   1.0   0:00.33 S      `- sshd: root@pts/1
  377 root      20   0    2.6m   1.7m   0.0   0.4   0:00.01 S          `- -sh
  379 root      20   0    3.1m   2.0m   1.3   0.4   0:16.72 R              `- top
  426 root      20   0    6.4m   4.8m   0.0   1.0   0:00.09 S      `- sshd: root@pts/2
  428 root      20   0    2.6m   1.7m   0.0   0.4   0:00.02 S          `- -sh
  434 root      20   0    1.7m   0.2m   0.0   0.0   0:00.00 S              `- ./exercice03
  435 root      20   0    1.8m   0.1m  49.7   0.0   0:12.69 R                  `- ./exercice03
  436 root      20   0    1.8m   0.1m  49.7   0.0   0:12.68 R                  `- ./exercice03
  252 root      20   0    2.6m   0.3m   0.0   0.1   0:00.00 S  `- /sbin/getty -L console 0 vt100
    2 root      20   0    0.0m   0.0m   0.0   0.0   0:00.01 S [kthreadd]
    3 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [rcu_gp]
    4 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [rcu_par_gp]
    5 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [slub_flushwq]
    6 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [netns]
    8 root       0 -20    0.0m   0.0m   0.0   0.0   0:00.00 I  `- [kworker/0:0H-events_highpri]
```

## 75% / 25% avec cpu.shares sur le coeur 4 (CPU3)

`cpu.shares` est une valeur relative : le ratio entre les deux cgroups détermine la répartition. Pour 75%/25% on utilise un ratio 3:1, soit 768 et 256.

```sh
mkdir /sys/fs/cgroup/cpuset/task_high
mkdir /sys/fs/cgroup/cpuset/task_low

echo 3 > /sys/fs/cgroup/cpuset/task_high/cpuset.cpus
echo 0 > /sys/fs/cgroup/cpuset/task_high/cpuset.mems
echo 768 > /sys/fs/cgroup/cpuset/task_high/cpu.shares

echo 3 > /sys/fs/cgroup/cpuset/task_low/cpuset.cpus
echo 0 > /sys/fs/cgroup/cpuset/task_low/cpuset.mems
echo 256 > /sys/fs/cgroup/cpuset/task_low/cpu.shares

echo <pid_75> > /sys/fs/cgroup/cpuset/task_high/tasks
echo <pid_25> > /sys/fs/cgroup/cpuset/task_low/tasks
```

Les deux tâches tournent sur CPU3 ; le scheduler Linux distribue le temps selon le ratio 768:256 = 3:1.

