/* rbtree.h - 红黑树
 * create by: zw.duan
 * 
 * 红黑树性质：
 * 1. 每个节点要么是红色，要么是黑色
 * 2. 根节点是黑色的
 * 3. 每个叶子节点（NIL）是黑色的
 * 4. 如果一个节点是红色的，则它的两个子节点都是黑色的
 * 5. 对于每个节点，从该节点到其所有后代叶子节点的简单路径上，均包含相同数目的黑色节点
 */

#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= 红黑树相关定义 =============

/**
 * 红黑树颜色枚举
 */
typedef enum {
    RED = 0,
    BLACK = 1
} RBColor;

/**
 * 红黑树节点定义
 */
typedef int RBElemType;
typedef struct RBNode {
    RBElemType data;        // 数据域
    RBColor color;          // 颜色：RED或BLACK
    struct RBNode *parent;  // 父节点指针
    struct RBNode *left;    // 左节点指针
    struct RBNode *right;   // 右节点指针
} RBNode;

/**
 * 红黑树结构定义
 */
typedef struct {
    RBNode *root;           // 根节点
    RBNode *nil;            // 哨兵节点，代表所有的叶子节点
    int size;               // 树中节点数量
} RBTree;

// ============= 红黑树基本操作 =============

// 初始化和销毁
void InitRBTree(RBTree *T);
void DestroyRBTree(RBTree *T);
bool RBTreeEmpty(RBTree *T);
int RBTreeSize(RBTree *T);

// 创建和销毁节点
RBNode* CreateRBNode(RBTree *T, RBElemType data);
void DestroyRBNode(RBTree *T, RBNode *node);

// 查找操作
RBNode* RBTreeSearch(RBTree *T, RBElemType key);
RBNode* RBTreeMinimum(RBTree *T, RBNode *x);
RBNode* RBTreeMaximum(RBTree *T, RBNode *x);
RBNode* RBTreeSuccessor(RBTree *T, RBNode *x);
RBNode* RBTreePredecessor(RBTree *T, RBNode *x);

// 插入和删除操作
bool RBTreeInsert(RBTree *T, RBElemType key);
bool RBTreeDelete(RBTree *T, RBElemType key);

// ============= 红黑树内部操作（维护红黑树性质）=============

// 旋转操作
void LeftRotate(RBTree *T, RBNode *x);
void RightRotate(RBTree *T, RBNode *x);

// 插入修复
void RBInsertFixup(RBTree *T, RBNode *z);

// 删除修复
void RBDeleteFixup(RBTree *T, RBNode *x);

// 节点移植操作
void RBTransplant(RBTree *T, RBNode *u, RBNode *v);

// ============= 红黑树遍历操作 =============

void RBTreeInOrder(RBTree *T, RBNode *x);
void RBTreePreOrder(RBTree *T, RBNode *x);
void RBTreePostOrder(RBTree *T, RBNode *x);
void RBTreeLevelOrder(RBTree *T);

// 带访问函数的遍历
void RBTreeInOrderTraverse(RBTree *T, RBNode *x, void (*visit)(RBElemType));
void RBTreePreOrderTraverse(RBTree *T, RBNode *x, void (*visit)(RBElemType));

// ============= 红黑树验证和调试 =============

// 红黑树性质验证
bool ValidateRBTree(RBTree *T);
bool CheckRBProperty1(RBTree *T, RBNode *node);  // 颜色有效性
bool CheckRBProperty2(RBTree *T);                // 根节点是黑色
bool CheckRBProperty4(RBTree *T, RBNode *node);  // 红色节点的子节点都是黑色
int CheckRBProperty5(RBTree *T, RBNode *node);   // 黑高相同

// 统计信息
int RBTreeBlackHeight(RBTree *T, RBNode *node);
int RBTreeDepth(RBTree *T);
int RBTreeNodeCount(RBTree *T, RBNode *node);
int RBTreeRedCount(RBTree *T, RBNode *node);
int RBTreeBlackCount(RBTree *T, RBNode *node);

// ============= 红黑树高级操作（ 算法设计）=============

// 范围查询
void RBTreeRangeQuery(RBTree *T, RBElemType low, RBElemType high, RBElemType *result, int *count);
int RBTreeCountRange(RBTree *T, RBElemType low, RBElemType high);

// 第k小元素
RBNode* RBTreeKthSmallest(RBTree *T, int k);
int RBTreeRank(RBTree *T, RBElemType key);

// 树的比较和复制
bool RBTreeEqual(RBTree *T1, RBTree *T2);
RBTree* RBTreeCopy(RBTree *T);

// 序列化和反序列化
void RBTreeSerialize(RBTree *T, char *buffer, int bufSize);
RBTree* RBTreeDeserialize(char *buffer);

// 合并和分割
RBTree* RBTreeMerge(RBTree *T1, RBTree *T2);
void RBTreeSplit(RBTree *T, RBElemType key, RBTree **T1, RBTree **T2);

// ============= 红黑树与其他数据结构的转换 =============

// 转换为数组
void RBTreeToArray(RBTree *T, RBElemType *arr, int *size);
RBTree* ArrayToRBTree(RBElemType *arr, int size);

// 转换为链表
typedef struct ListNode {
    RBElemType data;
    struct ListNode *next;
} ListNode;

ListNode* RBTreeToSortedList(RBTree *T);

// ============= 实用工具函数 =============

void PrintRBTree(RBTree *T);
void PrintRBTreeStructure(RBTree *T, RBNode *node, int space);
void PrintRBNode(RBNode *node, RBNode *nil);
const char* GetColorString(RBColor color);

// 错误处理
typedef enum {
    RB_SUCCESS = 0,
    RB_ERROR_NULL_POINTER,
    RB_ERROR_DUPLICATE_KEY,
    RB_ERROR_KEY_NOT_FOUND,
    RB_ERROR_INVALID_TREE,
    RB_ERROR_MEMORY_ALLOCATION
} RBError;

const char* RBGetErrorString(RBError error);

// ============= 性能测试和基准 =============

void PerformanceTest_RBTreeInsert(int size);
void PerformanceTest_RBTreeSearch(int size);
void PerformanceTest_RBTreeDelete(int size);
void PerformanceTest_RBTreeOps(int size);
void PerformanceCompare_RBTreeVsBST(int size);
void PerformanceCompare_RBTreeVsAVL(int size);

// 随机测试
void RandomTest_RBTreeOperations(int operations);
void StressTest_RBTree(int maxSize, int iterations);

// ============= 红黑树应用示例 =============

// 实现有序集合
typedef RBTree OrderedSet;
bool OrderedSet_Insert(OrderedSet *set, RBElemType elem);
bool OrderedSet_Delete(OrderedSet *set, RBElemType elem);
bool OrderedSet_Contains(OrderedSet *set, RBElemType elem);
int OrderedSet_Size(OrderedSet *set);

// 实现有序映射的键部分（简化版本）
typedef struct {
    RBTree keys;
    // 在实际实现中，这里还需要存储对应的值
} OrderedMap;

#endif // RBTREE_H

/* ================================
 * 红黑树实现说明：
 * 
 * 红黑树是一种自平衡的二叉搜索树，通过颜色标记和旋转操作维护平衡。
 * 时间复杂度：查找、插入、删除都是O(log n)
 * 空间复杂度：O(n)
 * 
 * 优点：
 * - 最坏情况下仍能保证O(log n)的性能
 * - 插入和删除操作的平均旋转次数较少
 * - 被广泛应用于标准库中（如C++ STL的map、set）
 * 
 * 编译建议：
 * gcc -std=c99 -Wall -O2 rbtree.c test_rbtree.c -o test_rbtree
 * ================================ */ 