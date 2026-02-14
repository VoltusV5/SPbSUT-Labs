#include <stdio.h>
#include <stdint.h>

// Глобальные переменные для 8-битных вычислений
signed char a_8, b_8, d_8;
long num_8_c, den_8_c, res_8_c;
long num_8_asm, den_8_asm, res_8_asm;

// Глобальные переменные для 16-битных вычислений
unsigned short int a_16, b_16, d_16;
long num_16_c, den_16_c, res_16_c;
long num_16_asm, den_16_asm, res_16_asm;

// Прототипы ассемблерных функций
extern void calc_8bit();
extern void calc_16bit();

// Проверка ввода для 8-битных данных
int read_int8(const char* prompt, signed char* value) {
    int temp;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &temp) != 1) {
            printf("Ошибка ввода! ");
            while (getchar() != '\n');
            continue;
        }
        if (temp >= -128 && temp <= 127) {
            *value = (signed char)temp;
            return 1;
        } else {
            printf("Число должно быть от -128 до 127. ");
        }
    }
}

// Проверка ввода для 16-битных данных
int read_int16(const char* prompt, unsigned short int* value) {
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
    // Ввод для 8-битных данных
    printf("Enter values for 8-bit (signed char, range -128 to 127):\n");
    read_int8("a_8 = ", &a_8);
    read_int8("b_8 = ", &b_8);
    read_int8("d_8 = ", &d_8);

    // Вычисления на C для 8 бит
    num_8_c = 1 + (7 * (long)b_8) / 2;
    den_8_c = (long)a_8 + ((long)b_8 * 2) / d_8;
    res_8_c = num_8_c / den_8_c;

    // Вызов ассемблерной функции для 8 бит
    calc_8bit();

    // Вывод результатов для 8 бит
    printf("\n8-bit Results (C):\n");
    printf("Numerator: %ld\n", num_8_c);
    printf("Denominator: %ld\n", den_8_c);
    printf("Result: %ld\n", res_8_c);
    printf("\n8-bit Results (ASM):\n");
    printf("Numerator: %ld\n", num_8_asm);
    printf("Denominator: %ld\n", den_8_asm);
    printf("Result: %ld\n", res_8_asm);

    // Ввод для 16-битных данных
    printf("\nEnter values for 16-bit (unsigned short int, range 0 to 65535):\n");
    read_int16("a_16 = ", &a_16);
    read_int16("b_16 = ", &b_16);
    read_int16("d_16 = ", &d_16);

    // Вычисления на C для 16 бит
    num_16_c = 1 + (7 * (long)b_16) / 2;
    den_16_c = (long)a_16 + ((long)b_16 * 2) / d_16;
    res_16_c = num_16_c / den_16_c;

    // Вызов ассемблерной функции для 16 бит
    calc_16bit();

    // Вывод результатов для 16 бит
    printf("\n16-bit Results (C):\n");
    printf("Numerator: %ld\n", num_16_c);
    printf("Denominator: %ld\n", den_16_c);
    printf("Result: %ld\n", res_16_c);
    printf("\n16-bit Results (ASM):\n");
    printf("Numerator: %ld\n", num_16_asm);
    printf("Denominator: %ld\n", den_16_asm);
    printf("Result: %ld\n", res_16_asm);

    return 0;
}