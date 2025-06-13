/* hashtable.h - 散列表（哈希表）数据结构
 * create by: zw.duan
 * 
 * 散列表特性：
 * 1. 通过哈希函数将关键字映射到表中位置
 * 2. 理想情况下查找时间复杂度为O(1)
 * 3. 需要处理哈希冲突问题
 * 4. 装填因子影响性能
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// ============= 散列表相关定义 =============

#define HASH_TABLE_SIZE 13       // 哈希表大小（建议使用质数）
#define MAX_LOAD_FACTOR 0.75     // 最大装填因子
#define DELETED_MARK -1          // 删除标记

/**
 * 哈希表状态枚举
 */
typedef enum {
    HASH_EMPTY = 0,              // 空位置
    HASH_OCCUPIED = 1,           // 占用位置
    HASH_DELETED = 2             // 已删除位置
} HashStatus;

/**
 * 哈希表元素结构（开放地址法）
 */
typedef struct {
    int key;                     // 关键字
    int value;                   // 数据值
    HashStatus status;           // 位置状态
} HashElement;

/**
 * 开放地址法哈希表结构
 */
typedef struct {
    HashElement* table;          // 哈希表数组
    int size;                    // 表的大小
    int count;                   // 当前元素个数
    double load_factor;          // 装填因子
    int collisions;              // 冲突次数统计
} OpenHashTable;

/**
 * 链表节点（用于链地址法）
 */
typedef struct ChainNode {
    int key;                     // 关键字
    int value;                   // 数据值
    struct ChainNode* next;      // 下一个节点
} ChainNode;

/**
 * 链地址法哈希表结构
 */
typedef struct {
    ChainNode** chains;          // 链表数组
    int size;                    // 表的大小
    int count;                   // 当前元素个数
    double load_factor;          // 装填因子
    int collisions;              // 冲突次数统计
} ChainHashTable;

/**
 * 哈希查找结果结构
 */
typedef struct {
    bool found;                  // 是否找到
    int index;                   // 找到的位置索引
    int probes;                  // 探测次数
    int value;                   // 找到的值
} HashSearchResult;

// ============= 哈希函数（ 重点）=============

/**
 *  除法哈希函数（Division Method， 基础）
 * 
 *   除法哈希原理：
 * Hash(key) = key % m
 * 其中m是哈希表的大小，通常选择质数
 * 
 *  算法特点：
 * - 实现简单，计算快速
 * - 哈希表大小m的选择很重要
 * - 应避免选择2的幂次，建议选择质数
 * - 对于连续的关键字序列可能产生较多冲突
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1)
 * - 空间复杂度：O(1)
 * 
 *  
 * - 理解除法哈希的基本原理
 * - 掌握哈希表大小的选择策略
 * - 了解质数的作用和选择方法
 * 
 * @param key 关键字
 * @param table_size 哈希表大小
 * @return 哈希地址
 */
unsigned int DivisionHash(int key, int table_size);

/**
 *  乘法哈希函数（Multiplication Method）
 * 
 *   乘法哈希原理：
 * Hash(key) = ⌊m * (key * A - ⌊key * A⌋)⌋
 * 其中A是0<A<1的常数，推荐A = (√5-1)/2 ≈ 0.618
 * 
 * @param key 关键字
 * @param table_size 哈希表大小
 * @return 哈希地址
 */
unsigned int MultiplicationHash(int key, int table_size);

/**
 *  平方取中法
 * 
 *   平方取中原理：
 * 1. 将关键字平方
 * 2. 取中间几位作为哈希地址
 * 
 * @param key 关键字
 * @param table_size 哈希表大小
 * @return 哈希地址
 */
unsigned int SquareHash(int key, int table_size);

/**
 *  字符串哈希函数
 * 
 * @param str 字符串关键字
 * @param table_size 哈希表大小
 * @return 哈希地址
 */
int StringHash(const char* str, int table_size);

// ============= 开放地址法（ 重点）=============

/**
 * 🔓 线性探测法（Linear Probing， 核心）
 * 
 *   线性探测过程图示（插入关键字23，表大小为13）：
 * 
 * 步骤1：计算初始地址 Hash(23) = 23 % 13 = 10
 * 步骤2：检查位置10，如果被占用，探测下一个位置
 * 步骤3：Hi = (H0 + i) % m，i = 1, 2, 3, ...
 * 
 * 探测序列：10 → 11 → 12 → 0 → 1 → ... 直到找到空位置
 * 
 *  算法特点：
 * - 实现简单，缓存友好
 * - 容易产生聚集现象（primary clustering）
 * - 删除操作需要特殊处理（延迟删除）
 * - 装填因子不能太高
 * 
 *   复杂度分析：
 * - 平均查找长度：与装填因子相关
 * - 成功查找：ASL ≈ 1/2 * (1 + 1/(1-α))
 * - 失败查找：ASL ≈ 1/2 * (1 + 1/(1-α)²)
 * - 其中α为装填因子
 * 
 *  
 * - 理解线性探测的探测序列
 * - 掌握ASL的计算方法
 * - 理解聚集现象的产生和影响
 * - 了解删除操作的处理方式
 */

/**
 * 初始化开放地址法哈希表
 */
void InitOpenHashTable(OpenHashTable* table, int size);
void DestroyOpenHashTable(OpenHashTable* table);

/**
 * 线性探测插入
 */
bool LinearProbingInsert(OpenHashTable* table, int key, int value);

/**
 * 线性探测查找
 */
HashSearchResult LinearProbingSearch(OpenHashTable* table, int key);

/**
 * 线性探测删除
 */
bool LinearProbingDelete(OpenHashTable* table, int key);

/**
 * 🔓 二次探测法（Quadratic Probing， 重点）
 * 
 *   二次探测原理：
 * Hi = (H0 + i²) % m，i = 1, 2, 3, ...
 * 或者 Hi = (H0 ± i²) % m
 * 
 *  算法特点：
 * - 减少了一次聚集现象
 * - 但可能产生二次聚集（secondary clustering）
 * - 探测序列与初始哈希值有关
 * - 可能无法探测到所有位置
 * 
 *   探测序列示例（H0 = 5）：
 * 5 → 6 → 9 → 1 → 8 → 4 → 5 ...
 * (5+1²)%13=6, (5+2²)%13=9, (5+3²)%13=1, ...
 * 
 * @param table 哈希表指针
 * @param key 关键字
 * @param value 数据值
 * @return 插入是否成功
 */
bool QuadraticProbingInsert(OpenHashTable* table, int key, int value);
HashSearchResult QuadraticProbingSearch(OpenHashTable* table, int key);
bool QuadraticProbingDelete(OpenHashTable* table, int key);

/**
 * 🔓 双重哈希法（Double Hashing， 了解）
 * 
 *   双重哈希原理：
 * Hi = (H1(key) + i * H2(key)) % m
 * 其中H1和H2是两个不同的哈希函数
 * 
 *  算法特点：
 * - 避免了聚集现象
 * - 探测序列的步长由第二个哈希函数决定
 * - H2(key)应与m互质，通常选择H2(key) = 7 - (key % 7)
 * 
 * @param table 哈希表指针
 * @param key 关键字
 * @param value 数据值
 * @return 插入是否成功
 */
bool DoubleHashingInsert(OpenHashTable* table, int key, int value);
HashSearchResult DoubleHashingSearch(OpenHashTable* table, int key);
bool DoubleHashingDelete(OpenHashTable* table, int key);

// 辅助哈希函数（用于双重哈希）
int SecondaryHash(int key);

// ============= 链地址法（ 重点）=============

/**
 * 🔗 链地址法（Chaining， 核心）
 * 
 *   链地址法结构图示：
 * 
 * 索引  链表
 *  0  → [10] → [23] → [36] → NULL
 *  1  → [14] → NULL
 *  2  → [15] → [28] → NULL
 *  3  → NULL
 *  4  → [17] → [30] → NULL
 *  ...
 * 
 *  算法特点：
 * - 冲突处理简单，直接链接到链表
 * - 不会产生聚集现象
 * - 装填因子可以大于1
 * - 删除操作简单
 * - 需要额外的存储空间存储指针
 * 
 *   复杂度分析：
 * - 成功查找：ASL = 1 + α/2
 * - 失败查找：ASL = α
 * - 其中α为装填因子
 * 
 *  
 * - 理解链地址法的基本原理
 * - 掌握ASL的计算方法
 * - 了解与开放地址法的比较
 * - 理解装填因子对性能的影响
 */

/**
 * 初始化链地址法哈希表
 */
void InitChainHashTable(ChainHashTable* table, int size);
void DestroyChainHashTable(ChainHashTable* table);

/**
 * 链地址法插入
 */
bool ChainHashInsert(ChainHashTable* table, int key, int value);

/**
 * 链地址法查找
 */
HashSearchResult ChainHashSearch(ChainHashTable* table, int key);

/**
 * 链地址法删除
 */
bool ChainHashDelete(ChainHashTable* table, int key);

// 链表辅助函数
ChainNode* CreateChainNode(int key, int value);
void DestroyChainNode(ChainNode* node);

// ============= 哈希表动态调整 =============

/**
 *  哈希表扩容（Rehashing）
 * 
 *   扩容过程：
 * 1. 创建新的更大的哈希表
 * 2. 重新计算所有元素的哈希值
 * 3. 将元素插入到新表中
 * 4. 释放旧表的内存
 * 
 *  扩容时机：
 * - 装填因子超过阈值时
 * - 冲突过于频繁时
 * - 查找性能下降时
 * 
 * @param table 哈希表指针
 * @param new_size 新的表大小
 * @return 扩容是否成功
 */
bool RehashOpenTable(OpenHashTable* table, int new_size);
bool RehashChainTable(ChainHashTable* table, int new_size);

/**
 * 检查是否需要扩容
 */
bool NeedRehash(double load_factor, int collisions);

/**
 * 获取下一个质数（用于确定新表大小）
 */
int GetNextPrime(int n);
bool IsPrime(int n);

// ============= 哈希表统计和分析 =============

/**
 * 哈希表统计信息
 */
typedef struct {
    int total_elements;          // 总元素数
    int table_size;              // 表大小
    double load_factor;          // 装填因子
    int collisions;              // 冲突次数
    double avg_probe_length;     // 平均探测长度
    int max_probe_length;        // 最大探测长度
    int empty_slots;             // 空槽位数
} HashTableStats;

/**
 * 计算哈希表统计信息
 */
void CalculateOpenHashStats(OpenHashTable* table, HashTableStats* stats);
void CalculateChainHashStats(ChainHashTable* table, HashTableStats* stats);
void PrintHashTableStats(HashTableStats* stats, const char* table_type);

/**
 * 分析哈希函数分布均匀性
 */
void AnalyzeHashDistribution(int keys[], int n, int table_size, const char* hash_func_name);

// ============= 哈希表性能测试 =============

/**
 * 性能测试结果
 */
typedef struct {
    char method_name[50];        // 方法名称
    double insert_time;          // 插入时间
    double search_time;          // 查找时间
    double delete_time;          // 删除时间
    double avg_probe_length;     // 平均探测长度
    int collision_count;         // 冲突次数
} HashPerformanceResult;

/**
 * 哈希表性能比较测试
 */
void CompareHashMethods(int data_sizes[], int size_count, 
                       HashPerformanceResult results[]);

void PerformanceTest_OpenHashing(int size);
void PerformanceTest_ChainHashing(int size);
void PerformanceTest_HashFunctions(int size);

// ============= 哈希表应用示例 =============

/**
 * 符号表应用（编译器中的应用）
 */
typedef struct {
    char name[50];               // 变量名
    int type;                    // 类型
    int scope;                   // 作用域
    int offset;                  // 偏移量
} Symbol;

typedef struct {
    ChainHashTable* table;       // 哈希表
    Symbol* symbols;             // 符号数组
    int symbol_count;            // 符号数量
} SymbolTable;

void InitSymbolTable(SymbolTable* st, int size);
bool AddSymbol(SymbolTable* st, const char* name, int type, int scope, int offset);
Symbol* FindSymbol(SymbolTable* st, const char* name);
bool RemoveSymbol(SymbolTable* st, const char* name);
void PrintSymbolTable(SymbolTable* st);

/**
 * 缓存系统应用
 */
typedef struct {
    int key;                     // 缓存键
    void* data;                  // 缓存数据
    int access_count;            // 访问次数
    time_t timestamp;            // 时间戳
} CacheEntry;

typedef struct {
    ChainHashTable* table;       // 哈希表
    CacheEntry* entries;         // 缓存项数组
    int capacity;                // 容量
    int hit_count;               // 命中次数
    int miss_count;              // 失效次数
} HashCache;

void InitHashCache(HashCache* cache, int capacity);
bool CachePut(HashCache* cache, int key, void* data, int data_size);
void* CacheGet(HashCache* cache, int key);
void PrintCacheStats(HashCache* cache);

// ============= 哈希表调试和可视化 =============

void PrintOpenHashTable(OpenHashTable* table);
void PrintChainHashTable(ChainHashTable* table);
void VisualizeHashTable(OpenHashTable* table);
void VisualizeChainTable(ChainHashTable* table);

// =============  真题演示 =============

/**
 *  真题演示函数
 */
void ExamDemo_HashFunction(void);        // 哈希函数计算
void ExamDemo_LinearProbing(void);       // 线性探测
void ExamDemo_ChainHashing(void);        // 链地址法
void ExamDemo_HashASL(void);             // 平均查找长度计算

/**
 * 哈希表教学演示
 */
void HashTableDemo(void);
void InteractiveHashDemo(void);

#endif // HASHTABLE_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 hashtable.c test_hashtable.c -o test_hashtable
 * ================================ */ 