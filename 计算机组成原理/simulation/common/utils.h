#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include "types.h"

/**
 * 公共工具函数
 * 
 * 提供模拟器通用的工具函数，包括：
 * 1. 字符串处理函数
 * 2. 数值转换函数
 * 3. 时间和性能测量
 * 4. 文件操作辅助函数
 */

// ==================== 字符串处理 ====================

/**
 * 安全的字符串复制
 */
void safe_strcpy(char *dest, const char *src, size_t dest_size);

/**
 * 安全的字符串连接
 */
void safe_strcat(char *dest, const char *src, size_t dest_size);

/**
 * 去除字符串首尾空白字符
 */
char* trim_whitespace(char *str);

// ==================== 数值转换 ====================

/**
 * 十六进制字符串转数值
 */
bool hex_to_uint32(const char *hex_str, u32 *value);

/**
 * 数值转十六进制字符串
 */
void uint32_to_hex(u32 value, char *hex_str, size_t str_size);

/**
 * 二进制字符串转数值
 */
bool bin_to_uint32(const char *bin_str, u32 *value);

/**
 * 数值转二进制字符串
 */
void uint32_to_bin(u32 value, char *bin_str, size_t str_size);

// ==================== 时间测量 ====================

/**
 * 获取当前时间戳（微秒）
 */
u64 get_timestamp_us(void);

/**
 * 获取当前时间戳（毫秒）
 */
u64 get_timestamp_ms(void);

/**
 * 计算时间差（微秒）
 */
u64 time_diff_us(u64 start_time, u64 end_time);

/**
 * 获取当前时间（秒级时间戳）
 */
u32 get_current_time(void);

// ==================== 文件操作 ====================

/**
 * 检查文件是否存在
 */
bool file_exists(const char *filename);

/**
 * 获取文件大小
 */
size_t get_file_size(const char *filename);

/**
 * 读取整个文件到缓冲区
 */
bool read_file_to_buffer(const char *filename, byte_t **buffer, size_t *size);

// ==================== 内存操作 ====================

/**
 * 安全的内存分配
 */
void* safe_malloc(size_t size);

/**
 * 安全的内存重分配
 */
void* safe_realloc(void *ptr, size_t size);

/**
 * 安全的内存释放
 */
void safe_free(void **ptr);

// ==================== 数学工具 ====================

/**
 * 计算2的幂次
 */
bool is_power_of_two(u32 value);

/**
 * 向上对齐到指定边界
 */
u32 align_up(u32 value, u32 alignment);

/**
 * 向下对齐到指定边界
 */
u32 align_down(u32 value, u32 alignment);

// ==================== 位操作 ====================

/**
 * 计算设置的位数
 */
int count_set_bits(u32 value);

/**
 * 查找最高位
 */
int find_highest_bit(u32 value);

/**
 * 查找最低位
 */
int find_lowest_bit(u32 value);

#endif // COMMON_UTILS_H 