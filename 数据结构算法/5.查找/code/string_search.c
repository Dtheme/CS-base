/* string_search.c - 字符串查找算法完整实现
 * create by: zw.duan
 * 
 * 实现数据结构中的字符串模式匹配算法：
 * 1. 朴素模式匹配算法（Brute Force）
 * 2. KMP算法（Knuth-Morris-Pratt）
 * 3. Boyer-Moore算法
 * 4. Sunday算法
 */

#include "string_search.h"

// ============= 工具函数实现 =============

/**
 * 获取当前时间（毫秒）
 */
double GetCurrentTimeMs(void) {
    return (double)clock() / CLOCKS_PER_SEC * 1000.0;
}

/**
 * 生成随机字符串
 */
void GenerateRandomString(char* str, int length, const char* charset) {
    int charset_len = strlen(charset);
    for (int i = 0; i < length; i++) {
        str[i] = charset[rand() % charset_len];
    }
    str[length] = '\0';
}

/**
 * 验证查找结果的正确性
 */
bool VerifySearchResult(const char* text, const char* pattern, 
                       StringSearchResult result) {
    if (!result.found) {
        // 检查是否真的不存在
        return strstr(text, pattern) == NULL;
    } else {
        // 检查位置是否正确
        int text_len = strlen(text);
        int pattern_len = strlen(pattern);
        
        if (result.position < 0 || result.position > text_len - pattern_len) {
            return false;
        }
        
        return strncmp(text + result.position, pattern, pattern_len) == 0;
    }
}

/**
 * 打印查找结果
 */
void PrintSearchResult(StringSearchResult result, const char* algorithm_name, 
                      const char* pattern) {
    printf("\n--- %s 查找结果 ---\n", algorithm_name);
    printf("模式串: \"%s\"\n", pattern);
    printf("查找结果: %s\n", result.found ? " 找到" : " 未找到");
    if (result.found) {
        printf("位置: %d\n", result.position);
    }
    printf("比较次数: %d\n", result.comparisons);
    printf("耗时: %.4f ms\n", result.time_cost);
}

// ============= 朴素模式匹配算法实现 =============

/**
 *  朴素模式匹配算法（Brute Force）- 数据结构基础算法
 * 
 *   算法过程演示：
 * 主串: "ababcababa"
 * 模式: "ababa"
 * 
 * 第1次匹配：
 * ababcababa
 * ababa      ✗ (第5个字符不匹配)
 * 
 * 第2次匹配：
 * ababcababa
 *  ababa     ✗ (第1个字符不匹配)
 * 
 * 第3次匹配：
 * ababcababa
 *   ababa    ✗ (第1个字符不匹配)
 * 
 * 第4次匹配：
 * ababcababa
 *    ababa   ✗ (第1个字符不匹配)
 * 
 * 第5次匹配：
 * ababcababa
 *     ababa  ✗ (第1个字符不匹配)
 * 
 * 第6次匹配：
 * ababcababa
 *      ababa ✓ (完全匹配)
 * 
 *  数据结构要点：
 * - 理解回退机制的低效性
 * - 掌握最坏情况的时间复杂度分析
 * - 了解与KMP算法的对比
 */
StringSearchResult BruteForceSearch(const char* text, const char* pattern) {
    StringSearchResult result = {false, -1, 0, 0.0};
    
    double start_time = GetCurrentTimeMs();
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    if (pattern_len == 0) {
        result.found = true;
        result.position = 0;
        result.time_cost = GetCurrentTimeMs() - start_time;
        return result;
    }
    
    // 朴素匹配过程
    for (int i = 0; i <= text_len - pattern_len; i++) {
        int j = 0;
        
        // 逐个字符比较
        while (j < pattern_len) {
            result.comparisons++;
            if (text[i + j] != pattern[j]) {
                break;  // 不匹配，跳出内层循环
            }
            j++;
        }
        
        // 检查是否完全匹配
        if (j == pattern_len) {
            result.found = true;
            result.position = i;
            break;
        }
    }
    
    result.time_cost = GetCurrentTimeMs() - start_time;
    return result;
}

// ============= KMP算法实现 =============

/**
 * 构建KMP算法的next数组
 * 
 *   next数组计算过程演示（模式串 "ababaca"）：
 * 
 * j:     0  1  2  3  4  5  6
 * 模式:  a  b  a  b  a  c  a
 * next: -1  0  0  1  2  0  1
 * 
 * 计算过程：
 * - next[0] = -1 (约定)
 * - next[1] = 0  (单字符无前后缀)
 * - next[2] = 0  ("ab"无相同前后缀)
 * - next[3] = 1  ("aba"有相同前后缀"a")
 * - next[4] = 2  ("abab"有相同前后缀"ab")
 * - next[5] = 0  ("ababa"最长相同前后缀"aba"，但下一字符相同，需要继续)
 * - next[6] = 1  ("ababac"有相同前后缀"a")
 */
void BuildNextArray(const char* pattern, int* next) {
    int pattern_len = strlen(pattern);
    
    if (pattern_len == 0) return;
    
    next[0] = -1;  // 约定第一个字符的next值为-1
    
    if (pattern_len == 1) return;
    
    next[1] = 0;   // 第二个字符的next值为0
    
    int i = 2;     // 当前计算位置
    int j = 0;     // 前缀末尾位置
    
    while (i < pattern_len) {
        if (pattern[i-1] == pattern[j]) {
            // 前缀和后缀匹配，next值为j+1
            next[i] = j + 1;
            i++;
            j++;
        } else if (j > 0) {
            // 不匹配，回退j
            j = next[j];
        } else {
            // j已经为0，无法回退
            next[i] = 0;
            i++;
        }
    }
}

/**
 * 构建KMP算法的改进next数组（nextval）
 * 
 *   nextval数组优化原理：
 * 当pattern[j] == pattern[next[j]]时，
 * 可以直接使用nextval[next[j]]，避免重复比较
 */
void BuildNextvalArray(const char* pattern, int* nextval) {
    int pattern_len = strlen(pattern);
    int* next = (int*)malloc(pattern_len * sizeof(int));
    
    // 先计算普通的next数组
    BuildNextArray(pattern, next);
    
    // 计算改进的nextval数组
    nextval[0] = -1;
    
    for (int i = 1; i < pattern_len; i++) {
        if (next[i] == 0) {
            nextval[i] = 0;
        } else if (pattern[i] == pattern[next[i]]) {
            nextval[i] = nextval[next[i]];
        } else {
            nextval[i] = next[i];
        }
    }
    
    free(next);
}

/**
 *  KMP算法（Knuth-Morris-Pratt）- 数据结构重点算法
 * 
 *   算法过程演示：
 * 主串: "ababcababa"
 * 模式: "ababa"
 * next: [-1, 0, 0, 1, 2]
 * 
 * 第1次匹配：
 * ababcababa
 * ababa      ✗ (第5个字符'c'不匹配'a')
 * 根据next[4]=2，模式串向右移动2位
 * 
 * 第2次匹配：
 * ababcababa
 *    aba     ✗ (第4个字符'c'不匹配'b')
 * 根据next[2]=0，模式串向右移动到下一位置
 * 
 * 第3次匹配：
 * ababcababa
 *     ababa  ✓ (完全匹配)
 * 
 *  数据结构要点：
 * - 理解next数组的含义和计算方法
 * - 掌握KMP算法的匹配过程
 * - 了解时间复杂度为O(n+m)的原因
 */
StringSearchResult KMPSearch(const char* text, const char* pattern) {
    StringSearchResult result = {false, -1, 0, 0.0};
    
    double start_time = GetCurrentTimeMs();
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    if (pattern_len == 0) {
        result.found = true;
        result.position = 0;
        result.time_cost = GetCurrentTimeMs() - start_time;
        return result;
    }
    
    // 构建next数组
    int* next = (int*)malloc(pattern_len * sizeof(int));
    BuildNextArray(pattern, next);
    
    int i = 0;  // 主串指针
    int j = 0;  // 模式串指针
    
    while (i < text_len && j < pattern_len) {
        result.comparisons++;
        
        if (j == -1 || text[i] == pattern[j]) {
            // 匹配成功，两指针都前进
            i++;
            j++;
        } else {
            // 匹配失败，利用next数组跳跃
            j = next[j];
        }
    }
    
    if (j == pattern_len) {
        // 找到匹配
        result.found = true;
        result.position = i - pattern_len;
    }
    
    free(next);
    result.time_cost = GetCurrentTimeMs() - start_time;
    return result;
}

// ============= Boyer-Moore算法实现 =============

/**
 * 构建Boyer-Moore算法的坏字符表
 */
void BuildBadCharTable(const char* pattern, int* bad_char) {
    int pattern_len = strlen(pattern);
    
    // 初始化坏字符表，所有字符的位移都是模式串长度
    for (int i = 0; i < 256; i++) {
        bad_char[i] = pattern_len;
    }
    
    // 计算模式串中每个字符的最右出现位置
    for (int i = 0; i < pattern_len - 1; i++) {
        bad_char[(unsigned char)pattern[i]] = pattern_len - 1 - i;
    }
}

/**
 *  Boyer-Moore算法 - 数据结构扩展算法
 * 
 *   算法特点：
 * - 从右向左比较字符
 * - 利用坏字符规则进行跳跃
 * - 平均情况下性能优异
 */
StringSearchResult BoyerMooreSearch(const char* text, const char* pattern) {
    StringSearchResult result = {false, -1, 0, 0.0};
    
    double start_time = GetCurrentTimeMs();
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    if (pattern_len == 0) {
        result.found = true;
        result.position = 0;
        result.time_cost = GetCurrentTimeMs() - start_time;
        return result;
    }
    
    // 构建坏字符表
    int bad_char[256];
    BuildBadCharTable(pattern, bad_char);
    
    int i = pattern_len - 1;  // 主串指针，从模式串长度-1开始
    
    while (i < text_len) {
        int j = pattern_len - 1;  // 模式串指针，从末尾开始
        int k = i;                // 主串比较位置
        
        // 从右向左比较
        while (j >= 0 && text[k] == pattern[j]) {
            result.comparisons++;
            j--;
            k--;
        }
        
        if (j < 0) {
            // 找到匹配
            result.found = true;
            result.position = k + 1;
            break;
        } else {
            // 不匹配，使用坏字符规则跳跃
            result.comparisons++;
            int shift = bad_char[(unsigned char)text[k]];
            i += shift;
        }
    }
    
    result.time_cost = GetCurrentTimeMs() - start_time;
    return result;
}

// ============= Sunday算法实现 =============

/**
 * 构建Sunday算法的位移表
 */
void BuildShiftTable(const char* pattern, int* shift) {
    int pattern_len = strlen(pattern);
    
    // 初始化位移表
    for (int i = 0; i < 256; i++) {
        shift[i] = pattern_len + 1;
    }
    
    // 计算模式串中每个字符的位移值
    for (int i = 0; i < pattern_len; i++) {
        shift[(unsigned char)pattern[i]] = pattern_len - i;
    }
}

/**
 *  Sunday算法 - 数据结构扩展算法
 * 
 *   算法特点：
 * - Boyer-Moore算法的简化版
 * - 只考虑坏字符规则
 * - 实现简单，性能良好
 */
StringSearchResult SundaySearch(const char* text, const char* pattern) {
    StringSearchResult result = {false, -1, 0, 0.0};
    
    double start_time = GetCurrentTimeMs();
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    if (pattern_len == 0) {
        result.found = true;
        result.position = 0;
        result.time_cost = GetCurrentTimeMs() - start_time;
        return result;
    }
    
    // 构建位移表
    int shift[256];
    BuildShiftTable(pattern, shift);
    
    int i = 0;  // 主串指针
    
    while (i <= text_len - pattern_len) {
        int j = 0;  // 模式串指针
        
        // 从左向右比较
        while (j < pattern_len && text[i + j] == pattern[j]) {
            result.comparisons++;
            j++;
        }
        
        if (j == pattern_len) {
            // 找到匹配
            result.found = true;
            result.position = i;
            break;
        } else {
            // 不匹配，使用Sunday规则跳跃
            if (j < pattern_len) {
                result.comparisons++;
            }
            
            if (i + pattern_len < text_len) {
                i += shift[(unsigned char)text[i + pattern_len]];
            } else {
                break;
            }
        }
    }
    
    result.time_cost = GetCurrentTimeMs() - start_time;
    return result;
}

// ============= 统计信息函数实现 =============

/**
 * 初始化统计信息
 */
void InitStringSearchStats(StringSearchStats* stats) {
    stats->total_searches = 0;
    stats->successful_searches = 0;
    stats->total_comparisons = 0;
    stats->total_time = 0.0;
    stats->avg_comparisons = 0.0;
    stats->avg_time = 0.0;
}

/**
 * 更新统计信息
 */
void UpdateStringSearchStats(StringSearchStats* stats, StringSearchResult result) {
    stats->total_searches++;
    if (result.found) {
        stats->successful_searches++;
    }
    stats->total_comparisons += result.comparisons;
    stats->total_time += result.time_cost;
    
    stats->avg_comparisons = (double)stats->total_comparisons / stats->total_searches;
    stats->avg_time = stats->total_time / stats->total_searches;
}

/**
 * 打印统计信息
 */
void PrintStringSearchStats(StringSearchStats* stats, const char* algorithm_name) {
    printf("\n%s 算法统计信息:\n", algorithm_name);
    printf("  总查找次数: %d\n", stats->total_searches);
    printf("  成功查找次数: %d\n", stats->successful_searches);
    printf("  成功率: %.2f%%\n", 
           stats->total_searches > 0 ? 
           (double)stats->successful_searches / stats->total_searches * 100 : 0);
    printf("  总比较次数: %d\n", stats->total_comparisons);
    printf("  平均比较次数: %.2f\n", stats->avg_comparisons);
    printf("  总耗时: %.4f ms\n", stats->total_time);
    printf("  平均耗时: %.4f ms\n", stats->avg_time);
}

// ============= 性能测试函数实现 =============

/**
 * 字符串查找算法性能比较
 */
void CompareStringSearchAlgorithms(const char* text, const char* pattern) {
    printf("\n  ========== 字符串查找算法性能比较 ==========\n");
    printf("主串长度: %zu\n", strlen(text));
    printf("模式串: \"%s\" (长度: %zu)\n", pattern, strlen(pattern));
    
    // 测试朴素算法
    StringSearchResult bf_result = BruteForceSearch(text, pattern);
    PrintSearchResult(bf_result, "朴素算法", pattern);
    
    // 测试KMP算法
    StringSearchResult kmp_result = KMPSearch(text, pattern);
    PrintSearchResult(kmp_result, "KMP算法", pattern);
    
    // 测试Boyer-Moore算法
    StringSearchResult bm_result = BoyerMooreSearch(text, pattern);
    PrintSearchResult(bm_result, "Boyer-Moore算法", pattern);
    
    // 测试Sunday算法
    StringSearchResult sunday_result = SundaySearch(text, pattern);
    PrintSearchResult(sunday_result, "Sunday算法", pattern);
    
    // 验证结果一致性
    printf("\n--- 结果验证 ---\n");
    bool all_correct = true;
    
    if (!VerifySearchResult(text, pattern, bf_result)) {
        printf(" 朴素算法结果错误\n");
        all_correct = false;
    }
    if (!VerifySearchResult(text, pattern, kmp_result)) {
        printf(" KMP算法结果错误\n");
        all_correct = false;
    }
    if (!VerifySearchResult(text, pattern, bm_result)) {
        printf(" Boyer-Moore算法结果错误\n");
        all_correct = false;
    }
    if (!VerifySearchResult(text, pattern, sunday_result)) {
        printf(" Sunday算法结果错误\n");
        all_correct = false;
    }
    
    if (all_correct) {
        printf(" 所有算法结果一致且正确\n");
    }
    
    // 性能比较
    printf("\n--- 性能比较 ---\n");
    printf("算法名称      | 比较次数 | 耗时(ms) | 效率提升\n");
    printf("--------------|----------|----------|----------\n");
    printf("朴素算法      | %8d | %8.4f | 基准\n", 
           bf_result.comparisons, bf_result.time_cost);
    printf("KMP算法       | %8d | %8.4f | %.2fx\n", 
           kmp_result.comparisons, kmp_result.time_cost,
           (double)bf_result.comparisons / kmp_result.comparisons);
    printf("Boyer-Moore   | %8d | %8.4f | %.2fx\n", 
           bm_result.comparisons, bm_result.time_cost,
           (double)bf_result.comparisons / bm_result.comparisons);
    printf("Sunday算法    | %8d | %8.4f | %.2fx\n", 
           sunday_result.comparisons, sunday_result.time_cost,
           (double)bf_result.comparisons / sunday_result.comparisons);
}

// ============= 批量性能测试实现 =============

/**
 * 批量性能测试
 */
void BatchPerformanceTest(int* text_lengths, int* pattern_lengths, int test_count) {
    printf("\n 字符串搜索算法批量性能测试\n");
    printf("==================================================\n");
    
    const char* charset = "abcdefghijklmnopqrstuvwxyz";
    
    for (int i = 0; i < test_count; i++) {
        printf("\n测试 %d: 主串长度=%d, 模式串长度=%d\n", 
               i+1, text_lengths[i], pattern_lengths[i]);
        
        // 生成测试数据
        char* text = (char*)malloc(text_lengths[i] + 1);
        char* pattern = (char*)malloc(pattern_lengths[i] + 1);
        
        GenerateRandomString(text, text_lengths[i], charset);
        GenerateRandomString(pattern, pattern_lengths[i], charset);
        
        // 确保模式串在主串中存在（50%概率）
        if (rand() % 2 == 0 && text_lengths[i] > pattern_lengths[i]) {
            int pos = rand() % (text_lengths[i] - pattern_lengths[i]);
            memcpy(text + pos, pattern, pattern_lengths[i]);
        }
        
        // 测试各个算法
        StringSearchResult results[4];
        const char* algorithm_names[] = {"朴素匹配", "KMP", "Boyer-Moore", "Sunday"};
        
        results[0] = BruteForceSearch(text, pattern);
        results[1] = KMPSearch(text, pattern);
        results[2] = BoyerMooreSearch(text, pattern);
        results[3] = SundaySearch(text, pattern);
        
        // 打印结果
        printf("%-12s %-8s %-10s %-10s %-10s\n", 
               "算法", "结果", "位置", "比较次数", "耗时(ms)");
        printf("-------------------------------------------------------\n");
        
        for (int j = 0; j < 4; j++) {
            printf("%-12s %-8s %-10d %-10d %-10.4f\n",
                   algorithm_names[j],
                   results[j].found ? "找到" : "未找到",
                   results[j].position,
                   results[j].comparisons,
                   results[j].time_cost);
        }
        
        free(text);
        free(pattern);
    }
} 