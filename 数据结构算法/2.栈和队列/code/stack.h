/* stack.h - 栈（线性表的受限操作）
 *   栈和队列专题
 * create by: zw.duan
 * 
 *  
 * 1. 栈的基本概念：LIFO（后进先出）原理
 * 2. 顺序栈 vs 链栈的实现差异和优缺点
 * 3. 共享栈的设计思想和空间利用
 * 4. 栈的经典应用：括号匹配、表达式求值、递归转非递归
 * 5. 栈在算法设计中的应用：单调栈、最小栈等
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// 栈的最大容量
#define MAXSIZE 100

// ============= 顺序栈相关定义 =============

/**
 *  顺序栈结构定义
 * 
 *   顺序栈存储结构图示：
 * 
 * 栈的逻辑结构（LIFO - Last In First Out）：
 * ┌─────────────────────────────────────┐
 * │  栈顶 ← 只能在这一端进行插入和删除    │
 * │   ↑                                │
 * │   │   a₄  ← 最后进入，最先出去      │
 * │   │   a₃                           │
 * │   │   a₂                           │
 * │   │   a₁  ← 最先进入，最后出去      │
 * │  栈底                              │
 * └─────────────────────────────────────┘
 * 
 * 顺序栈的物理存储结构：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₁  │ a₂  │ a₃  │ a₄  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6    (数组下标)
 *                     ↑                      
 *                   top=3                    
 * 
 *  核心特点：
 * - LIFO原理：后进先出，只能在栈顶操作
 * - top指针：指向栈顶元素的数组下标
 * - 空栈条件：top = -1
 * - 满栈条件：top = MAXSIZE-1
 * - 基本操作：Push(入栈)、Pop(出栈)、GetTop(取栈顶)
 * 
 *  适用场景：
 *  需要LIFO特性的场合
 *  递归算法的非递归实现
 *  表达式求值和语法分析
 *  函数调用栈的模拟
 *  需要随机访问元素
 *  频繁查找中间元素
 * 
 *  
 * - 理解top指针的两种定义方式
 * - 掌握栈满和栈空的判断条件
 * - 熟练掌握入栈和出栈的操作步骤
 * - 理解栈溢出的概念和处理
 */
typedef struct {
    int data[MAXSIZE];  // 存储栈中元素的数组
    int top;            // 栈顶指针，指向栈顶元素在数组中的下标
} SqStack;

// 顺序栈基本操作函数声明
void InitSqStack(SqStack *S);
bool SqStackEmpty(SqStack S);
bool SqStackFull(SqStack S);
bool SqPush(SqStack *S, int e);
bool SqPop(SqStack *S, int *e);
bool SqGetTop(SqStack S, int *e);
void DestroySqStack(SqStack *S);
void PrintSqStack(SqStack S);
int SqStackLength(SqStack S);

// ============= 链栈相关定义 =============

/**
 * 🔗 链栈节点定义
 * 
 *   链栈存储结构图示：
 * 
 * 链栈的逻辑和物理结构：
 * 
 * 栈顶指针S
 *     ↓
 * ┌────┬────┐    ┌────┬────┐    ┌────┬────┐    ┌────┬────┐
 * │ a₄ │ ●──┼──→ │ a₃ │ ●──┼──→ │ a₂ │ ●──┼──→ │ a₁ │NULL│
 * └────┴────┘    └────┴────┘    └────┴────┘    └────┴────┘
 *   栈顶元素        第2个元素       第3个元素       栈底元素
 * 
 *  核心特点：
 * - 采用链式存储，动态分配内存
 * - 栈顶指针S指向栈顶节点
 * - 空栈条件：S = NULL
 * - 无栈满限制（除非内存耗尽）
 * - 入栈：头插法，O(1)时间复杂度
 * - 出栈：删除头节点，O(1)时间复杂度
 * 
 *  优势：
 * - 动态分配，无固定大小限制
 * - 空间利用率高，按需分配
 * - 不会出现栈满的情况
 * 
 *  劣势：
 * - 需要额外的指针空间
 * - 内存分配和释放的开销
 * - 不支持随机访问
 * 
 *  
 * - 理解链栈的头插法实现
 * - 掌握链栈与顺序栈的对比
 * - 注意内存管理和指针操作
 * - 理解链栈无栈满限制的原因
 */
typedef struct StackNode {
    int data;                    // 数据域
    struct StackNode *next;      // 指针域，指向下一个节点
} StackNode, *LinkStack;

// 链栈基本操作函数声明
void InitLinkStack(LinkStack *S);
bool LinkStackEmpty(LinkStack S);
bool LinkPush(LinkStack *S, int e);
bool LinkPop(LinkStack *S, int *e);
bool LinkGetTop(LinkStack S, int *e);
void DestroyLinkStack(LinkStack *S);
void PrintLinkStack(LinkStack S);
int LinkStackLength(LinkStack S);

// ============= 共享栈（ 重点）=============

/**
 * 🤝 共享栈结构定义
 * 
 *   共享栈存储结构图示：
 * 
 * 两个栈共享一个数组空间，从两端向中间增长：
 * 
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₁  │ a₂  │ a₃  │     │     │     │ b₃  │ b₂  │ b₁  │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6     7     8     9
 *         ↑                                   ↑
 *      top0=2                              top1=8
 * 
 * 栈0：从左端开始，向右增长 (top0 = -1 表示空)
 * 栈1：从右端开始，向左增长 (top1 = MAXSIZE 表示空)
 * 
 *  设计思想：
 * - 充分利用数组空间，避免浪费
 * - 两个栈的增长方向相反
 * - 只有当 top0+1 = top1 时才算栈满
 * - 适用于两个栈的大小变化相反的情况
 * 
 *   空间利用率分析：
 * - 单独两个栈：最多利用50%空间
 * - 共享栈：理论上可以100%利用空间
 * - 实际效果取决于两个栈的使用模式
 * 
 *  
 * - 理解共享栈的设计思想
 * - 掌握栈满和栈空的判断条件
 * - 理解空间利用率的提升原理
 * - 能够分析共享栈的适用场景
 */
typedef struct {
    int data[MAXSIZE];
    int top0;  // 栈0的栈顶指针，初始值为-1
    int top1;  // 栈1的栈顶指针，初始值为MAXSIZE
} SharedStack;

// 共享栈操作
void InitSharedStack(SharedStack *S);
bool SharedStackEmpty(SharedStack S, int stackNum);
bool SharedStackFull(SharedStack S);
bool SharedPush(SharedStack *S, int stackNum, int e);
bool SharedPop(SharedStack *S, int stackNum, int *e);
bool SharedGetTop(SharedStack S, int stackNum, int *e);
void PrintSharedStack(SharedStack S);

// ============= 栈的应用算法（ 重点）=============

/*  栈的经典应用
 * 这些应用是 的高频考点，体现了栈LIFO特性的巧妙运用
 */

// 括号匹配检查
bool BracketCheck(const char *str);

/*   表达式求值相关算法
 * 栈在表达式处理中的核心应用
 */

// 获取运算符优先级
int GetPriority(char op);
// 中缀表达式转后缀表达式
bool InfixToPostfix(const char *infix, char *postfix);
// 计算后缀表达式的值
int EvaluatePostfix(const char *postfix);
// 直接计算中缀表达式的值
int EvaluateInfix(const char *infix);

/*  进制转换
 * 利用栈的LIFO特性实现数制转换
 */
void DecimalToBase(int decimal, int base, char *result);

/*  递归转非递归
 * 用栈模拟系统调用栈
 */
long long FactorialNonRecursive(int n);

// ============= 栈的高级应用（ 算法设计题）=============

/*  用两个栈实现队列
 * 经典的数据结构设计题
 */
typedef struct {
    SqStack s1;  // 输入栈
    SqStack s2;  // 输出栈
} StackQueue;

void InitStackQueue(StackQueue *Q);
bool StackQueueEmpty(StackQueue Q);
bool StackEnQueue(StackQueue *Q, int e);
bool StackDeQueue(StackQueue *Q, int *e);
bool StackGetHead(StackQueue Q, int *e);

/*   最小栈设计
 * 要求在O(1)时间内获取栈中最小元素
 */
typedef struct {
    SqStack data;    // 数据栈
    SqStack min;     // 最小值栈
} MinStack;

void InitMinStack(MinStack *S);
bool MinStackEmpty(MinStack S);
bool MinPush(MinStack *S, int e);
bool MinPop(MinStack *S, int *e);
int GetMin(MinStack S);

/*  单调栈应用
 * 解决"下一个更大元素"类问题
 */
void NextGreaterElement(int *nums, int size, int *result);

/*  栈的排序
 * 只使用一个额外栈对栈进行排序
 */
void StackSort(SqStack *S);

/* 🔤 有效的括号序列生成
 * 生成所有有效的括号组合
 */
void GenerateParenthesis(int n, char **result, int *returnSize);

#endif // STACK_H 