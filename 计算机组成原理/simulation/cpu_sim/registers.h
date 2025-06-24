#ifndef CPU_SIM_REGISTERS_H
#define CPU_SIM_REGISTERS_H

#include "../common/types.h"

/**
 * CPU寄存器组管理模块
 * 
 * 设计原理：
 * 寄存器是CPU中最快的存储单元，直接连接到ALU和控制单元。
 * 本模块实现了完整的32位RISC架构寄存器组，模拟现代处理器的寄存器结构。
 * 
 * 架构特点：
 * 1. 统一寄存器文件 - 通用寄存器可用于任何操作
 * 2. 硬件级访问速度 - 单周期读写访问
 * 3. 多端口设计 - 支持同时读写多个寄存器
 * 4. 零寄存器规约 - R0始终为0，写入无效
 * 5. 标志位集成 - 内置完整的条件码系统
 * 
 * 寄存器布局：
 * - R0-R15: 通用寄存器（32位）
 * - PC: 程序计数器（地址位宽）
 * - SP: 栈指针（地址位宽）  
 * - FLAGS: 标志位寄存器（32位）
 * 
 * 调用约定：
 * - R0: 常量零寄存器
 * - R1-R3: 函数参数和返回值
 * - R4-R11: 临时寄存器
 * - R12-R14: 保存寄存器
 * - R15: 链接寄存器（函数调用）
 * 
 * 标志位系统：
 * 实现完整的NZCV标志位，支持条件执行和分支判断
 */

// ==================== 寄存器组结构定义 ====================

/**
 * CPU寄存器组结构体
 * 
 * 设计考虑：
 * 1. 内存对齐 - 确保高效的缓存访问
 * 2. 字段顺序 - 按访问频率排列
 * 3. 大小优化 - 最小化结构体占用空间
 * 4. 扩展性 - 预留空间支持未来架构扩展
 * 
 * 存储布局：
 * - general[]: 通用寄存器数组，支持高效的索引访问
 * - pc: 32位或64位程序计数器，指向下一条指令
 * - sp: 栈指针，指向当前栈顶位置
 * - flags: 32位标志位寄存器，存储运算状态
 * 
 * 访问模式：
 * - 直接索引：通过寄存器编号快速访问
 * - 边界检查：防止越界访问和非法操作
 * - 原子操作：确保寄存器操作的原子性
 */
typedef struct {
    word_t general[MAX_REGISTERS];  // 通用寄存器组 R0-R15
    addr_t pc;                      // 程序计数器（指令地址）
    addr_t sp;                      // 栈指针（栈顶地址）
    u32 flags;                      // 标志位寄存器（NZCV等）
} registers_t;

// ==================== 寄存器操作函数 ====================

/**
 * 初始化寄存器组到默认状态
 * 
 * 初始化规程：
 * 1. 清零所有通用寄存器（R0-R15 = 0）
 * 2. 设置PC为程序起始地址（通常为0）
 * 3. 初始化SP为栈底地址
 * 4. 清零所有标志位
 * 5. 执行硬件级自检和验证
 * 
 * 特殊处理：
 * - R0寄存器：永远保持为0
 * - SP初始化：设置为合理的栈底地址
 * - PC初始化：可配置的程序入口点
 * 
 * @param regs 寄存器组结构指针，不能为NULL
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t registers_init(registers_t *regs);

/**
 * 重置寄存器组到初始状态
 * 
 * 重置策略：
 * 与初始化相同，但保留某些关键设置
 * 用于程序重启、异常恢复等场景
 * 
 * 重置范围：
 * - 清零所有通用寄存器
 * - 重置PC到程序入口
 * - 恢复SP到初始栈位置
 * - 清除所有状态标志位
 * 
 * 保留内容：
 * - 寄存器组配置信息
 * - 调试和性能计数器
 * 
 * @param regs 寄存器组结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t registers_reset(registers_t *regs);

// ==================== 通用寄存器操作 ====================

/**
 * 设置通用寄存器值
 * 
 * 操作机制：
 * 1. 寄存器编号验证（0-15有效范围）
 * 2. R0特殊处理（写入被忽略，始终为0）
 * 3. 原子写入操作
 * 4. 可选的写入日志记录
 * 
 * 硬件语义：
 * 模拟真实CPU的寄存器写入时序
 * 单周期完成，不会被中断
 * 
 * 边界检查：
 * - 寄存器编号范围验证
 * - 空指针检查
 * - 值范围验证（32位）
 * 
 * @param regs 寄存器组结构指针
 * @param reg_num 寄存器编号（0-15）
 * @param value 要设置的32位值
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t registers_set(registers_t *regs, u8 reg_num, word_t value);

/**
 * 读取通用寄存器值
 * 
 * 读取特性：
 * 1. 非破坏性读取（不改变寄存器内容）
 * 2. 原子读取操作（单周期完成）
 * 3. R0恒定返回0值
 * 4. 边界检查和错误处理
 * 
 * 性能优化：
 * - 直接数组访问，O(1)时间复杂度
 * - 无需拷贝，直接返回值
 * - 编译器优化友好
 * 
 * 错误处理：
 * 无效寄存器编号返回0值
 * 空指针情况返回0值
 * 
 * @param regs 寄存器组结构指针
 * @param reg_num 寄存器编号（0-15）
 * @return 寄存器的32位值，错误时返回0
 */
word_t registers_get(const registers_t *regs, u8 reg_num);

// ==================== 特殊寄存器操作 ====================

/**
 * 设置程序计数器（PC）
 * 
 * PC管理原理：
 * 程序计数器指向下一条要执行的指令地址
 * 在指令执行过程中自动递增
 * 
 * 地址对齐：
 * 1. 指令地址必须按字对齐（4字节边界）
 * 2. 自动对齐无效地址（清除低2位）
 * 3. 地址范围验证
 * 
 * 分支处理：
 * - 直接分支：PC = target_address
 * - 相对分支：PC = PC + offset
 * - 间接分支：PC = register_value
 * 
 * 特殊情况：
 * - 中断处理：保存当前PC，加载中断向量
 * - 异常处理：PC指向异常处理程序
 * 
 * @param regs 寄存器组结构指针
 * @param pc 新的程序计数器值
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t registers_set_pc(registers_t *regs, addr_t pc);

/**
 * 读取程序计数器（PC）当前值
 * 
 * 返回值语义：
 * 返回当前PC值，指向下一条指令
 * 
 * @param regs 寄存器组结构指针
 * @return 当前程序计数器值
 */
addr_t registers_get_pc(const registers_t *regs);

/**
 * 设置栈指针（SP）
 * 
 * 栈管理原理：
 * 栈指针指向当前栈顶元素的地址
 * 采用满递减栈模型（Full Descending Stack）
 * 
 * 栈操作语义：
 * - PUSH：SP = SP - 4，然后写入数据
 * - POP：读取数据，然后SP = SP + 4
 * 
 * 对齐要求：
 * 栈指针必须按字对齐（4字节边界）
 * 自动对齐不规范的栈指针值
 * 
 * 边界检查：
 * - 栈溢出检测：SP超出栈空间下界
 * - 栈下溢检测：SP超出栈空间上界
 * - 地址有效性验证
 * 
 * @param regs 寄存器组结构指针
 * @param sp 新的栈指针值
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t registers_set_sp(registers_t *regs, addr_t sp);

/**
 * 读取栈指针（SP）当前值
 * 
 * @param regs 寄存器组结构指针
 * @return 当前栈指针值
 */
addr_t registers_get_sp(const registers_t *regs);

/**
 * 设置标志位寄存器
 * 
 * 标志位布局（32位寄存器）：
 * Bit 31 (N): Negative Flag    - 结果为负
 * Bit 30 (Z): Zero Flag        - 结果为零
 * Bit 29 (C): Carry Flag       - 进位/借位
 * Bit 28 (V): Overflow Flag    - 有符号溢出
 * Bit 27-0:   保留位           - 预留给扩展标志
 * 
 * 条件码映射：
 * - EQ (Equal): Z=1
 * - NE (Not Equal): Z=0
 * - GT (Greater Than): Z=0 && N=V
 * - LT (Less Than): N!=V
 * - GE (Greater Equal): N=V
 * - LE (Less Equal): Z=1 || N!=V
 * 
 * @param regs 寄存器组结构指针
 * @param flags 新的标志位值
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t registers_set_flags(registers_t *regs, u32 flags);

/**
 * 读取标志位寄存器当前值
 * 
 * @param regs 寄存器组结构指针
 * @return 当前标志位寄存器值
 */
u32 registers_get_flags(const registers_t *regs);

// ==================== 标志位操作 ====================

/**
 * 设置指定标志位
 * 
 * 位操作原理：
 * 使用按位或操作设置指定位
 * flags |= flag_mask
 * 
 * 原子操作：
 * 确保标志位操作的原子性
 * 防止并发访问时的竞态条件
 * 
 * @param regs 寄存器组结构指针
 * @param flag_mask 要设置的标志位掩码
 */
void registers_set_flag(registers_t *regs, u32 flag_mask);

/**
 * 清除指定标志位
 * 
 * 位操作原理：
 * 使用按位与非操作清除指定位
 * flags &= ~flag_mask
 * 
 * @param regs 寄存器组结构指针
 * @param flag_mask 要清除的标志位掩码
 */
void registers_clear_flag(registers_t *regs, u32 flag_mask);

/**
 * 测试指定标志位状态
 * 
 * 测试方法：
 * 使用按位与操作检查标志位
 * (flags & flag_mask) != 0
 * 
 * @param regs 寄存器组结构指针
 * @param flag_mask 要测试的标志位掩码
 * @return 标志位为1返回true，为0返回false
 */
bool registers_test_flag(const registers_t *regs, u32 flag_mask);

/**
 * 根据ALU运算结果自动更新标志位
 * 
 * 自动更新机制：
 * 1. Zero Flag: result == 0
 * 2. Negative Flag: result[31] == 1
 * 3. Carry Flag: 由具体运算类型决定
 * 4. Overflow Flag: 有符号运算溢出检测
 * 
 * 更新策略：
 * - 保留未涉及的标志位
 * - 仅更新相关的标志位
 * - 支持选择性更新
 * 
 * 应用场景：
 * - ALU运算后的标志位更新
 * - 比较指令的标志位设置
 * - 条件执行的依据更新
 * 
 * @param regs 寄存器组结构指针
 * @param result ALU运算结果值
 */
void registers_update_flags(registers_t *regs, word_t result);

// ==================== 栈操作 ====================

/**
 * 执行压栈操作并更新栈指针
 * 
 * 压栈时序：
 * 1. SP = SP - 4 (栈指针递减)
 * 2. Memory[SP] = value (写入数据)
 * 3. 检查栈溢出
 * 
 * 栈溢出检测：
 * 检查SP是否超出栈空间下界
 * 防止栈溢出损坏其他内存区域
 * 
 * 错误处理：
 * 栈溢出时不执行写入，返回错误码
 * 
 * @param regs 寄存器组结构指针
 * @param value 要压入栈的32位值
 * @return 成功返回CPU_SUCCESS，栈溢出返回错误码
 */
cpu_error_t registers_push_stack(registers_t *regs, word_t value);

/**
 * 执行弹栈操作并更新栈指针
 * 
 * 弹栈时序：
 * 1. value = Memory[SP] (读取数据)
 * 2. SP = SP + 4 (栈指针递增)
 * 3. 检查栈下溢
 * 
 * 栈下溢检测：
 * 检查SP是否超出栈空间上界
 * 防止读取无效的栈数据
 * 
 * @param regs 寄存器组结构指针
 * @param value 输出参数，存储弹出的值
 * @return 成功返回CPU_SUCCESS，栈下溢返回错误码
 */
cpu_error_t registers_pop_stack(registers_t *regs, word_t *value);

// ==================== 调试和显示功能 ====================

/**
 * 转储所有寄存器的详细状态
 * 
 * 输出格式：
 * 1. 通用寄存器：十六进制和十进制双格式显示
 * 2. 特殊寄存器：PC、SP的地址显示
 * 3. 标志位：逐位解析显示（N/Z/C/V）
 * 4. 寄存器使用情况统计
 * 
 * 显示布局：
 * 采用表格形式，便于阅读和分析
 * 突出显示非零寄存器和活跃标志位
 * 
 * @param regs 寄存器组结构指针
 */
void registers_dump(const registers_t *regs);

/**
 * 简化输出寄存器状态
 * 
 * 简化内容：
 * 仅显示非零寄存器和关键信息
 * 适用于快速状态检查
 * 
 * @param regs 寄存器组结构指针
 */
void registers_dump_simple(const registers_t *regs);

// ==================== 验证和工具函数 ====================

/**
 * 验证寄存器组状态的一致性
 * 
 * 验证项目：
 * 1. 结构指针有效性
 * 2. R0寄存器恒为0
 * 3. PC地址对齐检查
 * 4. SP地址对齐检查
 * 5. 标志位合法性
 * 
 * 完整性检查：
 * - 内存地址范围验证
 * - 标志位组合合理性
 * - 寄存器值域检查
 * 
 * @param regs 寄存器组结构指针
 * @return 状态一致返回true，否则返回false
 */
bool registers_validate(const registers_t *regs);

/**
 * 根据寄存器编号获取寄存器名称
 * 
 * 命名规则：
 * - R0-R15: 通用寄存器名称
 * - 特殊别名：R13=SP, R14=LR, R15=PC
 * 
 * @param reg_num 寄存器编号（0-15）
 * @return 寄存器名称字符串，无效编号返回"INVALID"
 */
const char* register_name(u8 reg_num);

/**
 * 根据寄存器名称获取编号
 * 
 * 名称解析：
 * 支持标准名称（R0-R15）和别名（SP、LR、PC）
 * 大小写不敏感匹配
 * 
 * @param name 寄存器名称字符串
 * @return 寄存器编号，无效名称返回0xFF
 */
u8 register_number(const char* name);

#endif // CPU_SIM_REGISTERS_H 