/*
    循环队列(circular queue)

    用一个环形的数组(array)来表示。
    初始状态：front == rear;

    牺牲掉队列的一个空间，用来标识队满或空。

    操作：1. 初始化队列
         2. 入队
         3. 出队
         4. 判断队满或空

    Author: IreneX86
    Email: Irene.127.0.0.1@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10

typedef struct Queue
{
    void *data[MAXSIZE]; // 用数组来存储循环队列
    int front;           // 数组首部索引(front index)
    int rear;            // 数组尾部索引(rear index)
} queue_t;

// 初始化队列
queue_t *init_queue()
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    queue->front = queue->rear = 0; // 环形数组
    return queue;
}

// 判断队列是否已满
int is_full(queue_t *queue)
{
    return (queue->rear + 1) % MAXSIZE == queue->front;
}

// 判断队列是否为空
int is_empty(queue_t *queue)
{
    return queue->front == queue->rear;
}

// 入队操作
int en_queue(queue_t *queue, void *data)
{
    if (is_full(queue))
        return 0;
    queue->data[queue->rear] = data;
    queue->rear = (queue->rear + 1) % MAXSIZE;
    return 1;
}

// 出队操作
void *de_queue(queue_t *queue)
{
    if (is_empty(queue))
        return NULL;
    void *data = queue->data[queue->front];
    queue->front = (queue->front + 1) % MAXSIZE;
    return data;
}

// 输出队列的所有元素
void print_queue(queue_t *queue, void (*print_data)(void *))
{
    int length = (queue->rear - queue->front + MAXSIZE) % MAXSIZE;
    int index = queue->front;
    for (int i = 0; i < length; i++)
    {
        print_data(queue->data[index]);
        index = (index + 1) % MAXSIZE;
    }
    printf("HEAD\n");
}

// 释放队列内存
void free_queue(queue_t *queue)
{
    free(queue);
}

// 示例：打印整型数据
void print_int(void *data)
{
    printf("%d->", *(int *)data);
}

int main(int argc, char *argv[])
{
    queue_t *Q = init_queue();

    int a = 1, b = 4, c = 9, d = 6, e = 2, f = 5, g = 0;

    en_queue(Q, &a);
    en_queue(Q, &b);
    en_queue(Q, &c);
    en_queue(Q, &d);
    en_queue(Q, &e);
    en_queue(Q, &f);
    en_queue(Q, &g);
    print_queue(Q, print_int);

    int *deq_data = de_queue(Q);
    printf("de_queue data = %d\n", *deq_data);

    deq_data = de_queue(Q);
    printf("de_queue data = %d\n", *deq_data);

    print_queue(Q, print_int);

    free_queue(Q);

    return 0;
}
