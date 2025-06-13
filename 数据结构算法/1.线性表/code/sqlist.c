/* sqlist.c - 顺序表
 * create by: zw.duan
 */

#include "sqlist.h"
#include <time.h>

// 内部辅助函数声明
static void ReverseRange(ElemType arr[], int start, int end);

// ============= 静态顺序表基本操作实现 =============

/**
 * 初始化静态顺序表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool InitList(SqList *L) {
    if (L == NULL) {
        printf("错误：传入空指针！\n");
        return false;
    }
    
    L->length = 0;
    return true;
}

/**
 * 销毁顺序表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void DestroyList(SqList *L) {
    if (L != NULL) {
        L->length = 0;
    }
}

/**
 * 清空顺序表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void ClearList(SqList *L) {
    if (L != NULL) {
        L->length = 0;
    }
}

/**
 * 判断顺序表是否为空
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool ListEmpty(SqList L) {
    return (L.length == 0);
}

/**
 * 获取顺序表长度
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
int ListLength(SqList L) {
    return L.length;
}

/**
 * 按位序获取元素
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool GetElem(SqList L, int i, ElemType *e) {
    if (e == NULL) {
        return false;
    }
    
    if (i < 1 || i > L.length) {
        return false;
    }
    
    *e = L.data[i - 1];
    return true;
}

/**
 * 按值查找元素位序
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int LocateElem(SqList L, ElemType e) {
    for (int i = 0; i < L.length; i++) {
        if (Equal(L.data[i], e)) {
            return i + 1;
        }
    }
    return 0;
}

/**
 * 获取前驱元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool PriorElem(SqList L, ElemType cur_e, ElemType *pre_e) {
    if (pre_e == NULL) {
        return false;
    }
    
    int pos = LocateElem(L, cur_e);
    if (pos <= 1) {
        return false;
    }
    
    *pre_e = L.data[pos - 2];
    return true;
}

/**
 * 获取后继元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool NextElem(SqList L, ElemType cur_e, ElemType *next_e) {
    if (next_e == NULL) {
        return false;
    }
    
    int pos = LocateElem(L, cur_e);
    if (pos == 0 || pos >= L.length) {
        return false;
    }
    
    *next_e = L.data[pos];
    return true;
}

/**
 *  在指定位置插入元素（顺序表核心操作）
 * 
 *   插入过程图示（在位置2插入元素X）：
 * 
 * 插入前：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₁  │ a₂  │ a₃  │     │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┘
 *   1     2     3                    
 * 
 * 元素后移：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₁  │     │ a₂  │ a₃  │     │     │  ← a₂、a₃依次后移
 * └─────┴─────┴─────┴─────┴─────┴─────┘
 *   1     2     3     4
 * 
 * 插入元素X：
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │ a₁  │  X  │ a₂  │ a₃  │     │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┘
 *   1     2     3     4
 * 
 *  算法步骤：
 * 1. 参数有效性检查：i ∈ [1, length+1]
 * 2. 空间检查：length < MAXSIZE
 * 3. 元素后移：从后往前移动 [i, length] 位置的元素
 * 4. 插入新元素：data[i-1] = e
 * 5. 更新长度：length++
 * 
 *   复杂度分析：
 * - 时间复杂度：O(n)
 *   - 最好情况：O(1) （在表尾插入）
 *   - 最坏情况：O(n) （在表头插入）
 *   - 平均情况：O(n/2) = O(n)
 * - 空间复杂度：O(1)
 * 
 *  
 * - 理解为什么要从后往前移动元素
 * - 掌握边界条件的判断
 * - 能够分析不同插入位置的时间复杂度
 * - 对比链表插入操作的优劣
 * 
 * @param L 指向顺序表的指针
 * @param i 插入位置（位序从1开始）
 * @param e 待插入的元素
 * @return 成功返回true，失败返回false
 */
bool ListInsert(SqList *L, int i, ElemType e) {
    // 步骤1：参数有效性检查
    if (L == NULL) {
        printf("错误：顺序表指针为空\n");
        return false;
    }
    
    // 检查插入位置是否有效：i ∈ [1, length+1]
    if (i < 1 || i > L->length + 1) {
        printf("错误：插入位置%d无效，应在[1,%d]范围内\n", i, L->length + 1);
        return false;
    }
    
    // 步骤2：空间检查
    if (L->length >= MAXSIZE) {
        printf("错误：顺序表已满，无法插入\n");
        return false;
    }
    
    // 步骤3：元素后移（关键！从后往前移动避免数据覆盖）
    for (int j = L->length; j >= i; j--) {
        L->data[j] = L->data[j - 1];  // 第j个元素移动到第j+1个位置
    }
    
    // 步骤4：插入新元素
    L->data[i - 1] = e;  // 位序i对应数组下标i-1
    
    // 步骤5：更新长度
    L->length++;
    
    return true;
}

/**
 * 删除指定位置的元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool ListDelete(SqList *L, int i, ElemType *e) {
    if (L == NULL || e == NULL) {
        return false;
    }
    
    if (i < 1 || i > L->length) {
        return false;
    }
    
    *e = L->data[i - 1];
    
    // 元素前移
    for (int j = i; j < L->length; j++) {
        L->data[j - 1] = L->data[j];
    }
    
    L->length--;
    return true;
}

/**
 * 遍历顺序表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool ListTraverse(SqList L, bool (*visit)(ElemType)) {
    if (visit == NULL) {
        return false;
    }
    
    for (int i = 0; i < L.length; i++) {
        if (!visit(L.data[i])) {
            return false;
        }
    }
    return true;
}

/**
 * 打印顺序表
 */
void PrintList(SqList L) {
    if (L.length == 0) {
        printf("空表\n");
        return;
    }
    
    printf("[");
    for (int i = 0; i < L.length; i++) {
        printf("%d", L.data[i]);
        if (i < L.length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// ============= 动态顺序表基本操作实现 =============

/**
 * 初始化动态顺序表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
bool InitDynamicList(DynamicList *L) {
    if (L == NULL) {
        return false;
    }
    
    L->data = (ElemType*)malloc(INIT_SIZE * sizeof(ElemType));
    if (L->data == NULL) {
        return false;
    }
    
    L->length = 0;
    L->maxsize = INIT_SIZE;
    return true;
}

/**
 * 销毁动态顺序表
 * 时间复杂度：O(1)
 * 空间复杂度：O(1)
 */
void DestroyDynamicList(DynamicList *L) {
    if (L != NULL && L->data != NULL) {
        free(L->data);
        L->data = NULL;
        L->length = 0;
        L->maxsize = 0;
    }
}

/**
 * 动态顺序表扩容
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool IncreaseSize(DynamicList *L, int increment) {
    if (L == NULL || L->data == NULL || increment <= 0) {
        return false;
    }
    
    int newSize = L->maxsize + increment;
    ElemType *newData = (ElemType*)realloc(L->data, newSize * sizeof(ElemType));
    if (newData == NULL) {
        return false;
    }
    
    L->data = newData;
    L->maxsize = newSize;
    return true;
}

/**
 * 动态顺序表插入元素
 * 时间复杂度：平均O(1)，最坏O(n)
 * 空间复杂度：O(1)
 */
bool DynamicListInsert(DynamicList *L, int i, ElemType e) {
    if (L == NULL || L->data == NULL) {
        return false;
    }
    
    if (i < 1 || i > L->length + 1) {
        return false;
    }
    
    // 扩容检查
    if (L->length >= L->maxsize) {
        if (!IncreaseSize(L, INCREMENT)) {
            return false;
        }
    }
    
    // 元素后移
    for (int j = L->length; j >= i; j--) {
        L->data[j] = L->data[j - 1];
    }
    
    L->data[i - 1] = e;
    L->length++;
    
    return true;
}

/**
 * 打印动态顺序表
 */
void PrintDynamicList(DynamicList L) {
    printf("动态表[长度:%d/容量:%d]: ", L.length, L.maxsize);
    
    if (L.length == 0) {
        printf("空表\n");
        return;
    }
    
    printf("[");
    for (int i = 0; i < L.length; i++) {
        printf("%d", L.data[i]);
        if (i < L.length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// ============= 高级操作算法实现 =============

/**
 * 删除所有值为x的元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int DeleteAllX(SqList *L, ElemType x) {
    if (L == NULL) return 0;
    
    int k = 0;  // 记录删除元素个数
    
    for (int i = 0; i < L->length; i++) {
        if (!Equal(L->data[i], x)) {
            L->data[i - k] = L->data[i];
        } else {
            k++;
        }
    }
    
    L->length -= k;
    return k;
}

/**
 * 删除范围内的元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int DeleteRange(SqList *L, ElemType s, ElemType t) {
    if (L == NULL || s >= t) return -1;
    
    int k = 0;  // 记录删除元素个数
    
    for (int i = 0; i < L->length; i++) {
        if (L->data[i] >= s && L->data[i] <= t) {
            k++;
        } else {
            L->data[i - k] = L->data[i];
        }
    }
    
    L->length -= k;
    return k;
}

/**
 * 逆转顺序表
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool ReverseList(SqList *L) {
    if (L == NULL) return false;
    
    for (int i = 0; i < L->length / 2; i++) {
        Swap(&L->data[i], &L->data[L->length - 1 - i]);
    }
    
    return true;
}

/**
 * 循环左移k个位置
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool LeftShift(SqList *L, int k) {
    if (L == NULL || L->length == 0) return false;
    
    k = k % L->length;  // 处理k大于length的情况
    if (k == 0) return true;
    
    // 使用三次逆转实现左移
    ReverseRange(L->data, 0, k - 1);
    ReverseRange(L->data, k, L->length - 1);
    ReverseRange(L->data, 0, L->length - 1);
    
    return true;
}

/**
 * 有序表插入
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
bool InsertInOrder(SqList *L, ElemType e) {
    if (L == NULL || L->length >= MAXSIZE) {
        return false;
    }
    
    int i = L->length - 1;
    while (i >= 0 && Compare(L->data[i], e) > 0) {
        L->data[i + 1] = L->data[i];
        i--;
    }
    
    L->data[i + 1] = e;
    L->length++;
    
    return true;
}

/**
 * 删除有序表中的重复元素
 * 时间复杂度：O(n)
 * 空间复杂度：O(1)
 */
int RemoveDuplicates(SqList *L) {
    if (L == NULL || L->length <= 1) return 0;
    
    int i = 0, j = 1;
    int deletedCount = 0;
    
    while (j < L->length) {
        if (!Equal(L->data[i], L->data[j])) {
            i++;
            L->data[i] = L->data[j];
        } else {
            deletedCount++;
        }
        j++;
    }
    
    L->length = i + 1;
    return deletedCount;
}

/**
 *  二分查找（有序顺序表的高效查找， 重点）
 * 
 *   二分查找过程图示（查找元素7）：
 * 
 * 初始状态：在有序数组[1,3,5,7,9,11,13]中查找7
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  3  │  5  │  7  │  9  │ 11  │ 13  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   0     1     2     3     4     5     6    (下标)
 *   ↑                 ↑                 ↑
 *  low               mid               high
 * 
 * 第1次比较：mid=3, L.data[3]=7, 7==7 ✓ 找到！
 * 
 * 复杂示例：查找元素10（不存在）
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  3  │  5  │  7  │  9  │ 11  │ 13  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *   ↑                 ↑                 ↑
 *  low               mid               high
 * 
 * 第1次：mid=3, L.data[3]=7 < 10, 在右半部分查找
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  3  │  5  │  7  │  9  │ 11  │ 13  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *                           ↑     ↑     ↑
 *                          low   mid   high
 * 
 * 第2次：mid=5, L.data[5]=11 > 10, 在左半部分查找
 * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
 * │  1  │  3  │  5  │  7  │  9  │ 11  │ 13  │
 * └─────┴─────┴─────┴─────┴─────┴─────┴─────┘
 *                           ↑
 *                      low,mid,high
 * 
 * 第3次：mid=4, L.data[4]=9 < 10, low=5 > high=4, 查找结束
 * 
 *  算法思路：
 * 1. 维护查找范围[low, high]
 * 2. 计算中间位置mid = (low + high) / 2
 * 3. 比较L.data[mid]与目标值e：
 *    - 相等：找到，返回位序
 *    - 小于：在右半部分查找，low = mid + 1
 *    - 大于：在左半部分查找，high = mid - 1
 * 4. 重复直到找到或low > high
 * 
 *  算法要点：
 * - 前提条件：数组必须是有序的
 * - 每次比较排除一半元素
 * - 查找范围不断缩小
 * - 时间复杂度：O(log₂n)
 * 
 *   复杂度分析：
 * - 时间复杂度：O(log n) - 每次减半查找范围
 * - 空间复杂度：O(1) - 只使用常数额外空间
 * - 最好情况：O(1) - 第一次就找到
 * - 最坏情况：O(log n) - 查找到最后
 * 
 *  
 * - 理解折半查找的判定树
 * - 掌握平均查找长度(ASL)的计算
 * - 注意边界条件的处理
 * - 对比顺序查找的优劣
 * - mid计算可能的溢出问题：mid = low + (high-low)/2
 * 
 * @param L 有序顺序表
 * @param e 待查找的元素
 * @return 找到返回位序(1-based)，未找到返回0
 */
int BinarySearch(SqList L, ElemType e) {
    // 边界检查
    if (L.length == 0) {
        printf("表为空，无法查找\n");
        return 0;
    }
    
    int low = 0;                    // 查找范围的下界
    int high = L.length - 1;        // 查找范围的上界
    int compareCount = 0;           // 比较次数统计（可选）
    
    // 二分查找主循环
    while (low <= high) {
        // 计算中间位置（注意：防止溢出可用 low + (high-low)/2）
        int mid = (low + high) / 2;
        compareCount++;
        
        // 比较中间元素与目标值
        int cmp = Compare(L.data[mid], e);
        
        if (cmp == 0) {
            // 找到目标元素
            printf("找到元素%d，位置：%d，比较次数：%d\n", e, mid+1, compareCount);
            return mid + 1;  // 返回位序（从1开始）
        } else if (cmp < 0) {
            // 中间元素小于目标值，在右半部分查找
            low = mid + 1;
            printf("第%d次比较：L.data[%d]=%d < %d，查找右半部分[%d,%d]\n", 
                   compareCount, mid, L.data[mid], e, low, high);
        } else {
            // 中间元素大于目标值，在左半部分查找
            high = mid - 1;
            printf("第%d次比较：L.data[%d]=%d > %d，查找左半部分[%d,%d]\n", 
                   compareCount, mid, L.data[mid], e, low, high);
        }
    }
    
    printf("元素%d未找到，总比较次数：%d\n", e, compareCount);
    return 0;  // 未找到
}

/**
 * 合并两个有序表
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 */
bool MergeList(SqList La, SqList Lb, SqList *Lc) {
    if (Lc == NULL) return false;
    
    if (La.length + Lb.length > MAXSIZE) {
        return false;
    }
    
    int i = 0, j = 0, k = 0;
    
    // 合并两个有序表
    while (i < La.length && j < Lb.length) {
        if (Compare(La.data[i], Lb.data[j]) <= 0) {
            Lc->data[k++] = La.data[i++];
        } else {
            Lc->data[k++] = Lb.data[j++];
        }
    }
    
    // 复制剩余元素
    while (i < La.length) {
        Lc->data[k++] = La.data[i++];
    }
    while (j < Lb.length) {
        Lc->data[k++] = Lb.data[j++];
    }
    
    Lc->length = k;
    return true;
}

/**
 * 求两个有序表的交集
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 */
bool Intersection(SqList La, SqList Lb, SqList *Lc) {
    if (Lc == NULL) return false;
    
    int i = 0, j = 0, k = 0;
    
    while (i < La.length && j < Lb.length) {
        int cmp = Compare(La.data[i], Lb.data[j]);
        if (cmp < 0) {
            i++;
        } else if (cmp > 0) {
            j++;
        } else {
            Lc->data[k++] = La.data[i];
            i++;
            j++;
        }
    }
    
    Lc->length = k;
    return true;
}

/**
 * 求两个有序表的并集
 * 时间复杂度：O(m+n)
 * 空间复杂度：O(1)
 */
bool Union(SqList La, SqList Lb, SqList *Lc) {
    if (Lc == NULL) return false;
    
    int i = 0, j = 0, k = 0;
    
    while (i < La.length && j < Lb.length) {
        int cmp = Compare(La.data[i], Lb.data[j]);
        if (cmp < 0) {
            Lc->data[k++] = La.data[i++];
        } else if (cmp > 0) {
            Lc->data[k++] = Lb.data[j++];
        } else {
            Lc->data[k++] = La.data[i];
            i++;
            j++;
        }
    }
    
    // 复制剩余元素
    while (i < La.length) {
        Lc->data[k++] = La.data[i++];
    }
    while (j < Lb.length) {
        Lc->data[k++] = Lb.data[j++];
    }
    
    Lc->length = k;
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
 * 交换两个元素
 */
void Swap(ElemType *a, ElemType *b) {
    if (a != NULL && b != NULL) {
        ElemType temp = *a;
        *a = *b;
        *b = temp;
    }
}

/**
 * 访问函数：打印元素
 */
bool Visit_Print(ElemType e) {
    printf("%d ", e);
    return true;
}

/**
 * 内部辅助函数：逆转数组范围
 */
static void ReverseRange(ElemType arr[], int start, int end) {
    while (start < end) {
        Swap(&arr[start], &arr[end]);
        start++;
        end--;
    }
}

// ============= 性能测试函数实现 =============

/**
 * 基本操作性能测试
 */
void PerformanceTest_Basic(int size) {
    printf("=== 顺序表基本操作性能测试 (规模: %d) ===\n", size);
    
    SqList L;
    InitList(&L);
    
    // 插入性能测试
    clock_t start = clock();
    for (int i = 0; i < size && i < MAXSIZE; i++) {
        ListInsert(&L, L.length + 1, i);
    }
    clock_t end = clock();
    
    double insertTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("插入%d个元素用时：%.4f秒\n", L.length, insertTime);
    
    // 查找性能测试
    start = clock();
    for (int i = 0; i < 1000; i++) {
        LocateElem(L, i % L.length);
    }
    end = clock();
    
    double searchTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("1000次查找用时：%.4f秒\n", searchTime);
    
    // 删除性能测试
    start = clock();
    while (L.length > 0) {
        ElemType e;
        ListDelete(&L, L.length, &e);
    }
    end = clock();
    
    double deleteTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("删除所有元素用时：%.4f秒\n", deleteTime);
}

/**
 * 高级算法性能测试
 */
void PerformanceTest_Advanced(int size) {
    printf("=== 顺序表高级算法性能测试 (规模: %d) ===\n", size);
    
    SqList L;
    InitList(&L);
    
    // 构造测试数据
    for (int i = 0; i < size && i < MAXSIZE; i++) {
        ListInsert(&L, L.length + 1, i);
    }
    
    clock_t start, end;
    
    // 逆转测试
    start = clock();
    ReverseList(&L);
    end = clock();
    printf("逆转操作用时：%.4f秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    
    // 左移测试
    start = clock();
    LeftShift(&L, size / 4);
    end = clock();
    printf("左移操作用时：%.4f秒\n", ((double)(end - start)) / CLOCKS_PER_SEC);
}

/**
 * 静态表vs动态表性能对比
 */
void PerformanceCompare_StaticVsDynamic(int size) {
    printf("=== 静态表vs动态表性能对比 (规模: %d) ===\n", size);
    
    // 静态表测试
    SqList staticList;
    InitList(&staticList);
    
    clock_t start = clock();
    for (int i = 0; i < size && i < MAXSIZE; i++) {
        ListInsert(&staticList, staticList.length + 1, i);
    }
    clock_t end = clock();
    double staticTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 动态表测试
    DynamicList dynamicList;
    InitDynamicList(&dynamicList);
    
    start = clock();
    for (int i = 0; i < size; i++) {
        DynamicListInsert(&dynamicList, dynamicList.length + 1, i);
    }
    end = clock();
    double dynamicTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("静态表插入用时：%.4f秒 (插入%d个元素)\n", staticTime, staticList.length);
    printf("动态表插入用时：%.4f秒 (插入%d个元素)\n", dynamicTime, dynamicList.length);
    
    DestroyDynamicList(&dynamicList);
} 