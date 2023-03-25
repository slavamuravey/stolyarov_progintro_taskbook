    global  _start

    section .bss
argc:       
    resd    1
argv:
    resd    1

    section .text
nlstr:
    db      10, 0

point_a:
    db      "a)", 10
point_a_len: equ $ - point_a

point_b:
    db      "b)", 10
point_b_len: equ $ - point_b

point_c:
    db      "c)", 10
point_c_len: equ $ - point_c

; input: string address = [ebp + 8]
; output: strlen = eax
strlen:
    push    ebp
    mov     ebp, esp

    mov     ecx, [ebp + 8]
    mov     eax, ecx

.lp:
    cmp     byte [eax], 0
    je      .lp_quit
    inc     eax
    jmp     short .lp
.lp_quit:
    sub     eax, ecx 

.quit:
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

matches_count:
    push    ebp
    mov     ebp, esp

    push    ebx

    mov     ecx, [ebp + 8]
    mov     edx, [ebp + 12]
    xor     ebx, ebx

.lp:
    cmp     byte [ecx], 0
    je      .lp_quit
    
    push    dword [ecx]
    call    edx
    add     esp, 4

    test    eax, eax
    jz      .if_match_else
    inc     ebx
    jmp     .if_match_quit
.if_match_else:
.if_match_quit:
    inc     ecx
    jmp     short .lp
.lp_quit:
    mov     eax, ebx

.quit:
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret

is_capital_letter:
    push    ebp
    mov     ebp, esp

    push    ebx

    xor     eax, eax
    mov     bl, [ebp + 8]
    cmp     bl, 'A'
    jl      .quit
    cmp     bl, 'Z'
    jg      .quit
    mov     eax, 1
.quit:
    pop     ebx

    mov     esp, ebp
    pop     ebp
    
    ret

is_lowercase_letter:
    push    ebp
    mov     ebp, esp

    push    ebx

    xor     eax, eax
    mov     bl, [ebp + 8]
    cmp     bl, 'a'
    jl      .quit
    cmp     bl, 'z'
    jg      .quit
    mov     eax, 1
.quit:
    pop     ebx

    mov     esp, ebp
    pop     ebp
    
    ret

is_at_char:
    push    ebp
    mov     ebp, esp

    push    dword '@'
    push    dword [ebp + 8]
    call    is_a_char
    add     esp, 8

    mov     esp, ebp
    pop     ebp    
    
    ret

is_dot_char:
    push    ebp
    mov     ebp, esp

    push    dword '.'
    push    dword [ebp + 8]
    call    is_a_char
    add     esp, 8

    mov     esp, ebp
    pop     ebp    
    
    ret

is_a_char:
    push    ebp
    mov     ebp, esp

    push    ebx

    xor     eax, eax

    mov     ebx, [ebp + 12]
    cmp     byte [ebp + 8], bl
    jne     .quit
    mov     eax, 1
.quit:
    pop     ebx

    mov     esp, ebp
    pop     ebp    
    
    ret

print_str:
    push    ebp
    mov     ebp, esp

    push    ebx

    mov     ebx, [ebp + 8]

    push    ebx
    call    strlen
    add     esp, 4

    mov     ecx, ebx
    mov     edx, eax
    mov     eax, 4
    mov     ebx, 1
    int     0x80

    pop     ebx

.quit:
    mov     esp, ebp
    pop     ebp

    ret

filter_a:
    push    ebp
    mov     ebp, esp

    push    ebx

    xor     ebx, ebx

    push    is_capital_letter
    push    dword [ebp + 8]
    call    matches_count
    add     esp, 8

    test    eax, eax
    jz      .quit
    mov     ebx, 1
.quit:
    mov     eax, ebx
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret

filter_b:
    push    ebp
    mov     ebp, esp

    push    ebx

    xor     ebx, ebx

    push    is_at_char
    push    dword [ebp + 8]
    call    matches_count
    add     esp, 8

    cmp     eax, 1
    jne     .quit

    push    is_dot_char
    push    dword [ebp + 8]
    call    matches_count
    add     esp, 8

    test    eax, eax
    jz      .quit

    mov     ebx, 1

.quit:
    mov     eax, ebx
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret

filter_c:
    push    ebp
    mov     ebp, esp

    push    ebx

    xor     ebx, ebx

    push    dword 0
    push    dword [ebp + 8]
    call    strchar
    add     esp, 8

    push    eax
    call    is_capital_letter
    add     esp, 4

    test    eax, eax
    jz      .quit

    push    dword [ebp + 8]
    call    strlen
    add     esp, 4

    dec     eax
    push    eax
    push    dword [ebp + 8]
    call    strchar
    add     esp, 8

    push    eax
    call    is_lowercase_letter
    add     esp, 4

    test    eax, eax
    jz      .quit

    mov     ebx, 1
.quit:
    mov     eax, ebx
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret

print_args:
    push    ebp
    mov     ebp, esp

    push    ebx
    push    esi

    mov     ebx, [argc]
    mov     esi, [argv]
    add     esi, 4
    dec     ebx
    jz      .lp_quit
.lp:
    push    dword [esi]
    call    [ebp + 8]
    add     esp, 4

    test    eax, eax
    jz      .lp_continue

    push    dword [esi]
    call    print_str
    add     esp, 4

    push    nlstr
    call    print_str
    add     esp, 4
    
.lp_continue:
    add     esi, 4
    dec     ebx
    jnz     .lp
.lp_quit:
.quit:
    pop     esi
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret

_start:
main:
    pop     dword [argc]
    mov     [argv], esp

    mov     eax, 4
    mov     ebx, 1
    mov     ecx, point_a
    mov     edx, point_a_len
    int     0x80

    push    filter_a
    call    print_args
    add     esp, 4

    mov     eax, 4
    mov     ebx, 1
    mov     ecx, point_b
    mov     edx, point_b_len
    int     0x80

    push    filter_b
    call    print_args
    add     esp, 4

    mov     eax, 4
    mov     ebx, 1
    mov     ecx, point_c
    mov     edx, point_c_len
    int     0x80

    push    filter_c
    call    print_args
    add     esp, 4

quit:
    mov     eax, 1
    mov     ebx, 0
    int     0x80

