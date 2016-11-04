#include "mm.h"

.globl _start
_start = V2P_WO(entry)

.globl entry
entry:
	#如果需要用栈，比如call指令等，则需要设置esp, 放开下一行代码即可(不设置的话，会出现什么情况不好说)
	#movl $V2P_WO((stack + KSTACKSIZE)), %esp

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

	#将esp设定为2G+x，即内核空间线性地址
	movl $(stack + KSTACKSIZE), %esp
	call clearScreen

	mov $main, %eax
  	jmp *%eax

spin:
	jmp spin

.comm stack, KSTACKSIZE
