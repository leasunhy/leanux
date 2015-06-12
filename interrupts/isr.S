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


