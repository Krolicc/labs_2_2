#ifndef H_arithmetic
#define H_arithmetic

char* mySum (char* a, char* b);
char* mySub (char* a, char* b);
char* myMult(char* a, char* b);
char* myDiv (char* a, char* b);
char* myRem (char* a, char* b);
char* myXor (char* a, char* b);
char* myOr  (char* a, char* b);
char* myAnd (char* a, char* b);
char* myPow (char* a, char* b);

char* myNot (char* a);

char* regulator(char* a, char* b, const char* opName, char* base);

#include "H_extraStuff.h"
#include "H_binaryFunction.h"


char* regulator(char* a, char* b, const char* opName, char* base)
{
    char* res = NULL;

    if (isCorrectNumber(a, atoi(base)) == 0 || isCorrectNumber(b, atoi(base)) == 0) return NULL;

    if(strcmp(opName, "add") == 0) {
        a = convertToDecimal(a, base);
        b = convertToDecimal(b, base);
        res = mySum(a, b);
    }
    if(strcmp(opName, "sub") == 0) {
        a = convertToDecimal(a, base);
        b = convertToDecimal(b, base);
        res = mySub(a, b);
    }
    if(strcmp(opName, "mult") == 0) {
        a = convertToDecimal(a, base);
        b = convertToDecimal(b, base);
        res = myMult(a, b);
    }
    if(strcmp(opName, "div") == 0) {
        a = convertToDecimal(a, base);
        b = convertToDecimal(b, base);
        res = myDiv(a, b);
    }
    if(strcmp(opName, "rem") == 0) {
        a = convertToDecimal(a, base);
        b = convertToDecimal(b, base);
        res = myRem(a, b);
    }
    if(strcmp(opName, "pow") == 0) {
        a = convertToDecimal(a, base);
        b = convertToDecimal(b, base);
        res = myPow(a, b);
    }
    if(strcmp(opName, "CTD") == 0) {
        res = convertToDecimal(a, b);
    }
    if(strcmp(opName, "CTB") == 0) {
        a = convertToDecimal(a, base);
        res = convertToBinary(a, b);
    }
    if(strcmp(opName, "xor") == 0) {
        a = convertToBinary(a, base);
        b = convertToBinary(b, base);
        res = myXor(a, b);
        res = convertToDecimal(res, "2");
    }
    if(strcmp(opName, "or") == 0) {
        a = convertToBinary(a, base);
        b = convertToBinary(b, base);
        res = myOr(a, b);
        res = convertToDecimal(res, "2");
    }
    if(strcmp(opName, "not") == 0) {
        a = convertToBinary(a, base);
        res = myNot(a);
        res = convertToDecimal(res, "2");
    }
    if(strcmp(opName, "and") == 0) {
        a = convertToBinary(a, base);
        b = convertToBinary(b, base);
        res = myAnd(a, b);
        res = convertToDecimal(res, "2");
    }

    if (res == NULL) return NULL;

    while (res[0] == '0' && res[1] != '\0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* mySum(char* a, char* b)
{
    if(strcmp(a, "0") == 0) return b;
    if(strcmp(b, "0") == 0) return a;

    if (strlen(a) < strlen(b))
    {
        return regulator(b,a, "add", "10");
    }

    int aLEN = strlen(a);
    int bLEN = strlen(b);
    int extra = 0;

    char* res = malloc(sizeof(char) * (2 + aLEN));
    if(res == NULL) return "-1";
    memset(res, '0', (1 + aLEN) * sizeof(char));
    res[1+aLEN] = '\0';


    while(bLEN != 0)
    {
        int temp = (a[aLEN - 1] - '0') + (b[bLEN - 1] - '0') + extra;
        res[aLEN] = (temp % 10) + '0';
        extra = temp / 10;
        --aLEN;
        --bLEN;
    }

    while (aLEN != 0)
    {
        int temp = (a[aLEN - 1] - '0') + extra;
        res[aLEN] = (temp % 10) + '0';
        extra = temp / 10;

        --aLEN;
    }

    if(extra > 0)
    {
        res[0] = extra + '0';
    }

    if (res[0] == '0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* mySub(char* a, char* b)
{
    if(strcmp(b, "0") == 0) return a;

    int aLEN = strlen(a);
    int bLEN = strlen(b);
    int extra = 0;

    char* res = malloc(sizeof(char) * (2 + aLEN));
    if(res == NULL) return "-1";
    memset(res, '0', (1 + aLEN) * sizeof(char));
    res[1+aLEN] = '\0';

    while (bLEN != 0)
    {
        int temp;
        if ((a[aLEN - 1] - '0') - (b[bLEN - 1] - '0') + extra < 0)
        {
            temp = (a[aLEN - 1] - '0') - (b[bLEN - 1] - '0') + 10 + extra;
            extra = -1;
        }
        else
        {
            temp = (a[aLEN - 1] - '0') - (b[bLEN - 1] - '0') + extra;
            extra = 0;
        }
        res[aLEN] = (temp % 10) + '0';
        --aLEN;
        --bLEN;
    }

    while (aLEN > 0)
    {
        int temp;
        if ((a[aLEN - 1] - '0') + extra < 0)
        {
            temp = (a[aLEN - 1] - '0') + 10 + extra;
            extra = -1;
        }
        else
        {
            temp = (a[aLEN - 1] - '0') + extra;
            extra = 0;
        }
        res[aLEN] = (temp % 10) + '0';
        extra = temp / 10 + extra;

        --aLEN;
    }

    while (res[0] == '0' && res[1] != '\0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* myMult(char* a, char* b)
{
    if(strcmp(a, "0") == 0) return "0";
    if(strcmp(a, "0") == 0) return "0";

    if (strlen(a) < strlen(b))
    {
        return regulator(b, a, "mult", "10");
    }

    int aLEN = strlen(a);
    int bLEN = strlen(b);
    int extra = 0;

    int resSize = 1 + bLEN + aLEN;
    char* res = malloc(sizeof(char) * resSize);
    if(res == NULL) return "-1";
    memset(res, '0', resSize * sizeof(char));
    res[resSize - 1] = '\0';


    for (int i = aLEN - 1; i >= 0; --i)
    {
        for (int j = bLEN - 1; j >= 0; --j)
        {
            int temp = (a[i] - '0') * (b[j] - '0') + (res[i + j + 1] - '0') + extra;
            res[i + j + 1] = (temp % 10) + '0';
            extra = temp / 10;
        }
    }

    if (extra != 0)
    {
        res[0] = extra + '0';
    }

    while (res[0] == '0' && res[1] != '\0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* myDiv(char* a, char* b)
{
    if(b[0] == '0') return NULL;
    if(a[0] == '0' || strlen(a) == strlen(b) && strcmp(a, b) < 0 || strlen(a) < strlen(b)) return "0";

    int j = 0;
    int aLEN = strlen(a);
    int bLEN = strlen(b);
    int resSize = aLEN - bLEN + 2;

    char* res = malloc(sizeof(char) * resSize);
    if(res == NULL) return "-1";

    memset(res, '0', resSize * sizeof(char));
    res[resSize - 1] = '\0';
    char* tt;

    while (j < resSize - 1)
    {
        res[j] = '0';
        while (strcmp(a, (tt = regulator(b, res, "mult", "10"))) >= 0 && strlen(a) == strlen(tt) || strlen(a) > strlen(tt))
        {
            res[j] = ((res[j] - '0') + 1) + '0';
        }
        res[j] = ((res[j] - '0') - 1) + '0';
        j++;
    }


    if(res[0] == '0') {
        for(int i = 0; i <= strlen(res); ++i) {
            if(i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* myRem(char* a, char* b)
{
    if(strcmp(b, "0") == 0) return NULL;

    if(strcmp(b, "1") == 0) return "0";

    if(strcmp(a, "1") == 0) return "1";

    int a_len = strlen(a);
    int b_len = strlen(b);

    if(a_len < b_len || a_len == b_len && strcmp(a,b) < 0)
    {
        return a;
    }

    return regulator(a, regulator(b, regulator(a, b, "div", "10"), "mult", "10"), "sub", "10");
}



char* myPow(char* a, char* b)
{
    char* module = "4294967296";
    if(strcmp(b, "0") == 0) return "1";
    if(strcmp(b, "1") == 0) return a;
    if(strcmp(b, "2") == 0) return myRem(myMult(a,a), module);

    char* number[32];
    int i = 1;
    char degrees[32][32];
    int startDegree = 1;
    int finishDegree = atoi(b);

    while(startDegree < finishDegree)
    {
        if(i >= 2)
        {
            number[i] = myRem(myMult(number[i - 1], number[i - 1]) ,module);
        }
        else if (i == 1)
        {
            number[i] = myRem(a ,module);
        }

        itoa(startDegree, degrees[i], 10);
        startDegree *= 2;

        ++i;
    }

    --i;

    char* res = number[i];
    int resDegrees = atoi(degrees[i]);
    if(i - 2 > 0)
    {
        i-=2;
    }

    while (resDegrees != finishDegree) {
        if (resDegrees + atoi(degrees[i]) <= finishDegree) {
            res =  myRem(myMult(res, number[i]), module);
            resDegrees += atoi(degrees[i]);
        }

        --i;
    }

    return res;
}



char* myXor(char* a, char* b)
{
    if (strlen(a) < strlen(b))
    {
        return regulator(b, a, "xor", "10");
    }

    int resSize = strlen(a);
    int i = strlen(a) - 1;
    int j = strlen(b) - 1;

    char* res = malloc(sizeof(char) * (resSize + 1));
    if (res == NULL) return NULL;
    memset(res, '\0', resSize + 1);

    while (j >= 0)
    {
        res[resSize - 1] = a[i] != b[j] ? '1' : '0';
        --i;
        --j;
        --resSize;
    }

    while (i >= 0)
    {
        res[resSize - 1] = a[i] == '1' ? '1' : '0';
        --i;
        --resSize;
    }

    while (res[0] == '\0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* myOr(char* a, char* b)
{
    if (strlen(a) < strlen(b))
    {
        return regulator(b, a, "or", "10");
    }

    int resSize = strlen(a);
    int i = strlen(a) - 1;
    int j = strlen(b) - 1;

    char* res = malloc(sizeof(char) * (resSize + 1));
    if (res == NULL) return NULL;
    memset(res, '\0', resSize + 1);

    while (j >= 0)
    {
        res[resSize - 1] = a[i] == '1' || b[j] == '1'
                           ? '1'
                           : '0';
        --i;
        --j;
        --resSize;
    }

    while (i >= 0)
    {
        res[resSize - 1] = a[i];
        --i;
        --resSize;
    }

    while (res[0] == '\0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* myNot(char* a)
{
    int resSize = strlen(a);

    char* res = malloc(sizeof(char) * (resSize + 1));
    if (res == NULL) return NULL;
    memset(res, '\0', resSize + 1);

    for (int k = 0; k <= resSize - 1; ++k)
    {
        res[k] = a[k] == '1'
                     ? '0'
                     : '1';
    }

    return res;
}

char* myAnd(char* a, char* b)
{
    if (strlen(a) > strlen(b))
    {
        return regulator(b, a, "and", "10");
    }

    int resSize = strlen(a);
    int i = strlen(a) - 1;
    int j = strlen(b) - 1;

    char* res = malloc(sizeof(char) * (resSize + 1));
    if (res == NULL) return NULL;
    memset(res, '\0', resSize + 1);

    while (i >= 0)
    {
        res[resSize - 1] = a[i] == '1' && a[i] == b[j]
                           ? '1'
                           : '0';
        --i;
        --j;
        --resSize;
    }

    while (res[0] == '\0')
    {
        for (int i = 0; i <= strlen(res); ++i)
        {
            if (i == strlen(res))
            {
                res[i] = '\0';
                break;
            }

            res[i] = res[i + 1];
        }
    }

    return res;
}

char* convertCekNumberToDecimal (char* num)
{
    int decimal = 0;
    int base = 1;
    int len = strlen(num);
    char* result = malloc(sizeof(char) * 12); // abbastanza grande per contenere qualsiasi numero in base 10

    for (int i = len - 1; i >= 0; i--) {
        if (num[i] == '1') {
            decimal += base;
        }
        base *= 3;
    }

    sprintf(result, "%d", decimal);
    return result;
}

char* romanToDecimal(char *roman) {
    int decimal = 0;
    const char* symbols = "IVXLCDM";
    int values[] = {1, 5, 10, 50, 100, 500, 1000};
    int i, j;

    for (i = 0; i < strlen(roman); i++) {
        for (j = 0; j < 7; j++) {
            if (roman[i] == symbols[j]) {
                if (i > 0 && (roman[i - 1] == 'I' || roman[i - 1] == 'X') && values[j] > values[j - 1]) {
                    decimal += values[j] - 2 * values[j - 1];
                } else {
                    decimal += values[j];
                }
                break;
            }
        }
        if (j == 7) {
            return NULL; // символ не найден в массиве symbols
        }
    }

    if (decimal == 0) {
        return NULL; // пустая строка или неверное римское число
    }

    char* result = malloc(sizeof(char) * 12); // abbastanza grande per contenere qualsiasi numero in base 10
    sprintf(result, "%d", decimal);
    return result;
}


#endif