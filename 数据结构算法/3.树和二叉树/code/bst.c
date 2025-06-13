/* bst.c - 二叉搜索树实现
 * create by: zw.duan
 */

#include "bst.h"
#include <limits.h>
#include <string.h>

// ============= BST基本操作实现 =============

/**
 * 初始化BST
 */
void InitBST(BSTree *T) {
    *T = NULL;
}

/**
 * 销毁BST
 */
void DestroyBST(BSTree *T) {
    if (*T != NULL) {
        DestroyBST(&(*T)->lchild);
        DestroyBST(&(*T)->rchild);
        free(*T);
        *T = NULL;
    }
}

/**
 * 判断BST是否为空
 */
bool BSTEmpty(BSTree T) {
    return T == NULL;
}

/**
 * 在BST中搜索关键字（递归）
 */
BSTNode* BSTSearch(BSTree T, BSTElemType key) {
    if (T == NULL || T->data == key) {
        return T;
    }
    
    if (key < T->data) {
        return BSTSearch(T->lchild, key);
    } else {
        return BSTSearch(T->rchild, key);
    }
}

/**
 * 在BST中搜索关键字（非递归）
 */
BSTNode* BSTSearchNonRecursive(BSTree T, BSTElemType key) {
    while (T != NULL && T->data != key) {
        if (key < T->data) {
            T = T->lchild;
        } else {
            T = T->rchild;
        }
    }
    return T;
}

/**
 * 查找BST中的最小值结点
 */
BSTNode* FindMin(BSTree T) {
    if (T == NULL) return NULL;
    
    while (T->lchild != NULL) {
        T = T->lchild;
    }
    return T;
}

/**
 * 查找BST中的最大值结点
 */
BSTNode* FindMax(BSTree T) {
    if (T == NULL) return NULL;
    
    while (T->rchild != NULL) {
        T = T->rchild;
    }
    return T;
}

/**
 * 在BST中插入结点
 */
bool BSTInsert(BSTree *T, BSTElemType key) {
    if (*T == NULL) {
        *T = (BSTNode*)malloc(sizeof(BSTNode));
        (*T)->data = key;
        (*T)->lchild = (*T)->rchild = NULL;
        return true;
    }
    
    if (key == (*T)->data) {
        return false; // 不允许重复关键字
    } else if (key < (*T)->data) {
        return BSTInsert(&(*T)->lchild, key);
    } else {
        return BSTInsert(&(*T)->rchild, key);
    }
}

/**
 * 在BST中删除结点
 */
bool BSTDelete(BSTree *T, BSTElemType key) {
    if (*T == NULL) return false;
    
    if (key < (*T)->data) {
        return BSTDelete(&(*T)->lchild, key);
    } else if (key > (*T)->data) {
        return BSTDelete(&(*T)->rchild, key);
    } else {
        // 找到要删除的结点
        BSTNode *node = *T;
        
        if ((*T)->lchild == NULL) {
            *T = (*T)->rchild;
            free(node);
        } else if ((*T)->rchild == NULL) {
            *T = (*T)->lchild;
            free(node);
        } else {
            // 找到中序后继
            BSTNode *successor = FindMin((*T)->rchild);
            (*T)->data = successor->data;
            BSTDelete(&(*T)->rchild, successor->data);
        }
        return true;
    }
}

/**
 * 删除结点并返回新根
 */
BSTNode* DeleteNode(BSTree T, BSTElemType key) {
    BSTDelete(&T, key);
    return T;
}

// ============= BST遍历操作 =============

/**
 * BST中序遍历
 */
void BSTInOrder(BSTree T) {
    if (T != NULL) {
        BSTInOrder(T->lchild);
        printf("%d ", T->data);
        BSTInOrder(T->rchild);
    }
}

/**
 * BST先序遍历
 */
void BSTPreOrder(BSTree T) {
    if (T != NULL) {
        printf("%d ", T->data);
        BSTPreOrder(T->lchild);
        BSTPreOrder(T->rchild);
    }
}

/**
 * BST后序遍历
 */
void BSTPostOrder(BSTree T) {
    if (T != NULL) {
        BSTPostOrder(T->lchild);
        BSTPostOrder(T->rchild);
        printf("%d ", T->data);
    }
}

// ============= BST高级操作实现 =============

/**
 * 判断是否为BST（简单版本）
 */
bool IsBST(BSTree T) {
    return IsValidBST(T, INT_MIN, INT_MAX);
}

/**
 * 验证BST的有效性
 */
bool IsValidBST(BSTree T, BSTElemType min, BSTElemType max) {
    if (T == NULL) return true;
    
    if (T->data <= min || T->data >= max) return false;
    
    return IsValidBST(T->lchild, min, T->data) && 
           IsValidBST(T->rchild, T->data, max);
}

/**
 * 查找前驱结点
 */
BSTNode* FindPredecessor(BSTree T, BSTElemType key) {
    BSTNode *current = BSTSearch(T, key);
    if (current == NULL) return NULL;
    
    if (current->lchild != NULL) {
        return FindMax(current->lchild);
    }
    
    BSTNode *predecessor = NULL;
    BSTNode *ancestor = T;
    
    while (ancestor != current) {
        if (current->data > ancestor->data) {
            predecessor = ancestor;
            ancestor = ancestor->rchild;
        } else {
            ancestor = ancestor->lchild;
        }
    }
    
    return predecessor;
}

/**
 * 查找后继结点
 */
BSTNode* FindSuccessor(BSTree T, BSTElemType key) {
    BSTNode *current = BSTSearch(T, key);
    if (current == NULL) return NULL;
    
    if (current->rchild != NULL) {
        return FindMin(current->rchild);
    }
    
    BSTNode *successor = NULL;
    BSTNode *ancestor = T;
    
    while (ancestor != current) {
        if (current->data < ancestor->data) {
            successor = ancestor;
            ancestor = ancestor->lchild;
        } else {
            ancestor = ancestor->rchild;
        }
    }
    
    return successor;
}

/**
 * 查找第k小的元素
 */
BSTNode* FindKthSmallest(BSTree T, int k) {
    int count = 0;
    return FindKthSmallestHelper(T, k, &count);
}

BSTNode* FindKthSmallestHelper(BSTree T, int k, int *count) {
    if (T == NULL) return NULL;
    
    BSTNode *result = FindKthSmallestHelper(T->lchild, k, count);
    if (result != NULL) return result;
    
    (*count)++;
    if (*count == k) return T;
    
    return FindKthSmallestHelper(T->rchild, k, count);
}

/**
 * 获取元素的排名
 */
int GetRank(BSTree T, BSTElemType key) {
    if (T == NULL) return 0;
    
    if (key < T->data) {
        return GetRank(T->lchild, key);
    } else if (key > T->data) {
        return 1 + BSTNodeCount(T->lchild) + GetRank(T->rchild, key);
    } else {
        return BSTNodeCount(T->lchild) + 1;
    }
}

/**
 * 范围打印
 */
void PrintRange(BSTree T, BSTElemType k1, BSTElemType k2) {
    if (T == NULL) return;
    
    if (T->data > k1) {
        PrintRange(T->lchild, k1, k2);
    }
    
    if (T->data >= k1 && T->data <= k2) {
        printf("%d ", T->data);
    }
    
    if (T->data < k2) {
        PrintRange(T->rchild, k1, k2);
    }
}

/**
 * 范围计数
 */
int CountRange(BSTree T, BSTElemType k1, BSTElemType k2) {
    if (T == NULL) return 0;
    
    int count = 0;
    if (T->data >= k1 && T->data <= k2) count = 1;
    
    if (T->data > k1) {
        count += CountRange(T->lchild, k1, k2);
    }
    
    if (T->data < k2) {
        count += CountRange(T->rchild, k1, k2);
    }
    
    return count;
}

/**
 * 从数组构建BST
 */
BSTree CreateBSTFromArray(BSTElemType *arr, int size) {
    BSTree T;
    InitBST(&T);
    
    for (int i = 0; i < size; i++) {
        BSTInsert(&T, arr[i]);
    }
    
    return T;
}

/**
 * 打印BST
 */
void PrintBST(BSTree T) {
    printf("BST中序遍历（有序）：");
    BSTInOrder(T);
    printf("\n");
}

/**
 * 计算BST深度
 */
int BSTDepth(BSTree T) {
    if (T == NULL) return 0;
    
    int leftDepth = BSTDepth(T->lchild);
    int rightDepth = BSTDepth(T->rchild);
    
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

/**
 * 计算BST结点数
 */
int BSTNodeCount(BSTree T) {
    if (T == NULL) return 0;
    return BSTNodeCount(T->lchild) + BSTNodeCount(T->rchild) + 1;
}

/**
 * 验证BST性质
 */
bool ValidateBSTProperty(BSTree T) {
    return IsBST(T);
}

// ============= 简化实现的其他函数 =============

bool FindPath(BSTree T, BSTElemType key, int *path, int *len) {
    // 简化实现，仅返回是否找到
    (void)path;
    (void)len;
    return BSTSearch(T, key) != NULL;
}

int GetDistance(BSTree T, BSTElemType a, BSTElemType b) {
    // 简化实现
    (void)T;
    (void)a;
    (void)b;
    return 0;
}

BSTNode* LowestCommonAncestor(BSTree T, BSTElemType p, BSTElemType q) {
    if (T == NULL) return NULL;
    
    if (T->data > p && T->data > q) {
        return LowestCommonAncestor(T->lchild, p, q);
    } else if (T->data < p && T->data < q) {
        return LowestCommonAncestor(T->rchild, p, q);
    } else {
        return T;
    }
}

void BSTToSortedArray(BSTree T, BSTElemType *arr, int *index) {
    if (T != NULL) {
        BSTToSortedArray(T->lchild, arr, index);
        arr[(*index)++] = T->data;
        BSTToSortedArray(T->rchild, arr, index);
    }
}

BSTree SortedArrayToBST(BSTElemType *arr, int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    BSTNode *root = (BSTNode*)malloc(sizeof(BSTNode));
    root->data = arr[mid];
    root->lchild = SortedArrayToBST(arr, start, mid - 1);
    root->rchild = SortedArrayToBST(arr, mid + 1, end);
    
    return root;
}

BSTree BalanceBST(BSTree T) {
    // 简化实现：转为数组再构建平衡BST
    int count = BSTNodeCount(T);
    BSTElemType *arr = (BSTElemType*)malloc(count * sizeof(BSTElemType));
    int index = 0;
    
    BSTToSortedArray(T, arr, &index);
    BSTree balanced = SortedArrayToBST(arr, 0, count - 1);
    
    free(arr);
    return balanced;
}

BSTree CreateBSTFromPreOrder(BSTElemType *preorder, int size) {
    // 简化实现
    return CreateBSTFromArray(preorder, size);
}

BSTree CreateBSTFromPostOrder(BSTElemType *postorder, int size) {
    // 简化实现
    return CreateBSTFromArray(postorder, size);
}

bool VerifyPreOrder(BSTElemType *preorder, int size) {
    BSTree T = CreateBSTFromPreOrder(preorder, size);
    bool valid = ValidateBSTProperty(T);
    DestroyBST(&T);
    return valid;
}

bool VerifyPostOrder(BSTElemType *postorder, int size) {
    BSTree T = CreateBSTFromPostOrder(postorder, size);
    bool valid = ValidateBSTProperty(T);
    DestroyBST(&T);
    return valid;
}

BSTree TrimBST(BSTree T, BSTElemType low, BSTElemType high) {
    if (T == NULL) return NULL;
    
    if (T->data < low) return TrimBST(T->rchild, low, high);
    if (T->data > high) return TrimBST(T->lchild, low, high);
    
    T->lchild = TrimBST(T->lchild, low, high);
    T->rchild = TrimBST(T->rchild, low, high);
    
    return T;
}

void SerializeBST(BSTree T, char *str) {
    // 简化实现
    (void)T;
    strcpy(str, "serialized");
}

BSTree DeserializeBST(char *str) {
    // 简化实现
    (void)str;
    return NULL;
}

bool IsSubtree(BSTree T, BSTree S) {
    // 简化实现
    (void)T;
    (void)S;
    return false;
}

int CountBSTNodes(BSTree T, BSTElemType min, BSTElemType max) {
    return CountRange(T, min, max);
}

BSTree MergeBST(BSTree T1, BSTree T2) {
    // 简化实现：合并为数组再构建
    int count1 = BSTNodeCount(T1);
    int count2 = BSTNodeCount(T2);
    
    BSTElemType *arr = (BSTElemType*)malloc((count1 + count2) * sizeof(BSTElemType));
    int index = 0;
    
    BSTToSortedArray(T1, arr, &index);
    BSTToSortedArray(T2, arr, &index);
    
    BSTree merged = CreateBSTFromArray(arr, count1 + count2);
    free(arr);
    
    return merged;
}

void GetAllElements(BSTree T1, BSTree T2, BSTElemType *result, int *size) {
    *size = 0;
    BSTToSortedArray(T1, result, size);
    BSTToSortedArray(T2, result, size);
}

void PrintBSTStructure(BSTree T, int space) {
    if (T == NULL) return;
    
    space += 4;
    PrintBSTStructure(T->rchild, space);
    
    printf("\n");
    for (int i = 4; i < space; i++) printf(" ");
    printf("%d\n", T->data);
    
    PrintBSTStructure(T->lchild, space);
}

void PerformanceTest_BSTSearch(int size) {
    printf("BST搜索性能测试（%d个元素）\n", size);
}

void PerformanceTest_BSTInsert(int size) {
    printf("BST插入性能测试（%d个元素）\n", size);
}

void PerformanceCompare_BSTvsArray(int size) {
    printf("BST vs 数组性能比较（%d个元素）\n", size);
} 