/* test_heap.c - 堆测试程序（ 核心版本）
 * create by: zw.duan
 */

#include "heap.h"
#include <time.h>
#include <assert.h>

void PrintTestHeader(const char *testName) {
    printf("\n==================================================\n");
    printf("测试项目: %s\n", testName);
    printf("==================================================\n");
}

void PrintTestResult(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

void TestHeapBasicOperations() {
    PrintTestHeader("堆基本操作测试");
    
    Heap maxHeap, minHeap;
    assert(InitHeap(&maxHeap, 100, MAX_HEAP) == true);   // 最大堆
    assert(InitHeap(&minHeap, 100, MIN_HEAP) == true);   // 最小堆
    
    // 测试空堆
    assert(HeapEmpty(maxHeap) == true);
    assert(HeapEmpty(minHeap) == true);
    PrintTestResult("空堆测试", true);
    
    // 插入测试
    int arr[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    for (int i = 0; i < size; i++) {
        assert(HeapInsert(&maxHeap, arr[i]) == true);
        assert(HeapInsert(&minHeap, arr[i]) == true);
    }
    
    PrintTestResult("插入操作测试", true);
    
    // 验证堆性质
    assert(IsValidHeap(maxHeap) == true);
    assert(IsValidHeap(minHeap) == true);
    PrintTestResult("堆性质验证", true);
    
    printf("最大堆:\n");
    PrintHeap(maxHeap);
    printf("最小堆:\n");
    PrintHeap(minHeap);
    
    // 堆顶测试
    assert(HeapTop(maxHeap) == 16);  // 最大值
    assert(HeapTop(minHeap) == 1);   // 最小值
    PrintTestResult("堆顶元素测试", true);
    
    // 删除测试
    HeapElemType top;
    assert(HeapDeleteTop(&maxHeap, &top) == true && top == 16);
    assert(HeapDeleteTop(&minHeap, &top) == true && top == 1);
    
    assert(IsValidHeap(maxHeap) == true);
    assert(IsValidHeap(minHeap) == true);
    PrintTestResult("删除操作测试", true);
    
    // 清理
    DestroyHeap(&maxHeap);
    DestroyHeap(&minHeap);
    PrintTestResult("销毁测试", true);
}

void TestHeapSort() {
    PrintTestHeader("堆排序测试");
    
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 5, 77, 30};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("原始数组: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // 升序排序
    HeapSort(arr, size, true);
    printf("升序排序: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // 验证排序结果
    bool ascending = true;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) {
            ascending = false;
            break;
        }
    }
    PrintTestResult("升序堆排序", ascending);
    
    // 降序排序
    int arr2[] = {64, 34, 25, 12, 22, 11, 90, 5, 77, 30};
    HeapSort(arr2, size, false);
    printf("降序排序: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr2[i]);
    }
    printf("\n");
    
    // 验证排序结果
    bool descending = true;
    for (int i = 1; i < size; i++) {
        if (arr2[i] > arr2[i-1]) {
            descending = false;
            break;
        }
    }
    PrintTestResult("降序堆排序", descending);
}

void TestPriorityQueue() {
    PrintTestHeader("优先队列测试");
    
    PriorityQueue maxPQ, minPQ;
    assert(InitPriorityQueue(&maxPQ, 50, MAX_HEAP) == true);
    assert(InitPriorityQueue(&minPQ, 50, MIN_HEAP) == true);
    
    // 入队测试
    int priorities[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    int size = sizeof(priorities) / sizeof(priorities[0]);
    
    for (int i = 0; i < size; i++) {
        assert(Enqueue(&maxPQ, priorities[i]) == true);
        assert(Enqueue(&minPQ, priorities[i]) == true);
    }
    
    printf("最大优先队列:\n");
    PrintPriorityQueue(maxPQ);
    printf("最小优先队列:\n");
    PrintPriorityQueue(minPQ);
    
    // 出队测试（验证优先级）
    printf("\n最大优先队列出队顺序: ");
    HeapElemType elem;
    HeapElemType prev = INT_MAX;
    while (!PQEmpty(maxPQ)) {
        assert(Dequeue(&maxPQ, &elem) == true);
        assert(elem <= prev);  // 最大堆，应该递减
        printf("%d ", elem);
        prev = elem;
    }
    printf("\n");
    
    printf("最小优先队列出队顺序: ");
    prev = INT_MIN;
    while (!PQEmpty(minPQ)) {
        assert(Dequeue(&minPQ, &elem) == true);
        assert(elem >= prev);  // 最小堆，应该递增
        printf("%d ", elem);
        prev = elem;
    }
    printf("\n");
    
    PrintTestResult("优先队列测试", true);
    
    DestroyPriorityQueue(&maxPQ);
    DestroyPriorityQueue(&minPQ);
}

void TestTopKProblems() {
    PrintTestHeader("TopK问题测试");
    
    int arr[] = {7, 10, 4, 3, 20, 15, 8, 2, 9, 6};
    int size = sizeof(arr) / sizeof(arr[0]);
    int k = 3;
    
    printf("原始数组: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // 测试TopK最大值
    HeapElemType *topKMax = (HeapElemType*)malloc(k * sizeof(HeapElemType));
    FindTopK(arr, size, k, topKMax, true);
    printf("前%d个最大值: ", k);
    for (int i = 0; i < k; i++) {
        printf("%d ", topKMax[i]);
    }
    printf("\n");
    
    // 测试TopK最小值
    HeapElemType *topKMin = (HeapElemType*)malloc(k * sizeof(HeapElemType));
    FindTopK(arr, size, k, topKMin, false);
    printf("前%d个最小值: ", k);
    for (int i = 0; i < k; i++) {
        printf("%d ", topKMin[i]);
    }
    printf("\n");
    
    // 测试第K大元素
    HeapElemType kthLargest;
    FindKthLargest(arr, size, k, &kthLargest);
    printf("第%d大元素: %d\n", k, kthLargest);
    
    // 测试第K小元素
    HeapElemType kthSmallest;
    FindKthSmallest(arr, size, k, &kthSmallest);
    printf("第%d小元素: %d\n", k, kthSmallest);
    
    PrintTestResult("TopK问题测试", true);
    
    free(topKMax);
    free(topKMin);
}

void TestBuildHeap() {
    PrintTestHeader("建堆操作测试");
    
    int arr[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    Heap maxHeap, minHeap;
    assert(InitHeap(&maxHeap, size, MAX_HEAP) == true);
    assert(InitHeap(&minHeap, size, MIN_HEAP) == true);
    
    printf("原始数组: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    // 建最大堆
    BuildHeap(&maxHeap, arr, size);
    printf("建最大堆后:\n");
    PrintHeap(maxHeap);
    PrintHeapStructure(maxHeap);
    
    // 建最小堆
    BuildHeap(&minHeap, arr, size);
    printf("建最小堆后:\n");
    PrintHeap(minHeap);
    PrintHeapStructure(minHeap);
    
    // 验证堆性质
    assert(IsValidHeap(maxHeap) == true);
    assert(IsValidHeap(minHeap) == true);
    PrintTestResult("建堆操作", true);
    
    DestroyHeap(&maxHeap);
    DestroyHeap(&minHeap);
}

void TestCorrectnessOnLargeData() {
    PrintTestHeader("大数据量正确性测试");
    
    TestHeapCorrectness(1000);
    PrintTestResult("大数据量测试", true);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                堆测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
  
    
    TestHeapBasicOperations();
    TestBuildHeap();
    TestHeapSort();
    TestPriorityQueue();
    TestTopKProblems();
    TestCorrectnessOnLargeData();
    
    printf("\n============================================================\n");
    printf("所有堆测试完成！\n");
    printf("============================================================\n");
    
    return 0;
} 