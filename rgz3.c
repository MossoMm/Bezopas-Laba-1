#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char digit_to_char(int n) //функция переводит число в символ для использования систем счисления с основанием больше 10
{
    if (n < 10) return '0' + n;
    else return 'A' + (n - 10);
}

int char_to_digit(char c) //функция переводит символ в число для использования систем счисления с основанием больше 10
{
    if (c >= '0' && c <= '9') return c - '0';
    else return 10 + (c - 'A');
}

void print_num(int n, int base) //функция выводит число
{
    if (n < base)
    {
        printf("%c", digit_to_char(n));
    }
    else
    {
        print_num(n/base, base);
        printf("%c", digit_to_char(n % base));
    }
}

void print_in_table(int n, int base, int w)  //функция выводит число в заданной системе счисления
{
    char reserve[32];
    int i = 0;
    int temp = n;
    
    if (temp == 0)
    {
        reserve[i++] = '0';
    }
    else
    {
        while (temp > 0)
        {
            if (i >= 31) break;
            reserve[i++] = digit_to_char(temp % base);
            temp /= base;
        }
    }
    reserve[i] = '\0';

    for (int j = 0; j < i/2; j++)
    {
        char temp_char = reserve[j];
        reserve[j] = reserve[i - j - 1];
        reserve[i - j - 1] = temp_char;
    }

    for (int j = i; j < w; j++)
    {
        printf(" ");
    }
    printf("%s", reserve);
}

void addition_table(int base) //функция создаёт таблицу сложения для заданной системы счисления
{
    int max_sum = 2 * (base - 1);
    int max_width = 1;
    int temp = max_sum;
    while (temp >= base)
    {
        temp /= base;
        max_width++;
    }

    printf("\nТаблица сложения для %d-ричной системы:\n", base);
    printf("+ ");
    for (int i = 0; i < base; i++)
    {
        printf(" ");
        print_in_table(i, base, max_width);
    }
    printf("\n   ");
    for (int i = 0; i < base; i++)
    {
        for (int j = 0; j < max_width; j++) printf("-");
        printf(" ");
    }
    printf("\n");

    for (int i = 0; i < base; i++)
    {
        printf("%c|", digit_to_char(i));
        for (int j = 0; j < base; j++)
        {
            printf(" ");
            print_in_table(i + j, base, max_width);
        }
        printf("\n");
    }
}

void multiplication_table(int base) //функция создаёт таблицу умножения для заданной системы счисления
{
    int max_prod = (base - 1) * (base - 1);
    int max_width = 1;
    int temp = max_prod;
    while (temp >= base)
    {
        temp /= base;
        max_width++;
    }

    printf("\nТаблица умножения для %d-ричной системы:\n", base);
    printf("* ");
    for (int i = 0; i < base; i++)
    {
        printf(" ");
        print_in_table(i, base, max_width);
    }
    printf("\n   ");
    for (int i = 0; i < base; i++)
    {
        for (int j = 0; j < max_width; j++) printf("-");
        printf(" ");
    }
    printf("\n");
    
    for (int i = 0; i < base; i++)
    {
        printf("%c|", digit_to_char(i));
        for (int j = 0; j < base; j++)
        {
            printf(" ");
            print_in_table(i * j, base, max_width);
        }
        printf("\n");
    }
}

void demonstrate_multiplication(const char* num1_str, const char* num2_str, int base) //функция перемножает два числа в заданной системе счисления
{ 
    for (int i = 0; num1_str[i] != '\0'; i++)
    {
        if (char_to_digit(num1_str[i]) >= base)
        {
            printf("Слишком большой символ '%c' в первом числе для %d-ричной системы\n", num1_str[i], base);
            return;
        }
    }
    for (int i = 0; num2_str[i] != '\0'; i++)
    {
        if (char_to_digit(num2_str[i]) >= base)
        {
            printf("Слишком большой символ '%c' во втором числе для %d-ричной системы\n", num2_str[i], base);
            return;
        }
    }

    printf("\n===========================================\n");
    printf("ДЕМОНСТРАЦИЯ УМНОЖЕНИЯ В %d-РИЧНОЙ СИСТЕМЕ:\n", base);
    printf("===========================================\n\n");
    printf("Умножаем: %s × %s (в %d-ричной системе)\n\n", num1_str, num2_str, base);
     
    int len1 = strlen(num1_str);
    int len2 = strlen(num2_str);
    
    printf("Первое число: %s (разряды: ", num1_str);
    for (int i = 0; i < len1; i++)
    {
        printf("%c", num1_str[i]);
        if (i < len1 - 1) printf(", ");
    }
    printf(")\n");
    
    printf("Второе число: %s (разряды: ", num2_str);
    for (int i = 0; i < len2; i++)
    {
        printf("%c", num2_str[i]);
        if (i < len2 - 1) printf(", ");
    }
    printf(")\n\n");
    
    int max_len = len1 + len2;
    int part_prod[len2][max_len];
    int result[max_len];
    
    for (int i = 0; i < len2; i++)
    {
        for (int j = 0; j < max_len; j++)
        {
            part_prod[i][j] = 0;
        }
    }
    for (int i = 0; i < max_len; i++)
    {
        result[i] = 0;
    }
    
    printf("1) Перемножение каждого разряда первого числа на второе число\n");
    for (int i = len2 - 1; i >= 0; i--)
    {
        int digit2 = char_to_digit(num2_str[i]);
        int move = 0;
        int pos = len2 - 1 - i;

        printf("\nУмножаем '%c' (значение %d):\n", num2_str[i], digit2);
        for (int j = len1 - 1; j >= 0; j--)
        {
            int digit1 = char_to_digit(num1_str[j]);
            int product = digit1 * digit2 + move;
            printf("  %c × %c = %d (в %d-ричной: ", num1_str[j], num2_str[i], digit1*digit2, base);
            print_num(digit1 * digit2, base);
            printf(")");
            if (move > 0)
            {
                printf(" + перенос %c = ", digit_to_char(move));
                print_num(product, base);
            }
            printf("\n");
            int position = (len1 - 1 - j) + pos;
            part_prod[len2 - 1 - i][position] = product % base;
            move = product / base;
        }
        if (move > 0)
        {
            int position = len1 + pos;
            if (position < max_len)
            {
                part_prod[len2 - 1 - i][position] = move;
            }
            printf("  + перенос %c\n", digit_to_char(move));
        }
        printf("\nПромежуточное произведение для этого разряда: ");

        int start_pos = max_len - 1;
        while (start_pos >= 0 && part_prod[len2 - 1 - i][start_pos] == 0)
        {
            start_pos--;
        }
        
        if (start_pos < 0)
        {
            printf("0");
        }
        else
        {
            for (int k = start_pos; k >= 0; k--)
            {
                printf("%c", digit_to_char(part_prod[len2 - 1 - i][k]));
            }
        }
        printf("\n");
    }
    
    printf("\n2) Сложение промежуточных произведений\n");
    printf("\nВыравнивание:\n\n");
    
    for (int i = 0; i < len2; i++)
    {
        printf("  ");
        int f_not_zero = max_len - 1;
        while (f_not_zero >= 0 && part_prod[i][f_not_zero] == 0)
        {
            f_not_zero--;
        }
        
        int space = max_len - (f_not_zero + 1);
        for (int s = 0; s < space; s++)
        {
            printf(" ");
        }
        for (int j = f_not_zero; j >= 0; j--)
        {
            printf("%c", digit_to_char(part_prod[i][j]));
        }
        printf("\n");
    }

    printf("\nCложение:\n");
    int move = 0;
    for (int i = 0; i < max_len; i++)
    {
        int sum = move;
        int count = 0;
        
        for (int j = 0; j < len2; j++)
        {
            if (part_prod[j][i] != 0 || (i == 0 && j == 0))
            {
                sum += part_prod[j][i];
                if (count > 0) printf(" + ");
                printf("%c", digit_to_char(part_prod[j][i]));
                count++;
            }
        }
        if (move > 0)
        {
            if (count == 0)
            {
                printf("перенос %c", digit_to_char(move));
            }
            else printf(" + перенос %c", digit_to_char(move));
        }
        if (count == 0 && move == 0)
        {
            printf("0");
        }
        result[i] = sum % base;
        move = sum / base;
        printf(" = ");
        print_num(sum, 10);
        printf(" (в %d-ричной: ", base);
        print_num(sum, base);
        printf(") -> записываем %c", digit_to_char(result[i]));
        if (move > 0)
        {
            printf(", переносим %c", digit_to_char(move));
        }
        printf("\n");
    }
    
    printf("\nОтвет:\n");
    int start_pos = max_len - 1;
    while (start_pos > 0 && result[start_pos] == 0)
    {
        start_pos--;
    }
    
    for (int i = start_pos; i >= 0; i--)
    {
        printf("%c", digit_to_char(result[i]));
    }
    int num1_dec = 0;
    int num2_dec = 0;
    
    for (int i = 0; i < len1; i++)
    {
        num1_dec = num1_dec * base + char_to_digit(num1_str[i]);
    }
    
    for (int i = 0; i < len2; i++)
    {
        num2_dec = num2_dec * base + char_to_digit(num2_str[i]);
    }
    
    printf("\nПроверка в %d-ричной системе:\n", base);
    printf("%s × %s = ", num1_str, num2_str);
    for (int i = start_pos; i >= 0; i--)
    {
        printf("%c", digit_to_char(result[i]));
    }
    printf("\n");
}

int main()
{
    printf("=========================================\n");
    printf("ТАБЛИЦЫ ДЛЯ 12-РИЧНОЙ СИСТЕМЫ СЧИСЛЕНИЯ\n");
    printf("=========================================\n");
    addition_table(12);
    multiplication_table(12);

    printf("\n\n=========================================\n");
    printf("ТАБЛИЦЫ ДЛЯ 14-РИЧНОЙ СИСТЕМЫ СЧИСЛЕНИЯ\n");
    printf("=========================================\n");
    addition_table(14);
    multiplication_table(14);

    demonstrate_multiplication("27", "14", 12);
    demonstrate_multiplication("A30B", "C4", 14);
    demonstrate_multiplication("D59", "A2", 14);
    return 0;
}