    global  _start

    section .bss
exit_code:
    resd    1

    section .text
EXIT_CODE_OK: equ 0
EXIT_CODE_ERROR: equ 1

_start:
main:
    mov     ecx, [esp]
    cmp     ecx, 4                              ; cmd name + 3 args
    jne     if_args_ok_else
    mov     [exit_code], dword EXIT_CODE_OK
    jmp     if_args_ok_quit
if_args_ok_else:
    mov     [exit_code], dword EXIT_CODE_ERROR
if_args_ok_quit:
    mov     eax, 1
    mov     ebx, [exit_code]
    int     0x80