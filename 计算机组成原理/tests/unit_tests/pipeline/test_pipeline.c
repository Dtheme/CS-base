/* test_pipeline.c - 流水线模拟器单元测试
 * 计算机组成原理模拟器 - 流水线模拟器单元测试模块
 * create by: zw.duan
 */
#include "../../../simulation/pipeline_sim/pipeline.h"
#include "../../../simulation/common/test_framework.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

/**
 * 流水线模拟器测试程序
 * 
 * 测试内容：
 * 1. 基本流水线功能测试
 * 2. 数据冲突检测和处理
 * 3. 分支预测测试
 * 4. 性能指标计算
 * 5. 408考研典型场景
 */

// 测试用的简单程序
static u32 test_program_basic[] = {
    0x20010001,  // addi$1,$0, 1     #$1 = 1
    0x20020002,  // addi$2,$0, 2     #$2 = 2
    0x00221820,  // add $3,$1,$2    #$3 =$1 +$2
    0x00000000   // 程序结束
};

static u32 test_program_load_use[] = {
    0x8c010000,  // lw  $1, 0($0)     # Load from memory
    0x00211820,  // add $3,$1,$1    # Use$1 immediately (Load-Use hazard)
    0x00000000   // 程序结束
};

static u32 test_program_forwarding[] = {
    0x20010001,  // addi$1,$0, 1     #$1 = 1
    0x00211820,  // add $3,$1,$1    # Use$1 (needs forwarding)
    0x00611820,  // add $3,$3,$1    # Use$3 and$1
    0x00000000   // 程序结束
};

// 全局测试状态
static bool test_passed = true;
static int tests_run = 0;
static int tests_passed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (!(condition)) { \
            printf("❌ FAIL: %s\n", message); \
            test_passed = false; \
        } else { \
            printf("✅ PASS: %s\n", message); \
            tests_passed++; \
        } \
    } while(0)

/**
 * 测试基本流水线功能
 */
void test_basic_pipeline() {
    printf("\n=== 测试基本流水线功能 ===\n");
    
    pipeline_t pipeline;
    cpu_error_t result = pipeline_init(&pipeline);
    TEST_ASSERT(result == CPU_SUCCESS, "流水线初始化");
    
    bool load_result = pipeline_load_program(&pipeline, test_program_basic, sizeof(test_program_basic));
    TEST_ASSERT(load_result == true, "程序加载");
    
    // 执行程序
    u32 cycles = pipeline_run(&pipeline, 20);
    TEST_ASSERT(cycles > 0, "程序执行");
    
    // 检查结果
    TEST_ASSERT(pipeline.registers[1] == 1, "寄存器$1的值");
    TEST_ASSERT(pipeline.registers[2] == 2, "寄存器$2的值");
    TEST_ASSERT(pipeline.registers[3] == 3, "寄存器$3的值（$1+$2）");
    
    pipeline_stats_t stats = pipeline_get_stats(&pipeline);
    TEST_ASSERT(stats.total_instructions == 3, "总指令数统计");
    TEST_ASSERT(stats.cpi > 1.0 && stats.cpi < 2.0, "合理的CPI值");
    
    pipeline_destroy(&pipeline);
}

/**
 * 测试Load-Use冲突检测
 */
void test_load_use_hazard() {
    printf("\n=== 测试Load-Use冲突检测 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    // 初始化存储器数据
    pipeline.data_memory[0] = 0x12345678;
    
    pipeline_load_program(&pipeline, test_program_load_use, sizeof(test_program_load_use));
    
    // 开启调试模式
    pipeline_set_debug_mode(&pipeline, false);
    
    u32 cycles = pipeline_run(&pipeline, 20);
    
    // Load-Use冲突应该导致额外的周期
    TEST_ASSERT(cycles > 7, "Load-Use冲突导致的额外周期");
    
    // 检查最终结果
    TEST_ASSERT(pipeline.registers[1] == 0x12345678, "Load指令正确执行");
    TEST_ASSERT(pipeline.registers[3] == 0x246ACF0, "Add指令正确执行（考虑32位溢出）");
    
    pipeline_destroy(&pipeline);
}

/**
 * 测试数据转发
 */
void test_data_forwarding() {
    printf("\n=== 测试数据转发 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    pipeline_load_program(&pipeline, test_program_forwarding, sizeof(test_program_forwarding));
    
    u32 cycles = pipeline_run(&pipeline, 20);
    
    // 验证转发正确工作
    TEST_ASSERT(pipeline.registers[1] == 1, "第一条指令执行正确");
    TEST_ASSERT(pipeline.registers[3] == 2, "数据转发正确处理");
    
    pipeline_destroy(&pipeline);
}

/**
 * 测试分支预测器
 */
void test_branch_prediction() {
    printf("\n=== 测试分支预测器 ===\n");
    
    branch_predictor_t predictor;
    branch_reset_predictor(&predictor);
    
    // 测试初始状态
    bool initial_prediction = branch_predict(&predictor, 0x1000);
    TEST_ASSERT(initial_prediction == false, "初始预测为不跳转");
    
    // 模拟分支指令执行
    for (int i = 0; i < 5; i++) {
        branch_update_predictor(&predictor, 0x1000, true);  // 连续跳转
    }
    
    bool after_training = branch_predict(&predictor, 0x1000);
    TEST_ASSERT(after_training == true, "训练后预测跳转");
    TEST_ASSERT(predictor.accuracy > 0.6, "预测准确率合理");
    
    // 测试状态转换
    branch_update_predictor(&predictor, 0x1000, false);  // 一次不跳转
    bool after_miss = branch_predict(&predictor, 0x1000);
    TEST_ASSERT(after_miss == true, "强预测状态的稳定性");
}

/**
 * 测试指令解析
 */
void test_instruction_decode() {
    printf("\n=== 测试指令解析 ===\n");
    
    // 测试R型指令：add$1,$2,$3
    u32 r_type = 0x00431020;  // add$2,$2,$3
    instruction_t r_inst = pipeline_decode_instruction(r_type);
    
    TEST_ASSERT(r_inst.type == INST_R_TYPE, "R型指令识别");
    TEST_ASSERT(r_inst.rs == 2, "Rs字段解析");
    TEST_ASSERT(r_inst.rt == 3, "Rt字段解析");
    TEST_ASSERT(r_inst.rd == 2, "Rd字段解析");
    TEST_ASSERT(r_inst.funct == 0x20, "功能码解析");
    
    // 测试I型指令：addi$1,$0, 100
    u32 i_type = 0x20010064;  // addi$1,$0, 100
    instruction_t i_inst = pipeline_decode_instruction(i_type);
    
    TEST_ASSERT(i_inst.type == INST_I_TYPE, "I型指令识别");
    TEST_ASSERT(i_inst.rs == 0, "源寄存器解析");
    TEST_ASSERT(i_inst.rt == 1, "目标寄存器解析");
    TEST_ASSERT(i_inst.immediate == 100, "立即数解析");
    
    // 测试Load指令：lw$1, 0($2)
    u32 load_type = 0x8c410000;  // lw$1, 0($2)
    instruction_t load_inst = pipeline_decode_instruction(load_type);
    
    TEST_ASSERT(load_inst.type == INST_LOAD, "Load指令识别");
    TEST_ASSERT(load_inst.rs == 2, "基址寄存器解析");
    TEST_ASSERT(load_inst.rt == 1, "目标寄存器解析");
    
    // 测试NOP指令
    instruction_t nop_inst = pipeline_decode_instruction(0x00000000);
    TEST_ASSERT(nop_inst.type == INST_NOP, "NOP指令识别");
    TEST_ASSERT(nop_inst.valid == false, "NOP指令无效标志");
}

/**
 * 测试性能统计
 */
void test_performance_stats() {
    printf("\n=== 测试性能统计 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    pipeline_load_program(&pipeline, test_program_basic, sizeof(test_program_basic));
    u32 cycles = pipeline_run(&pipeline, 20);
    
    pipeline_stats_t stats = pipeline_get_stats(&pipeline);
    
    TEST_ASSERT(stats.total_cycles == cycles, "总周期数统计");
    TEST_ASSERT(stats.total_instructions > 0, "总指令数统计");
    TEST_ASSERT(stats.cpi > 0, "CPI计算");
    TEST_ASSERT(stats.ipc > 0, "IPC计算");
    TEST_ASSERT(stats.efficiency >= 0 && stats.efficiency <= 100, "效率计算范围");
    
    pipeline_destroy(&pipeline);
}

/**
 * 408考研场景测试：性能计算
 */
void test_408_performance_calculation() {
    printf("\n=== 408考研场景：性能计算 ===\n");
    
    // 模拟408考研题目：计算流水线性能指标
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    // 创建包含不同类型指令的程序
    u32 exam_program[] = {
        0x20010001,  // addi$1,$0, 1
        0x8c020000,  // lw  $2, 0($0)     # Load指令
        0x00411820,  // add $3,$2,$1    # 可能的Load-Use冲突
        0x20040002,  // addi$4,$0, 2
        0x00832020,  // add $4,$4,$3
        0x00000000
    };
    
    pipeline.data_memory[0] = 10;  // 为Load指令准备数据
    
    pipeline_load_program(&pipeline, exam_program, sizeof(exam_program));
    
    u32 cycles = pipeline_run(&pipeline, 30);
    pipeline_stats_t stats = pipeline_get_stats(&pipeline);
    
    printf("📊 性能分析结果：\n");
    printf("   总周期数: %llu\n", stats.total_cycles);
    printf("   总指令数: %llu\n", stats.total_instructions);
    printf("   平均CPI: %.3f\n", stats.cpi);
    printf("   流水线效率: %.2f%%\n", stats.efficiency);
    
    // 验证408考研常见的性能指标
    TEST_ASSERT(stats.cpi > 1.0, "存在冲突导致CPI > 1");
    TEST_ASSERT(stats.efficiency < 100.0, "冲突导致效率下降");
    
    // 验证计算结果的正确性
    double theoretical_cpi = (double)stats.total_cycles / stats.total_instructions;
    TEST_ASSERT(fabs(stats.cpi - theoretical_cpi) < 0.001, "CPI计算精度");
    
    pipeline_destroy(&pipeline);
}

/**
 * 408考研场景测试：冲突分析
 */
void test_408_hazard_analysis() {
    printf("\n=== 408考研场景：冲突分析 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    // 构造包含各种冲突的指令序列
    u32 hazard_program[] = {
        0x8c010000,  // lw  $1, 0($0)     # Load指令
        0x00220820,  // add $1,$1,$2    # RAW冲突 + Load-Use
        0x00411820,  // add $3,$2,$1    # RAW冲突
        0xac030004,  // sw  $3, 4($0)     # Store指令
        0x00000000
    };
    
    pipeline.data_memory[0] = 0x12345678;
    
    size_t program_size = sizeof(hazard_program);
    pipeline_load_program(&pipeline, hazard_program, program_size);
    
    // 逐步执行，检测冲突
    pipeline_set_debug_mode(&pipeline, false);
    
    u32 cycles = pipeline_run(&pipeline, 30);
    
    printf("📊 冲突分析结果：\n");
    printf("   执行周期: %u\n", cycles);
    printf("   理想周期: %u\n", 4 + 5 - 1); // 4条指令，5级流水线
    printf("   额外周期: %u（由冲突造成）\n", cycles - 8);
    
    // 验证冲突检测
    hazard_detection_t hazard = pipeline_detect_data_hazard(&pipeline);
    
    // 验证最终结果
    TEST_ASSERT(cycles > 8, "冲突导致额外周期");
    TEST_ASSERT(pipeline.data_memory[1] == pipeline.registers[3], "Store指令正确执行");
    
    pipeline_destroy(&pipeline);
}

/**
 * 运行所有测试
 */
int main() {
    printf("🚀 启动流水线模拟器测试\n");
    printf("=========================================\n");
    
    test_basic_pipeline();
    test_load_use_hazard();
    test_data_forwarding();
    test_branch_prediction();
    test_instruction_decode();
    test_performance_stats();
    test_408_performance_calculation();
    test_408_hazard_analysis();
    
    printf("\n=== pipeline测试结果统计 ===\n");
    printf("📊 测试结果汇总：\n");
    printf("   总测试数: %d\n", tests_run);
    printf("   通过测试: %d\n", tests_passed);
    printf("   失败测试: %d\n", tests_run - tests_passed);
    printf("   成功率: %.1f%%\n", (double)tests_passed / tests_run * 100);
    
    if (tests_passed == tests_run) {
        printf("🎉 所有测试通过！流水线模拟器工作正常。\n");
        return 0;
    } else {
        printf("❌ 存在失败的测试，请检查实现。\n");
        return 1;
    }
} 