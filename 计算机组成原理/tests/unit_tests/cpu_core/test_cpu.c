/* test_cpu.c - CPU核心单元测试
 * 计算机组成原理模拟器 - CPU核心模块测试
 * create by: zw.duan
 */
#include "cpu_core.h"
#include "alu.h"
#include "registers.h"
#include "memory.h"
#include "../common/test_framework.h"
#include "../common/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * CPU模拟器测试程序
 * 
 * 测试内容：
 * 1. CPU核心功能测试
 * 2. 寄存器操作测试
 * 3. 内存管理测试
 * 4. ALU运算测试
 * 5. 指令执行测试
 * 6. 性能测试
 */

// ==================== 测试用例声明 ====================

static bool test_cpu_initialization(void);
static bool test_cpu_state_management(void);
static bool test_cpu_registers(void);
static bool test_cpu_memory(void);
static bool test_cpu_alu(void);
static bool test_cpu_instruction_execution(void);
static bool test_cpu_program_loading(void);
static bool test_cpu_performance_counters(void);
static bool test_cpu_error_handling(void);
static bool test_cpu_stress(void);

// ==================== 测试辅助函数 ====================

/**
 * 创建简单的测试程序
 */
static void create_test_program(byte_t *program, size_t size) {
    memset(program, 0, size);
    
    // 简单的测试程序：加载立即数，执行加法，停机
    size_t i = 0;
    
    // MOV R0, 10 (加载立即数10到R0)
    program[i++] = 0x10; // OP_MOV
    program[i++] = 0x00; // 目标寄存器R0
    program[i++] = 0x0A; // 立即数10
    program[i++] = 0x00; // 填充
    
    // MOV R1, 20 (加载立即数20到R1)
    program[i++] = 0x10; // OP_MOV
    program[i++] = 0x01; // 目标寄存器R1
    program[i++] = 0x14; // 立即数20
    program[i++] = 0x00; // 填充
    
    // ADD R0, R1 (R0 = R0 + R1)
    program[i++] = 0x20; // OP_ADD
    program[i++] = 0x00; // 目标寄存器R0
    program[i++] = 0x01; // 源寄存器R1
    program[i++] = 0x00; // 填充
    
    // HLT (停机)
    program[i++] = 0xFF; // OP_HLT
    program[i++] = 0x00; // 填充
    program[i++] = 0x00; // 填充
    program[i++] = 0x00; // 填充
}

// ==================== CPU初始化测试 ====================

static bool test_cpu_initialization(void) {
    TEST_START("CPU初始化测试");
    
    cpu_t cpu;
    cpu_error_t err;
    
    // 测试正常初始化
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    TEST_ASSERT(cpu.state == CPU_STATE_STOPPED, "初始状态应该是STOPPED");
    
    // 测试NULL指针
    err = cpu_init(NULL, 4096);
    TEST_ASSERT(err == CPU_ERROR_NULL_POINTER, "NULL指针应该返回错误");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== CPU状态管理测试 ====================

static bool test_cpu_state_management(void) {
    TEST_START("CPU状态管理测试");
    
    cpu_t cpu;
    cpu_error_t err;
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 测试状态设置
    err = cpu_set_state(&cpu, CPU_STATE_RUNNING);
    TEST_ASSERT(err == CPU_SUCCESS, "设置运行状态应该成功");
    TEST_ASSERT(cpu_get_state(&cpu) == CPU_STATE_RUNNING, "状态应该是RUNNING");
    
    // 测试状态查询
    TEST_ASSERT(cpu_is_running(&cpu) == true, "应该识别为运行状态");
    TEST_ASSERT(cpu_is_halted(&cpu) == false, "不应该识别为停机状态");
    
    // 测试停机
    err = cpu_halt(&cpu);
    TEST_ASSERT(err == CPU_SUCCESS, "停机应该成功");
    TEST_ASSERT(cpu_get_state(&cpu) == CPU_STATE_HALTED, "状态应该是HALTED");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 寄存器测试 ====================

static bool test_cpu_registers(void) {
    TEST_START("CPU寄存器测试");
    
    cpu_t cpu;
    cpu_error_t err;
    word_t value;
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 测试寄存器读写
    err = registers_set(&cpu.registers, 0, 0x12345678);
    TEST_ASSERT(err == CPU_SUCCESS, "寄存器写入应该成功");
    
    value = registers_get(&cpu.registers, 0);
    TEST_ASSERT(value == 0x12345678, "读取值应该与写入值相等");
    
    // 测试无效寄存器
    err = registers_set(&cpu.registers, 99, 0);
    TEST_ASSERT(err == CPU_ERROR_INVALID_REGISTER, "无效寄存器应该返回错误");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 内存测试 ====================

static bool test_cpu_memory(void) {
    TEST_START("CPU内存测试");
    
    cpu_t cpu;
    cpu_error_t err;
    word_t value;
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 测试内存读写
    err = memory_write_word(&cpu.memory, 0x1000, 0xDEADBEEF);
    TEST_ASSERT(err == CPU_SUCCESS, "内存写入应该成功");
    
    err = memory_read_word(&cpu.memory, 0x1000, &value);
    TEST_ASSERT(err == CPU_SUCCESS, "内存读取应该成功");
    TEST_ASSERT(value == 0xDEADBEEF, "读取值应该与写入值相等");
    
    // 测试内存边界
    err = memory_write_word(&cpu.memory, 0x2000, 0x12345678);
    TEST_ASSERT(err != CPU_SUCCESS, "超出边界的写入应该失败");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== ALU测试 ====================

static bool test_cpu_alu(void) {
    TEST_START("CPU ALU测试");
    
    cpu_t cpu;
    cpu_error_t err;
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 测试ALU运算（正确的调用方式）
    alu_result_t alu_result;
    
    // 测试加法运算
    alu_result = alu_add(10, 20);
    TEST_ASSERT(alu_result.valid == true, "ALU加法应该成功");
    TEST_ASSERT(alu_result.result == 30, "10 + 20 应该等于 30");
    
    // 测试减法运算
    alu_result = alu_sub(50, 30);
    TEST_ASSERT(alu_result.valid == true, "ALU减法应该成功");
    TEST_ASSERT(alu_result.result == 20, "50 - 30 应该等于 20");
    
    // 测试乘法运算
    alu_result = alu_mul(5, 6);
    TEST_ASSERT(alu_result.valid == true, "ALU乘法应该成功");
    TEST_ASSERT(alu_result.result == 30, "5 * 6 应该等于 30");
    
    // 测试除法运算
    alu_result = alu_div(100, 5);
    TEST_ASSERT(alu_result.valid == true, "ALU除法应该成功");
    TEST_ASSERT(alu_result.result == 20, "100 / 5 应该等于 20");
    
    // 测试除零错误
    alu_result = alu_div(10, 0);
    TEST_ASSERT(alu_result.valid == false, "除零应该返回无效结果");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 指令执行测试 ====================

static bool test_cpu_instruction_execution(void) {
    TEST_START("CPU指令执行测试");
    
    cpu_t cpu;
    cpu_error_t err;
    byte_t program[64];
    word_t result;
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 创建测试程序
    create_test_program(program, sizeof(program));
    
    // 加载程序
    err = cpu_load_program_from_bytes(&cpu, program, sizeof(program), 0x0000);
    TEST_ASSERT(err == CPU_SUCCESS, "程序加载应该成功");
    
    // 设置入口点
    err = cpu_set_entry_point(&cpu, 0x0000);
    TEST_ASSERT(err == CPU_SUCCESS, "设置入口点应该成功");
    
    // 设置运行状态
    err = cpu_set_state(&cpu, CPU_STATE_RUNNING);
    TEST_ASSERT(err == CPU_SUCCESS, "设置运行状态应该成功");
    
    // 执行程序（限制执行次数避免无限循环）
    for (int i = 0; i < 10 && cpu_is_running(&cpu); i++) {
        err = cpu_execute_instruction(&cpu);
        if (err != CPU_SUCCESS && err != CPU_ERROR_NOT_RUNNING) {
            printf("指令执行失败，错误码: %d\n", err);
            break;
        }
    }
    
    // 检查程序是否正常停机
    TEST_ASSERT(cpu_is_halted(&cpu), "程序应该正常停机");
    
    // 检查执行结果（R0应该包含30）
    result = registers_get(&cpu.registers, 0);
    if (result == 30) {
        printf("✓ 计算结果正确: R0 = %u\n", result);
    } else {
        printf("✗ 计算结果错误: R0 = %u (期望30)\n", result);
    }
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 程序加载测试 ====================

static bool test_cpu_program_loading(void) {
    TEST_START("CPU程序加载测试");
    
    cpu_t cpu;
    cpu_error_t err;
    byte_t program[32];
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 创建测试程序
    memset(program, 0xFF, sizeof(program)); // 使用0xFF填充
    
    // 测试程序加载
    err = cpu_load_program_from_bytes(&cpu, program, sizeof(program), 0x0000);
    TEST_ASSERT(err == CPU_SUCCESS, "程序加载应该成功");
    
    // 验证程序是否正确加载
    byte_t read_byte;
    err = memory_read_byte(&cpu.memory, 0x0000, &read_byte);
    TEST_ASSERT(err == CPU_SUCCESS, "内存读取应该成功");
    TEST_ASSERT(read_byte == 0xFF, "程序内容应该正确加载");
    
    // 测试NULL指针
    err = cpu_load_program_from_bytes(&cpu, NULL, sizeof(program), 0x0000);
    TEST_ASSERT(err == CPU_ERROR_NULL_POINTER, "NULL指针应该返回错误");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 性能计数器测试 ====================

static bool test_cpu_performance_counters(void) {
    TEST_START("CPU性能计数器测试");
    
    cpu_t cpu;
    cpu_error_t err;
    cpu_performance_t perf;
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 重置性能计数器
    err = cpu_reset_performance_counters(&cpu);
    TEST_ASSERT(err == CPU_SUCCESS, "重置性能计数器应该成功");
    
    // 获取性能数据
    err = cpu_get_performance(&cpu, &perf);
    TEST_ASSERT(err == CPU_SUCCESS, "获取性能数据应该成功");
    TEST_ASSERT(perf.total_instructions == 0, "指令计数应该为0");
    TEST_ASSERT(perf.total_cycles == 0, "周期计数应该为0");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 错误处理测试 ====================

static bool test_cpu_error_handling(void) {
    TEST_START("CPU错误处理测试");
    
    cpu_t cpu;
    cpu_error_t err;
    
    // 测试未初始化的CPU操作
    err = cpu_set_state(NULL, CPU_STATE_RUNNING);
    TEST_ASSERT(err == CPU_ERROR_NULL_POINTER, "NULL指针应该返回错误");
    
    // 初始化CPU
    err = cpu_init(&cpu, 4096);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 测试在停止状态下执行指令
    err = cpu_execute_instruction(&cpu);
    TEST_ASSERT(err == CPU_ERROR_NOT_RUNNING, "停止状态下执行指令应该失败");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 压力测试 ====================

static bool test_cpu_stress(void) {
    TEST_START("CPU压力测试");
    
    cpu_t cpu;
    cpu_error_t err;
    byte_t program[1024];
    
    // 初始化CPU
    err = cpu_init(&cpu, 8192);
    TEST_ASSERT(err == CPU_SUCCESS, "CPU初始化应该成功");
    
    // 创建大型测试程序
    memset(program, 0x00, sizeof(program)); // NOP指令
    program[sizeof(program) - 4] = 0xFF;    // 最后添加HLT指令
    
    // 加载程序
    err = cpu_load_program_from_bytes(&cpu, program, sizeof(program), 0x0000);
    TEST_ASSERT(err == CPU_SUCCESS, "大程序加载应该成功");
    
    // 设置运行状态
    err = cpu_set_state(&cpu, CPU_STATE_RUNNING);
    TEST_ASSERT(err == CPU_SUCCESS, "设置运行状态应该成功");
    
    // 执行大量指令
    err = cpu_execute_instructions(&cpu, 100);
    TEST_ASSERT(err == CPU_SUCCESS || err == CPU_ERROR_NOT_RUNNING, 
                "大量指令执行应该成功");
    
    // 清理资源
    cpu_cleanup(&cpu);
    
    TEST_END();
    return true;
}

// ==================== 主测试函数 ====================

int main(void) {
    printf("\n=== cpu_core测试结果统计 ===\n");
        
    bool all_passed = true;
    
    // 运行所有测试
    all_passed &= test_cpu_initialization();
    all_passed &= test_cpu_state_management();
    all_passed &= test_cpu_registers();
    all_passed &= test_cpu_memory();
    all_passed &= test_cpu_alu();
    all_passed &= test_cpu_instruction_execution();
    all_passed &= test_cpu_program_loading();
    all_passed &= test_cpu_performance_counters();
    all_passed &= test_cpu_error_handling();
    all_passed &= test_cpu_stress();

    // 输出测试结果
    printf("\n=========================================\n");
    if (all_passed) {
        printf("✓ 所有CPU测试通过！CPU模拟器功能正常\n");
    } else {
        printf("✗ 某些CPU测试失败！\n");
        printf("请检查CPU模拟器实现，修复相关问题。\n");
    }
    printf("=========================================\n");
    printf("\n=== cpu_core测试结果统计 ===\n");

    return all_passed ? 0 : 1;
} 