/**
 * CPU核心模拟器 - 32位RISC处理器完整实现
 * 
 * ========================================================================
 * 系统架构概述
 * ========================================================================
 * 
 * 本模块实现完整的32位RISC处理器核心，基于冯·诺依曼架构设计，
 * 模拟现代CPU的核心功能、性能特征和执行模型。
 * 
 * 核心设计理念：
 * 1. 教学导向：清晰展示CPU内部工作原理和机制
 * 2. 功能完整：覆盖现代处理器的主要功能特性
 * 3. 性能可观测：提供详细的性能监控和分析能力
 * 4. 可调试性：支持完整的调试和状态检查功能
 * 5. 可配置性：灵活的配置选项适应不同需求
 * 
 * CPU系统架构图：
 * ```
 *     +==============================================================+
 *     |                    CPU核心 (cpu_t)                           |
 *     |                                                              |
 *     |  +----------------+  +----------------+  +----------------+ |
 *     |  |   控制单元      |  |   数据通路      |  |   存储单元      | |
 *     |  | (Control Unit) |  |  (Data Path)   |  | (Storage Unit) | |
 *     |  |                |  |                |  |                | |
 *     |  | • 指令译码      |  | • ALU运算      |  | • 寄存器组      | |
 *     |  | • 控制信号      |  | • 数据选择      |  | • 程序计数器    | |
 *     |  | • 状态管理      |  | • 结果转发      |  | • 状态寄存器    | |
 *     |  +----------------+  +----------------+  +----------------+ |
 *     |           |                   |                   |         |
 *     |           +-------------------+-------------------+         |
 *     |                              |                             |
 *     +==============================|=============================+
 *                                    |
 *                              系统总线
 *                                    |
 *     +==============================|=============================+
 *     |                       内存子系统                            |
 *     |                                                              |
 *     |  +----------------+  +----------------+  +----------------+ |
 *     |  |   指令存储器    |  |   数据存储器    |  |   缓存系统      | |
 *     |  |(Instruction Mem)|  |  (Data Memory) |  | (Cache System) | |
 *     |  |                |  |                |  |                | |
 *     |  | • 程序代码      |  | • 变量数据      |  | • I-Cache      | |
 *     |  | • 常量数据      |  | • I/O映射      |  | • D-Cache      | |
 *     |  | • 中断向量      |  | • I/O映射      |  | • 一致性协议    | |
 *     |  +----------------+  +----------------+  +----------------+ |
 *     +==============================================================+
 * ```
 * 
 * 指令执行流水线模型：
 * ```
 * 时钟周期:  1    2    3    4    5    6    7    8
 *          +----+----+----+----+----+----+----+----+
 * 指令1:   | IF | ID | EX | MEM| WB |    |    |    |
 *          +----+----+----+----+----+----+----+----+
 * 指令2:   |    | IF | ID | EX | MEM| WB |    |    |
 *          +----+----+----+----+----+----+----+----+
 * 指令3:   |    |    | IF | ID | EX | MEM| WB |    |
 *          +----+----+----+----+----+----+----+----+
 * 指令4:   |    |    |    | IF | ID | EX | MEM| WB |
 *          +----+----+----+----+----+----+----+----+
 * 
 * 流水线阶段说明：
 * IF  (Instruction Fetch)   - 指令取指：从内存读取指令
 * ID  (Instruction Decode)  - 指令译码：解析指令和操作数
 * EX  (Execute)            - 指令执行：ALU运算和地址计算
 * MEM (Memory Access)      - 内存访问：数据读写操作
 * WB  (Write Back)         - 结果写回：更新寄存器和标志位
 * ```
 * 
 * 数据通路和控制信号：
 * ```
 *    PC     指令存储器      译码器       ALU        数据存储器    寄存器组
 *     |        |           |           |             |            |
 *     +---> [读地址] --> [指令] --> [控制信号] --> [运算] --> [结果] --> [写回]
 *           [+4逻辑]      [操作数]    [选择信号]   [标志位]   [地址]    [数据]
 *                          |                                   |
 *                         [立即数]                          [读数据]
 *                          |                                   |
 *                          +-----------------------------------+
 * ```
 * 
 * 性能特征和优化技术：
 * 1. **流水线技术**：5级经典流水线，提高指令吞吐量
 * 2. **分支预测**：静态预测策略，减少分支惩罚
 * 3. **缓存系统**：指令/数据缓存分离，提升内存访问效率
 * 4. **前递技术**：数据前递减少流水线停顿
 * 5. **异常处理**：精确异常支持，保证程序状态一致性
 * 
 * 教学价值：
 * - 完整展示CPU内部结构和工作原理
 * - 清晰的流水线执行模型演示
 * - 详细的性能监控和分析功能
 * - 可视化的调试和状态观察
 * - 真实的指令集架构实现
 */

#ifndef CPU_SIM_CPU_CORE_H
#define CPU_SIM_CPU_CORE_H

#include "../common/types.h"
#include "registers.h"
#include "memory.h"
#include "alu.h"

/**
 * ========================================================================
 * CPU状态管理和控制
 * ========================================================================
 */

/**
 * CPU运行状态枚举
 * 
 * 状态机设计：
 * CPU采用有限状态机模型，确保状态转换的确定性和可预测性
 * 
 * 状态转换图：
 * ```
 *     [STOPPED] --------启动-------> [RUNNING]
 *         ^                            |
 *         |                            |
 *      强制停止                     正常停机
 *         |                            v
 *     [ERROR] <----异常发生----- [HALTED]
 *         ^                            |
 *         |                            |
 *      错误恢复                     调试介入
 *         |                            v
 *         +--------重置----------> [DEBUGGING]
 * ```
 * 
 * 状态语义和行为：
 * - STOPPED: CPU处于初始状态，可以接受配置和程序加载
 * - RUNNING: CPU正常执行指令，更新程序计数器和性能统计
 * - HALTED: CPU遇到停机指令或满足停机条件，保持最终状态
 * - ERROR: CPU遇到不可恢复错误，需要重置或修复
 * - DEBUGGING: CPU处于调试模式，支持单步执行和状态检查
 * 
 * 状态不变式：
 * 每个状态都有明确的前置条件和后置条件，保证系统的可靠性
 */
typedef enum {
    CPU_STATE_STOPPED = 0,     // CPU停止状态：未运行，可配置
    CPU_STATE_RUNNING = 1,     // CPU运行状态：正常执行指令
    CPU_STATE_HALTED = 2,      // CPU停机状态：程序结束或停机指令
    CPU_STATE_ERROR = 3,       // CPU错误状态：异常或错误需要处理
    CPU_STATE_DEBUGGING = 4    // CPU调试状态：单步模式，可观察状态
} cpu_state_t;

/**
 * CPU停机原因分类
 * 
 * 停机原因决定了后续的处理策略和恢复方法：
 * 
 * 原因分类体系：
 * ```
 * 停机原因
 * ├── 正常停机 (Normal Halt)
 * │   ├── 程序正常结束
 * │   └── 执行停机指令
 * ├── 异常停机 (Exception Halt)
 * │   ├── 内存访问违例
 * │   ├── 非法指令
 * │   └── 除零异常
 * └── 外部停机 (External Halt)
 *     ├── 用户中断请求
 *     ├── 调试器介入
 *     └── 系统超时
 * ```
 * 
 * 处理策略：
 * - 正常停机：保存最终状态，允许重新启动
 * - 异常停机：记录异常信息，可能需要修复
 * - 外部停机：保持当前状态，支持继续执行
 */
typedef enum {
    CPU_HALT_NONE = 0,           // 无停机：CPU正常运行
    CPU_HALT_INSTRUCTION = 1,    // 指令停机：执行HLT等停机指令
    CPU_HALT_ERROR = 2,          // 错误停机：遇到不可恢复错误
    CPU_HALT_USER_REQUEST = 3,   // 用户停机：外部请求停止
    CPU_HALT_TIMEOUT = 4         // 超时停机：运行时间超过限制
} cpu_halt_reason_t;

/**
 * ========================================================================
 * 性能监控和分析系统
 * ========================================================================
 */

/**
 * CPU性能统计和分析
 * 
 * 性能模型：
 * 采用多层次性能监控体系，从基础计数器到复合性能指标
 * 
 * 指标层次结构：
 * ```
 * 性能监控体系
 * ├── 基础计数器 (Basic Counters)
 * │   ├── 时钟周期计数
 * │   ├── 指令执行计数
 * │   └── 内存访问计数
 * ├── 缓存性能 (Cache Performance)
 * │   ├── 缓存命中统计
 * │   ├── 缓存失效分析
 * │   └── 平均访问时间
 * ├── 分支性能 (Branch Performance)
 * │   ├── 分支指令统计
 * │   ├── 预测准确率
 * │   └── 分支惩罚周期
 * └── 综合指标 (Derived Metrics)
 *     ├── IPC (Instructions Per Cycle)
 *     ├── CPI (Cycles Per Instruction)
 *     └── 有效执行效率
 * ```
 * 
 * 关键性能公式：
 * 
 * 1. 基础性能指标：
 *    IPC = 总指令数 / 总周期数
 *    CPI = 总周期数 / 总指令数 = 1 / IPC
 * 
 * 2. 缓存性能指标：
 *    命中率 = 命中次数 / (命中次数 + 失效次数)
 *    平均访问时间 = 命中时间 + 失效率 × 失效惩罚
 * 
 * 3. 分支性能指标：
 *    分支频率 = 分支指令数 / 总指令数
 *    预测准确率 = 预测正确次数 / 分支指令总数
 * 
 * 4. 系统性能指标：
 *    有效频率 = 实际执行指令数 / 理论最大指令数
 *    资源利用率 = 有效工作周期 / 总周期数
 */
typedef struct {
    // 基础执行统计
    u64 total_cycles;               // 总时钟周期数：CPU运行的时间基准
    u64 total_instructions;         // 总指令数：衡量程序复杂度
    u64 memory_accesses;            // 内存访问次数：存储系统负载
    u64 register_accesses;          // 寄存器访问次数：数据通路活动
    
    // 缓存系统性能
    u64 cache_hits;                 // 缓存命中次数：高速访问统计
    u64 cache_misses;               // 缓存失效次数：慢速访问统计
    double cache_hit_rate;          // 缓存命中率：存储层次效率
    
    // 分支预测性能
    u64 branch_instructions;        // 分支指令数：控制流复杂度
    u64 branch_taken;               // 分支跳转次数：实际控制转移
    double branch_prediction_rate;  // 分支预测成功率：预测器效率
    
    // 综合性能指标
    double instructions_per_cycle;  // IPC：指令级并行度体现
    double cycles_per_instruction;  // CPI：平均指令复杂度
    double effective_frequency;     // 有效频率：实际计算能力
    double resource_utilization;    // 资源利用率：硬件效率
} cpu_performance_t;

/**
 * ========================================================================
 * CPU配置和控制参数
 * ========================================================================
 */

/**
 * CPU配置参数系统
 * 
 * 配置设计哲学：
 * 提供分层的配置体系，支持从基础功能到高级性能优化的全面控制
 * 
 * 配置分类：
 * ```
 * CPU配置体系
 * ├── 功能配置 (Feature Configuration)
 * │   ├── 缓存系统开关
 * │   ├── 流水线模式
 * │   ├── 分支预测器
 * │   └── 中断处理
 * ├── 性能配置 (Performance Configuration)
 * │   ├── 时钟频率设置
 * │   ├── 缓存参数
 * │   ├── 流水线级数
 * │   └── 预测器类型
 * ├── 调试配置 (Debug Configuration)
 * │   ├── 调试输出控制
 * │   ├── 性能监控级别
 * │   ├── 状态跟踪
 * │   └── 异常处理模式
 * └── 兼容性配置 (Compatibility Configuration)
 *     ├── 内存对齐要求
 *     ├── 指令集版本
 *     └── 异常模型
 * ```
 * 
 * 配置影响分析：
 * 不同配置选项对系统性能和功能的影响：
 * 
 * 1. 缓存配置影响：
 *    - 开启缓存：提高内存访问速度，增加系统复杂度
 *    - 缓存大小：影响命中率和访问延迟
 * 
 * 2. 流水线配置影响：
 *    - 流水线深度：影响吞吐量和分支惩罚
 *    - 前递机制：减少数据冲突延迟
 * 
 * 3. 调试配置影响：
 *    - 调试模式：详细状态跟踪，降低执行速度
 *    - 性能监控：实时统计收集，轻微性能开销
 */
typedef struct {
    // 核心功能开关
    bool enable_cache;              // 缓存系统：启用内存层次结构
    bool enable_pipeline;           // 流水线：启用指令并行执行
    bool enable_branch_prediction;  // 分支预测：减少控制冲突
    bool enable_interrupts;         // 中断处理：支持异步事件
    bool enable_performance_monitor; // 性能监控：详细统计收集
    
    // 系统配置参数
    bool strict_alignment;          // 严格对齐：内存访问对齐检查
    bool debug_output;              // 调试输出：详细执行信息
    bool trace_execution;           // 执行跟踪：指令级执行记录
    
    // 性能参数设置
    u32 clock_frequency;            // 时钟频率：系统基准频率(Hz)
    u32 cache_size;                 // 缓存大小：高速缓存容量(字节)
    u8 pipeline_stages;             // 流水线级数：并行执行深度
    u8 branch_predictor_type;       // 分支预测器类型：预测算法选择
} cpu_config_t;

/**
 * ========================================================================
 * 指令表示和执行模型
 * ========================================================================
 */

/**
 * 指令结构设计
 * 
 * 指令格式哲学：
 * 采用32位定长指令格式，平衡编码密度和解码复杂度
 * 
 * 指令编码格式：
 * ```
 * 32位指令字结构 (RISC格式):
 * 
 * R型指令 (寄存器-寄存器操作):
 * +--------+--------+--------+--------+--------+--------+
 * | 31-26  | 25-21  | 20-16  | 15-11  | 10-6   | 5-0    |
 * |  op    |  rs    |  rt    |  rd    | shamt  | funct  |
 * +--------+--------+--------+--------+--------+--------+
 * 
 * I型指令 (立即数操作):
 * +--------+--------+--------+-----------------------+
 * | 31-26  | 25-21  | 20-16  |        15-0           |
 * |  op    |  rs    |  rt    |      immediate        |
 * +--------+--------+--------+-----------------------+
 * 
 * J型指令 (跳转操作):
 * +--------+---------------------------------------+
 * | 31-26  |              25-0                   |
 * |  op    |            address                  |
 * +--------+---------------------------------------+
 * ```
 * 
 * 寻址方式体系：
 * ```
 * 寻址方式分类
 * ├── 立即寻址 (Immediate)
 * │   └── 操作数直接嵌入指令中
 * ├── 寄存器寻址 (Register)
 * │   └── 操作数在指定寄存器中
 * ├── 直接寻址 (Direct)
 * │   └── 操作数在指定内存地址
 * ├── 间接寻址 (Indirect)
 * │   └── 地址存储在寄存器中
 * ├── 相对寻址 (Relative)
 * │   └── 相对PC的偏移地址
 * └── 变址寻址 (Indexed)
 *     └── 基址+偏移量寻址
 * ```
 * 
 * 指令执行模型：
 * 每条指令经过完整的五阶段流水线处理：
 * 1. 取指阶段：从内存获取指令字
 * 2. 译码阶段：解析指令格式和操作数
 * 3. 执行阶段：ALU运算和地址计算
 * 4. 访存阶段：内存数据读写操作
 * 5. 写回阶段：结果存储和状态更新
 */
typedef struct {
    // 指令基本信息
    opcode_t opcode;                // 操作码：确定执行的操作类型
    u32 raw_instruction;            // 原始指令字：32位完整指令编码
    
    // 操作数和寻址
    addressing_mode_t addr_mode1;   // 第一操作数寻址方式
    addressing_mode_t addr_mode2;   // 第二操作数寻址方式
    word_t operand1;                // 第一操作数：源操作数或目标
    word_t operand2;                // 第二操作数：源操作数或立即数
    word_t immediate;               // 立即数：指令中的常数值
    
    // 寄存器字段
    u8 reg1;                        // 第一寄存器编号：通常是目标寄存器
    u8 reg2;                        // 第二寄存器编号：通常是源寄存器
    u8 reg3;                        // 第三寄存器编号：用于三操作数指令
    
    // 执行控制信息
    u32 cycles;                     // 指令执行周期数：性能分析基础
    bool is_branch;                 // 分支指令标志：流水线控制
    bool is_memory_op;              // 内存操作标志：缓存和内存控制
    bool causes_exception;          // 异常标志：异常处理准备
} instruction_t;

/**
 * ========================================================================
 * CPU核心数据结构
 * ========================================================================
 */

/**
 * CPU核心结构体 - 完整处理器状态
 * 
 * 结构设计原则：
 * 1. 模块化封装：每个子系统相对独立，便于测试和维护
 * 2. 状态可观测：丰富的状态信息，支持调试和性能分析
 * 3. 配置灵活性：支持不同的配置和优化选项
 * 4. 接口标准化：统一的接口设计，便于扩展和集成
 * 
 * 系统组件关系：
 * ```
 * CPU核心结构关系图
 *                    +----------------+
 *                    |   cpu_t 核心   |
 *                    +----------------+
 *                            |
 *           +----------------+----------------+
 *           |                |                |
 *    +-------------+  +-------------+  +-------------+
 *    | 执行引擎     |  | 存储系统     |  | 控制系统     |
 *    +-------------+  +-------------+  +-------------+
 *    | • registers |  | • memory    |  | • state     |
 *    | • alu       |  | • cache     |  | • config    |
 *    | • pipeline  |  | • mmu       |  | • debug     |
 *    +-------------+  +-------------+  +-------------+
 *           |                |                |
 *           +----------------+----------------+
 *                            |
 *                   +----------------+
 *                   | 性能监控系统    |
 *                   | • perf        |
 *                   | • counters    |
 *                   | • statistics  |
 *                   +----------------+
 * ```
 * 
 * 状态管理模型：
 * CPU状态由多个层次组成，每个层次负责特定的功能：
 * 
 * 1. 架构状态 (Architectural State)：
 *    - 程序可见的寄存器和内存
 *    - 程序计数器和状态标志
 *    - 异常和中断状态
 * 
 * 2. 微架构状态 (Microarchitectural State)：
 *    - 流水线寄存器和缓冲区
 *    - 缓存标签和状态位
 *    - 分支预测表和历史信息
 * 
 * 3. 实现状态 (Implementation State)：
 *    - 性能计数器和统计信息
 *    - 调试和跟踪数据
 *    - 配置和控制参数
 * 
 * 性能监控集成：
 * 性能监控系统深度集成到CPU执行流程中：
 * - 每条指令执行时更新基础计数器
 * - 内存访问时更新缓存统计
 * - 分支指令时更新预测统计
 * - 周期性计算综合性能指标
 */
typedef struct {
    // ========== 核心执行状态 ==========
    cpu_state_t state;              // CPU运行状态：控制执行流程
    cpu_halt_reason_t halt_reason;  // 停机原因：故障诊断和恢复
    
    // ========== 硬件组件 ==========
    registers_t registers;          // 寄存器组：程序状态和数据存储
    memory_t memory;                // 内存系统：程序和数据存储空间
    alu_t alu;                      // 算术逻辑单元：数值计算核心
    
    // ========== 执行控制 ==========
    instruction_t current_instruction; // 当前执行指令：流水线状态
    addr_t next_pc;                 // 下一指令地址：分支和异常处理
    bool interrupt_pending;         // 中断待处理标志：异步事件处理
    u8 interrupt_vector;            // 中断向量号：中断源识别
    
    // ========== 性能监控 ==========
    u64 cycle_count;                // 周期计数器：基础时间参考
    u64 instruction_count;          // 指令计数器：程序进度跟踪
    cpu_performance_t perf;         // 详细性能统计：分析和优化
    
    // ========== 调试支持 ==========
    bool debug_mode;                // 调试模式：单步执行和跟踪
    addr_t breakpoints[16];         // 断点数组：调试停止点
    u8 breakpoint_count;            // 断点数量：调试状态管理
    bool trace_enabled;             // 执行跟踪：详细执行记录
    
    // ========== 系统配置 ==========
    cpu_config_t config;            // 配置参数：功能和性能控制
    
    // ========== 扩展状态 ==========
    void *extension_data;           // 扩展数据：模块化扩展支持
    size_t extension_size;          // 扩展大小：内存管理
} cpu_t;

/**
 * CPU核心控制模块
 * 
 * 本模块是CPU模拟器的核心，负责：
 * 1. CPU的初始化和生命周期管理
 * 2. 指令的取指、译码、执行、写回流程
 * 3. CPU状态控制和异常处理
 * 4. 调试和性能监控
 */

// ==================== CPU生命周期管理 ====================

/**
 * 初始化CPU到可运行状态
 * 
 * 初始化流程：
 * 1. 内存系统初始化：分配指定大小的内存空间
 * 2. 寄存器组初始化：设置寄存器到默认状态
 * 3. ALU单元初始化：准备算术逻辑运算能力
 * 4. 性能计数器清零：重置所有统计信息
 * 5. 默认配置加载：设置合理的默认配置
 * 6. 自检验证：确保各个组件正常工作
 * 
 * 内存布局：
 * - 代码段：存储程序指令
 * - 数据段：存储全局和静态数据
 * - 堆空间：动态内存分配
 * - 栈空间：函数调用和局部变量
 * 
 * 错误处理：
 * 初始化失败时，清理已分配的资源，返回错误码
 * 
 * @param cpu CPU结构指针，不能为NULL
 * @param memory_size 内存大小（字节），必须为4的倍数
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_init(cpu_t *cpu, size_t memory_size);

/**
 * 重置CPU到初始状态
 * 
 * 重置策略：
 * 软重置，保留内存内容和配置，重置执行状态
 * 
 * 重置范围：
 * 1. 寄存器组：重置为初始值
 * 2. 程序计数器：指向程序入口点
 * 3. 栈指针：恢复到栈基地址
 * 4. 状态标志：清除所有异常状态
 * 5. 性能计数器：可选择性重置
 * 
 * 保留内容：
 * - 内存内容：程序和数据保持不变
 * - 配置设置：用户配置保持有效
 * - 断点设置：调试断点保持有效
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_reset(cpu_t *cpu);

/**
 * 清理CPU，释放所有资源
 * 
 * 清理流程：
 * 1. 停止CPU运行：确保CPU处于安全状态
 * 2. 保存重要状态：可选的状态保存
 * 3. 释放内存资源：归还系统内存
 * 4. 清理调试资源：移除断点和调试信息
 * 5. 重置结构状态：清零结构体字段
 * 
 * 资源管理：
 * 遵循RAII原则，确保所有分配的资源都被正确释放
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_cleanup(cpu_t *cpu);

/**
 * 设置CPU运行状态
 * 
 * 状态转换控制：
 * 验证状态转换的合法性，执行相应的转换操作
 * 
 * 转换验证：
 * - STOPPED -> RUNNING: 检查初始化完成
 * - RUNNING -> HALTED: 保存当前状态
 * - ANY -> ERROR: 记录错误信息
 * - ANY -> DEBUGGING: 激活调试模式
 * 
 * 副作用：
 * 状态转换可能触发相关的系统操作
 * 
 * @param cpu CPU结构指针
 * @param state 新的CPU状态
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_set_state(cpu_t *cpu, cpu_state_t state);

/**
 * 获取CPU当前运行状态
 * 
 * @param cpu CPU结构指针
 * @return CPU当前状态，错误时返回CPU_STATE_ERROR
 */
cpu_state_t cpu_get_state(const cpu_t *cpu);

/**
 * 检查CPU是否处于运行状态
 * 
 * 运行状态判定：
 * CPU状态为RUNNING，且没有错误标志
 * 
 * @param cpu CPU结构指针
 * @return 运行状态返回true，否则返回false
 */
bool cpu_is_running(const cpu_t *cpu);

/**
 * 检查CPU是否已停机
 * 
 * 停机状态判定：
 * CPU状态为HALTED、ERROR或STOPPED
 * 
 * @param cpu CPU结构指针
 * @return 停机状态返回true，否则返回false
 */
bool cpu_is_halted(const cpu_t *cpu);

// ==================== 指令执行控制 ====================

/**
 * 执行单条指令（完整的指令周期）
 * 
 * 指令周期阶段：
 * 1. 取指（Fetch）：从PC指向的内存地址读取指令
 * 2. 译码（Decode）：解析指令格式，确定操作和操作数
 * 3. 执行（Execute）：在ALU中执行指定运算
 * 4. 访存（Memory）：进行必要的内存读写操作
 * 5. 写回（Writeback）：将结果写回目标寄存器
 * 
 * 时序控制：
 * 每个阶段消耗指定的时钟周期，更新性能计数器
 * 
 * 异常处理：
 * 在任何阶段发生异常时，保存现场并转入异常处理
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_execute_instruction(cpu_t *cpu);

/**
 * 执行指定数量的指令
 * 
 * 批量执行：
 * 连续执行多条指令，减少函数调用开销
 * 
 * 中断检查：
 * 定期检查中断和异常条件
 * 
 * 性能优化：
 * 批量更新性能计数器，提高执行效率
 * 
 * @param cpu CPU结构指针
 * @param count 要执行的指令数量
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_execute_instructions(cpu_t *cpu, u64 count);

/**
 * 执行指定数量的时钟周期
 * 
 * 周期精确模拟：
 * 按照真实CPU的时钟周期执行，更精确的时序模拟
 * 
 * 流水线模拟：
 * 支持流水线执行模型，一个周期可能同时处理多条指令的不同阶段
 * 
 * @param cpu CPU结构指针
 * @param cycles 要执行的时钟周期数
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_execute_cycles(cpu_t *cpu, u64 cycles);

/**
 * 连续运行CPU直到停机或错误
 * 
 * 运行循环：
 * 持续执行指令直到遇到停机条件
 * 
 * 停机条件：
 * - 执行停机指令（HALT）
 * - 发生不可恢复错误
 * - 遇到调试断点
 * - 外部停止请求
 * 
 * 性能监控：
 * 实时更新性能统计，支持性能分析
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_run(cpu_t *cpu);

/**
 * 运行CPU指定时间（毫秒）
 * 
 * 超时控制：
 * 防止无限循环，提供可控的执行时间
 * 
 * 时间计算：
 * 基于时钟频率和周期数计算执行时间
 * 
 * @param cpu CPU结构指针
 * @param timeout_ms 超时时间（毫秒）
 * @return 成功返回CPU_SUCCESS，超时返回CPU_HALT_TIMEOUT
 */
cpu_error_t cpu_run_with_timeout(cpu_t *cpu, u32 timeout_ms);

/**
 * 停止CPU运行
 * 
 * 安全停止：
 * 等待当前指令执行完成后停止，保证状态一致性
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_halt(cpu_t *cpu);

/**
 * 单步执行（调试模式）
 * 
 * 调试支持：
 * 执行一条指令后自动停止，便于调试分析
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_step(cpu_t *cpu);

// ==================== 指令流水线阶段 ====================

/**
 * 指令取指阶段
 * 
 * 取指操作：
 * 1. 读取PC指向的内存地址
 * 2. 获取32位指令字
 * 3. 更新PC指向下一条指令
 * 4. 处理指令缓存
 * 
 * @param cpu CPU结构指针
 * @param instruction 输出的指令结构
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_fetch(cpu_t *cpu, instruction_t *instruction);

/**
 * 指令译码阶段
 * 
 * 译码操作：
 * 1. 解析指令格式
 * 2. 提取操作码和操作数
 * 3. 确定寻址方式
 * 4. 计算有效地址
 * 
 * @param cpu CPU结构指针
 * @param instruction 要译码的指令结构
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_decode(cpu_t *cpu, instruction_t *instruction);

/**
 * 指令执行阶段
 * 
 * 执行操作：
 * 1. 读取操作数
 * 2. 在ALU中执行运算
 * 3. 计算结果和标志位
 * 4. 处理分支跳转
 * 
 * @param cpu CPU结构指针
 * @param instruction 要执行的指令结构
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_execute(cpu_t *cpu, const instruction_t *instruction);

/**
 * 结果写回阶段
 * 
 * 写回操作：
 * 1. 将计算结果写入目标寄存器
 * 2. 更新标志位寄存器
 * 3. 处理内存写入
 * 4. 提交指令执行结果
 * 
 * @param cpu CPU结构指针
 * @param instruction 指令结构
 * @param result 执行结果
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_writeback(cpu_t *cpu, const instruction_t *instruction, word_t result);

// ==================== 程序加载和管理 ====================

/**
 * 从文件加载程序到内存
 * 
 * @param cpu CPU结构指针
 * @param filename 程序文件路径
 * @param load_addr 加载地址
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_load_program(cpu_t *cpu, const char *filename, addr_t load_addr);

/**
 * 从字节数组加载程序到内存
 * 
 * @param cpu CPU结构指针
 * @param program 程序字节数组
 * @param size 程序大小
 * @param load_addr 加载地址
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_load_program_from_bytes(cpu_t *cpu, const byte_t *program, size_t size, addr_t load_addr);

/**
 * 设置程序入口点
 * 
 * @param cpu CPU结构指针
 * @param entry_point 程序入口地址
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_set_entry_point(cpu_t *cpu, addr_t entry_point);

/**
 * 初始化程序栈
 * 
 * @param cpu CPU结构指针
 * @param stack_base 栈基地址
 * @param stack_size 栈大小
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_init_stack(cpu_t *cpu, addr_t stack_base, size_t stack_size);

// ==================== 中断和异常处理 ====================

/**
 * 触发中断
 * 
 * @param cpu CPU结构指针
 * @param interrupt_vector 中断向量号
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_trigger_interrupt(cpu_t *cpu, u8 interrupt_vector);

/**
 * 处理异常
 * 
 * @param cpu CPU结构指针
 * @param exception_code 异常代码
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_handle_exception(cpu_t *cpu, cpu_error_t exception_code);

/**
 * 设置中断使能状态
 * 
 * @param cpu CPU结构指针
 * @param enabled 中断使能标志
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_set_interrupt_enable(cpu_t *cpu, bool enabled);

/**
 * 检查是否有待处理的中断
 * 
 * @param cpu CPU结构指针
 * @return 有中断返回true，否则返回false
 */
bool cpu_has_pending_interrupt(const cpu_t *cpu);

// ==================== 性能监控和统计 ====================

/**
 * 获取CPU性能统计
 * 
 * @param cpu CPU结构指针
 * @param perf 输出的性能统计结构
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_get_performance(const cpu_t *cpu, cpu_performance_t *perf);

/**
 * 重置性能计数器
 * 
 * @param cpu CPU结构指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_reset_performance_counters(cpu_t *cpu);

/**
 * 获取当前周期计数
 * 
 * @param cpu CPU结构指针
 * @return 当前周期计数
 */
u64 cpu_get_cycle_count(const cpu_t *cpu);

/**
 * 获取当前指令计数
 * 
 * @param cpu CPU结构指针
 * @return 当前指令计数
 */
u64 cpu_get_instruction_count(const cpu_t *cpu);

/**
 * 计算CPU频率
 * 
 * @param cpu CPU结构指针
 * @param elapsed_time_us 经过的时间（微秒）
 * @return 计算得到的频率（Hz）
 */
double cpu_calculate_frequency(const cpu_t *cpu, u64 elapsed_time_us);

// ==================== 调试和诊断 ====================

/**
 * 转储CPU状态
 * 
 * @param cpu CPU结构指针
 * @param detailed 是否显示详细信息
 */
void cpu_dump_state(const cpu_t *cpu, bool detailed);

/**
 * 转储指令信息
 * 
 * @param cpu CPU结构指针
 * @param instruction 指令结构
 */
void cpu_dump_instruction(const cpu_t *cpu, const instruction_t *instruction);

/**
 * 反汇编指令
 * 
 * @param instruction 指令结构
 * @param buffer 输出缓冲区
 * @param buffer_size 缓冲区大小
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_disassemble_instruction(const instruction_t *instruction, char *buffer, size_t buffer_size);

/**
 * 设置断点
 * 
 * @param cpu CPU结构指针
 * @param addr 断点地址
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_set_breakpoint(cpu_t *cpu, addr_t addr);

/**
 * 清除断点
 * 
 * @param cpu CPU结构指针
 * @param addr 断点地址
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_clear_breakpoint(cpu_t *cpu, addr_t addr);

/**
 * 检查断点
 * 
 * @param cpu CPU结构指针
 * @param addr 要检查的地址
 * @return 是断点返回true，否则返回false
 */
bool cpu_check_breakpoint(const cpu_t *cpu, addr_t addr);

/**
 * 设置调试模式
 * 
 * @param cpu CPU结构指针
 * @param debug_mode 调试模式标志
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_set_debug_mode(cpu_t *cpu, bool debug_mode);

// ==================== 配置和管理 ====================

/**
 * 配置CPU参数
 * 
 * @param cpu CPU结构指针
 * @param config 配置结构
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_configure(cpu_t *cpu, const cpu_config_t *config);

/**
 * 获取CPU配置
 * 
 * @param cpu CPU结构指针
 * @param config 输出的配置结构
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_get_configuration(const cpu_t *cpu, cpu_config_t *config);

/**
 * 保存CPU状态到文件
 * 
 * @param cpu CPU结构指针
 * @param filename 状态文件路径
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_save_state(const cpu_t *cpu, const char *filename);

/**
 * 从文件恢复CPU状态
 * 
 * @param cpu CPU结构指针
 * @param filename 状态文件路径
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_restore_state(cpu_t *cpu, const char *filename);

// ==================== 测试和验证 ====================

/**
 * CPU自检测试
 * 
 * @return 测试通过返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_self_test(void);

/**
 * 验证CPU状态一致性
 * 
 * @param cpu CPU结构指针
 * @return 状态一致返回true，否则返回false
 */
bool cpu_validate_state(const cpu_t *cpu);

/**
 * 运行指令测试套件
 * 
 * @param cpu CPU结构指针
 * @return 测试通过返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_run_instruction_tests(cpu_t *cpu);

/**
 * CPU性能基准测试
 * 
 * @param cpu CPU结构指针
 * @param test_duration_ms 测试持续时间（毫秒）
 * @return 测试完成返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t cpu_benchmark(cpu_t *cpu, u32 test_duration_ms);

#endif // CPU_SIM_CPU_CORE_H 