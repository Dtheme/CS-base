/* hashtable.c - 散列表完整实现
 * create by: zw.duan
 * 
 * 实现数据结构中的散列表（哈希表）：
 * 1. 哈希函数：除法散列、乘法散列、平方取中法
 * 2. 冲突处理：开放地址法（线性探测、二次探测、双重哈希）
 * 3. 冲突处理：链地址法（拉链法）
 * 4. 动态扩容和性能统计
 */

#include "hashtable_simple.h"
#include <time.h>

// ============= 哈希函数实现 =============

/**
 *  除法散列函数 - 数据结构基础哈希函数
 * 
 *   算法原理：
 * h(key) = key % m
 * 其中 m 是散列表大小，通常选择质数
 * 
 *  数据结构要点：
 * - 最简单、最常用的哈希函数
 * - m 的选择很重要，质数效果更好
 * - 避免选择 2^k 或 10^k 形式的数
 * 
 * @param key 关键字
 * @param table_size 散列表大小
 * @return 哈希值
 */
unsigned int DivisionHash(int key, int table_size) {
    return (unsigned int)(key % table_size);
}

/**
 *  乘法散列函数 - 数据结构改进哈希函数
 * 
 *   算法原理：
 * h(key) = floor(m * (key * A - floor(key * A)))
 * 其中 A 是常数，0 < A < 1，推荐 A = (√5 - 1) / 2 ≈ 0.618
 * 
 *  算法特点：
 * - 对 m 的选择不敏感，可以选择 2 的幂
 * - 使用黄金分割比，分布更均匀
 * - 计算稍复杂，但分布性能更好
 * 
 * @param key 关键字
 * @param table_size 散列表大小
 * @return 哈希值
 */
unsigned int MultiplicationHash(int key, int table_size) {
    const double A = 0.6180339887;  // 黄金分割比
    double temp = key * A;
    temp = temp - (int)temp;  // 取小数部分
    return (unsigned int)(table_size * temp);
}

/**
 *  平方取中法 - 数据结构特殊哈希函数
 * 
 *   算法原理：
 * 1. 计算 key^2
 * 2. 取中间几位作为哈希值
 * 3. 对散列表大小取模
 * 
 *  适用场景：
 * - 关键字位数已知且固定
 * - 关键字的每一位都参与运算
 * - 分布相对均匀
 * 
 * @param key 关键字
 * @param table_size 散列表大小
 * @return 哈希值
 */
unsigned int SquareHash(int key, int table_size) {
    unsigned long long square = (unsigned long long)key * key;
    // 取中间位数（这里简化为取低位）
    unsigned int middle = (unsigned int)(square >> 8) & 0xFFFF;
    return middle % table_size;
}

/**
 *  双重哈希函数 - 用于双重哈希探测
 * 
 * @param key 关键字
 * @param table_size 散列表大小
 * @return 第二个哈希值
 */
unsigned int SecondHash(int key, int table_size) {
    // 使用一个与table_size互质的数
    int prime = table_size - 1;
    while (prime > 1) {
        bool is_prime = true;
        for (int i = 2; i * i <= prime; i++) {
            if (prime % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime && table_size % prime != 0) break;
        prime--;
    }
    return 1 + (key % prime);
}

// ============= 开放地址法实现 =============

/**
 * 初始化开放地址散列表
 */
void InitOpenHashTable(OpenHashTable* table, int size, HashFunction hash_func, 
                      ProbeMethod probe_method) {
    table->table = (HashEntry*)malloc(size * sizeof(HashEntry));
    table->size = size;
    table->count = 0;
    table->hash_func = hash_func;
    table->probe_method = probe_method;
    
    // 初始化统计信息
    table->stats.total_insertions = 0;
    table->stats.total_searches = 0;
    table->stats.total_deletions = 0;
    table->stats.collision_count = 0;
    table->stats.probe_count = 0;
    table->stats.max_probe_length = 0;
    
    // 初始化所有槽位为空
    for (int i = 0; i < size; i++) {
        table->table[i].key = -1;  // -1 表示空槽
        table->table[i].status = EMPTY;
        table->table[i].value = 0;
    }
}

/**
 *  开放地址法插入 - 数据结构重点算法
 * 
 *   探测方法：
 * 1. 线性探测：h(key, i) = (h(key) + i) % m
 * 2. 二次探测：h(key, i) = (h(key) + i²) % m
 * 3. 双重哈希：h(key, i) = (h1(key) + i * h2(key)) % m
 * 
 *  数据结构要点：
 * - 理解不同探测方法的特点
 * - 掌握装载因子的概念
 * - 了解聚集现象的产生和影响
 * 
 * @param table 开放地址散列表
 * @param key 关键字
 * @param value 值
 * @return 插入是否成功
 */
bool OpenHashInsert(OpenHashTable* table, int key, int value) {
    if (table->count >= table->size) {
        return false;  // 表已满
    }
    
    table->stats.total_insertions++;
    
    unsigned int hash_val = table->hash_func(key, table->size);
    int probe_length = 0;
    
    for (int i = 0; i < table->size; i++) {
        unsigned int pos;
        
        // 根据探测方法计算位置
        switch (table->probe_method) {
            case LINEAR_PROBE:
                pos = (hash_val + i) % table->size;
                break;
            case QUADRATIC_PROBE:
                pos = (hash_val + i * i) % table->size;
                break;
            case DOUBLE_HASH:
                pos = (hash_val + i * SecondHash(key, table->size)) % table->size;
                break;
            default:
                pos = (hash_val + i) % table->size;
        }
        
        probe_length++;
        table->stats.probe_count++;
        
        if (table->table[pos].status == EMPTY || table->table[pos].status == DELETED) {
            // 找到空槽或已删除槽
            table->table[pos].key = key;
            table->table[pos].value = value;
            table->table[pos].status = OCCUPIED;
            table->count++;
            
            if (i > 0) {
                table->stats.collision_count++;
            }
            
            if (probe_length > table->stats.max_probe_length) {
                table->stats.max_probe_length = probe_length;
            }
            
            return true;
        } else if (table->table[pos].key == key) {
            // 更新已存在的键
            table->table[pos].value = value;
            return true;
        }
    }
    
    return false;  // 插入失败
}

/**
 *  开放地址法查找 - 数据结构重点算法
 * 
 * @param table 开放地址散列表
 * @param key 关键字
 * @param value 输出参数，存储找到的值
 * @return 查找是否成功
 */
bool OpenHashSearch(OpenHashTable* table, int key, int* value) {
    table->stats.total_searches++;
    
    unsigned int hash_val = table->hash_func(key, table->size);
    
    for (int i = 0; i < table->size; i++) {
        unsigned int pos;
        
        // 根据探测方法计算位置
        switch (table->probe_method) {
            case LINEAR_PROBE:
                pos = (hash_val + i) % table->size;
                break;
            case QUADRATIC_PROBE:
                pos = (hash_val + i * i) % table->size;
                break;
            case DOUBLE_HASH:
                pos = (hash_val + i * SecondHash(key, table->size)) % table->size;
                break;
            default:
                pos = (hash_val + i) % table->size;
        }
        
        table->stats.probe_count++;
        
        if (table->table[pos].status == EMPTY) {
            // 遇到空槽，查找失败
            return false;
        } else if (table->table[pos].status == OCCUPIED && table->table[pos].key == key) {
            // 找到目标键
            *value = table->table[pos].value;
            return true;
        }
        // 继续探测（跳过已删除的槽）
    }
    
    return false;  // 查找失败
}

/**
 *  开放地址法删除 - 数据结构重点算法
 * 
 *   删除策略：
 * - 不能直接删除，否则会影响后续查找
 * - 使用"懒惰删除"，标记为DELETED状态
 * - 插入时可以重用DELETED槽位
 * 
 * @param table 开放地址散列表
 * @param key 关键字
 * @return 删除是否成功
 */
bool OpenHashDelete(OpenHashTable* table, int key) {
    table->stats.total_deletions++;
    
    unsigned int hash_val = table->hash_func(key, table->size);
    
    for (int i = 0; i < table->size; i++) {
        unsigned int pos;
        
        // 根据探测方法计算位置
        switch (table->probe_method) {
            case LINEAR_PROBE:
                pos = (hash_val + i) % table->size;
                break;
            case QUADRATIC_PROBE:
                pos = (hash_val + i * i) % table->size;
                break;
            case DOUBLE_HASH:
                pos = (hash_val + i * SecondHash(key, table->size)) % table->size;
                break;
            default:
                pos = (hash_val + i) % table->size;
        }
        
        if (table->table[pos].status == EMPTY) {
            // 遇到空槽，删除失败
            return false;
        } else if (table->table[pos].status == OCCUPIED && table->table[pos].key == key) {
            // 找到目标键，标记为删除
            table->table[pos].status = DELETED;
            table->count--;
            return true;
        }
    }
    
    return false;  // 删除失败
}

// ============= 链地址法实现 =============

/**
 * 初始化链地址散列表
 */
void InitChainHashTable(ChainHashTable* table, int size, HashFunction hash_func) {
    table->table = (HashNode**)malloc(size * sizeof(HashNode*));
    table->size = size;
    table->count = 0;
    table->hash_func = hash_func;
    
    // 初始化统计信息
    table->stats.total_insertions = 0;
    table->stats.total_searches = 0;
    table->stats.total_deletions = 0;
    table->stats.collision_count = 0;
    table->stats.max_chain_length = 0;
    table->stats.total_chain_length = 0;
    
    // 初始化所有链表头为空
    for (int i = 0; i < size; i++) {
        table->table[i] = NULL;
    }
}

/**
 *  链地址法插入 - 数据结构重点算法
 * 
 *   算法特点：
 * - 每个槽位维护一个链表
 * - 冲突时直接加入链表
 * - 不存在装载因子限制
 * - 删除操作简单
 * 
 *  数据结构要点：
 * - 理解链地址法的优缺点
 * - 掌握平均查找长度的计算
 * - 了解装载因子对性能的影响
 * 
 * @param table 链地址散列表
 * @param key 关键字
 * @param value 值
 * @return 插入是否成功
 */
bool ChainHashInsert(ChainHashTable* table, int key, int value) {
    table->stats.total_insertions++;
    
    unsigned int hash_val = table->hash_func(key, table->size);
    
    // 检查是否已存在
    HashNode* current = table->table[hash_val];
    while (current != NULL) {
        if (current->key == key) {
            // 更新已存在的键
            current->value = value;
            return true;
        }
        current = current->next;
    }
    
    // 创建新节点
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    if (new_node == NULL) {
        return false;  // 内存分配失败
    }
    
    new_node->key = key;
    new_node->value = value;
    new_node->next = table->table[hash_val];
    table->table[hash_val] = new_node;
    table->count++;
    
    // 更新统计信息
    if (new_node->next != NULL) {
        table->stats.collision_count++;
    }
    
    // 计算链长度
    int chain_length = 0;
    current = table->table[hash_val];
    while (current != NULL) {
        chain_length++;
        current = current->next;
    }
    
    if (chain_length > table->stats.max_chain_length) {
        table->stats.max_chain_length = chain_length;
    }
    
    return true;
}

/**
 *  链地址法查找 - 数据结构重点算法
 * 
 * @param table 链地址散列表
 * @param key 关键字
 * @param value 输出参数，存储找到的值
 * @return 查找是否成功
 */
bool ChainHashSearch(ChainHashTable* table, int key, int* value) {
    table->stats.total_searches++;
    
    unsigned int hash_val = table->hash_func(key, table->size);
    
    HashNode* current = table->table[hash_val];
    while (current != NULL) {
        if (current->key == key) {
            *value = current->value;
            return true;
        }
        current = current->next;
    }
    
    return false;  // 查找失败
}

/**
 *  链地址法删除 - 数据结构重点算法
 * 
 * @param table 链地址散列表
 * @param key 关键字
 * @return 删除是否成功
 */
bool ChainHashDelete(ChainHashTable* table, int key) {
    table->stats.total_deletions++;
    
    unsigned int hash_val = table->hash_func(key, table->size);
    
    HashNode* current = table->table[hash_val];
    HashNode* prev = NULL;
    
    while (current != NULL) {
        if (current->key == key) {
            // 找到目标节点
            if (prev == NULL) {
                // 删除头节点
                table->table[hash_val] = current->next;
            } else {
                // 删除中间或尾节点
                prev->next = current->next;
            }
            
            free(current);
            table->count--;
            return true;
        }
        
        prev = current;
        current = current->next;
    }
    
    return false;  // 删除失败
}

// ============= 动态扩容实现 =============

/**
 *  开放地址散列表扩容 - 数据结构扩展知识
 * 
 *   扩容策略：
 * - 当装载因子超过阈值时触发扩容
 * - 通常扩容为原来的2倍
 * - 需要重新哈希所有元素
 * 
 * @param table 开放地址散列表
 * @return 扩容是否成功
 */
bool ResizeOpenHashTable(OpenHashTable* table) {
    int old_size = table->size;
    HashEntry* old_table = table->table;
    
    // 扩容为原来的2倍
    table->size = old_size * 2;
    table->table = (HashEntry*)malloc(table->size * sizeof(HashEntry));
    if (table->table == NULL) {
        // 恢复原状态
        table->size = old_size;
        table->table = old_table;
        return false;
    }
    
    // 初始化新表
    for (int i = 0; i < table->size; i++) {
        table->table[i].key = -1;
        table->table[i].status = EMPTY;
        table->table[i].value = 0;
    }
    
    // 重新插入所有元素
    table->count = 0;
    
    for (int i = 0; i < old_size; i++) {
        if (old_table[i].status == OCCUPIED) {
            OpenHashInsert(table, old_table[i].key, old_table[i].value);
        }
    }
    
    free(old_table);
    return true;
}

/**
 *  链地址散列表扩容
 * 
 * @param table 链地址散列表
 * @return 扩容是否成功
 */
bool ResizeChainHashTable(ChainHashTable* table) {
    int old_size = table->size;
    HashNode** old_table = table->table;
    
    // 扩容为原来的2倍
    table->size = old_size * 2;
    table->table = (HashNode**)malloc(table->size * sizeof(HashNode*));
    if (table->table == NULL) {
        // 恢复原状态
        table->size = old_size;
        table->table = old_table;
        return false;
    }
    
    // 初始化新表
    for (int i = 0; i < table->size; i++) {
        table->table[i] = NULL;
    }
    
    // 重新插入所有元素
    table->count = 0;
    
    for (int i = 0; i < old_size; i++) {
        HashNode* current = old_table[i];
        while (current != NULL) {
            HashNode* next = current->next;
            
            // 重新计算哈希值并插入
            unsigned int new_hash = table->hash_func(current->key, table->size);
            current->next = table->table[new_hash];
            table->table[new_hash] = current;
            table->count++;
            
            current = next;
        }
    }
    
    free(old_table);
    return true;
}

// ============= 性能统计和分析 =============

/**
 * 计算开放地址散列表的装载因子
 */
double GetOpenHashLoadFactor(OpenHashTable* table) {
    return (double)table->count / table->size;
}

/**
 * 计算链地址散列表的装载因子
 */
double GetChainHashLoadFactor(ChainHashTable* table) {
    return (double)table->count / table->size;
}

/**
 * 计算链地址散列表的平均链长度
 */
double GetAverageChainLength(ChainHashTable* table) {
    if (table->count == 0) return 0.0;
    
    int total_length = 0;
    int non_empty_chains = 0;
    
    for (int i = 0; i < table->size; i++) {
        if (table->table[i] != NULL) {
            int chain_length = 0;
            HashNode* current = table->table[i];
            while (current != NULL) {
                chain_length++;
                current = current->next;
            }
            total_length += chain_length;
            non_empty_chains++;
        }
    }
    
    return non_empty_chains > 0 ? (double)total_length / non_empty_chains : 0.0;
}

/**
 * 打印开放地址散列表统计信息
 */
void PrintOpenHashStats(OpenHashTable* table) {
    printf("开放地址散列表统计信息:\n");
    printf("  表大小: %d\n", table->size);
    printf("  元素数量: %d\n", table->count);
    printf("  装载因子: %.3f\n", GetOpenHashLoadFactor(table));
    printf("  总插入次数: %d\n", table->stats.total_insertions);
    printf("  总查找次数: %d\n", table->stats.total_searches);
    printf("  总删除次数: %d\n", table->stats.total_deletions);
    printf("  冲突次数: %d\n", table->stats.collision_count);
    printf("  探测次数: %d\n", table->stats.probe_count);
    printf("  最大探测长度: %d\n", table->stats.max_probe_length);
    if (table->stats.total_searches > 0) {
        printf("  平均探测长度: %.2f\n", 
               (double)table->stats.probe_count / table->stats.total_searches);
    }
}

/**
 * 打印链地址散列表统计信息
 */
void PrintChainHashStats(ChainHashTable* table) {
    printf("链地址散列表统计信息:\n");
    printf("  表大小: %d\n", table->size);
    printf("  元素数量: %d\n", table->count);
    printf("  装载因子: %.3f\n", GetChainHashLoadFactor(table));
    printf("  总插入次数: %d\n", table->stats.total_insertions);
    printf("  总查找次数: %d\n", table->stats.total_searches);
    printf("  总删除次数: %d\n", table->stats.total_deletions);
    printf("  冲突次数: %d\n", table->stats.collision_count);
    printf("  最大链长度: %d\n", table->stats.max_chain_length);
    printf("  平均链长度: %.2f\n", GetAverageChainLength(table));
}

// ============= 工具函数实现 =============

/**
 * 释放开放地址散列表
 */
void FreeOpenHashTable(OpenHashTable* table) {
    if (table->table != NULL) {
        free(table->table);
        table->table = NULL;
    }
    table->size = 0;
    table->count = 0;
}

/**
 * 释放链地址散列表
 */
void FreeChainHashTable(ChainHashTable* table) {
    for (int i = 0; i < table->size; i++) {
        HashNode* current = table->table[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);
            current = next;
        }
    }
    
    if (table->table != NULL) {
        free(table->table);
        table->table = NULL;
    }
    table->size = 0;
    table->count = 0;
}

/**
 * 打印开放地址散列表内容
 */
void PrintOpenHashTable(OpenHashTable* table) {
    printf("开放地址散列表内容:\n");
    for (int i = 0; i < table->size; i++) {
        printf("槽位 %2d: ", i);
        switch (table->table[i].status) {
            case EMPTY:
                printf("空\n");
                break;
            case OCCUPIED:
                printf("键=%d, 值=%d\n", table->table[i].key, table->table[i].value);
                break;
            case DELETED:
                printf("已删除\n");
                break;
        }
    }
}

/**
 * 打印链地址散列表内容
 */
void PrintChainHashTable(ChainHashTable* table) {
    printf("链地址散列表内容:\n");
    for (int i = 0; i < table->size; i++) {
        printf("槽位 %2d: ", i);
        HashNode* current = table->table[i];
        if (current == NULL) {
            printf("空\n");
        } else {
            while (current != NULL) {
                printf("(键=%d, 值=%d)", current->key, current->value);
                current = current->next;
                if (current != NULL) printf(" -> ");
            }
            printf("\n");
        }
    }
} 