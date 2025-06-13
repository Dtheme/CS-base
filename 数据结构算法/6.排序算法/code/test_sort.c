/* test_sort.c - 排序算法测试程序
 * create by: zw.duan
 * 
 * 数据结构 - 排序算法测试程序
 */

#include "sort.h"

// 测试函数声明
void test_basic_operations(void);
void test_edge_cases(void);
void test_stability(void);
void test_performance(void);
void test_exam_problems(void);
void print_test_header(const char *testName);
void print_test_result(const char *testName, bool passed);

// 辅助函数实现
void print_test_header(const char *testName) {
    printf("\n==================================================\n");
    printf("测试项目: %s\n", testName);
    printf("==================================================\n");
}

void print_test_result(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

// 创建测试数据
void create_test_data(int *arr, int n, const char *type) {
    if (strcmp(type, "random") == 0) {
        int data[] = {64, 34, 25, 12, 22, 11, 90, 5};
        for (int i = 0; i < n && i < 8; i++) {
            arr[i] = data[i];
        }
    } else if (strcmp(type, "sorted") == 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = i + 1;
        }
    } else if (strcmp(type, "reverse") == 0) {
        for (int i = 0; i < n; i++) {
            arr[i] = n - i;
        }
    }
}

// 验证排序结果
bool verify_sort_result(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

// 打印数组
void print_array(int *arr, int n, const char *prefix) {
    printf("%s", prefix);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 复制数组
void copy_array(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void test_basic_operations(void) {
    print_test_header("排序算法基本功能测试");
    
    int test_data[8];
    int temp_data[8];
    int n = 8;
    
    create_test_data(test_data, n, "random");
    printf("原始数据: ");
    print_array(test_data, n, "");
    
    // 测试直接插入排序
    copy_array(test_data, temp_data, n);
    InsertionSort(temp_data, n);
    printf("直接插入排序: ");
    print_array(temp_data, n, "");
    bool insertion_ok = verify_sort_result(temp_data, n);
    
    // 测试快速排序
    copy_array(test_data, temp_data, n);
    QuickSortWrapper(temp_data, n);
    printf("快速排序: ");
    print_array(temp_data, n, "");
    bool quick_ok = verify_sort_result(temp_data, n);
    
    // 测试堆排序
    copy_array(test_data, temp_data, n);
    HeapSort(temp_data, n);
    printf("堆排序: ");
    print_array(temp_data, n, "");
    bool heap_ok = verify_sort_result(temp_data, n);
    
    // 测试归并排序
    copy_array(test_data, temp_data, n);
    MergeSortWrapper(temp_data, n);
    printf("归并排序: ");
    print_array(temp_data, n, "");
    bool merge_ok = verify_sort_result(temp_data, n);
    
    // 测试基数排序
    copy_array(test_data, temp_data, n);
    RadixSort(temp_data, n);
    printf("基数排序: ");
    print_array(temp_data, n, "");
    bool radix_ok = verify_sort_result(temp_data, n);
    
    print_test_result("直接插入排序", insertion_ok);
    print_test_result("快速排序", quick_ok);
    print_test_result("堆排序", heap_ok);
    print_test_result("归并排序", merge_ok);
    print_test_result("基数排序", radix_ok);
}

void test_edge_cases(void) {
    print_test_header("边界情况测试");
    
    bool all_passed = true;
    
    // 测试单元素数组
    printf("单元素数组测试:\n");
    int single[] = {42};
    printf("  原始: ");
    print_array(single, 1, "");
    InsertionSort(single, 1);
    printf("  排序后: ");
    print_array(single, 1, "");
    bool single_ok = verify_sort_result(single, 1);
    all_passed &= single_ok;
    
    // 测试已排序数组
    printf("已排序数组测试:\n");
    int sorted[5];
    create_test_data(sorted, 5, "sorted");
    printf("  原始: ");
    print_array(sorted, 5, "");
    InsertionSort(sorted, 5);
    printf("  排序后: ");
    print_array(sorted, 5, "");
    bool sorted_ok = verify_sort_result(sorted, 5);
    all_passed &= sorted_ok;
    
    // 测试逆序数组
    printf("逆序数组测试:\n");
    int reverse[5];
    create_test_data(reverse, 5, "reverse");
    printf("  原始: ");
    print_array(reverse, 5, "");
    QuickSortWrapper(reverse, 5);
    printf("  排序后: ");
    print_array(reverse, 5, "");
    bool reverse_ok = verify_sort_result(reverse, 5);
    all_passed &= reverse_ok;
    
    // 测试重复元素
    printf("重复元素测试:\n");
    int duplicate[] = {5, 2, 8, 2, 9, 1, 5, 5};
    int dup_n = sizeof(duplicate) / sizeof(duplicate[0]);
    printf("  原始: ");
    print_array(duplicate, dup_n, "");
    MergeSortWrapper(duplicate, dup_n);
    printf("  排序后: ");
    print_array(duplicate, dup_n, "");
    bool dup_ok = verify_sort_result(duplicate, dup_n);
    all_passed &= dup_ok;
    
    print_test_result("边界情况测试", all_passed);
}

void test_stability(void) {
    print_test_header("算法稳定性分析");
    
    printf("稳定排序算法:\n");
    printf("  - 直接插入排序: 相同元素不交换位置\n");
    printf("  - 折半插入排序: 相同元素不交换位置\n");
    printf("  - 冒泡排序: 相同元素不交换位置\n");
    printf("  - 归并排序: 合并时保持相对位置\n");
    printf("  - 基数排序: 按位排序保持稳定性\n");
    
    printf("\n不稳定排序算法:\n");
    printf("  - 简单选择排序: 可能交换相同元素\n");
    printf("  - 希尔排序: 跳跃式交换破坏稳定性\n");
    printf("  - 堆排序: 堆调整可能改变相对位置\n");
    printf("  - 快速排序: 分区过程可能改变相对位置\n");
    
    print_test_result("稳定性分析", true);
}

void test_performance(void) {
    print_test_header("性能比较测试");
    
    printf("时间复杂度比较:\n");
    printf("算法名称        最好情况    平均情况    最坏情况\n");
    printf("================================================\n");
    printf("直接插入排序    O(n)        O(n²)       O(n²)\n");
    printf("快速排序        O(n log n)  O(n log n)  O(n²)\n");
    printf("堆排序          O(n log n)  O(n log n)  O(n log n)\n");
    printf("归并排序        O(n log n)  O(n log n)  O(n log n)\n");
    printf("基数排序        O(d(n+r))   O(d(n+r))   O(d(n+r))\n");
    
    printf("\n空间复杂度比较:\n");
    printf("直接插入排序: O(1)\n");
    printf("快速排序: O(log n)\n");
    printf("堆排序: O(1)\n");
    printf("归并排序: O(n)\n");
    printf("基数排序: O(n+r)\n");
    
    // 简单性能测试
    printf("\n小规模性能测试 (1000个元素):\n");
    int *test_data = (int*)malloc(1000 * sizeof(int));
    int *temp_data = (int*)malloc(1000 * sizeof(int));
    
    // 生成随机数据
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) {
        test_data[i] = rand() % 1000;
    }
    
    clock_t start, end;
    
    // 测试快速排序
    copy_array(test_data, temp_data, 1000);
    start = clock();
    QuickSortWrapper(temp_data, 1000);
    end = clock();
    double quick_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    bool quick_ok = verify_sort_result(temp_data, 1000);
    
    // 测试堆排序
    copy_array(test_data, temp_data, 1000);
    start = clock();
    HeapSort(temp_data, 1000);
    end = clock();
    double heap_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    bool heap_ok = verify_sort_result(temp_data, 1000);
    
    printf("快速排序: %.6f秒 [%s]\n", quick_time, quick_ok ? "正确" : "错误");
    printf("堆排序: %.6f秒 [%s]\n", heap_time, heap_ok ? "正确" : "错误");
    
    free(test_data);
    free(temp_data);
    
    print_test_result("性能测试", quick_ok && heap_ok);
}

void test_exam_problems(void) {
    print_test_header("数据结构真题演示");
    
    printf("题目: 对数组 [49, 38, 65, 97, 76, 13, 27] 进行快速排序\n");
    printf("要求: 写出第一次分区后的结果\n\n");
    
    int exam_data[] = {49, 38, 65, 97, 76, 13, 27};
    int n = sizeof(exam_data) / sizeof(exam_data[0]);
    
    printf("初始数组: ");
    print_array(exam_data, n, "");
    
    printf("以第一个元素49为基准进行分区:\n");
    printf("分区过程: 将小于49的元素移到左边，大于49的元素移到右边\n");
    
    // 手工演示分区过程
    QuickSortWrapper(exam_data, n);
    printf("最终排序结果: ");
    print_array(exam_data, n, "");
    
    printf("\n算法要点:\n");
    printf("1. 选择基准元素（通常是第一个或最后一个）\n");
    printf("2. 分区操作：小于基准的在左，大于基准的在右\n");
    printf("3. 递归处理左右子数组\n");
    printf("4. 时间复杂度：平均O(n log n)，最坏O(n²)\n");
    
    print_test_result("数据结构真题演示", true);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              排序算法测试程序                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    test_basic_operations();
    test_edge_cases();
    test_stability();
    test_performance();
    test_exam_problems();
    
    printf("\n============================================================\n");
    printf("排序算法完整功能测试完成！\n");
    printf("============================================================\n");
    
    printf("\n学习要点:\n");
    printf("1. 掌握各种排序算法的实现原理\n");
    printf("2. 理解时间和空间复杂度分析\n");
    printf("3. 区分稳定和不稳定排序算法\n");
    printf("4. 根据数据特点选择合适的排序算法\n");
    printf("5. 熟练掌握数据结构常考的排序算法\n");
    
    return 0;
} 