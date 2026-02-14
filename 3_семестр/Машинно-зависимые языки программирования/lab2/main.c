#include <stdio.h>
#include <stdint.h>

// Глобальные переменные для знаковых 16-битных вычислений
signed short int a_signed, b_signed;
long result_signed_c, result_signed_asm;

// Глобальные переменные для беззнаковых 16-битных вычислений
unsigned short int a_unsigned, b_unsigned;
long result_unsigned_c, result_unsigned_asm;

// Прототипы ассемблерных функций
extern void calc_signed();
extern void calc_unsigned();

// Проверка ввода для знаковых 16-битных данных
int read_int16_signed(const char* prompt, signed short int* value) {
    int temp;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &temp) != 1) {
            printf("Ошибка ввода! ");
            while (getchar() != '\n');
            continue;
        }
        if (temp >= -32768 && temp <= 32767) {
            *value = (signed short int)temp;
            return 1;
        } else {
            printf("Число должно быть от -32768 до 32767. ");
        }
    }
}

// Проверка ввода для беззнаковых 16-битных данных
int read_int16_unsigned(const char* prompt, unsigned short int* value) {
    int temp;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &temp) != 1) {
            printf("Ошибка ввода! ");
            while (getchar() != '\n');
            continue;
        }
        if (temp >= 0 && temp <= 65535) {
            *value = (unsigned short int)temp;
            return 1;
        } else {
            printf("Число должно быть от 0 до 65535. ");
        }
    }
}

int main() {
    // Ввод для знаковых 16-битных данных
    printf("Enter values for signed 16-bit (signed short int, range -32768 to 32767):\n");
    read_int16_signed("a_signed = ", &a_signed);
    read_int16_signed("b_signed = ", &b_signed);

    // Вычисления на C для знаковых 16 бит
    if (a_signed > b_signed) {
        result_signed_c = ((long)a_signed - b_signed) / b_signed;
    } else if (a_signed == b_signed) {
        result_signed_c = 256;
    } else {
        result_signed_c = (long)a_signed / b_signed + 5;
    }

    // Вызов ассемблерной функции для знаковых 16 бит
    calc_signed();

    // Вывод результатов для знаковых 16 бит
    printf("\nSigned 16-bit Results (C):\n");
    printf("Result: %ld\n", result_signed_c);
    printf("\nSigned 16-bit Results (ASM):\n");
    printf("Result: %ld\n", result_signed_asm);

    // Ввод для беззнаковых 16-битных данных
    printf("\nEnter values for unsigned 16-bit (unsigned short int, range 0 to 65535):\n");
    read_int16_unsigned("a_unsigned = ", &a_unsigned);
    read_int16_unsigned("b_unsigned = ", &b_unsigned);

    // Вычисления на C для беззнаковых 16 бит
    if (a_unsigned > b_unsigned) {
        result_unsigned_c = ((long)a_unsigned - b_unsigned) / b_unsigned;
    } else if (a_unsigned == b_unsigned) {
        result_unsigned_c = 256;
    } else {
        result_unsigned_c = (long)a_unsigned / b_unsigned + 5;
    }

    // Вызов ассемблерной функции для беззнаковых 16 бит
    calc_unsigned();

    // Вывод результатов для беззнаковых 16 бит
    printf("\nUnsigned 16-bit Results (C):\n");
    printf("Result: %ld\n", result_unsigned_c);
    printf("\nUnsigned 16-bit Results (ASM):\n");
    printf("Result: %ld\n", result_unsigned_asm);

    return 0;
}