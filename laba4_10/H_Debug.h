#ifndef H_Debug
#define H_Debug

#include "H_extraStuff.h"

Variable* debugProcessing(Variable* var_s, int base_assign)
{
    char choiceNum[33];
    int choiceNumDigit = 0;
    while (1)
    {
        printf("Ваши возможности:\n1) Вывести переменную\n2) Вывести все переменные\n3) Изменить переменную\n4) Объявить переменную\n5) Отменить переменную\n6) Вернуться к выполнению программы\n7) Завершить работы программы\n");
        fgets(choiceNum, 33, stdin);

        deleteNsymbol(choiceNum, 33);

        if (choiceNum && isNumber(choiceNum))
        {
            choiceNumDigit = atoi(choiceNum);
        }

        switch (choiceNumDigit)
        {
            case 1 :
                Variable** OnlyPtrVar_s = &var_s;
                char varNameFind[256];

                printf("Введите имя переменной:\n");
                fgets(varNameFind, 256, stdin);

                deleteNsymbol(varNameFind, 256);

                Variable* foundVar = LookForVar(*OnlyPtrVar_s, varNameFind);

                printf("\nПеременная, которую вы искали:\n");

                if (foundVar != NULL)
                {
                    char extr[3] = "\0\0\0";
                    itoa(base_assign, extr, 10);

                    printf("%s = %s\n", foundVar->name, convertBase(convertToDecimal(foundVar->value, extr),16));
                    dump_memory(&foundVar, sizeof(foundVar));
                }
                else
                {
                    printf("\nНе найдена\n");
                }
                printf("\n");
                break;

            case 2 :
                printf("\nВсе переменные:\n");
                PrintAllVar_s(var_s);
                printf("\n");
                break;

            case 3 :
                char newVarValue[33];
                char varName[256];

                printf("Введите имя переменной:\n");
                fgets(varName, 256, stdin);

                Variable* tempVar = LookForVar(var_s, varName);

                if (tempVar == NULL)
                {
                    printf("\nПеременная не найдена\n\n");
                    break;
                }

                printf("Введите новое значение переменной:\n");
                fgets(newVarValue, 32, stdin);

                deleteNsymbol(varName, 256);
                deleteNsymbol(newVarValue, 32);

                if (tempVar && isCorrectNumber(newVarValue, 16))
                {
                    char* newVarTemp = convertToDecimal(newVarValue, "16");
                    newVarTemp = convertBase(newVarTemp, base_assign);

                    for(int i = 0; i < 33; ++i) {
                        tempVar->value[i] = newVarTemp[i];
                    }

                    printf("\nПеременная изменена\n\n");
                }
                else if (!isNumber(newVarValue))
                {
                    printf("\nЗначение не является цифрой\n\n");
                }
                break;

            case 4 :
                char addName[256];

                printf("Введите имя переменной:\n");
                fgets(addName, 256, stdin);
                deleteNsymbol(addName, 256);

                Variable* tempAddVar = LookForVar(var_s, addName);

                if (tempAddVar)
                {
                    printf("\nПеременная уже существует\n\n");
                    break;
                }

                printf("Вы собираетесь ввести:\n 1) Цекеднорфово представление\n 2) Число записанное римскими цифрами\n");

                char choiceNum2[33];
                int choiceNumDigit2 = 0;
                fgets(choiceNum2, 33, stdin);
                deleteNsymbol(choiceNum2, 33);

                if (choiceNum2 && isNumber(choiceNum) && strlen(choiceNum2) == 1)
                {
                    choiceNumDigit2 = atoi(choiceNum2);
                }
                else
                {
                    printf("Что-то не то...\n");
                    break;
                }

                char newAddVarValue[256];
                char* newAddVarTemp;


                if(choiceNumDigit2 == 1)
                {
                    printf("Введите значение переменной:\n");
                    fgets(newAddVarValue, 255, stdin);
                    deleteNsymbol(newAddVarValue, 255);

                    newAddVarTemp = convertCekNumberToDecimal(newAddVarValue);
                }
                else if(choiceNumDigit2 == 2)
                {
                    printf("Введите значение переменной:\n");
                    fgets(newAddVarValue, 255, stdin);
                    deleteNsymbol(newAddVarValue, 255);

                    newAddVarTemp = romanToDecimal(newAddVarValue);
                }
                else
                {
                    printf("Что-то не то...\n");
                    break;
                }

                if (newAddVarTemp == NULL)
                {
                    printf("Что-то не то...\n");
                    break;
                }

                char* newAddVarRes = convertBase(newAddVarTemp, base_assign);

                InsertVar(var_s, addName, addName, newAddVarRes);

                printf("\nПеременная Создана\n\n");

                break;

            case 5 :
                char deleteVarName[256];
                printf("Введите имя переменной:\n");
                fgets(deleteVarName, 256, stdin);

                deleteNsymbol(deleteVarName, 256);

                Variable* deleteTempVar = LookForVar(var_s, deleteVarName);

                if (deleteTempVar)
                {
                    DeleteOnlyVar(var_s, deleteVarName);
                    printf("\nПеременная удалена\n\n");

                }
                else
                {
                    printf("\nПеременная не найдена\n\n");
                }

                break;

            case 6 :
                return var_s;

            case 7 :
                return NULL;

            default:
                printf("Что-то не то...\n");
                break;
        }
    }
}



#endif