#include "mm.h"

.globl _start
_start = V2P_WO(entry)

.globl entry
entry:
	movl $V2P_WO((stack + KSTACKSIZE)), %esp

	movl    %cr4, %eax
	orl     $(CR4_PSE), %eax
	movl    %eax, %cr4
	# 设置页目录
	movl    $(V2P_WO(entrypgdir)), %eax
	movl    %eax, %cr3
	# 打开分页
	movl    %cr0, %eax
	orl     $(CR0_PG|CR0_WP), %eax
	movl    %eax, %cr0

	call clearScreen
	mov $main, %eax
  	jmp *%eax

spin:
	jmp spin

.comm stack, KSTACKSIZE
