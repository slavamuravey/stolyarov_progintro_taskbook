    global  sys_exit
    global  sys_read
    global  sys_write
    global  sys_open
    global  sys_close

    section .text
syscall_1:
    push    ebp
    mov     ebp, esp
    push    ebx

    mov     ebx, [ebp + 8]
    int     0x80

    mov     edx, eax
    and     edx, 0xfffff000
    cmp     edx, 0xfffff000
    jnz     .quit
    mov     [sys_errno], eax
    mov     eax, -1
.quit:
    pop     ebx
    mov     esp, ebp
    pop     ebp

    ret

syscall_3:
    push    ebp
    mov     ebp, esp
    push    ebx

    mov     ebx, [ebp + 8]
    mov     ecx, [ebp + 12]
    mov     edx, [ebp + 16]
    int     0x80
    
    mov     edx, eax
    and     edx, 0xfffff000
    cmp     edx, 0xfffff000
    jnz     .quit
    mov     [sys_errno], eax
    mov     eax, -1
.quit:
    pop     ebx
    mov     esp, ebp
    pop     ebp

    ret
    
sys_exit:
    mov     eax, 1
    jmp     syscall_1

sys_read:
    mov     eax, 3
    jmp     syscall_3

sys_write:
    mov     eax, 4
    jmp     syscall_3

sys_open:
    mov     eax, 5
    jmp     syscall_3

sys_close:
    mov     eax, 6
    jmp     syscall_1

    section .bss
sys_errno:
    resb    1
