#!/bin/sh

name=hello

########################################################

cc65 -Oi -I include -t nes $name.c --add-source
ca65 $name.s
ca65 reset.s
ld65 -C nes.cfg -o bin/$name.nes reset.o $name.o lib/nes.lib
rm *.o
rm $name.s
open bin/$name.nes