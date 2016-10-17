#include "memlayout.h"

.globl _start
_start = V2P_WO(entry)

.globl entry
entry:
	movl $V2P_WO((stack + KSTACKSIZE)), %esp
	call clearScreen
	call showTinyJos
spin:
	jmp spin

showTinyJos:
	mov   $0,  %esi
	call  computeLen

	mov   $0xb8000,        %ebx
	leal  V2P_WO(showStr), %edx
L:
	movb  (%edx,%esi),  %al
	movb  %al,          (%ebx,%esi,2)
	movb  $0x0c,        1(%ebx,%esi,2)
	incl  %esi
	call showCursor
	loop  L
	ret

clearScreen:
	mov   $0xb8000,        %ebx
	mov   $2000,           %ecx
	mov   $0,              %esi
L2:
	movb  $0,  %al
	movb  %al,          (%ebx,%esi,2)
	movb  $0x0c,        1(%ebx,%esi,2)
	incl  %esi
	loop  L2
	ret

showCursor:
	pushl %edx

	mov $0x03d4, %dx
	mov $0x0e, %al
	out %al, %dx
	inc %dx
	mov %esi, %eax
	mov %ah,  %al
	out %al, %dx

	dec %dx
	mov $0x0f, %al
	out %al, %dx
	inc %dx
	mov %esi, %eax
	out %al, %dx

	popl %edx

	ret

computeLen:
	mov   $0, %ecx
	leal  V2P_WO(showStr), %edx
L3:
	movb  (%edx,%ecx),  %al
	mov   $0,  %bl
	cmp   %al, %bl
	je    L4
	inc   %ecx
	jne   L3
L4:
	ret


showStr:
	.ascii "Hello, TinyJos!"
	.byte 0x0

.comm stack, KSTACKSIZE
