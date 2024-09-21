#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct BinaryHeap {
    application** arr;
    int capacity;
    int size;
} binary_heap;

binary_heap* create_binary_heap() {
    binary_heap* bh = malloc(sizeof(binary_heap));
    if (bh == NULL) return NULL;

    bh->arr = (application**)malloc(10 * sizeof(application*));
    if (bh->arr == NULL) {
        free(bh);
        return NULL;
    }

    bh->capacity = 10;
    bh->size = 0;
    return bh;
}

void swap(application** arr, const int index1, const int index2) {
    application* temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}

void heapifyUp(const binary_heap* bh, int index) {
    int parent = (index - 1) / 2;

    while (index > 0 && (bh->arr[index]->prior > bh->arr[parent]->prior ||
                         (bh->arr[index]->prior == bh->arr[parent]->prior && strcmp(bh->arr[index]->time_app, bh->arr[parent]->time_app) < 0))) {
        //swap(bh->arr[index], bh->arr[parent]);
        swap(bh->arr, index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

enum answer insert_binary_heap(void* temp_bh, application* app) {
    binary_heap* bh = (binary_heap*)temp_bh;

    if (bh->size == bh->capacity) {
        bh->capacity *= 2;
        application** temp_arr = realloc(bh->arr, bh->capacity * sizeof(application*));
        if (temp_arr == NULL) {
            bh->capacity /= 2;
            free(bh->arr);
            return ERROR_MEMORY;
        }
        bh->arr = temp_arr; // Присваиваем новое значение указателю
    }

    bh->arr[bh->size] = app;

    if(bh->arr == NULL) return ERROR_MEMORY;

    heapifyUp(bh, bh->size);
    bh->size++;

    return OK;
}

void heapifyDown(binary_heap* bh, const int index) {
    const int left = 2 * index + 1;
    const int right = 2 * index + 2;
    int largest = index;
    if (left < bh->size && bh->arr[left]->prior > bh->arr[largest]->prior) {
        largest = left;
    }

    if (right < bh->size && bh->arr[right]->prior > bh->arr[largest]->prior) {
        largest = right;
    }

    if (largest != index) {
        //swap(bh->arr[index], bh->arr[largest]);
        swap(bh->arr, index, largest);
        heapifyDown(bh, largest);
    }
}

application* extract_max_binary_heap(void* temp_bh) {
    binary_heap* bh = (binary_heap*)temp_bh;
    if (bh->size <= 0) {
        exit(1);
    }

    application* max_app = bh->arr[0];
    bh->arr[0] = bh->arr[bh->size - 1];
    bh->size--;
    heapifyDown(bh, 0);
    return max_app;
}

application* find_max_binary_heap(const binary_heap* bh) {
    if (bh->size <= 0) {
        exit(1);
    }
    application* max_app = bh->arr[0];
    return max_app;
}

void free_binary_heap(void* temp_heap, int isFull)
{
    binary_heap* heap = (binary_heap*)temp_heap;
    for (int i = 0; i < heap->size; i++)
    {
        free_application(heap->arr[i]);
    }
    free(heap->arr);
    free(heap);
}

void mergeBinaryHeapsWithoutDestruction(void* temp_heap1, void* temp_heap2) {
    binary_heap* heap1 = (binary_heap*)temp_heap1;
    binary_heap* heap2 = (binary_heap*)temp_heap2;
    while(heap2->size > 0) {
        insert_binary_heap(heap1, extract_max_binary_heap(heap2));
    }
}
void mergeHeapsWithDestruction(binary_heap* heap1, binary_heap* heap2) {
    // Объединение элементов в одну кучу (heap1)
    for (int i = 0; i < heap2->size; i++) {
        insert_binary_heap(heap1, heap2->arr[i]);
    }
    
    // Удаление памяти для второй кучи (heap2) 
    free(heap2->arr);
    free(heap2);
    heap2 = NULL;
}