/*
    B+ 树 (B+ Tree)
    B+ 树是一种自平衡的树数据结构，常用于数据库和文件系统中。
    B+ 树的非叶子节点只存储键值用于搜索，所有实际数据都存储在叶子节点中。
    叶子节点通过链表相连，以支持高效的区间查询。

    特点：
    - B+ 树的所有叶子节点位于同一层，且叶子节点之间通过链表相连。
    - 非叶子节点只存储用于导航的键值，不存储实际数据。

    操作：
    1. 初始化 B+ 树
    2. 插入键值
    3. 查找键值
    4. 输出 B+ 树

    Author: IreneX86
    Email: Irene.127.0.0.1@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3  // 每个节点最大键数（用于演示，小于实际数据库中的 B+ 树）

typedef struct BPTreeNode {
    int keys[MAX_KEYS];               // 存储键的数组
    struct BPTreeNode *children[MAX_KEYS + 1]; // 存储子节点的指针数组
    struct BPTreeNode *next;          // 指向下一个叶子节点
    int n;                            // 当前存储的键数量
    int leaf;                         // 是否为叶子节点的标志
} BPTreeNode;

// 初始化 B+ 树节点
BPTreeNode* init_bptree_node(int leaf) {
    BPTreeNode *node = (BPTreeNode *)malloc(sizeof(BPTreeNode));
    node->leaf = leaf;
    node->n = 0;
    node->next = NULL;
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// 插入非满节点
void insert_non_full(BPTreeNode *node, int k) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && k < node->keys[i]) {
            node->keys[i+1] = node->keys[i];
            i--;
        }
        node->keys[i+1] = k;
        node->n++;
    } else {
        while (i >= 0 && k < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->n == MAX_KEYS) {
            split_child(node, i, node->children[i]);
            if (k > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], k);
    }
}

// 分裂子节点
void split_child(BPTreeNode *parent, int i, BPTreeNode *child) {
    BPTreeNode *new_child = init_bptree_node(child->leaf);
    new_child->n = (MAX_KEYS - 1) / 2;

    for (int j = 0; j < new_child->n; j++) {
        new_child->keys[j] = child->keys[j + (MAX_KEYS + 1) / 2];
    }

    if (!child->leaf) {
        for (int j = 0; j <= new_child->n; j++) {
            new_child->children[j] = child->children[j + (MAX_KEYS + 1) / 2];
        }
    }

    child->n = (MAX_KEYS - 1) / 2;

    for (int j = parent->n; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[i + 1] = new_child;

    for (int j = parent->n - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[i] = child->keys[(MAX_KEYS - 1) / 2];
    parent->n++;

    if (child->leaf) {
        new_child->next = child->next;
        child->next = new_child;
    }
}

// 插入键值到 B+ 树
void insert_bptree(BPTreeNode **root, int k) {
    BPTreeNode *r = *root;
    if (r->n == MAX_KEYS) {
        BPTreeNode *s = init_bptree_node(0);
        *root = s;
        s->children[0] = r;
        split_child(s, 0, r);
        insert_non_full(s, k);
    } else {
        insert_non_full(r, k);
    }
}

// 查找键值
BPTreeNode* search(BPTreeNode *node, int k) {
    int i = 0;
    while (i < node->n && k > node->keys[i]) {
        i++;
    }

    if (i < node->n && k == node->keys[i]) {
        if (node->leaf) {
            return node;
        } else {
            return search(node->children[i + 1], k);
        }
    }

    if (node->leaf) {
        return NULL;
    } else {
        return search(node->children[i], k);
    }
}

// 输出 B+ 树
void print_bptree(BPTreeNode *node, int depth) {
    if (node != NULL) {
        for (int i = 0; i < node->n; i++) {
            print_bptree(node->children[i], depth + 1);
            for (int j = 0; j < depth; j++) printf("  ");
            printf("%d\n", node->keys[i]);
        }
        print_bptree(node->children[node->n], depth + 1);
    }
}

int main() {
    BPTreeNode *root = init_bptree_node(1); // 初始化根节点为叶子节点

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        insert_bptree(&root, keys[i]);
    }

    print_bptree(root, 0);

    int key_to_search = 6;
    BPTreeNode *result = search(root, key_to_search);
    if (result != NULL) {
        printf("\nKey %d found in B+ tree.\n", key_to_search);
    } else {
        printf("\nKey %d not found in B+ tree.\n", key_to_search);
    }

    return 0;
}
