#include "pipeline.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * 流水线模拟器主程序
 * 演示流水线的基本功能和408考研应用场景
 */

void demo_basic_pipeline() {
    printf("=== 基本流水线功能演示 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    // 简单的测试程序
    u32 program[] = {
        0x20010001,  // addi$1,$0, 1     #$1 = 1
        0x20020002,  // addi$2,$0, 2     #$2 = 2
        0x00221820,  // add $3,$1,$2    #$3 =$1 +$2 = 3
        0x20040005,  // addi$4,$0, 5     #$4 = 5
        0x00641020,  // add $2,$3,$4    #$2 =$3 +$4 = 8
        0x00000000   // 程序结束
    };
    
    pipeline_load_program(&pipeline, program, sizeof(program));
    pipeline_set_debug_mode(&pipeline, true);
    
    printf("开始执行流水线程序...\n");
    u32 cycles = pipeline_run(&pipeline, 30);
    
    printf("\n执行完成！\n");
    pipeline_print_stats(&pipeline);
    
    printf("\n最终寄存器状态：\n");
    printf("$1 = %d\n", pipeline.registers[1]);
    printf("$2 = %d\n", pipeline.registers[2]);
    printf("$3 = %d\n", pipeline.registers[3]);
    printf("$4 = %d\n", pipeline.registers[4]);
    
    pipeline_destroy(&pipeline);
}

void demo_load_use_hazard() {
    printf("\n=== Load-Use冲突演示 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    // 设置存储器数据
    pipeline.data_memory[0] = 100;
    pipeline.data_memory[1] = 200;
    
    // 包含Load-Use冲突的程序
    u32 program[] = {
        0x8c010000,  // lw  $1, 0($0)     # Load$1 from memory[0]
        0x00211020,  // add $2,$1,$1    # Use$1 immediately (冲突!)
        0x8c030004,  // lw  $3, 4($0)     # Load$3 from memory[1]  
        0x00621820,  // add $3,$3,$2    # Use$3 and$2
        0x00000000
    };
    
    pipeline_load_program(&pipeline, program, sizeof(program));
    pipeline_set_debug_mode(&pipeline, false);
    
    printf("执行包含Load-Use冲突的程序...\n");
    u32 cycles = pipeline_run(&pipeline, 30);
    
    printf("执行结果：\n");
    printf("总周期数: %u\n", cycles);
    printf("$1 = %d (应该是100)\n", pipeline.registers[1]);
    printf("$2 = %d (应该是200)\n", pipeline.registers[2]);
    printf("$3 = %d (应该是400)\n", pipeline.registers[3]);
    
    pipeline_destroy(&pipeline);
}

void demo_408_exam_scenario() {
    printf("\n=== 408考研场景演示 ===\n");
    
    pipeline_t pipeline;
    pipeline_init(&pipeline);
    
    // 典型的408考研程序：包含多种指令类型
    u32 program[] = {
        0x20010064,  // addi$1,$0, 100   #$1 = 100
        0x20020032,  // addi$2,$0, 50    #$2 = 50
        0x8c030000,  // lw  $3, 0($0)     # Load from memory
        0x00411820,  // add $3,$2,$1    #$3 =$2 +$1 (可能转发)
        0x00621022,  // sub $2,$3,$2    #$2 =$3 -$2 (数据冲突)
        0xac020004,  // sw  $2, 4($0)     # Store to memory
        0x00000000
    };
    
    // 初始化存储器数据
    pipeline.data_memory[0] = 25;
    
    pipeline_load_program(&pipeline, program, sizeof(program));
    
    printf("执行408考研典型程序...\n");
    u32 cycles = pipeline_run(&pipeline, 30);
    
    pipeline_stats_t stats = pipeline_get_stats(&pipeline);
    
    printf("\n📊 性能分析（408考研重点）：\n");
    printf("==========================================\n");
    printf("总指令数: %llu\n", stats.total_instructions);
    printf("总周期数: %llu\n", stats.total_cycles);
    printf("平均CPI: %.3f\n", stats.cpi);
    printf("指令吞吐率(IPC): %.3f\n", stats.ipc);
    printf("流水线效率: %.2f%%\n", stats.efficiency);
    printf("==========================================\n");
    
    // 408考研常考的计算
    double speedup = 5.0 * stats.total_instructions / stats.total_cycles;
    printf("相对于非流水线的加速比: %.3f\n", speedup);
    
    double ideal_cpi = 1.0;
    double overhead_cpi = stats.cpi - ideal_cpi;
    printf("冲突开销(额外CPI): %.3f\n", overhead_cpi);
    
    printf("\n存储器内容:\n");
    printf("Memory[0] = %d\n", pipeline.data_memory[0]);
    printf("Memory[1] = %d\n", pipeline.data_memory[1]);
    
    printf("\n寄存器内容:\n");
    for (int i = 1; i <= 3; i++) {
        printf("$%d = %d\n", i, pipeline.registers[i]);
    }
    
    pipeline_destroy(&pipeline);
}

int main() {
    printf("🚀 计算机组成原理 - 指令流水线模拟器\n");
    printf("=========================================\n");
    printf("本程序演示五级流水线的工作原理，包括：\n");
    printf("• 基本流水线执行过程\n");
    printf("• 数据冲突检测与处理\n");
    printf("• 流水线性能分析\n");
    printf("• 408考研典型应用场景\n");
    printf("=========================================\n");
    
    // 演示各种功能
    demo_basic_pipeline();
    demo_load_use_hazard();
    demo_408_exam_scenario();
    
    printf("\n🎓 流水线原理总结：\n");
    printf("1. 流水线通过并行执行提高吞吐率\n");
    printf("2. 数据冲突需要通过转发或停顿解决\n");
    printf("3. Load-Use冲突必须停顿处理\n");
    printf("4. 实际CPI通常大于1，效率受冲突影响\n");
    printf("5. 408考研重点：性能计算和冲突分析\n");
    
    printf("\n✅ 演示完成！\n");
    return 0;
} 