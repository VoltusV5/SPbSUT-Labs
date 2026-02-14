section .data
    extern a_unsigned, b_unsigned
    extern result_unsigned_asm

section .text
    global calc_unsigned
calc_unsigned:

    push ebp
    mov ebp, esp
    push ebx

    ; Сравнение a_unsigned и b_unsigned
    movzx eax, word [a_unsigned] ; Расширяем a_unsigned до 32 бит
    movzx ebx, word [b_unsigned] ; Расширяем b_unsigned до 32 бит
    cmp eax, ebx                 ; Сравниваем a_unsigned и b_unsigned
    ja above                     ; Переход, если a_unsigned > b_unsigned
    je equal                     ; Переход, если a_unsigned = b_unsigned
    jmp below                    ; Переход, если a_unsigned < b_unsigned

above:
    ; Вычисление (a_unsigned - b_unsigned) / b_unsigned
    movzx eax, word [a_unsigned] ; eax = a_unsigned
    movzx ebx, word [b_unsigned] ; ebx = b_unsigned
    sub eax, ebx                 ; eax = a_unsigned - b_unsigned
    xor edx, edx                 ; Обнуляем edx для беззнакового деления
    div ebx                      ; eax = (a_unsigned - b_unsigned) / b_unsigned
    mov [result_unsigned_asm], eax ; Сохраняем результат
    
    pop ebx
    mov esp, ebp
    pop ebp
    
    ret

equal:
    ; Результат = 256
    mov eax, 256
    mov [result_unsigned_asm], eax ; Сохраняем результат
    
    pop ebx
    mov esp, ebp
    pop ebp
    
    ret

below:
    ; Вычисление a_unsigned / b_unsigned + 5
    movzx eax, word [a_unsigned] ; eax = a_unsigned
    movzx ebx, word [b_unsigned] ; ebx = b_unsigned
    xor edx, edx                 ; Обнуляем edx для беззнакового деления
    div ebx                      ; eax = a_unsigned / b_unsigned
    add eax, 5                   ; eax = a_unsigned / b_unsigned + 5
    mov [result_unsigned_asm], eax ; Сохраняем результат
    
    pop ebx
    mov esp, ebp
    pop ebp
    
    ret
    
    
    
section .note.GNU-stack noalloc noexec nowrite progbits