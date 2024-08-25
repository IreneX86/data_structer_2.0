/**
 * 分布计数排序（计数排序）
 * 
 * 特点：
 * 1.不基于比较
 * 2.适合小范围数组（即最小值和最大值之间跨度不大）
 * 3.稳定的
 * 
 * Author: IreneX86
 * Email: irene.127.0.0.1@gmail.com
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_arr(int arr[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void counting_sort(int arr[], int len)
{
    if (len < 1) return;

    // 寻找最小和最大的元素
    int max = arr[0], min = arr[0];
    for (size_t i = 1; i < len; i++) {
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }

    // 计数数组 count 的范围
    int range = max - min + 1;

    // 分配一个长度为 range 的数组 count 存储计数，并初始化为 0
    int *count = (int *)malloc(sizeof(int) * range);
    memset(count, 0, sizeof(int) * range);

    // 计数
    for (size_t i = 0; i < len; i++)
        count[arr[i] - min]++;

    // 统计计数的累计值cumulative
    for (size_t i = 1; i < range; i++)
        count[i] += count[i - 1];

    // 创建一个临时数组保存结果
    int *res = (int *)malloc(sizeof(int) * len);

    // 将元素放到正确的位置上
    for (size_t i = len; i > 0; i--)
    {
        res[count[arr[i - 1] - min] - 1] = arr[i - 1];
        count[arr[i - 1] - min]--;
    }

    // 将结果复制回原数组
    for (size_t i = 0; i < len; i++)
        arr[i] = res[i];

    // 释放分配的内存
    free(count);
    free(res);
}

int main(int argc, char const *argv[])
{
    int arr[] = {91, 94, 99, 96, 92, 95, 91, 92, 94};
    printf("Origin array: ");
    print_arr(arr, 9);
    counting_sort(arr, 9);
    printf("Sorted array: ");
    print_arr(arr, 9);
    return 0;
}

