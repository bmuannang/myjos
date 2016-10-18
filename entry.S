#include "memlayout.h"

.globl _start
_start = V2P_WO(entry)

.globl entry
entry:
	movl $V2P_WO((stack + KSTACKSIZE)), %esp
	call clearScreen
	call showWelcome
	call main
spin:
	jmp spin

.comm stack, KSTACKSIZE
