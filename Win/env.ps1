# Desktop short cut to start envoirement.
# C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -NoExit -File "C:\Projekts\ooo\Win\env.ps1"

# https://winlibs.com/
# winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-17.0.6-mingw-w64ucrt-11.0.1-r4.zip
$env:path = "C:\mingw64\bin;" + $env:path

# I had issues with tar for unziping thats wy I use unzip.
# For windows download https://gnuwin32.sourceforge.net/packages/unzip.htm 
$env:path = "C:\unzip-5.51-1-bin\bin;" + $env:path

cd "C:\Projekts\ooo"
