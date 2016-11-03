/*
 * mm.c
 *
 *  Created on: 2016-11-2
 *      Author: jared
 */

#include "types.h"
#include "mm.h"
#include "defs.h"

extern char end[];

struct run {
  struct run *next;
};

struct {
  uint lock;
  int use_lock;
  struct run *freelist;
} kmem;

void kinit(void *vstart, void *vend, int first){
	if(first==1)
		kmem.lock = 0;
	freeVM(vstart, vend);
	if(first==0)
		kmem.lock = 1;
}

void freeVM(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}

void kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || v2p(v) >= PHYSTOP)
    panic("kfree");

  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
}
