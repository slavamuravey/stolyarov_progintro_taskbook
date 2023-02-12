    global  _start

    section .bss
argc:
    resd    1

    section .data
arg_max_len:
    dd      0
arg_max_len_idx:
    dd      0

    section .text
nl:
    db 10, 0

; input: string address = [ebp + 8]
; output: strlen = eax
strlen:
    push    ebp
    mov     ebp, esp

    push    esi

    mov     esi, [ebp + 8]
    xor     eax, eax
.lp:
    mov     cl, [esi + eax]
    test    cl, cl
    jz      .lp_quit
    inc     eax
    jmp     .lp
.lp_quit:
.quit:
    pop     esi

    mov     esp, ebp
    pop     ebp

    ret

_start:
main:
    mov     ebp, esp
    mov     eax, [esp]
    mov     [argc], eax

    mov     ecx, [argc]
    dec     ecx

    jecxz   lp_quit
lp:
    push    ecx
    mov     esi, [ebp + 4 + 4 * ecx]

    push    esi
    call    strlen
    add     esp, 4

    pop     ecx

    mov     ebx, [arg_max_len]
    cmp     eax, ebx
    jle     if_new_max_len_arg_quit
    mov     [arg_max_len], eax
    mov     [arg_max_len_idx], ecx
if_new_max_len_arg_quit:
    loop    lp

    mov     esi, [arg_max_len_idx]
    
    mov     eax, 4
    mov     ebx, 1
    mov     ecx, [ebp + 4 + 4 * esi]
    mov     edx, [arg_max_len]
    int     0x80

    mov     eax, 4
    mov     ebx, 1
    mov     ecx, nl
    mov     edx, 2
    int     0x80
lp_quit:

 quit:
    mov     eax, 1
    mov     ebx, 0
    int     0x80

