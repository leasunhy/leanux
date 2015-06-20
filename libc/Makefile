CWD=$(shell pwd)

CC:=i686-elf-gcc
LIBC_CFLAGS:=$(CFLAGS) -ffreestanding -O2 -Wall -Wextra -masm=intel -std=gnu11 -g
LIBC_LDFLAGS:=$(LDFLAGS)

AR:=i686-elf-ar

LIBS:=$(LIBS) -nostdlib -I$(CWD)/include -lgcc

STDIO_OBJS:=\
	stdio/vprintf.o \
	stdio/putchar.o \
	stdio/puts.o \

STRING_OBJS:=\
	string/strlen.o \
	string/strcpy.o \
	string/strncpy.o \

STDLIB_OBJS:=\
	stdlib/atoi.o \

OBJS:=\
	$(STDIO_OBJS) \
	$(STRING_OBJS) \
	$(STDLIB_OBJS) \

ARCHIVES:=\
	stdio.o \
	string.o \
	stdlib.o \

.PHONY: all clean run

all: libc.a #$(ARCHIVES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

stdio.o: $(STDIO_OBJS)
	$(CC) $(LIBC_CFLAGS) $(LIBC_LDFLAGS) $(LIBS) $(STDIO_OBJS) -o $@

string.o: $(STRING_OBJS)
	$(CC) $(LIBC_CFLAGS) $(LIBC_LDFLAGS) $(LIBS) $(STRING_OBJS) -o $@

stdlib.o: $(STDLIB_OBJS)
	$(CC) $(LIBC_CFLAGS) $(LIBC_LDFLAGS) $(LIBS) $(STDLIB_OBJS) -o $@

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(LIBS)

clean:
	-rm -f $(OBJS) $(ARCHIVES) libc.a
	-find . -type f -name \*.o -delete


