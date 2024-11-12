#include <pthread.h>
#ifndef __TREEMT_H
#define __TREEMT_H

typedef struct TNode {
    int val;
    struct TNode* left;
    struct TNode* right;
    pthread_mutex_t lock;
} TNode;

typedef struct Tree {
    TNode* root;
    pthread_mutex_t lock;
} Tree;

typedef struct ThreadArgs {
    Tree* t;
    int* numbers;
    int n;
} ThreadArgs;

Tree* makeEmptyTree();
void insertIntoTree(Tree* t, int val);
void printTree(Tree* t);
void destroyTree(Tree* t);
#endif