export
MAKE=make
CWD=$(shell pwd)

CC:=i686-elf-gcc
CFLAGS:=$(CFLAGS) -ffreestanding -O2 -Wall -Wextra -masm=intel
LDFLAGS:=$(LDFLAGS)

AS=nasm

QEMU=qemu-system-i386
QEMUOPTIONS=-enable-kvm

LIBS:=$(LIBS) -nostdlib -lgcc -I$(CWD)/include

OBJS:=

KERNEL_OBJ_LINK_LIST:=init/multiboot.o init/main.o kernel/tty.o

.PHONY: all clean run libc

all: leanux

leanux: $(KERNEL_OBJ_LINK_LIST) linker.ld
	$(CC) -T linker.ld -o $@ $(CFLAGS) $(KERNEL_OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

libc:
	$(MAKE) -C libc

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(LIBS)

%.o: %.S
	$(AS) $(ASFLAGS) -f elf32 $< -o $@

clean:
	-$(MAKE) -C libc clean
	-rm -f leanux $(OBJS)
	-find -type f -name \*.o -delete

run: all
	$(QEMU) $(QEMUOPTIONS) -kernel leanux -s


