#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    void *data; // 定义了一个链表节点结构，节点数据类型为 void*，可以存储任何类型的数据
    struct Node *next;
} node_t;

// 初始化链表，返回头节点
node_t *initList()
{
    node_t *headNode = (node_t *)malloc(sizeof(node_t));
    headNode->data = NULL;
    headNode->next = NULL;
    return headNode;
}

// 头插
void headInsert(node_t *headNode, void *data, int *count)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->next = headNode->next;
    headNode->next = node;
    (*count)++;
}

// 尾插
void tailInsert(node_t *headNode, void *data, int *count)
{
    node_t *tailNode = headNode;
    while (tailNode->next != NULL)
    {
        tailNode = tailNode->next; // 遍历直到最后一个节点
    }

    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->data = data;
    node->next = NULL;
    tailNode->next = node;
    (*count)++;
}

// 删除节点，若成功则返回1，未找到data则返回0
int deleteNode(node_t *headNode, void *data, int (*cmp)(void *, void *), int *count)
{
    node_t *preNode = headNode;
    node_t *current = headNode->next;

    while (current)
    {
        if (cmp(current->data, data) == 0)
        {
            preNode->next = current->next;
            free(current->data); // 释放节点中的数据
            free(current);       // 释放节点本身
            (*count)--;
            return 1;
        }
        preNode = current;
        current = current->next;
    }
    return 0;
}

// 输出链表
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
    *data = 805;
    tailInsert(L, data, &count);

    data = malloc(sizeof(int));
    *data = 1325;
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