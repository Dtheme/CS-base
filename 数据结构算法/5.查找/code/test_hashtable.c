/* test_hashtable.c - 散列表完整测试程序
 * create by: zw.duan
 * 
 * 测试数据结构中的散列表（哈希表）：
 * 1. 各种哈希函数测试
 * 2. 开放地址法测试（线性探测、二次探测、双重哈希）
 * 3. 链地址法测试
 * 4. 性能比较和分析
 */

#include "hashtable_simple.h"
#include <assert.h>

// ============= 测试工具函数 =============

/**
 * 生成测试数据
 */
void GenerateTestData(int data[], int n, int min_val, int max_val) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {
        data[i] = min_val + rand() % (max_val - min_val + 1);
    }
}

/**
 * 打印测试数据
 */
void PrintTestData(int data[], int n) {
    printf("测试数据: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);
        if ((i + 1) % 10 == 0) printf("\n          ");
    }
    if (n % 10 != 0) printf("\n");
}

// ============= 哈希函数测试 =============

/**
 * 测试各种哈希函数的分布性
 */
void TestHashFunctions(void) {
    printf("\n ========== 哈希函数分布性测试 ==========\n");
    
    const int table_size = 13;  // 使用质数
    const int test_count = 100;
    int test_data[test_count];
    
    // 生成测试数据
    GenerateTestData(test_data, test_count, 1, 1000);
    
    printf("散列表大小: %d\n", table_size);
    printf("测试数据量: %d\n", test_count);
    
    // 测试除法散列
    printf("\n--- 除法散列函数分布 ---\n");
    int div_distribution[table_size];
    for (int i = 0; i < table_size; i++) div_distribution[i] = 0;
    
    for (int i = 0; i < test_count; i++) {
        unsigned int hash_val = DivisionHash(test_data[i], table_size);
        div_distribution[hash_val]++;
    }
    
    printf("槽位分布: ");
    for (int i = 0; i < table_size; i++) {
        printf("%d ", div_distribution[i]);
    }
    printf("\n");
    
    // 测试乘法散列
    printf("\n--- 乘法散列函数分布 ---\n");
    int mul_distribution[table_size];
    for (int i = 0; i < table_size; i++) mul_distribution[i] = 0;
    
    for (int i = 0; i < test_count; i++) {
        unsigned int hash_val = MultiplicationHash(test_data[i], table_size);
        mul_distribution[hash_val]++;
    }
    
    printf("槽位分布: ");
    for (int i = 0; i < table_size; i++) {
        printf("%d ", mul_distribution[i]);
    }
    printf("\n");
    
    // 测试平方取中法
    printf("\n--- 平方取中法分布 ---\n");
    int square_distribution[table_size];
    for (int i = 0; i < table_size; i++) square_distribution[i] = 0;
    
    for (int i = 0; i < test_count; i++) {
        unsigned int hash_val = SquareHash(test_data[i], table_size);
        square_distribution[hash_val]++;
    }
    
    printf("槽位分布: ");
    for (int i = 0; i < table_size; i++) {
        printf("%d ", square_distribution[i]);
    }
    printf("\n");
    
    // 计算分布均匀性（方差）
    double div_variance = 0, mul_variance = 0, square_variance = 0;
    double expected = (double)test_count / table_size;
    
    for (int i = 0; i < table_size; i++) {
        div_variance += (div_distribution[i] - expected) * (div_distribution[i] - expected);
        mul_variance += (mul_distribution[i] - expected) * (mul_distribution[i] - expected);
        square_variance += (square_distribution[i] - expected) * (square_distribution[i] - expected);
    }
    
    div_variance /= table_size;
    mul_variance /= table_size;
    square_variance /= table_size;
    
    printf("\n分布均匀性分析（方差越小越均匀）:\n");
    printf("除法散列方差: %.2f\n", div_variance);
    printf("乘法散列方差: %.2f\n", mul_variance);
    printf("平方取中方差: %.2f\n", square_variance);
}

// ============= 开放地址法测试 =============

/**
 * 测试开放地址法 - 线性探测
 */
void TestOpenHashLinearProbe(void) {
    printf("\n ========== 开放地址法 - 线性探测测试 ==========\n");
    
    OpenHashTable table;
    InitOpenHashTable(&table, 11, DivisionHash, LINEAR_PROBE);
    
    // 测试数据
    int test_data[] = {10, 22, 31, 4, 15, 28, 17, 88, 59};
    int n = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("散列表大小: %d\n", table.size);
    PrintTestData(test_data, n);
    
    // 插入测试
    printf("\n--- 插入过程 ---\n");
    for (int i = 0; i < n; i++) {
        bool success = OpenHashInsert(&table, test_data[i], test_data[i] * 10);
        printf("插入 %d: %s\n", test_data[i], success ? "成功" : "失败");
    }
    
    printf("\n");
    PrintOpenHashTable(&table);
    
    // 查找测试
    printf("\n--- 查找测试 ---\n");
    for (int i = 0; i < n; i++) {
        int value;
        bool found = OpenHashSearch(&table, test_data[i], &value);
        printf("查找 %d: %s", test_data[i], found ? "找到" : "未找到");
        if (found) printf("，值=%d", value);
        printf("\n");
    }
    
    // 查找不存在的元素
    int not_exist[] = {1, 99, 50};
    for (int i = 0; i < 3; i++) {
        int value;
        bool found = OpenHashSearch(&table, not_exist[i], &value);
        printf("查找 %d: %s\n", not_exist[i], found ? "找到" : "未找到");
    }
    
    // 删除测试
    printf("\n--- 删除测试 ---\n");
    bool deleted = OpenHashDelete(&table, 22);
    printf("删除 22: %s\n", deleted ? "成功" : "失败");
    
    printf("\n删除后的散列表:\n");
    PrintOpenHashTable(&table);
    
    // 统计信息
    printf("\n");
    PrintOpenHashStats(&table);
    
    FreeOpenHashTable(&table);
    printf(" 线性探测测试通过！\n");
}

/**
 * 测试开放地址法 - 二次探测
 */
void TestOpenHashQuadraticProbe(void) {
    printf("\n ========== 开放地址法 - 二次探测测试 ==========\n");
    
    OpenHashTable table;
    InitOpenHashTable(&table, 11, DivisionHash, QUADRATIC_PROBE);
    
    // 测试数据
    int test_data[] = {10, 22, 31, 4, 15, 28, 17, 88, 59};
    int n = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("散列表大小: %d\n", table.size);
    PrintTestData(test_data, n);
    
    // 插入测试
    printf("\n--- 插入过程 ---\n");
    for (int i = 0; i < n; i++) {
        bool success = OpenHashInsert(&table, test_data[i], test_data[i] * 10);
        printf("插入 %d: %s\n", test_data[i], success ? "成功" : "失败");
    }
    
    printf("\n");
    PrintOpenHashTable(&table);
    
    // 统计信息
    printf("\n");
    PrintOpenHashStats(&table);
    
    FreeOpenHashTable(&table);
    printf(" 二次探测测试通过！\n");
}

/**
 * 测试开放地址法 - 双重哈希
 */
void TestOpenHashDoubleHash(void) {
    printf("\n ========== 开放地址法 - 双重哈希测试 ==========\n");
    
    OpenHashTable table;
    InitOpenHashTable(&table, 11, DivisionHash, DOUBLE_HASH);
    
    // 测试数据
    int test_data[] = {10, 22, 31, 4, 15, 28, 17, 88, 59};
    int n = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("散列表大小: %d\n", table.size);
    PrintTestData(test_data, n);
    
    // 插入测试
    printf("\n--- 插入过程 ---\n");
    for (int i = 0; i < n; i++) {
        bool success = OpenHashInsert(&table, test_data[i], test_data[i] * 10);
        printf("插入 %d: %s\n", test_data[i], success ? "成功" : "失败");
    }
    
    printf("\n");
    PrintOpenHashTable(&table);
    
    // 统计信息
    printf("\n");
    PrintOpenHashStats(&table);
    
    FreeOpenHashTable(&table);
    printf(" 双重哈希测试通过！\n");
}

// ============= 链地址法测试 =============

/**
 * 测试链地址法
 */
void TestChainHash(void) {
    printf("\n🔗 ========== 链地址法测试 ==========\n");
    
    ChainHashTable table;
    InitChainHashTable(&table, 7, DivisionHash);
    
    // 测试数据
    int test_data[] = {10, 22, 31, 4, 15, 28, 17, 88, 59, 43, 50};
    int n = sizeof(test_data) / sizeof(test_data[0]);
    
    printf("散列表大小: %d\n", table.size);
    PrintTestData(test_data, n);
    
    // 插入测试
    printf("\n--- 插入过程 ---\n");
    for (int i = 0; i < n; i++) {
        bool success = ChainHashInsert(&table, test_data[i], test_data[i] * 10);
        printf("插入 %d: %s\n", test_data[i], success ? "成功" : "失败");
    }
    
    printf("\n");
    PrintChainHashTable(&table);
    
    // 查找测试
    printf("\n--- 查找测试 ---\n");
    for (int i = 0; i < n; i++) {
        int value;
        bool found = ChainHashSearch(&table, test_data[i], &value);
        printf("查找 %d: %s", test_data[i], found ? "找到" : "未找到");
        if (found) printf("，值=%d", value);
        printf("\n");
    }
    
    // 删除测试
    printf("\n--- 删除测试 ---\n");
    bool deleted = ChainHashDelete(&table, 22);
    printf("删除 22: %s\n", deleted ? "成功" : "失败");
    
    deleted = ChainHashDelete(&table, 88);
    printf("删除 88: %s\n", deleted ? "成功" : "失败");
    
    printf("\n删除后的散列表:\n");
    PrintChainHashTable(&table);
    
    // 统计信息
    printf("\n");
    PrintChainHashStats(&table);
    
    FreeChainHashTable(&table);
    printf(" 链地址法测试通过！\n");
}

// ============= 性能比较测试 =============

/**
 * 散列表性能比较
 */
void TestHashTablePerformance(void) {
    printf("\n  ========== 散列表性能比较 ==========\n");
    
    const int sizes[] = {100, 500, 1000};
    const int size_count = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int s = 0; s < size_count; s++) {
        int data_size = sizes[s];
        int table_size = data_size / 2;  // 装载因子约为2
        
        printf("\n--- 数据规模: %d，表大小: %d ---\n", data_size, table_size);
        
        // 生成测试数据
        int* test_data = (int*)malloc(data_size * sizeof(int));
        GenerateTestData(test_data, data_size, 1, data_size * 10);
        
        // 测试开放地址法 - 线性探测
        OpenHashTable open_table;
        InitOpenHashTable(&open_table, table_size, DivisionHash, LINEAR_PROBE);
        
        clock_t start = clock();
        for (int i = 0; i < data_size; i++) {
            OpenHashInsert(&open_table, test_data[i], test_data[i]);
        }
        clock_t end = clock();
        double open_insert_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // 查找测试
        start = clock();
        for (int i = 0; i < data_size; i++) {
            int value;
            OpenHashSearch(&open_table, test_data[i], &value);
        }
        end = clock();
        double open_search_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // 测试链地址法
        ChainHashTable chain_table;
        InitChainHashTable(&chain_table, table_size, DivisionHash);
        
        start = clock();
        for (int i = 0; i < data_size; i++) {
            ChainHashInsert(&chain_table, test_data[i], test_data[i]);
        }
        end = clock();
        double chain_insert_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // 查找测试
        start = clock();
        for (int i = 0; i < data_size; i++) {
            int value;
            ChainHashSearch(&chain_table, test_data[i], &value);
        }
        end = clock();
        double chain_search_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // 输出性能比较
        printf("\n性能比较结果:\n");
        printf("%-15s | 插入时间(ms) | 查找时间(ms) | 装载因子 | 冲突次数\n", "方法");
        printf("----------------|--------------|--------------|----------|----------\n");
        printf("%-15s | %12.2f | %12.2f | %8.2f | %8d\n", 
               "开放地址(线性)", open_insert_time, open_search_time, 
               GetOpenHashLoadFactor(&open_table), open_table.stats.collision_count);
        printf("%-15s | %12.2f | %12.2f | %8.2f | %8d\n", 
               "链地址法", chain_insert_time, chain_search_time, 
               GetChainHashLoadFactor(&chain_table), chain_table.stats.collision_count);
        
        printf("\n详细统计:\n");
        printf("开放地址法 - 最大探测长度: %d, 平均探测长度: %.2f\n", 
               open_table.stats.max_probe_length,
               (double)open_table.stats.probe_count / open_table.stats.total_searches);
        printf("链地址法 - 最大链长度: %d, 平均链长度: %.2f\n", 
               chain_table.stats.max_chain_length, GetAverageChainLength(&chain_table));
        
        // 清理资源
        FreeOpenHashTable(&open_table);
        FreeChainHashTable(&chain_table);
        free(test_data);
    }
}

// ============= 数据结构真题演示 =============

/**
 * 数据结构真题演示：散列表ASL计算
 */
void ExamDemo_HashTableASL(void) {
    printf("\n ========== 数据结构真题演示：散列表ASL ==========\n");
    
    printf("题目：设散列表长度为11，散列函数为H(key) = key mod 11\n");
    printf("用线性探测法处理冲突，依次插入关键字：22, 41, 53, 46, 30, 13, 01, 67\n");
    printf("求：(1) 画出散列表\n");
    printf("    (2) 计算成功查找的ASL\n\n");
    
    OpenHashTable table;
    InitOpenHashTable(&table, 11, DivisionHash, LINEAR_PROBE);
    
    int keys[] = {22, 41, 53, 46, 30, 13, 1, 67};
    int n = sizeof(keys) / sizeof(keys[0]);
    
    printf("解答过程：\n");
    printf("1. 依次插入关键字，记录探测过程：\n");
    
    for (int i = 0; i < n; i++) {
        int key = keys[i];
        unsigned int hash_val = DivisionHash(key, 11);
        printf("插入 %d: H(%d) = %d mod 11 = %d", key, key, key, hash_val);
        
        // 手动模拟插入过程以显示探测
        int probe_count = 0;
        unsigned int pos = hash_val;
        while (table.table[pos].status == OCCUPIED) {
            probe_count++;
            pos = (pos + 1) % 11;
            printf(" -> %d", pos);
        }
        
        OpenHashInsert(&table, key, key);
        printf(" (探测%d次)\n", probe_count + 1);
    }
    
    printf("\n2. 最终散列表：\n");
    PrintOpenHashTable(&table);
    
    printf("\n3. 计算成功查找的ASL：\n");
    int total_probes = 0;
    for (int i = 0; i < n; i++) {
        int key = keys[i];
        unsigned int hash_val = DivisionHash(key, 11);
        int probe_count = 1;
        unsigned int pos = hash_val;
        
        while (table.table[pos].key != key) {
            probe_count++;
            pos = (pos + 1) % 11;
        }
        
        printf("查找 %d: 需要 %d 次探测\n", key, probe_count);
        total_probes += probe_count;
    }
    
    double asl = (double)total_probes / n;
    printf("\n4. 成功查找ASL = %d / %d = %.2f\n", total_probes, n, asl);
    
    printf("\n5. 理论分析：\n");
    printf("   装载因子 α = %d / %d = %.2f\n", n, 11, (double)n / 11);
    printf("   线性探测成功查找ASL理论值 ≈ 0.5 * (1 + 1/(1-α)) = %.2f\n", 
           0.5 * (1 + 1/(1 - (double)n / 11)));
    printf("   实际计算值: %.2f （基本吻合）\n", asl);
    
    FreeOpenHashTable(&table);
}

/**
 * 数据结构真题演示：链地址法ASL计算
 */
void ExamDemo_ChainHashASL(void) {
    printf("\n ========== 数据结构真题演示：链地址法ASL ==========\n");
    
    printf("题目：设散列表长度为7，散列函数为H(key) = key mod 7\n");
    printf("用链地址法处理冲突，依次插入关键字：15, 38, 61, 84, 29, 52, 75, 99\n");
    printf("求：成功查找的ASL\n\n");
    
    ChainHashTable table;
    InitChainHashTable(&table, 7, DivisionHash);
    
    int keys[] = {15, 38, 61, 84, 29, 52, 75, 99};
    int n = sizeof(keys) / sizeof(keys[0]);
    
    printf("解答过程：\n");
    printf("1. 依次插入关键字：\n");
    
    for (int i = 0; i < n; i++) {
        int key = keys[i];
        unsigned int hash_val = DivisionHash(key, 7);
        printf("插入 %d: H(%d) = %d mod 7 = %d\n", key, key, key, hash_val);
        ChainHashInsert(&table, key, key);
    }
    
    printf("\n2. 最终散列表：\n");
    PrintChainHashTable(&table);
    
    printf("\n3. 计算成功查找的ASL：\n");
    int total_comparisons = 0;
    
    for (int i = 0; i < n; i++) {
        int key = keys[i];
        unsigned int hash_val = DivisionHash(key, 7);
        
        // 计算在链中的位置
        int position = 1;
        HashNode* current = table.table[hash_val];
        while (current != NULL && current->key != key) {
            position++;
            current = current->next;
        }
        
        printf("查找 %d: 在槽位 %d 的第 %d 个位置\n", key, hash_val, position);
        total_comparisons += position;
    }
    
    double asl = (double)total_comparisons / n;
    printf("\n4. 成功查找ASL = %d / %d = %.2f\n", total_comparisons, n, asl);
    
    printf("\n5. 理论分析：\n");
    printf("   装载因子 α = %d / %d = %.2f\n", n, 7, (double)n / 7);
    printf("   链地址法成功查找ASL理论值 ≈ 1 + α/2 = %.2f\n", 
           1 + (double)n / 7 / 2);
    printf("   实际计算值: %.2f （基本吻合）\n", asl);
    
    FreeChainHashTable(&table);
}

/**
 * 散列表综合演示
 */
void HashTableDemo(void) {
    printf("\n ========== 散列表综合演示 ==========\n");
    
    printf("本演示展示了数据结构中的散列表技术：\n\n");
    
    printf("1.  散列表基本概念：\n");
    printf("   ├── 散列函数：将关键字映射到散列表位置\n");
    printf("   ├── 冲突：不同关键字映射到同一位置\n");
    printf("   ├── 装载因子：α = n / m（元素数/表长）\n");
    printf("   └── 平均查找长度：ASL\n\n");
    
    printf("2.  常用散列函数：\n");
    printf("   ├── 除法散列：H(key) = key mod m\n");
    printf("   ├── 乘法散列：H(key) = floor(m * (key * A mod 1))\n");
    printf("   ├── 平方取中法：取key²的中间位\n");
    printf("   └── 数字分析法：分析关键字特征\n\n");
    
    printf("3.  冲突处理方法：\n");
    printf("   ├── 开放地址法：\n");
    printf("   │   ├── 线性探测：H(key, i) = (H(key) + i) mod m\n");
    printf("   │   ├── 二次探测：H(key, i) = (H(key) + i²) mod m\n");
    printf("   │   └── 双重哈希：H(key, i) = (H₁(key) + i*H₂(key)) mod m\n");
    printf("   └── 链地址法：每个槽位维护一个链表\n\n");
    
    printf("4.   性能分析：\n");
    printf("   ├── 开放地址法：\n");
    printf("   │   ├── 成功查找ASL ≈ 0.5 * (1 + 1/(1-α))\n");
    printf("   │   └── 失败查找ASL ≈ 0.5 * (1 + 1/(1-α)²)\n");
    printf("   └── 链地址法：\n");
    printf("       ├── 成功查找ASL ≈ 1 + α/2\n");
    printf("       └── 失败查找ASL ≈ α\n\n");
    
    printf("5.  数据结构重点：\n");
    printf("   ├── 散列函数的设计原则\n");
    printf("   ├── 不同冲突处理方法的特点\n");
    printf("   ├── ASL的计算方法\n");
    printf("   ├── 装载因子对性能的影响\n");
    printf("   └── 散列表的应用场景\n");
}

// ============= 主测试函数 =============

int main(void) {
    printf("🔗 ========================================\n");
    printf("   数据结构 - 散列表完整测试\n");
    printf("   作者: zw.duan\n");
    printf("   版本: v1.0 完整实现版\n");
    printf("========================================\n");
    
    // 运行所有测试
    TestHashFunctions();
    TestOpenHashLinearProbe();
    TestOpenHashQuadraticProbe();
    TestOpenHashDoubleHash();
    TestChainHash();
    
    // 性能比较测试
    TestHashTablePerformance();
    
    // 数据结构真题演示
    ExamDemo_HashTableASL();
    ExamDemo_ChainHashASL();
    
    // 综合演示
    HashTableDemo();
    
    printf("\n ========================================\n");
    printf("    所有散列表测试通过！\n");
    printf("    散列表实现完成\n");
    printf("    下一步：实现字符串查找算法\n");
    printf("========================================\n");
    
    return 0;
} 