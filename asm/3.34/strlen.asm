    global  strlen

    section .text

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
