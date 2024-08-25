/* 扩容哈希表示例 (Rehashing with Load Factor)
 * 
 * 原理：当哈希表的装载因子达到指定阈值时，增加哈希表的大小并重新哈希所有键值对。
 * 
 * 特点：通过增加哈希表的大小，避免因哈希表变满而导致的效率下降。
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 10    // 初始哈希表大小
#define LOAD_FACTOR 0.75 // 装载因子阈值

// 哈希表结构
typedef struct HashTable {
    int *data;   // 存储键值对的数组
    int size;    // 哈希表的大小
    int count;   // 当前存储的元素数量
} hash_table_t;

// 哈希函数
int hash(int key, int size) {
    return key % size;
}

// 线性探测法处理哈希冲突
int linear_probe(hash_table_t *table, int key) {
    int index = hash(key, table->size);
    while (table->data[index] != -1) {
        index = (index + 1) % table->size;
    }
    return index;
}

// 初始化哈希表
hash_table_t *init_hash_table(int size) {
    hash_table_t *table = (hash_table_t *)malloc(sizeof(hash_table_t));
    table->data = (int *)malloc(sizeof(int) * size);
    memset(table->data, -1, sizeof(int) * size);
    table->size = size;
    table->count = 0;
    return table;
}

// 再哈希（Rehashing）：增加哈希表大小并重新哈希所有键值对
void rehash(hash_table_t *table) {
    int new_size = table->size * 2;
    int *new_data = (int *)malloc(sizeof(int) * new_size);
    memset(new_data, -1, sizeof(int) * new_size);

    // 重新哈希所有现有的键
    for (int i = 0; i < table->size; i++) {
        if (table->data[i] != -1) {
            int new_index = hash(table->data[i], new_size);
            while (new_data[new_index] != -1) {
                new_index = (new_index + 1) % new_size;
            }
            new_data[new_index] = table->data[i];
        }
    }

    // 释放旧数据并更新哈希表
    free(table->data);
    table->data = new_data;
    table->size = new_size;
}

// 插入键值对到哈希表
void insert(hash_table_t *table, int key) {
    if ((float)table->count / table->size >= LOAD_FACTOR) {
        rehash(table);  // 装载因子达到阈值，进行再哈希
    }
    int index = linear_probe(table, key);
    table->data[index] = key;
    table->count++;
}

// 输出哈希表
void print_hash_table(hash_table_t *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->data[i] == -1) {
            printf("NULL -> ");
        } else {
            printf("%d -> ", table->data[i]);
        }
    }
    printf("NULL\n");
}

// 释放哈希表
void free_hash_table(hash_table_t *table) {
    free(table->data);
    free(table);
}

// 主函数
int main(int argc, char *argv[]) {
    hash_table_t *table = init_hash_table(INIT_SIZE);

    // 插入键值对
    int keys[] = {1, 4, 9, 6, 2, 5, 12, 16, 23, 7, 18, 29};
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        insert(table, keys[i]);
    }

    // 输出哈希表
    print_hash_table(table);

    // 插入更多键值对，触发再哈希
    insert(table, 31);
    insert(table, 42);
    insert(table, 10);
    insert(table, 91);
    insert(table, 87);
    insert(table, 55);
    insert(table, 62);

    // 输出哈希表
    print_hash_table(table);

    // 释放哈希表
    free_hash_table(table);

    return 0;
}
