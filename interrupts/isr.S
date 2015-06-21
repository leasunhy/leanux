[BITS 32]

%macro INT_HANDLER_WRAPPER 1
[extern _%1]
[global %1]
%1:
    pushad
    call dword _%1
    popad
    iretd
%endmacro


[global int_null]
int_null:
    iretd


[global int_null_0]
int_null_0:
    pushad
    mov al, 0x20
    out 0x20, al
    popad
    iretd


[global int_null_1]
int_null_1:
    pushad
    mov al, 0x20
    out 0xA0, al
    out 0x20, al
    popad
    iretd

[extern _int_page_fault]
[global int_page_fault]
int_page_fault:
    mov eax, cr2
    push eax
    call int_page_fault
    iretd

; since eax is used to return the result, can't just pushad/popad
[extern _syscall]
[global syscall]
syscall:
    push ecx
    push edx
    push ebx
    push esi
    push edi
    call _syscall
    pop edi
    pop esi
    pop ebx
    pop edx
    pop ecx
    iretd


INT_HANDLER_WRAPPER int_20_timer
INT_HANDLER_WRAPPER int_21_keyboard

