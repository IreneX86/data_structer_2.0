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
    headNode->data = NULL;
    headNode->pre = headNode;
    headNode->next = headNode;
    return headNode;
}

void headInsert(node_t *headNode, void *data, int *count)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->pre = headNode;
    node->next = headNode->next;
    headNode->next->pre = node;
    headNode->next = node;
    (*count)++;
}

void tailInsert(node_t *headNode, void *data, int *count)
{
    node_t *tailNode = headNode;
    while (tailNode->next != headNode)
    {
        tailNode = tailNode->next;
    }

    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->pre = tailNode;
    node->next = headNode;
    tailNode->next = node;
    headNode->pre = node;
    (*count)++;
}

// 删除
int deleteNode(node_t *headNode, void *data, int (*cmp)(void *, void *), int *count)
{
    node_t *current = headNode->next;
    while (current != headNode)
    {
        if (cmp(current->data, data) == 0)
        {
            current->pre->next = current->next;
            current->next->pre = current->pre;
            free(current->data);
            free(current);
            (*count)--;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 输出链表
void printList(node_t *headNode, void (*print)(void *))
{
    node_t *node = headNode->next;
    while (node != headNode)
    {
        print(node->data);
        node = node->next;
    }
    printf("HEAD\n");
}

// 打印整数数据的函数
void printInt(void *data)
{
    printf("%d->", *((int *)data));
}

// 比较整数数据的函数
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
    *data = 410;
    tailInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 740;
    tailInsert(L, data, &count);

    printList(L, printInt);

    int key = 9;
    if (deleteNode(L, &key, cmpInt, &count))
        printf("successfully deleted\n");
    else
        printf("failed to delete");
    printList(L, printInt);

    return 0;
}
