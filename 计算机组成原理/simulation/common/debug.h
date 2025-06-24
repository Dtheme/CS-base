#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#include "types.h"
#include <stdio.h>

/**
 * 调试和日志系统
 * 
 * 提供统一的日志输出和调试功能，包括：
 * 1. 多级别日志输出
 * 2. 条件编译的调试信息
 * 3. 性能分析辅助
 * 4. 错误跟踪和报告
 */

// ==================== 日志级别定义 ====================

typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN  = 1,
    LOG_LEVEL_INFO  = 2,
    LOG_LEVEL_DEBUG = 3,
    LOG_LEVEL_TRACE = 4
} log_level_t;

// ==================== 日志配置 ====================

extern log_level_t current_log_level;

/**
 * 设置日志级别
 */
void set_log_level(log_level_t level);

/**
 * 获取当前日志级别
 */
log_level_t get_log_level(void);

// ==================== 日志宏定义 ====================

#define LOG_ERROR(fmt, ...) \
    do { \
        if (current_log_level >= LOG_LEVEL_ERROR) { \
            fprintf(stderr, "[ERROR] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

#define LOG_WARN(fmt, ...) \
    do { \
        if (current_log_level >= LOG_LEVEL_WARN) { \
            fprintf(stderr, "[WARN]  %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

#define LOG_INFO(fmt, ...) \
    do { \
        if (current_log_level >= LOG_LEVEL_INFO) { \
            printf("[INFO]  " fmt "\n", ##__VA_ARGS__); \
        } \
    } while(0)

#define LOG_DEBUG(fmt, ...) \
    do { \
        if (current_log_level >= LOG_LEVEL_DEBUG) { \
            printf("[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

#define LOG_TRACE(fmt, ...) \
    do { \
        if (current_log_level >= LOG_LEVEL_TRACE) { \
            printf("[TRACE] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        } \
    } while(0)

// ==================== 断言宏 ====================

#ifdef DEBUG
#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "[ASSERT] %s:%d: %s\n", __FILE__, __LINE__, message); \
            abort(); \
        } \
    } while(0)
#else
#define ASSERT(condition, message) ((void)0)
#endif

// ==================== 调试辅助宏 ====================

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) \
    printf("[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif

// ==================== 性能分析 ====================

/**
 * 性能计时器结构
 */
typedef struct {
    u64 start_time;
    u64 end_time;
    const char *name;
} perf_timer_t;

/**
 * 开始性能计时
 */
void perf_timer_start(perf_timer_t *timer, const char *name);

/**
 * 结束性能计时并输出结果
 */
void perf_timer_end(perf_timer_t *timer);

/**
 * 性能计时宏
 */
#define PERF_TIMER_START(name) \
    perf_timer_t timer_##name; \
    perf_timer_start(&timer_##name, #name)

#define PERF_TIMER_END(name) \
    perf_timer_end(&timer_##name)

// ==================== 内存调试 ====================

#ifdef DEBUG_MEMORY
/**
 * 调试版本的内存分配
 */
void* debug_malloc(size_t size, const char *file, int line);
void debug_free(void *ptr, const char *file, int line);

#define malloc(size) debug_malloc(size, __FILE__, __LINE__)
#define free(ptr) debug_free(ptr, __FILE__, __LINE__)
#endif

// ==================== 错误跟踪 ====================

/**
 * 错误信息结构
 */
typedef struct {
    int error_code;
    const char *error_message;
    const char *file;
    int line;
    u64 timestamp;
} error_info_t;

/**
 * 记录错误信息
 */
void record_error(int error_code, const char *message, const char *file, int line);

/**
 * 获取最后一个错误
 */
const error_info_t* get_last_error(void);

/**
 * 清除错误记录
 */
void clear_error(void);

/**
 * 错误记录宏
 */
#define RECORD_ERROR(code, message) \
    record_error(code, message, __FILE__, __LINE__)

// ==================== 十六进制转储 ====================

/**
 * 十六进制转储内存内容
 */
void hex_dump(const void *data, size_t size, addr_t base_addr);

/**
 * 格式化十六进制转储
 */
void hex_dump_formatted(const void *data, size_t size, addr_t base_addr, 
                        int bytes_per_line);

#endif // COMMON_DEBUG_H 