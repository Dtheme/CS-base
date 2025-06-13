/* unionfind.h - 并查集
 * create by: zw.duan
 */

#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= 并查集相关定义 =============

/**
 * 并查集数据结构定义
 */
typedef struct {
    int *parent;        // 父节点数组，parent[i]表示i的父节点
    int *rank;          // 秩数组，用于按秩合并优化
    int *size;          // 集合大小数组，size[i]表示以i为根的集合大小
    int capacity;       // 并查集的容量
    int count;          // 连通分量的数量
} UnionFind;

// ============= 并查集基本操作 =============

// 初始化和销毁
bool InitUnionFind(UnionFind *uf, int n);
void DestroyUnionFind(UnionFind *uf);
void ClearUnionFind(UnionFind *uf);

// 查找操作
int Find(UnionFind *uf, int x);
int FindWithoutCompression(UnionFind *uf, int x);

// 合并操作
bool Union(UnionFind *uf, int x, int y);
bool UnionByRank(UnionFind *uf, int x, int y);
bool UnionBySize(UnionFind *uf, int x, int y);

// 查询操作
bool Connected(UnionFind *uf, int x, int y);
int GetComponentSize(UnionFind *uf, int x);
int GetComponentCount(UnionFind *uf);
int GetCapacity(UnionFind *uf);

// ============= 并查集高级操作（ 重点）=============

// 路径操作
int GetPathLength(UnionFind *uf, int x);
void GetPath(UnionFind *uf, int x, int *path, int *length);
int GetMaxDepth(UnionFind *uf);

// 统计信息
void GetComponentInfo(UnionFind *uf, int **components, int **sizes, int *count);
int GetLargestComponentSize(UnionFind *uf);
int GetSmallestComponentSize(UnionFind *uf);

// 集合操作
bool IsSingleComponent(UnionFind *uf);
void GetAllRoots(UnionFind *uf, int *roots, int *count);
bool IsRoot(UnionFind *uf, int x);

// ============= 并查集应用（算法设计题）=============

// 图的连通性
bool IsGraphConnected(int **edges, int edgeCount, int vertexCount);
int CountConnectedComponents(int **edges, int edgeCount, int vertexCount);

// 朋友圈问题
int FindCircleNum(int **matrix, int n);
bool CanConnect(UnionFind *uf, int x, int y, int maxSize);

// 岛屿问题
int NumIslands(char **grid, int rows, int cols);
int NumIslandsUnionFind(char **grid, int rows, int cols);

// 动态连通性
typedef struct {
    UnionFind *uf;
    int **history;      // 操作历史
    int historySize;
    int historyCapacity;
} DynamicConnectivity;

bool InitDynamicConnectivity(DynamicConnectivity *dc, int n);
void DestroyDynamicConnectivity(DynamicConnectivity *dc);
bool AddConnection(DynamicConnectivity *dc, int x, int y);
bool QueryConnection(DynamicConnectivity *dc, int x, int y);
void PrintConnectionHistory(DynamicConnectivity *dc);

// ============= 并查集变种实现 =============

// 带权并查集
typedef struct {
    int *parent;
    double *weight;     // 到父节点的权重
    int capacity;
    int count;
} WeightedUnionFind;

bool InitWeightedUnionFind(WeightedUnionFind *wuf, int n);
void DestroyWeightedUnionFind(WeightedUnionFind *wuf);
int FindWeighted(WeightedUnionFind *wuf, int x, double *pathWeight);
bool UnionWeighted(WeightedUnionFind *wuf, int x, int y, double weight);
bool QueryWeight(WeightedUnionFind *wuf, int x, int y, double *weight);

// 可撤销并查集
typedef struct {
    int x, y;
    int parentX, parentY;
    int rankX, rankY;
    int oldCount;
} UndoOperation;

typedef struct {
    UnionFind *uf;
    UndoOperation *operations;
    int operationCount;
    int operationCapacity;
} UndoableUnionFind;

bool InitUndoableUnionFind(UndoableUnionFind *uuf, int n);
void DestroyUndoableUnionFind(UndoableUnionFind *uuf);
bool UndoableUnion(UndoableUnionFind *uuf, int x, int y);
bool UndoLastOperation(UndoableUnionFind *uuf);
int GetOperationCount(UndoableUnionFind *uuf);

// ============= 辅助函数 =============

void PrintUnionFind(UnionFind *uf);
void PrintUnionFindStructure(UnionFind *uf);
void PrintComponentInfo(UnionFind *uf);
bool ValidateUnionFind(UnionFind *uf);

// 可视化函数
void VisualizeUnionFind(UnionFind *uf);
void ExportUnionFindToDot(UnionFind *uf, const char *filename);

// 性能测试
void PerformanceTest_UnionFindOps(int size);
void PerformanceTest_FindWithVsWithoutCompression(int size);
void PerformanceTest_UnionStrategies(int size);

// 测试数据生成
void GenerateRandomOperations(int n, int opCount, int **operations, int *opTypes);
void GenerateConnectedGraph(int n, int **edges, int *edgeCount);
void GenerateRandomGraph(int n, double probability, int **edges, int *edgeCount);

#endif // UNIONFIND_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 unionfind.c test_unionfind.c -o test_unionfind
 * 
 *  重点：
 * 1. 路径压缩优化 - Find操作的实现
 * 2. 按秩合并优化 - Union操作的实现
 * 3. 时间复杂度分析 - 接近O(1)的均摊复杂度
 * 4. 应用场景 - 图的连通性、最小生成树算法
 * ================================ */ 