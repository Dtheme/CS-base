/* sort.h - 排序算法头文件
 * create by: zw.duan
 * 
 * 数据结构 - 排序算法完整实现
 * 
 * 包含内容：
 * 1. 插入排序类：直接插入、折半插入、希尔排序
 * 2. 选择排序类：简单选择、堆排序
 * 3. 交换排序类：冒泡排序、快速排序
 * 4. 归并排序
 * 5. 基数排序
 * 6. 排序算法性能分析和比较
 */

#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

// ============= 常量定义 =============

#define MAXSIZE 100000          // 最大数组大小
#define RADIX 10               // 基数排序的基数

// ============= 数据结构定义 =============

/**
 * 排序结果统计结构
 */
typedef struct {
    char algorithm_name[50];    // 算法名称
    int comparisons;           // 比较次数
    int swaps;                // 交换次数
    double time_ms;           // 执行时间（毫秒）
    bool is_stable;           // 是否稳定
    char time_complexity[20]; // 时间复杂度
    char space_complexity[20]; // 空间复杂度
} SortResult;

/**
 * 测试数据类型枚举
 */
typedef enum {
    RANDOM_DATA,      // 随机数据
    SORTED_DATA,      // 已排序数据
    REVERSE_DATA,     // 逆序数据
    NEARLY_SORTED,    // 基本有序数据
    DUPLICATE_DATA    // 大量重复数据
} DataType;

// ============= 全局统计变量 =============
extern int comparison_count;   // 比较次数计数器
extern int swap_count;        // 交换次数计数器

// ============= 工具函数声明 =============

/**
 * 交换两个元素
 */
void Swap(int *a, int *b);

/**
 * 比较两个元素（带计数）
 */
bool Compare(int a, int b);

/**
 * 重置统计计数器
 */
void ResetCounters(void);

/**
 * 打印数组内容
 */
void PrintArray(int arr[], int n, const char* title);

/**
 * 验证数组是否已排序
 */
bool IsSorted(int arr[], int n);

/**
 * 生成测试数据
 */
void GenerateTestData(int arr[], int n, DataType type);

/**
 * 复制数组
 */
void CopyArray(int src[], int dest[], int n);

// ============= 插入排序类算法 =============

/**
 *  直接插入排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n²)，最好情况O(n)
 * - 空间复杂度：O(1)
 * - 稳定性：稳定
 * - 适用场景：小规模数据、基本有序数据
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void InsertionSort(int arr[], int n);

/**
 *  折半插入排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n²)，减少比较次数
 * - 空间复杂度：O(1)
 * - 稳定性：稳定
 * - 优化：使用二分查找确定插入位置
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void BinaryInsertionSort(int arr[], int n);

/**
 *  希尔排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n^1.3) ~ O(n²)
 * - 空间复杂度：O(1)
 * - 稳定性：不稳定
 * - 优化：通过增量序列减少插入排序的移动次数
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void ShellSort(int arr[], int n);

// ============= 选择排序类算法 =============

/**
 *  简单选择排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n²)
 * - 空间复杂度：O(1)
 * - 稳定性：不稳定
 * - 特点：交换次数最少
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void SelectionSort(int arr[], int n);

/**
 *  堆排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n log n)
 * - 空间复杂度：O(1)
 * - 稳定性：不稳定
 * - 特点：最坏情况下仍为O(n log n)
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void HeapSort(int arr[], int n);

/**
 * 堆调整函数
 */
void Heapify(int arr[], int n, int i);

/**
 * 建立最大堆
 */
void BuildMaxHeap(int arr[], int n);

// ============= 交换排序类算法 =============

/**
 *  冒泡排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n²)，最好情况O(n)
 * - 空间复杂度：O(1)
 * - 稳定性：稳定
 * - 特点：相邻元素比较交换
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void BubbleSort(int arr[], int n);

/**
 *  快速排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：平均O(n log n)，最坏O(n²)
 * - 空间复杂度：O(log n) ~ O(n)
 * - 稳定性：不稳定
 * - 特点：分治思想，实际应用中最快
 * 
 * @param arr 待排序数组
 * @param low 起始位置
 * @param high 结束位置
 */
void QuickSort(int arr[], int low, int high);

/**
 * 快速排序分区函数
 */
int Partition(int arr[], int low, int high);

/**
 * 快速排序包装函数
 */
void QuickSortWrapper(int arr[], int n);

// ============= 归并排序算法 =============

/**
 *  归并排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(n log n)
 * - 空间复杂度：O(n)
 * - 稳定性：稳定
 * - 特点：分治思想，性能稳定
 * 
 * @param arr 待排序数组
 * @param left 左边界
 * @param right 右边界
 */
void MergeSort(int arr[], int left, int right);

/**
 * 归并函数
 */
void Merge(int arr[], int left, int mid, int right);

/**
 * 归并排序包装函数
 */
void MergeSortWrapper(int arr[], int n);

// ============= 基数排序算法 =============

/**
 *  基数排序 - 数据结构重点算法
 * 
 *   算法特点：
 * - 时间复杂度：O(d(n+r))，d为位数，r为基数
 * - 空间复杂度：O(n+r)
 * - 稳定性：稳定
 * - 特点：非比较排序，适用于整数
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void RadixSort(int arr[], int n);

/**
 * 计数排序（基数排序的子过程）
 */
void CountingSort(int arr[], int n, int exp);

/**
 * 获取数组中的最大值
 */
int GetMax(int arr[], int n);

// ============= 其他排序算法 =============

/**
 * 计数排序
 */
void CountingSortDirect(int arr[], int n);

/**
 * 桶排序
 */
void BucketSort(int arr[], int n);

// ============= 排序算法测试和分析 =============

/**
 * 测试单个排序算法
 */
SortResult TestSortAlgorithm(void (*sort_func)(int[], int), 
                           const char* name, 
                           int arr[], int n,
                           bool is_stable,
                           const char* time_complexity,
                           const char* space_complexity);

/**
 * 测试快速排序（特殊处理）
 */
SortResult TestQuickSort(int arr[], int n);

/**
 * 测试归并排序（特殊处理）
 */
SortResult TestMergeSort(int arr[], int n);

/**
 * 比较所有排序算法
 */
void CompareAllSortAlgorithms(int n, DataType data_type);

/**
 * 打印排序结果
 */
void PrintSortResult(SortResult result);

/**
 * 打印排序算法比较表
 */
void PrintSortComparison(SortResult results[], int count);

// ============= 数据结构真题演示 =============

/**
 * 数据结构真题演示：排序算法分析
 */
void SortExamDemonstration(void);

/**
 * 数据结构真题演示：快速排序过程
 */
void QuickSortExamDemo(void);

/**
 * 数据结构真题演示：堆排序过程
 */
void HeapSortExamDemo(void);

/**
 * 数据结构真题演示：归并排序过程
 */
void MergeSortExamDemo(void);

/**
 * 排序算法综合演示
 */
void SortComprehensiveDemo(void);

#endif // SORT_H 