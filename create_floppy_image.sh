#!/bin/bash
# creates a FAT12 1.44MB floppy image
# requirements: dosfstools


IMAGE="floppy.img"
MOUNTPOINT="/tmp/floppyimg"
USE_GRUB2=0
GRUB2_MKIMAGE=grub-mkimage
GRUB2_INSTALL=grub-install


if [ -e "$IMAGE" ]; then
    # image already exists
    exit 0
fi

echo -en "\e[1;37mchecking GRUB version... \e[0m"
sudo which grub &> /dev/null
if [[ $? -ne 0 ]]; then
    USE_GRUB2=1

    # find correct grub2-command
    sudo which $GRUB2_MKIMAGE &> /dev/null
    if [[ $? -ne 0 ]]; then
        sudo which grub2-mkimage &> /dev/null
        if [[ $? -ne 0 ]]; then
            echo -e "\e[1;31mGRUB not installed\e[0m"
            exit 1
        fi

        GRUB2_MKIMAGE=grub2-mkimage
        GRUB2_INSTALL=grub2-install
    fi

    echo -e "\e[1;32mGRUB 2\e[m"
else
    echo -e "\e[1;32mGRUB legacy\e[m"
fi



echo -e "\e[1;37mcreating filesystem...\e[m"
dd if=/dev/zero of=$IMAGE bs=512 seek=2880 count=0
sudo mkfs.vfat $IMAGE
if [[ $? -ne 0 ]]; then
    echo -e "\e[1;31merror\e[0m"
    exit 1
fi


if [[ $USE_GRUB2 -eq 0 ]]; then

    echo -e "\e[1;37mcopying GRUB files...\e[m"
    mmd -i $IMAGE boot
    mmd -i $IMAGE boot/grub
    mcopy -i $IMAGE /boot/grub/stage1 ::/boot/grub
    mcopy -i $IMAGE /boot/grub/stage2 ::/boot/grub
    mcopy -i $IMAGE /boot/grub/fat_stage1_5 ::/boot/grub
    mcopy -i $IMAGE menu.lst ::/boot/grub
    mdir -i $IMAGE boot/grub


    echo -e "\e[1;37minstalling grub to bootsector...\e[m"
sudo grub --batch <<THEEND
device (fd0) ${IMAGE}
root (fd0)
setup (fd0)
quit
THEEND



    echo -e "\n\e[1;32mimage ready!\e[m"

else
    echo "GRUB2 sucks"
    exit 1

    echo -e "\e[1;37mmounting image...\e[m"
    mkdir $MOUNTPOINT
    sudo mount $IMAGE $MOUNTPOINT -t vfat -o loop
    if [ $? -ne 0 ]; then
        echo -e "\e[1;31merror\e[m"
        exit 1
    fi

    sudo mkdir -p $MOUNTPOINT/boot/grub
    sudo cp grub.cfg $MOUNTPOINT/boot/grub/

    echo -e "\e[1;37mcreating loopdevice...\e[m"
    LOOP_DEV=`sudo losetup -f $IMAGE --show`


    echo -e "\e[1;37mcreating image...\e[m"

    sudo $GRUB2_INSTALL \
        --allow-floppy \
        --themes=none \
        --boot-directory=$MOUNTPOINT/boot \
        --modules="fat part_msdos" \
        --install-modules="fat part_msdos minicmd" \
        --locales=en \
        $LOOP_DEV
    if [[ $? -ne 0 ]]; then
        echo -e "\e[1;31merror\e[0m"
        exit 1
    fi


    sudo losetup -d $LOOP_DEV


    while fuser -m $MOUNTPOINT; do sleep 1; done
    sudo umount $MOUNTPOINT
    sleep 1
    rmdir $MOUNTPOINT
fi
