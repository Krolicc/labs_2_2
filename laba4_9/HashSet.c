#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct Macros {
    char* key;
    otd* value;
    unsigned long hash;
    struct Macros* next;
} macros;

typedef struct HashTable {
    macros** items;
    int size;
} hashtable;

hashtable* create_hash_table() {
	hashtable* table = malloc(sizeof(hashtable));

	if (table == NULL) {
        return NULL;
	}

	table->size = 10;
	table->items = (macros**)malloc(sizeof(macros*) * 10);

	if (table->items == NULL) {
		free(table->items);
		table->items = NULL;
        return NULL;
	}

	for (int i = 0; i < table->size; i++) {
		table->items[i] = NULL;
	}

	return table;
}

unsigned long hash(const char *str) {
    //Для каждого символа происходит следующее:
    //1. Текущий value умножается на 32 (сдвиг влево на 5 битов эквивалентен умножению на 32) с последующим добавлением значения value.
    //2. Затем к результату добавляется ASCII-код очередного символа.
    unsigned long hash = 5381;
    //Значение 5381 в данной функции хеш-функции часто используется в качестве начального значения, так как оно является простым числом и обладает свойствами, 
    //которые делают его хорошим выбором для хеширования. Это число хорошо распределяет значения по битовому пространству и хорошо работает для большинства 
    //типов данных, что обычно чрезвычайно важно для хеширования.
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash;
}

enum answer insertMacro(const hashtable* hash_table, const char *key, otd *value) {
    const unsigned long h_value = hash(key);
    const unsigned long h = h_value % hash_table->size;
    macros* newMacro = malloc(sizeof(macros));
    if (newMacro == NULL) return ERROR_MEMORY;

    char* newKey = malloc(strlen(key) + 1);
    if (newKey == NULL) {
        free(newMacro);
        return ERROR_MEMORY;
    }

    strcpy(newKey, key);

    newMacro->hash = h_value;
    newMacro->key = newKey;
    newMacro->value = value;

    newMacro->next = hash_table->items[h];
    hash_table->items[h] = newMacro;

    free(newMacro);

    return OK;
}

otd* searchMacro(const hashtable *hash_table, const char *key) {
    const unsigned long h_value = hash(key);
    const unsigned long h = h_value % hash_table->size;
    const macros *mac = hash_table->items[h];
    while(mac != NULL) {
        if(strcmp(mac->key, key) == 0) return mac->value;
        mac = mac->next;
    }
    return NULL;
}

void free_table(hashtable* my_table, void(*free_heap)(void*, int)) {
    for (int i = 0; i < my_table->size; i++) {
        if (my_table->items[i] != NULL) {
            macros* current = my_table->items[i];
            while(current != NULL) {
                macros* prev = current;
                current = current->next;
                free(prev->key);
                free_heap(prev->value->heap, 1);
                free(prev->value->operators);
                free(prev);
            }
            //free_heap(current->value->heap,1);
            //free(current->value->operators);
            free(current);
        }
    }
    free(my_table->items);
    free(my_table);
}