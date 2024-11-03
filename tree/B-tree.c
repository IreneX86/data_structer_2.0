/*
    B 树 (B-tree)
    B 树是一种自平衡树数据结构，用于在磁盘或其他存储设备上存储数据，
    以减少读取和写入操作的次数。

    特点：
    - B 树中的节点可以有多个子节点（由阶数决定）。
    - B 树是自平衡的，所有叶子节点的深度相同。

    操作：
    1. 初始化 B 树
    2. 插入键值
    3. 查找键值

    Author: IreneX86
    Email: Irene.127.0.0.1@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>

#define T 3 // B 树的阶数（即每个节点最少有 T-1 个键，最多有 2*T-1 个键）

typedef struct BTreeNode {
    int keys[2*T-1];                // 存储键的数组
    struct BTreeNode *children[2*T]; // 存储子节点的指针数组
    int n;                          // 当前存储的键数量
    int leaf;                       // 是否为叶子节点的标志
} BTreeNode;

// 初始化 B 树节点
BTreeNode* init_btree_node(int leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->leaf = leaf;
    node->n = 0;
    for (int i = 0; i < 2*T; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// 分裂子节点
void split_child(BTreeNode *parent, int i, BTreeNode *child) {
    BTreeNode *new_child = init_btree_node(child->leaf);
    new_child->n = T - 1;

    for (int j = 0; j < T-1; j++) {
        new_child->keys[j] = child->keys[j+T];
    }

    if (!child->leaf) {
        for (int j = 0; j < T; j++) {
            new_child->children[j] = child->children[j+T];
        }
    }

    child->n = T - 1;

    for (int j = parent->n; j >= i+1; j--) {
        parent->children[j+1] = parent->children[j];
    }

    parent->children[i+1] = new_child;

    for (int j = parent->n-1; j >= i; j--) {
        parent->keys[j+1] = parent->keys[j];
    }

    parent->keys[i] = child->keys[T-1];
    parent->n++;
}

// 插入非满节点
void insert_non_full(BTreeNode *node, int k) {
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
        if (node->children[i]->n == 2*T-1) {
            split_child(node, i, node->children[i]);
            if (k > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], k);
    }
}

// 插入键值到 B 树
void insert_btree(BTreeNode **root, int k) {
    BTreeNode *r = *root;
    if (r->n == 2*T-1) {
        BTreeNode *s = init_btree_node(0);
        *root = s;
        s->children[0] = r;
        split_child(s, 0, r);
        insert_non_full(s, k);
    } else {
        insert_non_full(r, k);
    }
}

// 查找键值
BTreeNode* search(BTreeNode *root, int k) {
    int i = 0;
    while (i < root->n && k > root->keys[i]) {
        i++;
    }

    if (i < root->n && k == root->keys[i]) {
        return root;
    }

    if (root->leaf) {
        return NULL;
    } else {
        return search(root->children[i], k);
    }
}

// 输出 B 树
void print_btree(BTreeNode *node, int depth) {
    if (node != NULL) {
        for (int i = 0; i < node->n; i++) {
            print_btree(node->children[i], depth+1);
            for (int j = 0; j < depth; j++) printf("  ");
            printf("%d\n", node->keys[i]);
        }
        print_btree(node->children[node->n], depth+1);
    }
}

int main() {
    BTreeNode *root = init_btree_node(1); // 初始化根节点为叶子节点

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        insert_btree(&root, keys[i]);
    }

    print_btree(root, 0);

    int key_to_search = 6;
    BTreeNode *result = search(root, key_to_search);
    if (result != NULL) {
        printf("\nKey %d found in B-tree.\n", key_to_search);
    } else {
        printf("\nKey %d not found in B-tree.\n", key_to_search);
    }

    return 0;
}
