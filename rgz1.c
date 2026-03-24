#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_RESULT_SIZE 1000000
#define MAX_N 100      //  Максимальное значение основания
#define MAX_K 50       // Максимальное значение степени

// Функция для преобразования шестнадцатеричного символа в число
int hexCharToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

// Функция для преобразования числа в шестнадцатеричный символ
char valueToHexChar(int v) {
    if (v < 10) return '0' + v;
    return 'A' + (v - 10);
}

// Умножает шестнадцатеричную строку на целое число
void multiplyHexStringByInt(char *result, int n, int maxSize) {
    int carry = 0;
    int len = strlen(result);

    for (int i = len - 1; i >= 0; i--) {
        int digit = hexCharToValue(result[i]);
        int product = digit * n + carry;
        int newDigit = product % 16;
        carry = product / 16;
        result[i] = valueToHexChar(newDigit);
    }

    while (carry > 0) {
        if (len >= maxSize - 1) {
            printf("Ошибка: переполнение буфера результата!\n");
            return;
        }
        memmove(result + 1, result, len + 1);
        result[0] = valueToHexChar(carry % 16);
        carry /= 16;
        len++;
    }
}

// Возведение числа n в степень k в шестнадцатеричной системе
char *powerToHex(int n, int k) {
    char *result = (char *)malloc(MAX_RESULT_SIZE * sizeof(char));
    if (!result) return NULL;

    strcpy(result, "1");

    for (int i = 0; i < k; i++) {
        multiplyHexStringByInt(result, n, MAX_RESULT_SIZE);
    }

    return result;
}

void program3() {
    //  Инициализация генератора случайных чисел
    srand((unsigned int)time(NULL));
    
    //  Генерация случайных валидных значений
    //    n: от 2 до MAX_N (основание > 1)
    //    k: от 0 до MAX_K (степень >= 0)
    int n = (rand() % (MAX_N - 1)) + 2;  // [2, MAX_N]
    int k = rand() % (MAX_K + 1);         // [0, MAX_K]
    
    // Вывод сгенерированных параметров для отладки
    printf("Сгенерированные значения: n = %d, k = %d\n", n, k);
    printf("Вычисляем %d^%d в шестнадцатеричной системе...\n", n, k);
    
    //  Вычисление результата
    char *hexResult = powerToHex(n, k);
    if (!hexResult) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    printf("Результат: %s (в шестнадцатеричной системе)\n", hexResult);
    
    // . Проверка: вывод в десятичной системе для верификации (опционально)
    if (k <= 20) {  // Чтобы не переполнить double при больших степенях
        long long decimalResult = 1;
        for (int i = 0; i < k; i++) decimalResult *= n;
        printf("Проверка (десятичная): %lld = 0x%llX\n", decimalResult, decimalResult);
    }
    
    free(hexResult);  // ✅ Освобождение памяти
}
