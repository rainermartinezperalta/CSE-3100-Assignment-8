#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Tree* makeEmptyTree() {
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    if (tree == NULL) {
        perror("Failed to allocate memory for Tree");
        return NULL;
    }
    tree->root = NULL;
    pthread_mutex_init(&tree->lock, NULL);
    return tree;
}

void insertIntoTree(Tree* t, int val) {
    TNode* newNode = (TNode*)malloc(sizeof(TNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new node");
        return;
    }
    newNode->val = val;
    newNode->left = NULL;
    newNode->right = NULL;
    pthread_mutex_init(&newNode->lock, NULL);

    pthread_mutex_lock(&t->lock);

    if (t->root == NULL) {
        t->root = newNode;
        pthread_mutex_unlock(&t->lock);
        return;
    }

    TNode* current = t->root;
    TNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (val < current->val) {
            pthread_mutex_lock(&current->lock);
            if (current->left == NULL) {
                current->left = newNode;
                pthread_mutex_unlock(&current->lock);
                return;
            }
            current = current->left;
            pthread_mutex_unlock(&parent->lock); // unlock parent before moving to the child
        } else if (val > current->val) {
            pthread_mutex_lock(&current->lock);
            if (current->right == NULL) {
                current->right = newNode;
                pthread_mutex_unlock(&current->lock);
                return;
            }
            current = current->right;
            pthread_mutex_unlock(&parent->lock); // unlock parent before moving to the child
        } else {
            // Value already exists in the tree, so we can skip insertion.
            pthread_mutex_destroy(&newNode->lock);
            free(newNode);
            pthread_mutex_unlock(&current->lock);
            return;
        }
    }
}

void printTreeAux(TNode* root) {
    if (root == NULL)
        return;
    else {
        printf("(");
        printTreeAux(root->left);
        printf(" %d ", root->val);
        printTreeAux(root->right);
        printf(")");
    }
}

void printTree(Tree* t) {
    printTreeAux(t->root);
}

void destroyTreeAux(TNode* root) {
    if (root != NULL) {
        destroyTreeAux(root->left);
        destroyTreeAux(root->right);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
}

void destroyTree(Tree* t) {
    destroyTreeAux(t->root);
    pthread_mutex_destroy(&t->lock);
    free(t);
}