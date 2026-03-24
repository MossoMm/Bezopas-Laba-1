#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>




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

void program3() {
    char input[100];
    srand(time(NULL)); 
    int n = rand();
    int k = rand();
    printf("Введите основание и степень (n k): \n");
    
    
    
    
    printf("Заменяем на %d и %d:\n", n, k);

  


    char *hexResult = powerToHex(n, k);

    if (!hexResult) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    printf("Результат: %s\n", hexResult);
    free(hexResult);
}
