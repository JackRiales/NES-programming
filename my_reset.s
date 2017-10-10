; Startup Code for CC65 / CA65 in Assembly

.import _main
.export __STARTUP__:absolute=1
.importzp _nmi_flag, _frame_count

; Linker Generated Symbols

.import __STACK_START__, __STACK_SIZE__
.include "zeropage.inc"
.import initlib, copydata

.segment "ZEROPAGE"

; No vars yet

.segment "HEADER"

