#define KERNBASE 0x80000000   
#define V2P_WO(x) ((x) - KERNBASE)  

.globl _start
_start = V2P_WO(entry)

.globl entry
entry:
	mov   $0,  %esi
	mov   $12, %ecx

	mov   $0xb8000,        %ebx
	leal  V2P_WO(showStr), %edx
L:
	movb  (%edx,%esi),  %al
	movb  %al,          (%ebx,%esi,2)
	movb  $0x0c,        1(%ebx,%esi,2)
	incl  %esi
	loop  L
spin:
	jmp spin

showStr:
	.ascii "Hello, myos!"
