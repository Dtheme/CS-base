/* bintree.c - 二叉树实现
 * create by: zw.duan
 */

#include "bintree.h"
#include <limits.h>
#include <time.h>

// 队列结构（用于层次遍历）
typedef struct QueueNode {
    BiTNode *data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

// 栈结构（用于非递归遍历）
typedef struct StackNode {
    BiTNode *data;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
} Stack;

// ============= 辅助数据结构操作 =============

// 队列操作
void InitQueue(Queue *Q) {
    Q->front = Q->rear = NULL;
}

bool QueueEmpty(Queue Q) {
    return Q.front == NULL;
}

void EnQueue(Queue *Q, BiTNode *e) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = e;
    newNode->next = NULL;
    if (Q->rear == NULL) {
        Q->front = Q->rear = newNode;
    } else {
        Q->rear->next = newNode;
        Q->rear = newNode;
    }
}

bool DeQueue(Queue *Q, BiTNode **e) {
    if (QueueEmpty(*Q)) return false;
    QueueNode *temp = Q->front;
    *e = temp->data;
    Q->front = Q->front->next;
    if (Q->front == NULL) Q->rear = NULL;
    free(temp);
    return true;
}

// 栈操作
void InitStack(Stack *S) {
    S->top = NULL;
}

bool StackEmpty(Stack S) {
    return S.top == NULL;
}

void Push(Stack *S, BiTNode *e) {
    StackNode *newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = e;
    newNode->next = S->top;
    S->top = newNode;
}

bool Pop(Stack *S, BiTNode **e) {
    if (StackEmpty(*S)) return false;
    StackNode *temp = S->top;
    *e = temp->data;
    S->top = S->top->next;
    free(temp);
    return true;
}

bool GetTop(Stack S, BiTNode **e) {
    if (StackEmpty(S)) return false;
    *e = S.top->data;
    return true;
}

// ============= 二叉树基本操作实现 =============

/**
 * 初始化二叉树
 */
void InitBiTree(BiTree *T) {
    *T = NULL;
}

/**
 * 销毁二叉树（递归释放所有节点）
 */
void DestroyBiTree(BiTree *T) {
    if (*T != NULL) {
        DestroyBiTree(&(*T)->lchild);
        DestroyBiTree(&(*T)->rchild);
        free(*T);
        *T = NULL;
    }
}

/**
 * 判断二叉树是否为空
 */
bool BiTreeEmpty(BiTree T) {
    return T == NULL;
}

/**
 * 计算二叉树深度
 */
int BiTreeDepth(BiTree T) {
    if (T == NULL) return 0;
    int leftDepth = BiTreeDepth(T->lchild);
    int rightDepth = BiTreeDepth(T->rchild);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

/**
 * 计算二叉树节点总数
 */
int BiTreeNodeCount(BiTree T) {
    if (T == NULL) return 0;
    return BiTreeNodeCount(T->lchild) + BiTreeNodeCount(T->rchild) + 1;
}

/**
 * 计算二叉树叶子节点数
 */
int BiTreeLeafCount(BiTree T) {
    if (T == NULL) return 0;
    if (T->lchild == NULL && T->rchild == NULL) return 1;
    return BiTreeLeafCount(T->lchild) + BiTreeLeafCount(T->rchild);
}

/**
 * 根据先序遍历序列创建二叉树
 * str: 指向字符串的指针的指针，'#'表示空节点
 */
BiTNode* CreateBiTreeByPreOrder(char **str) {
    if (**str == '#' || **str == '\0') {
        (*str)++;
        return NULL;
    }
    
    BiTNode *T = (BiTNode*)malloc(sizeof(BiTNode));
    T->data = **str;
    (*str)++;
    T->lchild = CreateBiTreeByPreOrder(str);
    T->rchild = CreateBiTreeByPreOrder(str);
    
    return T;
}

/**
 * 根据数组创建二叉树（完全二叉树的顺序存储）
 */
BiTNode* CreateBiTreeByArray(char *arr, int index, int size) {
    if (index >= size || arr[index] == '#') return NULL;
    
    BiTNode *T = (BiTNode*)malloc(sizeof(BiTNode));
    T->data = arr[index];
    T->lchild = CreateBiTreeByArray(arr, 2 * index + 1, size);
    T->rchild = CreateBiTreeByArray(arr, 2 * index + 2, size);
    
    return T;
}

// ============= 二叉树遍历算法实现 =============

/**
 * 先序遍历（递归）
 */
void PreOrder(BiTree T) {
    if (T != NULL) {
        printf("%c ", T->data);
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }
}

/**
 * 中序遍历（递归）
 */
void InOrder(BiTree T) {
    if (T != NULL) {
        InOrder(T->lchild);
        printf("%c ", T->data);
        InOrder(T->rchild);
    }
}

/**
 * 后序遍历（递归）
 */
void PostOrder(BiTree T) {
    if (T != NULL) {
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        printf("%c ", T->data);
    }
}

/**
 * 层次遍历
 */
void LevelOrder(BiTree T) {
    if (T == NULL) return;
    
    Queue Q;
    InitQueue(&Q);
    EnQueue(&Q, T);
    
    BiTNode *current;
    while (!QueueEmpty(Q)) {
        DeQueue(&Q, &current);
        printf("%c ", current->data);
        
        if (current->lchild != NULL) 
            EnQueue(&Q, current->lchild);
        if (current->rchild != NULL) 
            EnQueue(&Q, current->rchild);
    }
}

/**
 * 先序遍历（非递归）
 */
void PreOrderNonRecursive(BiTree T) {
    if (T == NULL) return;
    
    Stack S;
    InitStack(&S);
    BiTNode *current = T;
    
    while (current != NULL || !StackEmpty(S)) {
        while (current != NULL) {
            printf("%c ", current->data);
            Push(&S, current);
            current = current->lchild;
        }
        Pop(&S, &current);
        current = current->rchild;
    }
}

/**
 * 中序遍历（非递归）
 */
void InOrderNonRecursive(BiTree T) {
    if (T == NULL) return;
    
    Stack S;
    InitStack(&S);
    BiTNode *current = T;
    
    while (current != NULL || !StackEmpty(S)) {
        while (current != NULL) {
            Push(&S, current);
            current = current->lchild;
        }
        Pop(&S, &current);
        printf("%c ", current->data);
        current = current->rchild;
    }
}

/**
 * 后序遍历（非递归）
 */
void PostOrderNonRecursive(BiTree T) {
    if (T == NULL) return;
    
    Stack S;
    InitStack(&S);
    BiTNode *current = T;
    BiTNode *lastVisited = NULL;
    
    while (current != NULL || !StackEmpty(S)) {
        if (current != NULL) {
            Push(&S, current);
            current = current->lchild;
        } else {
            GetTop(S, &current);
            if (current->rchild == NULL || current->rchild == lastVisited) {
                printf("%c ", current->data);
                Pop(&S, &current);
                lastVisited = current;
                current = NULL;
            } else {
                current = current->rchild;
            }
        }
    }
}

// ============= 遍历的扩展应用 =============

void PreOrderTraverse(BiTree T, void (*visit)(ElemType)) {
    if (T != NULL) {
        visit(T->data);
        PreOrderTraverse(T->lchild, visit);
        PreOrderTraverse(T->rchild, visit);
    }
}

void InOrderTraverse(BiTree T, void (*visit)(ElemType)) {
    if (T != NULL) {
        InOrderTraverse(T->lchild, visit);
        visit(T->data);
        InOrderTraverse(T->rchild, visit);
    }
}

void PostOrderTraverse(BiTree T, void (*visit)(ElemType)) {
    if (T != NULL) {
        PostOrderTraverse(T->lchild, visit);
        PostOrderTraverse(T->rchild, visit);
        visit(T->data);
    }
}

// ============= 二叉树查找操作 =============

/**
 * 在二叉树中查找值为key的节点
 */
BiTNode* FindNode(BiTree T, ElemType key) {
    if (T == NULL) return NULL;
    if (T->data == key) return T;
    
    BiTNode *left = FindNode(T->lchild, key);
    if (left != NULL) return left;
    
    return FindNode(T->rchild, key);
}

/**
 * 查找指定节点的父节点
 */
BiTNode* FindParent(BiTree T, BiTNode *node) {
    if (T == NULL || node == NULL || T == node) return NULL;
    
    if (T->lchild == node || T->rchild == node) return T;
    
    BiTNode *left = FindParent(T->lchild, node);
    if (left != NULL) return left;
    
    return FindParent(T->rchild, node);
}

/**
 * 查找从根到指定值的路径
 */
bool FindPathHelper(BiTree T, ElemType key, char *path, int *pos) {
    if (T == NULL) return false;
    
    if (T->data == key) {
        path[*pos] = T->data;
        path[*pos + 1] = '\0';
        return true;
    }
    
    path[(*pos)++] = T->data;
    
    if (FindPathHelper(T->lchild, key, path, pos) || 
        FindPathHelper(T->rchild, key, path, pos)) {
        return true;
    }
    
    (*pos)--; // 回溯
    return false;
}

bool FindPath(BiTree T, ElemType key, char *path) {
    int pos = 0;
    return FindPathHelper(T, key, path, &pos);
}

// ============= 二叉树修改操作 =============

/**
 * 为指定节点插入左节点
 */
bool InsertAsLeftChild(BiTNode *parent, ElemType e) {
    if (parent == NULL) return false;
    
    BiTNode *newNode = (BiTNode*)malloc(sizeof(BiTNode));
    newNode->data = e;
    newNode->lchild = parent->lchild;
    newNode->rchild = NULL;
    parent->lchild = newNode;
    
    return true;
}

/**
 * 为指定节点插入右节点
 */
bool InsertAsRightChild(BiTNode *parent, ElemType e) {
    if (parent == NULL) return false;
    
    BiTNode *newNode = (BiTNode*)malloc(sizeof(BiTNode));
    newNode->data = e;
    newNode->lchild = NULL;
    newNode->rchild = parent->rchild;
    parent->rchild = newNode;
    
    return true;
}

/**
 * 删除以指定节点为根的子树
 */
bool DeleteSubTree(BiTree *T, BiTNode *node) {
    if (*T == NULL || node == NULL) return false;
    
    if (*T == node) {
        DestroyBiTree(T);
        return true;
    }
    
    BiTNode *parent = FindParent(*T, node);
    if (parent == NULL) return false;
    
    if (parent->lchild == node) {
        DestroyBiTree(&parent->lchild);
    } else if (parent->rchild == node) {
        DestroyBiTree(&parent->rchild);
    }
    
    return true;
}

// ============= 顺序二叉树操作 =============

/**
 * 初始化顺序二叉树
 */
void InitSqBiTree(SqBiTree *T) {
    for (int i = 0; i < MAXSIZE; i++) {
        T->isEmpty[i] = true;
    }
    T->size = 0;
}

/**
 * 在顺序二叉树的指定位置插入元素
 */
bool SqBiTreeInsert(SqBiTree *T, int index, ElemType e) {
    if (index < 0 || index >= MAXSIZE) return false;
    
    if (T->isEmpty[index]) {
        T->size++;
    }
    T->data[index] = e;
    T->isEmpty[index] = false;
    
    return true;
}

/**
 * 删除顺序二叉树指定位置的元素
 */
bool SqBiTreeDelete(SqBiTree *T, int index) {
    if (index < 0 || index >= MAXSIZE || T->isEmpty[index]) return false;
    
    T->isEmpty[index] = true;
    T->size--;
    
    return true;
}

/**
 * 获取顺序二叉树指定位置的元素
 */
ElemType SqBiTreeGetNode(SqBiTree T, int index) {
    if (index < 0 || index >= MAXSIZE || T.isEmpty[index]) {
        return '\0'; // 返回空字符表示无效
    }
    return T.data[index];
}

/**
 * 计算父节点下标
 */
int SqBiTreeParent(int index) {
    if (index <= 0) return -1;
    return (index - 1) / 2;
}

/**
 * 计算左节点下标
 */
int SqBiTreeLeftChild(int index) {
    return 2 * index + 1;
}

/**
 * 计算右节点下标
 */
int SqBiTreeRightChild(int index) {
    return 2 * index + 2;
}

/**
 * 打印顺序二叉树
 */
void PrintSqBiTree(SqBiTree T) {
    printf("顺序二叉树：");
    for (int i = 0; i < MAXSIZE; i++) {
        if (!T.isEmpty[i]) {
            printf("[%d]:%c ", i, T.data[i]);
        }
    }
    printf("\n");
}

// ============= 实用工具函数 =============

/**
 * 打印二叉树（简单格式）
 */
void PrintBiTree(BiTree T) {
    printf("先序遍历：");
    PreOrder(T);
    printf("\n");
    
    printf("中序遍历：");
    InOrder(T);
    printf("\n");
    
    printf("后序遍历：");
    PostOrder(T);
    printf("\n");
    
    printf("层次遍历：");
    LevelOrder(T);
    printf("\n");
}

/**
 * 以树状结构打印二叉树
 */
void PrintBiTreeStructure(BiTree T, int space) {
    if (T == NULL) return;
    
    space += 4;
    PrintBiTreeStructure(T->rchild, space);
    
    printf("\n");
    for (int i = 4; i < space; i++) printf(" ");
    printf("%c\n", T->data);
    
    PrintBiTreeStructure(T->lchild, space);
}

/**
 * 判断字符是否为有效的二叉树字符
 */
bool IsValidBiTreeChar(char c) {
    return c != '#' && c != '\0';
}

/**
 * 访问函数：打印元素
 */
void VisitPrint(ElemType e) {
    printf("%c ", e);
}

// 继续实现其他算法...（由于篇幅限制，这里只实现了基本部分）

// ============= 二叉树算法题（ 重点）=============

/**
 * 计算二叉树深度（与BiTreeDepth相同，为了保持接口一致性）
 */
int CalculateDepth(BiTree T) {
    return BiTreeDepth(T);
}

/**
 * 计算二叉树的宽度（最大层的节点数）
 */
int CalculateWidth(BiTree T) {
    if (T == NULL) return 0;
    
    Queue Q;
    InitQueue(&Q);
    EnQueue(&Q, T);
    
    int maxWidth = 0;
    
    while (!QueueEmpty(Q)) {
        int levelSize = 0;
        Queue tempQ = Q;
        
        // 计算当前层的节点数
        while (tempQ.front != NULL) {
            levelSize++;
            tempQ.front = tempQ.front->next;
        }
        
        if (levelSize > maxWidth) {
            maxWidth = levelSize;
        }
        
        // 处理当前层的所有节点
        for (int i = 0; i < levelSize; i++) {
            BiTNode *current;
            DeQueue(&Q, &current);
            
            if (current->lchild) EnQueue(&Q, current->lchild);
            if (current->rchild) EnQueue(&Q, current->rchild);
        }
    }
    
    return maxWidth;
}

/**
 * 判断是否为完全二叉树
 */
bool IsCompleteBiTree(BiTree T) {
    if (T == NULL) return true;
    
    Queue Q;
    InitQueue(&Q);
    EnQueue(&Q, T);
    
    bool foundNull = false;
    BiTNode *current;
    
    while (!QueueEmpty(Q)) {
        DeQueue(&Q, &current);
        
        if (current == NULL) {
            foundNull = true;
        } else {
            if (foundNull) return false;
            EnQueue(&Q, current->lchild);
            EnQueue(&Q, current->rchild);
        }
    }
    
    return true;
}

/**
 * 判断是否为满二叉树
 */
bool IsFullBiTree(BiTree T) {
    if (T == NULL) return true;
    
    int depth = BiTreeDepth(T);
    int nodeCount = BiTreeNodeCount(T);
    
    // 满二叉树的节点数应该是 2^depth - 1
    int expectedNodes = (1 << depth) - 1;
    return nodeCount == expectedNodes;
}

/**
 * 判断是否为二叉搜索树
 */
bool IsBSTHelper(BiTree T, char min, char max) {
    if (T == NULL) return true;
    
    if (T->data <= min || T->data >= max) return false;
    
    return IsBSTHelper(T->lchild, min, T->data) && 
           IsBSTHelper(T->rchild, T->data, max);
}

bool IsBST(BiTree T) {
    return IsBSTHelper(T, 0, 127); // ASCII范围
}

/**
 * 判断是否存在路径和为目标值的路径
 */
bool HasPathSumHelper(BiTree T, int targetSum, int currentSum) {
    if (T == NULL) return false;
    
    currentSum += T->data;
    
    // 叶子节点
    if (T->lchild == NULL && T->rchild == NULL) {
        return currentSum == targetSum;
    }
    
    return HasPathSumHelper(T->lchild, targetSum, currentSum) || 
           HasPathSumHelper(T->rchild, targetSum, currentSum);
}

bool HasPathSum(BiTree T, int targetSum) {
    return HasPathSumHelper(T, targetSum, 0);
}

/**
 * 打印所有路径
 */
void PrintAllPathsHelper(BiTree T, char *path, int pathLen) {
    if (T == NULL) return;
    
    path[pathLen] = T->data;
    pathLen++;
    
    // 叶子节点，打印路径
    if (T->lchild == NULL && T->rchild == NULL) {
        printf("路径: ");
        for (int i = 0; i < pathLen; i++) {
            printf("%c", path[i]);
            if (i < pathLen - 1) printf("->");
        }
        printf("\n");
    } else {
        PrintAllPathsHelper(T->lchild, path, pathLen);
        PrintAllPathsHelper(T->rchild, path, pathLen);
    }
}

void PrintAllPaths(BiTree T) {
    char path[1000];
    PrintAllPathsHelper(T, path, 0);
}

/**
 * 计算最大路径和
 */
int MaxPathSumHelper(BiTree T, int *maxSum) {
    if (T == NULL) return 0;
    
    int leftSum = MaxPathSumHelper(T->lchild, maxSum);
    int rightSum = MaxPathSumHelper(T->rchild, maxSum);
    
    // 当前节点为根的最大路径和
    int currentMax = T->data + leftSum + rightSum;
    if (currentMax > *maxSum) {
        *maxSum = currentMax;
    }
    
    // 返回从当前节点向下的最大路径和
    int singlePath = T->data + (leftSum > rightSum ? leftSum : rightSum);
    return singlePath > 0 ? singlePath : 0;
}

int MaxPathSum(BiTree T) {
    int maxSum = INT_MIN;
    MaxPathSumHelper(T, &maxSum);
    return maxSum;
}

/**
 * 计算二叉树直径
 */
int DiameterHelper(BiTree T, int *diameter) {
    if (T == NULL) return 0;
    
    int leftDepth = DiameterHelper(T->lchild, diameter);
    int rightDepth = DiameterHelper(T->rchild, diameter);
    
    int currentDiameter = leftDepth + rightDepth;
    if (currentDiameter > *diameter) {
        *diameter = currentDiameter;
    }
    
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

int DiameterOfBinaryTree(BiTree T) {
    int diameter = 0;
    DiameterHelper(T, &diameter);
    return diameter;
}

/**
 * 镜像二叉树
 */
BiTree MirrorBiTree(BiTree T) {
    if (T == NULL) return NULL;
    
    BiTNode *temp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = temp;
    
    MirrorBiTree(T->lchild);
    MirrorBiTree(T->rchild);
    
    return T;
}

/**
 * 复制二叉树
 */
BiTree CopyBiTree(BiTree T) {
    if (T == NULL) return NULL;
    
    BiTNode *newNode = (BiTNode*)malloc(sizeof(BiTNode));
    newNode->data = T->data;
    newNode->lchild = CopyBiTree(T->lchild);
    newNode->rchild = CopyBiTree(T->rchild);
    
    return newNode;
}

/**
 * 判断是否为镜像对称的树
 */
bool IsSymmetricHelper(BiTree left, BiTree right) {
    if (left == NULL && right == NULL) return true;
    if (left == NULL || right == NULL) return false;
    
    return (left->data == right->data) && 
           IsSymmetricHelper(left->lchild, right->rchild) && 
           IsSymmetricHelper(left->rchild, right->lchild);
}

bool IsSymmetric(BiTree T) {
    if (T == NULL) return true;
    return IsSymmetricHelper(T->lchild, T->rchild);
}

/**
 * 判断两个树是否相同
 */
bool IsSameTree(BiTree T1, BiTree T2) {
    if (T1 == NULL && T2 == NULL) return true;
    if (T1 == NULL || T2 == NULL) return false;
    
    return (T1->data == T2->data) && 
           IsSameTree(T1->lchild, T2->lchild) && 
           IsSameTree(T1->rchild, T2->rchild);
}

/**
 * 根据先序和中序序列构造二叉树
 */
BiTree BuildTreePreIn(char *preorder, char *inorder, int len) {
    if (len <= 0) return NULL;
    
    BiTNode *root = (BiTNode*)malloc(sizeof(BiTNode));
    root->data = preorder[0];
    
    // 在中序中找到根的位置
    int rootIndex = -1;
    for (int i = 0; i < len; i++) {
        if (inorder[i] == preorder[0]) {
            rootIndex = i;
            break;
        }
    }
    
    if (rootIndex == -1) {
        free(root);
        return NULL;
    }
    
    root->lchild = BuildTreePreIn(preorder + 1, inorder, rootIndex);
    root->rchild = BuildTreePreIn(preorder + rootIndex + 1, inorder + rootIndex + 1, len - rootIndex - 1);
    
    return root;
}

/**
 * 根据后序和中序序列构造二叉树
 */
BiTree BuildTreePostIn(char *postorder, char *inorder, int len) {
    if (len <= 0) return NULL;
    
    BiTNode *root = (BiTNode*)malloc(sizeof(BiTNode));
    root->data = postorder[len - 1];
    
    // 在中序中找到根的位置
    int rootIndex = -1;
    for (int i = 0; i < len; i++) {
        if (inorder[i] == postorder[len - 1]) {
            rootIndex = i;
            break;
        }
    }
    
    if (rootIndex == -1) {
        free(root);
        return NULL;
    }
    
    root->lchild = BuildTreePostIn(postorder, inorder, rootIndex);
    root->rchild = BuildTreePostIn(postorder + rootIndex, inorder + rootIndex + 1, len - rootIndex - 1);
    
    return root;
}

// ============= 线索二叉树实现 =============

/**
 * 中序线索化
 */
void InThreading(ThreadTree T, ThreadTree *pre) {
    if (T != NULL) {
        InThreading(T->lchild, pre);
        
        if (T->lchild == NULL) {
            T->ltag = Thread;
            T->lchild = *pre;
        } else {
            T->ltag = Link;
        }
        
        if (*pre != NULL && (*pre)->rchild == NULL) {
            (*pre)->rtag = Thread;
            (*pre)->rchild = T;
        } else if (*pre != NULL) {
            (*pre)->rtag = Link;
        }
        
        *pre = T;
        InThreading(T->rchild, pre);
    }
}

/**
 * 创建中序线索二叉树
 */
void CreateInThread(ThreadTree *T) {
    ThreadTree pre = NULL;
    if (*T != NULL) {
        InThreading(*T, &pre);
        if (pre->rchild == NULL) {
            pre->rtag = Thread;
        }
    }
}

/**
 * 找到中序线索二叉树的第一个节点
 */
ThreadTree FirstNode(ThreadTree T) {
    while (T->ltag == Link) {
        T = T->lchild;
    }
    return T;
}

/**
 * 找到节点T在中序线索二叉树中的后继
 */
ThreadTree NextNode(ThreadTree T) {
    if (T->rtag == Thread) {
        return T->rchild;
    } else {
        return FirstNode(T->rchild);
    }
}

/**
 * 中序线索二叉树的遍历
 */
void InOrderThreadTraverse(ThreadTree T) {
    for (ThreadTree p = FirstNode(T); p != NULL; p = NextNode(p)) {
        printf("%c ", p->data);
    }
}

// ============= 性能测试函数 =============

/**
 * 基本操作性能测试
 */
void PerformanceTest_BasicOps(int nodeCount) {
    printf("基本操作性能测试（%d个节点）\n", nodeCount);
    clock_t start, end;
    
    // 创建大规模测试数据
    char *testData = (char*)malloc(nodeCount * 2 + 1);
    for (int i = 0; i < nodeCount; i++) {
        testData[i * 2] = 'A' + (i % 26);
        testData[i * 2 + 1] = '#';
    }
    testData[nodeCount * 2] = '\0';
    
    char *p = testData;
    start = clock();
    BiTree T = CreateBiTreeByPreOrder(&p);
    end = clock();
    
    double creation_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("树创建时间: %f 秒\n", creation_time);
    
    start = clock();
    int depth = BiTreeDepth(T);
    end = clock();
    
    double depth_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("深度计算时间: %f 秒，深度: %d\n", depth_time, depth);
    
    DestroyBiTree(&T);
    free(testData);
}

/**
 * 遍历性能测试
 */
void PerformanceTest_Traversal(int nodeCount) {
    printf("遍历性能测试（%d个节点）\n", nodeCount);
    
    // 创建完全二叉树
    char *testData = (char*)malloc(nodeCount);
    for (int i = 0; i < nodeCount; i++) {
        testData[i] = 'A' + (i % 26);
    }
    
    BiTree T = CreateBiTreeByArray(testData, 0, nodeCount);
    
    clock_t start, end;
    
    start = clock();
    PreOrder(T);
    end = clock();
    printf("先序遍历时间: %f 秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    start = clock();
    PreOrderNonRecursive(T);
    end = clock();
    printf("先序遍历（非递归）时间: %f 秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    DestroyBiTree(&T);
    free(testData);
}

/**
 * 递归与非递归性能比较
 */
void PerformanceCompare_RecursiveVsNonRecursive(int nodeCount) {
    printf("递归与非递归性能比较（%d个节点）\n", nodeCount);
    
    char *testData = (char*)malloc(nodeCount);
    for (int i = 0; i < nodeCount; i++) {
        testData[i] = 'A' + (i % 26);
    }
    
    BiTree T = CreateBiTreeByArray(testData, 0, nodeCount);
    
    clock_t start, end;
    
    // 递归遍历
    start = clock();
    InOrder(T);
    end = clock();
    double recursive_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 非递归遍历
    start = clock();
    InOrderNonRecursive(T);
    end = clock();
    double non_recursive_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("递归中序遍历时间: %f 秒\n", recursive_time);
    printf("非递归中序遍历时间: %f 秒\n", non_recursive_time);
    printf("性能比值: %.2f\n", recursive_time / non_recursive_time);
    
    DestroyBiTree(&T);
    free(testData);
} 