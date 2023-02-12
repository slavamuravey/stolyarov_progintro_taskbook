%include "../include/stud_io.inc"
    global  itoa
    section .text

; input: number value = [ebp + 8], result number string address = [ebp + 12]
itoa:
    push    ebp
    mov     ebp, esp

    push    ebx
    push    edi

    mov     eax, [ebp + 8]
    mov     ecx, [ebp + 12]

    mov     edi, 10
.clear_string_lp:
    mov     [ecx + edi], byte 0
    dec     edi
    jnz     .clear_string_lp
    xor     edi, edi
.read_digits_lp:
    xor     edx, edx                                
    mov     ebx, 10                                 
    div     ebx                                     
    add     edx, "0"
    push    edx   
    inc     edi                                  
    test    eax, eax            
    jnz     .read_digits_lp
.fill_string_lp:
    pop     eax
    mov     [ecx], al                               
    inc     ecx
    dec     edi
    jnz     .fill_string_lp
.quit:
    pop     edi
    pop     ebx

    mov     esp, ebp
    pop     ebp

    ret
