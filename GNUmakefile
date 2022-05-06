SOURCEDIR = src
KERNELDIR = $(SOURCEDIR)/kernel
STYLEDIR = $(SOURCEDIR)/style

KERNEL := $(KERNELDIR)/kernel.elf
BACKROUND = $(STYLEDIR)/amongus.bmp
UNIFONTBIN = $(STYLEDIR)/unifont.bin
UNIFONTSLN = $(STYLEDIR)/unifont.sfn

QEMU = qemu-system-x86_64
QEMUF = -M q35 -serial stdio -m 2G -cdrom oserax.iso -boot d

.PHONY: all
all: oserax.iso

.PHONY: all-hdd
all-hdd: oserax.hdd

.PHONY: run
run: oserax.iso
	$(QEMU) $(QEMUF)

.PHONY: run-uefi
run-uefi: ovmf-x64 oserax.iso
	$(QEMU) $(QEMUF) -bios ovmf-x64/OVMF.fd
#qemu-system-x86_64 -M q35 -m 2G -bios ovmf-x64/OVMF.fd -cdrom oserax.iso -boot d

.PHONY: run-hdd
run-hdd: oserax.hdd
	$(QEMU) -M q35 -serial stdio -m 2G -hda oserax.hdd

.PHONY: run-hdd-uefi
run-hdd-uefi: ovmf-x64 oserax.hdd
	$(QEMU) -M q35 -serial stdio -m 2G -bios ovmf-x64/OVMF.fd -hda oserax.hdd

ovmf-x64:
	mkdir -p ovmf-x64
	cd ovmf-x64 && curl -o OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && 7z x OVMF-X64.zip

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v3.0-branch-binary --depth=1
	make -C limine

.PHONY: $(KERNELDIR)
kernel:
	$(MAKE) -C $(SOURCEDIR)

oserax.iso: limine kernel
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
		iso_root -o oserax.iso
	limine/limine-deploy oserax.iso
	rm -rf iso_root

oserax.hdd: limine kernel
	rm -f oserax.hdd
	dd if=/dev/zero bs=1M count=0 seek=64 of=oserax.hdd
	parted -s oserax.hdd mklabel gpt
	parted -s oserax.hdd mkpart ESP fat32 2048s 100%
	parted -s oserax.hdd set 1 esp on
	limine/limine-deploy oserax.hdd
	sudo losetup -Pf --show oserax.hdd >loopback_dev
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
	rm -rf iso_root oserax.iso oserax.hdd
	$(MAKE) -C $(SOURCEDIR) clean

.PHONY: distclean
distclean: clean
	rm -rf limine ovmf-x64
	$(MAKE) -C $(SOURCEDIR) distclean