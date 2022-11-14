# OSeKAI Project  
> **[ Work in Progress ]**  
> Bootloader  : Limine  
> Protocol    : Limine 

## Todo list
- [x] Terminals & Framebuffers
- [x] GDT
- [x] IDT
- [x] PIC
- - [ ] PIT
- - [x] Keyboard
- - [ ] Mouse

<hr>

![OSeKAI-Keyboard](https://i.gyazo.com/a3ec7d32339097336f08a58bdd369053.gif)

<hr>

## How to run?  
Currently tested and work with Linux or WSL, it may be possible with Visual Studio on Windows but i haven't tried it.

### requirements
- [XLaunch](https://sourceforge.net/projects/xming/) (for WSL user)
- [qemu-system-x86_64](https://www.qemu.org/download/) recommended virtual box
- [build-essentials](https://linuxhint.com/install-build-essential-ubuntu/) C/C++ packages for compiling

### Clone the project
```bash
git clone https://github.com/Fastering18/OSeKAI
cd OSeKAI
```

### Compile & run with make
```bash
make all
```
That's all of compiling the code! you can also check for latest ISO file release [here](https://github.com/Fastering18/OSeKAI/tags).

<hr>

Join our conversation here to get help [Discord Server](https://discord.gg/8vdZsBBGRG)