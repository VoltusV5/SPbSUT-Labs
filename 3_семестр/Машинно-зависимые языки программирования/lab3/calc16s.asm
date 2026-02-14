global calc16s

extern a, b, res

section .text
calc16s:
    mov ax, [a]     ; Загружаем a в AX
    mov bx, [b]     ; Загружаем b в BX
    cmp ax, bx      ; Сравниваем AX и BX
    jl less_branch  ; Если a < b 
    jg greater_branch ; Если a > b
    je equal_branch ; Если ==

less_branch:    ; a < b -> a / b + 5
    mov ax, [a]     ; a в AX
    cwd             ; Расширяем знак AX в DX:AX
    mov bx, [b]     ; b в BX
    idiv bx         ; Signed деление DX:AX / BX,
    add ax, 5       ; +5
    mov [res], ax   ; Сохраняем
    ret

greater_branch: ; a > b -> (a - b) / b
    mov ax, [a]     ; a в AX
    sub ax, [b]     ; a - b
    cwd             ; Расширяем знак
    mov bx, [b]     ; b в BX
    idiv bx         ; Деление
    mov [res], ax   ; Сохраняем
    ret

equal_branch:   ; == -> 256
    mov ax, 256     ; 256 в AX
    mov [res], ax   ; Сохраняем
    ret