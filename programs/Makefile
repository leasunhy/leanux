CWD:=$(shell pwd)
LIBS:=$(LIBS) -lgcc -nostdlib -I$(CWD)/../libc/include

OBJCOPY:=i686-elf-objcopy

programs:=sh.bin prog1.bin prog2.bin prog3.bin prog9.bin
libc_libs:=$(CWD)/../libc/stdio/*.o $(CWD)/../libc/string/*.o

CRTI:=crti.o

.PHONY: clean all disk

all: disk

disk: $(programs)
	dd if=/dev/zero of=disk.img bs=1024 count=1440
	dd if=sh.bin of=disk.img bs=1 seek=37888 conv=notrunc
	dd if=prog1.bin of=disk.img bs=1 seek=1024 conv=notrunc
	dd if=prog2.bin of=disk.img bs=1 seek=5120 conv=notrunc
	dd if=prog3.bin of=disk.img bs=1 seek=9216 conv=notrunc
	dd if=prog9.bin of=disk.img bs=1 seek=33792 conv=notrunc

sh.bin: $(CRTI) sh.o $(CWD)/../lib/utils.o $(CWD)/../lib/asm_utils.o
	$(CC) $^ -o $(subst bin,elf,$@) $(CFLAGS) $(LIBS) -T user_linker.ld
	$(OBJCOPY) -O binary $(subst bin,elf,$@) $@

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(LIBS)

%.bin: $(CRTI) %.o $(libc_libs)
	$(CC) $^ -o $(subst bin,elf,$@) $(CFLAGS) $(LIBS) -T user_linker.ld
	$(OBJCOPY) -O binary $(subst bin,elf,$@) $@

%.o: %.S
	$(AS) $(ASFLAGS) -f elf32 $< -o $@

clean:
	-rm *.elf *.bin

