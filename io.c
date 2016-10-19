/*
 * io.c
 *
 *  Created on: 2016-10-17
 *      Author: jared
 */
#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "x86.h"

#define CRTPORT 0x3d4
static ushort *crt = (ushort*)(0xb8000);  // CGA memory

void cgaputc(int c)
{
  int pos;

  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);

  if(c == '\n')
    pos += 80 - pos%80;
  else{
//	  *((ushort*)(0xb8000+pos*2)) = (c&0xff) | 0x0700;
//	  pos+=1;
    crt[pos++] = (c&0xff) | 0x0700;  // 黑底白字
  }

  if((pos/80) >= 24){  // Scroll up.
    memmove(crt, crt+80, sizeof(crt[0])*23*80);
    pos -= 80;
    memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
  }

  setCursor(pos);
  crt[pos] = ' ' | 0x0700;
}

void setCursor(int pos){
	//高位放14号寄存器
	outb(CRTPORT, 14);
	outb(CRTPORT+1, pos>>8);
	//低位放15号寄存器
	outb(CRTPORT, 15);
	outb(CRTPORT+1, pos);
}

void clearScreen(){
	//光标移动到最开始
	setCursor(0);
	int i;
	for(i=0;i<2000;i++){
		cgaputc(' ');
	}
	//光标恢复到最开始
	setCursor(0);
}

void showWelcome(){
	char * welcome = "HELLO, WELCOME! -- BY TINYOS.";
	char * welcome2 = (char *)V2P(welcome);
	int i, len = strlen(welcome2);
	for(i=0;i<len;i++){
		cgaputc(welcome2[i]);
	}
}
