#!/bin/bash
sudo mkdir -p /mnt/flop/kernel
sudo losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 /mnt/flop
sudo cp src/kernel /mnt/flop/kernel
sudo umount /dev/loop0
sudo losetup -d /dev/loop0
