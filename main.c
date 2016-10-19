#include "types.h"
#include "defs.h"
#include "mmu.h"
#include "memlayout.h"

void showWelcome();

int main(void){
	showWelcome();
	for(;;){

	}
	return 0;
}

__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPTENTRIES] = {
  // 线性地址 0, 4MB) --> 物理地址 [0, 4MB)
  [0] = (0) | PTE_P | PTE_W | PTE_PS,
  // 线性地址 [2G, 2G+4MB) --> 物理地址 [0, 4MB)
  [KERNBASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};

