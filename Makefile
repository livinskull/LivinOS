ALLFILES = $(shell find -name '*.[cs]')


all:
	$(MAKE) --no-print-directory -C kernel

clean:
	$(MAKE) --no-print-directory -C kernel clean
	rm -f floppy.img

floppy.img: all
	./create_floppy_image.sh
	mcopy -o -i floppy.img kernel/kernel  ::/

test-qemu: floppy.img
	qemu-system-i386 -no-kvm -fda floppy.img -boot a -serial stdio | tee qemu_serial.log
	#qemu-kvm -fda floppy.img -boot a -serial stdio | tee qemu_serial.log

test-bochs: floppy.img
	bochs -f bochsrc.txt

todolist:
	-@for file in $(ALLFILES); do fgrep -H -e TODO -e FIXME $$file; done; true


.PHONY: all clean image test-qemu test-bochs
