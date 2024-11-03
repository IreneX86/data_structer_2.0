/* 2-3-4 Tree 
 *
 * B 树的一种特殊形式 (阶数为 4 的 B 树, 也称为 B-tree of order 4)
 * 是多路搜索树的一种，主要用于文件系统和数据库中。
 * 每个节点可以包含 1 到 3 个键，并且最多可以有 4 个子节点。
 * 
 * 分裂满节点：当节点满时，进行分裂操作，将节点拆分为两个较小的节点，并将中间键上移到父节点。
 * 插入新键：插入操作始终从叶子节点开始。如果当前节点已满，先进行分裂，再插入。
 * 
 * 复杂度：
 * 插入 (Insertion): O(log n)
 * 查找 (Lookup): O(log n)
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_KEYS 3  // 每个节点最大键数
#define MIN_KEYS 1  // 每个节点最小键数

typedef struct Node {
    int keys[MAX_KEYS];        // 存储键值的数组
    struct Node *children[4];  // 指向子节点的指针数组
    int num_keys;              // 当前节点的键数量
    int is_leaf;               // 标志该节点是否是叶子节点
} node_t;

// 创建一个新的节点
node_t *create_node(int is_leaf) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->is_leaf = is_leaf;
    new_node->num_keys = 0;
    for (int i = 0; i < 4; i++) {
        new_node->children[i] = NULL;
    }
    return new_node;
}

// 分裂满节点
void split_child(node_t *parent, int index, node_t *child) {
    node_t *new_child = create_node(child->is_leaf);
    new_child->num_keys = MIN_KEYS;

    for (int j = 0; j < MIN_KEYS; j++) {
        new_child->keys[j] = child->keys[j + 2];
    }

    if (!child->is_leaf) {
        for (int j = 0; j < 2; j++) {
            new_child->children[j] = child->children[j + 2];
        }
    }

    child->num_keys = MIN_KEYS;

    for (int j = parent->num_keys; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[index + 1] = new_child;

    for (int j = parent->num_keys - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[index] = child->keys[1];
    parent->num_keys++;
}

// 插入非满节点
void insert_non_full(node_t *node, int key) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == MAX_KEYS) {
            split_child(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

// 插入新的键
void insert(node_t **root, int key) {
    if ((*root)->num_keys == MAX_KEYS) {
        node_t *new_root = create_node(0);
        new_root->children[0] = *root;
        split_child(new_root, 0, *root);
        *root = new_root;
        insert_non_full(*root, key);
    } else {
        insert_non_full(*root, key);
    }
}

// 搜索给定键
node_t *search(node_t *node, int key) {
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }
    if (i < node->num_keys && key == node->keys[i]) {
        return node;
    }
    if (node->is_leaf) {
        return NULL;
    } else {
        return search(node->children[i], key);
    }
}

// 打印树
void print_tree(node_t *node, int level) {
    if (node) {
        for (int i = 0; i < node->num_keys; i++) {
            printf("%*s%d\n", level * 4, "", node->keys[i]);
        }
        if (!node->is_leaf) {
            for (int i = 0; i <= node->num_keys; i++) {
                print_tree(node->children[i], level + 1);
            }
        }
    }
}

// 释放节点
void free_node(node_t *node) {
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            free_node(node->children[i]);
        }
    }
    free(node);
}

// 主函数
int main(int argc, char *argv[]) {
    node_t *root = create_node(1);

    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        insert(&root, keys[i]);
    }

    printf("2-3-4 Tree:\n");
    print_tree(root, 0);

    free_node(root);
    return 0;
}
