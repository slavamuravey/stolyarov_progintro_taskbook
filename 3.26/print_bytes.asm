%include "../include/stud_io.inc"
    global  print_bytes

    section .text

; input: string address = [ebp + 8]
print_bytes:
    push    ebp
    mov     ebp, esp

    mov     eax, [ebp + 8]
.lp:
    mov     bl, [eax]
    test    bl, bl
    jz      .lp_quit
    PUTCHAR [eax]
    inc     eax
    jmp     .lp
.lp_quit:
.quit:
    mov     esp, ebp
    pop     ebp

    ret
