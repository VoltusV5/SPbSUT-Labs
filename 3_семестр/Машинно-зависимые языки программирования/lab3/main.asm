
global main       
extern calc16s, calc16

global a, b, res   

section .data      
a: dw 0            ; Переменная a: 0
b: dw 0           
res: dw 0          


msg_signed: db "Enter a signed number (-32768...32767): ", 0x0a  
len_signed: dd $-msg_signed                                      ; Длина промпта (dd для 32-бит)

msg_unsigned: db "Enter an unsigned number (0...65535): ", 0x0a  
len_unsigned: dd $-msg_unsigned                               

; Сообщения ошибок и результатов
msg_err: db "Out of range!", 0x0a                              
len_err: dd $-msg_err                                        
msg_signed_res: db "Signed result = ", 0                      
len_signed_res: dd $-msg_signed_res                         
msg_unsigned_res: db "Unsigned result = ", 0                
len_unsigned_res: dd $-msg_unsigned_res                      
newline: db 0x0a                                                 ; Символ новой строки

section .bss       
buf: resb 32       ; Буфер для чтения ввода (32 байта)

section .text      
main:              
    ; Блок для signed вычислений
    call read_signed_a    
    call read_signed_b    
    call calc16s     
    mov eax, 4              ; sys_write (eax=4)
    mov ebx, 1              ; stdout (ebx=1)
    mov ecx, msg_signed_res ; Адрес заголовка
    mov edx, [len_signed_res] ; Длина заголовка
    int 80h                 ; Выполняем syscall для вывода заголовка
    call print_result       ; Вызываем вывод результата

    ; Блок для unsigned вычислений
    call read_unsigned_a    
    call read_unsigned_b    
    call calc16             
    mov eax, 4              ; sys_write
    mov ebx, 1              ; stdout
    mov ecx, msg_unsigned_res ; Адрес заголовка
    mov edx, [len_unsigned_res] ; Длина
    int 80h                 ; Вывод заголовка
    call print_result       ; Вывод результата

    ; Завершение программы
    mov eax, 1              ; sys_exit (eax=1)
    xor ebx, ebx            ; Статус 0 (ebx=0)
    int 80h                 ; Выполняем syscall для выхода
    ret                  

; Функция чтения строки из stdin
read_line:
    mov eax, 3              ; sys_read (eax=3)
    mov ebx, 0              ; stdin (ebx=0)
    mov ecx, buf            ; Адрес буфера
    mov edx, 31             ; Максимум 31 байт (оставляем место для \0)
    int 80h                 ; Выполняем чтение
    test eax, eax           ; Проверяем длину (eax=0?)
    jz .zero_len            ; Если 0, переходим к обработке пустой строки
    mov byte [buf + eax - 1], 0  ; Добавляем \0 в конец строки
    ret                    
.zero_len:
    mov byte [buf], 0       ; Если пусто, буфер = \0
    ret                  

; Чтение signed числа в a (с повтором при ошибке)
read_signed_a:
    mov ecx, a              ; ECX = адрес для записи (a)
.repeat_a:
    mov eax, 4              ; sys_write
    mov ebx, 1              ; stdout
    mov ecx, msg_signed     ; Промпт
    mov edx, [len_signed]   ; Длина
    int 80h                 ; Вывод промпта
    call read_number_signed 
    jc .repeat_a            ; Если CF=1 (ошибка), повтор
    mov [a], ax             ; Сохраняем в a
    ret                   

; Чтение signed числа в b (аналогично a)
read_signed_b:
    mov ecx, b              ; ECX = адрес для записи (b)
.repeat_b:
    mov eax, 4              ; sys_write
    mov ebx, 1
    mov ecx, msg_signed     
    mov edx, [len_signed]
    int 80h
    call read_number_signed 
    jc .repeat_b            ; Повтор при ошибке
    mov [b], ax             ; Сохраняем в b
    ret

; Парсинг signed числа (возвращает в AX, CF=1 при ошибке)
read_number_signed:
    call read_line          ; Читаем строку в buf
    mov esi, buf            ; ESI = начало буфера
    xor eax, eax            ; EAX = 0 (накопитель числа, 32-бит)
    mov bl, 1               ; BL = 1 (знак положительный)

    ; Проверка на '-'
    mov cl, [esi]           ; CL = первый символ
    cmp cl, '-'             ; Сравниваем с '-'
    jne .parse              ; Если не '-', переходим к парсингу
    mov bl, -1              ; Устанавливаем знак отрицательный
    inc esi                 ; Сдвигаем указатель после '-'

.parse:
    mov cl, [esi]           ; CL = текущий символ
    cmp cl, 0               ; Конец строки?
    je .apply_sign          ; Да, применяем знак
    cmp cl, '0'             ; < '0'?
    jb .error               ; Да, ошибка
    cmp cl, '9'             ; > '9'?
    ja .error               ; Да, ошибка
    sub cl, '0'             ; CL = цифра (0-9)
    movzx edx, cl           ; EDX = цифра (расширяем до 32-бит)
    imul eax, eax, 10       ; EAX *= 10 (накопление)
    add eax, edx            ; EAX += цифра
    inc esi                 ; Следующий символ
    jmp .parse              ; Повтор

.apply_sign:
    cmp bl, 1               ; Знак положительный?
    je .skip_sign           ; Да, пропускаем
    neg eax                 ; EAX = -EAX (для отрицательного)

.skip_sign:
    ; Проверка диапазона signed 16-bit
    cmp eax, 32767          ; > 32767?
    jg .error               ; Да, ошибка
    cmp eax, -32768         ; < -32768?
    jl .error               ; Да, ошибка
.ok:
    mov ax, ax              ; AX = младшие 16 бит EAX (для возврата)
    clc                     ; CF=0
    ret
.error:
    mov eax, 4              ; sys_write
    mov ebx, 1
    mov ecx, msg_err        
    mov edx, [len_err]      
    int 80h                 ; Вывод ошибки
    stc                     ; CF=1 (ошибка)
    ret

; Чтение unsigned в a
read_unsigned_a:
    mov ecx, a              ; ECX = адрес a
.repeat_ua:
    mov eax, 4              ; sys_write
    mov ebx, 1
    mov ecx, msg_unsigned   
    mov edx, [len_unsigned]
    int 80h
    call read_number_unsigned 
    jc .repeat_ua           ; Повтор при ошибке
    mov [a], ax             
    ret

; Чтение unsigned в b
read_unsigned_b:
    mov ecx, b              ; ECX = адрес b
.repeat_ub:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_unsigned
    mov edx, [len_unsigned]
    int 80h
    call read_number_unsigned
    jc .repeat_ub
    mov [b], ax
    ret

; Парсинг unsigned
read_number_unsigned:
    call read_line
    mov esi, buf
    xor eax, eax            ; EAX = 0

.parse_unsigned:
    mov cl, [esi]
    cmp cl, 0
    je .ok_unsigned
    cmp cl, '0'
    jb .error_unsigned
    cmp cl, '9'
    ja .error_unsigned
    sub cl, '0'
    movzx edx, cl
    imul eax, eax, 10
    add eax, edx
    inc esi
    jmp .parse_unsigned

.ok_unsigned:
    cmp eax, 65535          ; > 65535?
    ja .error_unsigned      ; Да, ошибка
    clc                     ; CF=0
    ret
.error_unsigned:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_err
    mov edx, [len_err]
    int 80h
    stc
    ret

; Вывод результата (использует print_num)
print_result:
    call print_num          ; Вызываем функцию печати числа
    ret                   

; Функция печати 16-битного числа из res (с учетом знака)
print_num:
    mov ax, [res]           ; Загружаем res в AX (16-бит)
    call print16            ; Вызываем вспомогательную функцию
    ret                     ; Возврат

; Вспомогательная: печать 16-битного числа (с обработкой знака)
print16:
    mov edi, buf + 6        ; EDI = конец буфера (buf+6 для 7 байт: 5 цифр + '-' + \n)
    mov byte [edi], 0x0a    ; Добавляем newline в конец
    mov ecx, 1              ; ECX = 1 (счетчик символов)
    mov bx, 10              ; BX = 10 (основа для деления)

    test ax, ax             ; Проверяем знак (AX < 0?)
    jns .positive           ; Если >=0, положительное
    mov si, -1              ; SI = -1 (флаг отрицательного)
    neg ax                  ; AX = -AX (делаем положительным для обработки)
    jmp .next_digit         ; Переходим к цифрам

.positive:
    mov si, 1               ; SI = 1 (положительное)

.next_digit:
    xor dx, dx              ; DX = 0 (для div)
    div bx                  ; Делим AX на 10, остаток в DL
    add dl, '0'             ; DL += '0' 
    dec edi                 ; EDI-- (сдвиг назад в буфер)
    mov [edi], dl           ; Сохраняем цифру в буфер
    inc ecx                 ; ECX++ (счетчик символов)
    test ax, ax             ; AX == 0?
    jnz .next_digit         ; Нет, продолжаем

    cmp si, -1              ; Отрицательное?
    jne .write              ; Нет, выводим
    dec edi                 ; EDI-- для '-'
    mov byte [edi], '-'     ; Добавляем '-'
    inc ecx                 ; ECX++ 

.write:
    mov eax, 4              ; sys_write
    mov ebx, 1              ; stdout
    mov edx, ecx            ; Длина строки
    mov ecx, edi            ; Адрес начала строки
    int 80h                 ; Выводим
    ret                    