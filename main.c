#include "types.h"
#include "defs.h"
#include "mm.h"

extern char end[];

int main(void) {
	showWelcome();
	kFreeVMInit(end, P2V(4*1024*1024), 1);
	kPageTableInit(); //只映射内核空间，即2G+,0-4M没有映射，若使用则会抛异常
	panic();
	for (;;) {

	}
	return 0;
}

__attribute__((__aligned__(PGSIZE)))
 pde_t entrypgdir[NPTENTRIES] = {
// 线性地址 0, 4MB) --> 物理地址 [0, 4MB)
	[0] = (0) | PTE_P | PTE_W | PTE_PS,
	// 线性地址 [2G, 2G+4MB) --> 物理地址 [0, 4MB)
	[KERNBASE >> PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS, };

