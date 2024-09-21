#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "H_extraStuff.h"
#include "H_expression.h"
#include "H_operations.h"
#include "H_arithmetic.h"
#include "H_binaryFunction.h"
#include "H_variable.h"
#include "H_Debug.h"

int main(const int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    if (argc <= 2) {
        printf("Неверное кол-во аргументов для программы\n");
        return PROBLEM;
    }

    Operations op;

    Create(&op, "T_operations.txt");
    if (op.nextOp == NULL) {
        printf("Проверьте файл с базовыми операциями\n");
        return PROBLEM;
    }

    int isMayDebug = 0;
    int output_base = 10;
    int input_base = 10;
    int assign_base = 10;

    if (argc > 3)
    {
        if (argc == 4)
        {
            if (isDebug(argv[3]))
            {
                isMayDebug = 1;
            }
            else
            {
                assign_base = atoi(argv[3]);
            }
        }
        if (argc == 5)
        {
            if (isDebug(argv[3]))
            {
                isMayDebug = 1;
                assign_base = atoi(argv[4]);
            }
            else
            {
                assign_base = atoi(argv[3]);
            }

            if (isDebug(argv[4]))
            {
                isMayDebug = 1;
            }
            else return PROBLEM;
        }

        if (argc == 6)
        {
            if (isDebug(argv[3]))
            {
                isMayDebug = 1;
                assign_base = atoi(argv[4]);
                input_base = atoi(argv[5]);
            }
            else
            {
                assign_base = atoi(argv[3]);
            }

            if (isDebug(argv[4]))
            {
                isMayDebug = 1;
                input_base = atoi(argv[5]);
            }
            else
            {
                input_base = atoi(argv[4]);
            }

            if (isDebug(argv[5]))
            {
                isMayDebug = 1;
            }
            else return PROBLEM;
        }

        if (argc == 7)
        {
            if (isDebug(argv[3]))
            {
                isMayDebug = 1;
                assign_base = atoi(argv[4]);
                input_base = atoi(argv[5]);
                output_base = atoi(argv[6]);

            }
            else
            {
                assign_base = atoi(argv[3]);
            }

            if (isDebug(argv[4]))
            {
                isMayDebug = 1;
                input_base = atoi(argv[5]);
                output_base = atoi(argv[6]);
            }
            else
            {
                input_base = atoi(argv[4]);
            }

            if (isDebug(argv[5]))
            {
                isMayDebug = 1;
                output_base = atoi(argv[6]);
            }
            else
            {
                output_base = atoi(argv[5]);
            }

            if (isDebug(argv[6]))
            {
                isMayDebug = 1;
            }
            else return PROBLEM;
        }
        else return PROBLEM;
    }

    FILE* reserveSettingsFILE = fopen("reserveSettings.txt", "r");

    FILE* settingsFILE = fopen(argv[1], "r");

    if(reserveSettingsFILE && settingsFILE)
    {
        printf("Основной файл настроек рабоает, проигнорировать? Да(1), Нет(любая клавиша)\n");
        char symbol[256];
        symbol[1] = '\0';
        fgets(symbol, 255, stdin);

        if(strcmp(symbol, "1\n") == 0)
        {
            fclose(settingsFILE);
            settingsFILE = reserveSettingsFILE;
            printf("Проигнорирован\n");
        }
        else
        {
            printf("Не проигнорирован\n");
            copyFile(argv[1], "reserveSettings.txt");
            fclose(reserveSettingsFILE);
        }
    }
    else if (settingsFILE)
    {
        copyFile(argv[1], "reserveSettings.txt");
    }
    else if (reserveSettingsFILE)
    {
        settingsFILE = reserveSettingsFILE;
    }
    else
    {
        printf("Файлы настроек требуют внимания\n");
        return PROBLEM;
    }

    const size_t lineSize = 256;

    char* line = (char*)malloc(sizeof(char) * lineSize);
    if (line == NULL)
    {
        fclose(settingsFILE);
        printf("Ошибка выделения памяти line\n");
        return PROBLEM;
    }
    memset(line, '\0', sizeof(line));

    char* synonim = (char*)malloc(sizeof(char) * lineSize);
    if (synonim == NULL)
    {
        free(line);
        fclose(settingsFILE);
        printf("Ошибка выделения памяти synonim\n");
        return PROBLEM;
    }
    memset(synonim, '\0', sizeof(synonim));


    int i = 0;
    int k = 0;
    int isSynonim = 0;
    int isComment = 0;
    int opView = 0;
    int expView = 0;
    int symbol = fgetc(settingsFILE);

    Variable* mainVar_s = CreateVar();
    // Variable** ptr2Var_s = &mainVar_s;

    while (symbol != EOF)
    {
        if (symbol == ' ') {
            if (i == 0)
            {
                while((symbol = fgetc(settingsFILE)) != '\n' && symbol != EOF)
                {
                    if (symbol == '#' && isComment != 2) isComment = 1;
                    else if (symbol == '[' && isComment != 1) isComment = 2;
                    else if (symbol != ' ' && isComment == 0) {
                        printf("Герметичность комментария нарушена в файле Настроек - 1\n");
                        return PROBLEM;
                    }
                }

                if (isComment == 1) isComment = 0;
                if (symbol == EOF) break;
                symbol = fgetc(settingsFILE);
            }
            else if (k == 0 && i > 0) isSynonim = 1;
            else {
                Change(&op, line, synonim);

                for (int j = 0; j <= i; ++j)
                {
                    line[j] = '\0';
                }
                i = 0;

                for (int j = 0; j <= k; ++j)
                {
                    synonim[j] = '\0';
                }
                k = 0;

                isSynonim = 0;

                while((symbol = fgetc(settingsFILE)) != '\n' && symbol != EOF)
                {
                    if (symbol == '#' && isComment != 2) isComment = 1;
                    else if (symbol == '[' && isComment != 1) isComment = 2;
                    else if (symbol != ' ' && isComment == 0) {
                        printf("Герметичность комментария нарушена в файле Настроек - 2\n");
                        return PROBLEM;
                    }
                }

                if (isComment == 1) isComment = 0;
                if (symbol == EOF) break;
            }

        }
        else if (symbol == '\n')
        {
            if (k > 0 && i > 0)
            {
                Change(&op, line, synonim);
            }

            for (int j = 0; j <= i; ++j)
            {
                line[j] = '\0';
            }
            i = 0;

            for (int j = 0; j <= k; ++j)
            {
                synonim[j] = '\0';
            }
            k = 0;
            isSynonim = 0;
        }
        else if (symbol == '#')
        {
            if (k > 0 || i > 0) {
                printf("Герметичность комментария нарушена в файле Настроек - 3\n");
                return PROBLEM;
            }

            while((symbol = fgetc(settingsFILE)) != '\n' && symbol != EOF) {}

            isComment = 0;
            if (symbol == EOF) break;

        }
        else if (symbol == '[')
        {
            if (k > 0 || i > 0) {
                printf("Герметичность комментария нарушена в файле Настроек - 4\n");
                return PROBLEM;
            }

            while((symbol = fgetc(settingsFILE)) != ']' && symbol != EOF) {}

            if (symbol == EOF && isComment > 0) {
                printf("Герметичность комментария нарушена в файле Настроек - не хватает ']'\n");
                return PROBLEM;
            }
            if (symbol == EOF) break;
            isComment = 0;

            symbol = fgetc(settingsFILE);
        }

        if (isComment != 2 && symbol != ' ' && symbol != '\n')
        {
            if(isSynonim == 0)
            {
                line[i] = symbol;
                i++;
            }
            else {
                synonim[k] = symbol;
                k++;
            }
        }

        if (symbol == ')' || symbol == '=' && i > 2)
        {
            if(strcmp(line, "op()") == 0) opView = OP__;
            else if(strcmp(line, "()") == 0)
            {
                if('o' == fgetc(settingsFILE) && 'p' == fgetc(settingsFILE)) {
                    opView = __OP;
                }
            }
            else if(strcmp(line, "(op)") == 0) opView = _OP_;
            else if(strcmp(line, "right=") == 0) expView = RIGHT;
            else if(strcmp(line, "left=") == 0) expView = LEFT;
            else if(strcmp(line, "=") == 0) {}
            else {
                printf("Проверьте флаги настройки внешнего вида\n");
                return PROBLEM;
            }

            if (i > 1)
            {
                for (int j = 0; j <= i; ++j)
                {
                    line[j] = '\0';
                }
                i = 0;
            }
        }

        symbol = fgetc(settingsFILE);
    }

    free(line);
    fclose(settingsFILE);

    //-----------------------------------------------------------------------------------------------------------------------


    FILE* file1 = fopen(argv[2], "r");
    if (file1 == NULL) {
        printf("Ошибка с файлом программы\n");
        return PROBLEM;
    }

    FILE* file2 = fopen("log.txt", "w");
    if (file2 == NULL) {
        printf("Ошибка с log.txt\n");
        return PROBLEM;
    }

    i = 0;
    line = (char*)malloc(sizeof(char) * lineSize);
    memset(line, '\0', sizeof(line));

    BST expression;
    expression.parent = NULL;
    expression.rightSubtree = NULL;
    expression.leftSubtree = NULL;
    BST* root = &expression;
    BST* tempRoot = &expression;

    symbol = fgetc(file1);
    int lineNumber = 1;


    if(opView == 0)
    {
        int bracketsChecker = 0;
        int leftRightChecker = 0;

        while(symbol != EOF)
        {
            if (symbol == '#')
            {
                if (tempRoot->parent != NULL || tempRoot->leftSubtree != NULL || tempRoot->rightSubtree != NULL || strlen(line) != 0)
                {
                    return PROBLEM;
                }

                int BREAKPOINTRcheckSize = 11;
                char* curComment = malloc(sizeof(char) * BREAKPOINTRcheckSize);
                memset(curComment, '\0', BREAKPOINTRcheckSize * sizeof(char));
                BREAKPOINTRcheckSize = 0;

                while (symbol != '\n')
                {
                    if (symbol != '#' && symbol != ' ' && symbol != '\t' && BREAKPOINTRcheckSize < 11)
                    {
                        curComment[BREAKPOINTRcheckSize] = symbol;
                        ++BREAKPOINTRcheckSize;
                    }

                    symbol = fgetc(file1);
                }

                if (strcmp("BREAKPOINT", curComment) == 0)
                {
                    mainVar_s = debugProcessing(mainVar_s, assign_base);
                    if (mainVar_s == NULL) return OK;
                }

                free(curComment);
            }
            else if (symbol == '[')
            {
                if(tempRoot->parent != NULL && tempRoot->leftSubtree != NULL && tempRoot->rightSubtree != NULL)
                {
                    printf("Комментарий в ненужном месте - 1\n");
                    return PROBLEM;
                }

                while (symbol != ']' && symbol != EOF)
                {
                    symbol = fgetc(file1);
                }

                if (symbol == EOF) {
                    printf("Комментарий в ненужном месте - добавьте ']'\n");
                    return PROBLEM;
                }
            }
            else if ((symbol == ' ' && i != 0) || symbol == '(' || symbol == ',' || symbol == ')' || symbol == ';' || symbol == '\n')
            {
                if(strlen(line) > 0 && strcmp(line, ";") != 0)
                {
                    root = BSTinsert(root, line, i);
                    ++leftRightChecker;

                    if (leftRightChecker == 2 && strcmp(root->key, "=") == 0 && expView == 1) {
                        printf("Необходим флаг left= - 1\n");
                        return PROBLEM;
                    }
                    if (leftRightChecker == 2 && strcmp(root->key, "=") != 0 && expView == 0) {
                        printf("Необходим флаг right= - 1\n");
                        return PROBLEM;
                    }
                }

                if (root != tempRoot && root != tempRoot->leftSubtree)
                {
                    if (tempRoot->leftSubtree == NULL)
                    {
                        tempRoot->leftSubtree = root;
                    }
                    else if (tempRoot->rightSubtree == NULL)
                    {
                        tempRoot->rightSubtree = root;
                    }

                    root->parent = tempRoot;
                }

                if (symbol == '(')
                {
                    root = root->leftSubtree;

                    if (tempRoot->rightSubtree)
                    {
                        tempRoot = tempRoot->rightSubtree;
                    }
                    else if (tempRoot->leftSubtree)
                    {
                        tempRoot = tempRoot->leftSubtree;
                    }

                    ++bracketsChecker;
                }
                else if (symbol == ',')
                {
                    root = root->parent->rightSubtree;
                }
                else if (symbol == ')')
                {
                    root = root->parent;
                    if(tempRoot->parent)
                    {
                        tempRoot = tempRoot->parent;
                    }

                    --bracketsChecker;
                    if(bracketsChecker == 0 && tempRoot->parent == NULL && tempRoot->rightSubtree != NULL && tempRoot->leftSubtree != NULL)
                    {
                        root = root->parent;
                    }
                }
                else if (symbol == ';')
                {
                    lineNumber++;

                    if(root->leftSubtree == NULL && root->rightSubtree == NULL && tempRoot->parent == NULL && strcmp(tempRoot->key, "=") == 0)
                    {
                        tempRoot->rightSubtree = NULL;
                        root->leftSubtree = tempRoot;
                        root->parent = NULL;
                        tempRoot->parent = root;
                    }

                    Variable* s = treeProcessing(&op, tempRoot, mainVar_s, assign_base, input_base, output_base);
                    if (s == NULL)
                    {
                        printf("Ошибка в обработке выражения - 1 (%d)\n", lineNumber);
                        return PROBLEM;
                    }

                    BSTfree(expression.leftSubtree);

                    expression.parent = NULL;
                    expression.rightSubtree = NULL;
                    expression.leftSubtree = NULL;
                    root = &expression;
                    tempRoot = &expression;
                }
                else if (symbol == '\n') {}
                else if (root != NULL && i != 0)
                {
                    if(strcmp(root->key, "=") == 0 && root->leftSubtree == NULL && root->rightSubtree == NULL &&
                        tempRoot->parent == NULL && tempRoot->leftSubtree != NULL && tempRoot->rightSubtree != NULL)
                    {
                        root->leftSubtree = tempRoot;
                        root->parent = NULL;
                        tempRoot->parent = root;
                        root = root->parent;
                        tempRoot = tempRoot->parent;
                    }
                    else
                    {
                        root = root->leftSubtree;

                        if (tempRoot->rightSubtree)
                        {
                            tempRoot = tempRoot->rightSubtree;
                        }
                        else if (tempRoot->leftSubtree)
                        {
                            tempRoot = tempRoot->leftSubtree;
                        }
                    }
                }

                for (int j = 0; j <= i; ++j)
                {
                    line[j] = '\0';
                }
                i = 0;

            }

            if (symbol != ' ' && symbol != '\n' && symbol != '(' && symbol != ')' && symbol != ',' && symbol != ']' && symbol != ';')
            {
                line[i] = symbol;
                i++;
            }

            symbol = fgetc(file1);
        }
    }
    else if (opView == 1)
    {
        int flowChecker = 1;
        int leftRightChecker = 0;

        while(symbol != EOF)
        {
            if (symbol == '#')
            {
                if (tempRoot->parent != NULL || tempRoot->leftSubtree != NULL || tempRoot->rightSubtree != NULL || strlen(line) != 0)
                {
                    printf("Комментарий в ненужном месте - 2.1\n");
                    return PROBLEM;
                }

                int BREAKPOINTRcheckSize = 11;
                char* curComment = malloc(sizeof(char) * BREAKPOINTRcheckSize);
                memset(curComment, '\0', BREAKPOINTRcheckSize * sizeof(char));
                BREAKPOINTRcheckSize = 0;

                while (symbol != '\n')
                {
                    if (symbol != '#' && symbol != ' ' && symbol != '\t' && BREAKPOINTRcheckSize < 11)
                    {
                        curComment[BREAKPOINTRcheckSize] = symbol;
                        ++BREAKPOINTRcheckSize;
                    }

                    symbol = fgetc(file1);
                }

                if (strcmp("BREAKPOINT", curComment) == 0)
                {
                    mainVar_s = debugProcessing(mainVar_s, assign_base);
                    if (mainVar_s == NULL) return OK;
                }

                free(curComment);
            }
            else if (symbol == '[')
            {
                if(tempRoot->parent != NULL && tempRoot->leftSubtree != NULL && tempRoot->rightSubtree != NULL)
                {
                    printf("Комментарий в ненужном месте - 2.2\n");
                    return PROBLEM;
                }

                while (symbol != ']' && symbol != EOF)
                {
                    symbol = fgetc(file1);
                }

                if (symbol == EOF) {
                    printf("Комментарий в ненужном месте - 2 (закройте '[')\n");
                    return PROBLEM;
                }
            }
            else if ((symbol == ' ' && i != 0) || symbol == ';' || symbol == '\n')
            {
                root = BSTinsert(root, line, i);

                ++leftRightChecker;
                if (leftRightChecker == 2 && strcmp(root->key, "=") == 0 && expView == 1) {
                    printf("Необходим флаг left= - 2\n");
                    return PROBLEM;
                }
                if (leftRightChecker == 2 && strcmp(root->key, "=") != 0 && expView == 0) {
                    printf("Необходим флаг right= - 2\n");
                    return PROBLEM;
                }

                if (root != tempRoot)
                {
                    if (tempRoot->parent != NULL && tempRoot->rightSubtree &&
                        strlen(tempRoot->rightSubtree->key) == 0 && tempRoot->rightSubtree->parent == tempRoot)
                    {
                        tempRoot->rightSubtree = root;
                        root->parent = tempRoot;
                    }
                    else if (strcmp(root->key, "=") != 0 && tempRoot->leftSubtree == NULL && strlen(tempRoot->key) == 0)
                    {
                        tempRoot->leftSubtree = root;
                        root->parent = tempRoot;
                    }
                    else if (strcmp(root->key, "=") != 0 && tempRoot->leftSubtree == NULL)
                    {
                        Operations* tempOp = (&op, root->key);

                        if(tempOp && strcmp(tempOp->name, "input") != 0 && strcmp(tempOp->name, "output") != 0 && expView != 1)
                        {
                            BST* temp = BSTinsert(NULL, "", 1);
                            tempRoot->leftSubtree = temp;
                            temp->parent = tempRoot;
                            temp->leftSubtree = root;
                            root->parent = temp;
                            tempRoot = tempRoot->leftSubtree;
                        }
                        else
                        {
                            root->leftSubtree = tempRoot;
                            tempRoot->parent = root;
                            root = root->leftSubtree;
                            tempRoot = tempRoot->parent;
                            tempRoot->parent = NULL;

                            if (expView == 1)
                            {
                                root = root->parent;
                            }
                        }
                    }
                    else if (tempRoot->rightSubtree == NULL && expView != 1)
                    {
                        tempRoot->rightSubtree = root;
                        root->parent = tempRoot;
                    }
                    else if (tempRoot->leftSubtree == NULL)
                    {
                        tempRoot->leftSubtree = root;
                        root->parent = tempRoot;
                    }
                }

                if (symbol == ' ')
                {
                    if (strcmp(root->key, "=") == 0 && expView == 1)
                    {
                        tempRoot = tempRoot->parent;
                        tempRoot->rightSubtree = tempRoot->rightSubtree->leftSubtree;
                        tempRoot->rightSubtree->parent = tempRoot;

                        while(tempRoot->parent != NULL)
                        {
                            tempRoot = tempRoot->parent;
                        }

                        root->leftSubtree = tempRoot;
                        tempRoot->parent = root;
                        root = NULL;
                        tempRoot = tempRoot->parent;

                        flowChecker--;
                    }
                    else if (strlen(tempRoot->key) == 0)
                    {
                        root = root->parent;
                    }
                    else if (tempRoot->rightSubtree == NULL && tempRoot == root && tempRoot->leftSubtree )
                    {
                        BST* temp = BSTinsert(NULL, "", 1);
                        tempRoot->rightSubtree = temp;
                        temp->parent = tempRoot;
                        root = root->rightSubtree->leftSubtree;
                        tempRoot = tempRoot->rightSubtree;
                    }
                    else if (tempRoot->leftSubtree)
                    {
                        root = root->leftSubtree;

                        if(expView == 1)
                        {}
                        else
                        {
                            tempRoot = tempRoot->leftSubtree;
                        }
                    }
                    else if (tempRoot->rightSubtree == NULL)
                    {
                        root = root->rightSubtree;
                    }
                    else if (root->rightSubtree == NULL && root->parent == tempRoot)
                    {
                        root = root->rightSubtree;
                        tempRoot = tempRoot->rightSubtree;
                    }
                }
                else if (symbol == ';')
                {
                    lineNumber++;

                    if(expView == 1 && flowChecker == 0)
                    {
                        root->leftSubtree = tempRoot;
                        tempRoot->parent = root;
                    }
                    else
                    {
                        if (tempRoot->parent)
                        {
                            tempRoot = tempRoot->parent;
                            tempRoot->rightSubtree = root;
                        }

                        while(strcmp(tempRoot->key, "=") != 0 && tempRoot->parent != NULL)
                        {
                            tempRoot = tempRoot->parent;
                            root = root->parent;
                        }
                    }

                    Variable* s = treeProcessing(&op, tempRoot, mainVar_s, assign_base, input_base, output_base);
                    if (s == NULL)
                    {
                        printf("Ошибка в обработке выражения - 2 (%d)\n", lineNumber);
                        return PROBLEM;
                    }

                    BSTfree(expression.leftSubtree);

                    expression.parent = NULL;
                    expression.rightSubtree = NULL;
                    expression.leftSubtree = NULL;
                    root = &expression;
                    tempRoot = &expression;
                    flowChecker = 1;
                }
                else if (symbol == '\n') {}

                for (int j = 0; j <= i; ++j)
                {
                    line[j] = '\0';
                }
                i = 0;

            }

            if (symbol != ' ' && symbol != '\n' && symbol != ']' && symbol != ';')
            {
                line[i] = symbol;
                i++;
            }

            symbol = fgetc(file1);
        }
    }
    else
    {
        int bracketsChecker = 0;
        int leftRightChecker = 0;

        while(symbol != EOF)
        {
            if (symbol == '#')
            {
                if (tempRoot->parent != NULL || tempRoot->leftSubtree != NULL || tempRoot->rightSubtree != NULL || strlen(line) != 0)
                {
                    return PROBLEM;
                }

                int BREAKPOINTRcheckSize = 11;
                char* curComment = malloc(sizeof(char) * BREAKPOINTRcheckSize);
                memset(curComment, '\0', BREAKPOINTRcheckSize * sizeof(char));
                BREAKPOINTRcheckSize = 0;

                while (symbol != '\n')
                {
                    if (symbol != '#' && symbol != ' ' && symbol != '\t' && BREAKPOINTRcheckSize < 11)
                    {
                        curComment[BREAKPOINTRcheckSize] = symbol;
                        ++BREAKPOINTRcheckSize;
                    }

                    symbol = fgetc(file1);
                }

                if (strcmp("BREAKPOINT", curComment) == 0)
                {
                    mainVar_s = debugProcessing(mainVar_s, assign_base);
                    if (mainVar_s == NULL) return OK;
                }

                free(curComment);
            }
            else if (symbol == '[')
            {
                if(tempRoot->parent != NULL && tempRoot->leftSubtree != NULL && tempRoot->rightSubtree != NULL)
                {
                    return PROBLEM;
                }

                while (symbol != ']' && symbol != EOF)
                {
                    symbol = fgetc(file1);
                }

                if (symbol == EOF) return PROBLEM;
            }
            else if ((symbol == ' ' && i != 0) || symbol == '(' || symbol == ',' || symbol == ')' || symbol == ';' || symbol == '\n')
            {
                root = BSTinsert(root, line, i);

                ++leftRightChecker;
                if (leftRightChecker == 2 && strcmp(root->key, "=") == 0 && expView == 1) {
                    printf("Необходим флаг left= - 3\n");
                    return PROBLEM;
                }
                if (leftRightChecker == 2 && strcmp(root->key, "=") != 0 && expView == 0) {
                    printf("Необходим флаг right= - 3\n");
                    return PROBLEM;
                }

                if (root != tempRoot && root != tempRoot->leftSubtree && strlen(line) != 0)
                {
                    if (tempRoot->leftSubtree == NULL)
                    {
                        tempRoot->leftSubtree = root;
                    }
                    else if (tempRoot->rightSubtree == NULL)
                    {
                        tempRoot->rightSubtree = root;
                    }

                    root->parent = tempRoot;
                }

                if (symbol == '(')
                {
                    if(tempRoot->leftSubtree && bracketsChecker == 0 && strcmp(tempRoot->leftSubtree->key, "=") == 0)
                    {
                        tempRoot = tempRoot->leftSubtree;
                    }

                    //Для одинарных вывода и ввода
                    if(tempRoot == root)
                    {
                        root = root->leftSubtree;
                    }
                    else if(tempRoot->leftSubtree == NULL)
                    {
                        tempRoot->leftSubtree = root;
                        root->parent = tempRoot;
                        root = root->leftSubtree;
                    }
                    else
                    {
                        tempRoot->rightSubtree = root;
                        root->parent = tempRoot;
                        root = root->leftSubtree;
                    }

                    if (tempRoot->rightSubtree)
                    {
                        tempRoot = tempRoot->rightSubtree;
                    }
                    else if (tempRoot->leftSubtree)
                    {
                        tempRoot = tempRoot->leftSubtree;
                    }

                    ++bracketsChecker;
                }
                else if (symbol == ',')
                {
                    root = root->parent->rightSubtree;
                }
                else if (symbol == ')')
                {
                    root = root->parent;
                    if(tempRoot->parent)
                    {
                        tempRoot = tempRoot->parent;
                    }

                    --bracketsChecker;
                }
                else if (symbol == ';')
                {
                    lineNumber++;

                    if(root->leftSubtree == NULL && root->rightSubtree == NULL && tempRoot->parent == NULL && strcmp(tempRoot->key, "=") == 0)
                    {
                        tempRoot->rightSubtree = NULL;
                        root->leftSubtree = tempRoot;
                        root->parent = NULL;
                        tempRoot->parent = root;
                    }

                    Variable* s = treeProcessing(&op, tempRoot, mainVar_s, assign_base, input_base, output_base);
                    if (s == NULL) {
                        printf("Ошибка в обработке выражения - 3 (%d)\n", lineNumber);
                        return PROBLEM;
                    }

                    BSTfree(expression.leftSubtree);

                    expression.parent = NULL;
                    expression.rightSubtree = NULL;
                    expression.leftSubtree = NULL;
                    root = &expression;
                    tempRoot = &expression;
                }
                else if (symbol == '\n') {}
                else if (root != NULL && i != 0)
                {
                    if(strcmp(root->key, "=") == 0 && root->leftSubtree == NULL && root->rightSubtree == NULL &&
                        tempRoot->parent == NULL && tempRoot->leftSubtree != NULL && tempRoot->rightSubtree != NULL)
                    {
                        root->leftSubtree = tempRoot;
                        root->parent = NULL;
                        tempRoot->parent = root;
                        root = root->parent;
                        tempRoot = tempRoot->parent;
                    }
                    else
                    {
                        root = root->leftSubtree;

                        if (tempRoot->rightSubtree)
                        {
                            tempRoot = tempRoot->rightSubtree;
                        }
                        else if (tempRoot->leftSubtree)
                        {
                            tempRoot = tempRoot->leftSubtree;
                        }

                        if(bracketsChecker == 0)
                        {
                            root = NULL;
                            while(tempRoot->parent != NULL)
                            {
                                tempRoot = tempRoot->parent;
                            }
                        }
                    }
                }


                for (int j = 0; j <= i; ++j)
                {
                    line[j] = '\0';
                }
                i = 0;

            }


            if (symbol != ' ' && symbol != '\n' && symbol != '(' && symbol != ')' && symbol != ',' && symbol != ']' && symbol != ';')
            {
                line[i] = symbol;
                i++;
            }

            symbol = fgetc(file1);
        }
    }


    fclose(file1);
    fclose(file2);
    free(line);

    if(root != NULL)
    {
        BSTfree(root);
    }

    return 0;
}
