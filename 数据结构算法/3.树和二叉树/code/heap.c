/* heap.c - 堆实现（ 核心版本）
 * create by: zw.duan
 */

#include "heap.h"
#include <string.h>
#include <time.h>

// 内部辅助函数声明
void PrintHeapStructureHelper(Heap H, int index, int level);

// ============= 堆基本操作实现 =============

/**
 * 初始化堆
 */
bool InitHeap(Heap *H, int capacity, HeapType type) {
    if (capacity <= 0 || capacity > HEAP_MAXSIZE) return false;
    
    H->data = (HeapElemType*)malloc(capacity * sizeof(HeapElemType));
    if (H->data == NULL) return false;
    
    H->size = 0;
    H->capacity = capacity;
    H->type = type;
    
    return true;
}

/**
 * 销毁堆
 */
void DestroyHeap(Heap *H) {
    if (H->data != NULL) {
        free(H->data);
        H->data = NULL;
    }
    H->size = 0;
    H->capacity = 0;
}

/**
 * 判断堆是否为空
 */
bool HeapEmpty(Heap H) {
    return H.size == 0;
}

/**
 * 判断堆是否已满
 */
bool HeapFull(Heap H) {
    return H.size >= H.capacity;
}

/**
 * 获取堆大小
 */
int HeapSize(Heap H) {
    return H.size;
}

/**
 * 向上调整堆
 */
void HeapifyUp(Heap *H, int index) {
    if (index <= 0) return;
    
    int parent = HeapParent(index);
    bool shouldSwap = false;
    
    if (H->type == MAX_HEAP) {
        shouldSwap = H->data[index] > H->data[parent];
    } else {
        shouldSwap = H->data[index] < H->data[parent];
    }
    
    if (shouldSwap) {
        HeapSwap(&H->data[index], &H->data[parent]);
        HeapifyUp(H, parent);
    }
}

/**
 * 向下调整堆
 */
void HeapifyDown(Heap *H, int index) {
    int left = HeapLeftChild(index);
    int right = HeapRightChild(index);
    int extreme = index;
    
    if (left < H->size) {
        if (H->type == MAX_HEAP) {
            if (H->data[left] > H->data[extreme]) extreme = left;
        } else {
            if (H->data[left] < H->data[extreme]) extreme = left;
        }
    }
    
    if (right < H->size) {
        if (H->type == MAX_HEAP) {
            if (H->data[right] > H->data[extreme]) extreme = right;
        } else {
            if (H->data[right] < H->data[extreme]) extreme = right;
        }
    }
    
    if (extreme != index) {
        HeapSwap(&H->data[index], &H->data[extreme]);
        HeapifyDown(H, extreme);
    }
}

/**
 * 插入元素到堆
 */
bool HeapInsert(Heap *H, HeapElemType elem) {
    if (HeapFull(*H)) return false;
    
    H->data[H->size] = elem;
    H->size++;
    HeapifyUp(H, H->size - 1);
    
    return true;
}

/**
 * 删除堆顶元素
 */
bool HeapDeleteTop(Heap *H, HeapElemType *elem) {
    if (HeapEmpty(*H)) return false;
    
    *elem = H->data[0];
    H->data[0] = H->data[H->size - 1];
    H->size--;
    
    if (H->size > 0) {
        HeapifyDown(H, 0);
    }
    
    return true;
}

/**
 * 删除指定位置的元素
 */
bool HeapDelete(Heap *H, int index, HeapElemType *elem) {
    if (HeapEmpty(*H) || index < 0 || index >= H->size) return false;
    
    *elem = H->data[index];
    H->data[index] = H->data[H->size - 1];
    H->size--;
    
    if (H->size > 0 && index < H->size) {
        HeapifyDown(H, index);
        HeapifyUp(H, index);
    }
    
    return true;
}

/**
 * 获取堆顶元素
 */
HeapElemType HeapTop(Heap H) {
    if (HeapEmpty(H)) return INT_MIN; // 错误值
    return H.data[0];
}

/**
 * 查找元素在堆中的位置
 */
int HeapFind(Heap H, HeapElemType elem) {
    for (int i = 0; i < H.size; i++) {
        if (H.data[i] == elem) return i;
    }
    return -1; // 未找到
}

/**
 * 从数组构建堆
 */
void BuildHeap(Heap *H, HeapElemType *arr, int size) {
    if (size > H->capacity) size = H->capacity;
    
    memcpy(H->data, arr, size * sizeof(HeapElemType));
    H->size = size;
    
    // 从最后一个非叶子结点开始向下调整
    for (int i = (size - 2) / 2; i >= 0; i--) {
        HeapifyDown(H, i);
    }
}

// ============= 堆的辅助操作 =============

/**
 * 获取父结点索引
 */
int HeapParent(int index) {
    return (index - 1) / 2;
}

/**
 * 获取左子节点索引
 */
int HeapLeftChild(int index) {
    return 2 * index + 1;
}

/**
 * 获取右子节点索引
 */
int HeapRightChild(int index) {
    return 2 * index + 2;
}

/**
 * 比较两个元素
 */
bool HeapCompare(Heap H, HeapElemType a, HeapElemType b) {
    if (H.type == MAX_HEAP) {
        return a > b;
    } else {
        return a < b;
    }
}

/**
 * 交换两个元素
 */
void HeapSwap(HeapElemType *a, HeapElemType *b) {
    HeapElemType temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 验证堆性质
 */
bool IsValidHeap(Heap H) {
    return CheckHeapProperty(H, 0);
}

/**
 * 检查指定结点的堆性质
 */
bool CheckHeapProperty(Heap H, int index) {
    if (index >= H.size) return true;
    
    int left = HeapLeftChild(index);
    int right = HeapRightChild(index);
    
    if (left < H.size) {
        if (!HeapCompare(H, H.data[index], H.data[left]) && 
            H.data[index] != H.data[left]) {
            return false;
        }
        if (!CheckHeapProperty(H, left)) return false;
    }
    
    if (right < H.size) {
        if (!HeapCompare(H, H.data[index], H.data[right]) && 
            H.data[index] != H.data[right]) {
            return false;
        }
        if (!CheckHeapProperty(H, right)) return false;
    }
    
    return true;
}

// ============= 堆排序 =============

/**
 * 堆排序
 */
void HeapSort(HeapElemType *arr, int size, bool ascending) {
    if (size <= 1) return;
    
    Heap heap;
    HeapType type = ascending ? MAX_HEAP : MIN_HEAP;
    
    if (!InitHeap(&heap, size, type)) return;
    
    // 构建堆
    BuildHeap(&heap, arr, size);
    
    // 逐个取出堆顶元素
    for (int i = size - 1; i >= 0; i--) {
        HeapElemType top;
        HeapDeleteTop(&heap, &top);
        arr[i] = top;
    }
    
    DestroyHeap(&heap);
}

/**
 * 就地堆排序
 */
void HeapSortInPlace(Heap *H, bool ascending) {
    if (H->size <= 1) return;
    
    // 保存原始类型
    HeapType originalType = H->type;
    H->type = ascending ? MAX_HEAP : MIN_HEAP;
    
    // 重新构建堆
    for (int i = (H->size - 2) / 2; i >= 0; i--) {
        HeapifyDown(H, i);
    }
    
    // 排序过程
    int originalSize = H->size;
    for (int i = H->size - 1; i > 0; i--) {
        HeapSwap(&H->data[0], &H->data[i]);
        H->size--;
        HeapifyDown(H, 0);
    }
    
    // 恢复
    H->size = originalSize;
    H->type = originalType;
}

// ============= 优先队列实现 =============

/**
 * 初始化优先队列
 */
bool InitPriorityQueue(PriorityQueue *PQ, int capacity, HeapType type) {
    return InitHeap(&PQ->heap, capacity, type);
}

/**
 * 销毁优先队列
 */
void DestroyPriorityQueue(PriorityQueue *PQ) {
    DestroyHeap(&PQ->heap);
}

/**
 * 判断优先队列是否为空
 */
bool PQEmpty(PriorityQueue PQ) {
    return HeapEmpty(PQ.heap);
}

/**
 * 判断优先队列是否已满
 */
bool PQFull(PriorityQueue PQ) {
    return HeapFull(PQ.heap);
}

/**
 * 入队
 */
bool Enqueue(PriorityQueue *PQ, HeapElemType elem) {
    return HeapInsert(&PQ->heap, elem);
}

/**
 * 出队
 */
bool Dequeue(PriorityQueue *PQ, HeapElemType *elem) {
    return HeapDeleteTop(&PQ->heap, elem);
}

/**
 * 获取队首元素
 */
HeapElemType PQTop(PriorityQueue PQ) {
    return HeapTop(PQ.heap);
}

// ============= 高级操作（简化实现）=============

/**
 * 查找前K个最大/最小元素
 */
void FindTopK(HeapElemType *arr, int size, int k, HeapElemType *result, bool findMax) {
    if (k <= 0 || k > size) return;
    
    Heap heap;
    // 找最大用最小堆，找最小用最大堆
    HeapType type = findMax ? MIN_HEAP : MAX_HEAP;
    
    if (!InitHeap(&heap, k, type)) return;
    
    for (int i = 0; i < size; i++) {
        if (heap.size < k) {
            HeapInsert(&heap, arr[i]);
        } else {
            HeapElemType top = HeapTop(heap);
            bool shouldReplace = false;
            
            if (findMax && arr[i] > top) {
                shouldReplace = true;
            } else if (!findMax && arr[i] < top) {
                shouldReplace = true;
            }
            
            if (shouldReplace) {
                HeapDeleteTop(&heap, &top);
                HeapInsert(&heap, arr[i]);
            }
        }
    }
    
    // 提取结果
    for (int i = 0; i < k; i++) {
        HeapDeleteTop(&heap, &result[i]);
    }
    
    DestroyHeap(&heap);
}

/**
 * 寻找第K大元素
 */
void FindKthLargest(HeapElemType *arr, int size, int k, HeapElemType *result) {
    if (k <= 0 || k > size) {
        *result = INT_MIN;
        return;
    }
    
    Heap minHeap;
    if (!InitHeap(&minHeap, k, MIN_HEAP)) {
        *result = INT_MIN;
        return;
    }
    
    for (int i = 0; i < size; i++) {
        if (minHeap.size < k) {
            HeapInsert(&minHeap, arr[i]);
        } else if (arr[i] > HeapTop(minHeap)) {
            HeapElemType top;
            HeapDeleteTop(&minHeap, &top);
            HeapInsert(&minHeap, arr[i]);
        }
    }
    
    *result = HeapTop(minHeap);
    DestroyHeap(&minHeap);
}

/**
 * 寻找第K小元素
 */
void FindKthSmallest(HeapElemType *arr, int size, int k, HeapElemType *result) {
    if (k <= 0 || k > size) {
        *result = INT_MAX;
        return;
    }
    
    Heap maxHeap;
    if (!InitHeap(&maxHeap, k, MAX_HEAP)) {
        *result = INT_MAX;
        return;
    }
    
    for (int i = 0; i < size; i++) {
        if (maxHeap.size < k) {
            HeapInsert(&maxHeap, arr[i]);
        } else if (arr[i] < HeapTop(maxHeap)) {
            HeapElemType top;
            HeapDeleteTop(&maxHeap, &top);
            HeapInsert(&maxHeap, arr[i]);
        }
    }
    
    *result = HeapTop(maxHeap);
    DestroyHeap(&maxHeap);
}

// ============= 实用工具函数 =============

/**
 * 打印堆
 */
void PrintHeap(Heap H) {
    printf("堆类型: %s, 大小: %d/%d\n", 
           H.type == MAX_HEAP ? "最大堆" : "最小堆", 
           H.size, H.capacity);
    printf("元素: [");
    for (int i = 0; i < H.size; i++) {
        printf("%d", H.data[i]);
        if (i < H.size - 1) printf(", ");
    }
    printf("]\n");
}

/**
 * 打印堆结构
 */
void PrintHeapStructure(Heap H) {
    printf("堆结构（树形显示）:\n");
    if (H.size == 0) {
        printf("空堆\n");
        return;
    }
    PrintHeapStructureHelper(H, 0, 0);
}

void PrintHeapStructureHelper(Heap H, int index, int level) {
    if (index >= H.size) return;
    
    // 打印右子树
    if (HeapRightChild(index) < H.size) {
        PrintHeapStructureHelper(H, HeapRightChild(index), level + 1);
    }
    
    // 打印当前节点
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%d\n", H.data[index]);
    
    // 打印左子树
    if (HeapLeftChild(index) < H.size) {
        PrintHeapStructureHelper(H, HeapLeftChild(index), level + 1);
    }
}

/**
 * 打印优先队列
 */
void PrintPriorityQueue(PriorityQueue PQ) {
    printf("优先队列 - ");
    PrintHeap(PQ.heap);
}

/**
 * 测试堆的正确性
 */
void TestHeapCorrectness(int testSize) {
    printf("开始堆正确性测试，测试大小: %d\n", testSize);
    
    Heap maxHeap, minHeap;
    InitHeap(&maxHeap, testSize, MAX_HEAP);
    InitHeap(&minHeap, testSize, MIN_HEAP);
    
    // 生成测试数据
    HeapElemType *testData = (HeapElemType*)malloc(testSize * sizeof(HeapElemType));
    GenerateTestData(testData, testSize, 1000);
    
    // 测试插入
    for (int i = 0; i < testSize; i++) {
        HeapInsert(&maxHeap, testData[i]);
        HeapInsert(&minHeap, testData[i]);
    }
    
    // 验证堆性质
    bool maxHeapValid = IsValidHeap(maxHeap);
    bool minHeapValid = IsValidHeap(minHeap);
    
    printf("最大堆验证: %s\n", maxHeapValid ? "通过" : "失败");
    printf("最小堆验证: %s\n", minHeapValid ? "通过" : "失败");
    
    // 测试排序
    HeapSort(testData, testSize, true);
    bool sortCorrect = true;
    for (int i = 1; i < testSize; i++) {
        if (testData[i] < testData[i-1]) {
            sortCorrect = false;
            break;
        }
    }
    printf("堆排序验证: %s\n", sortCorrect ? "通过" : "失败");
    
    free(testData);
    DestroyHeap(&maxHeap);
    DestroyHeap(&minHeap);
}

/**
 * 生成测试数据
 */
void GenerateTestData(HeapElemType *arr, int size, int range) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % range;
    }
}

/* ============= 说明 =============
 * 
 * 包含了堆的一些核心功能：
 * 1. 堆的基本操作（插入、删除、查找）
 * 2. 堆化操作（上浮、下沉）
 * 3. 建堆操作
 * 4. 堆排序
 * 5. 优先队列应用
 * 6. TopK问题解决方案
 * 
 * 编译方式：
 * gcc -std=c99 -Wall -O2 heap.c test_heap.c -o test_heap
 * 
 * ============= */ 