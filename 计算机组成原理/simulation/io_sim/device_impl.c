/* device_impl.c - 设备特定实现
 * 计算机组成原理模拟器 - I/O系统模块
 * create by: zw.duan
 * 
 * 设备抽象层实现：
 * 本文件实现了各种典型I/O设备的具体行为模拟，通过软件方式
 * 重现真实硬件设备的工作特征和性能参数。每种设备都有独特的
 * 操作模式、时序要求和数据格式。
 * 
 * 设备类型覆盖：
 * 1. 键盘设备 - 字符输入设备，模拟扫描码生成
 * 2. 显示设备 - 字符输出设备，模拟字符显示缓冲
 * 3. 磁盘设备 - 块存储设备，模拟寻道和数据传输
 * 4. 打印机 - 慢速输出设备，模拟打印队列
 * 5. 鼠标设备 - 指点设备，模拟位置和按键事件
 * 6. 网络设备 - 通信设备，模拟数据包收发
 * 
 * 实现特色：
 * - 真实的设备特性模拟（扫描码、寻道时间、传输速率等）
 * - 完整的设备状态管理（就绪、忙碌、错误状态）
 * - 设备特定的参数配置（缓冲区大小、性能参数）
 * - 统一的设备接口抽象（初始化、读写、控制、清理）
 * - 详细的错误处理和状态反馈机制
 * 
 * 教学价值：
 * - 理解不同设备类型的工作原理和特性差异
 * - 学习设备驱动程序的基本设计模式
 * - 掌握硬件抽象层的实现技术
 * - 体验真实系统中设备管理的复杂性
 */

#include "io.h"
#include "../common/debug.h"
#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==================== 键盘设备实现 ====================

cpu_error_t keyboard_init(device_controller_t *ctrl) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 如果缓冲区已经存在，不重新分配，只初始化内容
    if (!ctrl->buffer) {
        // 分配键盘缓冲区
        ctrl->buffer_size = 26; // 26个字母的缓冲区
        ctrl->buffer = (u8*)malloc(ctrl->buffer_size);
        if (!ctrl->buffer) {
            return CPU_ERROR_OUT_OF_MEMORY;
        }
    }

    // 如果缓冲区太小，扩展它
    if (ctrl->buffer_size < 26) {
        free(ctrl->buffer);
        ctrl->buffer_size = 26;
        ctrl->buffer = (u8*)malloc(ctrl->buffer_size);
        if (!ctrl->buffer) {
            return CPU_ERROR_OUT_OF_MEMORY;
        }
    }

    // 初始化为标准键盘扫描码 (A-Z: 65-90)
    u32 init_size = MIN(26, ctrl->buffer_size);
    for (u32 i = 0; i < init_size; i++) {
        ctrl->buffer[i] = 65 + i; // ASCII 'A' 到 'Z'
    }
    
    // 确保缓冲区没有0值（如果缓冲区小于26）
    if (ctrl->buffer_size < 26) {
        for (u32 i = 0; i < ctrl->buffer_size; i++) {
            if (ctrl->buffer[i] == 0) {
                ctrl->buffer[i] = 65 + (i % 26); // 循环填充A-Z
            }
        }
    }

    // 设置键盘状态
    ctrl->status_reg = 0x01;  // 键盘就绪
    ctrl->buffer_pos = 0;
    
    return CPU_SUCCESS;
}

cpu_error_t keyboard_scan(device_controller_t *ctrl, u8 *keycode) {
    if (!ctrl || !keycode) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (ctrl->device_type != DEVICE_KEYBOARD) {
        return CPU_ERROR_INVALID_PARAM;
    }
    
    // 模拟键盘扫描
    static u32 scan_pos = 0;
    
    // 如果缓冲区未初始化，先初始化
    if (!ctrl->buffer || ctrl->buffer_size == 0) {
        cpu_error_t init_result = keyboard_init(ctrl);
        if (init_result != CPU_SUCCESS) {
            return init_result;
        }
    }

    // 确保有有效的扫描码
        u32 buffer_scan_codes = (ctrl->buffer_size < 26) ? ctrl->buffer_size : 26;
        *keycode = ctrl->buffer[scan_pos % buffer_scan_codes];
        scan_pos = (scan_pos + 1) % buffer_scan_codes;
    
    // 确保扫描码非零
    if (*keycode == 0) {
        *keycode = 65; // 默认返回 'A'
    }
        
        // 更新状态寄存器
        ctrl->status_reg |= 0x02; // 设置数据就绪位
    ctrl->status_reg |= 0x01; // 确保设备就绪位也被设置
        ctrl->data_reg = *keycode;
        
        return CPU_SUCCESS;
}

cpu_error_t keyboard_cleanup(device_controller_t *ctrl) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 键盘设备没有特殊的清理需求
    return CPU_SUCCESS;
}

// ==================== 显示设备实现 ====================

cpu_error_t display_init(device_controller_t *ctrl) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 初始化显示器特定参数
    ctrl->status_reg = 0x01;  // 显示器就绪
    ctrl->control_reg = 0x07; // 默认属性：白色前景，黑色背景
    
    // 清空显示缓冲区
    if (ctrl->buffer) {
        memset(ctrl->buffer, ' ', ctrl->buffer_size);
    }
    
    return CPU_SUCCESS;
}

cpu_error_t display_putchar(device_controller_t *ctrl, char c) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (ctrl->device_type != DEVICE_DISPLAY) {
        return CPU_ERROR_INVALID_PARAM;
    }
    
    // 写入字符到当前位置
    if (ctrl->buffer && ctrl->buffer_pos < ctrl->buffer_size) {
        ctrl->buffer[ctrl->buffer_pos] = (u8)c;
        ctrl->buffer_pos++;
        
        // 更新状态
        ctrl->data_reg = (u32)c;
        ctrl->status_reg |= 0x02; // 写入完成
        
        return CPU_SUCCESS;
    }
    
    return CPU_ERROR_BUFFER_FULL;
}

cpu_error_t display_cleanup(device_controller_t *ctrl) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 显示设备没有特殊的清理需求
    return CPU_SUCCESS;
}

// ==================== 磁盘设备实现 ====================

typedef struct disk_params {
    u32 tracks;           // 磁道数
    u32 sectors_per_track; // 每磁道扇区数
    u32 bytes_per_sector; // 每扇区字节数
    u32 current_track;    // 当前磁头位置
    u32 rpm;              // 转速
} disk_params_t;

cpu_error_t disk_init(device_controller_t *ctrl) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 初始化磁盘参数
    disk_params_t *params = (disk_params_t*)malloc(sizeof(disk_params_t));
    if (!params) {
        return CPU_ERROR_OUT_OF_MEMORY;
    }
    
    params->tracks = 1024;
    params->sectors_per_track = 63;
    params->bytes_per_sector = 512;
    params->current_track = 0;
    params->rpm = 7200;
    
    // 将参数存储在控制寄存器中（简化）
    ctrl->control_reg = (u32)params;
    
    // 设置磁盘状态
    ctrl->status_reg = 0x01; // 磁盘就绪
    
    return CPU_SUCCESS;
}

cpu_error_t disk_seek(device_controller_t *ctrl, u32 track) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (ctrl->device_type != DEVICE_DISK) {
        return CPU_ERROR_INVALID_PARAM;
    }
    
    disk_params_t *params = (disk_params_t*)ctrl->control_reg;
    if (!params || track >= params->tracks) {
        return CPU_ERROR_INVALID_PARAM;
    }
    
    // 计算寻道时间
    u32 distance = (track > params->current_track) ? 
                   (track - params->current_track) : 
                   (params->current_track - track);
    
    u32 seek_time = ctrl->seek_time + distance * 100; // 每磁道100us
    
    // 模拟寻道延时
    for (u32 i = 0; i < seek_time / 100; i++) {
        // 模拟延时
    }
    
    params->current_track = track;
    ctrl->address_reg = track;
    
    return CPU_SUCCESS;
}

cpu_error_t disk_cleanup(device_controller_t *ctrl) {
    if (!ctrl) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 清理磁盘参数结构
    if (ctrl->control_reg) {
        disk_params_t *params = (disk_params_t*)ctrl->control_reg;
        free(params);
        ctrl->control_reg = 0;
    }
    
    return CPU_SUCCESS;
}

// ==================== I/O调度算法实现 ====================

cpu_error_t io_schedule_fcfs(io_system_t *io_sys) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 先来先服务：按请求到达顺序处理
    io_request_t *request = io_sys->request_queue;
    if (!request) {
        return CPU_SUCCESS; // 队列为空，但这是成功的情况
    }
    
    // 移除队列头部请求
    io_sys->request_queue = request->next;
    if (io_sys->request_tail == request) {
        io_sys->request_tail = NULL;
    }
    
    // 确保队列长度正确递减
    if (io_sys->queue_length > 0) {
    io_sys->queue_length--;
    }
    
    // 执行请求
    cpu_error_t result;
    u32 start_time = request->timestamp;
    u32 current_time = get_current_time();
    
    io_sys->total_wait_time += (current_time - start_time);
    
    switch (io_sys->control_mode) {
    case IO_POLLING:
        result = io_polling_execute(io_sys, request);
        break;
    case IO_INTERRUPT:
        result = io_interrupt_execute(io_sys, request);
        break;
    case IO_DMA:
        result = io_dma_execute(io_sys, request);
        break;
    case IO_CHANNEL:
        result = io_channel_execute(io_sys, request);
        break;
    default:
        result = CPU_ERROR_INVALID_PARAM;
        break;
    }
    
    // 更新统计信息
    if (result == CPU_SUCCESS) {
        io_sys->completed_requests++;
    } else {
        io_sys->failed_requests++;
    }
    
    request->complete = true;
    request->status = result;
    
    io_request_destroy(request);
    
    return result;
}

cpu_error_t io_schedule_priority(io_system_t *io_sys) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (!io_sys->request_queue) {
        return CPU_SUCCESS;
    }
    
    // 优先级调度：选择优先级最高的请求
    io_request_t *prev = NULL;
    io_request_t *current = io_sys->request_queue;
    io_request_t *highest_prev = NULL;
    io_request_t *highest = current;
    u32 highest_priority = current->priority;
    
    // 找到优先级最高的请求
    while (current) {
        if (current->priority > highest_priority) {
            highest_priority = current->priority;
            highest_prev = prev;
            highest = current;
        }
        prev = current;
        current = current->next;
    }
    
    // 从队列中移除选中的请求
    if (highest_prev) {
        highest_prev->next = highest->next;
    } else {
        io_sys->request_queue = highest->next;
    }
    
    if (io_sys->request_tail == highest) {
        io_sys->request_tail = highest_prev;
    }
    
    // 确保队列长度正确递减
    if (io_sys->queue_length > 0) {
    io_sys->queue_length--;
    }

    // 直接执行选中的请求，而不是调用FCFS
    cpu_error_t result;
    u32 start_time = highest->timestamp;
    u32 current_time = get_current_time();
    
    io_sys->total_wait_time += (current_time - start_time);

    switch (io_sys->control_mode) {
    case IO_POLLING:
        result = io_polling_execute(io_sys, highest);
        break;
    case IO_INTERRUPT:
        result = io_interrupt_execute(io_sys, highest);
        break;
    case IO_DMA:
        result = io_dma_execute(io_sys, highest);
        break;
    case IO_CHANNEL:
        result = io_channel_execute(io_sys, highest);
        break;
    default:
        result = CPU_ERROR_INVALID_PARAM;
        break;
    }

    // 更新统计信息
    if (result == CPU_SUCCESS) {
        io_sys->completed_requests++;
    } else {
        io_sys->failed_requests++;
    }

    highest->complete = true;
    highest->status = result;

    io_request_destroy(highest);
    
    return result;
}

cpu_error_t io_schedule_sstf(io_system_t *io_sys) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 最短寻道时间优先（仅适用于磁盘设备）
    io_request_t *prev = NULL;
    io_request_t *current = io_sys->request_queue;
    io_request_t *shortest_prev = NULL;
    io_request_t *shortest = NULL;
    u32 shortest_distance = UINT32_MAX;
    
    device_controller_t *disk_ctrl = &io_sys->controllers[DEVICE_DISK];
    u32 current_track = disk_ctrl->address_reg;
    
    // 找到寻道距离最短的磁盘请求
    while (current) {
        if (current->device_type == DEVICE_DISK) {
            u32 target_track = current->address / 512; // 假设每扇区512字节
            u32 distance = (target_track > current_track) ? 
                          (target_track - current_track) : 
                          (current_track - target_track);
            
            if (distance < shortest_distance) {
                shortest_distance = distance;
                shortest_prev = prev;
                shortest = current;
            }
        }
        prev = current;
        current = current->next;
    }
    
    if (!shortest) {
        // 没有磁盘请求，使用FCFS
        return io_schedule_fcfs(io_sys);
    }
    
    // 从队列中移除选中的请求
    if (shortest_prev) {
        shortest_prev->next = shortest->next;
    } else {
        io_sys->request_queue = shortest->next;
    }
    
    if (io_sys->request_tail == shortest) {
        io_sys->request_tail = shortest_prev;
    }
    
    // 确保队列长度正确递减
    if (io_sys->queue_length > 0) {
    io_sys->queue_length--;
    }
    
    // 执行请求
    cpu_error_t result;
    switch (io_sys->control_mode) {
    case IO_POLLING:
        result = io_polling_execute(io_sys, shortest);
        break;
    case IO_INTERRUPT:
        result = io_interrupt_execute(io_sys, shortest);
        break;
    case IO_DMA:
        result = io_dma_execute(io_sys, shortest);
        break;
    case IO_CHANNEL:
        result = io_channel_execute(io_sys, shortest);
        break;
    default:
        result = CPU_ERROR_INVALID_PARAM;
        break;
    }
    
    // 更新统计信息
    if (result == CPU_SUCCESS) {
        io_sys->completed_requests++;
    } else {
        io_sys->failed_requests++;
    }
    
    shortest->complete = true;
    shortest->status = result;
    
    io_request_destroy(shortest);
    
    return result;
}

cpu_error_t io_schedule_elevator(io_system_t *io_sys) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 电梯算法（扫描算法）的简化实现
    // 实际实现需要维护磁头移动方向状态
    static bool direction_up = true; // 磁头移动方向
    
    device_controller_t *disk_ctrl = &io_sys->controllers[DEVICE_DISK];
    u32 current_track = disk_ctrl->address_reg;
    
    io_request_t *prev = NULL;
    io_request_t *current = io_sys->request_queue;
    io_request_t *selected_prev = NULL;
    io_request_t *selected = NULL;
    
    if (direction_up) {
        // 向上扫描：找到当前磁道上方最近的请求
        u32 min_distance = UINT32_MAX;
        while (current) {
            if (current->device_type == DEVICE_DISK) {
                u32 target_track = current->address / 512;
                if (target_track >= current_track) {
                    u32 distance = target_track - current_track;
                    if (distance < min_distance) {
                        min_distance = distance;
                        selected_prev = prev;
                        selected = current;
                    }
                }
            }
            prev = current;
            current = current->next;
        }
        
        if (!selected) {
            // 没有向上的请求，改变方向
            direction_up = false;
            return io_schedule_elevator(io_sys);
        }
    } else {
        // 向下扫描：找到当前磁道下方最近的请求
        u32 min_distance = UINT32_MAX;
        while (current) {
            if (current->device_type == DEVICE_DISK) {
                u32 target_track = current->address / 512;
                if (target_track <= current_track) {
                    u32 distance = current_track - target_track;
                    if (distance < min_distance) {
                        min_distance = distance;
                        selected_prev = prev;
                        selected = current;
                    }
                }
            }
            prev = current;
            current = current->next;
        }
        
        if (!selected) {
            // 没有向下的请求，改变方向
            direction_up = true;
            return io_schedule_elevator(io_sys);
        }
    }
    
    if (!selected) {
        // 没有磁盘请求，使用FCFS
        return io_schedule_fcfs(io_sys);
    }
    
    // 从队列中移除选中的请求
    if (selected_prev) {
        selected_prev->next = selected->next;
    } else {
        io_sys->request_queue = selected->next;
    }
    
    if (io_sys->request_tail == selected) {
        io_sys->request_tail = selected_prev;
    }
    
    io_sys->queue_length--;
    
    // 执行请求
    cpu_error_t result;
    switch (io_sys->control_mode) {
    case IO_POLLING:
        result = io_polling_execute(io_sys, selected);
        break;
    case IO_INTERRUPT:
        result = io_interrupt_execute(io_sys, selected);
        break;
    case IO_DMA:
        result = io_dma_execute(io_sys, selected);
        break;
    case IO_CHANNEL:
        result = io_channel_execute(io_sys, selected);
        break;
    default:
        result = CPU_ERROR_INVALID_PARAM;
        break;
    }
    
    // 更新统计信息
    if (result == CPU_SUCCESS) {
        io_sys->completed_requests++;
    } else {
        io_sys->failed_requests++;
    }
    
    selected->complete = true;
    selected->status = result;
    
    io_request_destroy(selected);
    
    return result;
}

// ==================== 性能分析函数 ====================

cpu_error_t io_calculate_statistics(const io_system_t *io_sys, io_statistics_t *stats) {
    if (!io_sys || !stats) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    memset(stats, 0, sizeof(io_statistics_t));
    
    // 计算基本统计
    stats->total_requests = (u32)io_sys->total_requests;
    stats->completed_requests = (u32)io_sys->completed_requests;
    stats->failed_requests = (u32)io_sys->failed_requests;
    stats->pending_requests = io_sys->queue_length;
    
    // 计算时间统计
    if (io_sys->completed_requests > 0) {
        stats->avg_wait_time = (double)io_sys->total_wait_time / io_sys->completed_requests;
        stats->avg_service_time = (double)io_sys->total_service_time / io_sys->completed_requests;
        stats->avg_response_time = stats->avg_wait_time + stats->avg_service_time;
    }
    
    // 计算吞吐率
    if (io_sys->clock_cycles > 0) {
        stats->throughput = (double)io_sys->completed_requests * 1000000.0 / io_sys->clock_cycles;
    }
    
    // 计算设备利用率
    for (int i = 0; i < DEVICE_TYPE_MAX; i++) {
        const device_controller_t *ctrl = &io_sys->controllers[i];
        if (ctrl->total_time > 0 && io_sys->clock_cycles > 0) {
            stats->device_utilization[i] = (double)ctrl->total_time / io_sys->clock_cycles * 100.0;
        }
    }
    
    // 计算系统利用率
    u64 total_device_time = 0;
    for (int i = 0; i < DEVICE_TYPE_MAX; i++) {
        total_device_time += io_sys->controllers[i].total_time;
    }
    
    if (io_sys->clock_cycles > 0) {
        stats->system_utilization = (double)total_device_time / io_sys->clock_cycles * 100.0;
    }
    
    return CPU_SUCCESS;
}

double io_calculate_throughput(const io_system_t *io_sys, u32 time_window) {
    if (!io_sys || time_window == 0) {
        return 0.0;
    }
    
    return (double)io_sys->completed_requests / time_window;
}

double io_calculate_utilization(const device_controller_t *ctrl, u32 time_window) {
    if (!ctrl || time_window == 0) {
        return 0.0;
    }
    
    return (double)ctrl->total_time / time_window * 100.0;
}

double io_calculate_response_time(const io_request_t *request) {
    if (!request) {
        return 0.0;
    }
    
    // 简化计算：返回固定值
    return 1000.0; // 1ms
}

// ==================== 系统监控函数 ====================

cpu_error_t io_system_step(io_system_t *io_sys) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    if (!io_sys->system_enabled) {
        return CPU_ERROR_NOT_READY;
    }
    
    io_sys->clock_cycles++;
    
    // 处理一个请求（如果有的话）
    if (io_sys->request_queue) {
        return io_schedule_fcfs(io_sys);
    }
    
    return CPU_SUCCESS;
}

cpu_error_t io_system_run(io_system_t *io_sys, u32 cycles) {
    if (!io_sys) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    for (u32 i = 0; i < cycles; i++) {
        cpu_error_t result = io_system_step(io_sys);
        if (result != CPU_SUCCESS && result != CPU_ERROR_NO_DATA) {
            return result;
        }
    }
    
    return CPU_SUCCESS;
}

void io_system_pause(io_system_t *io_sys) {
    if (io_sys) {
        io_sys->system_enabled = false;
    }
}

void io_system_resume(io_system_t *io_sys) {
    if (io_sys) {
        io_sys->system_enabled = true;
    }
}

// get_current_time函数已在utils.c中定义 