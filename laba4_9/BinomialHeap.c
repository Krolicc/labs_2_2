#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct Node {
    application* data;
    int degree;
    struct Node* child;
    struct Node* sibling;
    struct Node* parent;
} node;

typedef struct BinomialHeap {
    node* head;
} binomial_heap;


binomial_heap* createBinomaialHeap() {
    binomial_heap* bh = malloc(sizeof(binomial_heap));
    if (bh == NULL) return NULL;

    bh->head = NULL;
    return bh;
}

node* createNode(application* data) {
    node* new_node = malloc(sizeof(node));
    if (new_node == NULL) return NULL;

    new_node->data = data;
    new_node->degree = 0;
    new_node->child = NULL;
    new_node->sibling = NULL;
    new_node->parent = NULL;
    return new_node;
}

void freeNode(node* node, const int isFull) {
    if (node == NULL) return;

    freeNode(node->child, isFull);
    freeNode(node->sibling, isFull);
    if (node->data) free_application(node->data);
    free(node);
}

void freeHeapBinomial(void* temp_heap, const int isFull) {
    binomial_heap* heap = (binomial_heap*)temp_heap;
    if (heap == NULL) return;

    freeNode(heap->head, isFull);
    heap->head = NULL;
    free(heap);
}

node* mergeTrees(node* tree1, node* tree2) {
    if (tree1 == NULL)  return tree2;
    if (tree2 == NULL) return tree1;

    if (tree1->data->prior < tree2->data->prior || tree1->data->prior == tree2->data->prior &&
        strcmp(tree1->data->time_app, tree2->data->time_app) > 0) {
        node* temp = tree1;
        tree1 = tree2;
        tree2 = temp;
    }

    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;

    return tree1;
}

node* mergeHeaps_without_destr_in(node* heap1, node* heap2) {
    if (heap1 == NULL) return heap2;
    if (heap2 == NULL) return heap1;

    node* mergedHeap = NULL;
    node** lastPtr = &mergedHeap;
    node** heap1Ptr = &heap1;
    node** heap2Ptr = &heap2;

    while (*heap1Ptr && *heap2Ptr)
    {
        if (*heap1Ptr == NULL) {
            *lastPtr = *heap2Ptr;
            heap2Ptr = &(*heap2Ptr)->sibling;
        } else if (*heap2Ptr == NULL) {
            *lastPtr = *heap1Ptr;
            heap1Ptr = &(*heap1Ptr)->sibling;
        } else if ((*heap1Ptr)->degree > (*heap2Ptr)->degree)
        {
            node* some = *heap2Ptr;
            *lastPtr = *heap1Ptr;
            heap2Ptr = &some;
            heap1Ptr = &(*heap1Ptr)->sibling;
        } else if ((*heap1Ptr)->degree == (*heap2Ptr)->degree)
        {
            if((*heap1Ptr)->data->prior > (*heap2Ptr)->data->prior ||
                (*heap1Ptr)->data->prior == (*heap2Ptr)->data->prior &&
                    strcmp((*heap1Ptr)->data->time_app, (*heap2Ptr)->data->time_app) > 0) {
                node* some = *heap2Ptr;
                *lastPtr = *heap1Ptr;
                heap2Ptr = &some;
                heap1Ptr = &(*heap1Ptr)->sibling;
            } else {
                node* some = *heap1Ptr;
                *lastPtr = *heap2Ptr;
                heap1Ptr = &some;
                heap2Ptr = &(*heap2Ptr)->sibling;
            }
        } else
        {
            node* some = *heap1Ptr;
            *lastPtr = *heap2Ptr;
            heap1Ptr = &some;
            heap2Ptr = & (*heap2Ptr)->sibling;
        }

        lastPtr = &(*lastPtr)->sibling;
    }

    if (*heap1Ptr == NULL)
        *lastPtr = *heap2Ptr;
    else
        *lastPtr = *heap1Ptr;

    lastPtr = &mergedHeap;
    int isCorrectHeap = 1;
    while (*lastPtr && (*lastPtr)->sibling)
    {
        if((*lastPtr)->data->prior < (*lastPtr)->sibling->data->prior ||
            (*lastPtr)->data->prior == (*lastPtr)->sibling->data->prior &&
            strcmp((*lastPtr)->data->time_app, (*lastPtr)->sibling->data->time_app) > 0)
        {
            application* some = (*lastPtr)->data;
            (*lastPtr)->data = (*lastPtr)->sibling->data;
            (*lastPtr)->sibling->data = some;
            isCorrectHeap = 0;
        }

        lastPtr = &(*lastPtr)->sibling;
        if(isCorrectHeap == 0 && *lastPtr == NULL) {
            lastPtr = &mergedHeap;
            isCorrectHeap = 1;
        }
    }

    node* curr = mergedHeap;
    int correctMerge = 1;

    while (correctMerge == 0 || curr && curr->sibling) {
        if(correctMerge == 0) {
            correctMerge = 1;
            curr = mergedHeap;
        }

        node *next = curr->sibling;

        if(next)
            if (next->degree != curr->degree) {
                curr = next;
            } else {
                node* nextSibling = next->sibling;

                curr = mergeTrees(curr, next);
                if(nextSibling)
                    curr->sibling = nextSibling;
                else
                    curr->sibling = NULL;
                correctMerge = 0;
            }
    }

    // curr = mergedHeap;

    // while (curr && curr->sibling) {
    //     if(curr->degree == curr->sibling->degree && (curr->data->prior < curr->sibling->data->prior ||
    //         curr->data->prior == curr->sibling->data->prior && strcmp(curr->data->time_app, curr->sibling->data->time_app) > 0)) {
    //         node* tempData = curr;
    //         curr = curr->sibling;
    //         curr->sibling = tempData;
    //     }
    //     curr = curr->sibling;
    // }

    return mergedHeap;
}

void mergeHeaps_without_destr(void* temp_heap1, void* temp_heap2) {
    binomial_heap* heap1 = (binomial_heap*)temp_heap1;
    binomial_heap* heap2 = (binomial_heap*)temp_heap2;
    heap1->head = mergeHeaps_without_destr_in(heap1->head, heap2->head);
    heap2->head = NULL;
}

enum answer insertBinomialHeap(void* temp_bh, application* data) {
    binomial_heap* bh = (binomial_heap*)temp_bh;
    if(bh == NULL) return ERROR_MEMORY;

    node* newNode = createNode(data);
    bh->head = mergeHeaps_without_destr_in(bh->head, newNode); // Выполняем слияние куч

    return OK;
}

application* find_max_binomial(const binomial_heap* sr)  {
    if (sr == NULL) return NULL;
    return sr->head->data;
}

application* extract_max_binomial(void* temp_H) {
    binomial_heap* H = (binomial_heap*)temp_H;
    if (H == NULL || H->head == NULL)
        return NULL;

    node* childrenMaxNode = H->head->child;
    node* siblingMaxNode = H->head->sibling;

    application* appMaxNode = H->head->data;

    H->head->sibling = NULL;
    H->head->data    = NULL;
    H->head->child   = NULL;
    free(H->head);

    H->head = mergeHeaps_without_destr_in(siblingMaxNode, childrenMaxNode);

    return appMaxNode;
}