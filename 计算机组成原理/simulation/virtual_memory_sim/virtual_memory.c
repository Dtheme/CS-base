#include "virtual_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/**
 * 虚拟存储器管理模拟器核心实现
 * 
 * 实现架构：
 * 本模块实现了完整的虚拟存储器管理系统，包括地址翻译机制、页面替换算法、
 * TLB管理、缺页处理和性能统计等核心功能。
 * 
 * 核心功能：
 * 1. 多级地址翻译 - TLB、页表、物理内存的三级查找机制
 * 2. 页面管理系统 - 物理页框分配、回收和状态维护
 * 3. 多种替换算法 - FIFO、LRU、OPT、Clock算法的完整实现
 * 4. TLB缓存管理 - 高速地址翻译缓存的维护和替换
 * 5. 性能监控系统 - 命中率、缺页率等关键指标统计
 * 
 * 技术特点：
 * - 模块化设计：各个功能模块相对独立，便于扩展
 * - 算法可配置：支持运行时切换不同的页面替换算法
 * - 性能优化：TLB缓存有效减少页表访问开销
 * - 统计完善：提供详细的内存访问行为分析数据
 */

// ==================== 初始化和销毁函数 ====================

cpu_error_t vm_system_init(vm_system_t *vm, vm_management_mode_t mode, 
                          u32 total_frames, vm_replace_algorithm_t algorithm) {
    // 参数有效性检查
    // 原理：确保传入参数在合理范围内，防止系统异常
    if (!vm || total_frames == 0 || total_frames > VM_MAX_FRAMES) {
        return CPU_ERROR_INVALID_PARAM;
    }

    // 结构体初始化
    // 原理：使用memset快速清零整个结构体，确保所有字段都处于已知状态
    // 优势：避免遗留数据造成的不确定行为
    memset(vm, 0, sizeof(vm_system_t));

    // 基本系统配置
    // 原理：设置虚拟存储器系统的基本参数
    vm->mode = mode;                        // 存储管理模式
    vm->total_frames = total_frames;        // 物理页框总数
    vm->page_size = VM_PAGE_SIZE;          // 页面大小：4KB
    vm->virtual_address_bits = 32;         // 虚拟地址空间：4GB
    vm->physical_address_bits = 32;        // 物理地址空间：4GB

    // 访问时间配置
    // 原理：设置各级存储器的访问时间，用于性能计算
    // 参数基于典型的现代计算机系统配置
    vm->memory_access_time = 100;          // 内存访问时间：100纳秒
    vm->tlb_access_time = 1;               // TLB访问时间：1纳秒
    vm->page_fault_overhead = 1000;        // 缺页处理开销：1000微秒

    // 页面管理器初始化
    // 原理：设置页面替换算法和相关控制变量
    vm->page_manager.replace_algorithm = algorithm;     // 设置页面替换算法
    vm->page_manager.clock_pointer = 0;                 // Clock算法指针初始化
    vm->page_manager.current_time = 0;                  // 系统时间计数器
    vm->page_manager.allocated_frame_count = 0;         // 已分配页框数
    vm->page_manager.free_frame_count = total_frames;   // 空闲页框数
    vm->page_manager.future_access_sequence = NULL;     // OPT算法访问序列
    vm->page_manager.sequence_length = 0;               // 访问序列长度
    vm->page_manager.sequence_position = 0;             // 当前序列位置

    // 空闲页框列表初始化
    // 原理：将所有物理页框标记为空闲状态，按顺序编号
    // 实现：使用连续编号便于管理和分配
    for (u32 i = 0; i < total_frames; i++) {
        vm->page_manager.free_frames[i] = i;
    }

    // 页表初始化
    // 原理：将所有页表项设置为无效状态，清零所有统计信息
    // 作用：确保虚拟页面在首次访问时触发缺页中断
    for (u32 i = 0; i < VM_MAX_PAGES; i++) {
        vm->page_manager.page_table[i].frame_number = INVALID_FRAME_NUMBER;  // 无效页框号
        vm->page_manager.page_table[i].valid = 0;                           // 无效标志
        vm->page_manager.page_table[i].modified = 0;                        // 清除修改位
        vm->page_manager.page_table[i].referenced = 0;                      // 清除引用位
        vm->page_manager.page_table[i].protect = 0;                         // 清除保护位
        vm->page_manager.page_table[i].load_time = 0;                       // 清除加载时间
        vm->page_manager.page_table[i].last_access_time = 0;                // 清除访问时间
        vm->page_manager.page_table[i].access_count = 0;                    // 清除访问计数
    }

    // 初始化TLB管理器
    vm->tlb_manager.entry_count = 0;
    vm->tlb_manager.replace_algorithm = VM_REPLACE_LRU;
    vm->tlb_manager.current_time = 0;
    
    for (u32 i = 0; i < VM_TLB_SIZE; i++) {
        vm->tlb_manager.entries[i].virtual_page_number = INVALID_PAGE_NUMBER;
        vm->tlb_manager.entries[i].physical_frame_number = INVALID_FRAME_NUMBER;
        vm->tlb_manager.entries[i].valid = 0;
        vm->tlb_manager.entries[i].last_access_time = 0;
    }

    // 初始化统计信息
    vm_reset_statistics(vm);

    vm->initialized = true;
    return CPU_SUCCESS;
}

void vm_system_destroy(vm_system_t *vm) {
    if (vm && vm->initialized) {
        if (vm->page_manager.future_access_sequence) {
            free(vm->page_manager.future_access_sequence);
        }
        memset(vm, 0, sizeof(vm_system_t));
    }
}

void vm_reset_statistics(vm_system_t *vm) {
    if (!vm) return;

    memset(&vm->stats, 0, sizeof(vm_statistics_t));
}

cpu_error_t vm_set_future_sequence(vm_system_t *vm, u32 *sequence, u32 length) {
    if (!vm || !sequence || length == 0) {
        return CPU_ERROR_INVALID_PARAM;
    }

    // 释放旧序列
    if (vm->page_manager.future_access_sequence) {
        free(vm->page_manager.future_access_sequence);
    }

    // 分配新序列
    vm->page_manager.future_access_sequence = malloc(length * sizeof(u32));
    if (!vm->page_manager.future_access_sequence) {
        return CPU_ERROR_OUT_OF_MEMORY;
    }

    memcpy(vm->page_manager.future_access_sequence, sequence, length * sizeof(u32));
    vm->page_manager.sequence_length = length;
    vm->page_manager.sequence_position = 0;

    return CPU_SUCCESS;
}

// ==================== 地址解析和翻译 ====================

vm_virtual_address_t vm_parse_virtual_address(const vm_system_t *vm, u32 virtual_addr) {
    vm_virtual_address_t result;
    
    if (!vm) {
        memset(&result, 0, sizeof(result));
        return result;
    }

    result.full_address = virtual_addr;
    
    // 假设4KB页面大小，12位偏移
    u32 offset_bits = 12;  // log2(4096)
    u32 offset_mask = (1U << offset_bits) - 1;
    
    result.page_offset = virtual_addr & offset_mask;
    result.page_number = virtual_addr >> offset_bits;

    return result;
}

/**
 * 内部地址翻译函数
 * 
 * 设计原理：
 * 这是地址翻译的核心实现，采用分层查找策略提高翻译效率。
 * 通过update_stats参数控制是否更新统计信息，避免重复统计。
 * 
 * 翻译算法：
 * 1. TLB快表查找 - O(1)时间复杂度
 * 2. 页表查找 - O(1)时间复杂度 
 * 3. 物理地址组合 - 页框号 + 页内偏移
 * 
 * 性能优化：
 * - TLB命中时避免页表访问
 * - 并行更新访问统计信息
 * - 地址组合使用位运算提高效率
 */
static bool vm_translate_address_internal(vm_system_t *vm, u32 virtual_addr, u32 *physical_addr, bool update_stats) {
    // 参数有效性检查
    if (!vm || !physical_addr || !vm->initialized) {
        return false;
    }

    // 统计信息更新
    // 原理：记录每次内存访问，更新时间戳用于LRU算法
    if (update_stats) {
        vm->stats.total_accesses++;           // 总访问次数计数
        vm->page_manager.current_time++;      // 页面管理器时间推进
        vm->tlb_manager.current_time++;       // TLB管理器时间推进
    }

    // 虚拟地址解析
    // 原理：将32位虚拟地址分解为20位页号和12位偏移
    vm_virtual_address_t vaddr = vm_parse_virtual_address(vm, virtual_addr);
    u32 virtual_page_number = vaddr.page_number;
    u32 frame_number;

    // 第一级查找：TLB快表查找
    // 原理：TLB是页表的高速缓存，命中率通常在90%以上
    // 优势：避免访问主存中的页表，显著提高翻译速度
    if (vm_tlb_lookup(&vm->tlb_manager, virtual_page_number, &frame_number)) {
        // TLB命中统计
        if (update_stats) {
            vm->stats.tlb_hits++;             // TLB命中计数
            vm->stats.page_hits++;            // 页面命中计数
        }
        
        // 更新页表访问信息
        // 原理：即使TLB命中，也需要维护页表项的访问统计
        // 作用：为页面替换算法提供准确的访问历史
        if (virtual_page_number < VM_MAX_PAGES) {
            vm->page_manager.page_table[virtual_page_number].referenced = 1;     // 设置引用位
            vm->page_manager.page_table[virtual_page_number].last_access_time = vm->page_manager.current_time;  // 更新访问时间
            vm->page_manager.page_table[virtual_page_number].access_count++;     // 增加访问计数
        }

        // 物理地址合成
        // 原理：物理地址 = 页框号（高20位）+ 页内偏移（低12位）
        // 实现：页框号左移12位，与偏移进行或运算
        *physical_addr = (frame_number << 12) | vaddr.page_offset;
        return true;
    }

    if (update_stats) {
        vm->stats.tlb_misses++;
    }

    // TLB未命中，查找页表
    if (virtual_page_number >= VM_MAX_PAGES) {
        return false;
    }

    vm_page_table_entry_t *pte = &vm->page_manager.page_table[virtual_page_number];
    
    if (pte->valid) {
        // 页面在内存中
        if (update_stats) {
            vm->stats.page_hits++;
        }
        
        frame_number = pte->frame_number;
        pte->referenced = 1;
        pte->last_access_time = vm->page_manager.current_time;
        pte->access_count++;

        // 更新TLB
        vm_tlb_update(&vm->tlb_manager, virtual_page_number, frame_number);

        *physical_addr = (frame_number << 12) | vaddr.page_offset;
        return true;
    }

    // 页面不在内存中，发生缺页
    if (update_stats) {
        vm->stats.page_faults++;
    }
    return false;
}

bool vm_translate_address(vm_system_t *vm, u32 virtual_addr, u32 *physical_addr) {
    return vm_translate_address_internal(vm, virtual_addr, physical_addr, true);
}

cpu_error_t vm_translate_address_force(vm_system_t *vm, u32 virtual_addr, u32 *physical_addr) {
    if (!vm || !physical_addr) {
        return CPU_ERROR_INVALID_PARAM;
    }

    if (vm_translate_address(vm, virtual_addr, physical_addr)) {
        return CPU_SUCCESS;
    }

    // 处理缺页
    vm_virtual_address_t vaddr = vm_parse_virtual_address(vm, virtual_addr);
    u32 virtual_page_number = vaddr.page_number;

    cpu_error_t result = vm_handle_page_fault(vm, virtual_page_number);
    if (result != CPU_SUCCESS) {
        return result;
    }

    // 重新翻译地址，不更新统计信息（因为第一次调用已经更新了）
    if (vm_translate_address_internal(vm, virtual_addr, physical_addr, false)) {
        return CPU_SUCCESS;
    }

    return CPU_ERROR_UNKNOWN;
}

// ==================== TLB管理 ====================

bool vm_tlb_lookup(vm_tlb_manager_t *tlb, u32 virtual_page_number, u32 *frame_number) {
    if (!tlb || !frame_number) {
        return false;
    }

    for (u32 i = 0; i < tlb->entry_count; i++) {
        if (tlb->entries[i].valid && 
            tlb->entries[i].virtual_page_number == virtual_page_number) {
            
            *frame_number = tlb->entries[i].physical_frame_number;
            tlb->entries[i].last_access_time = tlb->current_time;
            return true;
        }
    }

    return false;
}

cpu_error_t vm_tlb_update(vm_tlb_manager_t *tlb, u32 virtual_page_number, u32 frame_number) {
    if (!tlb) {
        return CPU_ERROR_INVALID_PARAM;
    }

    // 检查是否已存在
    for (u32 i = 0; i < tlb->entry_count; i++) {
        if (tlb->entries[i].valid && 
            tlb->entries[i].virtual_page_number == virtual_page_number) {
            
            tlb->entries[i].physical_frame_number = frame_number;
            tlb->entries[i].last_access_time = tlb->current_time;
            return CPU_SUCCESS;
        }
    }

    // 添加新项
    u32 index;
    if (tlb->entry_count < VM_TLB_SIZE) {
        index = tlb->entry_count++;
    } else {
        // TLB满，需要替换
        index = 0;
        u32 oldest_time = tlb->entries[0].last_access_time;
        
        for (u32 i = 1; i < VM_TLB_SIZE; i++) {
            if (tlb->entries[i].last_access_time < oldest_time) {
                oldest_time = tlb->entries[i].last_access_time;
                index = i;
            }
        }
    }

    tlb->entries[index].virtual_page_number = virtual_page_number;
    tlb->entries[index].physical_frame_number = frame_number;
    tlb->entries[index].valid = 1;
    tlb->entries[index].last_access_time = tlb->current_time;

    return CPU_SUCCESS;
}

void vm_tlb_invalidate(vm_tlb_manager_t *tlb, u32 virtual_page_number) {
    if (!tlb) return;

    for (u32 i = 0; i < tlb->entry_count; i++) {
        if (tlb->entries[i].valid && 
            tlb->entries[i].virtual_page_number == virtual_page_number) {
            tlb->entries[i].valid = 0;
            break;
        }
    }
}

void vm_tlb_flush(vm_tlb_manager_t *tlb) {
    if (!tlb) return;

    for (u32 i = 0; i < VM_TLB_SIZE; i++) {
        tlb->entries[i].valid = 0;
    }
    tlb->entry_count = 0;
}

// ==================== 页框管理 ====================

u32 vm_allocate_frame(vm_page_manager_t *page_mgr) {
    if (!page_mgr || page_mgr->free_frame_count == 0) {
        return INVALID_FRAME_NUMBER;
    }

    u32 frame = page_mgr->free_frames[0];
    
    // 移动其他空闲页框
    for (u32 i = 0; i < page_mgr->free_frame_count - 1; i++) {
        page_mgr->free_frames[i] = page_mgr->free_frames[i + 1];
    }
    
    page_mgr->free_frame_count--;
    page_mgr->allocated_frames[page_mgr->allocated_frame_count++] = frame;

    return frame;
}

void vm_free_frame(vm_page_manager_t *page_mgr, u32 frame_number) {
    if (!page_mgr) return;

    // 从已分配列表中移除
    for (u32 i = 0; i < page_mgr->allocated_frame_count; i++) {
        if (page_mgr->allocated_frames[i] == frame_number) {
            // 移动其他项
            for (u32 j = i; j < page_mgr->allocated_frame_count - 1; j++) {
                page_mgr->allocated_frames[j] = page_mgr->allocated_frames[j + 1];
            }
            page_mgr->allocated_frame_count--;
            break;
        }
    }

    // 添加到空闲列表
    page_mgr->free_frames[page_mgr->free_frame_count++] = frame_number;
}

cpu_error_t vm_handle_page_fault(vm_system_t *vm, u32 virtual_page_number) {
    if (!vm || virtual_page_number >= VM_MAX_PAGES) {
        return CPU_ERROR_INVALID_PARAM;
    }

    u32 frame_number = vm_allocate_frame(&vm->page_manager);
    
    if (frame_number == INVALID_FRAME_NUMBER) {
        // 需要页面替换
        frame_number = vm_replace_page(vm, virtual_page_number);
        if (frame_number == INVALID_FRAME_NUMBER) {
            return CPU_ERROR_OUT_OF_MEMORY;
        }
    }

    // 设置页表项
    vm_page_table_entry_t *pte = &vm->page_manager.page_table[virtual_page_number];
    pte->frame_number = frame_number;
    pte->valid = 1;
    pte->modified = 0;
    pte->referenced = 1;
    pte->protect = 0x7;  // 读写执行权限
    pte->load_time = vm->page_manager.current_time;
    pte->last_access_time = vm->page_manager.current_time;
    pte->access_count = 1;

    // 更新TLB
    vm_tlb_update(&vm->tlb_manager, virtual_page_number, frame_number);

    return CPU_SUCCESS;
}

u32 vm_replace_page(vm_system_t *vm, u32 new_page_number) {
    if (!vm) return INVALID_FRAME_NUMBER;

    vm->stats.page_replacements++;

    switch (vm->page_manager.replace_algorithm) {
        case VM_REPLACE_FIFO:
            return vm_replace_fifo(vm);
        case VM_REPLACE_LRU:
            return vm_replace_lru(vm);
        case VM_REPLACE_OPT:
            return vm_replace_opt(vm);
        case VM_REPLACE_CLOCK:
            return vm_replace_clock(vm);
        default:
            return vm_replace_fifo(vm);
    }
}

// ==================== 页面替换算法实现 ====================

u32 vm_replace_fifo(vm_system_t *vm) {
    if (!vm || vm->page_manager.allocated_frame_count == 0) {
        return INVALID_FRAME_NUMBER;
    }

    u32 oldest_time = UINT32_MAX;
    u32 victim_page = INVALID_PAGE_NUMBER;
    u32 victim_frame = INVALID_FRAME_NUMBER;

    // 找到加载时间最早的页面
    for (u32 i = 0; i < VM_MAX_PAGES; i++) {
        vm_page_table_entry_t *pte = &vm->page_manager.page_table[i];
        if (pte->valid && pte->load_time < oldest_time) {
            oldest_time = pte->load_time;
            victim_page = i;
            victim_frame = pte->frame_number;
        }
    }

    if (victim_page != INVALID_PAGE_NUMBER) {
        // 从页表中移除
        vm->page_manager.page_table[victim_page].valid = 0;
        
        // 从TLB中移除
        vm_tlb_invalidate(&vm->tlb_manager, victim_page);
        
        return victim_frame;
    }

    return INVALID_FRAME_NUMBER;
}

u32 vm_replace_lru(vm_system_t *vm) {
    // 参数有效性检查
    if (!vm || vm->page_manager.allocated_frame_count == 0) {
        return INVALID_FRAME_NUMBER;
    }

    // LRU算法实现变量
    u32 oldest_time = UINT32_MAX;              // 最旧的访问时间
    u32 victim_page = INVALID_PAGE_NUMBER;     // 被替换的页面
    u32 victim_frame = INVALID_FRAME_NUMBER;   // 被替换的页框

    // LRU页面搜索算法
    // 原理：遍历所有有效页面，找到最久未访问的页面
    // 时间复杂度：O(n)，其中n为有效页面数
    // 优化空间：可使用双向链表或优先级队列优化到O(log n)
    for (u32 i = 0; i < VM_MAX_PAGES; i++) {
        vm_page_table_entry_t *pte = &vm->page_manager.page_table[i];
        
        // 检查页面有效性和访问时间
        if (pte->valid && pte->last_access_time < oldest_time) {
            oldest_time = pte->last_access_time;    // 更新最旧时间
            victim_page = i;                        // 记录候选页面
            victim_frame = pte->frame_number;       // 记录对应页框
        }
    }

    // 执行页面替换操作
    if (victim_page != INVALID_PAGE_NUMBER) {
        // 从页表中移除被替换页面
        // 原理：将页表项标记为无效，释放物理页框
        vm->page_manager.page_table[victim_page].valid = 0;
        
        // 从TLB中移除被替换页面
        // 原理：保持TLB与页表的一致性，避免无效翻译
        vm_tlb_invalidate(&vm->tlb_manager, victim_page);
        
        return victim_frame;
    }

    return INVALID_FRAME_NUMBER;
}

u32 vm_replace_opt(vm_system_t *vm) {
    if (!vm || vm->page_manager.allocated_frame_count == 0) {
        return INVALID_FRAME_NUMBER;
    }

    // 如果没有未来访问序列，回退到LRU
    if (!vm->page_manager.future_access_sequence) {
        return vm_replace_lru(vm);
    }

    u32 farthest_distance = 0;
    u32 victim_page = INVALID_PAGE_NUMBER;
    u32 victim_frame = INVALID_FRAME_NUMBER;

    // 对每个在内存中的页面，查找下次访问的距离
    for (u32 i = 0; i < VM_MAX_PAGES; i++) {
        vm_page_table_entry_t *pte = &vm->page_manager.page_table[i];
        if (!pte->valid) continue;

        u32 next_access_distance = UINT32_MAX;
        
        // 在未来访问序列中查找
        for (u32 j = vm->page_manager.sequence_position; j < vm->page_manager.sequence_length; j++) {
            if (vm->page_manager.future_access_sequence[j] == i) {
                next_access_distance = j - vm->page_manager.sequence_position;
                break;
            }
        }

        if (next_access_distance > farthest_distance) {
            farthest_distance = next_access_distance;
            victim_page = i;
            victim_frame = pte->frame_number;
        }
    }

    if (victim_page != INVALID_PAGE_NUMBER) {
        // 从页表中移除
        vm->page_manager.page_table[victim_page].valid = 0;
        
        // 从TLB中移除
        vm_tlb_invalidate(&vm->tlb_manager, victim_page);
        
        return victim_frame;
    }

    return INVALID_FRAME_NUMBER;
}

u32 vm_replace_clock(vm_system_t *vm) {
    if (!vm || vm->page_manager.allocated_frame_count == 0) {
        return INVALID_FRAME_NUMBER;
    }

    u32 start_pointer = vm->page_manager.clock_pointer;
    
    do {
        // 检查当前页面
        for (u32 i = 0; i < VM_MAX_PAGES; i++) {
            vm_page_table_entry_t *pte = &vm->page_manager.page_table[i];
            if (pte->valid && pte->frame_number == vm->page_manager.allocated_frames[vm->page_manager.clock_pointer]) {
                
                if (!pte->referenced) {
                    // 找到未引用的页面，替换它
                    u32 victim_frame = pte->frame_number;
                    pte->valid = 0;
                    
                    // 从TLB中移除
                    vm_tlb_invalidate(&vm->tlb_manager, i);
                    
                    // 移动时钟指针
                    vm->page_manager.clock_pointer = (vm->page_manager.clock_pointer + 1) % vm->page_manager.allocated_frame_count;
                    
                    return victim_frame;
                } else {
                    // 给第二次机会，清除引用位
                    pte->referenced = 0;
                }
                break;
            }
        }
        
        // 移动到下一个页框
        vm->page_manager.clock_pointer = (vm->page_manager.clock_pointer + 1) % vm->page_manager.allocated_frame_count;
        
    } while (vm->page_manager.clock_pointer != start_pointer);

    // 如果循环一圈都没找到，选择第一个
    if (vm->page_manager.allocated_frame_count > 0) {
        u32 victim_frame = vm->page_manager.allocated_frames[0];
        
        // 找到对应的页面并从页表中移除
        for (u32 i = 0; i < VM_MAX_PAGES; i++) {
            vm_page_table_entry_t *pte = &vm->page_manager.page_table[i];
            if (pte->valid && pte->frame_number == victim_frame) {
                pte->valid = 0;
                vm_tlb_invalidate(&vm->tlb_manager, i);
                break;
            }
        }
        
        return victim_frame;
    }

    return INVALID_FRAME_NUMBER;
}

// ==================== 性能计算和统计 ====================

void vm_calculate_performance(vm_system_t *vm) {
    if (!vm || vm->stats.total_accesses == 0) return;

    // 计算命中率
    vm->stats.page_hit_rate = (double)vm->stats.page_hits / vm->stats.total_accesses;
    vm->stats.page_fault_rate = (double)vm->stats.page_faults / vm->stats.total_accesses;
    vm->stats.tlb_hit_rate = (double)vm->stats.tlb_hits / vm->stats.total_accesses;

    // 计算平均访问时间
    double tlb_hit_time = vm->tlb_access_time;
    double memory_access_time = vm->tlb_access_time + vm->memory_access_time;
    double page_fault_time = vm->tlb_access_time + vm->page_fault_overhead * 1000;  // 转换为ns

    vm->stats.average_access_time = vm->stats.tlb_hit_rate * tlb_hit_time +
                                   vm->stats.page_hit_rate * memory_access_time +
                                   vm->stats.page_fault_rate * page_fault_time;
}

// ==================== 调试和输出函数 ====================

void vm_print_config(const vm_system_t *vm) {
    if (!vm) return;

    printf("\n=== 虚拟存储器配置信息 ===\n");
    printf("管理模式: %s\n", vm_mode_to_string(vm->mode));
    printf("替换算法: %s\n", vm_algorithm_to_string(vm->page_manager.replace_algorithm));
    printf("总页框数: %u\n", vm->total_frames);
    printf("页面大小: %u KB\n", vm->page_size / 1024);
    printf("虚拟地址位数: %u\n", vm->virtual_address_bits);
    printf("物理地址位数: %u\n", vm->physical_address_bits);
    printf("TLB大小: %u\n", VM_TLB_SIZE);
    printf("内存访问时间: %u ns\n", vm->memory_access_time);
    printf("TLB访问时间: %u ns\n", vm->tlb_access_time);
    printf("缺页处理时间: %u μs\n", vm->page_fault_overhead);
}

void vm_print_statistics(const vm_system_t *vm) {
    if (!vm) return;

    printf("\n=== 虚拟存储器统计信息 ===\n");
    printf("总访问次数: %llu\n", vm->stats.total_accesses);
    printf("页面命中次数: %llu\n", vm->stats.page_hits);
    printf("缺页次数: %llu\n", vm->stats.page_faults);
    printf("TLB命中次数: %llu\n", vm->stats.tlb_hits);
    printf("TLB未命中次数: %llu\n", vm->stats.tlb_misses);
    printf("页面替换次数: %llu\n", vm->stats.page_replacements);
    printf("\n=== 性能指标 ===\n");
    printf("页面命中率: %.2f%%\n", vm->stats.page_hit_rate * 100);
    printf("缺页率: %.2f%%\n", vm->stats.page_fault_rate * 100);
    printf("TLB命中率: %.2f%%\n", vm->stats.tlb_hit_rate * 100);
    printf("平均访问时间: %.2f ns\n", vm->stats.average_access_time);
}

// ==================== 辅助函数 ====================

const char* vm_algorithm_to_string(vm_replace_algorithm_t algorithm) {
    switch (algorithm) {
        case VM_REPLACE_FIFO: return "FIFO";
        case VM_REPLACE_LRU: return "LRU";
        case VM_REPLACE_OPT: return "OPT";
        case VM_REPLACE_CLOCK: return "CLOCK";
        default: return "UNKNOWN";
    }
}

const char* vm_mode_to_string(vm_management_mode_t mode) {
    switch (mode) {
        case VM_MODE_PAGING: return "纯分页";
        case VM_MODE_SEGMENTATION: return "纯分段";
        case VM_MODE_SEG_PAGING: return "段页式";
        default: return "UNKNOWN";
    }
}

bool vm_validate_config(const vm_system_t *vm) {
    if (!vm || !vm->initialized) return false;
    if (vm->total_frames == 0 || vm->total_frames > VM_MAX_FRAMES) return false;
    if (vm->page_size == 0 || (vm->page_size & (vm->page_size - 1)) != 0) return false;
    return true;
}