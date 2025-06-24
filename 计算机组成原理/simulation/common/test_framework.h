/* test_framework.h - 测试框架头文件
 * 计算机组成原理模拟器 - 公共测试框架
 * 遵循数据结构算法编码标准
 * create by: zw.duan
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"  // 包含CPU_SUCCESS等错误码定义

/**
 * 单元测试框架
 * 
 * 功能特性：
 * 1. 层次化测试组织 - 支持测试套件和测试用例的两级结构
 * 2. 自动化结果统计 - 实时统计通过/失败测试数量
 * 3. 彩色终端输出 - 增强可读性的颜色标识
 * 4. 多种断言类型 - 支持布尔、数值、字符串等断言
 * 5. 详细错误报告 - 提供文件名、行号、期望值等详细信息
 * 
 * 设计模式：
 * - 宏封装：简化测试代码编写
 * - 状态管理：维护全局测试统计信息
 * - 错误聚合：集中收集和报告测试失败
 */

// ==================== 颜色定义 ====================

/**
 * ANSI颜色码定义
 * 
 * 用途：在支持ANSI转义序列的终端中显示彩色输出
 * 格式：\x1b[颜色码m
 * 
 * 颜色映射：
 * - RED (31): 错误和失败信息
 * - GREEN (32): 成功和通过信息
 * - YELLOW (33): 警告和运行状态
 * - BLUE (34): 一般信息
 * - CYAN (36): 标题和分隔符
 * - RESET (0): 重置为默认颜色
 */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// ==================== 测试统计结构 ====================

/**
 * 测试统计信息结构体
 * 
 * 数据组织：
 * - 全局统计：total_tests, passed_tests, failed_tests
 * - 用例统计：current_case_tests, current_case_passed
 * - 状态信息：current_case_name, in_test_case
 * 
 * 生命周期：
 * 1. 初始化：test_init() 清零所有统计
 * 2. 用例开始：test_start_case() 重置用例统计
 * 3. 断言执行：test_pass()/test_fail() 更新计数
 * 4. 用例结束：test_end_case() 汇总用例结果
 * 5. 全局汇总：test_summary() 输出最终统计
 */
typedef struct {
    int total_tests;           // 总测试数量
    int passed_tests;          // 通过测试数量
    int failed_tests;          // 失败测试数量
    int current_case_tests;    // 当前用例测试数量
    int current_case_passed;   // 当前用例通过数量
    char current_case_name[256]; // 当前用例名称
    bool in_test_case;         // 是否在测试用例中
} test_stats_t;

// ==================== 测试宏定义 ====================

/**
 * 测试用例开始宏
 * 
 * 功能：
 * 1. 输出带颜色的用例标题
 * 2. 初始化用例级别的统计信息
 * 3. 设置当前用例名称和状态
 * 
 * 实现原理：
 * - 使用do-while(0)确保宏的语法完整性
 * - ANSI颜色码美化输出格式
 * - 调用test_start_case()进行状态管理
 */
#define TEST_START(name) \
    do { \
        printf(ANSI_COLOR_CYAN "\n=== %s ===" ANSI_COLOR_RESET "\n", name); \
        test_start_case(name); \
    } while(0)

/**
 * 测试用例结束宏
 * 
 * 功能：
 * 1. 结束当前测试用例
 * 2. 汇总用例执行结果
 * 3. 返回用例成功状态
 * 
 * 返回值：
 * - true: 用例中所有测试都通过
 * - false: 用例中存在失败的测试
 */
#define TEST_END() \
    do { \
        return test_end_case(); \
    } while(0)

/**
 * 基础断言宏
 * 
 * 断言机制：
 * 1. 条件评估：检查给定条件是否为真
 * 2. 失败处理：条件为假时输出错误信息并记录失败
 * 3. 成功记录：条件为真时静默记录成功
 * 
 * 错误信息包含：
 * - 失败标识：FAIL 标记
 * - 错误描述：用户提供的message
 * - 行号信息：便于定位问题
 * 
 * 技术细节：
 * - 使用!(condition)避免副作用
 * - __LINE__宏提供源码行号
 * - 颜色编码突出显示失败信息
 */
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf(ANSI_COLOR_RED "[FAIL] %s (line %d)" ANSI_COLOR_RESET "\n", \
                   message, __LINE__); \
            test_fail(message, __FILE__, __LINE__); \
        } else { \
            test_pass(); \
        } \
    } while(0)

/**
 * 数值相等断言宏
 * 
 * 比较机制：
 * 1. 值比较：使用==操作符比较expected和actual
 * 2. 详细报告：失败时显示期望值和实际值
 * 3. 类型安全：通过(int)强制转换统一输出格式
 * 
 * 错误信息格式：
 * [FAIL] message (expected: 期望值, actual: 实际值, line: 行号)
 * 
 * 适用场景：
 * - 函数返回值检查
 * - 变量状态验证
 * - 计算结果校验
 */
#define TEST_ASSERT_EQUAL(expected, actual, message) \
    do { \
        if ((expected) != (actual)) { \
            printf(ANSI_COLOR_RED "[FAIL] %s (expected: %d, actual: %d, line %d)" ANSI_COLOR_RESET "\n", \
                   message, (int)(expected), (int)(actual), __LINE__); \
            test_fail(message, __FILE__, __LINE__); \
        } else { \
            test_pass(); \
        } \
    } while(0)

/**
 * 字符串相等断言宏
 * 
 * 字符串比较：
 * 1. 使用strcmp进行字符串内容比较
 * 2. 返回值0表示字符串相等
 * 3. 失败时显示期望字符串和实际字符串
 * 
 * 错误处理：
 * - 空指针保护：strcmp会处理NULL指针
 * - 完整内容显示：便于识别差异
 * - 引号包围：明确字符串边界
 * 
 * 应用场景：
 * - 字符串处理函数测试
 * - 配置解析验证
 * - 文本输出检查
 */
#define TEST_ASSERT_STRING_EQUAL(expected, actual, message) \
    do { \
        if (strcmp((expected), (actual)) != 0) { \
            printf(ANSI_COLOR_RED "[FAIL] %s (expected: \"%s\", actual: \"%s\", line %d)" ANSI_COLOR_RESET "\n", \
                   message, expected, actual, __LINE__); \
            test_fail(message, __FILE__, __LINE__); \
        } else { \
            test_pass(); \
        } \
    } while(0)

/**
 * 空指针断言宏
 * 
 * 实现：调用基础断言宏检查指针是否为NULL
 * 用途：验证函数返回空指针的正确性
 */
#define TEST_ASSERT_NULL(ptr, message) \
    TEST_ASSERT((ptr) == NULL, message)

/**
 * 非空指针断言宏
 * 
 * 实现：调用基础断言宏检查指针是否非NULL
 * 用途：验证资源分配和对象创建的成功性
 */
#define TEST_ASSERT_NOT_NULL(ptr, message) \
    TEST_ASSERT((ptr) != NULL, message)

/**
 * 成功状态断言宏
 * 
 * 实现：检查表达式是否等于CPU_SUCCESS
 * 用途：验证函数调用是否成功执行
 */
#define TEST_ASSERT_SUCCESS(expr, message) \
    TEST_ASSERT((expr) == CPU_SUCCESS, message)

/**
 * 错误状态断言宏
 * 
 * 实现：检查表达式是否不等于CPU_SUCCESS
 * 用途：验证函数调用是否正确返回错误
 */
#define TEST_ASSERT_ERROR(expr, message) \
    TEST_ASSERT((expr) != CPU_SUCCESS, message)

/**
 * 布尔真值断言宏
 * 
 * 实现：检查表达式是否为真值（非零）
 * 用途：验证条件或表达式为真
 */
#define TEST_ASSERT_TRUE(expr, message) \
    TEST_ASSERT((expr) != 0, message)

/**
 * 布尔假值断言宏
 * 
 * 实现：检查表达式是否为假值（零）
 * 用途：验证条件或表达式为假
 */
#define TEST_ASSERT_FALSE(expr, message) \
    TEST_ASSERT((expr) == 0, message)

// ==================== 函数声明 ====================

/**
 * 初始化测试套件
 * 
 * 参数：suite_name - 测试套件名称
 * 功能：清零全局统计信息，设置套件上下文
 */
void test_init(const char *suite_name);

/**
 * 开始测试用例
 * 
 * 参数：case_name - 测试用例名称
 * 功能：重置用例级统计，设置当前用例状态
 */
void test_start_case(const char *case_name);

/**
 * 结束测试用例
 * 
 * 返回值：bool - 用例是否全部通过
 * 功能：汇总用例结果，更新全局统计
 */
bool test_end_case(void);

/**
 * 记录测试通过
 * 
 * 功能：增加通过计数器
 */
void test_pass(void);

/**
 * 记录测试失败
 * 
 * 参数：
 * - message: 失败描述
 * - file: 源文件名
 * - line: 源代码行号
 * 功能：增加失败计数器，记录错误详情
 */
void test_fail(const char *message, const char *file, int line);

/**
 * 输出测试总结
 * 
 * 功能：显示全局测试统计和最终结果
 */
void test_summary(void);

/**
 * 获取程序退出码
 * 
 * 返回值：
 * - 0: 所有测试通过
 * - 1: 存在失败测试
 */
int test_get_exit_code(void);

/**
 * 重置测试统计
 * 
 * 功能：清零所有统计信息，用于重新开始测试
 */
void test_reset_stats(void);

// ==================== 辅助宏 ====================

/**
 * 运行测试函数宏
 * 
 * 功能：
 * 1. 输出测试函数名称
 * 2. 调用指定的测试函数
 * 3. 提供统一的测试执行格式
 * 
 * 使用方式：RUN_TEST(my_test_function);
 */
#define RUN_TEST(test_func) \
    do { \
        printf(ANSI_COLOR_YELLOW "Running %s..." ANSI_COLOR_RESET "\n", #test_func); \
        test_func(); \
    } while(0)

/**
 * 期望断言宏系列
 * 
 * 设计目标：提供更语义化的断言接口
 * 命名规范：EXPECT_* 表示期望某种条件成立
 * 
 * 优点：
 * - 代码可读性更强
 * - 减少重复的message参数
 * - 自动生成有意义的错误描述
 */
#define EXPECT_TRUE(condition) TEST_ASSERT(condition, #condition " should be true")
#define EXPECT_FALSE(condition) TEST_ASSERT(!(condition), #condition " should be false")
#define EXPECT_EQ(expected, actual) TEST_ASSERT_EQUAL(expected, actual, #actual " should equal " #expected)
#define EXPECT_NE(expected, actual) TEST_ASSERT((expected) != (actual), #actual " should not equal " #expected)
#define EXPECT_NULL(ptr) TEST_ASSERT_NULL(ptr, #ptr " should be NULL")
#define EXPECT_NOT_NULL(ptr) TEST_ASSERT_NOT_NULL(ptr, #ptr " should not be NULL")

#endif // TEST_FRAMEWORK_H 