/* io.h - I/O系统模拟器头文件
 * 计算机组成原理模拟器 - I/O系统模块
 * create by: zw.duan
 * 
 * 设计目标：
 * 本模块实现了完整的计算机I/O子系统模拟，涵盖了四种经典的I/O控制方式、
 * 中断处理机制、DMA传输和设备调度算法。通过模拟真实的硬件行为和
 * 性能特征，为理解I/O系统工作原理提供直观的实践平台。
 * 
 * 核心特性：
 * 1. 多种I/O控制方式 - 程序查询、中断、DMA、通道方式
 * 2. 完整的中断系统 - 中断向量、优先级、屏蔽机制
 * 3. DMA控制器 - 直接内存访问，减少CPU负担  
 * 4. 设备调度算法 - FCFS、优先级、SSTF、电梯算法
 * 5. 性能监控系统 - 吞吐率、响应时间、利用率统计
 * 6. 多种设备模拟 - 键盘、显示器、磁盘、打印机等
 * 
 * I/O系统架构：
 * 
 *     CPU ←→ 中断控制器
 *      ↑        ↑
 *      ↓        ↓
 *   内存总线 ←→ DMA控制器
 *      ↑        ↑
 *      ↓        ↓
 *   I/O总线 ←→ 设备控制器1 ←→ 设备1(键盘)
 *      ↑        设备控制器2 ←→ 设备2(显示器)
 *      ↓        设备控制器3 ←→ 设备3(磁盘)
 *   系统总线     设备控制器N ←→ 设备N(打印机)
 * 
 * 工作流程：
 * 1. I/O请求产生 → 2. 设备调度 → 3. 控制方式选择 → 4. 数据传输 → 5. 完成处理
 * 
 * 教学价值：
 * - 深入理解I/O子系统的层次结构和工作机制
 * - 对比不同I/O控制方式的性能特征和适用场景
 * - 学习中断处理和DMA技术的实现原理
 * - 掌握I/O调度算法的设计思想和性能评估
 */

#ifndef IO_H
#define IO_H

#include "../common/types.h"
#include "../common/utils.h"
#include <stdbool.h>
#include <stdint.h>

// ==================== 基本数据类型 ====================

/**
 * I/O控制方式枚举
 * 
 * I/O控制方式对比分析：
 * 
 * ┌─────────────┬─────────┬─────────┬─────────┬─────────┬─────────┐
 * │ 控制方式     │ CPU占用  │ 响应速度  │ 实现复杂 │ 适用场景 │ 数据速率  │
 * ├────────────┼─────────┼─────────┼─────────┼─────────┼─────────┤
 * │ 程序查询     │ 很高    │ 快      │ 简单    │ 简单设备│ 低      │
 * │ 程序中断    │ 中等    │ 较快    │ 中等    │ 通用设备│ 中等    │
 * │ DMA方式     │ 低      │ 很快    │ 复杂    │ 高速设备│ 高      │
 * │ 通道方式     │ 很低    │ 很快    │ 很复杂  │ 大型机  │ 很高    │
 * └─────────────┴─────────┴─────────┴─────────┴─────────┴─────────┘
 * 
 * 技术演进历程：
 * 1. 程序查询：最早期的I/O控制方式，CPU主动轮询设备状态
 * 2. 程序中断：设备完成时主动通知CPU，提高了CPU利用率
 * 3. DMA方式：数据传输不经过CPU，大幅减少CPU开销
 * 4. 通道方式：具有简单处理能力的专用处理器控制I/O
 * 
 * 选择原则：
 * - 低速设备(键盘、鼠标)：程序查询或中断方式
 * - 中速设备(网络、音频)：中断方式
 * - 高速设备(磁盘、显卡)：DMA方式
 * - 超高速设备(大容量存储)：通道方式
 */
typedef enum {
    IO_POLLING = 0,         // 程序查询方式 - CPU主动轮询设备状态
    IO_INTERRUPT,           // 程序中断方式 - 设备主动通知CPU完成
    IO_DMA,                 // DMA方式 - 直接内存访问，绕过CPU
    IO_CHANNEL,             // 通道方式 - 专用处理器控制I/O操作
    IO_CONTROL_MAX
} io_control_mode_t;

// 设备类型
typedef enum {
    DEVICE_KEYBOARD = 0,    // 键盘
    DEVICE_DISPLAY,         // 显示器
    DEVICE_DISK,            // 磁盘
    DEVICE_PRINTER,         // 打印机
    DEVICE_MOUSE,           // 鼠标
    DEVICE_NETWORK,         // 网络设备
    DEVICE_TYPE_MAX
} device_type_t;

// 设备状态
typedef enum {
    DEVICE_IDLE = 0,        // 空闲
    DEVICE_BUSY,            // 忙碌
    DEVICE_ERROR,           // 错误
    DEVICE_OFFLINE,         // 离线
    DEVICE_STATUS_MAX
} device_status_t;

// I/O操作类型
typedef enum {
    IO_READ = 0,            // 读操作
    IO_WRITE,               // 写操作
    IO_CONTROL,             // 控制操作
    IO_STATUS,              // 状态操作
    IO_OP_MAX
} io_operation_t;

/**
 * 中断类型枚举
 * 
 * 中断分类与处理机制：
 * 
 * 中断优先级体系（数值越小优先级越高）：
 * ┌─────────────┬────────┬─────────────┬─────────────────┐
 * │ 中断类型    │ 优先级 │ 响应时间    │ 典型应用场景    │
 * ├─────────────┼────────┼─────────────┼─────────────────┤
 * │ 错误中断    │ 1      │ 立即        │ 硬件故障、数据错误│
 * │ 超时中断    │ 2      │ 很快        │ 看门狗、定时器  │
 * │ I/O完成中断 │ 3      │ 快          │ 数据传输完成    │
 * │ 设备就绪中断│ 4      │ 一般        │ 设备状态变化    │
 * └─────────────┴────────┴─────────────┴─────────────────┘
 * 
 * 中断处理流程：
 * 1. 中断信号产生 → 2. 中断识别 → 3. 保存现场 → 4. 中断服务 → 5. 恢复现场
 * 
 * 中断嵌套规则：
 * - 同级中断：不允许嵌套，需要等待当前中断处理完成
 * - 高级中断：可以打断低级中断，形成中断嵌套
 * - 屏蔽机制：可以通过中断屏蔽位控制特定中断的响应
 * 
 * 设计考虑：
 * - 错误中断具有最高优先级，确保系统安全
 * - 超时中断用于系统监控和异常恢复
 * - I/O完成中断是最常见的中断类型
 * - 设备就绪中断用于异步设备状态通知
 */
typedef enum {
    IRQ_NONE = 0,           // 无中断 - 正常运行状态
    IRQ_IO_COMPLETE,        // I/O完成中断 - 数据传输完成通知
    IRQ_ERROR,              // 错误中断 - 硬件错误或数据异常
    IRQ_DEVICE_READY,       // 设备就绪中断 - 设备状态变为可用
    IRQ_TIMEOUT,            // 超时中断 - 操作超时或定时器到期
    IRQ_TYPE_MAX
} irq_type_t;

// DMA状态
typedef enum {
    DMA_IDLE = 0,           // 空闲
    DMA_READY,              // 就绪
    DMA_ACTIVE,             // 活动中
    DMA_COMPLETE,           // 完成
    DMA_ERROR,              // 错误
    DMA_STATUS_MAX
} dma_status_t;

// ==================== 数据结构定义 ====================

// I/O请求结构
typedef struct io_request {
    u32 id;                         // 请求ID
    device_type_t device_type;      // 设备类型
    u32 device_id;                  // 设备ID
    io_operation_t operation;       // 操作类型
    u32 address;                    // 内存地址
    u32 size;                       // 数据大小
    u8 *data;                       // 数据缓冲区
    u32 timestamp;                  // 时间戳
    u32 priority;                   // 优先级
    bool complete;                  // 是否完成
    cpu_error_t status;             // 状态
    u32 service_time;               // 服务时间(us)
    struct io_request *next;        // 下一个请求（链表）
} io_request_t;

// 设备控制器结构
typedef struct device_controller {
    u32 id;                         // 控制器ID
    device_type_t device_type;      // 设备类型
    char name[32];                  // 控制器名称
    device_status_t status;         // 设备状态
    
    // 寄存器组
    u32 data_reg;                   // 数据寄存器
    u32 status_reg;                 // 状态寄存器
    u32 control_reg;                // 控制寄存器
    u32 address_reg;                // 地址寄存器
    
    // 缓冲区
    u8 *buffer;                     // 数据缓冲区
    u32 buffer_size;                // 缓冲区大小
    u32 buffer_pos;                 // 当前位置
    
    // 性能参数
    u32 transfer_rate;              // 传输速率(bytes/s)
    u32 access_time;                // 访问时间(us)
    u32 seek_time;                  // 寻道时间(us)
    
    // 统计信息
    u32 total_requests;             // 总请求数
    u32 completed_requests;         // 完成请求数
    u32 error_count;                // 错误计数
    u64 total_bytes;                // 总字节数
    u64 total_time;                 // 总时间(us)
    
    // 操作函数指针
    cpu_error_t (*init)(struct device_controller *ctrl);
    cpu_error_t (*read)(struct device_controller *ctrl, u32 addr, u8 *data, u32 size);
    cpu_error_t (*write)(struct device_controller *ctrl, u32 addr, const u8 *data, u32 size);
    cpu_error_t (*control)(struct device_controller *ctrl, u32 cmd, u32 param);
    void (*reset)(struct device_controller *ctrl);
} device_controller_t;

/**
 * 中断控制器结构体
 * 
 * 中断控制器工作原理：
 * 中断控制器是CPU与外设之间的中断管理中枢，负责中断信号的
 * 收集、优先级仲裁、向量分派和屏蔽控制等核心功能。
 * 
 * 核心组件说明：
 * 1. 中断向量表 - 存储16个中断服务程序的入口地址
 * 2. 中断屏蔽字 - 32位屏蔽寄存器，控制中断的启用/禁用
 * 3. 中断请求寄存器 - 记录待处理的中断请求
 * 4. 优先级数组 - 为每个中断号分配优先级权重
 * 
 * 中断处理机制：
 * 
 *   外设中断信号 → [中断请求寄存器] → [优先级仲裁]
 *                                        ↓
 *   CPU中断响应 ← [中断向量表查找] ← [屏蔽检查]
 * 
 * 优先级仲裁算法：
 * - 同时到达的多个中断按优先级数值排序
 * - 数值越小优先级越高（0为最高优先级）
 * - 高优先级中断可以抢占低优先级中断处理
 * 
 * 性能监控特性：
 * - 实时统计各类中断的发生频率
 * - 监控中断处理的累积时间开销
 * - 支持中断系统的性能分析和优化
 */
typedef struct interrupt_controller {
    u32 interrupt_vector[16];       // 中断向量表 - 存储中断服务程序地址
    u32 interrupt_mask;             // 中断屏蔽字 - 32位中断使能控制
    u32 interrupt_pending;          // 中断请求寄存器 - 待处理中断标志
    u32 interrupt_priority[16];     // 中断优先级 - 仲裁权重数组
    
    u32 current_irq;                // 当前中断号 - 正在处理的中断
    u32 irq_count[16];              // 各中断计数 - 统计中断频率
    u64 total_interrupts;           // 总中断数 - 系统级统计
    u64 interrupt_time;             // 中断处理总时间 - 性能分析
    
    bool enabled;                   // 中断使能 - 全局中断开关
} interrupt_controller_t;

/**
 * DMA控制器结构体
 * 
 * DMA（Direct Memory Access）工作原理：
 * DMA控制器是一种专用处理器，能够在不占用CPU的情况下
 * 直接在内存和外设之间进行高速数据传输，大幅提升I/O性能。
 * 
 * DMA传输过程：
 * 
 *   CPU设置 → [DMA初始化] → [总线仲裁] → [数据传输] → [完成中断]
 *             ↓              ↓           ↓           ↓
 *           寄存器配置      获得总线    字节级传输   状态更新
 * 
 * 核心寄存器组：
 * 1. 源地址寄存器 - 数据读取的起始内存地址
 * 2. 目的地址寄存器 - 数据写入的目标内存地址  
 * 3. 传输计数寄存器 - 待传输的数据字节数
 * 4. 控制寄存器 - 传输模式、方向、优先级等控制位
 * 5. 状态寄存器 - 传输进度、完成标志、错误状态
 * 
 * DMA传输模式：
 * - 单字节传输：每次传输一个字节，适合慢速设备
 * - 块传输：一次传输完整数据块，适合高速设备
 * - 突发传输：在总线空闲时进行高密度传输
 * 
 * 自动初始化模式：
 * 传输完成后自动重置地址和计数器，支持循环缓冲区操作，
 * 特别适用于音频、视频等连续数据流处理。
 * 
 * 性能优势：
 * - CPU利用率提升：减少CPU在数据传输上的时间开销
 * - 传输效率提高：专用硬件优化的传输速度
 * - 系统响应改善：CPU可以并行处理其他任务
 */
typedef struct dma_controller {
    u32 id;                         // DMA控制器ID - 系统唯一标识
    
    // DMA寄存器组 - 硬件控制接口
    u32 source_addr;                // 源地址寄存器 - 数据读取起始地址
    u32 dest_addr;                  // 目的地址寄存器 - 数据写入目标地址
    u32 transfer_count;             // 传输计数寄存器 - 剩余传输字节数
    u32 control_reg;                // 控制寄存器 - 传输模式和配置位
    u32 status_reg;                 // 状态寄存器 - 传输状态和标志位
    
    // DMA运行状态 - 当前工作状态
    dma_status_t status;            // DMA状态 - 空闲/就绪/活动/完成/错误
    bool active;                    // 是否活动 - 传输进行中标志
    bool auto_init;                 // 自动初始化 - 循环传输模式
    u32 channel;                    // DMA通道号 - 硬件通道标识
    
    // 性能统计 - 运行效率监控
    u32 total_transfers;            // 总传输次数 - 累计传输操作数
    u64 total_bytes;                // 总传输字节数 - 累计数据量
    u64 transfer_time;              // 传输总时间 - 性能分析基础
    
    // 当前操作 - 实时传输状态
    io_request_t *current_request;  // 当前请求 - 正在处理的I/O请求
} dma_controller_t;

// I/O系统主结构
typedef struct io_system {
    // 控制器数组
    device_controller_t controllers[DEVICE_TYPE_MAX];
    interrupt_controller_t irq_controller;
    dma_controller_t dma_controller;
    
    // 请求队列
    io_request_t *request_queue;    // 请求队列头
    io_request_t *request_tail;     // 请求队列尾
    u32 queue_length;               // 队列长度
    u32 max_queue_length;           // 最大队列长度
    
    // 系统状态
    io_control_mode_t control_mode; // 当前控制方式
    bool system_enabled;            // 系统使能
    u32 clock_cycles;               // 时钟周期计数
    
    // 性能统计
    u64 total_requests;             // 总请求数
    u64 completed_requests;         // 完成请求数
    u64 failed_requests;            // 失败请求数
    u64 total_wait_time;            // 总等待时间
    u64 total_service_time;         // 总服务时间
    
    // 调试信息
    bool debug_enabled;             // 调试开关
    char debug_buffer[1024];        // 调试缓冲区
} io_system_t;

// I/O操作结果
typedef struct io_result {
    bool success;                   // 是否成功
    u32 bytes_transferred;          // 传输字节数
    u32 time_used;                  // 使用时间(us)
    cpu_error_t error_code;         // 错误码
    char error_message[256];        // 错误消息
} io_result_t;

// 性能统计结构
typedef struct io_statistics {
    // 吞吐率统计
    double throughput;              // 吞吐率(requests/s)
    double bandwidth;               // 带宽(bytes/s)
    
    // 时间统计
    double avg_response_time;       // 平均响应时间(us)
    double avg_wait_time;           // 平均等待时间(us)
    double avg_service_time;        // 平均服务时间(us)
    
    // 利用率统计
    double cpu_utilization;         // CPU利用率
    double device_utilization[DEVICE_TYPE_MAX]; // 设备利用率
    double system_utilization;      // 系统利用率
    
    // 请求统计
    u32 total_requests;             // 总请求数
    u32 completed_requests;         // 完成请求数
    u32 failed_requests;            // 失败请求数
    u32 pending_requests;           // 待处理请求数
} io_statistics_t;

// ==================== 函数声明 ====================

// I/O系统生命周期管理
cpu_error_t io_system_init(io_system_t *io_sys);
void io_system_reset(io_system_t *io_sys);
void io_system_cleanup(io_system_t *io_sys);

// I/O请求管理
io_request_t* io_request_create(device_type_t device_type, u32 device_id, 
                               io_operation_t op, u32 addr, u32 size, u8 *data);
cpu_error_t io_request_submit(io_system_t *io_sys, io_request_t *request);
cpu_error_t io_request_cancel(io_system_t *io_sys, u32 request_id);
void io_request_destroy(io_request_t *request);

// 设备控制器操作
cpu_error_t device_controller_init(device_controller_t *ctrl, device_type_t type);
cpu_error_t device_controller_read(device_controller_t *ctrl, u32 addr, u8 *data, u32 size);
cpu_error_t device_controller_write(device_controller_t *ctrl, u32 addr, const u8 *data, u32 size);
cpu_error_t device_controller_control(device_controller_t *ctrl, u32 cmd, u32 param);
void device_controller_reset(device_controller_t *ctrl);

// I/O控制方式实现
cpu_error_t io_polling_execute(io_system_t *io_sys, io_request_t *request);
cpu_error_t io_interrupt_execute(io_system_t *io_sys, io_request_t *request);
cpu_error_t io_dma_execute(io_system_t *io_sys, io_request_t *request);
cpu_error_t io_channel_execute(io_system_t *io_sys, io_request_t *request);

// 中断控制
cpu_error_t interrupt_controller_init(interrupt_controller_t *irq_ctrl);
cpu_error_t interrupt_request(interrupt_controller_t *irq_ctrl, u32 irq_num, irq_type_t type);
cpu_error_t interrupt_acknowledge(interrupt_controller_t *irq_ctrl, u32 irq_num);
void interrupt_enable(interrupt_controller_t *irq_ctrl, u32 irq_mask);
void interrupt_disable(interrupt_controller_t *irq_ctrl, u32 irq_mask);

// DMA控制
cpu_error_t dma_controller_init(dma_controller_t *dma_ctrl);
cpu_error_t dma_setup_transfer(dma_controller_t *dma_ctrl, u32 src, u32 dest, u32 count);
cpu_error_t dma_start_transfer(dma_controller_t *dma_ctrl);
cpu_error_t dma_stop_transfer(dma_controller_t *dma_ctrl);
bool dma_is_complete(const dma_controller_t *dma_ctrl);

// I/O调度算法
cpu_error_t io_schedule_fcfs(io_system_t *io_sys);        // 先来先服务
cpu_error_t io_schedule_priority(io_system_t *io_sys);    // 优先级调度
cpu_error_t io_schedule_sstf(io_system_t *io_sys);        // 最短寻道时间优先
cpu_error_t io_schedule_elevator(io_system_t *io_sys);    // 电梯算法

// 性能分析
cpu_error_t io_calculate_statistics(const io_system_t *io_sys, io_statistics_t *stats);
double io_calculate_throughput(const io_system_t *io_sys, u32 time_window);
double io_calculate_utilization(const device_controller_t *ctrl, u32 time_window);
double io_calculate_response_time(const io_request_t *request);

// 系统监控
cpu_error_t io_system_step(io_system_t *io_sys);
cpu_error_t io_system_run(io_system_t *io_sys, u32 cycles);
void io_system_pause(io_system_t *io_sys);
void io_system_resume(io_system_t *io_sys);

// 设备特定操作
cpu_error_t keyboard_init(device_controller_t *ctrl);
cpu_error_t keyboard_scan(device_controller_t *ctrl, u8 *keycode);
cpu_error_t keyboard_cleanup(device_controller_t *ctrl);
cpu_error_t display_init(device_controller_t *ctrl);
cpu_error_t display_putchar(device_controller_t *ctrl, char c);
cpu_error_t display_cleanup(device_controller_t *ctrl);
cpu_error_t disk_init(device_controller_t *ctrl);
cpu_error_t disk_seek(device_controller_t *ctrl, u32 track);
cpu_error_t disk_cleanup(device_controller_t *ctrl);

// 调试和工具函数
const char* io_control_mode_to_string(io_control_mode_t mode);
const char* device_type_to_string(device_type_t type);
const char* device_status_to_string(device_status_t status);
const char* io_operation_to_string(io_operation_t op);

void io_system_print_status(const io_system_t *io_sys);
void io_request_print_info(const io_request_t *request);
void device_controller_print_info(const device_controller_t *ctrl);
void io_statistics_print(const io_statistics_t *stats);

bool io_system_validate(const io_system_t *io_sys);
void io_system_dump(const io_system_t *io_sys);

#endif // IO_H 