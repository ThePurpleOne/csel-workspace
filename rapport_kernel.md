## Exercice 1

## 1

Compile the module by running the following commands on the host: 

```sh
root ➜ /workspace (main) $ cd src/02_modules/exercice01/
root ➜ /workspace/src/02_modules/exercice01 (main) $ make
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice01 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/02_modules/exercice01/skeleton.o
  LD [M]  /workspace/src/02_modules/exercice01/mymodule.o
  MODPOST /workspace/src/02_modules/exercice01/Module.symvers
  CC [M]  /workspace/src/02_modules/exercice01/mymodule.mod.o
  LD [M]  /workspace/src/02_modules/exercice01/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

we can then copy the module inside the rootfs and insert it. 

on host:

```sh
root ➜ /workspace/src/02_modules/exercice01 (main) $ cp mymodule.ko /tmp/.
```

## 2

We can inspect the module with `modinfo`: 

```sh
modinfo mymodule.ko 
filename:       /root/mymodule.ko
license:        GPL
description:    Module skeleton
author:         Daniel Gachet <daniel.gachet@hefr.ch>
depends:        
name:           mymodule
vermagic:       5.15.148 SMP preempt mod_unload aarch64
parm:           text:charp
parm:           elements:int
```

## 3

Insert the module and check the logs : 

```sh
insmod mymodule.ko 
[ 1099.479572] Linux module 01 skeleton loaded
dmesg
[ 1099.479572] Linux module 01 skeleton loaded
[ 1099.483810]   text: dummy text
[ 1099.483810]   elements: 1
```

## 4

Check the difference between `lsmod` and `/proc/modules`

```sh
lsmod

Module                  Size  Used by
mymodule               16384  0
ipv6                  462848  18
brcmfmac              253952  0
brcmutil               20480  1 brcmfmac
cfg80211              393216  1 brcmfmac
rfkill                 36864  1 cfg80211
sunxi_wdt              20480  0
sun8i_mixer            40960  0
sun4i_tcon             36864  0
sun8i_tcon_top         16384  1 sun4i_tcon
drm_kms_helper        282624  2 sun8i_mixer,sun4i_tcon
lima                   61440  0
gpu_sched              36864  1 lima
drm                   585728  6 gpu_sched,sun8i_mixer,drm_kms_helper,lima,sun4i_tcon
sun6i_dma              28672  0
sun8i_ce               32768  0
crypto_engine          20480  1 sun8i_ce
crct10dif_ce           20480  1

cat /proc/modules

mymodule 16384 0 - Live 0xffff800001127000 (O)
ipv6 462848 18 [permanent], Live 0xffff80000117d000
brcmfmac 253952 0 - Live 0xffff80000113e000
brcmutil 20480 1 brcmfmac, Live 0xffff800001079000
cfg80211 393216 1 brcmfmac, Live 0xffff8000010c6000
rfkill 36864 1 cfg80211, Live 0xffff80000103d000
sunxi_wdt 20480 0 - Live 0xffff800001037000
sun8i_mixer 40960 0 - Live 0xffff80000102c000
sun4i_tcon 36864 0 - Live 0xffff80000104e000
sun8i_tcon_top 16384 1 sun4i_tcon, Live 0xffff800001049000
drm_kms_helper 282624 2 sun8i_mixer,sun4i_tcon, Live 0xffff800001080000
lima 61440 0 - Live 0xffff800001069000
gpu_sched 36864 1 lima, Live 0xffff80000105a000
drm 585728 6 sun8i_mixer,sun4i_tcon,drm_kms_helper,lima,gpu_sched, Live 0xffff800000f9c000
sun6i_dma 28672 0 - Live 0xffff800000f90000
sun8i_ce 32768 0 - Live 0xffff800000f83000
crypto_engine 20480 1 sun8i_ce, Live 0xffff800000f79000
crct10dif_ce 20480 1 - Live 0xffff800000f70000
```

In both cases we can see that our module is loaded, but with `cat /proc/modules`, we can see that it is an out-of-tree module with the `(O)` flag.

## 5

remove the module to see the unload print : 

```sh
rmmod mymodule.ko 
[ 1355.900389] Linux module skeleton unloaded
dmesg
[ 1099.479572] Linux module 01 skeleton loaded
[ 1099.483810]   text: dummy text
[ 1099.483810]   elements: 1
[ 1355.900389] Linux module skeleton unloaded
```

## 6 

To use modprobe, the module has to be in `/lib/modules/5.15.148`, so we need a make install target that will put the module in the correct directory. Then run make install on the host : 

```sh
root ➜ /workspace/src/02_modules/exercice01 (main) $ make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice01 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on the nanopi, we can insert the module with modprobe now : 

```sh
modprobe mymodule
[ 1724.768669] Linux module 01 skeleton loaded
dmesg
[ 1724.768669] Linux module 01 skeleton loaded
[ 1724.772944]   text: dummy text
[ 1724.772944]   elements: 1
rmmod mymodule
[ 1739.481962] Linux module skeleton unloaded
dmesg
[ 1724.768669] Linux module 01 skeleton loaded
[ 1724.772944]   text: dummy text
[ 1724.772944]   elements: 1
[ 1739.481962] Linux module skeleton unloaded
```

## Exercice 2 

Add a parameter to the module, and print it, then recompile and reinstall the module.

```c
static int  param = 67;
module_param(param, int, 0);

static int __init skeleton_init(void)
{
	pr_info ("Linux module 01 skeleton loaded\n");
	pr_debug ("  text: %s\n  elements: %d\n", text, elements);
	pr_debug ("  param: %d\n\n", param);
	return 0;
}
```

on nanopi: 

```sh
modprobe mymodule
[ 2220.739206] Linux module 01 skeleton loaded
rmmod mymodule
[ 2224.821402] Linux module skeleton unloaded
modprobe mymodule param=10
[ 2229.267323] Linux module 01 skeleton loaded
dmesg
[ 2220.739206] Linux module 01 skeleton loaded
[ 2220.743489]   text: dummy text
[ 2220.743489]   elements: 1
[ 2220.743499]   param: 67
[ 2220.743499] 
[ 2224.821402] Linux module skeleton unloaded
[ 2229.267323] Linux module 01 skeleton loaded
[ 2229.271617]   text: dummy text
[ 2229.271617]   elements: 1
[ 2229.271627]   param: 10
```

param works fine.

## Exercice 3

```sh
cat /proc/sys/kernel/printk
7	4	1	7
```

The result shows the `current`, `default`, `minimum` and `boot-time-default` log levels.

## Exercice 4

compile and install the module

```sh
root ➜ /workspace/src/02_modules/exercice04 (main) $ make
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice04 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/02_modules/exercice04/skeleton.o
  LD [M]  /workspace/src/02_modules/exercice04/mymodule.o
  MODPOST /workspace/src/02_modules/exercice04/Module.symvers
  CC [M]  /workspace/src/02_modules/exercice04/mymodule.mod.o
  LD [M]  /workspace/src/02_modules/exercice04/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
root ➜ /workspace/src/02_modules/exercice04 (main) $ make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice04 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on nanopi, insert the module and remove it with different parameters : 

```sh
modprobe mymodule && rmmod mymodule
[ 2843.695894] Linux module 04 skeleton loaded
[ 2843.700136]   text: dummy text
[ 2843.700136]   elements: 0
[ 2843.792596] All elements (0/0) of the list have been removed and deleted!
[ 2843.799449] Linux module skeleton unloaded
dmesg
[ 2843.695894] Linux module 04 skeleton loaded
[ 2843.700136]   text: dummy text
[ 2843.700136]   elements: 0
[ 2843.792596] All elements (0/0) of the list have been removed and deleted!
[ 2843.799449] Linux module skeleton unloaded


modprobe mymodule elements=10 text=helloworld && rmmod mymodule
[ 2884.847833] Linux module 04 skeleton loaded
[ 2884.852097]   text: helloworld
[ 2884.852097]   elements: 10
[ 2884.939920] ele [0/0/10] = helloworld
[ 2884.943654] ele [1/1/10] = helloworld
[ 2884.947317] ele [2/2/10] = helloworld
[ 2884.950995] ele [3/3/10] = helloworld
[ 2884.954666] ele [4/4/10] = helloworld
[ 2884.958339] ele [5/5/10] = helloworld
[ 2884.962009] ele [6/6/10] = helloworld
[ 2884.965682] ele [7/7/10] = helloworld
[ 2884.969353] ele [8/8/10] = helloworld
[ 2884.973025] ele [9/9/10] = helloworld
[ 2884.976698] All elements (10/10) of the list have been removed and deleted!
[ 2884.983663] Linux module skeleton unloaded
dmesg
[ 2843.695894] Linux module 04 skeleton loaded
[ 2843.700136]   text: dummy text
[ 2843.700136]   elements: 0
[ 2843.792596] All elements (0/0) of the list have been removed and deleted!
[ 2843.799449] Linux module skeleton unloaded
[ 2884.847833] Linux module 04 skeleton loaded
[ 2884.852097]   text: helloworld
[ 2884.852097]   elements: 10
[ 2884.939920] ele [0/0/10] = helloworld
[ 2884.943654] ele [1/1/10] = helloworld
[ 2884.947317] ele [2/2/10] = helloworld
[ 2884.950995] ele [3/3/10] = helloworld
[ 2884.954666] ele [4/4/10] = helloworld
[ 2884.958339] ele [5/5/10] = helloworld
[ 2884.962009] ele [6/6/10] = helloworld
[ 2884.965682] ele [7/7/10] = helloworld
[ 2884.969353] ele [8/8/10] = helloworld
[ 2884.973025] ele [9/9/10] = helloworld
[ 2884.976698] All elements (10/10) of the list have been removed and deleted!
[ 2884.983663] Linux module skeleton unloaded

```

## Exercice 5

compile and install the module:

```sh
root ➜ /workspace/src/02_modules/exercice05 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice05 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/02_modules/exercice05/skeleton.o
  LD [M]  /workspace/src/02_modules/exercice05/mymodule.o
  MODPOST /workspace/src/02_modules/exercice05/Module.symvers
  CC [M]  /workspace/src/02_modules/exercice05/mymodule.mod.o
  LD [M]  /workspace/src/02_modules/exercice05/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice05 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on nanopi, check the reserved memory first : 

```sh
cat /proc/iomem 

00018000-00033fff : 18000.sram sram@18000
01000000-0100ffff : 1000000.clock clock@1000000
01c00000-01c00fff : 1c00000.system-control system-control@1c00000
01c02000-01c02fff : 1c02000.dma-controller dma-controller@1c02000
01c0f000-01c0ffff : 1c0f000.mmc mmc@1c0f000
01c10000-01c10fff : 1c10000.mmc mmc@1c10000
01c11000-01c11fff : 1c11000.mmc mmc@1c11000
01c14000-01c143ff : 1c14000.eeprom eeprom@1c14000
01c15000-01c15fff : 1c15000.crypto crypto@1c15000
01c17000-01c17fff : 1c17000.mailbox mailbox@1c17000
01c19000-01c193ff : usb@1c19000
  01c19000-01c193ff : musb-hdrc.1.auto usb@1c19000
01c19400-01c1942b : 1c19400.phy phy_ctrl
01c1a000-01c1a0ff : 1c1a000.usb usb@1c1a000
01c1a400-01c1a4ff : 1c1a400.usb usb@1c1a400
01c1a800-01c1a803 : 1c19400.phy pmu0
01c1b800-01c1b803 : 1c19400.phy pmu1
01c1c800-01c1c803 : 1c19400.phy pmu2
01c1d000-01c1d0ff : 1c1d000.usb usb@1c1d000
01c1d400-01c1d4ff : 1c1d400.usb usb@1c1d400
01c1d800-01c1d803 : 1c19400.phy pmu3
01c20000-01c203ff : clock@1c20000
01c20800-01c20bff : 1c20800.pinctrl pinctrl@1c20800
01c20ca0-01c20cbf : 1c20ca0.watchdog watchdog@1c20ca0
01c25000-01c253ff : 1c25000.thermal-sensor thermal-sensor@1c25000
01c28000-01c2801f : serial
01c30000-01c3ffff : 1c30000.ethernet ethernet@1c30000
01e80000-01eaffff : 1e80000.gpu gpu@1e80000
01f00000-01f003ff : rtc@1f00000
01f00c00-01f00fff : interrupt-controller@1f00c00
01f01400-01f014ff : clock@1f01400
01f02c00-01f02fff : 1f02c00.pinctrl pinctrl@1f02c00
40000000-5fffffff : System RAM
  40210000-419cffff : Kernel code
  419d0000-41ffffff : reserved
  42000000-4238ffff : Kernel data
  49ff6000-49ffcfff : reserved
  5d400000-5f3fffff : reserved
  5f580000-5fdfffff : reserved
  5fe35000-5fe75fff : reserved
  5fe76000-5fec5fff : reserved
  5fec8000-5fec9fff : reserved
  5feca000-5fecafff : reserved
  5fecb000-5fedefff : reserved
  5fedf000-5fffffff : reserved
```

insert the module and validate the data : 

```
# modprobe mymodule
[ 3180.279818] Linux module 05 skeleton loaded
[ 3180.284122] Error while reserving memory region... [0]=1, [1]=1, [2]=1
[ 3180.290691] chipid=82800001'94004704'5036c304'301e0b4e
[ 3180.295919] temperature=39706 (1539)
[ 3180.299570] mac-addr=02:01:06:50:8d:45
# 
# cat /sys/class/thermal/thermal_zone0/temp 
40063
# ifconfig
eth0      Link encap:Ethernet  HWaddr 02:01:06:50:8D:45  
          inet addr:192.168.53.14  Bcast:192.168.53.255  Mask:255.255.255.0

```

The error is because the memory region for the Chip-ID (0x01C14200) is already reserved by the eeprom. But otherwise the data read by the module is good.

```sh
01c14000-01c143ff : 1c14000.eeprom eeprom@1c14000
```

## Exercice 6

on the host, build and install the module : 

```sh
root ➜ /workspace/src/02_modules/exercice06 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice06 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/02_modules/exercice06/skeleton.o
  LD [M]  /workspace/src/02_modules/exercice06/mymodule.o
  MODPOST /workspace/src/02_modules/exercice06/Module.symvers
  CC [M]  /workspace/src/02_modules/exercice06/mymodule.mod.o
  LD [M]  /workspace/src/02_modules/exercice06/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice06 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on nanopi test the module : 

```sh
modprobe mymodule
[ 4432.162001] Linux module 06 skeleton loaded
[ 4432.166500] skeleton thread is now active...
[ 4437.215449] skeleton thread is kick every 5 seconds...
[ 4442.335447] skeleton thread is kick every 5 seconds...
[ 4447.455445] skeleton thread is kick every 5 seconds...
[ 4452.575450] skeleton thread is kick every 5 seconds...
[ 4457.695451] skeleton thread is kick every 5 seconds...
rmmod mymodule
[ 4462.815463] skeleton thread is kick every 5 seconds...
[ 4462.820677] Linux module skeleton unloaded
```

## Exercice 7

On the host, build and install the module

```sh
root ➜ /workspace/src/02_modules/exercice07 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice07 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/02_modules/exercice07/skeleton.o
  LD [M]  /workspace/src/02_modules/exercice07/mymodule.o
  MODPOST /workspace/src/02_modules/exercice07/Module.symvers
  CC [M]  /workspace/src/02_modules/exercice07/mymodule.mod.o
  LD [M]  /workspace/src/02_modules/exercice07/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice07 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

on the nanopi test the module : 

```sh
modprobe mymodule
[ 4941.103849] Linux module 07 skeleton loaded
[ 4941.108337] skeleton thread_1 is now active...
[ 4941.108430] skeleton thread_2 is now active...
[ 4946.143461] skeleton thread_2 timout elapsed...
[ 4946.148053] skeleton thread_1 has been kicked
[ 4951.263455] skeleton thread_2 timout elapsed...
[ 4951.268030] skeleton thread_1 has been kicked
rmmod mymodule
[ 4956.287451] skeleton thread_2 timout elapsed...
[ 4956.292050] skeleton thread_1 has been kicked
[ 4956.296448] Linux module skeleton unloaded
```

## Exercice 8

build and install the module : 

```sh
root ➜ /workspace/src/02_modules/exercice08 (main) $ make && make install
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice08 ARCH=arm64 CROSS_COMPILE=/buildroot/output/host/usr/bin/aarch64-buildroot-linux-gnu- modules
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  CC [M]  /workspace/src/02_modules/exercice08/skeleton.o
  LD [M]  /workspace/src/02_modules/exercice08/mymodule.o
  MODPOST /workspace/src/02_modules/exercice08/Module.symvers
  CC [M]  /workspace/src/02_modules/exercice08/mymodule.mod.o
  LD [M]  /workspace/src/02_modules/exercice08/mymodule.ko
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
make -C /buildroot/output/build/linux-5.15.148/ M=/workspace/src/02_modules/exercice08 INSTALL_MOD_PATH=/rootfs modules_install
make[1]: Entering directory '/buildroot/output/build/linux-5.15.148'
  INSTALL /rootfs/lib/modules/5.15.148/extra/mymodule.ko
  DEPMOD  /rootfs/lib/modules/5.15.148
make[1]: Leaving directory '/buildroot/output/build/linux-5.15.148'
```

test the module on the nanopi : 

```sh
modprobe mymodule
[ 5101.606491] Linux module 08 skeleton loaded
[ 5105.086629] interrupt gpio_a.0-k1 raised...
[ 5107.025901] interrupt gpio_a.2-k2 raised...
[ 5108.277438] interrupt gpio_a.2-k2 raised...
[ 5109.036591] interrupt gpio_a.3-k3 raised...
[ 5110.196269] interrupt gpio_a.0-k1 raised...
[ 5110.860538] interrupt gpio_a.2-k2 raised...
[ 5111.282152] interrupt gpio_a.3-k3 raised...
rmmod mymodule
[ 5115.718990] Linux module skeleton unloaded
```