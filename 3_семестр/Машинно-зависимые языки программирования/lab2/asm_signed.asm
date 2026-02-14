section .data
    extern a_signed, b_signed
    extern result_signed_asm

section .text
    global calc_signed
calc_signed:
    push ebp
    mov ebp, esp
    push ebx


    ; Сравнение a_signed и b_signed
    movsx eax, word [a_signed] ; Расширяем a_signed до 32 бит
    movsx ebx, word [b_signed] ; Расширяем b_signed до 32 бит
    cmp eax, ebx               ; Сравниваем a_signed и b_signed
    jg greater                 ; Переход, если a_signed > b_signed
    je equal                   ; Переход, если a_signed = b_signed
    jmp less                   ; Переход, если a_signed < b_signed

greater:
    ; Вычисление (a_signed - b_signed) / b_signed
    movsx eax, word [a_signed] ; eax = a_signed
    movsx ebx, word [b_signed] ; ebx = b_signed
    sub eax, ebx               ; eax = a_signed - b_signed
    cdq                        ; Расширяем eax до edx:eax для знакового деления
    idiv ebx                   ; eax = (a_signed - b_signed) / b_signed
    mov [result_signed_asm], eax ; Сохраняем результат
    
    pop ebx
    mov esp, ebp
    pop ebp
    
    ret

equal:
    ; Результат = 256
    mov eax, 256
    mov [result_signed_asm], eax ; Сохраняем результат
    
    pop ebx
    mov esp, ebp
    pop ebp
    
    ret

less:
    ; Вычисление a_signed / b_signed + 5
    movsx eax, word [a_signed] ; eax = a_signed
    movsx ebx, word [b_signed] ; ebx = b_signed
    cdq                        ; Расширяем eax до edx:eax для знакового деления
    idiv ebx                   ; eax = a_signed / b_signed
    add eax, 5                 ; eax = a_signed / b_signed + 5
    mov [result_signed_asm], eax ; Сохраняем результат
    
    pop ebx
    mov esp, ebp
    pop ebp
    
    ret
    
    
section .note.GNU-stack noalloc noexec nowrite progbits