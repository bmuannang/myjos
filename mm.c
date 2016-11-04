/*
 * mm.c
 *
 *  Created on: 2016-11-2
 *      Author: jared
 */

#include "types.h"
#include "mm.h"
#include "defs.h"
#include "x86.h"

extern char end[];
extern char data[];
pde_t *kpgdir;          //内核页目录基址

struct run {
	struct run *next;
};

struct {
	uint lock;
	int use_lock;
	struct run *freelist;
} kmem;

void kFreeVMInit(void *vstart, void *vend, int first) {
	if (first == 1)
		kmem.lock = 0;
	freeVMRange(vstart, vend);
	if (first == 0)
		kmem.lock = 1;
}

void freeVMRange(void *vstart, void *vend) {
	char *p;
	p = (char*) PGROUNDUP((uint)vstart);
	for (; p + PGSIZE <= (char*) vend; p += PGSIZE)
		kFreePage(p);
}

void kFreePage(char *v) {
	struct run *r;

	if ((uint) v % PGSIZE || v < end || v2p(v) >= PHYSTOP)
		panic("kfree");

	r = (struct run*) v;
	r->next = kmem.freelist;
	kmem.freelist = r;
}

static struct kmap {
	void *virt;
	uint phys_start;
	uint phys_end;
	int perm;
} kmap[] = { { (void*) KERNBASE, 0, EXTMEM, PTE_W }, // I/O space
		{ (void*) KERNLINK, V2P(KERNLINK), V2P(data), 0 },   // kern text+rodata
		{ (void*) data, V2P(data), PHYSTOP, PTE_W }, // kern data+memory
		{ (void*) DEVSPACE, DEVSPACE, 0, PTE_W }, // more devices
		};

//内核page table初始化，只映射内核空间，即2G+
void kPageTableInit(void) {
	kpgdir = setupKVM();
	switchKVM();
}

void switchKVM(void) {
	lcr3(v2p(kpgdir));   // 加载页目录基址
}

pde_t* setupKVM(void) {
	pde_t *pgdir;
	struct kmap *k;

	if ((pgdir = (pde_t*) kAlloc()) == 0)
		return 0;

	memset(pgdir, 0, PGSIZE);
	for (k = kmap; k < &kmap[LENGTH(kmap)]; k++)
		if (mapPages(pgdir, k->virt, k->phys_end - k->phys_start,
				(uint) k->phys_start, k->perm) < 0)
			return 0;
	return pgdir;
}

pte_t * walkPGDir(pde_t *pgdir, const void *va, int alloc) {
	pde_t *pde;
	pte_t *pgtab;

	pde = &pgdir[PDX(va)];
	if (*pde & PTE_P) {
		pgtab = (pte_t*) p2v(PTE_ADDR(*pde)); //pgtab线性地址
	} else {
		if (!alloc || (pgtab = (pte_t*) kAlloc()) == 0) //pgtab线性地址
			return 0;

		memset(pgtab, 0, PGSIZE);
		*pde = v2p(pgtab) | PTE_P | PTE_W | PTE_U;
	}
	return &pgtab[PTX(va)];
}

int mapPages(pde_t *pgdir, void *va, uint size, uint pa, int perm) {
	char *a, *last;
	pte_t *pte;

	a = (char*) PGROUNDDOWN((uint)va);
	last = (char*) PGROUNDDOWN(((uint)va) + size - 1);
	for (;;) {
		if ((pte = walkPGDir(pgdir, a, 1)) == 0)
			return -1;
		if (*pte & PTE_P)
			panic();
		*pte = pa | perm | PTE_P;
		if (a == last)
			break;
		a += PGSIZE;
		pa += PGSIZE;
	}
	return 0;
}

char* kAlloc(void) {
	struct run *r;

	r = kmem.freelist;
	if (r)
		kmem.freelist = r->next;
	return (char*) r;
}

