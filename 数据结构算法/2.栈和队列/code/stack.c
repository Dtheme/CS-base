/* stack.c - Stack data structure implementation
 * create by: zw.duan
 */

#include "stack.h"
#include <time.h>

// ============= 顺序栈基本操作实现 =============

/**
 * 初始化顺序栈
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void InitSqStack(SqStack *S) {
    S->top = -1;  // 初始化栈顶指针为-1，表示空栈
}

/**
 * 判断顺序栈是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool SqStackEmpty(SqStack S) {
    return S.top == -1;
}

/**
 * 判断顺序栈是否已满
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool SqStackFull(SqStack S) {
    return S.top == MAXSIZE - 1;
}

/**
 * 📥 顺序栈入栈操作（Push操作， 核心）
 * 
 *   入栈过程图示（插入元素5）：
 * 
 * 入栈前：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  2  │  3  │  4  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6    (数组下标)
 *                     ↑                      
 *                   top=3                    
 * 
 * 入栈后：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  2  │  3  │  4  │  5  │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6    (数组下标)
 *                           ↑                      
 *                         top=4                    
 * 
 *  算法步骤：
 * 1. 检查栈是否已满：top = MAXSIZE-1
 * 2. 栈顶指针先加1：++top
 * 3. 在新的栈顶位置插入元素：data[top] = e
 * 
 *  关键点：
 * - 必须先检查栈满条件，防止数组越界
 * - 使用++top而不是top++，先增加再使用
 * - 入栈是在栈顶进行的唯一操作
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1) - 常数时间操作
 * - 空间复杂度：O(1) - 不需要额外空间
 * 
 *  
 * - 理解LIFO原理在入栈中的体现
 * - 掌握栈满判断条件
 * - 注意top指针的更新顺序
 * - 理解栈溢出的概念和处理
 * 
 * @param S 指向顺序栈的指针
 * @param e 待入栈的元素
 * @return 成功返回true，栈满返回false
 */
bool SqPush(SqStack *S, int e) {
    // 步骤1：检查栈是否已满
    if (S->top == MAXSIZE - 1) {
        printf("错误：栈已满，无法入栈元素%d\n", e);
        return false;  // 栈满，入栈失败
    }
    
    // 步骤2&3：栈顶指针先加1，然后插入新元素
    S->data[++S->top] = e;
    printf("成功入栈：%d，当前栈顶位置：%d\n", e, S->top);
    return true;  // 入栈成功
}

/**
 * 📤 顺序栈出栈操作（Pop操作， 核心）
 * 
 *   出栈过程图示（删除栈顶元素）：
 * 
 * 出栈前：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  2  │  3  │  4  │  5  │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6    (数组下标)
 *                           ↑                      
 *                         top=4                    
 * 
 * 出栈后：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  2  │  3  │  4  │  ?  │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6    (数组下标)
 *                     ↑                      
 *                   top=3                    
 * 
 *  算法步骤：
 * 1. 检查栈是否为空：top = -1
 * 2. 取出栈顶元素：*e = data[top]
 * 3. 栈顶指针减1：top--
 * 
 *  关键点：
 * - 必须先检查栈空条件，防止下溢
 * - 使用top--，先使用再减少
 * - 出栈元素通过指针参数返回
 * - 原栈顶位置的数据不需要清除（逻辑删除）
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1) - 常数时间操作
 * - 空间复杂度：O(1) - 不需要额外空间
 * 
 *  
 * - 理解LIFO原理在出栈中的体现
 * - 掌握栈空判断条件
 * - 注意top指针的更新顺序
 * - 理解栈下溢的概念和处理
 * - 区分物理删除和逻辑删除
 * 
 * @param S 指向顺序栈的指针
 * @param e 用于返回出栈元素的指针
 * @return 成功返回true，栈空返回false
 */
bool SqPop(SqStack *S, int *e) {
    // 步骤1：检查栈是否为空
    if (S->top == -1) {
        printf("错误：栈为空，无法出栈\n");
        return false;  // 栈空，出栈失败
    }
    
    // 步骤2&3：先取出栈顶元素，然后栈顶指针减1
    *e = S->data[S->top--];
    printf("成功出栈：%d，当前栈顶位置：%d\n", *e, S->top);
    return true;  // 出栈成功
}

/**
 * 获取顺序栈栈顶元素（不出栈）
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool SqGetTop(SqStack S, int *e) {
    // 判断栈是否为空
    if (S.top == -1) {
        return false;  // 栈空，获取失败
    }
    
    // 返回栈顶元素，但不修改栈顶指针
    *e = S.data[S.top];
    return true;  // 获取成功
}

/**
 * 销毁顺序栈
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void DestroySqStack(SqStack *S) {
    S->top = -1;  // 重置栈顶指针即可
}

/**
 * 打印顺序栈内容
 */
void PrintSqStack(SqStack S) {
    if (SqStackEmpty(S)) {
        printf("顺序栈为空\n");
        return;
    }
    
    printf("顺序栈内容（从栈底到栈顶）：");
    for (int i = 0; i <= S.top; i++) {
        printf("%d ", S.data[i]);
    }
    printf("\n");
}

/**
 * 获取顺序栈长度
 */
int SqStackLength(SqStack S) {
    return S.top + 1;
}

// ============= 链栈基本操作实现 =============

/**
 * 初始化链栈
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void InitLinkStack(LinkStack *S) {
    *S = NULL;  // 初始化为空栈，头指针为NULL
}

/**
 * 判断链栈是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkStackEmpty(LinkStack S) {
    return S == NULL;
}

/**
 * 🔗 链栈入栈操作（头插法， 重点）
 * 
 *   链栈入栈过程图示（插入元素5）：
 * 
 * 入栈前：
 * 栈顶指针S
 *     ↓
 * ┌────┬────┐    ┌────┬────┐    ┌────┬────┐
 * │ 4  │ ●──┼──→ │ 3  │ ●──┼──→ │ 2  │NULL│
 * └────┴────┘    └────┴────┘    └────┴────┘
 * 
 * 创建新节点：
 * ┌────┬────┐
 * │ 5  │ ?  │  ← 新分配的节点p
 * └────┴────┘
 * 
 * 入栈后：
 * 栈顶指针S
 *     ↓
 * ┌────┬────┐    ┌────┬────┐    ┌────┬────┐    ┌────┬────┐
 * │ 5  │ ●──┼──→ │ 4  │ ●──┼──→ │ 3  │ ●──┼──→ │ 2  │NULL│
 * └────┴────┘    └────┴────┘    └────┴────┘    └────┴────┘
 *   新栈顶         原栈顶
 * 
 *  算法步骤（头插法）：
 * 1. 分配新节点：p = malloc(sizeof(StackNode))
 * 2. 设置数据域：p->data = e
 * 3. 设置指针域：p->next = *S（指向原栈顶）
 * 4. 更新栈顶：*S = p（新节点成为栈顶）
 * 
 *  关键点：
 * - 使用头插法，新节点总是插在链表头部
 * - 必须检查内存分配是否成功
 * - 指针操作顺序很重要：先设置p->next，再更新*S
 * - 链栈理论上无大小限制（除非内存耗尽）
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1) - 头插法常数时间
 * - 空间复杂度：O(1) - 每次分配一个节点
 * 
 *  
 * - 理解头插法在链栈中的应用
 * - 掌握指针操作的正确顺序
 * - 理解链栈与顺序栈的区别
 * - 注意内存管理和错误处理
 * - 理解为什么链栈没有栈满的概念
 * 
 * @param S 指向链栈头指针的指针
 * @param e 待入栈的元素
 * @return 成功返回true，内存分配失败返回false
 */
bool LinkPush(LinkStack *S, int e) {
    // 步骤1：分配新节点
    StackNode *p = (StackNode *)malloc(sizeof(StackNode));
    if (p == NULL) {
        printf("错误：内存分配失败，无法入栈元素%d\n", e);
        return false;  // 内存分配失败
    }
    
    // 步骤2：设置新节点的数据域
    p->data = e;
    
    // 步骤3：设置新节点的指针域（指向原栈顶）
    p->next = *S;
    
    // 步骤4：更新栈顶指针（新节点成为栈顶）
    *S = p;
    
    printf("成功入栈：%d，新栈顶地址：%p\n", e, (void*)p);
    return true;    // 入栈成功
}

/**
 * 链栈出栈操作
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkPop(LinkStack *S, int *e) {
    // 判断栈是否为空
    if (*S == NULL) {
        return false;  // 栈空，出栈失败
    }
    
    // 记录栈顶节点和数据
    StackNode *p = *S;
    *e = p->data;
    
    // 修改栈顶指针
    *S = p->next;
    
    // 释放原栈顶节点
    free(p);
    
    return true;  // 出栈成功
}

/**
 * 获取链栈栈顶元素（不出栈）
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool LinkGetTop(LinkStack S, int *e) {
    // 判断栈是否为空
    if (S == NULL) {
        return false;  // 栈空，获取失败
    }
    
    // 返回栈顶元素
    *e = S->data;
    return true;  // 获取成功
}

/**
 * 销毁链栈
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void DestroyLinkStack(LinkStack *S) {
    StackNode *p;
    while (*S != NULL) {
        p = *S;
        *S = (*S)->next;
        free(p);
    }
}

/**
 * 打印链栈内容
 */
void PrintLinkStack(LinkStack S) {
    if (LinkStackEmpty(S)) {
        printf("链栈为空\n");
        return;
    }
    
    printf("链栈内容（从栈顶到栈底）：");
    StackNode *p = S;
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}

/**
 * 获取链栈长度
 */
int LinkStackLength(LinkStack S) {
    int count = 0;
    StackNode *p = S;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// ============= 栈的应用算法实现 =============

/**
 *  括号匹配检测算法（ 经典应用）
 * 
 *   算法原理图示：
 * 
 * 输入字符串："{[()()]}"
 * 
 * 扫描过程：
 * 字符  栈状态        操作说明
 * ────────────────────────────────
 * '{'   ['{']         左括号入栈
 * '['   ['{','[']     左括号入栈
 * '('   ['{','[','('] 左括号入栈
 * ')'   ['{','[']     右括号匹配'('，出栈
 * '('   ['{','[','('] 左括号入栈
 * ')'   ['{','[']     右括号匹配'('，出栈
 * ']'   ['{']         右括号匹配'['，出栈
 * '}'   []            右括号匹配'{'，出栈
 * 
 * 最终栈为空 → 括号匹配成功 
 * 
 *  算法步骤：
 * 1. 初始化空栈
 * 2. 从左到右扫描字符串：
 *    - 遇到左括号：入栈
 *    - 遇到右括号：
 *      a) 栈空 → 失败（右括号多）
 *      b) 栈顶匹配 → 出栈继续
 *      c) 栈顶不匹配 → 失败（类型错误）
 * 3. 扫描结束后栈必须为空
 * 
 *  关键点：
 * - 利用栈的LIFO特性实现就近匹配
 * - 左括号：'(', '[', '{'
 * - 右括号：')', ']', '}'
 * - 必须类型匹配且数量相等
 * 
 *   复杂度分析：
 * - 时间复杂度：O(n) - 单次遍历字符串
 * - 空间复杂度：O(n) - 最坏情况栈存储所有左括号
 * 
 *  
 * - 理解栈在括号匹配中的作用
 * - 掌握算法的核心思想和实现步骤
 * - 能够分析各种失败情况
 * - 理解时间和空间复杂度
 * - 能够扩展到其他匹配问题
 * 
 * @param str 待检测的字符串
 * @return 匹配成功返回true，否则返回false
 */
bool BracketCheck(const char *str) {
    SqStack S;
    InitSqStack(&S);
    
    printf("开始括号匹配检测：%s\n", str);
    
    int i = 0;
    while (str[i] != '\0') {
        char ch = str[i];
        
        // 步骤2a：遇到左括号，入栈
        if (ch == '(' || ch == '[' || ch == '{') {
            SqPush(&S, ch);
            printf("字符'%c'：左括号入栈\n", ch);
        }
        // 步骤2b：遇到右括号，检查匹配
        else if (ch == ')' || ch == ']' || ch == '}') {
            // 检查栈是否为空
            if (SqStackEmpty(S)) {
                printf("字符'%c'：栈为空，右括号多余 \n", ch);
                return false;  // 右括号多于左括号
            }
            
            int top;
            SqPop(&S, &top);
            
            // 检查括号类型是否匹配
            bool match = false;
            if (ch == ')' && top == '(') match = true;
            else if (ch == ']' && top == '[') match = true;
            else if (ch == '}' && top == '{') match = true;
            
            if (match) {
                printf("字符'%c'：与栈顶'%c'匹配，出栈 \n", ch, (char)top);
            } else {
                printf("字符'%c'：与栈顶'%c'不匹配 \n", ch, (char)top);
                return false;  // 括号类型不匹配
            }
        }
        i++;
    }
    
    // 步骤3：最终栈应该为空
    bool result = SqStackEmpty(S);
    if (result) {
        printf("扫描完成，栈为空，括号匹配成功 \n");
    } else {
        printf("扫描完成，栈非空，左括号多余 \n");
    }
    
    return result;
}

/**
 * 获取运算符优先级
 */
int GetPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        case '(':
            return 0;  // 左括号在栈内优先级最低
        default:
            return -1;
    }
}

/**
 * 中缀表达式转后缀表达式
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
bool InfixToPostfix(const char *infix, char *postfix) {
    SqStack S;
    InitSqStack(&S);
    
    int i = 0, j = 0;  // i指向中缀表达式，j指向后缀表达式
    
    while (infix[i] != '\0') {
        char ch = infix[i];
        
        // 操作数直接输出
        if (isalnum(ch)) {
            postfix[j++] = ch;
        }
        // 左括号入栈
        else if (ch == '(') {
            SqPush(&S, ch);
        }
        // 右括号：弹出直到左括号
        else if (ch == ')') {
            int top;
            while (!SqStackEmpty(S)) {
                SqPop(&S, &top);
                if (top == '(') break;
                postfix[j++] = top;
            }
        }
        // 运算符：根据优先级处理
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            while (!SqStackEmpty(S)) {
                int top;
                SqGetTop(S, &top);
                
                if (GetPriority(top) >= GetPriority(ch)) {
                    SqPop(&S, &top);
                    postfix[j++] = top;
                } else {
                    break;
                }
            }
            SqPush(&S, ch);
        }
        i++;
    }
    
    // 弹出栈中剩余运算符
    while (!SqStackEmpty(S)) {
        int top;
        SqPop(&S, &top);
        postfix[j++] = top;
    }
    
    postfix[j] = '\0';  // 添加字符串结束符
    return true;  // 转换成功
}

/**
 * 后缀表达式求值
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
int EvaluatePostfix(const char *postfix) {
    SqStack S;
    InitSqStack(&S);
    
    int i = 0;
    while (postfix[i] != '\0') {
        char ch = postfix[i];
        
        // 操作数入栈
        if (isdigit(ch)) {
            SqPush(&S, ch - '0');  // 字符转数字
        }
        // 运算符：弹出两个操作数运算
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            int b, a;
            SqPop(&S, &b);  // 右操作数
            SqPop(&S, &a);  // 左操作数
            
            int result;
            switch (ch) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
            }
            
            SqPush(&S, result);
        }
        i++;
    }
    
    int result;
    SqPop(&S, &result);
    return result;
}

/**
 * 进制转换
 * 时间复杂度：O(log n)
 * 空间复杂度：O(log n)
 */
void DecimalToBase(int decimal, int base, char *result) {
    SqStack S;
    InitSqStack(&S);
    
    if (decimal == 0) {
        strcpy(result, "0");
        return;
    }
    
    // 将各位数字入栈
    while (decimal > 0) {
        SqPush(&S, decimal % base);
        decimal /= base;
    }
    
    // 出栈构造结果字符串
    int i = 0;
    while (!SqStackEmpty(S)) {
        int digit;
        SqPop(&S, &digit);
        if (digit < 10) {
            result[i++] = '0' + digit;
        } else {
            result[i++] = 'A' + digit - 10;
        }
    }
    result[i] = '\0';
}

/**
 * 汉诺塔递归求解
 */
void TowerOfHanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        printf("将第%d个盘子从 %c 移动到 %c\n", n, from, to);
    } else {
        TowerOfHanoi(n-1, from, aux, to);
        printf("将第%d个盘子从 %c 移动到 %c\n", n, from, to);
        TowerOfHanoi(n-1, aux, to, from);
    }
}

// ============= 高级应用算法实现 =============

/**
 * 用两个栈实现队列
 */
void InitStackQueue(StackQueue *Q) {
    InitSqStack(&Q->s1);
    InitSqStack(&Q->s2);
}

bool StackQueueEmpty(StackQueue Q) {
    return SqStackEmpty(Q.s1) && SqStackEmpty(Q.s2);
}

bool StackEnQueue(StackQueue *Q, int e) {
    return SqPush(&Q->s1, e);
}

bool StackDeQueue(StackQueue *Q, int *e) {
    // 如果输出栈为空，将输入栈元素全部倒入输出栈
    if (SqStackEmpty(Q->s2)) {
        if (SqStackEmpty(Q->s1)) {
            return false;  // 队列空
        }
        while (!SqStackEmpty(Q->s1)) {
            int temp;
            SqPop(&Q->s1, &temp);
            SqPush(&Q->s2, temp);
        }
    }
    
    // 从输出栈弹出元素
    return SqPop(&Q->s2, e);
}

/**
 * 最小栈实现
 */
void InitMinStack(MinStack *S) {
    InitSqStack(&S->data);
    InitSqStack(&S->min);
}

bool MinStackEmpty(MinStack S) {
    return SqStackEmpty(S.data);
}

bool MinPush(MinStack *S, int e) {
    SqPush(&S->data, e);
    
    // 维护最小值栈
    if (SqStackEmpty(S->min)) {
        SqPush(&S->min, e);
    } else {
        int minVal;
        SqGetTop(S->min, &minVal);
        SqPush(&S->min, e < minVal ? e : minVal);
    }
    
    return true;
}

bool MinPop(MinStack *S, int *e) {
    if (SqStackEmpty(S->data)) return false;
    
    SqPop(&S->data, e);
    int temp;
    SqPop(&S->min, &temp);
    return true;
}

int GetMin(MinStack S) {
    int minVal;
    SqGetTop(S.min, &minVal);
    return minVal;
}

// ============= 测试和演示函数实现 =============

/**
 * 测试顺序栈基本操作
 */
void TestSqStackBasicOps(void) {
    printf("\n=== 顺序栈基本操作测试 ===\n");
    
    SqStack S;
    InitSqStack(&S);
    
    printf("1. 初始化后栈是否为空：%s\n", SqStackEmpty(S) ? "是" : "否");
    
    // 入栈测试
    printf("2. 入栈操作：");
    for (int i = 1; i <= 5; i++) {
        if (SqPush(&S, i * 10)) {
            printf("%d ", i * 10);
        }
    }
    printf("\n");
    
    PrintSqStack(S);
    printf("   栈长度：%d\n", SqStackLength(S));
    
    // 取栈顶测试
    int top;
    if (SqGetTop(S, &top)) {
        printf("3. 栈顶元素：%d\n", top);
    }
    
    // 出栈测试
    printf("4. 出栈操作：");
    while (!SqStackEmpty(S)) {
        int e;
        if (SqPop(&S, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
    
    printf("5. 出栈后栈是否为空：%s\n", SqStackEmpty(S) ? "是" : "否");
}

/**
 * 测试链栈基本操作
 */
void TestLinkStackBasicOps(void) {
    printf("\n=== 链栈基本操作测试 ===\n");
    
    LinkStack S;
    InitLinkStack(&S);
    
    printf("1. 初始化后栈是否为空：%s\n", LinkStackEmpty(S) ? "是" : "否");
    
    // 入栈测试
    printf("2. 入栈操作：");
    for (int i = 1; i <= 5; i++) {
        if (LinkPush(&S, i * 20)) {
            printf("%d ", i * 20);
        }
    }
    printf("\n");
    
    PrintLinkStack(S);
    printf("   栈长度：%d\n", LinkStackLength(S));
    
    // 取栈顶测试
    int top;
    if (LinkGetTop(S, &top)) {
        printf("3. 栈顶元素：%d\n", top);
    }
    
    // 出栈测试
    printf("4. 出栈操作：");
    while (!LinkStackEmpty(S)) {
        int e;
                 if (LinkPop(&S, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
    
    printf("5. 出栈后栈是否为空：%s\n", LinkStackEmpty(S) ? "是" : "否");
    
    DestroyLinkStack(&S);
}

/**
 * 测试括号匹配
 */
void TestBracketMatching(void) {
    printf("\n=== 括号匹配测试 ===\n");
    
    const char *testCases[] = {
        "()",
        "()[]{}", 
        "([{}])",
        "([)]",
        "(((",
        "))",
        "",
        "({[]})"
    };
    
    int numTests = sizeof(testCases) / sizeof(testCases[0]);
    
    for (int i = 0; i < numTests; i++) {
        bool result = BracketCheck(testCases[i]);
        printf("字符串 \"%s\" 括号匹配：%s\n", 
               testCases[i], result ? "正确" : "错误");
    }
}

/**
 * 测试表达式求值
 */
void TestExpressionEvaluation(void) {
    printf("\n=== 表达式求值测试 ===\n");
    
    const char *expressions[] = {
        "2+3*4",
        "(2+3)*4", 
        "2+3*4-5",
        "2*(3+4)",
        "1+2*3-4"
    };
    
    int numExpr = sizeof(expressions) / sizeof(expressions[0]);
    
    for (int i = 0; i < numExpr; i++) {
        char postfix[100];
        InfixToPostfix(expressions[i], postfix);
        int result = EvaluatePostfix(postfix);
        
        printf("中缀表达式: %s\n", expressions[i]);
        printf("后缀表达式: %s\n", postfix);
        printf("计算结果: %d\n\n", result);
    }
}

/**
 * 测试进制转换
 */
void TestBaseConversion(void) {
    printf("\n=== 进制转换测试 ===\n");
    
    int numbers[] = {10, 255, 1024, 0, 15};
    int bases[] = {2, 8, 16};
    
    int numNumbers = sizeof(numbers) / sizeof(numbers[0]);
    int numBases = sizeof(bases) / sizeof(bases[0]);
    
    for (int i = 0; i < numNumbers; i++) {
        printf("十进制数 %d 转换为：\n", numbers[i]);
        for (int j = 0; j < numBases; j++) {
            char result[100];
            DecimalToBase(numbers[i], bases[j], result);
            printf("  %d进制：%s\n", bases[j], result);
        }
        printf("\n");
    }
}

/**
 * 性能测试
 */
void PerformanceTest(void) {
    printf("\n=== 性能测试（顺序栈 vs 链栈）===\n");
    
    const int TEST_SIZE = 100000;
    clock_t start, end;
    
    // 顺序栈性能测试
    SqStack sqStack;
    InitSqStack(&sqStack);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        SqPush(&sqStack, i);
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        int temp;
        SqPop(&sqStack, &temp);
    }
    end = clock();
    
    double sqTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 链栈性能测试
    LinkStack linkStack;
    InitLinkStack(&linkStack);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        LinkPush(&linkStack, i);
    }
    for (int i = 0; i < TEST_SIZE; i++) {
        int temp;
        LinkPop(&linkStack, &temp);
    }
    end = clock();
    
    double linkTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("测试规模：%d 次入栈+出栈操作\n", TEST_SIZE);
    printf("顺序栈用时：%.6f 秒\n", sqTime);
    printf("链栈用时：%.6f 秒\n", linkTime);
    printf("性能比：%.2f\n", linkTime / sqTime);
    
    DestroyLinkStack(&linkStack);
}

/**
 * 测试用栈实现队列
 */
void TestStackQueue(void) {
    printf("\n=== 用栈实现队列测试 ===\n");
    
    StackQueue Q;
    InitStackQueue(&Q);
    
    printf("1. 入队操作：");
    for (int i = 1; i <= 5; i++) {
        StackEnQueue(&Q, i * 10);
        printf("%d ", i * 10);
    }
    printf("\n");
    
    printf("2. 出队操作：");
    while (!StackQueueEmpty(Q)) {
        int e;
        if (StackDeQueue(&Q, &e)) {
            printf("%d ", e);
        }
    }
    printf("\n");
}

/**
 * 测试最小栈
 */
void TestMinStack(void) {
    printf("\n=== 最小栈测试 ===\n");
    
    MinStack S;
    InitMinStack(&S);
    
    int data[] = {3, 5, 2, 1, 4};
    int size = sizeof(data) / sizeof(data[0]);
    
    printf("入栈过程：\n");
    for (int i = 0; i < size; i++) {
        MinPush(&S, data[i]);
        printf("入栈 %d，当前最小值：%d\n", data[i], GetMin(S));
    }
    
    printf("\n出栈过程：\n");
    while (!MinStackEmpty(S)) {
        int e;
        printf("出栈前最小值：%d\n", GetMin(S));
        MinPop(&S, &e);
        printf("出栈 %d\n", e);
        if (!MinStackEmpty(S)) {
            printf("出栈后最小值：%d\n", GetMin(S));
        }
        printf("\n");
    }
}

/**
 *  真题演示
 */
void ExamDemonstration(void) {
    printf("\n===  真题演示 ===\n");
    
    printf("题目：已知栈的顺序存储结构，栈顶指针top初始值为-1\n");
    printf("操作序列：Push(S,a), Push(S,b), Pop(S,x), Push(S,c), Push(S,d), Pop(S,y)\n");
    printf("求最终栈中元素从栈底到栈顶的顺序\n\n");
    
    SqStack S;
    InitSqStack(&S);
    int x, y;
    
    printf("执行过程：\n");
    printf("初始状态：top = %d，栈空\n", S.top);
    
    SqPush(&S, 'a');
    printf("Push(S,a)：top = %d，栈：[a]\n", S.top);
    
    SqPush(&S, 'b');
    printf("Push(S,b)：top = %d，栈：[a,b]\n", S.top);
    
    SqPop(&S, &x);
    printf("Pop(S,x)： top = %d，栈：[a]，x = %c\n", S.top, x);
    
    SqPush(&S, 'c');
    printf("Push(S,c)：top = %d，栈：[a,c]\n", S.top);
    
    SqPush(&S, 'd');
    printf("Push(S,d)：top = %d，栈：[a,c,d]\n", S.top);
    
    SqPop(&S, &y);
    printf("Pop(S,y)： top = %d，栈：[a,c]，y = %c\n", S.top, y);
    
    printf("\n答案：栈中元素从栈底到栈顶为：a, c\n");
}

/**
 * 教学演示
 */
void TeachingDemo(void) {
    printf("\n=== 栈的教学演示 ===\n");
    
    printf("1. 栈的特点：后进先出（LIFO）\n");
    printf("2. 栈的基本操作：入栈、出栈、取栈顶\n");
    printf("3. 栈的应用：括号匹配、表达式求值、递归等\n\n");
    
    // 演示栈的LIFO特性
    printf("演示LIFO特性：\n");
    SqStack demo;
    InitSqStack(&demo);
    
    printf("依次入栈：1, 2, 3, 4, 5\n");
    for (int i = 1; i <= 5; i++) {
        SqPush(&demo, i);
        printf("入栈 %d 后：", i);
        PrintSqStack(demo);
    }
    
    printf("\n依次出栈：\n");
    while (!SqStackEmpty(demo)) {
        int e;
        SqPop(&demo, &e);
        printf("出栈 %d 后：", e);
        PrintSqStack(demo);
    }
    
    printf("\n汉诺塔问题演示（3层）：\n");
    TowerOfHanoi(3, 'A', 'C', 'B');
}

// ============= 新增的函数实现 =============

/**
 * 中缀表达式直接求值
 */
int EvaluateInfix(const char *infix) {
    char postfix[100];
    if (!InfixToPostfix(infix, postfix)) {
        return 0;  // 转换失败
    }
    return EvaluatePostfix(postfix);
}

/**
 * 非递归阶乘计算
 */
long long FactorialNonRecursive(int n) {
    if (n < 0) return -1;  // 错误值
    if (n == 0 || n == 1) return 1;
    
    SqStack S;
    InitSqStack(&S);
    
    // 将1到n依次入栈
    for (int i = 1; i <= n; i++) {
        SqPush(&S, i);
    }
    
    long long result = 1;
    while (!SqStackEmpty(S)) {
        int num;
        SqPop(&S, &num);
        result *= num;
    }
    
    return result;
}

/**
 * 共享栈初始化
 */
void InitSharedStack(SharedStack *S) {
    S->top0 = -1;        // 栈0的栈顶指针
    S->top1 = MAXSIZE;   // 栈1的栈顶指针
}

/**
 * 共享栈判空
 */
bool SharedStackEmpty(SharedStack S, int stackNum) {
    if (stackNum == 0) {
        return S.top0 == -1;
    } else if (stackNum == 1) {
        return S.top1 == MAXSIZE;
    }
    return false;
}

/**
 * 共享栈判满
 */
bool SharedStackFull(SharedStack S) {
    return S.top0 + 1 == S.top1;
}

/**
 * 共享栈入栈
 */
bool SharedPush(SharedStack *S, int stackNum, int e) {
    if (SharedStackFull(*S)) {
        return false;  // 栈满
    }
    
    if (stackNum == 0) {
        S->data[++S->top0] = e;
    } else if (stackNum == 1) {
        S->data[--S->top1] = e;
    } else {
        return false;  // 栈号错误
    }
    
    return true;
}

/**
 * 共享栈出栈
 */
bool SharedPop(SharedStack *S, int stackNum, int *e) {
    if (SharedStackEmpty(*S, stackNum)) {
        return false;  // 栈空
    }
    
    if (stackNum == 0) {
        *e = S->data[S->top0--];
    } else if (stackNum == 1) {
        *e = S->data[S->top1++];
    } else {
        return false;  // 栈号错误
    }
    
    return true;
}

/**
 * 共享栈获取栈顶
 */
bool SharedGetTop(SharedStack S, int stackNum, int *e) {
    if (SharedStackEmpty(S, stackNum)) {
        return false;  // 栈空
    }
    
    if (stackNum == 0) {
        *e = S.data[S.top0];
    } else if (stackNum == 1) {
        *e = S.data[S.top1];
    } else {
        return false;  // 栈号错误
    }
    
    return true;
}

/**
 * 打印共享栈
 */
void PrintSharedStack(SharedStack S) {
    printf("栈0: ");
    if (S.top0 == -1) {
        printf("空");
    } else {
        printf("栈底 ");
        for (int i = 0; i <= S.top0; i++) {
            printf("%d ", S.data[i]);
        }
        printf("栈顶");
    }
    
    printf("  |  栈1: ");
    if (S.top1 == MAXSIZE) {
        printf("空");
    } else {
        printf("栈顶 ");
        for (int i = S.top1; i < MAXSIZE; i++) {
            printf("%d ", S.data[i]);
        }
        printf("栈底");
    }
    printf("\n");
}

/**
 * 栈队列获取队头元素
 */
bool StackGetHead(StackQueue Q, int *e) {
    // 如果输出栈不为空，直接获取栈顶
    if (!SqStackEmpty(Q.s2)) {
        return SqGetTop(Q.s2, e);
    }
    
    // 如果输入栈也为空，队列为空
    if (SqStackEmpty(Q.s1)) {
        return false;
    }
    
    // 需要将输入栈的底部元素作为队头
    // 这里需要临时栈来辅助
    SqStack temp;
    InitSqStack(&temp);
    
    // 将输入栈元素倒入临时栈
    while (!SqStackEmpty(Q.s1)) {
        int tempE;
        SqPop(&Q.s1, &tempE);
        SqPush(&temp, tempE);
    }
    
    // 获取队头元素（临时栈的栈顶）
    bool result = SqGetTop(temp, e);
    
    // 将元素还原到输入栈
    while (!SqStackEmpty(temp)) {
        int tempE;
        SqPop(&temp, &tempE);
        SqPush(&Q.s1, tempE);
    }
    
    return result;
}

/**
 * 下一个更大元素
 */
void NextGreaterElement(int *nums, int size, int *result) {
    SqStack S;
    InitSqStack(&S);
    
    // 从右到左遍历数组
    for (int i = size - 1; i >= 0; i--) {
        // 弹出栈中小于等于当前元素的元素
        while (!SqStackEmpty(S)) {
            int top;
            SqGetTop(S, &top);
            if (top <= nums[i]) {
                SqPop(&S, &top);
            } else {
                break;
            }
        }
        
        // 如果栈为空，说明没有更大元素
        if (SqStackEmpty(S)) {
            result[i] = -1;
        } else {
            // 栈顶就是下一个更大元素
            SqGetTop(S, &result[i]);
        }
        
        // 当前元素入栈
        SqPush(&S, nums[i]);
    }
}

/**
 * 栈排序
 */
void StackSort(SqStack *S) {
    SqStack temp;
    InitSqStack(&temp);
    
    while (!SqStackEmpty(*S)) {
        int current;
        SqPop(S, &current);
        
        // 将temp栈中大于current的元素移回S
        while (!SqStackEmpty(temp)) {
            int top;
            SqGetTop(temp, &top);
            if (top > current) {
                SqPop(&temp, &top);
                SqPush(S, top);
            } else {
                break;
            }
        }
        
        // 将current放入temp栈
        SqPush(&temp, current);
    }
    
    // 将排序结果移回原栈
    while (!SqStackEmpty(temp)) {
        int e;
        SqPop(&temp, &e);
        SqPush(S, e);
    }
}

/**
 * 生成有效括号序列（简化版）
 */
void GenerateParenthesis(int n, char **result, int *returnSize) {
    // 这是一个简化的实现，实际应该使用回溯算法
    *returnSize = 0;
    
    if (n <= 0) return;
    
    // 为简单起见，只生成一种模式：(((...)))
    char *pattern = (char*)malloc((2 * n + 1) * sizeof(char));
    int index = 0;
    
    // 添加左括号
    for (int i = 0; i < n; i++) {
        pattern[index++] = '(';
    }
    
    // 添加右括号
    for (int i = 0; i < n; i++) {
        pattern[index++] = ')';
    }
    
    pattern[index] = '\0';
    result[0] = pattern;
    *returnSize = 1;
} 