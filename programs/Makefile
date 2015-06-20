CWD:=$(shell pwd)
LIBS:=$(LIBS) -lgcc -nostdlib -I$(CWD)/../libc/include

OBJCOPY:=i686-elf-objcopy

programs:=prog1.bin
libc_libs:=$(CWD)/../libc/stdio/*.o

CRTI:=crti.o

.PHONY: clean all disk

all: disk

disk: $(programs)
	dd if=/dev/zero of=disk.img bs=1024 count=1440
	dd if=prog1.bin of=disk.img bs=1 seek=1024 conv=notrunc

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(LIBS)

%.bin: $(CRTI) %.o $(libc_libs)
	$(CC) $^ -o $(subst bin,elf,$@) $(CFLAGS) $(LIBS) -T user_linker.ld
	$(OBJCOPY) -O binary $(subst bin,elf,$@) $@


%.o: %.S
	$(AS) $(ASFLAGS) -f elf32 $< -o $@

clean:
	-rm *.elf *.bin

