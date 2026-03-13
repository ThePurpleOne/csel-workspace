setenv ipaddr      192.168.53.14
setenv serverip    192.168.53.4
setenv netmask     255.255.255.0
setenv gatewayip   192.168.53.4
setenv hostname    myhost
setenv mountpath   rootfs
setenv tftppath    output/images
setenv bootargs    console=ttyS0,115200 earlyprintk rootdelay=1 root=/dev/cifs rw cifsroot=//$serverip/$mountpath,username=root,password=toor,port=1445 ip=$ipaddr:$serverip:$gatewayip:$netmask:$hostname::off

usb start
ping $serverip

setenv kernel_comp_addr_r 0x50000000
tftp $kernel_comp_addr_r $serverip:$tftppath/Image.gz
unzip $kernel_comp_addr_r $kernel_addr_r
tftp $fdt_addr_r $serverip:$tftppath/nanopi-neo-plus2.dtb

booti $kernel_addr_r - $fdt_addr_r