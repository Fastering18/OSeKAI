export DISPLAY=:0
qemu-system-x86_64 -M q35 -serial stdio -m 2G -cdrom osekai.iso -boot d

# git add .
# git commit -m ""
# git push --set-upstream https://github.com/Fastering18/osekai work-c-limine