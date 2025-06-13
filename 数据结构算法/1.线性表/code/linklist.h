/* linklist.h - 链表（线性表的链式存储结构）
 *   线性表专题
 * create by: zw.duan
 * 
 *  
 * 1. 单链表、双链表、循环链表的结构差异
 * 2. 带头节点 vs 不带头节点的实现区别
 * 3. 链表基本操作的时间复杂度分析
 * 4. 链表 vs 顺序表的优缺点对比
 * 5. 链表高级算法：反转、查找中间节点、环检测等
 */

#ifndef LINKLIST_H
#define LINKLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= 单链表相关定义 =============

typedef int ElemType;

/**
 * 🔗 单链表节点定义
 * 
 *   单链表存储结构图示：
 * 
 * 头节点（不存储数据）     第1个节点        第2个节点        最后一个节点
 * ┌────┬────┐           ┌────┬────┐     ┌────┬────┐     ┌────┬────┐
 * │    │ ●──┼─────────→ │ a₁ │ ●──┼───→ │ a₂ │ ●──┼───→ │ aₙ │NULL│
 * └────┴────┘           └────┴────┘     └────┴────┘     └────┴────┘
 *      ↑                      ↑               ↑               ↑
 *   head节点               data|next        data|next       data|next
 * 
 *  核心特点：
 * - 逻辑相邻≠物理相邻，通过指针连接
 * - 只能顺序访问，不支持随机访问
 * - 插入/删除方便：O(1)时间复杂度（已知节点位置）
 * - 需要额外空间存储指针
 * 
 *  适用场景：
 *  频繁插入删除操作
 *  表长度变化很大
 *  不需要随机访问
 *  频繁按位置查找
 *  对空间要求严格
 */
typedef struct LNode {
    ElemType data;          // 数据域（存储元素值）
    struct LNode *next;     // 指针域（指向下一个节点）
} LNode, *LinkList;

// 单链表基本操作函数声明
LinkList InitList();
void DestroyList(LinkList L);
void ClearList(LinkList L);
bool ListEmpty(LinkList L);
int ListLength(LinkList L);
bool GetElem(LinkList L, int i, ElemType *e);
LNode* GetElemPtr(LinkList L, int i);
LNode* LocateElem(LinkList L, ElemType e);
int LocateElemPos(LinkList L, ElemType e);
bool ListInsert(LinkList L, int i, ElemType e);
bool ListDelete(LinkList L, int i, ElemType *e);
bool InsertNextNode(LNode *p, ElemType e);
bool DeleteNextNode(LNode *p, ElemType *e);
bool ListTraverse(LinkList L, bool (*visit)(ElemType));
void PrintList(LinkList L);

// 链表创建函数
bool CreateListHead(LinkList *L, int n);
bool CreateListTail(LinkList *L, int n);
bool CreateListFromArray(LinkList *L, ElemType arr[], int n);

// ============= 双链表相关定义 =============

/**
 *  双链表节点定义
 * 
 *   双链表存储结构图示：
 * 
 * 头节点                    第1个节点                   第2个节点
 * ┌────┬────┬────┐         ┌────┬────┬────┐         ┌────┬────┬────┐
 * │NULL│    │ ●──┼───────→ │ ●  │ a₁ │ ●──┼───────→ │ ●  │ a₂ │NULL│
 * └────┴────┴────┘         └─│──┴────┴────┘         └─│──┴────┴────┘
 *              ↓               ↑                        ↑
 *              └───────────────┘                        │
 *                                     ←─────────────────┘
 * 
 *  核心特点：
 * - 每个节点有两个指针域：prior、next
 * - 支持双向遍历
 * - 删除操作更方便（无需找前驱）
 * - 空间开销更大
 * 
 *  优势：
 * - 双向访问灵活性高
 * - 删除节点无需遍历找前驱
 * - 插入操作对称性好
 */
typedef struct DNode {
    ElemType data;          // 数据域
    struct DNode *prior;    // 前驱指针（指向前一个节点）
    struct DNode *next;     // 后继指针（指向下一个节点）
} DNode, *DLinkList;

// 双链表基本操作函数声明
DLinkList InitDList();
bool DListInsert(DLinkList L, int i, ElemType e);
bool DListDelete(DLinkList L, int i, ElemType *e);
void PrintDList(DLinkList L);

// ============= 循环链表相关定义 =============

/**
 *  循环单链表节点定义 (复用LNode)
 * 
 *   循环单链表存储结构图示：
 * 
 * 头节点        第1个节点       第2个节点       最后一个节点
 * ┌────┬────┐ ┌────┬────┐   ┌────┬────┐   ┌────┬────┐
 * │    │ ●──┼→│ a₁ │ ●──┼─→ │ a₂ │ ●──┼─→ │ aₙ │ ●──┼─┐
 * └────┴────┘ └────┴────┘   └────┴────┘   └────┴────┘ │
 *      ↑                                                │
 *      └────────────────────────────────────────────────┘
 * 
 *  核心特点：
 * - 最后一个节点的next指向头节点
 * - 形成环形结构，没有NULL指针
 * - 从任意节点出发都能访问所有节点
 * - 适合轮询、循环处理的场景
 */
typedef LNode *CircularList;

// 循环单链表基本操作函数声明
CircularList InitCircularList();
void DestroyCircularList(CircularList L);
void ClearCircularList(CircularList L);
bool CircularListEmpty(CircularList L);
int CircularListLength(CircularList L);
bool CircularGetElem(CircularList L, int i, ElemType *e);
bool CircularListInsert(CircularList L, int i, ElemType e);
bool CircularListDelete(CircularList L, int i, ElemType *e);
bool CircularListTraverse(CircularList L, bool (*visit)(ElemType));
void PrintCircularList(CircularList L);

/**
 *  循环双链表节点定义 (复用DNode)
 * 
 *   循环双链表存储结构图示：
 * 
 *      ┌─────────────────────────────────────────────┐
 *      ↓                                             │
 * ┌────┬────┬────┐   ┌────┬────┬────┐   ┌────┬────┬────┐
 * │ ●  │    │ ●──┼→  │ ●  │ a₁ │ ●──┼→  │ ●  │ aₙ │ ●──┼
 * └─│──┴────┴────┘   └─│──┴────┴────┘   └─│──┴────┴────┘
 *   │                   ↑                   ↑             ↑
 *   └───────────────────┼───────────────────┼─────────────┘
 *                       └───────────────────┘
 * 
 *  核心特点：
 * - 结合了双链表和循环链表的优点
 * - 头节点的prior指向最后一个节点
 * - 最后一个节点的next指向头节点
 * - 双向循环，访问灵活性最高
 */
typedef DNode *CircularDList;

// 循环双链表基本操作函数声明
CircularDList InitCircularDList();
void DestroyCircularDList(CircularDList L);
bool CircularDListInsert(CircularDList L, int i, ElemType e);
bool CircularDListDelete(CircularDList L, int i, ElemType *e);
void PrintCircularDList(CircularDList L);

// 循环链表高级操作
bool MergeCircularList(CircularList La, CircularList Lb, CircularList *Lc);
bool CircularListReverse(CircularList L);

// ============= 高级操作函数声明（ 重点）=============

/*  链表经典算法
 * 这些是 的高频考点，属于链表操作的核心算法
 */

// 链表反转算法
bool ReverseList(LinkList L);
bool ReverseListRecursive(LinkList L);
// 查找中间节点（快慢指针法）
LNode* FindMiddleNode(LinkList L);
// 环检测（Floyd判圈算法）
bool HasCycle(LinkList L);

/* 🔧 删除和修改操作
 * 掌握各种删除策略和边界条件处理
 */

// 删除重复元素（有序表）
int RemoveDuplicates(LinkList L);
// 删除所有值为x的节点
int RemoveAllX(LinkList L, ElemType x);
// 分割链表
bool SplitList(LinkList L, int k, LinkList *L2);

/*   有序表和集合操作
 * 利用链表特性实现高效的集合运算
 */

// 有序链表插入（保持有序）
bool InsertInOrder(LinkList L, ElemType e);
// 合并两个有序链表
bool MergeList(LinkList La, LinkList Lb, LinkList *Lc);

/* 🔗 集合运算
 * 基于链表的集合交、并运算
 */

// 求两个集合的交集
bool IntersectionList(LinkList La, LinkList Lb, LinkList *Lc);
// 求两个集合的并集
bool UnionList(LinkList La, LinkList Lb, LinkList *Lc);

// ============= 辅助函数声明 =============

// 比较和访问函数
bool Equal(ElemType a, ElemType b);
int Compare(ElemType a, ElemType b);
bool Visit_Print(ElemType e);

// 性能测试函数
void PerformanceTest_LinkList(int size);
void PerformanceCompare_ListVsArray(int size);
void PerformanceTest_Advanced(int size);

#endif // LINKLIST_H 