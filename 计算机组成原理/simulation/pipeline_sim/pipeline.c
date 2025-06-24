#include "pipeline.h"
#include "../common/debug.h"
#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * 指令流水线模拟器核心实现
 * 
 * 实现架构：
 * 本模块实现了完整的五级流水线处理器模拟，包括指令执行、冲突检测、
 * 性能分析和调试支持等功能。
 * 
 * 核心功能：
 * 1. 五级流水线执行 - IF/ID/EX/MEM/WB各阶段的精确模拟
 * 2. 完整冲突处理 - 数据冲突检测、转发机制、流水线停顿
 * 3. 分支预测器 - 两位饱和计数器动态预测
 * 4. 性能监控 - CPI、吞吐率、效率等指标实时计算
 * 5. 调试支持 - 详细的执行跟踪和状态输出
 * 
 * 技术特点：
 * - 周期精确模拟：严格按照时钟周期执行
 * - 状态完整保存：所有中间状态可查询
 * - 扩展性设计：易于增加新的优化技术
 */

/* ================================
 * 核心函数实现
 * ================================ */

cpu_error_t pipeline_init(pipeline_t *pipeline) {
    // 参数有效性检查
    if (!pipeline) {
        LOG_ERROR("空指针传递给pipeline_init");
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 内存清零初始化
    // 原理：使用memset快速将整个结构体清零，确保所有字段处于已知状态
    // 优势：避免逐字段初始化可能的遗漏，提高初始化效率
    memset(pipeline, 0, sizeof(pipeline_t));
    
    // 处理器核心状态初始化
    pipeline->pc = 0;                // 程序计数器归零，从地址0开始执行
    pipeline->clock_cycle = 0;       // 时钟周期计数器清零
    pipeline->running = false;       // 处理器初始为停止状态
    pipeline->debug_mode = false;    // 默认关闭调试模式
    
    // 寄存器文件初始化
    // 原理：将所有通用寄存器清零，模拟处理器复位状态
    // 注意：在实际处理器中，寄存器复位值可能是未定义的
    for (int i = 0; i < PIPELINE_MAX_REGISTERS; i++) {
        pipeline->registers[i] = 0;
    }
    
    // 分支预测器初始化
    // 原理：将所有预测状态设置为默认值，清空分支历史
    // 策略：采用保守的"不跳转"预测作为初始状态
    branch_reset_predictor(&pipeline->predictor);
    
    // 性能统计模块初始化
    // 原理：清零所有性能计数器，为新的执行统计做准备
    pipeline_reset_stats(pipeline);
    
    LOG_INFO("流水线处理器初始化完成");
    return CPU_SUCCESS;
}

void pipeline_destroy(pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    LOG_INFO("流水线处理器已销毁");
}

void pipeline_reset(pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    
    pipeline->pc = 0;
    pipeline->clock_cycle = 0;
    pipeline->running = false;
    pipeline->stall_pipeline = false;
    
    memset(&pipeline->if_id, 0, sizeof(if_id_reg_t));
    memset(&pipeline->id_ex, 0, sizeof(id_ex_reg_t));
    memset(&pipeline->ex_mem, 0, sizeof(ex_mem_reg_t));
    memset(&pipeline->mem_wb, 0, sizeof(mem_wb_reg_t));
    
    branch_reset_predictor(&pipeline->predictor);
    pipeline_reset_stats(pipeline);
    
    LOG_INFO("流水线处理器已重置");
}

bool pipeline_load_program(pipeline_t *pipeline, const u32 *program, size_t size) {
    if (!pipeline || !program) {
        LOG_ERROR("无效参数传递给pipeline_load_program");
        return false;
    }
    
    if (size > INSTRUCTION_MEMORY_SIZE) {
        LOG_ERROR("程序大小超过指令存储器容量");
        return false;
    }
    
    memset(pipeline->instruction_memory, 0, sizeof(pipeline->instruction_memory));
    memcpy(pipeline->instruction_memory, program, size);
    
    LOG_INFO("程序已加载，大小: %zu字节", size);
    return true;
}

bool pipeline_clock(pipeline_t *pipeline) {
    // 参数有效性检查
    if (!pipeline) {
        return false;
    }
    
    // 时钟周期递增
    // 原理：每次调用代表一个时钟周期的推进
    pipeline->clock_cycle++;
    
    // 调试信息输出
    if (pipeline->debug_mode) {
        printf("\n========== 时钟周期 %u ==========\n", pipeline->clock_cycle);
    }
    
    // 程序执行结束检测
    // 原理：程序结束需要满足两个条件：
    // 1. PC超出指令存储器范围，或者遇到空指令（0）
    // 2. 流水线中所有阶段都为空（处理完所有指令）
    bool pipeline_empty = !pipeline->if_id.valid && !pipeline->id_ex.valid && 
                          !pipeline->ex_mem.valid && !pipeline->mem_wb.valid;
    
    if (pipeline->pc >= INSTRUCTION_MEMORY_SIZE || 
        (pipeline->instruction_memory[pipeline->pc/4] == 0 && pipeline_empty)) {
        pipeline->running = false;
        return false;
    }
    
    // 数据冲突检测和停顿处理
    // 原理：在每个时钟周期开始时检测是否存在数据冲突
    // 策略：如果检测到Load-Use冲突，则停顿流水线一个周期
    hazard_detection_t hazard = pipeline_detect_data_hazard(pipeline);
    if (hazard.stall_required) {
        pipeline->stall_pipeline = true;
        // 插入气泡（NOP）：使ID/EX寄存器无效，但保持IF/ID寄存器内容
        pipeline->id_ex.valid = false;
        if (pipeline->debug_mode) {
            printf("检测到%s，插入停顿\n", hazard.description);
        }
    } else {
        pipeline->stall_pipeline = false;
    }
    
    // 流水线阶段执行
    // 执行顺序：逆序执行各阶段，避免数据覆盖
    // 原理：从WB阶段开始，依次向前执行到IF阶段
    // 好处：确保每个阶段使用的是上一周期的数据
    pipeline_stage_wb(pipeline);   // 第5阶段：写回
    pipeline_stage_mem(pipeline);  // 第4阶段：存储器访问
    pipeline_stage_ex(pipeline);   // 第3阶段：执行
    
    // 条件执行：当发生停顿时，不推进ID和IF阶段
    if (!pipeline->stall_pipeline) {
        pipeline_stage_id(pipeline);   // 第2阶段：指令译码
    }
    if (!pipeline->stall_pipeline) {
        pipeline_stage_if(pipeline);   // 第1阶段：取指
    }
    
    // 性能统计更新
    // 原理：在每个时钟周期结束时更新性能计数器
    pipeline_update_stats(pipeline);
    
    return true;
}

u32 pipeline_run(pipeline_t *pipeline, u32 max_cycles) {
    if (!pipeline) {
        return 0;
    }
    
    pipeline->running = true;
    u32 cycles = 0;
    
    while (pipeline->running && (max_cycles == 0 || cycles < max_cycles)) {
        if (!pipeline_clock(pipeline)) {
            break;
        }
        cycles++;
    }
    
    pipeline_calculate_metrics(pipeline);
    return cycles;
}

/* ================================
 * 流水线阶段实现
 * ================================ */

void pipeline_stage_if(pipeline_t *pipeline) {
    // 参数检查和停顿条件检查
    // 原理：当流水线停顿时，IF阶段不应该取新指令
    if (!pipeline || pipeline->stall_pipeline) {
        return;
    }
    
    // 指令取指操作
    // 原理：从指令存储器中读取PC地址对应的32位指令
    // 地址计算：PC除以4是因为指令存储器按字寻址（32位 = 4字节）
    u32 instruction_word = pipeline->instruction_memory[pipeline->pc / 4];
    
    // 更新IF/ID流水线寄存器
    // 数据流：将取到的指令和相关信息传递给下一阶段
    pipeline->if_id.pc = pipeline->pc;                                    // 当前指令地址
    pipeline->if_id.pc_plus_4 = pipeline->pc + 4;                        // 下一指令地址
    pipeline->if_id.instruction = pipeline_decode_instruction(instruction_word); // 解码指令
    pipeline->if_id.instruction.pc = pipeline->pc;                       // 绑定指令与地址
    pipeline->if_id.valid = (instruction_word != 0);                     // 有效性检查
    
    // 程序计数器更新
    // 原理：顺序执行时，PC递增4指向下一条指令
    // 注意：分支指令的PC更新在后续阶段处理
    pipeline->pc += 4;
    
    // 调试信息输出
    if (pipeline->debug_mode) {
        printf("IF: PC=0x%08X, 指令=0x%08X\n", pipeline->if_id.pc, instruction_word);
    }
}

void pipeline_stage_id(pipeline_t *pipeline) {
    // 参数和有效性检查
    // 原理：只有当IF/ID寄存器包含有效指令时才进行译码
    if (!pipeline || !pipeline->if_id.valid) {
        pipeline->id_ex.valid = false;
        return;
    }
    
    // 获取指令信息
    instruction_t *inst = &pipeline->if_id.instruction;
    
    // 数据通路设置
    // 原理：将IF/ID寄存器的数据传递到ID/EX寄存器
    pipeline->id_ex.pc_plus_4 = pipeline->if_id.pc_plus_4;               // 传递PC+4
    
    // 寄存器文件读取
    // 原理：根据指令的寄存器字段读取操作数
    // 读取时机：在ID阶段并行读取两个源寄存器的值
    pipeline->id_ex.read_data_1 = pipeline->registers[inst->rs];         // 源寄存器1的值
    pipeline->id_ex.read_data_2 = pipeline->registers[inst->rt];         // 源寄存器2的值
    
    // 立即数符号扩展
    // 原理：将16位立即数符号扩展为32位，保持数值语义
    // 作用：为I型指令提供正确的32位操作数
    pipeline->id_ex.sign_extend = (i32)inst->immediate;
    
    // 寄存器地址传递
    // 原理：将寄存器地址传递给后续阶段，用于写回时的目标选择
    pipeline->id_ex.instruction_20_16 = inst->rt;                       // I型指令的目标寄存器
    pipeline->id_ex.instruction_15_11 = inst->rd;                       // R型指令的目标寄存器
    pipeline->id_ex.rs = inst->rs;                                      // 源寄存器1地址
    pipeline->id_ex.rt = inst->rt;                                      // 源寄存器2地址
    
    // 指令信息保存
    pipeline->id_ex.instruction = *inst;                                // 完整指令信息
    pipeline->id_ex.valid = true;                                       // 标记数据有效
    
    // 简化的控制信号生成
    switch (inst->type) {
        case INST_R_TYPE:
            pipeline->id_ex.reg_write = true;
            pipeline->id_ex.reg_dst = true;
            pipeline->id_ex.alu_src = false;
            // 根据功能码设置ALU操作
            if (inst->funct == 0x20) {  // ADD
                pipeline->id_ex.alu_op = 0;
            } else if (inst->funct == 0x22) {  // SUB
                pipeline->id_ex.alu_op = 1;
            } else if (inst->funct == 0x24) {  // AND
                pipeline->id_ex.alu_op = 2;
            } else if (inst->funct == 0x25) {  // OR
                pipeline->id_ex.alu_op = 3;
            } else {
                pipeline->id_ex.alu_op = 0;  // 默认ADD
            }
            break;
        case INST_I_TYPE:
            pipeline->id_ex.reg_write = true;
            pipeline->id_ex.reg_dst = false;
            pipeline->id_ex.alu_src = true;
            pipeline->id_ex.alu_op = 0; // ADD for ADDI
            break;
        case INST_LOAD:
            pipeline->id_ex.reg_write = true;
            pipeline->id_ex.reg_dst = false;
            pipeline->id_ex.alu_src = true;
            pipeline->id_ex.mem_read = true;
            pipeline->id_ex.mem_to_reg = true;
            pipeline->id_ex.alu_op = 0; // ADD
            break;
        case INST_STORE:
            pipeline->id_ex.reg_write = false;
            pipeline->id_ex.alu_src = true;
            pipeline->id_ex.mem_write = true;
            pipeline->id_ex.alu_op = 0; // ADD
            break;
        default:
            break;
    }
    
    // 统计指令数（在ID阶段统计有效指令）
    pipeline->stats.total_instructions++;
    
    if (pipeline->debug_mode) {
        printf("ID: 指令类型=%d, Rs=R%d, Rt=R%d\n", 
               inst->type, inst->rs, inst->rt);
    }
}

void pipeline_stage_ex(pipeline_t *pipeline) {
    if (!pipeline || !pipeline->id_ex.valid) {
        pipeline->ex_mem.valid = false;
        return;
    }
    
    // ALU操作（包含数据转发）
    u32 alu_input_a = pipeline->id_ex.read_data_1;
    u32 alu_input_b = pipeline->id_ex.alu_src ? 
                     (u32)pipeline->id_ex.sign_extend : 
                     pipeline->id_ex.read_data_2;
    
    // EX-EX数据转发（从EX/MEM阶段转发）
    if (pipeline->ex_mem.valid && pipeline->ex_mem.reg_write && 
        pipeline->ex_mem.write_register != 0) {
        if (pipeline->ex_mem.write_register == pipeline->id_ex.rs) {
            alu_input_a = pipeline->ex_mem.alu_result;
            if (pipeline->debug_mode) {
                printf("    EX-EX转发: R%d=0x%08X -> ALU输入A\n", 
                       pipeline->ex_mem.write_register, pipeline->ex_mem.alu_result);
            }
        }
        if (pipeline->ex_mem.write_register == pipeline->id_ex.rt && !pipeline->id_ex.alu_src) {
            alu_input_b = pipeline->ex_mem.alu_result;
            if (pipeline->debug_mode) {
                printf("    EX-EX转发: R%d=0x%08X -> ALU输入B\n", 
                       pipeline->ex_mem.write_register, pipeline->ex_mem.alu_result);
            }
        }
    }
    
    // MEM-EX数据转发（从MEM/WB阶段转发）
    if (pipeline->mem_wb.valid && pipeline->mem_wb.reg_write && 
        pipeline->mem_wb.write_register != 0) {
        // 如果EX/MEM没有转发，则从MEM/WB转发
        if (!(pipeline->ex_mem.valid && pipeline->ex_mem.reg_write && 
              pipeline->ex_mem.write_register == pipeline->id_ex.rs)) {
            if (pipeline->mem_wb.write_register == pipeline->id_ex.rs) {
                u32 wb_data = pipeline->mem_wb.mem_to_reg ? 
                             pipeline->mem_wb.read_data : pipeline->mem_wb.alu_result;
                alu_input_a = wb_data;
                if (pipeline->debug_mode) {
                    printf("    MEM-EX转发: R%d=0x%08X -> ALU输入A\n", 
                           pipeline->mem_wb.write_register, wb_data);
                }
            }
        }
        if (!(pipeline->ex_mem.valid && pipeline->ex_mem.reg_write && 
              pipeline->ex_mem.write_register == pipeline->id_ex.rt) && !pipeline->id_ex.alu_src) {
            if (pipeline->mem_wb.write_register == pipeline->id_ex.rt) {
                u32 wb_data = pipeline->mem_wb.mem_to_reg ? 
                             pipeline->mem_wb.read_data : pipeline->mem_wb.alu_result;
                alu_input_b = wb_data;
            }
        }
    }
    
    u32 alu_result = 0;
    bool zero = false;
    
    switch (pipeline->id_ex.alu_op) {
        case 0: // ADD
            alu_result = alu_input_a + alu_input_b;
            break;
        case 1: // SUB
            alu_result = alu_input_a - alu_input_b;
            break;
        case 2: // AND
            alu_result = alu_input_a & alu_input_b;
            break;
        case 3: // OR
            alu_result = alu_input_a | alu_input_b;
            break;
        default:
            alu_result = alu_input_a;
            break;
    }
    
    zero = (alu_result == 0);
    
    // 选择写回寄存器
    u8 write_register = pipeline->id_ex.reg_dst ? 
                       pipeline->id_ex.instruction_15_11 : 
                       pipeline->id_ex.instruction_20_16;
    
    // 填充EX/MEM寄存器
    pipeline->ex_mem.reg_write = pipeline->id_ex.reg_write;
    pipeline->ex_mem.mem_to_reg = pipeline->id_ex.mem_to_reg;
    pipeline->ex_mem.mem_read = pipeline->id_ex.mem_read;
    pipeline->ex_mem.mem_write = pipeline->id_ex.mem_write;
    pipeline->ex_mem.zero = zero;
    pipeline->ex_mem.alu_result = alu_result;
    pipeline->ex_mem.write_data = pipeline->id_ex.read_data_2;
    pipeline->ex_mem.write_register = write_register;
    pipeline->ex_mem.instruction = pipeline->id_ex.instruction;
    pipeline->ex_mem.valid = true;
    
    if (pipeline->debug_mode) {
        printf("EX: ALU结果=0x%08X, 写回寄存器=R%d (输入A=0x%08X, 输入B=0x%08X)\n", 
               alu_result, write_register, alu_input_a, alu_input_b);
    }
}

void pipeline_stage_mem(pipeline_t *pipeline) {
    if (!pipeline || !pipeline->ex_mem.valid) {
        pipeline->mem_wb.valid = false;
        return;
    }
    
    u32 read_data = 0;
    
    if (pipeline->ex_mem.mem_read) {
        u32 address = pipeline->ex_mem.alu_result;
        if (address < DATA_MEMORY_SIZE) {
            read_data = pipeline->data_memory[address / 4];
        }
    }
    
    if (pipeline->ex_mem.mem_write) {
        u32 address = pipeline->ex_mem.alu_result;
        if (address < DATA_MEMORY_SIZE) {
            pipeline->data_memory[address / 4] = pipeline->ex_mem.write_data;
        }
    }
    
    // 填充MEM/WB寄存器
    pipeline->mem_wb.reg_write = pipeline->ex_mem.reg_write;
    pipeline->mem_wb.mem_to_reg = pipeline->ex_mem.mem_to_reg;
    pipeline->mem_wb.read_data = read_data;
    pipeline->mem_wb.alu_result = pipeline->ex_mem.alu_result;
    pipeline->mem_wb.write_register = pipeline->ex_mem.write_register;
    pipeline->mem_wb.instruction = pipeline->ex_mem.instruction;
    pipeline->mem_wb.valid = true;
    
    if (pipeline->debug_mode) {
        printf("MEM: 存储器数据=0x%08X\n", read_data);
    }
}

void pipeline_stage_wb(pipeline_t *pipeline) {
    if (!pipeline || !pipeline->mem_wb.valid || !pipeline->mem_wb.reg_write) {
        return;
    }
    
    u32 write_data = pipeline->mem_wb.mem_to_reg ? 
                    pipeline->mem_wb.read_data : 
                    pipeline->mem_wb.alu_result;
    
    if (pipeline->mem_wb.write_register != 0) {
        pipeline->registers[pipeline->mem_wb.write_register] = write_data;
    }
    
    if (pipeline->debug_mode) {
        printf("WB: R%d = 0x%08X\n", 
               pipeline->mem_wb.write_register, write_data);
    }
}

/* ================================
 * 指令解析和辅助函数
 * ================================ */

instruction_t pipeline_decode_instruction(u32 inst_word) {
    instruction_t inst = {0};
    
    if (inst_word == 0) {
        inst.type = INST_NOP;
        inst.valid = false;
        return inst;
    }
    
    inst.instruction = inst_word;
    inst.opcode = (inst_word >> 26) & 0x3F;
    inst.rs = (inst_word >> 21) & 0x1F;
    inst.rt = (inst_word >> 16) & 0x1F;
    inst.rd = (inst_word >> 11) & 0x1F;
    inst.shamt = (inst_word >> 6) & 0x1F;
    inst.funct = inst_word & 0x3F;
    inst.immediate = inst_word & 0xFFFF;
    inst.address = inst_word & 0x3FFFFFF;
    inst.valid = true;
    
    // 简化的指令类型判断
    if (inst.opcode == 0) {
        inst.type = INST_R_TYPE;
    } else if (inst.opcode == 35) { // LW
        inst.type = INST_LOAD;
    } else if (inst.opcode == 43) { // SW  
        inst.type = INST_STORE;
    } else if (inst.opcode == 4) { // BEQ
        inst.type = INST_BRANCH;
    } else {
        inst.type = INST_I_TYPE;
    }
    
    return inst;
}

/* ================================
 * 冲突检测实现
 * ================================ */

hazard_detection_t pipeline_detect_data_hazard(const pipeline_t *pipeline) {
    hazard_detection_t hazard = {HAZARD_NONE, false, 0, "无冲突"};
    
    if (!pipeline || !pipeline->id_ex.valid || !pipeline->if_id.valid) {
        return hazard;
    }
    
    // 检测Load-Use冲突
    if (pipeline->id_ex.mem_read) {
        const instruction_t *current = &pipeline->if_id.instruction;
        if ((current->rs == pipeline->id_ex.instruction_20_16 && current->rs != 0) ||
            (current->rt == pipeline->id_ex.instruction_20_16 && current->rt != 0)) {
            hazard.type = HAZARD_DATA_RAW;
            hazard.stall_required = true;
            hazard.stall_cycles = 1;
            hazard.description = "Load-Use数据冲突";
        }
    }
    
    return hazard;
}

/* ================================
 * 分支预测实现
 * ================================ */

bool branch_predict(branch_predictor_t *predictor, u32 pc) {
    // 参数有效性检查
    if (!predictor) {
        return false;
    }
    
    // 预测表索引计算
    // 原理：使用PC的低8位作为预测表索引，实现快速查找
    // 地址映射：PC右移2位是因为指令4字节对齐，低2位总是00
    // 索引范围：0-255，对应256项预测表
    u8 index = (pc >> 2) & 0xFF;
    
    // 获取当前分支的预测状态
    branch_prediction_state_t state = predictor->state[index];
    
    // 预测决策逻辑
    // 原理：基于两位饱和计数器的预测策略
    // 预测规则：状态值2、3表示"跳转"，状态值0、1表示"不跳转"
    // 返回值：true表示预测跳转，false表示预测不跳转
    return (state == BRANCH_WEAK_TAKEN || state == BRANCH_STRONG_TAKEN);
}

void branch_update_predictor(branch_predictor_t *predictor, u32 pc, bool taken) {
    if (!predictor) {
        return;
    }
    
    u8 index = (pc >> 2) & 0xFF;
    branch_prediction_state_t *state = &predictor->state[index];
    
    if (taken) {
        if (*state < BRANCH_STRONG_TAKEN) {
            (*state)++;
        }
    } else {
        if (*state > BRANCH_STRONG_NOT_TAKEN) {
            (*state)--;
        }
    }
    
    predictor->total_branches++;
    
    bool predicted = (*state == BRANCH_WEAK_TAKEN || *state == BRANCH_STRONG_TAKEN);
    if (predicted == taken) {
        predictor->correct_predictions++;
    }
    
    if (predictor->total_branches > 0) {
        predictor->accuracy = (double)predictor->correct_predictions / predictor->total_branches;
    }
}

void branch_reset_predictor(branch_predictor_t *predictor) {
    if (!predictor) {
        return;
    }
    
    for (int i = 0; i < 256; i++) {
        predictor->state[i] = BRANCH_WEAK_NOT_TAKEN;
    }
    
    memset(predictor->btb, 0, sizeof(predictor->btb));
    predictor->total_branches = 0;
    predictor->correct_predictions = 0;
    predictor->accuracy = 0.0;
}

/* ================================
 * 性能统计实现
 * ================================ */

void pipeline_update_stats(pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    
    pipeline->stats.total_cycles++;
}

void pipeline_calculate_metrics(pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    
    if (pipeline->stats.total_instructions > 0) {
        pipeline->stats.cpi = (double)pipeline->stats.total_cycles / 
                             pipeline->stats.total_instructions;
        pipeline->stats.ipc = 1.0 / pipeline->stats.cpi;
        pipeline->stats.efficiency = (double)pipeline->stats.total_instructions / 
                                   (pipeline->stats.total_cycles * PIPELINE_STAGES) * 100.0;
    }
    
    if (pipeline->predictor.total_branches > 0) {
        pipeline->stats.branch_accuracy = pipeline->predictor.accuracy * 100.0;
    }
}

pipeline_stats_t pipeline_get_stats(const pipeline_t *pipeline) {
    if (!pipeline) {
        pipeline_stats_t empty = {0};
        return empty;
    }
    
    return pipeline->stats;
}

void pipeline_reset_stats(pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    
    memset(&pipeline->stats, 0, sizeof(pipeline_stats_t));
}

/* ================================
 * 调试和显示函数
 * ================================ */

void pipeline_print_state(const pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    
    printf("\n========== 流水线状态 ==========\n");
    printf("PC: 0x%08X, 时钟周期: %u\n", pipeline->pc, pipeline->clock_cycle);
    printf("IF/ID: %s\n", pipeline->if_id.valid ? "有效" : "无效");
    printf("ID/EX: %s\n", pipeline->id_ex.valid ? "有效" : "无效");
    printf("EX/MEM: %s\n", pipeline->ex_mem.valid ? "有效" : "无效");
    printf("MEM/WB: %s\n", pipeline->mem_wb.valid ? "有效" : "无效");
}

void pipeline_print_stats(const pipeline_t *pipeline) {
    if (!pipeline) {
        return;
    }
    
    printf("\n========== 性能统计 ==========\n");
    printf("总周期数: %llu\n", pipeline->stats.total_cycles);
    printf("总指令数: %llu\n", pipeline->stats.total_instructions);
    printf("平均CPI: %.3f\n", pipeline->stats.cpi);
    printf("每周期指令数(IPC): %.3f\n", pipeline->stats.ipc);
    printf("流水线效率: %.2f%%\n", pipeline->stats.efficiency);
    printf("分支预测准确率: %.2f%%\n", pipeline->stats.branch_accuracy);
}

void pipeline_set_debug_mode(pipeline_t *pipeline, bool debug) {
    if (pipeline) {
        pipeline->debug_mode = debug;
    }
}

const char* pipeline_instruction_type_to_string(instruction_type_t type) {
    switch (type) {
        case INST_R_TYPE: return "R型";
        case INST_I_TYPE: return "I型";
        case INST_LOAD: return "Load";
        case INST_STORE: return "Store";
        case INST_BRANCH: return "Branch";
        case INST_JUMP: return "Jump";
        case INST_NOP: return "NOP";
        default: return "未知";
    }
}

const char* pipeline_hazard_type_to_string(hazard_type_t type) {
    switch (type) {
        case HAZARD_NONE: return "无冲突";
        case HAZARD_STRUCTURAL: return "结构冲突";
        case HAZARD_DATA_RAW: return "RAW数据冲突";
        case HAZARD_DATA_WAR: return "WAR数据冲突";
        case HAZARD_DATA_WAW: return "WAW数据冲突";
        case HAZARD_CONTROL: return "控制冲突";
        default: return "未知冲突";
    }
} 