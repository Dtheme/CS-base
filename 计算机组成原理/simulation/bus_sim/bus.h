/* bus.h - 总线系统模拟器头文件
 * 计算机组成原理模拟器 - 总线模拟器模块
 * 遵循数据结构算法编码标准
 * create by: zw.duan
 */

#ifndef BUS_H
#define BUS_H

#include "../common/types.h"
#include "../common/debug.h"

/**
 * 总线系统模拟器
 * 
 * 设计原理：
 * 总线是计算机系统中各部件之间进行信息传输的公共通路，它决定了系统的
 * 数据传输能力和整体性能。本模块模拟了完整的总线体系结构，包括总线仲裁、
 * 信号控制、时序管理和性能监控等核心机制。
 * 
 * 架构特点：
 * 1. 多设备支持 - 支持CPU、内存、I/O控制器等多种设备类型
 * 2. 多种仲裁算法 - 链式查询、轮询、优先级等经典仲裁方式
 * 3. 完整信号控制 - 模拟真实的总线控制信号和时序
 * 4. 性能分析系统 - 带宽利用率、平均等待时间等指标统计
 * 5. 灵活配置支持 - 可配置总线宽度、工作模式等参数
 * 
 * 总线结构模型：
 * 
 *   设备1    设备2    设备3    设备N
 *     |        |        |        |
 *   ┌─┴────────┴────────┴────────┴─┐
 *   │        数据总线              │
 *   ├─────────────────────────────┤
 *   │        地址总线              │
 *   ├─────────────────────────────┤
 *   │        控制总线              │
 *   └─────────────────────────────┘
 *              |
 *          总线仲裁器
 * 
 * 仲裁机制：
 * 当多个设备同时请求总线时，仲裁器根据预设算法决定授权顺序，
 * 确保总线在任意时刻只被一个设备使用，避免数据冲突。
 * 
 * 核心价值：
 * - 为计算机系统架构设计提供完整的总线行为模拟
 * - 支持多种仲裁策略的性能对比分析
 * - 提供详细的总线使用统计和性能优化建议
 * - 为总线协议设计和验证提供测试平台
 */

// ==================== 常量定义 ====================

#define MAX_BUS_DEVICES     16      // 最大连接设备数
#define MAX_BUS_WIDTH       64      // 最大总线宽度(位)
#define MAX_ARBITRATION_QUEUE 8     // 仲裁队列最大长度

// ==================== 错误码定义 ====================

typedef enum {
    BUS_SUCCESS = 0,                // 操作成功
    BUS_ERROR_NULL_POINTER,         // 空指针错误
    BUS_ERROR_INVALID_PARAM,        // 参数无效
    BUS_ERROR_BUS_BUSY,            // 总线忙
    BUS_ERROR_NO_DEVICE,           // 设备不存在
    BUS_ERROR_ARBITRATION_FAILED,   // 仲裁失败
    BUS_ERROR_TIMEOUT,             // 操作超时
    BUS_ERROR_SIGNAL_CONFLICT       // 信号冲突
} bus_error_t;

// ==================== 总线信号定义 ====================

/**
 * 总线控制信号结构体
 * 
 * 设计原理：
 * 总线控制信号用于协调总线上各设备的操作，确保数据传输的正确性和时序。
 * 这些信号模拟了真实计算机系统中的总线控制逻辑。
 * 
 * 信号分类：
 * 1. 操作控制信号 - 指示读写操作类型
 * 2. 请求信号 - 标识访问的目标（内存/I/O）
 * 3. 状态信号 - 反映操作进度和结果
 * 4. 异常信号 - 处理错误和中断情况
 * 
 * 信号时序：
 * 
 *   时钟: ___┌─┐_┌─┐_┌─┐_┌─┐___
 *   
 *   请求: ___┌─────────────┐____
 *   使能: _____┌─────────┐______
 *   就绪: _______┌─────┐________
 *   确认: _________┌─┐__________
 * 
 * 信号互斥：
 * read_enable和write_enable不能同时为真
 * memory_request和io_request通常互斥
 */
typedef struct {
    bool read_enable;       // 读使能信号 - 指示读操作
    bool write_enable;      // 写使能信号 - 指示写操作
    bool memory_request;    // 内存请求信号 - 访问内存设备
    bool io_request;        // I/O请求信号 - 访问I/O设备
    bool interrupt_request; // 中断请求信号 - 请求CPU中断
    bool ready;            // 就绪信号 - 设备准备完成
    bool acknowledge;      // 确认信号 - 操作确认应答
    bool error;            // 错误信号 - 操作错误指示
} bus_control_signals_t;

// 总线数据包
typedef struct {
    u32 address;                    // 地址
    u32 data;                       // 数据
    bus_control_signals_t control;  // 控制信号
    u8 device_id;                   // 设备ID
    u32 timestamp;                  // 时间戳
} bus_packet_t;

// ==================== 设备管理 ====================

// 设备类型
typedef enum {
    DEVICE_CPU = 0,         // CPU
    DEVICE_MEMORY,          // 内存
    DEVICE_IO_CONTROLLER,   // I/O控制器
    DEVICE_DMA_CONTROLLER,  // DMA控制器
    DEVICE_BRIDGE,          // 总线桥
    DEVICE_UNKNOWN          // 未知设备
} device_type_t;

// 设备状态
typedef enum {
    DEVICE_IDLE = 0,        // 空闲
    DEVICE_REQUESTING,      // 请求总线
    DEVICE_USING_BUS,       // 使用总线
    DEVICE_WAITING,         // 等待响应
    DEVICE_ERROR            // 错误状态
} device_state_t;

// 总线设备
typedef struct {
    u8 device_id;                   // 设备ID
    device_type_t type;             // 设备类型
    device_state_t state;           // 设备状态
    u8 priority;                    // 优先级(0-255, 值越小优先级越高)
    bool bus_request;               // 总线请求标志
    bool bus_grant;                 // 总线授权标志
    u32 request_time;               // 请求时间
    u32 grant_time;                 // 授权时间
    u32 operation_count;            // 操作计数
    char name[32];                  // 设备名称
} bus_device_t;

// ==================== 仲裁机制 ====================

/**
 * 总线仲裁方式枚举
 * 
 * 仲裁算法对比：
 * 
 * ┌──────────────┬──────────┬──────────┬──────────┬──────────┐
 * │ 仲裁算法     │ 实现复杂度│ 响应速度 │ 公平性   │ 适用场景 │
 * ├──────────────┼──────────┼──────────┼──────────┼──────────┤
 * │ 链式查询     │ 简单     │ 慢       │ 差       │ 简单系统 │
 * │ 计数器查询   │ 中等     │ 中等     │ 好       │ 通用     │
 * │ 独立请求     │ 复杂     │ 快       │ 差       │ 高性能   │
 * │ 轮询         │ 简单     │ 中等     │ 最好     │ 实时系统 │
 * │ 优先级       │ 中等     │ 快       │ 差       │ 关键系统 │
 * │ 分布式       │ 复杂     │ 快       │ 好       │ 大型系统 │
 * └──────────────┴──────────┴──────────┴──────────┴──────────┘
 * 
 * 算法原理：
 * 1. 链式查询 - 设备依次串联，逐个检查请求
 * 2. 计数器查询 - 使用计数器轮询各设备
 * 3. 独立请求 - 每个设备有独立的请求线
 * 4. 轮询 - 按固定顺序循环检查设备
 * 5. 优先级 - 根据设备优先级决定授权
 * 6. 分布式 - 设备间协商决定总线使用权
 */
typedef enum {
    ARB_CHAIN_POLLING = 0,      // 链式查询 - 串行检查设备请求
    ARB_COUNTER_POLLING,        // 计数器查询 - 计数器驱动轮询
    ARB_INDEPENDENT_REQUEST,    // 独立请求 - 并行请求线路
    ARB_ROUND_ROBIN,           // 轮询算法 - 公平循环调度
    ARB_PRIORITY_BASED,        // 优先级仲裁 - 基于设备优先级
    ARB_DISTRIBUTED            // 分布式仲裁 - 设备间协商
} arbitration_method_t;

// 仲裁器状态
typedef enum {
    ARB_IDLE = 0,              // 空闲
    ARB_ARBITRATING,           // 仲裁中
    ARB_GRANTED,               // 已授权
    ARB_WAITING                // 等待
} arbitration_state_t;

// 仲裁请求
typedef struct {
    u8 device_id;              // 请求设备ID
    u8 priority;               // 优先级
    u32 request_time;          // 请求时间
    bool urgent;               // 紧急标志
} arbitration_request_t;

// 总线仲裁器
typedef struct {
    arbitration_method_t method;                    // 仲裁方式
    arbitration_state_t state;                      // 仲裁器状态
    u8 current_master;                              // 当前总线主设备
    u8 last_granted;                                // 上次授权的设备
    u32 counter;                                    // 计数器(用于计数器查询)
    arbitration_request_t queue[MAX_ARBITRATION_QUEUE]; // 仲裁队列
    u8 queue_size;                                  // 队列大小
    u32 arbitration_count;                          // 仲裁次数
    u32 total_arbitration_time;                     // 总仲裁时间
} bus_arbitrator_t;

// ==================== 总线结构 ====================

// 总线类型
typedef enum {
    BUS_SYSTEM = 0,            // 系统总线
    BUS_MEMORY,                // 内存总线
    BUS_IO,                    // I/O总线
    BUS_EXPANSION              // 扩展总线
} bus_type_t;

// 总线工作模式
typedef enum {
    BUS_MODE_SYNCHRONOUS = 0,  // 同步模式
    BUS_MODE_ASYNCHRONOUS      // 异步模式
} bus_mode_t;

// 总线配置
typedef struct {
    u8 data_width;             // 数据总线宽度(位)
    u8 address_width;          // 地址总线宽度(位)
    u32 clock_frequency;       // 时钟频率(Hz)
    bus_mode_t mode;           // 工作模式
    u8 max_devices;            // 最大设备数
    u32 timeout;               // 超时时间(ns)
} bus_config_t;

/**
 * 总线性能统计结构体
 * 
 * 性能指标体系：
 * 
 * 1. 操作统计
 *    - 总操作次数：反映总线使用频率
 *    - 读写比例：分析访问模式特征
 * 
 * 2. 时间统计
 *    - 忙碌/空闲周期：计算总线利用率
 *    - 仲裁周期：分析仲裁开销
 *    - 等待时间：评估响应性能
 * 
 * 3. 效率指标
 *    - 带宽利用率 = 有效传输时间 / 总时间
 *    - 平均等待时间 = 总等待时间 / 总操作次数
 *    - 仲裁效率 = (总周期数 - 仲裁周期数) / 总周期数
 * 
 * 4. 队列统计
 *    - 最大队列长度：反映系统负载峰值
 *    - 平均队列长度：衡量系统繁忙程度
 * 
 * 性能优化建议：
 * - 带宽利用率低：考虑增加数据宽度或提高时钟频率
 * - 等待时间长：优化仲裁算法或增加缓存
 * - 队列长度大：增加总线数量或改进调度策略
 */
typedef struct {
    u64 total_operations;      // 总操作次数 - 衡量系统吞吐量
    u64 read_operations;       // 读操作次数 - 读取访问统计
    u64 write_operations;      // 写操作次数 - 写入访问统计
    u64 total_cycles;          // 总周期数 - 系统运行时间
    u64 busy_cycles;           // 忙碌周期数 - 有效工作时间
    u64 idle_cycles;           // 空闲周期数 - 等待时间
    u64 arbitration_cycles;    // 仲裁周期数 - 仲裁开销时间
    u32 max_queue_length;      // 最大队列长度 - 负载峰值指标
    u32 total_wait_time;       // 总等待时间 - 响应延迟统计
    double bandwidth_utilization; // 带宽利用率 - 效率指标(0-1)
} bus_statistics_t;

// 总线系统主结构
typedef struct {
    bus_type_t type;                        // 总线类型
    bus_config_t config;                    // 总线配置
    bus_arbitrator_t arbitrator;            // 仲裁器
    bus_device_t devices[MAX_BUS_DEVICES];  // 连接的设备
    u8 device_count;                        // 设备数量
    bool bus_busy;                          // 总线忙标志
    u32 current_cycle;                      // 当前周期
    bus_packet_t current_packet;            // 当前传输包
    bus_statistics_t stats;                 // 性能统计
    char name[32];                          // 总线名称
} bus_t;

// ==================== 函数声明 ====================

// 总线生命周期管理
bus_error_t bus_init(bus_t *bus, bus_type_t type, const bus_config_t *config);
void bus_reset(bus_t *bus);
void bus_destroy(bus_t *bus);

// 设备管理
bus_error_t bus_add_device(bus_t *bus, device_type_t type, u8 priority, const char *name);
bus_error_t bus_remove_device(bus_t *bus, u8 device_id);
bus_device_t* bus_get_device(bus_t *bus, u8 device_id);
bus_error_t bus_set_device_priority(bus_t *bus, u8 device_id, u8 priority);

// 总线操作
bus_error_t bus_read(bus_t *bus, u8 device_id, u32 address, u32 *data);
bus_error_t bus_write(bus_t *bus, u8 device_id, u32 address, u32 data);
bus_error_t bus_transfer(bus_t *bus, const bus_packet_t *packet);

// 总线仲裁
bus_error_t bus_request(bus_t *bus, u8 device_id);
bus_error_t bus_release(bus_t *bus, u8 device_id);
u8 bus_arbitrate(bus_t *bus);
bus_error_t bus_set_arbitration_method(bus_t *bus, arbitration_method_t method);

// 仲裁算法实现
u8 chain_polling_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count);
u8 counter_polling_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count);
u8 independent_request_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count);
u8 round_robin_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count);
u8 priority_based_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count);

// 性能分析
double bus_calculate_bandwidth(const bus_t *bus);
double bus_calculate_utilization(const bus_t *bus);
double bus_calculate_efficiency(const bus_t *bus);
u32 bus_calculate_average_wait_time(const bus_t *bus);

// 统计和调试
void bus_update_statistics(bus_t *bus, const bus_packet_t *packet);
void bus_print_status(const bus_t *bus);
void bus_print_statistics(const bus_t *bus);
void bus_print_device_info(const bus_t *bus, u8 device_id);
void bus_dump_all_devices(const bus_t *bus);

// 时序模拟
bus_error_t bus_clock_tick(bus_t *bus);
bus_error_t bus_simulate_operation(bus_t *bus, u32 cycles);

// 辅助函数
const char* bus_error_to_string(bus_error_t error);
const char* device_type_to_string(device_type_t type);
const char* device_state_to_string(device_state_t state);
const char* arbitration_method_to_string(arbitration_method_t method);
const char* bus_type_to_string(bus_type_t type);

// 默认配置
bus_config_t bus_get_default_config(bus_type_t type);
bus_error_t bus_validate_config(const bus_config_t *config);

#endif // BUS_H 