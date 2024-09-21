#include <stdlib.h>
#include <string.h>
#include "structures.h"

typedef struct Node_treap {
    application* app;
    int prior;
    struct Node_treap* left;
    struct Node_treap* right;
} treap_node;

treap_node* create_treap_node(application* app) {
    treap_node* newNode = malloc(sizeof(treap_node));
    if (newNode == NULL) return NULL;

    newNode->app = app;
    newNode->prior = app->prior;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void split(treap_node* root, application* app, treap_node** left, treap_node** right) {
    if (root == NULL) {
        *left = NULL;
        *right = NULL;
    } else if (root->app->prior < app->prior
                        || root->app->prior < app->prior && strcmp(root->app->time_app, app->time_app) > 0) {
        *left = root;
        split(root->right, app, &root->right, right);
    } else {
        *right = root;
        split(root->left, app, left, &root->left);
    }
}

treap_node* merge_treap_without_destr1(treap_node* left, treap_node* right) {
    if (left == NULL) return right;
    if (right == NULL) return left;

    if (left->prior > right->prior) {
        left->right = merge_treap_without_destr1(left->right, right);
        return left;
    }

    right->left = merge_treap_without_destr1(left, right->left);
    return right;
}

void merge_treap_without_destr(void* temp_left, void* temp_right) {
    treap_node* left = (treap_node*)temp_left;
    treap_node* right = (treap_node*)temp_right;

    left->right = merge_treap_without_destr1(left->right, right);
    right->left = NULL;
}

enum answer insert_treap(void* temp_root, application* app) {
    treap_node* root = (treap_node*)temp_root;
    treap_node* newNode = create_treap_node(app);
    treap_node* left, *right;
    split(root, app, &left, &right);
    merge_treap_without_destr1(merge_treap_without_destr1(left, newNode), right);
    return OK;
}


treap_node* erase(treap_node *root, const int key) {
    if (root == NULL) {
        return NULL;
    }
    if (root->app->ident_app == key) {
        treap_node* temp = root;
        root = merge_treap_without_destr1(root->left, root->right);
        free(temp);
    } else if (root->app->ident_app < key) {
        root->right = erase(root->right, key);
    } else {
        root->left = erase(root->left, key);
    }
    return root;
}

treap_node* extract_max_treap(treap_node** root) {
    treap_node* current = *root;
    while (current->right) {
        current = current->right;
    }
    *root = erase(*root, current->app->ident_app);
    return current;
}

application* find_max_treap(const treap_node* sr)  {
    if (sr == NULL) return NULL;
    return sr->app;
}

void free_treap(void* temp_node, int a)
{
    treap_node* node = (treap_node*)temp_node;

    if (node == NULL) return;

    free_treap(node->left, 0);
    free_treap(node->right, 0);

    if (node->app) free_application(node->app);
    free(node);
}