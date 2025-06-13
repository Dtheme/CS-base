/* heap.h - 堆
 * create by: zw.duan
 */

#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// 堆的最大容量
#define HEAP_MAXSIZE 1000

// ============= 堆的相关定义 =============

/**
 * 堆的类型枚举
 */
typedef enum {
    MIN_HEAP = 0,  // 最小堆
    MAX_HEAP = 1   // 最大堆
} HeapType;

/**
 * 堆的元素类型
 */
typedef int HeapElemType;

/**
 * 堆结构定义
 */
typedef struct {
    HeapElemType *data;     // 存储堆元素的数组
    int size;               // 堆中元素个数
    int capacity;           // 堆的容量
    HeapType type;          // 堆的类型（最大堆或最小堆）
} Heap;

// ============= 堆的基本操作 =============

// 初始化和销毁
bool InitHeap(Heap *H, int capacity, HeapType type);
void DestroyHeap(Heap *H);
bool HeapEmpty(Heap H);
bool HeapFull(Heap H);
int HeapSize(Heap H);

// 插入和删除
bool HeapInsert(Heap *H, HeapElemType elem);
bool HeapDeleteTop(Heap *H, HeapElemType *elem);
bool HeapDelete(Heap *H, int index, HeapElemType *elem);

// 查找操作
HeapElemType HeapTop(Heap H);
int HeapFind(Heap H, HeapElemType elem);

// 堆的维护操作
void HeapifyUp(Heap *H, int index);
void HeapifyDown(Heap *H, int index);
void BuildHeap(Heap *H, HeapElemType *arr, int size);

// ============= 堆的辅助操作 =============

// 位置计算
int HeapParent(int index);
int HeapLeftChild(int index);
int HeapRightChild(int index);

// 比较和交换操作
bool HeapCompare(Heap H, HeapElemType a, HeapElemType b);
void HeapSwap(HeapElemType *a, HeapElemType *b);

// 验证堆的性质
bool IsValidHeap(Heap H);
bool CheckHeapProperty(Heap H, int index);

// ============= 堆排序 =============

void HeapSort(HeapElemType *arr, int size, bool ascending);
void HeapSortInPlace(Heap *H, bool ascending);

// ============= 优先队列应用 =============

/**
 * 优先队列结构（基于堆实现）
 */
typedef struct {
    Heap heap;
} PriorityQueue;

// 优先队列操作
bool InitPriorityQueue(PriorityQueue *PQ, int capacity, HeapType type);
void DestroyPriorityQueue(PriorityQueue *PQ);
bool PQEmpty(PriorityQueue PQ);
bool PQFull(PriorityQueue PQ);
bool Enqueue(PriorityQueue *PQ, HeapElemType elem);
bool Dequeue(PriorityQueue *PQ, HeapElemType *elem);
HeapElemType PQTop(PriorityQueue PQ);

// ============= 堆的重要应用 =============

// TopK问题
void FindTopK(HeapElemType *arr, int size, int k, HeapElemType *result, bool findMax);
void FindKthLargest(HeapElemType *arr, int size, int k, HeapElemType *result);
void FindKthSmallest(HeapElemType *arr, int size, int k, HeapElemType *result);

// ============= 实用工具函数 =============

void PrintHeap(Heap H);
void PrintHeapStructure(Heap H);
void PrintPriorityQueue(PriorityQueue PQ);

// 测试和验证函数
void TestHeapCorrectness(int testSize);
void GenerateTestData(HeapElemType *arr, int size, int range);

#endif // HEAP_H

/* ================================
 * 堆实现说明：
 * 
 * 堆是一种特殊的完全二叉树，满足堆序性质：
 * - 最大堆：每个父节点的值都大于等于其子节点的值
 * - 最小堆：每个父节点的值都小于等于其子节点的值
 * 
 * 时间复杂度：
 * - 插入：O(log n)
 * - 删除堆顶：O(log n)
 * - 查找堆顶：O(1)
 * - 建堆：O(n)
 * 
 * 空间复杂度：O(n)
 * 
 * 主要应用：
 * - 优先队列
 * - 堆排序
 * - 选择问题（TopK问题）
 * - 图算法（Dijkstra、Prim等）
 * 
 * 编译建议：
 * gcc -std=c99 -Wall -O2 heap.c test_heap.c -o test_heap
 * ================================ */ 