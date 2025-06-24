#ifndef PIPELINE_SIM_H
#define PIPELINE_SIM_H

#include "../common/types.h"

/**
 * 指令流水线模拟器
 * 
 * 设计原理：
 * 流水线是一种提高处理器性能的重要技术，通过将指令执行过程分解为多个独立的阶段，
 * 使得多条指令能够并行处理，从而提高整体吞吐率。本模块模拟了经典的五级流水线架构。
 * 
 * 架构特点：
 * 1. 五级流水线结构 - IF(取指)、ID(译码)、EX(执行)、MEM(访存)、WB(写回)
 * 2. 完整冲突检测 - 结构冲突、数据冲突(RAW/WAR/WAW)、控制冲突
 * 3. 冲突解决机制 - 数据转发、流水线停顿、分支预测、流水线冲刷
 * 4. 性能监控系统 - CPI计算、吞吐率分析、效率评估、冲突统计
 * 5. 高级优化技术 - 动态分支预测、多级数据转发、Load-Use冲突检测
 * 
 * 流水线执行模型：
 * 
 *     时钟周期:  1    2    3    4    5    6    7    8
 *     指令1:    IF   ID   EX   MEM  WB
 *     指令2:         IF   ID   EX   MEM  WB
 *     指令3:              IF   ID   EX   MEM  WB
 *     指令4:                   IF   ID   EX   MEM
 *     指令5:                        IF   ID   EX
 * 
 * 核心价值：
 * - 提供完整的流水线行为模拟，包括正常执行和异常情况
 * - 支持多种冲突检测和解决策略的对比分析
 * - 实时计算和显示流水线性能指标
 * - 为处理器设计和优化提供理论验证平台
 */

// ==================== 流水线常量定义 ====================

#define PIPELINE_STAGES         5       // 流水线级数
#define MAX_INSTRUCTIONS        1000    // 最大指令数
#define PIPELINE_MAX_REGISTERS 32      // 流水线寄存器数量
#define INSTRUCTION_MEMORY_SIZE 4096    // 指令存储器大小
#define DATA_MEMORY_SIZE       4096     // 数据存储器大小

// ==================== 流水线阶段定义 ====================

/**
 * 流水线阶段枚举
 * 
 * 流水线分级原理：
 * 将指令执行过程分解为5个独立的功能阶段，每个阶段专门负责特定的操作，
 * 各阶段可以并行工作，从而实现指令级并行处理。
 * 
 * 阶段功能说明：
 * IF - 指令从存储器中读取到处理器
 * ID - 指令被解码并读取寄存器操作数
 * EX - 执行算术/逻辑运算或地址计算
 * MEM - 进行存储器访问（读取或写入数据）
 * WB - 将运算结果写回到寄存器文件
 */
typedef enum {
    STAGE_IF = 0,    // 取指阶段 - Instruction Fetch
    STAGE_ID = 1,    // 译码阶段 - Instruction Decode
    STAGE_EX = 2,    // 执行阶段 - Execute
    STAGE_MEM = 3,   // 访存阶段 - Memory Access
    STAGE_WB = 4     // 写回阶段 - Write Back
} pipeline_stage_t;

// 指令类型
typedef enum {
    INST_R_TYPE = 0,     // R型指令（算术逻辑）
    INST_I_TYPE = 1,     // I型指令（立即数）
    INST_LOAD = 2,       // Load指令
    INST_STORE = 3,      // Store指令
    INST_BRANCH = 4,     // 分支指令
    INST_JUMP = 5,       // 跳转指令
    INST_NOP = 6         // 空操作
} instruction_type_t;

/**
 * 流水线冲突类型枚举
 * 
 * 冲突产生原理：
 * 流水线中的冲突是由于指令间的相互依赖关系导致的，这些依赖关系阻止了
 * 指令按照理想的一个时钟周期一条的速度执行。
 * 
 * 冲突分类：
 * 1. 结构冲突 - 硬件资源争用冲突
 * 2. 数据冲突 - 指令间数据依赖冲突
 * 3. 控制冲突 - 分支跳转导致的控制流冲突
 * 
 * 数据冲突详细分析：
 * RAW (Read After Write) - 真数据依赖，后续指令需要前面指令的结果
 * WAR (Write After Read) - 反依赖，只在乱序执行中出现
 * WAW (Write After Write) - 输出依赖，只在乱序执行中出现
 * 
 * 冲突示例：
 * RAW: ADD R1, R2, R3    # R1 = R2 + R3
 *      SUB R4, R1, R5    # R4 = R1 - R5 (需要等待R1写入完成)
 */
typedef enum {
    HAZARD_NONE = 0,        // 无冲突状态
    HAZARD_STRUCTURAL = 1,  // 结构冲突 - 硬件资源竞争
    HAZARD_DATA_RAW = 2,    // 读后写数据冲突 - Read After Write
    HAZARD_DATA_WAR = 3,    // 写后读数据冲突 - Write After Read
    HAZARD_DATA_WAW = 4,    // 写后写数据冲突 - Write After Write
    HAZARD_CONTROL = 5      // 控制冲突 - 分支跳转冲突
} hazard_type_t;

/**
 * 分支预测状态枚举
 * 
 * 分支预测原理：
 * 使用两位饱和计数器实现动态分支预测，根据分支指令的历史行为预测其未来行为。
 * 这种预测机制能够显著减少分支指令造成的控制冲突。
 * 
 * 状态转换图：
 * 
 *    强不跳转(00) <-----> 弱不跳转(01)
 *        |                    |
 *        |                    |
 *        v                    v
 *    弱跳转(10)   <-----> 强跳转(11)
 * 
 * 状态转换规则：
 * - 分支发生：状态向"跳转"方向移动一级
 * - 分支不发生：状态向"不跳转"方向移动一级
 * 
 * 预测策略：
 * - 强/弱跳转状态：预测分支发生
 * - 强/弱不跳转状态：预测分支不发生
 */
typedef enum {
    BRANCH_STRONG_NOT_TAKEN = 0,  // 强不跳转状态 (二进制: 00)
    BRANCH_WEAK_NOT_TAKEN = 1,    // 弱不跳转状态 (二进制: 01)  
    BRANCH_WEAK_TAKEN = 2,        // 弱跳转状态 (二进制: 10)
    BRANCH_STRONG_TAKEN = 3       // 强跳转状态 (二进制: 11)
} branch_prediction_state_t;

// ==================== 指令和流水线寄存器 ====================

/**
 * 指令结构体
 * 
 * 设计原理：
 * 封装完整的指令信息，支持MIPS指令集的三种基本格式（R型、I型、J型）。
 * 通过统一的数据结构简化指令解码和执行流程。
 * 
 * 指令格式解析：
 * R型: [31:26]op [25:21]rs [20:16]rt [15:11]rd [10:6]shamt [5:0]funct
 * I型: [31:26]op [25:21]rs [20:16]rt [15:0]immediate
 * J型: [31:26]op [25:0]address
 * 
 * 字段用途：
 * - pc: 指令在内存中的地址，用于调试和分支计算
 * - instruction: 原始32位指令字，保留完整信息
 * - type: 指令类型分类，用于快速分发处理
 * - opcode: 操作码，确定指令的基本类别
 * - rs/rt/rd: 寄存器字段，分别表示源寄存器和目标寄存器
 * - immediate: 立即数，用于I型指令的常数操作数
 * - address: 跳转地址，用于J型指令
 */
typedef struct {
    u32 pc;                     // 程序计数器 - 指令地址
    u32 instruction;            // 原始指令字 - 32位机器码
    instruction_type_t type;    // 指令类型 - R/I/J型分类
    u8 opcode;                  // 操作码 - 指令[31:26]
    u8 rs;                      // 源寄存器1 - 指令[25:21]
    u8 rt;                      // 源寄存器2 - 指令[20:16]
    u8 rd;                      // 目标寄存器 - 指令[15:11]
    u8 shamt;                   // 移位量 - 指令[10:6]
    u8 funct;                   // 功能码 - 指令[5:0]
    i16 immediate;              // 立即数 - 指令[15:0]符号扩展
    u32 address;                // 跳转地址 - 指令[25:0]
    bool valid;                 // 指令有效性标志
} instruction_t;

/**
 * IF/ID流水线寄存器
 * 
 * 功能定义：
 * 连接取指(IF)和译码(ID)阶段的流水线寄存器，保存从指令存储器读取的指令
 * 以及相关的控制信息，为下一阶段的指令解码做准备。
 * 
 * 设计原理：
 * 流水线寄存器用于在时钟边沿锁存数据，确保各阶段的数据同步。
 * IF/ID寄存器是流水线的第一个寄存器，承担指令传递的关键作用。
 * 
 * 数据流向：
 * IF阶段 -> IF/ID寄存器 -> ID阶段
 * 
 * 字段说明：
 * - pc: 当前指令的程序计数器值
 * - pc_plus_4: 下一条指令的地址（用于顺序执行）
 * - instruction: 完整的指令信息（已解码）
 * - valid: 指示寄存器中的数据是否有效
 * - stall: 指示是否需要暂停流水线推进
 */
typedef struct {
    u32 pc;                     // 当前指令程序计数器
    u32 pc_plus_4;              // 下一指令地址 (PC + 4)
    instruction_t instruction;   // 解码后的指令信息
    bool valid;                 // 数据有效性标志
    bool stall;                 // 流水线停顿标志
} if_id_reg_t;

// ID/EX流水线寄存器  
typedef struct {
    // 控制信号
    bool reg_write;             // 寄存器写使能
    bool mem_to_reg;            // 存储器到寄存器
    bool branch;                // 分支控制
    bool mem_read;              // 存储器读
    bool mem_write;             // 存储器写
    bool reg_dst;               // 寄存器目标选择
    bool alu_src;               // ALU源选择
    u8 alu_op;                  // ALU操作
    
    // 数据
    u32 pc_plus_4;              // PC + 4
    u32 read_data_1;            // 寄存器读数据1
    u32 read_data_2;            // 寄存器读数据2
    i32 sign_extend;            // 符号扩展立即数
    u8 instruction_20_16;       // 指令[20:16]
    u8 instruction_15_11;       // 指令[15:11]
    u8 rs;                      // 源寄存器1
    u8 rt;                      // 源寄存器2
    instruction_t instruction;   // 原始指令
    bool valid;                 // 有效位
} id_ex_reg_t;

// EX/MEM流水线寄存器
typedef struct {
    // 控制信号
    bool reg_write;             // 寄存器写使能
    bool mem_to_reg;            // 存储器到寄存器
    bool branch;                // 分支控制
    bool mem_read;              // 存储器读
    bool mem_write;             // 存储器写
    
    // 数据
    u32 branch_target;          // 分支目标地址
    bool zero;                  // ALU零标志
    u32 alu_result;             // ALU结果
    u32 write_data;             // 写数据
    u8 write_register;          // 写寄存器
    instruction_t instruction;   // 原始指令
    bool valid;                 // 有效位
} ex_mem_reg_t;

// MEM/WB流水线寄存器
typedef struct {
    // 控制信号
    bool reg_write;             // 寄存器写使能
    bool mem_to_reg;            // 存储器到寄存器
    
    // 数据
    u32 read_data;              // 存储器读数据
    u32 alu_result;             // ALU结果
    u8 write_register;          // 写寄存器
    instruction_t instruction;   // 原始指令
    bool valid;                 // 有效位
} mem_wb_reg_t;

// ==================== 冲突检测和处理 ====================

// 数据转发控制
typedef struct {
    u8 forward_a;               // 前推控制A (00:无, 01:MEM前推, 10:EX前推)
    u8 forward_b;               // 前推控制B
    bool load_use_hazard;       // Load-Use冲突
} forwarding_control_t;

// 分支预测器
typedef struct {
    branch_prediction_state_t state[256];  // 预测状态表
    u32 branch_history;                    // 分支历史
    u32 btb[64];                          // 分支目标缓冲器
    u32 total_branches;                    // 总分支数
    u32 correct_predictions;               // 正确预测数
    double accuracy;                       // 预测准确率
} branch_predictor_t;

// 冲突检测结果
typedef struct {
    hazard_type_t type;         // 冲突类型
    bool stall_required;        // 是否需要停顿
    u32 stall_cycles;           // 停顿周期数
    const char* description;    // 冲突描述
} hazard_detection_t;

// ==================== 流水线性能统计 ====================

// 流水线统计信息
typedef struct {
    // 基本统计
    u64 total_cycles;           // 总周期数
    u64 total_instructions;     // 总指令数
    u64 stall_cycles;           // 停顿周期数
    
    // 冲突统计
    u64 structural_hazards;     // 结构冲突次数
    u64 data_hazards;           // 数据冲突次数
    u64 control_hazards;        // 控制冲突次数
    u64 load_use_hazards;       // Load-Use冲突次数
    
    // 分支统计
    u64 branch_instructions;    // 分支指令数
    u64 branch_predictions;     // 分支预测次数
    u64 branch_mispredictions;  // 分支预测错误次数
    
    // 转发统计
    u64 forwarding_ex;          // EX阶段转发次数
    u64 forwarding_mem;         // MEM阶段转发次数
    
    // 性能指标
    double cpi;                 // 平均CPI
    double ipc;                 // 每周期指令数
    double efficiency;          // 流水线效率
    double throughput;          // 吞吐率
    double branch_accuracy;     // 分支预测准确率
} pipeline_stats_t;

// ==================== 主要数据结构 ====================

// 流水线处理器
typedef struct {
    // 流水线寄存器
    if_id_reg_t if_id;          // IF/ID寄存器
    id_ex_reg_t id_ex;          // ID/EX寄存器
    ex_mem_reg_t ex_mem;        // EX/MEM寄存器
    mem_wb_reg_t mem_wb;        // MEM/WB寄存器
    
    // 处理器状态
    u32 pc;                     // 程序计数器
    u32 registers[PIPELINE_MAX_REGISTERS]; // 寄存器文件
    u32 instruction_memory[INSTRUCTION_MEMORY_SIZE/4]; // 指令存储器
    u32 data_memory[DATA_MEMORY_SIZE/4]; // 数据存储器
    
    // 冲突检测和处理
    forwarding_control_t forwarding; // 转发控制
    branch_predictor_t predictor;     // 分支预测器
    bool stall_pipeline;              // 流水线停顿标志
    u32 stall_reason;                 // 停顿原因
    
    // 统计信息
    pipeline_stats_t stats;     // 性能统计
    u32 clock_cycle;            // 当前时钟周期
    bool running;               // 运行状态
    bool debug_mode;            // 调试模式
} pipeline_t;

// ==================== 核心函数接口 ====================

/**
 * 初始化流水线处理器
 * 
 * 初始化流程：
 * 1. 参数有效性检查 - 防止空指针访问
 * 2. 内存清零初始化 - 确保所有字段处于已知状态
 * 3. 寄存器文件初始化 - 将所有寄存器设置为0
 * 4. 流水线寄存器初始化 - 清空所有流水线级间寄存器
 * 5. 分支预测器初始化 - 设置预测器初始状态
 * 6. 性能统计模块初始化 - 清零所有性能计数器
 * 
 * 设计考虑：
 * 提供完整的处理器状态初始化，确保模拟器从确定的状态开始执行。
 * 支持多次重复初始化，便于测试和调试。
 * 
 * @param pipeline 流水线处理器结构指针，不能为NULL
 * @return 成功返回CPU_SUCCESS，参数无效返回CPU_ERROR_NULL_POINTER
 */
cpu_error_t pipeline_init(pipeline_t *pipeline);

/**
 * 销毁流水线处理器
 * @param pipeline 流水线处理器指针
 */
void pipeline_destroy(pipeline_t *pipeline);

/**
 * 重置流水线处理器
 * @param pipeline 流水线处理器指针
 */
void pipeline_reset(pipeline_t *pipeline);

/**
 * 加载程序到指令存储器
 * @param pipeline 流水线处理器指针
 * @param program 程序数组
 * @param size 程序大小
 * @return 成功返回true，失败返回false
 */
bool pipeline_load_program(pipeline_t *pipeline, const u32 *program, size_t size);

/**
 * 执行一个时钟周期
 * @param pipeline 流水线处理器指针
 * @return 成功返回true，程序结束返回false
 */
bool pipeline_clock(pipeline_t *pipeline);

/**
 * 运行流水线直到程序结束
 * @param pipeline 流水线处理器指针
 * @param max_cycles 最大执行周期数，0表示无限制
 * @return 实际执行的周期数
 */
u32 pipeline_run(pipeline_t *pipeline, u32 max_cycles);

// ==================== 流水线阶段函数 ====================

/**
 * 取指阶段（IF）
 * @param pipeline 流水线处理器指针
 */
void pipeline_stage_if(pipeline_t *pipeline);

/**
 * 指令译码阶段（ID）
 * @param pipeline 流水线处理器指针
 */
void pipeline_stage_id(pipeline_t *pipeline);

/**
 * 执行阶段（EX）
 * @param pipeline 流水线处理器指针
 */
void pipeline_stage_ex(pipeline_t *pipeline);

/**
 * 访存阶段（MEM）
 * @param pipeline 流水线处理器指针
 */
void pipeline_stage_mem(pipeline_t *pipeline);

/**
 * 写回阶段（WB）
 * @param pipeline 流水线处理器指针
 */
void pipeline_stage_wb(pipeline_t *pipeline);

// ==================== 冲突检测函数 ====================

/**
 * 检测数据冲突
 * @param pipeline 流水线处理器指针
 * @return 冲突检测结果
 */
hazard_detection_t pipeline_detect_data_hazard(const pipeline_t *pipeline);

/**
 * 检测结构冲突
 * @param pipeline 流水线处理器指针
 * @return 冲突检测结果
 */
hazard_detection_t pipeline_detect_structural_hazard(const pipeline_t *pipeline);

/**
 * 检测控制冲突
 * @param pipeline 流水线处理器指针
 * @return 冲突检测结果
 */
hazard_detection_t pipeline_detect_control_hazard(const pipeline_t *pipeline);

/**
 * 检测Load-Use冲突
 * @param pipeline 流水线处理器指针
 * @return 是否存在Load-Use冲突
 */
bool pipeline_detect_load_use_hazard(const pipeline_t *pipeline);

// ==================== 冲突解决函数 ====================

/**
 * 计算数据转发控制信号
 * @param pipeline 流水线处理器指针
 * @return 转发控制结构
 */
forwarding_control_t pipeline_calculate_forwarding(const pipeline_t *pipeline);

/**
 * 处理流水线停顿
 * @param pipeline 流水线处理器指针
 * @param hazard 冲突检测结果
 */
void pipeline_handle_stall(pipeline_t *pipeline, hazard_detection_t hazard);

/**
 * 冲刷流水线（分支预测错误时）
 * @param pipeline 流水线处理器指针
 * @param correct_pc 正确的PC值
 */
void pipeline_flush(pipeline_t *pipeline, u32 correct_pc);

// ==================== 分支预测函数 ====================

/**
 * 分支预测
 * @param predictor 分支预测器指针
 * @param pc 程序计数器
 * @return 预测结果（true表示跳转）
 */
bool branch_predict(branch_predictor_t *predictor, u32 pc);

/**
 * 更新分支预测器
 * @param predictor 分支预测器指针
 * @param pc 程序计数器
 * @param taken 实际分支结果
 */
void branch_update_predictor(branch_predictor_t *predictor, u32 pc, bool taken);

/**
 * 获取分支目标地址
 * @param predictor 分支预测器指针
 * @param pc 程序计数器
 * @return 预测的分支目标地址
 */
u32 branch_get_target(const branch_predictor_t *predictor, u32 pc);

/**
 * 重置分支预测器
 * @param predictor 分支预测器指针
 */
void branch_reset_predictor(branch_predictor_t *predictor);

// ==================== 指令解析函数 ====================

/**
 * 解析指令
 * @param inst_word 指令字
 * @return 解析后的指令结构
 */
instruction_t pipeline_decode_instruction(u32 inst_word);

/**
 * 获取指令类型字符串
 * @param type 指令类型
 * @return 类型字符串
 */
const char* pipeline_instruction_type_to_string(instruction_type_t type);

/**
 * 获取冲突类型字符串
 * @param type 冲突类型
 * @return 类型字符串
 */
const char* pipeline_hazard_type_to_string(hazard_type_t type);

// ==================== 性能分析函数 ====================

/**
 * 更新性能统计
 * @param pipeline 流水线处理器指针
 */
void pipeline_update_stats(pipeline_t *pipeline);

/**
 * 计算性能指标
 * @param pipeline 流水线处理器指针
 */
void pipeline_calculate_metrics(pipeline_t *pipeline);

/**
 * 获取性能统计
 * @param pipeline 流水线处理器指针
 * @return 性能统计结构
 */
pipeline_stats_t pipeline_get_stats(const pipeline_t *pipeline);

/**
 * 重置性能统计
 * @param pipeline 流水线处理器指针
 */
void pipeline_reset_stats(pipeline_t *pipeline);

// ==================== 调试和显示函数 ====================

/**
 * 打印流水线状态
 * @param pipeline 流水线处理器指针
 */
void pipeline_print_state(const pipeline_t *pipeline);

/**
 * 打印性能统计
 * @param pipeline 流水线处理器指针
 */
void pipeline_print_stats(const pipeline_t *pipeline);

/**
 * 打印寄存器状态
 * @param pipeline 流水线处理器指针
 */
void pipeline_print_registers(const pipeline_t *pipeline);

/**
 * 打印流水线寄存器内容
 * @param pipeline 流水线处理器指针
 */
void pipeline_print_pipeline_registers(const pipeline_t *pipeline);

/**
 * 打印指令执行跟踪
 * @param pipeline 流水线处理器指针
 * @param instruction 指令
 * @param stage 当前阶段
 */
void pipeline_print_instruction_trace(const pipeline_t *pipeline, 
                                    const instruction_t *instruction, 
                                    pipeline_stage_t stage);

/**
 * 设置调试模式
 * @param pipeline 流水线处理器指针
 * @param debug 调试模式开关
 */
void pipeline_set_debug_mode(pipeline_t *pipeline, bool debug);

// ==================== 408考研测试函数 ====================

/**
 * 408考研真题演示：流水线性能计算
 */
void pipeline_exam_demo_performance_calculation(void);

/**
 * 408考研真题演示：数据冲突分析
 */
void pipeline_exam_demo_data_hazard_analysis(void);

/**
 * 408考研真题演示：分支预测器
 */
void pipeline_exam_demo_branch_prediction(void);

/**
 * 408考研真题演示：指令调度优化
 */
void pipeline_exam_demo_instruction_scheduling(void);

/**
 * 流水线综合演示
 */
void pipeline_comprehensive_demo(void);

#endif // PIPELINE_SIM_H 