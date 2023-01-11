%include "../include/stud_io.inc"
    global  _start

    section .data
words_count:
    dd      0                                   ; define initial words count
word_detected:
    db      FALSE                               ; define initial word detected flag

    section .text

; define constants
FALSE:      equ  0
TRUE:       equ  0xff

print_stars_ecx:                                ; print stars; count of stars is stored in ecx register
    jecxz   .lp_quit                            ; if the result is zero, skip star printing loop
.lp:
    PUTCHAR "*"                                 ; print stars, using ecx register and loop construction
    loop    .lp
.lp_quit:
    ret

_start:
main:
read_line_lp:
    GETCHAR                                     ; read char
    cmp     eax, -1
    je      read_line_lp_quit                   ; quit loop if EOF
    cmp     al, 10
    jne     if_end_line_else

    mov     ecx, [words_count]                  ; print words count stars
    call    print_stars_ecx
    PUTCHAR 10

    mov     [words_count], dword 0              ; drop the value for the new line
    mov     [word_detected], byte FALSE         ; set to false, because we have not detected a word yet on a new line
    
    jmp     if_end_line_quit                    ; read next char
if_end_line_else:
    cmp     al, " "                             ; check end of a word
    jne     if_end_word_else
    mov     [word_detected], byte FALSE         ; if word ends, set word detected flag to false
    jmp     if_end_word_quit
if_end_word_else:
    mov     ah, [word_detected]                 ; if we met a word
    cmp     ah, FALSE
    jne     if_word_detected_quit
    inc     dword [words_count]                 ; increase counter
if_word_detected_quit:
    mov     [word_detected], byte TRUE          ; and set flag to true
if_end_word_quit:
if_end_line_quit:
    jmp     read_line_lp                        ; read next char
read_line_lp_quit:    
quit:                                           ; finish the program
    FINISH