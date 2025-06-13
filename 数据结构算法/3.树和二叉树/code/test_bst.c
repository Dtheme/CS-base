/* test_bst.c - 二叉搜索树测试程序
 * create by: zw.duan
 */

#include "bst.h"
#include <time.h>
#include <assert.h>

// ============= 测试辅助函数 =============

void PrintTestHeader(const char *testName) {
    printf("\n==================================================\n");
    printf("测试项目: %s\n", testName);
    printf("==================================================\n");
}

void PrintTestResult(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

// ============= BST基本操作测试 =============

void TestBSTBasicOperations() {
    PrintTestHeader("BST基本操作测试");
    
    BSTree T;
    InitBST(&T);
    
    // 测试空树
    assert(BSTEmpty(T) == true);
    assert(BSTSearch(T, 5) == NULL);
    PrintTestResult("空树测试", true);
    
    // 插入测试
    assert(BSTInsert(&T, 5) == true);
    assert(BSTInsert(&T, 3) == true);
    assert(BSTInsert(&T, 7) == true);
    assert(BSTInsert(&T, 2) == true);
    assert(BSTInsert(&T, 4) == true);
    assert(BSTInsert(&T, 6) == true);
    assert(BSTInsert(&T, 8) == true);
    
    // 重复插入测试
    assert(BSTInsert(&T, 5) == false);
    
    PrintTestResult("插入操作测试", true);
    
    // 搜索测试
    assert(BSTSearch(T, 5) != NULL);
    assert(BSTSearch(T, 3) != NULL);
    assert(BSTSearch(T, 7) != NULL);
    assert(BSTSearch(T, 9) == NULL);
    
    assert(BSTSearchNonRecursive(T, 4) != NULL);
    assert(BSTSearchNonRecursive(T, 10) == NULL);
    
    PrintTestResult("搜索操作测试", true);
    
    // 最值测试
    BSTNode *minNode = FindMin(T);
    BSTNode *maxNode = FindMax(T);
    assert(minNode != NULL && minNode->data == 2);
    assert(maxNode != NULL && maxNode->data == 8);
    
    PrintTestResult("最值查找测试", true);
    
    printf("BST结构:\n");
    PrintBSTStructure(T, 0);
    
    DestroyBST(&T);
    assert(T == NULL);
    PrintTestResult("销毁测试", true);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                二叉搜索树测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // 执行基本测试
    TestBSTBasicOperations();
    
    printf("\n============================================================\n");
    printf("BST基本测试完成！\n");
    printf("============================================================\n");
    
    return 0;
}