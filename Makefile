OBJS = \
	main.o\
	io.o\
	string.o\
	mm.o
	
QEMU = qemu-system-i386

CC = gcc
LD = ld
OBJCOPY = objcopy
OBJDUMP = objdump
CFLAGS = -static -Wall -ggdb -m32 -Werror -O2
ASFLAGS = -m32 -gdwarf-2
LDFLAGS = -m elf_i386

xv6.img: bootblock kernel
	dd if=/dev/zero of=xv6.img count=10000
	dd if=bootblock of=xv6.img conv=notrunc
	dd if=kernel of=xv6.img seek=1 conv=notrunc

bootblock: bootasm.S bootmain.c
	$(CC) $(CFLAGS) -O -nostdinc -I. -c bootmain.c #-c表示只编译不链接 只生成目标文件 bootmain.o
	$(CC) $(CFLAGS) -nostdinc -I. -c bootasm.S
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o bootblock.o bootasm.o bootmain.o
	$(OBJDUMP) -S bootblock.o > bootblock.asm
	$(OBJCOPY) -S -O binary -j .text bootblock.o bootblock #-j .text 表示只copy .text段到目标文件bootblock里，
	                                                       #否则会超过510字节(最后两字节预留0x55AA)
	./makeboot.sh bootblock #制作启动盘

kernel: $(OBJS) entry.o  kernel.ld
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel entry.o $(OBJS)
	$(OBJDUMP) -S kernel > kernel.asm
	$(OBJDUMP) -t kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym

clean: 
	rm -f *.tex *.dvi *.idx *.aux *.log *.ind *.ilg \
	*.o *.d *.asm *.sym bootblock kernel xv6.img 
CPUS := 2
QEMUOPTS = xv6.img -smp $(CPUS) -m 512

qemu: xv6.img
	$(QEMU) -serial stdio $(QEMUOPTS)
	
qemu-gdb: xv6.img .gdbinit
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial stdio $(QEMUOPTS) -S -gdb tcp::26000