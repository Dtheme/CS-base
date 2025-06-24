#include "debug.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * 调试和日志系统实现
 * 
 * 本模块实现了分层式日志系统和性能分析工具
 * 核心功能：
 * 1. 分级日志输出 - 支持ERROR/WARN/INFO/DEBUG/TRACE五个级别
 * 2. 性能计时器 - 微秒级精度的执行时间测量
 * 3. 内存调试 - 内存分配跟踪和泄漏检测
 * 4. 错误跟踪 - 错误信息的集中记录和查询
 * 5. 十六进制转储 - 内存内容的可视化显示
 */

// ==================== 全局变量 ====================

log_level_t current_log_level = LOG_LEVEL_INFO;
static error_info_t last_error = {0};

// ==================== 日志级别管理 ====================

/**
 * 设置全局日志级别
 * 
 * 日志级别控制原理：
 * - 采用数值比较的方式过滤日志
 * - 只有级别 >= current_log_level 的日志才会输出
 * - 运行时可动态调整，便于调试和性能优化
 */
void set_log_level(log_level_t level) {
    current_log_level = level;
}

/**
 * 获取当前日志级别
 * 
 * 返回值说明：
 * - LOG_LEVEL_ERROR (0): 只输出错误信息
 * - LOG_LEVEL_WARN  (1): 输出警告及以上级别
 * - LOG_LEVEL_INFO  (2): 输出信息及以上级别
 * - LOG_LEVEL_DEBUG (3): 输出调试及以上级别
 * - LOG_LEVEL_TRACE (4): 输出所有级别信息
 */
log_level_t get_log_level(void) {
    return current_log_level;
}

// ==================== 性能分析 ====================

/**
 * 启动性能计时器
 * 
 * 计时原理：
 * 1. 记录当前微秒级时间戳作为起始时间
 * 2. 保存计时器名称用于结果输出
 * 3. 初始化结束时间为0表示计时中状态
 * 
 * 精度说明：
 * - 使用gettimeofday获取微秒级时间戳
 * - 理论精度1微秒，实际精度依赖系统
 * - 适用于函数级和代码块级性能分析
 */
void perf_timer_start(perf_timer_t *timer, const char *name) {
    if (!timer) return;
    
    timer->name = name;
    timer->start_time = get_timestamp_us();
    timer->end_time = 0;  // 标记计时器处于活动状态
}

/**
 * 结束性能计时并输出统计结果
 * 
 * 统计计算：
 * 1. 记录结束时间戳
 * 2. 计算执行时间差：end_time - start_time
 * 3. 格式化输出执行时间（微秒单位）
 * 
 * 输出格式：[PERF] 计时器名称: 执行时间 μs
 * 
 * 应用场景：
 * - 算法性能评估
 * - 系统瓶颈识别
 * - 优化效果验证
 */
void perf_timer_end(perf_timer_t *timer) {
    if (!timer) return;
    
    timer->end_time = get_timestamp_us();
    u64 duration = time_diff_us(timer->start_time, timer->end_time);
    
    if (current_log_level >= LOG_LEVEL_INFO) {
        printf("[PERF]  %s: %llu μs\n", timer->name ? timer->name : "Unknown", duration);
    }
}

// ==================== 内存调试 ====================

#ifdef DEBUG_MEMORY
/**
 * 内存使用统计变量
 * 
 * 跟踪机制：
 * - total_allocated: 累计分配的内存总量（字节）
 * - allocation_count: 当前未释放的分配次数
 * - 用于检测内存泄漏和过度分配
 */
static size_t total_allocated = 0;
static size_t allocation_count = 0;

/**
 * 调试版本的内存分配
 * 
 * 增强功能：
 * 1. 记录分配位置：文件名和行号
 * 2. 统计内存使用：累计分配量和分配次数
 * 3. 详细日志：分配地址、大小、位置信息
 * 
 * 调试信息格式：
 * [MALLOC] 地址: 大小 bytes at 文件:行号 (total: 累计大小)
 * 
 * 使用方法：
 * 通过宏重定义malloc，编译时添加-DDEBUG_MEMORY
 */
void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr) {
        total_allocated += size;
        allocation_count++;
        if (current_log_level >= LOG_LEVEL_DEBUG) {
            printf("[MALLOC] %p: %zu bytes at %s:%d (total: %zu)\n", 
                   ptr, size, file, line, total_allocated);
        }
    }
    return ptr;
}

/**
 * 调试版本的内存释放
 * 
 * 跟踪功能：
 * 1. 记录释放位置：文件名和行号
 * 2. 更新分配计数：递减未释放的分配次数
 * 3. 详细日志：释放地址、位置、剩余分配数
 * 
 * 调试信息格式：
 * [FREE] 地址 at 文件:行号 (remaining: 剩余分配数)
 * 
 * 内存泄漏检测：
 * 程序结束时allocation_count应为0
 */
void debug_free(void *ptr, const char *file, int line) {
    if (ptr) {
        allocation_count--;
        if (current_log_level >= LOG_LEVEL_DEBUG) {
            printf("[FREE]   %p at %s:%d (remaining: %zu)\n", 
                   ptr, file, line, allocation_count);
        }
        free(ptr);
    }
}
#endif

// ==================== 错误跟踪 ====================

/**
 * 记录错误信息到全局错误记录
 * 
 * 错误信息结构：
 * - error_code: 错误代码（通常对应cpu_error_t枚举）
 * - error_message: 错误描述字符串
 * - file: 发生错误的源文件名
 * - line: 发生错误的行号
 * - timestamp: 错误发生的时间戳（微秒级）
 * 
 * 设计理念：
 * 1. 单例模式：只保存最后一个错误，避免内存占用过多
 * 2. 即时输出：错误发生时立即输出到stderr
 * 3. 延迟查询：支持程序后续查询错误详情
 * 
 * 应用场景：
 * - 函数调用失败时记录详细错误信息
 * - 系统故障诊断和调试
 * - 错误传播和处理链追踪
 */
void record_error(int error_code, const char *message, const char *file, int line) {
    last_error.error_code = error_code;
    last_error.error_message = message;
    last_error.file = file;
    last_error.line = line;
    last_error.timestamp = get_timestamp_us();
    
    // 立即输出错误信息
    if (current_log_level >= LOG_LEVEL_ERROR) {
        fprintf(stderr, "[ERROR] Code %d: %s at %s:%d\n", 
                error_code, message, file, line);
    }
}

/**
 * 获取最后记录的错误信息
 * 
 * 返回值：指向全局错误记录的常量指针
 * 
 * 使用说明：
 * - 返回的指针在下次record_error调用前有效
 * - 用于错误处理函数查询详细错误信息
 * - 支持错误码、消息、位置和时间的完整查询
 */
const error_info_t* get_last_error(void) {
    return &last_error;
}

/**
 * 清除错误记录
 * 
 * 清除操作：
 * 将全局错误记录结构体清零
 * 
 * 使用场景：
 * - 错误处理完成后清除状态
 * - 新的操作开始前重置错误状态
 * - 避免陈旧错误信息的误导
 */
void clear_error(void) {
    memset(&last_error, 0, sizeof(last_error));
}

// ==================== 十六进制转储 ====================

/**
 * 标准格式十六进制转储
 * 
 * 调用关系：
 * hex_dump -> hex_dump_formatted (bytes_per_line=16)
 * 
 * 标准格式：每行16字节，符合传统的hexdump工具格式
 */
void hex_dump(const void *data, size_t size, addr_t base_addr) {
    hex_dump_formatted(data, size, base_addr, 16);
}

/**
 * 格式化十六进制转储
 * 
 * 输出格式解析：
 * 
 * 地址部分：8位十六进制地址 (base_addr + offset)
 * 十六进制部分：每字节2位十六进制 + 空格分隔
 * ASCII部分：可打印字符显示原字符，不可打印字符显示'.'
 * 
 * 示例输出：
 * 00001000: 48 65 6C 6C 6F 20 57 6F  72 6C 64 21 0A 00 00 00 |Hello World!....|
 * 
 * 格式说明：
 * - 地址：00001000
 * - 十六进制：48 65 6C 6C...（字节值）
 * - 分隔符：中间添加额外空格便于阅读
 * - ASCII：|Hello World!....| （可读字符区域）
 * 
 * 参数说明：
 * - data: 要转储的内存起始地址
 * - size: 转储的字节数
 * - base_addr: 显示的起始地址（可与data不同）
 * - bytes_per_line: 每行显示的字节数
 * 
 * 应用场景：
 * - 内存内容调试和分析
 * - 数据结构可视化
 * - 协议数据包分析
 * - 文件格式解析
 */
void hex_dump_formatted(const void *data, size_t size, addr_t base_addr, 
                        int bytes_per_line) {
    if (!data || size == 0 || bytes_per_line <= 0) return;
    
    const byte_t *bytes = (const byte_t*)data;
    
    for (size_t i = 0; i < size; i += bytes_per_line) {
        // 地址列：显示当前行的起始地址
        printf("%08X: ", (u32)(base_addr + i));
        
        // 十六进制数据列：逐字节显示十六进制值
        for (int j = 0; j < bytes_per_line; j++) {
            if (i + j < size) {
                printf("%02X "), bytes[i + j]);
            } else {
                printf("   ");  // 不足一行时填充空格
            }
            
            // 中间分隔：在一行的中间位置添加额外空格
            if (j == bytes_per_line / 2 - 1) {
                printf(" ");
            }
        }
        
        printf(" ┃ "));
        
        // ASCII字符列：显示可打印字符
        for (int j = 0; j < bytes_per_line && i + j < size; j++) {
            byte_t c = bytes[i + j];
            // 可打印ASCII字符范围：32-126 (空格到波浪号)
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        
        printf("|\n");
    }
} 