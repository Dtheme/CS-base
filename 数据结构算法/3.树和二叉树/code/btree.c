/* btree.c - B树数据结构完整实现
 * Create by: zw.duan
 *
 * 
 * - B树是一种平衡的多路搜索树，主要用于数据库和文件系统
 * - 支持范围查询、高效插入删除、保持平衡性
 * - 时间复杂度：O(log n)，适合大数据量存储
 */

#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// ============= B树节点操作实现 =============

/**
 * 创建新的B树节点
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
BTreeNode* CreateBTreeNode(bool is_leaf) {
    BTreeNode *node = (BTreeNode*)malloc(sizeof(BTreeNode));
    if (!node) return NULL;
    
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    
    // 初始化children指针
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    
    return node;
}

/**
 * 初始化B树节点
 */
void InitBTreeNode(BTreeNode* node, bool is_leaf) {
    if (!node) return;
    
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
}

/**
 * 销毁B树节点及其子树
 * 时间复杂度：O(n)
 * 空间复杂度：O(h) - 递归栈深度
 */
void DestroyBTreeNode(BTreeNode *node) {
    if (!node) return;
    
    // 递归销毁所有子节点
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            DestroyBTreeNode(node->children[i]);
        }
    }
    
    free(node);
}

// ============= B树基本操作实现 =============

/**
 * 初始化B树
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void InitBTree(BTree* tree, int order) {
    if (!tree || order < 3) return;  // B树的阶至少为3
    
    tree->root = NULL;
    tree->order = order;
    tree->total_nodes = 0;
    tree->height = 0;
}

/**
 * 销毁B树
 * 时间复杂度：O(n)
 * 空间复杂度：O(h)
 */
void DestroyBTree(BTree *tree) {
    if (!tree) return;
    
    if (tree->root) {
        DestroyBTreeNode(tree->root);
    }
    tree->root = NULL;
    tree->total_nodes = 0;
    tree->height = 0;
}

/**
 * 判断B树是否为空
 */
bool BTreeEmpty(BTree* tree) {
    return tree == NULL || tree->root == NULL;
}

/**
 *  B树查找（ 核心算法）
 * 
 *   B树查找过程图示（查找key=25）：
 * 
 * 初始B树（3阶）：
 *           [20, 40]
 *          /    |    \
 *      [10,15] [25,30] [45,50]
 * 
 * 查找步骤：
 * 1. 从根节点开始：[20, 40]
 *    - 25 > 20，25 < 40
 *    - 进入中间子树
 * 
 * 2. 搜索节点[25, 30]：
 *    - 找到25，查找成功
 * 
 *  算法步骤：
 * 1. 从根节点开始搜索
 * 2. 在当前节点中查找关键字位置
 * 3. 如果找到，返回成功
 * 4. 如果是叶子节点且未找到，返回失败
 * 5. 否则递归搜索相应子树
 * 
 *   复杂度分析：
 * - 时间复杂度：O(log n) - 树高为O(log_m n)
 * - 空间复杂度：O(h) - 递归栈深度
 * 
 * @param tree B树指针
 * @param key 要查找的关键字
 * @return 查找结果
 */
BTreeSearchResult BTreeSearch(BTree* tree, BTreeKeyType key) {
    BTreeSearchResult result = {NULL, -1, false};
    
    if (!tree || !tree->root) {
        return result;
    }
    
    return BTreeSearchNode(tree->root, key);
}

/**
 * 在指定节点中查找关键字
 */
BTreeSearchResult BTreeSearchNode(BTreeNode* node, BTreeKeyType key) {
    BTreeSearchResult result = {NULL, -1, false};
    
    if (!node) return result;
    
    // 在当前节点中查找key的位置
    int i = BTreeFindKeyIndex(node, key);
    
    // 找到关键字
    if (i < node->num_keys && key == node->keys[i]) {
        result.node = node;
        result.index = i;
        result.found = true;
        return result;
    }
    
    // 如果是叶子节点，查找失败
    if (node->is_leaf) {
        return result;
    }
    
    // 递归搜索子树
    return BTreeSearchNode(node->children[i], key);
}

/**
 * 查找关键字在节点中的位置
 */
int BTreeFindKeyIndex(BTreeNode* node, BTreeKeyType key) {
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }
    return i;
}

/**
 * 查找子节点索引
 */
int BTreeFindChildIndex(BTreeNode* node, BTreeKeyType key) {
    return BTreeFindKeyIndex(node, key);
}

/**
 * 🌱 B树插入（ 核心算法）
 * 
 *   B树插入过程图示（插入key=35到3阶B树）：
 * 
 * 插入前：
 *           [20, 40]
 *          /    |    \
 *      [10,15] [25,30] [45,50]
 * 
 * 步骤1：查找插入位置
 * - 35 > 20, 35 < 40，进入中间子树
 * - 在[25,30]中插入35
 * 
 * 步骤2：节点满，需要分裂
 * - [25,30,35] -> 分裂为[25] | 30 | [35]
 * - 30上升到父节点
 * 
 * 插入后：
 *         [20, 30, 40]
 *        /    |    |    \
 *    [10,15] [25] [35] [45,50]
 * 
 * @param tree B树指针
 * @param key 要插入的关键字
 * @return 插入是否成功
 */
bool BTreeInsert(BTree* tree, BTreeKeyType key) {
    if (!tree) return false;
    
    // 如果树为空，创建根节点
    if (!tree->root) {
        tree->root = CreateBTreeNode(true);
        if (!tree->root) return false;
        
        tree->root->keys[0] = key;
        tree->root->num_keys = 1;
        tree->total_nodes = 1;
        tree->height = 1;
        return true;
    }
    
    // 如果根节点满了，先分裂根节点
    if (tree->root->num_keys == tree->order - 1) {
        BTreeNode *newRoot = CreateBTreeNode(false);
        if (!newRoot) return false;
        
        newRoot->children[0] = tree->root;
        BTreeSplitChildWithOrder(newRoot, 0, tree->order);
        
        tree->root = newRoot;
        tree->height++;
    }
    
    // 递归插入到非满的树中
    BTreeInsertNonFullWithOrder(tree->root, key, tree->order);
    return true;
}

/**
 * 在非满节点中插入关键字
 */
void BTreeInsertNonFull(BTreeNode* node, BTreeKeyType key) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // 在叶子节点中插入
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        // 在内部节点中找到子树
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        
        // 检查子节点是否满了 - 注意：这里需要传入树的阶数
        // 由于我们无法在这里访问树结构，我们需要重新设计这个函数
        // 暂时使用固定值，稍后会修复
        if (node->children[i]->num_keys == MAX_KEYS) {
            BTreeSplitChild(node, i);
            
            // 决定插入到哪个子树
            if (key > node->keys[i]) {
                i++;
            }
        }
        
        BTreeInsertNonFull(node->children[i], key);
    }
}

/**
 * 分裂子节点
 */
void BTreeSplitChild(BTreeNode* parent, int index) {
    BTreeNode *fullChild = parent->children[index];
    BTreeNode *newChild = CreateBTreeNode(fullChild->is_leaf);
    
    int maxKeys = fullChild->num_keys;  // 使用实际的关键字数量
    int mid = maxKeys / 2;  // 计算中间位置
    
    // 将后半部分key移动到新节点
    for (int i = 0; i < maxKeys - mid - 1; i++) {
        newChild->keys[i] = fullChild->keys[i + mid + 1];
    }
    newChild->num_keys = maxKeys - mid - 1;
    
    // 如果不是叶子节点，移动子指针
    if (!fullChild->is_leaf) {
        for (int i = 0; i < maxKeys - mid; i++) {
            newChild->children[i] = fullChild->children[i + mid + 1];
        }
    }
    
    // 更新原节点的num_keys
    fullChild->num_keys = mid;
    
    // 在父节点中为新子节点腾出空间
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    
    // 插入提升的关键字和新子节点
    parent->keys[index] = fullChild->keys[mid];
    parent->children[index + 1] = newChild;
    parent->num_keys++;
}

/**
 * 在非满节点中插入关键字（带阶数参数）
 */
void BTreeInsertNonFullWithOrder(BTreeNode* node, BTreeKeyType key, int order) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // 在叶子节点中插入
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        // 在内部节点中找到子树
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        
        // 检查子节点是否满了
        if (node->children[i]->num_keys == order - 1) {
            BTreeSplitChildWithOrder(node, i, order);
            
            // 决定插入到哪个子树
            if (key > node->keys[i]) {
                i++;
            }
        }
        
        BTreeInsertNonFullWithOrder(node->children[i], key, order);
    }
}

/**
 * 分裂子节点（带阶数参数）
 */
void BTreeSplitChildWithOrder(BTreeNode* parent, int index, int order) {
    BTreeNode *fullChild = parent->children[index];
    BTreeNode *newChild = CreateBTreeNode(fullChild->is_leaf);
    
    int maxKeys = order - 1;  // B树阶数对应的最大关键字数
    int mid = maxKeys / 2;    // 计算中间位置
    
    // 将后半部分key移动到新节点
    for (int i = 0; i < maxKeys - mid - 1; i++) {
        newChild->keys[i] = fullChild->keys[i + mid + 1];
    }
    newChild->num_keys = maxKeys - mid - 1;
    
    // 如果不是叶子节点，移动子指针
    if (!fullChild->is_leaf) {
        for (int i = 0; i < maxKeys - mid; i++) {
            newChild->children[i] = fullChild->children[i + mid + 1];
        }
    }
    
    // 更新原节点的num_keys
    fullChild->num_keys = mid;
    
    // 在父节点中为新子节点腾出空间
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    for (int i = parent->num_keys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    
    // 插入提升的关键字和新子节点
    parent->keys[index] = fullChild->keys[mid];
    parent->children[index + 1] = newChild;
    parent->num_keys++;
}

/**
 *  B树删除（ 重点）
 * 
 * B树删除比插入复杂，需要处理多种情况：
 * 1. 删除叶子节点中的关键字
 * 2. 删除内部节点中的关键字
 * 3. 维护B树的平衡性质
 * 
 * @param tree B树指针
 * @param key 要删除的关键字
 * @return 删除是否成功
 */
bool BTreeDelete(BTree* tree, BTreeKeyType key) {
    if (!tree || !tree->root) return false;
    
    BTreeDeleteFromNode(tree->root, key);
    
    // 如果根节点为空，调整树高
    if (tree->root->num_keys == 0) {
        BTreeNode *oldRoot = tree->root;
        if (tree->root->is_leaf) {
            tree->root = NULL;
            tree->height = 0;
        } else {
            tree->root = tree->root->children[0];
            tree->height--;
        }
        free(oldRoot);
        tree->total_nodes--;
    }
    
    return true;
}

/**
 * 从指定节点删除关键字
 */
void BTreeDeleteFromNode(BTreeNode* node, BTreeKeyType key) {
    int i = BTreeFindKeyIndex(node, key);
    
    // 情况1：key在当前节点中
    if (i < node->num_keys && key == node->keys[i]) {
        if (node->is_leaf) {
            // 从叶子节点删除
            BTreeShiftKeysLeft(node, i + 1);
            node->num_keys--;
        } else {
            // 从内部节点删除，需要找替换值
            BTreeKeyType predKey = BTreeFindPredecessor(node->children[i]);
            node->keys[i] = predKey;
            BTreeDeleteFromNode(node->children[i], predKey);
        }
    } else {
        // 情况2：key不在当前节点中
        if (!node->is_leaf) {
            BTreeDeleteFromNode(node->children[i], key);
        }
    }
}

/**
 * 获取子树中的前驱关键字
 */
BTreeKeyType BTreeFindPredecessor(BTreeNode* node) {
    while (!node->is_leaf) {
        node = node->children[node->num_keys];
    }
    return node->keys[node->num_keys - 1];
}

/**
 * 获取子树中的后继关键字
 */
BTreeKeyType BTreeFindSuccessor(BTreeNode* node) {
    while (!node->is_leaf) {
        node = node->children[0];
    }
    return node->keys[0];
}

/**
 * 将关键字左移
 */
void BTreeShiftKeysLeft(BTreeNode* node, int start_index) {
    for (int i = start_index; i < node->num_keys; i++) {
        node->keys[i - 1] = node->keys[i];
    }
}

/**
 * 将关键字右移
 */
void BTreeShiftKeysRight(BTreeNode* node, int start_index) {
    for (int i = node->num_keys; i > start_index; i--) {
        node->keys[i] = node->keys[i - 1];
    }
}

/**
 * 合并两个子节点
 */
void BTreeMergeNodes(BTreeNode* parent, int left_index, int right_index) {
    BTreeNode *leftChild = parent->children[left_index];
    BTreeNode *rightChild = parent->children[right_index];
    
    // 将父节点的关键字下移到左子节点
    leftChild->keys[leftChild->num_keys] = parent->keys[left_index];
    leftChild->num_keys++;
    
    // 将右子节点的所有关键字复制到左子节点
    for (int i = 0; i < rightChild->num_keys; i++) {
        leftChild->keys[leftChild->num_keys + i] = rightChild->keys[i];
    }
    
    // 如果不是叶子节点，复制子指针
    if (!leftChild->is_leaf) {
        for (int i = 0; i <= rightChild->num_keys; i++) {
            leftChild->children[leftChild->num_keys + i] = rightChild->children[i];
        }
    }
    
    leftChild->num_keys += rightChild->num_keys;
    
    // 从父节点中删除关键字和子指针
    BTreeShiftKeysLeft(parent, left_index + 1);
    for (int i = right_index; i < parent->num_keys; i++) {
        parent->children[i] = parent->children[i + 1];
    }
    
    parent->num_keys--;
    
    // 释放右子节点
    free(rightChild);
}

/**
 * 从左兄弟借关键字
 */
void BTreeBorrowFromLeft(BTreeNode* parent, int child_index) {
    BTreeNode *child = parent->children[child_index];
    BTreeNode *sibling = parent->children[child_index - 1];
    
    // 将关键字右移为新关键字腾出空间
    BTreeShiftKeysRight(child, 0);
    
    // 将父节点的关键字下移到子节点
    child->keys[0] = parent->keys[child_index - 1];
    
    // 如果不是叶子节点，移动子指针
    if (!child->is_leaf) {
        for (int i = child->num_keys + 1; i > 0; i--) {
            child->children[i] = child->children[i - 1];
        }
        child->children[0] = sibling->children[sibling->num_keys];
    }
    
    // 将兄弟节点的最大关键字上移到父节点
    parent->keys[child_index - 1] = sibling->keys[sibling->num_keys - 1];
    
    child->num_keys++;
    sibling->num_keys--;
}

/**
 * 从右兄弟借关键字
 */
void BTreeBorrowFromRight(BTreeNode* parent, int child_index) {
    BTreeNode *child = parent->children[child_index];
    BTreeNode *sibling = parent->children[child_index + 1];
    
    // 将父节点的关键字下移到子节点
    child->keys[child->num_keys] = parent->keys[child_index];
    
    // 如果不是叶子节点，移动子指针
    if (!child->is_leaf) {
        child->children[child->num_keys + 1] = sibling->children[0];
    }
    
    // 将兄弟节点的最小关键字上移到父节点
    parent->keys[child_index] = sibling->keys[0];
    
    // 在兄弟节点中删除第一个关键字
    BTreeShiftKeysLeft(sibling, 1);
    
    // 如果不是叶子节点，移动子指针
    if (!sibling->is_leaf) {
        for (int i = 0; i < sibling->num_keys; i++) {
            sibling->children[i] = sibling->children[i + 1];
        }
    }
    
    child->num_keys++;
    sibling->num_keys--;
}

// ============= B树遍历和显示函数 =============

/**
 * B树中序遍历
 * 时间复杂度：O(n)
 * 空间复杂度：O(h)
 */
void BTreeInOrder(BTree* tree) {
    if (!tree || !tree->root) {
        printf("B树为空\n");
        return;
    }
    
    printf("B树中序遍历：");
    BTreeInOrderNode(tree->root);
    printf("\n");
}

void BTreeInOrderNode(BTreeNode* node) {
    if (!node) return;
    
    int i;
    for (i = 0; i < node->num_keys; i++) {
        if (!node->is_leaf) {
            BTreeInOrderNode(node->children[i]);
        }
        printf("%d ", node->keys[i]);
    }
    
    if (!node->is_leaf) {
        BTreeInOrderNode(node->children[i]);
    }
}

/**
 * B树前序遍历
 */
void BTreePreOrder(BTree* tree) {
    if (!tree || !tree->root) {
        printf("B树为空\n");
        return;
    }
    
    printf("B树前序遍历：");
    BTreePreOrderNode(tree->root);
    printf("\n");
}

void BTreePreOrderNode(BTreeNode* node) {
    if (!node) return;
    
    // 访问当前节点的所有关键字
    for (int i = 0; i < node->num_keys; i++) {
        printf("%d ", node->keys[i]);
    }
    
    // 递归访问所有子节点
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            BTreePreOrderNode(node->children[i]);
        }
    }
}

/**
 * B树层次遍历
 */
void BTreeLevelOrder(BTree* tree) {
    if (!tree || !tree->root) {
        printf("B树为空\n");
        return;
    }
    
    printf("B树层次遍历：\n");
    
    // 使用队列进行层次遍历
    BTreeNode *queue[1000];
    int front = 0, rear = 0;
    
    queue[rear++] = tree->root;
    int level = 1;
    
    while (front < rear) {
        int levelSize = rear - front;
        printf("第%d层：", level++);
        
        for (int i = 0; i < levelSize; i++) {
            BTreeNode *node = queue[front++];
            
            printf("[");
            for (int j = 0; j < node->num_keys; j++) {
                printf("%d", node->keys[j]);
                if (j < node->num_keys - 1) printf(",");
            }
            printf("] ");
            
            // 将子节点加入队列
            if (!node->is_leaf) {
                for (int j = 0; j <= node->num_keys; j++) {
                    queue[rear++] = node->children[j];
                }
            }
        }
        printf("\n");
    }
}

/**
 * 打印B树结构
 */
void PrintBTree(BTree* tree) {
    if (!tree || !tree->root) {
        printf("B树为空\n");
        return;
    }
    
    printf("B树结构（阶数：%d，节点数：%d，高度：%d）：\n", 
           tree->order, tree->total_nodes, tree->height);
    
    BTreeLevelOrder(tree);
}

void PrintBTreeNode(BTreeNode* node, int level) {
    if (!node) return;
    
    // 打印缩进
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    // 打印节点内容
    printf("[");
    for (int i = 0; i < node->num_keys; i++) {
        printf("%d", node->keys[i]);
        if (i < node->num_keys - 1) printf(",");
    }
    printf("]\n");
    
    // 递归打印子节点
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            PrintBTreeNode(node->children[i], level + 1);
        }
    }
}

void PrintBTreeStructure(BTree* tree) {
    if (!tree || !tree->root) {
        printf("B树为空\n");
        return;
    }
    
    printf("B树结构：\n");
    PrintBTreeNode(tree->root, 0);
}

// ============= B树高级操作实现 =============

/**
 * B树范围查询
 */
void BTreeRangeQuery(BTree* tree, BTreeKeyType low, BTreeKeyType high, 
                     BTreeKeyType* result, int* count) {
    *count = 0;
    if (!tree || !tree->root) return;
    
    BTreeRangeQueryHelper(tree->root, low, high, result, count);
}

void BTreeRangeQueryHelper(BTreeNode* node, BTreeKeyType low, BTreeKeyType high, 
                          BTreeKeyType* result, int* count) {
    if (!node) return;
    
    int i;
    for (i = 0; i < node->num_keys; i++) {
        // 递归搜索左子树
        if (!node->is_leaf && node->keys[i] > low) {
            BTreeRangeQueryHelper(node->children[i], low, high, result, count);
        }
        
        // 如果当前关键字在范围内，加入结果
        if (node->keys[i] >= low && node->keys[i] <= high) {
            result[(*count)++] = node->keys[i];
        }
        
        // 如果当前关键字超过范围，停止搜索
        if (node->keys[i] > high) {
            return;
        }
    }
    
    // 搜索最右子树
    if (!node->is_leaf) {
        BTreeRangeQueryHelper(node->children[i], low, high, result, count);
    }
}

/**
 * 统计范围内关键字数量
 */
int BTreeCountRange(BTree* tree, BTreeKeyType low, BTreeKeyType high) {
    BTreeKeyType result[1000];
    int count;
    BTreeRangeQuery(tree, low, high, result, &count);
    return count;
}

/**
 * 查找第k小的元素
 */
BTreeKeyType BTreeKthSmallest(BTree* tree, int k) {
    if (!tree || !tree->root || k <= 0) {
        return -1;
    }
    
    int current = 0;
    return BTreeKthHelper(tree->root, k, &current);
}

BTreeKeyType BTreeKthHelper(BTreeNode* node, int k, int* current) {
    if (!node) return -1;
    
    int i;
    for (i = 0; i < node->num_keys; i++) {
        // 先递归搜索左子树
        if (!node->is_leaf) {
            BTreeKeyType result = BTreeKthHelper(node->children[i], k, current);
            if (result != -1) return result;
        }
        
        // 访问当前关键字
        (*current)++;
        if (*current == k) {
            return node->keys[i];
        }
    }
    
    // 搜索最右子树
    if (!node->is_leaf) {
        return BTreeKthHelper(node->children[i], k, current);
    }
    
    return -1;
}

/**
 * 验证B树的合法性
 */
bool ValidateBTree(BTree* tree) {
    if (!tree) return false;
    if (!tree->root) return true;  // 空树是合法的
    
    return ValidateBTreeNode(tree->root, INT_MIN, INT_MAX, tree->height, 1);
}

/**
 * 验证B树节点的合法性（递归函数）
 */
bool ValidateBTreeNode(BTreeNode* node, BTreeKeyType min_key, BTreeKeyType max_key, 
                      int expected_height, int current_height) {
    if (!node) return true;
    
    // 检查关键字数量
    if (node->num_keys < 0 || node->num_keys >= MAX_KEYS) {
        return false;
    }
    
    // 检查关键字是否有序并在范围内
    for (int i = 0; i < node->num_keys; i++) {
        if (node->keys[i] <= min_key || node->keys[i] >= max_key) {
            return false;
        }
        if (i > 0 && node->keys[i] <= node->keys[i-1]) {
            return false;
        }
    }
    
    // 如果是叶子节点，检查高度
    if (node->is_leaf) {
        return current_height == expected_height;
    }
    
    // 递归验证子树
    for (int i = 0; i <= node->num_keys; i++) {
        BTreeKeyType childMinKey = (i == 0) ? min_key : node->keys[i-1];
        BTreeKeyType childMaxKey = (i == node->num_keys) ? max_key : node->keys[i];
        
        if (!ValidateBTreeNode(node->children[i], childMinKey, childMaxKey, 
                              expected_height, current_height + 1)) {
            return false;
        }
    }
    
    return true;
}

/**
 * 获取B树高度
 */
int BTreeHeight(BTree* tree) {
    return tree ? tree->height : 0;
}

/**
 * 获取B树关键字总数
 */
int BTreeKeyCount(BTree* tree) {
    if (!tree || !tree->root) return 0;
    return BTreeKeyCountNode(tree->root);
}

int BTreeKeyCountNode(BTreeNode* node) {
    if (!node) return 0;
    
    int count = node->num_keys;
    
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            count += BTreeKeyCountNode(node->children[i]);
        }
    }
    
    return count;
}

/**
 * 获取叶子节点数量
 */
int BTreeLeafCount(BTree* tree) {
    if (!tree || !tree->root) return 0;
    return BTreeLeafCountNode(tree->root);
}

int BTreeLeafCountNode(BTreeNode* node) {
    if (!node) return 0;
    
    if (node->is_leaf) {
        return 1;
    }
    
    int count = 0;
    for (int i = 0; i <= node->num_keys; i++) {
        count += BTreeLeafCountNode(node->children[i]);
    }
    
    return count;
} 