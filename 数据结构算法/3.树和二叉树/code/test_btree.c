/* test_btree.c - B树完整功能测试程序
 * 
 *  本程序测试B树的所有功能：
 * - 基本操作：创建、插入、删除、查找
 * - 高级操作：范围查询、第k小元素、验证
 * - 性能测试：大数据量插入删除性能
 * - 数据结构要点：ASL计算、复杂度分析
 * 
 * Create by: zw.duan
 */

#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// 测试数据
static int test_data[] = {10, 20, 5, 6, 12, 30, 7, 17, 15, 25, 40, 50, 35, 45, 55};
static int test_size = sizeof(test_data) / sizeof(test_data[0]);

// 函数声明
void TestBTreeBasicOps(void);
void TestBTreeInsertDelete(void);
void TestBTreeSearch(void);
void TestBTreeRangeQuery(void);
void TestBTreeKthElement(void);
void TestBTreeValidation(void);
void TestBTreePerformance(void);
void TestBTreeEdgeCases(void);
void BTreeExamDemonstration(void);
void PrintTestResult(const char* testName, bool passed);

/**
 * 主测试函数
 */
int main(void) {
    printf(" B树数据结构完整功能测试\n");
    printf("=======================\n");
    printf(" 本测试涵盖数据结构所有B树知识点\n\n");
    
    // 运行各项测试
    TestBTreeBasicOps();
    TestBTreeInsertDelete();
    TestBTreeSearch();
    TestBTreeRangeQuery();
    TestBTreeKthElement();
    TestBTreeValidation();
    TestBTreeEdgeCases();
    BTreeExamDemonstration();
    TestBTreePerformance();
    
    printf("\n 所有B树测试完成！\n");
    printf(" 测试覆盖了数据结构B树的所有核心知识点\n");
    
    return 0;
}

/**
 * 基本操作测试
 */
void TestBTreeBasicOps(void) {
    printf("🔧 测试1：B树基本操作\n");
    printf("--------------------\n");
    
    // 创建3阶B树
    BTree tree;
    InitBTree(&tree, 3);
    
    // 测试空树
    assert(BTreeEmpty(&tree) == true);
    printf(" 空树判断正确\n");
    
    // 插入数据并显示树结构
    printf("插入数据：");
    for (int i = 0; i < 10; i++) {
        printf("%d ", test_data[i]);
        BTreeInsert(&tree, test_data[i]);
    }
    printf("\n");
    
    // 显示统计信息
    printf("B树统计信息：\n");
    printf("- 树高度：%d\n", BTreeHeight(&tree));
    printf("- 关键字总数：%d\n", BTreeKeyCount(&tree));
    printf("- 叶子节点数：%d\n", BTreeLeafCount(&tree));
    
    // 遍历展示
    printf("\n B树遍历结果：\n");
    BTreeInOrder(&tree);
    
    // 清理
    DestroyBTree(&tree);
    PrintTestResult("基本操作测试", true);
}

/**
 * 插入删除测试
 */
void TestBTreeInsertDelete(void) {
    printf("\n🌱 测试2：B树插入删除操作\n");
    printf("-------------------------\n");
    
    BTree tree;
    InitBTree(&tree, 5);  // 创建5阶B树
    
    // 测试大量插入
    int insert_data[] = {10, 20, 5, 6, 12, 30, 7, 17, 15, 25, 40, 50, 35, 45, 55, 60, 65, 70};
    int insert_size = sizeof(insert_data) / sizeof(insert_data[0]);
    
    printf("📥 测试批量插入 %d 个元素...\n", insert_size);
    for (int i = 0; i < insert_size; i++) {
        bool success = BTreeInsert(&tree, insert_data[i]);
        assert(success == true);
        
        // 验证树的合法性
        assert(ValidateBTree(&tree) == true);
    }
    
    printf(" 插入完成，树高度：%d，关键字数：%d\n", 
           BTreeHeight(&tree), BTreeKeyCount(&tree));
    
    // 显示树结构
    printf("\n  插入后的B树结构：\n");
    PrintBTree(&tree);
    
    // 测试删除
    printf("\n 测试删除操作...\n");
    int delete_keys[] = {15, 25, 35};
    int delete_size = sizeof(delete_keys) / sizeof(delete_keys[0]);
    
    for (int i = 0; i < delete_size; i++) {
        printf("删除关键字：%d\n", delete_keys[i]);
        bool success = BTreeDelete(&tree, delete_keys[i]);
        assert(success == true);
        
        // 验证删除后树的合法性
        assert(ValidateBTree(&tree) == true);
        
        printf(" 删除成功，当前关键字数：%d\n", BTreeKeyCount(&tree));
    }
    
    printf("\n  删除后的B树结构：\n");
    PrintBTree(&tree);
    
    DestroyBTree(&tree);
    PrintTestResult("插入删除测试", true);
}

/**
 * 查找测试
 */
void TestBTreeSearch(void) {
    printf("\n 测试3：B树查找操作\n");
    printf("-------------------\n");
    
    BTree tree;
    InitBTree(&tree, 4);
    
    // 构建测试树
    int keys[] = {1, 3, 7, 10, 16, 20, 23, 30, 33, 36, 40, 46, 50, 53, 56, 60, 66, 70, 76, 80};
    int key_count = sizeof(keys) / sizeof(keys[0]);
    
    for (int i = 0; i < key_count; i++) {
        BTreeInsert(&tree, keys[i]);
    }
    
    printf(" 构建了包含 %d 个关键字的B树\n", key_count);
    PrintBTree(&tree);
    
    // 测试查找存在的关键字
    printf("\n 测试查找存在的关键字：\n");
    int search_keys[] = {10, 30, 50, 70};
    int search_size = sizeof(search_keys) / sizeof(search_keys[0]);
    
    for (int i = 0; i < search_size; i++) {
        BTreeSearchResult result = BTreeSearch(&tree, search_keys[i]);
        
        if (result.found) {
            printf(" 找到关键字 %d (位置：%d)\n", 
                   search_keys[i], result.index);
        } else {
            printf(" 未找到关键字 %d\n", search_keys[i]);
        }
        
        assert(result.found == true);
    }
    
    // 测试查找不存在的关键字
    printf("\n 测试查找不存在的关键字：\n");
    int missing_keys[] = {5, 15, 25, 75};
    int missing_size = sizeof(missing_keys) / sizeof(missing_keys[0]);
    
    for (int i = 0; i < missing_size; i++) {
        BTreeSearchResult result = BTreeSearch(&tree, missing_keys[i]);
        printf(" 查找关键字 %d: %s\n", 
               missing_keys[i], result.found ? "找到" : "未找到");
        assert(result.found == false);
    }
    
    DestroyBTree(&tree);
    PrintTestResult("查找测试", true);
}

/**
 * 范围查询测试
 */
void TestBTreeRangeQuery(void) {
    printf("\n 测试4：B树范围查询\n");
    printf("-------------------\n");
    
    BTree tree;
    InitBTree(&tree, 3);
    
    // 插入有序数据
    for (int i = 1; i <= 20; i++) {
        BTreeInsert(&tree, i * 5);  // 5, 10, 15, ..., 100
    }
    
    printf(" 构建了包含数据 5, 10, 15, ..., 100 的B树\n");
    
    // 测试范围查询
    BTreeKeyType result[100];
    int count;
    
    printf("\n  范围查询测试：\n");
    
    // 查询 [25, 55] 范围内的关键字
    BTreeRangeQuery(&tree, 25, 55, result, &count);
    printf(" 范围 [25, 55] 内的关键字 (%d个): ", count);
    for (int i = 0; i < count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    assert(count == 7);  // 25, 30, 35, 40, 45, 50, 55
    
    // 查询 [60, 80] 范围内的关键字
    BTreeRangeQuery(&tree, 60, 80, result, &count);
    printf(" 范围 [60, 80] 内的关键字 (%d个): ", count);
    for (int i = 0; i < count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    assert(count == 5);  // 60, 65, 70, 75, 80
    
    // 测试统计函数
    int range_count = BTreeCountRange(&tree, 30, 70);
    printf(" 范围 [30, 70] 内关键字数量: %d\n", range_count);
    assert(range_count == 9);
    
    DestroyBTree(&tree);
    PrintTestResult("范围查询测试", true);
}

/**
 * 第k小元素测试
 */
void TestBTreeKthElement(void) {
    printf("\n 测试5：第k小元素查找\n");
    printf("---------------------\n");
    
    BTree tree;
    InitBTree(&tree, 3);
    
    // 插入乱序数据
    int data[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 65, 90};
    int data_size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < data_size; i++) {
        BTreeInsert(&tree, data[i]);
    }
    
    printf(" 构建了包含乱序数据的B树\n");
    printf("原始数据: ");
    for (int i = 0; i < data_size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    BTreeInOrder(&tree);
    
    // 测试第k小元素
    printf("\n 第k小元素查找测试：\n");
    for (int k = 1; k <= data_size; k++) {
        BTreeKeyType kth = BTreeKthSmallest(&tree, k);
        printf("第%d小元素: %d\n", k, kth);
        assert(kth != -1);
    }
    
    // 测试边界情况
    assert(BTreeKthSmallest(&tree, 0) == -1);
    assert(BTreeKthSmallest(&tree, data_size + 1) == -1);
    
    DestroyBTree(&tree);
    PrintTestResult("第k小元素测试", true);
}

/**
 * 验证测试
 */
void TestBTreeValidation(void) {
    printf("\n 测试6：B树合法性验证\n");
    printf("---------------------\n");
    
    BTree tree;
    InitBTree(&tree, 5);
    
    // 测试空树验证
    assert(ValidateBTree(&tree) == true);
    printf(" 空树验证通过\n");
    
    // 插入数据并验证
    for (int i = 1; i <= 20; i++) {
        BTreeInsert(&tree, i);
        assert(ValidateBTree(&tree) == true);
    }
    printf(" 插入过程中树结构始终合法\n");
    
    // 删除数据并验证
    for (int i = 1; i <= 10; i += 2) {
        BTreeDelete(&tree, i);
        assert(ValidateBTree(&tree) == true);
    }
    printf(" 删除过程中树结构始终合法\n");
    
    DestroyBTree(&tree);
    PrintTestResult("合法性验证测试", true);
}

/**
 * 边界情况测试
 */
void TestBTreeEdgeCases(void) {
    printf("\n🚧 测试7：边界情况处理\n");
    printf("-------------------\n");
    
    BTree tree;
    InitBTree(&tree, 3);
    
    // 测试单个元素
    BTreeInsert(&tree, 42);
    assert(BTreeKeyCount(&tree) == 1);
    assert(BTreeHeight(&tree) == 1);
    printf(" 单元素B树测试通过\n");
    
    // 测试重复插入
    BTreeInsert(&tree, 42);  // 尝试插入重复元素
    assert(BTreeKeyCount(&tree) == 1);  // 应该不增加
    printf(" 重复元素处理正确\n");
    
    // 测试删除不存在的元素
    BTreeDelete(&tree, 99);
    assert(BTreeKeyCount(&tree) == 1);
    printf(" 删除不存在元素处理正确\n");
    
    // 测试删除所有元素
    BTreeDelete(&tree, 42);
    assert(BTreeEmpty(&tree) == true);
    printf(" 删除至空树处理正确\n");
    
    DestroyBTree(&tree);
    PrintTestResult("边界情况测试", true);
}

/**
 * 数据结构题目演示
 */
void BTreeExamDemonstration(void) {
    printf("\n 数据结构B树知识点演示\n");
    printf("======================\n");
    
    printf(" 演示题目：分析5阶B树的性质和操作过程\n\n");
    
    BTree tree;
    InitBTree(&tree, 5);
    
    // 模拟学习题目：向5阶B树中插入序列
    int exam_data[] = {18, 32, 45, 20, 25, 12, 30, 40, 15, 28, 50, 35};
    int exam_size = sizeof(exam_data) / sizeof(exam_data[0]);
    
    printf(" 题目：向5阶B树中依次插入序列：");
    for (int i = 0; i < exam_size; i++) {
        printf("%d ", exam_data[i]);
    }
    printf("\n\n");
    
    printf("  插入过程分析：\n");
    for (int i = 0; i < exam_size; i++) {
        printf("步骤%d: 插入 %d\n", i+1, exam_data[i]);
        BTreeInsert(&tree, exam_data[i]);
        
        printf("  当前树高: %d, 关键字数: %d\n", 
               BTreeHeight(&tree), BTreeKeyCount(&tree));
        
        if (i % 3 == 2) {  // 每3步显示一次树结构
            printf("  当前树结构：\n");
            PrintBTreeStructure(&tree);
            printf("\n");
        }
    }
    
    printf(" 最终B树结构：\n");
    PrintBTree(&tree);
    
    // ASL 计算示例
    printf("\n  平均查找长度(ASL)计算：\n");
    int total_nodes = BTreeKeyCount(&tree);
    int height = BTreeHeight(&tree);
    printf("- 关键字总数：%d\n", total_nodes);
    printf("- 树高度：%d\n", height);
    printf("- 理论最优ASL ≈ log_%d(%d) ≈ %.2f\n", 
           5, total_nodes, height * 0.7);  // 简化计算
    
    DestroyBTree(&tree);
    PrintTestResult("数据结构演示", true);
}

/**
 * 性能测试
 */
void TestBTreePerformance(void) {
    printf("\n⚡ 测试8：B树性能测试\n");
    printf("------------------\n");
    
    BTree tree;
    InitBTree(&tree, 100);  // 高阶B树
    
    const int test_size = 10000;
    clock_t start, end;
    
    // 插入性能测试
    start = clock();
    for (int i = 0; i < test_size; i++) {
        BTreeInsert(&tree, i);
    }
    end = clock();
    
    double insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  插入 %d 个元素耗时: %.4f 秒\n", test_size, insert_time);
    printf("   平均每次插入: %.6f 秒\n", insert_time / test_size);
    
    // 查找性能测试
    start = clock();
    for (int i = 0; i < test_size; i += 100) {
        BTreeSearch(&tree, i);
    }
    end = clock();
    
    double search_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  查找 %d 次耗时: %.4f 秒\n", test_size / 100, search_time);
    printf("   平均每次查找: %.6f 秒\n", search_time / (test_size / 100));
    
    printf(" 性能总结：\n");
    printf("- 树高度：%d\n", BTreeHeight(&tree));
    printf("- 关键字总数：%d\n", BTreeKeyCount(&tree));
    printf("- 平均插入时间复杂度：O(log n)\n");
    printf("- 平均查找时间复杂度：O(log n)\n");
    
    DestroyBTree(&tree);
    PrintTestResult("性能测试", true);
}

/**
 * 打印测试结果
 */
void PrintTestResult(const char* testName, bool passed) {
    if (passed) {
        printf(" %s 通过\n", testName);
    } else {
        printf(" %s 失败\n", testName);
    }
} 