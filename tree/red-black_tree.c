/* Red-Black Tree (红黑树)
 * 
 * 红黑树是一种自平衡的二叉搜索树，具有以下特性：
 * 1. 每个节点不是红色就是黑色。
 * 2. 根节点是黑色的。
 * 3. 所有叶子节点（NIL节点）都是黑色的。
 * 4. 如果一个节点是红色的，那么它的两个子节点必须是黑色的（即不能有两个连续的红色节点）。
 * 5. 从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点。
 * 
 * 红黑树的插入和删除操作通过旋转和重新着色来保持这些性质，从而使树保持平衡。
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

// 定义颜色常量
typedef enum { RED, BLACK } color_t;

// 定义树节点结构体
typedef struct Node {
    int key;
    color_t color;
    struct Node *left, *right, *parent;
} node_t;

// 初始化新节点
node_t *new_node(int key) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->key = key;
    node->color = RED; // 新插入的节点初始为红色
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

// 左旋操作
void left_rotate(node_t **root, node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 右旋操作
void right_rotate(node_t **root, node_t *y) {
    node_t *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;
    y->parent = x;
}

// 插入修复函数，确保红黑树的性质
void insert_fixup(node_t **root, node_t *z) {
    while (z != *root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            node_t *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(root, z->parent->parent);
            }
        } else {
            node_t *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

// 插入节点
void insert(node_t **root, int key) {
    node_t *z = new_node(key);
    node_t *y = NULL;
    node_t *x = *root;

    while (x != NULL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == NULL)
        *root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    insert_fixup(root, z);
}

// 中序遍历树
void inorder(node_t *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d(%s) ", root->key, root->color == RED ? "R" : "B");
        inorder(root->right);
    }
}

// 释放树
void free_tree(node_t *root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    node_t *root = NULL;

    // 插入节点
    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 30);
    insert(&root, 15);
    insert(&root, 25);
    insert(&root, 5);

    printf("Inorder traversal of the red-black tree:\n");
    inorder(root);
    printf("\n");

    free_tree(root);
    return 0;
}
