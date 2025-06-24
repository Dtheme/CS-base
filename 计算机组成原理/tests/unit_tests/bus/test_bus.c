/* test_bus.c - 总线系统模拟器测试程序
 * 计算机组成原理模拟器 - 总线模拟器测试模块
 * 遵循数据结构算法编码标准
 * create by: zw.duan
 */

#include "../../../simulation/bus_sim/bus.h"
#include "../../../simulation/common/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 总线系统模拟器测试程序
 * 
 * 本程序测试总线系统的各项功能
 */

// ==================== 测试辅助函数 ====================

static void print_test_header(const char *test_name) {
    printf("\n" ANSI_COLOR_CYAN "=== %s ===" ANSI_COLOR_RESET "\n", test_name);
}

static void setup_test_bus(bus_t *bus) {
    bus_config_t config = bus_get_default_config(BUS_SYSTEM);
    TEST_ASSERT(bus_init(bus, BUS_SYSTEM, &config) == BUS_SUCCESS, 
                "总线初始化失败");
    
    // 添加测试设备
    TEST_ASSERT(bus_add_device(bus, DEVICE_CPU, 0, "CPU") == BUS_SUCCESS,
                "添加CPU设备失败");
    TEST_ASSERT(bus_add_device(bus, DEVICE_MEMORY, 1, "MEMORY") == BUS_SUCCESS,
                "添加内存设备失败");
    TEST_ASSERT(bus_add_device(bus, DEVICE_IO_CONTROLLER, 2, "IO_CTRL") == BUS_SUCCESS,
                "添加I/O控制器失败");
    TEST_ASSERT(bus_add_device(bus, DEVICE_DMA_CONTROLLER, 3, "DMA_CTRL") == BUS_SUCCESS,
                "添加DMA控制器失败");
}

// ==================== 基本功能测试 ====================

bool test_bus_initialization(void) {
    TEST_START("总线初始化测试");
    
    bus_t bus;
    bus_config_t config;
    
    // 测试空指针
    TEST_ASSERT(bus_init(NULL, BUS_SYSTEM, &config) == BUS_ERROR_NULL_POINTER,
                "空指针检查失败");
    TEST_ASSERT(bus_init(&bus, BUS_SYSTEM, NULL) == BUS_ERROR_NULL_POINTER,
                "配置空指针检查失败");
    
    // 测试有效初始化
    config = bus_get_default_config(BUS_SYSTEM);
    TEST_ASSERT(bus_init(&bus, BUS_SYSTEM, &config) == BUS_SUCCESS,
                "总线初始化失败");
    
    // 验证初始化结果
    TEST_ASSERT(bus.type == BUS_SYSTEM, "总线类型设置错误");
    TEST_ASSERT(bus.device_count == 0, "设备计数初始化错误");
    TEST_ASSERT(bus.bus_busy == false, "总线忙状态初始化错误");
    TEST_ASSERT(bus.current_cycle == 0, "周期计数初始化错误");
    
    printf("✓ 总线初始化成功，类型: %s\n", bus_type_to_string(bus.type));
    
    TEST_END();
}

bool test_device_management(void) {
    TEST_START("设备管理测试");
    
    bus_t bus;
    setup_test_bus(&bus);
    
    // 测试设备添加
    TEST_ASSERT(bus.device_count == 4, "设备数量错误");
    
    // 测试设备查找
    bus_device_t *cpu = bus_get_device(&bus, 0);
    TEST_ASSERT(cpu != NULL, "CPU设备查找失败");
    TEST_ASSERT(cpu->type == DEVICE_CPU, "CPU设备类型错误");
    TEST_ASSERT(strcmp(cpu->name, "CPU") == 0, "CPU设备名称错误");
    
    // 测试设备优先级设置
    TEST_ASSERT(bus_set_device_priority(&bus, 0, 10) == BUS_SUCCESS,
                "设置设备优先级失败");
    TEST_ASSERT(cpu->priority == 10, "设备优先级设置错误");
    
    // 测试无效设备ID
    TEST_ASSERT(bus_get_device(&bus, 100) == NULL, "无效设备ID检查失败");
    TEST_ASSERT(bus_set_device_priority(&bus, 100, 5) == BUS_ERROR_NO_DEVICE,
                "无效设备ID优先级设置检查失败");
    
    printf("✓ 设备管理功能正常\n");
    bus_dump_all_devices(&bus);
    
    TEST_END();
}

// ==================== 仲裁机制测试 ====================

bool test_arbitration_algorithms(void) {
    TEST_START("仲裁算法测试");
    
    bus_t bus;
    setup_test_bus(&bus);
    
    // 设置不同优先级
    bus_set_device_priority(&bus, 0, 1);  // CPU: 高优先级
    bus_set_device_priority(&bus, 1, 3);  // MEMORY: 低优先级
    bus_set_device_priority(&bus, 2, 2);  // IO_CTRL: 中优先级
    bus_set_device_priority(&bus, 3, 4);  // DMA_CTRL: 最低优先级
    
    // 模拟设备请求
    bus_request(&bus, 1);  // MEMORY请求
    bus_request(&bus, 2);  // IO_CTRL请求
    bus_request(&bus, 3);  // DMA_CTRL请求
    
    // 测试链式查询仲裁
    bus_set_arbitration_method(&bus, ARB_CHAIN_POLLING);
    u8 winner = bus_arbitrate(&bus);
    TEST_ASSERT(winner == 1, "链式查询仲裁结果错误");
    printf("✓ 链式查询仲裁: 设备%d获胜\n", winner);
    
    // 重置请求状态
    for (u8 i = 0; i < bus.device_count; i++) {
        bus_release(&bus, i);
    }
    
    // 重新请求
    bus_request(&bus, 1);
    bus_request(&bus, 2);
    bus_request(&bus, 3);
    
    // 测试优先级仲裁
    bus_set_arbitration_method(&bus, ARB_PRIORITY_BASED);
    winner = bus_arbitrate(&bus);
    TEST_ASSERT(winner == 1 || winner == 2, "优先级仲裁结果错误");
    printf("✓ 优先级仲裁: 设备%d获胜 (优先级:%d)\n", 
           winner, bus.devices[winner].priority);
    
    // 重置并测试轮询仲裁
    for (u8 i = 0; i < bus.device_count; i++) {
        bus_release(&bus, i);
    }
    
    bus_set_arbitration_method(&bus, ARB_ROUND_ROBIN);
    
    // 连续仲裁测试
    printf("✓ 轮询仲裁测试:\n");
    for (int round = 0; round < 3; round++) {
        // 所有设备请求
        for (u8 i = 0; i < bus.device_count; i++) {
            bus_request(&bus, i);
        }
        
        winner = bus_arbitrate(&bus);
        printf("  轮次%d: 设备%d获胜\n", round + 1, winner);
        
        // 清除获胜者的请求
        bus_release(&bus, winner);
    }
    
    TEST_END();
}

// ==================== 总线操作测试 ====================

bool test_bus_operations(void) {
    TEST_START("总线操作测试");
    
    bus_t bus;
    setup_test_bus(&bus);
    
    u32 test_address = 0x1000;
    u32 test_data = 0xDEADBEEF;
    u32 read_data = 0;
    
    // 测试写操作
    bus_error_t result = bus_write(&bus, 0, test_address, test_data);
    TEST_ASSERT(result == BUS_SUCCESS, "写操作失败");
    printf("✓ 写操作成功: 地址=0x%X, 数据=0x%X\n", test_address, test_data);
    
    // 测试读操作
    result = bus_read(&bus, 0, test_address, &read_data);
    TEST_ASSERT(result == BUS_SUCCESS, "读操作失败");
    printf("✓ 读操作成功: 地址=0x%X, 数据=0x%X\n", test_address, read_data);
    
    // 测试无效设备操作
    result = bus_write(&bus, 100, test_address, test_data);
    TEST_ASSERT(result == BUS_ERROR_NO_DEVICE, "无效设备检查失败");
    
    // 测试空指针
    result = bus_read(&bus, 0, test_address, NULL);
    TEST_ASSERT(result == BUS_ERROR_NULL_POINTER, "空指针检查失败");
    
    // 打印操作统计
    printf("✓ 操作统计:\n");
    printf("  总操作次数: %llu\n", bus.stats.total_operations);
    printf("  读操作次数: %llu\n", bus.stats.read_operations);
    printf("  写操作次数: %llu\n", bus.stats.write_operations);
    
    TEST_END();
}

// ==================== 性能分析测试 ====================

bool test_performance_analysis(void) {
    TEST_START("性能分析测试");
    
    bus_t bus;
    setup_test_bus(&bus);
    
    // 执行一系列操作
    for (u32 i = 0; i < 100; i++) {
        u8 device_id = i % bus.device_count;
        u32 address = 0x1000 + i * 4;
        u32 data = 0xAAAA0000 + i;
        
        if (i % 2 == 0) {
            bus_write(&bus, device_id, address, data);
        } else {
            u32 read_data;
            bus_read(&bus, device_id, address, &read_data);
        }
    }
    
    // 分析性能
    double bandwidth = bus_calculate_bandwidth(&bus);
    double utilization = bus_calculate_utilization(&bus);
    double efficiency = bus_calculate_efficiency(&bus);
    u32 avg_wait_time = bus_calculate_average_wait_time(&bus);
    
    printf("✓ 性能分析结果:\n");
    printf("  理论带宽: %.2f MB/s\n", bandwidth / 1e6);
    printf("  总线利用率: %.2f%%\n", utilization);
    printf("  传输效率: %.2f%%\n", efficiency);
    printf("  平均等待时间: %d 周期\n", avg_wait_time);
    
    // 测试性能计算函数
    TEST_ASSERT(bandwidth > 0, "带宽计算错误");
    TEST_ASSERT(utilization >= 0 && utilization <= 100, "利用率计算错误");
    
    TEST_END();
}

// ==================== 时序模拟测试 ====================

bool test_timing_simulation(void) {
    TEST_START("时序模拟测试");
    
    bus_t bus;
    setup_test_bus(&bus);
    
    u32 initial_cycle = bus.current_cycle;
    
    // 模拟100个时钟周期
    bus_error_t result = bus_simulate_operation(&bus, 100);
    TEST_ASSERT(result == BUS_SUCCESS, "时序模拟失败");
    TEST_ASSERT(bus.current_cycle == initial_cycle + 100, "周期计数错误");
    
    printf("✓ 时序模拟成功: %d -> %d 周期\n", initial_cycle, bus.current_cycle);
    
    // 测试单个时钟周期
    u32 before_tick = bus.current_cycle;
    bus_clock_tick(&bus);
    TEST_ASSERT(bus.current_cycle == before_tick + 1, "时钟周期计数错误");
    
    printf("✓ 时钟周期测试成功\n");
    
    TEST_END();
}

// ==================== 错误处理测试 ====================

bool test_error_handling(void) {
    TEST_START("错误处理测试");
    
    bus_t bus;
    bus_config_t config;
    
    // 测试无效配置
    memset(&config, 0, sizeof(config));
    TEST_ASSERT(bus_init(&bus, BUS_SYSTEM, &config) != BUS_SUCCESS,
                "无效配置检查失败");
    
    // 测试配置验证
    config.data_width = 0;  // 无效宽度
    TEST_ASSERT(bus_validate_config(&config) != BUS_SUCCESS,
                "配置验证失败");
    
    config.data_width = 128;  // 超出范围
    TEST_ASSERT(bus_validate_config(&config) != BUS_SUCCESS,
                "配置范围检查失败");
    
    // 测试有效配置
    config = bus_get_default_config(BUS_SYSTEM);
    TEST_ASSERT(bus_validate_config(&config) == BUS_SUCCESS,
                "有效配置验证失败");
    
    printf("✓ 错误处理测试通过\n");
    
    TEST_END();
}

// ==================== 综合测试 ====================

bool test_comprehensive_scenario(void) {
    TEST_START("综合场景测试");
    
    bus_t system_bus;
    setup_test_bus(&system_bus);
    
    printf("=== 模拟计算机系统运行场景 ===\n");
    
    // 1. CPU取指令
    printf("1. CPU取指令...\n");
    u32 instruction;
    bus_read(&system_bus, 0, 0x00001000, &instruction);
    
    // 2. CPU访问内存读取数据
    printf("2. CPU访问内存读取数据...\n");
    u32 data;
    bus_read(&system_bus, 0, 0x00002000, &data);
    
    // 3. DMA控制器请求传输数据
    printf("3. DMA控制器请求传输...\n");
    bus_write(&system_bus, 3, 0x00003000, 0x12345678);
    
    // 4. I/O控制器处理中断
    printf("4. I/O控制器处理中断...\n");
    bus_write(&system_bus, 2, 0x00004000, 0x87654321);
    
    // 5. CPU写回结果
    printf("5. CPU写回结果...\n");
    bus_write(&system_bus, 0, 0x00005000, 0xFEDCBA98);
    
    // 打印最终状态
    printf("\n=== 最终系统状态 ===\n");
    bus_print_status(&system_bus);
    bus_print_statistics(&system_bus);
    
    // 验证系统正常运行
    TEST_ASSERT(system_bus.stats.total_operations == 5, "操作计数错误");
    TEST_ASSERT(system_bus.stats.read_operations == 2, "读操作计数错误");
    TEST_ASSERT(system_bus.stats.write_operations == 3, "写操作计数错误");
    
    printf("✓ 综合场景测试成功完成\n");
    
    TEST_END();
}

// ==================== 408真题模拟 ====================

bool test_408_exam_scenarios(void) {
    TEST_START("408真题模拟");
    
    printf("\n【真题1】总线性能计算\n");
    
    // 创建题目中的总线配置
    bus_t bus;
    bus_config_t config;
    config.data_width = 32;           // 32位数据总线
    config.address_width = 32;        // 32位地址总线
    config.clock_frequency = 100000000; // 100MHz
    config.mode = BUS_MODE_SYNCHRONOUS;
    config.max_devices = 4;
    config.timeout = 1000;
    
    bus_init(&bus, BUS_SYSTEM, &config);
    
    // 计算理论带宽
    double bandwidth = bus_calculate_bandwidth(&bus);
    printf("理论带宽: %.0f MB/s\n", bandwidth / 1e6);
    TEST_ASSERT(abs((int)(bandwidth / 1e6) - 400) < 1, "带宽计算错误");
    
    printf("\n【真题2】仲裁时间分析\n");
    
    // 添加4个设备，模拟链式仲裁
    bus_add_device(&bus, DEVICE_CPU, 0, "CPU");
    bus_add_device(&bus, DEVICE_MEMORY, 1, "MEM");
    bus_add_device(&bus, DEVICE_IO_CONTROLLER, 2, "IO1");
    bus_add_device(&bus, DEVICE_DMA_CONTROLLER, 3, "DMA");
    
    // 设置链式仲裁
    bus_set_arbitration_method(&bus, ARB_CHAIN_POLLING);
    
    // 模拟最低优先级设备请求（最坏情况）
    bus_request(&bus, 3);  // DMA设备请求
    u8 winner = bus_arbitrate(&bus);
    
    printf("链式仲裁获胜设备: %d (%s)\n", winner, bus.devices[winner].name);
    printf("仲裁次数: %d\n", bus.arbitrator.arbitration_count);
    
    printf("\n【真题3】总线利用率分析\n");
    
    // 执行一系列操作来产生统计数据
    for (int i = 0; i < 50; i++) {
        u32 addr = 0x1000 + i * 4;
        u32 data = 0xA000 + i;
        bus_write(&bus, i % 4, addr, data);
    }
    
    double utilization = bus_calculate_utilization(&bus);
    printf("总线利用率: %.2f%%\n", utilization);
    printf("总操作次数: %llu\n", bus.stats.total_operations);
    printf("忙碌周期: %llu\n", bus.stats.busy_cycles);
    printf("总周期: %llu\n", bus.stats.total_cycles);
    
    TEST_ASSERT(utilization >= 0 && utilization <= 100, "利用率计算范围错误");
    
    printf("✓ 408真题模拟完成\n");
    
    TEST_END();
}

// ==================== 主测试函数 ====================

int main(void) {
    printf(ANSI_COLOR_GREEN);
    printf("\n=== bus测试结果统计 ===\n");
    printf(ANSI_COLOR_RESET);
    
    // 初始化测试框架
    test_init("总线系统模拟器测试");
    
    // 执行所有测试
    test_bus_initialization();
    test_device_management();
    test_arbitration_algorithms();
    test_bus_operations();
    test_performance_analysis();
    test_timing_simulation();
    test_error_handling();
    test_comprehensive_scenario();
    test_408_exam_scenarios();
    
    // 输出测试结果
    test_summary();
    
    printf(ANSI_COLOR_YELLOW);
    printf("\n=== 测试完成 ===\n");
    printf("总线模拟器功能验证完毕！\n");
    printf("可以继续进行下一章节的开发。\n");
    printf(ANSI_COLOR_RESET);
    
    return test_get_exit_code();
} 