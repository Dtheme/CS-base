/* test_alu.c - ALU单元测试
 * 计算机组成原理模拟器 - ALU模块测试
 * create by: zw.duan
 */

#include "../../simulation/cpu_sim/alu.h"
#include "../../simulation/common/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * ALU单元测试程序
 * 
 * 本程序测试ALU的各项功能，包括：
 * 1. 基本算术运算（加减乘除）
 * 2. 逻辑运算（与或非异或）
 * 3. 移位运算（左移右移循环移位）
 * 4. 标志位设置和检查
 * 5. 408真题演示
 */

// 测试统计结构
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    char current_test_name[256];
} TestStats;

// 全局测试状态
static TestStats g_test_stats = {0};

#define TEST_START(name) do { \
    strcpy(g_test_stats.current_test_name, name); \
    printf("\n[测试] %s\n", name); \
    g_test_stats.total_tests++; \
} while(0)

#define TEST_ASSERT(condition, message) do { \
    if (condition) { \
        printf("  ✓ %s\n", message); \
    } else { \
        printf("  ✗ %s\n", message); \
        g_test_stats.failed_tests++; \
        return false; \
    } \
} while(0)

#define TEST_END() do { \
    g_test_stats.passed_tests++; \
    printf("  [通过] %s\n", g_test_stats.current_test_name); \
} while(0)

// 辅助函数：检查标志位
static bool check_flag(u8 flags, u8 flag) {
    return (flags & flag) != 0;
}

static bool is_zero(const alu_result_t *result) {
    return check_flag(result->flags, ALU_FLAG_ZERO);
}

static bool is_negative(const alu_result_t *result) {
    return check_flag(result->flags, ALU_FLAG_NEGATIVE);
}

static bool is_carry(const alu_result_t *result) {
    return check_flag(result->flags, ALU_FLAG_CARRY);
}

static bool is_overflow(const alu_result_t *result) {
    return check_flag(result->flags, ALU_FLAG_OVERFLOW);
}

/**
 * 测试ALU初始化和重置功能
 */
bool test_alu_initialization(void) {
    TEST_START("ALU初始化和重置测试");
    
    alu_t alu;
    
    // 测试初始化
    cpu_error_t result = alu_init(&alu);
    TEST_ASSERT(result == CPU_SUCCESS, "ALU初始化成功");
    TEST_ASSERT(alu.operation == ALU_ADD, "默认操作类型为ADD");
    TEST_ASSERT(alu.operand_a == 0, "操作数A初始化为0");
    TEST_ASSERT(alu.operand_b == 0, "操作数B初始化为0");
    TEST_ASSERT(alu.flags == 0, "标志位初始化为0");
    
    // 测试重置功能
    alu.operand_a = 0x12345678;
    alu.operand_b = 0x87654321;
    alu.operation = ALU_MUL;
    alu.flags = 0xFF;
    
    alu_reset(&alu);
    TEST_ASSERT(alu.operand_a == 0, "操作数A重置为0");
    TEST_ASSERT(alu.operand_b == 0, "操作数B重置为0");
    TEST_ASSERT(alu.operation == ALU_ADD, "操作类型重置为ADD");
    TEST_ASSERT(alu.flags == 0, "标志位重置为0");
    
    TEST_END();
    return true;
}

/**
 * 测试ALU执行和结果获取功能
 */
bool test_alu_execution(void) {
    TEST_START("ALU执行和结果获取测试");
    
    alu_t alu;
    alu_init(&alu);
    
    // 测试基本执行
    alu_result_t result = alu_execute(&alu, ALU_ADD, 100, 200);
    TEST_ASSERT(result.valid, "执行结果有效");
    TEST_ASSERT(result.result == 300, "加法结果正确");
    
    // 测试结果获取
    alu_result_t stored_result = alu_get_result(&alu);
    TEST_ASSERT(stored_result.result == result.result, "存储结果正确");
    TEST_ASSERT(stored_result.flags == result.flags, "标志位一致");
    
    TEST_END();
    return true;
}

/**
 * 测试加法运算
 */
bool test_addition_operations(void) {
    TEST_START("加法运算测试");
    
    // 基本加法测试
    alu_result_t result = alu_add(100, 200);
    TEST_ASSERT(result.result == 300, "基本加法：100 + 200 = 300");
    TEST_ASSERT(!is_zero(&result), "结果非零");
    TEST_ASSERT(!is_negative(&result), "结果为正");
    
    // 零结果测试
    result = alu_add(100, (word_t)(-100));
    TEST_ASSERT(result.result == 0, "零结果：100 + (-100) = 0");
    TEST_ASSERT(is_zero(&result), "零标志设置");
    
    // 进位测试
    result = alu_add(0xFFFFFFFF, 1);
    TEST_ASSERT(result.result == 0, "无符号溢出：0xFFFFFFFF + 1 = 0");
    TEST_ASSERT(is_carry(&result), "进位标志设置");
    TEST_ASSERT(is_zero(&result), "结果为零");
    
    TEST_END();
    return true;
}

/**
 * 测试减法运算
 */
bool test_subtraction_operations(void) {
    TEST_START("减法运算测试");
    
    // 基本减法测试
    alu_result_t result = alu_sub(300, 100);
    TEST_ASSERT(result.result == 200, "基本减法：300 - 100 = 200");
    TEST_ASSERT(!is_zero(&result), "结果非零");
    TEST_ASSERT(!is_negative(&result), "结果为正");
    
    // 零结果测试
    result = alu_sub(100, 100);
    TEST_ASSERT(result.result == 0, "零结果：100 - 100 = 0");
    TEST_ASSERT(is_zero(&result), "零标志设置");
    
    // 负数结果测试
    result = alu_sub(100, 200);
    TEST_ASSERT((i32)result.result == -100, "借位减法：100 - 200 = -100");
    TEST_ASSERT(is_negative(&result), "负数标志设置");
    
    TEST_END();
    return true;
}

/**
 * 测试乘法运算
 */
bool test_multiplication_operations(void) {
    TEST_START("乘法运算测试");
    
    // 基本乘法测试
    alu_result_t result = alu_mul(12, 13);
    TEST_ASSERT(result.result == 156, "基本乘法：12 × 13 = 156");
    TEST_ASSERT(!is_zero(&result), "结果非零");
    
    // 零乘法测试
    result = alu_mul(100, 0);
    TEST_ASSERT(result.result == 0, "零乘法：100 × 0 = 0");
    TEST_ASSERT(is_zero(&result), "零标志设置");
    
    TEST_END();
    return true;
}

/**
 * 测试除法运算
 */
bool test_division_operations(void) {
    TEST_START("除法运算测试");
    
    // 基本除法测试
    alu_result_t result = alu_div(156, 12);
    TEST_ASSERT(result.result == 13, "基本除法：156 ÷ 12 = 13");
    TEST_ASSERT(!is_zero(&result), "结果非零");
    TEST_ASSERT(result.valid, "结果有效");
    
    // 零结果测试
    result = alu_div(0, 100);
    TEST_ASSERT(result.result == 0, "零除法：0 ÷ 100 = 0");
    TEST_ASSERT(is_zero(&result), "零标志设置");
    
    TEST_END();
    return true;
}

/**
 * 测试逻辑运算
 */
bool test_logical_operations(void) {
    TEST_START("逻辑运算测试");
    
    // 与运算测试
    alu_result_t result = alu_and(0xFF00FF00, 0x0F0F0F0F);
    TEST_ASSERT(result.result == 0x0F000F00, "与运算：0xFF00FF00 & 0x0F0F0F0F");
    TEST_ASSERT(!is_zero(&result), "结果非零");
    
    // 或运算测试
    result = alu_or(0xFF00FF00, 0x0F0F0F0F);
    TEST_ASSERT(result.result == 0xFF0FFF0F, "或运算：0xFF00FF00 | 0x0F0F0F0F");
    
    // 异或运算测试
    result = alu_xor(0xFF00FF00, 0x0F0F0F0F);
    TEST_ASSERT(result.result == 0xF00FF00F, "异或运算：0xFF00FF00 ^ 0x0F0F0F0F");
    
    // 非运算测试
    result = alu_not(0x00000000);
    TEST_ASSERT(result.result == 0xFFFFFFFF, "非运算：~0x00000000 = 0xFFFFFFFF");
    
    TEST_END();
    return true;
}

/**
 * 测试移位运算
 */
bool test_shift_operations(void) {
    TEST_START("移位运算测试");
    
    // 左移测试
    alu_result_t result = alu_shl(0x12345678, 4);
    TEST_ASSERT(result.result == 0x23456780, "左移4位：0x12345678 << 4");
    
    // 右移测试
    result = alu_shr(0x12345678, 4);
    TEST_ASSERT(result.result == 0x01234567, "右移4位：0x12345678 >> 4");
    
    // 循环左移测试
    result = alu_rol(0x12345678, 4);
    TEST_ASSERT(result.result == 0x23456781, "循环左移4位：0x12345678 rol 4");
    
    // 循环右移测试
    result = alu_ror(0x12345678, 4);
    TEST_ASSERT(result.result == 0x81234567, "循环右移4位：0x12345678 ror 4");
    
    TEST_END();
    return true;
}

/**
 * 测试比较运算
 */
bool test_compare_operations(void) {
    TEST_START("比较运算测试");
    
    // 相等比较
    alu_result_t result = alu_cmp(100, 100);
    TEST_ASSERT(is_zero(&result), "相等比较：100 == 100");
    
    // 大于比较
    result = alu_cmp(200, 100);
    TEST_ASSERT(!is_zero(&result), "大于比较：200 > 100");
    TEST_ASSERT(!is_negative(&result), "大于结果为正");
    
    // 小于比较
    result = alu_cmp(100, 200);
    TEST_ASSERT(!is_zero(&result), "小于比较：100 < 200");
    TEST_ASSERT(is_negative(&result), "小于结果为负");
    
    TEST_END();
    return true;
}

/**
 * 408真题演示
 */
void demo_408_exam_questions(void) {
    printf("\n=== 408真题演示 ===\n");
    
    // 演示ALU基本运算
    struct {
        alu_op_t op;
        word_t a, b;
        const char* desc;
    } test_cases[] = {
        {ALU_ADD, 0x7FFFFFFF, 1, "有符号数溢出"},
        {ALU_SUB, 0x80000000, 1, "有符号数下溢"},
        {ALU_AND, 0xAAAAAAAA, 0x55555555, "位运算模式"},
        {ALU_SHL, 0x12345678, 4, "移位运算"},
        {ALU_CMP, 100, 200, "比较运算"}
    };
    
    alu_t alu;
    alu_init(&alu);
    
    for (int i = 0; i < 5; i++) {
        alu_result_t result = alu_execute(&alu, test_cases[i].op, test_cases[i].a, test_cases[i].b);
        printf("  %s: 0x%08X %s 0x%08X = 0x%08X [标志: %02X]\n",
               test_cases[i].desc,
               test_cases[i].a,
               alu_op_to_string(test_cases[i].op),
               test_cases[i].b,
               result.result,
               result.flags);
    }
}

/**
 * 运行所有ALU测试
 */
void run_all_alu_tests(void) {
    printf("=== ALU单元测试开始 ===\n");
    
    // 重置测试统计
    g_test_stats.total_tests = 0;
    g_test_stats.passed_tests = 0;
    g_test_stats.failed_tests = 0;
    
    // 运行所有测试
    test_alu_initialization();
    test_alu_execution();
    test_addition_operations();
    test_subtraction_operations();
    test_multiplication_operations();
    test_division_operations();
    test_logical_operations();
    test_shift_operations();
    test_compare_operations();
    
    // 显示测试结果
    printf("\n=== ALU测试结果统计 ===\n");
    printf("总测试数: %d\n", g_test_stats.total_tests);
    printf("通过测试: %d\n", g_test_stats.passed_tests);
    printf("失败测试: %d\n", g_test_stats.failed_tests);
    printf("成功率: %.1f%%\n", 
           g_test_stats.total_tests > 0 ? 
           (float)g_test_stats.passed_tests / g_test_stats.total_tests * 100 : 0);
    
    if (g_test_stats.failed_tests == 0) {
        printf("🎉 所有ALU测试通过！\n");
    } else {
        printf("❌ 有 %d 个测试失败\n", g_test_stats.failed_tests);
    }
}

/**
 * 主函数
 */
int main(void) {
    printf("计算机组成原理模拟器 - ALU单元测试\n");
    printf("========================================\n");
    
    // 运行所有测试
    run_all_alu_tests();
    
    // 运行408演示
    demo_408_exam_questions();
    
    return g_test_stats.failed_tests == 0 ? 0 : 1;
} 