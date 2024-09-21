#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct DepartmentArray {
    int capacity;
    int size;
    char** keys;
    otd** values; 
} DepartmentArray;

DepartmentArray* create_department_array(const int initialCapacity) {
    DepartmentArray* depArray = malloc(sizeof(DepartmentArray));
    if (depArray == NULL) return NULL;

    depArray->capacity = initialCapacity;
    depArray->size = 0;
    depArray->keys = (char**)malloc(initialCapacity * sizeof(char*));
    if (depArray->keys == NULL) {
        free(depArray);
        return NULL;
    }

    depArray->values = (otd**)malloc(initialCapacity * sizeof(otd*));
    if (depArray->values == NULL) {
        free(depArray->keys);
        free(depArray);
        return NULL;
    }
    //depArray->least_load = NULL;
    return depArray;
}

enum answer insert_department_array(DepartmentArray* depArray, char* key, otd* value) {
    if (depArray->size == depArray->capacity) {
        depArray->capacity *= 2;

        depArray->keys = (char**)realloc(depArray->keys, depArray->capacity * sizeof(char*));
        if (depArray->keys == NULL) {
            free(depArray->values);
            free(depArray);
            return ERROR_MEMORY;
        }

        depArray->values = (otd**)realloc(depArray->values, depArray->capacity * sizeof(otd*));
        if (depArray->values == NULL) {
            free(depArray->keys);
            free(depArray);
            return ERROR_MEMORY;
        }
    }

    int index = depArray->size;
    while (index > 0 && strcmp(key, depArray->keys[index - 1]) < 0) {
        depArray->keys[index] = depArray->keys[index - 1];
        depArray->values[index] = depArray->values[index - 1];
        index--;
    }
    
    depArray->keys[index] = key;
    depArray->values[index] = value;
    depArray->size++;
    return OK;
}

otd* search_department_array(const DepartmentArray* depArray, const char* key) {
    for (int i = 0; i < depArray->size; ++i) {
        if (strcmp(key, depArray->keys[i]) == 0)
            return depArray->values[i];
    }

    return NULL;
}

void free_department_array(DepartmentArray* depArray, void(*free_heap)(void*, int)) {
    for (int i = 0; i < depArray->size - 1; i++) {
        free(depArray->keys[i]);
        free_heap(depArray->values[i]->heap, 1);
        free(depArray->values[i]->operators);
    }

    free(depArray->keys);
    free(depArray->values);
    free(depArray);
}

