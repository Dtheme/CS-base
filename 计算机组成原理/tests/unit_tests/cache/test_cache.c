/* test_cache.c - 缓存系统单元测试
 * 计算机组成原理模拟器 - 缓存模块测试
 * create by: zw.duan
 */

#include "../../simulation/cache_sim/cache.h"
#include "../../simulation/common/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 缓存系统单元测试程序
 * 
 * 本程序测试缓存的基本功能，包括：
 * 1. 缓存初始化和配置
 * 2. 基本读写操作
 * 3. 命中率统计
 * 4. 408场景演示
 */

// 测试统计结构
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    char current_test_name[256];
} TestStats;

// 全局测试状态
static TestStats g_test_stats = {0};

#define TEST_START(name) do { \
    strcpy(g_test_stats.current_test_name, name); \
    printf("\n[测试] %s\n", name); \
    g_test_stats.total_tests++; \
} while(0)

#define TEST_ASSERT(condition, message) do { \
    if (condition) { \
        printf("  ✓ %s\n", message); \
    } else { \
        printf("  ✗ %s\n", message); \
        g_test_stats.failed_tests++; \
        return false; \
    } \
} while(0)

#define TEST_END() do { \
    g_test_stats.passed_tests++; \
    printf("  [通过] %s\n", g_test_stats.current_test_name); \
} while(0)

/**
 * 测试缓存初始化
 */
bool test_cache_initialization(void) {
    TEST_START("缓存初始化测试");
    
    cache_t cache;
    cache_config_t config = {
        .size = 1024,
        .line_size = 64,
        .associativity = 2,
        .mapping = CACHE_SET_ASSOCIATIVE,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_THROUGH
    };
    
    // 计算配置参数
    cache_calculate_parameters(&config);
    
    // 验证配置
    TEST_ASSERT(cache_validate_config(&config), "配置验证通过");
    
    // 初始化缓存
    cpu_error_t result = cache_init(&cache, &config);
    TEST_ASSERT(result == CPU_SUCCESS, "缓存初始化成功");
    TEST_ASSERT(cache.initialized, "初始化标志正确");
    TEST_ASSERT(cache.config.size == 1024, "缓存大小正确");
    TEST_ASSERT(cache.config.line_size == 64, "缓存行大小正确");
    
    // 清理
    cache_destroy(&cache);
    
    TEST_END();
    return true;
}

/**
 * 测试缓存配置验证
 */
bool test_cache_config_validation(void) {
    TEST_START("缓存配置验证测试");
    
    cache_config_t valid_config = {
        .size = 2048,
        .line_size = 32,
        .associativity = 4,
        .mapping = CACHE_SET_ASSOCIATIVE,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_BACK
    };
    
    TEST_ASSERT(cache_validate_config(&valid_config), "有效配置验证通过");
    
    // 测试无效配置
    cache_config_t invalid_config = {
        .size = 0,  // 无效大小
        .line_size = 32,
        .associativity = 4,
        .mapping = CACHE_SET_ASSOCIATIVE,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_BACK
    };
    
    TEST_ASSERT(!cache_validate_config(&invalid_config), "无效配置验证失败");
    
    TEST_END();
    return true;
}

/**
 * 测试基本读写操作
 */
bool test_cache_basic_operations(void) {
    TEST_START("缓存基本操作测试");
    
    cache_t cache;
    cache_config_t config = {
        .size = 512,
        .line_size = 32,
        .associativity = 1,  // 直接映射
        .mapping = CACHE_DIRECT_MAPPED,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_THROUGH
    };
    
    cache_calculate_parameters(&config);
    cpu_error_t result = cache_init(&cache, &config);
    TEST_ASSERT(result == CPU_SUCCESS, "缓存初始化成功");
    
    // 测试读操作（第一次应该是miss）
    u32 data;
    bool hit = cache_read(&cache, 0x1000, &data, sizeof(data));
    TEST_ASSERT(!hit, "首次读取应该miss");
    
    // 测试写操作
    u32 write_data = 0x12345678;
    hit = cache_write(&cache, 0x1000, &write_data, sizeof(write_data));
    TEST_ASSERT(!hit, "首次写入应该miss");
    
    // 再次读取相同地址（应该hit）
    hit = cache_read(&cache, 0x1000, &data, sizeof(data));
    TEST_ASSERT(hit, "再次读取应该hit");
    
    // 获取统计信息
    cache_stats_t stats = cache_get_stats(&cache);
    TEST_ASSERT(stats.total_accesses > 0, "总访问次数大于0");
    TEST_ASSERT(stats.hits > 0, "命中次数大于0");
    TEST_ASSERT(stats.misses > 0, "未命中次数大于0");
    
    cache_destroy(&cache);
    
    TEST_END();
    return true;
}

/**
 * 测试地址解析
 */
bool test_cache_address_parsing(void) {
    TEST_START("缓存地址解析测试");
    
    cache_t cache;
    cache_config_t config = {
        .size = 1024,
        .line_size = 64,
        .associativity = 2,
        .mapping = CACHE_SET_ASSOCIATIVE,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_THROUGH
    };
    
    cache_calculate_parameters(&config);
    cpu_error_t result = cache_init(&cache, &config);
    TEST_ASSERT(result == CPU_SUCCESS, "缓存初始化成功");
    
    // 测试地址解析
    addr_t test_addr = 0x12345678;
    cache_address_t parsed = cache_parse_address(&cache, test_addr);
    
    TEST_ASSERT(parsed.offset < config.line_size, "偏移位合理");
    TEST_ASSERT(parsed.index < config.num_sets, "索引位合理");
    TEST_ASSERT(parsed.tag > 0, "标记位非零");
    
    cache_destroy(&cache);
    
    TEST_END();
    return true;
}

/**
 * 测试统计信息
 */
bool test_cache_statistics(void) {
    TEST_START("缓存统计信息测试");
    
    cache_t cache;
    cache_config_t config = {
        .size = 256,
        .line_size = 32,
        .associativity = 1,
        .mapping = CACHE_DIRECT_MAPPED,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_THROUGH
    };
    
    cache_calculate_parameters(&config);
    cpu_error_t result = cache_init(&cache, &config);
    TEST_ASSERT(result == CPU_SUCCESS, "缓存初始化成功");
    
    // 重置统计信息
    cache_reset_stats(&cache);
    cache_stats_t stats = cache_get_stats(&cache);
    TEST_ASSERT(stats.total_accesses == 0, "重置后总访问为0");
    TEST_ASSERT(stats.hits == 0, "重置后命中为0");
    TEST_ASSERT(stats.misses == 0, "重置后未命中为0");
    
    // 执行一些操作
    u32 data = 0x12345678;
    cache_write(&cache, 0x1000, &data, sizeof(data));
    cache_read(&cache, 0x1000, &data, sizeof(data));
    cache_read(&cache, 0x2000, &data, sizeof(data));
    
    // 检查统计信息
    stats = cache_get_stats(&cache);
    TEST_ASSERT(stats.total_accesses == 3, "总访问次数正确");
    TEST_ASSERT(stats.hits >= 1, "至少有一次命中");
    TEST_ASSERT(stats.misses >= 1, "至少有一次未命中");
    
    cache_destroy(&cache);
    
    TEST_END();
    return true;
}

/**
 * 408场景演示
 */
void demo_408_exam_scenarios(void) {
    printf("\n=== 408场景演示 ===\n");
    
    // 场景1：直接映射缓存
    printf("\n【场景1】直接映射缓存分析\n");
    cache_t cache1;
    cache_config_t config1 = {
        .size = 128,
        .line_size = 16,
        .associativity = 1,
        .mapping = CACHE_DIRECT_MAPPED,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_THROUGH
    };
    
    cache_calculate_parameters(&config1);
    cache_init(&cache1, &config1);
    
    printf("缓存配置：大小=%d字节，行大小=%d字节，组数=%d\n", 
           config1.size, config1.line_size, config1.num_sets);
    
    // 模拟访问序列
    addr_t addresses[] = {0x0000, 0x0010, 0x0080, 0x0000, 0x0090};
    for (int i = 0; i < 5; i++) {
        u32 data;
        bool hit = cache_read(&cache1, addresses[i], &data, sizeof(data));
        cache_address_t parsed = cache_parse_address(&cache1, addresses[i]);
        printf("访问地址0x%04X: 标记=%d, 索引=%d, 偏移=%d, %s\n",
               addresses[i], parsed.tag, parsed.index, parsed.offset,
               hit ? "命中" : "未命中");
    }
    
    cache_stats_t stats1 = cache_get_stats(&cache1);
    printf("命中率: %.2f%%\n", stats1.hit_rate * 100);
    cache_destroy(&cache1);
    
    // 场景2：组相联缓存
    printf("\n【场景2】组相联缓存分析\n");
    cache_t cache2;
    cache_config_t config2 = {
        .size = 256,
        .line_size = 32,
        .associativity = 2,
        .mapping = CACHE_SET_ASSOCIATIVE,
        .replace = CACHE_REPLACE_LRU,
        .write_policy = CACHE_WRITE_BACK
    };
    
    cache_calculate_parameters(&config2);
    cache_init(&cache2, &config2);
    
    printf("缓存配置：大小=%d字节，行大小=%d字节，相联度=%d，组数=%d\n", 
           config2.size, config2.line_size, config2.associativity, config2.num_sets);
    
    cache_destroy(&cache2);
}

/**
 * 运行所有缓存测试
 */
void run_all_cache_tests(void) {
    printf("=== 缓存系统单元测试开始 ===\n");
    
    // 重置测试统计
    g_test_stats.total_tests = 0;
    g_test_stats.passed_tests = 0;
    g_test_stats.failed_tests = 0;
    
    // 运行所有测试
    test_cache_initialization();
    test_cache_config_validation();
    test_cache_basic_operations();
    test_cache_address_parsing();
    test_cache_statistics();
    
    // 显示测试结果
    printf("\n=== cache测试结果统计 ===\n");
    printf("总测试数: %d\n", g_test_stats.total_tests);
    printf("通过测试: %d\n", g_test_stats.passed_tests);
    printf("失败测试: %d\n", g_test_stats.failed_tests);
    printf("成功率: %.1f%%\n", 
           g_test_stats.total_tests > 0 ? 
           (float)g_test_stats.passed_tests / g_test_stats.total_tests * 100 : 0);
    
    if (g_test_stats.failed_tests == 0) {
        printf("🎉 所有缓存测试通过！\n");
    } else {
        printf("❌ 有 %d 个测试失败\n", g_test_stats.failed_tests);
    }
}

/**
 * 主函数
 */
int main(void) {
    printf("计算机组成原理模拟器 - 缓存系统单元测试\n");
    printf("==========================================\n");
    
    // 运行所有测试
    run_all_cache_tests();
    
    // 运行408演示
    demo_408_exam_scenarios();
    
    return g_test_stats.failed_tests == 0 ? 0 : 1;
} 