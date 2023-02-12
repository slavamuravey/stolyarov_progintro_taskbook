%include "../include/stud_io.inc"

%ifidn SIZE, b
    %define dx db
%elifidn SIZE, w
    %define dx dw
%elifidn SIZE, d
    %define dx dd
%elifidn SIZE, q
    %define dx dq
%else
    %define dx db
%endif

%macro string_alloc 1
    %strlen string_len %1
    %assign index 1
    %rep string_len
        %substr char %1 index
    dx      char
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
