/* 线性链表法（Linear Chaining），又叫单独链接（Separate Chaining）
 *
 * 原理：
 * 当两个或多个键映射到哈希表的同一位置时，将这些键存储在一个链表（Linked List）中。
 * 
 * 实现：
 * 哈希表中的每个位置（槽）实际上是一个链表的头指针。冲突发生时，新的键被添加到该槽对应的链表中。
 * 
 * 优点：
 * 对于表中已存在大量数据时，插入新数据依旧较为高效。
 * 
 * 缺点：
 * 当冲突较多时，链表可能变得很长，导致查找效率降低；需要额外的空间来存储链表节点。
 * 
 * 复杂度：
 * 每次插入新键时，时间复杂度为 O(1)，但是在最坏情况下，查找或删除操作的时间复杂度为 O(n)，其中 n 是链表的长度。
*/

#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

typedef struct Node
{
    int data;
    struct Node *next;
} node_t;

node_t *hash_table[SIZE] = {NULL}; // 哈希表，每个元素是一个指向链表头的指针

// 哈希函数，计算键的哈希值
int hash(int key)
{
    return key % SIZE;
}

// 插入键值对到哈希表
void insert(int key)
{
    int index = hash(key);                               // 计算键的哈希值
    node_t *new_node = (node_t *)malloc(sizeof(node_t)); // 创建新节点
    new_node->data = key;                                // 设置节点的键
    new_node->next = NULL;                               // 初始化节点的下一个指针为 NULL

    if (hash_table[index] == NULL) // 如果哈希表当前位置为空
    {
        hash_table[index] = new_node; // 直接将新节点设为头节点
    }
    else // 如果哈希表当前位置不为空（发生哈希冲突）
    {
        node_t *temp = hash_table[index]; // 临时指针指向当前位置的头节点
        while (temp->next != NULL)        // 遍历链表，直到找到尾节点
        {
            temp = temp->next;
        }
        temp->next = new_node; // 将新节点连接到链表的末尾
    }
}

// 输出哈希表
void display(void)
{
    for (int i = 0; i < SIZE; i++) // 遍历哈希表数组
    {
        printf("Index %d: ", i);      // 打印当前索引
        node_t *temp = hash_table[i]; // 临时指针指向当前位置的头节点
        while (temp != NULL)          // 遍历链表
        {
            printf("%d->", temp->data); // 打印当前节点的键
            temp = temp->next;          // 移动到下一个节点
        }
        printf("NULL\n"); // 打印链表末尾的 NULL
    }
}

int main(int argc, char *argv[])
{
    int keys[] = {1, 4, 9, 6, 2, 5};
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
    {
        insert(keys[i]);
    }
    display();

    return 0;
}