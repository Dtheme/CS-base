/* huffman.c - 哈夫曼树（Huffman Tree）数据结构实现
 * create by: zw.duan
 *   第3章 树和二叉树
 */

#include "huffman.h"
#include <time.h>
#include <math.h>
#include <limits.h>

// ============= 哈夫曼树基本操作 =============

/**
 * 初始化哈夫曼树
 */
bool InitHuffmanTree(HuffmanTree *tree) {
    if (tree == NULL) return false;
    
    tree->root = NULL;
    tree->nodeCount = 0;
    tree->leafCount = 0;
    
    return true;
}

/**
 * 销毁哈夫曼树
 */
void DestroyHuffmanTree(HuffmanTree *tree) {
    if (tree == NULL) return;
    
    DestroyHuffmanNode(tree->root);
    tree->root = NULL;
    tree->nodeCount = 0;
    tree->leafCount = 0;
}

/**
 * 递归销毁哈夫曼树节点
 */
void DestroyHuffmanNode(HuffmanNode *node) {
    if (node == NULL) return;
    
    DestroyHuffmanNode(node->left);
    DestroyHuffmanNode(node->right);
    free(node);
}

/**
 * 创建哈夫曼树节点
 */
HuffmanNode* CreateHuffmanNode(char data, int weight, bool isLeaf) {
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    if (node == NULL) return NULL;
    
    node->data = data;
    node->weight = weight;
    node->isLeaf = isLeaf;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    
    return node;
}

/**
 * 创建内部节点
 */
HuffmanNode* CreateInternalNode(HuffmanNode *left, HuffmanNode *right) {
    if (left == NULL || right == NULL) return NULL;
    
    HuffmanNode *node = CreateHuffmanNode('\0', left->weight + right->weight, false);
    if (node == NULL) return NULL;
    
    node->left = left;
    node->right = right;
    left->parent = node;
    right->parent = node;
    
    return node;
}

// ============= 优先队列实现（用于构建哈夫曼树）=============

/**
 * 初始化优先队列
 */
bool InitPriorityQueue(HuffmanPriorityQueue *pq, int capacity) {
    if (pq == NULL || capacity <= 0) return false;
    
    pq->nodes = (HuffmanNode **)malloc(capacity * sizeof(HuffmanNode *));
    if (pq->nodes == NULL) return false;
    
    pq->size = 0;
    pq->capacity = capacity;
    
    return true;
}

/**
 * 销毁优先队列
 */
void DestroyPriorityQueue(HuffmanPriorityQueue *pq) {
    if (pq == NULL) return;
    
    free(pq->nodes);
    pq->nodes = NULL;
    pq->size = 0;
    pq->capacity = 0;
}

/**
 * 向上调整堆
 */
void HeapifyUp(HuffmanPriorityQueue *pq, int index) {
    if (pq == NULL || index <= 0) return;
    
    int parent = (index - 1) / 2;
    
    if (pq->nodes[index]->weight < pq->nodes[parent]->weight) {
        // 交换节点
        HuffmanNode *temp = pq->nodes[index];
        pq->nodes[index] = pq->nodes[parent];
        pq->nodes[parent] = temp;
        
        HeapifyUp(pq, parent);
    }
}

/**
 * 向下调整堆
 */
void HeapifyDown(HuffmanPriorityQueue *pq, int index) {
    if (pq == NULL) return;
    
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    
    if (left < pq->size && pq->nodes[left]->weight < pq->nodes[smallest]->weight) {
        smallest = left;
    }
    
    if (right < pq->size && pq->nodes[right]->weight < pq->nodes[smallest]->weight) {
        smallest = right;
    }
    
    if (smallest != index) {
        // 交换节点
        HuffmanNode *temp = pq->nodes[index];
        pq->nodes[index] = pq->nodes[smallest];
        pq->nodes[smallest] = temp;
        
        HeapifyDown(pq, smallest);
    }
}

/**
 * 入队操作
 */
bool EnqueueNode(HuffmanPriorityQueue *pq, HuffmanNode *node) {
    if (pq == NULL || node == NULL || pq->size >= pq->capacity) {
        return false;
    }
    
    pq->nodes[pq->size] = node;
    HeapifyUp(pq, pq->size);
    pq->size++;
    
    return true;
}

/**
 * 出队操作
 */
HuffmanNode* DequeueNode(HuffmanPriorityQueue *pq) {
    if (pq == NULL || pq->size == 0) return NULL;
    
    HuffmanNode *minNode = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;
    
    if (pq->size > 0) {
        HeapifyDown(pq, 0);
    }
    
    return minNode;
}

/**
 * 判断队列是否为空
 */
bool IsQueueEmpty(HuffmanPriorityQueue *pq) {
    return (pq == NULL || pq->size == 0);
}

/**
 * 获取队列大小
 */
int GetQueueSize(HuffmanPriorityQueue *pq) {
    return (pq == NULL) ? 0 : pq->size;
}

// ============= 字符频率统计 =============

/**
 * 统计字符频率
 */
CharFrequency* CountCharacterFrequencies(const char *text, int *count) {
    if (text == NULL || count == NULL) return NULL;
    
    // 使用数组统计频率（假设ASCII字符）
    int frequencies[256] = {0};
    int textLen = strlen(text);
    
    // 统计每个字符的频率
    for (int i = 0; i < textLen; i++) {
        frequencies[(unsigned char)text[i]]++;
    }
    
    // 计算不同字符的数量
    int charCount = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            charCount++;
        }
    }
    
    if (charCount == 0) {
        *count = 0;
        return NULL;
    }
    
    // 分配内存并填充结果
    CharFrequency *result = (CharFrequency *)malloc(charCount * sizeof(CharFrequency));
    if (result == NULL) {
        *count = 0;
        return NULL;
    }
    
    int index = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            result[index].character = (char)i;
            result[index].frequency = frequencies[i];
            index++;
        }
    }
    
    *count = charCount;
    return result;
}

/**
 * 按权重排序频率数组
 */
void SortFrequenciesByWeight(CharFrequency *frequencies, int count) {
    if (frequencies == NULL || count <= 1) return;
    
    // 简单的冒泡排序
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (frequencies[j].frequency > frequencies[j + 1].frequency) {
                CharFrequency temp = frequencies[j];
                frequencies[j] = frequencies[j + 1];
                frequencies[j + 1] = temp;
            }
        }
    }
}

/**
 * 打印字符频率
 */
void PrintCharacterFrequencies(CharFrequency *frequencies, int count) {
    if (frequencies == NULL || count <= 0) return;
    
    printf("字符频率统计:\n");
    printf("字符\t频率\n");
    printf("----\t----\n");
    
    for (int i = 0; i < count; i++) {
        char ch = frequencies[i].character;
        if (ch == ' ') {
            printf("空格\t%d\n", frequencies[i].frequency);
        } else if (ch == '\n') {
            printf("换行\t%d\n", frequencies[i].frequency);
        } else if (ch == '\t') {
            printf("制表\t%d\n", frequencies[i].frequency);
        } else {
            printf("%c\t%d\n", ch, frequencies[i].frequency);
        }
    }
}

// ============= 哈夫曼树构建 =============

/**
 * 从频率数组构建哈夫曼树
 */
HuffmanTree* BuildHuffmanTree(CharFrequency *frequencies, int count) {
    if (frequencies == NULL || count <= 0) return NULL;
    
    HuffmanTree *tree = (HuffmanTree *)malloc(sizeof(HuffmanTree));
    if (tree == NULL) return NULL;
    
    InitHuffmanTree(tree);
    
    // 特殊情况：只有一个字符
    if (count == 1) {
        tree->root = CreateHuffmanNode(frequencies[0].character, frequencies[0].frequency, true);
        tree->nodeCount = 1;
        tree->leafCount = 1;
        return tree;
    }
    
    // 创建优先队列
    HuffmanPriorityQueue pq;
    if (!InitPriorityQueue(&pq, count * 2)) {
        free(tree);
        return NULL;
    }
    
    // 为每个字符创建叶子节点并加入队列
    for (int i = 0; i < count; i++) {
        HuffmanNode *node = CreateHuffmanNode(frequencies[i].character, frequencies[i].frequency, true);
        if (node == NULL) {
            DestroyPriorityQueue(&pq);
            DestroyHuffmanTree(tree);
            free(tree);
            return NULL;
        }
        EnqueueNode(&pq, node);
    }
    
    tree->leafCount = count;
    tree->nodeCount = count;
    
    // 构建哈夫曼树
    while (GetQueueSize(&pq) > 1) {
        HuffmanNode *left = DequeueNode(&pq);
        HuffmanNode *right = DequeueNode(&pq);
        
        HuffmanNode *internal = CreateInternalNode(left, right);
        if (internal == NULL) {
            DestroyPriorityQueue(&pq);
            DestroyHuffmanTree(tree);
            free(tree);
            return NULL;
        }
        
        EnqueueNode(&pq, internal);
        tree->nodeCount++;
    }
    
    tree->root = DequeueNode(&pq);
    DestroyPriorityQueue(&pq);
    
    return tree;
}

/**
 * 从字符串构建哈夫曼树
 */
HuffmanTree* BuildHuffmanTreeFromString(const char *text) {
    if (text == NULL || strlen(text) == 0) return NULL;
    
    int count;
    CharFrequency *frequencies = CountCharacterFrequencies(text, &count);
    if (frequencies == NULL) return NULL;
    
    HuffmanTree *tree = BuildHuffmanTree(frequencies, count);
    free(frequencies);
    
    return tree;
}

// ============= 哈夫曼编码表操作 =============

/**
 * 初始化编码表
 */
bool InitHuffmanCodeTable(HuffmanCodeTable *table, int capacity) {
    if (table == NULL || capacity <= 0) return false;
    
    table->codes = (HuffmanCode *)malloc(capacity * sizeof(HuffmanCode));
    if (table->codes == NULL) return false;
    
    // 初始化编码
    for (int i = 0; i < capacity; i++) {
        table->codes[i].character = '\0';
        table->codes[i].code = NULL;
        table->codes[i].codeLength = 0;
    }
    
    table->count = 0;
    table->capacity = capacity;
    
    return true;
}

/**
 * 销毁编码表
 */
void DestroyHuffmanCodeTable(HuffmanCodeTable *table) {
    if (table == NULL) return;
    
    for (int i = 0; i < table->count; i++) {
        free(table->codes[i].code);
    }
    
    free(table->codes);
    table->codes = NULL;
    table->count = 0;
    table->capacity = 0;
}

/**
 * 添加编码到编码表
 */
bool AddHuffmanCode(HuffmanCodeTable *table, char character, const char *code) {
    if (table == NULL || code == NULL || table->count >= table->capacity) {
        return false;
    }
    
    // 检查字符是否已存在
    for (int i = 0; i < table->count; i++) {
        if (table->codes[i].character == character) {
            return false;  // 字符已存在
        }
    }
    
    // 添加新编码
    table->codes[table->count].character = character;
    table->codes[table->count].codeLength = strlen(code);
    table->codes[table->count].code = (char *)malloc((table->codes[table->count].codeLength + 1) * sizeof(char));
    
    if (table->codes[table->count].code == NULL) {
        return false;
    }
    
    strcpy(table->codes[table->count].code, code);
    table->count++;
    
    return true;
}

/**
 * 递归生成哈夫曼编码
 */
void GenerateCodesRecursive(HuffmanNode *node, char *currentCode, int depth, HuffmanCodeTable *table) {
    if (node == NULL) return;
    
    if (node->isLeaf) {
        // 叶子节点，记录编码
        currentCode[depth] = '\0';
        AddHuffmanCode(table, node->data, currentCode);
        return;
    }
    
    // 左子树：添加'0'
    if (node->left != NULL) {
        currentCode[depth] = '0';
        GenerateCodesRecursive(node->left, currentCode, depth + 1, table);
    }
    
    // 右子树：添加'1'
    if (node->right != NULL) {
        currentCode[depth] = '1';
        GenerateCodesRecursive(node->right, currentCode, depth + 1, table);
    }
}

/**
 * 生成哈夫曼编码
 */
void GenerateHuffmanCodes(HuffmanTree *tree, HuffmanCodeTable *table) {
    if (tree == NULL || tree->root == NULL || table == NULL) return;
    
    // 特殊情况：只有一个字符
    if (tree->leafCount == 1) {
        AddHuffmanCode(table, tree->root->data, "0");
        return;
    }
    
    char currentCode[256];  // 假设编码长度不超过255
    GenerateCodesRecursive(tree->root, currentCode, 0, table);
}

/**
 * 获取字符的编码
 */
const char* GetCharacterCode(HuffmanCodeTable *table, char character) {
    if (table == NULL) return NULL;
    
    for (int i = 0; i < table->count; i++) {
        if (table->codes[i].character == character) {
            return table->codes[i].code;
        }
    }
    
    return NULL;
}

// ============= 编码和解码 =============

/**
 * 编码字符串
 */
char* EncodeString(const char *text, HuffmanCodeTable *table) {
    if (text == NULL || table == NULL) return NULL;
    
    int textLen = strlen(text);
    if (textLen == 0) return NULL;
    
    // 估算编码后的长度（保守估计）
    int estimatedLen = textLen * 16;  // 假设平均编码长度为16位
    char *encoded = (char *)malloc(estimatedLen * sizeof(char));
    if (encoded == NULL) return NULL;
    
    encoded[0] = '\0';
    
    for (int i = 0; i < textLen; i++) {
        const char *code = GetCharacterCode(table, text[i]);
        if (code == NULL) {
            free(encoded);
            return NULL;  // 字符未找到编码
        }
        
        strcat(encoded, code);
    }
    
    return encoded;
}

/**
 * 解码字符串
 */
char* DecodeString(const char *encodedText, HuffmanTree *tree) {
    if (encodedText == NULL || tree == NULL || tree->root == NULL) return NULL;
    
    int encodedLen = strlen(encodedText);
    if (encodedLen == 0) return NULL;
    
    // 分配结果缓冲区
    char *decoded = (char *)malloc((encodedLen + 1) * sizeof(char));
    if (decoded == NULL) return NULL;
    
    int decodedIndex = 0;
    HuffmanNode *current = tree->root;
    
    // 特殊情况：只有一个字符的树
    if (tree->leafCount == 1) {
        for (int i = 0; i < encodedLen; i++) {
            decoded[decodedIndex++] = tree->root->data;
        }
        decoded[decodedIndex] = '\0';
        return decoded;
    }
    
    for (int i = 0; i < encodedLen; i++) {
        if (encodedText[i] == '0') {
            current = current->left;
        } else if (encodedText[i] == '1') {
            current = current->right;
        } else {
            free(decoded);
            return NULL;  // 无效字符
        }
        
        if (current == NULL) {
            free(decoded);
            return NULL;  // 无效路径
        }
        
        if (current->isLeaf) {
            decoded[decodedIndex++] = current->data;
            current = tree->root;  // 重新开始
        }
    }
    
    decoded[decodedIndex] = '\0';
    return decoded;
}

// ============= 哈夫曼树高级操作 =============

/**
 * 计算WPL（带权路径长度）
 */
int CalculateWPL(HuffmanTree *tree) {
    if (tree == NULL || tree->root == NULL) return 0;
    
    return CalculateWPLRecursive(tree->root, 0);
}

/**
 * 递归计算WPL
 */
int CalculateWPLRecursive(HuffmanNode *node, int depth) {
    if (node == NULL) return 0;
    
    if (node->isLeaf) {
        return node->weight * depth;
    }
    
    return CalculateWPLRecursive(node->left, depth + 1) + 
           CalculateWPLRecursive(node->right, depth + 1);
}

/**
 * 获取树的深度
 */
int GetHuffmanTreeDepth(HuffmanTree *tree) {
    if (tree == NULL || tree->root == NULL) return 0;
    
    return GetNodeDepthRecursive(tree->root);
}

/**
 * 递归计算节点深度
 */
int GetNodeDepthRecursive(HuffmanNode *node) {
    if (node == NULL) return 0;
    
    int leftDepth = GetNodeDepthRecursive(node->left);
    int rightDepth = GetNodeDepthRecursive(node->right);
    
    return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
}

// ============= 辅助和工具函数 =============

/**
 * 判断是否为叶子节点
 */
bool IsLeafNode(HuffmanNode *node) {
    return (node != NULL && node->isLeaf);
}

/**
 * 获取节点权重
 */
int GetNodeWeight(HuffmanNode *node) {
    return (node == NULL) ? 0 : node->weight;
}

/**
 * 获取节点数据
 */
char GetNodeData(HuffmanNode *node) {
    return (node == NULL) ? '\0' : node->data;
}

/**
 * 打印哈夫曼树结构
 */
void PrintHuffmanTreeStructure(HuffmanNode *node, int level) {
    if (node == NULL) return;
    
    // 打印右子树
    PrintHuffmanTreeStructure(node->right, level + 1);
    
    // 打印当前节点
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    
    if (node->isLeaf) {
        if (node->data == ' ') {
            printf("[空格:%d]\n", node->weight);
        } else if (node->data == '\n') {
            printf("[换行:%d]\n", node->weight);
        } else {
            printf("[%c:%d]\n", node->data, node->weight);
        }
    } else {
        printf("[内部:%d]\n", node->weight);
    }
    
    // 打印左子树
    PrintHuffmanTreeStructure(node->left, level + 1);
}

/**
 * 打印哈夫曼树
 */
void PrintHuffmanTree(HuffmanTree *tree) {
    if (tree == NULL) {
        printf("哈夫曼树为空\n");
        return;
    }
    
    printf("哈夫曼树结构 (节点总数: %d, 叶子节点: %d):\n", 
           tree->nodeCount, tree->leafCount);
    PrintHuffmanTreeStructure(tree->root, 0);
}

/**
 * 打印编码表
 */
void PrintHuffmanCodeTable(HuffmanCodeTable *table) {
    if (table == NULL) {
        printf("编码表为空\n");
        return;
    }
    
    printf("哈夫曼编码表:\n");
    printf("字符\t编码\t长度\n");
    printf("----\t----\t----\n");
    
    for (int i = 0; i < table->count; i++) {
        char ch = table->codes[i].character;
        if (ch == ' ') {
            printf("空格\t%s\t%d\n", table->codes[i].code, table->codes[i].codeLength);
        } else if (ch == '\n') {
            printf("换行\t%s\t%d\n", table->codes[i].code, table->codes[i].codeLength);
        } else if (ch == '\t') {
            printf("制表\t%s\t%d\n", table->codes[i].code, table->codes[i].codeLength);
        } else {
            printf("%c\t%s\t%d\n", ch, table->codes[i].code, table->codes[i].codeLength);
        }
    }
}

