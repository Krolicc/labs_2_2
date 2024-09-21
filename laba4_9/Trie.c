#include <stdlib.h>
#include <string.h>
#include "structures.h"

#define DIGIT_SIZE 10

typedef struct TrieNode {
    struct TrieNode* children[DIGIT_SIZE];
    otd* value;
    int isEndOfNumber;
} trie_node;

trie_node* create_node_trie() {
    trie_node* node = malloc(sizeof(trie_node));
    if (node != NULL) {
        node->isEndOfNumber = 0;
        node->value = NULL;
        for (int i = 0; i < DIGIT_SIZE; i++) {
            node->children[i] = NULL;
        }
    }

    return node;
}

void insert_trie(trie_node* root, const char* key, otd* value) {
    trie_node* curr = root;
    const int len = strlen(key);

    for (int i = 0; i < len; ++i) {
        const int index = key[i] - '0';
        if (curr->children[index] == NULL) {
            curr->children[index] = create_node_trie();
            if (curr->children[index] == NULL) {
                printf("Ошибка выделения памяти\n");
                return;
            }
        }
        curr = curr->children[index];
    }

    curr->isEndOfNumber = 1;
    curr->value = value;
}

otd* search_trie(const trie_node* root, const char* key) {
    const trie_node* curr = root;
    const int len = strlen(key);
    for (int i = 0; i < len; i++) {
        const int index = key[i] - '0';
        if (curr->children[index] == NULL) return NULL;
        curr = curr->children[index];
    }
    if (curr != NULL && curr->isEndOfNumber) return curr->value;

    return NULL;
}

void free_trie(trie_node* node, void(*free_heap)(void*, int)) {
    if (node == NULL) return;

    for (int i = 0; i < DIGIT_SIZE; i++) {
        if (node->children[i]) {
            free_trie(node->children[i], free_heap);
            node->children[i] = NULL;
        }
    }

    if (node->isEndOfNumber)  {
        free_heap(node->value->heap, 1);
        free(node->value->operators);
    }

    free(node);
}
