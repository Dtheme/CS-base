/* io.c - I/O系统模拟器实现
 * 计算机组成原理模拟器 - I/O系统模块
 * create by: zw.duan
 * 
 * 实现理念：
 * 本文件实现了完整的I/O子系统核心功能，采用模块化分层设计，
 * 精确模拟计算机I/O硬件的工作机制和性能特征。通过软件模拟
 * 再现I/O控制方式的演进历程和技术特点。
 * 
 * 架构特色：
 * 1. 分层抽象设计 - 设备层、控制器层、系统层清晰分离
 * 2. 策略模式应用 - 支持多种I/O控制方式的动态切换
 * 3. 事件驱动模型 - 基于中断和回调的异步处理机制
 * 4. 性能监控体系 - 全方位的I/O性能指标采集和分析
 * 5. 可扩展架构 - 便于添加新设备类型和控制算法
 * 
 * 核心技术实现：
 * - 中断系统：完整的中断向量、优先级和嵌套处理
 * - DMA机制：真实模拟直接内存访问的工作流程
 * - 设备调度：实现经典的I/O调度算法和性能对比
 * - 状态机驱动：精确控制设备和传输的状态转换
 * - 时间驱动仿真：基于时钟周期的精确时序模拟
 * 
 * 教学价值：
 * - 深入理解I/O硬件工作原理和软件控制机制
 * - 体验不同I/O控制方式的性能差异和适用场景
 * - 掌握中断处理和DMA技术的实现细节
 * - 学习I/O系统的性能优化方法和设计原则
 */

#include "io.h"
#include "../common/debug.h"
#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ==================== 全局变量 ====================

static u32 g_request_id_counter = 1;
static u32 g_current_time = 0;

// ==================== 辅助函数 ====================

static u32 io_get_current_time(void) {
    return g_current_time++;
}

static u32 generate_request_id(void) {
    return g_request_id_counter++;
}

// ==================== I/O系统生命周期管理 ====================

/**
 * I/O系统初始化函数
 * 
 * 初始化流程设计：
 * I/O系统的初始化是一个复杂的多阶段过程，需要按照特定顺序
 * 初始化各个子系统，确保系统启动时所有组件都处于正确状态。
 * 
 * 初始化阶段分解：
 * 1. 结构体清零 - 确保所有字段都有确定的初始值
 * 2. 中断控制器初始化 - 设置中断向量表和优先级
 * 3. DMA控制器初始化 - 配置DMA通道和传输参数
 * 4. 设备控制器初始化 - 为每种设备类型创建控制器实例
 * 5. 系统参数设置 - 配置默认工作模式和性能参数
 * 
 * 依赖关系管理：
 * - 中断控制器必须最先初始化，为后续组件提供中断服务
 * - DMA控制器依赖于中断系统的完成信号
 * - 设备控制器需要中断和DMA系统的支持
 * 
 * 默认配置策略：
 * - 使用程序查询方式作为默认I/O控制模式，简单可靠
 * - 队列长度设为64，平衡内存占用和响应性能
 * - 调试功能默认关闭，避免影响性能测试
 * 
 * 错误恢复机制：
 * 任一子系统初始化失败都会导致整个系统初始化失败，
 * 调用者需要进行适当的清理和错误处理。
 * 
 * @param io_sys I/O系统主结构指针
 * @return 初始化结果码，CPU_SUCCESS表示成功
 */
cpu_error_t io_system_init(io_system_t *io_sys) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 第一阶段：结构体清零初始化 - 保证初始状态一致性
    memset(io_sys, 0, sizeof(io_system_t));
    
    // 第二阶段：初始化中断控制器 - I/O系统的核心基础设施
    if (interrupt_controller_init(&io_sys->irq_controller) != CPU_SUCCESS) {
        return CPU_ERROR_INIT_FAILED;
    }
    
    // 第三阶段：初始化DMA控制器 - 高性能数据传输引擎
    if (dma_controller_init(&io_sys->dma_controller) != CPU_SUCCESS) {
        return CPU_ERROR_INIT_FAILED;
    }
    
    // 第四阶段：初始化所有设备控制器 - 创建设备抽象层
    for (int i = 0; i < DEVICE_TYPE_MAX; i++) {
        if (device_controller_init(&io_sys->controllers[i], i) != CPU_SUCCESS) {
            return CPU_ERROR_INIT_FAILED;
        }
    }
    
    // 第五阶段：设置系统默认参数 - 配置初始工作状态
    io_sys->control_mode = IO_POLLING;      // 默认使用程序查询方式
    io_sys->system_enabled = true;          // 系统默认启用
    io_sys->max_queue_length = 64;          // 队列长度上限
    io_sys->debug_enabled = false;          // 调试功能默认关闭
    
    return CPU_SUCCESS;
}

void io_system_reset(io_system_t *io_sys) {
    if (!io_sys) {
        return;
    }
    
    // 清空请求队列
    io_request_t *current = io_sys->request_queue;
    while (current) {
        io_request_t *next = current->next;
        io_request_destroy(current);
        current = next;
    }
    
    // 重置控制器
    for (int i = 0; i < DEVICE_TYPE_MAX; i++) {
        device_controller_reset(&io_sys->controllers[i]);
    }
    
    // 重置统计信息
    io_sys->total_requests = 0;
    io_sys->completed_requests = 0;
    io_sys->failed_requests = 0;
    io_sys->total_wait_time = 0;
    io_sys->total_service_time = 0;
    io_sys->clock_cycles = 0;
    io_sys->queue_length = 0;
    io_sys->request_queue = NULL;
    io_sys->request_tail = NULL;
}

void io_system_cleanup(io_system_t *io_sys) {
    if (!io_sys) {
        return;
    }
    
    io_system_reset(io_sys);
    
    // 清理设备控制器
    for (int i = 0; i < DEVICE_TYPE_MAX; i++) {
        if (io_sys->controllers[i].buffer) {
            free(io_sys->controllers[i].buffer);
            io_sys->controllers[i].buffer = NULL;
        }
    }
}

// ==================== I/O请求管理 ====================

io_request_t* io_request_create(device_type_t device_type, u32 device_id, 
                               io_operation_t op, u32 addr, u32 size, u8 *data) {
    io_request_t *request = (io_request_t*)malloc(sizeof(io_request_t));
    if (!request) {
        return NULL;
    }
    
    memset(request, 0, sizeof(io_request_t));
    request->id = generate_request_id();
    request->device_type = device_type;
    request->device_id = device_id;
    request->operation = op;
    request->address = addr;
    request->size = size;
    request->data = data;
    request->timestamp = io_get_current_time();
    request->priority = 0;
    request->complete = false;
    request->status = CPU_SUCCESS;
    request->next = NULL;
    
    return request;
}

cpu_error_t io_request_submit(io_system_t *io_sys, io_request_t *request) {
    if (!io_sys || !request) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (!io_sys->system_enabled) {
        return CPU_ERROR_NOT_READY;
    }
    
    if (io_sys->queue_length >= io_sys->max_queue_length) {
        return CPU_ERROR_BUFFER_FULL;
    }
    
    // 添加到队列尾部
    if (!io_sys->request_queue) {
        io_sys->request_queue = request;
        io_sys->request_tail = request;
    } else {
        io_sys->request_tail->next = request;
        io_sys->request_tail = request;
    }
    
    io_sys->queue_length++;
    io_sys->total_requests++;
    
    return CPU_SUCCESS;
}

cpu_error_t io_request_cancel(io_system_t *io_sys, u32 request_id) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    io_request_t *prev = NULL;
    io_request_t *current = io_sys->request_queue;
    
    while (current) {
        if (current->id == request_id) {
            // 从队列中移除
            if (prev) {
                prev->next = current->next;
            } else {
                io_sys->request_queue = current->next;
            }
            
            if (current == io_sys->request_tail) {
                io_sys->request_tail = prev;
            }
            
            io_request_destroy(current);
            io_sys->queue_length--;
            return CPU_SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    
    return CPU_ERROR_NOT_FOUND;
}

void io_request_destroy(io_request_t *request) {
    if (request) {
        free(request);
    }
}

// ==================== 设备控制器操作 ====================

cpu_error_t device_controller_init(device_controller_t *ctrl, device_type_t type) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    memset(ctrl, 0, sizeof(device_controller_t));
    
    ctrl->id = type;
    ctrl->device_type = type;
    ctrl->status = DEVICE_IDLE;
    
    // 设置设备名称和参数
    switch (type) {
    case DEVICE_KEYBOARD:
        strcpy(ctrl->name, "键盘控制器");
        ctrl->transfer_rate = 100;      // 100 bytes/s
        ctrl->access_time = 1000;       // 1ms
        ctrl->buffer_size = 16;
        ctrl->init = keyboard_init;
        break;
    case DEVICE_DISPLAY:
        strcpy(ctrl->name, "显示控制器");
        ctrl->transfer_rate = 10000;    // 10KB/s
        ctrl->access_time = 100;        // 0.1ms
        ctrl->buffer_size = 2048;
        ctrl->init = display_init;
        break;
    case DEVICE_DISK:
        strcpy(ctrl->name, "磁盘控制器");
        ctrl->transfer_rate = 100000000; // 100MB/s
        ctrl->access_time = 5000;       // 5ms
        ctrl->seek_time = 8000;         // 8ms平均寻道时间
        ctrl->buffer_size = 65536;      // 64KB缓冲区
        ctrl->init = disk_init;
        break;
    case DEVICE_PRINTER:
        strcpy(ctrl->name, "打印机控制器");
        ctrl->transfer_rate = 5000;     // 5KB/s
        ctrl->access_time = 2000;       // 2ms
        ctrl->buffer_size = 1024;
        break;
    case DEVICE_MOUSE:
        strcpy(ctrl->name, "鼠标控制器");
        ctrl->transfer_rate = 200;      // 200 bytes/s
        ctrl->access_time = 500;        // 0.5ms
        ctrl->buffer_size = 8;
        break;
    case DEVICE_NETWORK:
        strcpy(ctrl->name, "网络控制器");
        ctrl->transfer_rate = 10000000; // 10MB/s
        ctrl->access_time = 1000;       // 1ms
        ctrl->buffer_size = 32768;      // 32KB缓冲区
        break;
    default:
        return CPU_ERROR_INVALID_PARAM;
    }
    
    // 分配缓冲区
    ctrl->buffer = (u8*)malloc(ctrl->buffer_size);
    if (!ctrl->buffer) {
        return CPU_ERROR_OUT_OF_MEMORY;
    }
    memset(ctrl->buffer, 0, ctrl->buffer_size);
    
    // 调用设备特定初始化
    if (ctrl->init) {
        return ctrl->init(ctrl);
    }
    
    return CPU_SUCCESS;
}

cpu_error_t device_controller_read(device_controller_t *ctrl, u32 addr, u8 *data, u32 size) {
    if (!ctrl || !data) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (ctrl->status != DEVICE_IDLE) {
        return CPU_ERROR_DEVICE_BUSY;
    }
    
    ctrl->status = DEVICE_BUSY;
    
    // 模拟访问时间
    u32 access_time = ctrl->access_time + (size * 1000000) / ctrl->transfer_rate;
    
    // 模拟读操作
    if (ctrl->read) {
        cpu_error_t result = ctrl->read(ctrl, addr, data, size);
        ctrl->status = (result == CPU_SUCCESS) ? DEVICE_IDLE : DEVICE_ERROR;
        return result;
    }
    
    // 默认实现：从缓冲区读取
    // 对于缓冲区访问，进行更灵活的处理
    if (ctrl->buffer && ctrl->buffer_size > 0) {
        // 如果访问超出缓冲区，只读取可用部分
        u32 actual_size = size;
        u32 start_addr = addr % ctrl->buffer_size; // 使用模运算处理大地址
        
        if (start_addr + actual_size > ctrl->buffer_size) {
            actual_size = ctrl->buffer_size - start_addr;
    }
    
        if (actual_size > 0) {
            memcpy(data, ctrl->buffer + start_addr, actual_size);
        }
        
        // 如果需要读取的数据比可用数据多，用零填充剩余部分
        if (actual_size < size) {
            memset(data + actual_size, 0, size - actual_size);
        }
    } else {
        // 如果没有缓冲区，用零填充
        memset(data, 0, size);
    }
    
    // 更新统计信息
    ctrl->total_requests++;
    ctrl->completed_requests++;
    ctrl->total_bytes += size;
    ctrl->total_time += access_time;
    
    ctrl->status = DEVICE_IDLE;
    return CPU_SUCCESS;
}

cpu_error_t device_controller_write(device_controller_t *ctrl, u32 addr, const u8 *data, u32 size) {
    if (!ctrl || !data) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (ctrl->status != DEVICE_IDLE) {
        return CPU_ERROR_DEVICE_BUSY;
    }
    
    ctrl->status = DEVICE_BUSY;
    
    // 模拟访问时间
    u32 access_time = ctrl->access_time + (size * 1000000) / ctrl->transfer_rate;
    
    // 模拟写操作
    if (ctrl->write) {
        cpu_error_t result = ctrl->write(ctrl, addr, data, size);
        ctrl->status = (result == CPU_SUCCESS) ? DEVICE_IDLE : DEVICE_ERROR;
        return result;
    }
    
    // 默认实现：写入缓冲区
    // 对于缓冲区访问，进行更灵活的处理
    if (ctrl->buffer && ctrl->buffer_size > 0) {
        // 如果访问超出缓冲区，只写入可用部分
        u32 actual_size = size;
        u32 start_addr = addr % ctrl->buffer_size; // 使用模运算处理大地址
        
        if (start_addr + actual_size > ctrl->buffer_size) {
            actual_size = ctrl->buffer_size - start_addr;
    }
    
        if (actual_size > 0) {
            memcpy(ctrl->buffer + start_addr, data, actual_size);
        }
    }
    
    // 更新统计信息
    ctrl->total_requests++;
    ctrl->completed_requests++;
    ctrl->total_bytes += size;
    ctrl->total_time += access_time;
    
    ctrl->status = DEVICE_IDLE;
    return CPU_SUCCESS;
}

cpu_error_t device_controller_control(device_controller_t *ctrl, u32 cmd, u32 param) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (ctrl->control) {
        return ctrl->control(ctrl, cmd, param);
    }
    
    // 默认控制命令处理
    switch (cmd) {
    case 0: // RESET
        device_controller_reset(ctrl);
        break;
    case 1: // ENABLE
        ctrl->status = DEVICE_IDLE;
        break;
    case 2: // DISABLE
        ctrl->status = DEVICE_OFFLINE;
        break;
    default:
        return CPU_ERROR_INVALID_PARAM;
    }
    
    return CPU_SUCCESS;
}

void device_controller_reset(device_controller_t *ctrl) {
    if (!ctrl) {
        return;
    }
    
    ctrl->status = DEVICE_IDLE;
    ctrl->data_reg = 0;
    ctrl->status_reg = 0;
    ctrl->control_reg = 0;
    ctrl->address_reg = 0;
    ctrl->buffer_pos = 0;
    
    if (ctrl->buffer) {
        memset(ctrl->buffer, 0, ctrl->buffer_size);
    }
    
    if (ctrl->reset) {
        ctrl->reset(ctrl);
    }
}

// ==================== I/O控制方式实现 ====================

/**
 * 程序查询方式I/O执行函数
 * 
 * 程序查询方式原理：
 * 这是最早期和最简单的I/O控制方式，CPU主动、反复地检查设备状态，
 * 直到设备准备好进行数据传输。虽然实现简单，但会占用大量CPU时间。
 * 
 * 工作流程：
 * 1. CPU发出I/O命令 → 2. 循环检查设备状态 → 3. 设备就绪后传输数据 → 4. 完成操作
 * 
 * 轮询循环模拟：
 *   while (设备忙碌) {
 *       检查状态寄存器;
 *       消耗CPU周期;
 *   }
 *   执行数据传输;
 * 
 * 性能特征：
 * - 优点：实现简单，不需要中断机制，适合简单系统
 * - 缺点：CPU利用率低，响应时间不确定，不适合多任务
 * - 适用场景：单任务系统、简单嵌入式设备、系统启动阶段
 * 
 * 时间开销分析：
 * 总时间 = 轮询等待时间 + 实际传输时间
 * 其中轮询时间通常远大于传输时间，造成CPU资源浪费
 * 
 * @param io_sys I/O系统指针
 * @param request I/O请求指针
 * @return 执行结果码
 */
cpu_error_t io_polling_execute(io_system_t *io_sys, io_request_t *request) {
    if (!io_sys || !request) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    device_controller_t *ctrl = &io_sys->controllers[request->device_type];
    
    u32 start_time = io_get_current_time();
    
    // 模拟轮询等待过程 - CPU反复检查设备状态
    // 这里用循环模拟CPU在等待过程中的时间消耗
    for (int i = 0; i < 100; i++) {
        io_get_current_time();  // 模拟状态检查的时间开销
    }
    
    // 执行实际I/O操作 - 设备准备就绪后的数据传输
    cpu_error_t result;
    if (request->operation == IO_READ) {
        result = device_controller_read(ctrl, request->address, request->data, request->size);
    } else {
        result = device_controller_write(ctrl, request->address, request->data, request->size);
    }
    
    // 记录操作完成信息
    u32 end_time = io_get_current_time();
    request->service_time = end_time - start_time;
    request->complete = true;
    request->status = result;
    
    return result;
}

/**
 * 程序中断方式I/O执行函数
 * 
 * 程序中断方式原理：
 * CPU发出I/O命令后不再等待，转去执行其他任务。设备完成操作后
 * 通过中断信号通知CPU，CPU响应中断并处理I/O完成事件。
 * 
 * 工作流程：
 * 1. CPU发出I/O命令 → 2. CPU执行其他任务 → 3. 设备完成后发中断 → 4. CPU响应中断处理
 * 
 * 中断处理机制：
 *   I/O操作启动 → 设备独立工作 → 操作完成 → 中断信号 → 中断服务程序
 * 
 * 性能优势：
 * - CPU利用率提高：不需要等待设备，可以执行其他任务
 * - 响应及时性：设备完成立即通知，减少延迟
 * - 多任务支持：支持并发处理多个I/O请求
 * 
 * 中断开销：
 * - 中断响应时间：从中断信号到开始处理的延迟
 * - 现场保存/恢复：寄存器和状态的保存开销
 * - 中断服务时间：处理中断的实际时间
 * 
 * 适用场景：
 * 通用计算机系统、多任务操作系统、中等速度的I/O设备
 * 
 * @param io_sys I/O系统指针
 * @param request I/O请求指针
 * @return 执行结果码
 */
cpu_error_t io_interrupt_execute(io_system_t *io_sys, io_request_t *request) {
    if (!io_sys || !request) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    device_controller_t *ctrl = &io_sys->controllers[request->device_type];
    
    u32 start_time = io_get_current_time();
    
    // 启动I/O操作 - CPU发出命令后即可去做其他事情
    cpu_error_t result;
    if (request->operation == IO_READ) {
        result = device_controller_read(ctrl, request->address, request->data, request->size);
    } else {
        result = device_controller_write(ctrl, request->address, request->data, request->size);
    }
    
    // 模拟设备完成后产生中断信号
    // 实际系统中这是由硬件自动产生的
    interrupt_request(&io_sys->irq_controller, request->device_type, IRQ_IO_COMPLETE);
    
    // 记录操作时间和状态
    u32 end_time = io_get_current_time();
    request->service_time = end_time - start_time;
    request->complete = true;
    request->status = result;
    
    return result;
}

/**
 * DMA方式I/O执行函数
 * 
 * DMA方式原理：
 * 使用专用的DMA控制器进行数据传输，CPU只需设置传输参数，
 * 数据传输完全由DMA控制器完成，CPU可以并行执行其他任务。
 * 
 * 工作流程：
 * 1. CPU设置DMA参数 → 2. 启动DMA传输 → 3. DMA独立传输 → 4. 传输完成中断
 * 
 * DMA传输过程：
 *   CPU配置 → DMA接管总线 → 内存↔设备直接传输 → 完成中断通知
 * 
 * 技术优势：
 * - 最高的CPU利用率：数据传输不占用CPU时间
 * - 高传输速度：硬件专用传输路径，无软件开销
 * - 大数据量支持：适合块设备的大容量数据传输
 * - 并行处理能力：CPU和I/O可以真正并行工作
 * 
 * DMA配置参数：
 * - 源地址：数据读取的内存起始地址
 * - 目标地址：数据写入的内存目标地址
 * - 传输计数：需要传输的字节数量
 * - 传输模式：单字节、块传输或突发传输
 * 
 * 应用场景：
 * 高速设备（磁盘、网卡、显卡）、大容量数据传输、
 * 多媒体数据流、高性能计算系统
 * 
 * @param io_sys I/O系统指针
 * @param request I/O请求指针
 * @return 执行结果码
 */
cpu_error_t io_dma_execute(io_system_t *io_sys, io_request_t *request) {
    if (!io_sys || !request) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    dma_controller_t *dma = &io_sys->dma_controller;
    
    // 第一步：设置DMA传输参数 - CPU的唯一工作
    cpu_error_t result = dma_setup_transfer(dma, request->address, 
                                          (uintptr_t)request->data, request->size);
    if (result != CPU_SUCCESS) {
        return result;
    }
    
    // 第二步：启动DMA传输 - 之后DMA独立工作
    result = dma_start_transfer(dma);
    if (result != CPU_SUCCESS) {
        return result;
    }
    
    // 第三步：DMA传输完成后自动产生中断
    // 实际硬件中，这是DMA控制器在传输完成时自动产生的
    interrupt_request(&io_sys->irq_controller, 0, IRQ_IO_COMPLETE);
    
    return CPU_SUCCESS;
}

/**
 * 通道方式I/O执行函数
 * 
 * 通道方式原理：
 * 通道是一种具有简单处理能力的专用处理器，能够执行通道程序
 * 来控制多个设备的I/O操作。这是大型机常用的高级I/O控制方式。
 * 
 * 通道特征：
 * - 独立的处理单元：有自己的指令集和控制逻辑
 * - 程序控制：通过通道程序（Channel Program）控制I/O
 * - 多设备管理：一个通道可以控制多个同类设备
 * - 智能调度：能够优化I/O操作的执行顺序
 * 
 * 通道程序构成：
 * - 通道命令字（CCW）：描述具体的I/O操作
 * - 数据地址：指定数据在内存中的位置
 * - 传输计数：指定传输的数据量
 * - 控制信息：链接、跳转、结束等控制标志
 * 
 * 工作流程：
 * 1. CPU编写通道程序 → 2. 启动通道执行 → 3. 通道独立执行程序 → 4. 完成后中断CPU
 * 
 * 性能特点：
 * - 最低的CPU干预：CPU只需启动通道，其余工作由通道完成
 * - 最高的I/O效率：专用处理器优化的I/O执行
 * - 复杂操作支持：支持复杂的I/O操作序列和错误处理
 * 
 * 注意：本实现中简化为调用中断方式，实际通道功能更加复杂
 * 
 * @param io_sys I/O系统指针
 * @param request I/O请求指针
 * @return 执行结果码
 */
cpu_error_t io_channel_execute(io_system_t *io_sys, io_request_t *request) {
    if (!io_sys || !request) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 通道方式：通过通道程序执行I/O操作
    // 在实际系统中，这里会涉及通道程序的解析和执行
    // 本实现中简化为调用中断方式，保持接口一致性
    return io_interrupt_execute(io_sys, request);
}

// ==================== 中断控制 ====================

/**
 * 中断控制器初始化函数
 * 
 * 初始化目标：
 * 建立完整的中断管理基础设施，包括中断向量表、优先级体系、
 * 屏蔽机制和统计系统，为I/O设备提供可靠的中断服务。
 * 
 * 中断向量表设计：
 * 采用固定向量表方案，每个中断号对应一个4字节的向量地址。
 * 向量表起始地址为0x100，第i个中断的向量地址为0x100 + i*4。
 * 这种设计简化了中断分派逻辑，提高了响应速度。
 * 
 * 优先级分配策略：
 * 默认采用中断号即优先级的简单方案，中断号越小优先级越高。
 * 这适合大多数应用场景，同时支持后续的动态优先级调整。
 * 
 * 屏蔽机制：
 * 使用32位屏蔽寄存器，每一位对应一个中断的使能状态。
 * 位值为1表示允许中断，位值为0表示屏蔽中断。
 * 默认允许所有中断（0xFFFF），提供最大的灵活性。
 * 
 * 统计功能：
 * 为每个中断号维护独立的计数器，记录中断发生频率。
 * 同时统计系统级的总中断数，支持性能分析和调优。
 * 
 * @param irq_ctrl 中断控制器指针
 * @return 初始化结果码
 */
cpu_error_t interrupt_controller_init(interrupt_controller_t *irq_ctrl) {
    if (!irq_ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    memset(irq_ctrl, 0, sizeof(interrupt_controller_t));
    irq_ctrl->enabled = true;
    irq_ctrl->interrupt_mask = 0xFFFF; // 默认允许所有中断
    irq_ctrl->interrupt_pending = 0;   // 确保初始化时没有待处理中断
    irq_ctrl->current_irq = 0;         // 初始化当前中断号
    irq_ctrl->total_interrupts = 0;    // 初始化总中断计数
    
    // 初始化每个中断源的计数器
    for (int i = 0; i < 16; i++) {
        irq_ctrl->irq_count[i] = 0;
    }
    
    return CPU_SUCCESS;
}

/**
 * 中断请求处理函数
 * 
 * 中断请求流程：
 * 当设备需要CPU服务时，通过此函数向中断控制器提交中断请求。
 * 中断控制器负责检查合法性、屏蔽状态，并记录请求信息。
 * 
 * 处理步骤详解：
 * 1. 参数合法性检查 - 验证中断控制器和中断号的有效性
 * 2. 系统状态检查 - 确认中断系统处于启用状态
 * 3. 屏蔽检查 - 验证该中断是否被屏蔽
 * 4. 请求记录 - 在待处理寄存器中设置对应位
 * 5. 统计更新 - 更新中断计数器和统计信息
 * 
 * 中断屏蔽机制：
 * 通过检查interrupt_mask寄存器的对应位来决定是否接受中断。
 * 这种位掩码方式允许细粒度的中断控制，支持中断优先级管理。
 * 
 * 统计信息维护：
 * - 单个中断计数：跟踪每个中断源的活跃程度
 * - 系统总计数：监控整体中断负载
 * - 支持性能分析：为系统优化提供数据基础
 * 
 * 错误处理：
 * - 无效参数：返回参数错误码
 * - 系统未启用：返回未就绪错误码  
 * - 中断被屏蔽：返回阻塞错误码
 * 
 * @param irq_ctrl 中断控制器指针
 * @param irq_num 中断号（0-15）
 * @param type 中断类型（用于扩展，当前未使用）
 * @return 处理结果码
 */
cpu_error_t interrupt_request(interrupt_controller_t *irq_ctrl, u32 irq_num, irq_type_t type) {
    (void)type; // 避免编译器未使用参数警告
    
    // 第一步：参数合法性检查
    if (!irq_ctrl || irq_num >= 16) {
        return CPU_ERROR_INVALID_PARAM;
    }
    
    // 第二步：系统状态检查 - 确保中断系统已启用
    if (!irq_ctrl->enabled) {
        return CPU_ERROR_NOT_READY;
    }
    
    // 第三步：中断屏蔽检查 - 验证中断是否被允许
    if (!(irq_ctrl->interrupt_mask & (1 << irq_num))) {
        return CPU_ERROR_BLOCKED;
    }
    
    // 第四步：记录中断请求 - 设置待处理标志位
    irq_ctrl->interrupt_pending |= (1 << irq_num);
    
    // 第五步：更新统计信息 - 维护中断频率统计
    irq_ctrl->irq_count[irq_num]++;     // 单个中断计数递增
    irq_ctrl->total_interrupts++;       // 系统总中断数递增
    
    return CPU_SUCCESS;
}

cpu_error_t interrupt_acknowledge(interrupt_controller_t *irq_ctrl, u32 irq_num) {
    if (!irq_ctrl || irq_num >= 16) {
        return CPU_ERROR_INVALID_PARAM;
    }
    
    // 检查中断是否真的在待处理状态
    if (!(irq_ctrl->interrupt_pending & (1 << irq_num))) {
        return CPU_ERROR_INVALID_PARAM; // 中断未待处理
    }
    
    // 清除中断请求
    irq_ctrl->interrupt_pending &= ~(1 << irq_num);
    irq_ctrl->current_irq = irq_num;
    
    return CPU_SUCCESS;
}

void interrupt_enable(interrupt_controller_t *irq_ctrl, u32 irq_mask) {
    if (irq_ctrl) {
        irq_ctrl->interrupt_mask |= irq_mask;
    }
}

void interrupt_disable(interrupt_controller_t *irq_ctrl, u32 irq_mask) {
    if (irq_ctrl) {
        irq_ctrl->interrupt_mask &= ~irq_mask;
    }
}

// ==================== DMA控制 ====================

cpu_error_t dma_controller_init(dma_controller_t *dma_ctrl) {
    if (!dma_ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    memset(dma_ctrl, 0, sizeof(dma_controller_t));
    dma_ctrl->status = DMA_IDLE;
    dma_ctrl->channel = 0;
    dma_ctrl->auto_init = false;
    
    return CPU_SUCCESS;
}

cpu_error_t dma_setup_transfer(dma_controller_t *dma_ctrl, u32 src, u32 dest, u32 count) {
    if (!dma_ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (dma_ctrl->status == DMA_ACTIVE) {
        return CPU_ERROR_DEVICE_BUSY;
    }
    
    dma_ctrl->source_addr = src;
    dma_ctrl->dest_addr = dest;
    dma_ctrl->transfer_count = count;
    dma_ctrl->status_reg = 0;
    dma_ctrl->status = DMA_READY;
    
    return CPU_SUCCESS;
}

cpu_error_t dma_start_transfer(dma_controller_t *dma_ctrl) {
    if (!dma_ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (dma_ctrl->status != DMA_READY) {
        return CPU_ERROR_NOT_READY;
    }
    
    dma_ctrl->status = DMA_ACTIVE;
    dma_ctrl->active = true;
    u32 start_time = io_get_current_time();
    
    // 模拟DMA传输
    for (u32 i = 0; i < dma_ctrl->transfer_count; i++) {
        io_get_current_time(); // 模拟传输时间
    }
    
    u32 end_time = io_get_current_time();
    dma_ctrl->transfer_time += end_time - start_time;
    dma_ctrl->total_transfers++;
    dma_ctrl->total_bytes += dma_ctrl->transfer_count;
    dma_ctrl->status = DMA_COMPLETE;
    dma_ctrl->active = false;
    
    return CPU_SUCCESS;
}

cpu_error_t dma_stop_transfer(dma_controller_t *dma_ctrl) {
    if (!dma_ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (dma_ctrl->status == DMA_ACTIVE) {
        dma_ctrl->status = DMA_IDLE;
    }
    dma_ctrl->active = false;
    dma_ctrl->control_reg &= ~0x01; // 清除启动位
    
    return CPU_SUCCESS;
}

bool dma_is_complete(const dma_controller_t *dma_ctrl) {
    if (!dma_ctrl) {
        return false;
    }
    
    return dma_ctrl->status == DMA_COMPLETE;
}

// ==================== 调试和工具函数 ====================

const char* io_control_mode_to_string(io_control_mode_t mode) {
    switch (mode) {
    case IO_POLLING: return "程序查询";
    case IO_INTERRUPT: return "程序中断";
    case IO_DMA: return "DMA";
    case IO_CHANNEL: return "通道";
    default: return "未知";
    }
}

const char* device_type_to_string(device_type_t type) {
    switch (type) {
    case DEVICE_KEYBOARD: return "键盘";
    case DEVICE_DISPLAY: return "显示器";
    case DEVICE_DISK: return "磁盘";
    case DEVICE_PRINTER: return "打印机";
    case DEVICE_MOUSE: return "鼠标";
    case DEVICE_NETWORK: return "网络";
    default: return "未知设备";
    }
}

const char* device_status_to_string(device_status_t status) {
    switch (status) {
    case DEVICE_IDLE: return "空闲";
    case DEVICE_BUSY: return "忙碌";
    case DEVICE_ERROR: return "错误";
    case DEVICE_OFFLINE: return "离线";
    default: return "未知状态";
    }
}

const char* io_operation_to_string(io_operation_t op) {
    switch (op) {
    case IO_READ: return "读";
    case IO_WRITE: return "写";
    case IO_CONTROL: return "控制";
    case IO_STATUS: return "状态";
    default: return "未知操作";
    }
}

void io_system_print_status(const io_system_t *io_sys) {
    if (!io_sys) {
        printf("I/O系统: NULL\n");
        return;
    }
    
    printf("=== I/O系统状态 ===\n");
    printf("控制方式: %s\n", io_control_mode_to_string(io_sys->control_mode));
    printf("系统状态: %s\n", io_sys->system_enabled ? "启用" : "禁用");
    printf("时钟周期: %u\n", io_sys->clock_cycles);
    printf("队列长度: %u/%u\n", io_sys->queue_length, io_sys->max_queue_length);
    printf("总请求数: %llu\n", io_sys->total_requests);
    printf("完成请求: %llu\n", io_sys->completed_requests);
    printf("失败请求: %llu\n", io_sys->failed_requests);
    printf("==================\n");
}

bool io_system_validate(const io_system_t *io_sys) {
    if (!io_sys) {
        return false;
    }
    
    // 检查基本结构
    if (io_sys->queue_length > io_sys->max_queue_length) {
        return false;
    }
    
    if (io_sys->completed_requests + io_sys->failed_requests > io_sys->total_requests) {
        return false;
    }
    
    return true;
} 