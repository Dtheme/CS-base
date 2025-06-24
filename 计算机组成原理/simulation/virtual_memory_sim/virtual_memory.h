#ifndef VIRTUAL_MEMORY_SIM_H
#define VIRTUAL_MEMORY_SIM_H

#include "../common/types.h"

/**
 * 虚拟存储器管理模拟器
 * 
 * 设计原理：
 * 虚拟存储器是现代计算机系统的核心组成部分，它通过页面映射机制将有限的物理内存
 * 扩展为更大的虚拟地址空间。本模块实现了完整的虚拟存储器管理系统，包括地址翻译、
 * 页面替换、TLB管理等核心功能。
 * 
 * 架构特点：
 * 1. 分页存储管理 - 支持4KB页面大小的分页机制
 * 2. 多级地址翻译 - TLB -> 页表 -> 物理内存的三级查找
 * 3. 多种替换算法 - FIFO、LRU、OPT、Clock等经典算法
 * 4. 完整统计系统 - 命中率、缺页率等性能指标监控
 * 5. 灵活配置支持 - 可配置页框数量、替换策略等参数
 * 
 * 内存层次结构：
 * 
 *   虚拟地址     TLB快表      页表        物理内存
 *   ┌─────────┐  ┌────────┐  ┌─────────┐  ┌──────────┐
 *   │页号│偏移│->│VPN│PFN │->│PTE     │->│物理页框  │
 *   └─────────┘  └────────┘  └─────────┘  └──────────┘
 *       |           |            |            |
 *       |        快速查找    页表查找      最终访问
 *       └─────────────────→ 缺页处理 ←─────────┘
 * 
 * 地址翻译流程：
 * 1. 解析虚拟地址为页号和页内偏移
 * 2. 查找TLB，命中则直接返回物理地址
 * 3. TLB未命中，查找页表
 * 4. 页表命中，更新TLB并返回物理地址
 * 5. 页表未命中，触发缺页中断，执行页面替换
 * 
 * 核心价值：
 * - 为操作系统内存管理提供完整的模拟环境
 * - 支持多种页面替换算法的性能对比分析
 * - 提供详细的内存访问行为统计和分析
 * - 为内存优化和性能调优提供理论依据
 */

// ==================== 配置常量 ====================

#define VM_PAGE_SIZE                4096        // 页面大小：4KB
#define VM_MAX_PAGES                1048576     // 最大页面数 (20位页号)
#define VM_MAX_FRAMES               256         // 最大页框数
#define VM_TLB_SIZE                 64          // TLB大小

// ==================== 枚举类型 ====================

/**
 * 页面替换算法枚举
 * 
 * 替换算法原理：
 * 当物理内存已满而需要调入新页面时，必须选择一个已存在的页面进行替换。
 * 不同的替换算法具有不同的性能特点和实现复杂度。
 * 
 * 算法特性对比：
 * 
 * ┌─────────┬──────────┬──────────┬───────────┐
 * │ 算法    │ 实现复杂度│ 性能     │ 硬件需求   │
 * ├─────────┼──────────┼──────────┼───────────┤
 * │ FIFO    │ 简单     │ 一般     │ 无特殊需求 │
 * │ LRU     │ 中等     │ 较好     │ 访问时间戳 │
 * │ OPT     │ 复杂     │ 最优     │ 未来信息   │
 * │ Clock   │ 简单     │ 较好     │ 引用位     │
 * └─────────┴──────────┴──────────┴───────────┘
 * 
 * 算法选择策略：
 * - FIFO: 实现最简单，适合教学演示
 * - LRU: 实用性较强，接近最优性能
 * - OPT: 理论最优，主要用于性能基准
 * - Clock: 工程实现的折中方案
 */
typedef enum {
    VM_REPLACE_FIFO = 0,        // 先进先出算法 - First In First Out
    VM_REPLACE_LRU = 1,         // 最近最少使用算法 - Least Recently Used
    VM_REPLACE_OPT = 2,         // 最优替换算法 - Optimal
    VM_REPLACE_CLOCK = 3        // 时钟替换算法 - Clock Algorithm
} vm_replace_algorithm_t;

// 存储管理方式
typedef enum {
    VM_MODE_PAGING = 0,         // 纯分页
    VM_MODE_SEGMENTATION = 1,   // 纯分段
    VM_MODE_SEG_PAGING = 2      // 段页式
} vm_management_mode_t;

// ==================== 基础数据结构 ====================

// 虚拟地址结构
typedef struct {
    u32 full_address;           // 完整虚拟地址
    u32 page_number;            // 页号
    u32 page_offset;            // 页内偏移
} vm_virtual_address_t;

/**
 * 页表项结构体
 * 
 * 设计原理：
 * 页表项是虚拟内存管理的核心数据结构，每个页表项对应一个虚拟页面，
 * 记录该页面的状态信息和物理位置。通过位域优化存储空间。
 * 
 * 页表项结构分析：
 * 
 *   31                16 15    8 7   5 4   2 1 0
 *   ┌───────────────────┬───────┬─────┬─────┬─┬─┬─┐
 *   │   物理页框号      │保留位 │保护位│引用│修│有│
 *   │  (Frame Number)   │       │ RWX │ 位 │改│效│
 *   └───────────────────┴───────┴─────┴─────┴─┴─┴─┘
 * 
 * 关键字段说明：
 * - frame_number: 指向物理内存中的页框
 * - valid: 页面是否在物理内存中（0=不在，1=在）
 * - modified: 页面是否被修改过（脏页标记）
 * - referenced: 页面是否被访问过（用于LRU算法）
 * - protect: 页面保护权限（读/写/执行）
 * 
 * 替换算法支持：
 * 通过时间戳和计数器字段支持多种页面替换算法的实现
 */
typedef struct {
    u32 frame_number;           // 物理页框号 - 指向物理内存页框
    u8 valid:1;                 // 有效位 - 页面是否在内存中
    u8 modified:1;              // 修改位(脏位) - 页面是否被修改
    u8 referenced:1;            // 引用位 - 页面是否被访问
    u8 protect:3;               // 保护位 - 访问权限(读/写/执行)
    u8 reserved:2;              // 保留位 - 预留给扩展功能
    
    // 替换算法辅助字段
    u32 load_time;              // 页面加载时间 - FIFO算法使用
    u32 last_access_time;       // 最后访问时间 - LRU算法使用
    u32 access_count;           // 累计访问次数 - 统计分析使用
} vm_page_table_entry_t;

/**
 * TLB(Translation Lookaside Buffer)表项结构体
 * 
 * 设计原理：
 * TLB是一种高速缓存，用于存储最近使用的页面地址翻译结果。
 * 通过TLB可以避免每次地址翻译都访问页表，显著提高内存访问性能。
 * 
 * TLB工作机制：
 * 
 *   虚拟地址 → TLB查找 → 命中？ → 物理地址
 *        |              |
 *        |              ↓ 未命中
 *        |         页表查找 → 更新TLB
 *        |              |
 *        └──────────────┘
 * 
 * 性能影响：
 * - TLB命中：1-2个时钟周期
 * - TLB未命中：10-100个时钟周期（需访问页表）
 * - 缺页：数百万个时钟周期（需访问磁盘）
 * 
 * 设计考量：
 * TLB容量有限（通常64-1024项），需要高效的替换策略来维持较高的命中率
 */
typedef struct {
    u32 virtual_page_number;    // 虚拟页号 - TLB查找的键值
    u32 physical_frame_number;  // 物理页框号 - 翻译结果
    u8 valid:1;                 // 有效位 - 该TLB项是否有效
    u8 reserved:7;              // 保留位 - 预留给扩展功能
    u32 last_access_time;       // 最后访问时间 - 用于LRU替换
} vm_tlb_entry_t;

// ==================== 核心管理结构 ====================

// 页面管理器
typedef struct {
    vm_page_table_entry_t page_table[VM_MAX_PAGES];  // 页表
    u32 allocated_frames[VM_MAX_FRAMES];              // 已分配页框列表
    u32 free_frames[VM_MAX_FRAMES];                   // 空闲页框列表
    u32 allocated_frame_count;                        // 已分配页框数
    u32 free_frame_count;                            // 空闲页框数
    
    // 替换算法数据
    vm_replace_algorithm_t replace_algorithm;         // 替换算法
    u32 clock_pointer;                               // 时钟指针
    u32 current_time;                                // 当前时间
    
    // 访问序列(OPT算法用)
    u32 *future_access_sequence;                     // 未来访问序列
    u32 sequence_length;                             // 序列长度
    u32 sequence_position;                           // 当前位置
} vm_page_manager_t;

// TLB管理器
typedef struct {
    vm_tlb_entry_t entries[VM_TLB_SIZE];             // TLB项数组
    u32 entry_count;                                 // 当前项数
    vm_replace_algorithm_t replace_algorithm;        // TLB替换算法
    u32 current_time;                                // 当前时间
} vm_tlb_manager_t;

// 统计信息
typedef struct {
    // 访问统计
    u64 total_accesses;                              // 总访问次数
    u64 page_hits;                                   // 页面命中次数
    u64 page_faults;                                 // 缺页次数
    u64 tlb_hits;                                    // TLB命中次数
    u64 tlb_misses;                                  // TLB未命中次数
    
    // 替换统计
    u64 page_replacements;                           // 页面替换次数
    
    // 性能指标
    double page_hit_rate;                            // 页面命中率
    double page_fault_rate;                          // 缺页率
    double tlb_hit_rate;                             // TLB命中率
    double average_access_time;                      // 平均访问时间
} vm_statistics_t;

// 虚拟存储器主结构
typedef struct {
    vm_management_mode_t mode;                       // 管理模式
    vm_page_manager_t page_manager;                  // 页面管理器
    vm_tlb_manager_t tlb_manager;                    // TLB管理器
    vm_statistics_t stats;                           // 统计信息
    
    // 配置参数
    u32 total_frames;                                // 总页框数
    u32 page_size;                                   // 页面大小
    u32 virtual_address_bits;                        // 虚拟地址位数
    u32 physical_address_bits;                       // 物理地址位数
    
    // 时间配置(性能计算用)
    u32 memory_access_time;                          // 内存访问时间(ns)
    u32 tlb_access_time;                            // TLB访问时间(ns)
    u32 page_fault_overhead;                        // 缺页处理时间(μs)
    
    bool initialized;                                // 初始化标志
} vm_system_t;

// ==================== 函数声明 ====================

/**
 * 初始化虚拟存储器管理系统
 * 
 * 初始化过程：
 * 1. 参数验证 - 检查参数有效性和范围
 * 2. 结构体清零 - 确保所有字段处于已知状态
 * 3. 配置系统参数 - 设置页面大小、地址位数等
 * 4. 初始化页面管理器 - 设置页表和页框管理
 * 5. 初始化TLB管理器 - 配置TLB表项和替换策略
 * 6. 统计系统初始化 - 清零所有性能计数器
 * 
 * 关键设计决策：
 * - 页面大小固定为4KB（兼容主流系统）
 * - 虚拟/物理地址空间均为32位
 * - TLB默认使用LRU替换算法
 * - 预设合理的访问时间参数
 * 
 * @param vm 虚拟存储器系统结构指针，不能为NULL
 * @param mode 存储管理模式（当前仅支持分页模式）
 * @param total_frames 物理页框总数，范围[1, VM_MAX_FRAMES]
 * @param algorithm 页面替换算法类型
 * @return 成功返回CPU_SUCCESS，参数无效返回CPU_ERROR_INVALID_PARAM
 */
cpu_error_t vm_system_init(vm_system_t *vm, vm_management_mode_t mode, 
                          u32 total_frames, vm_replace_algorithm_t algorithm);
void vm_system_destroy(vm_system_t *vm);
void vm_reset_statistics(vm_system_t *vm);
cpu_error_t vm_set_future_sequence(vm_system_t *vm, u32 *sequence, u32 length);

/**
 * 解析虚拟地址为页号和页内偏移
 * 
 * 地址解析原理：
 * 虚拟地址按照页面大小分割为两部分：高位的页号用于查找页表，
 * 低位的偏移用于在页面内定位具体字节。
 * 
 * 地址结构（4KB页面）：
 * 
 *   31           12 11          0
 *   ┌─────────────┬─────────────┐
 *   │   页号      │  页内偏移   │
 *   │ (20 bits)   │ (12 bits)   │
 *   └─────────────┴─────────────┘
 *      |              |
 *   用于页表查找    页内字节定位
 * 
 * @param vm 虚拟存储器系统指针
 * @param virtual_addr 32位虚拟地址
 * @return 解析后的地址结构，包含页号和偏移
 */
vm_virtual_address_t vm_parse_virtual_address(const vm_system_t *vm, u32 virtual_addr);

/**
 * 虚拟地址到物理地址翻译（常规翻译）
 * 
 * 翻译流程：
 * 1. 解析虚拟地址获取页号和偏移
 * 2. 查找TLB缓存，命中则直接返回
 * 3. TLB未命中，查找页表
 * 4. 页表命中，更新TLB并返回物理地址
 * 5. 页表未命中，返回失败（不处理缺页）
 * 
 * 性能特点：
 * - 更新访问统计信息
 * - 不强制处理缺页中断
 * - 适用于性能分析和正常访问
 * 
 * @param vm 虚拟存储器系统指针
 * @param virtual_addr 待翻译的虚拟地址
 * @param physical_addr 输出的物理地址指针
 * @return 翻译成功返回true，失败返回false
 */
bool vm_translate_address(vm_system_t *vm, u32 virtual_addr, u32 *physical_addr);

/**
 * 强制虚拟地址翻译（处理缺页）
 * 
 * 翻译流程：
 * 1. 首先尝试常规地址翻译
 * 2. 翻译失败则触发缺页处理
 * 3. 分配物理页框或执行页面替换
 * 4. 更新页表和TLB
 * 5. 重新执行地址翻译
 * 
 * 使用场景：
 * - 模拟实际的内存访问行为
 * - 确保地址翻译最终成功
 * - 完整的缺页中断处理流程
 * 
 * @param vm 虚拟存储器系统指针
 * @param virtual_addr 待翻译的虚拟地址
 * @param physical_addr 输出的物理地址指针
 * @return 成功返回CPU_SUCCESS，失败返回相应错误码
 */
cpu_error_t vm_translate_address_force(vm_system_t *vm, u32 virtual_addr, u32 *physical_addr);

bool vm_tlb_lookup(vm_tlb_manager_t *tlb, u32 virtual_page_number, u32 *frame_number);
cpu_error_t vm_tlb_update(vm_tlb_manager_t *tlb, u32 virtual_page_number, u32 frame_number);
void vm_tlb_invalidate(vm_tlb_manager_t *tlb, u32 virtual_page_number);
void vm_tlb_flush(vm_tlb_manager_t *tlb);

u32 vm_allocate_frame(vm_page_manager_t *page_mgr);
void vm_free_frame(vm_page_manager_t *page_mgr, u32 frame_number);
u32 vm_replace_page(vm_system_t *vm, u32 new_page_number);
cpu_error_t vm_handle_page_fault(vm_system_t *vm, u32 virtual_page_number);

u32 vm_replace_fifo(vm_system_t *vm);
u32 vm_replace_lru(vm_system_t *vm);
u32 vm_replace_opt(vm_system_t *vm);
u32 vm_replace_clock(vm_system_t *vm);

void vm_calculate_performance(vm_system_t *vm);
void vm_print_config(const vm_system_t *vm);
void vm_print_statistics(const vm_system_t *vm);

const char* vm_algorithm_to_string(vm_replace_algorithm_t algorithm);
const char* vm_mode_to_string(vm_management_mode_t mode);
bool vm_validate_config(const vm_system_t *vm);

// ==================== 常量定义 ====================

#define INVALID_FRAME_NUMBER        0xFFFFFFFF
#define INVALID_PAGE_NUMBER         0xFFFFFFFF

#endif // VIRTUAL_MEMORY_SIM_H 