#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



// Проверка корректности всех вводимых данных
int validate_input(int n, int k, const char *input_str) {
    // Проверка, что в строке только цифры и пробелы
    for (int i = 0; input_str[i] != '\0'; i++) {
        if (!isdigit(input_str[i]) && input_str[i] != ' ' &&
            input_str[i] != '\n' && input_str[i] != '\t' &&
            !(input_str[i] == '-' && (i == 0 || input_str[i-1] == ' '))) {
            printf("Ошибка: введены некорректные символы. Разрешены только цифры, пробелы и знак минус.\n");
            return 0;
        }
    }

    // Проверка основания (n должно быть целым числом больше 1)
    if (n <= 1) {
        printf("Ошибка: основание должно быть целым числом больше 1.\n");
        return 0;
    }

    // Проверка степени (k должно быть целым неотрицательным числом)
    if (k < 0) {
        printf("Ошибка: степень должна быть целым неотрицательным числом.\n");
        return 0;
    }

    return 1;
}


// Функция для преобразования шестнадцатеричного символа в число
int hexCharToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0; // Некорректный символ
}

// Функция для преобразования числа в шестнадцатеричный символ
char valueToHexChar(int v) {
    if (v < 10) return '0' + v;
    return 'A' + (v - 10);
}

// Умножает шестнадцатеричную строку на целое число
void multiplyHexStringByInt(char *result, int n) {
    int carry = 0;
    int len = strlen(result);

    // Идем по цифрам справа налево
    for (int i = len - 1; i >= 0; i--) {
        // Получаем числовое значение текущей цифры
        int digit = hexCharToValue(result[i]);

        // Умножаем на n и добавляем перенос
        int product = digit * n + carry;

        // Новая цифра для этой позиции
        int newDigit = product % 16;
        // Перенос для следующего разряда
        carry = product / 16;

        // Записываем новую цифру
        result[i] = valueToHexChar(newDigit);
    }

    // Если остался перенос, добавляем его как новую старшую цифру
    while (carry > 0) {
        // Сдвигаем строку вправо
        memmove(result + 1, result, len + 1);

        // Добавляем цифру переноса
        int newDigit = carry % 16;
        result[0] = valueToHexChar(newDigit);
        carry = carry / 16;
        len++; // Длина строки увеличилась
    }
}

// Возведение числа n в степень k в шестнадцатеричной системе
char *powerToHex(int n, int k) {
    // Выделяем память для результата
    char *result = (char *)malloc(1000000 * sizeof(char));
    if (!result) return NULL;

    // Начинаем с 1
    strcpy(result, "1");

    // Умножаем k раз
    for (int i = 0; i < k; i++) {
        multiplyHexStringByInt(result, n);
    }

    return result;
}
void program3()
{
    int n, k;
    char input[100];

    printf("Введите основание и степень (n k): ");

    // Читаем всю строку ввода
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Ошибка ввода!\n");
        return 1;
    }

    // Пытаемся преобразовать в числа
    if (sscanf(input, "%d %d", &n, &k) != 2) {
        printf("Ошибка: необходимо ввести два целых числа через пробел.\n");
        return 1;
    }

    // Проверка корректности ввода (включая проверку символов)
    if (!validate_input(n, k, input)) {
        return 1;
    }

    char *hexResult = powerToHex(n, k);

    if (!hexResult) {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    printf("Результат: %s\n", hexResult);
    free(hexResult);

    return 0;
}
int main() {
    program3();
    return 0;
}
