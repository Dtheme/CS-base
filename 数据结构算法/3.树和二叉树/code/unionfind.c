/* unionfind.c - 并查集
 * create by: zw.duan
 */

#include "unionfind.h"
#include <time.h>
#include <math.h>

// ============= 并查集基本操作 =============

/**
 * 初始化并查集
 * 时间复杂度: O(n)
 * 空间复杂度: O(n)
 */
bool InitUnionFind(UnionFind *uf, int n) {
    if (uf == NULL || n <= 0) {
        return false;
    }
    
    uf->parent = (int *)malloc(n * sizeof(int));
    uf->rank = (int *)malloc(n * sizeof(int));
    uf->size = (int *)malloc(n * sizeof(int));
    
    if (uf->parent == NULL || uf->rank == NULL || uf->size == NULL) {
        DestroyUnionFind(uf);
        return false;
    }
    
    uf->capacity = n;
    uf->count = n;
    
    // 初始化：每个节点都是自己的父节点
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;      // 初始时每个节点的父节点是自己
        uf->rank[i] = 0;        // 初始秩为0
        uf->size[i] = 1;        // 初始集合大小为1
    }
    
    return true;
}

/**
 * 销毁并查集
 */
void DestroyUnionFind(UnionFind *uf) {
    if (uf == NULL) return;
    
    free(uf->parent);
    free(uf->rank);
    free(uf->size);
    
    uf->parent = NULL;
    uf->rank = NULL;
    uf->size = NULL;
    uf->capacity = 0;
    uf->count = 0;
}

/**
 * 清空并查集（重新初始化）
 */
void ClearUnionFind(UnionFind *uf) {
    if (uf == NULL || uf->parent == NULL) return;
    
    int n = uf->capacity;
    uf->count = n;
    
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
        uf->size[i] = 1;
    }
}

/**
 * 查找操作（带路径压缩）
 * 时间复杂度: O(α(n))，其中α是反阿克曼函数
 */
int Find(UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->capacity) {
        return -1;
    }
    
    // 路径压缩：将路径上的所有节点直接连接到根节点
    if (uf->parent[x] != x) {
        uf->parent[x] = Find(uf, uf->parent[x]);
    }
    
    return uf->parent[x];
}

/**
 * 查找操作（不带路径压缩，用于性能对比）
 * 时间复杂度: O(树的高度)
 */
int FindWithoutCompression(UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->capacity) {
        return -1;
    }
    
    while (uf->parent[x] != x) {
        x = uf->parent[x];
    }
    
    return x;
}

/**
 * 合并操作（按秩合并）
 * 时间复杂度: O(α(n))
 */
bool Union(UnionFind *uf, int x, int y) {
    return UnionByRank(uf, x, y);
}

/**
 * 按秩合并
 */
bool UnionByRank(UnionFind *uf, int x, int y) {
    if (uf == NULL || x < 0 || x >= uf->capacity || y < 0 || y >= uf->capacity) {
        return false;
    }
    
    int rootX = Find(uf, x);
    int rootY = Find(uf, y);
    
    if (rootX == rootY) {
        return false;  // 已经在同一个集合中
    }
    
    // 按秩合并：将秩小的树合并到秩大的树上
    if (uf->rank[rootX] < uf->rank[rootY]) {
        uf->parent[rootX] = rootY;
        uf->size[rootY] += uf->size[rootX];
    } else if (uf->rank[rootX] > uf->rank[rootY]) {
        uf->parent[rootY] = rootX;
        uf->size[rootX] += uf->size[rootY];
    } else {
        uf->parent[rootY] = rootX;
        uf->size[rootX] += uf->size[rootY];
        uf->rank[rootX]++;  // 只有在秩相等时才增加秩
    }
    
    uf->count--;  // 连通分量减少1
    return true;
}

/**
 * 按大小合并
 */
bool UnionBySize(UnionFind *uf, int x, int y) {
    if (uf == NULL || x < 0 || x >= uf->capacity || y < 0 || y >= uf->capacity) {
        return false;
    }
    
    int rootX = Find(uf, x);
    int rootY = Find(uf, y);
    
    if (rootX == rootY) {
        return false;
    }
    
    // 按大小合并：将小树合并到大树上
    if (uf->size[rootX] < uf->size[rootY]) {
        uf->parent[rootX] = rootY;
        uf->size[rootY] += uf->size[rootX];
    } else {
        uf->parent[rootY] = rootX;
        uf->size[rootX] += uf->size[rootY];
    }
    
    uf->count--;
    return true;
}

/**
 * 判断两个元素是否连通
 */
bool Connected(UnionFind *uf, int x, int y) {
    if (uf == NULL || x < 0 || x >= uf->capacity || y < 0 || y >= uf->capacity) {
        return false;
    }
    
    return Find(uf, x) == Find(uf, y);
}

/**
 * 获取元素所在集合的大小
 */
int GetComponentSize(UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->capacity) {
        return -1;
    }
    
    int root = Find(uf, x);
    return uf->size[root];
}

/**
 * 获取连通分量的数量
 */
int GetComponentCount(UnionFind *uf) {
    if (uf == NULL) return -1;
    return uf->count;
}

/**
 * 获取并查集的容量
 */
int GetCapacity(UnionFind *uf) {
    if (uf == NULL) return -1;
    return uf->capacity;
}

// ============= 并查集高级操作 =============

/**
 * 获取路径长度
 */
int GetPathLength(UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->capacity) {
        return -1;
    }
    
    int length = 0;
    int current = x;
    
    while (uf->parent[current] != current) {
        current = uf->parent[current];
        length++;
    }
    
    return length;
}

/**
 * 获取到根节点的路径
 */
void GetPath(UnionFind *uf, int x, int *path, int *length) {
    if (uf == NULL || x < 0 || x >= uf->capacity || path == NULL || length == NULL) {
        if (length) *length = 0;
        return;
    }
    
    *length = 0;
    int current = x;
    
    while (current != uf->parent[current]) {
        path[(*length)++] = current;
        current = uf->parent[current];
    }
    path[(*length)++] = current;  // 加入根节点
}

/**
 * 获取最大深度
 */
int GetMaxDepth(UnionFind *uf) {
    if (uf == NULL) return -1;
    
    int maxDepth = 0;
    for (int i = 0; i < uf->capacity; i++) {
        int depth = GetPathLength(uf, i);
        if (depth > maxDepth) {
            maxDepth = depth;
        }
    }
    
    return maxDepth;
}

/**
 * 获取连通分量信息
 */
void GetComponentInfo(UnionFind *uf, int **components, int **sizes, int *count) {
    if (uf == NULL || components == NULL || sizes == NULL || count == NULL) {
        return;
    }
    
    *count = 0;
    bool *visited = (bool *)calloc(uf->capacity, sizeof(bool));
    
    // 临时数组
    int *tempComponents = (int *)malloc(uf->capacity * sizeof(int));
    int *tempSizes = (int *)malloc(uf->capacity * sizeof(int));
    
    for (int i = 0; i < uf->capacity; i++) {
        int root = Find(uf, i);
        if (!visited[root]) {
            tempComponents[*count] = root;
            tempSizes[*count] = uf->size[root];
            visited[root] = true;
            (*count)++;
        }
    }
    
    // 分配实际需要的内存
    *components = (int *)malloc(*count * sizeof(int));
    *sizes = (int *)malloc(*count * sizeof(int));
    
    for (int i = 0; i < *count; i++) {
        (*components)[i] = tempComponents[i];
        (*sizes)[i] = tempSizes[i];
    }
    
    free(visited);
    free(tempComponents);
    free(tempSizes);
}

/**
 * 获取最大连通分量大小
 */
int GetLargestComponentSize(UnionFind *uf) {
    if (uf == NULL) return -1;
    
    int maxSize = 0;
    bool *visited = (bool *)calloc(uf->capacity, sizeof(bool));
    
    for (int i = 0; i < uf->capacity; i++) {
        int root = Find(uf, i);
        if (!visited[root]) {
            if (uf->size[root] > maxSize) {
                maxSize = uf->size[root];
            }
            visited[root] = true;
        }
    }
    
    free(visited);
    return maxSize;
}

/**
 * 获取最小连通分量大小
 */
int GetSmallestComponentSize(UnionFind *uf) {
    if (uf == NULL) return -1;
    
    int minSize = uf->capacity + 1;
    bool *visited = (bool *)calloc(uf->capacity, sizeof(bool));
    
    for (int i = 0; i < uf->capacity; i++) {
        int root = Find(uf, i);
        if (!visited[root]) {
            if (uf->size[root] < minSize) {
                minSize = uf->size[root];
            }
            visited[root] = true;
        }
    }
    
    free(visited);
    return (minSize == uf->capacity + 1) ? 0 : minSize;
}

/**
 * 判断是否只有一个连通分量
 */
bool IsSingleComponent(UnionFind *uf) {
    if (uf == NULL) return false;
    return uf->count == 1;
}

/**
 * 获取所有根节点
 */
void GetAllRoots(UnionFind *uf, int *roots, int *count) {
    if (uf == NULL || roots == NULL || count == NULL) return;
    
    *count = 0;
    bool *visited = (bool *)calloc(uf->capacity, sizeof(bool));
    
    for (int i = 0; i < uf->capacity; i++) {
        int root = Find(uf, i);
        if (!visited[root]) {
            roots[(*count)++] = root;
            visited[root] = true;
        }
    }
    
    free(visited);
}

/**
 * 判断是否为根节点
 */
bool IsRoot(UnionFind *uf, int x) {
    if (uf == NULL || x < 0 || x >= uf->capacity) {
        return false;
    }
    
    return Find(uf, x) == x;
}

// ============= 并查集应用 =============

/**
 * 判断图是否连通
 */
bool IsGraphConnected(int **edges, int edgeCount, int vertexCount) {
    if (edges == NULL || edgeCount < 0 || vertexCount <= 0) {
        return false;
    }
    
    UnionFind uf;
    if (!InitUnionFind(&uf, vertexCount)) {
        return false;
    }
    
    for (int i = 0; i < edgeCount; i++) {
        Union(&uf, edges[i][0], edges[i][1]);
    }
    
    bool connected = GetComponentCount(&uf) == 1;
    DestroyUnionFind(&uf);
    
    return connected;
}

/**
 * 计算连通分量个数
 */
int CountConnectedComponents(int **edges, int edgeCount, int vertexCount) {
    if (edges == NULL || edgeCount < 0 || vertexCount <= 0) {
        return -1;
    }
    
    UnionFind uf;
    if (!InitUnionFind(&uf, vertexCount)) {
        return -1;
    }
    
    for (int i = 0; i < edgeCount; i++) {
        Union(&uf, edges[i][0], edges[i][1]);
    }
    
    int count = GetComponentCount(&uf);
    DestroyUnionFind(&uf);
    
    return count;
}

/**
 * 朋友圈问题
 */
int FindCircleNum(int **matrix, int n) {
    if (matrix == NULL || n <= 0) {
        return 0;
    }
    
    UnionFind uf;
    if (!InitUnionFind(&uf, n)) {
        return -1;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j] == 1) {
                Union(&uf, i, j);
            }
        }
    }
    
    int count = GetComponentCount(&uf);
    DestroyUnionFind(&uf);
    
    return count;
}

/**
 * 岛屿数量问题（网格版本）
 */
int NumIslandsUnionFind(char **grid, int rows, int cols) {
    if (grid == NULL || rows <= 0 || cols <= 0) {
        return 0;
    }
    
    UnionFind uf;
    if (!InitUnionFind(&uf, rows * cols)) {
        return -1;
    }
    
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int waterCount = 0;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '1') {
                // 检查四个方向
                for (int d = 0; d < 4; d++) {
                    int ni = i + directions[d][0];
                    int nj = j + directions[d][1];
                    
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && grid[ni][nj] == '1') {
                        Union(&uf, i * cols + j, ni * cols + nj);
                    }
                }
            } else {
                waterCount++;
            }
        }
    }
    
    int totalComponents = GetComponentCount(&uf);
    int islands = totalComponents - waterCount;
    
    DestroyUnionFind(&uf);
    return islands;
}

// ============= 辅助函数 =============

/**
 * 打印并查集状态
 */
void PrintUnionFind(UnionFind *uf) {
    if (uf == NULL) {
        printf("并查集为空\n");
        return;
    }
    
    printf("并查集状态 (容量: %d, 连通分量: %d):\n", uf->capacity, uf->count);
    printf("节点:   ");
    for (int i = 0; i < uf->capacity; i++) {
        printf("%3d ", i);
    }
    printf("\n父节点: ");
    for (int i = 0; i < uf->capacity; i++) {
        printf("%3d ", uf->parent[i]);
    }
    printf("\n秩:     ");
    for (int i = 0; i < uf->capacity; i++) {
        printf("%3d ", uf->rank[i]);
    }
    printf("\n大小:   ");
    for (int i = 0; i < uf->capacity; i++) {
        printf("%3d ", uf->size[i]);
    }
    printf("\n");
}

/**
 * 打印并查集结构
 */
void PrintUnionFindStructure(UnionFind *uf) {
    if (uf == NULL) return;
    
    printf("\n并查集树结构:\n");
    for (int i = 0; i < uf->capacity; i++) {
        if (uf->parent[i] == i) {  // 根节点
            printf("根节点 %d (大小: %d): ", i, uf->size[i]);
            
            // 查找所有属于这个根的节点
            for (int j = 0; j < uf->capacity; j++) {
                if (Find(uf, j) == i) {
                    printf("%d ", j);
                }
            }
            printf("\n");
        }
    }
}

/**
 * 打印连通分量信息
 */
void PrintComponentInfo(UnionFind *uf) {
    if (uf == NULL) return;
    
    int *components, *sizes, count;
    GetComponentInfo(uf, &components, &sizes, &count);
    
    printf("\n连通分量信息:\n");
    printf("总共 %d 个连通分量:\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("  分量 %d: 根节点 %d, 大小 %d\n", 
               i + 1, components[i], sizes[i]);
    }
    
    printf("最大分量大小: %d\n", GetLargestComponentSize(uf));
    printf("最小分量大小: %d\n", GetSmallestComponentSize(uf));
    
    free(components);
    free(sizes);
}

/**
 * 验证并查集的正确性
 */
bool ValidateUnionFind(UnionFind *uf) {
    if (uf == NULL || uf->parent == NULL) {
        return false;
    }
    
    // 检查每个节点的父节点是否有效
    for (int i = 0; i < uf->capacity; i++) {
        if (uf->parent[i] < 0 || uf->parent[i] >= uf->capacity) {
            return false;
        }
    }
    
    // 检查连通分量数量是否正确
    int actualCount = 0;
    bool *visited = (bool *)calloc(uf->capacity, sizeof(bool));
    
    for (int i = 0; i < uf->capacity; i++) {
        int root = Find(uf, i);
        if (!visited[root]) {
            actualCount++;
            visited[root] = true;
        }
    }
    
    free(visited);
    return actualCount == uf->count;
}

// ============= 性能测试 =============

/**
 * 并查集操作性能测试
 */
void PerformanceTest_UnionFindOps(int size) {
    printf("\n=== 并查集性能测试 (规模: %d) ===\n", size);
    
    UnionFind uf;
    if (!InitUnionFind(&uf, size)) {
        printf("初始化失败\n");
        return;
    }
    
    clock_t start, end;
    
    // 测试随机Union操作
    start = clock();
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < size; i++) {
        int x = rand() % size;
        int y = rand() % size;
        Union(&uf, x, y);
    }
    
    end = clock();
    printf("随机Union操作 (%d次): %.2f毫秒\n", 
           size, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    // 测试Find操作
    start = clock();
    
    for (int i = 0; i < size; i++) {
        Find(&uf, i);
    }
    
    end = clock();
    printf("Find操作 (%d次): %.2f毫秒\n", 
           size, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    printf("最终连通分量数: %d\n", GetComponentCount(&uf));
    printf("最大树深度: %d\n", GetMaxDepth(&uf));
    
    DestroyUnionFind(&uf);
}

/**
 * 路径压缩性能对比测试
 */
void PerformanceTest_FindWithVsWithoutCompression(int size) {
    printf("\n=== 路径压缩性能对比测试 (规模: %d) ===\n", size);
    
    UnionFind uf1, uf2;
    InitUnionFind(&uf1, size);
    InitUnionFind(&uf2, size);
    
    // 构建相同的结构（链式结构，最坏情况）
    for (int i = 1; i < size; i++) {
        Union(&uf1, i-1, i);
        Union(&uf2, i-1, i);
    }
    
    clock_t start, end;
    
    // 测试不带路径压缩的Find
    start = clock();
    for (int i = 0; i < 1000; i++) {
        FindWithoutCompression(&uf1, size - 1);
    }
    end = clock();
    printf("不带路径压缩: %.2f毫秒\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    // 测试带路径压缩的Find
    start = clock();
    for (int i = 0; i < 1000; i++) {
        Find(&uf2, size - 1);
    }
    end = clock();
    printf("带路径压缩: %.2f毫秒\n", 
           ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    
    DestroyUnionFind(&uf1);
    DestroyUnionFind(&uf2);
} 