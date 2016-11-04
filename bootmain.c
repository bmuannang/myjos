#include "types.h"
#include "elf.h"
#include "x86.h"

#define SECTSIZE  512

void readseg(uchar*, uint, uint);

//	获取elf文件段信息 readelf -l kernel,输出如下, elf文件头4K，代码段从4K(0x001000)开始
//	Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
//	LOAD           0x001000 0x80100000 0x00100000 0x003fc 0x003fc R E 0x1000  代码段
//	LOAD           0x002000 0x80101000 0x00101000 0x01004 0x02010 RW  0x1000  数据段
//	GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RWE 0x10

void bootmain(void) {
	struct elfhdr *elf;
	struct proghdr *ph, *eph;
	void (*entry)(void);
	uchar* pa;

	elf = (struct elfhdr*) 0x10000;  // 64K, temp space

	readseg((uchar*) elf, 4096, 0);

	if (elf->magic != ELF_MAGIC)
		return;

	ph = (struct proghdr*) ((uchar*) elf + elf->phoff);
	eph = ph + elf->phnum;
	for (; ph < eph; ph++) {
		pa = (uchar*) ph->paddr;
		readseg(pa, ph->filesz, ph->off);
		//补0，如bss段，在文件中不占空间，在内存中占空间，例如filesz=0x1004 < memsz=0x2010
		if (ph->memsz > ph->filesz)
			stosb(pa + ph->filesz, 0, ph->memsz - ph->filesz);
	}

	entry = (void (*)(void)) (elf->entry);
	entry();
}

void waitdisk(void) {
	while ((inb(0x1F7) & 0xC0) != 0x40)
		;
}

void readsect(void *dst, uint offset) {
	waitdisk();
	outb(0x1F2, 1);   // count = 1
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);  // cmd 0x20

	waitdisk();
	insl(0x1F0, dst, SECTSIZE / 4);
}

void readseg(uchar* pa, uint count, uint offset) {
	uchar* epa;

	epa = pa + count;

	pa -= offset % SECTSIZE;

	offset = (offset / SECTSIZE) + 1;

	for (; pa < epa; pa += SECTSIZE, offset++)
		readsect(pa, offset);
}
