/* bst.h - 二叉搜索树
 * create by: zw.duan
 */

#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ============= 二叉搜索树相关定义 =============

/**
 * 二叉搜索树节点定义
 */
typedef int BSTElemType;
typedef struct BSTNode {
    BSTElemType data;
    struct BSTNode *lchild;
    struct BSTNode *rchild;
} BSTNode, *BSTree;

// ============= 二叉搜索树基本操作 =============

// 初始化和销毁
void InitBST(BSTree *T);
void DestroyBST(BSTree *T);
bool BSTEmpty(BSTree T);

// 查找操作
BSTNode* BSTSearch(BSTree T, BSTElemType key);
BSTNode* BSTSearchNonRecursive(BSTree T, BSTElemType key);
BSTNode* FindMin(BSTree T);
BSTNode* FindMax(BSTree T);

// 插入和删除操作
bool BSTInsert(BSTree *T, BSTElemType key);
bool BSTDelete(BSTree *T, BSTElemType key);
BSTNode* DeleteNode(BSTree T, BSTElemType key);

// 遍历操作
void BSTInOrder(BSTree T);
void BSTPreOrder(BSTree T);
void BSTPostOrder(BSTree T);

// ============= 二叉搜索树高级操作（ 重点）=============

// 树的性质判断
bool IsBST(BSTree T);
bool IsValidBST(BSTree T, BSTElemType min, BSTElemType max);

// 查找相关操作
BSTNode* FindPredecessor(BSTree T, BSTElemType key);
BSTNode* FindSuccessor(BSTree T, BSTElemType key);
BSTNode* FindKthSmallest(BSTree T, int k);
int GetRank(BSTree T, BSTElemType key);

// 路径和距离
bool FindPath(BSTree T, BSTElemType key, int *path, int *len);
int GetDistance(BSTree T, BSTElemType a, BSTElemType b);
BSTNode* LowestCommonAncestor(BSTree T, BSTElemType p, BSTElemType q);

// 范围查询
void PrintRange(BSTree T, BSTElemType k1, BSTElemType k2);
int CountRange(BSTree T, BSTElemType k1, BSTElemType k2);

// 树的转换
void BSTToSortedArray(BSTree T, BSTElemType *arr, int *index);
BSTree SortedArrayToBST(BSTElemType *arr, int start, int end);
BSTree BalanceBST(BSTree T);

// ============= BST构造和验证 =============

// 从不同输入构造BST
BSTree CreateBSTFromArray(BSTElemType *arr, int size);
BSTree CreateBSTFromPreOrder(BSTElemType *preorder, int size);
BSTree CreateBSTFromPostOrder(BSTElemType *postorder, int size);

// BST验证
bool VerifyPreOrder(BSTElemType *preorder, int size);
bool VerifyPostOrder(BSTElemType *postorder, int size);

// ============= 特殊BST操作（算法设计题）=============

// 修剪BST
BSTree TrimBST(BSTree T, BSTElemType low, BSTElemType high);

// BST序列化和反序列化
void SerializeBST(BSTree T, char *str);
BSTree DeserializeBST(char *str);

// BST的模式匹配
bool IsSubtree(BSTree T, BSTree S);
int CountBSTNodes(BSTree T, BSTElemType min, BSTElemType max);

// 两个BST的合并
BSTree MergeBST(BSTree T1, BSTree T2);
void GetAllElements(BSTree T1, BSTree T2, BSTElemType *result, int *size);

// ============= 辅助函数 =============

void PrintBST(BSTree T);
void PrintBSTStructure(BSTree T, int space);
int BSTDepth(BSTree T);
int BSTNodeCount(BSTree T);
bool ValidateBSTProperty(BSTree T);

// 辅助函数
BSTNode* FindKthSmallestHelper(BSTree T, int k, int *count);

// 性能测试
void PerformanceTest_BSTSearch(int size);
void PerformanceTest_BSTInsert(int size);
void PerformanceCompare_BSTvsArray(int size);

#endif // BST_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 bst.c test_bst.c -o test_bst
 * ================================ */ 