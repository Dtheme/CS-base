/* bitree.h - 树状数组（Binary Indexed Tree/Fenwick Tree）
 * create by: zw.duan
 * 
 * 树状数组是一种高效的数据结构，用于处理前缀和查询和单点更新操作
 * 时间复杂度：查询和更新都是 O(log n)
 * 空间复杂度：O(n)
 */

#ifndef BITREE_H
#define BITREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= 树状数组相关定义 =============

/**
 * 树状数组结构定义
 */
typedef int BITreeElemType;
typedef struct {
    BITreeElemType *tree;    // 树状数组
    int size;                // 数组大小
    int capacity;            // 容量
} BITree;

// ============= 树状数组基本操作 =============

// 初始化和销毁
BITree* CreateBITree(int size);
void DestroyBITree(BITree *bit);
void InitBITree(BITree *bit, int size);
bool BITreeEmpty(BITree *bit);
int BITreeSize(BITree *bit);

// 核心操作：单点更新和前缀和查询
void Update(BITree *bit, int index, BITreeElemType delta);
BITreeElemType Query(BITree *bit, int index);
BITreeElemType QueryRange(BITree *bit, int left, int right);

// 数组操作
void BuildBITree(BITree *bit, BITreeElemType *arr, int size);
void ResetBITree(BITree *bit);

// ============= 树状数组高级操作 =============

// 区间操作
void UpdateRange(BITree *bit, int left, int right, BITreeElemType delta);
BITreeElemType GetElement(BITree *bit, int index);
void SetElement(BITree *bit, int index, BITreeElemType value);

// 统计操作
int CountLessThan(BITree *bit, BITreeElemType value);
int CountGreaterThan(BITree *bit, BITreeElemType value);
int CountInRange(BITree *bit, BITreeElemType min, BITreeElemType max);

// 查找操作
int FindKthSmallest(BITree *bit, int k);
int FindKthLargest(BITree *bit, int k);
int FindFirstGreaterThan(BITree *bit, BITreeElemType value);
int FindLastLessThan(BITree *bit, BITreeElemType value);

// 逆序对计算
long long CountInversions(BITreeElemType *arr, int size);
long long CountInversionsWithBITree(BITree *bit, BITreeElemType *arr, int size);

// ============= 二维树状数组 =============

typedef struct {
    BITreeElemType **tree;   // 二维树状数组
    int rows;                // 行数
    int cols;                // 列数
} BITree2D;

// 二维树状数组操作
BITree2D* CreateBITree2D(int rows, int cols);
void DestroyBITree2D(BITree2D *bit2d);
void Update2D(BITree2D *bit2d, int row, int col, BITreeElemType delta);
BITreeElemType Query2D(BITree2D *bit2d, int row, int col);
BITreeElemType QueryRange2D(BITree2D *bit2d, int row1, int col1, int row2, int col2);

// ============= 树状数组应用场景 =============

// 动态区间统计
typedef struct {
    BITree *bit;
    int *freq;               // 频率数组
    int max_value;           // 最大值
} DynamicRangeCounter;

DynamicRangeCounter* CreateDynamicRangeCounter(int max_value);
void DestroyDynamicRangeCounter(DynamicRangeCounter *drc);
void AddElement(DynamicRangeCounter *drc, int value);
void RemoveElement(DynamicRangeCounter *drc, int value);
int GetFrequency(DynamicRangeCounter *drc, int value);
int GetRangeFrequency(DynamicRangeCounter *drc, int min, int max);

// 离线查询处理
typedef struct {
    int left, right;         // 查询区间
    int query_id;            // 查询ID
    BITreeElemType result;   // 查询结果
} OfflineQuery;

void ProcessOfflineQueries(BITreeElemType *arr, int size, OfflineQuery *queries, int query_count);

// ============= 工具函数 =============

// 打印和调试
void PrintBITree(BITree *bit);
void PrintBITreeStructure(BITree *bit);
void ValidateBITree(BITree *bit);

// 性能测试
void PerformanceTest_BITree(int size);
void PerformanceCompare_BITree_vs_Array(int size);
void PerformanceTest_2D_BITree(int rows, int cols);

// 内存管理
void ResizeBITree(BITree *bit, int new_size);
void ClearBITree(BITree *bit);

// 序列化和反序列化
void SerializeBITree(BITree *bit, char *filename);
BITree* DeserializeBITree(char *filename);

// ============= 内联函数 =============

/**
 * 获取最低位1的位置
 */
static inline int LowBit(int x) {
    return x & (-x);
}

/**
 * 检查索引是否有效
 */
static inline bool IsValidIndex(BITree *bit, int index) {
    return bit != NULL && index >= 0 && index < bit->size;
}

/**
 * 获取树状数组大小
 */
static inline int GetBITreeSize(BITree *bit) {
    return bit != NULL ? bit->size : 0;
}

#endif // BITREE_H 