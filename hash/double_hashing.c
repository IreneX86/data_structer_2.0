/*
 * Double Hashing (双重哈希) （又叫rehashing）
 * 
 * 原理：是一种解决哈希冲突的开放寻址方法。当第一次哈希函数产生冲突时，
 *       使用第二个哈希函数计算出一个新的索引，直到找到一个空位置或遍历整个哈希表。
 * 
 * 优点：
 *       相比线性探测和二次探测，Double Hashing 可以更好地减少堆积现象。
 * 
 * 缺点：
 *       当表接近满时，插入和查找效率可能会下降。
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

// 哈希函数1，计算键的哈希值
int hash1(int key) {
    return key % SIZE;
}

// 哈希函数2，计算用于探测的增量
int hash2(int key) {
    return 7 - (key % 7); // 使用一个小于 SIZE 的质数
}

// 双重哈希探测函数，解决哈希冲突
int double_hash(int hash_table[], int key) {
    int index = hash1(key); // 计算键的第一个哈希值
    int stepSize = hash2(key); // 计算探测的增量

    while (hash_table[index] != -1) { // 如果哈希表当前位置不为空
        index = (index + stepSize) % SIZE; // 使用双重哈希进行探测
    }
    return index; // 返回可以插入键的位置
}

int main(int argc, char *argv[]) {
    int hash_table[SIZE]; 
    // 使用 `memset` 函数将整个数组初始化为 -1
    memset(hash_table, -1, sizeof(hash_table));

    int keys[] = {1,4,11,2,21,5};
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        int index = double_hash(hash_table, keys[i]); // 使用双重哈希找到可以插入键的位置
        hash_table[index] = keys[i]; // 将键插入哈希表
    }

    // 输出哈希表
    for (int i = 0; i < SIZE; i++) {
        if(hash_table[i] == -1) {
            printf("NULL -> ");
        } else {
            printf("%d -> ", hash_table[i]);
        }
    }
    printf("NULL\n");

    return 0;
}
