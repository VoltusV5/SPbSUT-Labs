
global main          

section .data     
promptL: db "Введите L (количество элементов >0): ", 0  
promptC: db "Введите C: ", 0                            
promptD: db "Введите D: ", 0                          
msgRes: db "Product = ", 0                              
newline: db 10, 0                                     
errOpen: db "Error: Cannot open file!", 10, 0          
errRead: db "Error: Cannot read file!", 10, 0           
errL: db "Error: L must be >0!", 10, 0               
default_filename: db "input.txt", 0                      ; Имя файла по умолчанию

section .bss           
L: resq 1               ; Переменная L (64-bit, количество)
C: resq 1               ; Переменная C (64-bit, верхняя граница)
D: resq 1               ; Переменная D (64-bit, нижняя граница)
product: resq 1         ; Переменная для произведения
buf: resb 2048          ; Буфер для чтения файла (до 2000 байт + запас)
num: resq 1             ; Временная переменная для текущего числа
filename: resb 256      ; Буфер для имени файла из cmdline (max 255 + \0)

section .text        

; Функция чтения signed 64-bit числа из stdin в RAX
read_int:
    mov rax, 0          ; rax = 0 (sys_read)
    mov rdi, 0          ; rdi = 0 (stdin)
    mov rsi, buf        ; rsi = адрес буфера buf
    mov rdx, 32         ; rdx = 32 (макс. длина строки)
    syscall             ; Выполняем sys_read: читаем строку в buf
    xor rcx, rcx        ; rcx = 0 (флаг знака: 0 = положительный)
    xor rbx, rbx        ; rbx = 0 (накопитель числа)
    mov rsi, buf        ; rsi = начало буфера (buf)
.read_next:
    mov al, [rsi]       ; al = текущий байт из буфера
    cmp al, 10          ; Сравниваем с \n (10)
    je .read_done       ; Если \n, конец чтения
    cmp al, 0           ; Сравниваем с \0
    je .read_done       ; Если \0, конец
    cmp al, '-'         ; Сравниваем с '-'
    jne .read_digit     ; Если не '-', переходим к цифрам
    mov rcx, 1          ; rcx = 1 (флаг отрицательного)
    inc rsi             ; rsi++ (пропускаем '-')
    jmp .read_next      ; Продолжаем цикл
.read_digit:
    cmp al, '0'         ; al < '0'?
    jb .read_done       ; Да, конец (не цифра)
    cmp al, '9'         ; al > '9'?
    ja .read_done       ; Да, конец
    sub al, '0'         ; al -= '0' (преобразуем в цифру 0-9)
    movzx rdx, al       
    imul rbx, rbx, 10   ; rbx *= 10 (накопление)
    add rbx, rdx        ; rbx += rdx (добавляем цифру)
    inc rsi             ; rsi++ (следующий символ)
    jmp .read_next      
.read_done:
    cmp rcx, 0          ; rcx == 0 (положительный)?
    je .read_ret        ; Да, пропускаем neg
    neg rbx             ; rbx = -rbx (для отрицательного)
.read_ret:
    mov rax, rbx        
    ret                

; Функция вывода строки
print_str:
    mov rax, 1          ; rax = 1 (sys_write)
    mov rdi, 1          ; rdi = 1 (stdout)
    mov rdx, 0          ; rdx = 0 (счетчик длины)
.count:
    cmp byte [rsi + rdx], 0  ; [rsi + rdx] == \0?
    je .len_ok         ; Да, длина посчитана
    inc rdx            ; rdx++ (увеличиваем длину)
    jmp .count         
.len_ok:
    mov rsi, rsi        ; rsi = rsi (параметр уже в rsi)
    syscall             ; Выполняем sys_write: выводим строку
    ret               

; Функция вывода signed 64-bit числа из RAX
print_int:
    push rbx            ; Сохраняем rbx 
    mov rbx, 10         ; rbx = 10 (делитель для цифр)
    mov rdi, buf        ; rdi = начало buf
    add rdi, 64         ; rdi = buf + 64 (64 байт)
    mov byte [rdi], 0   ; [rdi] = \0 
    mov rcx, 0          ; rcx = 0 (флаг отрицательного)
    cmp rax, 0          ; rax >= 0?
    jge .loop_start     ; Да, положительное
    neg rax             ; rax = -rax (делаем положительным)
    mov rcx, 1          ; rcx = 1 (флаг минуса)
.loop_start:
.loop:
    xor rdx, rdx        ; rdx = 0 (для div)
    div rbx             ; rax /= 10, остаток в rdx (dl)
    add dl, '0'         ; dl += '0'
    dec rdi             ; rdi-- (сдвиг назад)
    mov [rdi], dl       ; [rdi] = dl (запись цифры)
    test rax, rax       ; rax == 0?
    jnz .loop           ; Нет
    cmp rcx, 0          ; rcx == 0 (не отрицательное)?
    je .done            ; Да
    dec rdi             ; rdi-- (место для '-')
    mov byte [rdi], '-' ; [rdi] = '-'
.done:
    mov rsi, rdi        ; rsi = rdi (начало строки)
    call print_str      ; Вызываем print_str для вывода
    pop rbx             ; Восстанавливаем rbx
    ret                

; Общая функция для вывода ошибки и выхода (использует RSI для сообщения)
print_error_and_exit:
    call print_str      
    mov rax, 60         ; rax = 60 (sys_exit)
    mov rdi, 1          ; rdi = 1 (статус ошибки)
    syscall           

; Главная функция
main:
    pop rdi             ; rdi = argc 
    pop rsi             ; rsi = argv[0]
    cmp rdi, 2          ; argc == 2? 
    jne .use_default    ; Нет
    pop rdi             ; rdi = argv[1] (имя файла)
    mov rsi, filename   ; rsi = filename буфер
.copy_name:
    mov al, [rdi]       ; al = [rdi] (символ из argv[1])
    mov [rsi], al       ; [rsi] = al (копируем)
    cmp al, 0           ; al == \0?
    je .filename_ok     ; Да
    inc rdi             ; rdi++ 
    inc rsi             ; rsi++
    jmp .copy_name      ; Продолжаем
.use_default:
    mov rsi, default_filename  ; rsi = "input.txt"
    mov rdi, filename   ; rdi = filename буфер
.copy_default:
    mov al, [rsi]       ; al = [rsi]
    mov [rdi], al       ; [rdi] = al
    cmp al, 0           ; al == \0?
    je .filename_ok     ; Да
    inc rsi             ; rsi++
    inc rdi             ; rdi++
    jmp .copy_default   ; Продолжаем
.filename_ok:

    ; ===== Ввод L =====
    mov rsi, promptL    
    call print_str      
    call read_int       ; Читаем L в rax
    mov [L], rax        ; [L] = rax
    cmp rax, 0          ; L <= 0?
    jle error_l         ; Да

    ; ===== Ввод C =====
    mov rsi, promptC    
    call print_str      
    call read_int      
    mov [C], rax        

    ; ===== Ввод D =====
    mov rsi, promptD  
    call print_str     
    call read_int     
    mov [D], rax     

    ; ===== Открываем файл =====
    mov rax, 2          ; rax = 2 (sys_open)
    mov rdi, filename   ; rdi = имя файла
    mov rsi, 0          ; rsi = 0 
    xor rdx, rdx        ; rdx = 0 
    syscall             ; Выполняем open
    cmp rax, 0          ; rax < 0 (ошибка)?
    jl error_open       ; Да
    mov r12, rax        ; r12 = fd (file descriptor)

    ; ===== Читаем файл =====
    mov rax, 0          ; rax = 0 (sys_read)
    mov rdi, r12        ; rdi = fd
    mov rsi, buf        ; rsi = buf
    mov rdx, 2047       ; rdx = 2047 (макс. размер <2000 + \0)
    syscall             ; Выполняем read
    cmp rax, 0          ; rax <= 0 (ошибка/пусто)?
    jle error_read      ; Да, ошибка
    mov r13, rax        ; r13 = количество байт прочитано
    mov byte [buf + r13], 0  ; Добавляем \0 в конец буфера

    ; ===== Закрываем файл =====
    mov rax, 3          ; rax = 3 (sys_close)
    mov rdi, r12        ; rdi = fd
    syscall             ; Выполняем close

    ; ===== Инициализация обработки =====
    mov rbx, 1          ; rbx = 1 (product init)
    mov [product], rbx  ; [product] = 1
    xor r14, r14        ; r14 = 0 (счетчик отрицательных в range, "counter_neg")
    mov rcx, 0          ; rcx = 0 
parse_loop:
    cmp rcx, r13        ; rcx >= r13 (конец буфера)?
    jge done_parse      ; Да
    mov al, [buf + rcx] ; al = [buf + rcx] (текущий символ)
    cmp al, 0           ; al == \0?
    je done_parse       ; Да, конец
    cmp al, 10          ; al == \n?
    je next_char        ; Да, пропуск
    cmp al, ' '         ; al == ' '?
    je next_char        ; Да, разделитель
    cmp al, ','         ; al == ','?
    je next_char        ; Да, разделитель

    ; Начинаем парсинг числа (если цифра или '-')
    mov rdx, rcx        ; rdx = rcx (начало числа)
    mov rsi, buf        ; rsi = buf
    add rsi, rdx        ; rsi += rdx (указатель на текущий символ)
    xor rdi, rdi        ; rdi = 0 (флаг знака: 0=положительный)
    mov rbx, 0          ; rbx = 0 (накопитель числа)
    cmp byte [rsi], '-' ; [rsi] == '-'?
    jne parse_num       ; Нет, парсинг
    mov rdi, 1          ; rdi = 1 (отрицательный)
    inc rsi             ; rsi++ (пропуск '-')
parse_num:
    mov al, [rsi]       ; al = [rsi] (символ)
    cmp al, '0'         ; al < '0'?
    jb end_num          ; Да, конец числа
    cmp al, '9'         ; al > '9'?
    ja end_num          ; Да, конец
    sub al, '0'         ; al -= '0' (цифра)
    movzx rax, al       ; rax = al (zero-extend)
    imul rbx, rbx, 10   ; rbx *= 10
    add rbx, rax        ; rbx += rax (добавляем цифру)
    inc rsi             ; rsi++ (следующий)
    jmp parse_num       
end_num:
    cmp rdi, 0          ; rdi == 0 (положительный)?
    je num_ok           ; Да
    neg rbx             ; rbx = -rbx
num_ok:
    mov [num], rbx     

    ; ===== Проверка условия: отрицательное, в range, первые L =====
    mov rbx, [num]      ; rbx = num
    cmp rbx, 0          ; rbx >= 0 (не отрицательное)?
    jge skip_mult       ; Да
    mov rax, [C]        ; rax = C
    cmp rbx, rax        ; rbx <= C?
    jg skip_mult        ; Нет (C >= a[i] не выполнено)
    mov rax, [D]        ; rax = D
    cmp rbx, rax        ; rbx >= D?
    jl skip_mult        ; Нет (a[i] >= D не выполнено)
    ; Условие выполнено: отрицательное в [D, C]
    cmp r14, [L]        ; r14 >= L (уже L штук)?
    jge skip_mult       ; Да, пропускаем (не первые L)
    mov rax, [product]  ; rax = product
    imul rax, rbx       ; rax *= rbx (умножаем)
    mov [product], rax  ; [product] = rax
    inc r14             ; r14++ (счетчик++)
skip_mult:
    ; Сдвигаем индекс на конец числа
    mov rax, rsi        ; rax = rsi (конец числа)
    sub rax, buf        ; rax -= buf (относительный индекс)
    mov rcx, rax        ; rcx = rax (обновляем индекс)
    jmp parse_loop      ; Продолжаем цикл

next_char:
    inc rcx             ; rcx++ (пропуск разделителя)
    jmp parse_loop      ; Продолжаем

done_parse:
    ; ===== Вывод результата =====
    mov rsi, msgRes     ; rsi = "Product = "
    call print_str      ; Выводим
    mov rax, [product]  ; rax = product
    call print_int      ; Печатаем число
    mov rsi, newline    ; rsi = \n
    call print_str      ; Выводим новую строку

    ; ===== Нормальный выход =====
    mov rax, 60         ; rax = 60 (sys_exit)
    xor rdi, rdi        ; rdi = 0 (статус успеха)
    syscall             ; Выполняем exit

; ===== Обработчики ошибок (глобальные метки) =====
error_l:
    mov rsi, errL       ; rsi = сообщение об ошибке L
    call print_error_and_exit  ; Выводим ошибку и выходим

error_open:
    mov rsi, errOpen    ; rsi = сообщение об ошибке открытия
    call print_error_and_exit  ; Выводим ошибку и выходим

error_read:
    mov rsi, errRead    ; rsi = сообщение об ошибке чтения
    call print_error_and_exit  ; Выводим ошибку и выходим