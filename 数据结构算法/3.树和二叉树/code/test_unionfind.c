/* test_unionfind.c - 并查集完整功能测试程序
 * create by: zw.duan
 *   第3章 树和二叉树
 */

#include "unionfind.h"
#include <time.h>
#include <assert.h>

void PrintTestHeader(const char *testName) {
    printf("------ %s 测试用例 ------\n", testName);
}

void PrintTestResult(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

void TestUnionFindBasicOperations() {
    PrintTestHeader("并查集基本操作");
    
    UnionFind uf;
    
    // 测试初始化
    assert(InitUnionFind(&uf, 10) == true);
    assert(GetCapacity(&uf) == 10);
    assert(GetComponentCount(&uf) == 10);
    PrintTestResult("初始化测试", true);
    
    // 测试初始状态
    for (int i = 0; i < 10; i++) {
        assert(Find(&uf, i) == i);
        assert(GetComponentSize(&uf, i) == 1);
        assert(IsRoot(&uf, i) == true);
    }
    PrintTestResult("初始状态测试", true);
    
    // 测试连通性
    assert(Connected(&uf, 0, 1) == false);
    assert(Connected(&uf, 0, 0) == true);
    PrintTestResult("初始连通性测试", true);
    
    PrintUnionFind(&uf);
    
    DestroyUnionFind(&uf);
    PrintTestResult("销毁测试", true);
}

void TestUnionFindUnionOperations() {
    PrintTestHeader("并查集合并操作");
    
    UnionFind uf;
    InitUnionFind(&uf, 10);
    
    // 测试Union操作
    assert(Union(&uf, 0, 1) == true);
    assert(GetComponentCount(&uf) == 9);
    assert(Connected(&uf, 0, 1) == true);
    PrintTestResult("基本Union操作", true);
    
    // 测试重复Union
    assert(Union(&uf, 0, 1) == false);
    assert(GetComponentCount(&uf) == 9);
    PrintTestResult("重复Union测试", true);
    
    // 测试链式Union
    assert(Union(&uf, 1, 2) == true);
    assert(Union(&uf, 2, 3) == true);
    assert(Connected(&uf, 0, 3) == true);
    assert(GetComponentCount(&uf) == 7);
    PrintTestResult("链式Union测试", true);
    
    // 测试集合大小
    assert(GetComponentSize(&uf, 0) == 4);
    assert(GetComponentSize(&uf, 1) == 4);
    assert(GetComponentSize(&uf, 2) == 4);
    assert(GetComponentSize(&uf, 3) == 4);
    PrintTestResult("集合大小测试", true);
    
    printf("Union操作后的状态:\n");
    PrintUnionFind(&uf);
    PrintUnionFindStructure(&uf);
    
    DestroyUnionFind(&uf);
}

void TestUnionFindPathOperations() {
    PrintTestHeader("并查集路径操作");
    
    UnionFind uf;
    InitUnionFind(&uf, 8);
    
    // 构建链式结构：0-1-2-3-4-5-6-7
    for (int i = 0; i < 7; i++) {
        Union(&uf, i, i + 1);
    }
    
    printf("构建链式结构后:\n");
    PrintUnionFind(&uf);
    
    // 测试路径长度（在路径压缩前）
    printf("路径压缩前的路径长度:\n");
    for (int i = 0; i < 8; i++) {
        printf("节点 %d 到根的路径长度: %d\n", i, GetPathLength(&uf, i));
    }
    
    // 执行一次Find操作触发路径压缩
    Find(&uf, 7);
    
    printf("\n路径压缩后:\n");
    PrintUnionFind(&uf);
    
    // 测试路径获取
    int path[10];
    int length;
    GetPath(&uf, 7, path, &length);
    printf("节点7到根的路径: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");
    
    PrintTestResult("路径操作测试", true);
    
    DestroyUnionFind(&uf);
}

void TestUnionFindComponentInfo() {
    PrintTestHeader("并查集连通分量信息");
    
    UnionFind uf;
    InitUnionFind(&uf, 12);
    
    // 构建几个连通分量
    // 分量1: {0, 1, 2}
    Union(&uf, 0, 1);
    Union(&uf, 1, 2);
    
    // 分量2: {3, 4, 5, 6}
    Union(&uf, 3, 4);
    Union(&uf, 4, 5);
    Union(&uf, 5, 6);
    
    // 分量3: {7, 8}
    Union(&uf, 7, 8);
    
    // 单独节点: {9}, {10}, {11}
    
    printf("构建多个连通分量后:\n");
    PrintUnionFindStructure(&uf);
    PrintComponentInfo(&uf);
    
    // 测试统计信息
    assert(GetComponentCount(&uf) == 6);
    assert(GetLargestComponentSize(&uf) == 4);
    assert(GetSmallestComponentSize(&uf) == 1);
    assert(IsSingleComponent(&uf) == false);
    
    PrintTestResult("连通分量信息测试", true);
    
    // 测试获取所有根节点
    int roots[15];
    int rootCount;
    GetAllRoots(&uf, roots, &rootCount);
    
    printf("\n所有根节点 (%d个): ", rootCount);
    for (int i = 0; i < rootCount; i++) {
        printf("%d ", roots[i]);
    }
    printf("\n");
    
    PrintTestResult("根节点获取测试", true);
    
    DestroyUnionFind(&uf);
}

void TestUnionFindApplications() {
    PrintTestHeader("并查集应用");
    
    // 测试图连通性判断
    // 图1: 边 {0,1}, {1,2}, {3,4}，5个顶点
    // 连通分量: {0,1,2} 和 {3,4}，总共2个分量  
    int edges1[][2] = {{0, 1}, {1, 2}, {3, 4}};
    int edgeCount1 = 3;
    int vertexCount1 = 5;
    
    int *edgePtr1[3];
    for (int i = 0; i < 3; i++) {
        edgePtr1[i] = edges1[i];
    }
    
    bool connected1 = IsGraphConnected(edgePtr1, edgeCount1, vertexCount1);
    int components1 = CountConnectedComponents(edgePtr1, edgeCount1, vertexCount1);
    
    printf("图1连通性: %s, 连通分量数: %d\n", 
           connected1 ? "连通" : "不连通", components1);
    assert(connected1 == false);
    assert(components1 == 2);
    
    PrintTestResult("图连通性测试1", true);
    
    // 测试有3个连通分量的图
    // 图1.5: 边 {0,1}, {2,3}，4个顶点
    // 连通分量: {0,1}, {2,3}，总共2个分量（不含孤立点）
    // 如果有6个顶点：连通分量: {0,1}, {2,3}, {4}, {5}，总共4个分量
    int edges1_5[][2] = {{0, 1}, {2, 3}};
    int edgeCount1_5 = 2;
    int vertexCount1_5 = 6;  // 包含孤立顶点4和5
    
    int *edgePtr1_5[2];
    for (int i = 0; i < 2; i++) {
        edgePtr1_5[i] = edges1_5[i];
    }
    
    bool connected1_5 = IsGraphConnected(edgePtr1_5, edgeCount1_5, vertexCount1_5);
    int components1_5 = CountConnectedComponents(edgePtr1_5, edgeCount1_5, vertexCount1_5);
    
    printf("图1.5连通性: %s, 连通分量数: %d\n", 
           connected1_5 ? "连通" : "不连通", components1_5);
    assert(connected1_5 == false);
    assert(components1_5 == 4);  // {0,1}, {2,3}, {4}, {5}
    
    PrintTestResult("多连通分量测试", true);
    
    // 测试连通图
    int edges2[][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    int edgeCount2 = 4;
    int vertexCount2 = 5;
    
    int *edgePtr2[4];
    for (int i = 0; i < 4; i++) {
        edgePtr2[i] = edges2[i];
    }
    
    bool connected2 = IsGraphConnected(edgePtr2, edgeCount2, vertexCount2);
    int components2 = CountConnectedComponents(edgePtr2, edgeCount2, vertexCount2);
    
    printf("图2连通性: %s, 连通分量数: %d\n", 
           connected2 ? "连通" : "不连通", components2);
    assert(connected2 == true);
    assert(components2 == 1);
    
    PrintTestResult("图连通性测试2", true);
    
    // 测试朋友圈问题
    int friendMatrix[4][4] = {
        {1, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 1}
    };
    
    int *matrixPtr[4];
    for (int i = 0; i < 4; i++) {
        matrixPtr[i] = friendMatrix[i];
    }
    
    int circles = FindCircleNum(matrixPtr, 4);
    printf("朋友圈数量: %d\n", circles);
    assert(circles == 2);
    
    PrintTestResult("朋友圈问题测试", true);
}

void TestUnionFindValidation() {
    PrintTestHeader("并查集验证");
    
    UnionFind uf;
    InitUnionFind(&uf, 10);
    
    // 测试空并查集验证
    assert(ValidateUnionFind(&uf) == true);
    PrintTestResult("初始状态验证", true);
    
    // 进行一些操作后验证
    Union(&uf, 0, 1);
    Union(&uf, 2, 3);
    Union(&uf, 1, 3);
    
    assert(ValidateUnionFind(&uf) == true);
    PrintTestResult("操作后验证", true);
    
    // 测试边界情况
    assert(Find(&uf, -1) == -1);
    assert(Find(&uf, 100) == -1);
    assert(Union(&uf, -1, 0) == false);
    assert(Union(&uf, 0, 100) == false);
    
    PrintTestResult("边界情况测试", true);
    
    DestroyUnionFind(&uf);
}

void TestUnionFindPerformance() {
    PrintTestHeader("并查集性能");
    
    printf("测试不同规模下的并查集性能：\n");
    
    int testSizes[] = {100, 1000, 10000};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    
    for (int i = 0; i < numTests; i++) {
        PerformanceTest_UnionFindOps(testSizes[i]);
    }
    
    // 路径压缩性能对比
    printf("\n路径压缩优化效果对比：\n");
    for (int i = 0; i < 2; i++) {
        PerformanceTest_FindWithVsWithoutCompression(testSizes[i]);
    }
    
    PrintTestResult("性能测试", true);
}

void TestUnionFindEdgeCases() {
    PrintTestHeader("并查集边界");
    
    // 测试单节点
    UnionFind uf1;
    assert(InitUnionFind(&uf1, 1) == true);
    assert(GetComponentCount(&uf1) == 1);
    assert(Find(&uf1, 0) == 0);
    assert(Union(&uf1, 0, 0) == false);
    assert(Connected(&uf1, 0, 0) == true);
    DestroyUnionFind(&uf1);
    
    PrintTestResult("单节点测试", true);
    
    // 测试大规模合并
    UnionFind uf2;
    InitUnionFind(&uf2, 1000);
    
    // 将所有节点连通
    for (int i = 1; i < 1000; i++) {
        Union(&uf2, 0, i);
    }
    
    assert(GetComponentCount(&uf2) == 1);
    assert(IsSingleComponent(&uf2) == true);
    assert(GetLargestComponentSize(&uf2) == 1000);
    
    // 验证所有节点都连通
    for (int i = 0; i < 1000; i++) {
        assert(Connected(&uf2, 0, i) == true);
    }
    
    DestroyUnionFind(&uf2);
    
    PrintTestResult("大规模合并测试", true);
    
    // 测试清空操作
    UnionFind uf3;
    InitUnionFind(&uf3, 5);
    
    Union(&uf3, 0, 1);
    Union(&uf3, 2, 3);
    assert(GetComponentCount(&uf3) == 3);
    
    ClearUnionFind(&uf3);
    assert(GetComponentCount(&uf3) == 5);
    for (int i = 0; i < 5; i++) {
        assert(Find(&uf3, i) == i);
    }
    
    DestroyUnionFind(&uf3);
    
    PrintTestResult("清空操作测试", true);
}

void TestUnionFindStrategies() {
    PrintTestHeader("并查集合并策略对比");
    
    printf("按秩合并 vs 按大小合并性能对比：\n");
    
    clock_t start, end;
    
    // 测试按秩合并
    UnionFind uf1;
    InitUnionFind(&uf1, 10000);
    
    start = clock();
    srand(42);  // 固定随机种子确保可重复
    for (int i = 0; i < 5000; i++) {
        int x = rand() % 10000;
        int y = rand() % 10000;
        UnionByRank(&uf1, x, y);
    }
    end = clock();
    
    printf("按秩合并: %.2f毫秒, 最大深度: %d, 连通分量: %d\n",
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000,
           GetMaxDepth(&uf1), GetComponentCount(&uf1));
    
    // 测试按大小合并
    UnionFind uf2;
    InitUnionFind(&uf2, 10000);
    
    start = clock();
    srand(42);  // 相同的随机种子
    for (int i = 0; i < 5000; i++) {
        int x = rand() % 10000;
        int y = rand() % 10000;
        UnionBySize(&uf2, x, y);
    }
    end = clock();
    
    printf("按大小合并: %.2f毫秒, 最大深度: %d, 连通分量: %d\n",
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000,
           GetMaxDepth(&uf2), GetComponentCount(&uf2));
    
    DestroyUnionFind(&uf1);
    DestroyUnionFind(&uf2);
    
    PrintTestResult("合并策略对比", true);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                并查集测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    TestUnionFindBasicOperations();
    TestUnionFindUnionOperations();
    TestUnionFindPathOperations();
    TestUnionFindComponentInfo();
    TestUnionFindApplications();
    TestUnionFindValidation();
    TestUnionFindEdgeCases();
    TestUnionFindStrategies();
    TestUnionFindPerformance();
    
    printf("\n============================================================\n");
    printf("并查集完整功能测试完成！\n");
    printf("============================================================\n");
    
    
    return 0;
} 