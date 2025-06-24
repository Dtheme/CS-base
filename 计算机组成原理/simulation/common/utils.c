#include "utils.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/stat.h>

/**
 * 公共工具函数实现
 * 
 * 本文件实现了计算机组成原理模拟器中常用的工具函数
 * 涵盖字符串处理、数值转换、时间测量、文件操作等核心功能
 */

// ==================== 字符串处理 ====================

/**
 * 安全的字符串复制
 * 
 * 实现原理：
 * 1. 使用strncpy限制复制长度，防止缓冲区溢出
 * 2. 强制在目标字符串末尾添加'\0'终结符
 * 3. 确保即使源字符串过长也不会导致内存越界
 * 
 * 安全机制：
 * - 参数有效性检查
 * - 长度边界控制
 * - 强制null终结
 */
void safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return;
    
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';  // 强制null终结，防止字符串未终结
}

/**
 * 安全的字符串连接
 * 
 * 实现原理：
 * 1. 计算目标字符串当前长度
 * 2. 检查剩余空间是否足够
 * 3. 使用strncat安全连接，保留空间给null终结符
 * 
 * 防护措施：
 * - 检查目标缓冲区剩余空间
 * - 防止字符串溢出
 * - 保持字符串的完整性
 */
void safe_strcat(char *dest, const char *src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return;
    
    size_t dest_len = strlen(dest);
    if (dest_len >= dest_size - 1) return;  // 无剩余空间
    
    strncat(dest, src, dest_size - dest_len - 1);
}

/**
 * 去除字符串首尾空白字符
 * 
 * 算法流程：
 * 1. 前向扫描：跳过字符串开头的所有空白字符
 * 2. 后向扫描：从字符串末尾向前查找非空白字符
 * 3. 截断处理：在最后一个非空白字符后插入null终结符
 * 
 * 时间复杂度：O(n)，其中n为字符串长度
 * 空间复杂度：O(1)，原地修改
 */
char* trim_whitespace(char *str) {
    if (!str) return NULL;
    
    // 前向扫描：跳过前导空白字符
    while (isspace(*str)) str++;
    
    if (*str == '\0') return str;  // 字符串全为空白
    
    // 后向扫描：定位最后一个非空白字符
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    
    *(end + 1) = '\0';  // 截断尾随空白
    return str;
}

// ==================== 数值转换 ====================

/**
 * 十六进制字符串转32位无符号整数
 * 
 * 转换原理：
 * 1. 使用strtoul进行基数16的字符串解析
 * 2. 检查转换是否完全成功（endptr指向字符串末尾）
 * 3. 验证结果是否在32位无符号整数范围内
 * 
 * 错误处理：
 * - 无效字符检测
 * - 数值溢出检测
 * - 格式错误检测
 */
bool hex_to_uint32(const char *hex_str, u32 *value) {
    if (!hex_str || !value) return false;
    
    char *endptr;
    unsigned long result = strtoul(hex_str, &endptr, 16);
    
    // 检查转换完整性和数值范围
    if (*endptr != '\0' || result > UINT32_MAX) {
        return false;
    }
    
    *value = (u32)result;
    return true;
}

/**
 * 32位无符号整数转十六进制字符串
 * 
 * 格式化规则：
 * - 固定8位宽度输出
 * - 大写字母表示
 * - 前导零填充
 */
void uint32_to_hex(u32 value, char *hex_str, size_t str_size) {
    if (!hex_str || str_size < 9) return;  // 需要8位字符+null终结符
    
    snprintf(hex_str, str_size, "%08X", value);
}

/**
 * 二进制字符串转32位无符号整数
 * 
 * 转换算法：
 * 1. 逐位扫描二进制字符串
 * 2. 使用左移操作构建数值：result = result * 2 + bit_value
 * 3. 验证每个字符是否为有效的二进制数字（0或1）
 * 
 * 位运算优化：
 * - 左移1位等效于乘以2
 * - 避免浮点运算，提高效率
 */
bool bin_to_uint32(const char *bin_str, u32 *value) {
    if (!bin_str || !value) return false;
    
    u32 result = 0;
    size_t len = strlen(bin_str);
    
    if (len > 32) return false;  // 超出32位限制
    
    for (size_t i = 0; i < len; i++) {
        if (bin_str[i] != '0' && bin_str[i] != '1') {
            return false;  // 无效的二进制字符
        }
        result = (result << 1) + (bin_str[i] - '0');  // 左移并加入新位
    }
    
    *value = result;
    return true;
}

/**
 * 32位无符号整数转二进制字符串
 * 
 * 位提取算法：
 * 1. 从最高位开始逐位检查
 * 2. 使用位掩码(1U << i)提取第i位
 * 3. 根据位值设置字符'0'或'1'
 * 
 * 位运算原理：
 * - (value >> i) & 1：提取第i位的值
 * - 从高位到低位遍历确保正确的位序
 */
void uint32_to_bin(u32 value, char *bin_str, size_t str_size) {
    if (!bin_str || str_size < 33) return;  // 需要32位字符+null终结符
    
    for (int i = 31; i >= 0; i--) {
        bin_str[31 - i] = ((value >> i) & 1) ? '1' : '0';
    }
    bin_str[32] = '\0';
}

// ==================== 时间测量 ====================

/**
 * 获取微秒级时间戳
 * 
 * 系统调用原理：
 * 1. gettimeofday获取当前时间的秒和微秒部分
 * 2. 转换为统一的微秒时间戳：秒*1000000 + 微秒
 * 3. 返回64位时间戳以避免2038年问题
 * 
 * 精度说明：
 * - 理论精度：1微秒
 * - 实际精度：依赖系统时钟频率
 */
u64 get_timestamp_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u64)tv.tv_sec * 1000000 + tv.tv_usec;
}

/**
 * 获取毫秒级时间戳
 * 
 * 实现方式：复用微秒时间戳并除以1000
 * 优点：保持时间源的一致性
 */
u64 get_timestamp_ms(void) {
    return get_timestamp_us() / 1000;
}

/**
 * 计算时间差（微秒）
 * 
 * 安全计算：
 * - 检查时间顺序，防止负值结果
 * - 处理时间回退情况
 */
u64 time_diff_us(u64 start_time, u64 end_time) {
    return end_time > start_time ? end_time - start_time : 0;
}

/**
 * 获取当前时间（秒级）
 * 
 * 用途：适用于不需要高精度的场景
 * 返回：Unix时间戳（秒）
 */
u32 get_current_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (u32)tv.tv_sec;
}

// ==================== 文件操作 ====================

/**
 * 检查文件是否存在
 * 
 * 系统调用：使用stat获取文件状态
 * 判断依据：stat调用成功即表示文件存在
 * 优点：不需要实际打开文件，开销较小
 */
bool file_exists(const char *filename) {
    if (!filename) return false;
    
    struct stat st;
    return stat(filename, &st) == 0;
}

/**
 * 获取文件大小
 * 
 * 实现方法：
 * 1. 使用stat系统调用获取文件信息
 * 2. 从struct stat中提取st_size字段
 * 3. 错误时返回0
 */
size_t get_file_size(const char *filename) {
    if (!filename) return 0;
    
    struct stat st;
    if (stat(filename, &st) != 0) return 0;
    
    return st.st_size;
}

/**
 * 读取整个文件到内存缓冲区
 * 
 * 算法流程：
 * 1. 文件操作：打开文件并获取大小
 * 2. 内存管理：分配足够大小的缓冲区
 * 3. 数据读取：一次性读取全部文件内容
 * 4. 完整性验证：检查读取字节数是否与文件大小匹配
 * 
 * 错误处理：
 * - 文件打开失败
 * - 内存分配失败
 * - 读取不完整
 * 
 * 注意事项：
 * - 适用于小到中等大小的文件
 * - 大文件可能导致内存不足
 */
bool read_file_to_buffer(const char *filename, byte_t **buffer, size_t *size) {
    if (!filename || !buffer || !size) return false;
    
    FILE *file = fopen(filename, "rb");
    if (!file) return false;
    
    // 获取文件大小：移动到文件末尾并记录位置
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);  // 重置到文件开头
    
    if (file_size <= 0) {
        fclose(file);
        return false;
    }
    
    // 分配内存缓冲区
    *buffer = (byte_t*)malloc(file_size);
    if (!*buffer) {
        fclose(file);
        return false;
    }
    
    // 读取文件内容
    size_t bytes_read = fread(*buffer, 1, file_size, file);
    fclose(file);
    
    // 验证读取完整性
    if (bytes_read != (size_t)file_size) {
        free(*buffer);
        *buffer = NULL;
        return false;
    }
    
    *size = bytes_read;
    return true;
}

// ==================== 内存操作 ====================

/**
 * 安全的内存分配
 * 
 * 安全机制：
 * 1. 零大小检查：防止无意义的分配请求
 * 2. 失败处理：malloc失败时终止程序运行
 * 3. 错误日志：记录分配失败的详细信息
 * 
 * 设计理念：
 * - 快速失败：内存不足时立即终止而非返回错误
 * - 简化错误处理：调用者无需检查返回值
 */
void* safe_malloc(size_t size) {
    if (size == 0) return NULL;
    
    void *ptr = malloc(size);
    if (!ptr) {
        LOG_ERROR("内存分配失败，大小: %zu", size);
        exit(EXIT_FAILURE);  // 内存不足时终止程序
    }
    
    return ptr;
}

/**
 * 安全的内存重分配
 * 
 * 处理逻辑：
 * 1. 零大小处理：等同于free操作
 * 2. 失败保护：重分配失败时原指针仍然有效
 * 3. 错误终止：内存不足时终止程序
 */
void* safe_realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        LOG_ERROR("内存重分配失败，大小: %zu", size);
        exit(EXIT_FAILURE);
    }
    
    return new_ptr;
}

/**
 * 安全的内存释放
 * 
 * 双重安全机制：
 * 1. 空指针检查：防止释放NULL指针
 * 2. 指针置空：防止悬空指针的使用
 * 
 * 用法：safe_free((void**)&ptr);
 * 效果：释放内存并将ptr设置为NULL
 */
void safe_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;  // 防止悬空指针
    }
}

// ==================== 数学工具 ====================

/**
 * 判断是否为2的幂次
 * 
 * 位运算技巧：
 * 对于2的幂次数值n，其二进制表示中只有一个位为1
 * 因此：n & (n-1) == 0
 * 
 * 原理解释：
 * - 2的幂次：1000...0（二进制）
 * - n-1：    0111...1（二进制）
 * - n & (n-1) = 0
 * 
 * 特殊情况：0不是2的幂次
 */
bool is_power_of_two(u32 value) {
    return value > 0 && (value & (value - 1)) == 0;
}

/**
 * 向上对齐到指定边界
 * 
 * 对齐算法：
 * 1. 加上(alignment-1)使数值跨越边界
 * 2. 使用掩码清除低位，实现向下对齐
 * 3. 结果即为向上对齐的值
 * 
 * 位运算实现：
 * - (value + alignment - 1)：确保跨越边界
 * - & ~(alignment - 1)：清除低位实现对齐
 * 
 * 前提条件：alignment必须是2的幂次
 */
u32 align_up(u32 value, u32 alignment) {
    if (!is_power_of_two(alignment)) return value;
    
    return (value + alignment - 1) & ~(alignment - 1);
}

/**
 * 向下对齐到指定边界
 * 
 * 对齐原理：
 * 使用掩码清除低位，保留高位对齐部分
 * 
 * 位运算：
 * - ~(alignment - 1)：创建对齐掩码
 * - value & mask：保留对齐位，清除非对齐位
 */
u32 align_down(u32 value, u32 alignment) {
    if (!is_power_of_two(alignment)) return value;
    
    return value & ~(alignment - 1);
}

// ==================== 位操作 ====================

/**
 * 计算32位整数中设置的位数（汉明重量）
 * 
 * 算法：逐位检查法
 * 1. 检查最低位：value & 1
 * 2. 右移一位：value >>= 1
 * 3. 重复直到value为0
 * 
 * 时间复杂度：O(log n)，n为数值大小
 * 优化版本可使用Brian Kernighan算法：O(设置位数)
 */
int count_set_bits(u32 value) {
    int count = 0;
    while (value) {
        count += value & 1;  // 检查最低位
        value >>= 1;         // 右移检查下一位
    }
    return count;
}

/**
 * 查找最高位（最左边的1）
 * 
 * 搜索算法：
 * 1. 从最高位（第31位）开始检查
 * 2. 使用位掩码(1U << bit)检查指定位
 * 3. 返回第一个发现的1的位置
 * 
 * 返回值：
 * - 找到：位位置（0-31）
 * - 未找到：-1（输入为0）
 */
int find_highest_bit(u32 value) {
    if (value == 0) return -1;
    
    int bit = 31;
    while (bit >= 0 && !(value & (1U << bit))) {
        bit--;
    }
    return bit;
}

/**
 * 查找最低位（最右边的1）
 * 
 * 搜索算法：
 * 1. 从最低位（第0位）开始检查
 * 2. 使用位掩码(1U << bit)检查指定位
 * 3. 返回第一个发现的1的位置
 * 
 * 优化方法：可使用value & (-value)直接获取最低位
 */
int find_lowest_bit(u32 value) {
    if (value == 0) return -1;
    
    int bit = 0;
    while (bit < 32 && !(value & (1U << bit))) {
        bit++;
    }
    return bit < 32 ? bit : -1;
} 