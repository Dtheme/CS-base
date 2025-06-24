/* alu.c - 算术逻辑单元实现
 * 计算机组成原理模拟器 - CPU模拟器模块
 * 遵循数据结构算法编码标准
 * create by: zw.duan
 */

#include "alu.h"
#include "../common/debug.h"
#include <string.h>

/**
 * ALU (算术逻辑单元) 实现
 * 
 * 本模块实现了完整的32位ALU功能，模拟真实处理器中的算术逻辑单元
 * 
 * 实现特点：
 * 1. 硬件级精度 - 严格按照二进制补码运算规则实现
 * 2. 完整标志位 - 实现ZNCO四个标志位的精确计算
 * 3. 异常处理 - 对除零、溢出等异常情况进行适当处理
 * 4. 性能优化 - 使用64位中间结果避免精度损失
 * 5. 调试支持 - 提供丰富的状态输出和验证功能
 * 
 * 标志位计算原理：
 * - Zero (Z): result == 0，所有位都为0
 * - Negative (N): result[31] == 1，符号位检测
 * - Carry (C): 无符号运算的进位/借位
 * - Overflow (V): 有符号运算的溢出检测
 */

// ==================== 辅助函数 ====================

/**
 * 创建有效的ALU运算结果
 * 
 * 结果封装：
 * 将计算结果和标志位封装成统一的结果结构
 * 标记结果为有效状态，便于后续处理
 * 
 * @param result 32位运算结果
 * @param flags 8位标志位组合
 * @return 封装后的ALU结果结构
 */
static alu_result_t create_result(word_t result, u8 flags) {
    alu_result_t res;
    res.result = result;
    res.flags = flags;
    res.valid = true;
    return res;
}

/**
 * 创建错误状态的ALU结果
 * 
 * 错误处理：
 * 在运算异常（如除零）时返回无效结果
 * 清零所有数值字段，标记为无效状态
 * 
 * @return 标记为无效的ALU结果结构
 */
static alu_result_t create_error_result(void) {
    alu_result_t res;
    res.result = 0;
    res.flags = 0;
    res.valid = false;
    return res;
}

// ==================== ALU生命周期管理 ====================

/**
 * 初始化ALU实例
 * 
 * 初始化策略：
 * 1. 参数有效性检查 - 防止空指针访问
 * 2. 内存清零 - 确保所有字段处于已知状态
 * 3. 默认设置 - 设置合理的初始操作类型
 * 4. 状态验证 - 确保初始化完成后状态正确
 * 
 * 内存安全：
 * 使用memset进行快速零初始化
 * 避免逐字段赋值可能的遗漏
 */
cpu_error_t alu_init(alu_t *alu) {
    if (!alu) {
        return CPU_ERROR_NULL_POINTER;
    }
    
    // 快速零初始化整个结构体
    memset(alu, 0, sizeof(alu_t));
    
    // 设置默认操作类型为加法（最基础的运算）
    alu->operation = ALU_ADD;
    
    return CPU_SUCCESS;
}

/**
 * 重置ALU状态到初始状态
 * 
 * 重置范围：
 * 1. 操作类型重置为默认值
 * 2. 清除上次运算结果
 * 3. 清零所有标志位
 * 4. 清零操作数寄存器
 * 
 * 安全设计：
 * 允许空指针调用，安全返回
 * 保持ALU实例的有效性
 */
void alu_reset(alu_t *alu) {
    if (!alu) {
        return;
    }
    
    alu->operation = ALU_ADD;
    memset(&alu->result, 0, sizeof(alu_result_t));
    alu->flags = 0;
    alu->operand_a = 0;
    alu->operand_b = 0;
}

// ==================== ALU运算执行 ====================

/**
 * 执行ALU综合运算分发器
 * 
 * 分发机制：
 * 1. 参数验证 - 检查ALU实例有效性
 * 2. 状态保存 - 记录操作类型和操作数
 * 3. 运算分发 - 根据操作码调用相应的运算函数
 * 4. 结果缓存 - 保存运算结果到ALU状态中
 * 5. 标志位同步 - 更新ALU的标志位寄存器
 * 
 * 错误处理：
 * - 空指针检查：防止段错误
 * - 无效操作码：返回错误结果
 * - 运算异常：透传具体运算函数的错误
 * 
 * 性能优化：
 * 使用switch语句实现O(1)的操作分发
 * 避免函数指针数组的间接调用开销
 */
alu_result_t alu_execute(alu_t *alu, alu_op_t op, word_t a, word_t b) {
    if (!alu) {
        return create_error_result();
    }
    
    // 保存操作上下文到ALU状态
    alu->operation = op;
    alu->operand_a = a;
    alu->operand_b = b;
    
    alu_result_t result;
    
    // 基于操作码的运算分发
    switch (op) {
    case ALU_ADD:
        result = alu_add(a, b);
        break;
    case ALU_SUB:
        result = alu_sub(a, b);
        break;
    case ALU_MUL:
        result = alu_mul(a, b);
        break;
    case ALU_DIV:
        result = alu_div(a, b);
        break;
    case ALU_AND:
        result = alu_and(a, b);
        break;
    case ALU_OR:
        result = alu_or(a, b);
        break;
    case ALU_XOR:
        result = alu_xor(a, b);
        break;
    case ALU_NOT:
        result = alu_not(a);  // 一元运算，忽略操作数B
        break;
    case ALU_SHL:
        result = alu_shl(a, (u8)b);  // 移位数限制在8位范围
        break;
    case ALU_SHR:
        result = alu_shr(a, (u8)b);
        break;
    case ALU_ROL:
        result = alu_rol(a, (u8)b);
        break;
    case ALU_ROR:
        result = alu_ror(a, (u8)b);
        break;
    case ALU_CMP:
        result = alu_cmp(a, b);
        break;
    default:
        // 无效操作码处理
        result = create_error_result();
        break;
    }
    
    // 更新ALU内部状态
    alu->result = result;
    alu->flags = result.flags;
    
    return result;
}

/**
 * 获取ALU最后一次运算结果
 * 
 * 状态查询：
 * 返回ALU内部缓存的最后一次运算结果
 * 包括数值结果、标志位和有效性状态
 * 
 * 安全机制：
 * 空指针检查，返回错误结果
 */
alu_result_t alu_get_result(const alu_t *alu) {
    if (!alu) {
        return create_error_result();
    }
    return alu->result;
}

// ==================== 基本算术运算 ====================

/**
 * 32位二进制补码加法运算
 * 
 * 算法实现：
 * 1. 64位精度计算 - 使用64位无符号整数避免中间结果溢出
 * 2. 进位检测 - 比较64位结果与32位最大值
 * 3. 有符号溢出检测 - 检查符号位变化的异常情况
 * 4. 标志位计算 - 根据结果设置ZNCO标志位
 * 
 * 进位检测原理：
 * 当64位结果大于32位最大值(0xFFFFFFFF)时产生进位
 * 
 * 溢出检测原理：
 * 有符号溢出发生在：
 * - 正数 + 正数 = 负数 (正溢出)
 * - 负数 + 负数 = 正数 (负溢出)
 * 
 * 标志位设置：
 * - Z: 结果为0时置1
 * - N: 结果符号位为1时置1
 * - C: 无符号运算产生进位时置1
 * - V: 有符号运算发生溢出时置1
 */
alu_result_t alu_add(word_t a, word_t b) {
    // 使用64位精度计算避免中间结果丢失
    u64 result64 = (u64)a + (u64)b;
    word_t result = (word_t)result64;
    
    u8 flags = 0;
    
    // Zero标志位：结果为零检测
    if (result == 0) flags |= ALU_FLAG_ZERO;
    
    // Negative标志位：符号位检测
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // Carry标志位：无符号进位检测
    if (result64 > 0xFFFFFFFF) flags |= ALU_FLAG_CARRY;
    
    // Overflow标志位：有符号溢出检测
    // 使用符号位异或检测算法
    if (((i32)a > 0 && (i32)b > 0 && (i32)result < 0) ||
        ((i32)a < 0 && (i32)b < 0 && (i32)result > 0)) {
        flags |= ALU_FLAG_OVERFLOW;
    }
    
    return create_result(result, flags);
}

/**
 * 32位二进制补码减法运算
 * 
 * 减法实现策略：
 * 使用直接减法而非加法实现，保持语义清晰
 * 
 * 借位检测：
 * 当被减数小于减数时产生借位
 * 对应无符号比较 a < b
 * 
 * 溢出检测：
 * 有符号减法溢出发生在：
 * - 正数 - 负数 = 负数 (正溢出)
 * - 负数 - 正数 = 正数 (负溢出)
 * 
 * 标志位语义：
 * Carry标志位在减法中表示借位
 * 其他标志位与加法保持一致
 */
alu_result_t alu_sub(word_t a, word_t b) {
    word_t result = a - b;
    
    u8 flags = 0;
    
    // Zero标志位：结果为零检测
    if (result == 0) flags |= ALU_FLAG_ZERO;
    
    // Negative标志位：符号位检测
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // Carry标志位：减法借位检测
    if (a < b) flags |= ALU_FLAG_CARRY;
    
    // Overflow标志位：有符号减法溢出检测
    if (((i32)a > 0 && (i32)b < 0 && (i32)result < 0) ||
        ((i32)a < 0 && (i32)b > 0 && (i32)result > 0)) {
        flags |= ALU_FLAG_OVERFLOW;
    }
    
    return create_result(result, flags);
}

/**
 * 32位无符号整数乘法运算
 * 
 * 乘法算法：
 * 1. 64位中间计算 - 防止32位乘法结果溢出
 * 2. 溢出检测 - 检查高32位是否非零
 * 3. 结果截断 - 保留低32位作为最终结果
 * 
 * 性能考虑：
 * 利用现代处理器的硬件乘法器
 * 编译器会优化为高效的乘法指令
 * 
 * 标志位设置：
 * - Z: 结果为0时置1
 * - N: 结果符号位为1时置1
 * - C: 清零（乘法不产生传统意义的进位）
 * - V: 结果超出32位范围时置1
 */
alu_result_t alu_mul(word_t a, word_t b) {
    // 64位精度乘法计算
    u64 result64 = (u64)a * (u64)b;
    word_t result = (word_t)result64;
    
    u8 flags = 0;
    
    // Zero标志位：结果为零检测
    if (result == 0) flags |= ALU_FLAG_ZERO;
    
    // Negative标志位：符号位检测
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // Overflow标志位：32位范围溢出检测
    if (result64 > 0xFFFFFFFF) flags |= ALU_FLAG_OVERFLOW;
    
    return create_result(result, flags);
}

/**
 * 32位无符号整数除法运算
 * 
 * 除法实现：
 * 1. 除零检测 - 除数为0时返回错误结果
 * 2. 整数除法 - 执行标准的整数相除操作
 * 3. 商值返回 - 返回除法的商，忽略余数
 * 
 * 异常处理：
 * 除零异常不抛出，而是返回无效结果
 * 调用者通过检查result.valid判断运算是否成功
 * 
 * 标志位设置：
 * - Z: 商为0时置1
 * - N: 商的符号位为1时置1
 * - C: 清零（除法不产生进位）
 * - V: 清零（整数除法不会溢出）
 * 
 * 注意事项：
 * 整数除法会向零舍入
 * 负数除法的结果依赖于编译器实现
 */
alu_result_t alu_div(word_t a, word_t b) {
    // 除零检测
    if (b == 0) {
        return create_error_result();
    }
    
    word_t result = a / b;
    
    u8 flags = 0;
    
    // Zero标志位：商为零检测
    if (result == 0) flags |= ALU_FLAG_ZERO;
    
    // Negative标志位：商的符号位检测
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    return create_result(result, flags);
}

// ==================== 逻辑运算 ====================

/**
 * 32位按位逻辑与运算
 * 
 * 运算机制：
 * 对两个操作数的每一位执行AND操作
 * 只有对应位都为1时结果位才为1
 * 
 * 应用场景：
 * 1. 位掩码操作 - 清零特定位
 * 2. 条件测试 - 检查特定位状态
 * 3. 数据过滤 - 保留特定位模式
 * 
 * 标志位设置：
 * - Z: 结果为0时置1（所有位都被清零）
 * - N: 结果符号位为1时置1
 * - C: 清零（逻辑运算不产生进位）
 * - V: 清零（逻辑运算不产生溢出）
 */
alu_result_t alu_and(word_t a, word_t b) {
    word_t result = a & b;
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    return create_result(result, flags);
}

/**
 * 32位按位逻辑或运算
 * 
 * 运算机制：
 * 对两个操作数的每一位执行OR操作
 * 只要对应位有一个为1结果位就为1
 * 
 * 应用场景：
 * 1. 位设置操作 - 置位特定位
 * 2. 数据合并 - 组合多个位模式
 * 3. 标志位累积 - 合并多个状态标志
 * 
 * 数学性质：
 * - A | 0 = A (与0或保持不变)
 * - A | A = A (幂等性)
 * - A | (~A) = 0xFFFFFFFF (全1)
 */
alu_result_t alu_or(word_t a, word_t b) {
    word_t result = a | b;
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    return create_result(result, flags);
}

/**
 * 32位按位逻辑异或运算
 * 
 * 运算机制：
 * 对两个操作数的每一位执行XOR操作
 * 对应位不同时结果位为1，相同时为0
 * 
 * 特殊性质：
 * 1. 可逆性 - A ^ B ^ B = A
 * 2. 自异或零 - A ^ A = 0
 * 3. 与零异或不变 - A ^ 0 = A
 * 
 * 应用场景：
 * 1. 位翻转操作 - 翻转特定位
 * 2. 简单加密 - 一次性密码本
 * 3. 奇偶校验 - 计算奇偶性
 * 4. 快速比较 - 检测数值差异
 */
alu_result_t alu_xor(word_t a, word_t b) {
    word_t result = a ^ b;
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    return create_result(result, flags);
}

/**
 * 32位按位逻辑非运算
 * 
 * 运算机制：
 * 对操作数的每一位执行NOT操作
 * 0变1，1变0，完全翻转所有位
 * 
 * 一元运算：
 * 只需要一个操作数，忽略第二个参数
 * 
 * 数学性质：
 * - ~(~A) = A (双重取反恢复原值)
 * - ~0 = 0xFFFFFFFF (全1)
 * - ~0xFFFFFFFF = 0 (全0)
 * 
 * 应用场景：
 * 1. 位模式反转
 * 2. 补码计算的中间步骤
 * 3. 位掩码生成
 */
alu_result_t alu_not(word_t a) {
    word_t result = ~a;
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    return create_result(result, flags);
}

// ==================== 移位运算 ====================

/**
 * 32位逻辑左移运算
 * 
 * 移位机制：
 * 1. 所有位向左移动指定位数
 * 2. 右侧空位用0填充
 * 3. 左侧移出的位丢失
 * 
 * 边界处理：
 * - 移位数 >= 32：结果为0
 * - 移位数 == 0：结果不变
 * - 移位数 < 0：未定义行为，实际中通常模32处理
 * 
 * 进位检测：
 * 检查最后一个移出的位是否为1
 * 计算公式：a & (1U << (32 - shift))
 * 
 * 数学等价：
 * 左移n位等效于乘以2^n（忽略溢出）
 * 
 * 性能特点：
 * 移位运算通常比乘法更快
 * 编译器常用移位优化乘以2的幂次的乘法
 */
alu_result_t alu_shl(word_t a, u8 shift) {
    // 边界情况：移位数过大
    if (shift >= 32) {
        return create_result(0, ALU_FLAG_ZERO);
    }
    
    word_t result = a << shift;
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // 进位检测：检查最后移出的位
    if (shift > 0 && (a & (1U << (32 - shift)))) {
        flags |= ALU_FLAG_CARRY;
    }
    
    return create_result(result, flags);
}

/**
 * 32位逻辑右移运算
 * 
 * 移位机制：
 * 1. 所有位向右移动指定位数
 * 2. 左侧空位用0填充（逻辑右移）
 * 3. 右侧移出的位丢失
 * 
 * 逻辑 vs 算术右移：
 * - 逻辑右移：左侧总是填充0
 * - 算术右移：左侧填充符号位（保持符号）
 * 本实现为逻辑右移
 * 
 * 进位检测：
 * 检查最后一个移出的位是否为1
 * 计算公式：a & (1U << (shift - 1))
 * 
 * 数学等价：
 * 右移n位等效于除以2^n并向下取整
 */
alu_result_t alu_shr(word_t a, u8 shift) {
    // 边界情况：移位数过大
    if (shift >= 32) {
        return create_result(0, ALU_FLAG_ZERO);
    }
    
    word_t result = a >> shift;
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // 进位检测：检查最后移出的位
    if (shift > 0 && (a & (1U << (shift - 1)))) {
        flags |= ALU_FLAG_CARRY;
    }
    
    return create_result(result, flags);
}

/**
 * 32位循环左移运算
 * 
 * 循环移位原理：
 * 1. 左侧移出的位从右侧移入
 * 2. 不丢失任何位信息
 * 3. 具有周期性（移位32次回到原值）
 * 
 * 实现算法：
 * result = (a << shift) | (a >> (32 - shift))
 * 
 * 模运算优化：
 * shift %= 32 确保移位数在有效范围内
 * 避免大移位数的未定义行为
 * 
 * 进位设置：
 * 传统上设置为移入最低位的值
 * 本实现检查结果的最低位
 * 
 * 应用场景：
 * 1. 密码学算法（DES、AES等）
 * 2. 哈希函数
 * 3. 伪随机数生成器
 */
alu_result_t alu_rol(word_t a, u8 shift) {
    shift %= 32;  // 模运算确保移位数有效
    
    if (shift == 0) {
        // 无移位情况：直接返回原值
        u8 flags = 0;
        if (a == 0) flags |= ALU_FLAG_ZERO;
        if ((i32)a < 0) flags |= ALU_FLAG_NEGATIVE;
        return create_result(a, flags);
    }
    
    // 循环左移实现
    word_t result = (a << shift) | (a >> (32 - shift));
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // 进位设置：检查最低位（移入位）
    if (result & 1) flags |= ALU_FLAG_CARRY;
    
    return create_result(result, flags);
}

/**
 * 32位循环右移运算
 * 
 * 循环移位原理：
 * 1. 右侧移出的位从左侧移入
 * 2. 保持所有位信息不丢失
 * 3. 与循环左移互为逆操作
 * 
 * 实现算法：
 * result = (a >> shift) | (a << (32 - shift))
 * 
 * 进位设置：
 * 检查结果的最高位（符号位）
 * 反映最后移入最高位的值
 * 
 * 对称性：
 * ROL(a, n) == ROR(a, 32-n)
 * 循环左移n位等效于循环右移(32-n)位
 */
alu_result_t alu_ror(word_t a, u8 shift) {
    shift %= 32;  // 模运算确保移位数有效
    
    if (shift == 0) {
        // 无移位情况：直接返回原值
        u8 flags = 0;
        if (a == 0) flags |= ALU_FLAG_ZERO;
        if ((i32)a < 0) flags |= ALU_FLAG_NEGATIVE;
        return create_result(a, flags);
    }
    
    // 循环右移实现
    word_t result = (a >> shift) | (a << (32 - shift));
    
    u8 flags = 0;
    if (result == 0) flags |= ALU_FLAG_ZERO;
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // 进位设置：检查最高位（移入位）
    if (result & (1U << 31)) flags |= ALU_FLAG_CARRY;
    
    return create_result(result, flags);
}

// ==================== 比较运算 ====================

/**
 * 32位数值比较运算
 * 
 * 比较实现原理：
 * 通过减法运算实现比较，但不保存减法结果
 * 仅通过标志位反映两数的大小关系
 * 
 * 标志位语义：
 * - Z=1: a == b （相等）
 * - Z=0, N=0: a > b （大于，有符号）
 * - Z=0, N=1: a < b （小于，有符号）
 * - C=1: a < b （小于，无符号）
 * - C=0: a >= b （大于等于，无符号）
 * 
 * 有符号 vs 无符号比较：
 * 1. 有符号比较：基于N和Z标志位
 *    - a > b: Z=0 && N=0
 *    - a < b: Z=0 && N=1
 * 2. 无符号比较：基于C和Z标志位
 *    - a > b: Z=0 && C=0
 *    - a < b: C=1
 * 
 * 比较结果不保存：
 * 虽然计算减法结果，但通常不使用该值
 * 主要目的是设置标志位供后续条件跳转使用
 */
alu_result_t alu_cmp(word_t a, word_t b) {
    word_t result = a - b;
    
    u8 flags = 0;
    
    // Zero标志位：相等检测
    if (result == 0) flags |= ALU_FLAG_ZERO;
    
    // Negative标志位：有符号比较的符号位
    if ((i32)result < 0) flags |= ALU_FLAG_NEGATIVE;
    
    // Carry标志位：无符号比较的借位
    if (a < b) flags |= ALU_FLAG_CARRY;
    
    return create_result(result, flags);
}

// ==================== 标志位操作 ====================

/**
 * 零标志位计算函数
 * 
 * 计算原理：
 * 检查32位结果的所有位是否都为0
 * 零标志位是最简单的标志位计算
 * 
 * @param result 运算结果
 * @return true表示结果为零，false表示非零
 */
bool alu_set_zero_flag(word_t result) {
    return result == 0;
}

/**
 * 进位标志位计算函数
 * 
 * 进位检测算法：
 * 使用64位无符号整数进行加法运算
 * 检查结果是否超出32位范围
 * 
 * 应用场景：
 * 1. 多精度算术运算
 * 2. 无符号数比较
 * 3. 条件分支判断
 * 
 * @param a 操作数A
 * @param b 操作数B
 * @param result 运算结果（在此函数中未直接使用）
 * @return true表示产生进位，false表示无进位
 */
bool alu_set_carry_flag(word_t a, word_t b, word_t result) {
    (void)result;  // 抑制未使用参数警告
    return (u64)a + (u64)b > 0xFFFFFFFF;
}

/**
 * 溢出标志位计算函数
 * 
 * 有符号溢出检测原理：
 * 当两个同号数相加得到异号结果时发生溢出
 * 
 * 检测条件：
 * 1. 正数 + 正数 = 负数 （正溢出）
 * 2. 负数 + 负数 = 正数 （负溢出）
 * 
 * 位运算检测法：
 * ((a ^ result) & (b ^ result) & 0x80000000) != 0
 * 
 * @param a 操作数A
 * @param b 操作数B
 * @param result 运算结果
 * @return true表示发生溢出，false表示无溢出
 */
bool alu_set_overflow_flag(word_t a, word_t b, word_t result) {
    (void)result;  // 当前实现中未使用，保留接口兼容性
    return ((i32)a > 0 && (i32)b > 0 && (i32)result < 0) ||
           ((i32)a < 0 && (i32)b < 0 && (i32)result > 0);
}

/**
 * 负数标志位计算函数
 * 
 * 符号位检测：
 * 在二进制补码表示中，最高位（第31位）为符号位
 * 符号位为1表示负数，为0表示非负数
 * 
 * @param result 运算结果
 * @return true表示结果为负数，false表示非负数
 */
bool alu_set_negative_flag(word_t result) {
    return (i32)result < 0;
}

// ==================== 调试和工具函数 ====================

/**
 * ALU操作类型转换为字符串表示
 * 
 * 字符串映射表：
 * 为每个ALU操作类型提供可读的字符串表示
 * 便于调试输出、日志记录和用户界面显示
 * 
 * 查找算法：
 * 使用switch语句实现O(1)的查找效率
 * 对于无效操作码返回"UNKNOWN"
 */
const char* alu_op_to_string(alu_op_t op) {
    switch (op) {
    case ALU_ADD: return "ADD";
    case ALU_SUB: return "SUB";
    case ALU_MUL: return "MUL";
    case ALU_DIV: return "DIV";
    case ALU_AND: return "AND";
    case ALU_OR:  return "OR";
    case ALU_XOR: return "XOR";
    case ALU_NOT: return "NOT";
    case ALU_SHL: return "SHL";
    case ALU_SHR: return "SHR";
    case ALU_ROL: return "ROL";
    case ALU_ROR: return "ROR";
    case ALU_CMP: return "CMP";
    case ALU_INC: return "INC";
    case ALU_DEC: return "DEC";
    case ALU_NEG: return "NEG";
    default: return "UNKNOWN";
    }
}

/**
 * 格式化输出ALU当前状态
 * 
 * 输出信息包括：
 * 1. 当前执行的操作类型
 * 2. 操作数A和B的十六进制和十进制表示
 * 3. 运算结果的十六进制和十进制表示
 * 4. 四个标志位的状态（Z/N/C/V）
 * 
 * 安全处理：
 * 检查空指针，避免段错误
 */
void alu_print_status(const alu_t *alu) {
    if (!alu) {
        printf("ALU: NULL\n");
        return;
    }
    
    printf("ALU状态:\n");
    printf("  操作: %s\n", alu_op_to_string(alu->operation));
    printf("  操作数A: 0x%08X (%d)\n", alu->operand_a, (i32)alu->operand_a);
    printf("  操作数B: 0x%08X (%d)\n", alu->operand_b, (i32)alu->operand_b);
    printf("  结果: 0x%08X (%d)\n", alu->result.result, (i32)alu->result.result);
    printf("  标志位: Z=%d N=%d C=%d V=%d\n",
           (alu->flags & ALU_FLAG_ZERO) ? 1 : 0,
           (alu->flags & ALU_FLAG_NEGATIVE) ? 1 : 0,
           (alu->flags & ALU_FLAG_CARRY) ? 1 : 0,
           (alu->flags & ALU_FLAG_OVERFLOW) ? 1 : 0);
}

/**
 * 格式化输出ALU运算结果
 * 
 * 输出格式：
 * 1. 结果值的十六进制和十进制表示
 * 2. 结果有效性状态
 * 3. 标志位详细状态
 * 
 * 用途：
 * 单次运算结果的快速查看
 * 不依赖ALU实例状态
 */
void alu_print_result(const alu_result_t *result) {
    if (!result) {
        printf("ALU结果: NULL\n");
        return;
    }
    
    printf("ALU结果:\n");
    printf("  值: 0x%08X (%d)\n", result->result, (i32)result->result);
    printf("  有效: %s\n", result->valid ? "是" : "否");
    printf("  标志位: Z=%d N=%d C=%d V=%d\n",
           (result->flags & ALU_FLAG_ZERO) ? 1 : 0,
           (result->flags & ALU_FLAG_NEGATIVE) ? 1 : 0,
           (result->flags & ALU_FLAG_CARRY) ? 1 : 0,
           (result->flags & ALU_FLAG_OVERFLOW) ? 1 : 0);
}

/**
 * 验证ALU运算结果的有效性
 * 
 * 验证项目：
 * 1. 结果指针非空
 * 2. valid标志位为true
 * 
 * 返回值：
 * true表示结果有效可用
 * false表示结果无效或出错
 */
bool alu_validate_result(const alu_result_t *result) {
    return result && result->valid;
}

/**
 * 转储ALU完整状态信息
 * 
 * 调试用途：
 * 提供ALU实例的完整状态快照
 * 便于深度调试和问题诊断
 * 
 * 输出内容：
 * 复用alu_print_status函数
 * 添加标题框架增强可读性
 */
void alu_dump(const alu_t *alu) {
    if (!alu) {
        printf("ALU转储: NULL指针\n");
        return;
    }
    
    printf("=== ALU状态转储 ===\n");
    alu_print_status(alu);
    printf("==================\n");
}
