/* queue.h - 队列（线性表的受限操作）
 * 栈和队列专题
 * create by: zw.duan
 * 
 *  
 * 1. 队列的基本概念：FIFO（先进先出）原理
 * 2. 顺序队列 vs 循环队列 vs 链式队列的实现差异
 * 3. 循环队列的关键：如何判断队空和队满
 * 4. 双端队列的设计思想和操作特点
 * 5. 队列的经典应用：BFS、层次遍历、任务调度等
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 队列的最大容量
#define MAXSIZE 100

// ============= 顺序队列相关定义 =============

/**
 *  顺序队列结构定义
 * 
 *   顺序队列存储结构图示：
 * 
 * 队列的逻辑结构（FIFO - First In First Out）：
 * ┌─────────────────────────────────────────────────────┐
 * │  队尾 ← 插入(入队)    队列中间    删除(出队) → 队头  │
 * │   ↑                                           ↑    │
 * │   │   a₄ ← 最后进入   a₃   a₂   a₁ ← 最先出去  │    │
 * │  rear                                      front   │
 * └─────────────────────────────────────────────────────┘
 * 
 * 顺序队列的物理存储结构：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │     │ a₁  │ a₂  │ a₃  │ a₄  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8
 *               ↑                 ↑
 *            front=2           rear=6
 * 
 *  核心特点：
 * - FIFO原理：先进先出，队头删除，队尾插入
 * - front指针：指向队头元素
 * - rear指针：指向队尾元素的下一个位置
 * - 空队条件：front = rear
 * - 队满条件：rear = MAXSIZE（但会造成空间浪费）
 * 
 *  顺序队列的问题：
 * - "假溢出"现象：rear到达数组末尾但前面有空位
 * - 空间利用率低，无法重复使用已出队的空间
 * - 解决方案：使用循环队列
 * 
 *  
 * - 理解顺序队列的假溢出问题
 * - 掌握front和rear指针的含义
 * - 理解为什么需要循环队列
 */
typedef struct {
    int data[MAXSIZE];  // 存储队列元素的数组
    int front;          // 队头指针，指向队头元素
    int rear;           // 队尾指针，指向队尾元素的下一个位置
} SqQueue;

// 顺序队列基本操作函数声明
void InitSqQueue(SqQueue *Q);
bool SqQueueEmpty(SqQueue Q);
bool SqQueueFull(SqQueue Q);
bool SqEnQueue(SqQueue *Q, int e);
bool SqDeQueue(SqQueue *Q, int *e);
bool SqGetHead(SqQueue Q, int *e);
void DestroySqQueue(SqQueue *Q);
void PrintSqQueue(SqQueue Q);
int SqQueueLength(SqQueue Q);

// ============= 循环队列相关定义（ 重点）=============

/**
 *  循环队列结构定义（牺牲一个单元法）
 * 
 *   循环队列存储结构图示：
 * 
 * 循环队列的逻辑结构（环形）：
 *           rear
 *             ↓
 *         ┌─────┐
 *      ┌─→│  5  │←─┐
 *   ┌─→│  └─────┘  │←─┐
 * ┌─│──┴─┐       ┌─┴──│─┐
 * │  4   │       │  6  │
 * └─┬──┬─┘       └─┬──┬─┘
 *   │↓ │           │ ↑│
 *   │3 │           │ 7│
 * ┌─┴──┴─┐       ┌─┴──┴─┐
 * │  2   │       │  8   │
 * └─┬──┬─┘       └─┬──┬─┘
 *   │↓ │           │ ↑│
 *   └─→│  └─────┘  │←─┘
 *      └─→│  1  │←─┘
 *         └─────┘
 *           ↑
 *         front
 * 
 * 循环队列的物理存储结构：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₆  │ a₇  │     │ a₁  │ a₂  │ a₃  │ a₄  │ a₅  │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *               ↑                             ↑
 *            rear=2                       front=7
 * 
 *  核心特点：
 * - 逻辑上将数组首尾相连，形成环形结构
 * - 指针移动：(index + 1) % MAXSIZE
 * - 牺牲一个单元来区分队空和队满：
 *   - 队空条件：front = rear
 *   - 队满条件：(rear + 1) % MAXSIZE = front
 * - 队列长度：(rear - front + MAXSIZE) % MAXSIZE
 * 
 *  优势：
 * - 解决了顺序队列的假溢出问题
 * - 充分利用数组空间
 * - 操作时间复杂度仍为O(1)
 * 
 *  
 * - 掌握循环队列的队空和队满判断
 * - 理解取模运算在指针移动中的作用
 * - 熟练计算循环队列的长度
 * - 理解"牺牲一个单元"的设计思想
 */
typedef struct {
    int data[MAXSIZE];
    int front, rear;
} CircularQueue;

// 循环队列基本操作函数声明
void InitCircularQueue(CircularQueue *Q);
bool CircularQueueEmpty(CircularQueue Q);
bool CircularQueueFull(CircularQueue Q);
bool CircularEnQueue(CircularQueue *Q, int e);
bool CircularDeQueue(CircularQueue *Q, int *e);
bool CircularGetHead(CircularQueue Q, int *e);
void DestroyCircularQueue(CircularQueue *Q);
void PrintCircularQueue(CircularQueue Q);
int CircularQueueLength(CircularQueue Q);

// 循环队列扩展操作（ 算法设计题）
bool GetKthElement(CircularQueue Q, int k, int *e);
bool IsMoreThanHalf(CircularQueue Q);

// ============= 计数器法循环队列 =============

/**
 *   带计数器的循环队列
 * 
 *  设计思想：
 * - 增加count字段记录元素个数
 * - 队空条件：count = 0
 * - 队满条件：count = MAXSIZE
 * - 不需要牺牲一个单元
 * - 可以充分利用所有数组空间
 * 
 *  优势：
 * - 空间利用率100%
 * - 队空队满判断更直观
 * - 长度计算更简单
 * 
 *  劣势：
 * - 需要额外的计数器空间
 * - 每次操作都要维护计数器
 * 
 *  
 * - 理解计数器法的设计思想
 * - 对比牺牲一个单元法的优劣
 * - 掌握不同实现方式的选择
 */
typedef struct {
    int data[MAXSIZE];
    int front, rear;
    int count;  // 元素个数计数器
} CircularQueueWithCount;

void InitCountQueue(CircularQueueWithCount *Q);
bool CountQueueEmpty(CircularQueueWithCount Q);
bool CountQueueFull(CircularQueueWithCount Q);
bool CountEnQueue(CircularQueueWithCount *Q, int e);
bool CountDeQueue(CircularQueueWithCount *Q, int *e);

// ============= 双端队列（ 重点）=============

/**
 *  双端队列结构定义
 * 
 *   双端队列存储结构图示：
 * 
 * 双端队列的逻辑结构：
 * ┌─────────────────────────────────────────────────────┐
 * │  ← 可以插入/删除    队列中间    可以插入/删除 →      │
 * │   ↑                                           ↑    │
 * │   │   a₄          a₃   a₂          a₁         │    │
 * │  rear                                      front   │
 * └─────────────────────────────────────────────────────┘
 * 
 * 双端队列的操作：
 * - PushFront: 队头插入
 * - PushRear:  队尾插入  
 * - PopFront:  队头删除
 * - PopRear:   队尾删除
 * 
 *  核心特点：
 * - 两端都可以进行插入和删除操作
 * - 是栈和队列的推广
 * - 可以实现栈的功能（只在一端操作）
 * - 可以实现队列的功能（一端插入，另一端删除）
 * 
 *   操作复杂度：
 * - 所有基本操作都是O(1)时间复杂度
 * - 空间复杂度O(n)
 * 
 *  
 * - 理解双端队列的操作特点
 * - 掌握双端队列实现栈和队列的方法
 * - 理解双端队列在算法中的应用
 */
typedef struct {
    int data[MAXSIZE];
    int front, rear;
} Deque;

// 双端队列操作
void InitDeque(Deque *D);
bool DequeEmpty(Deque D);
bool DequeFull(Deque D);
bool PushFront(Deque *D, int e);
bool PushRear(Deque *D, int e);
bool PopFront(Deque *D, int *e);
bool PopRear(Deque *D, int *e);
bool GetFront(Deque D, int *e);
bool GetRear(Deque D, int *e);
void PrintDeque(Deque D);

// ============= 链式队列相关定义 =============

/**
 * 🔗 队列节点定义
 */
typedef struct QueueNode {
    int data;                    // 数据域
    struct QueueNode *next;      // 指针域
} QueueNode;

/**
 * 🔗 链式队列定义
 * 
 *   链式队列存储结构图示：
 * 
 * 链式队列的逻辑和物理结构：
 * 
 * front指针                                    rear指针
 *     ↓                                           ↓
 * ┌────┬────┐    ┌────┬────┐    ┌────┬────┐    ┌────┬────┐
 * │ a₁ │ ●──┼──→ │ a₂ │ ●──┼──→ │ a₃ │ ●──┼──→ │ a₄ │NULL│
 * └────┴────┘    └────┴────┘    └────┴────┘    └────┴────┘
 *   队头元素        第2个元素       第3个元素       队尾元素
 * 
 *  核心特点：
 * - 采用链式存储，动态分配内存
 * - front指针指向队头节点
 * - rear指针指向队尾节点
 * - 空队条件：front = rear = NULL
 * - 无队满限制（除非内存耗尽）
 * - 入队：在rear后插入新节点
 * - 出队：删除front指向的节点
 * 
 *  优势：
 * - 动态分配，无固定大小限制
 * - 不存在假溢出问题
 * - 空间利用率高
 * 
 *  劣势：
 * - 需要额外的指针空间
 * - 内存分配和释放的开销
 * 
 *  
 * - 理解链式队列的指针操作
 * - 掌握入队和出队的实现细节
 * - 注意空队列的特殊处理
 * - 对比顺序队列的优劣
 */
typedef struct {
    QueueNode *front;   // 队头指针
    QueueNode *rear;    // 队尾指针
} LinkQueue;

// 链式队列基本操作函数声明
void InitLinkQueue(LinkQueue *Q);
bool LinkQueueEmpty(LinkQueue Q);
bool LinkEnQueue(LinkQueue *Q, int e);
bool LinkDeQueue(LinkQueue *Q, int *e);
bool LinkGetHead(LinkQueue Q, int *e);
void DestroyLinkQueue(LinkQueue *Q);
void PrintLinkQueue(LinkQueue Q);
int LinkQueueLength(LinkQueue Q);

// ============= 队列的应用算法（ 重点）=============

/*  队列的经典应用
 * 这些应用体现了队列FIFO特性在算法中的重要作用
 */

// 二叉树层次遍历（用于演示）
typedef struct TreeNode {
    int data;
    struct TreeNode *left, *right;
} TreeNode;

void LevelOrder(TreeNode *root);
TreeNode* CreateSampleTree(void);
void DestroyTree(TreeNode *root);

/*   图的广度优先搜索
 * 队列在图算法中的核心应用
 */
#define MAXVERTEX 20
typedef struct {
    int vertices;
    int edges[MAXVERTEX][MAXVERTEX];
    bool visited[MAXVERTEX];
} Graph;

void BFS(Graph *G, int start);
Graph* CreateSampleGraph(void);

/*  约瑟夫环问题
 * 经典的队列应用问题
 */
int JosephusProblem(int n, int k);

// ============= 队列的高级应用（ 算法设计题）=============

/*  用两个队列实现栈
 * 经典的数据结构设计题
 */
typedef struct {
    CircularQueue q1;  // 主队列
    CircularQueue q2;  // 辅助队列
} QueueStack;

void InitQueueStack(QueueStack *S);
bool QueueStackEmpty(QueueStack S);
bool QueuePush(QueueStack *S, int e);
bool QueuePop(QueueStack *S, int *e);
bool QueueStackTop(QueueStack S, int *e);

/*  队列逆转
 * 利用栈的特性逆转队列
 */
void ReverseQueue(CircularQueue *Q);

/*   滑动窗口最大值
 * 双端队列的经典应用
 */
void SlidingWindowMaximum(int *nums, int size, int k, int *result);

/*  优先队列（简单实现）
 * 按优先级处理元素的队列
 */
typedef struct {
    int data;
    int priority;
} PriorityElement;

typedef struct {
    PriorityElement elements[MAXSIZE];
    int size;
} PriorityQueue;

void InitPriorityQueue(PriorityQueue *PQ);
bool PriorityQueueEmpty(PriorityQueue PQ);
bool PriorityQueueFull(PriorityQueue PQ);
bool PriorityInsert(PriorityQueue *PQ, int data, int priority);
bool PriorityDeleteMax(PriorityQueue *PQ, int *data);
void PrintPriorityQueue(PriorityQueue PQ);

/* 📋 任务调度队列
 * 实际应用中的队列使用示例
 */
typedef struct {
    int taskId;
    int priority;
    char description[50];
} Task;

typedef struct {
    Task data[MAXSIZE];
    int front, rear;
} TaskQueue;

void InitTaskQueue(TaskQueue *Q);
bool AddTask(TaskQueue *Q, Task task);
bool ExecuteTask(TaskQueue *Q, Task *task);
void PrintTaskQueue(TaskQueue Q);

#endif // QUEUE_H 