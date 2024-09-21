#ifndef H_variable
#define H_variable

#include "H_extraStuff.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// typedef struct variable
// {
//     char* name;
//     char value[33];
//     struct variable* nextOp;
// } Variable;

#define ALPHABET_SIZE 63 // A-Z, a-z, 0-9, _

typedef struct variable {
    struct variable* children[ALPHABET_SIZE];
    char* name;
    char value[33];
    bool isEndOfWord;
} Variable;

Variable* CreateVar();
void InsertVar(Variable* root, char* key, char* name, char* value);
void PrintAllVar_s(Variable* root);
Variable* LookForVar(Variable* root, char *key);
void inline DeleteVar_s(Variable* op);
inline Variable* DeleteOnly(Variable* a);

void DeleteVar_s(Variable* op)
{
    for(int i = 0; i < ALPHABET_SIZE; ++i)
    {
        DeleteVar_s(op->children[i]);
        op->children[i] = NULL;
    }


}

Variable* DeleteOnly(Variable* a) {
    for(int i = 0; i < ALPHABET_SIZE; ++i)
    {
        a->children[i] = NULL;
    }

    return NULL;
}

Variable* DeleteOnlyVar(Variable* root, char* key)
{
    Variable *current = LookForVar(root, key);

    current->isEndOfWord = false;

    return root;
}


Variable* CreateVar() {
    Variable *node = (Variable*)malloc(sizeof(Variable));
    node->isEndOfWord = false;
    node->name = NULL;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    return node;
}

void InsertVar(Variable* root, char* key, char* name, char* value) {
    Variable *current = root;

    for (int i = 0; key[i] != '\0'; i++) {
        int index;
        if (isalpha(key[i])) {
            index = tolower(key[i]) - 'a';
            index += 26; // Increment for a-z
        } else if (isdigit(key[i])) {
            index = key[i] - '0';
            index += 52; // Increment for 0-9
        } else if (key[i] == '_') {
            index = 62; // Index for underscore
        }

        if (current->children[index] == NULL) {
            current->children[index] = CreateVar();
        }

        current = current->children[index];
    }

    current->isEndOfWord = true;
    memcpy(current->value, value, strlen(value) * sizeof(char));

    if (current && current->name && strlen(current->name) < strlen(name)) current->name = NULL;

    if(current->name == NULL)
    {
        current->name = (char*)malloc(sizeof(char) * strlen(name));
    }

    memcpy(current->name, name, strlen(name) * sizeof(char));
}

Variable* LookForVar(Variable* root, char *key) {
    if (root == NULL) return NULL;

    Variable *current = root;

    for (int i = 0; key[i] != '\0'; i++) {
        int index;
        if (isalpha(key[i])) {
            index = tolower(key[i]) - 'a';
            index += 26;
        } else if (isdigit(key[i])) {
            index = key[i] - '0';
            index += 52;
        } else if (key[i] == '_') {
            index = 62;
        }

        if (current->children[index] == NULL) {
            return NULL;
        }

        current = current->children[index];
    }

    return (current != NULL && current->isEndOfWord) ? current : NULL;
}

void PrintAllVar_s(Variable* root) {
    if (root)
    {
        for(int i = 0; i < ALPHABET_SIZE; ++i)
        {
            PrintAllVar_s(root->children[i]);
        }

        if(root->isEndOfWord)
        {
            printf("%s = %s\n", root->name, root->value);
        }
    }
}

#endif