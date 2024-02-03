echo off
REM Desktop short cut to start envoirement.
REM C:\Windows\System32\cmd.exe /k "C:\Projekts\ooo\Win\env.bat"

REM https://winlibs.com/
REM winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-17.0.6-mingw-w64ucrt-11.0.1-r4.zip
SET PATH=C:\mingw64\bin;%PATH%

REM I had issues with tar for unziping thats wy I use unzip.
REM For windows download https://gnuwin32.sourceforge.net/packages/unzip.htm 
SET PATH=C:\unzip-5.51-1-bin\bin;%PATH%
path
cd "C:\Projekts\ooo"
echo on