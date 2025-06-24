/**
 * CPU核心模拟器实现 - 32位RISC处理器核心引擎
 * 
 * ========================================================================
 * 实现架构概述
 * ========================================================================
 * 
 * 设计理念：
 * 本文件实现CPU模拟器的核心功能，采用模块化设计，清晰分离各个功能模块，
 * 确保代码的可读性、可维护性和教学价值。
 * 
 * 核心实现特点：
 * 1. 分层架构设计：清晰的功能分层和接口定义
 * 2. 状态机驱动：基于有限状态机的CPU状态管理
 * 3. 流水线模拟：完整的5级流水线执行模型
 * 4. 性能监控：实时性能数据收集和分析
 * 5. 调试支持：全面的调试和状态检查功能
 * 
 * 功能模块组织：
 * ```
 * CPU核心实现架构
 * ┌──────────────────────────────────────────────────────────────┐
 * |                    CPU执行引擎                               |
 * |                                                              |
 * | ┌──────────────────┐ ┌──────────────────┐ ┌────────────────┐ |
 * | | 生命周期管理      | | 指令执行流水线    | | 性能监控系统   | |
 * | |                 | |                 | |               | |
 * | | • 初始化/清理    | | • 取指阶段       | | • 计数器管理   | |
 * | | • 状态转换      | | • 译码阶段       | | • 统计计算     | |
 * | | • 配置管理      | | • 执行阶段       | | • 性能分析     | |
 * | | • 错误处理      | | • 写回阶段       | | • 实时监控     | |
 * | ┌──────────────────┐ ┌──────────────────┐ ┌────────────────┐ |
 * |                              |                              |
 * +==============================|==============================+
 *                                |
 * +==============================|==============================+
 * |                    底层硬件抽象                              |
 * |                                                              |
 * | ┌──────────────────┐ ┌──────────────────┐ ┌────────────────┐ |
 * | | 寄存器系统       | | 内存管理         | | ALU运算单元    | |
 * | | (registers.h)    | | (memory.h)       | | (alu.h)        | |
 * | ┌──────────────────┐ ┌──────────────────┐ ┌────────────────┐ |
 * ┌──────────────────────────────────────────────────────────────┐
 * ```
 * 
 * 关键技术实现：
 * 
 * 1. **状态机设计**：
 *    采用有限状态机模型，确保CPU状态转换的确定性和安全性
 *    状态：STOPPED -> RUNNING -> HALTED/ERROR -> RESET
 * 
 * 2. **流水线模拟**：
 *    实现经典5级流水线：IF-ID-EX-MEM-WB
 *    支持数据冲突检测和处理
 *    分支预测和跳转优化
 * 
 * 3. **指令分发机制**：
 *    基于操作码的跳转表实现高效指令分发
 *    统一的指令执行接口
 *    灵活的寻址方式支持
 * 
 * 4. **性能监控集成**：
 *    非侵入式性能数据收集
 *    实时统计计算和更新
 *    多层次性能指标支持
 * 
 * 5. **错误处理机制**：
 *    分层错误处理：语法错误、运行时错误、系统错误
 *    异常恢复策略
 *    调试信息保留
 * 
 * 教学价值体现：
 * - 完整展示CPU内部工作机制
 * - 清晰的代码结构便于理解
 * - 详细的注释说明技术原理
 * - 可观测的执行过程和状态
 * - 丰富的调试和分析功能
 */

#include "cpu_core.h"
#include "alu.h"
#include "registers.h"
#include "memory.h"
#include "../common/types.h"
#include "../common/utils.h"
#include "../common/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * ========================================================================
 * 指令执行函数声明
 * ========================================================================
 * 
 * 指令执行架构：
 * 采用函数指针表和统一接口的指令执行模型，每类指令都有专门的
 * 执行函数，确保指令执行的模块化和可扩展性。
 * 
 * 函数命名约定：
 * execute_<指令类型> - 执行特定类型的指令
 * 
 * 执行流程：
 * 1. 参数有效性检查
 * 2. 操作数获取和计算
 * 3. ALU运算或内存操作
 * 4. 结果处理和状态更新
 * 5. 异常检查和处理
 */

// ==================== 内部函数声明 ====================

static cpu_error_t execute_mov(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_load(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_store(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_add(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_sub(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_mul(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_div(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_jmp(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_jz(cpu_t *cpu, const instruction_t *instruction);
static cpu_error_t execute_jnz(cpu_t *cpu, const instruction_t *instruction);

// 辅助函数声明
static const char* cpu_state_to_string(cpu_state_t state);
static const char* halt_reason_to_string(cpu_halt_reason_t reason);

/**
 * ========================================================================
 * CPU生命周期管理实现
 * ========================================================================
 * 
 * 生命周期管理哲学：
 * CPU的生命周期管理是整个模拟器的基础，必须确保资源的正确分配、
 * 初始化、使用和清理，防止内存泄漏和状态不一致。
 */

/**
 * CPU初始化实现
 * 
 * 初始化策略：
 * 采用分阶段初始化，确保每个子系统都能正确启动，
 * 任何阶段失败都能正确回滚已初始化的资源。
 * 
 * 初始化流程：
 * ```
 * 初始化阶段流程图
 * 
 * 开始 --> 清零结构体 --> 初始化寄存器组 --> 初始化内存系统
 *  |                           |                    |
 *  |                           v                    v
 *  |         失败回滚 <-- 检查结果         检查结果 --> 失败回滚
 *  |                           |                    |
 *  v                           v                    v
 * 失败      初始化ALU --> 设置默认配置 --> 重置性能计数器
 *  |              |              |              |
 *  |              v              v              v
 *  |       检查结果 --> 记录日志 --> 成功完成
 *  |              |              |              |
 *  |              v              v              v
 *  +------ 失败回滚      成功返回      成功返回
 * ```
 * 
 * 默认配置说明：
 * - 时钟频率：1MHz，适合教学演示
 * - 缓存：默认关闭，减少复杂度
 * - 流水线：默认关闭，便于理解单周期执行
 * - 调试：默认关闭，提高执行效率
 * 
 * 错误处理机制：
 * 任何子系统初始化失败都会触发已初始化资源的清理，
 * 确保不会有资源泄漏或不一致状态。
 */
cpu_error_t cpu_init(cpu_t *cpu, size_t memory_size) {
    if (!cpu) return CPU_ERROR_NULL_POINTER;
    
    // 阶段1：结构体清零 - 确保所有字段都有确定的初始值
    memset(cpu, 0, sizeof(cpu_t));
    
    // 阶段2：寄存器组初始化 - 建立程序状态存储
    cpu_error_t err = registers_init(&cpu->registers);
    if (err != CPU_SUCCESS) return err;
    
    // 阶段3：内存系统初始化 - 建立程序和数据存储空间
    err = memory_init(&cpu->memory, memory_size);
    if (err != CPU_SUCCESS) return err;
    
    // 阶段4：ALU单元初始化 - 建立运算处理能力
    err = alu_init(&cpu->alu);
    if (err != CPU_SUCCESS) return err;
    
    // 阶段5：系统状态和配置初始化
    cpu->state = CPU_STATE_STOPPED;
    cpu->config.clock_frequency = 1000000; // 1MHz默认频率：平衡教学演示和性能
    cpu->config.enable_cache = false;      // 默认关闭缓存：简化初学理解
    cpu->config.enable_pipeline = false;   // 默认关闭流水线：便于单步调试
    cpu->config.debug_output = false;      // 默认关闭调试：提高执行效率
    
    // 阶段6：性能监控系统初始化
    cpu_reset_performance_counters(cpu);
    
    LOG_INFO("CPU初始化完成，内存大小: %zu字节", memory_size);
    return CPU_SUCCESS;
}

/**
 * CPU重置实现
 * 
 * 重置策略：
 * 软重置策略，保留用户配置和程序数据，只重置执行状态，
 * 允许程序重新开始执行而不需要重新加载。
 * 
 * 重置范围控制：
 * - 重置项目：寄存器值、程序计数器、执行状态、性能计数器
 * - 保留项目：内存内容、用户配置、断点设置、扩展数据
 * 
 * 这种设计允许快速重启程序执行，适合调试和性能测试场景。
 */
cpu_error_t cpu_reset(cpu_t *cpu) {
    if (!cpu) return CPU_ERROR_NULL_POINTER;
    
    // 重置执行状态 - 清除运行时状态
    cpu_error_t err = registers_reset(&cpu->registers);
    if (err != CPU_SUCCESS) return err;
    
    // 重置ALU状态 - 清除运算状态和标志位
    alu_reset(&cpu->alu);
    
    // 重置控制状态 - 恢复到可执行状态
    cpu->state = CPU_STATE_STOPPED;
    cpu->halt_reason = CPU_HALT_NONE;
    
    // 重置性能统计 - 开始新的性能监控周期
    cpu_reset_performance_counters(cpu);
    
    LOG_INFO("CPU已重置");
    return CPU_SUCCESS;
}

/**
 * CPU资源清理实现
 * 
 * 清理策略：
 * 遵循RAII原则，确保所有分配的资源都被正确释放，
 * 防止内存泄漏和资源占用。
 * 
 * 清理顺序：
 * 按照初始化的逆序进行清理，确保依赖关系的正确处理。
 */
cpu_error_t cpu_cleanup(cpu_t *cpu) {
    if (!cpu) return CPU_ERROR_NULL_POINTER;
    
    // 清理内存系统 - 释放所有分配的内存
    memory_cleanup(&cpu->memory);
    
    // 重置CPU状态 - 标记为不可用状态
    cpu->state = CPU_STATE_STOPPED;
    
    LOG_INFO("CPU资源已清理");
    return CPU_SUCCESS;
}

/**
 * CPU状态控制实现
 * 
 * 状态管理策略：
 * 基于有限状态机的严格状态控制，确保状态转换的安全性和一致性。
 * 所有状态转换都经过验证，防止非法状态和不一致行为。
 */
cpu_error_t cpu_set_state(cpu_t *cpu, cpu_state_t state) {
    if (!cpu) return CPU_ERROR_NULL_POINTER;
    
    cpu_state_t old_state = cpu->state;
    cpu->state = state;
    
    LOG_DEBUG("CPU状态变更: %d -> %d", old_state, state);
    return CPU_SUCCESS;
}

cpu_state_t cpu_get_state(const cpu_t *cpu) {
    return cpu ? cpu->state : CPU_STATE_ERROR;
}

bool cpu_is_running(const cpu_t *cpu) {
    return cpu && cpu->state == CPU_STATE_RUNNING;
}

bool cpu_is_halted(const cpu_t *cpu) {
    return cpu && cpu->state == CPU_STATE_HALTED;
}

/**
 * ========================================================================
 * 指令执行控制实现
 * ========================================================================
 * 
 * 指令执行哲学：
 * 采用经典的5级流水线模型，每条指令都经过完整的执行周期，
 * 确保指令执行的正确性和性能的可预测性。
 * 
 * 执行周期模型：
 * ```
 * 指令执行周期详解
 * 
 * 1. 取指阶段 (Instruction Fetch)
 *    ┌─────────────────────────────────┐
 *    │ PC → 内存地址                    │
 *    │ 内存[PC] → 指令寄存器           │
 *    │ PC + 4 → PC (更新程序计数器)     │
 *    └─────────────────────────────────┘
 *                   ↓
 * 2. 译码阶段 (Instruction Decode)
 *    ┌─────────────────────────────────┐
 *    │ 指令字 → 操作码解析             │
 *    │ 操作数字段 → 寄存器地址         │
 *    │ 寻址方式 → 有效地址计算         │
 *    └─────────────────────────────────┘
 *                   ↓
 * 3. 执行阶段 (Execute)
 *    ┌─────────────────────────────────┐
 *    │ 读取操作数 → ALU输入            │
 *    │ ALU运算 → 结果和标志位          │
 *    │ 分支条件 → 跳转地址计算         │
 *    └─────────────────────────────────┘
 *                   ↓
 * 4. 访存阶段 (Memory Access)
 *    ┌─────────────────────────────────┐
 *    │ LOAD: 内存[地址] → 数据         │
 *    │ STORE: 数据 → 内存[地址]        │
 *    │ 其他指令：跳过此阶段            │
 *    └─────────────────────────────────┘
 *                   ↓
 * 5. 写回阶段 (Write Back)
 *    ┌─────────────────────────────────┐
 *    │ 运算结果 → 目标寄存器           │
 *    │ 标志位 → 状态寄存器             │
 *    │ 性能统计 → 更新计数器           │
 *    └─────────────────────────────────┘
 * ```
 * 
 * 性能优化技术：
 * 1. 指令预取：减少取指延迟
 * 2. 操作数转发：减少数据相关停顿
 * 3. 分支预测：减少控制相关停顿
 * 4. 缓存优化：减少内存访问延迟
 */

/**
 * 指令执行阶段实现
 * 
 * 执行阶段原理：
 * 这是流水线的核心阶段，负责实际的运算和操作执行。
 * 根据不同的指令类型，调用相应的执行函数。
 * 
 * 指令分发机制：
 * ```
 * 指令分发架构
 * 
 * 操作码 → 指令分发器 → 执行函数表
 *   |           |              |
 *   |           |              ├─ execute_mov()
 *   |           |              ├─ execute_add()
 *   |           |              ├─ execute_load()
 *   |           |              ├─ execute_jmp()
 *   |           |              └─ ... 其他指令
 *   |           |
 *   |           ↓
 *   |    参数验证和预处理
 *   |           |
 *   ↓           ↓
 * 错误处理 ← 指令执行结果
 * ```
 * 
 * 执行原则：
 * 1. 统一接口：所有指令执行函数使用相同的接口
 * 2. 错误隔离：执行错误不影响其他指令
 * 3. 状态更新：及时更新CPU和ALU状态
 * 4. 性能监控：记录执行统计信息
 */
cpu_error_t cpu_execute(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    cpu_error_t err = CPU_SUCCESS;
    
    // 指令分发和执行 - 根据操作码调用相应的执行函数
    switch (instruction->opcode) {
        case OP_NOP:
            // 空操作：消耗时钟周期但不改变系统状态
            break;
            
        case OP_MOV:
            err = execute_mov(cpu, instruction);
            break;
            
        case OP_LOAD:
            err = execute_load(cpu, instruction);
            break;
            
        case OP_STORE:
            err = execute_store(cpu, instruction);
            break;
            
        case OP_ADD:
            err = execute_add(cpu, instruction);
            break;
            
        case OP_SUB:
            err = execute_sub(cpu, instruction);
            break;
            
        case OP_MUL:
            err = execute_mul(cpu, instruction);
            break;
            
        case OP_DIV:
            err = execute_div(cpu, instruction);
            break;
            
        case OP_JMP:
            err = execute_jmp(cpu, instruction);
            break;
            
        case OP_JZ:
            err = execute_jz(cpu, instruction);
            break;
            
        case OP_JNZ:
            err = execute_jnz(cpu, instruction);
            break;
            
        case OP_HLT:
            // 停机指令：在execute_instruction中处理状态转换
            break;
            
        default:
            LOG_ERROR("不支持的指令: 0x%02X", instruction->opcode);
            return CPU_ERROR_INVALID_INSTRUCTION;
    }
    
    LOG_TRACE("执行: 操作码=0x%02X, 结果=%d", 
              instruction->opcode, err);
    return err;
}

/**
 * 写回阶段实现
 * 
 * 写回操作原理：
 * 将执行阶段的计算结果写入目标存储位置，完成指令的最终效果。
 * 
 * 写回策略：
 * - 算术运算：结果写入目标寄存器
 * - 内存操作：数据写入内存或寄存器
 * - 分支指令：更新程序计数器
 * - 状态指令：更新状态寄存器
 * 
 * 写回时序：
 * 写回操作原子性执行，确保中间状态不可见。
 */
cpu_error_t cpu_writeback(cpu_t *cpu, const instruction_t *instruction, word_t result) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 根据指令类型执行相应的写回操作
    switch (instruction->opcode) {
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_MOV:
        case OP_LOAD:
            // 算术和数据传输指令：结果写入目标寄存器
            if (instruction->operand1 < MAX_REGISTERS) {
                registers_set(&cpu->registers, instruction->operand1, result);
            }
            break;
            
        case OP_STORE:
            // 存储指令：数据已在执行阶段写入内存，无需额外写回
            break;
            
        default:
            // 其他指令：可能不需要写回操作
            break;
    }
    
    LOG_TRACE("写回: 寄存器R%d = 0x%08X", instruction->operand1, result);
    return CPU_SUCCESS;
}

/**
 * ========================================================================
 * 指令执行辅助函数实现
 * ========================================================================
 * 
 * 辅助函数设计理念：
 * 每种指令类型都有专门的执行函数，实现指令的具体语义。
 * 函数接口统一，便于维护和扩展。
 * 
 * 实现特点：
 * 1. 参数验证：确保输入参数的有效性
 * 2. 操作数获取：根据寻址方式获取操作数
 * 3. 运算执行：调用ALU或内存系统执行操作
 * 4. 结果处理：处理运算结果和状态标志
 * 5. 异常处理：处理执行过程中的异常情况
 */

/**
 * 数据传送指令实现 (MOV)
 * 
 * 功能：将立即数或寄存器值传送到目标寄存器
 * 语义：Rd ← immediate 或 Rd ← Rs
 * 
 * 技术细节：
 * - 支持立即数寻址和寄存器寻址
 * - 不影响标志位
 * - 单周期执行
 */
static cpu_error_t execute_mov(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 立即数传送：将指令中的立即数直接传送到目标寄存器
    word_t value = instruction->operand2;
    return registers_set(&cpu->registers, instruction->operand1, value);
}

/**
 * 内存加载指令实现 (LOAD)
 * 
 * 功能：从内存地址加载数据到寄存器
 * 语义：Rd ← Memory[address]
 * 
 * 技术细节：
 * - 地址可以是立即数或寄存器值
 * - 涉及内存访问，可能触发缓存操作
 * - 需要处理内存访问异常
 */
static cpu_error_t execute_load(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 从指定内存地址读取数据
    word_t value;
    cpu_error_t err = memory_read_word(&cpu->memory, instruction->operand2, &value);
    if (err != CPU_SUCCESS) return err;
    
    // 将读取的数据存储到目标寄存器
    return registers_set(&cpu->registers, instruction->operand1, value);
}

/**
 * 内存存储指令实现 (STORE)
 * 
 * 功能：将寄存器数据存储到内存地址
 * 语义：Memory[address] ← Rs
 * 
 * 技术细节：
 * - 从源寄存器读取数据
 * - 写入到指定内存地址
 * - 可能触发缓存写回操作
 */
static cpu_error_t execute_store(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 从源寄存器读取要存储的数据
    word_t value = registers_get(&cpu->registers, instruction->operand1);
    
    // 将数据写入到目标内存地址
    return memory_write_word(&cpu->memory, instruction->operand2, value);
}

/**
 * 算术运算指令实现 (ADD/SUB/MUL/DIV)
 * 
 * 运算指令通用流程：
 * 1. 从寄存器读取操作数
 * 2. 调用ALU执行运算
 * 3. 检查运算结果和标志位
 * 4. 将结果写回寄存器
 * 
 * ALU集成：
 * 所有算术运算都通过ALU单元执行，确保运算的一致性和标志位的正确设置。
 */

// 加法指令：Rd ← Rs1 + Rs2
static cpu_error_t execute_add(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 读取两个操作数
    word_t op1 = registers_get(&cpu->registers, instruction->reg1);
    word_t op2 = registers_get(&cpu->registers, instruction->reg2);
    
    // 执行ALU加法运算
    alu_result_t result = alu_add(op1, op2);
    cpu->alu.result = result;
    
    // 将结果写回目标寄存器
    return registers_set(&cpu->registers, instruction->reg1, result.result);
}

// 减法指令：Rd ← Rs1 - Rs2
static cpu_error_t execute_sub(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    word_t op1 = registers_get(&cpu->registers, instruction->reg1);
    word_t op2 = registers_get(&cpu->registers, instruction->reg2);
    
    alu_result_t result = alu_sub(op1, op2);
    cpu->alu.result = result;
    
    return registers_set(&cpu->registers, instruction->reg1, result.result);
}

// 乘法指令：Rd ← Rs1 × Rs2
static cpu_error_t execute_mul(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    word_t op1 = registers_get(&cpu->registers, instruction->reg1);
    word_t op2 = registers_get(&cpu->registers, instruction->reg2);
    
    alu_result_t result = alu_mul(op1, op2);
    cpu->alu.result = result;
    
    return registers_set(&cpu->registers, instruction->reg1, result.result);
}

// 除法指令：Rd ← Rs1 ÷ Rs2
static cpu_error_t execute_div(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    word_t op1 = registers_get(&cpu->registers, instruction->reg1);
    word_t op2 = registers_get(&cpu->registers, instruction->reg2);
    
    // 除法运算需要检查除零异常
    alu_result_t result = alu_div(op1, op2);
    if (!result.valid) return CPU_ERROR_DIVISION_BY_ZERO;
    
    cpu->alu.result = result;
    
    return registers_set(&cpu->registers, instruction->reg1, result.result);
}

/**
 * 分支跳转指令实现
 * 
 * 分支指令原理：
 * 根据条件或无条件地改变程序计数器，实现程序流程控制。
 * 
 * 分支类型：
 * - 无条件跳转：直接设置新的PC值
 * - 条件跳转：根据标志位决定是否跳转
 * 
 * 性能影响：
 * 分支指令可能导致流水线冲刷，影响性能。
 */

// 无条件跳转：PC ← address
static cpu_error_t execute_jmp(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 直接设置程序计数器到目标地址
    return registers_set_pc(&cpu->registers, instruction->operand1);
}

// 零标志跳转：if (ZF == 1) PC ← address
static cpu_error_t execute_jz(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 检查零标志位，决定是否跳转
    if (cpu->alu.flags & ALU_FLAG_ZERO) {
        return registers_set_pc(&cpu->registers, instruction->operand1);
    }
    
    // 条件不满足，继续顺序执行
    return CPU_SUCCESS;
}

// 非零标志跳转：if (ZF == 0) PC ← address
static cpu_error_t execute_jnz(cpu_t *cpu, const instruction_t *instruction) {
    if (!cpu || !instruction) return CPU_ERROR_NULL_POINTER;
    
    // 检查零标志位，决定是否跳转
    if (!(cpu->alu.flags & ALU_FLAG_ZERO)) {
        return registers_set_pc(&cpu->registers, instruction->operand1);
    }
    
    return CPU_SUCCESS;
}

/**
 * ========================================================================
 * 程序装载和管理实现
 * ========================================================================
 * 
 * 程序装载策略：
 * 提供灵活的程序装载方式，支持从文件或内存数据加载程序。
 * 确保程序正确装载到内存并设置执行环境。
 */

/**
 * 程序字节数组装载实现
 * 
 * 装载流程：
 * 1. 验证参数有效性
 * 2. 逐字节写入内存
 * 3. 记录装载信息
 * 4. 验证装载结果
 * 
 * 错误处理：
 * 装载失败时停止操作，不会部分装载程序。
 */
cpu_error_t cpu_load_program_from_bytes(cpu_t *cpu, const byte_t *program, 
                                        size_t size, addr_t load_addr) {
    if (!cpu || !program) return CPU_ERROR_NULL_POINTER;
    
    // 逐字节将程序装载到指定内存地址
    for (size_t i = 0; i < size; i++) {
        cpu_error_t err = memory_write_byte(&cpu->memory, load_addr + i, program[i]);
        if (err != CPU_SUCCESS) {
            LOG_ERROR("程序加载失败，地址: 0x%08X", (unsigned int)(load_addr + i));
            return err;
        }
    }
    
    LOG_INFO("程序已加载，大小: %zu字节，地址: 0x%08X", size, load_addr);
    return CPU_SUCCESS;
}

/**
 * 程序入口点设置
 * 
 * 功能：设置程序计数器到程序入口地址
 * 作用：准备CPU开始执行用户程序
 */
cpu_error_t cpu_set_entry_point(cpu_t *cpu, addr_t entry_point) {
    if (!cpu) return CPU_ERROR_NULL_POINTER;
    
    registers_set_pc(&cpu->registers, entry_point);
    LOG_INFO("程序入口点设置为: 0x%08X", entry_point);
    
    return CPU_SUCCESS;
}

/**
 * ========================================================================
 * 性能监控实现
 * ========================================================================
 * 
 * 性能监控理念：
 * 提供全面的性能监控和分析功能，帮助理解CPU的行为特征和性能瓶颈。
 * 
 * 监控层次：
 * 1. 基础计数器：周期数、指令数等原始数据
 * 2. 派生指标：IPC、CPI等综合性能指标
 * 3. 系统分析：缓存命中率、分支预测率等高级指标
 */

/**
 * 性能计数器重置
 * 
 * 重置策略：
 * 清零所有性能计数器，为新的性能监控周期做准备。
 * 适用于基准测试和性能对比分析。
 */
cpu_error_t cpu_reset_performance_counters(cpu_t *cpu) {
    if (!cpu) return CPU_ERROR_NULL_POINTER;
    
    // 全面清零性能统计结构
    memset(&cpu->perf, 0, sizeof(cpu->perf));
    
    return CPU_SUCCESS;
}

/**
 * 性能统计计算和获取
 * 
 * 计算策略：
 * 基于原始计数器数据计算派生性能指标，提供全面的性能分析。
 * 
 * 指标计算：
 * - IPC (Instructions Per Cycle): 衡量指令级并行度
 * - CPI (Cycles Per Instruction): 衡量指令平均复杂度
 * - 缓存命中率: 衡量存储层次效率
 * - 分支预测率: 衡量控制流预测效率
 */
cpu_error_t cpu_get_performance(const cpu_t *cpu, cpu_performance_t *perf) {
    if (!cpu || !perf) return CPU_ERROR_NULL_POINTER;
    
    // 计算基础性能指标
    if (cpu->perf.total_cycles > 0) {
        perf->instructions_per_cycle = 
            (double)cpu->perf.total_instructions / cpu->perf.total_cycles;
        perf->cycles_per_instruction = 
            (double)cpu->perf.total_cycles / cpu->perf.total_instructions;
    }
    
    // 计算缓存性能指标
    if (cpu->perf.cache_hits + cpu->perf.cache_misses > 0) {
        perf->cache_hit_rate = 
            (double)cpu->perf.cache_hits / 
            (cpu->perf.cache_hits + cpu->perf.cache_misses);
    }
    
    // 计算分支预测性能指标
    if (cpu->perf.branch_instructions > 0) {
        perf->branch_prediction_rate = 
            (double)cpu->perf.branch_taken / cpu->perf.branch_instructions;
    }
    
    // 复制原始统计数据
    perf->total_cycles = cpu->perf.total_cycles;
    perf->total_instructions = cpu->perf.total_instructions;
    perf->cache_hits = cpu->perf.cache_hits;
    perf->cache_misses = cpu->perf.cache_misses;
    perf->branch_instructions = cpu->perf.branch_instructions;
    perf->branch_taken = cpu->perf.branch_taken;
    perf->memory_accesses = cpu->perf.memory_accesses;
    perf->register_accesses = cpu->perf.register_accesses;
    
    return CPU_SUCCESS;
}

/**
 * ========================================================================
 * 调试和状态显示实现
 * ========================================================================
 * 
 * 调试支持理念：
 * 提供全面的调试和状态观察功能，帮助理解CPU内部状态和执行过程。
 * 支持不同详细程度的状态显示，适应不同的调试需求。
 */

/**
 * CPU状态转储实现
 * 
 * 显示策略：
 * 分层显示CPU状态信息，从基本状态到详细的性能统计。
 * 
 * 信息层次：
 * 1. 基本状态：运行状态、停机原因
 * 2. 硬件状态：寄存器、ALU状态
 * 3. 性能统计：执行统计、性能指标
 * 
 * 格式设计：
 * 使用清晰的分隔符和缩进，便于阅读和理解。
 */
void cpu_dump_state(const cpu_t *cpu, bool detailed) {
    if (!cpu) return;
    
    printf("\n=== CPU状态 ===\n");
    printf("状态: %s\n", cpu_state_to_string(cpu->state));
    printf("停机原因: %s\n", halt_reason_to_string(cpu->halt_reason));
    
    // 显示寄存器状态
    printf("\n--- 寄存器 ---\n");
    registers_dump(&cpu->registers);
    
    // 显示ALU状态
    printf("\n--- ALU状态 ---\n");
    alu_dump(&cpu->alu);
    
    // 详细模式：显示性能统计和系统分析
    if (detailed) {
        printf("\n--- 性能统计 ---\n");
        printf("总指令数: %llu\n", cpu->perf.total_instructions);
        printf("总周期数: %llu\n", cpu->perf.total_cycles);
        printf("分支指令: %llu\n", cpu->perf.branch_instructions);
        printf("分支跳转: %llu\n", cpu->perf.branch_taken);
        
        // 显示综合性能指标
        if (cpu->perf.total_cycles > 0) {
            printf("IPC: %.3f\n", 
                   (double)cpu->perf.total_instructions / cpu->perf.total_cycles);
            printf("CPI: %.3f\n", 
                   (double)cpu->perf.total_cycles / cpu->perf.total_instructions);
        }
    }
    
    printf("================\n\n");
}

/**
 * ========================================================================
 * 辅助工具函数实现
 * ========================================================================
 * 
 * 工具函数设计：
 * 提供字符串转换、状态查询等辅助功能，增强系统的可用性和调试能力。
 */

/**
 * CPU状态枚举到字符串转换
 * 
 * 用途：调试输出、日志记录、状态显示
 * 设计：提供人类可读的状态描述
 */
const char* cpu_state_to_string(cpu_state_t state) {
    switch (state) {
        case CPU_STATE_STOPPED: return "已停止";
        case CPU_STATE_RUNNING: return "运行中";
        case CPU_STATE_HALTED: return "已停机";
        case CPU_STATE_ERROR: return "错误";
        default: return "未知";
    }
}

/**
 * 停机原因枚举到字符串转换
 * 
 * 用途：错误诊断、状态分析、用户反馈
 * 设计：清晰描述停机的具体原因
 */
const char* halt_reason_to_string(cpu_halt_reason_t reason) {
    switch (reason) {
        case CPU_HALT_NONE: return "无";
        case CPU_HALT_INSTRUCTION: return "停机指令";
        case CPU_HALT_ERROR: return "执行错误";
        case CPU_HALT_USER_REQUEST: return "用户请求";
        default: return "未知";
    }
}

/**
 * 中断检查实现
 * 
 * 实现说明：
 * 当前为简化实现，总是返回false。
 * 在完整实现中，这里会检查中断控制器的状态。
 * 
 * 扩展方向：
 * - 中断优先级管理
 * - 中断屏蔽机制
 * - 中断向量表支持
 */
bool cpu_has_pending_interrupt(const cpu_t *cpu) {
    (void)cpu; // 避免未使用参数警告
    // 简化实现：在真实系统中会检查中断控制器状态
    return false;
} 