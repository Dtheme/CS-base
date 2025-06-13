/* huffman.h - 哈夫曼树
 * create by: zw.duan
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ============= 哈夫曼树相关定义 =============

/**
 * 哈夫曼树节点定义
 */
typedef struct HuffmanNode {
    int weight;                     // 权重（频率）
    char data;                      // 字符数据（叶子节点有效）
    bool isLeaf;                    // 是否为叶子节点
    struct HuffmanNode *left;       // 左子树
    struct HuffmanNode *right;      // 右子树
    struct HuffmanNode *parent;     // 父节点（用于某些算法）
} HuffmanNode;

/**
 * 哈夫曼树结构
 */
typedef struct {
    HuffmanNode *root;              // 根节点
    int nodeCount;                  // 节点总数
    int leafCount;                  // 叶子节点数
} HuffmanTree;

/**
 * 字符频率统计结构
 */
typedef struct {
    char character;                 // 字符
    int frequency;                  // 频率
} CharFrequency;

/**
 * 哈夫曼编码表项
 */
typedef struct {
    char character;                 // 字符
    char *code;                     // 哈夫曼编码
    int codeLength;                 // 编码长度
} HuffmanCode;

/**
 * 哈夫曼编码表
 */
typedef struct {
    HuffmanCode *codes;             // 编码数组
    int count;                      // 编码数量
    int capacity;                   // 容量
} HuffmanCodeTable;

// ============= 哈夫曼树基本操作 =============

// 初始化和销毁
bool InitHuffmanTree(HuffmanTree *tree);
void DestroyHuffmanTree(HuffmanTree *tree);
void DestroyHuffmanNode(HuffmanNode *node);

// 节点创建
HuffmanNode* CreateHuffmanNode(char data, int weight, bool isLeaf);
HuffmanNode* CreateInternalNode(HuffmanNode *left, HuffmanNode *right);

// 树构建
HuffmanTree* BuildHuffmanTree(CharFrequency *frequencies, int count);
HuffmanTree* BuildHuffmanTreeFromString(const char *text);
HuffmanTree* BuildHuffmanTreeFromArray(int *weights, char *characters, int count);

// ============= 哈夫曼编码相关操作 =============

// 编码表操作
bool InitHuffmanCodeTable(HuffmanCodeTable *table, int capacity);
void DestroyHuffmanCodeTable(HuffmanCodeTable *table);
bool AddHuffmanCode(HuffmanCodeTable *table, char character, const char *code);

// 编码生成
void GenerateHuffmanCodes(HuffmanTree *tree, HuffmanCodeTable *table);
void GenerateCodesRecursive(HuffmanNode *node, char *currentCode, int depth, HuffmanCodeTable *table);
const char* GetCharacterCode(HuffmanCodeTable *table, char character);

// 编码和解码
char* EncodeString(const char *text, HuffmanCodeTable *table);
char* DecodeString(const char *encodedText, HuffmanTree *tree);
bool EncodeCharacter(char ch, HuffmanCodeTable *table, char *result, int maxLength);

// ============= 字符频率统计 =============

// 频率统计
CharFrequency* CountCharacterFrequencies(const char *text, int *count);
void PrintCharacterFrequencies(CharFrequency *frequencies, int count);
void SortFrequenciesByWeight(CharFrequency *frequencies, int count);

// 频率操作
int GetCharacterFrequency(CharFrequency *frequencies, int count, char character);
bool UpdateCharacterFrequency(CharFrequency *frequencies, int count, char character, int newFreq);

// ============= 哈夫曼树高级操作（ 重点）=============

// 树的性质
int GetHuffmanTreeDepth(HuffmanTree *tree);
int GetNodeDepth(HuffmanNode *node, HuffmanNode *target, int currentDepth);
int GetLeafCount(HuffmanTree *tree);
int GetInternalNodeCount(HuffmanTree *tree);

// 路径操作
bool FindPathToCharacter(HuffmanNode *node, char character, char *path, int *pathLength);
void GetAllLeafPaths(HuffmanNode *node, char *currentPath, int depth);

// WPL计算（带权路径长度）
int CalculateWPL(HuffmanTree *tree);
int CalculateWPLRecursive(HuffmanNode *node, int depth);
double CalculateAverageCodeLength(HuffmanTree *tree, CharFrequency *frequencies, int count);

// 验证操作
bool IsValidHuffmanTree(HuffmanTree *tree);
bool ValidateHuffmanProperty(HuffmanNode *node);
bool IsOptimalHuffmanTree(HuffmanTree *tree, CharFrequency *frequencies, int count);

// ============= 哈夫曼算法应用（算法设计题）=============

// 文件压缩
typedef struct {
    char *originalText;
    char *encodedText;
    HuffmanCodeTable *codeTable;
    int originalSize;
    int encodedSize;
    double compressionRatio;
} CompressionResult;

CompressionResult* CompressText(const char *text);
char* DecompressText(const char *encodedText, HuffmanCodeTable *table);
void DestroyCompressionResult(CompressionResult *result);

// 文件操作
bool SaveHuffmanTreeToFile(HuffmanTree *tree, const char *filename);
HuffmanTree* LoadHuffmanTreeFromFile(const char *filename);
bool SaveCodeTableToFile(HuffmanCodeTable *table, const char *filename);
HuffmanCodeTable* LoadCodeTableFromFile(const char *filename);

// 多进制哈夫曼树
typedef struct {
    HuffmanNode **children;         // 子节点数组
    int childCount;                 // 子节点数量
    int maxChildren;                // 最大子节点数（k进制）
} KaryHuffmanNode;

typedef struct {
    KaryHuffmanNode *root;
    int k;                          // k进制
    int nodeCount;
} KaryHuffmanTree;

KaryHuffmanTree* BuildKaryHuffmanTree(CharFrequency *frequencies, int count, int k);
void DestroyKaryHuffmanTree(KaryHuffmanTree *tree);

// ============= 遍历和输出 =============

// 树遍历
void PrintHuffmanTree(HuffmanTree *tree);
void PrintHuffmanTreeStructure(HuffmanNode *node, int level);
void PreOrderTraversal(HuffmanNode *node);
void InOrderTraversal(HuffmanNode *node);
void PostOrderTraversal(HuffmanNode *node);
void LevelOrderTraversal(HuffmanTree *tree);

// 编码表输出
void PrintHuffmanCodeTable(HuffmanCodeTable *table);
void PrintCodeTableDetailed(HuffmanCodeTable *table, CharFrequency *frequencies, int count);

// 可视化
void VisualizeHuffmanTree(HuffmanTree *tree);
void ExportHuffmanTreeToDot(HuffmanTree *tree, const char *filename);
void DrawHuffmanTreeASCII(HuffmanNode *node, int space);

// ============= 辅助函数和工具 =============

// 节点操作
bool IsLeafNode(HuffmanNode *node);
int GetNodeWeight(HuffmanNode *node);
char GetNodeData(HuffmanNode *node);
int CompareNodes(const void *a, const void *b);

// 字符串处理
char* StringDuplicate(const char *str);
bool IsValidCharacter(char ch);
int GetStringLength(const char *str);

// 内存管理
void* SafeMalloc(size_t size);
void* SafeRealloc(void *ptr, size_t size);

// 内部辅助函数
int GetNodeDepthRecursive(HuffmanNode *node);

// 优先队列（用于构建哈夫曼树）
typedef struct {
    HuffmanNode **nodes;
    int size;
    int capacity;
} HuffmanPriorityQueue;

bool InitPriorityQueue(HuffmanPriorityQueue *pq, int capacity);
void DestroyPriorityQueue(HuffmanPriorityQueue *pq);
bool EnqueueNode(HuffmanPriorityQueue *pq, HuffmanNode *node);
HuffmanNode* DequeueNode(HuffmanPriorityQueue *pq);
bool IsQueueEmpty(HuffmanPriorityQueue *pq);
int GetQueueSize(HuffmanPriorityQueue *pq);
void HeapifyUp(HuffmanPriorityQueue *pq, int index);
void HeapifyDown(HuffmanPriorityQueue *pq, int index);

// ============= 性能测试和分析 =============

// 性能测试
void PerformanceTest_HuffmanTreeBuild(int textLength);
void PerformanceTest_HuffmanEncoding(const char *text);
void PerformanceTest_HuffmanDecoding(const char *encodedText, HuffmanTree *tree);

// 压缩效果分析
void AnalyzeCompressionEfficiency(const char *text);
void CompareWithFixedLengthEncoding(const char *text);
double CalculateEntropy(CharFrequency *frequencies, int count);
double CalculateCompressionRatio(int originalSize, int compressedSize);

// 测试数据生成
char* GenerateRandomText(int length);
char* GenerateTextWithFrequency(CharFrequency *frequencies, int count, int totalLength);
CharFrequency* GenerateRandomFrequencies(int charCount, int maxFreq);

// ============= 错误处理和调试 =============

// 错误码定义
typedef enum {
    HUFFMAN_SUCCESS = 0,
    HUFFMAN_ERROR_NULL_POINTER,
    HUFFMAN_ERROR_INVALID_PARAMETER,
    HUFFMAN_ERROR_MEMORY_ALLOCATION,
    HUFFMAN_ERROR_EMPTY_INPUT,
    HUFFMAN_ERROR_INVALID_CODE,
    HUFFMAN_ERROR_FILE_OPERATION,
    HUFFMAN_ERROR_TREE_INVALID
} HuffmanErrorCode;

// 错误处理
const char* GetErrorMessage(HuffmanErrorCode code);
void LogError(HuffmanErrorCode code, const char *function, int line);

// 调试函数
void DebugPrintNode(HuffmanNode *node);
void DebugPrintTree(HuffmanTree *tree);
void DebugPrintCodeTable(HuffmanCodeTable *table);
bool ValidateTreeIntegrity(HuffmanTree *tree);

#endif // HUFFMAN_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -O2 huffman.c test_huffman.c -o test_huffman
 * 
 *  重点：
 * 1. 哈夫曼树构造算法 - 贪心算法的经典应用
 * 2. 哈夫曼编码生成 - 前缀编码的实现
 * 3. WPL计算 - 带权路径长度的含义和计算
 * 4. 压缩原理 - 变长编码vs定长编码的优势
 * 5. 算法时间复杂度 - O(n log n)的构建复杂度
 * ================================ */ 