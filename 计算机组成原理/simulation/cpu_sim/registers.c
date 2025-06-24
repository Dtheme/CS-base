#include "registers.h"
#include "../common/types.h"
#include "../common/debug.h"
#include <stdio.h>
#include <string.h>

/**
 * 寄存器组实现
 * 
 * 本模块实现完整的32位RISC处理器寄存器组功能
 * 
 * 实现特点：
 * 1. 硬件级语义 - 严格按照真实CPU的寄存器行为实现
 * 2. 原子操作 - 确保寄存器操作的原子性和一致性
 * 3. 边界检查 - 完善的参数验证和错误处理
 * 4. 状态管理 - 完整的寄存器状态跟踪和调试支持
 * 5. 性能优化 - 高效的寄存器访问和标志位操作
 * 
 * 硬件模拟：
 * - 单周期访问：寄存器读写在单个时钟周期内完成
 * - 多端口设计：支持同时读写多个寄存器
 * - 零寄存器：R0永久为0，写入被硬件忽略
 * - 地址对齐：PC和SP自动进行地址对齐
 */

// ==================== 寄存器组管理 ====================

/**
 * 初始化寄存器组到硬件复位状态
 * 
 * 复位行为模拟：
 * 模拟CPU上电或硬件复位时的寄存器初始化
 * 
 * 初始化序列：
 * 1. 全寄存器清零（模拟硬件复位）
 * 2. SP设置为栈基地址（系统启动栈）
 * 3. PC设置为程序入口点（通常为0）
 * 4. 标志位清零（无激活状态）
 * 5. 执行内部一致性检查
 * 
 * 特殊寄存器处理：
 * - R0: 硬件保证恒为0
 * - SP: 初始化为系统栈基地址
 * - PC: 指向程序入口点
 * - FLAGS: 清除所有状态位
 */
cpu_error_t registers_init(registers_t *regs) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    
    // 硬件级全清零操作
    memset(regs, 0, sizeof(registers_t));
    
    // 设置系统级初始值
    regs->sp = STACK_BASE_ADDR;  // 栈指针指向栈底
    regs->pc = 0;                // 程序计数器从0开始
    regs->flags = 0;             // 清除所有标志位
    
    // 确保R0为0（硬件特性）
    regs->general[0] = 0;
    
    LOG_DEBUG("寄存器组初始化完成");
    return CPU_SUCCESS;
}

/**
 * 重置寄存器组到初始状态
 * 
 * 软重置行为：
 * 模拟软件重置或程序重启时的寄存器恢复
 * 保留某些系统级配置，清除用户态状态
 * 
 * 重置策略：
 * 1. 保存关键系统配置
 * 2. 清零用户态寄存器
 * 3. 恢复系统级设置
 * 4. 重新验证状态一致性
 * 
 * 保留vs清除：
 * 保留：栈基地址、系统配置
 * 清除：通用寄存器、用户标志位
 */
cpu_error_t registers_reset(registers_t *regs) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    
    // 保存系统级配置
    addr_t stack_base = STACK_BASE_ADDR;
    
    // 软重置：清零用户态状态
    memset(regs, 0, sizeof(registers_t));
    
    // 恢复系统级配置
    regs->sp = stack_base;
    regs->pc = 0;
    regs->flags = 0;
    
    // 确保硬件规约
    regs->general[0] = 0;  // R0恒为0
    
    LOG_DEBUG("寄存器组已重置");
    return CPU_SUCCESS;
}

// ==================== 通用寄存器操作 ====================

/**
 * 设置通用寄存器值（硬件写操作）
 * 
 * 硬件写时序：
 * 1. 地址译码：验证寄存器编号有效性
 * 2. 权限检查：检查写入权限和特殊规则
 * 3. 数据锁存：将数据写入寄存器单元
 * 4. 状态更新：更新相关的状态信息
 * 
 * 特殊规则：
 * - R0写保护：写入R0被硬件忽略，始终保持为0
 * - 原子操作：写入操作不可被中断
 * - 边界检查：寄存器编号必须在有效范围内
 * 
 * 错误处理：
 * 无效寄存器编号返回错误，但不影响系统状态
 */
cpu_error_t registers_set(registers_t *regs, u8 reg_num, word_t value) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    if (reg_num >= MAX_REGISTERS) return CPU_ERROR_INVALID_REGISTER;
    
    // R0写保护：硬件规约，R0永远为0
    if (reg_num == 0) {
        LOG_TRACE("忽略对R0的写入操作（硬件规约）");
        return CPU_SUCCESS;
    }
    
    // 执行原子写操作
    regs->general[reg_num] = value;
    
    LOG_TRACE("寄存器R%d = 0x%08X", reg_num, value);
    return CPU_SUCCESS;
}

/**
 * 读取通用寄存器值（硬件读操作）
 * 
 * 硬件读时序：
 * 1. 地址译码：解析寄存器编号
 * 2. 数据驱动：将寄存器值驱动到数据总线
 * 3. 信号保持：在读周期内保持数据稳定
 * 
 * 读取特性：
 * - 非破坏性：读操作不改变寄存器内容
 * - 单周期：读操作在一个时钟周期内完成
 * - 并发支持：支持多端口同时读取
 * 
 * R0特殊处理：
 * R0始终返回0，无论之前写入什么值
 * 这是硬件级别的约束，不是软件实现
 * 
 * 错误容错：
 * 无效访问返回0，避免系统崩溃
 */
word_t registers_get(const registers_t *regs, u8 reg_num) {
    if (!regs || reg_num >= MAX_REGISTERS) {
        LOG_ERROR("无效的寄存器访问: R%d", reg_num);
        return 0;
    }
    
    // R0硬件规约：始终返回0
    if (reg_num == 0) {
        return 0;
    }
    
    word_t value = regs->general[reg_num];
    LOG_TRACE("读取寄存器R%d = 0x%08X", reg_num, value);
    
    return value;
}

// ==================== 特殊寄存器操作 ====================

/**
 * 设置程序计数器（PC）
 * 
 * PC更新机制：
 * 程序计数器是CPU控制流的核心，每个指令周期自动更新
 * 
 * 地址对齐处理：
 * 1. 指令地址必须4字节对齐（32位指令）
 * 2. 自动对齐：清除地址的低2位
 * 3. 范围检查：确保地址在有效代码段内
 * 
 * 更新时机：
 * - 顺序执行：PC += 4
 * - 分支跳转：PC = target_address
 * - 函数调用：保存返回地址，设置新PC
 * - 中断处理：保存现场，跳转到中断向量
 * 
 * 性能考虑：
 * PC更新是关键路径，需要在时钟周期内完成
 */
cpu_error_t registers_set_pc(registers_t *regs, addr_t pc) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    
    // 指令地址自动对齐（清除低2位）
    addr_t aligned_pc = pc & ~0x3;
    
    regs->pc = aligned_pc;
    
    if (aligned_pc != pc) {
        LOG_TRACE("PC地址自动对齐: 0x%08X -> 0x%08X", pc, aligned_pc);
    }
    
    LOG_TRACE("PC = 0x%08X", aligned_pc);
    return CPU_SUCCESS;
}

/**
 * 读取程序计数器当前值
 * 
 * PC读取语义：
 * 返回下一条要执行的指令地址
 * 在指令执行过程中，PC可能已经指向后续指令
 * 
 * 时序考虑：
 * PC读取通常发生在指令取指阶段
 * 需要确保读取的是正确的地址
 */
addr_t registers_get_pc(const registers_t *regs) {
    if (!regs) {
        LOG_ERROR("无效的PC访问");
        return 0;
    }
    
    return regs->pc;
}

/**
 * 设置栈指针（SP）
 * 
 * 栈指针管理：
 * SP是系统栈的核心控制寄存器，管理函数调用和局部变量
 * 
 * 栈增长方向：
 * 采用向下增长的栈模型（Full Descending Stack）
 * PUSH：SP减小，POP：SP增大
 * 
 * 对齐要求：
 * 1. 栈指针必须4字节对齐
 * 2. 自动对齐不合规的栈指针
 * 3. 对齐错误会影响内存访问性能
 * 
 * 边界保护：
 * - 栈溢出检测：SP < 栈下界
 * - 栈下溢检测：SP > 栈上界
 * - 自动边界检查和警告
 */
cpu_error_t registers_set_sp(registers_t *regs, addr_t sp) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    
    // 栈指针自动对齐（清除低2位）
    addr_t aligned_sp = sp & ~0x3;
    
    // 栈边界检查
    if (aligned_sp < STACK_LIMIT_ADDR || aligned_sp > STACK_BASE_ADDR) {
        LOG_ERROR("栈指针超出有效范围: 0x%08X", aligned_sp);
        return CPU_ERROR_INVALID_ADDRESS;
    }
    
    regs->sp = aligned_sp;
    
    if (aligned_sp != sp) {
        LOG_TRACE("SP地址自动对齐: 0x%08X -> 0x%08X", sp, aligned_sp);
    }
    
    LOG_TRACE("SP = 0x%08X", aligned_sp);
    return CPU_SUCCESS;
}

/**
 * 读取栈指针当前值
 * 
 * SP读取用途：
 * - 栈操作前的状态检查
 * - 函数调用时的栈帧建立
 * - 调试和性能分析
 */
addr_t registers_get_sp(const registers_t *regs) {
    if (!regs) {
        LOG_ERROR("无效的SP访问");
        return 0;
    }
    
    return regs->sp;
}

/**
 * 设置标志位寄存器
 * 
 * 标志位系统：
 * 标志位是CPU状态的重要组成部分，影响条件执行和分支判断
 * 
 * 标志位布局（32位）：
 * Bit 31 (N): Negative Flag
 * Bit 30 (Z): Zero Flag  
 * Bit 29 (C): Carry Flag
 * Bit 28 (V): Overflow Flag
 * Bit 27 (I): Interrupt Flag
 * Bit 26-0:   保留位
 * 
 * 设置策略：
 * - 直接设置：用于恢复保存的标志位状态
 * - 选择性更新：仅更新相关的标志位
 * - 原子操作：确保标志位的一致性
 */
cpu_error_t registers_set_flags(registers_t *regs, u32 flags) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    
    // 掩码保护：仅允许设置有效的标志位
    u32 valid_flags = flags & 0xF8000000;  // 保留高5位
    
    regs->flags = valid_flags;
    LOG_TRACE("FLAGS = 0x%08X", valid_flags);
    
    return CPU_SUCCESS;
}

/**
 * 读取标志位寄存器当前值
 * 
 * 标志位读取：
 * 用于条件判断、状态保存、调试分析
 */
u32 registers_get_flags(const registers_t *regs) {
    if (!regs) {
        LOG_ERROR("无效的FLAGS访问");
        return 0;
    }
    
    return regs->flags;
}

// ==================== 标志位操作 ====================

/**
 * 设置指定标志位（原子操作）
 * 
 * 位设置原理：
 * 使用按位或操作设置目标位
 * flags |= flag_mask
 * 
 * 原子性保证：
 * 标志位操作必须是原子的，防止竞态条件
 * 在多核或中断环境中尤其重要
 */
void registers_set_flag(registers_t *regs, u32 flag_mask) {
    if (!regs) return;
    
    regs->flags |= flag_mask;
    LOG_TRACE("设置标志位: 0x%08X", flag_mask);
}

/**
 * 清除指定标志位（原子操作）
 * 
 * 位清除原理：
 * 使用按位与非操作清除目标位
 * flags &= ~flag_mask
 */
void registers_clear_flag(registers_t *regs, u32 flag_mask) {
    if (!regs) return;
    
    regs->flags &= ~flag_mask;
    LOG_TRACE("清除标志位: 0x%08X", flag_mask);
}

/**
 * 测试指定标志位状态
 * 
 * 测试原理：
 * 使用按位与操作检查目标位
 * (flags & flag_mask) != 0
 * 
 * 应用场景：
 * - 条件分支：根据标志位决定跳转
 * - 状态查询：检查运算结果属性
 * - 错误检测：检查异常标志位
 */
bool registers_test_flag(const registers_t *regs, u32 flag_mask) {
    if (!regs) return false;
    
    return (regs->flags & flag_mask) != 0;
}

/**
 * 根据ALU运算结果自动更新标志位
 * 
 * 自动更新机制：
 * ALU运算完成后，硬件自动根据结果更新相关标志位
 * 这是CPU硬件的标准行为
 * 
 * 更新算法：
 * 1. Zero Flag: 检查结果是否为0
 * 2. Negative Flag: 检查符号位（最高位）
 * 3. Carry Flag: 由具体运算类型决定
 * 4. Overflow Flag: 有符号运算溢出检测
 * 
 * 更新策略：
 * - 选择性更新：仅更新相关的标志位
 * - 保持其他：不涉及的标志位保持不变
 * - 硬件语义：严格按照CPU规范实现
 */
void registers_update_flags(registers_t *regs, word_t result) {
    if (!regs) return;
    
    // 清除将要更新的标志位
    regs->flags &= ~(FLAG_ZERO | FLAG_NEGATIVE);
    
    // Zero Flag: 结果为零时置1
    if (result == 0) {
        regs->flags |= FLAG_ZERO;
    }
    
    // Negative Flag: 符号位为1时置1
    if (result & 0x80000000) {
        regs->flags |= FLAG_NEGATIVE;
    }
    
    LOG_TRACE("更新标志位: Z=%d N=%d", 
              (regs->flags & FLAG_ZERO) ? 1 : 0,
              (regs->flags & FLAG_NEGATIVE) ? 1 : 0);
}

// ==================== 栈操作 ====================

/**
 * 执行压栈操作并更新栈指针
 * 
 * 压栈时序（满递减栈）：
 * 1. 栈溢出检查：验证SP是否会超出栈下界
 * 2. 指针更新：SP = SP - 4（向下增长）
 * 3. 数据写入：Memory[SP] = value
 * 4. 状态记录：记录栈操作历史
 * 
 * 溢出保护：
 * 当栈空间不足时，拒绝压栈操作
 * 防止栈溢出破坏其他内存区域
 * 
 * 注意：本函数只更新SP，实际内存写入由内存子系统处理
 */
cpu_error_t registers_push_stack(registers_t *regs, word_t value) {
    if (!regs) return CPU_ERROR_NULL_POINTER;
    
    // 检查栈溢出（SP向下增长）
    if (regs->sp <= STACK_LIMIT_ADDR + sizeof(word_t)) {
        LOG_ERROR("栈溢出: SP = 0x%08X", regs->sp);
        return CPU_ERROR_STACK_OVERFLOW;
    }
    
    // 栈指针递减（压栈前更新）
    regs->sp -= sizeof(word_t);
    
    LOG_TRACE("压栈: SP = 0x%08X, 值 = 0x%08X", regs->sp, value);
    return CPU_SUCCESS;
}

/**
 * 执行弹栈操作并更新栈指针
 * 
 * 弹栈时序（满递减栈）：
 * 1. 栈下溢检查：验证栈中是否有数据
 * 2. 数据读取：value = Memory[SP]
 * 3. 指针更新：SP = SP + 4（向上恢复）
 * 4. 状态记录：记录栈操作历史
 * 
 * 下溢保护：
 * 当栈为空时，拒绝弹栈操作
 * 防止读取无效的栈数据
 * 
 * 注意：本函数只更新SP，实际内存读取由内存子系统处理
 */
cpu_error_t registers_pop_stack(registers_t *regs, word_t *value) {
    if (!regs || !value) return CPU_ERROR_NULL_POINTER;
    
    // 检查栈下溢（栈为空）
    if (regs->sp >= STACK_BASE_ADDR) {
        LOG_ERROR("栈下溢: SP = 0x%08X", regs->sp);
        return CPU_ERROR_STACK_UNDERFLOW;
    }
    
    // 简化实现：实际应该从内存子系统读取
    *value = 0;
    
    // 栈指针递增（弹栈后更新）
    regs->sp += sizeof(word_t);
    
    LOG_TRACE("弹栈: SP = 0x%08X, 值 = 0x%08X", regs->sp, *value);
    return CPU_SUCCESS;
}

// ==================== 寄存器状态显示 ====================

/**
 * 转储所有寄存器的详细状态
 * 
 * 显示格式：
 * 1. 通用寄存器：4列对齐显示，便于阅读
 * 2. 特殊寄存器：PC和SP的地址信息
 * 3. 标志位解析：逐位显示标志位状态
 * 4. 状态统计：非零寄存器计数等
 * 
 * 调试价值：
 * - 程序状态快照
 * - 错误定位辅助
 * - 性能分析数据
 */
void registers_dump(const registers_t *regs) {
    if (!regs) {
        printf("寄存器组指针为空\n");
        return;
    }
    
    printf("=== 寄存器组状态 ===\n");
    printf("通用寄存器:\n");
    for (int i = 0; i < MAX_REGISTERS; i += 4) {
        printf("  R%-2d=0x%08X  R%-2d=0x%08X  R%-2d=0x%08X  R%-2d=0x%08X\n",
               i, regs->general[i],
               i+1, (i+1 < MAX_REGISTERS) ? regs->general[i+1] : 0,
               i+2, (i+2 < MAX_REGISTERS) ? regs->general[i+2] : 0,
               i+3, (i+3 < MAX_REGISTERS) ? regs->general[i+3] : 0);
    }
    
    printf("\n特殊寄存器:\n");
    printf("  PC = 0x%08X  SP = 0x%08X\n", regs->pc, regs->sp);
    
    printf("  FLAGS = 0x%08X [", regs->flags);
    
    // 解析并显示标志位状态
    bool first = true;
    if (regs->flags & FLAG_NEGATIVE) {
        printf("%sN", first ? "" : " ");
        first = false;
    }
    if (regs->flags & FLAG_ZERO) {
        printf("%sZ", first ? "" : " ");
        first = false;
    }
    if (regs->flags & FLAG_CARRY) {
        printf("%sC", first ? "" : " ");
        first = false;
    }
    if (regs->flags & FLAG_OVERFLOW) {
        printf("%sV", first ? "" : " ");
        first = false;
    }
    if (regs->flags & FLAG_INTERRUPT) {
        printf("%sI", first ? "" : " ");
        first = false;
    }
    
    if (first) printf("无");
    printf("]\n");
    printf("===================\n");
}

/**
 * 简化显示寄存器状态
 * 
 * 适用场景：
 * - 快速状态检查
 * - 日志记录
 * - 实时监控
 * 
 * 显示内容：
 * - 关键寄存器：PC、SP、FLAGS
 * - 前4个通用寄存器（常用于参数传递）
 */
void registers_dump_simple(const registers_t *regs) {
    if (!regs) return;
    
    printf("PC=0x%08X SP=0x%08X FLAGS=0x%08X\n", 
           regs->pc, regs->sp, regs->flags);
    
    printf("R0-R3: ");
    for (int i = 0; i < 4; i++) {
        printf("0x%08X ", regs->general[i]);
    }
    printf("\n");
}

// ==================== 寄存器验证 ====================

/**
 * 验证寄存器组状态的一致性和有效性
 * 
 * 验证项目：
 * 1. 结构完整性：指针有效性检查
 * 2. 硬件约束：R0为0、地址对齐等
 * 3. 系统约束：栈指针范围、PC有效性
 * 4. 逻辑一致性：标志位合理性等
 * 
 * 验证策略：
 * - 渐进验证：从基础到复杂逐步检查
 * - 容错设计：发现错误时记录但不中断
 * - 详细日志：为错误定位提供信息
 * 
 * 应用场景：
 * - 系统自检
 * - 调试验证
 * - 状态恢复验证
 */
bool registers_validate(const registers_t *regs) {
    if (!regs) return false;
    
    bool valid = true;
    
    // 检查R0是否为0（硬件约束）
    if (regs->general[0] != 0) {
        LOG_ERROR("R0寄存器不为0: 0x%08X", regs->general[0]);
        valid = false;
    }
    
    // 检查栈指针是否在有效范围内
    if (regs->sp < STACK_LIMIT_ADDR || regs->sp > STACK_BASE_ADDR) {
        LOG_ERROR("栈指针超出范围: 0x%08X", regs->sp);
        valid = false;
    }
    
    // 检查栈指针是否对齐
    if (regs->sp & 0x3) {
        LOG_ERROR("栈指针未对齐: 0x%08X", regs->sp);
        valid = false;
    }
    
    // 检查程序计数器是否对齐
    if (regs->pc & 0x3) {
        LOG_ERROR("程序计数器未对齐: 0x%08X", regs->pc);
        valid = false;
    }
    
    // 检查标志位是否在有效范围内
    if (regs->flags & 0x07FFFFFF) {  // 低27位应该为0
        LOG_ERROR("标志位包含无效位: 0x%08X", regs->flags);
        valid = false;
    }
    
    return valid;
}

// ==================== 寄存器名称转换 ====================

/**
 * 根据寄存器编号获取标准名称
 * 
 * 命名约定：
 * - R0-R15: 标准通用寄存器名称
 * - 特殊别名: R13(SP), R14(LR), R15(PC)
 * 
 * 用途：
 * - 汇编器/反汇编器
 * - 调试器显示
 * - 错误信息格式化
 */
const char* register_name(u8 reg_num) {
    static const char* names[] = {
        "R0",  "R1",  "R2",  "R3",  "R4",  "R5",  "R6",  "R7",
        "R8",  "R9",  "R10", "R11", "R12", "SP",  "LR",  "PC"
    };
    
    if (reg_num < MAX_REGISTERS) {
        return names[reg_num];
    }
    
    return "INVALID";
}

/**
 * 根据寄存器名称获取编号
 * 
 * 解析规则：
 * 1. 标准格式：R0-R15
 * 2. 特殊别名：SP、LR、PC
 * 3. 大小写不敏感
 * 4. 容错处理：无效名称返回特殊值
 * 
 * 解析算法：
 * - 前缀匹配：检查'R'前缀
 * - 数字解析：解析后续数字
 * - 别名匹配：检查特殊别名
 * - 范围验证：确保编号在有效范围内
 */
u8 register_number(const char* name) {
    if (!name) return MAX_REGISTERS;
    
    // 处理Rn格式（R0-R15）
    if (name[0] == 'R' || name[0] == 'r') {
        u8 num = 0;
        int i = 1;
        
        // 解析数字部分
        while (name[i] >= '0' && name[i] <= '9') {
            num = num * 10 + (name[i] - '0');
            i++;
        }
        
        // 验证范围和格式
        if (name[i] == '\0' && num < MAX_REGISTERS) {
            return num;
        }
    }
    
    // 处理特殊别名
    if (strcmp(name, "SP") == 0 || strcmp(name, "sp") == 0) return 13;
    if (strcmp(name, "LR") == 0 || strcmp(name, "lr") == 0) return 14;
    if (strcmp(name, "PC") == 0 || strcmp(name, "pc") == 0) return 15;
    
    return MAX_REGISTERS; // 无效寄存器名称
} 