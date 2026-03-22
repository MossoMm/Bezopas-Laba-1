#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define DBL_LEN 64
#define MANT_LEN 52
#define EXP_LEN 11
#define EXP_BIAS 1023

#define B52 0x000FFFFFFFFFFFFFULL
#define B11 0x7FF

/* ================= ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ================= */

void printBin(unsigned long long val, int low, int high) {
    for (int i = high - 1; i >= low; i--)
        printf("%llu", (val >> i) & 1);
}

int isZero(unsigned* arr, int n) {
    for (int i = 0; i < n; i++)
        if (arr[i]) return 0;
    return 1;
}

unsigned binToDec(unsigned* arr, int n) {
    unsigned res = 0, p = 1;
    for (int i = 0; i < n; i++) {
        if (arr[i]) res += p;
        p <<= 1;
    }
    return res;
}

/* бинарное умножение с проверкой размера */
int binMul(unsigned* a, int lenA, unsigned* b, int lenB, unsigned* overflow, int maxOverflow) {
    unsigned temp[512] = { 0 }; // Увеличенный буфер
    
    // Проверка на переполнение буфера
    if (lenA + lenB > 512) {
        printf("Ошибка: слишком большая длина для умножения\n");
        return -1;
    }
    
    for (int i = 0; i < lenA; i++) {
        unsigned carry = 0;
        for (int j = 0; j < lenB; j++) {
            unsigned s = temp[i + j] + a[i] * b[j] + carry;
            temp[i + j] = s & 1;
            carry = s >> 1;
        }
        temp[i + lenB] += carry;
    }
    
    for (int i = 0; i < lenA; i++)
        a[i] = temp[i];
    
    // Копируем overflow с проверкой границ
    int overflowLen = (lenA + lenB < maxOverflow) ? lenA + lenB : maxOverflow;
    for (int i = 0; i < overflowLen; i++)
        overflow[i] = temp[lenA + i];
    
    return overflowLen;
}

/* ================= IEEE 754 ================= */

int getSign(unsigned long long v) {
    return (v >> 63) & 1;
}

int getExp(unsigned long long v) {
    return ((v >> MANT_LEN) & B11) - EXP_BIAS;
}

unsigned long long getMant(unsigned long long v) {
    return v & B52;
}

/* Проверка на специальные значения */
int isSpecial(double d) {
    if (isinf(d)) return 1;  // Бесконечность
    if (isnan(d)) return 2;   // NaN
    return 0;
}

/* ================= ОСНОВНАЯ ФУНКЦИЯ ================= */

void romantoint() {
    srand(time(NULL));
    union {
        unsigned long long ll;
        double d;
    } num;
    
    printf("Введите число типа double: ");
    num.d = rand();
    
    printf("\n=== ДВОИЧНОЕ ПРЕДСТАВЛЕНИЕ (64 бита) ===\n");
    printf("Биты: ");
    printBin(num.d, 0, DBL_LEN);
    printf("\n");
    
    // Проверка специальных значений
    int special = isSpecial(num.d);
    if (special == 1) {
        printf("\n=== СПЕЦИАЛЬНОЕ ЗНАЧЕНИЕ: БЕСКОНЕЧНОСТЬ ===\n");
        printf("Знак: %d\n", getSign(num.ll));
        printf("Значение: %sinf\n", getSign(num.ll) ? "-" : "");
        printf("\nprintf значение: %lf\n", num.d);
        return;
    }
    if (special == 2) {
        printf("\n=== СПЕЦИАЛЬНОЕ ЗНАЧЕНИЕ: NaN (НЕ ЧИСЛО) ===\n");
        printf("\nprintf значение: %lf\n", num.d);
        return;
    }
    
    int sign = getSign(num.ll);
    int exp = getExp(num.ll);
    unsigned long long mant = getMant(num.ll);
    
    // Проверка на ноль
    if (exp == -1023 && mant == 0) {
        printf("\n=== НОЛЬ ===\n");
        printf("Знак: %d\n", sign);
        printf("Значение: 0.0\n");
        printf("\nprintf значение: %lf\n", num.d);
        return;
    }
    
    printf("\n=== КОМПОНЕНТЫ ЧИСЛА ===\n");
    printf("Знак: %d (%s)\n", sign, sign ? "отрицательный" : "положительный");
    printf("Экспонента: %d (смещённая: %d)\n", exp, exp + EXP_BIAS);
    printf("Мантисса: ");
    printBin(num.ll, 0, MANT_LEN);
    printf(" (с隐藏ым битом: 1)");
    
    /* ================= ЦЕЛАЯ ЧАСТЬ ================= */
    
    unsigned long long fullMant;
    unsigned long long intPart = 0;
    
    if (exp >= -1022) { // Нормализованное число
        fullMant = mant | (1ULL << MANT_LEN); // скрытая 1
        if (exp >= 0)
            intPart = fullMant >> (MANT_LEN - exp);
        else
            intPart = 0;
    } else { // Денормализованное число
        fullMant = mant;
        intPart = 0;
        exp = -1022; // Корректировка для денормализованных
    }
    
    /* ================= ДРОБНАЯ ЧАСТЬ ================= */
    
    /* бинарное представление мантиссы */
    unsigned frac[MANT_LEN + 2] = { 0 }; // +2 для безопасности
    
    if (exp >= -1022) { // Нормализованное
        frac[MANT_LEN] = 1; // скрытая единица
    }
    
    for (int i = 0; i < MANT_LEN; i++)
        frac[i] = (mant >> i) & 1;
    
    /* применение экспоненты */
    if (exp < 0) {
        int shift = -exp;
        if (shift > MANT_LEN + 1) shift = MANT_LEN + 1; // Ограничиваем сдвиг
        
        for (int k = 0; k < shift; k++) {
            for (int i = 0; i < MANT_LEN + 1; i++)
                frac[i] = frac[i + 1];
            frac[MANT_LEN + 1] = 0;
        }
    } else if (exp > 0) {
        // Для положительной экспоненты сдвигаем влево
        for (int k = 0; k < exp && k < MANT_LEN; k++) {
            for (int i = MANT_LEN + 1; i > 0; i--)
                frac[i] = frac[i - 1];
            frac[0] = 0;
        }
    }
    
    /* перевод дробной части в десятичную */
    unsigned ten[4] = { 0, 1, 0, 1 }; // 10 в двоичном (младший бит первый)
    unsigned over[8] = { 0 }; // Увеличенный буфер для переноса
    unsigned dec[100] = { 0 };
    int cnt = 0;
    int maxDigits = 100; // Максимальное количество десятичных цифр
    
    // Определяем длину дробной части для проверки на ноль
    int fracLen = MANT_LEN + 2;
    
    while (!isZero(frac, fracLen) && cnt < maxDigits) {
        int overLen = binMul(frac, fracLen, ten, 4, over, 8);
        if (overLen < 0) break;
        
        dec[cnt++] = binToDec(over, 4);
    }
    
    /* ================= ВЫВОД ================= */
    
    printf("\n\n=== ДЕСЯТИЧНОЕ ПРЕДСТАВЛЕНИЕ ===\n");
    printf("Точное значение:\n");
    if (sign) printf("-");
    printf("%llu.", intPart);
    
    if (cnt == 0) {
        printf("0");
    } else {
        for (int i = 0; i < cnt; i++)
            printf("%u", dec[i]);
    }
    
    if (cnt >= maxDigits) {
        printf("... (обрезано)");
    }
    
    printf("\n\n=== СТАНДАРТНЫЙ ВЫВОД ===\n");
    printf("printf значение (%.15lf):\n", num.d);
    printf("С точностью 15 знаков: %.15lf\n", num.d);
    printf("С точностью 60 знаков: %.60lf\n", num.d);
}
