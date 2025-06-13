/* search.h - 查找算法实现
 * create by: zw.duan
 * 
 * 包含 所有查找算法：
 * 1. 线性查找（顺序查找）
 * 2. 二分查找（折半查找）
 * 3. 插值查找
 * 4. 斐波那契查找
 * 5. 分块查找（索引顺序查找）
 * 6. 树表查找（BST、AVL、红黑树、B树、B+树）
 * 7. 散列查找（哈希表）
 */

#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

// 引入树结构头文件
#include "../../3.树和二叉树/code/bst.h"
#include "../../3.树和二叉树/code/rbtree.h"
#include "../../3.树和二叉树/code/btree.h"
#include "../../3.树和二叉树/code/bplustree.h"

// ============= 基本查找相关定义 =============

#define MAXSIZE 100          // 最大数组大小
#define MAXBLOCKS 10         // 最大分块数
#define BLOCKSIZE 10         // 每块最大大小

/**
 * 查找结果结构
 */
typedef struct {
    int index;               // 找到的元素位置（-1表示未找到）
    int comparisons;         // 比较次数
    bool found;              // 是否找到
    double time_cost;        // 查找耗时（毫秒）
} SearchResult;

/**
 * 查找统计信息
 */
typedef struct {
    int total_searches;      // 总查找次数
    int successful_searches; // 成功查找次数
    int total_comparisons;   // 总比较次数
    double total_time;       // 总耗时
    double avg_comparisons;  // 平均比较次数
    double success_rate;     // 成功率
} SearchStats;

// ============= 基本查找算法（ 核心）=============

/**
 *  顺序查找（线性查找，Linear Search， 基础）
 * 
 *   顺序查找过程图示（在数组中查找元素5）：
 * 
 * 数组：[2, 7, 5, 3, 8, 1, 9, 4]
 * 查找5的过程：
 * 步骤1：比较 2 ≠ 5，继续
 * 步骤2：比较 7 ≠ 5，继续
 * 步骤3：比较 5 = 5，找到！返回下标2
 * 
 *  算法特点：
 * - 适用于任何存储结构（数组、链表）
 * - 对数据无序性要求
 * - 实现简单，但效率较低
 * - 时间复杂度：O(n)
 * 
 *   复杂度分析：
 * - 最好情况：O(1) - 第一个元素就是目标
 * - 最坏情况：O(n) - 目标在最后或不存在
 * - 平均情况：O(n/2) - 目标在中间位置
 * - 空间复杂度：O(1)
 * 
 *  
 * - 理解平均查找长度ASL的计算
 * - 成功查找：ASL = (n+1)/2
 * - 失败查找：ASL = n+1
 * - 哨兵优化：减少边界判断
 * 
 * @param arr 数组指针
 * @param n 数组长度
 * @param key 查找关键字
 * @return 查找结果结构
 */
SearchResult SequentialSearch(int arr[], int n, int key);

/**
 *  带哨兵的顺序查找（优化版本）
 * 
 *   哨兵优化原理：
 * - 在数组末尾设置哨兵元素（待查找的key）
 * - 避免每次循环都检查数组边界
 * - 减少比较次数，提高效率
 * 
 * @param arr 数组指针（末尾预留哨兵位置）
 * @param n 数组长度
 * @param key 查找关键字
 * @return 查找结果结构
 */
SearchResult SequentialSearchWithSentinel(int arr[], int n, int key);

/**
 *  二分查找（折半查找，Binary Search， 重点）
 * 
 *   二分查找过程图示（在有序数组中查找元素7）：
 * 
 * 数组：[1, 3, 5, 7, 9, 11, 13, 15]
 * 查找7的过程：
 * 
 * 步骤1：low=0, high=7, mid=3
 *        arr[3]=7, 找到！返回下标3
 * 
 * 示例2：查找6
 * 步骤1：low=0, high=7, mid=3, arr[3]=7 > 6, high=2
 * 步骤2：low=0, high=2, mid=1, arr[1]=3 < 6, low=2
 * 步骤3：low=2, high=2, mid=2, arr[2]=5 < 6, low=3
 * 步骤4：low=3 > high=2, 查找失败
 * 
 *  算法特点：
 * - 要求数组必须有序
 * - 每次比较可以排除一半的元素
 * - 效率远高于顺序查找
 * - 适用于静态查找表
 * 
 *   复杂度分析：
 * - 时间复杂度：O(log n)
 * - 空间复杂度：O(1)（迭代版本）
 * - 最多比较次数：⌊log₂n⌋ + 1
 * 
 *  
 * - 理解判定树的构造
 * - 掌握ASL的计算方法
 * - 成功查找：ASL = (log₂(n+1)) - 1
 * - 失败查找：ASL = log₂(n+1)
 * - 递归与迭代两种实现方式
 * 
 * @param arr 有序数组指针
 * @param n 数组长度
 * @param key 查找关键字
 * @return 查找结果结构
 */
SearchResult BinarySearch(int arr[], int n, int key);

/**
 *  递归版本的二分查找
 * 
 * @param arr 有序数组指针
 * @param low 查找范围下界
 * @param high 查找范围上界
 * @param key 查找关键字
 * @param comparisons 比较次数统计
 * @return 查找位置（-1表示未找到）
 */
int BinarySearchRecursive(int arr[], int low, int high, int key, int* comparisons);

/**
 *  插值查找（Interpolation Search， 了解）
 * 
 *   插值查找改进原理：
 * 
 * 二分查找：mid = (low + high) / 2
 * 插值查找：mid = low + (key - arr[low]) / (arr[high] - arr[low]) * (high - low)
 * 
 *  算法思想：
 * - 根据关键字的值来确定查找位置
 * - 类似于查字典的方式
 * - 适用于数据分布均匀的有序数组
 * - 对于分布不均匀的数据可能退化为顺序查找
 * 
 *   复杂度分析：
 * - 平均时间复杂度：O(log log n)
 * - 最坏时间复杂度：O(n)
 * - 空间复杂度：O(1)
 * 
 * @param arr 有序数组指针
 * @param n 数组长度
 * @param key 查找关键字
 * @return 查找结果结构
 */
SearchResult InterpolationSearch(int arr[], int n, int key);

/**
 *  斐波那契查找（Fibonacci Search， 了解）
 * 
 *   斐波那契查找原理：
 * - 利用斐波那契数列的特性来分割数组
 * - 分割点不是中点，而是斐波那契数列的分割点
 * - 避免了除法运算，只使用加法和减法
 * 
 *  算法特点：
 * - 适用于数据量大且不能随机访问的情况
 * - 分割比例基于黄金分割比
 * - 实现相对复杂
 * 
 * @param arr 有序数组指针
 * @param n 数组长度
 * @param key 查找关键字
 * @return 查找结果结构
 */
SearchResult FibonacciSearch(int arr[], int n, int key);

// ============= 分块查找（索引顺序查找）=============

/**
 * 分块查找的块结构
 */
typedef struct {
    int max_key;             // 块中最大关键字
    int start_index;         // 块的起始位置
    int length;              // 块的长度
} Block;

/**
 * 分块查找表结构
 */
typedef struct {
    Block blocks[MAXBLOCKS]; // 块索引表
    int data[MAXSIZE];       // 数据数组
    int block_count;         // 块数
    int data_count;          // 数据总数
} BlockSearchTable;

/**
 *  分块查找（索引顺序查找，Block Search， 重点）
 * 
 *   分块查找结构图示：
 * 
 * 索引表：  [块1:max=10] [块2:max=20] [块3:max=30]
 *             ↓           ↓           ↓
 * 数据块：  [3,7,10]   [12,15,20]  [25,28,30]
 * 
 * 查找过程（查找15）：
 * 步骤1：在索引表中查找，15 > 10 且 15 ≤ 20，确定在块2
 * 步骤2：在块2中顺序查找，找到15
 * 
 *  算法特点：
 * - 块间有序，块内可以无序
 * - 兼顾了查找效率和插入删除的灵活性
 * - 适用于动态查找表
 * - 是顺序查找和二分查找的折中方案
 * 
 *   复杂度分析：
 * - 时间复杂度：O(√n)
 * - 空间复杂度：O(√n)（用于存储索引）
 * - 平均查找长度：ASL = (√n + 1) / 2
 * 
 *  
 * - 理解分块的原理和适用场景
 * - 掌握ASL的计算方法
 * - 最优分块：每块大小为√n
 * - 块数为√n时，ASL最小
 * 
 * @param table 分块查找表指针
 * @param key 查找关键字
 * @return 查找结果结构
 */
SearchResult BlockSearch(BlockSearchTable* table, int key);

void InitBlockTable(BlockSearchTable* table);
bool InsertIntoBlock(BlockSearchTable* table, int key);
void PrintBlockTable(BlockSearchTable* table);

// ============= 树表查找封装 =============

/**
 * 树表查找结果结构
 */
typedef struct {
    bool found;              // 是否找到
    int comparisons;         // 比较次数
    double time_cost;        // 查找耗时
    void* node;              // 找到的节点指针
} TreeSearchResult;

/**
 *  二叉搜索树查找封装
 */
TreeSearchResult BST_Search(BSTree tree, int key);

/**
 *  红黑树查找封装
 */
TreeSearchResult RBTree_Search(RBTree* tree, int key);

/**
 *  B树查找封装
 */
TreeSearchResult BTree_Search(BTree* tree, int key);

/**
 *  B+树查找封装
 */
TreeSearchResult BPlusTree_Search(BPlusTree* tree, int key);

// ============= 查找算法性能比较 =============

/**
 * 查找算法性能测试结果
 */
typedef struct {
    char algorithm_name[50]; // 算法名称
    double avg_time;         // 平均查找时间
    double avg_comparisons;  // 平均比较次数
    double success_rate;     // 成功率
    int test_count;          // 测试次数
} PerformanceResult;

/**
 * 🔬 查找算法性能比较测试
 * 
 *   测试内容：
 * 1. 不同数据规模下的性能表现
 * 2. 不同查找算法的比较次数
 * 3. 时间复杂度的实际验证
 * 4. 不同数据分布下的性能差异
 * 
 * @param data_sizes 测试数据规模数组
 * @param size_count 数据规模数量
 * @param test_times 每个规模测试次数
 * @param results 性能结果数组
 */
void CompareSearchAlgorithms(int data_sizes[], int size_count, int test_times, 
                           PerformanceResult results[]);

void GenerateTestData(int arr[], int n, bool sorted, bool uniform);
void PrintPerformanceResults(PerformanceResult results[], int count);

// ============= 查找算法应用示例 =============

/**
 * 学生信息结构（查找应用示例）
 */
typedef struct {
    int student_id;          // 学号
    char name[50];           // 姓名
    float score;             // 成绩
    int class_id;            // 班级号
} Student;

/**
 * 学生信息查找系统
 */
typedef struct {
    Student* students;       // 学生数组
    int count;               // 学生数量
    int capacity;            // 容量
    bool sorted_by_id;       // 是否按学号排序
    bool sorted_by_score;    // 是否按成绩排序
} StudentSearchSystem;

void InitStudentSystem(StudentSearchSystem* system, int capacity);
bool AddStudent(StudentSearchSystem* system, int id, const char* name, float score, int class_id);
Student* FindStudentById(StudentSearchSystem* system, int id);
Student* FindStudentByName(StudentSearchSystem* system, const char* name);
void FindStudentsByScoreRange(StudentSearchSystem* system, float min_score, float max_score,
                             Student results[], int* count);
void SortStudentsById(StudentSearchSystem* system);
void SortStudentsByScore(StudentSearchSystem* system);
void PrintStudentSystem(StudentSearchSystem* system);

// ============= 查找算法工具函数 =============

// 数据生成
void GenerateRandomArray(int arr[], int n, int min_val, int max_val);
void GenerateOrderedArray(int arr[], int n, int start_val, int step);
void GenerateFibonacci(int fib[], int n);

// 数组操作
void QuickSort(int arr[], int low, int high);
int Partition(int arr[], int low, int high);
void SortArray(int arr[], int n);
bool IsArraySorted(int arr[], int n);
void PrintArray(int arr[], int n);
void CopyArray(int src[], int dest[], int n);

// 查找统计
void InitSearchStats(SearchStats* stats);
void UpdateSearchStats(SearchStats* stats, SearchResult result);
void PrintSearchStats(SearchStats* stats);

// 性能测试
double GetCurrentTime(void);
void TimeSearchAlgorithm(SearchResult (*search_func)(int[], int, int), 
                        int arr[], int n, int key, const char* algorithm_name);

// =============  真题演示 =============

/**
 *  真题演示函数
 */
void ExamDemo_SearchASL(void);           // 平均查找长度计算
void ExamDemo_BinarySearchTree(void);    // 二分查找判定树
void ExamDemo_BlockSearch(void);         // 分块查找
void ExamDemo_SearchComparison(void);    // 查找算法比较

/**
 * 查找算法教学演示
 */
void SearchAlgorithmDemo(void);
void InteractiveSearchDemo(void);

#endif // SEARCH_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 search.c hashtable.c tree_search.c test_search.c -o test_search
 * 需要链接树结构的相关库文件
 * ================================ */ 