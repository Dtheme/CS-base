/* btree.h - B树数据结构
 * create by: zw.duan
 * 
 * B树特性（m阶B树）：
 * 1. 每个节点最多有m个子节点
 * 2. 除根节点外，每个内部节点至少有⌈m/2⌉个子节点
 * 3. 根节点至少有2个子节点（如果它不是叶子节点）
 * 4. 有k个子节点的非叶子节点有k-1个关键字
 * 5. 所有叶子节点都在同一层
 */

#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= B树相关定义 =============

#define BTREE_ORDER 5  // B树的阶数（可调整）
#define MAX_KEYS (BTREE_ORDER - 1)      // 节点最大关键字数
#define MIN_KEYS ((BTREE_ORDER + 1) / 2 - 1)  // 节点最小关键字数（除根节点）
#define MAX_CHILDREN BTREE_ORDER         // 节点最大子节点数
#define MIN_CHILDREN ((BTREE_ORDER + 1) / 2)  // 节点最小子节点数

/**
 * B树关键字类型
 */
typedef int BTreeKeyType;

/**
 * B树节点定义
 */
typedef struct BTreeNode {
    BTreeKeyType keys[MAX_KEYS];         // 关键字数组
    struct BTreeNode* children[MAX_CHILDREN];  // 子节点指针数组
    int num_keys;                        // 当前关键字数量
    bool is_leaf;                        // 是否为叶子节点
} BTreeNode;

/**
 * B树结构定义
 */
typedef struct {
    BTreeNode* root;                     // 根节点指针
    int order;                           // B树的阶数
    int height;                          // 树的高度
    int total_nodes;                     // 总节点数
} BTree;

/**
 * 查找结果结构
 */
typedef struct {
    BTreeNode* node;                     // 找到的节点
    int index;                           // 关键字在节点中的位置
    bool found;                          // 是否找到
} BTreeSearchResult;

// ============= B树基本操作 =============

// 初始化和销毁
void InitBTree(BTree* tree, int order);
void DestroyBTree(BTree* tree);
void DestroyBTreeNode(BTreeNode* node);
bool BTreeEmpty(BTree* tree);

// 节点操作
BTreeNode* CreateBTreeNode(bool is_leaf);
void InitBTreeNode(BTreeNode* node, bool is_leaf);

// ============= B树核心操作（ 重点）=============

/**
 *  B树查找操作（Search， 核心）
 * 
 *   B树查找过程图示（在5阶B树中查找关键字15）：
 * 
 * 初始状态：
 *              [10, 20]
 *            /    |    \
 *      [5, 8]  [12, 15, 18]  [25, 30]
 * 
 * 查找过程：
 * 1. 从根节点开始：比较15与10、20
 *    15 > 10 且 15 < 20，进入中间子树
 * 2. 在节点[12, 15, 18]中查找：
 *    找到15，返回成功
 * 
 *  算法步骤：
 * 1. 从根节点开始
 * 2. 在当前节点中进行线性查找或二分查找
 * 3. 如果找到，返回结果
 * 4. 如果未找到且为叶子节点，返回失败
 * 5. 如果未找到且为内部节点，递归到相应子树
 * 
 *  关键点：
 * - 每个节点内关键字有序
 * - 利用B树的性质确定搜索路径
 * - 查找路径长度 ≤ 树的高度
 * - 磁盘存储中减少I/O次数
 * 
 *   复杂度分析：
 * - 时间复杂度：O(log_m n)，m为阶数，n为关键字总数
 * - 空间复杂度：O(h)，h为树高（递归栈深度）
 * 
 * @param tree B树指针
 * @param key 要查找的关键字
 * @return 查找结果结构
 */
BTreeSearchResult BTreeSearch(BTree* tree, BTreeKeyType key);
BTreeSearchResult BTreeSearchNode(BTreeNode* node, BTreeKeyType key);

/**
 * 🌱 B树插入操作（Insert， 核心）
 * 
 *   B树插入过程图示（向5阶B树插入关键字16）：
 * 
 * 插入前：
 *              [10, 20]
 *            /    |    \
 *      [5, 8]  [12, 15, 18]  [25, 30]
 * 
 * 步骤1：找到插入位置（叶子节点[12, 15, 18]）
 * 步骤2：节点已满，需要分裂
 * 步骤3：分裂节点，中间关键字15上移
 * 
 * 插入后：
 *           [10, 15, 20]
 *         /    |    |    \
 *    [5, 8]  [12]  [16, 18]  [25, 30]
 * 
 *  算法步骤：
 * 1. 如果树为空，创建根节点
 * 2. 查找插入位置（叶子节点）
 * 3. 如果叶子节点未满，直接插入
 * 4. 如果叶子节点已满，分裂节点：
 *    a) 创建新节点
 *    b) 将关键字和子节点分配到两个节点
 *    c) 中间关键字上移到父节点
 * 5. 递归处理父节点的分裂（如果需要）
 * 
 *  关键点：
 * - 保持B树的平衡性质
 * - 分裂时选择中间关键字上移
 * - 自底向上的分裂传播
 * - 根节点分裂时树高增加
 * 
 *   复杂度分析：
 * - 时间复杂度：O(log_m n)
 * - 空间复杂度：O(h)
 * 
 * @param tree B树指针
 * @param key 要插入的关键字
 * @return 插入是否成功
 */
bool BTreeInsert(BTree* tree, BTreeKeyType key);
void BTreeInsertNonFull(BTreeNode* node, BTreeKeyType key);
void BTreeInsertNonFullWithOrder(BTreeNode* node, BTreeKeyType key, int order);
void BTreeSplitChild(BTreeNode* parent, int index);
void BTreeSplitChildWithOrder(BTreeNode* parent, int index, int order);

/**
 *  B树删除操作（Delete， 重点）
 * 
 *   B树删除的三种情况：
 * 
 * 情况1：删除叶子节点中的关键字
 * 情况2：删除内部节点中的关键字
 * 情况3：删除导致节点关键字数量过少需要合并
 * 
 *  算法步骤：
 * 1. 找到要删除的关键字
 * 2. 根据不同情况处理：
 *    - 叶子节点：直接删除
 *    - 内部节点：用前驱或后继替换
 *    - 节点过少：合并或借用
 * 3. 维护B树性质
 * 
 * @param tree B树指针
 * @param key 要删除的关键字
 * @return 删除是否成功
 */
bool BTreeDelete(BTree* tree, BTreeKeyType key);
void BTreeDeleteFromNode(BTreeNode* node, BTreeKeyType key);
void BTreeMergeNodes(BTreeNode* parent, int left_index, int right_index);
void BTreeBorrowFromLeft(BTreeNode* parent, int child_index);
void BTreeBorrowFromRight(BTreeNode* parent, int child_index);

// ============= B树辅助操作 =============

// 节点操作
BTreeKeyType BTreeFindPredecessor(BTreeNode* node);
BTreeKeyType BTreeFindSuccessor(BTreeNode* node);
void BTreeShiftKeysLeft(BTreeNode* node, int start_index);
void BTreeShiftKeysRight(BTreeNode* node, int start_index);

// 查找辅助
int BTreeFindKeyIndex(BTreeNode* node, BTreeKeyType key);
int BTreeFindChildIndex(BTreeNode* node, BTreeKeyType key);

// ============= B树遍历操作 =============

void BTreeInOrder(BTree* tree);
void BTreeInOrderNode(BTreeNode* node);
void BTreePreOrder(BTree* tree);
void BTreePreOrderNode(BTreeNode* node);
void BTreeLevelOrder(BTree* tree);

// ============= B树高级操作（ 算法设计）=============

// 范围查询
void BTreeRangeQuery(BTree* tree, BTreeKeyType low, BTreeKeyType high, 
                     BTreeKeyType* result, int* count);
int BTreeCountRange(BTree* tree, BTreeKeyType low, BTreeKeyType high);

// 第k小元素
BTreeKeyType BTreeKthSmallest(BTree* tree, int k);
int BTreeRank(BTree* tree, BTreeKeyType key);

// 树的合并和分割
BTree* BTreeMerge(BTree* tree1, BTree* tree2);
void BTreeSplit(BTree* tree, BTreeKeyType key, BTree** left_tree, BTree** right_tree);

// ============= B树验证和调试 =============

bool ValidateBTree(BTree* tree);
bool ValidateBTreeNode(BTreeNode* node, BTreeKeyType min_key, BTreeKeyType max_key, int expected_height, int current_height);
void PrintBTree(BTree* tree);
void PrintBTreeNode(BTreeNode* node, int level);
void PrintBTreeStructure(BTree* tree);

// 统计信息
int BTreeHeight(BTree* tree);
int BTreeNodeCount(BTree* tree);
int BTreeKeyCount(BTree* tree);
int BTreeKeyCountNode(BTreeNode* node);
int BTreeLeafCount(BTree* tree);
int BTreeLeafCountNode(BTreeNode* node);

// 内部辅助函数
void BTreeRangeQueryHelper(BTreeNode* node, BTreeKeyType low, BTreeKeyType high, 
                          BTreeKeyType* result, int* count);
BTreeKeyType BTreeKthHelper(BTreeNode* node, int k, int* current);

// ============= B树性能测试 =============

void PerformanceTest_BTreeInsert(int size);
void PerformanceTest_BTreeSearch(int size);
void PerformanceTest_BTreeDelete(int size);
void PerformanceCompare_BTreeVsBST(int size);

// ============= B树应用示例 =============

// 文件系统索引模拟
typedef struct {
    char filename[256];
    int file_id;
    long file_size;
} FileInfo;

typedef struct {
    BTree* index_tree;
    FileInfo* files;
    int file_count;
} FileSystem;

void InitFileSystem(FileSystem* fs);
bool AddFile(FileSystem* fs, const char* filename, int file_id, long file_size);
FileInfo* FindFile(FileSystem* fs, const char* filename);
bool DeleteFile(FileSystem* fs, const char* filename);

#endif // BTREE_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 btree.c test_btree.c -o test_btree
 * ================================ */ 