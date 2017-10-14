#!/bin/sh

name=hello

########################################################

cc65 -Oi -I include -I nam -t nes prg/$name.c --add-source
cc65 -Oi -I include -I nam -t nes prg/nesdev.c --add-source
ca65 prg/$name.s
ca65 prg/nesdev.s
ca65 lib/neslib/crt0.s
ld65 -C config/nrom_128_horz.cfg -o bin/$name.nes lib/neslib/crt0.o prg/$name.o prg/nesdev.o lib/nes.lib
rm prg/*.o
rm prg/*.s
open bin/$name.nes