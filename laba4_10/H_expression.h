#ifndef H_expression
#define H_expression

#include "H_extraStuff.h"
#include "H_variable.h"
#include "H_arithmetic.h"
#include "H_operations.h"

typedef struct bst {
    char* key;
    struct bst* parent;
    struct bst* leftSubtree;
    struct bst* rightSubtree;
} BST;

BST* BSTinsert(BST* bst, const char* string, const int stringSize) {
    if (bst == NULL)
    {
        bst = malloc(sizeof(BST));
        if (bst == NULL)
        {
            return NULL;
        }

        bst->key = (char*)malloc(sizeof(char) * stringSize);
        if (bst->key == NULL)
        {
            return NULL;
        }
        memset(bst->key, '\0', strlen(bst->key) * sizeof(char));


        bst->leftSubtree = NULL;
        bst->rightSubtree = NULL;
    }

    bst->key = (char*)malloc(sizeof(char) * stringSize + 1);
    if (bst->key == NULL)
    {
        return NULL;
    }
    memset(bst->key, '\0', stringSize * sizeof(char) + 1);
    memcpy(bst->key, string, stringSize * sizeof(char));

    return bst;
}

void BSTfree(BST* bst)
{
    if (bst != NULL) {
        if(bst->rightSubtree)
        {
            BSTfree(bst->rightSubtree);
        }
        if(bst->leftSubtree)
        {
            BSTfree(bst->leftSubtree);
        }

        bst->key = NULL;
        bst->parent = NULL;
        bst->rightSubtree = NULL;
        bst->leftSubtree = NULL;

        bst = NULL;
    }
}

void BSTchange(BST* bst, char* value)
{
    free(bst->key);
    bst->key = malloc(sizeof(char) * (strlen(value) + 1));
    memset(bst->key, '\0', sizeof(char) * (strlen(value) + 1));
    memcpy(bst->key, value, sizeof(char) * (strlen(value) + 1));
}

Variable* treeProcessing(Operations* opers, BST* bst, Variable* variables, int base_assign, int base_input, int base_ouput)
{
    int a;
    if (bst->leftSubtree)
    {
        Variable* tempA = treeProcessing(opers, bst->leftSubtree, variables, base_assign, base_input, base_ouput);
        if (tempA == NULL) return NULL;
    }
    if (bst->rightSubtree)
    {
        Variable* tempB = treeProcessing(opers, bst->rightSubtree, variables, base_assign, base_input, base_ouput);
        if (tempB == NULL) return NULL;
    }

    if(bst->leftSubtree && bst->rightSubtree)
    {
        char* a;
        char* b;
        char* op;
        Operations* temp = LookFor(opers, bst->key);

        if (isCorrectNumber(bst->leftSubtree->key, base_assign))
        {
            a = bst->leftSubtree->key;
        }
        else
        {
            Variable* tempVar = LookForVar(variables, bst->leftSubtree->key);
            if (tempVar)
            {
                a = tempVar->value;
            }
            else
            {
                printf("Неверная переменная a\n");
                return NULL;
            }
        }

        if (isCorrectNumber(bst->rightSubtree->key, base_assign))
        {
            b = bst->rightSubtree->key;
        }
        else
        {
            Variable* tempVar = LookForVar(variables, bst->leftSubtree->key);
            if (tempVar)
            {
                b = tempVar->value;
            }
            else
            {
                printf("Неверная переменная b\n");
                return NULL;
            }
        }

        if (temp)
        {
            op = temp->name;
        }
        else
        {
            return NULL;
        }

        char extr[3] = "\0\0\0";
        itoa(base_assign, extr, 10);

        bst->key = regulator(a, b, op, extr);
        if (bst->key == NULL) {
            printf("\nПроверьте числа!\n\n");
            return NULL;
        }

        bst->key = convertBase(bst->key, base_assign);

        BSTfree(bst->leftSubtree);
        BSTfree(bst->rightSubtree);
        bst->leftSubtree = NULL;
        bst->rightSubtree = NULL;
    }

    if(strcmp(bst->key, "=") == 0)
    {
        Variable* tempVar = LookForVar(variables, bst->leftSubtree->key);
        if (tempVar)
        {
            bst->leftSubtree->key = tempVar->value;
        }

        if(isValidVariableName(bst->parent->key) == 0) return NULL;

        InsertVar(variables, bst->parent->key, bst->parent->key, bst->leftSubtree->key);
    }
    else if (strcmp(bst->key, "output") == 0)
    {
        Variable* tempVar = LookForVar(variables, bst->leftSubtree->key);

        char extr[3] = "\0\0\0";
        itoa(base_assign, extr, 10);

        if (tempVar && bst->leftSubtree)
        {
            bst->leftSubtree->key = tempVar->value;

            bst->leftSubtree->key = convertToDecimal(bst->leftSubtree->key, extr);
            bst->leftSubtree->key = convertBase(bst->leftSubtree->key, base_ouput);

            printf("%s\n", bst->leftSubtree->key);
        }
        else if (bst->leftSubtree && isNumber(bst->leftSubtree->key))
        {
            bst->leftSubtree->key = convertToDecimal(bst->leftSubtree->key, extr);
            bst->leftSubtree->key = convertBase(bst->leftSubtree->key, base_ouput);
            printf("%s\n", bst->leftSubtree->key);
        }
        else
        {
            if (bst->leftSubtree)
            {
                printf("Вывод переменной %s не выполнен: переменная с таким именем не найдена и она не является числом\n", bst->leftSubtree->key);
            }
            else
            {
                printf("Непредвиденный результат\n");
            }
        }

        // return NULL;
    } else if (strcmp(bst->key, "input") == 0)
    {
        char* tempNum = malloc(sizeof(char) * 33);
        if (tempNum == NULL) return NULL;
        memset(tempNum, '\0', 33 * sizeof(char));
        fgets(tempNum, 32, stdin);

        deleteNsymbol(tempNum,32);

        char extr[3] = "\0\0\0";
        itoa(base_assign, extr, 10);

        char* val = convertToDecimal(tempNum, extr);
        val = convertBase(val, base_assign);

        Variable* tempVar = LookForVar(variables, bst->leftSubtree->key);

        if (tempVar)
        {
            for(int i = 0; i < 33; ++i) {
                tempVar->value[i] = val[i];
            }
            // return NULL;
        }

        InsertVar(variables, bst->leftSubtree->key, bst->leftSubtree->key, val);
    }

    // return NULL;
}

#endif