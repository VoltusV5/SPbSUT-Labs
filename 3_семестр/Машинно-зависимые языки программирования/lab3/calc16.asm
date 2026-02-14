
global calc16  ; Экспорт функции calc16 

extern a, b, res  ; Глобальные переменные из main.asm

section .text  
calc16:        
    mov ax, [a]     ; a в регистр AX
    mov bx, [b]     ; b в регистр BX 
    cmp ax, bx      ; Сравниваем a с b
    jb less_branch  ; Если a < b
    ja greater_branch ; Если a > b
    je equal_branch ; Если a == b

less_branch:   ; Ветка: a < b -> результат = a / b + 5
    mov ax, [a]     ; a в AX
    xor dx, dx      ; Обнуляем DX
    mov bx, [b]     ; Загружаем b в BX
    div bx          ; Делим AX:DX на BX
    add ax, 5       ; Добавляем 5 к результату
    mov [res], ax   ; Сохраняем результат в res
    ret             

greater_branch:; Ветка: a > b -> результат = (a - b) / b
    mov ax, [a]     ; a в AX
    sub ax, [b]     ; Вычитаем b из a
    xor dx, dx      ; Обнуляем DX 
    mov bx, [b]     ; b в BX
    div bx          ; Делим AX:DX на BX
    mov [res], ax   ; Сохраняем результат в res
    ret            

equal_branch:  ; Ветка: a == b -> результат = 256
    mov ax, 256     ; Загружаем константу 256 в AX
    mov [res], ax   ; Сохраняем в res
    ret        