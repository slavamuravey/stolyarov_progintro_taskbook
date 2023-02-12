%include "../include/stud_io.inc"

%macro jmpseq 1-*
    jmp     %%labels_end
%%labels:    
    %rep %0
    dd      %1
        %rotate 1
    %endrep
%%labels_end:
    cmp     eax, 1
    jl      %%quit
    cmp     eax, %0
    jg      %%quit

    jmp     [%%labels + (eax - 1) * 4]
%%quit:
%endmacro

    global  _start

    section .text
_start:
main:
    mov     eax, 2
    jmpseq  label1, label2, label3
    jmp     quit
label1:
    PRINT   "I am label1"
    PUTCHAR 10
    jmp     quit
label2:
    PRINT   "I am label2"
    PUTCHAR 10
    jmp     quit
label3:
    PRINT   "I am label3"
    PUTCHAR 10
    jmp     quit

quit:                                           ; finish the program
    FINISH
