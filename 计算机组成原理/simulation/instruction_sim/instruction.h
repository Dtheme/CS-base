#ifndef INSTRUCTION_SIM_H
#define INSTRUCTION_SIM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 指令系统模拟器 - 专业级处理器指令集架构仿真平台
 * 
 * 本模块实现了完整的指令集架构(ISA)仿真，基于现代处理器设计原理：
 * 
 * === 指令集设计原理 ===
 * 1. 指令格式设计
 *    - 固定长度vs变长编码的权衡: 固定长度简化取指单元设计，变长提高代码密度
 *    - 操作码扩展技术: 使用分层编码最大化指令空间利用率
 *    - 字段对齐策略: 关键字段对齐到字节边界，减少译码逻辑复杂度
 * 
 * 2. 寻址方式优化
 *    - 立即寻址: 减少内存访问，提升指令吞吐率，但限制操作数范围
 *    - 基址+偏移: 支持大地址空间，硬件实现复杂度为O(1)加法器
 *    - 后增量寻址: 简化循环优化，减少地址计算指令数量
 *    - 相对寻址: 支持位置无关代码，简化动态链接器设计
 * 
 * 3. 指令类型分类
 *    - 数据传输: 占指令流20-40%，是内存子系统性能瓶颈
 *    - 算术逻辑: 占指令流30-50%，决定ALU设计复杂度
 *    - 控制流转移: 占指令流10-20%，影响分支预测器设计
 *    - 系统管理: 特权级别控制，支持操作系统虚拟化
 * 
 * === 微架构性能优化 ===
 * 1. 指令编码优化
 *    - 使用Huffman编码原理，高频指令分配短操作码
 *    - 关键路径优化: 译码器设计考虑时序约束
 *    - 并行译码: 支持超标量处理器多发射需求
 * 
 * 2. 执行效率提升
 *    - 指令融合: 常见指令序列合并为单一微操作
 *    - 零延迟bypass: 前向数据通路减少流水线气泡
 *    - 推测执行: 分支预测配合乱序执行提升IPC
 * 
 * 3. 存储访问优化
 *    - 地址生成单元(AGU): 专用硬件加速有效地址计算
 *    - 访存指令预取: 基于访问模式的硬件预取器
 *    - Store buffer: 减少写后读数据相关的延迟
 * 
 * === RISC-V架构特性 ===
 * - 模块化扩展: 基础整数指令集(RV32I) + 可选扩展
 * - 线性指令编码: 简化译码逻辑，降低功耗
 * - 压缩指令集(C扩展): 提高代码密度，减少指令缓存失效
 * - 向量处理(V扩展): 支持SIMD并行计算模式
 * 
 * 本模拟器支持教学和研究用途的完整指令集仿真，
 * 包含详细的微架构级别性能分析和优化技术演示。
 */

// ==================== 基本类型定义 ====================

typedef uint8_t  u8;    // 8位无符号整数
typedef uint16_t u16;   // 16位无符号整数  
typedef uint32_t u32;   // 32位无符号整数
typedef uint64_t u64;   // 64位无符号整数
typedef int32_t  s32;   // 32位有符号整数

// ==================== 基本常量定义 ====================

#define MAX_REGISTERS           16      // 最大寄存器数量(基于典型RISC架构)
#define MAX_MEMORY_SIZE         65536   // 最大内存大小：64KB(教学友好的地址空间)
#define MAX_INSTRUCTION_SIZE    4       // 最大指令长度（字节，对应32位固定长度）
#define MAX_PROGRAM_SIZE        1024    // 最大程序大小（指令数，适合教学演示）
#define STACK_SIZE              1024    // 栈大小(支持合理的递归深度)

// ==================== 错误码定义 ====================

typedef enum {
    CPU_SUCCESS = 0,                    // 执行成功
    CPU_ERROR_NONE = 0,                 // 无错误（与CPU_SUCCESS相同）
    CPU_ERROR_NULL_POINTER = -1,        // 空指针错误
    CPU_ERROR_INVALID_ADDR = -2,        // 无效地址
    CPU_ERROR_MEMORY_FAULT = -3,        // 内存错误
    CPU_ERROR_DIVISION_BY_ZERO = -4,    // 除零错误
    CPU_ERROR_STACK_OVERFLOW = -5,      // 栈溢出
    CPU_ERROR_STACK_UNDERFLOW = -6,     // 栈下溢
    CPU_ERROR_INVALID_OPCODE = -7,      // 无效操作码
    CPU_ERROR_INVALID_REGISTER = -8,    // 无效寄存器
    CPU_ERROR_INVALID_PARAMETER = -9,   // 无效参数
    
    // 指令系统专用错误码
    CPU_ERROR_PROGRAM_TOO_LARGE = -100,    // 程序过大
    CPU_ERROR_PROGRAM_FINISHED = -101,     // 程序执行完成
    CPU_ERROR_INVALID_PC = -102,           // 无效程序计数器
    CPU_ERROR_INVALID_ADDRESSING_MODE = -103, // 无效寻址方式
    CPU_ERROR_MEMORY_ACCESS_VIOLATION = -104, // 内存访问违例
    CPU_ERROR_INVALID_OPERATION = -105,    // 无效操作
    CPU_ERROR_EXECUTION_TIMEOUT = -106,    // 执行超时
    CPU_ERROR_INTERRUPT = -107,            // 中断
} cpu_error_t;

// ==================== 指令格式定义 ====================

/**
 * 指令字长类型
 * 
 * 设计考虑:
 * - 16位: 适合嵌入式系统，代码密度高，但指令表达能力有限
 * - 32位: 平衡性能和复杂度，主流RISC架构选择
 * - 固定长度简化取指单元设计，支持更高频率
 */
typedef enum {
    INST_FORMAT_16BIT = 0,      // 16位指令字(压缩指令集)
    INST_FORMAT_32BIT = 1       // 32位指令字(标准RISC格式)
} instruction_format_t;

/**
 * 地址码数量类型
 * 
 * 指令格式演化:
 * - 零地址: 栈机器，实现简单但表达能力受限
 * - 一地址: 累加器架构，早期计算机常用
 * - 二地址: 现代处理器主流，平衡复杂度和性能
 * - 三地址: 最强表达能力，但编码空间消耗大
 */
typedef enum {
    ADDR_ZERO = 0,              // 零地址指令(栈操作)
    ADDR_ONE = 1,               // 一地址指令(累加器模式)
    ADDR_TWO = 2,               // 二地址指令(寄存器-寄存器)
    ADDR_THREE = 3              // 三地址指令(完全通用)
} address_type_t;

// ==================== 寻址方式定义 ====================

/**
 * 寻址方式枚举
 * 
 * 微架构实现复杂度分析:
 * - 立即寻址: 0个额外时钟周期，操作数嵌入指令中
 * - 直接寻址: 1个内存访问周期，简单地址译码
 * - 间接寻址: 2个内存访问周期，支持动态地址计算
 * - 寄存器寻址: 0个额外周期，最快访问方式
 * - 寄存器间接: 1个内存访问 + 寄存器读取
 * - 相对寻址: 1个加法器操作，支持位置无关代码
 * - 基址寻址: 专用AGU(地址生成单元)，支持数组访问
 * - 变址寻址: 复杂AGU，支持多维数组和循环优化
 */
typedef enum {
    ADDR_IMMEDIATE = 0,         // 立即寻址(操作数在指令中)
    ADDR_DIRECT = 1,            // 直接寻址(指令包含内存地址)
    ADDR_INDIRECT = 2,          // 间接寻址(指令包含地址的地址)
    ADDR_REGISTER = 3,          // 寄存器寻址(操作数在寄存器中)
    ADDR_REG_INDIRECT = 4,      // 寄存器间接寻址(寄存器包含内存地址)
    ADDR_RELATIVE = 5,          // 相对寻址(PC + 偏移量)
    ADDR_BASE = 6,              // 基址寻址(基址寄存器 + 偏移)
    ADDR_INDEX = 7              // 变址寻址(基址 + 变址寄存器 * 比例因子)
} addressing_mode_t;

// ==================== 指令类型定义 ====================

/**
 * 指令分类枚举
 * 
 * 基于现代处理器指令混合分析:
 * - 数据传送指令: 20-40%，决定内存子系统设计
 * - 算术运算指令: 30-50%，影响ALU和乘除器配置
 * - 逻辑运算指令: 5-15%，简单组合逻辑实现
 * - 程序控制指令: 10-20%，分支预测器性能关键
 * - 系统指令: <5%，特权模式和中断处理
 */
typedef enum {
    INST_DATA_TRANSFER = 0,     // 数据传送指令(LOAD/STORE架构基础)
    INST_ARITHMETIC = 1,        // 算术运算指令(整数ALU操作)
    INST_LOGICAL = 2,           // 逻辑运算指令(位操作和布尔运算)
    INST_PROGRAM_CONTROL = 3,   // 程序控制指令(分支和跳转)
    INST_SYSTEM = 4             // 系统指令(特权操作和中断)
} instruction_category_t;

/**
 * 具体指令操作码
 * 
 * 编码策略:
 * - 使用分层编码节省操作码空间
 * - 高频指令分配短编码(类Huffman编码)
 * - 相关指令归组，简化译码器设计
 * - 保留扩展空间支持未来指令集扩展
 */
typedef enum {
    // 数据传送指令 (0x00-0x0F) - 内存子系统交互
    OP_NOP = 0x00,              // 空操作(pipeline bubble填充)
    OP_LOAD = 0x01,             // 加载(内存到寄存器)
    OP_STORE = 0x02,            // 存储(寄存器到内存)
    OP_MOVE = 0x03,             // 移动(寄存器间传输)
    OP_PUSH = 0x04,             // 压栈(自动递减栈指针)
    OP_POP = 0x05,              // 弹栈(自动递增栈指针)
    OP_XCHG = 0x06,             // 交换(原子操作，支持多线程)
    
    // 算术运算指令 (0x10-0x1F) - ALU核心功能
    OP_ADD = 0x10,              // 加法(基础整数运算)
    OP_SUB = 0x11,              // 减法(通过补码加法实现)
    OP_MUL = 0x12,              // 乘法(可能需要多周期或专用乘法器)
    OP_DIV = 0x13,              // 除法(复杂操作，通常微编码实现)
    OP_INC = 0x14,              // 自增(优化的+1操作)
    OP_DEC = 0x15,              // 自减(优化的-1操作)
    OP_NEG = 0x16,              // 取负(补码运算)
    OP_ABS = 0x17,              // 绝对值(条件操作)
    
    // 逻辑运算指令 (0x20-0x2F) - 位级操作
    OP_AND = 0x20,              // 逻辑与(按位AND)
    OP_OR = 0x21,               // 逻辑或(按位OR)
    OP_XOR = 0x22,              // 逻辑异或(按位XOR)
    OP_NOT = 0x23,              // 逻辑非(按位取反)
    OP_SHL = 0x24,              // 逻辑左移(填0，等价乘2^n)
    OP_SHR = 0x25,              // 逻辑右移(填0，无符号除2^n)
    OP_SAR = 0x26,              // 算术右移(符号扩展，有符号除2^n)
    OP_ROL = 0x27,              // 循环左移(位旋转，无丢失)
    OP_ROR = 0x28,              // 循环右移(位旋转，无丢失)
    
    // 程序控制指令 (0x30-0x3F) - 控制流管理
    OP_JMP = 0x30,              // 无条件跳转(直接修改PC)
    OP_JZ = 0x31,               // 零跳转(条件分支)
    OP_JNZ = 0x32,              // 非零跳转(条件分支)
    OP_JC = 0x33,               // 进位跳转(算术结果检查)
    OP_JNC = 0x34,              // 非进位跳转(算术结果检查)
    OP_JE = 0x35,               // 相等跳转(比较结果)
    OP_JNE = 0x36,              // 不等跳转(比较结果)
    OP_JL = 0x37,               // 小于跳转(有符号比较)
    OP_JG = 0x38,               // 大于跳转(有符号比较)
    OP_CALL = 0x39,             // 子程序调用(保存返回地址)
    OP_RET = 0x3A,              // 子程序返回(恢复PC)
    OP_CMP = 0x3B,              // 比较(设置标志位，不保存结果)
    
    // 系统指令 (0x40-0x4F) - 特权操作
    OP_HALT = 0x40,             // 停机(处理器进入停止状态)
    OP_INT = 0x41,              // 中断(软件中断触发)
    OP_IRET = 0x42              // 中断返回(恢复中断前状态)
} opcode_t;

// ==================== 指令结构定义 ====================

/**
 * 16位指令格式（演示扩展操作码技术）
 * 
 * 扩展操作码设计原理:
 * - 使用分层结构最大化指令空间利用率
 * - 高频指令使用短操作码，低频指令使用长操作码
 * - 译码器可采用树形结构，平衡译码延迟和硬件复杂度
 * 
 * 字段布局考虑:
 * - 操作码位于高位，简化初级译码
 * - 寄存器字段对齐到4位边界，简化译码逻辑
 * - 立即数字段最大化，提高常数编码效率
 */
typedef union {
    u16 raw;                    // 原始16位数据

    // 三地址指令格式 - 最大操作数灵活性
    struct {
        u16 addr3:4;            // 第三操作数地址(4位，支持16个寄存器)
        u16 addr2:4;            // 第二操作数地址
        u16 addr1:4;            // 第一操作数地址  
        u16 opcode:4;           // 操作码(4位，支持16种基础操作)
    } three_addr;
    
    // 二地址指令格式 - 平衡表达能力和编码效率
    struct {
        u16 addr2:4;            // 第二操作数地址
        u16 addr1:4;            // 第一操作数地址
        u16 ext_op:4;           // 扩展操作码(在4位基础操作码基础上扩展)
        u16 opcode:4;           // 基本操作码
    } two_addr;
    
    // 一地址指令格式 - 累加器架构风格
    struct {
        u16 addr1:8;            // 操作数地址(8位，支持256地址空间)
        u16 ext_op:4;           // 扩展操作码
        u16 opcode:4;           // 基本操作码
    } one_addr;
    
    // 零地址指令格式 - 立即数或栈操作
    struct {
        u16 immediate:12;       // 立即数(12位，范围0-4095)
        u16 opcode:4;           // 操作码
    } zero_addr;
} instruction_16_t;

/**
 * 32位指令格式（标准RISC架构）
 * 
 * RISC设计原理:
 * - 固定32位长度，简化取指和译码单元
 * - 三种基本格式覆盖所有指令类型
 * - 寄存器字段位置固定，支持并行译码
 * - 立即数字段最大化，减少多指令序列
 * 
 * 字段对齐优化:
 * - 所有寄存器字段5位对齐(支持32个寄存器)
 * - 功能码字段独立，支持指令变种
 * - 移位量字段专用，支持桶形移位器
 */
typedef union {
    u32 raw;                    // 原始32位数据
    
    // R型指令格式（寄存器-寄存器操作）
    struct {
        u32 funct:6;            // 功能码(扩展操作码，支持64种变种)
        u32 shamt:5;            // 移位数(0-31位移位，支持桶形移位器)
        u32 rd:5;               // 目标寄存器(写回目标)
        u32 rt:5;               // 第二源寄存器(右操作数)
        u32 rs:5;               // 第一源寄存器(左操作数)
        u32 opcode:6;           // 操作码(主操作码，支持64种基础操作)
    } r_type;
    
    // I型指令格式（立即数操作）
    struct {
        u32 immediate:16;       // 立即数(16位，范围-32768到32767)
        u32 rt:5;               // 目标/源寄存器(双重用途，节省编码空间)
        u32 rs:5;               // 基址寄存器(地址计算基础)
        u32 opcode:6;           // 操作码
    } i_type;
    
    // J型指令格式（跳转操作）
    struct {
        u32 address:26;         // 跳转地址(26位，结合PC高位构成完整地址)
        u32 opcode:6;           // 操作码
    } j_type;
} instruction_32_t;

/**
 * 通用指令结构
 * 
 * 设计目标:
 * - 统一不同格式的指令表示
 * - 支持动态指令分析和优化
 * - 便于模拟器状态跟踪和调试
 * - 兼容扩展指令集和自定义操作
 */
typedef struct {
    opcode_t opcode;            // 操作码(指令的基本功能)
    addressing_mode_t addr_mode1; // 第一操作数寻址方式
    addressing_mode_t addr_mode2; // 第二操作数寻址方式  
    addressing_mode_t addr_mode3; // 第三操作数寻址方式
    u32 operand1;               // 第一操作数(寄存器号或立即数)
    u32 operand2;               // 第二操作数
    u32 operand3;               // 第三操作数
    address_type_t addr_type;   // 地址码类型(指令格式)
    u32 length;                 // 指令长度（字节数，支持变长指令）
} instruction_t;

/**
 * 程序状态字(PSW) - 处理器状态控制
 * 
 * 标志位设计:
 * - 零标志(Z): 结果为零时置位，支持条件分支
 * - 进位标志(C): 无符号运算溢出，用于多精度运算
 * - 负数标志(N): 结果最高位，有符号数符号位
 * - 溢出标志(V): 有符号运算溢出，补码运算关键
 * - 中断使能(I): 全局中断开关，支持原子操作
 * - 条件码: 扩展比较结果，支持复杂分支条件
 */
typedef struct {
    bool zero_flag;             // 零标志位(Z) - 运算结果为零
    bool carry_flag;            // 进位标志位(C) - 无符号溢出
    bool negative_flag;         // 负数标志位(N) - 有符号数符号
    bool overflow_flag;         // 溢出标志位(V) - 有符号溢出  
    bool interrupt_enable;      // 中断使能位(I) - 全局中断控制
    u8 condition_code;          // 条件码(扩展状态信息)
} psw_t;

/**
 * 寄存器组结构
 * 
 * 寄存器组织:
 * - 通用寄存器: 统一编址，简化指令编码
 * - 专用寄存器: 特定功能，硬件直接访问
 * - 状态寄存器: 处理器状态，支持系统软件
 * 
 * 性能考虑:
 * - 多端口寄存器堆: 支持超标量执行
 * - 寄存器重命名: 消除假相关，提高ILP
 * - 影子寄存器: 快速中断上下文切换
 */
typedef struct {
    u32 general[MAX_REGISTERS]; // 通用寄存器R0-R15(RISC架构标准)
    u32 pc;                     // 程序计数器(指向下一条指令)
    u32 sp;                     // 栈指针(支持过程调用和局部变量)
    u32 ir;                     // 指令寄存器(当前执行指令)
    u32 mar;                    // 内存地址寄存器(内存访问地址)
    u32 mdr;                    // 内存数据寄存器(内存读写数据)
    psw_t psw;                  // 程序状态字(处理器状态标志)
} register_file_t;

/**
 * 内存系统结构
 * 
 * 内存组织:
 * - 统一地址空间: 简化编程模型
 * - 栈区分离: 硬件栈支持，提高过程调用效率
 * - 字节寻址: 支持多种数据类型
 * 
 * 性能优化:
 * - 内存预取: 基于访问模式的预取策略
 * - 写缓冲: 减少写操作对性能的影响
 * - 存储器保护: 支持虚拟内存和进程隔离
 */
typedef struct {
    u8 *data;                   // 内存数据(字节寻址的线性地址空间)
    u32 size;                   // 内存大小(地址空间上限)
    u32 *stack;                 // 栈区域(专用硬件栈，支持快速过程调用)
    u32 stack_top;              // 栈顶指针(当前栈使用深度)
} memory_t;

/**
 * 执行统计信息
 * 
 * 性能指标:
 * - IPC(指令/周期): 衡量处理器效率的关键指标
 * - 指令混合: 不同类型指令的比例分析
 * - 内存访问: 内存子系统性能瓶颈分析
 * - 执行时间: 整体性能评估
 * 
 * 用途:
 * - 性能调优: 识别瓶颈和优化机会
 * - 架构设计: 指导处理器设计决策
 * - 基准测试: 标准化性能比较
 */
typedef struct {
    u64 total_instructions;     // 总指令数(动态指令计数)
    u64 data_transfer_count;    // 数据传送指令数(内存系统负载指标)
    u64 arithmetic_count;       // 算术指令数(ALU利用率指标)
    u64 logical_count;          // 逻辑指令数(简单操作比例)
    u64 control_count;          // 控制指令数(分支密度指标)
    u64 memory_accesses;        // 内存访问次数(包括指令和数据)
    u64 register_accesses;      // 寄存器访问次数(寄存器堆负载)
    u64 cycles;                 // 执行周期数(时钟周期计数)
    double execution_time;      // 执行时间（秒，墙钟时间）
} execution_stats_t;

/**
 * 指令模拟器主结构
 * 
 * 模拟器架构:
 * - 功能级仿真: 指令级别的精确行为模拟
 * - 性能模型: 周期级别的时序分析
 * - 调试支持: 单步执行和状态检查
 * - 扩展性: 支持不同指令集和微架构
 * 
 * 使用场景:
 * - 教学演示: 计算机组成原理课程
 * - 研究工具: 新指令集和优化技术验证
 * - 软件开发: 嵌入式系统代码调试
 */
typedef struct {
    register_file_t registers;  // 寄存器组(处理器状态核心)
    memory_t memory;            // 内存(代码和数据存储)
    instruction_t *program;     // 程序指令序列(静态指令存储)
    u32 program_size;           // 程序大小(指令条数)
    execution_stats_t stats;    // 执行统计(性能分析数据)
    bool running;               // 运行状态(模拟器控制标志)
    bool debug_mode;            // 调试模式(单步执行支持)
    instruction_format_t format; // 指令格式(16位或32位选择)
} instruction_simulator_t;

// ==================== 核心API函数声明 ====================

/**
 * 初始化指令模拟器
 * 
 * @param sim 模拟器实例指针
 * @param memory_size 内存大小(字节)
 * @param format 指令格式(16位或32位)
 * @return 错误码(CPU_SUCCESS表示成功)
 */
cpu_error_t instruction_sim_init(instruction_simulator_t *sim, 
                                u32 memory_size, 
                                instruction_format_t format);

/**
 * 销毁指令模拟器，释放所有资源
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_destroy(instruction_simulator_t *sim);

/**
 * 重置模拟器状态到初始状态
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_reset(instruction_simulator_t *sim);

/**
 * 加载程序到模拟器
 * 
 * @param sim 模拟器实例指针
 * @param program 指令序列
 * @param size 程序大小(指令条数)
 * @return 错误码
 */
cpu_error_t instruction_sim_load_program(instruction_simulator_t *sim,
                                        const instruction_t *program,
                                        u32 size);

/**
 * 单步执行一条指令
 * 
 * @param sim 模拟器实例指针
 * @return 错误码
 */
cpu_error_t instruction_sim_step(instruction_simulator_t *sim);

/**
 * 运行程序直到完成或错误
 * 
 * @param sim 模拟器实例指针  
 * @return 错误码
 */
cpu_error_t instruction_sim_run(instruction_simulator_t *sim);

// ==================== 指令编码译码函数 ====================

/**
 * 指令译码 - 将机器码转换为内部指令表示
 * 
 * @param data 机器码数据
 * @param format 指令格式
 * @param inst 输出的指令结构
 * @return 错误码
 */
cpu_error_t instruction_decode(u32 data, 
                              instruction_format_t format,
                              instruction_t *inst);

/**
 * 指令编码 - 将内部指令表示转换为机器码
 * 
 * @param inst 指令结构
 * @param format 指令格式
 * @param data 输出的机器码
 * @return 错误码
 */
cpu_error_t instruction_encode(const instruction_t *inst,
                              instruction_format_t format,
                              u32 *data);

// ==================== 寻址方式处理函数 ====================

/**
 * 计算有效地址 - 根据寻址方式计算最终内存地址
 * 
 * @param sim 模拟器实例指针
 * @param mode 寻址方式
 * @param operand 操作数
 * @param effective_addr 输出的有效地址
 * @return 错误码
 */
cpu_error_t calculate_effective_address(instruction_simulator_t *sim,
                                       addressing_mode_t mode,
                                       u32 operand,
                                       u32 *effective_addr);

/**
 * 获取操作数值 - 根据寻址方式获取操作数的实际值
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
                             u32 *value);

/**
 * 设置操作数值 - 根据寻址方式设置操作数的值
 * 
 * @param sim 模拟器实例指针
 * @param mode 寻址方式
 * @param operand 操作数  
 * @param value 要设置的值
 * @return 错误码
 */
cpu_error_t set_operand_value(instruction_simulator_t *sim,
                             addressing_mode_t mode,
                             u32 operand,
                             u32 value);

// ==================== 指令执行函数 ====================

/**
 * 执行数据传送指令
 * 
 * @param sim 模拟器实例指针
 * @param inst 指令结构
 * @return 错误码
 */
cpu_error_t execute_data_transfer(instruction_simulator_t *sim,
                                 const instruction_t *inst);

/**
 * 执行算术运算指令
 * 
 * @param sim 模拟器实例指针
 * @param inst 指令结构
 * @return 错误码
 */
cpu_error_t execute_arithmetic(instruction_simulator_t *sim,
                              const instruction_t *inst);

/**
 * 执行逻辑运算指令
 * 
 * @param sim 模拟器实例指针
 * @param inst 指令结构
 * @return 错误码
 */
cpu_error_t execute_logical(instruction_simulator_t *sim,
                           const instruction_t *inst);

/**
 * 执行程序控制指令
 * 
 * @param sim 模拟器实例指针
 * @param inst 指令结构
 * @return 错误码
 */
cpu_error_t execute_program_control(instruction_simulator_t *sim,
                                   const instruction_t *inst);

// ==================== 状态管理函数 ====================

/**
 * 更新标志位 - 根据运算结果更新处理器标志位
 * 
 * @param sim 模拟器实例指针
 * @param result 运算结果
 * @param operation 操作类型
 */
void update_flags(instruction_simulator_t *sim, u32 result, opcode_t operation);

/**
 * 检查条件 - 根据标志位检查分支条件
 * 
 * @param sim 模拟器实例指针
 * @param condition 条件码
 * @return 条件是否满足
 */
bool check_condition(instruction_simulator_t *sim, u8 condition);

// ==================== 统计和调试函数 ====================

/**
 * 获取执行统计信息
 * 
 * @param sim 模拟器实例指针
 * @return 执行统计结构
 */
execution_stats_t instruction_sim_get_stats(const instruction_simulator_t *sim);

/**
 * 打印模拟器状态
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_print_state(const instruction_simulator_t *sim);

/**
 * 打印执行统计信息
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_print_stats(const instruction_simulator_t *sim);

/**
 * 打印寄存器状态
 * 
 * @param sim 模拟器实例指针
 */
void instruction_sim_print_registers(const instruction_simulator_t *sim);

/**
 * 打印内存内容
 * 
 * @param sim 模拟器实例指针
 * @param start_addr 起始地址
 * @param length 长度
 */
void instruction_sim_print_memory(const instruction_simulator_t *sim,
                                 u32 start_addr, u32 length);

// ==================== 工具函数 ====================

/**
 * 操作码转字符串
 * 
 * @param opcode 操作码
 * @return 操作码字符串表示
 */
const char* opcode_to_string(opcode_t opcode);

/**
 * 寻址方式转字符串
 * 
 * @param mode 寻址方式
 * @return 寻址方式字符串表示
 */
const char* addressing_mode_to_string(addressing_mode_t mode);

/**
 * 指令类型转字符串
 * 
 * @param category 指令类型
 * @return 指令类型字符串表示
 */
const char* instruction_category_to_string(instruction_category_t category);

#endif // INSTRUCTION_SIM_H 