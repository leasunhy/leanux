[extern main]

[global _start]
_start:
;pushfd
;pushad

call main

mov ebx, eax
mov eax, 1
int 0x80

;popad
;popfd
;ret

