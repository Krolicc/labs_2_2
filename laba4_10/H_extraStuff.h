#ifndef H_extraStuff
#define H_extraStuff

#include <ctype.h>

enum ProgramCondition
{
    OK = 0,
    PROBLEM,
};

enum OperationView
{
    OP__ = 0,
    _OP_,
    __OP,
};

enum ExpressionView
{
    LEFT = 0,
    RIGHT,
};

void dump_memory(void* ptr, size_t num_bytes) {
    unsigned char* bytes = (unsigned char*)ptr; // Преобразуем указатель на void в указатель на unsigned char
    for (size_t i = 0; i < num_bytes; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (bytes[i] >> bit) & 1); // Выводим 0 или 1 для каждого бита байта
        }
        printf(" "); // Разделяем каждый байт пробелом
    }
    printf("\n");
}

int isValidVariableName(char* str) {
    if (!str || !isalpha(str[0]) && str[0] != '_') {
        return 0; // Имя переменной не может начинаться с цифры или быть пустым
    }

    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            return 0; // Имя переменной должно содержать только буквы, цифры и символ подчеркивания
        }
    }

    return 1; // Строка является допустимым именем переменной
}

void copyFile(const char *srcFileName, const char *destFileName) {
    FILE *srcFile, *destFile;
    char buffer[1024];
    size_t bytesRead;

    srcFile = fopen(srcFileName, "rb");
    if (srcFile == NULL) {
        perror("Error opening source file");
        return;
    }

    destFile = fopen(destFileName, "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(srcFile);
        return;
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    fclose(srcFile);
    fclose(destFile);
}

int isNumber(char* a)
{
    for (int i = 0; i < strlen(a); ++i)
    {
        if (a[i] > '9' || a[i] < '0') return 0;
    }

    return 1;
}

int isCorrectNumber(char* a, int base)
{
    if (base < 2 || base > 36) return 0;
    char characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < strlen(a); ++i)
    {
        char symbol = a[i];
        if (symbol > 96 && symbol < 123) symbol = symbol - ' ';

        if ((symbol > '9' || symbol < '0') && (symbol > characters[base] || symbol < characters[10]))
            return 0;
    }

    return 1;
}

int isDebug(char* a)
{
    return strcmp(a, "-d") == 0 || strcmp(a, "--debug") == 0 || strcmp(a, "/debuh") == 0;
}

char* deleteNsymbol(char word[], int n) {

    for (int i = 0; i < n; ++i)
    {
        if(word[i] == '\n')
        {
            word[i] = '\0';
        }
    }

    return word;
}

int romanCharValue(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return -1; // Недопустимый символ
    }
}

// Функция для проверки корректности римского числа
int isValidRoman(const char *s) {
    int prev_value = 0; // Значение предыдущего символа
    int total = 0;      // Общее значение числа
    int repeat = 0;     // Счетчик повторений символов
    int subtraction_occurrence = 0; // Счетчик уменьшений

    // Проверка на корректность каждого символа и правила убывания
    for (int i = 0; s[i] != '\0'; i++) {
        int value = romanCharValue(s[i]);
        if (value < 0) return 0; // Недопустимый символ

        // Проверка на повторение символов
        if (i > 0 && s[i] == s[i - 1]) {
            repeat++;
            if (repeat > 2) return 0; // Символы I, X, C не могут повторяться более трех раз
            if ((value == 5 || value == 50 || value == 500) && repeat > 0) return 0; // V, L, D не могут повторяться
        } else {
            repeat = 0;
        }

        // Проверка на убывание значения символов
        if (prev_value < value) {
            if (prev_value != 0 && ((value / prev_value) != 5 && (value / prev_value) != 10)) {
                return 0; // Недопустимое уменьшение (например, IC или XM)
            }
            if (subtraction_occurrence > 0) return 0; // Уменьшение может произойти только один раз
            subtraction_occurrence++;
            total -= 2 * prev_value; // Корректировка общего значения для случаев типа IV или IX
        } else {
            subtraction_occurrence = 0; // Сброс счетчика уменьшений
        }

        total += value;
        prev_value = value;
    }

    // Проверка на максимальное значение римского числа
    if (total > 3999) return 0;

    return 1; // Строка корректна
}

int isValidCek(char *s)
{
    for (int i = 0; i < strlen(s); ++i)
    {
        if (s[i] != '0' && s[i] != '1') return 0;
    }

    return 1;
}

#endif