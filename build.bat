@echo off

REM config
set name="hello"
set cc65=c:\nes\cc65
set include=include
set lib=lib
set ccflags=-Oi
set platform=nes
set config=nrom_128_horz.cfg

REM set path
set path=%path%;%cc65%\bin

cc65 %ccflags% -I %include% -I nam -t %platform% prg/%name%.c --add-source
ca65 prg/%name%.s
ca65 lib\neslib\crt0.s 
ld65 -C config\%config% -o bin/%name%.nes lib\neslib\crt0.o prg/%name%.o lib\nes.lib
.\tools\fceux\fceux.exe bin/%name%.nes