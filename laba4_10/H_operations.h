#ifndef H_operations
#define H_operations

#include "H_extraStuff.h"

typedef struct operationName
{
    char name[7];
    char synonimName[256];
    int isBinary;
    struct operationName* nextOp;
} Operations;

void Create(Operations* root, const char* file);
Operations* Insert(Operations** op, char* opName, char* synonimName, char isBinary);
Operations* Change(Operations* op, char* opName, char* synonimName);
Operations* LookFor(Operations* op, char* opName);
void inline Delete(Operations* op);


void Create(
    Operations* root,
    const char* file)
{
    FILE* initOperations = fopen(file, "r");
    if (initOperations == NULL) return;

    Operations* ptrRoot = root;

    char sometxt[] = "\0\0\0\0\0\0";
    char* opName = sometxt;

    int i = 0;
    int ch = fgetc(initOperations);
    char isBinary;

    while(ch != EOF)
    {
        if(ch == ' ') {
            isBinary = (char)fgetc(initOperations);
            char some[] = " ";
            char* ptrSome = some;
            root = Insert(&root, sometxt, ptrSome, isBinary);
            if (ptrRoot != root)
            {
                ptrRoot->nextOp = root;
                ptrRoot = ptrRoot->nextOp;
            }
            root = root->nextOp;

            for (int j = i; j >= 0; --j)
            {
                if(opName != NULL)
                {
                    *opName = '\0';
                    if(j != 0)
                        --opName;
                }
            }

            i = 0;

            while ((isBinary = fgetc(initOperations)) != '\n' && isBinary != EOF) {}

            if(isBinary == EOF) return;

            ch = fgetc(initOperations);
        }

        if(opName != NULL)
        {
            *opName = (char)ch;
            ++opName;
        }
        i++;
        ch = fgetc(initOperations);
    }

    fclose(initOperations);
}


inline Operations* Insert(
    Operations** op,
    char* opName,
    char* synonimName,
    char isBinary)
{
    if (*op == NULL)
    {
        *op = malloc(sizeof(Operations));
        if (*op == NULL) return NULL;
    }

    memset((*op)->name, '\0', sizeof((*op)->name));
    memcpy((*op)->name, opName, strlen(opName) * sizeof(char));
    memset((*op)->synonimName, '\0', sizeof((*op)->synonimName));
    memcpy((*op)->synonimName, synonimName, strlen(synonimName) * sizeof(char));
    (*op)->isBinary = isBinary - '0';
    (*op)->nextOp = NULL;

    return *op;
}

inline Operations* Change(
    Operations* op,
    char* opName,
    char* synonimName)
{
    if (op == NULL) return NULL;

    while(strcmp(opName, op->name) != 0)
    {
        op = op->nextOp;
    }

    memcpy(op->synonimName, synonimName, strlen(synonimName) * sizeof(char));

    return op;
}

inline Operations* LookFor(
    Operations* op,
    char* opName)
{
    if (op == NULL) return NULL;

    while(op && strcmp(opName, op->name) != 0 && strcmp(opName, op->synonimName) != 0)
    {
        op = op->nextOp;
    }

    return op;
}

void Delete(Operations* op)
{
    while (op != NULL)
    {
        Operations* temp = op;
        free(op->name);
        free(op->synonimName);
        op = op->nextOp;
        free(temp);
    }
}

#endif