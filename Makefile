export
MAKE=make
CWD=$(shell pwd)
MAIN_WD:=$(CWD)

CC:=i686-elf-gcc
CFLAGS:=$(CFLAGS) -ffreestanding -O2 -Wall -Wextra -masm=intel -std=gnu11 -g
LDFLAGS:=$(LDFLAGS)

AS=nasm

QEMU=qemu-system-i386
QEMUOPTIONS=-enable-kvm

LIBS:=$(LIBS) -nostdlib -lgcc -I$(CWD)/include

OBJS:=

KERNEL_OBJ_LINK_LIST:=$(CRTI_OBJ) $(CRTN_OBJ) $(CRTBEGIN_OBJ) $(CRTEND_OBJ)\
	init/multiboot.o init/main.o\
	interrupts/isr.o\
	kernel/interrupt.o kernel/low_level.o kernel/mm.o kernel/syscall.o kernel/timer.o kernel/sched.o\
	drivers/keyboard.o drivers/tty.o\
	lib/printk.o lib/cirqueue.o lib/utils.o lib/asm_utils.o\

.PHONY: all clean run libc programs iso

all: leanux programs

leanux: $(KERNEL_OBJ_LINK_LIST) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(KERNEL_OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

libc:
	$(MAKE) -C libc

programs: libc
	$(MAKE) -C programs
	cp programs/disk.img .

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(LIBS)

%.o: %.S
	$(AS) $(ASFLAGS) -f elf32 $< -o $@

clean:
	-$(MAKE) -C libc clean
	-$(MAKE) -C programs clean
	-rm -f leanux $(OBJS)
	-find -type f -name \*.o -delete

run: all
	$(QEMU) $(QEMUOPTIONS) -kernel leanux -s -monitor telnet:localhost:2234,server,nowait -drive file=disk.img,format=raw,index=0,media=disk

iso: all
	cp leanux iso/boot
	grub-mkrescue -o leanux.iso iso


