    global  _start
    extern  main

    section .text
_start:
    mov     ecx, [esp]
    mov     eax, esp
    add     eax, 4
    
    push    eax
    push    ecx
    call    main
    add     esp, 8
    
    mov     edx, eax
    
    mov     eax, 1
    mov     ebx, edx 
    int     0x80
