    global  strchar
    extern  strlen

    section .text

; input: string address = [ebp + 8], index = [ebp + 12]
; output: char = al, al = -1 if error
strchar:
    push    ebp
    mov     ebp, esp

    push    esi
    push    edi

    mov     esi, [ebp + 8]
    mov     edi, [ebp + 12]

    push    esi
    call    strlen
    add     esp, 4

    cmp     edi, 0
    jl      .error_quit

    mov     ecx, eax
    sub     ecx, edi
    cmp     ecx, 1
    jl      .error_quit

    xor     eax, eax
    mov     al, [esi + edi]

    jmp     .quit
.error_quit:
    xor     eax, eax
    mov     al, -1
.quit:
    pop     edi
    pop     esi

    mov     esp, ebp
    pop     ebp

    ret