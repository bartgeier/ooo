# OOO

Is a c code formater.  
Amalgamated ooo.c is all you need.  

## Build ooo from Source
Linux gcc
```console
$ gcc ooo.c -o ooo 
```
Windows Microsoft MSVC 
```console
$ cl ooo.c 
```
Windows WinLibs standalone build of GCC and MinGW-w64 for Windows\
https://winlibs.com/
```console
$ gcc ooo.c -o ooo 
```

## Run ooo
```console
$ ./ooo --help
./ooo -i ./example/hello.c -o ./example/hello.c   Apply code style: input -> output file

./ooo -i ./example/hello.c -o *                   Apply code style in-place (overwrite input file)

./ooo -i ./example/hello.c -o -                   Print styled code to stdout

./ooo --kr -t8 -i./example/hello.c -o*            -t8 8 spaces indentation,
                                                  --kr places function opening braces 
                                                       on a new line (Kernighan & Ritchie style variant)

./ooo -p=42=56 -i./example/hello.c                Print Tree-sitter output for a range of source lines to stdout
                                                  (row_end is not included in the output)

-v --version()
-h --help()
-l --license()
-p --print(uint row_begin = 0, uint row_end = 0xFFFFFFFF)
-i --input(cstr_t file_path)
-o --output(cstr_t file_path)
-t --indent(uint indent)
-k --kr()

https://github.com/bartgeier/ooo 
Command line parser => https://github.com/bartgeier/arq 
```

# Examples

```console
$ cd examples
$ gcc -o style style.c # ONLY ONCE!!!
$ ./style
```

Files in examples/source are formatted and saved in examples/styled_source.
- donut.c is from => https://www.a1k0n.net/2006/09/15/obfuscated-c-donut.html
- conway.c is from => https://github.com/tsoding/conway/blob/main/conway.c
- arr.c is my test file.

donut.c and conway.c can be compiled and executed
```console
$ gcc -o donut donut.c -lm
$ ./donut
```
```console
$ gcc -o conway conway.c
$ ./conway
```

# Nob
Build system from Tsoding\
https://youtu.be/eRt7vhosgKE?si=ECJcNhP0EiBCh3Ib \
https://youtu.be/l9_TUMZSewo?si=gic5P1pCdEyIOB0Y \
https://youtu.be/wH963jJ1lRM?si=8i0S8ofyEtxvPAvk \
I made some modifications to nob.h for my needs.

### ./nob clean
Deletes all the downloads and builds.
