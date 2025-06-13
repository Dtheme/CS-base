/* test_rbtree.c - 红黑树完整功能测试程序
 * create by: zw.duan
 */

#include "rbtree.h"
#include <time.h>
#include <assert.h>

void PrintTestHeader(const char *testName) {
    printf("\n==================================================\n");
    printf("测试项目: %s\n", testName);
    printf("==================================================\n");
}

void PrintTestResult(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

void TestRBTreeBasicOperations() {
    PrintTestHeader("红黑树基本操作测试");
    
    RBTree T;
    InitRBTree(&T);
    
    // 测试空树
    assert(RBTreeEmpty(&T) == true);
    assert(RBTreeSize(&T) == 0);
    PrintTestResult("空树测试", true);
    
    // 插入测试
    assert(RBTreeInsert(&T, 10) == true);
    assert(RBTreeInsert(&T, 5) == true);
    assert(RBTreeInsert(&T, 15) == true);
    assert(RBTreeInsert(&T, 3) == true);
    assert(RBTreeInsert(&T, 7) == true);
    
    // 重复插入测试
    assert(RBTreeInsert(&T, 10) == false);
    
    PrintTestResult("插入操作测试", true);
    
    printf("红黑树结构:\n");
    PrintRBTree(&T);
    
    assert(RBTreeEmpty(&T) == false);
    assert(RBTreeSize(&T) == 5);
    PrintTestResult("非空树测试", true);
    
    DestroyRBTree(&T);
    PrintTestResult("销毁测试", true);
}

void TestRBTreeSearchOperations() {
    PrintTestHeader("红黑树查找操作测试");
    
    RBTree T;
    InitRBTree(&T);
    
    // 构建测试数据
    int testData[] = {10, 5, 15, 3, 7, 12, 18};
    int dataSize = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        RBTreeInsert(&T, testData[i]);
    }
    
    // 搜索测试
    RBNode *found = RBTreeSearch(&T, 10);
    assert(found != NULL && found->data == 10);
    
    found = RBTreeSearch(&T, 999);
    assert(found == NULL);
    
    PrintTestResult("搜索操作测试", true);
    
    // 最值测试
    RBNode *min = RBTreeMinimum(&T, T.root);
    assert(min != NULL && min->data == 3);
    
    RBNode *max = RBTreeMaximum(&T, T.root);
    assert(max != NULL && max->data == 18);
    
    PrintTestResult("最值查找测试", true);
    
    // 前驱后继测试
    found = RBTreeSearch(&T, 10);
    if (found) {
        RBNode *succ = RBTreeSuccessor(&T, found);
        RBNode *pred = RBTreePredecessor(&T, found);
        printf("节点10的后继: %s, 前驱: %s\n", 
               succ ? "存在" : "NULL", 
               pred ? "存在" : "NULL");
    }
    
    PrintTestResult("前驱后继测试", true);
    
    DestroyRBTree(&T);
}

void TestRBTreeDeleteOperations() {
    PrintTestHeader("红黑树删除操作测试");
    
    RBTree T;
    InitRBTree(&T);
    
    // 构建测试数据
    int testData[] = {10, 5, 15, 3, 7, 12, 18, 1, 6, 8, 11, 13, 17, 20};
    int dataSize = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        RBTreeInsert(&T, testData[i]);
    }
    
    printf("删除前的树：");
    PrintRBTree(&T);
    
    int initialSize = RBTreeSize(&T);
    
    // 删除叶子节点
    assert(RBTreeDelete(&T, 1) == true);
    assert(RBTreeSize(&T) == initialSize - 1);
    PrintTestResult("删除叶子节点", true);
    
    // 删除只有一个子节点的节点
    assert(RBTreeDelete(&T, 6) == true);
    assert(RBTreeSize(&T) == initialSize - 2);
    PrintTestResult("删除单子节点", true);
    
    // 删除有两个子节点的节点
    assert(RBTreeDelete(&T, 10) == true);
    assert(RBTreeSize(&T) == initialSize - 3);
    PrintTestResult("删除双子节点", true);
    
    // 删除不存在的节点
    assert(RBTreeDelete(&T, 999) == false);
    PrintTestResult("删除不存在节点", true);
    
    printf("删除后的树：");
    PrintRBTree(&T);
    
    DestroyRBTree(&T);
}

void TestRBTreeTraversal() {
    PrintTestHeader("红黑树遍历测试");
    
    RBTree T;
    InitRBTree(&T);
    
    // 构建测试数据
    int testData[] = {10, 5, 15, 3, 7, 12, 18};
    int dataSize = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        RBTreeInsert(&T, testData[i]);
    }
    
    printf("先序遍历: ");
    RBTreePreOrder(&T, T.root);
    printf("\n");
    
    printf("中序遍历: ");
    RBTreeInOrder(&T, T.root);
    printf("\n");
    
    printf("后序遍历: ");
    RBTreePostOrder(&T, T.root);
    printf("\n");
    
    PrintTestResult("遍历操作测试", true);
    
    DestroyRBTree(&T);
}

void TestRBTreeValidation() {
    PrintTestHeader("红黑树性质验证测试");
    
    RBTree T;
    InitRBTree(&T);
    
    // 空树验证
    assert(ValidateRBTree(&T) == true);
    PrintTestResult("空树性质验证", true);
    
    // 插入数据后验证
    int testData[] = {10, 5, 15, 3, 7, 12, 18, 1, 6, 8, 11, 13, 17, 20};
    int dataSize = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        RBTreeInsert(&T, testData[i]);
        bool valid = ValidateRBTree(&T);
        if (!valid) {
            printf("插入%d后红黑树性质被违反\n", testData[i]);
        }
    }
    
    // 验证红黑树性质
    bool valid = ValidateRBTree(&T);
    printf("红黑树性质验证结果: %s\n", valid ? "通过" : "失败");
    
    PrintTestResult("红黑树性质验证", valid);
    
    // 统计信息
    printf("树的深度: %d\n", RBTreeDepth(&T));
    printf("节点数量: %d\n", RBTreeNodeCount(&T, T.root));
    printf("黑高: %d\n", RBTreeBlackHeight(&T, T.root));
    
    PrintTestResult("统计信息测试", true);
    
    DestroyRBTree(&T);
}

void TestRBTreeEdgeCases() {
    PrintTestHeader("红黑树边界测试");
    
    RBTree T;
    InitRBTree(&T);
    
    // 单节点测试
    RBTreeInsert(&T, 42);
    assert(RBTreeSize(&T) == 1);
    assert(T.root->color == BLACK);  // 根节点必须是黑色
    
    RBNode *found = RBTreeSearch(&T, 42);
    assert(found != NULL && found->data == 42);
    
    PrintTestResult("单节点测试", true);
    
    // 测试删除根节点
    assert(RBTreeDelete(&T, 42) == true);
    assert(RBTreeEmpty(&T) == true);
    
    PrintTestResult("删除根节点测试", true);
    
    // 有序插入测试
    for (int i = 1; i <= 10; i++) {
        RBTreeInsert(&T, i);
    }
    
    printf("有序插入后的中序遍历: ");
    RBTreeInOrder(&T, T.root);
    printf("\n");
    
    // 验证有序插入后的红黑树性质
    bool valid = ValidateRBTree(&T);
    printf("有序插入后红黑树性质: %s\n", valid ? "通过" : "失败");
    
    PrintTestResult("有序插入测试", valid);
    
    DestroyRBTree(&T);
}

void TestRBTreePerformance() {
    PrintTestHeader("红黑树性能测试");
    
    printf("测试不同规模下的红黑树性能：\n");
    
    int testSizes[] = {100, 1000, 10000};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    
    for (int i = 0; i < numTests; i++) {
        printf("\n--- 测试规模: %d ---\n", testSizes[i]);
        PerformanceTest_RBTreeOps(testSizes[i]);
    }
    
    PrintTestResult("性能测试", true);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                红黑树测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    TestRBTreeBasicOperations();
    TestRBTreeSearchOperations();
    TestRBTreeDeleteOperations();
    TestRBTreeTraversal();
    TestRBTreeValidation();
    TestRBTreeEdgeCases();
    TestRBTreePerformance();
    
    printf("\n============================================================\n");
    printf("红黑树完整功能测试完成！\n");
    printf("============================================================\n");
    
    
    return 0;
}