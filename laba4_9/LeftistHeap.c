#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct LeftistHeap {
    application* app;
    int npl;
    struct LeftistHeap* right;
    struct LeftistHeap* left;
} leftist_heap;

typedef struct LeftistHeapRoot {
    leftist_heap* root;
} leftist_root;

leftist_heap* create_leftist_node(application* app) {
    leftist_heap* newNode = malloc(sizeof(leftist_heap));
    if (newNode == NULL) return NULL;

    newNode->app = app;
    newNode->npl = 0;
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}

leftist_root* create_leftist_root(){
    leftist_root* new = malloc(sizeof(leftist_root));
    if (new == NULL) return NULL;

    new->root = NULL;
    return new;
}

leftist_heap* merge1(leftist_heap* h1, leftist_heap* h2) {
    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;
    if (h1->app == NULL) return NULL;

    if (h1->app->prior < h2->app->prior
        || h1->app->prior == h2->app->prior  &&  strcmp(h1->app->time_app, h2->app->time_app) < 0) {
        leftist_heap* tmp = h1;
        h1 = h2;
        h2 = tmp;
    }

    h1->right = merge1(h1->right, h2);
    
    if (h1->left == NULL || h1->left->npl < h1->right->npl) {
        leftist_heap* tmp = h1->left;
        h1->left = h1->right;
        h1->right = tmp;
    }
    
    if (h1->right == NULL) h1->npl = 0;
    else h1->npl = h1->right->npl + 1;

    return h1;
}

void merge_leftist(void* temp_sr1, void* temp_sr2) {
    leftist_root* sr1 = (leftist_root*)temp_sr1;
    leftist_root* sr2 = (leftist_root*)temp_sr2;

    sr1->root = merge1(sr1->root, sr2->root);
    sr2->root = NULL; 
}

enum answer insert_leftist(void* temp_root, application* app) {
    leftist_root* root = (leftist_root*)temp_root;
    leftist_heap* newNode = create_leftist_node(app);

    root->root = merge1(root->root, newNode);
    return OK;
}

application* extract_max_leftist(void* temp_root) {
    leftist_root* root = (leftist_root*)temp_root;
    if (root == NULL) {
        return NULL;
    }

    leftist_heap* max_node = root->root;
    leftist_heap* left_subtree = max_node->left;
    leftist_heap* right_subtree = max_node->right;

    application* app = max_node->app;

    free(max_node); 

    root->root = merge1(left_subtree, right_subtree);

    return app;
}


leftist_heap* merge_and_destroy_helper_l(leftist_heap* h1, leftist_heap* h2)
{
    if (h1 == NULL)
    {
        free(h2);
        return NULL;
    }
    if (h2 == NULL)
    {
        free(h1);
        return NULL;
    }

    if (h1->app->prior < h2->app->prior
                            || h1->app->prior == h2->app->prior && strcmp(h1->app->time_app, h2->app->time_app) < 0) {
        leftist_heap* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    leftist_heap* temp = merge_and_destroy_helper_l(h1->right, h2);
    h1->right = h1->left;
    h1->left = temp;

    free(h2);
    return h1;
}

void merge_destroy_lefttist(leftist_root* root1, leftist_root* root2) {
    root1->root = merge_and_destroy_helper_l(root1->root, root2->root);
    root2->root = NULL;
}

application* find_max_leftist(const leftist_root* sr)  {
    if (sr == NULL) return NULL;
    return sr->root->app;
}

void free_left1(leftist_heap* root, int isFull) {
    if (root == NULL) return;
    
    free_left1(root->right, isFull);
    free_left1(root->left, isFull);
    if (root->app) free_application(root->app);
    free(root);
}

void free_left(void* temp_root, int isFull) {
    leftist_root* root = (leftist_root*)temp_root;

    if (root == NULL) return;
    
    free_left1(root->root, 1);
    free(root);
}