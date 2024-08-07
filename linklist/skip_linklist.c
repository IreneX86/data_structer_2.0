/*
    Author: IreneX86
    Email: Irene.127.0.0.1@gmail.com

    search 平均时间复杂度： O(log(n))
    insert 平均时间复杂度： O(log(n))
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 4

typedef struct Node
{
    void *data;
    struct Node **forward; // 指向指针的指针数组
} node_t;

typedef struct SkipList
{
    int level;
    node_t *header;
} skipList_t;

// 初始化 skip list
skipList_t *initSkipList()
{
    skipList_t *list = (skipList_t *)malloc(sizeof(skipList_t));
    list->level = 0;
    list->header = (node_t *)malloc(sizeof(node_t));
    // MAX_LEVEL + 1 表示我们希望分配的指针数量。
    // 这里的 MAX_LEVEL 是 Skip List 的最大层级数，加 1 是因为层级是从 0 开始计算的。
    list->header->forward = (node_t **)malloc(sizeof(node_t *) * (MAX_LEVEL + 1));
    for (int i = 0; i <= MAX_LEVEL; i++)
    {
        list->header->forward[i] = NULL;
    }
    return list;
}

// 生成随机层数
/*
 * 1. 随机层数的引入
 * 引入随机层数，让跳表的节点分布更加均匀，避免最坏情况的出现。
 * 这样做可以将搜索、插入和删除操作的时间复杂度保持在 O(log n)，其中 n 是节点数。
 *
 * 2. 逐层增加的概率
 * 使用 rand() < RAND_MAX / 2 这种方式来决定是否增加层数，实质上是在模拟一种几何分布的概率。
 * 也就是说，每增加一层，下一层增加的概率减半。
 * 这会让大部分节点停留在较低层，而少部分节点到达更高层。
 *
 * 3. 减少最大层数的限制
 * 通过设置 MAX_LEVEL，我们可以限制跳表的最大层数，避免无限增加层数导致的内存浪费。
 * 实际应用中，MAX_LEVEL 一般设置为 log(n) 的值，
 * 这样可以保证跳表的空间和时间复杂度都在可控范围内。
 *
 */
int randomLevel()
{
    int level = 0;
    while (rand() < RAND_MAX / 2 && level < MAX_LEVEL)
    {
        level++;
    }
    return level;
}

// 插入元素
void insert(skipList_t *list, void *data, int (*cmp)(void *, void *))
{
    node_t *update[MAX_LEVEL + 1]; // update 数组用于保存每一层的前驱节点（即在插入新节点之前，链表中对应层次的最后一个节点）。
    node_t *x = list->header;      // x 初始化为跳表的头节点（header），从头节点开始进行搜索。

    // 从跳表的最高层（list->level）开始逐层向下搜索适合插入新节点的位置。
    for (int i = list->level; i >= 0; i--)
    {
        // 在每一层中，x 会沿着 forward 指针前进，直到找到第一个大于或等于 data 的节点。
        while (x->forward[i] != NULL && cmp(x->forward[i]->data, data) < 0)
        {
            x = x->forward[i];
        }
        update[i] = x; // update[i] 保存每一层中最后一个小于 data 的节点。
    }

    // 生成一个随机层数 level，用于新插入的节点。
    int level = randomLevel();

    // 如果生成的随机层数 level 大于当前跳表的最高层数 list->level，
    // 则更新 update 数组中对应层次的前驱节点为头节点，并更新跳表的最高层数。
    if (level > list->level)
    {
        for (int i = list->level + 1; i <= level; i++)
        {
            update[i] = list->header;
        }
        list->level = level;
    }

    // 分配内存创建一个新的节点 newNode，并初始化它的 data 和 forward 数组
    // （forward 数组的大小为 level + 1，即包含新节点所在的所有层次）。
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->data = data;
    newNode->forward = (node_t **)malloc(sizeof(node_t *) * (level + 1));

    // 插入新节点 newNode
    for (int i = 0; i <= level; i++)
    {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
}

// 搜索元素
node_t *search(skipList_t *list, void *data, int (*cmp)(void *, void *))
{
    node_t *x = list->header;

    // 从跳表的最高层（list->level）开始逐层向下搜索。
    for (int i = list->level; i >= 0; i--)
    {
        while (x->forward[i] != NULL && cmp(x->forward[i]->data, data) < 0)
        {
            x = x->forward[i];
        }
    }
    x = x->forward[0]; // 在最底层（第0层）继续前进到下一个节点。
    if (x != NULL && cmp(x->data, data) == 0)
    {
        return x;
    }
    else
    {
        return NULL;
    }
}

// 删除元素
void delete(skipList_t *list, void *data, int (*cmp)(void *, void *))
{
    node_t *update[MAX_LEVEL + 1];
    node_t *x = list->header;
    for (int i = list->level; i >= 0; i--)
    {
        while (x->forward[i] != NULL && cmp(x->forward[i]->data, data) < 0)
        {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];
    if (x != NULL && cmp(x->data, data) == 0)
    {
        for (int i = 0; i <= list->level; i++)
        {
            if (update[i]->forward[i] != x)
                break;
            update[i]->forward[i] = x->forward[i];
        }
        free(x->forward);
        free(x);

        // 重新调整跳表的层数：
        // 检查是否有空层（即在最高层没有节点）。如果有空层，减少跳表的层数。
        while (list->level > 0 && list->header->forward[list->level] == NULL)
        {
            list->level--;
        }
    }
}

// 打印Skip List
void printList(skipList_t *list, void (*print)(void *))
{
    for (int i = 0; i <= list->level; i++)
    {
        node_t *x = list->header->forward[i];
        printf("Level %d: ", i);
        while (x != NULL)
        {
            print(x->data);
            x = x->forward[i];
        }
        printf("NULL\n");
    }
}

// 打印整数数据的函数
void printInt(void *data)
{
    printf("%d -> ", *((int *)data));
}

// 比较整数数据的函数
int cmpInt(void *a, void *b)
{
    return *((int *)a) - *((int *)b);
}

int main()
{
    srand(time(NULL));

    skipList_t *list = initSkipList();

    int *data = malloc(sizeof(int));
    *data = 1;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 4;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 9;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 6;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 2;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 5;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 0;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 2024;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 410;
    insert(list, data, cmpInt);

    data = malloc(sizeof(int));
    *data = 740;
    insert(list, data, cmpInt);

    printList(list, printInt);

    int key = 9;
    if (search(list, &key, cmpInt) != NULL)
        printf("successfully found %d\n", key);
    else
        printf("failed to find %d\n", key);

    delete (list, &key, cmpInt);
    printList(list, printInt);

    return 0;
}