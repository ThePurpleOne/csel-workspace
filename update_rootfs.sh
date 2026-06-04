#!/usr/bin/env bash
mkdir /rootfs_new/
tar xf /buildroot/output/images/rootfs.tar -C /rootfs_new/
rsync -acO --progress --exclude=/etc/fstab /rootfs_new/ /rootfs
rm -Rf /rootfs_new/