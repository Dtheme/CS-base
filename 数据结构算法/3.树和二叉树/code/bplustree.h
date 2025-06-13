/* bplustree.h - B+树数据结构
 * create by: zw.duan
 * 
 * B+树特性（与B树的区别）：
 * 1. 所有关键字都出现在叶子节点中，叶子节点包含全部关键字信息
 * 2. 非叶子节点只起到索引作用，不存储实际数据
 * 3. 叶子节点通过指针连接，形成有序链表，便于范围查询
 * 4. 所有叶子节点在同一层，树完全平衡
 * 5. 非叶子节点的关键字是其子树中最大（或最小）关键字的副本
 */

#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= B+树相关定义 =============

#define BPLUS_ORDER 4  // B+树的阶数（可调整）
#define BPLUS_MAX_KEYS (BPLUS_ORDER - 1)        // 内部节点最大关键字数
#define BPLUS_MIN_KEYS ((BPLUS_ORDER + 1) / 2 - 1)  // 内部节点最小关键字数
#define BPLUS_MAX_LEAF_KEYS BPLUS_ORDER         // 叶子节点最大关键字数
#define BPLUS_MIN_LEAF_KEYS ((BPLUS_ORDER + 1) / 2)  // 叶子节点最小关键字数
#define BPLUS_MAX_CHILDREN BPLUS_ORDER          // 节点最大子节点数

/**
 * B+树关键字类型
 */
typedef int BPlusKeyType;

/**
 * B+树数据类型（叶子节点存储的实际数据）
 */
typedef struct {
    BPlusKeyType key;
    void* data;        // 指向实际数据的指针
    int data_size;     // 数据大小
} BPlusData;

/**
 * B+树节点类型枚举
 */
typedef enum {
    BPLUS_INTERNAL,    // 内部节点
    BPLUS_LEAF         // 叶子节点
} BPlusNodeType;

/**
 * B+树节点定义
 */
typedef struct BPlusNode {
    BPlusNodeType type;                          // 节点类型
    int num_keys;                                // 当前关键字数量
    BPlusKeyType keys[BPLUS_MAX_LEAF_KEYS];     // 关键字数组（统一用最大的）
    
    union {
        // 内部节点数据
        struct {
            struct BPlusNode* children[BPLUS_MAX_CHILDREN];  // 子节点指针
        } internal;
        
        // 叶子节点数据
        struct {
            BPlusData data[BPLUS_MAX_LEAF_KEYS];    // 数据数组
            struct BPlusNode* next;                  // 指向下一个叶子节点
            struct BPlusNode* prev;                  // 指向前一个叶子节点
        } leaf;
    };
    
    struct BPlusNode* parent;                    // 父节点指针
} BPlusNode;

/**
 * B+树结构定义
 */
typedef struct {
    BPlusNode* root;                             // 根节点指针
    BPlusNode* leftmost_leaf;                    // 最左叶子节点（用于顺序遍历）
    BPlusNode* rightmost_leaf;                   // 最右叶子节点
    int order;                                   // B+树的阶数
    int height;                                  // 树的高度
    int total_nodes;                             // 总节点数
    int total_keys;                              // 总关键字数
} BPlusTree;

/**
 * B+树查找结果结构
 */
typedef struct {
    BPlusNode* node;                             // 找到的叶子节点
    int index;                                   // 关键字在叶子节点中的位置
    bool found;                                  // 是否找到
    BPlusData* data;                            // 找到的数据（如果found为true）
} BPlusSearchResult;

// ============= B+树基本操作 =============

// 初始化和销毁
void InitBPlusTree(BPlusTree* tree, int order);
void DestroyBPlusTree(BPlusTree* tree);
void DestroyBPlusNode(BPlusNode* node);
bool BPlusTreeEmpty(BPlusTree* tree);

// 节点操作
BPlusNode* CreateBPlusNode(BPlusNodeType type);
void InitBPlusNode(BPlusNode* node, BPlusNodeType type);

// ============= B+树核心操作（ 重点）=============

/**
 *  B+树查找操作（Search， 核心）
 * 
 *   B+树查找过程图示（查找关键字15）：
 * 
 * B+树结构：
 *           [10, 20]           <- 内部节点（索引）
 *         /    |    \
 *    [5,8,10] [12,15,18,20] [25,30]  <- 叶子节点（数据）
 *       ↕        ↕           ↕
 *    [双向链表连接所有叶子节点]
 * 
 * 查找过程：
 * 1. 从根节点开始：15 > 10 且 15 < 20，进入中间子树
 * 2. 到达叶子节点[12,15,18,20]，找到15及其关联的数据
 * 
 *  算法步骤：
 * 1. 从根节点开始向下查找
 * 2. 在内部节点中确定子树方向
 * 3. 最终到达叶子节点进行精确查找
 * 4. 返回数据记录（而非仅仅关键字）
 * 
 *  关键点：
 * - 查找必须到达叶子节点
 * - 叶子节点包含完整数据
 * - 支持点查询和范围查询
 * - 所有查找路径长度相同
 * 
 *   复杂度分析：
 * - 时间复杂度：O(log_m n)
 * - 空间复杂度：O(h)（递归栈）
 * 
 * @param tree B+树指针
 * @param key 要查找的关键字
 * @return 查找结果结构
 */
BPlusSearchResult BPlusSearch(BPlusTree* tree, BPlusKeyType key);
BPlusNode* BPlusSearchLeaf(BPlusNode* node, BPlusKeyType key);

/**
 * 🌱 B+树插入操作（Insert， 核心）
 * 
 *   B+树插入特性：
 * 
 * 1. 所有插入都在叶子节点进行
 * 2. 叶子节点分裂时，右节点的第一个关键字复制到父节点
 * 3. 内部节点分裂时，中间关键字上移（不复制）
 * 
 * 插入过程示例（插入16）：
 * 插入前：[12,15,18,20] 
 * 插入后：[12,15,16,18,20] 或者分裂成两个节点
 * 
 *  算法步骤：
 * 1. 找到要插入的叶子节点
 * 2. 在叶子节点中插入数据
 * 3. 如果叶子节点溢出，分裂叶子节点
 * 4. 将分裂产生的关键字插入父节点
 * 5. 递归处理父节点的可能分裂
 * 
 * @param tree B+树指针
 * @param key 要插入的关键字
 * @param data 要插入的数据
 * @param data_size 数据大小
 * @return 插入是否成功
 */
bool BPlusInsert(BPlusTree* tree, BPlusKeyType key, void* data, int data_size);
void BPlusInsertIntoLeaf(BPlusNode* leaf, BPlusKeyType key, void* data, int data_size);
void BPlusInsertIntoParent(BPlusTree* tree, BPlusNode* left, BPlusKeyType key, BPlusNode* right);
BPlusNode* BPlusSplitLeaf(BPlusNode* leaf);
BPlusNode* BPlusSplitInternal(BPlusNode* node);

/**
 *  B+树删除操作（Delete， 重点）
 * 
 *   B+树删除特性：
 * 
 * 1. 只从叶子节点删除数据
 * 2. 内部节点的关键字可能需要更新
 * 3. 删除后可能需要合并节点或重新分配
 * 
 * @param tree B+树指针
 * @param key 要删除的关键字
 * @return 删除是否成功
 */
bool BPlusDelete(BPlusTree* tree, BPlusKeyType key);
void BPlusDeleteFromLeaf(BPlusNode* leaf, int index);
void BPlusMergeNodes(BPlusTree* tree, BPlusNode* left, BPlusNode* right);
void BPlusRedistribute(BPlusNode* left, BPlusNode* right, BPlusNode* parent, int parent_index);

// ============= B+树范围查询（ 重点应用）=============

/**
 *  B+树范围查询（Range Query，B+树的核心优势）
 * 
 *   范围查询过程图示（查询10 ≤ key ≤ 20）：
 * 
 * 步骤1：找到起始位置（key=10的叶子节点）
 * 步骤2：沿着叶子节点链表顺序遍历
 * 步骤3：收集满足条件的所有数据，直到key>20
 * 
 *  算法优势：
 * - 只需一次树遍历找到起始位置
 * - 利用叶子节点链表实现高效的顺序访问
 * - 非常适合数据库的范围查询需求
 * 
 * @param tree B+树指针
 * @param low 范围下界
 * @param high 范围上界
 * @param result 结果数组
 * @param count 结果数量
 */
void BPlusRangeQuery(BPlusTree* tree, BPlusKeyType low, BPlusKeyType high, 
                     BPlusData* result, int* count);
int BPlusCountRange(BPlusTree* tree, BPlusKeyType low, BPlusKeyType high);

// 叶子节点顺序遍历
void BPlusSequentialScan(BPlusTree* tree, void (*callback)(BPlusData*));
BPlusNode* BPlusGetNextLeaf(BPlusNode* current);
BPlusNode* BPlusGetPrevLeaf(BPlusNode* current);

// ============= B+树遍历操作 =============

void BPlusInOrder(BPlusTree* tree);
void BPlusInOrderNode(BPlusNode* node);
void BPlusLevelOrder(BPlusTree* tree);
void BPlusLeafOrder(BPlusTree* tree);  // 按叶子节点链表顺序遍历

// ============= B+树辅助操作 =============

// 查找辅助
int BPlusFindKeyIndex(BPlusNode* node, BPlusKeyType key);
int BPlusFindChildIndex(BPlusNode* node, BPlusKeyType key);
BPlusNode* BPlusFindLeafForKey(BPlusTree* tree, BPlusKeyType key);

// 节点操作
void BPlusShiftKeysLeft(BPlusNode* node, int start_index);
void BPlusShiftKeysRight(BPlusNode* node, int start_index);
void BPlusShiftChildrenLeft(BPlusNode* node, int start_index);
void BPlusShiftChildrenRight(BPlusNode* node, int start_index);

// 键值操作
BPlusKeyType BPlusGetMaxKey(BPlusNode* node);
BPlusKeyType BPlusGetMinKey(BPlusNode* node);
void BPlusUpdateParentKey(BPlusNode* node, BPlusKeyType old_key, BPlusKeyType new_key);

// ============= B+树验证和调试 =============

bool ValidateBPlusTree(BPlusTree* tree);
bool ValidateBPlusNode(BPlusNode* node, BPlusKeyType min_key, BPlusKeyType max_key, 
                       int expected_height, int current_height);
bool ValidateLeafChain(BPlusTree* tree);
void PrintBPlusTree(BPlusTree* tree);
void PrintBPlusNode(BPlusNode* node, int level);
void PrintBPlusStructure(BPlusTree* tree);
void PrintLeafChain(BPlusTree* tree);

// 统计信息
int BPlusHeight(BPlusTree* tree);
int BPlusNodeCount(BPlusTree* tree);
int BPlusLeafNodeCount(BPlusTree* tree);
int BPlusInternalNodeCount(BPlusTree* tree);
int BPlusKeyCount(BPlusTree* tree);

// ============= B+树性能测试 =============

void PerformanceTest_BPlusInsert(int size);
void PerformanceTest_BPlusSearch(int size);
void PerformanceTest_BPlusRangeQuery(int size);
void PerformanceCompare_BPlusVsBTree(int size);
void PerformanceCompare_BPlusVsBST(int size);

// ============= B+树高级应用 =============

// 数据库索引模拟
typedef struct {
    int record_id;
    char data[256];
    long timestamp;
} DatabaseRecord;

typedef struct {
    BPlusTree* primary_index;    // 主索引
    BPlusTree* secondary_index;  // 辅助索引
    DatabaseRecord* records;     // 实际数据存储
    int record_count;
    int capacity;
} DatabaseTable;

void InitDatabaseTable(DatabaseTable* table, int capacity);
bool InsertRecord(DatabaseTable* table, int key, const char* data);
DatabaseRecord* FindRecord(DatabaseTable* table, int key);
bool DeleteRecord(DatabaseTable* table, int key);
void RangeQueryRecords(DatabaseTable* table, int low, int high, 
                       DatabaseRecord* results, int* count);

// 文件索引应用
typedef struct {
    char filename[256];
    long file_offset;
    int file_size;
    int block_id;
} FileIndexEntry;

typedef struct {
    BPlusTree* file_index;
    FileIndexEntry* entries;
    int entry_count;
} FileIndexSystem;

void InitFileIndexSystem(FileIndexSystem* fis);
bool AddFileIndex(FileIndexSystem* fis, const char* filename, long offset, int size);
FileIndexEntry* FindFileIndex(FileIndexSystem* fis, const char* filename);
void GetFilesInRange(FileIndexSystem* fis, const char* start_name, const char* end_name,
                     FileIndexEntry* results, int* count);

#endif // BPLUSTREE_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 bplustree.c test_bplustree.c -o test_bplustree
 * ================================ */ 