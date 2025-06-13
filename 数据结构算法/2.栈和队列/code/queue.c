/* queue.c - Queue data structure implementation
 * create by: zw.duan
 */

#include "queue.h"
#include <time.h>

// ============= 顺序队列基本操作实现 =============

/**
 * 初始化顺序队列
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void InitSqQueue(SqQueue *Q) {
    Q->front = Q->rear = 0;  // 初始化队头和队尾指针
}

/**
 * 判断顺序队列是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool SqQueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

/**
 * 顺序队列入队操作
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool SqEnQueue(SqQueue *Q, int e) {
    // 判断队列是否已满
    if (Q->rear == MAXSIZE) {
        return false;  // 队列满，入队失败
    }
    
    // 在队尾插入新元素
    Q->data[Q->rear] = e;
    Q->rear++;          // 队尾指针后移
    
    return true;        // 入队成功
}

/**
 * 顺序队列出队操作
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool SqDeQueue(SqQueue *Q, int *e) {
    // 判断队列是否为空
    if (Q->front == Q->rear) {
        return false;  // 队列空，出队失败
    }
    
    // 取出队头元素
    *e = Q->data[Q->front];
    Q->front++;        // 队头指针后移
    
    return true;       // 出队成功
}

/**
 * 获取顺序队列队头元素（不出队）
 */
bool SqGetHead(SqQueue Q, int *e) {
    if (Q.front == Q.rear) {
        return false;  // 队列空，获取失败
    }
    
    *e = Q.data[Q.front];
    return true;
}

/**
 * 销毁顺序队列
 */
void DestroySqQueue(SqQueue *Q) {
    Q->front = Q->rear = 0;
}

/**
 * 打印顺序队列内容
 */
void PrintSqQueue(SqQueue Q) {
    if (SqQueueEmpty(Q)) {
        printf("顺序队列为空\n");
        return;
    }
    
    printf("顺序队列内容（从队头到队尾）：");
    for (int i = Q.front; i < Q.rear; i++) {
        printf("%d ", Q.data[i]);
    }
    printf("\n");
}

/**
 * 获取顺序队列长度
 */
int SqQueueLength(SqQueue Q) {
    return Q.rear - Q.front;
}

/**
 * 顺序队列判满
 */
bool SqQueueFull(SqQueue Q) {
    return Q.rear == MAXSIZE;
}

// ============= 循环队列基本操作实现 =============

/**
 * 初始化循环队列
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void InitCircularQueue(CircularQueue *Q) {
    Q->front = Q->rear = 0;
}

/**
 * 判断循环队列是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool CircularQueueEmpty(CircularQueue Q) {
    return Q.front == Q.rear;
}

/**
 * 判断循环队列是否已满
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool CircularQueueFull(CircularQueue Q) {
    return (Q.rear + 1) % MAXSIZE == Q.front;
}

/**
 *  循环队列入队操作（EnQueue， 核心）
 * 
 *   循环队列入队过程图示（插入元素7）：
 * 
 * 入队前（队列状态：front=2, rear=6）：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │ a₁  │ a₂  │ a₃  │ a₄  │     │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *               ↑                 ↑
 *            front=2           rear=6
 * 
 * 入队后（队列状态：front=2, rear=7）：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │ a₁  │ a₂  │ a₃  │ a₄  │  7  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *               ↑                       ↑
 *            front=2                 rear=7
 * 
 * 循环特性示例（rear指针到达数组末尾时）：
 * 入队前：rear=9, front=2
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │ a₁  │ a₂  │ a₃  │ a₄  │ a₅  │ a₆  │ a₇  │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *               ↑                                         ↑
 *            front=2                                   rear=9
 * 
 * 入队后：rear=0（循环到数组开头）
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │ a₁  │ a₂  │ a₃  │ a₄  │ a₅  │ a₆  │ a₇  │  8  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *   ↑           ↑
 * rear=0     front=2
 * 
 *  算法步骤：
 * 1. 检查队列是否已满：(rear+1) % MAXSIZE = front
 * 2. 在rear位置插入新元素：data[rear] = e
 * 3. 更新rear指针：rear = (rear+1) % MAXSIZE
 * 
 *  关键点：
 * - 牺牲一个单元法：队满条件是(rear+1)%MAXSIZE = front
 * - 取模运算实现循环：指针到达数组末尾时回到开头
 * - rear指向下一个可插入位置
 * - 解决了顺序队列的假溢出问题
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1) - 常数时间操作
 * - 空间复杂度：O(1) - 不需要额外空间
 * 
 *  
 * - 理解循环队列解决假溢出的原理
 * - 掌握队满判断条件的推导
 * - 熟练使用取模运算实现循环
 * - 理解"牺牲一个单元"的设计思想
 * - 能够分析循环队列的空间利用率
 * 
 * @param Q 指向循环队列的指针
 * @param e 待入队的元素
 * @return 成功返回true，队满返回false
 */
bool CircularEnQueue(CircularQueue *Q, int e) {
    // 步骤1：检查队列是否已满（牺牲一个单元法）
    if ((Q->rear + 1) % MAXSIZE == Q->front) {
        printf("错误：循环队列已满，无法入队元素%d\n", e);
        printf("当前状态：front=%d, rear=%d\n", Q->front, Q->rear);
        return false;  // 队列满，入队失败
    }
    
    // 步骤2：在队尾插入新元素
    Q->data[Q->rear] = e;
    printf("成功入队：%d，插入位置：%d\n", e, Q->rear);
    
    // 步骤3：队尾指针循环后移
    Q->rear = (Q->rear + 1) % MAXSIZE;
    printf("更新rear指针：%d\n", Q->rear);
    
    return true;  // 入队成功
}

/**
 *  循环队列出队操作（DeQueue， 核心）
 * 
 *   循环队列出队过程图示（删除队头元素）：
 * 
 * 出队前（队列状态：front=2, rear=7）：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │ a₁  │ a₂  │ a₃  │ a₄  │ a₅  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *               ↑                       ↑
 *            front=2                 rear=7
 *            (队头元素a₁)
 * 
 * 出队后（队列状态：front=3, rear=7）：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │  ?  │ a₂  │ a₃  │ a₄  │ a₅  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *                     ↑                 ↑
 *                  front=3           rear=7
 *                  (新队头a₂)
 * 
 * 循环特性示例（front指针到达数组末尾时）：
 * 出队前：front=9, rear=2
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ b₁  │ b₂  │     │     │     │     │     │     │     │ a₁  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *             ↑                                         ↑
 *          rear=2                                   front=9
 *                                                  (队头元素a₁)
 * 
 * 出队后：front=0（循环到数组开头）
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ b₁  │ b₂  │     │     │     │     │     │     │     │  ?  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *   ↑           ↑
 * front=0    rear=2
 * (新队头b₁)
 * 
 *  算法步骤：
 * 1. 检查队列是否为空：front = rear
 * 2. 取出队头元素：*e = data[front]
 * 3. 更新front指针：front = (front+1) % MAXSIZE
 * 
 *  关键点：
 * - 队空条件：front = rear
 * - front指向当前队头元素
 * - 取模运算实现循环：指针到达数组末尾时回到开头
 * - 出队后原位置数据不需要清除（逻辑删除）
 * - 体现FIFO原理：先进先出
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1) - 常数时间操作
 * - 空间复杂度：O(1) - 不需要额外空间
 * 
 *  
 * - 理解FIFO原理在循环队列中的体现
 * - 掌握队空判断条件
 * - 熟练使用取模运算实现循环
 * - 理解front指针的移动规律
 * - 区分物理删除和逻辑删除
 * 
 * @param Q 指向循环队列的指针
 * @param e 用于返回出队元素的指针
 * @return 成功返回true，队空返回false
 */
bool CircularDeQueue(CircularQueue *Q, int *e) {
    // 步骤1：检查队列是否为空
    if (Q->front == Q->rear) {
        printf("错误：循环队列为空，无法出队\n");
        printf("当前状态：front=%d, rear=%d\n", Q->front, Q->rear);
        return false;  // 队列空，出队失败
    }
    
    // 步骤2：取出队头元素
    *e = Q->data[Q->front];
    printf("成功出队：%d，出队位置：%d\n", *e, Q->front);
    
    // 步骤3：队头指针循环后移
    Q->front = (Q->front + 1) % MAXSIZE;
    printf("更新front指针：%d\n", Q->front);
    
    return true;  // 出队成功
}

/**
 * 获取循环队列队头元素（不出队）
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool CircularGetHead(CircularQueue Q, int *e) {
    // 判断队列是否为空
    if (Q.front == Q.rear) {
        return false;  // 队列空，获取失败
    }
    
    // 返回队头元素
    *e = Q.data[Q.front];
    return true;  // 获取成功
}

/**
 * 销毁循环队列
 */
void DestroyCircularQueue(CircularQueue *Q) {
    Q->front = Q->rear = 0;
}

/**
 * 打印循环队列内容
 */
void PrintCircularQueue(CircularQueue Q) {
    if (CircularQueueEmpty(Q)) {
        printf("循环队列为空\n");
        return;
    }
    
    printf("循环队列内容（从队头到队尾）：");
    int i = Q.front;
    while (i != Q.rear) {
        printf("%d ", Q.data[i]);
        i = (i + 1) % MAXSIZE;
    }
    printf("\n");
}

/**
 * 计算循环队列长度
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
int CircularQueueLength(CircularQueue Q) {
    return (Q.rear - Q.front + MAXSIZE) % MAXSIZE;
}

/**
 * 获取循环队列的第k个元素（从队头算起）
 * 时间复杂度：O(1)
 */
bool GetKthElement(CircularQueue Q, int k, int *e) {
    int length = CircularQueueLength(Q);
    if (k < 1 || k > length) {
        return false;  // k超出范围
    }
    
    int index = (Q.front + k - 1) % MAXSIZE;
    *e = Q.data[index];
    return true;
}

/**
 * 判断循环队列元素个数是否超过一半容量
 */
bool IsMoreThanHalf(CircularQueue Q) {
    int length = CircularQueueLength(Q);
    return length > (MAXSIZE - 1) / 2;  // 注意：实际容量是MAXSIZE-1
}

// ============= 计数器法循环队列实现 =============

/**
 * 初始化带计数器的循环队列
 */
void InitCountQueue(CircularQueueWithCount *Q) {
    Q->front = Q->rear = 0;
    Q->count = 0;
}

/**
 * 判断带计数器的循环队列是否为空
 */
bool CountQueueEmpty(CircularQueueWithCount Q) {
    return Q.count == 0;
}

/**
 * 判断带计数器的循环队列是否已满
 */
bool CountQueueFull(CircularQueueWithCount Q) {
    return Q.count == MAXSIZE;
}

/**
 * 带计数器的循环队列入队操作
 */
bool CountEnQueue(CircularQueueWithCount *Q, int e) {
    if (Q->count == MAXSIZE) {
        return false;  // 队列满
    }
    
    Q->data[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXSIZE;
    Q->count++;
    
    return true;
}

/**
 * 带计数器的循环队列出队操作
 */
bool CountDeQueue(CircularQueueWithCount *Q, int *e) {
    if (Q->count == 0) {
        return false;  // 队列空
    }
    
    *e = Q->data[Q->front];
    Q->front = (Q->front + 1) % MAXSIZE;
    Q->count--;
    
    return true;
}

// ============= 链式队列基本操作实现 =============

/**
 * 初始化链式队列
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void InitLinkQueue(LinkQueue *Q) {
    Q->front = Q->rear = NULL;  // 初始化为空队列
}

/**
 * 判断链式队列是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkQueueEmpty(LinkQueue Q) {
    return Q.front == NULL;
}

/**
 * 链式队列入队操作
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkEnQueue(LinkQueue *Q, int e) {
    // 分配新节点
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        return false;  // 内存分配失败
    }
    
    // 设置新节点
    newNode->data = e;
    newNode->next = NULL;
    
    // 判断队列是否为空
    if (Q->rear == NULL) {
        Q->front = Q->rear = newNode;  // 空队列，front和rear都指向新节点
    } else {
        Q->rear->next = newNode;       // 新节点链接到队尾
        Q->rear = newNode;             // 更新队尾指针
    }
    
    return true;  // 入队成功
}

/**
 * 链式队列出队操作
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkDeQueue(LinkQueue *Q, int *e) {
    // 判断队列是否为空
    if (Q->front == NULL) {
        return false;  // 队列空，出队失败
    }
    
    // 记录队头节点
    QueueNode *p = Q->front;
    *e = p->data;
    
    // 更新队头指针
    Q->front = p->next;
    
    // 如果队列变空，需要同时更新rear指针
    if (Q->front == NULL) {
        Q->rear = NULL;
    }
    
    // 释放原队头节点
    free(p);
    
    return true;  // 出队成功
}

/**
 * 获取链式队列队头元素（不出队）
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkGetHead(LinkQueue Q, int *e) {
    if (Q.front == NULL) {
        return false;  // 队列空，获取失败
    }
    
    *e = Q.front->data;
    return true;  // 获取成功
}

/**
 * 销毁链式队列
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void DestroyLinkQueue(LinkQueue *Q) {
    while (Q->front != NULL) {
        QueueNode *temp = Q->front;
        Q->front = Q->front->next;
        free(temp);
    }
    Q->rear = NULL;
}

/**
 * 打印链式队列内容
 */
void PrintLinkQueue(LinkQueue Q) {
    if (LinkQueueEmpty(Q)) {
        printf("链式队列为空\n");
        return;
    }
    
    printf("链式队列内容（从队头到队尾）：");
    QueueNode *p = Q.front;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/**
 * 获取链式队列长度
 */
int LinkQueueLength(LinkQueue Q) {
    int count = 0;
    QueueNode *p = Q.front;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// ============= 队列应用算法实现 =============

/**
 * 创建示例二叉树
 */
TreeNode* CreateSampleTree(void) {
    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode *n1 = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode *n2 = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode *n3 = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode *n4 = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode *n5 = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode *n6 = (TreeNode*)malloc(sizeof(TreeNode));
    
    root->data = 1;
    n1->data = 2; n2->data = 3;
    n3->data = 4; n4->data = 5;
    n5->data = 6; n6->data = 7;
    
    root->left = n1; root->right = n2;
    n1->left = n3; n1->right = n4;
    n2->left = n5; n2->right = n6;
    n3->left = n3->right = NULL;
    n4->left = n4->right = NULL;
    n5->left = n5->right = NULL;
    n6->left = n6->right = NULL;
    
    return root;
}

/**
 * 销毁二叉树
 */
void DestroyTree(TreeNode *root) {
    if (root != NULL) {
        DestroyTree(root->left);
        DestroyTree(root->right);
        free(root);
    }
}

/**
 *  二叉树层次遍历（队列经典应用， 重点）
 * 
 *   层次遍历过程图示：
 * 
 * 示例二叉树：
 *         1
 *       /   \
 *      2     3
 *     / \   / \
 *    4   5 6   7
 * 
 * 遍历过程（队列状态变化）：
 * 
 * 初始：队列 = [1]
 * 步骤1：出队1，访问1，入队2,3    → 队列 = [2,3]
 * 步骤2：出队2，访问2，入队4,5    → 队列 = [3,4,5]
 * 步骤3：出队3，访问3，入队6,7    → 队列 = [4,5,6,7]
 * 步骤4：出队4，访问4，无子节点    → 队列 = [5,6,7]
 * 步骤5：出队5，访问5，无子节点    → 队列 = [6,7]
 * 步骤6：出队6，访问6，无子节点    → 队列 = [7]
 * 步骤7：出队7，访问7，无子节点    → 队列 = []
 * 
 * 遍历结果：1 2 3 4 5 6 7（按层次从左到右）
 * 
 *  算法步骤：
 * 1. 初始化队列，根节点入队
 * 2. 当队列非空时：
 *    a) 出队一个节点并访问
 *    b) 将该节点的左右子树（如果存在）依次入队
 * 3. 重复步骤2直到队列为空
 * 
 *  关键点：
 * - 利用队列的FIFO特性实现层次访问
 * - 先访问的节点，其子节点也先被访问
 * - 保证同一层的节点按从左到右的顺序访问
 * - 队列中存储的是待访问节点的指针
 * 
 *   复杂度分析：
 * - 时间复杂度：O(n) - 每个节点访问一次
 * - 空间复杂度：O(w) - w为二叉树的最大宽度
 *   最坏情况：完全二叉树，最大宽度为n/2，空间复杂度O(n)
 * 
 *  
 * - 理解队列在层次遍历中的作用
 * - 掌握算法的核心思想和实现步骤
 * - 能够分析时间和空间复杂度
 * - 理解与深度优先遍历的区别
 * - 能够扩展到其他层次遍历问题
 * 
 * @param root 二叉树根节点指针
 */
void LevelOrder(TreeNode *root) {
    if (root == NULL) {
        printf("二叉树为空，无法进行层次遍历\n");
        return;
    }
    
    // 使用数组模拟队列存储树节点指针
    TreeNode *queue[MAXSIZE];
    int front = 0, rear = 0;
    
    printf("开始二叉树层次遍历...\n");
    
    // 步骤1：根节点入队
    queue[rear++] = root;
    printf("根节点%d入队\n", root->data);
    
    printf("层次遍历结果：");
    int level = 1;
    
    // 步骤2：队列非空时循环处理
    while (front < rear) {
        // 步骤2a：出队一个节点并访问
        TreeNode *node = queue[front++];
        printf("%d ", node->data);
        
        // 步骤2b：将左右子树依次入队
        if (node->left != NULL) {
            queue[rear++] = node->left;
            printf("\n节点%d的左子树%d入队", node->data, node->left->data);
        }
        if (node->right != NULL) {
            queue[rear++] = node->right;
            printf("\n节点%d的右子树%d入队", node->data, node->right->data);
        }
        
        // 显示当前队列状态（调试信息）
        if (front < rear) {
            printf("\n当前队列状态：[");
            for (int i = front; i < rear; i++) {
                printf("%d", queue[i]->data);
                if (i < rear - 1) printf(",");
            }
            printf("]\n");
        }
    }
    
    printf("\n层次遍历完成！\n");
}

/**
 * 创建示例图
 */
Graph* CreateSampleGraph(void) {
    Graph *G = (Graph*)malloc(sizeof(Graph));
    G->vertices = 6;
    
    // 初始化邻接矩阵
    for (int i = 0; i < MAXVERTEX; i++) {
        for (int j = 0; j < MAXVERTEX; j++) {
            G->edges[i][j] = 0;
        }
    }
    
    // 添加边
    G->edges[0][1] = G->edges[1][0] = 1;
    G->edges[0][2] = G->edges[2][0] = 1;
    G->edges[1][3] = G->edges[3][1] = 1;
    G->edges[1][4] = G->edges[4][1] = 1;
    G->edges[2][5] = G->edges[5][2] = 1;
    G->edges[3][4] = G->edges[4][3] = 1;
    
    return G;
}

/**
 * 图的广度优先搜索
 * 时间复杂度：O(V + E)，其中V为顶点数，E为边数
 * 空间复杂度：O(V)
 */
void BFS(Graph *G, int start) {
    bool visited[MAXVERTEX] = {false};  // 访问标记数组
    CircularQueue Q;
    InitCircularQueue(&Q);
    
    // 访问起始顶点
    printf("BFS遍历结果：%d ", start);
    visited[start] = true;
    CircularEnQueue(&Q, start);
    
    while (!CircularQueueEmpty(Q)) {
        int v;
        CircularDeQueue(&Q, &v);
        
        // 访问v的所有未访问邻接顶点
        for (int w = 0; w < G->vertices; w++) {
            if (G->edges[v][w] == 1 && !visited[w]) {
                printf("%d ", w);
                visited[w] = true;
                CircularEnQueue(&Q, w);
            }
        }
    }
    printf("\n");
}

/**
 * 任务调度队列操作
 */
bool AddTask(TaskQueue *Q, Task task) {
    if ((Q->rear + 1) % MAXSIZE == Q->front) {
        return false;  // 队列满
    }
    Q->data[Q->rear] = task;
    Q->rear = (Q->rear + 1) % MAXSIZE;
    return true;
}

bool ExecuteTask(TaskQueue *Q, Task *task) {
    if (Q->front == Q->rear) {
        return false;  // 队列空
    }
    *task = Q->data[Q->front];
    Q->front = (Q->front + 1) % MAXSIZE;
    return true;
}

void PrintTaskQueue(TaskQueue Q) {
    if (Q.front == Q.rear) {
        printf("任务队列为空\n");
        return;
    }
    
    printf("当前任务队列：\n");
    int i = Q.front;
    while (i != Q.rear) {
        Task task = Q.data[i];
        printf("任务ID:%d, 优先级:%d, 描述:%s\n", 
               task.taskId, task.priority, task.description);
        i = (i + 1) % MAXSIZE;
    }
}

// ============= 高级应用算法实现 =============

/**
 * 用两个队列实现栈
 */
void InitQueueStack(QueueStack *S) {
    InitCircularQueue(&S->q1);
    InitCircularQueue(&S->q2);
}

bool QueueStackEmpty(QueueStack S) {
    return CircularQueueEmpty(S.q1) && CircularQueueEmpty(S.q2);
}

bool QueuePush(QueueStack *S, int e) {
    return CircularEnQueue(&S->q1, e);
}

bool QueuePop(QueueStack *S, int *e) {
    if (QueueStackEmpty(*S)) {
        return false;
    }
    
    // 将q1中除最后一个元素外的所有元素移到q2
    while (CircularQueueLength(S->q1) > 1) {
        int temp;
        CircularDeQueue(&S->q1, &temp);
        CircularEnQueue(&S->q2, temp);
    }
    
    // 取出q1中的最后一个元素（即栈顶元素）
    CircularDeQueue(&S->q1, e);
    
    // 交换q1和q2
    CircularQueue temp = S->q1;
    S->q1 = S->q2;
    S->q2 = temp;
    
    return true;
}

bool QueueStackTop(QueueStack S, int *e) {
    if (QueueStackEmpty(S)) {
        return false;  // 栈空
    }
    
    // 需要遍历到最后一个元素
    int result = -1;
    int i = S.q1.front;
    while (i != S.q1.rear) {
        result = S.q1.data[i];
        i = (i + 1) % MAXSIZE;
    }
    *e = result;
    return true;
}

/**
 * 队列逆转
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
void ReverseQueue(CircularQueue *Q) {
    // 使用栈来辅助逆转
    int stack[MAXSIZE];
    int top = -1;
    
    // 队列元素全部压入栈
    while (!CircularQueueEmpty(*Q)) {
        int e;
        CircularDeQueue(Q, &e);
        stack[++top] = e;
    }
    
    // 栈中元素全部弹出到队列
    while (top >= 0) {
        CircularEnQueue(Q, stack[top--]);
    }
}

/**
 * 优先队列实现
 */
void InitPriorityQueue(PriorityQueue *PQ) {
    PQ->size = 0;
}

bool PriorityQueueEmpty(PriorityQueue PQ) {
    return PQ.size == 0;
}

bool PriorityQueueFull(PriorityQueue PQ) {
    return PQ.size == MAXSIZE;
}

/**
 * 插入元素（保持优先级有序）
 * 时间复杂度：O(n)
 */
bool PriorityInsert(PriorityQueue *PQ, int data, int priority) {
    if (PQ->size == MAXSIZE) return false;
    
    // 找到插入位置
    int i = PQ->size - 1;
    while (i >= 0 && PQ->elements[i].priority < priority) {
        PQ->elements[i + 1] = PQ->elements[i];
        i--;
    }
    
    // 插入新元素
    PQ->elements[i + 1].data = data;
    PQ->elements[i + 1].priority = priority;
    PQ->size++;
    
    return true;
}

/**
 * 删除最高优先级元素
 * 时间复杂度：O(1)
 */
bool PriorityDeleteMax(PriorityQueue *PQ, int *data) {
    if (PQ->size == 0) return false;
    
    *data = PQ->elements[PQ->size - 1].data;
    PQ->size--;
    
    return true;
}

void PrintPriorityQueue(PriorityQueue PQ) {
    if (PriorityQueueEmpty(PQ)) {
        printf("优先队列为空\n");
        return;
    }
    
    printf("优先队列（按优先级排序）：\n");
    for (int i = PQ.size - 1; i >= 0; i--) {
        printf("数据:%d, 优先级:%d\n", 
               PQ.elements[i].data, PQ.elements[i].priority);
    }
}

// ============= 测试和演示函数实现 =============

/**
 * 测试顺序队列基本操作
 */
void TestSqQueueBasicOps(void) {
    printf("\n=== 顺序队列基本操作测试 ===\n");
    
    SqQueue Q;
    InitSqQueue(&Q);
    
    printf("1. 初始化后队列是否为空：%s\n", SqQueueEmpty(Q) ? "是" : "否");
    
    // 入队测试
    printf("2. 入队操作：");
    for (int i = 1; i <= 5; i++) {
        if (SqEnQueue(&Q, i * 10)) {
            printf("%d ", i * 10);
        }
    }
    printf("\n");
    
    PrintSqQueue(Q);
    printf("   队列长度：%d\n", SqQueueLength(Q));
    
    // 取队头测试
    int head;
    if (SqGetHead(Q, &head)) {
        printf("3. 队头元素：%d\n", head);
    }
    
    // 出队测试
    printf("4. 出队操作：");
    while (!SqQueueEmpty(Q)) {
        int e;
        if (SqDeQueue(&Q, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
    
    printf("5. 出队后队列是否为空：%s\n", SqQueueEmpty(Q) ? "是" : "否");
    
    // 演示假溢出
    printf("\n6. 假溢出演示：\n");
    for (int i = 1; i <= 5; i++) {
        SqEnQueue(&Q, i);
    }
    printf("入队5个元素后，front=%d, rear=%d\n", Q.front, Q.rear);
    
    for (int i = 0; i < 3; i++) {
        int e;
        SqDeQueue(&Q, &e);
    }
    printf("出队3个元素后，front=%d, rear=%d\n", Q.front, Q.rear);
    printf("此时队列中还有%d个元素，但rear已经到达边界\n", SqQueueLength(Q));
}

/**
 * 测试循环队列基本操作
 */
void TestCircularQueueBasicOps(void) {
    printf("\n=== 循环队列基本操作测试 ===\n");
    
    CircularQueue Q;
    InitCircularQueue(&Q);
    
    printf("1. 初始化后队列是否为空：%s\n", CircularQueueEmpty(Q) ? "是" : "否");
    
    // 入队测试
    printf("2. 入队操作：");
    for (int i = 1; i <= 6; i++) {
        if (CircularEnQueue(&Q, i * 10)) {
            printf("%d ", i * 10);
        }
    }
    printf("\n");
    
    PrintCircularQueue(Q);
    printf("   队列长度：%d\n", CircularQueueLength(Q));
    printf("   front=%d, rear=%d\n", Q.front, Q.rear);
    
    // 出队部分元素
    printf("3. 出队3个元素：");
    for (int i = 0; i < 3; i++) {
        int e;
        if (CircularDeQueue(&Q, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
    
    PrintCircularQueue(Q);
    printf("   队列长度：%d\n", CircularQueueLength(Q));
    printf("   front=%d, rear=%d\n", Q.front, Q.rear);
    
    // 再入队测试循环特性
    printf("4. 再入队3个元素：");
    for (int i = 7; i <= 9; i++) {
        if (CircularEnQueue(&Q, i * 10)) {
            printf("%d ", i * 10);
        }
    }
    printf("\n");
    
    PrintCircularQueue(Q);
    printf("   队列长度：%d\n", CircularQueueLength(Q));
    printf("   front=%d, rear=%d\n", Q.front, Q.rear);
    
    // 测试扩展操作
    int kth;
    if (GetKthElement(Q, 3, &kth)) {
        printf("5. 第3个元素：%d\n", kth);
    }
    
    printf("6. 元素个数是否超过一半容量：%s\n", IsMoreThanHalf(Q) ? "是" : "否");
}

/**
 * 测试链式队列基本操作
 */
void TestLinkQueueBasicOps(void) {
    printf("\n=== 链式队列基本操作测试 ===\n");
    
    LinkQueue Q;
    InitLinkQueue(&Q);
    
    printf("1. 初始化后队列是否为空：%s\n", LinkQueueEmpty(Q) ? "是" : "否");
    
    // 入队测试
    printf("2. 入队操作：");
    for (int i = 1; i <= 5; i++) {
        if (LinkEnQueue(&Q, i * 20)) {
            printf("%d ", i * 20);
        }
    }
    printf("\n");
    
    PrintLinkQueue(Q);
    printf("   队列长度：%d\n", LinkQueueLength(Q));
    
    // 取队头测试
    int head;
    if (LinkGetHead(Q, &head)) {
        printf("3. 队头元素：%d\n", head);
    }
    
    // 出队测试
    printf("4. 出队操作：");
    while (!LinkQueueEmpty(Q)) {
        int e;
        if (LinkDeQueue(&Q, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
    
    printf("5. 出队后队列是否为空：%s\n", LinkQueueEmpty(Q) ? "是" : "否");
    
    DestroyLinkQueue(&Q);
}

/**
 * 测试循环队列高级功能
 */
void TestCircularQueueAdvanced(void) {
    printf("\n=== 循环队列高级功能测试 ===\n");
    
    // 测试判满条件
    CircularQueue Q;
    InitCircularQueue(&Q);
    
    printf("1. 测试队满条件：\n");
    int count = 0;
    while (!CircularQueueFull(Q)) {
        CircularEnQueue(&Q, count++);
    }
    printf("最大可入队元素个数：%d\n", count);
    printf("此时 front=%d, rear=%d\n", Q.front, Q.rear);
    printf("队满条件：(rear+1) %% MAXSIZE == front\n");
    printf("验证：(%d+1) %% %d = %d, front = %d\n", 
           Q.rear, MAXSIZE, (Q.rear+1)%MAXSIZE, Q.front);
    
    // 测试长度公式
    printf("\n2. 测试长度公式：\n");
    printf("当前长度（公式计算）：%d\n", CircularQueueLength(Q));
    printf("实际入队元素个数：%d\n", count);
    
    // 测试获取第k个元素
    printf("\n3. 测试获取第k个元素：\n");
    for (int k = 1; k <= 3; k++) {
        int element;
        if (GetKthElement(Q, k, &element)) {
            printf("第%d个元素：%d\n", k, element);
        }
    }
}

/**
 * 测试计数器法队列
 */
void TestCountQueue(void) {
    printf("\n=== 计数器法循环队列测试 ===\n");
    
    CircularQueueWithCount Q;
    InitCountQueue(&Q);
    
    printf("1. 初始状态：count=%d, empty=%s, full=%s\n", 
           Q.count, CountQueueEmpty(Q) ? "是" : "否", 
           CountQueueFull(Q) ? "是" : "否");
    
    // 入队到满
    printf("2. 入队到满：\n");
    int i = 0;
    while (!CountQueueFull(Q)) {
        CountEnQueue(&Q, i++);
        if (i <= 5 || i % 20 == 0) {
            printf("入队%d个元素，count=%d\n", i, Q.count);
        }
    }
    
    printf("最终状态：count=%d, empty=%s, full=%s\n", 
           Q.count, CountQueueEmpty(Q) ? "是" : "否", 
           CountQueueFull(Q) ? "是" : "否");
    
    // 出队测试
    printf("3. 出队一半元素：\n");
    for (int j = 0; j < Q.count / 2; j++) {
        int e;
        CountDeQueue(&Q, &e);
    }
    printf("出队后状态：count=%d, empty=%s, full=%s\n", 
           Q.count, CountQueueEmpty(Q) ? "是" : "否", 
           CountQueueFull(Q) ? "是" : "否");
}

/**
 * 测试二叉树层次遍历
 */
void TestTreeLevelOrder(void) {
    printf("\n=== 二叉树层次遍历测试 ===\n");
    
    TreeNode *root = CreateSampleTree();
    printf("创建示例二叉树结构：\n");
    printf("       1\n");
    printf("      / \\\n");
    printf("     2   3\n");
    printf("    / \\ / \\\n");
    printf("   4  5 6  7\n\n");
    
    LevelOrder(root);
    
    DestroyTree(root);
}

/**
 * 测试图的广度优先搜索
 */
void TestGraphBFS(void) {
    printf("\n=== 图的广度优先搜索测试 ===\n");
    
    Graph *G = CreateSampleGraph();
    
    printf("图的邻接关系：\n");
    printf("0 -- 1, 2\n");
    printf("1 -- 0, 3, 4\n");
    printf("2 -- 0, 5\n");
    printf("3 -- 1, 4\n");
    printf("4 -- 1, 3\n");
    printf("5 -- 2\n\n");
    
    for (int i = 0; i < G->vertices; i++) {
        printf("从顶点%d开始的", i);
        BFS(G, i);
    }
    
    free(G);
}

/**
 * 测试任务调度
 */
void TestTaskScheduling(void) {
    printf("\n=== 任务调度队列测试 ===\n");
    
    TaskQueue Q;
    Q.front = Q.rear = 0;
    
    // 创建任务
    Task tasks[] = {
        {1, 5, "高优先级任务"},
        {2, 3, "中优先级任务"},
        {3, 1, "低优先级任务"},
        {4, 4, "次高优先级任务"}
    };
    
    int numTasks = sizeof(tasks) / sizeof(tasks[0]);
    
    printf("1. 添加任务：\n");
    for (int i = 0; i < numTasks; i++) {
        if (AddTask(&Q, tasks[i])) {
            printf("添加任务%d：%s\n", tasks[i].taskId, tasks[i].description);
        }
    }
    
    printf("\n2. 当前任务队列状态：\n");
    PrintTaskQueue(Q);
    
    printf("\n3. 执行任务（FIFO顺序）：\n");
    while (Q.front != Q.rear) {
        Task task;
        if (ExecuteTask(&Q, &task)) {
            printf("执行任务%d：%s（优先级：%d）\n", 
                   task.taskId, task.description, task.priority);
        }
    }
}

/**
 * 队列性能对比测试
 */
void QueuePerformanceTest(void) {
    printf("\n=== 队列性能对比测试 ===\n");
    
    const int TEST_SIZE = 50000;
    clock_t start, end;
    
    // 顺序队列性能测试（只测试小规模，避免假溢出）
    printf("1. 顺序队列性能（小规模测试）：\n");
    SqQueue sqQueue;
    InitSqQueue(&sqQueue);
    
    start = clock();
    for (int i = 0; i < 1000; i++) {
        SqEnQueue(&sqQueue, i);
    }
    for (int i = 0; i < 1000; i++) {
        int temp;
        SqDeQueue(&sqQueue, &temp);
    }
    end = clock();
    
    double sqTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("   1000次操作用时：%.6f 秒\n", sqTime);
    
    // 循环队列性能测试
    CircularQueue circularQueue;
    InitCircularQueue(&circularQueue);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        if (CircularQueueFull(circularQueue)) {
            int temp;
            CircularDeQueue(&circularQueue, &temp);
        }
        CircularEnQueue(&circularQueue, i);
    }
    while (!CircularQueueEmpty(circularQueue)) {
        int temp;
        CircularDeQueue(&circularQueue, &temp);
    }
    end = clock();
    
    double circularTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 链式队列性能测试
    LinkQueue linkQueue;
    InitLinkQueue(&linkQueue);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        LinkEnQueue(&linkQueue, i);
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        int temp;
        LinkDeQueue(&linkQueue, &temp);
    }
    end = clock();
    
    double linkTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("2. 大规模测试结果（%d次入队+出队操作）：\n", TEST_SIZE);
    printf("   循环队列用时：%.6f 秒\n", circularTime);
    printf("   链式队列用时：%.6f 秒\n", linkTime);
    printf("   性能比（链式/循环）：%.2f\n", linkTime / circularTime);
    
    DestroyLinkQueue(&linkQueue);
}

/**
 * 测试用队列实现栈
 */
void TestQueueStack(void) {
    printf("\n=== 用队列实现栈测试 ===\n");
    
    QueueStack S;
    InitQueueStack(&S);
    
    printf("1. 入栈操作：");
    for (int i = 1; i <= 5; i++) {
        QueuePush(&S, i * 10);
        printf("%d ", i * 10);
    }
    printf("\n");
    
    int top;
    if (QueueStackTop(S, &top)) {
        printf("2. 栈顶元素：%d\n", top);
    } else {
        printf("2. 栈为空\n");
    }
    
    printf("3. 出栈操作：");
    while (!QueueStackEmpty(S)) {
        int e;
        if (QueuePop(&S, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
}

/**
 * 测试队列逆转
 */
void TestQueueReverse(void) {
    printf("\n=== 队列逆转测试 ===\n");
    
    CircularQueue Q;
    InitCircularQueue(&Q);
    
    // 入队
    printf("1. 原始队列：");
    for (int i = 1; i <= 6; i++) {
        CircularEnQueue(&Q, i * 10);
        printf("%d ", i * 10);
    }
    printf("\n");
    
    PrintCircularQueue(Q);
    
    // 逆转
    ReverseQueue(&Q);
    
    printf("2. 逆转后队列：");
    PrintCircularQueue(Q);
    
    // 验证逆转结果
    printf("3. 出队验证：");
    while (!CircularQueueEmpty(Q)) {
        int e;
        CircularDeQueue(&Q, &e);
        printf("%d ", e);
    }
    printf("\n");
}

/**
 * 测试优先队列
 */
void TestPriorityQueue(void) {
    printf("\n=== 优先队列测试 ===\n");
    
    PriorityQueue PQ;
    InitPriorityQueue(&PQ);
    
    // 插入元素
    printf("1. 插入元素：\n");
    int data[] = {10, 30, 20, 40, 15};
    int priority[] = {1, 3, 2, 4, 1};
    int size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < size; i++) {
        PriorityInsert(&PQ, data[i], priority[i]);
        printf("插入 数据:%d, 优先级:%d\n", data[i], priority[i]);
    }
    
    printf("\n2. 当前优先队列状态：\n");
    PrintPriorityQueue(PQ);
    
    printf("\n3. 按优先级删除元素：\n");
    while (!PriorityQueueEmpty(PQ)) {
        int element;
        if (PriorityDeleteMax(&PQ, &element)) {
            printf("删除最高优先级元素：%d\n", element);
        }
    }
}

/**
 *  真题演示
 */
void QueueExamDemonstration(void) {
    printf("\n===  真题演示 ===\n");
    
    printf("题目1：设循环队列容量为6，当前状态front=1, rear=5\n");
    printf("求：(1)队列长度 (2)再入队一个元素后是否队满\n\n");
    
    CircularQueue Q;
    Q.front = 1;
    Q.rear = 5;
    // 模拟数据
    for (int i = 0; i < MAXSIZE; i++) {
        Q.data[i] = i * 10;
    }
    
    printf("解答过程：\n");
    printf("(1) 队列长度公式：(rear - front + MAXSIZE) %% MAXSIZE\n");
    printf("    当前长度 = (%d - %d + %d) %% %d = %d\n", 
           Q.rear, Q.front, MAXSIZE, MAXSIZE, CircularQueueLength(Q));
    
    printf("\n(2) 入队一个元素后：rear = (%d + 1) %% %d = %d\n", 
           Q.rear, MAXSIZE, (Q.rear + 1) % MAXSIZE);
    printf("    队满条件：(rear + 1) %% MAXSIZE == front\n");
    printf("    验证：(%d + 1) %% %d = %d, front = %d\n", 
           (Q.rear + 1) % MAXSIZE, MAXSIZE, 
           ((Q.rear + 1) % MAXSIZE + 1) % MAXSIZE, Q.front);
    printf("    结论：%s\n", 
           ((Q.rear + 1) % MAXSIZE + 1) % MAXSIZE == Q.front ? "队满" : "不满");
    
    printf("\n题目2：用两个栈S1、S2实现队列，给出入队和出队算法\n");
    printf("解答：见StackQueue实现，核心思想是S1作输入栈，S2作输出栈\n");
    printf("入队：直接压入S1\n");
    printf("出队：从S2弹出，若S2空则先将S1所有元素倒入S2\n");
}

/**
 * 队列教学演示
 */
void QueueTeachingDemo(void) {
    printf("\n=== 队列教学演示 ===\n");
    
    printf("1. 队列的特点：先进先出（FIFO）\n");
    printf("2. 队列的基本操作：入队、出队、取队头\n");
    printf("3. 队列的应用：BFS遍历、任务调度、缓冲区等\n\n");
    
    // 演示队列的FIFO特性
    printf("演示FIFO特性：\n");
    CircularQueue demo;
    InitCircularQueue(&demo);
    
    printf("依次入队：1, 2, 3, 4, 5\n");
    for (int i = 1; i <= 5; i++) {
        CircularEnQueue(&demo, i);
        printf("入队 %d 后：", i);
        PrintCircularQueue(demo);
    }
    
    printf("\n依次出队：\n");
    while (!CircularQueueEmpty(demo)) {
        int e;
        CircularDeQueue(&demo, &e);
        printf("出队 %d 后：", e);
        PrintCircularQueue(demo);
    }
    
    printf("\n4. 循环队列解决假溢出问题演示：\n");
    printf("   - 顺序队列存在假溢出问题\n");
    printf("   - 循环队列通过模运算解决\n");
    printf("   - 判满条件：(rear+1)%%MAXSIZE == front\n");
    printf("   - 长度公式：(rear-front+MAXSIZE)%%MAXSIZE\n");
}

// ============= 新增的函数实现 =============

/**
 * 双端队列初始化
 */
void InitDeque(Deque *D) {
    D->front = D->rear = 0;
}

/**
 * 双端队列判空
 */
bool DequeEmpty(Deque D) {
    return D.front == D.rear;
}

/**
 * 双端队列判满
 */
bool DequeFull(Deque D) {
    return (D.rear + 1) % MAXSIZE == D.front;
}

/**
 * 双端队列队头插入
 */
bool PushFront(Deque *D, int e) {
    if (DequeFull(*D)) {
        return false;
    }
    
    D->front = (D->front - 1 + MAXSIZE) % MAXSIZE;
    D->data[D->front] = e;
    return true;
}

/**
 * 双端队列队尾插入
 */
bool PushRear(Deque *D, int e) {
    if (DequeFull(*D)) {
        return false;
    }
    
    D->data[D->rear] = e;
    D->rear = (D->rear + 1) % MAXSIZE;
    return true;
}

/**
 * 双端队列队头删除
 */
bool PopFront(Deque *D, int *e) {
    if (DequeEmpty(*D)) {
        return false;
    }
    
    *e = D->data[D->front];
    D->front = (D->front + 1) % MAXSIZE;
    return true;
}

/**
 * 双端队列队尾删除
 */
bool PopRear(Deque *D, int *e) {
    if (DequeEmpty(*D)) {
        return false;
    }
    
    D->rear = (D->rear - 1 + MAXSIZE) % MAXSIZE;
    *e = D->data[D->rear];
    return true;
}

/**
 * 获取双端队列队头元素
 */
bool GetFront(Deque D, int *e) {
    if (DequeEmpty(D)) {
        return false;
    }
    
    *e = D.data[D.front];
    return true;
}

/**
 * 获取双端队列队尾元素
 */
bool GetRear(Deque D, int *e) {
    if (DequeEmpty(D)) {
        return false;
    }
    
    int rear_index = (D.rear - 1 + MAXSIZE) % MAXSIZE;
    *e = D.data[rear_index];
    return true;
}

/**
 * 打印双端队列
 */
void PrintDeque(Deque D) {
    if (DequeEmpty(D)) {
        printf("双端队列为空\n");
        return;
    }
    
    printf("双端队列：队头 ");
    int i = D.front;
    while (i != D.rear) {
        printf("%d ", D.data[i]);
        i = (i + 1) % MAXSIZE;
    }
    printf("队尾\n");
}


/**
 * 约瑟夫问题求解
 */
int JosephusProblem(int n, int k) {
    CircularQueue Q;
    InitCircularQueue(&Q);
    
    // 将1到n的人编号加入队列
    for (int i = 1; i <= n; i++) {
        CircularEnQueue(&Q, i);
    }
    
    int result = 0;
    while (!CircularQueueEmpty(Q)) {
        // 数k-1个人，让他们重新排队
        for (int i = 0; i < k - 1; i++) {
            int person;
            CircularDeQueue(&Q, &person);
            CircularEnQueue(&Q, person);
        }
        
        // 第k个人出队（被淘汰）
        CircularDeQueue(&Q, &result);
    }
    
    return result;
}

/**
 * 滑动窗口最大值
 */
void SlidingWindowMaximum(int *nums, int numsSize, int k, int *result) {
    Deque D;
    InitDeque(&D);
    
    for (int i = 0; i < numsSize; i++) {
        // 移除超出窗口范围的元素
        while (!DequeEmpty(D)) {
            int front;
            GetFront(D, &front);
            if (front <= i - k) {
                PopFront(&D, &front);
            } else {
                break;
            }
        }
        
        // 维护递减队列
        while (!DequeEmpty(D)) {
            int rear;
            GetRear(D, &rear);
            if (nums[rear] <= nums[i]) {
                PopRear(&D, &rear);
            } else {
                break;
            }
        }
        
        // 当前元素下标入队
        PushRear(&D, i);
        
        // 窗口形成后，记录最大值
        if (i >= k - 1) {
            int maxIndex;
            GetFront(D, &maxIndex);
            result[i - k + 1] = nums[maxIndex];
        }
    }
}

/**
 * 任务队列初始化
 */
void InitTaskQueue(TaskQueue *Q) {
    Q->front = Q->rear = 0;
} 