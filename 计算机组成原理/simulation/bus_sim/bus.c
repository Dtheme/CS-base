/* bus.c - 总线系统模拟器实现
 * 计算机组成原理模拟器 - 总线模拟器模块
 * 遵循数据结构算法编码标准
 * create by: zw.duan
 */

#include "bus.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * 总线系统模拟器实现
 * 
 * 设计理念：
 * 本文件实现了完整的总线系统核心功能，基于分层模块化设计。
 * 通过精确模拟总线时序、仲裁算法和性能监控，为理解计算机
 * 组成原理中的总线机制提供实践平台。
 * 
 * 实现特点：
 * 1. 状态机驱动 - 使用状态机管理总线生命周期
 * 2. 策略模式 - 支持多种仲裁算法的动态切换
 * 3. 统计监控 - 实时收集和分析性能指标
 * 4. 错误处理 - 完善的异常检测和错误恢复机制
 * 5. 模块化设计 - 各功能模块独立，便于扩展维护
 * 
 * 技术亮点：
 * - 支持同步/异步总线模式
 * - 实现六种经典仲裁算法
 * - 提供完整的性能分析工具
 * - 模拟真实硬件的时序行为
 */

// ==================== 辅助函数 ====================

static void clear_control_signals(bus_control_signals_t *signals) {
    if (!signals) return;
    memset(signals, 0, sizeof(bus_control_signals_t));
}

static u32 get_current_time(void) {
    // 简化的时间获取，实际应用中可以使用系统时钟
    static u32 current_time = 0;
    return ++current_time;
}

// ==================== 总线生命周期管理 ====================

/**
 * 总线系统初始化函数
 * 
 * 初始化流程说明：
 * 1. 参数验证 - 检查输入参数的有效性
 * 2. 配置验证 - 验证总线配置的合理性
 * 3. 结构清零 - 初始化总线数据结构
 * 4. 基础配置 - 设置总线类型和工作参数
 * 5. 仲裁器初始化 - 配置默认仲裁算法
 * 6. 统计系统初始化 - 重置所有性能计数器
 * 
 * 设计考虑：
 * - 默认使用轮询仲裁算法，确保公平性
 * - 所有设备ID初始化为无效值(0xFF)
 * - 性能统计归零，准备收集运行数据
 * 
 * @param bus 总线系统指针
 * @param type 总线类型（系统/内存/I/O/扩展总线）
 * @param config 总线配置参数
 * @return 操作结果码
 */
bus_error_t bus_init(bus_t *bus, bus_type_t type, const bus_config_t *config) {
    if (!bus || !config) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    // 验证配置参数 - 检查数据宽度、时钟频率等参数的合理性
    if (bus_validate_config(config) != BUS_SUCCESS) {
        return BUS_ERROR_INVALID_PARAM;
    }
    
    // 清零总线结构 - 确保所有字段都是干净的初始状态
    memset(bus, 0, sizeof(bus_t));
    
    // 设置基本属性 - 复制配置参数并初始化运行状态
    bus->type = type;
    bus->config = *config;
    bus->device_count = 0;
    bus->bus_busy = false;
    bus->current_cycle = 0;
    
    // 初始化仲裁器 - 配置默认算法和状态
    bus->arbitrator.method = ARB_ROUND_ROBIN;  // 默认使用轮询算法
    bus->arbitrator.state = ARB_IDLE;          // 仲裁器初始为空闲状态
    bus->arbitrator.current_master = 0xFF;     // 无设备持有总线
    bus->arbitrator.last_granted = 0;          // 上次授权设备为0
    bus->arbitrator.counter = 0;               // 计数器归零
    bus->arbitrator.queue_size = 0;            // 仲裁队列为空
    bus->arbitrator.arbitration_count = 0;     // 仲裁次数归零
    bus->arbitrator.total_arbitration_time = 0; // 仲裁时间归零
    
    // 设置总线名称 - 根据类型生成描述性名称
    snprintf(bus->name, sizeof(bus->name), "%s_Bus", bus_type_to_string(type));
    
    // 初始化性能统计 - 所有计数器清零，准备收集数据
    memset(&bus->stats, 0, sizeof(bus_statistics_t));
    
    return BUS_SUCCESS;
}

void bus_reset(bus_t *bus) {
    if (!bus) return;
    
    // 重置所有设备状态
    for (u8 i = 0; i < bus->device_count; i++) {
        bus->devices[i].state = DEVICE_IDLE;
        bus->devices[i].bus_request = false;
        bus->devices[i].bus_grant = false;
        bus->devices[i].request_time = 0;
        bus->devices[i].grant_time = 0;
    }
    
    // 重置总线状态
    bus->bus_busy = false;
    bus->current_cycle = 0;
    memset(&bus->current_packet, 0, sizeof(bus_packet_t));
    
    // 重置仲裁器
    bus->arbitrator.state = ARB_IDLE;
    bus->arbitrator.current_master = 0xFF;
    bus->arbitrator.queue_size = 0;
    
    // 重置统计信息
    memset(&bus->stats, 0, sizeof(bus_statistics_t));
}

void bus_destroy(bus_t *bus) {
    if (!bus) return;
    
    // 清理所有资源
    memset(bus, 0, sizeof(bus_t));
}

// ==================== 设备管理 ====================

bus_error_t bus_add_device(bus_t *bus, device_type_t type, u8 priority, const char *name) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (bus->device_count >= bus->config.max_devices) {
        return BUS_ERROR_INVALID_PARAM;
    }
    
    u8 device_id = bus->device_count;
    bus_device_t *device = &bus->devices[device_id];
    
    // 初始化设备
    device->device_id = device_id;
    device->type = type;
    device->state = DEVICE_IDLE;
    device->priority = priority;
    device->bus_request = false;
    device->bus_grant = false;
    device->request_time = 0;
    device->grant_time = 0;
    device->operation_count = 0;
    
    // 设置设备名称
    if (name) {
        strncpy(device->name, name, sizeof(device->name) - 1);
        device->name[sizeof(device->name) - 1] = '\0';
    } else {
        snprintf(device->name, sizeof(device->name), "%s_%d", 
                device_type_to_string(type), device_id);
    }
    
    bus->device_count++;
    return BUS_SUCCESS;
}

bus_error_t bus_remove_device(bus_t *bus, u8 device_id) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    // 如果设备正在使用总线，返回错误
    if (bus->devices[device_id].state == DEVICE_USING_BUS) {
        return BUS_ERROR_BUS_BUSY;
    }
    
    // 移动后续设备
    for (u8 i = device_id; i < bus->device_count - 1; i++) {
        bus->devices[i] = bus->devices[i + 1];
        bus->devices[i].device_id = i;  // 更新设备ID
    }
    
    bus->device_count--;
    
    // 清零最后一个位置
    memset(&bus->devices[bus->device_count], 0, sizeof(bus_device_t));
    
    return BUS_SUCCESS;
}

bus_device_t* bus_get_device(bus_t *bus, u8 device_id) {
    if (!bus || device_id >= bus->device_count) {
        return NULL;
    }
    return &bus->devices[device_id];
}

bus_error_t bus_set_device_priority(bus_t *bus, u8 device_id, u8 priority) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    bus->devices[device_id].priority = priority;
    return BUS_SUCCESS;
}

// ==================== 总线操作 ====================

bus_error_t bus_read(bus_t *bus, u8 device_id, u32 address, u32 *data) {
    if (!bus || !data) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    // 创建读操作数据包
    bus_packet_t packet;
    packet.address = address;
    packet.data = 0;
    packet.device_id = device_id;
    packet.timestamp = get_current_time();
    clear_control_signals(&packet.control);
    packet.control.read_enable = true;
    packet.control.memory_request = true;
    
    // 执行传输
    bus_error_t result = bus_transfer(bus, &packet);
    if (result == BUS_SUCCESS) {
        *data = packet.data;
        bus->stats.read_operations++;
    }
    
    return result;
}

bus_error_t bus_write(bus_t *bus, u8 device_id, u32 address, u32 data) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    // 创建写操作数据包
    bus_packet_t packet;
    packet.address = address;
    packet.data = data;
    packet.device_id = device_id;
    packet.timestamp = get_current_time();
    clear_control_signals(&packet.control);
    packet.control.write_enable = true;
    packet.control.memory_request = true;
    
    // 执行传输
    bus_error_t result = bus_transfer(bus, &packet);
    if (result == BUS_SUCCESS) {
        bus->stats.write_operations++;
    }
    
    return result;
}

bus_error_t bus_transfer(bus_t *bus, const bus_packet_t *packet) {
    if (!bus || !packet) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    u8 device_id = packet->device_id;
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    // 请求总线
    bus_error_t result = bus_request(bus, device_id);
    if (result != BUS_SUCCESS) {
        return result;
    }
    
    // 等待仲裁
    u8 granted_device = bus_arbitrate(bus);
    if (granted_device != device_id) {
        return BUS_ERROR_ARBITRATION_FAILED;
    }
    
    // 执行传输
    bus->current_packet = *packet;
    bus->bus_busy = true;
    bus->devices[device_id].state = DEVICE_USING_BUS;
    
    // 模拟传输时间（根据总线模式）
    u32 transfer_cycles = (bus->config.mode == BUS_MODE_SYNCHRONOUS) ? 4 : 6;
    bus->current_cycle += transfer_cycles;
    bus->stats.busy_cycles += transfer_cycles;
    
    // 完成传输
    bus->bus_busy = false;
    bus->devices[device_id].state = DEVICE_IDLE;
    bus->devices[device_id].operation_count++;
    
    // 释放总线
    bus_release(bus, device_id);
    
    // 更新统计
    bus_update_statistics(bus, packet);
    
    return BUS_SUCCESS;
}

// ==================== 总线仲裁 ====================

bus_error_t bus_request(bus_t *bus, u8 device_id) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    bus_device_t *device = &bus->devices[device_id];
    
    // 设置请求标志
    device->bus_request = true;
    device->state = DEVICE_REQUESTING;
    device->request_time = get_current_time();
    
    return BUS_SUCCESS;
}

bus_error_t bus_release(bus_t *bus, u8 device_id) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    if (device_id >= bus->device_count) {
        return BUS_ERROR_NO_DEVICE;
    }
    
    bus_device_t *device = &bus->devices[device_id];
    
    // 清除请求和授权标志
    device->bus_request = false;
    device->bus_grant = false;
    device->state = DEVICE_IDLE;
    
    // 如果这是当前主设备，清除仲裁器状态
    if (bus->arbitrator.current_master == device_id) {
        bus->arbitrator.current_master = 0xFF;
        bus->arbitrator.state = ARB_IDLE;
    }
    
    return BUS_SUCCESS;
}

/**
 * 总线仲裁核心函数
 * 
 * 仲裁原理：
 * 当多个设备同时请求总线时，仲裁器需要按照预定算法决定哪个设备
 * 获得总线使用权。不同的仲裁算法有不同的公平性、响应速度和实现
 * 复杂度特征。
 * 
 * 仲裁流程：
 * 1. 检查仲裁器状态和设备请求
 * 2. 根据配置的仲裁方法调用相应算法
 * 3. 算法返回获胜设备ID（0xFF表示无获胜者）
 * 4. 更新仲裁器状态和设备授权标志
 * 5. 记录仲裁时间和统计信息
 * 
 * 仲裁策略对比：
 * - 链式查询：简单但不公平，固定优先级
 * - 计数器查询：公平性好，实现复杂度适中
 * - 独立请求：速度快但硬件成本高
 * - 轮询：最公平，适用于实时系统
 * - 优先级：响应快但可能饿死低优先级设备
 * 
 * @param bus 总线系统指针
 * @return 获胜设备ID，0xFF表示无设备获胜
 */
u8 bus_arbitrate(bus_t *bus) {
    if (!bus) {
        return 0xFF;
    }
    
    bus_arbitrator_t *arb = &bus->arbitrator;
    u8 winner = 0xFF;
    
    // 根据仲裁方式选择算法 - 策略模式实现
    switch (arb->method) {
    case ARB_CHAIN_POLLING:
        winner = chain_polling_arbitration(arb, bus->devices, bus->device_count);
        break;
    case ARB_COUNTER_POLLING:
        winner = counter_polling_arbitration(arb, bus->devices, bus->device_count);
        break;
    case ARB_INDEPENDENT_REQUEST:
        winner = independent_request_arbitration(arb, bus->devices, bus->device_count);
        break;
    case ARB_ROUND_ROBIN:
        winner = round_robin_arbitration(arb, bus->devices, bus->device_count);
        break;
    case ARB_PRIORITY_BASED:
        winner = priority_based_arbitration(arb, bus->devices, bus->device_count);
        break;
    default:
        // 默认使用链式查询作为后备方案
        winner = chain_polling_arbitration(arb, bus->devices, bus->device_count);
        break;
    }
    
    // 如果找到获胜者，更新仲裁器状态和设备权限
    if (winner != 0xFF) {
        arb->current_master = winner;          // 设置当前总线主设备
        arb->state = ARB_GRANTED;              // 仲裁器状态改为已授权
        arb->arbitration_count++;              // 仲裁次数递增
        bus->devices[winner].bus_grant = true; // 授权标志设置
        bus->devices[winner].grant_time = get_current_time(); // 记录授权时间
    }
    
    return winner;
}

bus_error_t bus_set_arbitration_method(bus_t *bus, arbitration_method_t method) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    bus->arbitrator.method = method;
    return BUS_SUCCESS;
}

// ==================== 仲裁算法实现 ====================

/**
 * 链式查询仲裁算法
 * 
 * 算法原理：
 * 设备按物理顺序连接成链式结构，仲裁器从第一个设备开始依次
 * 检查总线请求信号。第一个发出请求的设备获得总线使用权。
 * 
 * 特点分析：
 * - 优点：实现简单，硬件成本低，响应速度快
 * - 缺点：不公平，低编号设备优先级永远较高
 * - 适用：简单系统，设备较少的场景
 * 
 * 物理结构：
 *   仲裁器 → 设备0 → 设备1 → 设备2 → ... → 设备N
 *             ↓       ↓       ↓              ↓
 *           请求     请求     请求           请求
 * 
 * @param arb 仲裁器指针（此算法中未使用）
 * @param devices 设备数组
 * @param device_count 设备数量
 * @return 获胜设备ID，0xFF表示无设备请求
 */
u8 chain_polling_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count) {
    (void)arb; // 避免编译器未使用参数警告
    
    // 按设备编号顺序依次检查请求 - 体现固定优先级特性
    for (u8 i = 0; i < device_count; i++) {
        if (devices[i].bus_request) {
            return i;  // 返回第一个请求总线的设备
        }
    }
    return 0xFF;  // 无设备请求总线
}

/**
 * 计数器查询仲裁算法
 * 
 * 算法原理：
 * 使用计数器记录上次检查的设备位置，每次仲裁从计数器指示的
 * 设备开始循环检查，实现伪随机的起始点，提高公平性。
 * 
 * 特点分析：
 * - 优点：公平性较好，实现复杂度适中
 * - 缺点：需要额外的计数器硬件
 * - 适用：通用场景，平衡公平性和复杂度的需求
 * 
 * 检查顺序示例（设备0-3，计数器=2）：
 *   第1轮：设备2 → 设备3 → 设备0 → 设备1
 *   第2轮：设备3 → 设备0 → 设备1 → 设备2
 * 
 * @param arb 仲裁器指针（使用counter字段）
 * @param devices 设备数组
 * @param device_count 设备数量
 * @return 获胜设备ID，并更新计数器
 */
u8 counter_polling_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count) {
    // 从计数器指定位置开始循环检查 - 避免固定优先级
    u8 start = arb->counter % device_count;
    
    for (u8 i = 0; i < device_count; i++) {
        u8 device_id = (start + i) % device_count;
        if (devices[device_id].bus_request) {
            // 更新计数器到下一个位置，为下次仲裁做准备
            arb->counter = (device_id + 1) % device_count;
            return device_id;
        }
    }
    return 0xFF;  // 无设备请求总线
}

/**
 * 独立请求仲裁算法
 * 
 * 算法原理：
 * 每个设备都有独立的请求线连接到仲裁器，仲裁器可以并行
 * 检查所有请求，根据预设的优先级立即选择获胜者。
 * 
 * 特点分析：
 * - 优点：响应速度最快，支持真正的并行处理
 * - 缺点：硬件成本高，需要N条独立请求线
 * - 适用：高性能系统，对响应时间要求苛刻的场景
 * 
 * 硬件结构：
 *   设备0 ——请求线0——┐
 *   设备1 ——请求线1——┤ 仲裁器
 *   设备2 ——请求线2——┤ (并行比较)
 *   设备N ——请求线N——┘
 * 
 * @param arb 仲裁器指针（此算法中未使用）
 * @param devices 设备数组
 * @param device_count 设备数量
 * @return 优先级最高的请求设备ID
 */
u8 independent_request_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count) {
    (void)arb; // 避免编译器未使用参数警告
    
    // 并行扫描所有设备，寻找优先级最高的请求者
    u8 highest_priority = 255;  // 初始化为最低优先级
    u8 winner = 0xFF;           // 初始化为无效设备
    
    for (u8 i = 0; i < device_count; i++) {
        // 检查设备是否请求且优先级更高（数值越小优先级越高）
        if (devices[i].bus_request && devices[i].priority < highest_priority) {
            highest_priority = devices[i].priority;
            winner = i;
        }
    }
    
    return winner;
}

/**
 * 轮询仲裁算法（循环调度）
 * 
 * 算法原理：
 * 严格按照循环顺序为设备分配总线使用权，每次从上一个获得
 * 授权的设备的下一个设备开始检查，确保最大公平性。
 * 
 * 特点分析：
 * - 优点：公平性最好，防止设备饿死现象
 * - 缺点：可能影响高优先级设备的响应时间
 * - 适用：实时系统，要求公平调度的场景
 * 
 * 调度示例（设备0-3，上次授权设备1）：
 *   检查顺序：设备2 → 设备3 → 设备0 → 设备1
 *   确保每个设备都有相等的获得总线的机会
 * 
 * @param arb 仲裁器指针（使用last_granted字段）
 * @param devices 设备数组
 * @param device_count 设备数量
 * @return 按轮询顺序的下一个请求设备ID
 */
u8 round_robin_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count) {
    // 从上次授权设备的下一个开始检查 - 实现真正的轮询
    u8 start = (arb->last_granted + 1) % device_count;
    
    for (u8 i = 0; i < device_count; i++) {
        u8 device_id = (start + i) % device_count;
        if (devices[device_id].bus_request) {
            // 记录本次授权的设备，为下次轮询做准备
            arb->last_granted = device_id;
            return device_id;
        }
    }
    return 0xFF;  // 无设备请求总线
}

/**
 * 基于优先级的仲裁算法
 * 
 * 算法原理：
 * 纯粹基于设备优先级进行仲裁，总是选择优先级最高的请求设备。
 * 本实现复用独立请求算法的逻辑。
 * 
 * 特点分析：
 * - 优点：响应时间最短，关键设备优先保证
 * - 缺点：可能导致低优先级设备长期得不到服务（饿死）
 * - 适用：关键系统，有明确优先级要求的场景
 * 
 * 使用建议：
 * 在关键系统中，配合优先级继承或时间片轮转机制使用，
 * 避免低优先级设备完全饿死。
 * 
 * @param arb 仲裁器指针
 * @param devices 设备数组
 * @param device_count 设备数量
 * @return 优先级最高的请求设备ID
 */
u8 priority_based_arbitration(bus_arbitrator_t *arb, bus_device_t devices[], u8 device_count) {
    // 直接调用独立请求算法 - 两者逻辑相同，都是基于优先级选择
    return independent_request_arbitration(arb, devices, device_count);
}

// ==================== 性能分析 ====================

/**
 * 计算总线理论带宽
 * 
 * 带宽计算原理：
 * 总线的理论带宽取决于数据总线宽度和时钟频率，表示总线在
 * 理想条件下每秒能够传输的最大数据量。
 * 
 * 计算公式：
 * 理论带宽 = 数据总线宽度(位) ÷ 8 × 时钟频率(Hz)
 *         = 数据总线宽度(字节) × 时钟频率(Hz)
 * 
 * 应用场景：
 * - 系统设计时评估总线性能上限
 * - 与实际带宽对比，分析系统瓶颈
 * - 不同总线配置的性能对比
 * 
 * @param bus 总线系统指针
 * @return 理论带宽值(字节/秒)，出错时返回0.0
 */
double bus_calculate_bandwidth(const bus_t *bus) {
    if (!bus || bus->config.clock_frequency == 0) {
        return 0.0;
    }
    
    // 理论带宽 = 数据宽度(字节) × 时钟频率 - 基础性能上限
    double data_width_bytes = bus->config.data_width / 8.0;
    return data_width_bytes * bus->config.clock_frequency;
}

/**
 * 计算总线利用率
 * 
 * 利用率分析：
 * 利用率反映总线的繁忙程度，高利用率说明总线使用充分，
 * 但也可能意味着即将成为系统瓶颈。
 * 
 * 计算公式：
 * 利用率 = (忙碌周期数 ÷ 总周期数) × 100%
 * 
 * 评估标准：
 * - 0-30%：轻载，总线资源充足
 * - 30-70%：中载，正常工作范围
 * - 70-90%：重载，需要关注性能
 * - 90%以上：过载，可能成为瓶颈
 * 
 * @param bus 总线系统指针
 * @return 利用率百分比(0-100)，出错时返回0.0
 */
double bus_calculate_utilization(const bus_t *bus) {
    if (!bus || bus->stats.total_cycles == 0) {
        return 0.0;
    }
    
    // 利用率 = 忙碌周期 / 总周期 - 反映总线繁忙程度
    return (double)bus->stats.busy_cycles / bus->stats.total_cycles * 100.0;
}

/**
 * 计算总线传输效率
 * 
 * 效率分析：
 * 传输效率衡量总线在有效数据传输上的时间占比，排除仲裁等
 * 开销时间。高效率说明仲裁机制优化良好。
 * 
 * 计算公式：
 * 效率 = 有效数据传输周期 ÷ 总忙碌周期 × 100%
 *      = (忙碌周期 - 仲裁周期) ÷ 忙碌周期 × 100%
 * 
 * 优化建议：
 * - 效率低于60%：考虑优化仲裁算法
 * - 效率低于40%：检查总线配置和设备行为
 * - 效率低于20%：系统设计可能存在问题
 * 
 * @param bus 总线系统指针
 * @return 传输效率百分比(0-100)，出错时返回0.0
 */
double bus_calculate_efficiency(const bus_t *bus) {
    if (!bus || bus->stats.total_cycles == 0) {
        return 0.0;
    }
    
    // 计算有效数据传输周期 - 排除仲裁开销
    u64 data_cycles = bus->stats.busy_cycles - bus->stats.arbitration_cycles;
    if (bus->stats.busy_cycles == 0) return 0.0;
    
    // 效率 = 数据传输周期 / (数据传输周期 + 仲裁周期)
    return (double)data_cycles / bus->stats.busy_cycles * 100.0;
}

/**
 * 计算平均等待时间
 * 
 * 等待时间分析：
 * 平均等待时间反映系统的响应性能，对实时系统尤为重要。
 * 等待时间包括仲裁延迟、总线忙碌等待等因素。
 * 
 * 计算公式：
 * 平均等待时间 = 总等待时间 ÷ 总操作次数
 * 
 * 性能标准：
 * - 等待时间应小于操作时间的10%（实时系统）
 * - 等待时间应小于操作时间的50%（通用系统）
 * - 等待时间过长说明总线成为瓶颈
 * 
 * @param bus 总线系统指针
 * @return 平均等待时间(时间单位)，出错时返回0
 */
u32 bus_calculate_average_wait_time(const bus_t *bus) {
    if (!bus || bus->stats.total_operations == 0) {
        return 0;
    }
    
    // 平均等待时间 = 总等待时间 / 总操作次数
    return bus->stats.total_wait_time / (u32)bus->stats.total_operations;
}

// ==================== 统计和调试 ====================

void bus_update_statistics(bus_t *bus, const bus_packet_t *packet) {
    if (!bus || !packet) return;
    
    bus->stats.total_operations++;
    bus->stats.total_cycles = bus->current_cycle;
    
    // 更新队列长度统计
    u8 current_requests = 0;
    for (u8 i = 0; i < bus->device_count; i++) {
        if (bus->devices[i].bus_request) {
            current_requests++;
        }
    }
    if (current_requests > bus->stats.max_queue_length) {
        bus->stats.max_queue_length = current_requests;
    }
    
    // 计算带宽利用率
    bus->stats.bandwidth_utilization = bus_calculate_utilization(bus);
}

void bus_print_status(const bus_t *bus) {
    if (!bus) {
        printf("总线状态: NULL\n");
        return;
    }
    
    printf("=== 总线状态 ===\n");
    printf("总线名称: %s\n", bus->name);
    printf("总线类型: %s\n", bus_type_to_string(bus->type));
    printf("数据宽度: %d位\n", bus->config.data_width);
    printf("地址宽度: %d位\n", bus->config.address_width);
    printf("时钟频率: %d Hz\n", bus->config.clock_frequency);
    printf("工作模式: %s\n", (bus->config.mode == BUS_MODE_SYNCHRONOUS) ? "同步" : "异步");
    printf("设备数量: %d/%d\n", bus->device_count, bus->config.max_devices);
    printf("总线状态: %s\n", bus->bus_busy ? "忙碌" : "空闲");
    printf("当前周期: %d\n", bus->current_cycle);
    printf("仲裁方式: %s\n", arbitration_method_to_string(bus->arbitrator.method));
    printf("当前主设备: %s\n", (bus->arbitrator.current_master == 0xFF) ? "无" : 
           bus->devices[bus->arbitrator.current_master].name);
    printf("==================\n");
}

void bus_print_statistics(const bus_t *bus) {
    if (!bus) {
        printf("总线统计: NULL\n");
        return;
    }
    
    printf("=== 性能统计 ===\n");
    printf("总操作次数: %llu\n", bus->stats.total_operations);
    printf("读操作次数: %llu\n", bus->stats.read_operations);
    printf("写操作次数: %llu\n", bus->stats.write_operations);
    printf("总周期数: %llu\n", bus->stats.total_cycles);
    printf("忙碌周期: %llu\n", bus->stats.busy_cycles);
    printf("空闲周期: %llu\n", bus->stats.idle_cycles);
    printf("仲裁周期: %llu\n", bus->stats.arbitration_cycles);
    printf("最大队列长度: %d\n", bus->stats.max_queue_length);
    printf("平均等待时间: %d周期\n", bus_calculate_average_wait_time(bus));
    printf("理论带宽: %.2f MB/s\n", bus_calculate_bandwidth(bus) / 1e6);
    printf("总线利用率: %.2f%%\n", bus_calculate_utilization(bus));
    printf("传输效率: %.2f%%\n", bus_calculate_efficiency(bus));
    printf("带宽利用率: %.2f%%\n", bus->stats.bandwidth_utilization);
    printf("仲裁次数: %d\n", bus->arbitrator.arbitration_count);
    printf("===============\n");
}

void bus_print_device_info(const bus_t *bus, u8 device_id) {
    if (!bus || device_id >= bus->device_count) {
        printf("设备信息: 无效设备ID\n");
        return;
    }
    
    const bus_device_t *device = &bus->devices[device_id];
    
    printf("=== 设备信息 ===\n");
    printf("设备ID: %d\n", device->device_id);
    printf("设备名称: %s\n", device->name);
    printf("设备类型: %s\n", device_type_to_string(device->type));
    printf("设备状态: %s\n", device_state_to_string(device->state));
    printf("优先级: %d\n", device->priority);
    printf("总线请求: %s\n", device->bus_request ? "是" : "否");
    printf("总线授权: %s\n", device->bus_grant ? "是" : "否");
    printf("请求时间: %d\n", device->request_time);
    printf("授权时间: %d\n", device->grant_time);
    printf("操作次数: %d\n", device->operation_count);
    printf("===============\n");
}

void bus_dump_all_devices(const bus_t *bus) {
    if (!bus) {
        printf("设备列表: NULL总线\n");
        return;
    }
    
    printf("=== 所有设备列表 ===\n");
    printf("总设备数: %d\n", bus->device_count);
    printf("\n");
    
    for (u8 i = 0; i < bus->device_count; i++) {
        printf("设备%d: %s (%s) - 优先级:%d - 状态:%s\n",
               i, bus->devices[i].name,
               device_type_to_string(bus->devices[i].type),
               bus->devices[i].priority,
               device_state_to_string(bus->devices[i].state));
    }
    printf("==================\n");
}

// ==================== 时序模拟 ====================

bus_error_t bus_clock_tick(bus_t *bus) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    bus->current_cycle++;
    
    // 更新空闲周期统计
    if (!bus->bus_busy) {
        bus->stats.idle_cycles++;
    }
    
    return BUS_SUCCESS;
}

bus_error_t bus_simulate_operation(bus_t *bus, u32 cycles) {
    if (!bus) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    for (u32 i = 0; i < cycles; i++) {
        bus_clock_tick(bus);
    }
    
    return BUS_SUCCESS;
}

// ==================== 辅助函数 ====================

const char* bus_error_to_string(bus_error_t error) {
    switch (error) {
    case BUS_SUCCESS: return "SUCCESS";
    case BUS_ERROR_NULL_POINTER: return "NULL_POINTER";
    case BUS_ERROR_INVALID_PARAM: return "INVALID_PARAM";
    case BUS_ERROR_BUS_BUSY: return "BUS_BUSY";
    case BUS_ERROR_NO_DEVICE: return "NO_DEVICE";
    case BUS_ERROR_ARBITRATION_FAILED: return "ARBITRATION_FAILED";
    case BUS_ERROR_TIMEOUT: return "TIMEOUT";
    case BUS_ERROR_SIGNAL_CONFLICT: return "SIGNAL_CONFLICT";
    default: return "UNKNOWN_ERROR";
    }
}

const char* device_type_to_string(device_type_t type) {
    switch (type) {
    case DEVICE_CPU: return "CPU";
    case DEVICE_MEMORY: return "MEMORY";
    case DEVICE_IO_CONTROLLER: return "IO_CONTROLLER";
    case DEVICE_DMA_CONTROLLER: return "DMA_CONTROLLER";
    case DEVICE_BRIDGE: return "BRIDGE";
    case DEVICE_UNKNOWN: return "UNKNOWN";
    default: return "INVALID";
    }
}

const char* device_state_to_string(device_state_t state) {
    switch (state) {
    case DEVICE_IDLE: return "IDLE";
    case DEVICE_REQUESTING: return "REQUESTING";
    case DEVICE_USING_BUS: return "USING_BUS";
    case DEVICE_WAITING: return "WAITING";
    case DEVICE_ERROR: return "ERROR";
    default: return "INVALID";
    }
}

const char* arbitration_method_to_string(arbitration_method_t method) {
    switch (method) {
    case ARB_CHAIN_POLLING: return "CHAIN_POLLING";
    case ARB_COUNTER_POLLING: return "COUNTER_POLLING";
    case ARB_INDEPENDENT_REQUEST: return "INDEPENDENT_REQUEST";
    case ARB_ROUND_ROBIN: return "ROUND_ROBIN";
    case ARB_PRIORITY_BASED: return "PRIORITY_BASED";
    case ARB_DISTRIBUTED: return "DISTRIBUTED";
    default: return "UNKNOWN";
    }
}

const char* bus_type_to_string(bus_type_t type) {
    switch (type) {
    case BUS_SYSTEM: return "SYSTEM";
    case BUS_MEMORY: return "MEMORY";
    case BUS_IO: return "IO";
    case BUS_EXPANSION: return "EXPANSION";
    default: return "UNKNOWN";
    }
}

// ==================== 默认配置 ====================

bus_config_t bus_get_default_config(bus_type_t type) {
    bus_config_t config;
    
    switch (type) {
    case BUS_SYSTEM:
        config.data_width = 32;
        config.address_width = 32;
        config.clock_frequency = 100000000;  // 100MHz
        config.mode = BUS_MODE_SYNCHRONOUS;
        config.max_devices = 8;
        config.timeout = 1000;  // 1μs
        break;
        
    case BUS_MEMORY:
        config.data_width = 64;
        config.address_width = 32;
        config.clock_frequency = 200000000;  // 200MHz
        config.mode = BUS_MODE_SYNCHRONOUS;
        config.max_devices = 4;
        config.timeout = 500;   // 0.5μs
        break;
        
    case BUS_IO:
        config.data_width = 16;
        config.address_width = 16;
        config.clock_frequency = 50000000;   // 50MHz
        config.mode = BUS_MODE_ASYNCHRONOUS;
        config.max_devices = 12;
        config.timeout = 2000;  // 2μs
        break;
        
    case BUS_EXPANSION:
        config.data_width = 8;
        config.address_width = 16;
        config.clock_frequency = 25000000;   // 25MHz
        config.mode = BUS_MODE_ASYNCHRONOUS;
        config.max_devices = 16;
        config.timeout = 5000;  // 5μs
        break;
        
    default:
        memset(&config, 0, sizeof(config));
        break;
    }
    
    return config;
}

bus_error_t bus_validate_config(const bus_config_t *config) {
    if (!config) {
        return BUS_ERROR_NULL_POINTER;
    }
    
    // 验证数据宽度
    if (config->data_width == 0 || config->data_width > MAX_BUS_WIDTH) {
        return BUS_ERROR_INVALID_PARAM;
    }
    
    // 验证地址宽度
    if (config->address_width == 0 || config->address_width > 32) {
        return BUS_ERROR_INVALID_PARAM;
    }
    
    // 验证时钟频率
    if (config->clock_frequency == 0) {
        return BUS_ERROR_INVALID_PARAM;
    }
    
    // 验证最大设备数
    if (config->max_devices == 0 || config->max_devices > MAX_BUS_DEVICES) {
        return BUS_ERROR_INVALID_PARAM;
    }
    
    return BUS_SUCCESS;
} 