#ifndef CPU_SIM_MEMORY_H
#define CPU_SIM_MEMORY_H

#include "../common/types.h"

/**
 * CPU内存管理模块
 * 
 * 本模块负责CPU模拟器的内存管理，包括：
 * 1. 内存的初始化和清理
 * 2. 内存读写操作
 * 3. 内存保护和权限检查
 * 4. 内存对齐处理
 * 5. 调试和诊断功能
 */

// ==================== 内存结构定义 ====================

/**
 * 内存系统结构
 */
typedef struct {
    byte_t *data;           // 内存数据
    size_t size;            // 内存大小
    addr_t base_addr;       // 基地址
    u64 access_count;       // 访问计数
    u64 read_count;         // 读取计数
    u64 write_count;        // 写入计数
} memory_t;

// ==================== 内存初始化和管理 ====================

/**
 * 初始化内存系统
 */
cpu_error_t memory_init(memory_t *mem, size_t size);

/**
 * 清理内存系统，释放资源
 */
cpu_error_t memory_cleanup(memory_t *mem);

/**
 * 重置内存内容为零
 */
cpu_error_t memory_reset(memory_t *mem);

/**
 * 获取内存大小
 * @param memory 内存结构指针
 * @return 内存大小（字节数）
 */
size_t memory_get_size(const memory_t *memory);

/**
 * 检查内存地址是否有效
 * @param memory 内存结构指针
 * @param addr 要检查的地址
 * @param size 要访问的数据大小
 * @return 有效返回true，无效返回false
 */
bool memory_is_valid_address(const memory_t *memory, addr_t addr, size_t size);

// ==================== 内存读写操作 ====================

/**
 * 读取字节数据
 */
cpu_error_t memory_read_byte(memory_t *mem, addr_t addr, byte_t *value);

/**
 * 写入字节数据
 */
cpu_error_t memory_write_byte(memory_t *mem, addr_t addr, byte_t value);

/**
 * 读取16位数据
 */
cpu_error_t memory_read_halfword(memory_t *mem, addr_t addr, u16 *value);

/**
 * 写入16位数据
 */
cpu_error_t memory_write_halfword(memory_t *mem, addr_t addr, u16 value);

/**
 * 读取32位数据（一个机器字）
 */
cpu_error_t memory_read_word(memory_t *mem, addr_t addr, word_t *value);

/**
 * 写入32位数据（一个机器字）
 */
cpu_error_t memory_write_word(memory_t *mem, addr_t addr, word_t value);

/**
 * 批量读取数据
 */
cpu_error_t memory_read_block(memory_t *mem, addr_t addr, byte_t *buffer, size_t size);

/**
 * 批量写入数据
 */
cpu_error_t memory_write_block(memory_t *mem, addr_t addr, const byte_t *buffer, size_t size);

// ==================== 内存填充和比较 ====================

/**
 * 填充内存区域
 */
cpu_error_t memory_fill(memory_t *mem, addr_t addr, byte_t value, size_t size);

/**
 * 比较内存区域
 */
cpu_error_t memory_compare(memory_t *mem, addr_t addr1, addr_t addr2, size_t size, int *result);

// ==================== 内存状态显示 ====================

/**
 * 十六进制转储内存内容
 */
void memory_dump(const memory_t *mem, addr_t start_addr, size_t size);

/**
 * 显示内存统计信息
 */
void memory_dump_stats(const memory_t *mem);

// ==================== 内存验证 ====================

/**
 * 验证内存系统状态
 */
bool memory_validate(const memory_t *mem);

/**
 * 检查内存完整性
 */
cpu_error_t memory_check_integrity(const memory_t *mem);

// ==================== 内存映射管理 ====================

/**
 * 映射内存区域
 */
cpu_error_t memory_map_region(memory_t *mem, addr_t base_addr, size_t size);

/**
 * 检查地址是否已映射
 */
bool memory_is_mapped(const memory_t *mem, addr_t addr);

// ==================== 内存保护 ====================

/**
 * 设置内存保护
 */
cpu_error_t memory_set_protection(memory_t *mem, addr_t addr, size_t size, u32 protection);

/**
 * 检查内存访问权限
 */
bool memory_check_permission(const memory_t *mem, addr_t addr, u32 access_type);

// ==================== 程序加载功能 ====================

/**
 * 从文件加载程序到内存
 * @param memory 内存结构指针
 * @param filename 程序文件名
 * @param load_addr 加载地址
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_load_program(memory_t *memory, const char *filename, 
                                addr_t load_addr);

/**
 * 从字节数组加载程序到内存
 * @param memory 内存结构指针
 * @param program 程序字节数组
 * @param size 程序大小
 * @param load_addr 加载地址
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_load_program_from_bytes(memory_t *memory, 
                                           const byte_t *program, 
                                           size_t size, 
                                           addr_t load_addr);

/**
 * 保存内存内容到文件
 * @param memory 内存结构指针
 * @param filename 输出文件名
 * @param start_addr 起始地址
 * @param size 保存大小
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_save_to_file(const memory_t *memory, const char *filename, 
                                addr_t start_addr, size_t size);

// ==================== 栈操作功能 ====================

/**
 * 压栈操作（32位数据）
 * @param memory 内存结构指针
 * @param sp_reg 栈指针寄存器指针
 * @param value 要压栈的值
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_push_word32(memory_t *memory, word_t *sp_reg, word_t value);

/**
 * 出栈操作（32位数据）
 * @param memory 内存结构指针
 * @param sp_reg 栈指针寄存器指针
 * @param value 输出参数，存储出栈的值
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_pop_word32(memory_t *memory, word_t *sp_reg, word_t *value);

/**
 * 检查栈空间是否充足
 * @param memory 内存结构指针
 * @param sp_value 当前栈指针值
 * @param required_space 需要的栈空间（字节数）
 * @return 充足返回true，不足返回false
 */
bool memory_check_stack_space(const memory_t *memory, word_t sp_value, 
                              size_t required_space);

// ==================== 调试和诊断功能 ====================

/**
 * 打印内存区域内容（十六进制格式）
 * @param memory 内存结构指针
 * @param start_addr 起始地址
 * @param size 打印大小
 * @param bytes_per_line 每行显示的字节数
 */
void memory_dump_hex(const memory_t *memory, addr_t start_addr, 
                     size_t size, int bytes_per_line);

/**
 * 打印内存区域内容（反汇编格式）
 * @param memory 内存结构指针
 * @param start_addr 起始地址
 * @param size 打印大小
 */
void memory_dump_disasm(const memory_t *memory, addr_t start_addr, size_t size);

/**
 * 打印内存统计信息
 * @param memory 内存结构指针
 */
void memory_print_stats(const memory_t *memory);

/**
 * 查找内存中的字节模式
 * @param memory 内存结构指针
 * @param start_addr 搜索起始地址
 * @param end_addr 搜索结束地址
 * @param pattern 要查找的字节模式
 * @param pattern_size 模式大小
 * @param found_addr 输出参数，找到的地址
 * @return 找到返回CPU_SUCCESS，未找到返回错误码
 */
cpu_error_t memory_find_pattern(const memory_t *memory, 
                                addr_t start_addr, addr_t end_addr,
                                const byte_t *pattern, size_t pattern_size,
                                addr_t *found_addr);

/**
 * 设置内存访问断点
 * @param memory 内存结构指针
 * @param addr 断点地址
 * @param read_break 读断点标志
 * @param write_break 写断点标志
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_set_breakpoint(memory_t *memory, addr_t addr, 
                                  bool read_break, bool write_break);

/**
 * 清除内存访问断点
 * @param memory 内存结构指针
 * @param addr 断点地址
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_clear_breakpoint(memory_t *memory, addr_t addr);

// ==================== 性能监控 ====================

/**
 * 内存访问统计结构
 */
typedef struct {
    u64 read_count;         // 读操作计数
    u64 write_count;        // 写操作计数
    u64 byte_reads;         // 字节读取次数
    u64 byte_writes;        // 字节写入次数
    u64 word16_reads;       // 16位字读取次数
    u64 word16_writes;      // 16位字写入次数
    u64 word32_reads;       // 32位字读取次数
    u64 word32_writes;      // 32位字写入次数
    u64 alignment_errors;   // 对齐错误次数
    u64 invalid_accesses;   // 无效访问次数
} memory_stats_t;

/**
 * 获取内存访问统计信息
 * @param memory 内存结构指针
 * @param stats 输出参数，统计信息结构
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_get_stats(const memory_t *memory, memory_stats_t *stats);

/**
 * 重置内存访问统计
 * @param memory 内存结构指针
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_reset_stats(memory_t *memory);

// ==================== 测试和验证功能 ====================

/**
 * 内存系统自测试
 * @return 测试通过返回CPU_SUCCESS，否则返回错误码
 */
cpu_error_t memory_self_test(void);

/**
 * 内存填充测试
 * @param memory 内存结构指针
 * @param pattern 填充模式
 * @return 成功返回CPU_SUCCESS，失败返回错误码
 */
cpu_error_t memory_fill_test(memory_t *memory, byte_t pattern);

/**
 * 内存地址线测试
 * @param memory 内存结构指针
 * @return 测试通过返回CPU_SUCCESS，否则返回错误码
 */
cpu_error_t memory_address_test(memory_t *memory);

/**
 * 内存数据线测试
 * @param memory 内存结构指针
 * @return 测试通过返回CPU_SUCCESS，否则返回错误码
 */
cpu_error_t memory_data_test(memory_t *memory);

#endif // CPU_SIM_MEMORY_H 