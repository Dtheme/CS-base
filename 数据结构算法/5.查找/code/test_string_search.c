/* test_string_search.c - 字符串查找算法测试程序
 * create by: zw.duan
 */

#include "string_search.h"

// 测试函数声明
void test_basic_operations(void);
void test_algorithm_correctness(void);
void test_performance_comparison(void);
void test_exam_demos(void);
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

void test_basic_operations(void) {
    print_test_header("字符串搜索基本操作测试");
    
    const char* test_cases[][3] = {
        {"hello world", "world", "基本匹配"},
        {"hello world", "hello", "开头匹配"},
        {"hello world", "d", "末尾匹配"},
        {"hello world", "xyz", "不存在模式"},
        {"aaaaaaa", "aaa", "重复字符"},
        {"abababab", "abab", "重叠模式"},
        {"mississippi", "issip", "经典用例"}
    };
    
    int test_count = sizeof(test_cases) / sizeof(test_cases[0]);
    bool all_passed = true;
    
    for (int i = 0; i < test_count; i++) {
        const char* text = test_cases[i][0];
        const char* pattern = test_cases[i][1];
        const char* desc = test_cases[i][2];
        
        printf("\n--- %s ---\n", desc);
        printf("主串: \"%s\", 模式串: \"%s\"\n", text, pattern);
        
        // 测试所有算法
        StringSearchResult results[4];
        
        results[0] = BruteForceSearch(text, pattern);
        results[1] = KMPSearch(text, pattern);
        results[2] = BoyerMooreSearch(text, pattern);
        results[3] = SundaySearch(text, pattern);
        
        // 检查结果一致性
        bool consistent = true;
        for (int j = 1; j < 4; j++) {
            if (results[j].found != results[0].found || 
                (results[j].found && results[j].position != results[0].position)) {
                consistent = false;
                break;
            }
        }
        
        printf("结果: %s", results[0].found ? "找到" : "未找到");
        if (results[0].found) {
            printf(" (位置: %d)", results[0].position);
        }
        printf(" - %s\n", consistent ? " 一致" : " 不一致");
        
        if (!consistent) all_passed = false;
    }
    
    print_test_result("字符串搜索基本操作测试", all_passed);
}

void test_algorithm_correctness(void) {
    print_test_header("算法正确性验证");
    
    // 边界情况测试
    const char* edge_cases[][3] = {
        {"", "", "双空串"},
        {"a", "", "空模式串"},
        {"", "a", "空主串"},
        {"a", "a", "单字符匹配"},
        {"a", "b", "单字符不匹配"},
        {"abc", "abcd", "模式串比主串长"},
        {"aaaaab", "aaaab", "几乎全匹配"}
    };
    
    int case_count = sizeof(edge_cases) / sizeof(edge_cases[0]);
    bool all_passed = true;
    
    printf("\n边界情况测试:\n");
    for (int i = 0; i < case_count; i++) {
        const char* text = edge_cases[i][0];
        const char* pattern = edge_cases[i][1];
        const char* desc = edge_cases[i][2];
        
        printf("测试 %d (%s): ", i+1, desc);
        
        // 测试所有算法
        StringSearchResult results[4];
        results[0] = BruteForceSearch(text, pattern);
        results[1] = KMPSearch(text, pattern);
        results[2] = BoyerMooreSearch(text, pattern);
        results[3] = SundaySearch(text, pattern);
        
        // 检查结果一致性
        bool consistent = true;
        for (int j = 1; j < 4; j++) {
            if (results[j].found != results[0].found || 
                (results[j].found && results[j].position != results[0].position)) {
                consistent = false;
                break;
            }
        }
        
        printf("%s\n", consistent ? " 通过" : " 失败");
        if (!consistent) all_passed = false;
    }
    
    print_test_result("算法正确性验证", all_passed);
}

void test_performance_comparison(void) {
    print_test_header("性能对比测试");
    
    const char* test_text = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                           "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                           "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const char* test_pattern = "xyz";
    
    printf("测试数据: 主串长度=%d, 模式串=\"%s\"\n", 
           (int)strlen(test_text), test_pattern);
    
    // 测试各个算法
    StringSearchResult results[4];
    const char* algorithm_names[] = {"朴素匹配", "KMP", "Boyer-Moore", "Sunday"};
    
    results[0] = BruteForceSearch(test_text, test_pattern);
    results[1] = KMPSearch(test_text, test_pattern);
    results[2] = BoyerMooreSearch(test_text, test_pattern);
    results[3] = SundaySearch(test_text, test_pattern);
    
    // 打印结果
    printf("\n算法性能对比:\n");
    printf("%-12s %-8s %-10s %-10s %-10s\n", 
           "算法", "结果", "位置", "比较次数", "耗时(ms)");
    printf("-------------------------------------------------------\n");
    
    for (int i = 0; i < 4; i++) {
        printf("%-12s %-8s %-10d %-10d %-10.4f\n",
               algorithm_names[i],
               results[i].found ? "找到" : "未找到",
               results[i].position,
               results[i].comparisons,
               results[i].time_cost);
    }
    
    // 找出最快的算法
    int fastest = 0;
    for (int i = 1; i < 4; i++) {
        if (results[i].time_cost < results[fastest].time_cost) {
            fastest = i;
        }
    }
    printf("\n最快算法: %s\n", algorithm_names[fastest]);
    
    print_test_result("性能对比测试", true);
}

void test_exam_demos(void) {
    print_test_header("数据结构真题演示");
    
    // KMP算法next数组计算演示
    printf("\n--- KMP算法next数组计算 ---\n");
    const char* pattern = "ababaca";
    int len = strlen(pattern);
    int* next = (int*)malloc(len * sizeof(int));
    
    printf("模式串: \"%s\"\n", pattern);
    BuildNextArray(pattern, next);
    
    printf("位置:  ");
    for (int j = 0; j < len; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    printf("字符:  ");
    for (int j = 0; j < len; j++) {
        printf("%2c ", pattern[j]);
    }
    printf("\n");
    
    printf("next:  ");
    for (int j = 0; j < len; j++) {
        printf("%2d ", next[j]);
    }
    printf("\n");
    
    free(next);
    
    // KMP算法匹配过程演示
    printf("\n--- KMP算法匹配过程 ---\n");
    const char* text = "ababcababa";
    const char* match_pattern = "ababa";
    
    printf("主串: \"%s\", 模式串: \"%s\"\n", text, match_pattern);
    
    StringSearchResult result = KMPSearch(text, match_pattern);
    printf("匹配结果: %s", result.found ? "找到" : "未找到");
    if (result.found) {
        printf(" (位置: %d)", result.position);
    }
    printf("\n比较次数: %d\n", result.comparisons);
    
    // 算法复杂度对比
    printf("\n--- 算法复杂度对比 ---\n");
    printf("%-15s %-15s %-15s\n", "算法", "时间复杂度", "空间复杂度");
    printf("-----------------------------------------------\n");
    printf("%-15s %-15s %-15s\n", "朴素匹配", "O(n*m)", "O(1)");
    printf("%-15s %-15s %-15s\n", "KMP", "O(n+m)", "O(m)");
    printf("%-15s %-15s %-15s\n", "Boyer-Moore", "O(n*m)", "O(σ+m)");
    printf("%-15s %-15s %-15s\n", "Sunday", "O(n*m)", "O(σ)");
    
    print_test_result("数据结构真题演示", true);
}

void interactive_menu() {
    int choice;
    
    while (1) {
        printf("\n========== 字符串搜索算法测试菜单 ==========\n");
        printf("1. 基本操作测试\n");
        printf("2. 算法正确性验证\n");
        printf("3. 性能对比测试\n");
        printf("4. 数据结构真题演示\n");
        printf("0. 退出程序\n");
        printf("==========================================\n");
        printf("请选择测试项目 (0-4): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字!\n");
            while (getchar() != '\n'); // 清除输入缓冲区
            continue;
        }
        
        switch (choice) {
            case 1:
                test_basic_operations();
                break;
            case 2:
                test_algorithm_correctness();
                break;
            case 3:
                test_performance_comparison();
                break;
            case 4:
                test_exam_demos();
                break;
            case 0:
                printf("感谢使用字符串搜索算法测试程序!\n");
                return;
            default:
                printf("无效选择，请重新输入!\n");
                break;
        }
        
        printf("\n按任意键继续...");
        getchar(); // 清除换行符
        getchar(); // 等待用户输入
    }
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                字符串搜索算法测试用例                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    test_basic_operations();
    test_algorithm_correctness();
    test_performance_comparison();
    test_exam_demos();
    
    printf("\n============================================================\n");
    printf("字符串搜索算法完整功能测试完成！\n");
    printf("============================================================\n");
    
    return 0;
} 