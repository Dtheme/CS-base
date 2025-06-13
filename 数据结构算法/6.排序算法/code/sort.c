/* sort.c - 排序算法实现文件
 * create by: zw.duan
 * 
 * 数据结构 - 排序算法完整实现
 */

#include "sort.h"

// ============= 全局统计变量 =============
int comparison_count = 0;   // 比较次数计数器
int swap_count = 0;        // 交换次数计数器

// ============= 工具函数实现 =============

/**
 * 交换两个元素
 */
void Swap(int *a, int *b) {
    if (a != b) {  // 避免自己和自己交换
        int temp = *a;
        *a = *b;
        *b = temp;
        swap_count++;
    }
}

/**
 * 比较两个元素（带计数）
 */
bool Compare(int a, int b) {
    comparison_count++;
    return a > b;
}

/**
 * 重置统计计数器
 */
void ResetCounters(void) {
    comparison_count = 0;
    swap_count = 0;
}

/**
 * 打印数组内容
 */
void PrintArray(int arr[], int n, const char* title) {
    printf("%s: ", title);
    for (int i = 0; i < n && i < 20; i++) {  // 最多显示20个元素
        printf("%d ", arr[i]);
    }
    if (n > 20) {
        printf("... (共%d个元素)", n);
    }
    printf("\n");
}

/**
 * 验证数组是否已排序
 */
bool IsSorted(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

/**
 * 生成测试数据
 */
void GenerateTestData(int arr[], int n, DataType type) {
    srand((unsigned int)time(NULL));
    
    switch (type) {
        case RANDOM_DATA:
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % 1000;
            }
            break;
            
        case SORTED_DATA:
            for (int i = 0; i < n; i++) {
                arr[i] = i;
            }
            break;
            
        case REVERSE_DATA:
            for (int i = 0; i < n; i++) {
                arr[i] = n - i - 1;
            }
            break;
            
        case NEARLY_SORTED:
            for (int i = 0; i < n; i++) {
                arr[i] = i;
            }
            // 随机交换少量元素
            for (int i = 0; i < n / 10; i++) {
                int pos1 = rand() % n;
                int pos2 = rand() % n;
                int temp = arr[pos1];
                arr[pos1] = arr[pos2];
                arr[pos2] = temp;
            }
            break;
            
        case DUPLICATE_DATA:
            for (int i = 0; i < n; i++) {
                arr[i] = rand() % 10;  // 只有0-9这10个值
            }
            break;
    }
}

/**
 * 复制数组
 */
void CopyArray(int src[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

// ============= 插入排序类算法实现 =============

/**
 *  直接插入排序 - 数据结构重点算法
 * 
 *   算法过程图示（插入元素5）：
 * 
 * 初始状态：[2, 4, 6, 8, 5, 3, 7, 1]
 *           ↑        ↑
 *         已排序   待插入
 * 
 * 步骤1：保存待插入元素 temp = 5
 * 步骤2：从后向前比较已排序部分
 *   - 8 > 5，8后移：[2, 4, 6, _, 8, 3, 7, 1]
 *   - 6 > 5，6后移：[2, 4, _, 6, 8, 3, 7, 1]
 *   - 4 < 5，找到插入位置
 * 步骤3：插入元素：[2, 4, 5, 6, 8, 3, 7, 1]
 * 
 *  数据结构要点：
 * - 理解插入排序的基本思想
 * - 掌握最好、最坏、平均时间复杂度
 * - 理解稳定性的概念
 * - 能够手工模拟排序过程
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void InsertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int temp = arr[i];  // 保存待插入元素
        int j = i - 1;
        
        // 从后向前查找插入位置
        while (j >= 0 && Compare(arr[j], temp)) {
            arr[j + 1] = arr[j];  // 元素后移
            j--;
        }
        
        arr[j + 1] = temp;  // 插入元素
        if (j + 1 != i) {
            swap_count++;  // 记录插入操作为一次交换
        }
    }
}

/**
 *  折半插入排序 - 数据结构重点算法
 * 
 *   算法优化：
 * - 使用二分查找确定插入位置
 * - 减少比较次数，但移动次数不变
 * - 时间复杂度仍为O(n²)，但常数因子更小
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void BinaryInsertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int temp = arr[i];
        int left = 0, right = i - 1;
        
        // 二分查找插入位置
        while (left <= right) {
            int mid = (left + right) / 2;
            comparison_count++;
            if (arr[mid] > temp) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        // 移动元素
        for (int j = i - 1; j >= left; j--) {
            arr[j + 1] = arr[j];
        }
        
        arr[left] = temp;
        if (left != i) {
            swap_count++;
        }
    }
}

/**
 *  希尔排序 - 数据结构重点算法
 * 
 *   算法思想：
 * - 将数组按增量分组，对每组进行插入排序
 * - 逐步减小增量，最后增量为1
 * - 通过预排序减少插入排序的移动次数
 * 
 * 增量序列示例（Knuth序列）：
 * h = 1, 4, 13, 40, 121, ...
 * h = 3*h + 1
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void ShellSort(int arr[], int n) {
    // 计算初始增量
    int gap = 1;
    while (gap < n / 3) {
        gap = gap * 3 + 1;  // Knuth序列
    }
    
    while (gap >= 1) {
        // 对每个子序列进行插入排序
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            
            while (j >= gap && Compare(arr[j - gap], temp)) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            
            arr[j] = temp;
            if (j != i) {
                swap_count++;
            }
        }
        
        gap /= 3;  // 减小增量
    }
}

// ============= 选择排序类算法实现 =============

/**
 *  简单选择排序 - 数据结构重点算法
 * 
 *   算法过程图示：
 * 
 * 初始：[64, 25, 12, 22, 11]
 * 
 * 第1趟：找最小值11，与第1个元素交换
 * [11, 25, 12, 22, 64]
 *  ↑   已排序区
 * 
 * 第2趟：在剩余元素中找最小值12，与第2个元素交换
 * [11, 12, 25, 22, 64]
 *      ↑   已排序区
 * 
 *  数据结构要点：
 * - 每趟选择最小元素放到已排序区末尾
 * - 交换次数最少，最多n-1次交换
 * - 不稳定排序（相同元素可能改变相对位置）
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void SelectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        
        // 在未排序部分找最小元素
        for (int j = i + 1; j < n; j++) {
            if (Compare(arr[min_idx], arr[j])) {
                min_idx = j;
            }
        }
        
        // 交换最小元素到正确位置
        if (min_idx != i) {
            Swap(&arr[i], &arr[min_idx]);
        }
    }
}

/**
 *  堆排序 - 数据结构重点算法
 * 
 *   算法思想：
 * 1. 建立最大堆
 * 2. 将堆顶（最大元素）与末尾元素交换
 * 3. 调整剩余元素为最大堆
 * 4. 重复步骤2-3
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void HeapSort(int arr[], int n) {
    // 建立最大堆
    BuildMaxHeap(arr, n);
    
    // 逐个取出堆顶元素
    for (int i = n - 1; i > 0; i--) {
        Swap(&arr[0], &arr[i]);  // 将最大元素放到末尾
        Heapify(arr, i, 0);     // 调整剩余元素为堆
    }
}

/**
 * 堆调整函数（向下调整）
 * 
 *   调整过程：
 * - 将节点i与其子节点比较
 * - 如果子节点更大，则交换并继续调整
 * - 直到满足堆性质或到达叶节点
 * 
 * @param arr 数组
 * @param n 堆大小
 * @param i 待调整节点
 */
void Heapify(int arr[], int n, int i) {
    int largest = i;        // 假设父节点最大
    int left = 2 * i + 1;   // 左子节点
    int right = 2 * i + 2;  // 右子节点
    
    // 找出父节点和子节点中的最大值
    if (left < n) {
        comparison_count++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }
    
    if (right < n) {
        comparison_count++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }
    
    // 如果最大值不是父节点，则交换并继续调整
    if (largest != i) {
        Swap(&arr[i], &arr[largest]);
        Heapify(arr, n, largest);
    }
}

/**
 * 建立最大堆
 * 
 *   建堆过程：
 * - 从最后一个非叶节点开始向上调整
 * - 最后一个非叶节点的索引为 (n/2 - 1)
 * 
 * @param arr 数组
 * @param n 数组长度
 */
void BuildMaxHeap(int arr[], int n) {
    // 从最后一个非叶节点开始向上调整
    for (int i = n / 2 - 1; i >= 0; i--) {
        Heapify(arr, n, i);
    }
}

// ============= 交换排序类算法实现 =============

/**
 *  冒泡排序 - 数据结构重点算法
 * 
 *   算法过程图示：
 * 
 * 初始：[64, 34, 25, 12, 22, 11, 90]
 * 
 * 第1趟：相邻元素比较交换，最大元素"冒泡"到末尾
 * [34, 25, 12, 22, 11, 64, 90]
 *                          ↑
 *                      已排序
 * 
 *  数据结构要点：
 * - 相邻元素比较交换
 * - 每趟确定一个最大元素的位置
 * - 稳定排序
 * - 可以提前终止（如果某趟没有交换）
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void BubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;  // 优化：记录是否发生交换
        
        for (int j = 0; j < n - 1 - i; j++) {
            if (Compare(arr[j], arr[j + 1])) {
                Swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        
        // 如果没有发生交换，说明已经有序
        if (!swapped) {
            break;
        }
    }
}

/**
 *  快速排序 - 数据结构重点算法
 * 
 *   算法思想：
 * 1. 选择一个基准元素（pivot）
 * 2. 将数组分为两部分：小于基准和大于基准
 * 3. 递归排序两部分
 * 
 * @param arr 待排序数组
 * @param low 起始位置
 * @param high 结束位置
 */
void QuickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = Partition(arr, low, high);
        QuickSort(arr, low, pivot - 1);
        QuickSort(arr, pivot + 1, high);
    }
}

/**
 * 快速排序分区函数
 * 
 *   分区过程（以最后一个元素为基准）：
 * 
 * 初始：[3, 6, 8, 10, 1, 2, 1] (基准=1)
 *        ↑                 ↑
 *       low               high
 * 
 * 分区后：[1, 1, 3, 6, 8, 10, 2]
 *              ↑
 *           基准位置
 * 
 * @param arr 数组
 * @param low 起始位置
 * @param high 结束位置
 * @return 基准元素的最终位置
 */
int Partition(int arr[], int low, int high) {
    int pivot = arr[high];  // 选择最后一个元素作为基准
    int i = low - 1;        // 小于基准的元素的索引
    
    for (int j = low; j < high; j++) {
        comparison_count++;
        if (arr[j] <= pivot) {
            i++;
            Swap(&arr[i], &arr[j]);
        }
    }
    
    Swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/**
 * 快速排序包装函数
 */
void QuickSortWrapper(int arr[], int n) {
    QuickSort(arr, 0, n - 1);
}

// ============= 归并排序算法实现 =============

/**
 *  归并排序 - 数据结构重点算法
 * 
 *   算法思想：
 * 1. 分治：将数组分为两半
 * 2. 递归排序两半
 * 3. 合并两个有序数组
 * 
 * @param arr 待排序数组
 * @param left 左边界
 * @param right 右边界
 */
void MergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        // 递归排序左半部分
        MergeSort(arr, left, mid);
        
        // 递归排序右半部分
        MergeSort(arr, mid + 1, right);
        
        // 合并两个有序部分
        Merge(arr, left, mid, right);
    }
}

/**
 * 归并函数
 * 
 *   合并过程图示：
 * 
 * 左半部分：[2, 5, 8]    右半部分：[1, 6, 9]
 * 
 * 合并过程：
 * 比较 2 和 1 → 选择 1 → [1, _, _, _, _, _]
 * 比较 2 和 6 → 选择 2 → [1, 2, _, _, _, _]
 * 比较 5 和 6 → 选择 5 → [1, 2, 5, _, _, _]
 * 比较 8 和 6 → 选择 6 → [1, 2, 5, 6, _, _]
 * 剩余元素直接复制 → [1, 2, 5, 6, 8, 9]
 * 
 * @param arr 数组
 * @param left 左边界
 * @param mid 中间位置
 * @param right 右边界
 */
void Merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // 创建临时数组
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));
    
    // 复制数据到临时数组
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    
    // 合并临时数组回到原数组
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        comparison_count++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        swap_count++;  // 记录赋值操作
    }
    
    // 复制剩余元素
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        swap_count++;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        swap_count++;
    }
    
    free(L);
    free(R);
}

/**
 * 归并排序包装函数
 */
void MergeSortWrapper(int arr[], int n) {
    MergeSort(arr, 0, n - 1);
}

// ============= 基数排序算法实现 =============

/**
 *  基数排序 - 数据结构重点算法
 * 
 *   算法思想：
 * - 非比较排序，按位数进行排序
 * - 从最低位开始，逐位进行计数排序
 * - 适用于整数排序
 * 
 * 示例过程（对[170, 45, 75, 90, 2, 802, 24, 66]排序）：
 * 
 * 按个位排序：[170, 90, 2, 802, 24, 45, 75, 66]
 * 按十位排序：[2, 802, 24, 45, 66, 170, 75, 90]
 * 按百位排序：[2, 24, 45, 66, 75, 90, 170, 802]
 * 
 * @param arr 待排序数组
 * @param n 数组长度
 */
void RadixSort(int arr[], int n) {
    // 找到最大值，确定位数
    int max_val = GetMax(arr, n);
    
    // 对每一位进行计数排序
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        CountingSort(arr, n, exp);
    }
}

/**
 * 计数排序（基数排序的子过程）
 * 
 * @param arr 数组
 * @param n 数组长度
 * @param exp 当前位数（1表示个位，10表示十位，等等）
 */
void CountingSort(int arr[], int n, int exp) {
    int *output = (int*)malloc(n * sizeof(int));
    int count[10] = {0};
    
    // 统计每个数字出现的次数
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
        comparison_count++;  // 记录操作次数
    }
    
    // 计算累积计数
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    // 从后向前构建输出数组（保证稳定性）
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
        swap_count++;
    }
    
    // 复制输出数组到原数组
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
    
    free(output);
}

/**
 * 获取数组中的最大值
 */
int GetMax(int arr[], int n) {
    int max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
        comparison_count++;
    }
    return max_val;
}

// ============= 其他排序算法实现 =============

/**
 * 计数排序（直接版本）
 * 
 *   算法特点：
 * - 时间复杂度：O(n + k)，k为数据范围
 * - 空间复杂度：O(k)
 * - 稳定排序
 * - 适用于数据范围较小的情况
 */
void CountingSortDirect(int arr[], int n) {
    if (n <= 1) return;
    
    // 找到最大值和最小值
    int max_val = arr[0], min_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) max_val = arr[i];
        if (arr[i] < min_val) min_val = arr[i];
        comparison_count += 2;
    }
    
    int range = max_val - min_val + 1;
    int *count = (int*)calloc(range, sizeof(int));
    int *output = (int*)malloc(n * sizeof(int));
    
    // 统计每个元素出现的次数
    for (int i = 0; i < n; i++) {
        count[arr[i] - min_val]++;
    }
    
    // 计算累积计数
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }
    
    // 构建输出数组
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i] - min_val] - 1] = arr[i];
        count[arr[i] - min_val]--;
        swap_count++;
    }
    
    // 复制回原数组
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
    
    free(count);
    free(output);
}

/**
 * 桶排序
 * 
 *   算法思想：
 * - 将数据分散到多个桶中
 * - 对每个桶内的数据进行排序
 * - 按顺序合并所有桶
 */
void BucketSort(int arr[], int n) {
    if (n <= 1) return;
    
    // 找到最大值和最小值
    int max_val = arr[0], min_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) max_val = arr[i];
        if (arr[i] < min_val) min_val = arr[i];
        comparison_count += 2;
    }
    
    int bucket_count = n;  // 桶的数量
    int range = max_val - min_val + 1;
    
    // 创建桶
    int **buckets = (int**)malloc(bucket_count * sizeof(int*));
    int *bucket_sizes = (int*)calloc(bucket_count, sizeof(int));
    
    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (int*)malloc(n * sizeof(int));
    }
    
    // 将元素分配到桶中
    for (int i = 0; i < n; i++) {
        int bucket_index = (arr[i] - min_val) * bucket_count / range;
        if (bucket_index >= bucket_count) bucket_index = bucket_count - 1;
        
        buckets[bucket_index][bucket_sizes[bucket_index]++] = arr[i];
    }
    
    // 对每个桶进行排序并合并
    int index = 0;
    for (int i = 0; i < bucket_count; i++) {
        if (bucket_sizes[i] > 0) {
            // 对桶内元素进行插入排序
            for (int j = 1; j < bucket_sizes[i]; j++) {
                int key = buckets[i][j];
                int k = j - 1;
                while (k >= 0 && buckets[i][k] > key) {
                    buckets[i][k + 1] = buckets[i][k];
                    k--;
                    comparison_count++;
                    swap_count++;
                }
                buckets[i][k + 1] = key;
            }
            
            // 将桶中的元素复制回原数组
            for (int j = 0; j < bucket_sizes[i]; j++) {
                arr[index++] = buckets[i][j];
            }
        }
    }
    
    // 释放内存
    for (int i = 0; i < bucket_count; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(bucket_sizes);
} 