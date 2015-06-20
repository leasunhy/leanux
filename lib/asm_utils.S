;;=============================================================================
;; ATA read sectors (LBA mode)
;;
;; @param EAX Logical Block Address of sector
;; @param CL  Number of sectors to read
;; @param EDI The address of buffer to put data obtained from disk
;;
;; @return None
;;=============================================================================
;global read_disk
;read_disk:
;push edi

;mov eax, [esp+8]
;mov ecx, [esp+12]
;mov edi, [esp+16]

;pushfd
;and eax, 0x0FFFFFFF
;push eax
;push ebx
;push ecx
;push edx
;push edi

;mov ebx, eax         ; Save LBA in EBX

;mov edx, 0x01f6      ; Port to send drive and bit 24 - 27 of LBA
;shr eax, 24          ; Get bit 24 - 27 in al
;or al, 11100000b     ; Set bit 6 in al for LBA mode
;out dx, al

;mov edx, 0x01f2      ; Port to send number of sectors
;mov al, cl           ; Get number of sectors from CL
;out dx, al

;mov edx, 0x1f3       ; Port to send bit 0 - 7 of LBA
;mov eax, ebx         ; Get LBA from EBX
;out dx, al

;mov edx, 0x1f4       ; Port to send bit 8 - 15 of LBA
;mov eax, ebx         ; Get LBA from EBX
;shr eax, 8           ; Get bit 8 - 15 in AL
;out dx, al


;mov edx, 0x1f5       ; Port to send bit 16 - 23 of LBA
;mov eax, ebx         ; Get LBA from EBX
;shr eax, 16          ; Get bit 16 - 23 in AL
;out dx, al

;mov edx, 0x1f7       ; Command port
;mov al, 0x20         ; Read with retry.
;out dx, al

;.still_going:  in al, dx
;test al, 8           ; the sector buffer requires servicing.
;jz .still_going      ; until the sector buffer is ready.

;mov eax, 256         ; to read 256 words = 1 sector
;xor bx, bx
;mov bl, cl           ; read CL sectors
;mul bx
;mov ecx, eax         ; ECX is counter for INSW
;mov edx, 0x1f0       ; Data port, in and out
;rep insw             ; in to [EDI]

;pop edi
;pop edx
;pop ecx
;pop ebx
;pop eax
;popfd

;pop edi

;ret

; this routine is from the book

[global read_a_sector]
read_a_sector:                        ;从硬盘读取一个逻辑扇区
                                         ;EAX=逻辑扇区号
                                         ;DS:EBX=目标缓冲区地址
                                         ;返回：EBX=EBX+512
         push ebx
         push eax
         push ecx
         push edx
         pushfd

         mov eax, [esp+24]
         mov ebx, [esp+28]

         push eax

         mov dx,0x1f2
         mov al,1
         out dx,al                       ;读取的扇区数

         inc dx                          ;0x1f3
         pop eax
         out dx,al                       ;LBA地址7~0

         inc dx                          ;0x1f4
         mov cl,8
         shr eax,cl
         out dx,al                       ;LBA地址15~8

         inc dx                          ;0x1f5
         shr eax,cl
         out dx,al                       ;LBA地址23~16

         inc dx                          ;0x1f6
         shr eax,cl
         or al,0xe0                      ;第一硬盘  LBA地址27~24
         out dx,al

         inc dx                          ;0x1f7
         mov al,0x20                     ;读命令
         out dx,al

  .waits:
         in al,dx
         and al,0x88
         cmp al,0x08
         jnz .waits                      ;不忙，且硬盘已准备好数据传输 

         mov ecx,256                     ;总共要读取的字数
         mov dx,0x1f0
  .readw:
         in ax,dx
         mov [ebx],ax
         add ebx,2
         loop .readw

         popfd
         pop edx
         pop ecx
         pop eax

         pop ebx

         ret


