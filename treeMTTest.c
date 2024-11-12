#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void printInOrderAux(TNode* root) {
    if (root != NULL) {
        printInOrderAux(root->left);
        printf("%d ", root->val);
        printInOrderAux(root->right);
    }
}

void printInOrder(Tree* t) {
    printInOrderAux(t->root);
}

void shuffle(int* numbers, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int x = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = x;
    }
}

ThreadArgs* divideWork(int* numbers, int n, int nt, Tree* t) {
    ThreadArgs* args = calloc(nt, sizeof(ThreadArgs));
    int rem = n % nt;
    int index = 0;
    for (int i = 0; i < nt; i++) {
        args[i].t = t;
        args[i].numbers = numbers + index;
        args[i].n = n / nt;
        if (i < rem)
            args[i].n++;
        index += args[i].n;
    }
    return args;
}

void* startThread(void* arg) {
    ThreadArgs* ta = (ThreadArgs*)arg;
    for (int i = 0; i < ta->n; i++) {
        insertIntoTree(ta->t, ta->numbers[i]);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./treeMTTest n numThreads\n");
        return 1;
    }

    Tree* tree = makeEmptyTree();
    int n = atoi(argv[1]);
    int nt = atoi(argv[2]);

    int numbers[n];
    for (int i = 0; i < n; i++) {
        numbers[i] = i;
    }
    shuffle(numbers, n);

    ThreadArgs* args = divideWork(numbers, n, nt, tree);

    pthread_t threads[nt];
    for (int i = 0; i < nt; i++) {
        assert(pthread_create(&threads[i], NULL, startThread, &args[i]) == 0);
    }

    for (int i = 0; i < nt; i++) {
        assert(pthread_join(threads[i], NULL) == 0);
    }

    printInOrder(tree);
    printf("\n");
    destroyTree(tree);
    free(args);

    return 0;
}