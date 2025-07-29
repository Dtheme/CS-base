/* bitree.c - 树状数组（Binary Indexed Tree/Fenwick Tree）实现
 * create by: zw.duan
 */

#include "bitree.h"
#include <assert.h>
#include <time.h>

// ============= 树状数组基本操作实现 =============

/**
 * 创建树状数组
 */
BITree* CreateBITree(int size) {
    if (size <= 0) {
        fprintf(stderr, "错误：树状数组大小必须大于0\n");
        return NULL;
    }
    
    BITree *bit = (BITree*)malloc(sizeof(BITree));
    if (!bit) {
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    bit->tree = (BITreeElemType*)calloc(size + 1, sizeof(BITreeElemType));
    if (!bit->tree) {
        free(bit);
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    bit->size = size;
    bit->capacity = size;
    
    return bit;
}

/**
 * 销毁树状数组
 */
void DestroyBITree(BITree *bit) {
    if (bit) {
        if (bit->tree) {
            free(bit->tree);
        }
        free(bit);
    }
}

/**
 * 初始化树状数组
 */
void InitBITree(BITree *bit, int size) {
    if (!bit || size <= 0) return;
    
    if (bit->tree) {
        free(bit->tree);
    }
    
    bit->tree = (BITreeElemType*)calloc(size + 1, sizeof(BITreeElemType));
    bit->size = size;
    bit->capacity = size;
}

/**
 * 检查树状数组是否为空
 */
bool BITreeEmpty(BITree *bit) {
    return bit == NULL || bit->size == 0;
}

/**
 * 获取树状数组大小
 */
int BITreeSize(BITree *bit) {
    return bit ? bit->size : 0;
}

/**
 * 单点更新操作
 * 时间复杂度：O(log n)
 */
void Update(BITree *bit, int index, BITreeElemType delta) {
    if (!IsValidIndex(bit, index)) {
        fprintf(stderr, "错误：无效的索引 %d\n", index);
        return;
    }
    
    // 树状数组索引从1开始
    index++;
    
    while (index <= bit->size) {
        bit->tree[index] += delta;
        index += LowBit(index);
    }
}

/**
 * 前缀和查询
 * 时间复杂度：O(log n)
 */
BITreeElemType Query(BITree *bit, int index) {
    if (!IsValidIndex(bit, index)) {
        fprintf(stderr, "错误：无效的索引 %d\n", index);
        return 0;
    }
    
    BITreeElemType sum = 0;
    index++; // 树状数组索引从1开始
    
    while (index > 0) {
        sum += bit->tree[index];
        index -= LowBit(index);
    }
    
    return sum;
}

/**
 * 区间和查询 [left, right]
 * 时间复杂度：O(log n)
 */
BITreeElemType QueryRange(BITree *bit, int left, int right) {
    if (!IsValidIndex(bit, left) || !IsValidIndex(bit, right) || left > right) {
        fprintf(stderr, "错误：无效的区间 [%d, %d]\n", left, right);
        return 0;
    }
    
    return Query(bit, right) - Query(bit, left - 1);
}

/**
 * 从数组构建树状数组
 */
void BuildBITree(BITree *bit, BITreeElemType *arr, int size) {
    if (!bit || !arr || size <= 0 || size > bit->size) {
        fprintf(stderr, "错误：无效的参数\n");
        return;
    }
    
    // 清空树状数组
    memset(bit->tree, 0, (bit->size + 1) * sizeof(BITreeElemType));
    
    // 逐个插入元素
    for (int i = 0; i < size; i++) {
        Update(bit, i, arr[i]);
    }
}

/**
 * 重置树状数组
 */
void ResetBITree(BITree *bit) {
    if (bit && bit->tree) {
        memset(bit->tree, 0, (bit->size + 1) * sizeof(BITreeElemType));
    }
}

// ============= 树状数组高级操作实现 =============

/**
 * 区间更新（简单实现）
 */
void UpdateRange(BITree *bit, int left, int right, BITreeElemType delta) {
    if (!IsValidIndex(bit, left) || !IsValidIndex(bit, right) || left > right) {
        fprintf(stderr, "错误：无效的区间 [%d, %d]\n", left, right);
        return;
    }
    
    // 简单实现：逐个更新区间内的每个元素
    for (int i = left; i <= right; i++) {
        Update(bit, i, delta);
    }
}

/**
 * 获取单个元素值
 */
BITreeElemType GetElement(BITree *bit, int index) {
    if (!IsValidIndex(bit, index)) {
        fprintf(stderr, "错误：无效的索引 %d\n", index);
        return 0;
    }
    
    if (index == 0) {
        return Query(bit, 0);
    } else {
        return Query(bit, index) - Query(bit, index - 1);
    }
}

/**
 * 设置单个元素值
 */
void SetElement(BITree *bit, int index, BITreeElemType value) {
    if (!IsValidIndex(bit, index)) {
        fprintf(stderr, "错误：无效的索引 %d\n", index);
        return;
    }
    
    BITreeElemType current = GetElement(bit, index);
    BITreeElemType delta = value - current;
    Update(bit, index, delta);
}

// ============= 统计操作实现 =============

/**
 * 统计小于指定值的元素个数
 */
int CountLessThan(BITree *bit, BITreeElemType value) {
    if (!bit) return 0;
    
    int count = 0;
    for (int i = 0; i < bit->size; i++) {
        if (GetElement(bit, i) < value) {
            count++;
        }
    }
    return count;
}

/**
 * 统计大于指定值的元素个数
 */
int CountGreaterThan(BITree *bit, BITreeElemType value) {
    if (!bit) return 0;
    
    int count = 0;
    for (int i = 0; i < bit->size; i++) {
        if (GetElement(bit, i) > value) {
            count++;
        }
    }
    return count;
}

/**
 * 统计指定范围内的元素个数
 */
int CountInRange(BITree *bit, BITreeElemType min, BITreeElemType max) {
    if (!bit) return 0;
    
    int count = 0;
    for (int i = 0; i < bit->size; i++) {
        BITreeElemType val = GetElement(bit, i);
        if (val >= min && val <= max) {
            count++;
        }
    }
    return count;
}

// ============= 查找操作实现 =============

/**
 * 查找第k小的元素
 */
int FindKthSmallest(BITree *bit, int k) {
    if (!bit || k <= 0 || k > bit->size) {
        return -1;
    }
    
    // 使用二分查找
    int left = 0, right = bit->size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int count = CountLessThan(bit, GetElement(bit, mid)) + 1;
        
        if (count == k) {
            return mid;
        } else if (count < k) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

/**
 * 查找第k大的元素
 */
int FindKthLargest(BITree *bit, int k) {
    if (!bit || k <= 0 || k > bit->size) {
        return -1;
    }
    
    return FindKthSmallest(bit, bit->size - k + 1);
}

/**
 * 查找第一个大于指定值的元素位置
 */
int FindFirstGreaterThan(BITree *bit, BITreeElemType value) {
    if (!bit) return -1;
    
    for (int i = 0; i < bit->size; i++) {
        if (GetElement(bit, i) > value) {
            return i;
        }
    }
    return -1;
}

/**
 * 查找最后一个小于指定值的元素位置
 */
int FindLastLessThan(BITree *bit, BITreeElemType value) {
    if (!bit) return -1;
    
    int last = -1;
    for (int i = 0; i < bit->size; i++) {
        if (GetElement(bit, i) < value) {
            last = i;
        }
    }
    return last;
}

// ============= 逆序对计算实现 =============

/**
 * 计算逆序对数量（使用树状数组）
 */
long long CountInversionsWithBITree(BITree *bit, BITreeElemType *arr, int size) {
    if (!bit || !arr || size <= 0) return 0;
    
    // 离散化处理
    BITreeElemType *sorted = (BITreeElemType*)malloc(size * sizeof(BITreeElemType));
    memcpy(sorted, arr, size * sizeof(BITreeElemType));
    
    // 排序
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                BITreeElemType temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    
    // 创建映射
    int *rank = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (arr[i] == sorted[j]) {
                rank[i] = j + 1;
                break;
            }
        }
    }
    
    // 重置树状数组
    ResetBITree(bit);
    
    long long inversions = 0;
    for (int i = size - 1; i >= 0; i--) {
        inversions += Query(bit, rank[i] - 1);
        Update(bit, rank[i], 1);
    }
    
    free(sorted);
    free(rank);
    return inversions;
}

/**
 * 计算逆序对数量（暴力方法）
 */
long long CountInversions(BITreeElemType *arr, int size) {
    if (!arr || size <= 0) return 0;
    
    long long count = 0;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] > arr[j]) {
                count++;
            }
        }
    }
    return count;
}

// ============= 二维树状数组实现 =============

/**
 * 创建二维树状数组
 */
BITree2D* CreateBITree2D(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        fprintf(stderr, "错误：二维树状数组大小必须大于0\n");
        return NULL;
    }
    
    BITree2D *bit2d = (BITree2D*)malloc(sizeof(BITree2D));
    if (!bit2d) {
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    bit2d->tree = (BITreeElemType**)malloc((rows + 1) * sizeof(BITreeElemType*));
    if (!bit2d->tree) {
        free(bit2d);
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    for (int i = 0; i <= rows; i++) {
        bit2d->tree[i] = (BITreeElemType*)calloc(cols + 1, sizeof(BITreeElemType));
        if (!bit2d->tree[i]) {
            // 清理已分配的内存
            for (int j = 0; j < i; j++) {
                free(bit2d->tree[j]);
            }
            free(bit2d->tree);
            free(bit2d);
            fprintf(stderr, "错误：内存分配失败\n");
            return NULL;
        }
    }
    
    bit2d->rows = rows;
    bit2d->cols = cols;
    
    return bit2d;
}

/**
 * 销毁二维树状数组
 */
void DestroyBITree2D(BITree2D *bit2d) {
    if (bit2d) {
        if (bit2d->tree) {
            for (int i = 0; i <= bit2d->rows; i++) {
                if (bit2d->tree[i]) {
                    free(bit2d->tree[i]);
                }
            }
            free(bit2d->tree);
        }
        free(bit2d);
    }
}

/**
 * 二维树状数组更新
 */
void Update2D(BITree2D *bit2d, int row, int col, BITreeElemType delta) {
    if (!bit2d || row < 0 || row >= bit2d->rows || col < 0 || col >= bit2d->cols) {
        fprintf(stderr, "错误：无效的坐标 (%d, %d)\n", row, col);
        return;
    }
    
    row++; col++; // 树状数组索引从1开始
    
    for (int i = row; i <= bit2d->rows; i += LowBit(i)) {
        for (int j = col; j <= bit2d->cols; j += LowBit(j)) {
            bit2d->tree[i][j] += delta;
        }
    }
}

/**
 * 二维树状数组查询
 */
BITreeElemType Query2D(BITree2D *bit2d, int row, int col) {
    if (!bit2d || row < 0 || row >= bit2d->rows || col < 0 || col >= bit2d->cols) {
        fprintf(stderr, "错误：无效的坐标 (%d, %d)\n", row, col);
        return 0;
    }
    
    BITreeElemType sum = 0;
    row++; col++; // 树状数组索引从1开始
    
    for (int i = row; i > 0; i -= LowBit(i)) {
        for (int j = col; j > 0; j -= LowBit(j)) {
            sum += bit2d->tree[i][j];
        }
    }
    
    return sum;
}

/**
 * 二维区间查询
 */
BITreeElemType QueryRange2D(BITree2D *bit2d, int row1, int col1, int row2, int col2) {
    if (!bit2d || row1 > row2 || col1 > col2) {
        fprintf(stderr, "错误：无效的区间\n");
        return 0;
    }
    
    return Query2D(bit2d, row2, col2) 
           - Query2D(bit2d, row2, col1 - 1) 
           - Query2D(bit2d, row1 - 1, col2) 
           + Query2D(bit2d, row1 - 1, col1 - 1);
}

// ============= 动态区间统计实现 =============

/**
 * 创建动态区间统计器
 */
DynamicRangeCounter* CreateDynamicRangeCounter(int max_value) {
    if (max_value <= 0) {
        fprintf(stderr, "错误：最大值必须大于0\n");
        return NULL;
    }
    
    DynamicRangeCounter *drc = (DynamicRangeCounter*)malloc(sizeof(DynamicRangeCounter));
    if (!drc) {
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    drc->bit = CreateBITree(max_value + 1);
    if (!drc->bit) {
        free(drc);
        return NULL;
    }
    
    drc->freq = (int*)calloc(max_value + 1, sizeof(int));
    if (!drc->freq) {
        DestroyBITree(drc->bit);
        free(drc);
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    drc->max_value = max_value;
    
    return drc;
}

/**
 * 销毁动态区间统计器
 */
void DestroyDynamicRangeCounter(DynamicRangeCounter *drc) {
    if (drc) {
        if (drc->bit) {
            DestroyBITree(drc->bit);
        }
        if (drc->freq) {
            free(drc->freq);
        }
        free(drc);
    }
}

/**
 * 添加元素
 */
void AddElement(DynamicRangeCounter *drc, int value) {
    if (!drc || value < 0 || value > drc->max_value) {
        fprintf(stderr, "错误：无效的值 %d\n", value);
        return;
    }
    
    drc->freq[value]++;
    Update(drc->bit, value, 1);
}

/**
 * 移除元素
 */
void RemoveElement(DynamicRangeCounter *drc, int value) {
    if (!drc || value < 0 || value > drc->max_value || drc->freq[value] <= 0) {
        fprintf(stderr, "错误：无效的值或元素不存在 %d\n", value);
        return;
    }
    
    drc->freq[value]--;
    Update(drc->bit, value, -1);
}

/**
 * 获取元素频率
 */
int GetFrequency(DynamicRangeCounter *drc, int value) {
    if (!drc || value < 0 || value > drc->max_value) {
        return 0;
    }
    
    return drc->freq[value];
}

/**
 * 获取区间频率
 */
int GetRangeFrequency(DynamicRangeCounter *drc, int min, int max) {
    if (!drc || min > max || min < 0 || max > drc->max_value) {
        return 0;
    }
    
    return QueryRange(drc->bit, min, max);
}

// ============= 离线查询处理实现 =============

/**
 * 处理离线查询
 */
void ProcessOfflineQueries(BITreeElemType *arr, int size, OfflineQuery *queries, int query_count) {
    if (!arr || !queries || size <= 0 || query_count <= 0) {
        fprintf(stderr, "错误：无效的参数\n");
        return;
    }
    
    BITree *bit = CreateBITree(size);
    if (!bit) {
        fprintf(stderr, "错误：创建树状数组失败\n");
        return;
    }
    
    // 构建树状数组
    BuildBITree(bit, arr, size);
    
    // 处理查询
    for (int i = 0; i < query_count; i++) {
        queries[i].result = QueryRange(bit, queries[i].left, queries[i].right);
    }
    
    DestroyBITree(bit);
}

// ============= 工具函数实现 =============

/**
 * 打印树状数组
 */
void PrintBITree(BITree *bit) {
    if (!bit) {
        printf("树状数组为空\n");
        return;
    }
    
    printf("树状数组 (大小: %d):\n", bit->size);
    printf("索引: ");
    for (int i = 0; i < bit->size; i++) {
        printf("%4d ", i);
    }
    printf("\n");
    
    printf("值:   ");
    for (int i = 0; i < bit->size; i++) {
        printf("%4d ", GetElement(bit, i));
    }
    printf("\n");
    
    printf("前缀和: ");
    for (int i = 0; i < bit->size; i++) {
        printf("%4d ", Query(bit, i));
    }
    printf("\n");
}

/**
 * 打印树状数组结构
 */
void PrintBITreeStructure(BITree *bit) {
    if (!bit) {
        printf("树状数组为空\n");
        return;
    }
    
    printf("树状数组内部结构:\n");
    for (int i = 1; i <= bit->size; i++) {
        printf("tree[%d] = %d\n", i, bit->tree[i]);
    }
}

/**
 * 验证树状数组
 */
void ValidateBITree(BITree *bit) {
    if (!bit) {
        printf("树状数组为空\n");
        return;
    }
    
    printf("验证树状数组...\n");
    
    // 检查基本属性
    if (bit->size <= 0) {
        printf("❌ 错误：大小必须大于0\n");
        return;
    }
    
    if (!bit->tree) {
        printf("❌ 错误：树状数组指针为空\n");
        return;
    }
    
    printf("✅ 基本属性验证通过\n");
    
    // 检查前缀和一致性
    BITreeElemType sum = 0;
    for (int i = 0; i < bit->size; i++) {
        BITreeElemType element = GetElement(bit, i);
        sum += element;
        if (sum != Query(bit, i)) {
            printf("❌ 错误：前缀和不一致，索引 %d\n", i);
            return;
        }
    }
    
    printf("✅ 前缀和一致性验证通过\n");
    printf("✅ 树状数组验证完成\n");
}

// ============= 性能测试实现 =============

/**
 * 树状数组性能测试
 */
void PerformanceTest_BITree(int size) {
    printf("=== 树状数组性能测试 (大小: %d) ===\n", size);
    
    BITree *bit = CreateBITree(size);
    if (!bit) {
        printf("❌ 创建树状数组失败\n");
        return;
    }
    
    clock_t start, end;
    double cpu_time_used;
    
    // 测试更新性能
    start = clock();
    for (int i = 0; i < size; i++) {
        Update(bit, i, rand() % 100);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("更新操作耗时: %.6f 秒\n", cpu_time_used);
    
    // 测试查询性能
    start = clock();
    for (int i = 0; i < size; i++) {
        Query(bit, i);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("查询操作耗时: %.6f 秒\n", cpu_time_used);
    
    // 测试区间查询性能
    start = clock();
    for (int i = 0; i < size / 10; i++) {
        int left = rand() % size;
        int right = left + rand() % (size - left);
        QueryRange(bit, left, right);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("区间查询耗时: %.6f 秒\n", cpu_time_used);
    
    DestroyBITree(bit);
    printf("=== 性能测试完成 ===\n\n");
}

/**
 * 树状数组与普通数组性能比较
 */
void PerformanceCompare_BITree_vs_Array(int size) {
    printf("=== 树状数组 vs 普通数组性能比较 (大小: %d) ===\n", size);
    
    BITree *bit = CreateBITree(size);
    BITreeElemType *arr = (BITreeElemType*)calloc(size, sizeof(BITreeElemType));
    
    if (!bit || !arr) {
        printf("❌ 内存分配失败\n");
        return;
    }
    
    clock_t start, end;
    double bit_time, array_time;
    
    // 初始化数据
    for (int i = 0; i < size; i++) {
        BITreeElemType val = rand() % 100;
        Update(bit, i, val);
        arr[i] = val;
    }
    
    // 测试前缀和查询
    start = clock();
    for (int i = 0; i < size; i++) {
        Query(bit, i);
    }
    end = clock();
    bit_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    for (int i = 0; i < size; i++) {
        BITreeElemType sum = 0;
        for (int j = 0; j <= i; j++) {
            sum += arr[j];
        }
    }
    end = clock();
    array_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("前缀和查询:\n");
    printf("  树状数组: %.6f 秒\n", bit_time);
    printf("  普通数组: %.6f 秒\n", array_time);
    printf("  加速比: %.2fx\n", array_time / bit_time);
    
    // 测试区间更新
    start = clock();
    for (int i = 0; i < size / 10; i++) {
        int left = rand() % size;
        int right = left + rand() % (size - left);
        UpdateRange(bit, left, right, rand() % 10);
    }
    end = clock();
    bit_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    for (int i = 0; i < size / 10; i++) {
        int left = rand() % size;
        int right = left + rand() % (size - left);
        for (int j = left; j <= right; j++) {
            arr[j] += rand() % 10;
        }
    }
    end = clock();
    array_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("区间更新:\n");
    printf("  树状数组: %.6f 秒\n", bit_time);
    printf("  普通数组: %.6f 秒\n", array_time);
    printf("  加速比: %.2fx\n", array_time / bit_time);
    
    DestroyBITree(bit);
    free(arr);
    printf("=== 性能比较完成 ===\n\n");
}

/**
 * 二维树状数组性能测试
 */
void PerformanceTest_2D_BITree(int rows, int cols) {
    printf("=== 二维树状数组性能测试 (%dx%d) ===\n", rows, cols);
    
    BITree2D *bit2d = CreateBITree2D(rows, cols);
    if (!bit2d) {
        printf("❌ 创建二维树状数组失败\n");
        return;
    }
    
    clock_t start, end;
    double cpu_time_used;
    
    // 测试更新性能
    start = clock();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Update2D(bit2d, i, j, rand() % 100);
        }
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("更新操作耗时: %.6f 秒\n", cpu_time_used);
    
    // 测试查询性能
    start = clock();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Query2D(bit2d, i, j);
        }
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("查询操作耗时: %.6f 秒\n", cpu_time_used);
    
    DestroyBITree2D(bit2d);
    printf("=== 二维性能测试完成 ===\n\n");
}

// ============= 内存管理实现 =============

/**
 * 调整树状数组大小
 */
void ResizeBITree(BITree *bit, int new_size) {
    if (!bit || new_size <= 0) {
        fprintf(stderr, "错误：无效的参数\n");
        return;
    }
    
    if (new_size <= bit->capacity) {
        bit->size = new_size;
        return;
    }
    
    // 重新分配内存
    BITreeElemType *new_tree = (BITreeElemType*)calloc(new_size + 1, sizeof(BITreeElemType));
    if (!new_tree) {
        fprintf(stderr, "错误：内存分配失败\n");
        return;
    }
    
    // 复制原有数据
    if (bit->tree) {
        memcpy(new_tree, bit->tree, (bit->size + 1) * sizeof(BITreeElemType));
        free(bit->tree);
    }
    
    bit->tree = new_tree;
    bit->size = new_size;
    bit->capacity = new_size;
}

/**
 * 清空树状数组
 */
void ClearBITree(BITree *bit) {
    if (bit && bit->tree) {
        memset(bit->tree, 0, (bit->size + 1) * sizeof(BITreeElemType));
    }
}

// ============= 序列化实现 =============

/**
 * 序列化树状数组
 */
void SerializeBITree(BITree *bit, char *filename) {
    if (!bit || !filename) {
        fprintf(stderr, "错误：无效的参数\n");
        return;
    }
    
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "错误：无法打开文件 %s\n", filename);
        return;
    }
    
    // 写入基本信息
    fwrite(&bit->size, sizeof(int), 1, file);
    fwrite(&bit->capacity, sizeof(int), 1, file);
    
    // 写入树状数组数据
    fwrite(bit->tree, sizeof(BITreeElemType), bit->size + 1, file);
    
    fclose(file);
    printf("✅ 树状数组已序列化到 %s\n", filename);
}

/**
 * 反序列化树状数组
 */
BITree* DeserializeBITree(char *filename) {
    if (!filename) {
        fprintf(stderr, "错误：无效的文件名\n");
        return NULL;
    }
    
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "错误：无法打开文件 %s\n", filename);
        return NULL;
    }
    
    BITree *bit = (BITree*)malloc(sizeof(BITree));
    if (!bit) {
        fclose(file);
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    // 读取基本信息
    fread(&bit->size, sizeof(int), 1, file);
    fread(&bit->capacity, sizeof(int), 1, file);
    
    // 分配内存并读取数据
    bit->tree = (BITreeElemType*)malloc((bit->size + 1) * sizeof(BITreeElemType));
    if (!bit->tree) {
        free(bit);
        fclose(file);
        fprintf(stderr, "错误：内存分配失败\n");
        return NULL;
    }
    
    fread(bit->tree, sizeof(BITreeElemType), bit->size + 1, file);
    
    fclose(file);
    printf("✅ 树状数组已从 %s 反序列化\n", filename);
    return bit;
} 