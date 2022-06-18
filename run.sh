export DISPLAY=:0
qemu-system-x86_64 -M q35 -serial stdio -m 2G -cdrom oserax.iso -boot d

# git add .
# git commit -m ""
# git push --set-upstream https://github.com/Fastering18/OSeraX work-c-limine