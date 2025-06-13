/* sort_test.c - 排序算法测试和分析函数实现
 * create by: zw.duan
 */

#include "sort.h"

// ============= 排序算法测试和分析实现 =============

/**
 * 测试单个排序算法
 */
SortResult TestSortAlgorithm(void (*sort_func)(int[], int), 
                           const char* name, 
                           int arr[], int n,
                           bool is_stable,
                           const char* time_complexity,
                           const char* space_complexity) {
    SortResult result;
    strcpy(result.algorithm_name, name);
    result.is_stable = is_stable;
    strcpy(result.time_complexity, time_complexity);
    strcpy(result.space_complexity, space_complexity);
    
    // 重置计数器
    ResetCounters();
    
    // 记录开始时间
    clock_t start = clock();
    
    // 执行排序
    sort_func(arr, n);
    
    // 记录结束时间
    clock_t end = clock();
    
    // 计算统计信息
    result.comparisons = comparison_count;
    result.swaps = swap_count;
    result.time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    
    return result;
}

/**
 * 测试快速排序（特殊处理）
 */
SortResult TestQuickSort(int arr[], int n) {
    SortResult result;
    strcpy(result.algorithm_name, "快速排序");
    result.is_stable = false;
    strcpy(result.time_complexity, "O(n log n)");
    strcpy(result.space_complexity, "O(log n)");
    
    ResetCounters();
    clock_t start = clock();
    QuickSortWrapper(arr, n);
    clock_t end = clock();
    
    result.comparisons = comparison_count;
    result.swaps = swap_count;
    result.time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    
    return result;
}

/**
 * 测试归并排序（特殊处理）
 */
SortResult TestMergeSort(int arr[], int n) {
    SortResult result;
    strcpy(result.algorithm_name, "归并排序");
    result.is_stable = true;
    strcpy(result.time_complexity, "O(n log n)");
    strcpy(result.space_complexity, "O(n)");
    
    ResetCounters();
    clock_t start = clock();
    MergeSortWrapper(arr, n);
    clock_t end = clock();
    
    result.comparisons = comparison_count;
    result.swaps = swap_count;
    result.time_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    
    return result;
}

/**
 * 比较所有排序算法
 */
void CompareAllSortAlgorithms(int n, DataType data_type) {
    printf("\n=== 排序算法性能比较 ===\n");
    printf("数据规模：%d\n", n);
    
    const char* data_type_names[] = {
        "随机数据", "已排序数据", "逆序数据", "基本有序数据", "大量重复数据"
    };
    printf("数据类型：%s\n\n", data_type_names[data_type]);
    
    // 生成测试数据
    int *original_data = (int*)malloc(n * sizeof(int));
    int *test_data = (int*)malloc(n * sizeof(int));
    GenerateTestData(original_data, n, data_type);
    
    SortResult results[10];
    int result_count = 0;
    
    // 测试各种排序算法
    printf("正在测试各种排序算法...\n");
    
    // 插入排序
    if (n <= 10000) {  // 插入排序只测试小规模数据
        CopyArray(original_data, test_data, n);
        results[result_count++] = TestSortAlgorithm(InsertionSort, "直接插入排序", 
                                                   test_data, n, true, "O(n²)", "O(1)");
        printf("✓ 直接插入排序完成\n");
    }
    
    // 折半插入排序
    if (n <= 10000) {
        CopyArray(original_data, test_data, n);
        results[result_count++] = TestSortAlgorithm(BinaryInsertionSort, "折半插入排序", 
                                                   test_data, n, true, "O(n²)", "O(1)");
        printf("✓ 折半插入排序完成\n");
    }
    
    // 希尔排序
    CopyArray(original_data, test_data, n);
    results[result_count++] = TestSortAlgorithm(ShellSort, "希尔排序", 
                                               test_data, n, false, "O(n^1.3)", "O(1)");
    printf("✓ 希尔排序完成\n");
    
    // 选择排序
    if (n <= 10000) {
        CopyArray(original_data, test_data, n);
        results[result_count++] = TestSortAlgorithm(SelectionSort, "简单选择排序", 
                                                   test_data, n, false, "O(n²)", "O(1)");
        printf("✓ 简单选择排序完成\n");
    }
    
    // 堆排序
    CopyArray(original_data, test_data, n);
    results[result_count++] = TestSortAlgorithm(HeapSort, "堆排序", 
                                               test_data, n, false, "O(n log n)", "O(1)");
    printf("✓ 堆排序完成\n");
    
    // 冒泡排序
    if (n <= 10000) {
        CopyArray(original_data, test_data, n);
        results[result_count++] = TestSortAlgorithm(BubbleSort, "冒泡排序", 
                                                   test_data, n, true, "O(n²)", "O(1)");
        printf("✓ 冒泡排序完成\n");
    }
    
    // 快速排序
    CopyArray(original_data, test_data, n);
    results[result_count++] = TestQuickSort(test_data, n);
    printf("✓ 快速排序完成\n");
    
    // 归并排序
    CopyArray(original_data, test_data, n);
    results[result_count++] = TestMergeSort(test_data, n);
    printf("✓ 归并排序完成\n");
    
    // 基数排序
    CopyArray(original_data, test_data, n);
    results[result_count++] = TestSortAlgorithm(RadixSort, "基数排序", 
                                               test_data, n, true, "O(d(n+r))", "O(n+r)");
    printf("✓ 基数排序完成\n");
    
    // 打印比较结果
    PrintSortComparison(results, result_count);
    
    free(original_data);
    free(test_data);
}

/**
 * 打印排序结果
 */
void PrintSortResult(SortResult result) {
    printf("算法：%s\n", result.algorithm_name);
    printf("  比较次数：%d\n", result.comparisons);
    printf("  交换次数：%d\n", result.swaps);
    printf("  执行时间：%.3f ms\n", result.time_ms);
    printf("  稳定性：%s\n", result.is_stable ? "稳定" : "不稳定");
    printf("  时间复杂度：%s\n", result.time_complexity);
    printf("  空间复杂度：%s\n", result.space_complexity);
    printf("\n");
}

/**
 * 打印排序算法比较表
 */
void PrintSortComparison(SortResult results[], int count) {
    printf("\n  排序算法性能比较表\n");
    printf("================================================================================\n");
    printf("%-15s %-10s %-10s %-10s %-8s %-12s %-12s\n", 
           "算法名称", "比较次数", "交换次数", "时间(ms)", "稳定性", "时间复杂度", "空间复杂度");
    printf("================================================================================\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-15s %-10d %-10d %-10.3f %-8s %-12s %-12s\n",
               results[i].algorithm_name,
               results[i].comparisons,
               results[i].swaps,
               results[i].time_ms,
               results[i].is_stable ? "稳定" : "不稳定",
               results[i].time_complexity,
               results[i].space_complexity);
    }
    printf("================================================================================\n");
    
    // 找出最快的算法
    int fastest_index = 0;
    for (int i = 1; i < count; i++) {
        if (results[i].time_ms < results[fastest_index].time_ms) {
            fastest_index = i;
        }
    }
    
    printf("\n🏆 最快算法：%s (%.3f ms)\n", 
           results[fastest_index].algorithm_name, 
           results[fastest_index].time_ms);
    
    // 找出比较次数最少的算法
    int min_comparisons_index = 0;
    for (int i = 1; i < count; i++) {
        if (results[i].comparisons < results[min_comparisons_index].comparisons) {
            min_comparisons_index = i;
        }
    }
    
    printf(" 比较次数最少：%s (%d 次)\n", 
           results[min_comparisons_index].algorithm_name, 
           results[min_comparisons_index].comparisons);
    
    // 找出交换次数最少的算法
    int min_swaps_index = 0;
    for (int i = 1; i < count; i++) {
        if (results[i].swaps < results[min_swaps_index].swaps) {
            min_swaps_index = i;
        }
    }
    
    printf(" 交换次数最少：%s (%d 次)\n", 
           results[min_swaps_index].algorithm_name, 
           results[min_swaps_index].swaps);
}

// ============= 数据结构真题演示实现 =============

/**
 * 数据结构真题演示：排序算法分析
 */
void SortExamDemonstration(void) {
    printf("\n=== 数据结构真题演示：排序算法分析 ===\n");
    
    printf("题目1：比较各种排序算法的时间复杂度和稳定性\n");
    printf("================================================================================\n");
    printf("%-15s %-15s %-15s %-15s %-10s\n", 
           "排序算法", "最好情况", "平均情况", "最坏情况", "稳定性");
    printf("================================================================================\n");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "直接插入排序", "O(n)", "O(n²)", "O(n²)", "稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "折半插入排序", "O(n log n)", "O(n²)", "O(n²)", "稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "希尔排序", "O(n)", "O(n^1.3)", "O(n²)", "不稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "简单选择排序", "O(n²)", "O(n²)", "O(n²)", "不稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "堆排序", "O(n log n)", "O(n log n)", "O(n log n)", "不稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "冒泡排序", "O(n)", "O(n²)", "O(n²)", "稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "快速排序", "O(n log n)", "O(n log n)", "O(n²)", "不稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "归并排序", "O(n log n)", "O(n log n)", "O(n log n)", "稳定");
    printf("%-15s %-15s %-15s %-15s %-10s\n", "基数排序", "O(d(n+r))", "O(d(n+r))", "O(d(n+r))", "稳定");
    printf("================================================================================\n");
    
    printf("\n题目2：在什么情况下选择哪种排序算法？\n");
    printf(" 算法选择建议：\n");
    printf("  • 小规模数据（n < 50）：直接插入排序\n");
    printf("  • 基本有序数据：直接插入排序或冒泡排序\n");
    printf("  • 大规模随机数据：快速排序\n");
    printf("  • 要求稳定排序：归并排序\n");
    printf("  • 内存受限：堆排序\n");
    printf("  • 整数且范围较小：基数排序或计数排序\n");
    printf("  • 最坏情况性能保证：堆排序或归并排序\n");
}

/**
 * 数据结构真题演示：快速排序过程
 */
void QuickSortExamDemo(void) {
    printf("\n=== 数据结构真题演示：快速排序过程 ===\n");
    
    int arr[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("题目：对数组 [49, 38, 65, 97, 76, 13, 27, 49] 进行快速排序\n");
    printf("要求：写出每次分区后的结果\n\n");
    
    printf("解答过程：\n");
    printf("初始数组：");
    PrintArray(arr, n, "");
    
    printf("\n第1次分区（基准=49）：\n");
    printf("  分区前：[49, 38, 65, 97, 76, 13, 27, 49]\n");
    printf("  分区后：[38, 13, 27, 49, 76, 97, 65, 49]\n");
    printf("  基准位置：3\n");
    
    printf("\n第2次分区（左半部分，基准=27）：\n");
    printf("  分区前：[38, 13, 27]\n");
    printf("  分区后：[13, 27, 38]\n");
    printf("  基准位置：1\n");
    
    printf("\n第3次分区（右半部分，基准=49）：\n");
    printf("  分区前：[76, 97, 65, 49]\n");
    printf("  分区后：[49, 97, 65, 76]\n");
    printf("  基准位置：0\n");
    
    printf("\n继续递归分区...\n");
    printf("最终结果：[13, 27, 38, 49, 49, 65, 76, 97]\n");
    
    printf("\n 考点总结：\n");
    printf("  1. 理解分区过程的具体步骤\n");
    printf("  2. 掌握基准元素的选择和移动\n");
    printf("  3. 理解递归调用的过程\n");
    printf("  4. 分析时间复杂度：平均O(n log n)，最坏O(n²)\n");
}

/**
 * 数据结构真题演示：堆排序过程
 */
void HeapSortExamDemo(void) {
    printf("\n=== 数据结构真题演示：堆排序过程 ===\n");
    
    int arr[] = {16, 7, 3, 20, 17, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("题目：对数组 [16, 7, 3, 20, 17, 8] 进行堆排序\n");
    printf("要求：写出建堆和排序的每一步\n\n");
    
    printf("解答过程：\n");
    printf("初始数组：");
    PrintArray(arr, n, "");
    
    printf("\n步骤1：建立最大堆\n");
    printf("  从最后一个非叶节点开始调整（索引 %d）\n", n/2 - 1);
    
    printf("  调整节点2（值=3）：\n");
    printf("    [16, 7, 3, 20, 17, 8] → [16, 7, 8, 20, 17, 3]\n");
    
    printf("  调整节点1（值=7）：\n");
    printf("    [16, 7, 8, 20, 17, 3] → [16, 20, 8, 7, 17, 3]\n");
    
    printf("  调整节点0（值=16）：\n");
    printf("    [16, 20, 8, 7, 17, 3] → [20, 17, 8, 7, 16, 3]\n");
    
    printf("  建堆完成：[20, 17, 8, 7, 16, 3]\n");
    
    printf("\n步骤2：排序过程\n");
    printf("  第1次：交换20和3，调整堆 → [17, 16, 8, 7, 3, 20]\n");
    printf("  第2次：交换17和3，调整堆 → [16, 7, 8, 3, 17, 20]\n");
    printf("  第3次：交换16和3，调整堆 → [8, 7, 3, 16, 17, 20]\n");
    printf("  第4次：交换8和3，调整堆 → [7, 3, 8, 16, 17, 20]\n");
    printf("  第5次：交换7和3 → [3, 7, 8, 16, 17, 20]\n");
    
    printf("\n最终结果：[3, 7, 8, 16, 17, 20]\n");
    
    printf("\n 考点总结：\n");
    printf("  1. 理解堆的性质和存储结构\n");
    printf("  2. 掌握建堆的过程（从下往上调整）\n");
    printf("  3. 理解堆排序的过程（取堆顶，调整剩余元素）\n");
    printf("  4. 时间复杂度：O(n log n)，空间复杂度：O(1)\n");
}

/**
 * 数据结构真题演示：归并排序过程
 */
void MergeSortExamDemo(void) {
    printf("\n=== 数据结构真题演示：归并排序过程 ===\n");
    
    int arr[] = {49, 38, 65, 97, 76, 13, 27, 49};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("题目：对数组 [49, 38, 65, 97, 76, 13, 27, 49] 进行归并排序\n");
    printf("要求：写出分治和合并的每一步\n\n");
    
    printf("解答过程：\n");
    printf("初始数组：");
    PrintArray(arr, n, "");
    
    printf("\n步骤1：分治过程（递归分解）\n");
    printf("  [49, 38, 65, 97, 76, 13, 27, 49]\n");
    printf("       ↓\n");
    printf("  [49, 38, 65, 97]    [76, 13, 27, 49]\n");
    printf("       ↓                    ↓\n");
    printf("  [49, 38] [65, 97]    [76, 13] [27, 49]\n");
    printf("     ↓       ↓           ↓       ↓\n");
    printf("  [49][38] [65][97]   [76][13] [27][49]\n");
    
    printf("\n步骤2：合并过程（自底向上合并）\n");
    printf("  合并 [49] 和 [38] → [38, 49]\n");
    printf("  合并 [65] 和 [97] → [65, 97]\n");
    printf("  合并 [76] 和 [13] → [13, 76]\n");
    printf("  合并 [27] 和 [49] → [27, 49]\n");
    
    printf("\n  合并 [38, 49] 和 [65, 97] → [38, 49, 65, 97]\n");
    printf("  合并 [13, 76] 和 [27, 49] → [13, 27, 49, 76]\n");
    
    printf("\n  合并 [38, 49, 65, 97] 和 [13, 27, 49, 76] → [13, 27, 38, 49, 49, 65, 76, 97]\n");
    
    printf("\n最终结果：[13, 27, 38, 49, 49, 65, 76, 97]\n");
    
    printf("\n 考点总结：\n");
    printf("  1. 理解分治思想的应用\n");
    printf("  2. 掌握递归分解的过程\n");
    printf("  3. 理解合并两个有序数组的算法\n");
    printf("  4. 时间复杂度：O(n log n)，空间复杂度：O(n)\n");
    printf("  5. 稳定排序，性能稳定\n");
}

/**
 * 排序算法综合演示
 */
void SortComprehensiveDemo(void) {
    printf("\n=== 排序算法综合演示 ===\n");
    
    // 小规模演示
    printf(" 小规模数据演示（便于观察过程）\n");
    int small_data[] = {64, 34, 25, 12, 22, 11, 90};
    int small_n = sizeof(small_data) / sizeof(small_data[0]);
    
    printf("\n原始数据：");
    PrintArray(small_data, small_n, "");
    
    // 测试各种排序算法
    int test_data[10];
    
    printf("\n1. 直接插入排序：\n");
    CopyArray(small_data, test_data, small_n);
    ResetCounters();
    InsertionSort(test_data, small_n);
    printf("   结果：");
    PrintArray(test_data, small_n, "");
    printf("   比较次数：%d，交换次数：%d\n", comparison_count, swap_count);
    
    printf("\n2. 快速排序：\n");
    CopyArray(small_data, test_data, small_n);
    ResetCounters();
    QuickSortWrapper(test_data, small_n);
    printf("   结果：");
    PrintArray(test_data, small_n, "");
    printf("   比较次数：%d，交换次数：%d\n", comparison_count, swap_count);
    
    printf("\n3. 堆排序：\n");
    CopyArray(small_data, test_data, small_n);
    ResetCounters();
    HeapSort(test_data, small_n);
    printf("   结果：");
    PrintArray(test_data, small_n, "");
    printf("   比较次数：%d，交换次数：%d\n", comparison_count, swap_count);
    
    // 大规模性能测试
    printf("\n  大规模性能测试\n");
    int sizes[] = {1000, 5000, 10000};
    int size_count = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int i = 0; i < size_count; i++) {
        printf("\n数据规模：%d\n", sizes[i]);
        CompareAllSortAlgorithms(sizes[i], RANDOM_DATA);
    }
    
    // 不同数据类型测试
    printf("\n 不同数据类型性能测试（数据规模：5000）\n");
    DataType types[] = {RANDOM_DATA, SORTED_DATA, REVERSE_DATA, NEARLY_SORTED, DUPLICATE_DATA};
    const char* type_names[] = {"随机数据", "已排序", "逆序数据", "基本有序", "大量重复"};
    
    for (int i = 0; i < 5; i++) {
        printf("\n--- %s ---\n", type_names[i]);
        CompareAllSortAlgorithms(5000, types[i]);
    }
} 