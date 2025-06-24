/* test_virtual_memory.c - 虚拟内存模拟器单元测试
 * 计算机组成原理模拟器 - 虚拟内存模拟器单元测试模块
 * create by: zw.duan
 */
#include "../../../simulation/virtual_memory_sim/virtual_memory.h"
#include "../../../simulation/common/test_framework.h"
#include "../../../simulation/common/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * 虚拟存储器模拟器单元测试程序
 * 
 * 测试覆盖：
 * 1. 虚拟存储器系统初始化和配置
 * 2. 地址翻译机制(分页式)
 * 3. 页面替换算法(FIFO、LRU、OPT、Clock)
 * 4. TLB管理和性能优化
 * 5. 段页式存储管理
 * 6. 408典型场景测试
 * 7. 性能统计和分析
 */

// ==================== 全局测试数据 ====================

static vm_system_t test_vm;
static bool test_vm_initialized = false;

// 408典型访问序列
static u32 test_sequence_408[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
static u32 test_sequence_length = sizeof(test_sequence_408) / sizeof(u32);

// Belady异常测试序列
static u32 belady_sequence[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
static u32 belady_length = sizeof(belady_sequence) / sizeof(u32);

// ==================== 辅助函数 ====================

static void cleanup_test_vm(void) {
    if (test_vm_initialized) {
        vm_system_destroy(&test_vm);
        test_vm_initialized = false;
    }
}

static bool init_test_vm(vm_management_mode_t mode, u32 frames, vm_replace_algorithm_t algorithm) {
    cleanup_test_vm();
    
    cpu_error_t result = vm_system_init(&test_vm, mode, frames, algorithm);
    if (result == CPU_SUCCESS) {
        test_vm_initialized = true;
        return true;
    }
    return false;
}

static void print_test_header(const char *test_name) {
    printf("\n" ANSI_COLOR_CYAN "=== %s ===" ANSI_COLOR_RESET "\n", test_name);
}

// ==================== 基础功能测试 ====================

bool test_vm_system_initialization(void) {
    TEST_START("虚拟存储器系统初始化测试");
    
    // 测试1: 正常初始化
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, VM_REPLACE_FIFO), "系统初始化成功");
    TEST_ASSERT(test_vm.initialized, "初始化标志正确");
    TEST_ASSERT(test_vm.mode == VM_MODE_PAGING, "管理模式设置正确");
    TEST_ASSERT(test_vm.total_frames == 3, "页框数量设置正确");
    TEST_ASSERT(test_vm.page_manager.replace_algorithm == VM_REPLACE_FIFO, "替换算法设置正确");
    
    // 测试2: 无效参数测试
    vm_system_t invalid_vm;
    TEST_ASSERT(vm_system_init(NULL, VM_MODE_PAGING, 3, VM_REPLACE_FIFO) == CPU_ERROR_INVALID_PARAM, 
               "空指针参数检测");
    TEST_ASSERT(vm_system_init(&invalid_vm, VM_MODE_PAGING, 0, VM_REPLACE_FIFO) == CPU_ERROR_INVALID_PARAM, 
               "零页框数检测");
    TEST_ASSERT(vm_system_init(&invalid_vm, VM_MODE_PAGING, 1000, VM_REPLACE_FIFO) == CPU_ERROR_INVALID_PARAM, 
               "页框数超限检测");
    
    // 测试3: 统计信息初始化
    TEST_ASSERT(test_vm.stats.total_accesses == 0, "访问计数初始化");
    TEST_ASSERT(test_vm.stats.page_hits == 0, "命中计数初始化");
    TEST_ASSERT(test_vm.stats.page_faults == 0, "缺页计数初始化");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_address_parsing_and_translation(void) {
    TEST_START("地址解析和翻译测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 4, VM_REPLACE_LRU), "系统初始化成功");
    
    // 测试1: 虚拟地址解析
    u32 test_addr = 0x12345678;
    vm_virtual_address_t vaddr = vm_parse_virtual_address(&test_vm, test_addr);
    
    TEST_ASSERT(vaddr.full_address == test_addr, "完整地址解析正确");
    TEST_ASSERT(vaddr.page_number == (test_addr >> 12), "页号解析正确");
    TEST_ASSERT(vaddr.page_offset == (test_addr & 0xFFF), "页内偏移解析正确");
    
    // 测试2: 首次地址翻译(应该缺页)
    u32 physical_addr;
    bool hit = vm_translate_address(&test_vm, test_addr, &physical_addr);
    TEST_ASSERT(!hit, "首次访问应该缺页");
    TEST_ASSERT(test_vm.stats.page_faults == 1, "缺页计数增加");
    TEST_ASSERT(test_vm.stats.total_accesses == 1, "总访问计数增加");
    
    // 测试3: 强制地址翻译(处理缺页)
    cpu_error_t result = vm_translate_address_force(&test_vm, test_addr, &physical_addr);
    TEST_ASSERT(result == CPU_SUCCESS, "强制翻译成功");
    
    u32 expected_frame = 0; // 第一个分配的页框
    u32 expected_physical = (expected_frame << 12) | vaddr.page_offset;
    TEST_ASSERT(physical_addr == expected_physical, "物理地址计算正确");
    
    // 测试4: 再次访问相同地址(应该命中)
    hit = vm_translate_address(&test_vm, test_addr, &physical_addr);
    TEST_ASSERT(hit, "第二次访问应该命中");
    TEST_ASSERT(test_vm.stats.page_hits >= 1, "命中计数增加");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_tlb_functionality(void) {
    TEST_START("TLB功能测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 4, VM_REPLACE_LRU), "系统初始化成功");
    
    // 设置一些页面映射
    for (u32 i = 0; i < 4; i++) {
        u32 vaddr = i * VM_PAGE_SIZE;
        u32 paddr;
        vm_translate_address_force(&test_vm, vaddr, &paddr);
    }
    
    // 测试1: TLB查找
    u32 frame_number;
    bool found = vm_tlb_lookup(&test_vm.tlb_manager, 0, &frame_number);
    TEST_ASSERT(found, "TLB查找命中");
    TEST_ASSERT(frame_number == 0, "TLB返回正确页框号");
    
    // 测试2: TLB更新
    cpu_error_t result = vm_tlb_update(&test_vm.tlb_manager, 100, 50);
    TEST_ASSERT(result == CPU_SUCCESS, "TLB更新成功");
    
    found = vm_tlb_lookup(&test_vm.tlb_manager, 100, &frame_number);
    TEST_ASSERT(found, "新添加项查找成功");
    TEST_ASSERT(frame_number == 50, "新项页框号正确");
    
    // 测试3: TLB无效化
    vm_tlb_invalidate(&test_vm.tlb_manager, 100);
    found = vm_tlb_lookup(&test_vm.tlb_manager, 100, &frame_number);
    TEST_ASSERT(!found, "无效化后查找失败");
    
    // 测试4: TLB全部清空
    vm_tlb_flush(&test_vm.tlb_manager);
    found = vm_tlb_lookup(&test_vm.tlb_manager, 0, &frame_number);
    TEST_ASSERT(!found, "清空后所有项无效");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_fifo_page_replacement(void) {
    TEST_START("FIFO页面替换算法测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, VM_REPLACE_FIFO), "系统初始化成功");
    TEST_ASSERT(vm_set_future_sequence(&test_vm, test_sequence_408, test_sequence_length) == CPU_SUCCESS, 
               "设置访问序列成功");
    
    u32 page_fault_count = 0;
    
    // 模拟访问序列: 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
    for (u32 i = 0; i < test_sequence_length; i++) {
        u32 vaddr = test_sequence_408[i] * VM_PAGE_SIZE;
        u32 paddr;
        
        u32 old_faults = test_vm.stats.page_faults;
        cpu_error_t result = vm_translate_address_force(&test_vm, vaddr, &paddr);
        TEST_ASSERT(result == CPU_SUCCESS, "地址翻译成功");
        
        if (test_vm.stats.page_faults > old_faults) {
            page_fault_count++;
        }
        
        test_vm.page_manager.sequence_position = i + 1;
    }
    
    // FIFO算法预期缺页次数(根据理论计算)
    printf("FIFO算法缺页次数: %u\n", page_fault_count);
    TEST_ASSERT(page_fault_count >= 9, "FIFO缺页次数在合理范围内");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_lru_page_replacement(void) {
    TEST_START("LRU页面替换算法测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, VM_REPLACE_LRU), "系统初始化成功");
    TEST_ASSERT(vm_set_future_sequence(&test_vm, test_sequence_408, test_sequence_length) == CPU_SUCCESS, 
               "设置访问序列成功");
    
    u32 page_fault_count = 0;
    
    // 模拟相同的访问序列
    for (u32 i = 0; i < test_sequence_length; i++) {
        u32 vaddr = test_sequence_408[i] * VM_PAGE_SIZE;
        u32 paddr;
        
        u32 old_faults = test_vm.stats.page_faults;
        cpu_error_t result = vm_translate_address_force(&test_vm, vaddr, &paddr);
        TEST_ASSERT(result == CPU_SUCCESS, "地址翻译成功");
        
        if (test_vm.stats.page_faults > old_faults) {
            page_fault_count++;
        }
        
        test_vm.page_manager.sequence_position = i + 1;
    }
    
    // LRU算法预期缺页次数应该比FIFO少
    printf("LRU算法缺页次数: %u\n", page_fault_count);
    TEST_ASSERT(page_fault_count >= 8 && page_fault_count <= 10, "LRU缺页次数在合理范围内");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_opt_page_replacement(void) {
    TEST_START("OPT页面替换算法测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, VM_REPLACE_OPT), "系统初始化成功");
    TEST_ASSERT(vm_set_future_sequence(&test_vm, test_sequence_408, test_sequence_length) == CPU_SUCCESS, 
               "设置访问序列成功");
    
    u32 page_fault_count = 0;
    
    // 模拟相同的访问序列
    for (u32 i = 0; i < test_sequence_length; i++) {
        u32 vaddr = test_sequence_408[i] * VM_PAGE_SIZE;
        u32 paddr;
        
        u32 old_faults = test_vm.stats.page_faults;
        cpu_error_t result = vm_translate_address_force(&test_vm, vaddr, &paddr);
        TEST_ASSERT(result == CPU_SUCCESS, "地址翻译成功");
        
        if (test_vm.stats.page_faults > old_faults) {
            page_fault_count++;
        }
        
        test_vm.page_manager.sequence_position = i + 1;
    }
    
    // OPT算法应该有最少的缺页次数
    printf("OPT算法缺页次数: %u\n", page_fault_count);
    TEST_ASSERT(page_fault_count >= 7 && page_fault_count <= 9, "OPT缺页次数最优");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_clock_page_replacement(void) {
    TEST_START("Clock页面替换算法测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, VM_REPLACE_CLOCK), "系统初始化成功");
    
    u32 page_fault_count = 0;
    
    // 模拟访问序列
    for (u32 i = 0; i < test_sequence_length; i++) {
        u32 vaddr = test_sequence_408[i] * VM_PAGE_SIZE;
        u32 paddr;
        
        u32 old_faults = test_vm.stats.page_faults;
        cpu_error_t result = vm_translate_address_force(&test_vm, vaddr, &paddr);
        TEST_ASSERT(result == CPU_SUCCESS, "地址翻译成功");
        
        if (test_vm.stats.page_faults > old_faults) {
            page_fault_count++;
        }
    }
    
    // Clock算法缺页次数应该介于FIFO和LRU之间
    printf("Clock算法缺页次数: %u\n", page_fault_count);
    TEST_ASSERT(page_fault_count >= 8 && page_fault_count <= 11, "Clock缺页次数在合理范围内");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_performance_calculation(void) {
    TEST_START("性能计算测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, VM_REPLACE_LRU), "系统初始化成功");
    
    // 模拟一些访问
    for (u32 i = 0; i < 10; i++) {
        u32 vaddr = (i % 5) * VM_PAGE_SIZE;  // 访问页面0-4
        u32 paddr;
        vm_translate_address_force(&test_vm, vaddr, &paddr);
    }
    
    // 计算性能指标
    vm_calculate_performance(&test_vm);
    
    // 验证性能指标
    TEST_ASSERT(test_vm.stats.page_hit_rate >= 0.0 && test_vm.stats.page_hit_rate <= 1.0, 
               "页面命中率在有效范围内");
    TEST_ASSERT(test_vm.stats.page_fault_rate >= 0.0 && test_vm.stats.page_fault_rate <= 1.0, 
               "缺页率在有效范围内");
    TEST_ASSERT(test_vm.stats.page_hit_rate + test_vm.stats.page_fault_rate == 1.0, 
               "命中率与缺页率之和为1");
    TEST_ASSERT(test_vm.stats.average_access_time > 0, "平均访问时间大于0");
    
    printf("页面命中率: %.2f%%\n", test_vm.stats.page_hit_rate * 100);
    printf("缺页率: %.2f%%\n", test_vm.stats.page_fault_rate * 100);
    printf("平均访问时间: %.2f ns\n", test_vm.stats.average_access_time);
    
    cleanup_test_vm();
    TEST_END();
}

bool test_408_exam_scenarios(void) {
    TEST_START("408典型场景测试");
    
    // 场景1: 地址翻译计算
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 4, VM_REPLACE_LRU), "系统初始化成功");
    
    u32 test_vaddr = 0x12345678;
    vm_virtual_address_t parsed = vm_parse_virtual_address(&test_vm, test_vaddr);
    
    u32 expected_page = test_vaddr >> 12;
    u32 expected_offset = test_vaddr & 0xFFF;
    
    TEST_ASSERT(parsed.page_number == expected_page, "虚拟页号计算正确");
    TEST_ASSERT(parsed.page_offset == expected_offset, "页内偏移计算正确");
    
    printf("408例题: 虚拟地址0x%08X\n", test_vaddr);
    printf("  虚拟页号: %u (0x%X)\n", parsed.page_number, parsed.page_number);
    printf("  页内偏移: %u (0x%X)\n", parsed.page_offset, parsed.page_offset);
    
    // 场景2: 页面替换算法比较
    cleanup_test_vm();
    
    printf("\n408算法比较:\n");
    printf("访问序列: ");
    for (u32 i = 0; i < test_sequence_length; i++) {
        printf("%u ", test_sequence_408[i]);
    }
    printf("\n");
    
    // 测试各种算法的缺页次数
    vm_replace_algorithm_t algorithms[] = {VM_REPLACE_FIFO, VM_REPLACE_LRU, VM_REPLACE_OPT};
    const char* alg_names[] = {"FIFO", "LRU", "OPT"};
    u32 fault_counts[3];
    
    for (int alg = 0; alg < 3; alg++) {
        TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 3, algorithms[alg]), "算法测试初始化成功");
        
        if (algorithms[alg] == VM_REPLACE_OPT) {
            vm_set_future_sequence(&test_vm, test_sequence_408, test_sequence_length);
        }
        
        u32 faults = 0;
        for (u32 i = 0; i < test_sequence_length; i++) {
            u32 vaddr = test_sequence_408[i] * VM_PAGE_SIZE;
            u32 paddr;
            
            u32 old_faults = test_vm.stats.page_faults;
            vm_translate_address_force(&test_vm, vaddr, &paddr);
            
            if (test_vm.stats.page_faults > old_faults) {
                faults++;
            }
            
            if (algorithms[alg] == VM_REPLACE_OPT) {
                test_vm.page_manager.sequence_position = i + 1;
            }
        }
        
        fault_counts[alg] = faults;
        printf("%s算法缺页次数: %u\n", alg_names[alg], faults);
        
        cleanup_test_vm();
    }
    
    // 验证算法性能关系: OPT <= LRU <= FIFO (理论上)
    TEST_ASSERT(fault_counts[2] <= fault_counts[1], "OPT性能不劣于LRU");
    
    // 场景3: TLB性能影响计算
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 4, VM_REPLACE_LRU), "TLB测试初始化成功");
    
    // 设置TLB命中率为95%的典型场景
    for (u32 i = 0; i < 20; i++) {
        u32 vaddr = (i % 4) * VM_PAGE_SIZE;  // 重复访问4个页面
        u32 paddr;
        vm_translate_address_force(&test_vm, vaddr, &paddr);
    }
    
    vm_calculate_performance(&test_vm);
    
    printf("\nTLB性能分析:\n");
    printf("TLB命中次数: %llu\n", test_vm.stats.tlb_hits);
    printf("TLB未命中次数: %llu\n", test_vm.stats.tlb_misses);
    printf("TLB命中率: %.2f%%\n", test_vm.stats.tlb_hit_rate * 100);
    
    TEST_ASSERT(test_vm.stats.tlb_hit_rate > 0.5, "TLB命中率合理");
    
    cleanup_test_vm();
    TEST_END();
}

bool test_memory_management_stress(void) {
    TEST_START("内存管理压力测试");
    
    TEST_ASSERT(init_test_vm(VM_MODE_PAGING, 4, VM_REPLACE_LRU), "系统初始化成功");
    
    // 大量随机访问测试
    srand(12345); // 固定种子确保可重现
    
    u32 access_count = 100;
    u32 page_range = 20;  // 访问页面范围超过页框数
    
    for (u32 i = 0; i < access_count; i++) {
        u32 page_num = rand() % page_range;
        u32 vaddr = page_num * VM_PAGE_SIZE + (rand() % VM_PAGE_SIZE);
        u32 paddr;
        
        cpu_error_t result = vm_translate_address_force(&test_vm, vaddr, &paddr);
        TEST_ASSERT(result == CPU_SUCCESS, "大量访问处理成功");
    }
    
    vm_calculate_performance(&test_vm);
    
    // 验证系统状态
    TEST_ASSERT(test_vm.stats.total_accesses == access_count, "访问计数正确");
    TEST_ASSERT(test_vm.stats.page_faults > 0, "发生了缺页");
    TEST_ASSERT(test_vm.stats.page_hits > 0, "发生了命中");
    TEST_ASSERT(test_vm.page_manager.allocated_frame_count <= test_vm.total_frames, 
               "已分配页框数不超过限制");
    
    printf("压力测试结果:\n");
    printf("总访问次数: %llu\n", test_vm.stats.total_accesses);
    printf("缺页次数: %llu\n", test_vm.stats.page_faults);
    printf("页面替换次数: %llu\n", test_vm.stats.page_replacements);
    printf("缺页率: %.2f%%\n", test_vm.stats.page_fault_rate * 100);
    
    cleanup_test_vm();
    TEST_END();
}

bool test_algorithm_comparison(void) {
    TEST_START("算法性能对比测试");
    
    // 创建更复杂的访问序列用于比较
    u32 complex_sequence[] = {1, 2, 3, 4, 5, 1, 2, 3, 6, 7, 1, 2, 8, 9, 10, 1, 2, 3};
    u32 complex_length = sizeof(complex_sequence) / sizeof(u32);
    
    printf("访问序列: ");
    for (u32 i = 0; i < complex_length; i++) {
        printf("%u ", complex_sequence[i]);
    }
    printf("\n\n");
    
    // 测试不同页框数量下的算法性能
    u32 frame_counts[] = {3, 4, 5};
    vm_replace_algorithm_t algorithms[] = {VM_REPLACE_FIFO, VM_REPLACE_LRU, VM_REPLACE_OPT, VM_REPLACE_CLOCK};
    const char* alg_names[] = {"FIFO", "LRU", "OPT", "Clock"};
    
    for (u32 f = 0; f < 3; f++) {
        printf("页框数 = %u:\n", frame_counts[f]);
        
        for (u32 a = 0; a < 4; a++) {
            TEST_ASSERT(init_test_vm(VM_MODE_PAGING, frame_counts[f], algorithms[a]), 
                       "算法测试初始化成功");
            
            if (algorithms[a] == VM_REPLACE_OPT) {
                vm_set_future_sequence(&test_vm, complex_sequence, complex_length);
            }
            
            u32 faults = 0;
            for (u32 i = 0; i < complex_length; i++) {
                u32 vaddr = complex_sequence[i] * VM_PAGE_SIZE;
                u32 paddr;
                
                u32 old_faults = test_vm.stats.page_faults;
                vm_translate_address_force(&test_vm, vaddr, &paddr);
                
                if (test_vm.stats.page_faults > old_faults) {
                    faults++;
                }
                
                if (algorithms[a] == VM_REPLACE_OPT) {
                    test_vm.page_manager.sequence_position = i + 1;
                }
            }
            
            printf("  %s: %u 次缺页\n", alg_names[a], faults);
            cleanup_test_vm();
        }
        printf("\n");
    }
    
    TEST_END();
}

// ==================== 主测试函数 ====================

int main(void) {
    printf("\n=== virtual_memory测试程序 ===\n");
    
    
    // 执行所有测试
    bool all_passed = true;
    
    all_passed &= test_vm_system_initialization();
    all_passed &= test_address_parsing_and_translation();
    all_passed &= test_tlb_functionality();
    all_passed &= test_fifo_page_replacement();
    all_passed &= test_lru_page_replacement();
    all_passed &= test_opt_page_replacement();
    all_passed &= test_clock_page_replacement();
    all_passed &= test_performance_calculation();
    all_passed &= test_408_exam_scenarios();
    all_passed &= test_memory_management_stress();
    all_passed &= test_algorithm_comparison();
    
    // 清理资源
    cleanup_test_vm();
    
    // 输出测试结果
    printf("\n=== virtual_memory测试结果统计 ===\n");

    
    if (all_passed) {
        printf(ANSI_COLOR_GREEN "🎉 所有测试通过！\n" ANSI_COLOR_RESET);
        printf("虚拟存储器模拟器功能完全正常，可用于408学习。\n");
        return 0;
    } else {
        printf(ANSI_COLOR_RED "❌ 部分测试失败\n" ANSI_COLOR_RESET);
        printf("请检查虚拟存储器模拟器实现。\n");
        return 1;
    }
} 