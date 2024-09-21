#ifndef H_Debug
#define H_Debug

#include "H_extraStuff.h"

Variable* debugProcessing(Variable* var_s, int base_assign)
{
    char choiceNum[33];
    int choiceNumDigit = 0;
    while (1)
    {
        printf("���� �����������:\n1) ������� ����������\n2) ������� ��� ����������\n3) �������� ����������\n4) �������� ����������\n5) �������� ����������\n6) ��������� � ���������� ���������\n7) ��������� ������ ���������\n");
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

                printf("������� ��� ����������:\n");
                fgets(varNameFind, 256, stdin);

                deleteNsymbol(varNameFind, 256);

                Variable* foundVar = LookForVar(*OnlyPtrVar_s, varNameFind);

                printf("\n����������, ������� �� ������:\n");

                if (foundVar != NULL)
                {
                    char extr[3] = "\0\0\0";
                    itoa(base_assign, extr, 10);

                    printf("%s = %s\n", foundVar->name, convertBase(convertToDecimal(foundVar->value, extr),16));
                    dump_memory(&foundVar, sizeof(foundVar));
                }
                else
                {
                    printf("\n�� �������\n");
                }
                printf("\n");
                break;

            case 2 :
                printf("\n��� ����������:\n");
                PrintAllVar_s(var_s);
                printf("\n");
                break;

            case 3 :
                char newVarValue[33];
                char varName[256];

                printf("������� ��� ����������:\n");
                fgets(varName, 256, stdin);

                Variable* tempVar = LookForVar(var_s, varName);

                if (tempVar == NULL)
                {
                    printf("\n���������� �� �������\n\n");
                    break;
                }

                printf("������� ����� �������� ����������:\n");
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

                    printf("\n���������� ��������\n\n");
                }
                else if (!isNumber(newVarValue))
                {
                    printf("\n�������� �� �������� ������\n\n");
                }
                break;

            case 4 :
                char addName[256];

                printf("������� ��� ����������:\n");
                fgets(addName, 256, stdin);
                deleteNsymbol(addName, 256);

                Variable* tempAddVar = LookForVar(var_s, addName);

                if (tempAddVar)
                {
                    printf("\n���������� ��� ����������\n\n");
                    break;
                }

                printf("�� ����������� ������:\n 1) ������������ �������������\n 2) ����� ���������� �������� �������\n");

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
                    printf("���-�� �� ��...\n");
                    break;
                }

                char newAddVarValue[256];
                char* newAddVarTemp;


                if(choiceNumDigit2 == 1)
                {
                    printf("������� �������� ����������:\n");
                    fgets(newAddVarValue, 255, stdin);
                    deleteNsymbol(newAddVarValue, 255);

                    newAddVarTemp = convertCekNumberToDecimal(newAddVarValue);
                }
                else if(choiceNumDigit2 == 2)
                {
                    printf("������� �������� ����������:\n");
                    fgets(newAddVarValue, 255, stdin);
                    deleteNsymbol(newAddVarValue, 255);

                    newAddVarTemp = romanToDecimal(newAddVarValue);
                }
                else
                {
                    printf("���-�� �� ��...\n");
                    break;
                }

                if (newAddVarTemp == NULL)
                {
                    printf("���-�� �� ��...\n");
                    break;
                }

                char* newAddVarRes = convertBase(newAddVarTemp, base_assign);

                InsertVar(var_s, addName, addName, newAddVarRes);

                printf("\n���������� �������\n\n");

                break;

            case 5 :
                char deleteVarName[256];
                printf("������� ��� ����������:\n");
                fgets(deleteVarName, 256, stdin);

                deleteNsymbol(deleteVarName, 256);

                Variable* deleteTempVar = LookForVar(var_s, deleteVarName);

                if (deleteTempVar)
                {
                    DeleteOnlyVar(var_s, deleteVarName);
                    printf("\n���������� �������\n\n");

                }
                else
                {
                    printf("\n���������� �� �������\n\n");
                }

                break;

            case 6 :
                return var_s;

            case 7 :
                return NULL;

            default:
                printf("���-�� �� ��...\n");
                break;
        }
    }
}



#endif