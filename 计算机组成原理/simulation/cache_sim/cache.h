#ifndef CACHE_SIM_H
#define CACHE_SIM_H

#include "../common/types.h"

/**
 * 缓存存储器体系结构模拟实现
 * 
 * 计算机存储器层次结构设计原理：
 * 
 * 1. 存储器层次结构理论基础
 * ════════════════════════════════════════════════════════════════════════════════
 * 
 * 时间局部性原理（Temporal Locality）：
 * - 理论基础：最近访问的数据在近期被再次访问的概率较高
 * - 实现机制：通过LRU替换算法利用时间局部性，将最近访问的数据保持在缓存中
 * - 数学模型：访问概率 P(t) ∝ 1/t^α，其中t为时间间隔，α为局部性强度系数
 * 
 * 空间局部性原理（Spatial Locality）：
 * - 理论基础：与最近访问地址相邻的地址被访问的概率较高
 * - 实现机制：通过缓存行（Cache Line）设计，一次加载连续的内存块
 * - 优化策略：缓存行大小通常为32-128字节，平衡预取效果与缓存污染
 * 
 * 2. 缓存映射算法设计
 * ════════════════════════════════════════════════════════════════════════════════
 * 
 * 直接映射（Direct-Mapped Cache）：
 * - 映射函数：cache_index = (address >> offset_bits) & index_mask
 * - 地址分解：| tag (t bits) | index (i bits) | offset (o bits) |
 * - 冲突处理：同一索引位置只能存储一个内存块，冲突时强制替换
 * - 硬件复杂度：O(1)，实现简单，访问速度快
 * - 命中率特性：对地址分布敏感，冲突未命中较多
 * 
 * 组相联映射（Set-Associative Cache）：
 * - 映射函数：set_index = (address >> offset_bits) & set_mask
 * - 地址分解：| tag (t bits) | set_index (s bits) | offset (o bits) |
 * - 灵活性：每个组可存储k个内存块（k-way associative）
 * - 硬件复杂度：O(k)，需要k个比较器并行比较标签
 * - 性能平衡：在硬件复杂度和命中率之间取得平衡
 * 
 * 全相联映射（Fully-Associative Cache）：
 * - 映射函数：内存块可存储在缓存中的任意位置
 * - 地址分解：| tag (t bits) | offset (o bits) |
 * - 硬件复杂度：O(n)，需要n个比较器同时比较所有标签
 * - 命中率：理论上最优，但硬件代价高昂
 * 
 * 3. 替换算法数学模型
 * ════════════════════════════════════════════════════════════════════════════════
 * 
 * LRU（Least Recently Used）算法：
 * - 理论依据：最长时间未使用的数据未来被访问概率最低
 * - 实现方式：维护访问时间戳或访问顺序链表
 * - 硬件实现：对于n-way缓存，需要log2(n!)位存储排序信息
 * - 近似算法：Clock算法、Pseudo-LRU（二分树标记法）
 * - 性能特性：对时间局部性敏感的程序效果显著
 * 
 * FIFO（First In First Out）算法：
 * - 理论依据：最早加载的数据已使用时间最长，替换优先级最高
 * - 实现方式：维护加载时间戳或循环队列指针
 * - 硬件复杂度：O(log n)位计数器，实现简单
 * - 性能特性：对访问模式变化适应性差，可能出现Belady异常
 * 
 * Random算法：
 * - 理论依据：随机替换避免最坏情况下的性能退化
 * - 实现方式：线性反馈移位寄存器（LFSR）生成伪随机序列
 * - 硬件复杂度：极低，只需少量随机数生成逻辑
 * - 性能特性：平均性能稳定，最坏情况性能有保障
 * 
 * 4. 写策略优化技术
 * ════════════════════════════════════════════════════════════════════════════════
 * 
 * 写直达（Write-Through）策略：
 * - 一致性保证：写操作同时更新缓存和主存，确保数据一致性
 * - 延迟特性：写延迟 = max(cache_write_time, memory_write_time)
 * - 带宽需求：所有写操作都需要主存带宽，可能造成带宽瓶颈
 * - 适用场景：多核系统、实时系统等对一致性要求严格的环境
 * 
 * 写回（Write-Back）策略：
 * - 延迟写入：仅在缓存行被替换时才写回主存（脏数据回写）
 * - 性能优势：写延迟 = cache_write_time，显著降低写延迟
 * - 带宽优化：减少主存访问次数，提高总线带宽利用率
 * - 一致性挑战：需要复杂的一致性协议（如MESI、MOESI）
 * 
 * 5. 性能分析数学模型
 * ════════════════════════════════════════════════════════════════════════════════
 * 
 * 平均访问时间（Average Access Time, AAT）：
 * AAT = hit_rate × cache_access_time + miss_rate × miss_penalty
 * 其中：miss_penalty = cache_access_time + memory_access_time
 * 
 * 有效访问时间（Effective Access Time, EAT）：
 * EAT = cache_access_time + miss_rate × memory_access_time
 * 
 * 缓存性能指标：
 * - 命中率（Hit Rate）：HR = hits / (hits + misses)
 * - 未命中率（Miss Rate）：MR = 1 - HR = misses / (hits + misses)
 * - 未命中惩罚（Miss Penalty）：访问下一级存储器的额外时间
 * 
 * 带宽分析：
 * - 有效带宽：Effective_BW = Peak_BW × hit_rate + Lower_Level_BW × miss_rate
 * - 写回开销：Write_Back_Overhead = dirty_eviction_rate × write_back_time
 * 
 * 6. 高级优化技术
 * ════════════════════════════════════════════════════════════════════════════════
 * 
 * 预取技术（Prefetching）：
 * - 顺序预取：预测连续访问模式，预先加载后续缓存行
 * - 步长预取：检测访问步长模式，预测下一个访问地址
 * - 分支预测结合：基于分支预测结果进行数据预取
 * 
 * 多级缓存设计：
 * - L1缓存：小容量、低延迟、高频率访问
 * - L2缓存：中等容量、平衡延迟与容量
 * - L3缓存：大容量、共享设计、降低主存访问
 * 
 * 缓存一致性协议：
 * - MESI协议：Modified、Exclusive、Shared、Invalid四状态协议
 * - MOESI协议：增加Owned状态，优化写回操作
 * - 目录协议：分布式一致性维护，适用于大规模多核系统
 */

// ==================== 缓存体系结构参数 ====================

#define CACHE_LINE_SIZE         64      // 缓存行大小（字节）- 现代处理器标准
#define MAX_CACHE_SIZE          (64*1024)   // 最大缓存大小：64KB
#define MAX_CACHE_LINES         (MAX_CACHE_SIZE / CACHE_LINE_SIZE)
#define MAX_ASSOCIATIVITY       8       // 最大组相联度

// 地址位分配计算宏
#define CACHE_OFFSET_BITS(line_size)    (__builtin_ctz(line_size))
#define CACHE_INDEX_BITS(num_sets)      (__builtin_ctz(num_sets))
#define CACHE_TAG_BITS(addr_bits, idx_bits, off_bits)  ((addr_bits) - (idx_bits) - (off_bits))

// ==================== 缓存映射体系结构类型 ====================

/**
 * 缓存映射方式枚举
 * 
 * 映射方式决定了内存地址到缓存位置的映射函数：
 * - 直接映射：固定映射关系，硬件简单但冲突较多
 * - 组相联：在灵活性和硬件复杂度间平衡
 * - 全相联：最大灵活性但硬件复杂度高
 */
typedef enum {
    CACHE_DIRECT_MAPPED = 0,    // 直接映射 - 1路组相联的特例
    CACHE_SET_ASSOCIATIVE = 1,  // 组相联映射 - n路组相联（n > 1）
    CACHE_FULLY_ASSOCIATIVE = 2 // 全相联映射 - 所有行为一组的组相联
} cache_mapping_t;

/**
 * 缓存替换算法枚举
 * 
 * 替换算法决定缓存满时选择替换哪个缓存行：
 * - LRU：利用时间局部性，理论效果最优但实现复杂
 * - FIFO：实现简单，但可能出现Belady异常
 * - Random：避免最坏情况，硬件实现极简
 * - LFU：基于访问频率，适合重复访问模式
 */
typedef enum {
    CACHE_REPLACE_LRU = 0,      // 最近最少使用（Least Recently Used）
    CACHE_REPLACE_FIFO = 1,     // 先进先出（First In First Out）
    CACHE_REPLACE_RANDOM = 2,   // 伪随机替换（Pseudo-Random）
    CACHE_REPLACE_LFU = 3       // 最不经常使用（Least Frequently Used）
} cache_replace_t;

/**
 * 缓存写策略枚举
 * 
 * 写策略决定写操作如何处理缓存与主存的一致性：
 * - 写直达：保证强一致性，但增加写延迟
 * - 写回：优化写性能，但需要复杂的一致性机制
 */
typedef enum {
    CACHE_WRITE_THROUGH = 0,    // 写直达（Write-Through）
    CACHE_WRITE_BACK = 1        // 写回（Write-Back）
} cache_write_t;

// ==================== 缓存微体系结构定义 ====================

/**
 * 缓存行微结构
 * 
 * 缓存行是缓存管理的基本单位，包含：
 * - 状态位：valid（有效）、dirty（脏）
 * - 标签：用于地址匹配的高位地址
 * - 数据：存储的实际数据块
 * - 元数据：替换算法所需的辅助信息
 */
typedef struct {
    // ===== 缓存行状态信息 =====
    bool valid;                 // 有效位 - 标识缓存行是否包含有效数据
    bool dirty;                 // 脏位 - 标识数据是否已修改（写回策略用）
    u32 tag;                    // 标记位 - 存储地址的高位部分用于匹配
    
    // ===== 数据存储区域 =====
    byte_t data[CACHE_LINE_SIZE]; // 数据块 - 存储从主存加载的数据
    
    // ===== 替换算法元数据 =====
    u32 access_time;            // 最后访问时间戳（LRU算法使用）
    u32 load_time;              // 数据加载时间戳（FIFO算法使用）
    u32 access_count;           // 累计访问次数（LFU算法使用）
} cache_line_t;

/**
 * 缓存组微结构
 * 
 * 缓存组是组相联缓存的管理单位：
 * - 包含多个缓存行（associativity个）
 * - 实现组内的替换算法逻辑
 * - 维护组级别的状态信息
 */
typedef struct {
    cache_line_t *lines;        // 缓存行数组指针
    u32 associativity;          // 组相联度 - 本组包含的缓存行数
    u32 next_replace;           // FIFO替换算法的下一个替换位置
} cache_set_t;

/**
 * 缓存配置参数结构
 * 
 * 完整描述缓存的几何结构和行为特性：
 * - 几何参数：大小、行数、组数、相联度
 * - 行为参数：映射方式、替换算法、写策略
 */
typedef struct {
    // ===== 缓存几何结构参数 =====
    u32 size;                   // 缓存总大小（字节）
    u32 line_size;              // 缓存行大小（字节）
    u32 num_lines;              // 缓存行总数
    u32 num_sets;               // 缓存组数量
    u32 associativity;          // 组相联度（每组的缓存行数）
    
    // ===== 缓存行为控制参数 =====
    cache_mapping_t mapping;    // 地址映射方式
    cache_replace_t replace;    // 缓存行替换算法
    cache_write_t write_policy; // 写操作处理策略
} cache_config_t;

/**
 * 缓存性能统计结构
 * 
 * 收集缓存运行时的性能指标：
 * - 访问统计：命中、未命中、读写分类
 * - 性能指标：命中率、未命中率
 * - 系统开销：写回次数、替换次数
 */
typedef struct {
    // ===== 基础访问统计 =====
    u64 total_accesses;         // 总访问次数
    u64 hits;                   // 命中次数
    u64 misses;                 // 未命中次数（强制未命中+冲突未命中+容量未命中）
    u64 reads;                  // 读操作次数
    u64 writes;                 // 写操作次数
    u64 writebacks;             // 写回操作次数（脏数据回写）
    
    // ===== 性能指标 =====
    double hit_rate;            // 命中率 = hits / total_accesses
    double miss_rate;           // 未命中率 = misses / total_accesses
    
    // ===== 详细未命中分类（用于性能分析）=====
    u64 compulsory_misses;      // 强制未命中（首次访问）
    u64 capacity_misses;        // 容量未命中（缓存容量不足）
    u64 conflict_misses;        // 冲突未命中（映射冲突）
} cache_stats_t;

/**
 * 缓存系统主控制结构
 * 
 * 缓存模拟器的核心数据结构：
 * - 配置信息：几何结构和行为参数
 * - 存储阵列：缓存组和缓存行的物理组织
 * - 运行状态：统计信息和控制状态
 */
typedef struct {
    // ===== 配置与控制 =====
    cache_config_t config;      // 缓存配置参数
    cache_stats_t stats;        // 性能统计信息
    u32 current_time;           // 全局时间计数器（用于LRU时间戳）
    bool initialized;           // 初始化完成标志
    
    // ===== 存储阵列 =====
    cache_set_t *sets;          // 缓存组数组（存储阵列的逻辑组织）
} cache_t;

/**
 * 缓存地址解析结构
 * 
 * 将线性内存地址分解为缓存操作所需的字段：
 * - tag：用于缓存行匹配的标签部分
 * - index/set_index：确定缓存组位置的索引
 * - offset：缓存行内偏移量
 */
typedef struct {
    u32 tag;                    // 标记位域 - 用于缓存行标签匹配
    u32 index;                  // 索引位域 - 直接映射缓存的行索引
    u32 offset;                 // 偏移位域 - 缓存行内字节偏移
    u32 set_index;              // 组索引位域 - 组相联缓存的组选择
} cache_address_t;

// ==================== 缓存系统生命周期管理 ====================

/**
 * 缓存系统初始化
 * 
 * 初始化算法：
 * 1. 参数验证：检查配置参数的合法性和一致性
 *    - 缓存大小必须是2的幂次（便于位操作优化）
 *    - 缓存行大小通常为32-128字节（平衡空间局部性和存储开销）
 *    - 组相联度不超过硬件支持的最大值
 * 
 * 2. 存储分配：为缓存组和缓存行分配连续内存空间
 *    - 使用calloc确保初始状态清零（valid=false, dirty=false）
 *    - 对齐内存分配以优化缓存行访问性能
 * 
 * 3. 元数据初始化：设置替换算法所需的初始状态
 *    - LRU算法：初始化时间戳为0
 *    - FIFO算法：初始化队列指针为0
 *    - Random算法：初始化随机数生成器种子
 * 
 * 时间复杂度：O(n)，其中n为缓存行总数
 * 空间复杂度：O(n)，主要为缓存行和元数据存储
 * 
 * @param cache 缓存系统控制结构指针，不能为NULL
 * @param config 缓存配置参数，包含几何结构和行为控制参数
 * @return CPU_SUCCESS表示初始化成功，其他值表示错误类型
 */
cpu_error_t cache_init(cache_t *cache, const cache_config_t *config);

/**
 * 缓存系统析构
 * 
 * 析构过程：
 * 1. 脏数据回写：对于写回策略，将所有脏缓存行写回主存
 * 2. 内存释放：释放缓存组、缓存行及相关元数据的内存空间
 * 3. 状态清理：重置控制结构的所有字段为初始状态
 * 
 * 性能考虑：
 * - 脏数据回写可能产生大量主存写操作
 * - 在系统关闭或缓存重配置时调用
 * 
 * @param cache 缓存系统控制结构指针
 */
void cache_destroy(cache_t *cache);

/**
 * 缓存统计信息重置
 * 
 * 重置所有性能计数器为初始状态：
 * - 访问计数器：命中、未命中、读写操作计数
 * - 性能指标：命中率、未命中率清零
 * - 时间戳：重置全局时间计数器
 * 
 * 应用场景：
 * - 性能基准测试的初始化阶段
 * - 程序不同阶段的性能分析
 * - 缓存性能调优过程中的状态重置
 * 
 * @param cache 缓存系统控制结构指针
 */
void cache_reset_stats(cache_t *cache);

// ==================== 缓存访问核心算法 ====================

/**
 * 缓存读操作
 * 
 * 读操作算法流程：
 * 1. 地址解析：将线性地址分解为tag、index、offset字段
 *    - 直接映射：index直接确定缓存行位置
 *    - 组相联：index确定组，tag用于组内匹配
 *    - 全相联：只有tag和offset，需搜索所有缓存行
 * 
 * 2. 缓存查找：在目标组中搜索匹配的缓存行
 *    - 比较存储的tag与地址解析出的tag
 *    - 检查valid位确认缓存行有效性
 *    - 时间复杂度：O(k)，k为组相联度
 * 
 * 3. 命中处理：
 *    - 数据返回：从缓存行的data字段复制数据到用户缓冲区
 *    - 元数据更新：更新访问时间戳、访问计数等替换算法元数据
 *    - 统计更新：增加命中计数和读操作计数
 * 
 * 4. 未命中处理：
 *    - 替换算法：选择victim缓存行进行替换
 *    - 写回检查：如果victim是脏数据且使用写回策略，先写回主存
 *    - 数据加载：从主存加载新数据到缓存行
 *    - 元数据设置：设置valid=true，tag=新地址tag，初始化替换元数据
 * 
 * 性能特征：
 * - 命中延迟：1-3个时钟周期（L1缓存）
 * - 未命中惩罚：20-100个时钟周期（取决于下级存储器）
 * 
 * @param cache 缓存系统控制结构指针
 * @param address 32位线性内存地址
 * @param data 数据接收缓冲区，由调用者分配
 * @param size 请求读取的数据大小（字节）
 * @return true表示缓存命中，false表示缓存未命中
 */
bool cache_read(cache_t *cache, addr_t address, void *data, size_t size);

/**
 * 缓存写操作
 * 
 * 写操作算法流程：
 * 1. 地址解析和查找：与读操作相同的地址解析和缓存查找过程
 * 
 * 2. 写命中处理：
 *    - 写直达策略：
 *      * 同时更新缓存和主存
 *      * 确保缓存与主存的强一致性
 *      * 写延迟 = max(cache_write_time, memory_write_time)
 *    - 写回策略：
 *      * 仅更新缓存，设置dirty位为true
 *      * 延迟写入主存直到缓存行被替换
 *      * 写延迟 = cache_write_time
 * 
 * 3. 写未命中处理：
 *    - 写分配（Write-Allocate）：
 *      * 从主存加载数据到缓存
 *      * 执行写操作
 *      * 适用于写操作后可能的读操作
 *    - 写不分配（No-Write-Allocate）：
 *      * 直接写入主存，不加载到缓存
 *      * 减少不必要的缓存污染
 *      * 适用于流式写操作
 * 
 * 一致性考虑：
 * - 单核系统：写直达和写回都能保证一致性
 * - 多核系统：需要缓存一致性协议（MESI、MOESI等）
 * 
 * @param cache 缓存系统控制结构指针
 * @param address 32位线性内存地址
 * @param data 写入数据源缓冲区
 * @param size 写入数据大小（字节）
 * @return true表示缓存命中，false表示缓存未命中
 */
bool cache_write(cache_t *cache, addr_t address, const void *data, size_t size);

/**
 * 缓存预取操作
 * 
 * 预取策略：
 * 1. 顺序预取：基于空间局部性，预测连续地址访问
 *    - 检测到顺序访问模式时，预取下一个缓存行
 *    - 预取距离通常为1-4个缓存行
 * 
 * 2. 步长预取：检测固定步长的访问模式
 *    - 记录最近访问的地址序列
 *    - 计算访问步长并预测下一个地址
 *    - 适用于数组遍历等规律性访问
 * 
 * 3. 预取策略控制：
 *    - 预取积极度：平衡预取效果与缓存污染
 *    - 预取精度：预测准确率的统计和自适应调整
 * 
 * 实现特点：
 * - 不更新缓存统计信息（区别于正常访问）
 * - 优先级低于正常访问（避免影响关键路径性能）
 * - 可配置的预取策略和参数
 * 
 * @param cache 缓存系统控制结构指针
 * @param address 预取的内存地址
 * @param size 预取数据大小
 */
void cache_prefetch(cache_t *cache, addr_t address, size_t size);

// ==================== 地址映射与解析算法 ====================

/**
 * 内存地址解析
 * 
 * 地址分解算法：
 * 1. 位域计算：
 *    - offset_bits = log2(line_size)
 *    - index_bits = log2(num_sets) 
 *    - tag_bits = address_width - index_bits - offset_bits
 * 
 * 2. 字段提取：
 *    - offset = address & offset_mask
 *    - index = (address >> offset_bits) & index_mask
 *    - tag = address >> (offset_bits + index_bits)
 * 
 * 3. 映射方式特殊处理：
 *    - 直接映射：index即为缓存行位置
 *    - 组相联：index为组号，需要在组内进行tag匹配
 *    - 全相联：index=0，tag包含所有用于匹配的位
 * 
 * 硬件优化：
 * - 使用位移和掩码操作（高效的位操作）
 * - 避免除法运算（改用位移）
 * - 预计算mask值以减少运行时计算
 * 
 * @param cache 缓存系统控制结构指针
 * @param address 32位线性内存地址
 * @return 分解后的缓存地址结构
 */
cache_address_t cache_parse_address(const cache_t *cache, addr_t address);

/**
 * 缓存组定位
 * 
 * 组定位算法：
 * - 直接映射：set = &cache->sets[addr->index]
 * - 组相联：set = &cache->sets[addr->set_index]
 * - 全相联：set = &cache->sets[0]（只有一个组）
 * 
 * 性能特征：
 * - 时间复杂度：O(1)
 * - 内存访问：单次指针解引用
 * 
 * @param cache 缓存系统控制结构指针
 * @param addr 解析后的地址结构
 * @return 目标缓存组的指针
 */
cache_set_t* cache_get_set(cache_t *cache, const cache_address_t *addr);

/**
 * 缓存行查找
 * 
 * 查找算法：
 * 1. 线性搜索：遍历组内所有缓存行
 *    - 检查valid位（无效行直接跳过）
 *    - 比较tag字段
 *    - 找到匹配则返回缓存行指针
 * 
 * 2. 性能优化：
 *    - 组相联度通常较小（2-16路），线性搜索效率可接受
 *    - 硬件实现中使用并行比较器
 *    - 软件模拟中可考虑哈希表优化（对于大相联度）
 * 
 * 时间复杂度：O(k)，k为组相联度
 * 
 * @param set 目标缓存组指针
 * @param tag 要匹配的标签值
 * @return 匹配的缓存行指针，未找到返回NULL
 */
cache_line_t* cache_find_line(cache_set_t *set, u32 tag);

// ==================== 缓存替换算法实现 ====================

/**
 * LRU替换算法
 * 
 * 算法原理：
 * - 理论基础：时间局部性原理，最久未使用的数据未来被访问概率最低
 * - 实现方式：维护每个缓存行的最后访问时间戳
 * - 替换策略：选择access_time最小的缓存行
 * 
 * 硬件实现复杂度：
 * - n-way缓存需要log2(n!)位存储完整LRU序列
 * - 近似LRU算法：Pseudo-LRU（树状位记录）、Clock算法
 * 
 * 性能特征：
 * - 对时间局部性强的程序效果显著
 * - 时间复杂度：O(k)，k为组相联度
 * - 适用于通用计算场景
 * 
 * @param set 目标缓存组指针
 * @return 选中的victim缓存行指针
 */
cache_line_t* cache_select_victim_lru(cache_set_t *set);

/**
 * FIFO替换算法
 * 
 * 算法原理：
 * - 理论基础：最早加载的数据已存在时间最长，替换优先级最高
 * - 实现方式：维护加载时间戳或使用循环队列指针
 * - 替换策略：按照数据加载顺序进行替换
 * 
 * 实现优化：
 * - 使用next_replace指针实现循环队列
 * - 避免时间戳比较，仅需简单的指针操作
 * 
 * Belady异常：
 * - 在某些访问模式下，增加缓存大小可能导致更多未命中
 * - 理论分析和实际测试中需要注意这种现象
 * 
 * 性能特征：
 * - 硬件实现简单，成本低
 * - 时间复杂度：O(1)
 * - 对访问模式敏感，性能不如LRU稳定
 * 
 * @param set 目标缓存组指针
 * @return 选中的victim缓存行指针
 */
cache_line_t* cache_select_victim_fifo(cache_set_t *set);

/**
 * 随机替换算法
 * 
 * 算法原理：
 * - 理论基础：随机化避免最坏情况的性能退化
 * - 实现方式：使用伪随机数生成器选择替换目标
 * - 性能保证：提供概率性的性能保障
 * 
 * 随机数生成：
 * - 线性反馈移位寄存器（LFSR）：硬件实现简单
 * - 周期长度：选择合适的多项式确保足够的随机性
 * - 种子初始化：避免重复的随机序列
 * 
 * 应用场景：
 * - 实时系统：避免最坏情况延迟
 * - 安全系统：增加攻击预测难度
 * - 通用场景：简单可靠的替换策略
 * 
 * 性能特征：
 * - 硬件实现极简，功耗低
 * - 时间复杂度：O(1)
 * - 平均性能稳定，最坏情况有界
 * 
 * @param set 目标缓存组指针
 * @return 选中的victim缓存行指针
 */
cache_line_t* cache_select_victim_random(cache_set_t *set);

/**
 * 替换算法元数据更新
 * 
 * 元数据更新策略：
 * 1. LRU算法：
 *    - 更新命中缓存行的access_time为当前时间
 *    - 增加全局时间计数器
 *    - 保持其他缓存行的时间戳不变
 * 
 * 2. FIFO算法：
 *    - 命中时不更新元数据（严格按加载顺序）
 *    - 仅在新数据加载时设置load_time
 * 
 * 3. LFU算法：
 *    - 增加命中缓存行的access_count
 *    - 周期性重置防止计数器溢出
 * 
 * 4. Random算法：
 *    - 通常不需要维护额外元数据
 *    - 可选地维护一些统计信息
 * 
 * @param cache 缓存系统控制结构指针
 * @param set 目标缓存组指针  
 * @param line 被访问的缓存行指针
 */
void cache_update_replacement_info(cache_t *cache, cache_set_t *set, cache_line_t *line);

// ==================== 缓存性能分析与诊断 ====================

/**
 * 缓存统计信息获取
 * 
 * 性能指标计算：
 * 1. 基础指标：
 *    - hit_rate = hits / total_accesses
 *    - miss_rate = misses / total_accesses = 1 - hit_rate
 *    - read_hit_rate = read_hits / reads
 *    - write_hit_rate = write_hits / writes
 * 
 * 2. 高级指标：
 *    - 平均访问时间：AAT = hit_rate × cache_time + miss_rate × miss_penalty
 *    - 有效带宽：Eff_BW = Peak_BW × hit_rate + Memory_BW × miss_rate
 *    - 缓存效率：Efficiency = useful_data / total_data_transferred
 * 
 * 3. 未命中分类分析：
 *    - 强制未命中：首次访问导致的未命中
 *    - 容量未命中：缓存容量不足导致的未命中
 *    - 冲突未命中：地址映射冲突导致的未命中
 * 
 * @param cache 缓存系统控制结构指针
 * @return 完整的缓存性能统计结构
 */
cache_stats_t cache_get_stats(const cache_t *cache);

/**
 * 缓存配置信息显示
 * 
 * 显示内容：
 * - 几何参数：大小、行大小、组数、相联度
 * - 行为参数：映射方式、替换算法、写策略
 * - 地址分解：tag位、index位、offset位
 * - 硬件特征：比较器数量、存储开销
 * 
 * @param cache 缓存系统控制结构指针
 */
void cache_print_config(const cache_t *cache);

/**
 * 缓存性能统计显示
 * 
 * 统计信息展示：
 * - 访问统计：总访问数、命中数、未命中数
 * - 操作分类：读操作、写操作、写回操作
 * - 性能指标：命中率、未命中率、平均访问时间
 * - 效率分析：带宽利用率、缓存效率
 * 
 * @param cache 缓存系统控制结构指针
 */
void cache_print_stats(const cache_t *cache);

/**
 * 缓存内容详细显示
 * 
 * 显示模式：
 * - 简要模式：仅显示有效缓存行的标签和状态
 * - 详细模式：显示完整的缓存行内容（包括数据）
 * - 调试模式：显示替换算法的元数据信息
 * 
 * @param cache 缓存系统控制结构指针
 * @param detailed 是否显示详细信息
 */
void cache_print_contents(const cache_t *cache, bool detailed);

// ==================== 缓存配置验证与优化 ====================

/**
 * 缓存配置参数验证
 * 
 * 验证规则：
 * 1. 几何一致性：
 *    - cache_size = num_lines × line_size
 *    - num_lines = num_sets × associativity
 *    - 所有大小参数必须是2的幂次
 * 
 * 2. 硬件约束：
 *    - 相联度不超过MAX_ASSOCIATIVITY
 *    - 缓存大小不超过MAX_CACHE_SIZE
 *    - 缓存行大小在合理范围内（16-128字节）
 * 
 * 3. 性能合理性：
 *    - 避免病态配置（如过大的相联度）
 *    - 检查地址位分配的合理性
 * 
 * @param config 待验证的缓存配置参数
 * @return true表示配置有效，false表示配置无效
 */
bool cache_validate_config(const cache_config_t *config);

/**
 * 缓存几何参数计算
 * 
 * 参数计算算法：
 * 1. 从缓存大小和行大小计算行数
 * 2. 从行数和相联度计算组数
 * 3. 验证计算结果的一致性
 * 4. 计算地址位分配
 * 
 * 自动修正：
 * - 将非2的幂次参数调整为最近的2的幂次
 * - 警告用户参数被自动修正
 * 
 * @param config 配置参数结构指针（会被修改）
 */
void cache_calculate_parameters(cache_config_t *config);

// ==================== 字符串转换工具函数 ====================

/**
 * 缓存映射方式转字符串
 * @param mapping 映射方式枚举值
 * @return 对应的字符串描述
 */
const char* cache_mapping_to_string(cache_mapping_t mapping);

/**
 * 缓存替换算法转字符串
 * @param replace 替换算法枚举值
 * @return 对应的字符串描述
 */
const char* cache_replace_to_string(cache_replace_t replace);

/**
 * 缓存写策略转字符串
 * @param write_policy 写策略枚举值
 * @return 对应的字符串描述
 */
const char* cache_write_to_string(cache_write_t write_policy);

#endif // CACHE_SIM_H 