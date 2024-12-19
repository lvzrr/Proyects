# RECREATIONAL PROGRAMMING
> [!Warning]
**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**
## Clearf is coming ...

Inspired by [this project](https://github.com/tsoding/voidf), i decided to make my own (much simpler but it still works ig), and i decided to use my most used command in the last month for it: 

```bash
> zhs_stats

     1  2142  21.3517%   clear
     2  1538  15.3309%   nvim
     3  945   9.41986%   ls
     4  530   5.28309%   cd
     5  315   3.13995%   git
     6  250   2.49203%   sudo
     7  238   2.37241%   rm
     8  140   1.39553%   cat
     9  121   1.20614%   yay
    10  108   1.07656%   waybar
    11  77    0.767544%  l
    12  77    0.767544%  dec
    13  71    0.707735%  cp
    14  69    0.687799%  python
    15  65    0.647927%  mv
    16  64    0.637959%  mkdir
    17  64    0.637959%  cargo
    18  62    0.618022%  enc
    19  53    0.528309%  gcc
    20  51    0.508373%  gobuster
```
### To run it: 
Make sure you have raylib installed as well as the gcc compiler, then change the script _run.sh_ to fit your own, then run it like this: 
```bash
chmod +x ./run.sh && ./run.sh
```

---
## KeyWatcher 

A multithreaded daemon to display keystrokes through the keyboard using raylib (Only works on **POSIX** operating systems, or at least in Linux)

> [!Warning]
I'm currently working on it, it can't display certain symbols yet (basically because of raylib's shitty UTF-8 render implementation)

### To run it
Ensure gcc and raylib are installed and change the paths in run.sh, then: 
```bash 
chmod +x run.sh && ./run.sh
```
> [!Note] 
I wouldn't change the font, bc i adjusted the buffer so it has 1 more char than the display, but you do you

---
## Locksmith
Random Password Generator in C 

---
## Note-Cli
A simple TUI TO-DO app, this uses [this other project](https://github.com/lvzrr/Proyects/tree/main/ObfusKey) for encrypting the notes

> [!NOTE]
Precompiled proyect uses **MY OWN DIRECTORIES**, change the path macros in Note-Cli.h to your own directories and compile again

```bash
chmod +x run.sh
./run.sh
```



---
## ObfusKey / D-ObfusKey
Encrypt and decrypt files using randomly generated keys

---
## Pomodoro
TUI for a pomodoro timer 

---
## Sniffer
A ls type util that uses nerdfonts to display files clearer than the default ls command

---
## TapRec
A simple audio recorder, with somewhat of a waveform visualizer in the terminal

---
