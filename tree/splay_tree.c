/* Splay Tree (伸展树)
 * 
 * 伸展树是一种自调整二叉搜索树（BST），
 * 通过在每次插入或查找操作后，将访问的节点“伸展”到根节点来进行自我调整。
 * 这种操作使得最近访问的节点更接近根，从而提高了后续访问的效率。
 * 
 * 优点：
 * 1. 对于非均匀访问的数据表现优异，频繁访问的节点会被移动到树的顶部。
 * 2. 自动平衡，避免了长时间的退化（例如形成链表）。
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

// 定义树节点结构体
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} node_t;

// 初始化新节点
node_t *new_node(int key) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 右旋操作
node_t *right_rotate(node_t *x) {
    node_t *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// 左旋操作
node_t *left_rotate(node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// 伸展操作，将节点 splay 到根节点
node_t *splay(node_t *root, int key) {
    if (root == NULL || root->key == key)
        return root;

    // key 在左子树中
    if (key < root->key) {
        if (root->left == NULL) return root;

        // Zig-Zig (左左)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = right_rotate(root);
        }
        // Zig-Zag (左右)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = left_rotate(root->left);
        }

        return (root->left == NULL) ? root : right_rotate(root);
    }
    // key 在右子树中
    else {
        if (root->right == NULL) return root;

        // Zag-Zig (右左)
        if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = right_rotate(root->right);
        }
        // Zag-Zag (右右)
        else if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = left_rotate(root);
        }

        return (root->right == NULL) ? root : left_rotate(root);
    }
}

// 插入新节点，并进行伸展操作
node_t *insert(node_t *root, int key) {
    if (root == NULL) return new_node(key);

    root = splay(root, key);

    if (root->key == key) return root;

    node_t *new_root = new_node(key);

    if (key < root->key) {
        new_root->right = root;
        new_root->left = root->left;
        root->left = NULL;
    } else {
        new_root->left = root;
        new_root->right = root->right;
        root->right = NULL;
    }

    return new_root;
}

// 查找节点并进行伸展操作
node_t *search(node_t *root, int key) {
    return splay(root, key);
}

// 输出树中序遍历
void inorder(node_t *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
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
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    printf("Inorder traversal of the splay tree:\n");
    inorder(root);
    printf("\n");

    // 查找节点
    root = search(root, 20);
    printf("Inorder traversal after searching for 20:\n");
    inorder(root);
    printf("\n");

    free_tree(root);
    return 0;
}
