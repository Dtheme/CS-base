/**
 * @file test_performance.c
 * @brief 性能基准测试 - 测试各组件的性能表现
 * @author zw.duan
 * @date 2024-06-16
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "../../simulation/common/utils.h"
#include "../../simulation/common/debug.h"
#include "../../simulation/common/test_framework.h"
#include "../../simulation/cpu_sim/alu.h"

#define BENCHMARK_ITERATIONS 1000000

/**
 * @brief 测试ALU运算性能
 */
int test_alu_performance(void) {
    printf("🧪 运行ALU性能测试...\n");
    
    alu_t alu;
    alu_init(&alu);
    
    clock_t start, end;
    double cpu_time_used;
    
    // 测试加法性能
    start = clock();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        alu_add(i, i + 1);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  加法性能: %d 次操作耗时 %.6f 秒\n", BENCHMARK_ITERATIONS, cpu_time_used);
    printf("  平均每次操作: %.2f 纳秒\n", (cpu_time_used * 1000000000) / BENCHMARK_ITERATIONS);
    
    // 测试乘法性能
    start = clock();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        alu_mul(i & 0xFFFF, (i + 1) & 0xFFFF);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  乘法性能: %d 次操作耗时 %.6f 秒\n", BENCHMARK_ITERATIONS, cpu_time_used);
    printf("  平均每次操作: %.2f 纳秒\n", (cpu_time_used * 1000000000) / BENCHMARK_ITERATIONS);
    
    // 测试逻辑运算性能
    start = clock();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        alu_and(i, i ^ 0xAAAAAAAA);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  逻辑与性能: %d 次操作耗时 %.6f 秒\n", BENCHMARK_ITERATIONS, cpu_time_used);
    printf("  平均每次操作: %.2f 纳秒\n", (cpu_time_used * 1000000000) / BENCHMARK_ITERATIONS);
    
    printf("✓ ALU性能测试完成\n");
    return 1;
}

/**
 * @brief 测试内存访问性能
 */
int test_memory_performance(void) {
    printf("🧪 运行内存访问性能测试...\n");
    
    const int array_size = 1024 * 1024;  // 1MB
    uint32_t *test_array = malloc(array_size * sizeof(uint32_t));
    
    if (!test_array) {
        printf("❌ 内存分配失败\n");
        return 0;
    }
    
    clock_t start, end;
    double cpu_time_used;
    
    // 测试顺序写入性能
    start = clock();
    for (int i = 0; i < array_size; i++) {
        test_array[i] = i;
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  顺序写入性能: %d 次写入耗时 %.6f 秒\n", array_size, cpu_time_used);
    printf("  写入带宽: %.2f MB/s\n", (array_size * sizeof(uint32_t)) / (cpu_time_used * 1024 * 1024));
    
    // 测试顺序读取性能
    volatile uint32_t sum = 0;  // 防止编译器优化
    start = clock();
    for (int i = 0; i < array_size; i++) {
        sum += test_array[i];
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  顺序读取性能: %d 次读取耗时 %.6f 秒\n", array_size, cpu_time_used);
    printf("  读取带宽: %.2f MB/s\n", (array_size * sizeof(uint32_t)) / (cpu_time_used * 1024 * 1024));
    
    // 测试随机访问性能
    start = clock();
    for (int i = 0; i < BENCHMARK_ITERATIONS / 10; i++) {
        int index = rand() % array_size;
        sum += test_array[index];
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("  随机访问性能: %d 次访问耗时 %.6f 秒\n", BENCHMARK_ITERATIONS / 10, cpu_time_used);
    printf("  平均每次访问: %.2f 纳秒\n", (cpu_time_used * 1000000000) / (BENCHMARK_ITERATIONS / 10));
    
    free(test_array);
    printf("✓ 内存访问性能测试完成\n");
    return 1;
}

/**
 * @brief 主测试函数
 */
int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   性能基准测试套件                          ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // 初始化随机数种子
    srand((unsigned int)time(NULL));
    
    int total_tests = 0;
    int passed_tests = 0;
    
    printf("⚡ 开始性能基准测试...\n");
    printf("📏 基准操作次数: %d\n", BENCHMARK_ITERATIONS);
    printf("\n");
    
    // 运行测试
    total_tests++;
    if (test_alu_performance()) {
        passed_tests++;
    }
    
    printf("\n");
    
    total_tests++;
    if (test_memory_performance()) {
        passed_tests++;
    }
    
    // 显示测试结果
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════\n");
    printf("🎯 性能基准测试完成!\n");
    printf("📊 通过: %d/%d 测试\n", passed_tests, total_tests);
    
    if (passed_tests == total_tests) {
        printf("🎉 所有性能测试通过!\n");
        return 0;
    } else {
        printf("❌ 有 %d 个测试失败\n", total_tests - passed_tests);
        return 1;
    }
} 