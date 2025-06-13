/* string_search.h - 字符串查找算法头文件
 * create by: zw.duan
 * 
 * 实现数据结构中的字符串模式匹配算法：
 * 1. 朴素模式匹配算法（Brute Force）
 * 2. KMP算法（Knuth-Morris-Pratt）
 * 3. Boyer-Moore算法
 * 4. Sunday算法
 */

#ifndef STRING_SEARCH_H
#define STRING_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// ============= 基本类型定义 =============

// 字符串查找结果结构
typedef struct {
    bool found;                  // 是否找到
    int position;                // 找到的位置（-1表示未找到）
    int comparisons;             // 比较次数
    double time_cost;            // 耗时（毫秒）
} StringSearchResult;

// 字符串匹配统计信息
typedef struct {
    int total_searches;          // 总查找次数
    int successful_searches;     // 成功查找次数
    int total_comparisons;       // 总比较次数
    double total_time;           // 总耗时
    double avg_comparisons;      // 平均比较次数
    double avg_time;             // 平均耗时
} StringSearchStats;

// ============= 朴素模式匹配算法 =============

/**
 *  朴素模式匹配算法（Brute Force）- 数据结构基础算法
 * 
 *   算法原理：
 * 从主串的第一个字符开始，与模式串逐个字符比较
 * 如果匹配失败，主串指针回退，模式串指针重置
 * 
 *  算法特点：
 * - 实现简单，容易理解
 * - 时间复杂度：O(n*m)，最坏情况
 * - 空间复杂度：O(1)
 * - 适用于短模式串的匹配
 * 
 * @param text 主串
 * @param pattern 模式串
 * @return StringSearchResult 查找结果
 */
StringSearchResult BruteForceSearch(const char* text, const char* pattern);

// ============= KMP算法 =============

/**
 *  KMP算法（Knuth-Morris-Pratt）- 数据结构重点算法
 * 
 *   算法原理：
 * 利用模式串的前缀和后缀信息，避免不必要的回退
 * 构建next数组（部分匹配表），指导匹配过程
 * 
 *  算法特点：
 * - 时间复杂度：O(n+m)，线性时间
 * - 空间复杂度：O(m)，存储next数组
 * - 主串指针不回退，效率高
 * - 适用于长模式串的匹配
 * 
 * @param text 主串
 * @param pattern 模式串
 * @return StringSearchResult 查找结果
 */
StringSearchResult KMPSearch(const char* text, const char* pattern);

/**
 * 构建KMP算法的next数组
 * 
 * @param pattern 模式串
 * @param next next数组
 */
void BuildNextArray(const char* pattern, int* next);

/**
 * 构建KMP算法的改进next数组（nextval）
 * 
 * @param pattern 模式串
 * @param nextval 改进的next数组
 */
void BuildNextvalArray(const char* pattern, int* nextval);

// ============= Boyer-Moore算法 =============

/**
 *  Boyer-Moore算法 - 数据结构扩展算法
 * 
 *   算法原理：
 * 从模式串的末尾开始比较
 * 利用坏字符规则和好后缀规则进行跳跃
 * 
 *  算法特点：
 * - 平均时间复杂度：O(n/m)，亚线性时间
 * - 最坏时间复杂度：O(n*m)
 * - 适用于大字符集和长模式串
 * - 预处理时间较长
 * 
 * @param text 主串
 * @param pattern 模式串
 * @return StringSearchResult 查找结果
 */
StringSearchResult BoyerMooreSearch(const char* text, const char* pattern);

/**
 * 构建Boyer-Moore算法的坏字符表
 * 
 * @param pattern 模式串
 * @param bad_char 坏字符表
 */
void BuildBadCharTable(const char* pattern, int* bad_char);

// ============= Sunday算法 =============

/**
 *  Sunday算法 - 数据结构扩展算法
 * 
 *   算法原理：
 * Boyer-Moore算法的简化版本
 * 只使用坏字符规则，但考虑模式串后一个字符
 * 
 *  算法特点：
 * - 实现简单，预处理快
 * - 平均性能优于朴素算法
 * - 适用于一般文本搜索
 * 
 * @param text 主串
 * @param pattern 模式串
 * @return StringSearchResult 查找结果
 */
StringSearchResult SundaySearch(const char* text, const char* pattern);

/**
 * 构建Sunday算法的位移表
 * 
 * @param pattern 模式串
 * @param shift 位移表
 */
void BuildShiftTable(const char* pattern, int* shift);

// ============= 多模式匹配算法 =============

/**
 *  AC自动机算法（Aho-Corasick）- 数据结构了解算法
 * 
 *   算法原理：
 * 基于Trie树和KMP算法的失效函数
 * 可以同时匹配多个模式串
 * 
 * @param text 主串
 * @param patterns 模式串数组
 * @param pattern_count 模式串数量
 * @param results 查找结果数组
 * @return int 找到的模式串数量
 */
int ACSearch(const char* text, const char** patterns, int pattern_count, 
             StringSearchResult* results);

// ============= 工具函数 =============

/**
 * 获取当前时间（毫秒）
 */
double GetCurrentTimeMs(void);

/**
 * 生成随机字符串
 * 
 * @param str 输出字符串
 * @param length 字符串长度
 * @param charset 字符集
 */
void GenerateRandomString(char* str, int length, const char* charset);

/**
 * 验证查找结果的正确性
 * 
 * @param text 主串
 * @param pattern 模式串
 * @param result 查找结果
 * @return bool 结果是否正确
 */
bool VerifySearchResult(const char* text, const char* pattern, 
                       StringSearchResult result);

/**
 * 打印查找结果
 * 
 * @param result 查找结果
 * @param algorithm_name 算法名称
 * @param pattern 模式串
 */
void PrintSearchResult(StringSearchResult result, const char* algorithm_name, 
                      const char* pattern);

/**
 * 初始化统计信息
 * 
 * @param stats 统计信息结构
 */
void InitStringSearchStats(StringSearchStats* stats);

/**
 * 更新统计信息
 * 
 * @param stats 统计信息结构
 * @param result 查找结果
 */
void UpdateStringSearchStats(StringSearchStats* stats, StringSearchResult result);

/**
 * 打印统计信息
 * 
 * @param stats 统计信息结构
 * @param algorithm_name 算法名称
 */
void PrintStringSearchStats(StringSearchStats* stats, const char* algorithm_name);

// ============= 性能测试函数 =============

/**
 * 字符串查找算法性能比较
 * 
 * @param text 主串
 * @param pattern 模式串
 */
void CompareStringSearchAlgorithms(const char* text, const char* pattern);

/**
 * 批量性能测试
 * 
 * @param text_lengths 主串长度数组
 * @param pattern_lengths 模式串长度数组
 * @param test_count 测试数量
 */
void BatchPerformanceTest(int* text_lengths, int* pattern_lengths, int test_count);

// ============= 数据结构真题演示 =============

/**
 * KMP算法next数组计算演示
 */
void ExamDemo_KMPNextArray(void);

/**
 * KMP算法匹配过程演示
 */
void ExamDemo_KMPMatching(void);

/**
 * 字符串匹配算法比较演示
 */
void ExamDemo_StringSearchComparison(void);

/**
 * 字符串查找综合演示
 */
void StringSearchDemo(void);

#endif // STRING_SEARCH_H 