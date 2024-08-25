/*
 * 布谷鸟哈希（Cuckoo Hashing）
 * 
 * 原理：布谷鸟哈希使用两个哈希函数和两个哈希表来处理冲突。
 *       当一个位置发生冲突时，使用另一个哈希函数将冲突元素移到另一个表中。
 *       如果继续发生冲突，则不断尝试移动元素，直到找到空位或达到最大移动次数。
 * 
 * 优点：
 *       常数时间的查找和插入操作。
 * 
 * 缺点：
 *       可能会引入较多的递归移动操作，导致较大的开销。
 *       如果插入操作导致了过多的移动，则可能需要重新构造整个哈希表。
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10
#define MAX_MOVES 10

// 哈希函数1，计算键的哈希值
int hash1(int key) {
    return key % SIZE;
}

// 哈希函数2，计算键的备用哈希值
int hash2(int key) {
    return (key / SIZE) % SIZE; // 通过键值的高位部分计算备用哈希值
}

// 布谷鸟哈希函数，将键插入到哈希表中
void cuckoo_hash(int hash_table1[], int hash_table2[], int key) {
    int moves = 0; // 记录移动次数

    while (moves < MAX_MOVES) {
        int index1 = hash1(key); // 计算哈希值1
        if (hash_table1[index1] == -1) {
            hash_table1[index1] = key; // 插入到第一个哈希表
            return;
        } else {
            int temp = hash_table1[index1]; // 存储冲突元素
            hash_table1[index1] = key; // 将当前键插入第一个哈希表
            key = temp; // 交换元素，继续处理冲突
        }

        int index2 = hash2(key); // 计算哈希值2
        if (hash_table2[index2] == -1) {
            hash_table2[index2] = key; // 插入到第二个哈希表
            return;
        } else {
            int temp = hash_table2[index2]; // 存储冲突元素
            hash_table2[index2] = key; // 将当前键插入第二个哈希表
            key = temp; // 交换元素，继续处理冲突
        }

        moves++; // 增加移动次数
    }

    printf("Exceeded maximum moves, unable to insert key: %d\n", key);
}

// 输出哈希表
void display(int hash_table1[], int hash_table2[]) {
    printf("Hash Table 1: ");
    for (int i = 0; i < SIZE; i++) {
        if (hash_table1[i] == -1) {
            printf("NULL -> ");
        } else {
            printf("%d -> ", hash_table1[i]);
        }
    }
    printf("NULL\n");

    printf("Hash Table 2: ");
    for (int i = 0; i < SIZE; i++) {
        if (hash_table2[i] == -1) {
            printf("NULL -> ");
        } else {
            printf("%d -> ", hash_table2[i]);
        }
    }
    printf("NULL\n");
}

int main(int argc, char *argv[]) {
    int hash_table1[SIZE], hash_table2[SIZE];
    memset(hash_table1, -1, sizeof(hash_table1)); // 初始化第一个哈希表
    memset(hash_table2, -1, sizeof(hash_table2)); // 初始化第二个哈希表

    int keys[] = {1, 4, 9, 6, 2, 5, 11, 12, 13};
    int num_keys = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < num_keys; i++) {
        cuckoo_hash(hash_table1, hash_table2, keys[i]); // 插入键到哈希表
    }

    display(hash_table1, hash_table2);

    return 0;
}
