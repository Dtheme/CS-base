/* search.c - 查找算法完整实现
 * create by: zw.duan
 * 
 * 实现数据结构中的所有查找算法：
 * 1. 顺序查找（线性查找）- 基础算法
 * 2. 二分查找（折半查找）- 重点算法  
 * 3. 插值查找 - 改进算法
 * 4. 斐波那契查找 - 特殊算法
 * 5. 分块查找（索引顺序查找）- 混合算法
 */

#include "search.h"
#include <time.h>

// ============= 工具函数实现 =============

/**
 * 获取当前时间（毫秒）
 */
double GetCurrentTime(void) {
    return (double)clock() / CLOCKS_PER_SEC * 1000.0;
}

/**
 * 生成斐波那契数列
 */
void GenerateFibonacci(int fib[], int n) {
    if (n >= 1) fib[0] = 1;
    if (n >= 2) fib[1] = 1;
    
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }
}

/**
 * 数组排序（快速排序）
 */
void QuickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = Partition(arr, low, high);
        QuickSort(arr, low, pivot - 1);
        QuickSort(arr, pivot + 1, high);
    }
}

int Partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

void SortArray(int arr[], int n) {
    QuickSort(arr, 0, n - 1);
}

/**
 * 检查数组是否有序
 */
bool IsArraySorted(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

/**
 * 打印数组
 */
void PrintArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

/**
 * 复制数组
 */
void CopyArray(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// ============= 基本查找算法实现 =============

/**
 *  顺序查找（线性查找）- 数据结构基础算法
 * 
 *   算法过程详解：
 * 输入：数组arr[0..n-1]，查找关键字key
 * 输出：SearchResult结构，包含位置、比较次数、是否找到、耗时
 * 
 *  数据结构要点：
 * - 时间复杂度：O(n)
 * - 空间复杂度：O(1)
 * - 成功查找ASL = (n+1)/2
 * - 失败查找ASL = n
 * - 适用于任何存储结构
 * 
 * @param arr 待查找数组
 * @param n 数组长度
 * @param key 查找关键字
 * @return SearchResult 查找结果
 */
SearchResult SequentialSearch(int arr[], int n, int key) {
    SearchResult result = {-1, 0, false, 0.0};
    
    double start_time = GetCurrentTime();
    
    // 顺序扫描数组
    for (int i = 0; i < n; i++) {
        result.comparisons++;
        if (arr[i] == key) {
            result.index = i;
            result.found = true;
            break;
        }
    }
    
    double end_time = GetCurrentTime();
    result.time_cost = end_time - start_time;
    
    return result;
}

/**
 *  带哨兵的顺序查找 - 数据结构优化技巧
 * 
 *   哨兵优化原理：
 * - 在数组末尾设置哨兵元素（待查找的key）
 * - 避免每次循环都检查数组边界
 * - 减少判断次数，提高效率
 * 
 *  数据结构要点：
 * - 减少循环中的边界检查
 * - 时间复杂度仍为O(n)，但常数因子更小
 * - 需要额外的存储空间存放哨兵
 * 
 * @param arr 待查找数组（需要预留哨兵位置）
 * @param n 数组长度
 * @param key 查找关键字
 * @return SearchResult 查找结果
 */
SearchResult SequentialSearchWithSentinel(int arr[], int n, int key) {
    SearchResult result = {-1, 0, false, 0.0};
    
    double start_time = GetCurrentTime();
    
    // 保存原来的最后一个元素
    int last_element = arr[n];
    // 设置哨兵
    arr[n] = key;
    
    int i = 0;
    // 不需要检查边界，因为哨兵保证能找到
    while (arr[i] != key) {
        i++;
        result.comparisons++;
    }
    
    // 恢复原来的最后一个元素
    arr[n] = last_element;
    
    // 判断是否真正找到（不是哨兵）
    if (i < n) {
        result.index = i;
        result.found = true;
    }
    result.comparisons++; // 最后一次比较
    
    double end_time = GetCurrentTime();
    result.time_cost = end_time - start_time;
    
    return result;
}

/**
 *  二分查找（折半查找）- 数据结构重点算法
 * 
 *   算法过程详解：
 * 1. 设置左右边界：low=0, high=n-1
 * 2. 计算中点：mid = (low + high) / 2
 * 3. 比较arr[mid]与key：
 *    - 相等：找到，返回mid
 *    - arr[mid] < key：在右半部分查找，low = mid + 1
 *    - arr[mid] > key：在左半部分查找，high = mid - 1
 * 4. 重复直到找到或low > high
 * 
 *  数据结构要点：
 * - 前提：数组必须有序
 * - 时间复杂度：O(log n)
 * - 空间复杂度：O(1)
 * - 成功查找ASL ≈ log₂(n+1) - 1
 * - 最多比较次数：⌊log₂n⌋ + 1
 * 
 * @param arr 有序数组
 * @param n 数组长度
 * @param key 查找关键字
 * @return SearchResult 查找结果
 */
SearchResult BinarySearch(int arr[], int n, int key) {
    SearchResult result = {-1, 0, false, 0.0};
    
    double start_time = GetCurrentTime();
    
    int low = 0, high = n - 1;
    
    while (low <= high) {
        result.comparisons++;
        int mid = low + (high - low) / 2;  // 防止溢出
        
        if (arr[mid] == key) {
            result.index = mid;
            result.found = true;
            break;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    double end_time = GetCurrentTime();
    result.time_cost = end_time - start_time;
    
    return result;
}

/**
 *  递归版本的二分查找 - 数据结构算法变形
 * 
 * @param arr 有序数组
 * @param low 查找范围下界
 * @param high 查找范围上界
 * @param key 查找关键字
 * @param comparisons 比较次数统计
 * @return 查找位置（-1表示未找到）
 */
int BinarySearchRecursive(int arr[], int low, int high, int key, int* comparisons) {
    if (low > high) {
        return -1;  // 查找失败
    }
    
    (*comparisons)++;
    int mid = low + (high - low) / 2;
    
    if (arr[mid] == key) {
        return mid;  // 查找成功
    } else if (arr[mid] < key) {
        return BinarySearchRecursive(arr, mid + 1, high, key, comparisons);
    } else {
        return BinarySearchRecursive(arr, low, mid - 1, key, comparisons);
    }
}

/**
 *  插值查找 - 数据结构改进算法
 * 
 *   算法改进思想：
 * - 二分查找：mid = (low + high) / 2
 * - 插值查找：mid = low + (key - arr[low]) / (arr[high] - arr[low]) * (high - low)
 * 
 *  算法特点：
 * - 根据关键字的值来确定查找位置
 * - 适用于数据分布均匀的有序数组
 * - 平均时间复杂度：O(log log n)
 * - 最坏时间复杂度：O(n)
 * 
 *  数据结构要点：
 * - 理解插值公式的推导
 * - 掌握适用条件：数据分布均匀
 * - 了解退化情况：分布不均匀时性能下降
 * 
 * @param arr 有序数组
 * @param n 数组长度
 * @param key 查找关键字
 * @return SearchResult 查找结果
 */
SearchResult InterpolationSearch(int arr[], int n, int key) {
    SearchResult result = {-1, 0, false, 0.0};
    
    double start_time = GetCurrentTime();
    
    int low = 0, high = n - 1;
    
    while (low <= high && key >= arr[low] && key <= arr[high]) {
        result.comparisons++;
        
        // 如果数组元素都相等，避免除零错误
        if (arr[high] == arr[low]) {
            if (arr[low] == key) {
                result.index = low;
                result.found = true;
            }
            break;
        }
        
        // 插值公式计算查找位置
        int pos = low + (double)(key - arr[low]) / (arr[high] - arr[low]) * (high - low);
        
        // 确保pos在有效范围内
        if (pos < low) pos = low;
        if (pos > high) pos = high;
        
        if (arr[pos] == key) {
            result.index = pos;
            result.found = true;
            break;
        } else if (arr[pos] < key) {
            low = pos + 1;
        } else {
            high = pos - 1;
        }
    }
    
    double end_time = GetCurrentTime();
    result.time_cost = end_time - start_time;
    
    return result;
}

/**
 *  斐波那契查找 - 数据结构特殊算法
 * 
 *   算法原理：
 * - 利用斐波那契数列的特性来分割数组
 * - 分割点不是中点，而是斐波那契数列的分割点
 * - 避免了除法运算，只使用加法和减法
 * 
 *  算法特点：
 * - 时间复杂度：O(log n)
 * - 空间复杂度：O(log n)（存储斐波那契数列）
 * - 适用于数据量大且不能随机访问的情况
 * - 分割比例接近黄金分割比
 * 
 * @param arr 有序数组
 * @param n 数组长度
 * @param key 查找关键字
 * @return SearchResult 查找结果
 */
SearchResult FibonacciSearch(int arr[], int n, int key) {
    SearchResult result = {-1, 0, false, 0.0};
    
    double start_time = GetCurrentTime();
    
    // 生成斐波那契数列
    int fib[50];  // 足够大的数组存储斐波那契数列
    GenerateFibonacci(fib, 50);
    
    // 找到第一个大于等于n的斐波那契数
    int k = 0;
    while (k < 50 && fib[k] < n) k++;
    
    if (k >= 50) {
        // 数组太大，回退到二分查找
        double end_time = GetCurrentTime();
        result.time_cost = end_time - start_time;
        return result;
    }
    
    int offset = -1;  // 偏移量
    
    while (fib[k] > 1) {
        result.comparisons++;
        
        // 计算分割点
        int i = offset + fib[k-2];
        
        // 确保i在有效范围内
        if (i >= n) {
            i = n - 1;
        }
        
        if (arr[i] == key) {
            result.index = i;
            result.found = true;
            break;
        } else if (arr[i] < key) {
            // 在右半部分查找
            offset = i;
            k = k - 1;
        } else {
            // 在左半部分查找
            k = k - 2;
        }
    }
    
    // 检查最后一个元素
    if (!result.found && offset + 1 < n) {
        result.comparisons++;
        if (arr[offset + 1] == key) {
            result.index = offset + 1;
            result.found = true;
        }
    }
    
    double end_time = GetCurrentTime();
    result.time_cost = end_time - start_time;
    
    return result;
}

// ============= 分块查找实现 =============

/**
 * 初始化分块查找表
 */
void InitBlockTable(BlockSearchTable* table) {
    table->block_count = 0;
    table->data_count = 0;
    
    for (int i = 0; i < MAXBLOCKS; i++) {
        table->blocks[i].max_key = -1;
        table->blocks[i].start_index = -1;
        table->blocks[i].length = 0;
    }
}

/**
 * 向分块表中插入元素
 */
bool InsertIntoBlock(BlockSearchTable* table, int key) {
    if (table->data_count >= MAXSIZE) {
        return false;  // 表已满
    }
    
    // 插入到数据数组
    table->data[table->data_count] = key;
    table->data_count++;
    
    // 更新块信息（简化实现：每BLOCKSIZE个元素一块）
    int block_index = (table->data_count - 1) / BLOCKSIZE;
    
    if (block_index >= table->block_count) {
        // 创建新块
        table->blocks[block_index].start_index = block_index * BLOCKSIZE;
        table->blocks[block_index].length = 1;
        table->blocks[block_index].max_key = key;
        table->block_count++;
    } else {
        // 更新现有块
        table->blocks[block_index].length++;
        if (key > table->blocks[block_index].max_key) {
            table->blocks[block_index].max_key = key;
        }
    }
    
    return true;
}

/**
 *  分块查找（索引顺序查找）- 数据结构混合算法
 * 
 *   算法思想：
 * - 将数据分成若干块，块间有序，块内可以无序
 * - 建立索引表，记录每块的最大关键字和起始位置
 * - 先在索引表中查找确定块，再在块内顺序查找
 * 
 *  数据结构要点：
 * - 时间复杂度：O(√n)
 * - 空间复杂度：O(√n)（索引表）
 * - ASL = L_I + L_S（索引查找长度 + 块内查找长度）
 * - 适用于数据相对稳定的情况
 * 
 * @param table 分块查找表
 * @param key 查找关键字
 * @return SearchResult 查找结果
 */
SearchResult BlockSearch(BlockSearchTable* table, int key) {
    SearchResult result = {-1, 0, false, 0.0};
    
    double start_time = GetCurrentTime();
    
    // 第一步：在索引表中查找确定块
    int block_index = -1;
    for (int i = 0; i < table->block_count; i++) {
        result.comparisons++;
        if (key <= table->blocks[i].max_key) {
            block_index = i;
            break;
        }
    }
    
    if (block_index == -1) {
        // 关键字大于所有块的最大值，查找失败
        double end_time = GetCurrentTime();
        result.time_cost = end_time - start_time;
        return result;
    }
    
    // 第二步：在确定的块内顺序查找
    Block* block = &table->blocks[block_index];
    int start = block->start_index;
    int end = start + block->length;
    
    for (int i = start; i < end && i < table->data_count; i++) {
        result.comparisons++;
        if (table->data[i] == key) {
            result.index = i;
            result.found = true;
            break;
        }
    }
    
    double end_time = GetCurrentTime();
    result.time_cost = end_time - start_time;
    
    return result;
}

/**
 * 打印分块查找表
 */
void PrintBlockTable(BlockSearchTable* table) {
    printf("分块查找表信息:\n");
    printf("总数据量: %d, 块数: %d\n", table->data_count, table->block_count);
    
    printf("\n索引表:\n");
    for (int i = 0; i < table->block_count; i++) {
        printf("块%d: 最大值=%d, 起始位置=%d, 长度=%d\n", 
               i, table->blocks[i].max_key, 
               table->blocks[i].start_index, table->blocks[i].length);
    }
    
    printf("\n数据数组:\n");
    for (int i = 0; i < table->data_count; i++) {
        printf("%d ", table->data[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    if (table->data_count % 10 != 0) printf("\n");
}

// ============= 统计和工具函数实现 =============

/**
 * 初始化查找统计信息
 */
void InitSearchStats(SearchStats* stats) {
    stats->total_searches = 0;
    stats->successful_searches = 0;
    stats->total_comparisons = 0;
    stats->total_time = 0.0;
    stats->avg_comparisons = 0.0;
    stats->success_rate = 0.0;
}

/**
 * 更新查找统计信息
 */
void UpdateSearchStats(SearchStats* stats, SearchResult result) {
    stats->total_searches++;
    stats->total_comparisons += result.comparisons;
    stats->total_time += result.time_cost;
    
    if (result.found) {
        stats->successful_searches++;
    }
    
    stats->avg_comparisons = (double)stats->total_comparisons / stats->total_searches;
    stats->success_rate = (double)stats->successful_searches / stats->total_searches * 100.0;
}

/**
 * 打印查找统计信息
 */
void PrintSearchStats(SearchStats* stats) {
    printf("查找统计信息:\n");
    printf("  总查找次数: %d\n", stats->total_searches);
    printf("  成功查找次数: %d\n", stats->successful_searches);
    printf("  总比较次数: %d\n", stats->total_comparisons);
    printf("  平均比较次数: %.2f\n", stats->avg_comparisons);
    printf("  成功率: %.2f%%\n", stats->success_rate);
    printf("  总耗时: %.4f ms\n", stats->total_time);
    printf("  平均耗时: %.4f ms\n", stats->total_time / stats->total_searches);
}

/**
 * 生成随机数组
 */
void GenerateRandomArray(int arr[], int n, int min_val, int max_val) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = min_val + rand() % (max_val - min_val + 1);
    }
}

/**
 * 生成有序数组
 */
void GenerateOrderedArray(int arr[], int n, int start_val, int step) {
    for (int i = 0; i < n; i++) {
        arr[i] = start_val + i * step;
    }
}

/**
 * 测试查找算法性能
 */
void TimeSearchAlgorithm(SearchResult (*search_func)(int[], int, int), 
                        int arr[], int n, int key, const char* algorithm_name) {
    printf("\n=== %s 性能测试 ===\n", algorithm_name);
    
    SearchResult result = search_func(arr, n, key);
    
    printf("查找关键字: %d\n", key);
    printf("查找结果: %s\n", result.found ? "找到" : "未找到");
    if (result.found) {
        printf("位置: %d\n", result.index);
    }
    printf("比较次数: %d\n", result.comparisons);
    printf("耗时: %.4f ms\n", result.time_cost);
} 