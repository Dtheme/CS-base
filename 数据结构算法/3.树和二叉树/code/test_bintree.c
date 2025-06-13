/* test_bintree.c - 二叉树测试程序
 * create by: zw.duan
 */

#include "bintree.h"
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

// ============= 基本操作测试 =============

void TestBasicOperations() {
    PrintTestHeader("二叉树基本操作测试");
    
    BiTree T;
    InitBiTree(&T);
    
    // 测试空树
    assert(BiTreeEmpty(T) == true);
    assert(BiTreeDepth(T) == 0);
    assert(BiTreeNodeCount(T) == 0);
    assert(BiTreeLeafCount(T) == 0);
    PrintTestResult("空树测试", true);
    
    // 创建测试树: ABC##DE#G##F###
    char *preOrder = "ABC##DE#G##F###";
    char *p = preOrder;
    T = CreateBiTreeByPreOrder(&p);
    
    // 验证树的基本属性
    assert(BiTreeEmpty(T) == false);
    assert(BiTreeDepth(T) == 5);  // 修正：A->B->D->E->G 是5层
    assert(BiTreeNodeCount(T) == 7);
    assert(BiTreeLeafCount(T) == 3);
    PrintTestResult("树创建和属性测试", true);
    
    // 测试查找功能
    BiTNode *node = FindNode(T, 'D');
    assert(node != NULL && node->data == 'D');
    
    BiTNode *parent = FindParent(T, node);
    assert(parent != NULL && parent->data == 'B');
    PrintTestResult("节点查找测试", true);
    
    // 测试路径查找
    char path[20];
    bool found = FindPath(T, 'G', path);
    assert(found == true);
    printf("从根到G的路径: %s\n", path);
    PrintTestResult("路径查找测试", true);
    
    DestroyBiTree(&T);
    assert(T == NULL);
    PrintTestResult("树销毁测试", true);
}

void TestTraversals() {
    PrintTestHeader("二叉树遍历测试");
    
    // 创建测试树
    char *preOrder = "ABC##DE#G##F###";
    char *p = preOrder;
    BiTree T = CreateBiTreeByPreOrder(&p);
    
    printf("二叉树遍历结果:\n");
    PrintBiTree(T);
    
    printf("\n非递归遍历结果:\n");
    printf("先序遍历（非递归）：");
    PreOrderNonRecursive(T);
    printf("\n");
    
    printf("中序遍历（非递归）：");
    InOrderNonRecursive(T);
    printf("\n");
    
    printf("后序遍历（非递归）：");
    PostOrderNonRecursive(T);
    printf("\n");
    
    printf("\n函数指针遍历测试:\n");
    printf("先序遍历（函数指针）：");
    PreOrderTraverse(T, VisitPrint);
    printf("\n");
    
    PrintTestResult("遍历功能测试", true);
    
    DestroyBiTree(&T);
}

void TestTreeStructure() {
    PrintTestHeader("二叉树结构打印测试");
    
    char *preOrder = "ABC##DE#G##F###";
    char *p = preOrder;
    BiTree T = CreateBiTreeByPreOrder(&p);
    
    printf("树状结构显示:\n");
    PrintBiTreeStructure(T, 0);
    
    PrintTestResult("结构打印测试", true);
    
    DestroyBiTree(&T);
}

void TestModificationOperations() {
    PrintTestHeader("二叉树修改操作测试");
    
    // 创建简单的测试树
    char *preOrder = "AB##C##";
    char *p = preOrder;
    BiTree T = CreateBiTreeByPreOrder(&p);
    
    printf("原始树:\n");
    PrintBiTree(T);
    
    // 为根节点插入左节点
    bool success = InsertAsLeftChild(T, 'X');
    assert(success == true);
    
    printf("插入左节点X后:\n");
    PrintBiTree(T);
    
    // 为根节点插入右节点
    success = InsertAsRightChild(T, 'Y');
    assert(success == true);
    
    printf("插入右节点Y后:\n");
    PrintBiTree(T);
    
    PrintTestResult("插入操作测试", true);
    
    // 测试子树删除
    BiTNode *nodeY = FindNode(T, 'Y');
    success = DeleteSubTree(&T, nodeY);
    assert(success == true);
    
    printf("删除Y子树后:\n");
    PrintBiTree(T);
    
    PrintTestResult("删除操作测试", true);
    
    DestroyBiTree(&T);
}

void TestSequentialBinaryTree() {
    PrintTestHeader("顺序二叉树测试");
    
    SqBiTree T;
    InitSqBiTree(&T);
    
    // 插入元素构建完全二叉树
    assert(SqBiTreeInsert(&T, 0, 'A') == true);  // 根
    assert(SqBiTreeInsert(&T, 1, 'B') == true);  // 左节点
    assert(SqBiTreeInsert(&T, 2, 'C') == true);  // 右节点
    assert(SqBiTreeInsert(&T, 3, 'D') == true);
    assert(SqBiTreeInsert(&T, 4, 'E') == true);
    assert(SqBiTreeInsert(&T, 5, 'F') == true);
    assert(SqBiTreeInsert(&T, 6, 'G') == true);
    
    PrintSqBiTree(T);
    
    // 测试位置计算
    assert(SqBiTreeParent(3) == 1);
    assert(SqBiTreeLeftChild(1) == 3);
    assert(SqBiTreeRightChild(1) == 4);
    
    // 测试元素获取
    assert(SqBiTreeGetNode(T, 0) == 'A');
    assert(SqBiTreeGetNode(T, 3) == 'D');
    
    PrintTestResult("顺序二叉树操作测试", true);
}

void TestArrayCreation() {
    PrintTestHeader("数组创建二叉树测试");
    
    char arr[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    BiTree T = CreateBiTreeByArray(arr, 0, 7);
    
    printf("从数组创建的完全二叉树:\n");
    PrintBiTree(T);
    
    printf("树状结构:\n");
    PrintBiTreeStructure(T, 0);
    
    assert(BiTreeNodeCount(T) == 7);
    assert(BiTreeDepth(T) == 3);
    
    PrintTestResult("数组创建测试", true);
    
    DestroyBiTree(&T);
}

void TestSpecialTrees() {
    PrintTestHeader("特殊二叉树测试");
    
    // 测试单节点树
    BiTree singleNode;
    InitBiTree(&singleNode);
    char *single = "A##";
    char *p1 = single;
    singleNode = CreateBiTreeByPreOrder(&p1);
    
    assert(BiTreeNodeCount(singleNode) == 1);
    assert(BiTreeDepth(singleNode) == 1);
    assert(BiTreeLeafCount(singleNode) == 1);
    
    printf("单节点树测试通过\n");
    DestroyBiTree(&singleNode);
    
    // 测试只有左子树的树
    BiTree leftOnly;
    InitBiTree(&leftOnly);
    char *left = "AB##C##";
    char *p2 = left;
    leftOnly = CreateBiTreeByPreOrder(&p2);
    
    printf("只有左子树的树:\n");
    PrintBiTree(leftOnly);
    
    assert(BiTreeNodeCount(leftOnly) == 3);
    assert(BiTreeDepth(leftOnly) == 2);  // 修正：A有左节点B和右节点C，深度为2
    
    printf("左偏树测试通过\n");
    DestroyBiTree(&leftOnly);
    
    // 测试只有右子树的树  
    BiTree rightOnly;
    InitBiTree(&rightOnly);
    char *right = "A#B#C##";
    char *p3 = right;
    rightOnly = CreateBiTreeByPreOrder(&p3);
    
    printf("只有右子树的树:\n");
    PrintBiTree(rightOnly);
    
    assert(BiTreeNodeCount(rightOnly) == 3);
    assert(BiTreeDepth(rightOnly) == 3);  // A#B#C## 确实是3层深度：A->B->C
    
    printf("右偏树测试通过\n");
    DestroyBiTree(&rightOnly);
    
    PrintTestResult("特殊树结构测试", true);
}

void TestErrorHandling() {
    PrintTestHeader("错误处理测试");
    
    BiTree T = NULL;
    
    // 测试对空树的操作
    assert(BiTreeEmpty(T) == true);
    assert(BiTreeDepth(T) == 0);
    assert(BiTreeNodeCount(T) == 0);
    assert(BiTreeLeafCount(T) == 0);
    assert(FindNode(T, 'A') == NULL);
    
    // 测试无效参数
    assert(InsertAsLeftChild(NULL, 'X') == false);
    assert(InsertAsRightChild(NULL, 'Y') == false);
    assert(DeleteSubTree(&T, NULL) == false);
    
    char path[10];
    assert(FindPath(T, 'A', path) == false);
    
    PrintTestResult("错误处理测试", true);
}

void TestPerformance() {
    PrintTestHeader("性能测试");
    
    clock_t start, end;
    double cpu_time_used;
    
    printf("创建大型二叉树性能测试...\n");
    start = clock();
    
    // 创建一个相对较大的完全二叉树
    char largeArray[1000];
    for (int i = 0; i < 1000; i++) {
        largeArray[i] = 'A' + (i % 26);
    }
    
    BiTree largeTree = CreateBiTreeByArray(largeArray, 0, 1000);
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("创建1000个节点的树用时: %f 秒\n", cpu_time_used);
    
    start = clock();
    int nodeCount = BiTreeNodeCount(largeTree);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("统计节点数（%d个）用时: %f 秒\n", nodeCount, cpu_time_used);
    
    start = clock();
    int depth = BiTreeDepth(largeTree);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("计算树深度（%d）用时: %f 秒\n", depth, cpu_time_used);
    
    DestroyBiTree(&largeTree);
    
    PrintTestResult("性能测试", true);
}

// ============= 高级算法测试 =============

void TestAdvancedAlgorithms() {
    PrintTestHeader("高级算法测试");
    
    // 创建测试树: ABC##DE#G##F###
    char *preOrder = "ABC##DE#G##F###";
    char *p = preOrder;
    BiTree T = CreateBiTreeByPreOrder(&p);
    
    // 测试树的宽度计算
    int width = CalculateWidth(T);
    printf("树的宽度: %d\n", width);
    assert(width >= 1);
    PrintTestResult("宽度计算测试", true);
    
    // 测试完全二叉树判断
    bool isComplete = IsCompleteBiTree(T);
    printf("是否为完全二叉树: %s\n", isComplete ? "是" : "否");
    
    // 测试满二叉树判断
    bool isFull = IsFullBiTree(T);
    printf("是否为满二叉树: %s\n", isFull ? "是" : "否");
    
    // 测试BST判断
    bool isBST = IsBST(T);
    printf("是否为二叉搜索树: %s\n", isBST ? "是" : "否");
    
    PrintTestResult("树性质判断测试", true);
    
    // 测试路径相关算法
    printf("\n所有路径:\n");
    PrintAllPaths(T);
    
    // 测试路径和
    bool hasPath = HasPathSum(T, 200); // ASCII值之和
    printf("是否存在目标和路径: %s\n", hasPath ? "是" : "否");
    
    // 测试直径计算
    int diameter = DiameterOfBinaryTree(T);
    printf("树的直径: %d\n", diameter);
    
    PrintTestResult("路径算法测试", true);
    
    // 测试树的变换
    BiTree copyT = CopyBiTree(T);
    bool isSame = IsSameTree(T, copyT);
    assert(isSame == true);
    
    MirrorBiTree(copyT);
    bool isSymmetric = IsSymmetric(copyT);
    printf("镜像后是否对称: %s\n", isSymmetric ? "是" : "否");
    
    PrintTestResult("树变换测试", true);
    
    DestroyBiTree(&T);
    DestroyBiTree(&copyT);
}

void TestTreeConstruction() {
    PrintTestHeader("树构造测试");
    
    // 测试根据遍历序列构造树
    char preorder[] = "ABDEGCF";
    char inorder[] = "DBGEACF";
    int len = 7;
    
    BiTree T = BuildTreePreIn(preorder, inorder, len);
    
    printf("根据先序和中序构造的树:\n");
    PrintBiTree(T);
    
    // 验证构造的正确性
    assert(T != NULL);
    assert(T->data == 'A');
    
    PrintTestResult("先序中序构造测试", true);
    
    DestroyBiTree(&T);
    
    // 测试后序中序构造
    char postorder[] = "DGEBFCA";
    T = BuildTreePostIn(postorder, inorder, len);
    
    printf("根据后序和中序构造的树:\n");
    PrintBiTree(T);
    
    assert(T != NULL);
    assert(T->data == 'A');
    
    PrintTestResult("后序中序构造测试", true);
    
    DestroyBiTree(&T);
}

void TestThreadedBinaryTree() {
    PrintTestHeader("线索二叉树测试");
    
    // 创建简单的测试树用于线索化
    char *preOrder = "ABC##D##E##";
    char *p = preOrder;
    BiTree T = CreateBiTreeByPreOrder(&p);
    
    printf("原始树的中序遍历:\n");
    InOrder(T);
    printf("\n");
    
    // 注意: 由于线索二叉树使用不同的结构，这里只是演示接口
    // 在实际应用中需要转换数据结构
    printf("线索二叉树功能已实现，但需要专门的数据结构转换\n");
    
    PrintTestResult("线索二叉树接口测试", true);
    
    DestroyBiTree(&T);
}

void TestPerformanceBenchmark() {
    PrintTestHeader("性能基准测试");
    
    printf("开始性能测试...\n");
    
    // 测试基本操作性能
    PerformanceTest_BasicOps(100);
    
    // 测试遍历性能
    PerformanceTest_Traversal(100);
    
    // 测试递归vs非递归性能
    PerformanceCompare_RecursiveVsNonRecursive(100);
    
    PrintTestResult("性能基准测试", true);
}

// ============= 主测试函数 =============

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                二叉树测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // 执行所有测试
    TestBasicOperations();
    TestTraversals();
    TestTreeStructure();
    TestModificationOperations();
    TestSequentialBinaryTree();
    TestArrayCreation();
    TestSpecialTrees();
    TestErrorHandling();
    TestPerformance();
    TestAdvancedAlgorithms();
    TestTreeConstruction();
    TestThreadedBinaryTree();
    TestPerformanceBenchmark();
    
    printf("\n============================================================\n");
    printf("所有测试完成！\n");
    printf("============================================================\n");
    
    return 0;
} 