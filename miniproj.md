# Module 

```
# ls -la /sys/class/fanctl/fanctl/
total 0
drwxr-xr-x 3 root root    0 Jan  1 00:04 .
drwxr-xr-x 3 root root    0 Jan  1 00:04 ..
-rw-r--r-- 1 root root 4096 Jan  1 00:06 frequency
-rw-r--r-- 1 root root 4096 Jan  1 00:06 mode
drwxr-xr-x 2 root root    0 Jan  1 00:06 power
lrwxrwxrwx 1 root root    0 Jan  1 00:06 subsystem -> ../../../../class/fanctl
-r--r--r-- 1 root root 4096 Jan  1 00:06 temperature
-rw-r--r-- 1 root root 4096 Jan  1 00:04 uevent
# cd /sys/class/fanctl/fanctl/ 
# cat temperature 
33870
# cat frequency
2
# cat mode 
auto
# cat mode
auto
# echo "manual" > mode
# echo "20" > frequency
# echo "1" > frequency
# echo "100" > frequency
sh: write error: Invalid argument
```

# DAEMON

```
# cat /var/log/messages 
Jan  1 00:13:20 csel daemon.info fanctl-daemon[306]: starting
Jan  1 00:13:20 csel daemon.info fanctl-daemon[306]: listening on TCP port 1337
Jan  1 00:13:24 csel daemon.info fanctl-daemon[306]: K3: mode toggled
Jan  1 00:13:26 csel daemon.info fanctl-daemon[306]: K2: freq=9 (manual)
Jan  1 00:13:27 csel daemon.info fanctl-daemon[306]: K2: freq=8 (manual)
Jan  1 00:13:28 csel daemon.info fanctl-daemon[306]: K2: freq=7 (manual)
Jan  1 00:13:30 csel daemon.info fanctl-daemon[306]: K1: freq=8 (manual)
Jan  1 00:13:30 csel daemon.info fanctl-daemon[306]: K1: freq=9 (manual)
Jan  1 00:13:31 csel daemon.info fanctl-daemon[306]: K1: freq=10 (manual)

```

# CLI
```
# ./fanctl-cli STATUS
mode=auto freq=10 temp=42445
# ./fanctl-cli MODE MANUAL
OK
# ./fanctl-cli FREQ 1
OK
# ./fanctl-cli FREQ 100
ERR freq out of range
# ./fanctl-cli FREQ 20
OK
# ./fanctl-cli STATUS
mode=manual freq=20 temp=42802
```