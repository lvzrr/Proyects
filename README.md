> [!Warning]
**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**
## [Clearf](https://github.com/lvzrr/Recreational-Programming/tree/main/Clearf)

> [!Note]
Only works in **POSIX** systems, or at least in linux, as it gets keystrokes data directly from the driver

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
> [!Warning]
**CLEARF IS COMING**
## [KeyWatcher](https://github.com/lvzrr/Recreational-Programming/tree/main/KeyWatcher)

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
## [Locksmith](https://github.com/lvzrr/Recreational-Programming/tree/main/Locksmith)
Random Password Generator in C 
## [Note-Cli](https://github.com/lvzrr/Recreational-Programming/tree/main/Note-Cli)
A simple TUI TO-DO app, this uses [this other project](https://github.com/lvzrr/Proyects/tree/main/ObfusKey) for encrypting the notes

> [!NOTE]
Precompiled proyect uses **MY OWN DIRECTORIES**, change the path macros in Note-Cli.h to your own directories and compile again

```bash
chmod +x run.sh
./run.sh
```


## [ObfusKey](https://github.com/lvzrr/Recreational-Programming/tree/main/ObfusKey)
Encrypt and decrypt files using randomly generated keys
## [Pomodoro](https://github.com/lvzrr/Recreational-Programming/tree/main/Pomodoro)
TUI for a pomodoro timer 
## [Sniffer](https://github.com/lvzrr/Recreational-Programming/tree/main/Sniffer)
A ls type util that uses nerdfonts to display files clearer than the default ls command
## [TapRec](https://github.com/lvzrr/Recreational-Programming/tree/main/TapRec)
A simple audio recorder, with somewhat of a waveform visualizer in the terminal
# Latest Commits:
| Commit Hash | Commit Msg | Author | Date |
|-------------|------------|--------|------|
| e5435cc | Update README.md | lvzrr | 2024-12-19 |
| ca5ebca | Update README.md | lvzrr | 2024-12-19 |
| bf693a4 | removed author, added date | lvzrr | 2024-12-19 |
| cfd01c3 | added author to table | lvzrr | 2024-12-19 |
| d0b3e0a | table history generation method added | lvzrr | 2024-12-19 |
| f2a40a6 | made commithistory table | lvzrr | 2024-12-19 |
| ac1fe9e | created table generator function for commit history | lvzrr | 2024-12-19 |
| 3f7fb01 | added commit history table generation | lvzrr | 2024-12-19 |
| 9ccc538 | Updated Script | lvzrr | 2024-12-19 |
| dc213bb | updated README | lvzrr | 2024-12-19 |
