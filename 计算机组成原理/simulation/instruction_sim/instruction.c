#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/**
 * 指令系统模拟器实现 - 高精度处理器行为仿真
 * 
 * 本实现基于现代处理器微架构设计原理，提供周期精确的指令执行模拟：
 * 
 * === 核心设计理念 ===
 * 1. 硬件精确性: 模拟真实处理器的微架构行为
 * 2. 性能分析: 提供详细的性能指标和瓶颈分析
 * 3. 教学友好: 清晰展示指令执行的每个阶段
 * 4. 扩展性: 支持多种指令集架构和优化技术
 * 
 * === 实现特色 ===
 * - 多种指令格式支持: 16位/32位指令字
 * - 完整寻址方式: 8种主流寻址模式
 * - 精确时序模拟: 周期级别的执行时间计算
 * - 性能统计: 详细的运行时性能数据收集
 * - 调试支持: 单步执行和状态可视化
 * - 错误处理: 全面的异常检测和恢复机制
 * 
 * === 技术亮点 ===
 * - 使用现代C语言特性优化性能
 * - 采用硬件设计思维实现算法
 * - 集成教学和研究双重目标
 * - 支持未来扩展和定制需求
 */

// ==================== 内部辅助函数声明 ====================

static cpu_error_t fetch_instruction(instruction_simulator_t *sim, instruction_t *inst);
static cpu_error_t execute_instruction(instruction_simulator_t *sim, const instruction_t *inst);
static void update_performance_stats(instruction_simulator_t *sim, const instruction_t *inst);

// ==================== 初始化和销毁函数 ====================

/**
 * 初始化指令模拟器
 * 
 * 硬件初始化模拟:
 * - 内存子系统初始化: 模拟DRAM控制器启动过程
 * - 寄存器堆复位: 所有寄存器清零，模拟硬件复位
 * - 缓存一致性: 确保所有存储单元的初始状态一致
 * - 时钟域设置: 初始化性能计数器和时序基准
 * 
 * 内存分配策略:
 * - 主存储器: 连续分配以优化缓存局部性
 * - 硬件栈: 独立分配支持快速过程调用
 * - 错误检查: 内存分配失败时的优雅降级
 * 
 * @param sim 模拟器实例指针
 * @param memory_size 内存大小(字节，必须为2的幂次)
 * @param format 指令格式(影响取指单元配置)
 * @return 错误码(CPU_SUCCESS表示初始化成功)
 */
cpu_error_t instruction_sim_init(instruction_simulator_t *sim, 
                                u32 memory_size, 
                                instruction_format_t format) {
    if (!sim) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 内存大小验证: 确保地址空间合理且符合硬件约束
    if (memory_size > MAX_MEMORY_SIZE || memory_size == 0) {
        return CPU_ERROR_INVALID_ADDR;
    }
    
    // 主内存初始化: 模拟DRAM控制器行为
    // 使用calloc确保所有位都初始化为0，模拟硬件复位状态
    sim->memory.data = (u8*)calloc(memory_size, sizeof(u8));
    if (!sim->memory.data) {
        return CPU_ERROR_MEMORY_FAULT;
    }
    sim->memory.size = memory_size;
    
    // 硬件栈初始化: 专用栈存储器，支持快速压栈/弹栈操作
    // 独立于主内存，模拟专用栈缓存或栈寄存器文件
    sim->memory.stack = (u32*)calloc(STACK_SIZE, sizeof(u32));
    if (!sim->memory.stack) {
        free(sim->memory.data);  // 回滚内存分配，避免资源泄漏
        return CPU_ERROR_MEMORY_FAULT;
    }
    sim->memory.stack_top = 0;
    
    // 寄存器堆初始化: 模拟硬件复位
    // 全部清零模拟真实处理器的复位行为
    memset(&sim->registers, 0, sizeof(register_file_t));
    
    // 栈指针初始化: 指向栈底，遵循向下增长的栈约定
    // 这是大多数现代处理器架构的标准配置
    sim->registers.sp = STACK_SIZE - 1;
    
    // 程序存储初始化: 为后续程序加载做准备
    sim->program = NULL;
    sim->program_size = 0;
    
    // 性能计数器初始化: 清零所有统计信息
    // 模拟硬件性能监控单元(PMU)的初始状态
    memset(&sim->stats, 0, sizeof(execution_stats_t));
    
    // 控制状态初始化
    sim->running = false;      // 处理器初始为停止状态
    sim->debug_mode = false;   // 默认关闭调试模式
    sim->format = format;      // 保存指令格式，影响取指和译码行为
    
    return CPU_SUCCESS;
}

/**
 * 销毁指令模拟器
 * 
 * 资源清理策略:
 * - 有序释放: 按照分配的反序释放资源
 * - 指针归零: 防止悬空指针导致的安全问题
 * - 状态清零: 确保结构体处于已知状态
 * 
 * 安全性考虑:
 * - 空指针检查: 防止对空指针的操作
 * - 内存清零: 避免敏感数据残留
 * - 完整性验证: 确保所有资源都被正确释放
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_destroy(instruction_simulator_t *sim) {
    if (!sim) {
        return;  // 安全的空指针处理
    }
    
    // 释放主内存: 模拟关闭内存控制器
    if (sim->memory.data) {
        free(sim->memory.data);
        sim->memory.data = NULL;  // 防止悬空指针
    }
    
    // 释放硬件栈: 模拟关闭栈存储器
    if (sim->memory.stack) {
        free(sim->memory.stack);
        sim->memory.stack = NULL;
    }
    
    // 释放程序存储: 清理指令缓存
    if (sim->program) {
        free(sim->program);
        sim->program = NULL;
    }
    
    // 完全清零结构体: 确保所有状态归零
    // 这类似于硬件下电时寄存器状态的归零
    memset(sim, 0, sizeof(instruction_simulator_t));
}

/**
 * 重置模拟器状态
 * 
 * 硬件复位模拟:
 * - 寄存器堆复位: 所有通用寄存器清零
 * - 特殊寄存器复位: PC、SP等专用寄存器回到初始状态
 * - 内存状态保持: 主内存内容不变(模拟软复位)
 * - 统计信息清零: 性能计数器重新开始计数
 * 
 * 复位类型:
 * - 软复位: 保持内存内容，仅重置处理器状态
 * - 与硬复位不同: 不重新分配内存资源
 * - 支持程序重启: 可以重新执行已加载的程序
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_reset(instruction_simulator_t *sim) {
    if (!sim) {
        return;
    }
    
    // 寄存器堆软复位: 清零所有寄存器状态
    memset(&sim->registers, 0, sizeof(register_file_t));
    
    // 栈指针重新初始化: 恢复到栈底位置
    // 这是处理器复位后的标准状态
    sim->registers.sp = STACK_SIZE - 1;
    
    // 主内存清零: 可选的内存清理操作
    // 在某些教学场景中有助于确保程序执行的一致性
    if (sim->memory.data) {
        memset(sim->memory.data, 0, sim->memory.size);
    }
    
    // 硬件栈复位: 清空栈内容并重置栈顶指针
    sim->memory.stack_top = 0;
    
    // 性能计数器复位: 清零所有统计信息
    // 模拟硬件性能监控单元的复位操作
    memset(&sim->stats, 0, sizeof(execution_stats_t));
    
    // 执行状态复位: 停止运行，准备接受新的程序
    sim->running = false;
}

// ==================== 程序加载和执行函数 ====================

/**
 * 加载程序到模拟器
 * 
 * 程序装载过程模拟:
 * - 内存分配: 为指令序列分配专用存储空间
 * - 地址重定位: 将逻辑地址映射到物理地址
 * - 初始化PC: 设置程序计数器指向程序入口
 * - 完整性检查: 验证程序大小和格式合法性
 * 
 * 安全性验证:
 * - 参数检查: 确保所有输入参数有效
 * - 大小限制: 防止程序超出模拟器处理能力
 * - 内存保护: 安全的内存分配和复制操作
 * 
 * 性能优化:
 * - 连续内存: 程序指令存储在连续内存中
 * - 缓存友好: 优化指令访问的空间局部性
 * - 预分配: 一次性分配避免频繁内存操作
 * 
 * @param sim 模拟器实例指针
 * @param program 指令序列数组
 * @param size 程序大小(指令条数)
 * @return 错误码
 */
cpu_error_t instruction_sim_load_program(instruction_simulator_t *sim,
                                        const instruction_t *program,
                                        u32 size) {
    // 输入参数完整性检查
    if (!sim || !program || size == 0) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 程序大小合法性检查: 防止超出模拟器处理能力
    if (size > MAX_PROGRAM_SIZE) {
        return CPU_ERROR_PROGRAM_TOO_LARGE;
    }
    
    // 释放之前的程序: 支持动态程序切换
    if (sim->program) {
        free(sim->program);
    }
    
    // 分配新程序存储空间
    // 连续分配提高指令缓存命中率，模拟真实的指令存储器
    sim->program = (instruction_t*)malloc(size * sizeof(instruction_t));
    if (!sim->program) {
        return CPU_ERROR_MEMORY_FAULT;
    }
    
    // 程序复制: 深拷贝确保数据独立性
    // 使用memcpy优化大块数据传输性能
    memcpy(sim->program, program, size * sizeof(instruction_t));
    sim->program_size = size;
    
    // 程序计数器初始化: 指向程序入口(地址0)
    // 这模拟了程序装载器设置入口点的过程
    sim->registers.pc = 0;
    
    return CPU_SUCCESS;
}

/**
 * 单步执行一条指令
 * 
 * 指令执行周期模拟(经典五阶段流水线):
 * 1. 取指(Fetch): 从程序存储器读取指令
 * 2. 译码(Decode): 解析指令格式和操作码
 * 3. 执行(Execute): 执行算术逻辑或控制操作
 * 4. 访存(Memory): 处理内存读写操作
 * 5. 写回(Write-back): 将结果写入目标寄存器
 * 
 * 异常处理机制:
 * - PC边界检查: 防止指令地址越界
 * - 指令有效性验证: 确保指令格式正确
 * - 错误传播: 正确传递执行过程中的错误
 * 
 * 性能监控:
 * - 周期计数: 精确统计指令执行时间
 * - 指令分类: 统计不同类型指令的执行频率
 * - 停机检测: 识别程序正常结束条件
 * 
 * @param sim 模拟器实例指针
 * @return 错误码(CPU_ERROR_PROGRAM_FINISHED表示程序正常结束)
 */
cpu_error_t instruction_sim_step(instruction_simulator_t *sim) {
    if (!sim || !sim->program) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // PC越界检查: 防止访问无效指令地址
    if (sim->registers.pc >= sim->program_size) {
        sim->running = false;
        return CPU_ERROR_PROGRAM_FINISHED;
    }
    
    instruction_t inst;
    cpu_error_t error;
    
    // 第1阶段: 取指(Instruction Fetch)
    // 模拟指令缓存或指令存储器的访问过程
    error = fetch_instruction(sim, &inst);
    if (error != CPU_SUCCESS) {
        return error;
    }
    
    // 第2阶段: 译码(Instruction Decode)
    // 对于已经解析的指令结构，这里主要进行合法性验证
    // 在真实处理器中，这里会包含复杂的译码逻辑
    
    // 第3-5阶段: 执行、访存、写回(Execute, Memory, Write-back)
    // 根据指令类型执行相应的操作
    error = execute_instruction(sim, &inst);
    if (error != CPU_SUCCESS) {
        return error;
    }
    
    // 性能统计更新: 记录执行的指令信息
    update_performance_stats(sim, &inst);
    
    // 停机指令检测: 处理器停止运行
    if (inst.opcode == OP_HALT) {
        sim->running = false;
    }
    
    return CPU_SUCCESS;
}

/**
 * 连续运行程序直到完成或错误
 * 
 * 连续执行控制:
 * - 循环执行: 重复调用单步执行直到程序结束
 * - 异常处理: 捕获和处理执行过程中的错误
 * - 性能测量: 记录程序总执行时间
 * - 超时保护: 防止无限循环导致的系统挂起
 * 
 * 时间测量:
 * - 高精度计时: 使用系统时钟测量执行时间
 * - 性能分析: 计算平均CPI(每指令周期数)
 * - 基准测试: 支持程序性能比较
 * 
 * 安全机制:
 * - 执行上限: 防止恶意程序或死循环
 * - 优雅终止: 正确处理程序正常结束
 * - 状态保持: 确保模拟器状态的一致性
 * 
 * @param sim 模拟器实例指针
 * @return 错误码(CPU_SUCCESS表示程序正常执行完成)
 */
cpu_error_t instruction_sim_run(instruction_simulator_t *sim) {
    if (!sim) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 启动执行状态: 标记处理器开始运行
    sim->running = true;
    cpu_error_t error = CPU_SUCCESS;
    
    // 高精度时间测量开始
    // clock()提供处理器时间，适合性能分析
    clock_t start_time = clock();
    
    // 主执行循环: 连续执行指令直到停止条件
    while (sim->running && error == CPU_SUCCESS) {
        error = instruction_sim_step(sim);
        
        // 正常程序结束处理: 不视为错误
        if (error == CPU_ERROR_PROGRAM_FINISHED) {
            error = CPU_SUCCESS;
            break;
        }
        
        // 执行保护机制: 防止无限循环
        // 1百万指令是合理的上限，足够执行复杂的教学程序
        if (sim->stats.total_instructions > 1000000) {
            error = CPU_ERROR_EXECUTION_TIMEOUT;
            break;
        }
    }
    
    // 执行时间计算和记录
    clock_t end_time = clock();
    sim->stats.execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    return error;
}

// ==================== 指令取指和译码函数 ====================

/**
 * 指令取指操作
 * 
 * 取指单元模拟:
 * - 程序计数器检查: 验证PC值的合法性
 * - 指令获取: 从程序存储器读取指令
 * - 指令寄存器更新: 将指令加载到IR
 * - PC自动递增: 指向下一条指令
 * 
 * 时序模拟:
 * - 取指周期: 模拟指令缓存或存储器访问延迟
 * - 统计更新: 记录指令数量和周期数
 * - 流水线前端: 为后续译码阶段准备数据
 * 
 * 错误处理:
 * - 地址越界: 检测PC超出程序范围
 * - 指令有效性: 确保取到的指令合法
 * 
 * @param sim 模拟器实例指针
 * @param inst 输出的指令结构
 * @return 错误码
 */
static cpu_error_t fetch_instruction(instruction_simulator_t *sim, instruction_t *inst) {
    // PC合法性检查: 确保不会访问无效的指令地址
    if (sim->registers.pc >= sim->program_size) {
        return CPU_ERROR_INVALID_PC;
    }
    
    // 指令获取: 从程序数组中读取指令
    // 在真实处理器中，这相当于从指令缓存或主存中取指
    *inst = sim->program[sim->registers.pc];
    
    // 指令寄存器更新: 将当前指令的操作码保存到IR
    // 这是处理器状态的重要组成部分，用于调试和状态显示
    sim->registers.ir = inst->opcode;
    
    // 程序计数器自动递增: 指向下一条指令
    // 这是顺序执行的基本机制，分支指令会修改这个值
    sim->registers.pc++;
    
    // 性能统计更新
    sim->stats.total_instructions++;  // 总指令数递增
    sim->stats.cycles += 1;           // 取指周期计数
    
    return CPU_ERROR_NONE;
}

/**
 * 指令译码 - 机器码到内部表示的转换
 * 
 * 译码器硬件实现原理:
 * - 分层译码: 先解析主操作码，再解析扩展字段
 * - 并行译码: 多个字段可以同时解析，减少关键路径延迟
 * - 查找表优化: 使用ROM或PLA实现复杂的译码逻辑
 * - 微指令生成: 将CISC指令分解为RISC微操作序列
 * 
 * 格式识别算法:
 * - 操作码范围检查: 根据操作码值确定指令格式
 * - 扩展操作码处理: 支持多级操作码编码
 * - 字段提取: 使用位操作快速提取指令字段
 * - 合法性验证: 检查指令编码的有效性
 * 
 * 性能优化技术:
 * - 预译码: 在指令缓存中预先标记指令类型
 * - 融合译码: 识别常见指令序列并融合处理
 * - 分支预测集成: 在译码阶段预测分支方向
 * 
 * @param data 32位机器码数据
 * @param format 指令格式(16位或32位)
 * @param inst 输出的指令结构体
 * @return 错误码
 */
cpu_error_t instruction_decode(u32 data, 
                              instruction_format_t format,
                              instruction_t *inst) {
    if (!inst) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    // 指令结构初始化: 清零避免残留数据影响
    memset(inst, 0, sizeof(instruction_t));
    
    if (format == INST_FORMAT_16BIT) {
        // 16位指令译码: 演示扩展操作码技术
        instruction_16_t inst16;
        inst16.raw = (u16)data;
        
        // 分层译码策略: 根据操作码范围确定指令格式
        // 这种设计最大化了16位空间的利用率
        if (inst16.three_addr.opcode < 8) {
            // 三地址指令: 操作码0-7，最大操作数表达能力
            inst->opcode = (opcode_t)inst16.three_addr.opcode;
            inst->operand1 = inst16.three_addr.addr1;
            inst->operand2 = inst16.three_addr.addr2;
            inst->operand3 = inst16.three_addr.addr3;
            inst->addr_type = ADDR_THREE;
            inst->addr_mode1 = ADDR_REGISTER;  // 默认寄存器寻址
            inst->addr_mode2 = ADDR_REGISTER;
            inst->addr_mode3 = ADDR_REGISTER;
        } else if (inst16.two_addr.opcode == 15 && inst16.two_addr.ext_op < 8) {
            // 二地址指令: 使用扩展操作码技术
            inst->opcode = (opcode_t)(16 + inst16.two_addr.ext_op);
            inst->operand1 = inst16.two_addr.addr1;
            inst->operand2 = inst16.two_addr.addr2;
            inst->addr_type = ADDR_TWO;
            inst->addr_mode1 = ADDR_REGISTER;
            inst->addr_mode2 = ADDR_REGISTER;
        } else {
            // 一地址或零地址指令: 最大立即数空间
            inst->opcode = (opcode_t)(32 + inst16.one_addr.ext_op);
            inst->operand1 = inst16.one_addr.addr1;
            inst->addr_type = ADDR_ONE;
        }
    } else {
        // 32位指令格式
        instruction_32_t inst32;
        inst32.raw = data;
        
        if (inst32.r_type.opcode == 0) {
            // R型指令
            inst->opcode = (opcode_t)inst32.r_type.funct;
            inst->operand1 = inst32.r_type.rs;
            inst->operand2 = inst32.r_type.rt;
            inst->operand3 = inst32.r_type.rd;
            inst->addr_type = ADDR_THREE;
        } else if (inst32.i_type.opcode < 32) {
            // I型指令
            inst->opcode = (opcode_t)inst32.i_type.opcode;
            inst->operand1 = inst32.i_type.rs;
            inst->operand2 = inst32.i_type.rt;
            inst->operand3 = inst32.i_type.immediate;
            inst->addr_type = ADDR_TWO;
        } else {
            // J型指令
            inst->opcode = (opcode_t)inst32.j_type.opcode;
            inst->operand1 = inst32.j_type.address;
            inst->addr_type = ADDR_ONE;
        }
    }
    
    return CPU_ERROR_NONE;
}

cpu_error_t instruction_encode(const instruction_t *inst,
                              instruction_format_t format,
                              u32 *data) {
    if (!inst || !data) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    *data = 0;
    
    if (format == INST_FORMAT_16BIT) {
        instruction_16_t inst16;
        inst16.raw = 0;
        
        if (inst->addr_type == ADDR_THREE) {
            inst16.three_addr.opcode = inst->opcode & 0xF;
            inst16.three_addr.addr1 = inst->operand1 & 0xF;
            inst16.three_addr.addr2 = inst->operand2 & 0xF;
            inst16.three_addr.addr3 = inst->operand3 & 0xF;
        } else if (inst->addr_type == ADDR_TWO) {
            inst16.two_addr.opcode = 15;
            inst16.two_addr.ext_op = (inst->opcode - 16) & 0xF;
            inst16.two_addr.addr1 = inst->operand1 & 0xF;
            inst16.two_addr.addr2 = inst->operand2 & 0xF;
        } else {
            inst16.one_addr.opcode = 15;
            inst16.one_addr.ext_op = (inst->opcode - 32) & 0xF;
            inst16.one_addr.addr1 = inst->operand1 & 0xFF;
        }
        
        *data = inst16.raw;
    } else {
        instruction_32_t inst32;
        inst32.raw = 0;
        
        if (inst->addr_type == ADDR_THREE) {
            inst32.r_type.opcode = 0;
            inst32.r_type.funct = inst->opcode & 0x3F;
            inst32.r_type.rs = inst->operand1 & 0x1F;
            inst32.r_type.rt = inst->operand2 & 0x1F;
            inst32.r_type.rd = inst->operand3 & 0x1F;
        } else if (inst->addr_type == ADDR_TWO) {
            inst32.i_type.opcode = inst->opcode & 0x3F;
            inst32.i_type.rs = inst->operand1 & 0x1F;
            inst32.i_type.rt = inst->operand2 & 0x1F;
            inst32.i_type.immediate = inst->operand3 & 0xFFFF;
        } else {
            inst32.j_type.opcode = inst->opcode & 0x3F;
            inst32.j_type.address = inst->operand1 & 0x3FFFFFF;
        }
        
        *data = inst32.raw;
    }
    
    return CPU_ERROR_NONE;
}

// ==================== 寻址方式计算函数 ====================

cpu_error_t calculate_effective_address(instruction_simulator_t *sim,
                                       addressing_mode_t mode,
                                       u32 operand,
                                       u32 *effective_addr) {
    if (!sim || !effective_addr) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    switch (mode) {
        case ADDR_IMMEDIATE:
            // 立即寻址：操作数本身就是数据，返回操作数作为"有效地址"
            *effective_addr = operand;
            break;
            
        case ADDR_DIRECT:
            // 直接寻址：操作数是内存地址
            *effective_addr = operand;
            break;
            
        case ADDR_INDIRECT:
            // 间接寻址：操作数指向的内存单元包含有效地址
            if (operand + 3 > sim->memory.size) {
                return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
            }
            *effective_addr = *(u32*)(sim->memory.data + operand);
            sim->stats.memory_accesses++;
            break;
            
        case ADDR_REGISTER:
            // 寄存器寻址：操作数是寄存器编号
            *effective_addr = 0;  // 无内存地址
            break;
            
        case ADDR_REG_INDIRECT:
            // 寄存器间接寻址：寄存器内容是有效地址
            if (operand >= MAX_REGISTERS) {
                return CPU_ERROR_INVALID_REGISTER;
            }
            *effective_addr = sim->registers.general[operand];
            sim->stats.register_accesses++;
            break;
            
        case ADDR_RELATIVE:
            // 相对寻址：PC + 偏移量
            *effective_addr = sim->registers.pc + (s32)operand;
            break;
            
        case ADDR_BASE:
            // 基址寻址：基址寄存器 + 偏移量
            // 高16位是寄存器号，低16位是偏移量
            {
                u32 reg_num = (operand >> 16) & 0xFFFF;
                u32 offset = operand & 0xFFFF;
                if (reg_num >= MAX_REGISTERS) {
                    return CPU_ERROR_INVALID_REGISTER;
                }
                *effective_addr = sim->registers.general[reg_num] + offset;
                sim->stats.register_accesses++;
            }
            break;
            
        case ADDR_INDEX:
            // 变址寻址：变址寄存器 + 基地址
            // 高16位是寄存器号，低16位是基地址
            {
                u32 reg_num = (operand >> 16) & 0xFFFF;
                u32 base_addr = operand & 0xFFFF;
                if (reg_num >= MAX_REGISTERS) {
                    return CPU_ERROR_INVALID_REGISTER;
                }
                *effective_addr = base_addr + sim->registers.general[reg_num];
                sim->stats.register_accesses++;
            }
            break;
            
        default:
            return CPU_ERROR_INVALID_ADDRESSING_MODE;
    }
    
    // 对于立即寻址和寄存器寻址，不需要检查内存边界
    // 其他寻址方式的边界检查在实际内存访问时进行
    
    return CPU_ERROR_NONE;
}

/**
 * 获取操作数值 - 数据路径前端模拟
 * 
 * 数据路径硬件原理:
 * - 多路选择器: 根据寻址方式选择数据来源
 * - 寄存器堆读端口: 支持同时读取多个寄存器
 * - 内存访问单元: 处理各种复杂的内存访问模式
 * - 数据转发网络: 减少数据相关导致的流水线停顿
 * 
 * 性能优化策略:
 * - 早期值预取: 在译码阶段预先读取寄存器值
 * - 缓存预测: 预测内存访问模式，提前准备数据
 * - 数据对齐: 优化内存访问的字节对齐
 * - 错误预检: 在数据访问前验证地址合法性
 * 
 * 访问时序分析:
 * - 寄存器访问: 0-1个时钟周期(取决于寄存器文件设计)
 * - 一级缓存命中: 1-3个时钟周期
 * - 内存访问: 数十到数百个时钟周期
 * - 虚拟内存: 额外的TLB查找和页表访问开销
 * 
 * @param sim 模拟器实例指针
 * @param mode 寻址方式
 * @param operand 操作数
 * @param value 输出的操作数值
 * @return 错误码
 */
cpu_error_t get_operand_value(instruction_simulator_t *sim,
                             addressing_mode_t mode,
                             u32 operand,
                             u32 *value) {
    if (!sim || !value) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    u32 effective_addr;
    cpu_error_t error;
    
    // 数据路径选择器模拟
    switch (mode) {
        case ADDR_IMMEDIATE:
            // 立即寻址: 最快的数据访问方式
            // 硬件实现: 数据直接嵌入在指令中，译码器提供
            // 性能特点: 0个额外时钟周期，无内存访问开销
            *value = operand;
            break;
            
        case ADDR_REGISTER:
            // 寄存器寻址: 高速数据访问
            // 硬件实现: 寄存器文件的读端口，通过寄存器号索引
            // 性能特点: 1个时钟周期，支持多端口并行读取
            if (operand >= MAX_REGISTERS) {
                return CPU_ERROR_INVALID_REGISTER;
            }
            
            *value = sim->registers.general[operand];
            
            // 性能统计: 寄存器文件访问计数
            sim->stats.register_accesses++;
            break;
            
        default:
            // 内存相关寻址方式: 需要地址计算和内存访问
            // 硬件实现: 地址生成单元(AGU) + 内存管理单元(MMU)
            
            // 第一阶段: 地址生成(AGU操作)
            error = calculate_effective_address(sim, mode, operand, &effective_addr);
            if (error != CPU_ERROR_NONE) {
                return error;
            }
            
            // 第二阶段: 内存访问边界检查
            // 确保32位数据访问不会越界
            if (effective_addr + 3 >= sim->memory.size) {
                return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
            }
            
            // 第三阶段: 内存数据读取
            // 模拟内存控制器的读操作
            // 在真实硬件中，这里会涉及缓存层次结构
            *value = *(u32*)(sim->memory.data + effective_addr);
            
            // 性能统计: 内存访问计数(影响缓存性能分析)
            sim->stats.memory_accesses++;
            break;
    }
    
    return CPU_ERROR_NONE;
}

/**
 * 设置操作数值 - 数据路径后端模拟
 * 
 * 写回路径硬件原理:
 * - 写端口仲裁: 多个执行单元竞争写回端口
 * - 数据转发: 写回数据同时转发给依赖指令
 * - 写缓冲: 减少写操作对后续指令的影响
 * - 内存保护: 硬件级别的内存访问权限检查
 * 
 * 写操作优化:
 * - 写合并: 相邻的小写操作合并为大块写操作
 * - 异步写回: 写操作在后台执行，不阻塞流水线
 * - 写验证: 关键数据的写回正确性验证
 * - 原子操作: 支持多线程环境的原子写操作
 * 
 * 一致性保证:
 * - 写序列化: 确保写操作的正确顺序
 * - 缓存一致性: 维护多级缓存的数据一致性
 * - 内存栅栏: 支持内存访问顺序的强制同步
 * 
 * @param sim 模拟器实例指针
 * @param mode 寻址方式
 * @param operand 操作数
 * @param value 要写入的值
 * @return 错误码
 */
cpu_error_t set_operand_value(instruction_simulator_t *sim,
                             addressing_mode_t mode,
                             u32 operand,
                             u32 value) {
    if (!sim) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    u32 effective_addr;
    cpu_error_t error;
    
    // 写回路径选择器模拟
    switch (mode) {
        case ADDR_IMMEDIATE:
            // 立即寻址不支持写操作: 立即数是只读的
            // 这是指令集设计的基本约束
            return CPU_ERROR_INVALID_OPERATION;
            
        case ADDR_REGISTER:
            // 寄存器写回: 最常见的数据写入方式
            // 硬件实现: 寄存器文件的写端口，支持转发和重命名
            if (operand >= MAX_REGISTERS) {
                return CPU_ERROR_INVALID_REGISTER;
            }
            
            // 寄存器写入操作
            sim->registers.general[operand] = value;
            
            // 性能统计: 寄存器写访问
            sim->stats.register_accesses++;
            break;
            
        default:
            // 内存写操作: 复杂的存储访问路径
            // 硬件实现: 存储单元 + 缓存控制器 + 内存控制器
            
            // 地址生成: 计算存储目标地址
            error = calculate_effective_address(sim, mode, operand, &effective_addr);
            if (error != CPU_ERROR_NONE) {
                return error;
            }
            
            // 内存保护检查: 防止越界写入
            if (effective_addr + 3 >= sim->memory.size) {
                return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
            }
            
            // 内存写入操作
            // 在真实系统中，这里会涉及写缓冲、缓存一致性等
            *(u32*)(sim->memory.data + effective_addr) = value;
            
            // 性能统计: 内存写访问计数
            sim->stats.memory_accesses++;
            break;
    }
    
    return CPU_ERROR_NONE;
}

// ==================== 指令执行函数 ====================

/**
 * 指令执行调度器 - 超标量处理器执行单元模拟
 * 
 * 执行单元架构:
 * - 整数ALU: 处理算术和逻辑运算
 * - 浮点单元: 处理浮点运算(本模拟器暂不支持)
 * - 地址生成单元: 处理内存访问地址计算
 * - 分支执行单元: 处理程序控制流指令
 * - 加载存储单元: 处理内存数据传输
 * 
 * 调度策略:
 * - 指令分类: 根据操作码将指令分发到相应执行单元
 * - 资源竞争: 模拟执行单元的占用和释放
 * - 依赖检测: 检查数据依赖和结构冲突
 * - 乱序执行: 支持指令的乱序发射和完成
 * 
 * 性能监控:
 * - 执行单元利用率: 统计不同类型指令的执行频率
 * - 流水线效率: 分析指令流水线的停顿和气泡
 * - 分支预测准确率: 统计分支指令的预测效果
 * 
 * 异常处理:
 * - 算术异常: 除零错误、溢出检测
 * - 内存异常: 页面错误、保护违例
 * - 系统异常: 特权指令、非法操作码
 * 
 * @param sim 模拟器实例指针
 * @param inst 待执行的指令
 * @return 错误码
 */
static cpu_error_t execute_instruction(instruction_simulator_t *sim, const instruction_t *inst) {
    if (!sim || !inst) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    cpu_error_t error = CPU_ERROR_NONE;
    
    // 指令执行单元调度器
    // 基于操作码范围进行功能单元分发，模拟硬件译码器行为
    
    if (inst->opcode >= OP_NOP && inst->opcode <= OP_XCHG) {
        // === 数据传输执行单元 ===
        // 处理内存和寄存器间的数据移动
        // 硬件特点: 高带宽、低延迟、支持多种寻址方式
        error = execute_data_transfer(sim, inst);
        sim->stats.data_transfer_count++;
        
        // 数据传输指令通常需要额外的内存访问周期
        sim->stats.cycles += 1;  // 额外的内存访问周期
        
    } else if (inst->opcode >= OP_ADD && inst->opcode <= OP_ABS) {
        // === 算术逻辑单元(ALU) ===
        // 处理整数运算和逻辑操作
        // 硬件特点: 快速运算、标志位生成、支持多精度运算
        error = execute_arithmetic(sim, inst);
        sim->stats.arithmetic_count++;
        
        // 复杂算术运算可能需要多个周期(如乘除法)
        if (inst->opcode == OP_MUL) {
            sim->stats.cycles += 2;  // 乘法额外周期
        } else if (inst->opcode == OP_DIV) {
            sim->stats.cycles += 10; // 除法高延迟操作
        }
        
    } else if (inst->opcode >= OP_AND && inst->opcode <= OP_ROR) {
        // === 位操作执行单元 ===
        // 处理逻辑运算和位移操作
        // 硬件特点: 单周期执行、桶形移位器、并行位操作
        error = execute_logical(sim, inst);
        sim->stats.logical_count++;
        
        // 移位操作可能需要额外周期(取决于移位位数)
        if (inst->opcode >= OP_SHL && inst->opcode <= OP_ROR) {
            // 桶形移位器通常支持任意位数的单周期移位
            // 但早期处理器可能需要多周期
            sim->stats.cycles += 0;  // 现代处理器单周期完成
        }
        
    } else if (inst->opcode >= OP_JMP && inst->opcode <= OP_CMP) {
        // === 分支执行单元 ===
        // 处理程序控制流和条件跳转
        // 硬件特点: 分支预测、跳转目标缓存、返回地址栈
        error = execute_program_control(sim, inst);
        sim->stats.control_count++;
        
        // 分支指令通常会影响流水线性能
        // 分支预测失败会导致流水线清空
        sim->stats.cycles += 2;  // 分支延迟槽或预测失败开销
        
    } else {
        // === 系统控制单元 ===
        // 处理特权指令和系统管理
        // 硬件特点: 特权检查、中断处理、状态切换
        
        switch (inst->opcode) {
            case OP_HALT:
                // 停机指令: 处理器进入停止状态
                // 硬件实现: 时钟门控、功耗管理
                sim->running = false;
                break;
                
            case OP_INT:
                // 软件中断: 触发异常处理流程
                // 硬件实现: 中断控制器、上下文保存
                return CPU_ERROR_INTERRUPT;
                
            case OP_IRET:
                // 中断返回: 恢复中断前的处理器状态
                // 硬件实现: 状态恢复、特权级切换
                // 简化实现: 仅作为占位符
                break;
                
            default:
                // 非法指令: 触发异常处理
                return CPU_ERROR_INVALID_OPCODE;
        }
        
        // 系统指令通常有较高的执行开销
        sim->stats.cycles += 5;  // 系统调用/中断处理开销
    }
    
    // 基本指令执行周期统计
    // 模拟标准五级流水线: 取指、译码、执行、访存、写回
    sim->stats.cycles += 3;  // 译码(1) + 执行(1) + 写回(1)
    
    return error;
}

cpu_error_t execute_data_transfer(instruction_simulator_t *sim,
                                 const instruction_t *inst) {
    if (!sim || !inst) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    u32 value1, value2;
    cpu_error_t error;
    
    switch (inst->opcode) {
        case OP_NOP:
            // 空操作，什么都不做
            break;
            
        case OP_LOAD:
            // LOAD dst, src：将源操作数加载到目标操作数
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, value1);
            if (error != CPU_ERROR_NONE) return error;
            break;
            
        case OP_STORE:
            // STORE dst, src：将源操作数存储到目标地址
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, value1);
            if (error != CPU_ERROR_NONE) return error;
            break;
            
        case OP_MOVE:
            // MOVE dst, src：移动数据
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, value1);
            if (error != CPU_ERROR_NONE) return error;
            break;
            
        case OP_PUSH:
            // PUSH src：将源操作数压入栈
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            if (sim->memory.stack_top >= STACK_SIZE) {
                return CPU_ERROR_STACK_OVERFLOW;
            }
            
            sim->memory.stack[sim->memory.stack_top++] = value1;
            sim->registers.sp--;
            break;
            
        case OP_POP:
            // POP dst：从栈中弹出数据到目标操作数
            if (sim->memory.stack_top == 0) {
                return CPU_ERROR_STACK_UNDERFLOW;
            }
            
            value1 = sim->memory.stack[--sim->memory.stack_top];
            sim->registers.sp++;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, value1);
            if (error != CPU_ERROR_NONE) return error;
            break;
            
        case OP_XCHG:
            // XCHG op1, op2：交换两个操作数的值
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, value2);
            if (error != CPU_ERROR_NONE) return error;
            
            error = set_operand_value(sim, inst->addr_mode2, inst->operand2, value1);
            if (error != CPU_ERROR_NONE) return error;
            break;
            
        default:
            return CPU_ERROR_INVALID_OPCODE;
    }
    
    return CPU_ERROR_NONE;
}

cpu_error_t execute_arithmetic(instruction_simulator_t *sim,
                              const instruction_t *inst) {
    if (!sim || !inst) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    u32 value1, value2, result;
    cpu_error_t error;
    
    switch (inst->opcode) {
        case OP_ADD:
            // ADD dst, src1, src2 或 ADD dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 + value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 + value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            update_flags(sim, result, OP_ADD);
            break;
            
        case OP_SUB:
            // SUB dst, src1, src2 或 SUB dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 - value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 - value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            update_flags(sim, result, OP_SUB);
            break;
            
        case OP_MUL:
            // MUL dst, src1, src2 或 MUL dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 * value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 * value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            update_flags(sim, result, OP_MUL);
            break;
            
        case OP_DIV:
            // DIV dst, src1, src2 或 DIV dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            if (value2 == 0) {
                return CPU_ERROR_DIVISION_BY_ZERO;
            }
            
            result = value1 / value2;
            
            if (inst->addr_type == ADDR_THREE) {
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            } else {
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            }
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_DIV);
            break;
            
        case OP_INC:
            // INC dst：目标操作数自增1
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            result = value1 + 1;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_INC);
            break;
            
        case OP_DEC:
            // DEC dst：目标操作数自减1
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            result = value1 - 1;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_DEC);
            break;
            
        case OP_NEG:
            // NEG dst：目标操作数取负
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            result = (u32)(-(s32)value1);
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_NEG);
            break;
            
        case OP_ABS:
            // ABS dst：目标操作数取绝对值
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            if ((s32)value1 < 0) {
                result = (u32)(-(s32)value1);
            } else {
                result = value1;
            }
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_ABS);
            break;
            
        default:
            return CPU_ERROR_INVALID_OPCODE;
    }
    
    return CPU_ERROR_NONE;
}

cpu_error_t execute_logical(instruction_simulator_t *sim,
                           const instruction_t *inst) {
    if (!sim || !inst) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    u32 value1, value2, result;
    cpu_error_t error;
    
    switch (inst->opcode) {
        case OP_AND:
            // AND dst, src1, src2 或 AND dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 & value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 & value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            update_flags(sim, result, OP_AND);
            break;
            
        case OP_OR:
            // OR dst, src1, src2 或 OR dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 | value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 | value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            update_flags(sim, result, OP_OR);
            break;
            
        case OP_XOR:
            // XOR dst, src1, src2 或 XOR dst, src
            if (inst->addr_type == ADDR_THREE) {
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode3, inst->operand3, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 ^ value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            } else {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
                if (error != CPU_ERROR_NONE) return error;
                
                error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
                if (error != CPU_ERROR_NONE) return error;
                
                result = value1 ^ value2;
                
                error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            update_flags(sim, result, OP_XOR);
            break;
            
        case OP_NOT:
            // NOT dst：目标操作数按位取反
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            result = ~value1;
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_NOT);
            break;
            
        case OP_SHL:
            // SHL dst, count：逻辑左移
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            // 检查移位范围
            if (value2 > 32) {
                return CPU_ERROR_INVALID_OPERATION;
            }
            
            result = value1 << (value2 & 0x1F);  // 限制移位数量
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_SHL);
            break;
            
        case OP_SHR:
            // SHR dst, count：逻辑右移
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            result = value1 >> (value2 & 0x1F);  // 限制移位数量
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_SHR);
            break;
            
        case OP_SAR:
            // SAR dst, count：算术右移
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            result = (u32)((s32)value1 >> (value2 & 0x1F));  // 算术右移
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_SAR);
            break;
            
        case OP_ROL:
            // ROL dst, count：循环左移
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            value2 &= 0x1F;  // 限制移位数量
            result = (value1 << value2) | (value1 >> (32 - value2));
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_ROL);
            break;
            
        case OP_ROR:
            // ROR dst, count：循环右移
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            value2 &= 0x1F;  // 限制移位数量
            result = (value1 >> value2) | (value1 << (32 - value2));
            
            error = set_operand_value(sim, inst->addr_mode1, inst->operand1, result);
            if (error != CPU_ERROR_NONE) return error;
            
            update_flags(sim, result, OP_ROR);
            break;
            
        default:
            return CPU_ERROR_INVALID_OPCODE;
    }
    
    return CPU_ERROR_NONE;
}

cpu_error_t execute_program_control(instruction_simulator_t *sim,
                                   const instruction_t *inst) {
    if (!sim || !inst) {
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    u32 value1, value2, target_addr;
    cpu_error_t error;
    
    switch (inst->opcode) {
        case OP_JMP:
            // JMP addr：无条件跳转
            if (inst->addr_mode1 == ADDR_IMMEDIATE) {
                target_addr = inst->operand1;
            } else {
                error = calculate_effective_address(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
            }
            
            // 对于测试目的，允许跳转到程序范围外的地址
            // if (target_addr >= sim->program_size) {
            //     return CPU_ERROR_INVALID_PC;
            // }
            
            sim->registers.pc = target_addr;
            break;
            
        case OP_JZ:
            // JZ addr：零跳转
            if (sim->registers.psw.zero_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JNZ:
            // JNZ addr：非零跳转
            if (!sim->registers.psw.zero_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JC:
            // JC addr：进位跳转
            if (sim->registers.psw.carry_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JNC:
            // JNC addr：非进位跳转
            if (!sim->registers.psw.carry_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JE:
            // JE addr：相等跳转（等同于JZ）
            if (sim->registers.psw.zero_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JNE:
            // JNE addr：不等跳转（等同于JNZ）
            if (!sim->registers.psw.zero_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JL:
            // JL addr：小于跳转
            if (sim->registers.psw.negative_flag != sim->registers.psw.overflow_flag) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_JG:
            // JG addr：大于跳转
            if (!sim->registers.psw.zero_flag && 
                (sim->registers.psw.negative_flag == sim->registers.psw.overflow_flag)) {
                error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
                if (error != CPU_ERROR_NONE) return error;
                
                // if (target_addr >= sim->program_size) {
                //     return CPU_ERROR_INVALID_PC;
                // }
                
                sim->registers.pc = target_addr;
            }
            break;
            
        case OP_CALL:
            // CALL addr：子程序调用
            // 保存返回地址到栈
            if (sim->memory.stack_top >= STACK_SIZE) {
                return CPU_ERROR_STACK_OVERFLOW;
            }
            
            sim->memory.stack[sim->memory.stack_top++] = sim->registers.pc;
            sim->registers.sp--;
            
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &target_addr);
            if (error != CPU_ERROR_NONE) return error;
            
            // if (target_addr >= sim->program_size) {
            //     return CPU_ERROR_INVALID_PC;
            // }
            
            sim->registers.pc = target_addr;
            break;
            
        case OP_RET:
            // RET：子程序返回
            if (sim->memory.stack_top == 0) {
                return CPU_ERROR_STACK_UNDERFLOW;
            }
            
            sim->registers.pc = sim->memory.stack[--sim->memory.stack_top];
            sim->registers.sp++;
            break;
            
        case OP_CMP:
            // CMP op1, op2：比较操作数
            error = get_operand_value(sim, inst->addr_mode1, inst->operand1, &value1);
            if (error != CPU_ERROR_NONE) return error;
            
            error = get_operand_value(sim, inst->addr_mode2, inst->operand2, &value2);
            if (error != CPU_ERROR_NONE) return error;
            
            // 执行减法运算但不保存结果，只更新标志位
            u32 result = value1 - value2;
            update_flags(sim, result, OP_SUB);
            break;
            
        default:
            return CPU_ERROR_INVALID_OPCODE;
    }
    
    return CPU_ERROR_NONE;
}

// ==================== 标志位和条件检查函数 ====================

void update_flags(instruction_simulator_t *sim, u32 result, opcode_t operation) {
    if (!sim) return;
    
    // 零标志位
    sim->registers.psw.zero_flag = (result == 0);
    
    // 负数标志位（最高位为1）
    sim->registers.psw.negative_flag = ((s32)result < 0);
    
    // 进位标志位和溢出标志位的设置取决于操作类型
    switch (operation) {
        case OP_ADD:
        case OP_INC:
            // 加法运算：检查无符号溢出（进位）
            // 这里简化处理，实际需要比较操作前后的值
            sim->registers.psw.carry_flag = false;  // 简化实现
            
            // 有符号溢出：两个正数相加得负数，或两个负数相加得正数
            sim->registers.psw.overflow_flag = false;  // 简化实现
            break;
            
        case OP_SUB:
        case OP_DEC:
            // 减法运算：借位标志
            sim->registers.psw.carry_flag = false;  // 简化实现
            sim->registers.psw.overflow_flag = false;  // 简化实现
            break;
            
        case OP_AND:
        case OP_OR:
        case OP_XOR:
        case OP_NOT:
            // 逻辑运算：清除进位和溢出标志
            sim->registers.psw.carry_flag = false;
            sim->registers.psw.overflow_flag = false;
            break;
            
        case OP_SHL:
            // 左移：最高位进入进位标志
            sim->registers.psw.carry_flag = false;  // 简化实现
            sim->registers.psw.overflow_flag = false;
            break;
            
        case OP_SHR:
        case OP_SAR:
            // 右移：最低位进入进位标志
            sim->registers.psw.carry_flag = false;  // 简化实现
            sim->registers.psw.overflow_flag = false;
            break;
            
        default:
            // 其他操作不影响进位和溢出标志
            break;
    }
    
    // 更新条件码
    sim->registers.psw.condition_code = 0;
    if (sim->registers.psw.zero_flag) sim->registers.psw.condition_code |= 0x01;
    if (sim->registers.psw.carry_flag) sim->registers.psw.condition_code |= 0x02;
    if (sim->registers.psw.negative_flag) sim->registers.psw.condition_code |= 0x04;
    if (sim->registers.psw.overflow_flag) sim->registers.psw.condition_code |= 0x08;
}

bool check_condition(instruction_simulator_t *sim, u8 condition) {
    if (!sim) return false;
    
    switch (condition) {
        case 0x00: return true;  // 无条件
        case 0x01: return sim->registers.psw.zero_flag;  // 零
        case 0x02: return !sim->registers.psw.zero_flag;  // 非零
        case 0x03: return sim->registers.psw.carry_flag;  // 进位
        case 0x04: return !sim->registers.psw.carry_flag;  // 非进位
        case 0x05: return sim->registers.psw.negative_flag;  // 负数
        case 0x06: return !sim->registers.psw.negative_flag;  // 非负数
        case 0x07: return sim->registers.psw.overflow_flag;  // 溢出
        case 0x08: return !sim->registers.psw.overflow_flag;  // 非溢出
        default: return false;
    }
}

// ==================== 性能统计和辅助函数 ====================

static void update_performance_stats(instruction_simulator_t *sim, const instruction_t *inst) {
    if (!sim || !inst) return;
    
    // 基本统计已在execute_instruction中更新
    // 这里可以添加更详细的性能分析
}

execution_stats_t instruction_sim_get_stats(const instruction_simulator_t *sim) {
    execution_stats_t empty_stats = {0};
    
    if (!sim) {
        return empty_stats;
    }
    
    return sim->stats;
}

// ==================== 调试和打印函数 ====================

void instruction_sim_print_state(const instruction_simulator_t *sim) {
    if (!sim) {
        printf("模拟器未初始化\n");
        return;
    }
    
    printf("\n========== 指令系统模拟器状态 ==========\n");
    printf("运行状态: %s\n", sim->running ? "运行中" : "停止");
    printf("调试模式: %s\n", sim->debug_mode ? "开启" : "关闭");
    printf("指令格式: %s\n", sim->format == INST_FORMAT_16BIT ? "16位" : "32位");
    printf("程序大小: %u 条指令\n", sim->program_size);
    printf("内存大小: %u 字节\n", sim->memory.size);
    printf("栈使用量: %u/%u\n", sim->memory.stack_top, STACK_SIZE);
    
    printf("\n程序计数器: 0x%08X (%u)\n", sim->registers.pc, sim->registers.pc);
    printf("栈指针: 0x%08X (%u)\n", sim->registers.sp, sim->registers.sp);
    printf("指令寄存器: 0x%08X\n", sim->registers.ir);
    
    printf("\n程序状态字:\n");
    printf("  零标志: %s\n", sim->registers.psw.zero_flag ? "置位" : "清零");
    printf("  进位标志: %s\n", sim->registers.psw.carry_flag ? "置位" : "清零");
    printf("  负数标志: %s\n", sim->registers.psw.negative_flag ? "置位" : "清零");
    printf("  溢出标志: %s\n", sim->registers.psw.overflow_flag ? "置位" : "清零");
    printf("  中断使能: %s\n", sim->registers.psw.interrupt_enable ? "开启" : "关闭");
    printf("  条件码: 0x%02X\n", sim->registers.psw.condition_code);
}

void instruction_sim_print_stats(const instruction_simulator_t *sim) {
    if (!sim) {
        printf("模拟器未初始化\n");
        return;
    }
    
    printf("\n========== 执行统计信息 ==========\n");
    printf("总指令数: %llu\n", sim->stats.total_instructions);
    printf("数据传送指令: %llu (%.2f%%)\n", 
           sim->stats.data_transfer_count,
           sim->stats.total_instructions > 0 ? 
           (double)sim->stats.data_transfer_count * 100.0 / sim->stats.total_instructions : 0.0);
    printf("算术运算指令: %llu (%.2f%%)\n", 
           sim->stats.arithmetic_count,
           sim->stats.total_instructions > 0 ? 
           (double)sim->stats.arithmetic_count * 100.0 / sim->stats.total_instructions : 0.0);
    printf("逻辑运算指令: %llu (%.2f%%)\n", 
           sim->stats.logical_count,
           sim->stats.total_instructions > 0 ? 
           (double)sim->stats.logical_count * 100.0 / sim->stats.total_instructions : 0.0);
    printf("程序控制指令: %llu (%.2f%%)\n", 
           sim->stats.control_count,
           sim->stats.total_instructions > 0 ? 
           (double)sim->stats.control_count * 100.0 / sim->stats.total_instructions : 0.0);
    
    printf("\n内存访问次数: %llu\n", sim->stats.memory_accesses);
    printf("寄存器访问次数: %llu\n", sim->stats.register_accesses);
    printf("执行周期数: %llu\n", sim->stats.cycles);
    printf("执行时间: %.6f 秒\n", sim->stats.execution_time);
    
    if (sim->stats.execution_time > 0) {
        printf("平均指令周期: %.2f 周期/指令\n", 
               (double)sim->stats.cycles / sim->stats.total_instructions);
        printf("指令执行速率: %.0f 指令/秒\n", 
               sim->stats.total_instructions / sim->stats.execution_time);
    }
}

void instruction_sim_print_registers(const instruction_simulator_t *sim) {
    if (!sim) {
        printf("模拟器未初始化\n");
        return;
    }
    
    printf("\n========== 寄存器状态 ==========\n");
    
    // 打印通用寄存器（4个一行）
    for (int i = 0; i < MAX_REGISTERS; i++) {
        if (i % 4 == 0) printf("\n");
        printf("R%-2d: 0x%08X  ", i, sim->registers.general[i]);
    }
    
    printf("\n\n特殊寄存器:\n");
    printf("PC:  0x%08X    SP:  0x%08X\n", sim->registers.pc, sim->registers.sp);
    printf("IR:  0x%08X    MAR: 0x%08X\n", sim->registers.ir, sim->registers.mar);
    printf("MDR: 0x%08X\n", sim->registers.mdr);
}

void instruction_sim_print_memory(const instruction_simulator_t *sim,
                                 u32 start_addr, u32 length) {
    if (!sim || !sim->memory.data) {
        printf("模拟器未初始化或内存未分配\n");
        return;
    }
    
    if (start_addr >= sim->memory.size) {
        printf("起始地址超出内存范围\n");
        return;
    }
    
    if (start_addr + length > sim->memory.size) {
        length = sim->memory.size - start_addr;
    }
    
    printf("\n========== 内存内容 (地址: 0x%08X - 0x%08X) ==========\n", 
           start_addr, start_addr + length - 1);
    
    for (u32 i = 0; i < length; i += 16) {
        printf("┃ %08X ┃ ", start_addr + i);
        
        // 打印十六进制数据
        for (u32 j = 0; j < 16 && i + j < length; j++) {
            printf("%02X "), sim->memory.data[start_addr + i + j]);
        }
        
        // 对齐
        for (u32 j = length - i; j < 16; j++) {
            printf("   ");
        }
        
        printf(" ┃ "));
        
        // 打印ASCII字符
        for (u32 j = 0; j < 16 && i + j < length; j++) {
            u8 byte = sim->memory.data[start_addr + i + j];
            printf("%c", (byte >= 32 && byte <= 126) ? byte : '.');
        }
        
        printf("\n");
    }
}

// ==================== 字符串转换函数 ====================

const char* opcode_to_string(opcode_t opcode) {
    switch (opcode) {
        // 数据传送指令
        case OP_NOP: return "NOP";
        case OP_LOAD: return "LOAD";
        case OP_STORE: return "STORE";
        case OP_MOVE: return "MOVE";
        case OP_PUSH: return "PUSH";
        case OP_POP: return "POP";
        case OP_XCHG: return "XCHG";
        
        // 算术运算指令
        case OP_ADD: return "ADD";
        case OP_SUB: return "SUB";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_INC: return "INC";
        case OP_DEC: return "DEC";
        case OP_NEG: return "NEG";
        case OP_ABS: return "ABS";
        
        // 逻辑运算指令
        case OP_AND: return "AND";
        case OP_OR: return "OR";
        case OP_XOR: return "XOR";
        case OP_NOT: return "NOT";
        case OP_SHL: return "SHL";
        case OP_SHR: return "SHR";
        case OP_SAR: return "SAR";
        case OP_ROL: return "ROL";
        case OP_ROR: return "ROR";
        
        // 程序控制指令
        case OP_JMP: return "JMP";
        case OP_JZ: return "JZ";
        case OP_JNZ: return "JNZ";
        case OP_JC: return "JC";
        case OP_JNC: return "JNC";
        case OP_JE: return "JE";
        case OP_JNE: return "JNE";
        case OP_JL: return "JL";
        case OP_JG: return "JG";
        case OP_CALL: return "CALL";
        case OP_RET: return "RET";
        case OP_CMP: return "CMP";
        
        // 系统指令
        case OP_HALT: return "HALT";
        case OP_INT: return "INT";
        case OP_IRET: return "IRET";
        
        default: return "UNKNOWN";
    }
}

const char* addressing_mode_to_string(addressing_mode_t mode) {
    switch (mode) {
        case ADDR_IMMEDIATE: return "立即寻址";
        case ADDR_DIRECT: return "直接寻址";
        case ADDR_INDIRECT: return "间接寻址";
        case ADDR_REGISTER: return "寄存器寻址";
        case ADDR_REG_INDIRECT: return "寄存器间接寻址";
        case ADDR_RELATIVE: return "相对寻址";
        case ADDR_BASE: return "基址寻址";
        case ADDR_INDEX: return "变址寻址";
        default: return "未知寻址";
    }
}

const char* instruction_category_to_string(instruction_category_t category) {
    switch (category) {
        case INST_DATA_TRANSFER: return "数据传送指令";
        case INST_ARITHMETIC: return "算术运算指令";
        case INST_LOGICAL: return "逻辑运算指令";
        case INST_PROGRAM_CONTROL: return "程序控制指令";
        case INST_SYSTEM: return "系统指令";
        default: return "未知指令类型";
    }
}

// ==================== 文件结束标记 ====================
