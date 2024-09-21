#ifndef H_binaryFunction
#define H_binaryFunction

#include "H_extraStuff.h"
#include "H_arithmetic.h"

char* convert(char hx);
char* convertToDecimal(char* a, char* base);
char* convertToBinary(char* a, char* base);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Функция для переворота строки
void reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

char* convertBase(char* num, int n) {

    if (n == 10 || (num[0] - '0') == 0) return num;

    if (n < 2 || n > 36) return "Invalid base";

    char characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char result[1000];
    int numArray[1000], remainder, i = 0, j = 0;
    memset(numArray, 0, sizeof(int) * 1000);
    memset(result, '\0', sizeof(char) * 1000);


    while (num[i] != '\0') {
        numArray[i] = num[i] - '0';
        i++;
    }

    int temp = 0;
    int checker = n;

    while (checker >= n && i > 0)
    {
        temp = 0;

        for (int k = 0; k < i; k++)
        {
            temp = temp * 10 + numArray[k];
            numArray[k] = temp / n;
            temp %= n;
        }

        result[j++] = characters[temp];


        while (numArray[0] == 0 && i > 0)
        {
            --i;
            for (int k = 0; k < i + 1; ++k)
            {
                numArray[k] = numArray[k + 1];
            }
        }

        if (i == 1)
        {
            checker = numArray[0];

            if (checker < n)
            {
                temp = numArray[0];

            }
        }
        else if (i == 2)
        {
            checker = numArray[0] * 10 + numArray[1];

            if (checker < n)
            {
                temp = numArray[0] * 10 + numArray[1];

            }
        }
    }

    if(temp > 0 && i > 0)
    {
        result[j++] = characters[temp];
    }

    int l = 0, r = strlen(result) - 1;
    while (l < r) {
        char temp = result[l];
        result[l] = result[r];
        result[r] = temp;
        l++;
        r--;
    }

    return strdup(result);
}

// Функция для преобразования десятичного числа в двоичное
char* convertToBinary(char* number, char* base) {

    const char* decimal = convertToDecimal(number, base);

    // Предполагаем, что входная строка не пустая и содержит только цифры
    int len = strlen(decimal);
    char *binary = (char*)malloc(len * 4); // Достаточно места для двоичного представления
    binary[0] = '\0';

    char *tempDecimal = strdup(decimal); // Создаем копию входной строки для манипуляций
    char *remainder = (char*)malloc(2);
    remainder[1] = '\0';

    while (strcmp(tempDecimal, "0") != 0) {
        int carry = 0;
        for (int i = 0; i < len; i++) {
            int digit = carry * 10 + (tempDecimal[i] - '0');
            carry = digit % 2;
            tempDecimal[i] = '0' + digit / 2;
        }

        // Удаляем ведущие нули
        int nonZeroIndex = 0;
        while (nonZeroIndex < len - 1 && tempDecimal[nonZeroIndex] == '0') {
            nonZeroIndex++;
        }
        memmove(tempDecimal, tempDecimal + nonZeroIndex, len - nonZeroIndex);
        tempDecimal[len - nonZeroIndex] = '\0';
        len -= nonZeroIndex;

        // Добавляем остаток в начало двоичной строки
        remainder[0] = '0' + carry;
        strcat(binary, remainder);
    }

    reverse(binary); // Переворачиваем строку, чтобы получить правильный порядок битов

    free(tempDecimal);
    free(remainder);

    return binary;
}

char* convertToDecimal(char* a, char* base) {

    char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char* charToDigit[28] = {"10", "11", "12", "13", "14", "15", "16", "17", "18",
                                 "19", "20", "21", "22", "23", "24", "25", "26", "27",
                                 "28", "29", "30", "31", "32", "33", "34", "35", "36",
                                 "37"};

    //TODO: ДОДЕЛАТЬ
    if (strcmp(base, "2") < 0 && strlen(base) == 1 || strcmp(base, "36") > 0 && strlen(base) == 2 ||
        strlen(base) > 2 || strlen(base) == 0) return "";

    if (strcmp(base, "10") == 0) return a;

    int resSize = 2;
    char* res = malloc(sizeof(char) * resSize);
    if (res == NULL) return "";
    res[0] = '0'; res[1] = '\0';

    char* tempDigit = malloc(sizeof(char) * 2);
    if (tempDigit == NULL) return "";
    tempDigit[1] ='\0';


    for (int i = 0; i < strlen(a); ++i) {
        memset(tempDigit, a[i], 1);
        res = regulator(res, base, "mult", base);

        if (a[i] != '0')
        {
            int k = 0;
            for (int j = 0; j < 37; ++j)
            {
                if(alphabet[j] == a[i])
                {
                    k = j;
                    break;
                }
            }
            char* tempD;
            if(k < 10)
            {
                tempD = malloc(sizeof(char) * 2);
                tempD[1] = '\0';

                tempD[0] = alphabet[k];
            }
            else
            {
                tempD = malloc(sizeof(char) * (strlen(charToDigit[k - 10]) + 1));
                tempD[strlen(tempD) - 1] = '\0';

                for(int j = 0; j < strlen(tempD); ++j)
                {
                    tempD[j] = charToDigit[k - 10][j];
                }
            }
            //TODO
            res = regulator(res, tempD, "sum", base);

            if(tempD != res)
            {
                free(tempD);
            }
        }
    }

    free(tempDigit);

    return res;
}


#endif