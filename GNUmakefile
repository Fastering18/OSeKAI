SOURCEDIR = src
KERNELDIR = $(SOURCEDIR)/kernel
STYLEDIR = $(SOURCEDIR)/style

KERNEL := $(KERNELDIR)/kernel.elf
BACKROUND = $(STYLEDIR)/purple_esthetic_wallpaper.bmp
UNIFONTBIN = $(STYLEDIR)/unifont.bin
UNIFONTSLN = $(STYLEDIR)/unifont.sfn

QEMU = qemu-system-x86_64
QEMUF = -M q35 -serial stdio       \
        -cdrom osekai.iso -boot d  \
		-cpu max -smp 4 -m 512M	   \
		-device isa-debug-exit,iobase=0xf4,iosize=0x04

.PHONY: all
all: osekai.iso

.PHONY: all-hdd
all-hdd: osekai.hdd

.PHONY: run
run: osekai.iso
	$(QEMU) $(QEMUF)

.PHONY: run-uefi
run-uefi: ovmf-x64 osekai.iso
	$(QEMU) $(QEMUF) -bios ovmf-x64/OVMF.fd
#qemu-system-x86_64 -M q35 -m 2G -bios ovmf-x64/OVMF.fd -cdrom osekai.iso -boot d

.PHONY: run-hdd
run-hdd: osekai.hdd
	$(QEMU) -M q35 -serial stdio -m 2G -hda osekai.hdd

.PHONY: run-hdd-uefi
run-hdd-uefi: ovmf-x64 osekai.hdd
	$(QEMU) -M q35 -serial stdio -m 2G -bios ovmf-x64/OVMF.fd -hda osekai.hdd

ovmf-x64:
	mkdir -p ovmf-x64
	cd ovmf-x64 && curl -o OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && 7z x OVMF-X64.zip

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1
	make -C limine

.PHONY: $(KERNELDIR)
kernel:
	$(MAKE) -C $(SOURCEDIR)

osekai.iso: limine kernel
	rm -rf iso_root
	mkdir -p iso_root
	cp $(KERNEL) \
		limine.cfg limine/limine.sys limine/limine-cd.bin \
		limine/limine-cd-efi.bin \
		$(BACKROUND) $(UNIFONTBIN) $(UNIFONTSLN) iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o osekai.iso
	limine/limine-deploy osekai.iso
	rm -rf iso_root

osekai.hdd: limine kernel
	rm -f osekai.hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=osekai.hdd
	parted -s osekai.hdd mklabel gpt
	parted -s osekai.hdd mkpart ESP fat32 2048s 100%
	parted -s osekai.hdd set 1 esp on
	limine/limine-deploy osekai.hdd
	sudo losetup -Pf --show osekai.hdd >loopback_dev
	sudo mkfs.fat -F 32 `cat loopback_dev`p1
	mkdir -p img_mount
	sudo mount `cat loopback_dev`p1 img_mount
	sudo mkdir -p img_mount/EFI/BOOT
	sudo cp -v $(KERNELDIR)/kernel.elf limine.cfg limine/limine.sys img_mount/
	sudo cp -v limine/BOOTX64.EFI img_mount/EFI/BOOT/
	sync
	sudo umount img_mount
	sudo losetup -d `cat loopback_dev`
	rm -rf loopback_dev img_mount

.PHONY: clean
clean:
	rm -rf iso_root osekai.iso osekai.hdd
	$(MAKE) -C $(SOURCEDIR) clean

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-x64
	$(MAKE) -C $(SOURCEDIR) distclean