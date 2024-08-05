/*
    Author: IreneX86
    Email: Irene.127.0.0.1@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    void *data;
    struct Node *pre;
    struct Node *next;
} node_t;

node_t *initList()
{
    node_t *headNode = (node_t *)malloc(sizeof(node_t));
    headNode->data = 0;
    headNode->pre = NULL;
    headNode->next = NULL;
    return headNode;
}

void headInsert(node_t *headNode, void *data, int *count)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->pre = headNode;
    node->next = headNode->next;

    if (headNode->next)
    {
        headNode->next->pre = node;
    }
    headNode->next = node;
    (*count)++;
}

void tailInsert(node_t *headNode, void *data, int *count)
{
    node_t *tailNode = headNode;
    while (tailNode->next)
    {
        tailNode = tailNode->next;
    }

    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->pre = tailNode;
    node->next = NULL;
    tailNode->next = node;
    (*count)++;
}

int deleteNode(node_t *headNode, void *data, int (*cmp)(void *, void *), int *count)
{
    node_t *current = headNode->next;
    while (current)
    {
        if (cmp(current->data, data) == 0)
        {
            current->pre->next = current->next;
            if (current->next)
            {
                current->next->pre = current->pre;
            }
            free(current->data);
            free(current);
            (*count)--;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void printList(node_t *headNode, void (*print)(void *))
{
    node_t *node = headNode->next;
    while (node)
    {
        print(node->data);
        node = node->next;
    }
    printf("NULL\n");
}

void printInt(void *data)
{
    printf("%d->", *((int *)data));
}

int cmpInt(void *a, void *b)
{
    return *((int *)a) - *((int *)b);
}

int main(int argc, char *argv[])
{
    node_t *L = initList();
    int count = 0;

    int *data = malloc(sizeof(int));
    *data = 1;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 4;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 9;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 6;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 2;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 5;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 0;
    headInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 2024;
    tailInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 805;
    tailInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 1432;
    tailInsert(L, data, &count);

    printList(L, printInt);

    int key = 9;
    if (deleteNode(L, &key, cmpInt, &count))
        printf("successfully deleted\n");
    else
        printf("failed to delete\n");

    printList(L, printInt);

    // 释放链表
    while (L != NULL)
    {
        node_t *temp = L;
        L = L->next;
        free(temp->data);
        free(temp);
    }

    return 0;
}