section .data
    extern a_16, b_16, d_16
    extern num_16_asm, den_16_asm, res_16_asm

section .text
    global calc_16bit
calc_16bit:
    ; Вычисление числителя: num_16_asm = 1 + (7 * b_16) / 2
    movzx eax, word [b_16] ; Расширяем b_16 до 32 бит
    mov ebx, 7              ; ebx = 7
    mul ebx                 ; edx:eax = 7 * b_16 (беззнаковое умножение)
    shr edx, 1              ; Делим старшую часть на 2
    rcr eax, 1              ; Делим младшую часть с учётом переноса
    add eax, 1              ; eax = 1 + (7 * b_16) / 2
    mov [num_16_asm], eax  ; Сохраняем числитель (32 бита)

    ; Вычисление знаменателя: den_16_asm = a_16 + (b_16 * 2) / d_16
    movzx eax, word [b_16] ; Расширяем b_16 до 32 бит
    mov ebx, 2              ; ebx = 2
    mul ebx                 ; edx:eax = b_16 * 2
    movzx ebx, word [d_16] ; Расширяем d_16 до 32 бит
    xor edx, edx            ; Обнуляем edx для беззнакового деления
    div ebx                 ; eax = (b_16 * 2) / d_16
    movzx ebx, word [a_16] ; Расширяем a_16 до 32 бит
    add eax, ebx            ; eax = a_16 + (b_16 * 2) / d_16
    mov [den_16_asm], eax  ; Сохраняем знаменатель (32 бита)

    ; Вычисление результата: res_16_asm = num_16_asm / den_16_asm
    mov eax, [num_16_asm]  ; eax = числитель
    mov ebx, [den_16_asm]  ; ebx = знаменатель
    xor edx, edx            ; Обнуляем edx для беззнакового деления
    div ebx                 ; eax = num_16_asm / den_16_asm
    mov [res_16_asm], eax  ; Сохраняем результат (32 бита)
    ret