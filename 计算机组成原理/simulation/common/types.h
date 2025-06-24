#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

/**
 * 公共类型定义
 * 
 * 定义模拟器中使用的基本数据类型，包括：
 * 1. 基本整数类型
 * 2. 地址和指针类型
 * 3. 错误码类型
 * 4. 常用常量定义
 */

// ==================== 基本整数类型 ====================

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef uint8_t   byte_t;
typedef uint16_t  halfword_t;
typedef uint32_t  word_t;
typedef uint64_t  dword_t;

// ==================== 地址和指针类型 ====================

typedef u32 addr_t;     // 32位地址
typedef u32 offset_t;   // 地址偏移量

// ==================== 错误码类型 ====================

typedef enum {
    CPU_SUCCESS = 0,
    
    // 通用错误
    CPU_ERROR_NULL_POINTER = -1,
    CPU_ERROR_INVALID_PARAMETER = -2,
    CPU_ERROR_INVALID_PARAM = -2,  // 别名
    CPU_ERROR_OUT_OF_MEMORY = -3,
    CPU_ERROR_NO_MEMORY = -3,  // 别名
    CPU_ERROR_NOT_IMPLEMENTED = -4,
    CPU_ERROR_TIMEOUT = -5,
    CPU_ERROR_UNKNOWN = -6,
    
    // CPU相关错误
    CPU_ERROR_NOT_RUNNING = -10,
    CPU_ERROR_ALREADY_RUNNING = -11,
    CPU_ERROR_INVALID_STATE = -12,
    CPU_ERROR_HALT_CONDITION = -13,
    
    // 寄存器相关错误
    CPU_ERROR_INVALID_REGISTER = -20,
    CPU_ERROR_REGISTER_LOCKED = -21,
    
    // 内存相关错误
    CPU_ERROR_MEMORY_ACCESS_VIOLATION = -30,
    CPU_ERROR_MEMORY_ALIGNMENT = -31,
    CPU_ERROR_MEMORY_CORRUPTION = -32,
    CPU_ERROR_MEMORY_PROTECTION = -33,
    CPU_ERROR_INVALID_ADDRESS = -34,
    
    // 指令相关错误
    CPU_ERROR_INVALID_INSTRUCTION = -40,
    CPU_ERROR_INVALID_OPCODE = -41,
    CPU_ERROR_INVALID_ADDRESSING_MODE = -42,
    CPU_ERROR_INSTRUCTION_DECODE = -43,
    
    // ALU相关错误
    CPU_ERROR_DIVISION_BY_ZERO = -50,
    CPU_ERROR_ARITHMETIC_OVERFLOW = -51,
    CPU_ERROR_ARITHMETIC_UNDERFLOW = -52,
    
    // 栈相关错误
    CPU_ERROR_STACK_OVERFLOW = -60,
    CPU_ERROR_STACK_UNDERFLOW = -61,
    
    // I/O相关错误
    CPU_ERROR_IO_ERROR = -70,
    CPU_ERROR_DEVICE_NOT_FOUND = -71,
    CPU_ERROR_DEVICE_BUSY = -72,
    CPU_ERROR_NOT_READY = -73,
    CPU_ERROR_BUFFER_FULL = -74,
    CPU_ERROR_NOT_FOUND = -75,
    CPU_ERROR_PARAM_INVALID = -76,
    CPU_ERROR_BUFFER_OVERFLOW = -77,
    CPU_ERROR_BLOCKED = -78,
    CPU_ERROR_INIT_FAILED = -79,
    CPU_ERROR_NO_DATA = -80,
    
    // 文件相关错误  
    CPU_ERROR_FILE_NOT_FOUND = -90,
    CPU_ERROR_FILE_READ_ERROR = -91,
    CPU_ERROR_FILE_WRITE_ERROR = -92,
    CPU_ERROR_FILE_FORMAT_ERROR = -93
} cpu_error_t;

// ==================== 操作码定义 ====================

typedef enum {
    // 空操作
    OP_NOP = 0x00,
    
    // 数据传送指令
    OP_MOV = 0x10,
    OP_LOAD = 0x11,
    OP_STORE = 0x12,
    OP_PUSH = 0x13,
    OP_POP = 0x14,
    
    // 算术运算指令
    OP_ADD = 0x20,
    OP_SUB = 0x21,
    OP_MUL = 0x22,
    OP_DIV = 0x23,
    OP_INC = 0x24,
    OP_DEC = 0x25,
    OP_NEG = 0x26,
    
    // 逻辑运算指令
    OP_AND = 0x30,
    OP_OR = 0x31,
    OP_XOR = 0x32,
    OP_NOT = 0x33,
    OP_SHL = 0x34,
    OP_SHR = 0x35,
    
    // 比较指令
    OP_CMP = 0x40,
    OP_TEST = 0x41,
    
    // 跳转指令
    OP_JMP = 0x50,
    OP_JZ = 0x51,
    OP_JNZ = 0x52,
    OP_JC = 0x53,
    OP_JNC = 0x54,
    OP_JE = 0x55,
    OP_JNE = 0x56,
    OP_JL = 0x57,
    OP_JG = 0x58,
    
    // 子程序调用
    OP_CALL = 0x60,
    OP_RET = 0x61,
    
    // 系统指令
    OP_HLT = 0xFF,
    OP_INT = 0xFE,
    OP_IRET = 0xFD
} opcode_t;

// ==================== 寻址方式定义 ====================

typedef enum {
    ADDR_IMMEDIATE = 0,     // 立即寻址
    ADDR_REGISTER = 1,      // 寄存器寻址
    ADDR_DIRECT = 2,        // 直接寻址
    ADDR_INDIRECT = 3,      // 间接寻址
    ADDR_BASE = 4,          // 基址寻址
    ADDR_INDEX = 5,         // 变址寻址
    ADDR_BASE_INDEX = 6,    // 基址变址寻址
    ADDR_RELATIVE = 7       // 相对寻址
} addressing_mode_t;

// ==================== 指令格式定义 ====================

typedef enum {
    INST_FORMAT_16BIT = 0,
    INST_FORMAT_32BIT = 1
} instruction_format_t;

typedef enum {
    ADDR_ZERO = 0,
    ADDR_ONE = 1,
    ADDR_TWO = 2,
    ADDR_THREE = 3
} address_count_t;

// ==================== 常用常量定义 ====================

// 寄存器数量
#define MAX_REGISTERS 16

// 内存相关常量
#define MEMORY_ALIGNMENT 4
#define PAGE_SIZE 4096
#define CACHE_LINE_SIZE 64

// 栈相关常量
#define STACK_SIZE 1024
#define STACK_BASE_ADDR 0x10000
#define STACK_LIMIT_ADDR 0x0F000

// 标志位定义
#define FLAG_ZERO       0x00000001
#define FLAG_NEGATIVE   0x00000002
#define FLAG_CARRY      0x00000004
#define FLAG_OVERFLOW   0x00000008
#define FLAG_INTERRUPT  0x00000010

// ALU标志位定义
#define ALU_FLAG_ZERO       0x01
#define ALU_FLAG_NEGATIVE   0x02
#define ALU_FLAG_CARRY      0x04
#define ALU_FLAG_OVERFLOW   0x08

// 位操作宏
#define BIT(n)              (1U << (n))
#define SET_BIT(val, n)     ((val) |= BIT(n))
#define CLEAR_BIT(val, n)   ((val) &= ~BIT(n))
#define TEST_BIT(val, n)    (((val) & BIT(n)) != 0)

// 字节序转换宏（小端序）
#define SWAP16(x) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define SWAP32(x) ((((x) & 0xFF) << 24) | (((x) & 0xFF00) << 8) | \
                   (((x) & 0xFF0000) >> 8) | (((x) >> 24) & 0xFF))

// 对齐宏
#define ALIGN_UP(x, a)      (((x) + (a) - 1) & ~((a) - 1))
#define ALIGN_DOWN(x, a)    ((x) & ~((a) - 1))
#define IS_ALIGNED(x, a)    (((x) & ((a) - 1)) == 0)

// 数组大小宏
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))

// 最小/最大值宏
#define MIN(a, b)           ((a) < (b) ? (a) : (b))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))

// 范围检查宏
#define IN_RANGE(x, min, max) ((x) >= (min) && (x) <= (max))

#endif // COMMON_TYPES_H