/* bintree.h - 二叉树
 * create by: zw.duan
 */

#ifndef BINTREE_H
#define BINTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// 二叉树的最大容量（用于顺序存储）
#define MAXSIZE 100

// ============= 链式二叉树相关定义 =============

/**
 * 二叉树节点定义（链式存储）
 */
typedef char ElemType;  // 假设存储字符类型
typedef struct BiTNode {
    ElemType data;              // 数据域
    struct BiTNode *lchild;     // 左节点指针
    struct BiTNode *rchild;     // 右节点指针
} BiTNode, *BiTree;

// ============= 顺序二叉树相关定义 =============

/**
 * 二叉树顺序存储结构
 */
typedef struct {
    ElemType data[MAXSIZE];     // 存储数据元素的数组
    bool isEmpty[MAXSIZE];      // 标记数组元素是否为空
    int size;                   // 树中实际元素个数
} SqBiTree;

// ============= 二叉树基本操作函数声明 =============

// 链式二叉树基本操作
void InitBiTree(BiTree *T);
void DestroyBiTree(BiTree *T);
bool BiTreeEmpty(BiTree T);
int BiTreeDepth(BiTree T);
int BiTreeNodeCount(BiTree T);
int BiTreeLeafCount(BiTree T);
BiTNode* CreateBiTreeByPreOrder(char **str);
BiTNode* CreateBiTreeByArray(char *arr, int index, int size);

// 二叉树遍历算法
void PreOrder(BiTree T);
void InOrder(BiTree T);
void PostOrder(BiTree T);
void LevelOrder(BiTree T);

// 非递归遍历算法
void PreOrderNonRecursive(BiTree T);
void InOrderNonRecursive(BiTree T);
void PostOrderNonRecursive(BiTree T);

// 遍历的扩展应用
void PreOrderTraverse(BiTree T, void (*visit)(ElemType));
void InOrderTraverse(BiTree T, void (*visit)(ElemType));
void PostOrderTraverse(BiTree T, void (*visit)(ElemType));

// ============= 二叉树查找操作 =============

BiTNode* FindNode(BiTree T, ElemType key);
BiTNode* FindParent(BiTree T, BiTNode *node);
bool FindPath(BiTree T, ElemType key, char *path);

// ============= 二叉树修改操作 =============

bool InsertAsLeftChild(BiTNode *parent, ElemType e);
bool InsertAsRightChild(BiTNode *parent, ElemType e);
bool DeleteSubTree(BiTree *T, BiTNode *node);

// ============= 顺序二叉树操作 =============

void InitSqBiTree(SqBiTree *T);
bool SqBiTreeInsert(SqBiTree *T, int index, ElemType e);
bool SqBiTreeDelete(SqBiTree *T, int index);
ElemType SqBiTreeGetNode(SqBiTree T, int index);
int SqBiTreeParent(int index);
int SqBiTreeLeftChild(int index);
int SqBiTreeRightChild(int index);
void PrintSqBiTree(SqBiTree T);

// ============= 二叉树算法题（ 重点）=============

// 树的性质和计算
int CalculateDepth(BiTree T);
int CalculateWidth(BiTree T);
bool IsCompleteBiTree(BiTree T);
bool IsFullBiTree(BiTree T);
bool IsBST(BiTree T);

// 特殊路径和值计算
bool HasPathSum(BiTree T, int targetSum);
void PrintAllPaths(BiTree T);
int MaxPathSum(BiTree T);
int DiameterOfBinaryTree(BiTree T);

// 树的变换
BiTree MirrorBiTree(BiTree T);
BiTree CopyBiTree(BiTree T);
bool IsSymmetric(BiTree T);
bool IsSameTree(BiTree T1, BiTree T2);

// 根据遍历序列构造二叉树
BiTree BuildTreePreIn(char *preorder, char *inorder, int len);
BiTree BuildTreePostIn(char *postorder, char *inorder, int len);

// 二叉树的线索化
typedef enum {Link, Thread} PointerTag;
typedef struct ThreadNode {
    ElemType data;
    struct ThreadNode *lchild, *rchild;
    PointerTag ltag, rtag;
} ThreadNode, *ThreadTree;

void InThreading(ThreadTree T, ThreadTree *pre);
void CreateInThread(ThreadTree *T);
ThreadTree FirstNode(ThreadTree T);
ThreadTree NextNode(ThreadTree T);
void InOrderThreadTraverse(ThreadTree T);

// ============= 实用工具函数 =============

void PrintBiTree(BiTree T);
void PrintBiTreeStructure(BiTree T, int space);
bool IsValidBiTreeChar(char c);
void VisitPrint(ElemType e);

// 性能测试函数
void PerformanceTest_BasicOps(int nodeCount);
void PerformanceTest_Traversal(int nodeCount);
void PerformanceCompare_RecursiveVsNonRecursive(int nodeCount);

#endif // BINTREE_H

