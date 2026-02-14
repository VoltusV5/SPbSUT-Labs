; lab5.asm
; Вычисление выражения (1 + 7*b / 2) / (a + b*2 / d) с использованием сопроцессора x87
; a, b - вещественные (float), d - целое (int)
section .data
    ; Сообщения для ввода/вывода
    msg_enter_eq db "(1+7*b/2)/(a+b*2/d)", 10, 0
    msg_enter_a db "Enter a (float): ", 0
    msg_enter_b db "Enter b (float): ", 0
    msg_enter_d db "Enter d (int): ", 0
    msg_nominator db "Nominator: ", 0
    msg_denominator db "Denominator: ", 0
    msg_result db "Result: ", 0
    msg_error_input db "Error: Invalid input!", 10, 0
    msg_fpu_calc db "=== FPU Calculation ===", 10, 0
    msg_asm_calc db "Assembly calculation with FPU:", 10, 0
    msg_float_format db "%.6f", 10, 0 ; Формат для вывода float
   
    newline db 10, 0
   
    ; Константы для вычислений
    const_1 dd 1.0
    const_7 dd 7.0
    const_2 dd 2.0
section .bss
    ; Переменные для хранения введенных значений
    a resd 1 ; float
    b resd 1 ; float
    d resd 1 ; int
   
    ; Результаты вычислений
    nominator resd 1
    denominator resd 1
    result resd 1
   
    ; Буферы для ввода/вывода
    input_buffer resb 32
    temp_buffer resb 32
    int_part resd 1
    frac_part resd 1
    sign resb 1
    frac_divisor resq 1
section .text
    global _start
    extern printf ; Используем printf из libc для вывода float
; ========== СИСТЕМНЫЕ ФУНКЦИИ ==========
; Вывод строки (Linux x86-64)
; rdi - указатель на строку
print_string:
    push rbp
    mov rbp, rsp
    push rsi
    push rdx
   
    mov rsi, rdi ; указатель на строку
    mov rdx, 0
   
.count_loop:
    cmp byte [rsi+rdx], 0
    je .count_done
    inc rdx
    jmp .count_loop
   
.count_done:
    mov rax, 1 ; sys_write
    mov rdi, 1 ; stdout
    syscall
   
    pop rdx
    pop rsi
    pop rbp
    ret
; Ввод строки (Linux x86-64)
; rdi - буфер для ввода
; rsi - размер буфера
input_string:
    push rbp
    mov rbp, rsp
    push rsi
    push rdx
   
    mov rax, 0 ; sys_read
    mov rdx, rsi ; размер буфера
    mov rsi, rdi ; буфер
    mov rdi, 0 ; stdin
    syscall
   
    ; Заменяем символ новой строки на нулевой
    mov rcx, 0
.find_newline:
    cmp byte [rsi+rcx], 10
    je .replace_null
    cmp byte [rsi+rcx], 0
    je .done_input
    inc rcx
    jmp .find_newline
.replace_null:
    mov byte [rsi+rcx], 0
.done_input:
    pop rdx
    pop rsi
    pop rbp
    ret
; ========== ФУНКЦИИ ДЛЯ ВВОДА ВЕЩЕСТВЕННЫХ ЧИСЕЛ ==========
; Преобразование строки в float
; rdi - указатель на строку
; возвращает xmm0 = число, rdx = флаг ошибки (0 - ок, -1 - ошибка)
string_to_float:
    push rbp
    mov rbp, rsp
    push rbx
    push rcx
    push rsi
   
    ; Инициализация
    xorps xmm0, xmm0
    xorps xmm1, xmm1
    mov dword [int_part], 0
    mov dword [frac_part], 0
    mov byte [sign], 1 ; 1 = положительный, -1 = отрицательный
    mov qword [frac_divisor], 1 ; делитель для дробной части
   
    ; Проверка знака
    cmp byte [rdi], '-'
    jne .check_plus
    mov byte [sign], -1
    inc rdi
    jmp .parse_int_part
   
.check_plus:
    cmp byte [rdi], '+'
    jne .parse_int_part
    inc rdi
   
.parse_int_part:
    mov cl, byte [rdi]
    cmp cl, 0
    je .combine_parts
    cmp cl, '.'
    je .parse_frac_part
   
    ; Проверка цифры
    cmp cl, '0'
    jb .float_error
    cmp cl, '9'
    ja .float_error
   
    sub cl, '0'
   
    ; Умножение на 10 и добавление цифры
    mov eax, dword [int_part]
    imul eax, 10
    jo .float_error
    add eax, ecx
    mov dword [int_part], eax
   
    inc rdi
    jmp .parse_int_part
.parse_frac_part:
    inc rdi ; пропускаем точку
   
.frac_loop:
    mov al, byte [rdi]
    cmp al, 0
    je .combine_parts
   
    ; Проверка цифры
    cmp al, '0'
    jb .float_error
    cmp al, '9'
    ja .float_error
   
    sub al, '0'
   
    ; Добавляем к дробной части
    mov ebx, dword [frac_part]
    imul ebx, 10
    jo .float_error
    add ebx, eax
    mov dword [frac_part], ebx
   
    ; Увеличиваем делитель
    mov rax, qword [frac_divisor]
    imul rax, 10
    mov qword [frac_divisor], rax
   
    inc rdi
    jmp .frac_loop
.combine_parts:
    ; Преобразуем целую часть в float
    cvtsi2ss xmm0, dword [int_part]
   
    ; Добавляем дробную часть
    cvtsi2ss xmm1, dword [frac_part]
    cvtsi2ss xmm2, qword [frac_divisor]
    divss xmm1, xmm2
    addss xmm0, xmm1
   
    ; Применяем знак
    cmp byte [sign], -1
    jne .float_success
    movss xmm1, xmm0
    xorps xmm0, xmm0
    subss xmm0, xmm1
.float_success:
    xor rdx, rdx ; флаг успеха
    jmp .float_exit
.float_error:
    xorps xmm0, xmm0
    mov rdx, -1 ; флаг ошибки
.float_exit:
    pop rsi
    pop rcx
    pop rbx
    pop rbp
    ret
; Ввод вещественного числа
; возвращает xmm0 = число
input_float:
    push rbp
    mov rbp, rsp
   
.input_float_loop:
    ; Ввод строки
    mov rdi, input_buffer
    mov rsi, 32
    call input_string
   
    ; Преобразование в float
    mov rdi, input_buffer
    call string_to_float
   
    ; Проверка ошибки
    cmp rdx, -1
    jne .input_float_done
   
    ; Ошибка ввода
    mov rdi, msg_error_input
    call print_string
    jmp .input_float_loop
   
.input_float_done:
    pop rbp
    ret
; ========== ФУНКЦИИ ДЛЯ ВВОДА ЦЕЛЫХ ЧИСЕЛ ==========
; Преобразование строки в int (32-bit)
; rdi - указатель на строку
; возвращает eax = число, rdx = флаг ошибки (0 - ок, -1 - ошибка)
string_to_int:
    push rbp
    mov rbp, rsp
    push rbx
    push rcx
   
    xor rax, rax
    xor rcx, rcx
    mov rdx, 1 ; знак (1 = положительный, -1 = отрицательный)
   
    ; Проверка знака
    cmp byte [rdi], '-'
    jne .check_plus_int
    mov rdx, -1
    inc rdi
    jmp .parse_loop_int
   
.check_plus_int:
    cmp byte [rdi], '+'
    jne .parse_loop_int
    inc rdi
   
.parse_loop_int:
    mov cl, byte [rdi]
    cmp cl, 0
    je .int_done
   
    ; Проверка цифры
    cmp cl, '0'
    jb .int_error
    cmp cl, '9'
    ja .int_error
   
    sub cl, '0'
   
    ; Умножение на 10 и добавление цифры
    mov rbx, 10
    imul rax, rbx
    add rax, rcx
   
    inc rdi
    jmp .parse_loop_int
   
.int_done:
    imul rax, rdx ; применяем знак
    xor rdx, rdx ; флаг успеха
    jmp .int_exit
   
.int_error:
    xor rax, rax
    mov rdx, -1 ; флаг ошибки
   
.int_exit:
    pop rcx
    pop rbx
    pop rbp
    ret
; Ввод целого числа
; возвращает eax = число
input_int:
    push rbp
    mov rbp, rsp
   
.input_int_loop:
    ; Ввод строки
    mov rdi, input_buffer
    mov rsi, 16
    call input_string
   
    ; Преобразование в число
    mov rdi, input_buffer
    call string_to_int
   
    ; Проверка ошибки
    cmp rdx, -1
    jne .input_int_done
   
    ; Ошибка ввода
    mov rdi, msg_error_input
    call print_string
    jmp .input_int_loop
   
.input_int_done:
    pop rbp
    ret
; ========== ФУНКЦИИ ДЛЯ ВЫВОДА ==========
; Преобразование int в строку (32-bit)
; edi - число для конверсии
; rsi - буфер для результата
int_to_string:
    push rbp
    mov rbp, rsp
    push rbx
    push rcx
    push r8
   
    mov eax, edi ; число
    mov rcx, rsi ; буфер
   
    ; Проверка знака
    test eax, eax
    jns .positive_int
    neg eax
    mov byte [rsi], '-'
    inc rsi
   
.positive_int:
    mov ebx, 10
    mov r8, 0 ; счетчик цифр
   
    ; Сохраняем цифры в стек
.digit_loop_int:
    xor edx, edx
    div ebx
    add dl, '0'
    push rdx
    inc r8
    test eax, eax
    jnz .digit_loop_int
   
    ; Извлекаем цифры из стека
.pop_loop_int:
    pop rax
    mov [rsi], al
    inc rsi
    dec r8
    jnz .pop_loop_int
   
    mov byte [rsi], 0 ; нулевой терминатор
   
    pop r8
    pop rcx
    pop rbx
    pop rbp
    ret
; Вывод целого числа
; edi - число для вывода
print_int:
    push rbp
    mov rbp, rsp
   
    mov rsi, temp_buffer
    call int_to_string
   
    mov rdi, temp_buffer
    call print_string
   
    pop rbp
    ret
; Вывод float числа с использованием printf
; xmm0 - число для вывода
print_float:
    push rbp
    mov rbp, rsp
   
    ; Сохраняем xmm0 в стеке (выравнивание 16 байт)
    sub rsp, 16
    movss [rsp], xmm0
    movss xmm0, [rsp]
    cvtss2sd xmm0, xmm0 ; Конвертируем float в double для printf
   
    ; Вызываем printf
    mov rdi, msg_float_format
    mov rax, 1 ; количество векторных аргументов
    call printf
   
    add rsp, 16
    pop rbp
    ret
; ========== ФУНКЦИИ С СОПРОЦЕССОРОМ ==========
; Вычисление числителя: 1 + 7*b / 2
; Параметры: b (float) через стек
; Возврат: результат в ST(0)
numerator_proc:
    push rbp
    mov rbp, rsp
   
    fld dword [rbp + 16] ; ST(0) = b
    fmul dword [const_7] ; ST(0) = 7*b
    fdiv dword [const_2] ; ST(0) = 7*b / 2
    fadd dword [const_1] ; ST(0) = 1 + 7*b / 2
   
    pop rbp
    ret

; Вычисление знаменателя: a + b*2 / d
; Параметры: a (float), b (float), d (int) через стек
; Возврат: результат в ST(0)
denominator_proc:
    push rbp
    mov rbp, rsp
   
    fld dword [rbp + 24] ; ST(0) = b
    fmul dword [const_2] ; ST(0) = b*2
    fild dword [rbp + 32] ; ST(0) = d, ST(1) = b*2
    fdivp st1, st0 ; ST(0) = b*2 / d
    fld dword [rbp + 16] ; ST(0) = a, ST(1) = b*2 / d
    faddp st1, st0 ; ST(0) = a + b*2 / d
   
    pop rbp
    ret

; Вычисление результата: num / denom
; Параметры: num (float), denom (float) через стек
; Возврат: результат в ST(0)
result_proc:
    push rbp
    mov rbp, rsp
   
    fld dword [rbp + 16] ; ST(0) = num
    fld dword [rbp + 24] ; ST(0) = denom, ST(1) = num
    fdivp st1, st0 ; ST(0) = num / denom
   
    pop rbp
    ret
; ========== ГЛАВНАЯ ПРОГРАММА ==========
_start:
    ; Вывод уравнения в самом начале программы
    mov rdi, msg_enter_eq
    call print_string
   
    mov rdi, newline
    call print_string
   
    ; Вывод заголовка
    mov rdi, msg_fpu_calc
    call print_string
   
    mov rdi, msg_asm_calc
    call print_string
   
    ; Ввод значения a (float)
    mov rdi, msg_enter_a
    call print_string
    call input_float
    movss dword [a], xmm0
   
    ; Ввод значения b (float)
    mov rdi, msg_enter_b
    call print_string
    call input_float
    movss dword [b], xmm0
   
    ; Ввод значения d (int)
    mov rdi, msg_enter_d
    call print_string
    call input_int
    mov dword [d], eax
   
    ; Инициализация сопроцессора
    finit
   
    ; Вычисление числителя
    sub rsp, 8
    movss xmm0, dword [b]
    movss [rsp], xmm0
    call numerator_proc
    fstp dword [nominator]
    add rsp, 8
   
    ; Вычисление знаменателя
    sub rsp, 24
    movss xmm0, dword [a]
    movss [rsp], xmm0
    movss xmm0, dword [b]
    movss [rsp + 8], xmm0
    mov eax, dword [d]
    mov [rsp + 16], eax
    call denominator_proc
    fstp dword [denominator]
    add rsp, 24
   
    ; Вычисление результата
    sub rsp, 16
    movss xmm0, dword [nominator]
    movss [rsp], xmm0
    movss xmm0, dword [denominator]
    movss [rsp + 8], xmm0
    call result_proc
    fstp dword [result]
    add rsp, 16
   
    ; Вывод результатов
   
    ; Числитель
    mov rdi, msg_nominator
    call print_string
    movss xmm0, dword [nominator]
    call print_float
    mov rdi, newline
    call print_string
   
    ; Знаменатель
    mov rdi, msg_denominator
    call print_string
    movss xmm0, dword [denominator]
    call print_float
    mov rdi, newline
    call print_string
   
    ; Результат
    mov rdi, msg_result
    call print_string
    movss xmm0, dword [result]
    call print_float
    mov rdi, newline
    call print_string
   
    ; Завершение программы
    mov rax, 60 ; sys_exit
    xor rdi, rdi ; код возврата 0
    syscall