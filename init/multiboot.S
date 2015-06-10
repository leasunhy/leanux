# This file is use to wrap the kernel into a multiboot compatible format.

MBALIGN    equ 1<<0
MEMINFO    equ 1<<1
FLAGS      equ MBALIGN | MEMINFO
MAGIC      equ 0x1BADB002
CHECKSUM   equ -(MAGIC + FLAGS)


section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .bootstrap_stack, nobits
align 4
stack_bottom:
resb 16384
stack_top:


section .descriptors
align 4
global gdt, idt, gdtr, idtr
gdt:
    dq 0  ; 8 bytes empty
gdt_kcode:
    ; kernel code
    dw 0xffff  ; limit[15:0]
    dw 0       ; base[15:0]
    db 0       ; base[23:16]
    db 0x9A    ; flags
    db 0xCF    ; flags
    db 0       ; base[31:24]
gdt_kdata:
    ; kernel data
    dw 0xffff  ; limit[15:0]
    dw 0       ; base[15:0]
    db 0       ; base[23:16]
    db 0x92    ; flags
    db 0xCF    ; flags
    db 0       ; base[31:24]
gdt_ucode:
    ; user code
    dq 0x00CFFA000000FFFF
gdt_udata:
    ; user data
    dq 0x00CFF2000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt

CODE_SEG equ gdt_kcode - gdt
DATA_SEG equ gdt_kdata - gdt

section .text:
global _start
_start:
    mov esp, stack_top

    ; disable interrupts
    cli

    ; load gdt
    lgdt [gdt_descriptor]

    ; fast A20 gate
    in al, 0x92
    or al, 2
    out 0x92, al

    ; set pe bit to 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; make an epic jump...!
    jmp CODE_SEG:pm_init

[bits 32]
pm_init:

    ; set all the data segment registers...
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    extern kernel_main
    call kernel_main

    cli
.hang
    hlt
    jmp .hang
