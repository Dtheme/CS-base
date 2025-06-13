/* sqlist.h - 顺序表（线性表的顺序存储结构）
 *   线性表专题
 * create by: zw.duan
 * 
 *  
 * 1. 顺序表的逻辑结构 vs 物理结构
 * 2. 静态分配 vs 动态分配的实现区别
 * 3. 基本操作的时间复杂度分析
 * 4. 顺序表 vs 链表的优缺点对比
 * 5. 高级算法：删除、插入、查找、合并等操作
 */

#ifndef SQLIST_H
#define SQLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 顺序表的最大容量
#define MAXSIZE 100        // 静态分配的最大长度
#define INIT_SIZE 10       // 动态顺序表初始长度
#define INCREMENT 5        // 动态顺序表扩容增量

// ============= 静态顺序表相关定义 =============

/**
 *  静态顺序表结构定义
 * 
 *   存储结构图示：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₁  │ a₂  │ a₃  │ ... │ aₙ  │ ... │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2           n-1         MAXSIZE-1
 *   ↑                       ↑
 * data[0]                length-1
 * 
 *  核心特点：
 * - 逻辑相邻的元素在物理位置上也相邻
 * - 支持随机访问：O(1)时间复杂度
 * - 插入/删除需要移动元素：O(n)时间复杂度
 * - 空间利用率高，无额外指针开销
 * 
 *  适用场景：
 *  频繁随机访问元素
 *  表长度相对稳定
 *  主要进行查找操作
 *  频繁插入删除操作
 *  表长度变化很大
 */
typedef int ElemType;
typedef struct {
    ElemType data[MAXSIZE]; // 存储数据元素的数组（静态分配）
    int length;             // 顺序表当前长度（有效元素个数）
} SqList;

// 静态顺序表基本操作函数声明
bool InitList(SqList *L);
void DestroyList(SqList *L);
void ClearList(SqList *L);
bool ListEmpty(SqList L);
int ListLength(SqList L);
bool GetElem(SqList L, int i, ElemType *e);
int LocateElem(SqList L, ElemType e);
bool PriorElem(SqList L, ElemType cur_e, ElemType *pre_e);
bool NextElem(SqList L, ElemType cur_e, ElemType *next_e);
bool ListInsert(SqList *L, int i, ElemType e);
bool ListDelete(SqList *L, int i, ElemType *e);
bool ListTraverse(SqList L, bool (*visit)(ElemType));
void PrintList(SqList L);

// ============= 动态顺序表相关定义 =============

/**
 *  动态顺序表结构定义
 * 
 *   动态分配示意图：
 *         heap memory
 * ┌─────────────────────────────────┐
 * │  data → ┌─────┬─────┬─────┬───┐ │
 * │         │ a₁  │ a₂  │ a₃  │...│ │
 * │         └─────┴─────┴─────┴───┘ │
 * │  length: 当前元素个数            │
 * │  maxsize: 数组容量              │
 * └─────────────────────────────────┘
 * 
 *  扩容策略：
 * 1. 检查当前容量是否足够
 * 2. 若不够，申请更大的内存空间
 * 3. 复制原有数据到新空间
 * 4. 释放原有空间
 * 5. 更新指针和容量信息
 * 
 *  优势：
 * - 空间按需分配，节省内存
 * - 支持动态扩容
 * - 仍保持随机访问特性
 */
typedef struct {
    ElemType *data;         // 指向动态分配数组的指针
    int maxsize;            // 数组最大容量
    int length;             // 数组当前长度（有效元素个数）
} DynamicList;

// 动态顺序表基本操作函数声明
bool InitDynamicList(DynamicList *L);
void DestroyDynamicList(DynamicList *L);
bool IncreaseSize(DynamicList *L, int increment);
bool DynamicListInsert(DynamicList *L, int i, ElemType e);
void PrintDynamicList(DynamicList L);

// ============= 高级操作函数声明（ 重点）=============

/*  删除和修改操作
 * 这类操作是 的高频考点，需要熟练掌握：
 * - 时间复杂度分析
 * - 元素移动的具体过程
 * - 边界条件处理
 */

// 删除所有值为x的元素
int DeleteAllX(SqList *L, ElemType x);
// 删除值在[s,t]范围内的所有元素
int DeleteRange(SqList *L, ElemType s, ElemType t);
// 逆置顺序表
bool ReverseList(SqList *L);
// 循环左移k位
bool LeftShift(SqList *L, int k);

/*  有序表操作
 * 这类操作利用有序性质，可以优化算法效率
 */

// 在有序表中插入元素（保持有序）
bool InsertInOrder(SqList *L, ElemType e);
// 删除有序表中的重复元素
int RemoveDuplicates(SqList *L);
// 二分查找（仅适用于有序表）
int BinarySearch(SqList L, ElemType e);

/*   集合运算
 *  常考的表合并问题
 */

// 合并两个有序表
bool MergeList(SqList La, SqList Lb, SqList *Lc);
// 求两个集合的交集
bool Intersection(SqList La, SqList Lb, SqList *Lc);
// 求两个集合的并集
bool Union(SqList La, SqList Lb, SqList *Lc);

// ============= 辅助函数声明 =============

// 比较和访问函数
bool Equal(ElemType a, ElemType b);
int Compare(ElemType a, ElemType b);
void Swap(ElemType *a, ElemType *b);
bool Visit_Print(ElemType e);

// 性能测试函数
void PerformanceTest_Basic(int size);
void PerformanceTest_Advanced(int size);
void PerformanceCompare_StaticVsDynamic(int size);

#endif // SQLIST_H

 