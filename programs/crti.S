[extern main]

[global _start]
_start:
pushfd
pushad

call main

popad
popfd
ret

