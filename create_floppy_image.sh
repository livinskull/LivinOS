#!/bin/bash
# creates a FAT12 1.44MB floppy image
# requirements: dosfstools


IMAGE="floppy.img"




if ! [ -e "$IMAGE" ]; then
	echo -e "creating filesystem...\n"
	dd if=/dev/zero of=$IMAGE bs=512 seek=2880 count=0
	sudo mkdosfs $IMAGE

	echo -e "copying GRUB files...\n"
	mmd -i $IMAGE boot
	mmd -i $IMAGE boot/grub
	mcopy -i $IMAGE /boot/grub/stage1 ::/boot/grub
	mcopy -i $IMAGE /boot/grub/stage2 ::/boot/grub
	mcopy -i $IMAGE /boot/grub/fat_stage1_5 ::/boot/grub
	mcopy -i $IMAGE menu.lst ::/boot/grub
	mdir -i $IMAGE boot/grub
	

	echo -e "installing grub to bootsector...\n"
sudo grub --batch <<THEEND
device (fd0) ${IMAGE}
root (fd0)
setup (fd0)
quit
THEEND

fi


echo -e "\nimage ready!"
