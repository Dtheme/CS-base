/* hashtable_simple.h - 简化的散列表头文件
 * create by: zw.duan
 * 
 * 与hashtable.c实现文件匹配的头文件
 */

#ifndef HASHTABLE_SIMPLE_H
#define HASHTABLE_SIMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// ============= 基本类型定义 =============

// 哈希表状态枚举
typedef enum {
    EMPTY = 0,                   // 空位置
    OCCUPIED = 1,                // 占用位置
    DELETED = 2                  // 已删除位置
} HashStatus;

// 哈希函数类型定义
typedef unsigned int (*HashFunction)(int key, int table_size);

// 探测方法枚举
typedef enum {
    LINEAR_PROBE = 0,            // 线性探测
    QUADRATIC_PROBE = 1,         // 二次探测
    DOUBLE_HASH = 2              // 双重哈希
} ProbeMethod;

// ============= 开放地址法结构 =============

// 哈希表元素
typedef struct {
    int key;                     // 关键字
    int value;                   // 数据值
    HashStatus status;           // 位置状态
} HashEntry;

// 统计信息结构
typedef struct {
    int total_insertions;        // 总插入次数
    int total_searches;          // 总查找次数
    int total_deletions;         // 总删除次数
    int collision_count;         // 冲突次数
    int probe_count;             // 探测次数
    int max_probe_length;        // 最大探测长度
} HashStats;

// 开放地址法哈希表
typedef struct {
    HashEntry* table;            // 哈希表数组
    int size;                    // 表的大小
    int count;                   // 当前元素个数
    HashFunction hash_func;      // 哈希函数
    ProbeMethod probe_method;    // 探测方法
    HashStats stats;             // 统计信息
} OpenHashTable;

// ============= 链地址法结构 =============

// 链表节点
typedef struct HashNode {
    int key;                     // 关键字
    int value;                   // 数据值
    struct HashNode* next;       // 下一个节点
} HashNode;

// 链地址法统计信息
typedef struct {
    int total_insertions;        // 总插入次数
    int total_searches;          // 总查找次数
    int total_deletions;         // 总删除次数
    int collision_count;         // 冲突次数
    int max_chain_length;        // 最大链长度
    int total_chain_length;      // 总链长度
} ChainHashStats;

// 链地址法哈希表
typedef struct {
    HashNode** table;            // 链表数组
    int size;                    // 表的大小
    int count;                   // 当前元素个数
    HashFunction hash_func;      // 哈希函数
    ChainHashStats stats;        // 统计信息
} ChainHashTable;

// ============= 查找结果结构 =============

typedef struct {
    bool found;                  // 是否找到
    int index;                   // 找到的位置索引
    int comparisons;             // 比较次数
    double time_cost;            // 耗时
} SearchResult;

// ============= 哈希函数声明 =============

unsigned int DivisionHash(int key, int table_size);
unsigned int MultiplicationHash(int key, int table_size);
unsigned int SquareHash(int key, int table_size);
unsigned int SecondHash(int key, int table_size);

// ============= 开放地址法函数声明 =============

void InitOpenHashTable(OpenHashTable* table, int size, HashFunction hash_func, 
                      ProbeMethod probe_method);
bool OpenHashInsert(OpenHashTable* table, int key, int value);
bool OpenHashSearch(OpenHashTable* table, int key, int* value);
bool OpenHashDelete(OpenHashTable* table, int key);
bool ResizeOpenHashTable(OpenHashTable* table);
void FreeOpenHashTable(OpenHashTable* table);
void PrintOpenHashTable(OpenHashTable* table);
void PrintOpenHashStats(OpenHashTable* table);
double GetOpenHashLoadFactor(OpenHashTable* table);

// ============= 链地址法函数声明 =============

void InitChainHashTable(ChainHashTable* table, int size, HashFunction hash_func);
bool ChainHashInsert(ChainHashTable* table, int key, int value);
bool ChainHashSearch(ChainHashTable* table, int key, int* value);
bool ChainHashDelete(ChainHashTable* table, int key);
bool ResizeChainHashTable(ChainHashTable* table);
void FreeChainHashTable(ChainHashTable* table);
void PrintChainHashTable(ChainHashTable* table);
void PrintChainHashStats(ChainHashTable* table);
double GetChainHashLoadFactor(ChainHashTable* table);
double GetAverageChainLength(ChainHashTable* table);

#endif // HASHTABLE_SIMPLE_H 