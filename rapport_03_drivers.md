# Pilotes orientés mémoire

## Exercice 01

on host compile and copy the app inside the nanopi

```sh
root ➜ /workspace/src/03_drivers/exercice01 (main) $ make
mkdir -p .obj/nano
/buildroot/output/host/usr/bin/aarch64-linux-gcc -Wall -Wextra -g -c -O0 -MD -std=gnu11 -mcpu=cortex-a53 -funwind-tables main.c -o .obj/nano/main.o
/buildroot/output/host/usr/bin/aarch64-linux-gcc .obj/nano/main.o  -o app
root ➜ /workspace/src/03_drivers/exercice01 (main) $ cp app /rootfs/root/.
```

on nanopi run the app

```sh
# ./app 
psz=1000, addr=1c14200, offset=1c14000, ofs=200
NanoPi NEO Plus2 chipid=82800001'94004704'5036c304'301e0b4e
```

# Pilotes orientés caractère

## Exercice 2

compile and copy the module inside the nanopi

```sh
root ➜ /workspace/src/03_drivers/exercice02 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice02 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/03_drivers/exercice02/skeleton.o
  LD [M]  /workspace/src/03_drivers/exercice02/mymodule.o
  MODPOST /workspace/src/03_drivers/exercice02/Module.symvers
  CC [M]  /workspace/src/03_drivers/exercice02/mymodule.mod.o
  LD [M]  /workspace/src/03_drivers/exercice02/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice02 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on nanopi insert the module and test a write and read operation

```
# modprobe mymodule
[  538.437397] Linux module skeleton loaded

# cat /proc/devices 
Character devices:
...
511 mymodule

# mknod /dev/mymodule c 511 0
# ls /dev/mymodule -la
crw-r--r-- 1 root root 511, 0 Jan  1 00:22 /dev/mymodule

# echo "Hello World" > /dev/mymodule 
# cat /dev/mymodule 
Hello World
# rm /dev/mymodule 
# rmmod mymodule
[ 1457.445180] Linux module skeleton unloaded
```

## Exercice 3

compile and copy the module inside the nanopi

```sh
root ➜ /workspace/src/03_drivers/exercice03 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice03 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/03_drivers/exercice03/skeleton.o
  LD [M]  /workspace/src/03_drivers/exercice03/mymodule.o
  MODPOST /workspace/src/03_drivers/exercice03/Module.symvers
  CC [M]  /workspace/src/03_drivers/exercice03/mymodule.mod.o
  LD [M]  /workspace/src/03_drivers/exercice03/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice03 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on nanopi insert the module and test a write and read operation

```
# modprobe mymodule
[ 1636.415561] Linux module skeleton loaded
[ 1636.419606] The number of instances: 3


# cat /proc/devices 
Character devices:
...
511 mymodule

# mknod /dev/mymodule.0 c 511 0
# mknod /dev/mymodule.1 c 511 1
# mknod /dev/mymodule.2 c 511 2
# ls -la /dev/mymodule*
crw-r--r-- 1 root root 511, 0 Jan  1 00:28 /dev/mymodule.0
crw-r--r-- 1 root root 511, 1 Jan  1 00:28 /dev/mymodule.1
crw-r--r-- 1 root root 511, 2 Jan  1 00:28 /dev/mymodule.2

# echo "Hello World module 0" > /dev/mymodule.0
# echo "Hello World module 1" > /dev/mymodule.1
# echo "Hello World module 2" > /dev/mymodule.2
# 
# cat /dev/mymodule.0
Hello World module 0
# cat /dev/mymodule.1
Hello World module 1
# cat /dev/mymodule.2
Hello World module 2

# rmmod mymodule
```

dmesg output : 

```
[ 1761.550934] skeleton : open operation... major:511, minor:0
[ 1761.556596] skeleton : opened for reading & writing...
[ 1761.561750] skeleton: private_data=00000000023ee00b
[ 1761.566796] skeleton: read operation... read=10000
[ 1761.571876] skeleton: read operation... read=0
[ 1761.576450] skeleton: release operation...
[ 1773.957837] skeleton : open operation... major:511, minor:0
[ 1773.963544] skeleton : opened for reading & writing...
[ 1773.968705] skeleton: private_data=00000000023ee00b
[ 1773.973638] skeleton: write operation... private_data=00000000023ee00b, written=21
[ 1773.981240] skeleton: release operation...
[ 1776.757463] skeleton : open operation... major:511, minor:1
[ 1776.763187] skeleton : opened for reading & writing...
[ 1776.768341] skeleton: private_data=00000000c3ef85c2
[ 1776.773269] skeleton: write operation... private_data=00000000c3ef85c2, written=21
[ 1776.780870] skeleton: release operation...
[ 1779.618836] skeleton : open operation... major:511, minor:2
[ 1779.624521] skeleton : opened for reading & writing...
[ 1779.629675] skeleton: private_data=00000000b894265b
[ 1779.634602] skeleton: write operation... private_data=00000000b894265b, written=21
[ 1779.642200] skeleton: release operation...
[ 1785.001902] skeleton : open operation... major:511, minor:0
[ 1785.007572] skeleton : opened for reading & writing...
[ 1785.012725] skeleton: private_data=00000000023ee00b
[ 1785.017770] skeleton: read operation... read=10000
[ 1785.022857] skeleton: read operation... read=0
[ 1785.027421] skeleton: release operation...
[ 1788.131574] skeleton : open operation... major:511, minor:1
[ 1788.137220] skeleton : opened for reading & writing...
[ 1788.142372] skeleton: private_data=00000000c3ef85c2
[ 1788.147418] skeleton: read operation... read=10000
[ 1788.152504] skeleton: read operation... read=0
[ 1788.157073] skeleton: release operation...
[ 1789.005626] skeleton : open operation... major:511, minor:2
[ 1789.011286] skeleton : opened for reading & writing...
[ 1789.016445] skeleton: private_data=00000000b894265b
[ 1789.021477] skeleton: read operation... read=10000
[ 1789.026565] skeleton: read operation... read=0
[ 1789.031117] skeleton: release operation...
[ 1813.602245] Linux module skeleton unloaded
```

## Exercice 4

```sh
root ➜ /workspace/src/03_drivers/exercice04 (main) $ make
mkdir -p .obj
/buildroot/output/host/usr/bin/aarch64-linux-gcc -Wall -Wextra -g -c -O0 -MD -std=gnu11 -mcpu=cortex-a53 -funwind-tables main.c -o .obj/main.o
/buildroot/output/host/usr/bin/aarch64-linux-gcc .obj/main.o  -o app
root ➜ /workspace/src/03_drivers/exercice04 (main) $ cp app /rootfs/root/app 
```

test the app on the nanopi

```
# ./app /dev/mymodule 
/dev/mymodule
bonjour le monde
ce mois d'octobre est plutot humide...
ce n'est qu'un petit texte de test...

et voici un complement au premier text..
ce n'est qu'un deuxieme petit texte de test...
blabla blabla blabla blabla blabla blabla blabla
...
blabla blabla blabla blabla blabla blabla blabla
blabla blabla blabla blabla blabla blabla blabla
blabla blabla blabla blabla blabla blabla bl# 
# 
# 
# dmesg
[ 2018.712577] skeleton : open operation... major:511, minor:0
[ 2018.718218] skeleton : opened for reading & writing...
[ 2018.723401] skeleton: private_data=00000000a502ec24
[ 2018.728340] skeleton: write operation... private_data=00000000a502ec24, written=13
[ 2018.735928] skeleton: write operation... private_data=00000000a502ec24, written=95
[ 2018.743503] skeleton: write operation... private_data=00000000a502ec24, written=89
[ 2018.751082] skeleton: write operation... private_data=00000000a502ec24, written=49
[ 2020.250938] skeleton: write operation... private_data=00000000a502ec24, written=49
[ 2020.258509] skeleton: write operation... private_data=00000000a502ec24, written=49
[ 2020.266196] ------------[ cut here ]------------
[ 2020.270808] WARNING: CPU: 3 PID: 332 at include/linux/thread_info.h:230 skeleton_write+0x44/0x150 [mymodule]
[ 2020.280647] Modules linked in: mymodule(O) ipv6 brcmfmac brcmutil cfg80211 rfkill sunxi_wdt sun8i_mixer sun4i_tcon sun8i_tcon_top drm_kms_helper lima gpu_sched drm sun6i_dma sun8i_ce crypto_engine crct10dif_ce [last unloaded: mymodule]
[ 2020.301556] CPU: 3 PID: 332 Comm: app Tainted: G           O      5.15.148 #1
[ 2020.308688] Hardware name: FriendlyARM NanoPi NEO Plus2 (DT)
[ 2020.314341] pstate: 80000005 (Nzcv daif -PAN -UAO -TCO -DIT -SSBS BTYPE=--)
[ 2020.321298] pc : skeleton_write+0x44/0x150 [mymodule]
[ 2020.326352] lr : vfs_write+0xf0/0x290
[ 2020.330025] sp : ffff80000a533d40
[ 2020.333334] x29: ffff80000a533d40 x28: ffff000002775400 x27: 0000000000000000
[ 2020.340472] x26: 0000000000000000 x25: 0000000000000000 x24: 0000000000000000
[ 2020.347608] x23: 0000000000000000 x22: ffff80000a533df0 x21: 0000aaaab3aafb98
[ 2020.354745] x20: ffff000003c03300 x19: 0000000000000031 x18: 0000000000000000
[ 2020.361882] x17: 0000000000000000 x16: 0000000000000000 x15: 0000000000000000
[ 2020.369017] x14: 0000000000000000 x13: 0000000000000000 x12: 0000000000000000
[ 2020.376154] x11: 0000000000000000 x10: 0000000000000000 x9 : 0000000000000000
[ 2020.383289] x8 : 0000000000000000 x7 : 0000000000000000 x6 : 0000000000000000
[ 2020.390424] x5 : 0000000000000003 x4 : ffff000003e7e70d x3 : ffff80000a533df0
[ 2020.397560] x2 : 0000000000002708 x1 : 0000aaaab3aafb98 x0 : ffff000003c03300
[ 2020.404698] Call trace:
[ 2020.407143]  skeleton_write+0x44/0x150 [mymodule]
[ 2020.411850]  vfs_write+0xf0/0x290
[ 2020.415168]  ksys_write+0x68/0x100
[ 2020.418573]  __arm64_sys_write+0x1c/0x30
[ 2020.422499]  invoke_syscall+0x48/0x120
[ 2020.426253]  el0_svc_common.constprop.0+0x44/0x100
[ 2020.431045]  do_el0_svc+0x28/0xa0
[ 2020.434361]  el0_svc+0x28/0x80
[ 2020.437424]  el0t_64_sync_handler+0xa4/0x130
[ 2020.441692]  el0t_64_sync+0x1a0/0x1a4
[ 2020.445356] ---[ end trace dfd906bdd2d7ba28 ]---
[ 2020.449996] skeleton: write operation... private_data=00000000a502ec24, written=-14
[ 2020.457669] skeleton: release operation...
[ 2020.463055] skeleton : open operation... major:511, minor:0
[ 2020.468650] skeleton : opened for reading & writing...
[ 2020.473799] skeleton: private_data=00000000a502ec24
[ 2020.478695] skeleton: read operation... read=99
...
[ 2020.934847] skeleton: read operation... read=99
[ 2020.939402] skeleton: read operation... read=1
[ 2020.943865] skeleton: read operation... read=0
[ 2020.948326] skeleton: release operation...
```

we can see an error in the driver, this is caused by the `if (count >= remaining) count = -EIO;` line once the buffer is full. Since the count is `size_t` (unsigned), assigning `-EIO` will write a big value to count, and copy_from_user will create a warning.

# Sysfs

## Exercice 5

compile and install the module 

```sh
root ➜ /workspace/src/03_drivers/exercice05 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice05 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/03_drivers/exercice05/skeleton.o
  LD [M]  /workspace/src/03_drivers/exercice05/mymodule.o
  MODPOST /workspace/src/03_drivers/exercice05/Module.symvers
  CC [M]  /workspace/src/03_drivers/exercice05/mymodule.mod.o
  LD [M]  /workspace/src/03_drivers/exercice05/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice05 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

```sh
modprobe mymodule
dmesg
[ 2892.384456] Linux module skeleton loaded
ls /sys/class/my_sysfs_class/ -la
total 0
drwxr-xr-x  2 root root 0 Jan  1 00:48 .
drwxr-xr-x 72 root root 0 Jan  1 00:00 ..
lrwxrwxrwx  1 root root 0 Jan  1 00:48 my_sysfs_device -> ../../devices/virtual/my_sysfs_class/my_sysfs_device

cd /sys/class/my_sysfs_class/my_sysfs_device/
ls
cfg  power  subsystem  uevent  val
cat cfg 
0 0  
cat val 
0
echo "10 120 name description" > cfg 
cat cfg 
10 120 name description
echo "123123" > val 
cat val 
123123
```

## Exercice 5.1

build and install the module 

```sh
root ➜ /workspace/src/03_drivers/exercice05.1 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice05.1 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/03_drivers/exercice05.1/skeleton.o
  LD [M]  /workspace/src/03_drivers/exercice05.1/mymodule.o
  MODPOST /workspace/src/03_drivers/exercice05.1/Module.symvers
  CC [M]  /workspace/src/03_drivers/exercice05.1/mymodule.mod.o
  LD [M]  /workspace/src/03_drivers/exercice05.1/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice05.1 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
Warning: 'make modules_install' requires depmod. Please install it.
This is probably in the kmod package.
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

test the driver

```sh
# modprobe mymodule
[ 3350.032241] Linux module skeleton loaded

# echo "10 10 name description" > /sys/devices/platform/my_platform_module/cfg 
# cat /sys/devices/platform/my_platform_module/cfg 
10 10 name description
# echo "10123" > /sys/devices/platform/my_platform_module/val 
# cat /sys/devices/platform/my_platform_module/val 
10123

# ./app /dev/my_platform_module 
/dev/my_platform_module
bonjour le monde
ce mois d'octobre est plutot humide...
ce n'est qu'un petit texte de test...

et voici un complement au premier text..
ce n'est qu'un deuxieme petit texte de test...
blabla blabla blabla blabla blabla blabla blabla
blabla blabla blabla blabla blabla blabla blabla
blabla blabla blabla blabla blabla blabla blabla
...
```


dmesg output : 

```
[ 3350.032241] Linux module skeleton loaded
[ 3727.273478] skeleton : open operation... major:511, minor:0
[ 3727.279121] skeleton : opened for reading & writing...
[ 3727.284324] skeleton: write operation... written=23
[ 3727.289266] skeleton: write operation... written=95
[ 3727.294195] skeleton: write operation... written=89
[ 3727.299113] skeleton: write operation... written=49
```

# Opérations bloquantes

## Exercice 7

build the app and the driver, and install them 

```sh
root ➜ /workspace/src/03_drivers/exercice07 (main) $ make
for dir in app drv; do make all -C $dir; done
make[1]: Entering directory '/workspace/src/03_drivers/exercice07/app'
mkdir -p .obj
/buildroot/output/host/usr/bin/aarch64-linux-gcc -Wall -Wextra -g -c -O0 -MD -std=gnu11 -mcpu=cortex-a53 -funwind-tables app.c -o .obj/app.o
/buildroot/output/host/usr/bin/aarch64-linux-gcc .obj/app.o  -o app
make[1]: Leaving directory '/workspace/src/03_drivers/exercice07/app'
make[1]: Entering directory '/workspace/src/03_drivers/exercice07/drv'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice07/drv ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[2]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/03_drivers/exercice07/drv/skeleton.o
  LD [M]  /workspace/src/03_drivers/exercice07/drv/mymodule.o
  MODPOST /workspace/src/03_drivers/exercice07/drv/Module.symvers
  CC [M]  /workspace/src/03_drivers/exercice07/drv/mymodule.mod.o
  LD [M]  /workspace/src/03_drivers/exercice07/drv/mymodule.ko
make[2]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make[1]: Leaving directory '/workspace/src/03_drivers/exercice07/drv'

root ➜ /workspace/src/03_drivers/exercice07 (main) $ make install
for dir in app drv; do make install -C $dir; done
make[1]: Entering directory '/workspace/src/03_drivers/exercice07/app'
make[1]: *** No rule to make target 'install'.  Stop.
make[1]: Leaving directory '/workspace/src/03_drivers/exercice07/app'
make[1]: Entering directory '/workspace/src/03_drivers/exercice07/drv'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/03_drivers/exercice07/drv INSTALL_MOD_PATH=/rootfs modules_install
make[2]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[2]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make[1]: Leaving directory '/workspace/src/03_drivers/exercice07/drv'
root ➜ /workspace/src/03_drivers/exercice07 (main) $ cp app/app /rootfs/root/app
```

test the app inside the nanopi. First test the interrupts without the app (dmesg), then with the app.

```
# modprobe mymodule
# dmesg
[ 4841.254104] Linux module skeleton loaded(status=0)
[ 4874.427784] interrupt gpio_a.0-k1 raised...
[ 4874.730157] interrupt gpio_a.2-k2 raised...
[ 4875.159274] interrupt gpio_a.3-k3 raised...
[ 4875.286385] interrupt gpio_a.3-k3 raised...

# ./app /dev/mymodule 
Waiting for interrupts on /dev/mymodule...
Press K1, K2, or K3

Interrupt detected! Total count: 1
Interrupt detected! Total count: 2
Interrupt detected! Total count: 3
Interrupt detected! Total count: 4
Interrupt detected! Total count: 5

```

