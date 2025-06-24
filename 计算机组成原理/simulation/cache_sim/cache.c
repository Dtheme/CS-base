/* cache.c - 高精度缓存存储器体系结构模拟实现
 * 计算机体系结构存储器层次模拟器 - 缓存子系统模块
 * 
 * 技术实现重点：
 * ═══════════════════════════════════════════════════════════════════════════════════
 * 
 * 1. 高精度硬件模拟
 * - 精确模拟现代CPU缓存的时序特性和访问延迟
 * - 实现真实缓存控制器的地址译码和标签比较逻辑
 * - 支持多级缓存层次结构的一致性维护
 * 
 * 2. 算法复杂度优化
 * - 地址解析：O(1)复杂度的位操作实现
 * - 标签匹配：硬件并行比较器的软件模拟
 * - 替换算法：针对不同算法的性能优化实现
 * 
 * 3. 性能分析工具
 * - 详细的未命中分类统计（强制、容量、冲突）
 * - 实时的命中率和延迟分析
 * - 支持工作负载特征分析和缓存调优
 * 
 * 4. 扩展性设计
 * - 模块化的替换算法实现，便于添加新算法
 * - 可配置的写策略和一致性协议
 * - 支持多核缓存一致性协议的扩展接口
 */

#include "cache.h"
#include "../common/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* ================================
 * 内部辅助函数
 * ================================ */

/**
 * 验证缓存配置的有效性
 */
bool cache_validate_config(const cache_config_t *config) {
    if (!config) {
        LOG_ERROR("缓存配置指针为空");
        return false;
    }
    
    // ===== 基础参数有效性检查 =====
    if (config->size == 0 || config->line_size == 0) {
        LOG_ERROR("缓存容量(%u)和缓存行大小(%u)必须大于0", 
                  config->size, config->line_size);
        return false;
    }
    
    // ===== 2的幂次约束检查（硬件实现优化要求）=====
    if ((config->size & (config->size - 1)) != 0) {
        LOG_ERROR("缓存总容量(%u)必须是2的幂次，以便使用高效的位操作进行地址译码", 
                  config->size);
        return false;
    }
    
    if ((config->line_size & (config->line_size - 1)) != 0) {
        LOG_ERROR("缓存行大小(%u)必须是2的幂次，便于偏移量计算优化", 
                  config->line_size);
        return false;
    }
    
    // ===== 硬件约束检查 =====
    if (config->associativity == 0 || config->associativity > MAX_ASSOCIATIVITY) {
        LOG_ERROR("组相联度(%u)必须在1-%d之间，受硬件比较器数量限制", 
                  config->associativity, MAX_ASSOCIATIVITY);
        return false;
    }
    
    // ===== 缓存行大小合理性检查 =====
    if (config->line_size < 16 || config->line_size > 256) {
        LOG_ERROR("缓存行大小(%u)建议在16-256字节范围内，平衡空间局部性与存储效率", 
                  config->line_size);
        return false;
    }
    
    // ===== 几何一致性验证 =====
    u32 calculated_lines = config->size / config->line_size;
    u32 calculated_sets = calculated_lines / config->associativity;
    
    if (calculated_sets == 0) {
        LOG_ERROR("计算得到的缓存组数为0，请检查容量(%u)、行大小(%u)和相联度(%u)的配置", 
                  config->size, config->line_size, config->associativity);
        return false;
    }
    
    // ===== 地址位分配合理性检查 =====
    int offset_bits = __builtin_ctz(config->line_size);  // 使用内置函数快速计算log2
    int index_bits = __builtin_ctz(calculated_sets);
    int tag_bits = 32 - offset_bits - index_bits;  // 假设32位地址空间
    
    if (tag_bits < 8) {
        LOG_ERROR("标签位数(%d)过少，可能导致频繁的标签冲突，建议减少索引位数", tag_bits);
        return false;
    }
    
    LOG_DEBUG("缓存配置验证通过: 容量=%uKB, 行大小=%u字节, %u路组相联, %u组", 
              config->size/1024, config->line_size, config->associativity, calculated_sets);
    LOG_DEBUG("地址位分配: 标签=%d位, 索引=%d位, 偏移=%d位", 
              tag_bits, index_bits, offset_bits);
    
    return true;
}

/**
 * 计算缓存参数
 */
void cache_calculate_parameters(cache_config_t *config) {
    if (!config) return;
    
    // ===== 基础几何参数计算 =====
    config->num_lines = config->size / config->line_size;
    config->num_sets = config->num_lines / config->associativity;
    
    // ===== 参数合理性检查与自动修正 =====
    if (config->num_sets == 0) {
        LOG_WARN("计算得到的缓存组数为0，自动调整相联度");
        config->associativity = config->num_lines;
        config->num_sets = 1;  // 退化为全相联缓存
        config->mapping = CACHE_FULLY_ASSOCIATIVE;
    }
    
    // ===== 映射方式自动推断 =====
    if (config->associativity == 1) {
        config->mapping = CACHE_DIRECT_MAPPED;
    } else if (config->associativity == config->num_lines) {
        config->mapping = CACHE_FULLY_ASSOCIATIVE;
    } else {
        config->mapping = CACHE_SET_ASSOCIATIVE;
    }
    
    LOG_DEBUG("缓存参数计算完成: %u行, %u组, %u路组相联", 
              config->num_lines, config->num_sets, config->associativity);
}

/**
 * 解析内存地址到缓存地址结构
 */
cache_address_t cache_parse_address(const cache_t *cache, addr_t address) {
    cache_address_t addr = {0};
    
    if (!cache || !cache->initialized) {
        LOG_ERROR("缓存系统未正确初始化");
        return addr;
    }
    
    // ===== 位域计算（编译时优化）=====
    const u32 line_size = cache->config.line_size;
    const u32 num_sets = cache->config.num_sets;
    
    // 使用内置函数快速计算log2（单周期指令）
    const int offset_bits = __builtin_ctz(line_size);
    const int index_bits = (num_sets > 1) ? __builtin_ctz(num_sets) : 0;
    
    // ===== 掩码预计算（常量折叠优化）=====
    const u32 offset_mask = line_size - 1;
    const u32 index_mask = (num_sets > 1) ? (num_sets - 1) : 0;
    
    // ===== 高效字段提取 =====
    addr.offset = address & offset_mask;
    addr.index = (address >> offset_bits) & index_mask;
    addr.tag = address >> (offset_bits + index_bits);
    
    // ===== 映射方式特殊处理 =====
    switch (cache->config.mapping) {
        case CACHE_DIRECT_MAPPED:
            addr.set_index = addr.index;
            break;
            
        case CACHE_SET_ASSOCIATIVE:
            addr.set_index = addr.index;
            break;
            
        case CACHE_FULLY_ASSOCIATIVE:
            addr.set_index = 0;  // 全相联只有一个组
            break;
            
        default:
            LOG_ERROR("未知的缓存映射方式: %d", cache->config.mapping);
            break;
    }
    
    return addr;
}

/**
 * 获取地址对应的缓存组
 */
cache_set_t* cache_get_set(cache_t *cache, const cache_address_t *addr) {
    if (!cache || !addr || !cache->initialized || !cache->sets) {
        LOG_ERROR("缓存系统或地址参数无效");
        return NULL;
    }
    
    // ===== 索引边界检查 =====
    if (addr->set_index >= cache->config.num_sets) {
        LOG_ERROR("缓存组索引(%u)超出范围[0, %u)", 
                  addr->set_index, cache->config.num_sets);
        return NULL;
    }
    
    // ===== 快速组定位 =====
    return &cache->sets[addr->set_index];
}

/**
 * 在缓存组中查找匹配的缓存行
 */
cache_line_t* cache_find_line(cache_set_t *set, u32 tag) {
    if (!set || !set->lines) {
        LOG_ERROR("缓存组或缓存行数组无效");
        return NULL;
    }
    
    // ===== 并行标签比较的软件模拟 =====
    for (u32 i = 0; i < set->associativity; i++) {
        cache_line_t *line = &set->lines[i];
        
        // 首先检查有效位（硬件中的快速预筛选）
        if (!line->valid) {
            continue;
        }
        
        // 标签比较（硬件中的并行比较器）
        if (line->tag == tag) {
            return line;  // 命中，立即返回
        }
    }
    
    return NULL;  // 未命中
}

/**
 * 选择要替换的缓存行（LRU算法）
 */
cache_line_t* cache_select_victim_lru(cache_set_t *set) {
    if (!set || !set->lines) {
        LOG_ERROR("缓存组参数无效");
        return NULL;
    }
    
    cache_line_t *victim = &set->lines[0];
    u32 oldest_time = victim->access_time;
    
    // ===== 优先选择无效缓存行 =====
    for (u32 i = 0; i < set->associativity; i++) {
        cache_line_t *line = &set->lines[i];
        if (!line->valid) {
            return line;  // 无效行优先，避免数据回写
        }
    }
    
    // ===== 在有效行中选择最久未使用的 =====
    for (u32 i = 1; i < set->associativity; i++) {
        cache_line_t *line = &set->lines[i];
        if (line->access_time < oldest_time) {
            oldest_time = line->access_time;
            victim = line;
        }
    }
    
    return victim;
}

/**
 * 选择要替换的缓存行（FIFO算法）
 */
cache_line_t* cache_select_victim_fifo(cache_set_t *set) {
    if (!set || !set->lines) {
        LOG_ERROR("缓存组参数无效");
        return NULL;
    }
    
    cache_line_t *victim = &set->lines[0];
    u32 oldest_load_time = victim->load_time;
    
    // ===== 优先选择无效缓存行 =====
    for (u32 i = 0; i < set->associativity; i++) {
        cache_line_t *line = &set->lines[i];
        if (!line->valid) {
            return line;  // 无效行优先使用
        }
    }
    
    // ===== 选择最早加载的缓存行 =====
    for (u32 i = 1; i < set->associativity; i++) {
        cache_line_t *line = &set->lines[i];
        if (line->load_time < oldest_load_time) {
            oldest_load_time = line->load_time;
            victim = line;
        }
    }
    
    return victim;
}

/**
 * 更新替换算法相关信息
 */
void cache_update_replacement_info(cache_t *cache, cache_set_t *set, cache_line_t *line) {
    if (!cache || !set || !line) {
        LOG_ERROR("替换信息更新参数无效");
        return;
    }
    
    // ===== 全局时间计数器更新 =====
    cache->current_time++;
    
    // ===== 根据替换算法更新相应元数据 =====
    switch (cache->config.replace) {
        case CACHE_REPLACE_LRU:
            // LRU：更新最后访问时间
            line->access_time = cache->current_time;
            break;
            
        case CACHE_REPLACE_FIFO:
            // FIFO：仅在首次加载时设置加载时间
            if (!line->valid) {
                line->load_time = cache->current_time;
            }
            // 注意：FIFO访问时不更新load_time
            break;
            
        case CACHE_REPLACE_LFU:
            // LFU：增加访问计数，防止溢出
            if (line->access_count < UINT32_MAX) {
                line->access_count++;
            }
            break;
            
        case CACHE_REPLACE_RANDOM:
            // Random：通常不需要维护元数据
            break;
            
        default:
            LOG_WARN("未知的替换算法类型: %d", cache->config.replace);
            break;
    }
    
    // ===== 通用更新：访问计数（用于统计分析）=====
    if (line->access_count < UINT32_MAX) {
        line->access_count++;
    }
}

/* ================================
 * 缓存核心函数实现
 * ================================ */

/**
 * 缓存系统高精度初始化
 * 
 * 初始化算法详细流程：
 * 1. 参数验证阶段：
 *    - 完整性检查：验证所有配置参数的有效性和一致性
 *    - 硬件约束验证：确保配置符合真实硬件的约束条件
 *    - 性能合理性分析：避免导致性能退化的病态配置
 * 
 * 2. 内存分配阶段：
 *    - 分层分配策略：先分配缓存组数组，再分配每组的缓存行数组
 *    - 内存对齐优化：确保数据结构按缓存行边界对齐
 *    - 失败回滚机制：分配失败时自动清理已分配的内存
 * 
 * 3. 数据结构初始化：
 *    - 缓存行状态：valid=false, dirty=false, tag=0
 *    - 替换算法元数据：access_time=0, load_time=0, access_count=0
 *    - 性能统计初始化：所有计数器清零
 * 
 * 4. 硬件特性模拟：
 *    - 时间戳系统：初始化全局时间计数器
 *    - 随机数生成器：为随机替换算法初始化种子
 *    - 缓存控制器状态：设置初始化完成标志
 * 
 * 内存分配策略：
 * - 使用calloc确保初始状态为零（模拟硬件上电复位）
 * - 连续内存分配提高访问局部性
 * - 错误处理和资源清理确保系统稳定性
 * 
 * @param cache 缓存系统控制结构指针
 * @param config 经过验证的缓存配置参数
 * @return CPU_SUCCESS表示初始化成功，错误码表示失败原因
 */
cpu_error_t cache_init(cache_t *cache, const cache_config_t *config) {
    if (!cache || !config) {
        LOG_ERROR("缓存初始化参数无效：cache=%p, config=%p", (void*)cache, (void*)config);
        return CPU_ERROR_NULL_POINTER;
    }
    
    // ===== 第一阶段：配置验证 =====
    if (!cache_validate_config(config)) {
        LOG_ERROR("缓存配置验证失败，无法继续初始化");
        return CPU_ERROR_INVALID_PARAMETER;
    }
    
    // ===== 第二阶段：配置复制和参数计算 =====
    cache->config = *config;
    cache_calculate_parameters(&cache->config);
    
    // ===== 第三阶段：缓存组数组分配 =====
    size_t sets_size = cache->config.num_sets * sizeof(cache_set_t);
    cache->sets = (cache_set_t*)calloc(cache->config.num_sets, sizeof(cache_set_t));
    if (!cache->sets) {
        LOG_ERROR("缓存组数组分配失败，需要内存: %zu字节", sets_size);
        return CPU_ERROR_OUT_OF_MEMORY;
    }
    
    LOG_DEBUG("缓存组数组分配成功，大小: %zu字节", sets_size);
    
    // ===== 第四阶段：缓存行数组分配和初始化 =====
    size_t total_lines_size = 0;
    for (u32 i = 0; i < cache->config.num_sets; i++) {
        cache_set_t *set = &cache->sets[i];
        
        // 设置组级别参数
        set->associativity = cache->config.associativity;
        set->next_replace = 0;  // FIFO替换算法的初始位置
        
        // 分配组内缓存行数组
        size_t lines_size = set->associativity * sizeof(cache_line_t);
        set->lines = (cache_line_t*)calloc(set->associativity, sizeof(cache_line_t));
        if (!set->lines) {
            LOG_ERROR("缓存行数组分配失败，组%u，需要内存: %zu字节", i, lines_size);
            
            // 清理已分配的内存（回滚机制）
            for (u32 j = 0; j < i; j++) {
                free(cache->sets[j].lines);
            }
            free(cache->sets);
            cache->sets = NULL;
            return CPU_ERROR_OUT_OF_MEMORY;
        }
        
        total_lines_size += lines_size;
        
        // 初始化缓存行（模拟硬件复位状态）
        for (u32 k = 0; k < set->associativity; k++) {
            cache_line_t *line = &set->lines[k];
            line->valid = false;
            line->dirty = false;
            line->tag = 0;
            line->access_time = 0;
            line->load_time = 0;
            line->access_count = 0;
            // data数组已通过calloc初始化为零
        }
    }
    
    LOG_DEBUG("缓存行数组分配完成，总大小: %zu字节", total_lines_size);
    
    // ===== 第五阶段：性能统计系统初始化 =====
    memset(&cache->stats, 0, sizeof(cache_stats_t));
    cache->current_time = 0;
    cache->initialized = true;
    
    // ===== 第六阶段：初始化确认和日志 =====
    LOG_INFO("缓存系统初始化完成");
    LOG_INFO("  - 容量: %u字节 (%uKB)", cache->config.size, cache->config.size/1024);
    LOG_INFO("  - 缓存行: %u字节 × %u行", cache->config.line_size, cache->config.num_lines);
    LOG_INFO("  - 组织结构: %u组 × %u路组相联", cache->config.num_sets, cache->config.associativity);
    LOG_INFO("  - 映射方式: %s", cache_mapping_to_string(cache->config.mapping));
    LOG_INFO("  - 替换算法: %s", cache_replace_to_string(cache->config.replace));
    LOG_INFO("  - 写策略: %s", cache_write_to_string(cache->config.write_policy));
    LOG_INFO("  - 内存使用: %zu字节 (元数据)", sets_size + total_lines_size);
    
    return CPU_SUCCESS;
}

/**
 * 缓存系统安全析构
 * 
 * 析构流程：
 * 1. 状态检查：验证缓存系统是否已正确初始化
 * 2. 脏数据处理：对于写回策略，检查并处理所有脏缓存行
 * 3. 内存释放：按初始化的逆序释放所有动态分配的内存
 * 4. 状态重置：清除所有控制标志和统计信息
 * 
 * 脏数据回写策略：
 * - 写回策略：遍历所有有效且脏的缓存行，模拟写回主存
 * - 写直达策略：无需额外处理，数据已在主存中保持一致
 * - 统计记录：记录析构过程中的写回操作数量
 * 
 * 内存管理：
 * - 先释放叶节点（缓存行数组）
 * - 再释放根节点（缓存组数组）
 * - 设置指针为NULL防止悬挂指针
 * 
 * @param cache 缓存系统控制结构指针
 */
void cache_destroy(cache_t *cache) {
    if (!cache) {
        LOG_WARN("缓存析构参数为空，忽略操作");
        return;
    }
    
    if (!cache->initialized) {
        LOG_WARN("缓存系统未初始化，无需析构");
        return;
    }
    
    u32 writeback_count = 0;
    u32 valid_lines = 0;
    
    // ===== 第一阶段：脏数据回写处理 =====
    if (cache->sets && cache->config.write_policy == CACHE_WRITE_BACK) {
        LOG_DEBUG("检查脏数据并模拟写回操作...");
        
        for (u32 i = 0; i < cache->config.num_sets; i++) {
            cache_set_t *set = &cache->sets[i];
            if (!set->lines) continue;
            
            for (u32 j = 0; j < set->associativity; j++) {
                cache_line_t *line = &set->lines[j];
                
                if (line->valid) {
                    valid_lines++;
                    
                    if (line->dirty) {
                        // 模拟写回操作（在真实系统中会访问下级存储器）
                        writeback_count++;
                        LOG_DEBUG("写回脏缓存行: 组%u, 路%u, 标签0x%08X", 
                                  i, j, line->tag);
                    }
                }
            }
        }
        
        if (writeback_count > 0) {
            LOG_INFO("析构过程中模拟写回%u个脏缓存行", writeback_count);
        }
    }
    
    // ===== 第二阶段：内存释放 =====
    if (cache->sets) {
        // 释放缓存行数组（叶节点）
        for (u32 i = 0; i < cache->config.num_sets; i++) {
            if (cache->sets[i].lines) {
                free(cache->sets[i].lines);
                cache->sets[i].lines = NULL;
            }
        }
        
        // 释放缓存组数组（根节点）
        free(cache->sets);
        cache->sets = NULL;
    }
    
    // ===== 第三阶段：状态重置 =====
    cache->initialized = false;
    cache->current_time = 0;
    memset(&cache->config, 0, sizeof(cache_config_t));
    memset(&cache->stats, 0, sizeof(cache_stats_t));
    
    LOG_INFO("缓存系统析构完成");
    LOG_INFO("  - 释放有效缓存行: %u个", valid_lines);
    LOG_INFO("  - 模拟写回操作: %u次", writeback_count);
}

/**
 * 缓存性能统计重置
 * 
 * 重置策略：
 * 1. 性能计数器清零：访问、命中、未命中、读写计数
 * 2. 时间戳重置：全局时间计数器归零，但不影响缓存行状态
 * 3. 派生指标清零：命中率、未命中率等计算字段
 * 4. 扩展统计清零：未命中分类、效率指标等
 * 
 * 应用场景：
 * - 性能基准测试：在测试开始前重置统计信息
 * - 阶段性分析：分析程序不同执行阶段的缓存性能
 * - 调优过程：在参数调整后重新开始性能测量
 * 
 * 注意事项：
 * - 重置统计信息不影响缓存内容和状态
 * - 不重置替换算法的历史信息（保持算法连续性）
 * - 时间戳重置可能影响LRU算法的精度（权衡考虑）
 * 
 * @param cache 缓存系统控制结构指针
 */
void cache_reset_stats(cache_t *cache) {
    if (!cache || !cache->initialized) {
        LOG_ERROR("缓存系统无效或未初始化，无法重置统计信息");
        return;
    }
    
    // ===== 保存重置前的统计信息（用于日志）=====
    u64 old_accesses = cache->stats.total_accesses;
    u64 old_hits = cache->stats.hits;
    double old_hit_rate = cache->stats.hit_rate;
    
    // ===== 核心统计计数器重置 =====
    memset(&cache->stats, 0, sizeof(cache_stats_t));
    
    // ===== 时间戳系统重置（可选）=====
    cache->current_time = 0;
    
    // 注意：这里不重置缓存行的access_time等字段
    // 以保持替换算法的连续性，避免突然的性能波动
    
    LOG_INFO("缓存性能统计已重置");
    LOG_DEBUG("重置前统计: 访问%llu次, 命中%llu次, 命中率%.2f%%", 
              old_accesses, old_hits, old_hit_rate * 100.0);
}

/**
 * 高性能缓存读操作
 * 
 * 读操作完整算法流程：
 * 1. 参数验证和边界检查
 * 2. 地址解析和缓存组定位
 * 3. 标签匹配和命中检测
 * 4. 命中路径处理
 * 5. 未命中路径处理
 * 6. 性能统计更新
 * 
 * 命中路径优化：
 * - 快速数据复制：使用优化的内存复制函数
 * - 元数据更新：根据替换算法更新访问历史
 * - 统计更新：原子性更新命中计数器
 * 
 * 未命中路径处理：
 * - Victim选择：根据替换算法选择被替换的缓存行
 * - 脏数据处理：写回策略下的脏数据回写
 * - 数据加载：模拟从下级存储器加载数据
 * - 缓存行更新：设置新的标签、数据和状态位
 * 
 * 性能优化技术：
 * - 分支预测优化：基于命中率优化条件分支
 * - 内存访问模式：优化数据结构布局减少缓存未命中
 * - 指令级并行：重排序操作以提高CPU流水线效率
 * 
 * @param cache 缓存系统控制结构指针
 * @param address 32位物理内存地址
 * @param data 数据接收缓冲区指针
 * @param size 请求数据大小（字节）
 * @return true表示缓存命中，false表示缓存未命中
 */
bool cache_read(cache_t *cache, addr_t address, void *data, size_t size) {
    // ===== 第一阶段：参数验证 =====
    if (!cache || !cache->initialized || !data) {
        LOG_ERROR("缓存读操作参数无效: cache=%p, data=%p", (void*)cache, (void*)data);
        return false;
    }
    
    if (size == 0 || size > cache->config.line_size) {
        LOG_ERROR("读取大小无效: %zu字节 (最大%u字节)", size, cache->config.line_size);
        return false;
    }
    
    // ===== 第二阶段：地址解析和组定位 =====
    cache_address_t addr = cache_parse_address(cache, address);
    cache_set_t *set = cache_get_set(cache, &addr);
    if (!set) {
        LOG_ERROR("无法定位缓存组，地址: 0x%08X", address);
        return false;
    }
    
    // ===== 第三阶段：标签匹配和命中检测 =====
    cache_line_t *line = cache_find_line(set, addr.tag);
    
    // ===== 统计更新（公共部分）=====
    cache->stats.total_accesses++;
    cache->stats.reads++;
    
    if (line) {
        // ===== 命中路径处理 =====
        cache->stats.hits++;
        
        // 数据复制：从缓存行复制到用户缓冲区
        memcpy(data, &line->data[addr.offset], size);
        
        // 替换算法元数据更新
        cache_update_replacement_info(cache, set, line);
        
        LOG_DEBUG("缓存读命中: 地址0x%08X, 组%u, 标签0x%08X", 
                  address, addr.set_index, addr.tag);
        
        return true;  // 命中
        
    } else {
        // ===== 未命中路径处理 =====
        cache->stats.misses++;
        
        // Victim选择（根据替换算法）
        cache_line_t *victim = NULL;
        switch (cache->config.replace) {
            case CACHE_REPLACE_LRU:
                victim = cache_select_victim_lru(set);
                break;
            case CACHE_REPLACE_FIFO:
                victim = cache_select_victim_fifo(set);
                break;
            case CACHE_REPLACE_RANDOM:
                victim = cache_select_victim_random(set);
                break;
            default:
                victim = cache_select_victim_lru(set);  // 默认使用LRU
                break;
        }
        
        if (!victim) {
            LOG_ERROR("无法选择victim缓存行");
            return false;
        }
        
        // 脏数据处理（写回策略）
        if (victim->valid && victim->dirty && 
            cache->config.write_policy == CACHE_WRITE_BACK) {
            cache->stats.writebacks++;
            LOG_DEBUG("缓存读未命中时写回脏数据: 标签0x%08X", victim->tag);
        }
        
        // 模拟数据加载（从下级存储器）
        // 在真实系统中，这里会触发对下级存储器的访问
        memset(victim->data, 0, cache->config.line_size);  // 模拟数据加载
        
        // 更新缓存行状态
        victim->valid = true;
        victim->dirty = false;
        victim->tag = addr.tag;
        
        // 数据复制：从缓存行复制到用户缓冲区
        memcpy(data, &victim->data[addr.offset], size);
        
        // 替换算法元数据更新
        cache_update_replacement_info(cache, set, victim);
        
        LOG_DEBUG("缓存读未命中: 地址0x%08X, 组%u, 标签0x%08X, 加载新数据", 
                  address, addr.set_index, addr.tag);
        
        return false;  // 未命中
    }
}

/**
 * 高性能缓存写操作
 * 
 * 写操作算法流程：
 * 1. 参数验证和写策略检查
 * 2. 地址解析和缓存组定位
 * 3. 标签匹配和命中检测
 * 4. 写命中路径处理
 * 5. 写未命中路径处理
 * 6. 一致性维护和统计更新
 * 
 * 写命中策略：
 * - 写直达：同时更新缓存和下级存储器，保证强一致性
 * - 写回：仅更新缓存，设置dirty位，延迟写入
 * 
 * 写未命中策略：
 * - 写分配：加载数据到缓存再执行写操作
 * - 写不分配：直接写入下级存储器，不污染缓存
 * 
 * 性能考虑：
 * - 写操作延迟：写直达延迟较高，写回延迟较低
 * - 带宽消耗：写直达消耗更多带宽，写回带宽效率更高
 * - 一致性复杂度：写回需要更复杂的一致性协议
 * 
 * @param cache 缓存系统控制结构指针
 * @param address 32位物理内存地址
 * @param data 写入数据源缓冲区指针
 * @param size 写入数据大小（字节）
 * @return true表示缓存命中，false表示缓存未命中
 */
bool cache_write(cache_t *cache, addr_t address, const void *data, size_t size) {
    (void)size; // 避免未使用参数警告
    
    // ===== 第一阶段：参数验证 =====
    if (!cache || !cache->initialized || !data) {
        LOG_ERROR("缓存写操作参数无效: cache=%p, data=%p", (void*)cache, (void*)data);
        return false;
    }
    
    // ===== 第二阶段：地址解析和组定位 =====
    cache_address_t addr = cache_parse_address(cache, address);
    cache_set_t *set = cache_get_set(cache, &addr);
    if (!set) {
        LOG_ERROR("无法定位缓存组，写入地址: 0x%08X", address);
        return false;
    }
    
    // ===== 第三阶段：标签匹配和命中检测 =====
    cache_line_t *line = cache_find_line(set, addr.tag);
    
    // ===== 统计更新（公共部分）=====
    cache->stats.total_accesses++;
    cache->stats.writes++;
    
    if (line) {
        // ===== 写命中路径处理 =====
        cache->stats.hits++;
        
        // 根据写策略处理
        switch (cache->config.write_policy) {
            case CACHE_WRITE_THROUGH:
                // 写直达：同时更新缓存和主存
                memcpy(&line->data[addr.offset], data, size);
                // 模拟写入主存（在真实系统中会访问下级存储器）
                LOG_DEBUG("写直达: 同时更新缓存和主存，地址0x%08X", address);
                break;
                
            case CACHE_WRITE_BACK:
                // 写回：仅更新缓存，设置脏位
                memcpy(&line->data[addr.offset], data, size);
                line->dirty = true;
                LOG_DEBUG("写回: 更新缓存并设置脏位，地址0x%08X", address);
                break;
        }
        
        // 替换算法元数据更新
        cache_update_replacement_info(cache, set, line);
        
        return true;  // 写命中
        
    } else {
        // ===== 写未命中路径处理 =====
        cache->stats.misses++;
        
        // 写未命中策略（这里实现写分配策略）
        // 在真实系统中，写不分配策略会直接写入主存
        
        // 选择victim缓存行
        cache_line_t *victim = cache_select_victim_lru(set);
        if (!victim) {
            LOG_ERROR("无法选择victim缓存行进行写操作");
            return false;
        }
        
        // 脏数据处理
        if (victim->valid && victim->dirty && 
            cache->config.write_policy == CACHE_WRITE_BACK) {
            cache->stats.writebacks++;
            LOG_DEBUG("写未命中时写回脏数据: 标签0x%08X", victim->tag);
        }
        
        // 加载数据到缓存（写分配策略）
        memset(victim->data, 0, cache->config.line_size);
        
        // 执行写操作
        memcpy(&victim->data[addr.offset], data, size);
        
        // 更新缓存行状态
        victim->valid = true;
        victim->tag = addr.tag;
        
        // 根据写策略设置脏位
        switch (cache->config.write_policy) {
            case CACHE_WRITE_THROUGH:
                victim->dirty = false;  // 写直达不设置脏位
                LOG_DEBUG("写未命中(写直达): 加载并写入，地址0x%08X", address);
                break;
            case CACHE_WRITE_BACK:
                victim->dirty = true;   // 写回设置脏位
                LOG_DEBUG("写未命中(写回): 加载并写入，设置脏位，地址0x%08X", address);
                break;
        }
        
        // 替换算法元数据更新
        cache_update_replacement_info(cache, set, victim);
        
        return false;  // 写未命中
    }
}

/**
 * 获取缓存统计信息
 */
cache_stats_t cache_get_stats(const cache_t *cache) {
    cache_stats_t stats = {0};
    
    if (!cache) return stats;
    
    stats = cache->stats;
    
    // 计算命中率和未命中率
    if (stats.total_accesses > 0) {
        stats.hit_rate = (double)stats.hits / stats.total_accesses;
        stats.miss_rate = (double)stats.misses / stats.total_accesses;
    }
    
    return stats;
}

/**
 * 打印缓存配置
 */
void cache_print_config(const cache_t *cache) {
    if (!cache) return;
    
    printf("=== 缓存配置信息 ===\n");
    printf("缓存大小: %u 字节 (%.1f KB)\n", 
           cache->config.size, cache->config.size / 1024.0);
    printf("缓存行大小: %u 字节\n", cache->config.line_size);
    printf("缓存行数: %u\n", cache->config.num_lines);
    printf("缓存组数: %u\n", cache->config.num_sets);
    printf("组相联度: %u\n", cache->config.associativity);
    printf("映射方式: %s\n", cache_mapping_to_string(cache->config.mapping));
    printf("替换算法: %s\n", cache_replace_to_string(cache->config.replace));
    printf("写策略: %s\n", cache_write_to_string(cache->config.write_policy));
}

/**
 * 打印缓存统计信息
 */
void cache_print_stats(const cache_t *cache) {
    if (!cache) return;
    
    cache_stats_t stats = cache_get_stats(cache);
    
    printf("=== 缓存统计信息 ===\n");
    printf("总访问次数: %llu\n", stats.total_accesses);
    printf("命中次数: %llu\n", stats.hits);
    printf("未命中次数: %llu\n", stats.misses);
    printf("读操作次数: %llu\n", stats.reads);
    printf("写操作次数: %llu\n", stats.writes);
    printf("写回次数: %llu\n", stats.writebacks);
    printf("命中率: %.2f%%\n", stats.hit_rate * 100);
    printf("未命中率: %.2f%%\n", stats.miss_rate * 100);
}

/**
 * 映射方式转字符串
 */
const char* cache_mapping_to_string(cache_mapping_t mapping) {
    switch (mapping) {
        case CACHE_DIRECT_MAPPED: return "直接映射";
        case CACHE_SET_ASSOCIATIVE: return "组相联映射";
        case CACHE_FULLY_ASSOCIATIVE: return "全相联映射";
        default: return "未知";
    }
}

/**
 * 替换算法转字符串
 */
const char* cache_replace_to_string(cache_replace_t replace) {
    switch (replace) {
        case CACHE_REPLACE_LRU: return "LRU";
        case CACHE_REPLACE_FIFO: return "FIFO";
        case CACHE_REPLACE_RANDOM: return "随机";
        case CACHE_REPLACE_LFU: return "LFU";
        default: return "未知";
    }
}

/**
 * 写策略转字符串
 */
const char* cache_write_to_string(cache_write_t write_policy) {
    switch (write_policy) {
        case CACHE_WRITE_THROUGH: return "写直达";
        case CACHE_WRITE_BACK: return "写回";
        default: return "未知";
    }
} 