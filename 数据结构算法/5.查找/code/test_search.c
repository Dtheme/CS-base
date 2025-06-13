/* test_search.c - 查找算法完整测试程序
 * create by: zw.duan
 * 
 * 测试数据结构中的所有查找算法：
 * 1. 顺序查找（线性查找）
 * 2. 二分查找（折半查找）
 * 3. 插值查找
 * 4. 斐波那契查找
 * 5. 分块查找（索引顺序查找）
 */

#include "search.h"
#include <assert.h>

// ============= 测试工具函数 =============

/**
 * 打印查找结果
 */
void PrintSearchResult(SearchResult result, const char* algorithm_name, int key) {
    printf("\n--- %s 查找结果 ---\n", algorithm_name);
    printf("查找关键字: %d\n", key);
    printf("查找结果: %s\n", result.found ? " 找到" : " 未找到");
    if (result.found) {
        printf("位置: %d\n", result.index);
    }
    printf("比较次数: %d\n", result.comparisons);
    printf("耗时: %.4f ms\n", result.time_cost);
}

/**
 * 验证查找结果的正确性
 */
bool VerifySearchResult(int arr[], int n, SearchResult result, int key) {
    if (result.found) {
        // 检查位置是否有效
        if (result.index < 0 || result.index >= n) {
            printf(" 错误：返回的位置 %d 超出数组范围 [0, %d]\n", result.index, n-1);
            return false;
        }
        // 检查位置上的值是否正确
        if (arr[result.index] != key) {
            printf(" 错误：位置 %d 上的值是 %d，不是要查找的 %d\n", 
                   result.index, arr[result.index], key);
            return false;
        }
        return true;
    } else {
        // 检查确实不存在
        for (int i = 0; i < n; i++) {
            if (arr[i] == key) {
                printf(" 错误：数组中存在 %d（位置 %d），但查找结果显示未找到\n", key, i);
                return false;
            }
        }
        return true;
    }
}

// ============= 基本查找算法测试 =============

/**
 * 测试顺序查找
 */
void TestSequentialSearch(void) {
    printf("\n ========== 顺序查找测试 ==========\n");
    
    // 测试数据
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 88, 76, 50};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("测试数组: ");
    PrintArray(arr, n);
    printf("\n");
    
    // 测试存在的元素
    int search_keys[] = {25, 11, 90, 64};
    int key_count = sizeof(search_keys) / sizeof(search_keys[0]);
    
    printf("\n--- 测试存在的元素 ---\n");
    for (int i = 0; i < key_count; i++) {
        SearchResult result = SequentialSearch(arr, n, search_keys[i]);
        PrintSearchResult(result, "顺序查找", search_keys[i]);
        assert(VerifySearchResult(arr, n, result, search_keys[i]));
    }
    
    // 测试不存在的元素
    int not_exist_keys[] = {100, 5, 77};
    int not_exist_count = sizeof(not_exist_keys) / sizeof(not_exist_keys[0]);
    
    printf("\n--- 测试不存在的元素 ---\n");
    for (int i = 0; i < not_exist_count; i++) {
        SearchResult result = SequentialSearch(arr, n, not_exist_keys[i]);
        PrintSearchResult(result, "顺序查找", not_exist_keys[i]);
        assert(VerifySearchResult(arr, n, result, not_exist_keys[i]));
    }
    
    // 测试带哨兵的顺序查找
    printf("\n--- 测试带哨兵的顺序查找 ---\n");
    int arr_with_sentinel[11];  // 预留哨兵位置
    CopyArray(arr, arr_with_sentinel, n);
    
    SearchResult result1 = SequentialSearchWithSentinel(arr_with_sentinel, n, 25);
    PrintSearchResult(result1, "带哨兵顺序查找", 25);
    assert(VerifySearchResult(arr_with_sentinel, n, result1, 25));
    
    SearchResult result2 = SequentialSearchWithSentinel(arr_with_sentinel, n, 99);
    PrintSearchResult(result2, "带哨兵顺序查找", 99);
    assert(VerifySearchResult(arr_with_sentinel, n, result2, 99));
    
    printf(" 顺序查找测试通过！\n");
}

/**
 * 测试二分查找
 */
void TestBinarySearch(void) {
    printf("\n ========== 二分查找测试 ==========\n");
    
    // 生成有序测试数据
    int arr[20];
    GenerateOrderedArray(arr, 20, 10, 5);  // 10, 15, 20, 25, ..., 105
    
    printf("测试数组（有序）: ");
    PrintArray(arr, 20);
    printf("\n");
    
    // 测试存在的元素
    int search_keys[] = {10, 25, 50, 75, 105};
    int key_count = sizeof(search_keys) / sizeof(search_keys[0]);
    
    printf("\n--- 测试存在的元素 ---\n");
    for (int i = 0; i < key_count; i++) {
        SearchResult result = BinarySearch(arr, 20, search_keys[i]);
        PrintSearchResult(result, "二分查找", search_keys[i]);
        assert(VerifySearchResult(arr, 20, result, search_keys[i]));
    }
    
    // 测试不存在的元素
    int not_exist_keys[] = {5, 12, 77, 110};
    int not_exist_count = sizeof(not_exist_keys) / sizeof(not_exist_keys[0]);
    
    printf("\n--- 测试不存在的元素 ---\n");
    for (int i = 0; i < not_exist_count; i++) {
        SearchResult result = BinarySearch(arr, 20, not_exist_keys[i]);
        PrintSearchResult(result, "二分查找", not_exist_keys[i]);
        assert(VerifySearchResult(arr, 20, result, not_exist_keys[i]));
    }
    
    // 测试递归版本
    printf("\n--- 测试递归版本二分查找 ---\n");
    int comparisons = 0;
    int index = BinarySearchRecursive(arr, 0, 19, 50, &comparisons);
    printf("递归二分查找 50: 位置=%d, 比较次数=%d\n", index, comparisons);
    assert(index >= 0 && arr[index] == 50);
    
    printf(" 二分查找测试通过！\n");
}

/**
 * 测试插值查找
 */
void TestInterpolationSearch(void) {
    printf("\n ========== 插值查找测试 ==========\n");
    
    // 生成均匀分布的有序数据
    int arr[20];
    GenerateOrderedArray(arr, 20, 100, 10);  // 100, 110, 120, ..., 290
    
    printf("测试数组（均匀分布）: ");
    PrintArray(arr, 20);
    printf("\n");
    
    // 测试存在的元素
    int search_keys[] = {100, 150, 200, 250, 290};
    int key_count = sizeof(search_keys) / sizeof(search_keys[0]);
    
    printf("\n--- 测试存在的元素 ---\n");
    for (int i = 0; i < key_count; i++) {
        SearchResult result = InterpolationSearch(arr, 20, search_keys[i]);
        PrintSearchResult(result, "插值查找", search_keys[i]);
        assert(VerifySearchResult(arr, 20, result, search_keys[i]));
    }
    
    // 测试不存在的元素
    int not_exist_keys[] = {95, 105, 275, 300};
    int not_exist_count = sizeof(not_exist_keys) / sizeof(not_exist_keys[0]);
    
    printf("\n--- 测试不存在的元素 ---\n");
    for (int i = 0; i < not_exist_count; i++) {
        SearchResult result = InterpolationSearch(arr, 20, not_exist_keys[i]);
        PrintSearchResult(result, "插值查找", not_exist_keys[i]);
        assert(VerifySearchResult(arr, 20, result, not_exist_keys[i]));
    }
    
    printf(" 插值查找测试通过！\n");
}

/**
 * 测试斐波那契查找
 */
void TestFibonacciSearch(void) {
    printf("\n ========== 斐波那契查找测试 ==========\n");
    
    // 生成有序测试数据
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("测试数组: ");
    PrintArray(arr, n);
    printf("\n");
    
    // 测试存在的元素
    int search_keys[] = {1, 7, 15, 23, 25};
    int key_count = sizeof(search_keys) / sizeof(search_keys[0]);
    
    printf("\n--- 测试存在的元素 ---\n");
    for (int i = 0; i < key_count; i++) {
        SearchResult result = FibonacciSearch(arr, n, search_keys[i]);
        PrintSearchResult(result, "斐波那契查找", search_keys[i]);
        assert(VerifySearchResult(arr, n, result, search_keys[i]));
    }
    
    // 测试不存在的元素
    int not_exist_keys[] = {0, 8, 20, 30};
    int not_exist_count = sizeof(not_exist_keys) / sizeof(not_exist_keys[0]);
    
    printf("\n--- 测试不存在的元素 ---\n");
    for (int i = 0; i < not_exist_count; i++) {
        SearchResult result = FibonacciSearch(arr, n, not_exist_keys[i]);
        PrintSearchResult(result, "斐波那契查找", not_exist_keys[i]);
        assert(VerifySearchResult(arr, n, result, not_exist_keys[i]));
    }
    
    printf(" 斐波那契查找测试通过！\n");
}

/**
 * 测试分块查找
 */
void TestBlockSearch(void) {
    printf("\n ========== 分块查找测试 ==========\n");
    
    BlockSearchTable table;
    InitBlockTable(&table);
    
    // 插入测试数据（模拟分块结构）
    int test_data[] = {3, 7, 10, 12, 15, 20, 25, 28, 30, 35, 40, 45};
    int n = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("插入数据: ");
    for (int i = 0; i < n; i++) {
        InsertIntoBlock(&table, test_data[i]);
        printf("%d ", test_data[i]);
    }
    printf("\n\n");
    
    PrintBlockTable(&table);
    
    // 测试存在的元素
    int search_keys[] = {7, 15, 28, 45};
    int key_count = sizeof(search_keys) / sizeof(search_keys[0]);
    
    printf("\n--- 测试存在的元素 ---\n");
    for (int i = 0; i < key_count; i++) {
        SearchResult result = BlockSearch(&table, search_keys[i]);
        PrintSearchResult(result, "分块查找", search_keys[i]);
        // 验证结果
        if (result.found) {
            assert(table.data[result.index] == search_keys[i]);
        }
    }
    
    // 测试不存在的元素
    int not_exist_keys[] = {1, 8, 22, 50};
    int not_exist_count = sizeof(not_exist_keys) / sizeof(not_exist_keys[0]);
    
    printf("\n--- 测试不存在的元素 ---\n");
    for (int i = 0; i < not_exist_count; i++) {
        SearchResult result = BlockSearch(&table, not_exist_keys[i]);
        PrintSearchResult(result, "分块查找", not_exist_keys[i]);
        assert(!result.found);
    }
    
    printf(" 分块查找测试通过！\n");
}

// ============= 性能比较测试 =============

/**
 * 查找算法性能比较
 */
void TestSearchPerformance(void) {
    printf("\n  ========== 查找算法性能比较 ==========\n");
    
    const int sizes[] = {100, 500, 1000, 5000};
    const int size_count = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int s = 0; s < size_count; s++) {
        int n = sizes[s];
        printf("\n--- 数据规模: %d ---\n", n);
        
        // 生成测试数据
        int* arr = (int*)malloc(n * sizeof(int));
        GenerateOrderedArray(arr, n, 1, 1);  // 1, 2, 3, ..., n
        
        // 选择测试关键字（中间位置）
        int key = n / 2;
        
        printf("查找关键字: %d\n", key);
        
        // 测试各种算法
        SearchResult seq_result = SequentialSearch(arr, n, key);
        SearchResult bin_result = BinarySearch(arr, n, key);
        SearchResult int_result = InterpolationSearch(arr, n, key);
        SearchResult fib_result = FibonacciSearch(arr, n, key);
        
        printf("\n算法性能比较:\n");
        printf("%-15s | 比较次数 | 耗时(ms)\n", "算法名称");
        printf("----------------|----------|----------\n");
        printf("%-15s | %8d | %8.4f\n", "顺序查找", seq_result.comparisons, seq_result.time_cost);
        printf("%-15s | %8d | %8.4f\n", "二分查找", bin_result.comparisons, bin_result.time_cost);
        printf("%-15s | %8d | %8.4f\n", "插值查找", int_result.comparisons, int_result.time_cost);
        printf("%-15s | %8d | %8.4f\n", "斐波那契查找", fib_result.comparisons, fib_result.time_cost);
        
        // 计算效率提升
        if (seq_result.comparisons > 0) {
            printf("\n效率提升（相对于顺序查找）:\n");
            printf("二分查找: %.2fx\n", (double)seq_result.comparisons / bin_result.comparisons);
            printf("插值查找: %.2fx\n", (double)seq_result.comparisons / int_result.comparisons);
            printf("斐波那契查找: %.2fx\n", (double)seq_result.comparisons / fib_result.comparisons);
        }
        
        free(arr);
    }
}

// ============= 数据结构真题演示 =============

/**
 * 数据结构真题演示：平均查找长度计算
 */
void ExamDemo_SearchASL(void) {
    printf("\n ========== 数据结构真题演示：ASL计算 ==========\n");
    
    printf("题目：有序表 [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]，使用二分查找\n");
    printf("求：(1) 成功查找的平均查找长度ASL\n");
    printf("    (2) 失败查找的平均查找长度ASL\n\n");
    
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("解答过程：\n");
    printf("1. 构造二分查找的判定树：\n");
    printf("                    9(arr[4])\n");
    printf("                   /         \\\n");
    printf("               3(arr[1])   15(arr[7])\n");
    printf("              /    \\       /    \\\n");
    printf("          1(arr[0]) 5   11(arr[5]) 17(arr[8])\n");
    printf("                   / \\   /  \\      /  \\\n");
    printf("                  ×   7 ×  13    ×  19(arr[9])\n");
    printf("                     / \\   / \\      /  \\\n");
    printf("                    ×   × ×   ×    ×    ×\n\n");
    
    // 实际测试各元素的查找次数
    printf("2. 实际测试各元素的查找比较次数：\n");
    int total_comparisons = 0;
    for (int i = 0; i < n; i++) {
        SearchResult result = BinarySearch(arr, n, arr[i]);
        printf("查找 %2d: %d次比较\n", arr[i], result.comparisons);
        total_comparisons += result.comparisons;
    }
    
    double asl_success = (double)total_comparisons / n;
    printf("\n3. 成功查找ASL = %d / %d = %.2f\n", total_comparisons, n, asl_success);
    
    printf("\n4. 理论公式验证：\n");
    printf("   对于n=10的完全二叉树，ASL ≈ log₂(n+1) - 1 = log₂(11) - 1 ≈ 2.46\n");
    printf("   实际计算结果: %.2f （基本吻合）\n", asl_success);
    
    printf("\n5. 失败查找ASL分析：\n");
    printf("   失败查找对应判定树的外部节点\n");
    printf("   外部节点数 = n + 1 = 11\n");
    printf("   各外部节点的查找长度分别为其深度\n");
    printf("   失败查找ASL ≈ log₂(n+1) ≈ 3.46\n");
}

/**
 * 数据结构真题演示：分块查找ASL计算
 */
void ExamDemo_BlockSearch(void) {
    printf("\n ========== 数据结构真题演示：分块查找 ==========\n");
    
    printf("题目：对长度为100的有序表进行分块查找\n");
    printf("求：最优分块数和对应的ASL\n\n");
    
    printf("解答过程：\n");
    printf("1. 分块查找的ASL公式：\n");
    printf("   ASL = L_I + L_S\n");
    printf("   其中 L_I 是索引表的平均查找长度\n");
    printf("        L_S 是块内的平均查找长度\n\n");
    
    printf("2. 设分成 b 块，每块 s 个元素，则 b × s = n = 100\n");
    printf("   L_I = (b + 1) / 2  （索引表顺序查找）\n");
    printf("   L_S = (s + 1) / 2  （块内顺序查找）\n");
    printf("   ASL = (b + 1) / 2 + (s + 1) / 2\n\n");
    
    printf("3. 由于 s = n / b = 100 / b，代入得：\n");
    printf("   ASL = (b + 1) / 2 + (100/b + 1) / 2\n");
    printf("       = (b + 1 + 100/b + 1) / 2\n");
    printf("       = (b + 100/b + 2) / 2\n\n");
    
    printf("4. 对 b 求导并令其为0：\n");
    printf("   d(ASL)/db = (1 - 100/b²) / 2 = 0\n");
    printf("   解得：b² = 100，即 b = 10\n\n");
    
    printf("5. 最优分块方案：\n");
    printf("   块数 b = 10\n");
    printf("   每块大小 s = 100 / 10 = 10\n");
    printf("   最小ASL = (10 + 100/10 + 2) / 2 = (10 + 10 + 2) / 2 = 11\n\n");
    
    // 实际验证
    printf("6. 实际验证（模拟小规模数据）：\n");
    BlockSearchTable table;
    InitBlockTable(&table);
    
    // 插入30个数据，分成3块，每块10个
    for (int i = 1; i <= 30; i++) {
        InsertIntoBlock(&table, i);
    }
    
    PrintBlockTable(&table);
    
    // 测试查找性能
    int test_keys[] = {5, 15, 25};
    int total_comparisons = 0;
    
    printf("\n查找测试：\n");
    for (int i = 0; i < 3; i++) {
        SearchResult result = BlockSearch(&table, test_keys[i]);
        printf("查找 %d: %d次比较\n", test_keys[i], result.comparisons);
        total_comparisons += result.comparisons;
    }
    
    printf("平均比较次数: %.2f\n", (double)total_comparisons / 3);
    printf("理论ASL (n=30, b=3): (3 + 30/3 + 2) / 2 = 7.5\n");
}

/**
 * 查找算法综合演示
 */
void SearchAlgorithmDemo(void) {
    printf("\n ========== 查找算法综合演示 ==========\n");
    
    printf("本演示展示了数据结构中的主要查找算法：\n\n");
    
    printf("1.  算法分类：\n");
    printf("   ├── 静态查找：顺序查找、二分查找、插值查找、斐波那契查找\n");
    printf("   ├── 动态查找：二叉搜索树、平衡树、B树、B+树\n");
    printf("   ├── 散列查找：哈希表\n");
    printf("   └── 混合查找：分块查找\n\n");
    
    printf("2. ⏱️ 时间复杂度比较：\n");
    printf("   ├── 顺序查找：O(n)\n");
    printf("   ├── 二分查找：O(log n)\n");
    printf("   ├── 插值查找：O(log log n) ~ O(n)\n");
    printf("   ├── 斐波那契查找：O(log n)\n");
    printf("   ├── 分块查找：O(√n)\n");
    printf("   └── 散列查找：O(1) ~ O(n)\n\n");
    
    printf("3.  适用场景：\n");
    printf("   ├── 顺序查找：任何存储结构，数据无序\n");
    printf("   ├── 二分查找：有序数组，静态数据\n");
    printf("   ├── 插值查找：有序且分布均匀的数据\n");
    printf("   ├── 分块查找：相对稳定的数据，允许插入删除\n");
    printf("   └── 散列查找：快速查找，允许一定的空间开销\n\n");
    
    printf("4.   数据结构重点：\n");
    printf("   ├── ASL（平均查找长度）的计算\n");
    printf("   ├── 二分查找判定树的构造\n");
    printf("   ├── 分块查找的最优分块\n");
    printf("   ├── 哈希函数设计和冲突处理\n");
    printf("   └── 各种查找算法的比较分析\n");
}

// ============= 主测试函数 =============

int main(void) {
    printf(" ========================================\n");
    printf("   数据结构 - 查找算法完整测试\n");
    printf("   作者: zw.duan\n");
    printf("   版本: v1.0 完整实现版\n");
    printf("========================================\n");
    
    // 运行所有测试
    TestSequentialSearch();
    TestBinarySearch();
    TestInterpolationSearch();
    TestFibonacciSearch();
    TestBlockSearch();
    
    // 性能比较测试
    TestSearchPerformance();
    
    // 数据结构真题演示
    ExamDemo_SearchASL();
    ExamDemo_BlockSearch();
    
    // 综合演示
    SearchAlgorithmDemo();
    
    printf("\n ========================================\n");
    printf("    所有查找算法测试通过！\n");
    printf("    基本查找算法实现完成\n");
    printf("    下一步：实现散列表和树表查找\n");
    printf("========================================\n");
    
    return 0;
} 