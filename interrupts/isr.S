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

[extern _int_20_timer]
[global int_20_timer]
[global int_20_timer_end]
[global stored_esp]
int_20_timer:
    pushad
    mov eax, esp
    sub eax, 4
    mov [stored_esp], eax
    call dword _int_20_timer
int_20_timer_end:
    popad
    iretd
stored_esp: resd 1


[extern _sys_fork]
[global sys_fork]
[global sys_fork_end]
sys_fork:
    mov eax, esp
    mov [stored_esp], eax
    call _sys_fork
sys_fork_end:
    ret

INT_HANDLER_WRAPPER int_21_keyboard

