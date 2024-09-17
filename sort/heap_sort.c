#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* heap_sort 堆排序 (用数组实现)*/


void downheap(int* PQ, int k, int n);
void upheap(int* PQ, int k);
int deletemax(int* PQ, int n);
void buildHeap(int *PQ, int n);
void heapSort(int *PQ, int n);
void printArray(int *A, int n);
void swap(int *n1, int *n2);

int main(int argc, char **argv) {
    // 定义一个初始的优先队列，PQ[0]不使用，最大值放在数组的第一个位置
    int PQ[] = {INT_MAX, 40, 59, 37, 11, 26, 14, 29}; 
    int n = sizeof(PQ)/sizeof(*PQ) - 1; // 计算队列的实际大小

    // 排序前打印数组
    printf("Before: ");
    printArray(PQ, n);
    
    // 调用堆排序
    heapSort(PQ, n);

    // 排序后打印数组
    printf("After: ");
    printArray(PQ, n);
}

// 将元素下沉（downheap），用于保持堆的性质
void downheap(int* PQ, int k, int n) {
    int j, v; 
    v = PQ[k];  // 获取当前节点的值
    while (k <= n/2) { /* 如果 A[k] 还有子节点 */
        j = k * 2;  // j 指向左子节点
        /* j 设置为左右子节点中的最大值 */
        if (j < n && PQ[j] < PQ[j+1]) {
            j++;  // 如果右子节点比左子节点大，指向右子节点
        }
        if (v >= PQ[j]){
            break; // 如果当前节点比最大子节点大，则堆已经满足性质
        }
        // 打印交换信息
        printf("swap %d and %d\n", v, PQ[j]);
        PQ[k] = PQ[j]; // 将较大的子节点上移
        k = j; // 继续下沉
    } 
    PQ[k] = v; // 将当前值放在最终位置
}

// 构建堆
void buildHeap(int *PQ, int n) {
    // 从堆的中间开始，对每个节点调用 downheap
    for (int k = n/2; k >= 1; k--) {
        downheap(PQ, k, n);
    }
}

// 删除最大元素并修复堆
int deletemax(int* PQ, int n) { 
    int max = PQ[1]; // 最大元素是堆顶元素，即 PQ[1]
    printf("Deleting max element: %d\n", max);
    
    PQ[1] = PQ[n];  // 用最后一个元素替换堆顶
    downheap(PQ, 1, n-1);  // 修复堆的性质
    return max;
}

// 堆排序函数
void heapSort(int *PQ, int n) {
    // 第一步：构建最大堆
    buildHeap(PQ, n); 
    
    // 第二步：依次删除堆顶元素并调整堆
    for (int i = n; i >= 2; i--) {
        // 打印交换信息，将堆顶元素放到数组末尾
        printf("Swapping %d and %d\n", PQ[1], PQ[i]);
        swap(&PQ[1], &PQ[i]);  // 交换堆顶和末尾元素
        downheap(PQ, 1, i-1);  // 对剩下的元素重新进行堆调整
    }
}

// 打印数组内容
void printArray(int *A, int n) {
    for (int i = 1; i <= n; i++) {
        printf("%3d  ", A[i]);
    }
    printf("\n");
}

// 交换两个数值
void swap(int *n1, int *n2) {
    int tmp = *n1;
    *n1 = *n2;
    *n2 = tmp;
}
