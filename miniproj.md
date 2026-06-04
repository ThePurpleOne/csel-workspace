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
