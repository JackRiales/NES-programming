#!/bin/sh

BIN="MyGame.nes"
cc65 -Oi src/game.c --add-source
ca65 lib/crt0.s
ca65 game.s
ld65 -C lib/nrom_128_horz.cfg -o $BIN crt0.o game.o lib/runtime.lib
