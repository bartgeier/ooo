# OOO

> [!WARNING]
> This software is unfinished. Keep your expectations low.

## Build ooo from Source

### Linux

```console
$ cc -o nob nob.c # ONLY ONCE!!!
$ ./nob
```

### Windows 
#### unzip
https://gnuwin32.sourceforge.net/packages/unzip.htm \
Binaries....Zip....457440....14 February 2005....925a5d4d9b4a63ff1a473cc4d47f2f05 \

I used unzip because I had issues on windows with unzipping tree-sitter using `tar -xvzf tree-sitter.zip'.

#### Compiler
WinLibs standalone build of GCC and MinGW-w64 for Windows\
https://winlibs.com/

```console
$ gcc -o nob nob.c # ONLY ONCE!!!
$ ./nob
```

## Nob
Build system from Tsoding\
https://youtu.be/eRt7vhosgKE?si=ECJcNhP0EiBCh3Ib \
https://youtu.be/l9_TUMZSewo?si=gic5P1pCdEyIOB0Y \
https://youtu.be/wH963jJ1lRM?si=8i0S8ofyEtxvPAvk \
I made some modifications to nob.h for my needs.

### ./nob
If ./nob is executed it will download unzip and build:
- tree-sitter 
- tree-sitter-c 
- google unittest 

Next it will create the build directory with the executables:
+ ooo_copy_treesitter_symbols (generates ooo_treesitter_symbols_ids.h see /tree-sitter-c)
+ ooo
+ otest

### ./nob clean
Deletes all the downloads and builds.




