@echo off
gcc -Wall -ansi -o pull1.exe pull1.c xmlreader.c -l"../../win32/mingw/dll/parsifal" -I"../../include" -L./
@echo Run it using: pull1 ^< document.xml