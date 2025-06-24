#include "memory.h"
#include "../common/types.h"
#include "../common/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 内存系统实现
 * 
 * 本文件实现CPU模拟器的内存管理功能，包括：
 * 1. 内存的初始化和清理
 * 2. 内存的读写操作
 * 3. 内存保护和边界检查
 * 4. 内存状态的显示和调试
 */

// ==================== 内存系统管理 ====================

cpu_error_t memory_init(memory_t *mem, size_t size) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    if (size == 0) return CPU_ERROR_INVALID_PARAMETER;
    
    // 分配内存空间
    mem->data = (byte_t*)malloc(size);
    if (!mem->data) {
        LOG_ERROR("内存分配失败，大小: %zu字节", size);
        return CPU_ERROR_OUT_OF_MEMORY;
    }
    
    // 初始化内存参数
    mem->size = size;
    mem->base_addr = 0;
    mem->access_count = 0;
    mem->read_count = 0;
    mem->write_count = 0;
    
    // 清零内存内容
    memset(mem->data, 0, size);
    
    LOG_INFO("内存系统初始化完成，大小: %zu字节", size);
    return CPU_SUCCESS;
}

cpu_error_t memory_cleanup(memory_t *mem) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    
    if (mem->data) {
        free(mem->data);
        mem->data = NULL;
    }
    
    mem->size = 0;
    mem->base_addr = 0;
    mem->access_count = 0;
    mem->read_count = 0;
    mem->write_count = 0;
    
    LOG_INFO("内存系统已清理");
    return CPU_SUCCESS;
}

cpu_error_t memory_reset(memory_t *mem) {
    if (!mem || !mem->data) return CPU_ERROR_NULL_POINTER;
    
    // 清零内存内容
    memset(mem->data, 0, mem->size);
    
    // 重置统计计数器
    mem->access_count = 0;
    mem->read_count = 0;
    mem->write_count = 0;
    
    LOG_INFO("内存系统已重置");
    return CPU_SUCCESS;
}

// ==================== 地址检查 ====================

static bool is_valid_address(const memory_t *mem, addr_t addr, size_t access_size) {
    if (!mem || !mem->data) return false;
    
    // 检查地址是否在有效范围内
    if (addr < mem->base_addr) return false;
    if (addr + access_size > mem->base_addr + mem->size) return false;
    
    return true;
}

static size_t get_memory_offset(const memory_t *mem, addr_t addr) {
    return addr - mem->base_addr;
}

// ==================== 字节级内存操作 ====================

cpu_error_t memory_read_byte(memory_t *mem, addr_t addr, byte_t *value) {
    if (!mem || !value) return CPU_ERROR_NULL_POINTER;
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, sizeof(byte_t))) {
        LOG_ERROR("内存读取地址无效: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 读取数据
    size_t offset = get_memory_offset(mem, addr);
    *value = mem->data[offset];
    
    // 更新统计
    mem->access_count++;
    mem->read_count++;
    
    LOG_TRACE("内存读取: 地址=0x%08X, 值=0x%02X", addr, *value);
    return CPU_SUCCESS;
}

cpu_error_t memory_write_byte(memory_t *mem, addr_t addr, byte_t value) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, sizeof(byte_t))) {
        LOG_ERROR("内存写入地址无效: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 写入数据
    size_t offset = get_memory_offset(mem, addr);
    mem->data[offset] = value;
    
    // 更新统计
    mem->access_count++;
    mem->write_count++;
    
    LOG_TRACE("内存写入: 地址=0x%08X, 值=0x%02X", addr, value);
    return CPU_SUCCESS;
}

// ==================== 半字级内存操作 ====================

cpu_error_t memory_read_halfword(memory_t *mem, addr_t addr, u16 *value) {
    if (!mem || !value) return CPU_ERROR_NULL_POINTER;
    
    // 地址对齐检查
    if (addr & 0x1) {
        LOG_ERROR("半字读取地址未对齐: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ALIGNMENT;
    }
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, sizeof(u16))) {
        LOG_ERROR("内存读取地址无效: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 读取数据（小端序）
    size_t offset = get_memory_offset(mem, addr);
    *value = mem->data[offset] | (mem->data[offset + 1] << 8);
    
    // 更新统计
    mem->access_count++;
    mem->read_count++;
    
    LOG_TRACE("内存读取半字: 地址=0x%08X, 值=0x%04X", addr, *value);
    return CPU_SUCCESS;
}

cpu_error_t memory_write_halfword(memory_t *mem, addr_t addr, u16 value) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    
    // 地址对齐检查
    if (addr & 0x1) {
        LOG_ERROR("半字写入地址未对齐: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ALIGNMENT;
    }
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, sizeof(u16))) {
        LOG_ERROR("内存写入地址无效: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 写入数据（小端序）
    size_t offset = get_memory_offset(mem, addr);
    mem->data[offset] = value & 0xFF;
    mem->data[offset + 1] = (value >> 8) & 0xFF;
    
    // 更新统计
    mem->access_count++;
    mem->write_count++;
    
    LOG_TRACE("内存写入半字: 地址=0x%08X, 值=0x%04X", addr, value);
    return CPU_SUCCESS;
}

// ==================== 字级内存操作 ====================

cpu_error_t memory_read_word(memory_t *mem, addr_t addr, word_t *value) {
    if (!mem || !value) return CPU_ERROR_NULL_POINTER;
    
    // 地址对齐检查
    if (addr & 0x3) {
        LOG_ERROR("字读取地址未对齐: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ALIGNMENT;
    }
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, sizeof(word_t))) {
        LOG_ERROR("内存读取地址无效: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 读取数据（小端序）
    size_t offset = get_memory_offset(mem, addr);
    *value = mem->data[offset] |
             (mem->data[offset + 1] << 8) |
             (mem->data[offset + 2] << 16) |
             (mem->data[offset + 3] << 24);
    
    // 更新统计
    mem->access_count++;
    mem->read_count++;
    
    LOG_TRACE("内存读取字: 地址=0x%08X, 值=0x%08X", addr, *value);
    return CPU_SUCCESS;
}

cpu_error_t memory_write_word(memory_t *mem, addr_t addr, word_t value) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    
    // 地址对齐检查
    if (addr & 0x3) {
        LOG_ERROR("字写入地址未对齐: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ALIGNMENT;
    }
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, sizeof(word_t))) {
        LOG_ERROR("内存写入地址无效: 0x%08X", addr);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 写入数据（小端序）
    size_t offset = get_memory_offset(mem, addr);
    mem->data[offset] = value & 0xFF;
    mem->data[offset + 1] = (value >> 8) & 0xFF;
    mem->data[offset + 2] = (value >> 16) & 0xFF;
    mem->data[offset + 3] = (value >> 24) & 0xFF;
    
    // 更新统计
    mem->access_count++;
    mem->write_count++;
    
    LOG_TRACE("内存写入字: 地址=0x%08X, 值=0x%08X", addr, value);
    return CPU_SUCCESS;
}

// ==================== 块级内存操作 ====================

cpu_error_t memory_read_block(memory_t *mem, addr_t addr, byte_t *buffer, size_t size) {
    if (!mem || !buffer) return CPU_ERROR_NULL_POINTER;
    if (size == 0) return CPU_SUCCESS;
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, size)) {
        LOG_ERROR("内存块读取地址无效: 0x%08X, 大小: %zu", addr, size);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 读取数据块
    size_t offset = get_memory_offset(mem, addr);
    memcpy(buffer, &mem->data[offset], size);
    
    // 更新统计
    mem->access_count++;
    mem->read_count++;
    
    LOG_TRACE("内存块读取: 地址=0x%08X, 大小=%zu", addr, size);
    return CPU_SUCCESS;
}

cpu_error_t memory_write_block(memory_t *mem, addr_t addr, const byte_t *buffer, size_t size) {
    if (!mem || !buffer) return CPU_ERROR_NULL_POINTER;
    if (size == 0) return CPU_SUCCESS;
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, size)) {
        LOG_ERROR("内存块写入地址无效: 0x%08X, 大小: %zu", addr, size);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 写入数据块
    size_t offset = get_memory_offset(mem, addr);
    memcpy(&mem->data[offset], buffer, size);
    
    // 更新统计
    mem->access_count++;
    mem->write_count++;
    
    LOG_TRACE("内存块写入: 地址=0x%08X, 大小=%zu", addr, size);
    return CPU_SUCCESS;
}

// ==================== 内存填充和比较 ====================

cpu_error_t memory_fill(memory_t *mem, addr_t addr, byte_t value, size_t size) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    if (size == 0) return CPU_SUCCESS;
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr, size)) {
        LOG_ERROR("内存填充地址无效: 0x%08X, 大小: %zu", addr, size);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 填充内存
    size_t offset = get_memory_offset(mem, addr);
    memset(&mem->data[offset], value, size);
    
    // 更新统计
    mem->access_count++;
    mem->write_count++;
    
    LOG_TRACE("内存填充: 地址=0x%08X, 值=0x%02X, 大小=%zu", addr, value, size);
    return CPU_SUCCESS;
}

cpu_error_t memory_compare(memory_t *mem, addr_t addr1, addr_t addr2, size_t size, int *result) {
    if (!mem || !result) return CPU_ERROR_NULL_POINTER;
    if (size == 0) {
        *result = 0;
        return CPU_SUCCESS;
    }
    
    // 地址有效性检查
    if (!is_valid_address(mem, addr1, size) || !is_valid_address(mem, addr2, size)) {
        LOG_ERROR("内存比较地址无效: 0x%08X, 0x%08X, 大小: %zu", addr1, addr2, size);
        return CPU_ERROR_MEMORY_ACCESS_VIOLATION;
    }
    
    // 比较内存内容
    size_t offset1 = get_memory_offset(mem, addr1);
    size_t offset2 = get_memory_offset(mem, addr2);
    *result = memcmp(&mem->data[offset1], &mem->data[offset2], size);
    
    // 更新统计
    mem->access_count += 2;
    mem->read_count += 2;
    
    LOG_TRACE("内存比较: 地址1=0x%08X, 地址2=0x%08X, 大小=%zu, 结果=%d", 
              addr1, addr2, size, *result);
    return CPU_SUCCESS;
}

// ==================== 内存状态显示 ====================

void memory_dump(const memory_t *mem, addr_t start_addr, size_t size) {
    if (!mem || !mem->data) {
        printf("内存系统未初始化\n");
        return;
    }
    
    // 限制显示大小
    if (size > 256) size = 256;
    
    printf("内存转储 (地址: 0x%08X, 大小: %zu字节):\n", start_addr, size);
    
    for (size_t i = 0; i < size; i += 16) {
        printf("┃ %08X ┃ ", (u32)(start_addr + i));
        
        // 十六进制显示
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            if (is_valid_address(mem, start_addr + i + j, 1)) {
                size_t offset = get_memory_offset(mem, start_addr + i + j);
                printf("%02X "), mem->data[offset]);
            } else {
                printf("?? ");
            }
        }
        
        // 填充空格
        for (size_t j = size - i; j < 16; j++) {
            printf("   ");
        }
        
        printf(" ┃ "));
        
        // ASCII显示
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            if (is_valid_address(mem, start_addr + i + j, 1)) {
                size_t offset = get_memory_offset(mem, start_addr + i + j);
                byte_t c = mem->data[offset];
                printf("%c", (c >= 32 && c <= 126) ? c : '.');
            } else {
                printf("?");
            }
        }
        
        printf("|\n");
    }
}

void memory_dump_stats(const memory_t *mem) {
    if (!mem) {
        printf("内存系统未初始化\n");
        return;
    }
    
    printf("内存系统统计:\n");
    printf("  总大小: %zu字节 (%.2f KB)\n", mem->size, mem->size / 1024.0);
    printf("  基地址: 0x%08X\n", mem->base_addr);
    printf("  总访问次数: %llu\n", mem->access_count);
    printf("  读取次数: %llu\n", mem->read_count);
    printf("  写入次数: %llu\n", mem->write_count);
    
    if (mem->access_count > 0) {
        printf("  读写比例: %.2f%%读取, %.2f%%写入\n",
               (double)mem->read_count / mem->access_count * 100,
               (double)mem->write_count / mem->access_count * 100);
    }
}

// ==================== 内存验证 ====================

bool memory_validate(const memory_t *mem) {
    if (!mem) return false;
    if (!mem->data && mem->size > 0) return false;
    if (mem->data && mem->size == 0) return false;
    
    return true;
}

cpu_error_t memory_check_integrity(const memory_t *mem) {
    if (!memory_validate(mem)) {
        LOG_ERROR("内存系统完整性检查失败");
        return CPU_ERROR_MEMORY_CORRUPTION;
    }
    
    LOG_DEBUG("内存系统完整性检查通过");
    return CPU_SUCCESS;
}

// ==================== 内存映射管理 ====================

cpu_error_t memory_map_region(memory_t *mem, addr_t base_addr, size_t size) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    
    // 简化实现：只支持单一内存区域
    mem->base_addr = base_addr;
    
    LOG_INFO("内存区域映射: 基地址=0x%08X, 大小=%zu", base_addr, size);
    return CPU_SUCCESS;
}

bool memory_is_mapped(const memory_t *mem, addr_t addr) {
    if (!mem) return false;
    
    return is_valid_address(mem, addr, 1);
}

// ==================== 内存保护 ====================

cpu_error_t memory_set_protection(memory_t *mem, addr_t addr, size_t size, u32 protection) {
    if (!mem) return CPU_ERROR_NULL_POINTER;
    
    // 简化实现：不支持内存保护
    (void)addr;
    (void)size;
    (void)protection;
    
    LOG_DEBUG("内存保护设置（简化实现）");
    return CPU_SUCCESS;
}

bool memory_check_permission(const memory_t *mem, addr_t addr, u32 access_type) {
    if (!mem) return false;
    
    // 简化实现：总是允许访问
    (void)addr;
    (void)access_type;
    
    return true;
} 