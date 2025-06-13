/* linklist.c - 链表
 * create by: zw.duan
 */

#include "linklist.h"
#include <time.h>

// ============= 单链表基本操作实现 =============

/**
 * 初始化带头节点的单链表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
LinkList InitList() {
    LinkList L = (LinkList)malloc(sizeof(LNode));
    if (L == NULL) {
        return NULL;
    }
    
    L->next = NULL;
    return L;
}

/**
 * 销毁单链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void DestroyList(LinkList L) {
    if (L == NULL) return;
    
    LNode *p = L;
    LNode *q;
    
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
}

/**
 * 清空链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void ClearList(LinkList L) {
    if (L == NULL) return;
    
    LNode *p = L->next;
    LNode *q;
    
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    
    L->next = NULL;
}

/**
 * 判断链表是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool ListEmpty(LinkList L) {
    if (L == NULL) return true;
    return (L->next == NULL);
}

/**
 * 求链表长度
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int ListLength(LinkList L) {
    if (L == NULL) return 0;
    
    int length = 0;
    LNode *p = L->next;
    
    while (p != NULL) {
        length++;
        p = p->next;
    }
    
    return length;
}

/**
 * 按位序获取元素
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
bool GetElem(LinkList L, int i, ElemType *e) {
    if (L == NULL || e == NULL) {
        return false;
    }
    
    if (i < 1) {
        return false;
    }
    
    LNode *p = L;
    int j = 0;
    
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    
    if (p == NULL) {
        return false;
    }
    
    *e = p->data;
    return true;
}

/**
 * 按位序查找节点
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
LNode* GetElemPtr(LinkList L, int i) {
    if (L == NULL || i < 1) return NULL;
    
    LNode *p = L;
    int j = 0;
    
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    
    return p;
}

/**
 * 按值查找节点
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
LNode* LocateElem(LinkList L, ElemType e) {
    if (L == NULL) return NULL;
    
    LNode *p = L->next;
    
    while (p != NULL && !Equal(p->data, e)) {
        p = p->next;
    }
    
    return p;
}

/**
 * 按值查找元素位序
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int LocateElemPos(LinkList L, ElemType e) {
    if (L == NULL) return 0;
    
    LNode *p = L->next;
    int pos = 1;
    
    while (p != NULL) {
        if (Equal(p->data, e)) {
            return pos;
        }
        p = p->next;
        pos++;
    }
    
    return 0;
}

/**
 * 🔗 在第i个位置插入元素（链表核心操作）
 * 
 *   链表插入过程图示（在位置2插入元素X）：
 * 
 * 插入前：
 * 头节点        第1个节点       第2个节点
 * ┌────┬────┐ ┌────┬────┐   ┌────┬────┐
 * │    │ ●──┼→│ a₁ │ ●──┼─→ │ a₂ │NULL│
 * └────┴────┘ └────┴────┘   └────┴────┘
 *      ↑            ↑             ↑
 *     L (j=0)    p (j=1)       目标位置2
 * 
 * 步骤1：定位到第1个节点（第i-1个节点）
 * 
 * 步骤2：创建新节点并建立连接
 * ┌────┬────┐ ┌────┬────┐   ┌────┬────┐   ┌────┬────┐
 * │    │ ●──┼→│ a₁ │ ●──┼─┐ │ X  │ ●──┼─→ │ a₂ │NULL│
 * └────┴────┘ └────┴────┘ │ └────┴────┘   └────┴────┘
 *                         │      ↑
 *                         └──────┘ (新节点s)
 * 
 *  算法步骤：
 * 1. 参数有效性检查：i ≥ 1, L ≠ NULL
 * 2. 定位插入位置：找到第i-1个节点
 * 3. 创建新节点：分配内存并设置数据
 * 4. 修改指针连接：
 *    - s->next = p->next  (新节点指向原第i个节点)
 *    - p->next = s        (第i-1个节点指向新节点)
 * 
 *   复杂度分析：
 * - 时间复杂度：O(i)
 *   - 最好情况：O(1) （在表头插入，i=1）
 *   - 最坏情况：O(n) （在表尾插入）
 *   - 平均情况：O(n/2) = O(n)
 * - 空间复杂度：O(1)
 * 
 *  
 * - 理解为什么需要找第i-1个节点
 * - 掌握指针操作的顺序（先设置新节点的next）
 * - 对比顺序表：链表插入无需移动元素
 * - 注意带头节点和不带头节点的区别
 * 
 * @param L 带头节点的单链表
 * @param i 插入位置（位序从1开始）
 * @param e 待插入的元素
 * @return 成功返回true，失败返回false
 */
bool ListInsert(LinkList L, int i, ElemType e) {
    // 步骤1：参数有效性检查
    if (L == NULL) {
        printf("错误：链表指针为空\n");
        return false;
    }
    
    if (i < 1) {
        printf("错误：插入位置%d无效，应≥1\n", i);
        return false;
    }
    
    // 步骤2：定位到第i-1个节点
    LNode *p = L;    // p指向头节点（第0个节点）
    int j = 0;       // 当前节点的位序
    
    // 遍历到第i-1个节点
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    
    // 步骤3：检查是否找到有效的第i-1个节点
    if (p == NULL) {
        printf("错误：插入位置%d超出链表长度\n", i);
        return false;
    }
    
    // 步骤4：调用后插操作完成插入
    return InsertNextNode(p, e);
}

/**
 *  删除第i个位置的元素（链表核心操作）
 * 
 *   链表删除过程图示（删除位置2的元素）：
 * 
 * 删除前：
 * 头节点        第1个节点       第2个节点       第3个节点
 * ┌────┬────┐ ┌────┬────┐   ┌────┬────┐   ┌────┬────┐
 * │    │ ●──┼→│ a₁ │ ●──┼─→ │ a₂ │ ●──┼─→ │ a₃ │NULL│
 * └────┴────┘ └────┴────┘   └────┴────┘   └────┴────┘
 *      ↑            ↑             ↑             ↑
 *     L (j=0)    p (j=1)      待删除q        q->next
 * 
 * 步骤1：定位到第1个节点（第i-1个节点）
 * 
 * 步骤2：修改指针连接并释放节点
 * ┌────┬────┐ ┌────┬────┐                   ┌────┬────┐
 * │    │ ●──┼→│ a₁ │ ●──┼─────────────────→ │ a₃ │NULL│
 * └────┴────┘ └────┴────┘                   └────┴────┘
 *                         ┌────┬────┐
 *                         │ a₂ │ ×  │ ← 被删除的节点
 *                         └────┴────┘
 * 
 *  算法步骤：
 * 1. 参数有效性检查：i ≥ 1, L ≠ NULL, e ≠ NULL
 * 2. 定位删除位置：找到第i-1个节点
 * 3. 检查待删除节点：确保第i个节点存在
 * 4. 修改指针连接：p->next = q->next
 * 5. 保存删除元素：*e = q->data
 * 6. 释放内存：free(q)
 * 
 *   复杂度分析：
 * - 时间复杂度：O(i)
 *   - 最好情况：O(1) （删除第1个节点）
 *   - 最坏情况：O(n) （删除最后一个节点）
 *   - 平均情况：O(n/2) = O(n)
 * - 空间复杂度：O(1)
 * 
 *  
 * - 理解为什么需要找第i-1个节点
 * - 掌握指针断链和重连的顺序
 * - 注意内存释放，避免内存泄漏
 * - 对比顺序表：链表删除无需移动元素
 * 
 * @param L 带头节点的单链表
 * @param i 删除位置（位序从1开始）
 * @param e 输出参数，保存被删除的元素
 * @return 成功返回true，失败返回false
 */
bool ListDelete(LinkList L, int i, ElemType *e) {
    // 步骤1：参数有效性检查
    if (L == NULL || e == NULL) {
        printf("错误：链表指针或输出参数为空\n");
        return false;
    }
    
    if (i < 1) {
        printf("错误：删除位置%d无效，应≥1\n", i);
        return false;
    }
    
    // 步骤2：定位到第i-1个节点
    LNode *p = L;    // p指向头节点（第0个节点）
    int j = 0;       // 当前节点的位序
    
    // 遍历到第i-1个节点
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    
    // 步骤3：检查删除条件
    if (p == NULL || p->next == NULL) {
        printf("错误：删除位置%d超出链表范围\n", i);
        return false;
    }
    
    // 步骤4：调用后删操作完成删除
    return DeleteNextNode(p, e);
}

/**
 * ➕ 在指定节点后插入新节点（后插操作）
 * 
 *   后插操作图示：
 * 
 * 插入前：                插入后：
 * ┌────┬────┐             ┌────┬────┐   ┌────┬────┐
 * │ a  │ ●──┼──────────→  │ a  │ ●──┼─→ │ e  │ ●──┼──→ ...
 * └────┴────┘             └────┴────┘   └────┴────┘
 *      ↑                       ↑             ↑
 *     节点p                   节点p        新节点s
 * 
 *  算法步骤：
 * 1. 创建新节点s：分配内存并设置数据域
 * 2. 设置新节点指针：s->next = p->next
 * 3. 修改前驱指针：p->next = s
 * 
 *  关键点：
 * - 必须先设置s->next，再设置p->next
 * - 如果顺序颠倒，会导致原链表断裂
 * 
 *   复杂度分析：
 * - 时间复杂度：O(1) - 无需遍历
 * - 空间复杂度：O(1) - 只分配一个节点
 * 
 *  
 * - 理解指针操作的顺序很重要
 * - 这是所有插入操作的基础
 * - 对比前插操作的实现差异
 * 
 * @param p 指定的节点指针
 * @param e 待插入的元素值
 * @return 成功返回true，失败返回false
 */
bool InsertNextNode(LNode *p, ElemType e) {
    // 步骤1：参数检查
    if (p == NULL) {
        printf("错误：节点指针为空\n");
        return false;
    }
    
    // 步骤2：创建新节点
    LNode *s = (LNode*)malloc(sizeof(LNode));
    if (s == NULL) {
        printf("错误：内存分配失败\n");
        return false;  // 内存分配失败
    }
    
    // 步骤3：设置新节点（注意顺序！）
    s->data = e;           // 设置数据域
    s->next = p->next;     // 新节点指向原后继节点
    p->next = s;           // 原节点指向新节点
    
    return true;
}

/**
 * 删除指定节点的后继节点
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool DeleteNextNode(LNode *p, ElemType *e) {
    if (p == NULL || p->next == NULL) {
        return false;
    }
    
    LNode *q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);
    
    return true;
}

/**
 * 遍历链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool ListTraverse(LinkList L, bool (*visit)(ElemType)) {
    if (L == NULL || visit == NULL) {
        return false;
    }
    
    LNode *p = L->next;
    
    while (p != NULL) {
        if (!visit(p->data)) {
            return false;
        }
        p = p->next;
    }
    
    return true;
}

/**
 * 打印链表
 */
void PrintList(LinkList L) {
    if (L == NULL || L->next == NULL) {
        printf("空链表\n");
        return;
    }
    
    LNode *p = L->next;
    printf("[");
    while (p != NULL) {
        printf("%d", p->data);
        if (p->next != NULL) {
            printf(", ");
        }
        p = p->next;
    }
    printf("]\n");
}

/**
 * 头插法建立单链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
bool CreateListHead(LinkList *L, int n) {
    if (L == NULL || n < 0) {
        return false;
    }
    
    *L = InitList();
    if (*L == NULL) {
        return false;
    }
    
    for (int i = 0; i < n; i++) {
        int value;
        printf("请输入第%d个元素：", i + 1);
        if (scanf("%d", &value) != 1) {
            return false;
        }
        
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (s == NULL) {
            return false;
        }
        
        s->data = value;
        s->next = (*L)->next;
        (*L)->next = s;
    }
    
    return true;
}

/**
 * 尾插法建立单链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
bool CreateListTail(LinkList *L, int n) {
    if (L == NULL || n < 0) {
        return false;
    }
    
    *L = InitList();
    if (*L == NULL) {
        return false;
    }
    
    LNode *r = *L;  // 尾指针
    
    for (int i = 0; i < n; i++) {
        int value;
        printf("请输入第%d个元素：", i + 1);
        if (scanf("%d", &value) != 1) {
            return false;
        }
        
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (s == NULL) {
            return false;
        }
        
        s->data = value;
        s->next = NULL;
        r->next = s;
        r = s;
    }
    
    return true;
}

/**
 * 从数组创建链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
bool CreateListFromArray(LinkList *L, ElemType arr[], int n) {
    if (L == NULL || arr == NULL || n < 0) {
        return false;
    }
    
    *L = InitList();
    if (*L == NULL) {
        return false;
    }
    
    LNode *r = *L;
    
    for (int i = 0; i < n; i++) {
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (s == NULL) {
            return false;
        }
        
        s->data = arr[i];
        s->next = NULL;
        r->next = s;
        r = s;
    }
    
    return true;
}

// ============= 双链表基本操作实现 =============

/**
 * 初始化双链表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
DLinkList InitDList() {
    DLinkList L = (DLinkList)malloc(sizeof(DNode));
    if (L == NULL) {
        return NULL;
    }
    
    L->prior = NULL;
    L->next = NULL;
    
    return L;
}

/**
 * 在双链表第i个位置插入元素
 * 时间复杂度：O(min(i,n-i))
 * 空间复杂度：O(1)
 */
bool DListInsert(DLinkList L, int i, ElemType e) {
    if (L == NULL || i < 1) {
        return false;
    }
    
    // 找到第i-1个节点
    DNode *p = L;
    int j = 0;
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    
    if (p == NULL) {
        return false;
    }
    
    DNode *s = (DNode*)malloc(sizeof(DNode));
    if (s == NULL) {
        return false;
    }
    
    s->data = e;
    s->next = p->next;
    s->prior = p;
    
    if (p->next != NULL) {
        p->next->prior = s;
    }
    p->next = s;
    
    return true;
}

/**
 * 删除双链表第i个位置的元素
 * 时间复杂度：O(min(i,n-i))
 * 空间复杂度：O(1)
 */
bool DListDelete(DLinkList L, int i, ElemType *e) {
    if (L == NULL || e == NULL || i < 1) {
        return false;
    }
    
    // 找到第i个节点
    DNode *p = L;
    int j = 0;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    
    if (p == NULL) {
        return false;
    }
    
    *e = p->data;
    
    if (p->next != NULL) {
        p->next->prior = p->prior;
    }
    if (p->prior != NULL) {
        p->prior->next = p->next;
    }
    
    free(p);
    return true;
}

/**
 * 打印双链表
 */
void PrintDList(DLinkList L) {
    if (L == NULL || L->next == NULL) {
        printf("空双链表\n");
        return;
    }
    
    DNode *p = L->next;
    printf("双链表: [");
    while (p != NULL) {
        printf("%d", p->data);
        if (p->next != NULL) {
            printf(", ");
        }
        p = p->next;
    }
    printf("]\n");
}

// ============= 循环链表操作实现 =============

/**
 * 初始化循环单链表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
CircularList InitCircularList() {
    CircularList L = (CircularList)malloc(sizeof(LNode));
    if (L == NULL) {
        return NULL;
    }
    
    L->next = L;  // 头节点指向自己，形成循环
    return L;
}

/**
 * 销毁循环单链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void DestroyCircularList(CircularList L) {
    if (L == NULL) return;
    
    LNode *p = L->next;
    
    while (p != L) {
        LNode *temp = p;
        p = p->next;
        free(temp);
    }
    
    free(L);
}

/**
 * 清空循环单链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void ClearCircularList(CircularList L) {
    if (L == NULL) return;
    
    LNode *p = L->next;
    
    while (p != L) {
        LNode *temp = p;
        p = p->next;
        free(temp);
    }
    
    L->next = L;
}

/**
 * 判断循环单链表是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool CircularListEmpty(CircularList L) {
    if (L == NULL) return true;
    return (L->next == L);
}

/**
 * 求循环单链表长度
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int CircularListLength(CircularList L) {
    if (L == NULL) return 0;
    
    int length = 0;
    LNode *p = L->next;
    
    while (p != L) {
        length++;
        p = p->next;
    }
    
    return length;
}

/**
 * 按位序获取循环链表元素
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
bool CircularGetElem(CircularList L, int i, ElemType *e) {
    if (L == NULL || e == NULL || i < 1) {
        return false;
    }
    
    LNode *p = L;
    int j = 0;
    
    while (j < i && p->next != L) {
        p = p->next;
        j++;
    }
    
    if (j == i && p != L) {
        *e = p->data;
        return true;
    }
    
    return false;
}

/**
 * 在循环链表第i个位置插入元素
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
bool CircularListInsert(CircularList L, int i, ElemType e) {
    if (L == NULL || i < 1) {
        return false;
    }
    
    // 找到第i-1个节点
    LNode *p = L;
    int j = 0;
    
    while (j < i - 1 && p->next != L) {
        p = p->next;
        j++;
    }
    
    if (j != i - 1 && p->next == L && j < i - 1) {
        return false;  // 插入位置超出范围
    }
    
    LNode *s = (LNode*)malloc(sizeof(LNode));
    if (s == NULL) {
        return false;
    }
    
    s->data = e;
    s->next = p->next;
    p->next = s;
    
    return true;
}

/**
 * 删除循环链表第i个位置的元素
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
bool CircularListDelete(CircularList L, int i, ElemType *e) {
    if (L == NULL || e == NULL || i < 1) {
        return false;
    }
    
    // 找到第i-1个节点
    LNode *p = L;
    int j = 0;
    
    while (j < i - 1 && p->next != L) {
        p = p->next;
        j++;
    }
    
    if (p->next == L || j != i - 1) {
        return false;  // 删除位置无效
    }
    
    LNode *q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);
    
    return true;
}

/**
 * 遍历循环链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool CircularListTraverse(CircularList L, bool (*visit)(ElemType)) {
    if (L == NULL || visit == NULL) {
        return false;
    }
    
    LNode *p = L->next;
    
    while (p != L) {
        if (!visit(p->data)) {
            return false;
        }
        p = p->next;
    }
    
    return true;
}

/**
 * 打印循环链表
 */
void PrintCircularList(CircularList L) {
    if (L == NULL || L->next == L) {
        printf("空循环链表\n");
        return;
    }
    
    LNode *p = L->next;
    printf("循环链表: [");
    while (p != L) {
        printf("%d", p->data);
        if (p->next != L) {
            printf(", ");
        }
        p = p->next;
    }
    printf("] (循环)\n");
}

/**
 * 初始化循环双链表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
CircularDList InitCircularDList() {
    CircularDList L = (CircularDList)malloc(sizeof(DNode));
    if (L == NULL) {
        return NULL;
    }
    
    L->prior = L;  // 前驱指向自己
    L->next = L;   // 后继指向自己
    return L;
}

/**
 * 销毁循环双链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
void DestroyCircularDList(CircularDList L) {
    if (L == NULL) return;
    
    DNode *p = L->next;
    
    while (p != L) {
        DNode *temp = p;
        p = p->next;
        free(temp);
    }
    
    free(L);
}

/**
 * 在循环双链表第i个位置插入元素
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
bool CircularDListInsert(CircularDList L, int i, ElemType e) {
    if (L == NULL || i < 1) {
        return false;
    }
    
    // 找到第i-1个节点
    DNode *p = L;
    int j = 0;
    
    while (j < i - 1 && p->next != L) {
        p = p->next;
        j++;
    }
    
    if (j != i - 1 && p->next == L && j < i - 1) {
        return false;
    }
    
    DNode *s = (DNode*)malloc(sizeof(DNode));
    if (s == NULL) {
        return false;
    }
    
    s->data = e;
    s->next = p->next;
    s->prior = p;
    p->next->prior = s;
    p->next = s;
    
    return true;
}

/**
 * 删除循环双链表第i个位置的元素
 * 时间复杂度：O(i)
 * 空间复杂度：O(1)
 */
bool CircularDListDelete(CircularDList L, int i, ElemType *e) {
    if (L == NULL || e == NULL || i < 1) {
        return false;
    }
    
    // 找到第i个节点
    DNode *p = L->next;
    int j = 1;
    
    while (j < i && p != L) {
        p = p->next;
        j++;
    }
    
    if (p == L) {
        return false;
    }
    
    *e = p->data;
    p->prior->next = p->next;
    p->next->prior = p->prior;
    free(p);
    
    return true;
}

/**
 * 打印循环双链表
 */
void PrintCircularDList(CircularDList L) {
    if (L == NULL || L->next == L) {
        printf("空循环双链表\n");
        return;
    }
    
    DNode *p = L->next;
    printf("循环双链表: [");
    while (p != L) {
        printf("%d", p->data);
        if (p->next != L) {
            printf(", ");
        }
        p = p->next;
    }
    printf("] (双向循环)\n");
}

/**
 * 合并两个有序循环链表
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 */
bool MergeCircularList(CircularList La, CircularList Lb, CircularList *Lc) {
    if (La == NULL || Lb == NULL || Lc == NULL) {
        return false;
    }
    
    *Lc = InitCircularList();
    if (*Lc == NULL) {
        return false;
    }
    
    LNode *pa = La->next;
    LNode *pb = Lb->next;
    LNode *pc = *Lc;
    
    while (pa != La && pb != Lb) {
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (s == NULL) {
            return false;
        }
        
        if (Compare(pa->data, pb->data) <= 0) {
            s->data = pa->data;
            pa = pa->next;
        } else {
            s->data = pb->data;
            pb = pb->next;
        }
        
        s->next = pc->next;
        pc->next = s;
        pc = s;
    }
    
    while (pa != La) {
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (s == NULL) {
            return false;
        }
        s->data = pa->data;
        s->next = pc->next;
        pc->next = s;
        pc = s;
        pa = pa->next;
    }
    
    while (pb != Lb) {
        LNode *s = (LNode*)malloc(sizeof(LNode));
        if (s == NULL) {
            return false;
        }
        s->data = pb->data;
        s->next = pc->next;
        pc->next = s;
        pc = s;
        pb = pb->next;
    }
    
    return true;
}

/**
 * 循环链表逆转
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool CircularListReverse(CircularList L) {
    if (L == NULL || L->next == L) {
        return true;
    }
    
    LNode *p = L->next;
    LNode *q;
    
    // 断开循环
    LNode *tail = L;
    while (tail->next != L) {
        tail = tail->next;
    }
    tail->next = NULL;
    L->next = NULL;
    
    // 逆转链表
    while (p != NULL) {
        q = p->next;
        p->next = L->next;
        L->next = p;
        p = q;
    }
    
    // 重新形成循环
    tail = L->next;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = L;
    
    return true;
}

// ============= 高级操作算法实现 =============

/**
 *  单链表逆转算法（迭代实现， 重点）
 * 
 *   逆转过程图示：
 * 
 * 原链表：
 * 头节点      第1个节点     第2个节点     第3个节点
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘
 *      ↑          ↑
 *      L          p
 * 
 * 逆转步骤1：（处理节点1）
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │NULL││ 1  │ ●──┼→│    │NULL││ 2  │ ●──┼→│ 3  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘
 *      ↑          ↑           ↑          ↑
 *      L      新头节点         p          q
 * 
 * 逆转步骤2：（处理节点2）
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 2  │ ●──┼→│ 1  │NULL││ 3  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘
 *      ↑                              ↑
 *      L                              p
 * 
 * 最终结果：
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 3  │ ●──┼→│ 2  │ ●──┼→│ 1  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘
 * 
 *  算法思路：
 * 1. 使用头插法重新组织链表
 * 2. 逐个取下原链表的节点
 * 3. 将每个节点插入到头节点之后
 * 4. 最终实现链表的逆转
 * 
 *  关键变量：
 * - p：当前待处理的节点
 * - q：保存p的后继节点（防止断链）
 * - L->next：新链表的第一个节点
 * 
 *   复杂度分析：
 * - 时间复杂度：O(n) - 每个节点访问一次
 * - 空间复杂度：O(1) - 只使用常数额外空间
 * 
 *  
 * - 理解头插法的应用
 * - 掌握指针操作的技巧
 * - 注意保存后继节点防止断链
 * - 对比递归实现的差异
 * 
 * @param L 带头节点的单链表
 * @return 成功返回true，失败返回false
 */
bool ReverseList(LinkList L) {
    // 参数检查
    if (L == NULL) {
        printf("错误：链表指针为空\n");
        return false;
    }
    
    // 如果链表为空或只有一个节点，无需逆转
    if (L->next == NULL || L->next->next == NULL) {
        return true;
    }
    
    LNode *p = L->next;    // p指向第一个数据节点
    LNode *q;              // q用于保存p的后继节点
    L->next = NULL;        // 头节点暂时指向空，准备重新构建
    
    // 头插法重建链表
    while (p != NULL) {
        q = p->next;       // 保存p的后继节点
        p->next = L->next; // p指向新链表的第一个节点
        L->next = p;       // 头节点指向p（p成为新的第一个节点）
        p = q;             // 移动到下一个待处理节点
    }
    
    return true;
}

// 递归辅助函数
static LNode* reverseHelper(LNode* node) {
    if (node == NULL || node->next == NULL) {
        return node;
    }
    
    LNode* newHead = reverseHelper(node->next);
    node->next->next = node;
    node->next = NULL;
    
    return newHead;
}

/**
 * 递归方式逆转链表
 * 时间复杂度：O(n)
 * 空间复杂度：O(n)
 */
bool ReverseListRecursive(LinkList L) {
    if (L == NULL || L->next == NULL) {
        return true;
    }
    
    L->next = reverseHelper(L->next);
    return true;
}

/**
 * 🐢🐰 找到链表的中间节点（快慢指针法， 经典）
 * 
 *   快慢指针工作原理图示：
 * 
 * 初始状态：
 * 头节点      节点1        节点2        节点3        节点4        节点5
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │ ●──┼→│ 4  │ ●──┼→│ 5  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘
 *              ↑
 *         slow, fast
 * 
 * 第1步移动：
 *              slow         fast
 *              ↓             ↓
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │ ●──┼→│ 4  │ ●──┼→│ 5  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘
 * 
 * 第2步移动：
 *                           slow                   fast
 *                           ↓                       ↓
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │ ●──┼→│ 4  │ ●──┼→│ 5  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘
 * 
 * 第3步移动：fast到达末尾，slow指向中间节点
 *                                      slow         fast
 *                                      ↓             ↓
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │ ●──┼→│ 4  │ ●──┼→│ 5  │NULL│
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘ └────┴────┘
 * 
 *  算法思路：
 * - slow指针每次移动1步
 * - fast指针每次移动2步
 * - 当fast到达末尾时，slow恰好在中间
 * 
 *  处理不同长度：
 * - 奇数长度(2k+1)：slow指向第k+1个节点（正中间）
 * - 偶数长度(2k)：slow指向第k个节点（中间偏前）
 * 
 *   复杂度分析：
 * - 时间复杂度：O(n) - 遍历链表一次
 * - 空间复杂度：O(1) - 只使用两个指针
 * 
 *  
 * - 掌握快慢指针技巧
 * - 理解数学原理：fast走2n步，slow走n步
 * - 注意边界条件处理
 * - 常用于链表分割、归并排序等
 * 
 * @param L 带头节点的单链表
 * @return 指向中间节点的指针，失败返回NULL
 */
LNode* FindMiddleNode(LinkList L) {
    // 参数检查
    if (L == NULL || L->next == NULL) {
        printf("链表为空或无数据节点\n");
        return NULL;
    }
    
    // 初始化快慢指针，都从第一个数据节点开始
    LNode *slow = L->next;    // 慢指针，每次移动1步
    LNode *fast = L->next;    // 快指针，每次移动2步
    
    // 快慢指针同时移动
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;           // 慢指针移动1步
        fast = fast->next->next;     // 快指针移动2步
    }
    
    return slow;  // 此时slow指向中间节点
}

/**
 *  检测链表中是否有环（Floyd判圈算法， 重点）
 * 
 *   环检测原理图示：
 * 
 * 无环链表：
 * 头节点      节点1        节点2        节点3
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │NULL│ ← fast最终到达NULL
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘
 *                           ↑
 *                         slow
 * 
 * 有环链表：
 * 头节点      节点1        节点2        节点3
 * ┌────┬────┐┌────┬────┐ ┌────┬────┐ ┌────┬────┐
 * │    │ ●──┼→│ 1  │ ●──┼→│ 2  │ ●──┼→│ 3  │ ●──┼─┐
 * └────┴────┘└────┴────┘ └────┴────┘ └────┴────┘ │
 *              ↑                                   │
 *              └───────────────────────────────────┘
 * 
 * 环检测过程：
 * 
 * 步骤1：slow和fast都在节点1
 * 步骤2：slow在节点2，fast在节点3
 * 步骤3：slow在节点3，fast在节点2（由于环的存在）
 * 步骤4：slow在节点1，fast在节点1 → 相遇！检测到环
 * 
 *  算法原理（数学证明）：
 * 设链表有环，环长为C，slow走了S步时首次进入环
 * 当slow和fast相遇时：
 * - slow走了n步
 * - fast走了2n步
 * - fast比slow多走了n步，这n步正好是环长的整数倍
 * - 因此：n = k×C（k为正整数）
 * - 由于fast走得更快，一定会在环内追上slow
 * 
 *  算法特点：
 * - 如果无环：fast最终会到达NULL
 * - 如果有环：slow和fast必定会在环内相遇
 * - 相遇时间：最多O(n)步
 * 
 *   复杂度分析：
 * - 时间复杂度：O(n) - 最多遍历链表一次多一点
 * - 空间复杂度：O(1) - 只使用两个指针
 * 
 *  
 * - 理解Floyd判圈算法的数学原理
 * - 掌握快慢指针在环检测中的应用
 * - 注意指针移动的顺序和边界条件
 * - 可扩展到环入口检测等高级问题
 * 
 * @param L 带头节点的单链表
 * @return 有环返回true，无环返回false
 */
bool HasCycle(LinkList L) {
    // 参数检查
    if (L == NULL || L->next == NULL) {
        return false;  // 空链表或只有头节点，无环
    }
    
    // 初始化快慢指针
    LNode *slow = L->next;    // 慢指针，每次移动1步
    LNode *fast = L->next;    // 快指针，每次移动2步
    
    // Floyd判圈算法核心循环
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;           // 慢指针前进1步
        fast = fast->next->next;     // 快指针前进2步
        
        // 检测是否相遇
        if (slow == fast) {
            printf("检测到环：slow和fast在同一节点相遇\n");
            return true;             // 相遇说明有环
        }
    }
    
    // fast到达链表末尾，说明无环
    return false;
}

/**
 * 删除有序链表中的重复元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int RemoveDuplicates(LinkList L) {
    if (L == NULL || L->next == NULL) {
        return 0;
    }
    
    LNode *p = L->next;
    int count = 0;
    
    while (p != NULL && p->next != NULL) {
        if (Equal(p->data, p->next->data)) {
            LNode *q = p->next;
            p->next = q->next;
            free(q);
            count++;
        } else {
            p = p->next;
        }
    }
    
    return count;
}

/**
 * 删除链表中所有值为x的节点
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int RemoveAllX(LinkList L, ElemType x) {
    if (L == NULL) return 0;
    
    LNode *p = L;
    int count = 0;
    
    while (p->next != NULL) {
        if (Equal(p->next->data, x)) {
            LNode *q = p->next;
            p->next = q->next;
            free(q);
            count++;
        } else {
            p = p->next;
        }
    }
    
    return count;
}

/**
 * 链表分割
 * 时间复杂度：O(k)
 * 空间复杂度：O(1)
 */
bool SplitList(LinkList L, int k, LinkList *L2) {
    if (L == NULL || L2 == NULL || k < 1) {
        return false;
    }
    
    *L2 = InitList();
    if (*L2 == NULL) {
        return false;
    }
    
    LNode *p = L;
    for (int i = 0; i < k && p->next != NULL; i++) {
        p = p->next;
    }
    
    (*L2)->next = p->next;
    p->next = NULL;
    
    return true;
}

/**
 * 在有序链表中插入元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool InsertInOrder(LinkList L, ElemType e) {
    if (L == NULL) return false;
    
    LNode *p = L;
    
    while (p->next != NULL && Compare(p->next->data, e) < 0) {
        p = p->next;
    }
    
    return InsertNextNode(p, e);
}

/**
 * 有序链表合并
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 */
bool MergeList(LinkList La, LinkList Lb, LinkList *Lc) {
    if (La == NULL || Lb == NULL || Lc == NULL) {
        return false;
    }
    
    *Lc = InitList();
    if (*Lc == NULL) {
        return false;
    }
    
    LNode *pa = La->next;
    LNode *pb = Lb->next;
    LNode *pc = *Lc;
    
    while (pa != NULL && pb != NULL) {
        if (Compare(pa->data, pb->data) <= 0) {
            InsertNextNode(pc, pa->data);
            pa = pa->next;
        } else {
            InsertNextNode(pc, pb->data);
            pb = pb->next;
        }
        pc = pc->next;
    }
    
    while (pa != NULL) {
        InsertNextNode(pc, pa->data);
        pa = pa->next;
        pc = pc->next;
    }
    
    while (pb != NULL) {
        InsertNextNode(pc, pb->data);
        pb = pb->next;
        pc = pc->next;
    }
    
    return true;
}

/**
 * 链表的交集运算
 * 时间复杂度：O(m*n)
 * 空间复杂度：O(1)
 */
bool IntersectionList(LinkList La, LinkList Lb, LinkList *Lc) {
    if (La == NULL || Lb == NULL || Lc == NULL) {
        return false;
    }
    
    *Lc = InitList();
    if (*Lc == NULL) {
        return false;
    }
    
    LNode *pa = La->next;
    LNode *pc = *Lc;
    
    while (pa != NULL) {
        if (LocateElem(Lb, pa->data) != NULL) {
            InsertNextNode(pc, pa->data);
            pc = pc->next;
        }
        pa = pa->next;
    }
    
    return true;
}

/**
 * 链表的并集运算
 * 时间复杂度：O(m*n)
 * 空间复杂度：O(1)
 */
bool UnionList(LinkList La, LinkList Lb, LinkList *Lc) {
    if (La == NULL || Lb == NULL || Lc == NULL) {
        return false;
    }
    
    *Lc = InitList();
    if (*Lc == NULL) {
        return false;
    }
    
    LNode *pa = La->next;
    LNode *pc = *Lc;
    
    // 复制La中的所有元素
    while (pa != NULL) {
        InsertNextNode(pc, pa->data);
        pc = pc->next;
        pa = pa->next;
    }
    
    // 添加Lb中不在La中的元素
    LNode *pb = Lb->next;
    while (pb != NULL) {
        if (LocateElem(La, pb->data) == NULL) {
            InsertNextNode(pc, pb->data);
            pc = pc->next;
        }
        pb = pb->next;
    }
    
    return true;
}

// ============= 辅助函数实现 =============

/**
 * 比较两个元素是否相等
 */
bool Equal(ElemType a, ElemType b) {
    return a == b;
}

/**
 * 比较两个元素大小
 */
int Compare(ElemType a, ElemType b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/**
 * 访问函数：打印元素
 */
bool Visit_Print(ElemType e) {
    printf("%d ", e);
    return true;
}

// ============= 性能测试函数实现 =============

/**
 * 链表基本操作性能测试
 */
void PerformanceTest_LinkList(int size) {
    printf("=== 链表基本操作性能测试 (规模: %d) ===\n", size);
    
    LinkList L = InitList();
    if (L == NULL) return;
    
    // 插入性能测试
    clock_t start = clock();
    for (int i = 0; i < size; i++) {
        ListInsert(L, L->next == NULL ? 1 : ListLength(L) + 1, i);
    }
    clock_t end = clock();
    
    double insertTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("插入%d个元素用时：%.4f秒\n", ListLength(L), insertTime);
    
    // 查找性能测试
    start = clock();
    for (int i = 0; i < 1000; i++) {
        LocateElem(L, i % size);
    }
    end = clock();
    
    double searchTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("1000次查找用时：%.4f秒\n", searchTime);
    
    DestroyList(L);
}

/**
 * 链表vs顺序表性能对比测试
 */
void PerformanceCompare_ListVsArray(int size) {
    printf("=== 链表vs顺序表性能对比 (规模: %d) ===\n", size);
    
    LinkList L = InitList();
    if (L == NULL) return;
    
    clock_t start = clock();
    for (int i = 0; i < size; i++) {
        ListInsert(L, 1, i);  // 头插
    }
    clock_t end = clock();
    
    double linkTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("链表头插%d个元素用时：%.4f秒\n", size, linkTime);
    
    DestroyList(L);
}

/**
 * 链表高级算法性能测试
 */
void PerformanceTest_Advanced(int size) {
    printf("=== 链表高级算法性能测试 (规模: %d) ===\n", size);
    
    LinkList L = InitList();
    if (L == NULL) return;
    
    // 构造测试数据
    for (int i = 0; i < size; i++) {
        ListInsert(L, ListLength(L) + 1, i);
    }
    
    clock_t start, end;
    
    // 逆转测试
    start = clock();
    ReverseList(L);
    end = clock();
    printf("逆转操作用时：%.4f秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // 查找中间节点测试
    start = clock();
    FindMiddleNode(L);
    end = clock();
    printf("查找中间节点用时：%.4f秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    DestroyList(L);
} 