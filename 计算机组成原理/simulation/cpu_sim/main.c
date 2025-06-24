#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu_core.h"
#include "registers.h"
#include "memory.h"
#include "../common/types.h"
#include "../common/debug.h"

/**
 * CPU模拟器主程序 - 简化版本
 */

void print_usage(const char *program_name) {
    printf("使用方法: %s [选项]\n", program_name);
    printf("\n选项:\n");
    printf("  -h, --help           显示此帮助信息\n");
    printf("  -v, --version        显示版本信息\n");
    printf("  -t, --test           运行基本测试\n");
    printf("\n示例:\n");
    printf("  %s -t                    # 运行基本测试\n", program_name);
}

void print_version(void) {
    printf("CPU模拟器 v1.0 - 简化版\n");
    printf("408考研复习资料工程 - 计算机组成原理模块\n");
    printf("构建时间: %s %s\n", __DATE__, __TIME__);
}

int run_basic_test(void) {
    printf("=== CPU模拟器基本测试 ===\n");
    
    // 创建CPU实例
    cpu_t cpu;
    cpu_error_t err = cpu_init(&cpu, 1024 * 1024); // 1MB内存
    if (err != CPU_SUCCESS) {
        printf("CPU初始化失败: %d\n", err);
        return 1;
    }
    
    printf("✓ CPU初始化成功\n");
    
    // 测试寄存器操作
    err = registers_set(&cpu.registers, 0, 0x12345678);
    if (err != CPU_SUCCESS) {
        printf("寄存器设置失败: %d\n", err);
        cpu_cleanup(&cpu);
        return 1;
    }
    
    word_t value = registers_get(&cpu.registers, 0);
    if (value != 0x12345678) {
        printf("寄存器读取失败: 期望 0x12345678, 实际 0x%08X\n", value);
        cpu_cleanup(&cpu);
        return 1;
    }
    
    printf("✓ 寄存器操作测试通过\n");
    
    // 测试内存操作
    err = memory_write_word(&cpu.memory, 0x1000, 0xDEADBEEF);
    if (err != CPU_SUCCESS) {
        printf("内存写入失败: %d\n", err);
        cpu_cleanup(&cpu);
        return 1;
    }
    
    word_t mem_value;
    err = memory_read_word(&cpu.memory, 0x1000, &mem_value);
    if (err != CPU_SUCCESS) {
        printf("内存读取失败: %d\n", err);
        cpu_cleanup(&cpu);
        return 1;
    }
    
    if (mem_value != 0xDEADBEEF) {
        printf("内存读写失败: 期望 0xDEADBEEF, 实际 0x%08X\n", mem_value);
        cpu_cleanup(&cpu);
        return 1;
    }
    
    printf("✓ 内存操作测试通过\n");
    
    // 测试ALU操作
    alu_result_t alu_result = alu_add(100, 200);
    if (!alu_result.valid || alu_result.result != 300) {
        printf("ALU加法测试失败: 期望 300, 实际 %u\n", alu_result.result);
        cpu_cleanup(&cpu);
        return 1;
    }
    
    printf("✓ ALU操作测试通过\n");
    
    // 显示CPU状态
    printf("\n--- CPU状态 ---\n");
    cpu_dump_state(&cpu, true);
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    printf("\n=== 所有测试通过! ===\n");
    return 0;
}

int main(int argc, char *argv[]) {
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--test") == 0) {
            return run_basic_test();
        } else {
            printf("未知选项: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }
    
    // 默认显示帮助信息
    print_usage(argv[0]);
    return 0;
} 