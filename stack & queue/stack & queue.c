/** 栈(stack) 是一种线性表。
 * 特点：后进先出。(Last In First Out)
 *
 * 操作：1.初始化栈
 *     2.入栈
 *     3.出栈
 *     4.判断栈空
 *
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

// 定义节点结构体
typedef struct Node {
    void *data;
    struct Node *next;
} node_t;

// 定义链表结构体，并将其命名为stackADT_t以用于表示栈
typedef struct list {
    node_t *head;
    node_t *tail;
    size_t n; // 栈中元素的数量
} stackADT_t;

// 创建一个新的链表（stack）
stackADT_t *create() {
    stackADT_t *list = (stackADT_t *)malloc(sizeof(stackADT_t));
    list->head = NULL;
    list->tail = NULL;
    list->n = 0;
    return list;
}

// 在链表头部插入一个新元素（stack: push）
void prepend(stackADT_t *stack, void *data) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = stack->head;
    stack->head = new_node;

    if (stack->n == 0) {
        stack->tail = new_node;
    }
    stack->n++;
}

// 在链表尾部插入一个新元素（queue: append）
void append(stackADT_t *stack, void *data) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = NULL;

    if (stack->n == 0) {
        stack->head = new_node;
        stack->tail = new_node;
    } else {
        stack->tail->next = new_node;
        stack->tail = new_node;
    }
    stack->n++;
}

// 删除链表头部的元素并返回其值（stack: pop）
void *deleteHead(stackADT_t *stack) {
    if (stack->n == 0) {
        return NULL; // 栈为空
    }

    node_t *del_node = stack->head;
    void *data = del_node->data;
    stack->head = stack->head->next;

    if (stack->head == NULL) {
        stack->tail = NULL;
    }

    free(del_node);
    stack->n--;
    return data;
}

// 删除链表尾部的元素并返回其值（queue: dequeue）
void *deleteTail(stackADT_t *stack) {
    if (stack->n == 0) {
        return NULL; // 栈为空
    }

    node_t *del_node = stack->tail;
    void *data = del_node->data;

    if (stack->n == 1) {
        stack->head = NULL;
        stack->tail = NULL;
    } else {
        node_t *current = stack->head;
        while (current->next != del_node) {
            current = current->next;
        }
        stack->tail = current;
        current->next = NULL;
    }

    free(del_node);
    stack->n--;
    return data;
}

// 判断栈是否为空
int is_empty(stackADT_t *stack) {
    return stack->n == 0;
}

// 返回栈中元素的个数
size_t stack_size(stackADT_t *stack) {
    return stack->n;
}

// 输出栈的所有元素
void print_stack(stackADT_t *stack, void (*print_data)(void *)) {
    node_t *node = stack->head;
    while (node) {
        print_data(node->data);
        node = node->next;
    }
    printf("NULL\n");
}

// 释放链表（栈）占用的内存
void freeList(stackADT_t *stack) {
    node_t *current = stack->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current);
        current = next;
    }
    free(stack);
}

// 辅助函数：打印整数数据
void print_int(void *data) {
    printf("%d->", *(int *)data);
}

// 示例主函数
int main() {
    stackADT_t *stack = create();

    int data1 = 1, data2 = 2, data3 = 3, data4 = 4;

    prepend(stack, &data1);
    prepend(stack, &data2);
    append(stack, &data3);
    append(stack, &data4);

    printf("elements in stack:\n");
    print_stack(stack, print_int);

    printf("size of stack: %zu\n", stack_size(stack));

    int *popped_data = deleteHead(stack);
    printf("elements popped: %d\n", *popped_data);

    printf("elements in stack:\n");
    print_stack(stack, print_int);

    freeList(stack);

    return 0;
}
