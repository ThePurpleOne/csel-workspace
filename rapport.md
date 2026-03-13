```
root ➜ /workspace (main) $ /usr/local/bin/sync-images.sh
sending incremental file list
created directory /workspace/buildroot-images
./
Image
     34,650,624 100%  492.75MB/s    0:00:00 (xfr#1, to-chk=69/71)
bl31.bin
         20,484 100%  294.18kB/s    0:00:00 (xfr#2, to-chk=68/71)
boot.scr
            269 100%    3.86kB/s    0:00:00 (xfr#3, to-chk=67/71)
boot.vfat
     67,108,864 100%  387.88MB/s    0:00:00 (xfr#4, to-chk=66/71)
nanopi-neo-plus2.dtb
         28,019 100%  164.83kB/s    0:00:00 (xfr#5, to-chk=65/71)
rootfs.ext2
  2,147,483,648 100%  647.90MB/s    0:00:03 (xfr#6, to-chk=64/71)
rootfs.ext4 -> rootfs.ext2
rootfs.tar
    166,942,720 100%   27.22MB/s    0:00:05 (xfr#7, to-chk=62/71)
sdcard.img
  2,215,682,048 100%  254.77MB/s    0:00:08 (xfr#8, to-chk=61/71)
sun50i-h5-nanopi-neo-plus2.dtb
         28,247 100%  113.52kB/s    0:00:00 (xfr#9, to-chk=60/71)
sunxi-spl.bin
         32,768 100%  131.69kB/s    0:00:00 (xfr#10, to-chk=59/71)
u-boot.bin
        610,199 100%    2.38MB/s    0:00:00 (xfr#11, to-chk=58/71)
u-boot.itb
        631,608 100%    2.33MB/s    0:00:00 (xfr#12, to-chk=57/71)
brcm/
brcm/brcmfmac43012-sdio.bin -> ../cypress/cyfmac43012-sdio.bin
brcm/brcmfmac43012-sdio.clm_blob -> ../cypress/cyfmac43012-sdio.clm_blob
brcm/brcmfmac43143-sdio.bin
        385,067 100%    1.42MB/s    0:00:00 (xfr#13, to-chk=51/71)
brcm/brcmfmac43143.bin
        397,312 100%    1.45MB/s    0:00:00 (xfr#14, to-chk=50/71)
brcm/brcmfmac43236b.bin
        348,160 100%    1.27MB/s    0:00:00 (xfr#15, to-chk=49/71)
brcm/brcmfmac43241b0-sdio.bin
        455,745 100%    1.65MB/s    0:00:00 (xfr#16, to-chk=48/71)
brcm/brcmfmac43241b4-sdio.bin
        403,855 100%    1.45MB/s    0:00:00 (xfr#17, to-chk=47/71)
brcm/brcmfmac43241b5-sdio.bin
        408,682 100%    1.46MB/s    0:00:00 (xfr#18, to-chk=46/71)
brcm/brcmfmac43242a.bin
        479,232 100%    1.71MB/s    0:00:00 (xfr#19, to-chk=45/71)
brcm/brcmfmac43340-sdio.bin -> ../cypress/cyfmac43340-sdio.bin
brcm/brcmfmac43340-sdio.meegopad-t08.txt
          1,693 100%    6.17kB/s    0:00:00 (xfr#20, to-chk=43/71)
brcm/brcmfmac43340-sdio.pov-tab-p1006w-data.txt
          1,724 100%    6.28kB/s    0:00:00 (xfr#21, to-chk=42/71)
brcm/brcmfmac43340-sdio.predia-basic.txt
          1,689 100%    6.15kB/s    0:00:00 (xfr#22, to-chk=41/71)
brcm/brcmfmac43362-sdio.bin -> ../cypress/cyfmac43362-sdio.bin
brcm/brcmfmac43362-sdio.cubietech,cubietruck.txt
            803 100%    2.93kB/s    0:00:00 (xfr#23, to-chk=39/71)
brcm/brcmfmac43362-sdio.lemaker,bananapro.txt -> brcmfmac43362-sdio.cubietech,cubietruck.txt
brcm/brcmfmac43430-sdio.AP6212.txt
            875 100%    3.19kB/s    0:00:00 (xfr#24, to-chk=37/71)
brcm/brcmfmac43430-sdio.Hampoo-D2D3_Vi8A1.txt
            928 100%    3.38kB/s    0:00:00 (xfr#25, to-chk=36/71)
brcm/brcmfmac43430-sdio.MUR1DX.txt
            831 100%    3.03kB/s    0:00:00 (xfr#26, to-chk=35/71)
brcm/brcmfmac43430-sdio.bin -> ../cypress/cyfmac43430-sdio.bin
brcm/brcmfmac43430-sdio.clm_blob -> ../cypress/cyfmac43430-sdio.clm_blob
brcm/brcmfmac43430-sdio.friendlyarm,nanopi-r1.txt -> brcmfmac43430-sdio.AP6212.txt
brcm/brcmfmac43430-sdio.raspberrypi,3-model-b.txt
            874 100%    3.18kB/s    0:00:00 (xfr#27, to-chk=31/71)
brcm/brcmfmac43430-sdio.raspberrypi,model-zero-w.txt -> brcmfmac43430-sdio.raspberrypi,3-model-b.txt
brcm/brcmfmac43430-sdio.sinovoip,bpi-m2-plus.txt -> brcmfmac43430-sdio.AP6212.txt
brcm/brcmfmac43430-sdio.sinovoip,bpi-m2-ultra.txt -> brcmfmac43430-sdio.AP6212.txt
brcm/brcmfmac43430-sdio.sinovoip,bpi-m2-zero.txt -> brcmfmac43430-sdio.AP6212.txt
brcm/brcmfmac43430-sdio.sinovoip,bpi-m3.txt -> brcmfmac43430-sdio.AP6212.txt
brcm/brcmfmac43430a0-sdio.bin
        382,455 100%    1.34MB/s    0:00:00 (xfr#28, to-chk=25/71)
brcm/brcmfmac43430a0-sdio.ilife-S806.txt
            908 100%    3.26kB/s    0:00:00 (xfr#29, to-chk=24/71)
brcm/brcmfmac43430a0-sdio.jumper-ezpad-mini3.txt
            951 100%    3.41kB/s    0:00:00 (xfr#30, to-chk=23/71)
brcm/brcmfmac43455-sdio.Raspberry Pi Foundation-Raspberry Pi 4 Model B.txt -> brcmfmac43455-sdio.raspberrypi,4-model-b.txt
brcm/brcmfmac43455-sdio.Raspberry Pi Foundation-Raspberry Pi Compute Module 4.txt -> brcmfmac43455-sdio.raspberrypi,4-model-b.txt
brcm/brcmfmac43455-sdio.bin -> ../cypress/cyfmac43455-sdio.bin
brcm/brcmfmac43455-sdio.clm_blob -> ../cypress/cyfmac43455-sdio.clm_blob
brcm/brcmfmac43455-sdio.raspberrypi,3-model-a-plus.txt -> brcmfmac43455-sdio.raspberrypi,3-model-b-plus.txt
brcm/brcmfmac43455-sdio.raspberrypi,3-model-b-plus.txt
          1,884 100%    6.76kB/s    0:00:00 (xfr#31, to-chk=17/71)
brcm/brcmfmac43455-sdio.raspberrypi,4-model-b.txt
          1,883 100%    6.76kB/s    0:00:00 (xfr#32, to-chk=16/71)
brcm/brcmfmac43569.bin
        557,056 100%    1.93MB/s    0:00:00 (xfr#33, to-chk=15/71)
brcm/brcmfmac43570-pcie.bin -> ../cypress/cyfmac43570-pcie.bin
brcm/brcmfmac43570-pcie.clm_blob -> ../cypress/cyfmac43570-pcie.clm_blob
brcm/brcmfmac43602-pcie.ap.bin
        595,472 100%    2.06MB/s    0:00:00 (xfr#34, to-chk=12/71)
brcm/brcmfmac43602-pcie.bin
        635,449 100%    2.17MB/s    0:00:00 (xfr#35, to-chk=11/71)
cypress/
cypress/cyfmac43012-sdio.bin
        490,686 100%    1.67MB/s    0:00:00 (xfr#36, to-chk=10/71)
cypress/cyfmac43012-sdio.clm_blob
          4,692 100%   16.36kB/s    0:00:00 (xfr#37, to-chk=9/71)
cypress/cyfmac43340-sdio.bin
        409,717 100%    1.35MB/s    0:00:00 (xfr#38, to-chk=8/71)
cypress/cyfmac43362-sdio.bin
        202,623 100%  682.32kB/s    0:00:00 (xfr#39, to-chk=7/71)
cypress/cyfmac43430-sdio.bin
        419,798 100%    1.38MB/s    0:00:00 (xfr#40, to-chk=6/71)
cypress/cyfmac43430-sdio.clm_blob
          4,733 100%   15.88kB/s    0:00:00 (xfr#41, to-chk=5/71)
cypress/cyfmac43455-sdio.bin
        643,651 100%    2.09MB/s    0:00:00 (xfr#42, to-chk=4/71)
cypress/cyfmac43455-sdio.clm_blob
          4,733 100%   15.77kB/s    0:00:00 (xfr#43, to-chk=3/71)
cypress/cyfmac43570-pcie.bin
        558,388 100%    1.81MB/s    0:00:00 (xfr#44, to-chk=2/71)
cypress/cyfmac43570-pcie.clm_blob
          5,750 100%   19.10kB/s    0:00:00 (xfr#45, to-chk=1/71)
extlinux/
extlinux/extlinux.conf
            144 100%    0.48kB/s    0:00:00 (xfr#46, to-chk=0/71)
```


```
➜  ~ ssh root@192.168.53.14
The authenticity of host '192.168.53.14 (192.168.53.14)' can't be established.
ED25519 key fingerprint is: SHA256:BDseDCDp8Qz+D6lbjdsTw8eVGees+dY9X3qyX4NQ09Y
This key is not known by any other names.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '192.168.53.14' (ED25519) to the list of known hosts.
*****************************************************
   __  __ ____  _____       ____ ____  _____ _
  |  \/  / ___|| ____|     / ___/ ___|| ____| |
  | |\/| \___ \|  _| _____| |   \___ \|  _| | |
  | |  | |___) | |__|_____| |___ ___) | |___| |___
  |_|  |_|____/|_____|     \____|____/|_____|_____|
  
*****************************************************
# 
# uname -a
Linux csel 5.15.148 #1 SMP PREEMPT Thu Mar 5 14:53:00 UTC 2026 aarch64 GNU/Linux
# 

```



```sh
# fdisk /dev/mmcblk2

The number of cylinders for this disk is set to 487136.
There is nothing wrong with that, but this is larger than 1024,
and could in certain setups cause problems with:
1) software that runs at boot time (e.g., old versions of LILO)
2) booting and partitioning software from other OSs
   (e.g., DOS FDISK, OS/2 FDISK)

Command (m for help): p
Disk /dev/mmcblk2: 15 GB, 15962472448 bytes, 31176704 sectors
487136 cylinders, 4 heads, 16 sectors/track
Units: sectors of 1 * 512 = 512 bytes

Device       Boot StartCHS    EndCHS        StartLBA     EndLBA    Sectors  Size Id Type
/dev/mmcblk2p1 *  0,33,50     8,74,18           2128     133199     131072 64.0M  c Win95 FAT32 (LBA)
/dev/mmcblk2p2    8,74,19     269,95,34       133200    4327503    4194304 2048M 83 Linux

Command (m for help): n
Partition type
   p   primary partition (1-4)
   e   extended
p
Partition number (1-4): 3
First sector (16-31176703, default 16): 4327504
Last sector or +size{,K,M,G,T} (4327504-31176703, default 31176703): 
Using default value 31176703

Command (m for help): p
Disk /dev/mmcblk2: 15 GB, 15962472448 bytes, 31176704 sectors
487136 cylinders, 4 heads, 16 sectors/track
Units: sectors of 1 * 512 = 512 bytes

Device       Boot StartCHS    EndCHS        StartLBA     EndLBA    Sectors  Size Id Type
/dev/mmcblk2p1 *  0,33,50     8,74,18           2128     133199     131072 64.0M  c Win95 FAT32 (LBA)
/dev/mmcblk2p2    8,74,19     269,95,34       133200    4327503    4194304 2048M 83 Linux
/dev/mmcblk2p3    1023,3,16   1023,3,16      4327504   31176703   26849200 12.8G 83 Linux

Command (m for help): w
The partition table has been altered.
Calling ioctl() to re-read partition table
[ 1293.196716]  mmcblk2: p1 p2 p3
# 
# mkfs.ext4 /dev/mmcblk2p3
mke2fs 1.46.5 (30-Dec-2021)
Discarding device blocks: done                            
Creating filesystem with 3356150 4k blocks and 840480 inodes
Filesystem UUID: 225c5a5b-411a-4925-ba19-9d77066bc079
Superblock backups stored on blocks: 
	32768, 98304, 163840, 229376, 294912, 819200, 884736, 1605632, 2654208

Allocating group tables: done                            
Writing inode tables: done                            
Creating journal (16384 blocks): 
done
Writing superblocks and filesystem accounting information: done   
```


edit fstab to automount partition on /opt 

```sh
Welcome to FriendlyARM Nanopi NEO Plus2
csel login: root
*****************************************************
   __  __ ____  _____       ____ ____  _____ _
  |  \/  / ___|| ____|     / ___/ ___|| ____| |
  | |\/| \___ \|  _| _____| |   \___ \|  _| | |
  | |  | |___) | |__|_____| |___ ___) | |___| |___
  |_|  |_|____/|_____|     \____|____/|_____|_____|
  
*****************************************************
# 
# 
# 

# ls /opt/
lost+found  startup_hello

# mount
//192.168.53.4/rootfs on / type cifs (rw,relatime,vers=1.0,cache=strict,username=root,uid=0,noforceuid,gid=0,noforcegid,addr=192.168.53.4,hard,unix,posixpaths,serverino,mapposix,cifsacl,acl,mfsymlinks,rsize=1048576,wsize=65536,bsize=1048576,echo_interval=60,noblocksend,actimeo=1,closetimeo=1)
devtmpfs on /dev type devtmpfs (rw,relatime,size=223336k,nr_inodes=55834,mode=755)
proc on /proc type proc (rw,relatime)
devpts on /dev/pts type devpts (rw,relatime,gid=5,mode=620,ptmxmode=666)
tmpfs on /dev/shm type tmpfs (rw,relatime,mode=777)
tmpfs on /tmp type tmpfs (rw,relatime)
tmpfs on /run type tmpfs (rw,nosuid,nodev,relatime,mode=755)
sysfs on /sys type sysfs (rw,relatime)
/dev/mmcblk2p3 on /opt type ext4 (rw,relatime)

# cat /etc/fstab 
# <file system>	<mount pt>	<type>	<options>	<dump>	<pass>
/dev/root	/		ext2	rw,noauto	0	1
proc		/proc		proc	defaults	0	0
devpts		/dev/pts	devpts	defaults,gid=5,mode=620,ptmxmode=0666	0	0
tmpfs		/dev/shm	tmpfs	mode=0777	0	0
tmpfs		/tmp		tmpfs	mode=1777	0	0
tmpfs		/run		tmpfs	mode=0755,nosuid,nodev	0	0
sysfs		/sys		sysfs	defaults	0	0

/dev/mmcblk2p3 /opt ext4 defaults 0 0
```

add S99hello to /etc/init.d to start an executable at boot : 

```sh
#!/bin/sh
#

# Make sure the ssh-keygen progam exists
[ -f /opt/startup_hello ] || exit 0

umask 077

start() {
    /opt/startup_hello
}
stop() {
    echo "STOP"
}
restart() {
	stop
	start
}

case "$1" in
  start)
	start
	;;
  stop)
	stop
	;;
  restart|reload)
	restart
	;;
  *)
	echo "Usage: $0 {start|stop|restart}"
	exit 1
esac

exit $?
```

```
[   10.555385] random: crng init done
Starting sshd: [   10.755711] NET: Registered PF_INET6 protocol family
[   10.762444] Segment Routing with IPv6
[   10.766193] In-situ OAM (IOAM) with IPv6
OK
Hello World!

Welcome to FriendlyARM Nanopi NEO Plus2
csel login: 
```



# Questions Lab 1

## Comment faut-il procéder pour générer l’U-Boot ?

la commande `make uboot` devrait générer uboot.

## Comment peut-on ajouter et générer un package supplémentaire dans le Buildroot ?

il faut aller dans le répertoire de buildroot, lancer `make menuconfig`, puis activer le package que l'on veut rajouter. Une fois effectué, il suffit de recompiler l'image avec la commande `make`

## Comment doit-on procéder pour modifier la configuration du noyau Linux ?

il faut lancer la commande `make linux-menuconfig` dans le répertoire buildroot

## Comment faut-il faire pour générer son propre rootfs ?

il est possible de modifier le rootfs avec des overlay, pour notre board le dossier d'overlay est : 

`/buildroot/board/friendlyarm/nanopi-neo-plus2/rootfs_overlay/`

## Comment faudrait-il procéder pour utiliser la carte eMMC en lieu et place de la carte SD ?

pour utiliser la eMMC, il faudrait changer le `mmc 0` en `mmc 1`.

```sh
fatload mmc 0 $kernel_addr_r Image
fatload mmc 0 $fdt_addr_r nanopi-neo-plus2.dtb
```

## Dans le support de cours, on trouve différentes configurations de l’environnement de développement. Quelle serait la configuration optimale pour le développement uniquement d’applications en espace utilisateur ?

la meilleur config pour du development userspace est boot.cifs, car nous n'avons pas besoin de changer le kernel donc on peut le laisser dans la carte sd ce qui nous permet de boot plus rapidement. Et avec le rootfs en cifs, nous pouvons recompiler l'app, et elle sera directement disponible sur la cible, pas besoin de redémarrer.