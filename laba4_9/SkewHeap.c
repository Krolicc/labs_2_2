#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct SkewHeap {
    application* app;
    struct SkewHeap* right;
    struct SkewHeap* left;
} skew_heap;

typedef struct SkewHeapRoot {
    skew_heap* root;
} skew_root;

skew_heap* create_skew_node(application* app) {
    skew_heap* newNode = malloc(sizeof(skew_heap));
    if (newNode == NULL) return NULL;

    newNode->app = app;
    newNode->right = NULL;
    newNode->left = NULL;

    return newNode;
}

skew_root* create_skew_root() {
    skew_root* new = malloc(sizeof(skew_root));
    if (new == NULL) return NULL;

    new->root = NULL;
    return new;
}
skew_heap* merge(skew_heap* h1, skew_heap* h2) {
    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;

    if (h1->app->prior > h2->app->prior
                            || h1->app->prior == h2->app->prior && strcmp(h1->app->time_app, h2->app->time_app) < 0) {
        skew_heap* tmp = h1->left;
        h1->left = h1->right;
        h1->right = merge(tmp, h2);
        return h1;
    }

    skew_heap* tmp = h2->left;
    h2->left = h2->right;
    h2->right = merge(tmp, h1);
    return h2;
}

void merge_skew(void* temp_sr1, void* temp_sr2) {
    skew_root* sr1 = (skew_root*)temp_sr1;
    skew_root* sr2 = (skew_root*)temp_sr2;
    sr1->root = merge(sr1->root, sr2->root);
    sr2->root = NULL; 
}

application* extract_max_skew(void* temp_sr) {
    skew_root* sr = (skew_root*)temp_sr;
    if (sr == NULL || sr->root == NULL) return NULL; // Куча пуста

    // Сохраняем максимальное значение
    application* maxApp = sr->root->app;
    
    // Объединяем правое и левое поддерево корня
    skew_heap* oldRoot = sr->root;
    sr->root = merge(sr->root->left, sr->root->right);
    
    // Освобождаем память, занимаемую предыдущим корнем
    free(oldRoot);
    
    return maxApp;
}
application* find_max_skew(const skew_root* sr)  {
    if (sr == NULL) return NULL;
    return sr->root->app;
}

enum answer insert_skew(void* temp_sr, application* newApp) {
    skew_root* sr = (skew_root*)temp_sr;
    skew_heap* newHeap = create_skew_node(newApp);
    newHeap->app = newApp;
    newHeap->left = NULL;
    newHeap->right = NULL;
    
    // Объединяем существующую кучу с новым элементом
    sr->root = merge(sr->root, newHeap);
    
    return OK;
}

void free_skew(void* temp_root, const int isFull) {
    skew_heap* root = (skew_heap*)temp_root;

     if (root == NULL) return;

    free_skew(root->left, isFull);
    free_skew(root->right, isFull);  
    if (root->app) free_application(root->app);  
    free(root);
}


skew_heap* merge_and_destroy_helper_s(skew_heap* h1, skew_heap* h2) {
    if (h1 == NULL) {
        free(h2);
        return NULL;
    }

    if (h2 == NULL) {
        free(h1);
        return NULL;
    }

    if (h1->app->prior > h2->app->prior
    || h1->app->prior == h2->app->prior && strcmp(h1->app->time_app, h2->app->time_app) < 0) {
        skew_heap* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    skew_heap* temp = merge_and_destroy_helper_s(h1->right, h2);
    h1->right = h1->left;
    h1->left = temp;

    free(h2);

    return h1;
}

void merge_destroy_skew(skew_root* root1, skew_root* root2) {
    root1->root = merge_and_destroy_helper_s(root1->root, root2->root);
    root2->root = NULL;
}
