/* k-D Tree
 *
 * k-D Tree is a binary tree that stores points in a k-dimensional space.
 * Each node in the tree is a k-dimensional point. 
 * The tree divides the space into smaller regions based on the dimensions.
 *
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

#define K 2  // Define the dimensionality of the space (2D in this case)

typedef struct Node
{
    int point[K];           // Array to hold k-dimensional point
    struct Node *left, *right;  // Pointers to left and right children
} node_t;

// Function to create a new k-D Tree node
node_t* create_node(int point[])
{
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    for (int i = 0; i < K; i++)
        new_node->point[i] = point[i];
    new_node->left = new_node->right = NULL;
    return new_node;
}

// Function to insert a new point into the k-D Tree
node_t* insert(node_t* root, int point[], int depth)
{
    if (root == NULL)
        return create_node(point);

    // Calculate the current dimension (depth % K)
    int cd = depth % K;

    if (point[cd] < root->point[cd])
        root->left = insert(root->left, point, depth + 1);
    else
        root->right = insert(root->right, point, depth + 1);

    return root;
}

// Function to print the k-D Tree in-order
void in_order_print(node_t* root)
{
    if (root != NULL)
    {
        in_order_print(root->left);
        printf("(%d, %d)\n", root->point[0], root->point[1]);
        in_order_print(root->right);
    }
}

// Function to check if two points are the same
int are_points_same(int point1[], int point2[])
{
    for (int i = 0; i < K; i++)
        if (point1[i] != point2[i])
            return 0;
    return 1;
}

// Function to search a point in the k-D Tree
int search(node_t* root, int point[], int depth)
{
    if (root == NULL)
        return 0;
    if (are_points_same(root->point, point))
        return 1;

    int cd = depth % K;
    if (point[cd] < root->point[cd])
        return search(root->left, point, depth + 1);
    return search(root->right, point, depth + 1);
}

int main()
{
    node_t* root = NULL;
    int points[][K] = {{3, 6}, {17, 15}, {13, 15}, {6, 12}, {9, 1}, {2, 7}, {10, 19}};
    int n = sizeof(points)/sizeof(points[0]);

    for (int i = 0; i < n; i++)
        root = insert(root, points[i], 0);

    printf("In-order traversal of the k-D Tree:\n");
    in_order_print(root);

    int point1[] = {10, 19};
    (search(root, point1, 0))? printf("Found\n"): printf("Not Found\n");

    int point2[] = {12, 19};
    (search(root, point2, 0))? printf("Found\n"): printf("Not Found\n");

    return 0;
}
