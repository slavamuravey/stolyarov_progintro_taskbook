%include "../include/stud_io.inc"

%macro string_alloc 1
    %strlen string_len %1
    %assign index 1
    %rep string_len
        %substr char %1 index
    dd      char
        %assign index index + 1
    %endrep
%endmacro

    global  _start

    section .text
_start:
main:
    jmp     string_end
string:
    string_alloc "qwe"
string_end:
quit:                                           ; finish the program
    FINISH
