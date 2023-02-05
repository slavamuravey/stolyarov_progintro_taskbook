    global  _start

%macro exit 1
    mov     eax, 1
    mov     ebx, %1
    int     0x80
%endmacro

%macro exit_ok 0
    exit    0
%endmacro

%macro exit_error 0
    exit    1
%endmacro

    section .bss
argc:
    resd    1
arg1_len:
    resd    1
arg1_last_char:
    resb    1
arg2_len:
    resd    1
arg2_last_char:
    resb    1

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

_start:
main:
    mov     ebp, esp
    mov     eax, [esp]
    mov     [argc], eax
 
    mov     ecx, [argc]
    cmp     ecx, 3                              ; cmd name + 2 args
    jne     if_args_count_ok_else

    push    dword [ebp + 8]
    call    strlen
    add     esp, 4
    mov     [arg1_len], eax

    push    dword [ebp + 12]
    call    strlen
    add     esp, 4
    mov     [arg2_len], eax

    mov     eax, [arg1_len]
    mov     ecx, [arg2_len]
    cmp     eax, ecx
    jne     if_args_len_equ_else

    mov     ecx, [arg1_len]
    dec     ecx
    push    ecx
    push    dword [ebp + 8]
    call    strchar
    add     esp, 8
    mov     [arg1_last_char], al

    mov     ecx, [arg2_len]
    dec     ecx
    push    ecx
    push    dword [ebp + 12]
    call    strchar
    add     esp, 8
    mov     [arg2_last_char], al

    mov     al, [arg1_last_char]
    mov     cl, [arg2_last_char]
    cmp     al, cl
    jne     if_args_last_char_equ_else
    exit_ok
if_args_last_char_equ_else:
    exit_error
if_args_len_equ_else:
    exit_error
if_args_len_equ_quit:

    exit_ok
if_args_count_ok_else:
    exit_error
if_args_count_ok_quit:

