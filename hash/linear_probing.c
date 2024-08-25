/* linear probing hash (线性探测哈希)
 * 
 * 原理：是一种解决冲突的开放寻址方法，当发生冲突时，它会沿着哈希表的下一个位置继续查找，直到找到一个空的位置或者遍历整个哈希表。
 * 
 * 优点：
 * 不需要额外的存储空间（如链表的空间）。
 * 
 * 缺点：
 * 可能导致“堆积”现象（Clustering），即多个冲突的键会堆积在相邻位置，导致后续冲突处理变慢。
 * 表接近满时，插入和查找效率下降明显。
 * 
 * Author: IreneX86
 * Email: Irene.127.0.0.1@gmail.com
 */


#include <stdio.h>
#define SIZE 10

int hash(int key)
{
    return key % SIZE;
}

int linear_probe(int hash_table[], int key)
{
    int index = hash(key);          // 计算键的哈希值
    while (hash_table[index] != -1) // 如果哈希表当前位置不为空
    {
        index = (index + 1) % SIZE; // 线性探测下一个位置
    }
    return index; // 返回可以插入键的位置
}

int main(int argc, char *argv[])
{
    int hash_table[SIZE]; 
    // 手动将数组的每个元素初始化为 -1
    for (int i = 0; i < SIZE; i++) {
        hash_table[i] = -1;
    }

    int keys[] = {1, 4, 9, 6, 2, 5};
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
    {
        int index = linear_probe(hash_table, keys[i]); // 线性探测找到可以插入键的位置
        hash_table[index] = keys[i];                   // 将键插入哈希表
    }

    // 输出哈希表
    for (int i = 0; i < SIZE; i++)
    {
        if(hash_table[i] == -1){
            printf("NULL");
        } else {
            printf("%d", hash_table[i]);
        }

        if (i < SIZE - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    return 0;
}
