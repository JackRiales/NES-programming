@echo off

set name="hello"
set cc65=c:\nes\cc65
set libdir=lib
set incdir=include
set ccflags=-Oi

set path=%path%;%cc65%\bin

cc65 -Oi -I include -t nes %name%.c --add-source
ca65 %name%.s
ca65 reset.s
ld65 -C nes.cfg -o bin/hello.nes reset.o %name%.o lib/nes.lib
.\fceux\fceux.exe bin/hello.nes

del *.o
del %name%.s