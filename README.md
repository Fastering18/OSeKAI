# OSeKAI Project  
[![OSeKAI logo](https://see.fontimg.com/api/renderfont4/gRqP/eyJyIjoiZnMiLCJoIjoxNDEsInciOjEwOTAsImZzIjoxMjksImZnYyI6IiNCNjI5RkEiLCJiZ2MiOiIjMEYwODE5In0/T1NlS0FJ/presa-ultralight-antipixelcomar.png)](https://github.com/Fastering18/OSeKAI/#)
![Build status](https://img.shields.io/github/workflow/status/Fastering18/OSeKAI/compile-test?color=pink&logo=github&style=for-the-badge)
![Discord community](https://img.shields.io/discord/795942661163974656?color=purple&label=Discord&logo=Discord&logoColor=white&style=for-the-badge)
![Download count](https://img.shields.io/github/downloads/Fastering18/OSeKAI/total?style=for-the-badge) 

> **[ Work in Progress ]**  
> Bootloader  : Limine  
> Protocol    : Limine 

## Todo list
- [x] Terminals & Framebuffers
- [x] GDT
- [x] IDT
- [x] PIC
- [ ] PMM
- [ ] VMM
- - [x] PIT
- - [x] Keyboard
- - [ ] Mouse

<hr>

![OSeKAI-Keyboard](https://i.gyazo.com/a3ec7d32339097336f08a58bdd369053.gif)

<hr>

## How to compile & run?  
Currently tested and work with Linux or WSL, it may be possible with Visual Studio on Windows but i haven't tried it.

### requirements
- [XLaunch](https://sourceforge.net/projects/xming/) (for WSL user)
- [qemu-system-x86_64](https://www.qemu.org/download/) recommended emulator
- [build-essentials](https://linuxhint.com/install-build-essential-ubuntu/) C/C++ packages for compiling

### Clone the project
```bash
git clone https://github.com/Fastering18/OSeKAI
cd OSeKAI
```

### Compile with make
```bash
make all
```
### Run the ISO
```bash
bash run.sh
```

That's all of compiling the code! you can also check for latest ISO file release [here](https://github.com/Fastering18/OSeKAI/tags).

<hr>

Join our conversation here to get help [Discord Server](https://discord.gg/8vdZsBBGRG)