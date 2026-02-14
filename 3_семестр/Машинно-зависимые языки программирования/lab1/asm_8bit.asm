section .data
    extern a_8, b_8, d_8
    extern num_8_asm, den_8_asm, res_8_asm

section .text
    global calc_8bit
calc_8bit:
    ; Вычисление числителя: num_8_asm = 1 + (7 * b_8) / 2
    movsx eax, byte [b_8] ; Расширяем b_8 до 32 бит
    mov ebx, 7             ; ebx = 7
    imul eax, ebx          ; eax = 7 * b_8 (знаковое умножение)
    sar eax, 1             ; eax = (7 * b_8) / 2 (арифметический сдвиг)
    add eax, 1             ; eax = 1 + (7 * b_8) / 2
    mov [num_8_asm], eax  ; Сохраняем числитель (32 бита)

    ; Вычисление знаменателя: den_8_asm = a_8 + (b_8 * 2) / d_8
    movsx eax, byte [b_8] ; Расширяем b_8 до 32 бит
    mov ebx, 2             ; ebx = 2
    imul eax, ebx          ; eax = b_8 * 2
    movsx ebx, byte [d_8] ; Расширяем d_8 до 32 бит
    cdq                    ; Расширяем eax до edx:eax для знакового деления
    idiv ebx               ; eax = (b_8 * 2) / d_8
    movsx ebx, byte [a_8] ; Расширяем a_8 до 32 бит
    add eax, ebx           ; eax = a_8 + (b_8 * 2) / d_8
    mov [den_8_asm], eax  ; Сохраняем знаменатель (32 бита)

    ; Вычисление результата: res_8_asm = num_8_asm / den_8_asm
    mov eax, [num_8_asm]  ; eax = числитель
    mov ebx, [den_8_asm]  ; ebx = знаменатель
    cdq                    ; Расширяем eax до edx:eax
    idiv ebx               ; eax = num_8_asm / den_8_asm
    mov [res_8_asm], eax  ; Сохраняем результат (32 бита)
    ret