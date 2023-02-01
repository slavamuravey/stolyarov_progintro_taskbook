%include "../include/stud_io.inc"

; seq_alloc allocate sequence of 32-bit numbers
; %1 - start number
; %2 - step
; %3 - numbers count
%macro seq_alloc 3
    %assign item %1
    %rep %3
    dd      item
    %assign item item + %2
    %endrep
%endmacro

    global  _start

    section .data
seq:
    seq_alloc 2, 4, 6
seq_end:

    section .text
print_stars:
    push    ebp
    mov     ebp, esp

    mov     ecx, [ebp + 8]

    jecxz   .lp_quit                            
.lp:
    PUTCHAR "*"                                 
    loop    .lp
.lp_quit:
.quit:
    mov     esp, ebp
    pop     ebp

    ret

_start:
main:
    xor     eax, eax
    xor     esi, esi
lp:
    lea     edi, [seq + esi * 4]
    cmp     edi, seq_end
    je      lp_quit
    add     eax, [edi]
    inc     esi
    jmp     lp
lp_quit:

    push    eax
    call    print_stars
    add     esp, 4

    PUTCHAR 10
quit:                                           ; finish the program
    FINISH
