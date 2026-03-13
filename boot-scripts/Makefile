all: boot.cifs boot.net

boot.cifs: boot_cifs.cmd
	mkimage -T script -A arm -C none -d $< $@

boot.net: boot_net.cmd
	mkimage -T script -A arm -C none -d $< $@
