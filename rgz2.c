#include <stdio.h>

#define DBL_LEN 64
#define MANT_LEN 52
#define EXP_LEN 11

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

/* бинарное умножение */
void binMul(unsigned* a, int lenA,
            unsigned* b, int lenB,
            unsigned* overflow)
{
    unsigned temp[256] = { 0 };

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

    for (int i = 0; i < lenB; i++)
        overflow[i] = temp[lenA + i];
}

/* ================= IEEE 754 ================= */

int getSign(unsigned long long v) {
    return (v >> 63) & 1;
}

int getExp(unsigned long long v) {
    return ((v >> MANT_LEN) & B11) - 1023;
}

unsigned long long getMant(unsigned long long v) {
    return v & B52;
}

/* ================= MAIN ================= */
void romantoint()
{
  union {
        unsigned long long ll;
        double d;
    } num;

    scanf("%lf", &num.d);

    printf("Binary (64 bits):\n");
    printBin(num.ll, 0, DBL_LEN);

    int sign = getSign(num.ll);
    int exp  = getExp(num.ll);
    unsigned long long mant = getMant(num.ll);

    printf("\n\nSign: %d", sign);
    printf("\nExponent: %d", exp);
    printf("\nMantissa: ");
    printBin(num.ll, 0, MANT_LEN);

    /* ================= ЦЕЛАЯ ЧАСТЬ ================= */

    unsigned long long fullMant = mant | (1ULL << MANT_LEN); // скрытая 1
    unsigned long long intPart = 0;

    if (exp >= 0)
        intPart = fullMant >> (MANT_LEN - exp);

    /* ================= ДРОБНАЯ ЧАСТЬ ================= */

    /* бинарное представление 1.mantissa */
    unsigned frac[MANT_LEN + 1] = { 0 };
    frac[MANT_LEN] = 1; // скрытая единица

    for (int i = 0; i < MANT_LEN; i++)
        frac[i] = (mant >> i) & 1;

    /* применение экспоненты */
    if (exp < 0) {
        for (int k = 0; k < -exp; k++) {
            for (int i = 0; i < MANT_LEN; i++)
                frac[i] = frac[i + 1];
            frac[MANT_LEN] = 0;
        }
    }

    /* перевод дробной части в десятичную */
    unsigned ten[4] = { 0, 1, 0, 1 }; // 10
    unsigned over[4];
    unsigned dec[64];
    int cnt = 0;

    while (!isZero(frac, MANT_LEN) && cnt < 60) {
        binMul(frac, MANT_LEN, ten, 4, over);
        dec[cnt++] = binToDec(over, 4);
    }

    /* ================= ВЫВОД ================= */

    printf("\n\nDecimal value:\n");
    printf("%llu.", intPart);
    for (int i = 0; i < cnt; i++)
        printf("%u", dec[i]);

    printf("\n\nprintf value:\n%.60lf\n", num.d);
}

int main(void) {
    
    romantoint();
    return 0;
}
