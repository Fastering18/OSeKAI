## Some tags may contain information and commands about iso build
## uncomment them to run different modes such as EFI, UEFI, HDD 

## XLaunch params (for WSL + XLaunch)
export DISPLAY=:0

## RUNNING ISO
## qemu default efi
qemu-system-x86_64 -M q35 -serial stdio -m 2G -cdrom osekai.iso -boot d

## qemu UEFI
# qemu-system-x86_64 -M q35 -serial stdio -cdrom osekai.iso -boot d -cpu max -smp 4 -m 512M	-device isa-debug-exit,iobase=0xf4,iosize=0x04-bios ovmf-x64/OVMF.fd

## qemu HDD (need to compile with `make all-hdd`)
# qemu-system-x86_64 -M q35 -serial stdio -m 2G -hda osekai.hdd

## qemu HDD + UEFI
# qemu-system-x86_64 -M q35 -serial stdio -m 2G -bios ovmf-x64/OVMF.fd -hda osekai.hdd



## PUSH UPDATES
# git add .
# git commit -m ""
# git push --set-upstream https://github.com/Fastering18/osekai work-c-limine

# ![OSeKAI-Preview](https://i.gyazo.com/6612d17dcdd95bfdde4acba44f9cfb42.png)  