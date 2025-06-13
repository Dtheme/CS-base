/* graph_algorithms.c - 图算法实现
 *   最小生成树和最短路径算法
 * create by: zw.duan
 */

#include "graph.h"

// ============= 优先队列实现（用于Dijkstra和Prim算法）=============

/**
 * 初始化优先队列
 * @param pq 优先队列指针
 */
void init_priority_queue(PriorityQueue *pq) {
    if (pq) {
        pq->size = 0;
    }
}

/**
 * 检查优先队列是否为空
 * @param pq 优先队列指针
 * @return 空返回true，非空返回false
 */
bool is_priority_queue_empty(PriorityQueue *pq) {
    return pq && pq->size == 0;
}

/**
 * 向优先队列插入元素（最小堆）
 * @param pq 优先队列指针
 * @param vertex 顶点索引
 * @param priority 优先级（权重）
 */
void priority_queue_insert(PriorityQueue *pq, int vertex, int priority) {
    if (!pq || pq->size >= MAX_VERTICES) return;
    
    int i = pq->size;
    pq->data[i].vertex = vertex;
    pq->data[i].priority = priority;
    pq->size++;
    
    // 向上调整（最小堆）
    while (i > 0 && pq->data[i].priority < pq->data[(i - 1) / 2].priority) {
        PriorityNode temp = pq->data[i];
        pq->data[i] = pq->data[(i - 1) / 2];
        pq->data[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

/**
 * 从优先队列中提取最小元素
 * @param pq 优先队列指针
 * @param vertex 输出顶点索引
 * @param priority 输出优先级
 * @return 成功返回true，失败返回false
 */
bool priority_queue_extract_min(PriorityQueue *pq, int *vertex, int *priority) {
    if (!pq || pq->size == 0 || !vertex || !priority) return false;
    
    *vertex = pq->data[0].vertex;
    *priority = pq->data[0].priority;
    
    pq->size--;
    pq->data[0] = pq->data[pq->size];
    
    // 向下调整（最小堆）
    int i = 0;
    while (2 * i + 1 < pq->size) {
        int min_child = 2 * i + 1;
        if (2 * i + 2 < pq->size && 
            pq->data[2 * i + 2].priority < pq->data[min_child].priority) {
            min_child = 2 * i + 2;
        }
        
        if (pq->data[i].priority <= pq->data[min_child].priority) {
            break;
        }
        
        PriorityNode temp = pq->data[i];
        pq->data[i] = pq->data[min_child];
        pq->data[min_child] = temp;
        i = min_child;
    }
    
    return true;
}

/**
 * 减少优先队列中某个顶点的键值
 * @param pq 优先队列指针
 * @param vertex 顶点索引
 * @param new_priority 新的优先级
 */
void priority_queue_decrease_key(PriorityQueue *pq, int vertex, int new_priority) {
    if (!pq) return;
    
    // 找到顶点在队列中的位置
    int i = -1;
    for (int j = 0; j < pq->size; j++) {
        if (pq->data[j].vertex == vertex) {
            i = j;
            break;
        }
    }
    
    if (i == -1 || new_priority >= pq->data[i].priority) return;
    
    pq->data[i].priority = new_priority;
    
    // 向上调整
    while (i > 0 && pq->data[i].priority < pq->data[(i - 1) / 2].priority) {
        PriorityNode temp = pq->data[i];
        pq->data[i] = pq->data[(i - 1) / 2];
        pq->data[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// ============= 并查集实现（用于Kruskal算法）=============

/**
 * 初始化并查集
 * @param uf 并查集指针
 * @param size 元素个数
 */
void init_union_find(UnionFindSet *uf, int size) {
    if (!uf) return;
    
    for (int i = 0; i < size; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
}

/**
 * 查找元素所属的集合（路径压缩）
 * @param uf 并查集指针
 * @param x 元素
 * @return 根节点
 */
int find_set(UnionFindSet *uf, int x) {
    if (!uf || x < 0 || x >= MAX_VERTICES) return -1;
    
    if (uf->parent[x] != x) {
        uf->parent[x] = find_set(uf, uf->parent[x]); // 路径压缩
    }
    return uf->parent[x];
}

/**
 * 合并两个集合（按秩合并）
 * @param uf 并查集指针
 * @param x 元素1
 * @param y 元素2
 * @return 成功返回true，失败返回false
 */
bool union_sets(UnionFindSet *uf, int x, int y) {
    if (!uf) return false;
    
    int root_x = find_set(uf, x);
    int root_y = find_set(uf, y);
    
    if (root_x == root_y) return false; // 已在同一集合
    
    // 按秩合并
    if (uf->rank[root_x] < uf->rank[root_y]) {
        uf->parent[root_x] = root_y;
    } else if (uf->rank[root_x] > uf->rank[root_y]) {
        uf->parent[root_y] = root_x;
    } else {
        uf->parent[root_y] = root_x;
        uf->rank[root_x]++;
    }
    
    return true;
}

// ============= 最小生成树算法实现 =============

/**
 *  Prim算法 - 邻接矩阵版本（贪心策略求最小生成树）
 * 
 *  算法原理：
 * Prim算法是1957年由Robert C. Prim发现的。它在加权连通图里搜索最小生成树。
 * 意即由此算法搜索到的边子集所构成的树中，不但包括了连通图里的所有顶点，
 * 且其所有边的权重之和亦为最小。算法采用贪心策略，从任意一个顶点开始，
 * 每次选择连接已选顶点集合与未选顶点集合之间权重最小的边。
 * 
 *  核心思想：
 * - 维护两个顶点集合：已在MST中的顶点集合S，未在MST中的顶点集合V-S
 * - 每次选择连接S和V-S之间权重最小的安全边
 * - 逐步扩大MST，直到包含所有顶点
 * 
 *  算法步骤：
 * 1. 初始化：选择任意顶点作为起始点，加入MST
 * 2. 维护key数组：key[v]表示连接v到MST的最小边权重
 * 3. 从未在MST中的顶点选择key值最小的顶点u
 * 4. 将u加入MST，添加相应的边
 * 5. 更新与u相邻的所有顶点的key值
 * 6. 重复步骤3-5，直到所有顶点都在MST中
 * 
 *   复杂度分析：
 * - 时间复杂度：O(V²) （邻接矩阵 + 线性搜索最小key）
 * - 空间复杂度：O(V) （key数组 + parent数组 + in_mst数组）
 * - 优化版本（优先队列）：O(E log V)
 * 
 * 🌲 算法执行示例：
 * 图：    A --2-- B --3-- C
 *        / \      |      /
 *       6   4     1     5
 *      /     \    |    /
 *     E --7-- D --┘---┘
 * 
 * 执行过程（从A开始）：
 * 初始: MST={}, key=[0,∞,∞,∞,∞]
 * 第1步: 选择A, MST={A}, 更新key=[0,2,∞,4,6]
 * 第2步: 选择B, MST={A,B}, 边A-B(2), 更新key=[0,2,3,1,6]
 * 第3步: 选择D, MST={A,B,D}, 边B-D(1), 更新key=[0,2,3,1,7]
 * 第4步: 选择C, MST={A,B,D,C}, 边B-C(3), 更新key=[0,2,3,1,7]
 * 第5步: 选择E, MST={A,B,D,C,E}, 边A-E(6)
 * 
 * 最终MST: A-B(2), B-D(1), B-C(3), A-E(6), 总权重=12
 * 
 * 🆚 Prim vs Kruskal比较：
 * - Prim：顶点导向，适合稠密图，时间复杂度O(V²)
 * - Kruskal：边导向，适合稀疏图，时间复杂度O(E log E)
 * - Prim：需要连通图，Kruskal：可处理森林
 * 
 *  
 * - 理解贪心选择性质：局部最优导致全局最优
 * - 掌握cut定理：横跨cut的最小权重边是安全边
 * - MST性质：V个顶点的MST恰好有V-1条边
 * - 算法正确性：基于cut性质和贪心选择性质
 * - 与Dijkstra的区别：Prim关注边权重，Dijkstra关注路径长度
 * 
 * @param graph 图指针（邻接矩阵表示，无向连通图）
 * @param mst 最小生成树边数组（输出参数）
 * @return 最小生成树的边数（成功返回V-1，失败返回0）
 */
int prim_matrix(MatrixGraph *graph, Edge mst[]) {
    if (!graph || !mst || graph->vertex_count < 2) {
        printf("错误：图为空或顶点数不足\n");
        return 0;
    }
    
    if (graph->is_directed) {
        printf("警告：有向图不适用于最小生成树算法\n");
    }
    
    printf("\n========== Prim算法（邻接矩阵）==========\n");
    
    bool in_mst[MAX_VERTICES] = {false};  // 标记顶点是否已在MST中
    int key[MAX_VERTICES];                // key[i]表示连接顶点i到MST的最小边权重
    int parent[MAX_VERTICES];             // parent[i]表示顶点i在MST中的父节点
    
    // 步骤1：初始化所有顶点
    for (int i = 0; i < graph->vertex_count; i++) {
        key[i] = INFINITY;      // 初始距离设为无穷大
        parent[i] = -1;         // 初始无父节点
    }
    
    key[0] = 0; // 步骤2：选择顶点0作为起始点，距离设为0
    int mst_edge_count = 0;
    
    // 步骤3：主循环，每次向MST添加一个顶点
    for (int count = 0; count < graph->vertex_count; count++) {
        // 步骤3.1：贪心选择 - 找到key值最小且不在MST中的顶点
        int u = -1;
        for (int v = 0; v < graph->vertex_count; v++) {
            if (!in_mst[v] && (u == -1 || key[v] < key[u])) {
                u = v;  // 选择距离MST最近的顶点
            }
        }
        
        // 检查图的连通性
        if (u == -1 || key[u] == INFINITY) {
            printf("图不连通，无法构成最小生成树\n");
            break;
        }
        
        // 步骤3.2：将选中的顶点加入MST
        in_mst[u] = true;
        
        // 步骤3.3：添加边到MST（除了第一个顶点）
        if (parent[u] != -1) {
            mst[mst_edge_count].from = parent[u];
            mst[mst_edge_count].to = u;
            mst[mst_edge_count].weight = key[u];
            mst_edge_count++;
            
            printf("添加边：%s(%d) -> %s(%d)，权重：%d\n",
                   graph->vertices[parent[u]], parent[u],
                   graph->vertices[u], u, key[u]);
        }
        
        // 步骤3.4：更新操作 - 更新与u相邻的所有未在MST中的顶点的key值
        for (int v = 0; v < graph->vertex_count; v++) {
            // 如果存在边u-v，v不在MST中，且通过u到v的距离更短
            if (graph->adj_matrix[u][v] != NO_EDGE && !in_mst[v] &&
                graph->adj_matrix[u][v] < key[v]) {
                key[v] = graph->adj_matrix[u][v];  // 更新最小边权重
                parent[v] = u;                     // 更新父节点
            }
        }
    }
    
    printf("Prim算法完成，MST边数：%d\n", mst_edge_count);
    return mst_edge_count;
}

/**
 * Prim算法 - 邻接表版本
 * @param graph 图指针
 * @param mst 最小生成树边数组
 * @return 最小生成树的边数
 */
int prim_list(ListGraph *graph, Edge mst[]) {
    if (!graph || !mst || graph->vertex_count < 2) {
        printf("错误：图为空或顶点数不足\n");
        return 0;
    }
    
    if (graph->is_directed) {
        printf("警告：有向图不适用于最小生成树算法\n");
    }
    
    printf("\n========== Prim算法（邻接表）==========\n");
    
    bool in_mst[MAX_VERTICES] = {false};
    int key[MAX_VERTICES];
    int parent[MAX_VERTICES];
    
    // 初始化
    for (int i = 0; i < graph->vertex_count; i++) {
        key[i] = INFINITY;
        parent[i] = -1;
    }
    
    key[0] = 0; // 从顶点0开始
    int mst_edge_count = 0;
    
    for (int count = 0; count < graph->vertex_count; count++) {
        // 找到key值最小且不在MST中的顶点
        int u = -1;
        for (int v = 0; v < graph->vertex_count; v++) {
            if (!in_mst[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }
        
        if (u == -1 || key[u] == INFINITY) {
            printf("图不连通，无法构成最小生成树\n");
            break;
        }
        
        in_mst[u] = true;
        
        // 添加边到MST（除了第一个顶点）
        if (parent[u] != -1) {
            mst[mst_edge_count].from = parent[u];
            mst[mst_edge_count].to = u;
            mst[mst_edge_count].weight = key[u];
            mst_edge_count++;
            
            printf("添加边：%s(%d) -> %s(%d)，权重：%d\n",
                   graph->vertex_list[parent[u]].name, parent[u],
                   graph->vertex_list[u].name, u, key[u]);
        }
        
        // 更新相邻顶点的key值
        EdgeNode *edge = graph->vertex_list[u].first_edge;
        while (edge) {
            int v = edge->vertex;
            if (!in_mst[v] && edge->weight < key[v]) {
                key[v] = edge->weight;
                parent[v] = u;
            }
            edge = edge->next;
        }
    }
    
    printf("Prim算法完成，MST边数：%d\n", mst_edge_count);
    return mst_edge_count;
}

/**
 * 边排序比较函数（用于Kruskal算法）
 */
static int compare_edges(const void *a, const void *b) {
    Edge *edge_a = (Edge*)a;
    Edge *edge_b = (Edge*)b;
    return edge_a->weight - edge_b->weight;
}

/**
 * Kruskal算法 - 邻接矩阵版本
 * @param graph 图指针
 * @param mst 最小生成树边数组
 * @return 最小生成树的边数
 */
int kruskal_matrix(MatrixGraph *graph, Edge mst[]) {
    if (!graph || !mst || graph->vertex_count < 2) {
        printf("错误：图为空或顶点数不足\n");
        return 0;
    }
    
    if (graph->is_directed) {
        printf("警告：有向图不适用于最小生成树算法\n");
    }
    
    printf("\n========== Kruskal算法（邻接矩阵）==========\n");
    
    // 收集所有边
    Edge *edges = (Edge*)malloc(graph->edge_count * sizeof(Edge));
    if (!edges) {
        printf("错误：内存分配失败\n");
        return 0;
    }
    
    int edge_count = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        for (int j = i + 1; j < graph->vertex_count; j++) { // 无向图只考虑上三角
            if (graph->adj_matrix[i][j] != NO_EDGE) {
                edges[edge_count].from = i;
                edges[edge_count].to = j;
                edges[edge_count].weight = graph->adj_matrix[i][j];
                edge_count++;
            }
        }
    }
    
    // 按权重排序
    qsort(edges, edge_count, sizeof(Edge), compare_edges);
    
    // 初始化并查集
    UnionFindSet uf;
    init_union_find(&uf, graph->vertex_count);
    
    int mst_edge_count = 0;
    printf("边按权重排序：\n");
    
    for (int i = 0; i < edge_count && mst_edge_count < graph->vertex_count - 1; i++) {
        Edge current_edge = edges[i];
        printf("考虑边：%s(%d) -> %s(%d)，权重：%d",
               graph->vertices[current_edge.from], current_edge.from,
               graph->vertices[current_edge.to], current_edge.to,
               current_edge.weight);
        
        // 检查是否会形成环
        if (find_set(&uf, current_edge.from) != find_set(&uf, current_edge.to)) {
            union_sets(&uf, current_edge.from, current_edge.to);
            mst[mst_edge_count] = current_edge;
            mst_edge_count++;
            printf(" -> 接受\n");
        } else {
            printf(" -> 拒绝（会形成环）\n");
        }
    }
    
    free(edges);
    printf("Kruskal算法完成，MST边数：%d\n", mst_edge_count);
    return mst_edge_count;
}

/**
 * Kruskal算法 - 邻接表版本
 * @param graph 图指针
 * @param mst 最小生成树边数组
 * @return 最小生成树的边数
 */
int kruskal_list(ListGraph *graph, Edge mst[]) {
    if (!graph || !mst || graph->vertex_count < 2) {
        printf("错误：图为空或顶点数不足\n");
        return 0;
    }
    
    if (graph->is_directed) {
        printf("警告：有向图不适用于最小生成树算法\n");
    }
    
    printf("\n========== Kruskal算法（邻接表）==========\n");
    
    // 收集所有边
    Edge *edges = (Edge*)malloc(graph->edge_count * sizeof(Edge));
    if (!edges) {
        printf("错误：内存分配失败\n");
        return 0;
    }
    
    int edge_count = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        EdgeNode *current = graph->vertex_list[i].first_edge;
        while (current) {
            // 无向图避免重复边
            if (i < current->vertex) {
                edges[edge_count].from = i;
                edges[edge_count].to = current->vertex;
                edges[edge_count].weight = current->weight;
                edge_count++;
            }
            current = current->next;
        }
    }
    
    // 按权重排序
    qsort(edges, edge_count, sizeof(Edge), compare_edges);
    
    // 初始化并查集
    UnionFindSet uf;
    init_union_find(&uf, graph->vertex_count);
    
    int mst_edge_count = 0;
    printf("边按权重排序：\n");
    
    for (int i = 0; i < edge_count && mst_edge_count < graph->vertex_count - 1; i++) {
        Edge current_edge = edges[i];
        printf("考虑边：%s(%d) -> %s(%d)，权重：%d",
               graph->vertex_list[current_edge.from].name, current_edge.from,
               graph->vertex_list[current_edge.to].name, current_edge.to,
               current_edge.weight);
        
        // 检查是否会形成环
        if (find_set(&uf, current_edge.from) != find_set(&uf, current_edge.to)) {
            union_sets(&uf, current_edge.from, current_edge.to);
            mst[mst_edge_count] = current_edge;
            mst_edge_count++;
            printf(" -> 接受\n");
        } else {
            printf(" -> 拒绝（会形成环）\n");
        }
    }
    
    free(edges);
    printf("Kruskal算法完成，MST边数：%d\n", mst_edge_count);
    return mst_edge_count;
}

/**
 * 计算最小生成树的总权重
 * @param mst 最小生成树边数组
 * @param edge_count 边数
 * @return 总权重
 */
int calculate_mst_weight(Edge mst[], int edge_count) {
    int total_weight = 0;
    for (int i = 0; i < edge_count; i++) {
        total_weight += mst[i].weight;
    }
    return total_weight;
}

/**
 * 打印最小生成树
 * @param mst 最小生成树边数组
 * @param edge_count 边数
 * @param total_weight 总权重
 */
void print_mst(Edge mst[], int edge_count, int total_weight) {
    printf("\n========== 最小生成树 ==========\n");
    printf("边数：%d\n", edge_count);
    printf("总权重：%d\n", total_weight);
    printf("边列表：\n");
    
    for (int i = 0; i < edge_count; i++) {
        printf("  边%d：顶点%d -> 顶点%d，权重：%d\n",
               i + 1, mst[i].from, mst[i].to, mst[i].weight);
    }
    printf("===============================\n");
}

/**
 * 验证最小生成树的正确性
 * @param mst 最小生成树边数组
 * @param edge_count 边数
 * @param vertex_count 顶点数
 * @return 有效返回true，无效返回false
 */
bool validate_mst(Edge mst[], int edge_count, int vertex_count) {
    if (!mst || edge_count != vertex_count - 1) {
        printf("MST验证失败：边数不正确\n");
        return false;
    }
    
    // 使用并查集检查连通性
    UnionFindSet uf;
    init_union_find(&uf, vertex_count);
    
    for (int i = 0; i < edge_count; i++) {
        if (find_set(&uf, mst[i].from) == find_set(&uf, mst[i].to)) {
            printf("MST验证失败：存在环\n");
            return false;
        }
        union_sets(&uf, mst[i].from, mst[i].to);
    }
    
    // 检查是否所有顶点都连通
    int root = find_set(&uf, 0);
    for (int i = 1; i < vertex_count; i++) {
        if (find_set(&uf, i) != root) {
            printf("MST验证失败：不是连通的\n");
            return false;
        }
    }
    
    printf("MST验证成功\n");
    return true;
}

// ============= 最短路径算法实现 =============

/**
 *  Dijkstra算法 - 邻接矩阵版本（贪心策略求单源最短路径）
 * 
 *  算法原理：
 * Dijkstra算法是由荷兰计算机科学家Edsger Dijkstra在1959年提出的算法。
 * 用于在加权图中查找从单个源点到所有其他顶点的最短路径。算法基于贪心策略，
 * 每次选择距离源点最近的未访问顶点，并更新其邻接顶点的距离。
 * 
 *  适用条件：
 * - 适用于有权图（权值非负）
 * - 不能处理负权边（负权边请使用Bellman-Ford算法）
 * - 可处理有向图和无向图
 * 
 *  算法步骤：
 * 1. 初始化：源点距离为0，其他顶点距离为∞
 * 2. 选择当前距离最小且未访问的顶点u
 * 3. 标记顶点u为已访问
 * 4. 更新顶点u的所有邻接顶点v的距离：
 *    如果 dist[u] + weight(u,v) < dist[v]，则更新dist[v]
 * 5. 重复步骤2-4，直到所有可达顶点被访问
 * 
 *   复杂度分析：
 * - 时间复杂度：O(V²) （邻接矩阵 + 线性搜索最小值）
 * - 空间复杂度：O(V) （距离数组 + 访问标记数组）
 * - 使用优先队列优化：O((V+E)logV)
 * 
 *  算法执行示例：
 * 图：    A --2-- B
 *        / \      |
 *       4   1     3
 *      /     \    |
 *     D --5-- C --┘
 * 
 * 从A开始的执行过程：
 * 初始化: dist=[0,∞,∞,∞], visited=[F,F,F,F]
 * 第1轮: u=A, dist=[0,2,1,4], visited=[T,F,F,F]
 * 第2轮: u=C, dist=[0,2,1,4], visited=[T,F,T,F] (更新B: 1+3=4 > 2)
 * 第3轮: u=B, dist=[0,2,1,4], visited=[T,T,T,F] (无更新)
 * 第4轮: u=D, dist=[0,2,1,4], visited=[T,T,T,T] (无更新)
 * 
 * 最终结果：A到各点最短距离 = [0,2,1,4]
 * 
 *  
 * - 理解贪心策略：每次选择当前最优解
 * - 掌握松弛操作：dist[v] = min(dist[v], dist[u] + weight(u,v))
 * - 时间复杂度分析：O(V²)主要由于V次寻找最小距离顶点
 * - 与BFS的区别：BFS适用于无权图，Dijkstra适用于有权图
 * - 无法处理负权边的原因：贪心策略假设已选择的顶点距离确定
 * 
 * @param graph 图指针（邻接矩阵表示，权值非负）
 * @param source 源顶点索引
 * @param distances 距离数组（输出参数，存储到各顶点的最短距离）
 * @param predecessors 前驱数组（输出参数，用于路径重构）
 * @return 成功返回true，失败返回false
 */
bool dijkstra_matrix(MatrixGraph *graph, int source, int distances[], int predecessors[]) {
    if (!graph || source < 0 || source >= graph->vertex_count || 
        !distances || !predecessors) {
        printf("错误：参数无效\n");
        return false;
    }
    
    printf("\n========== Dijkstra算法（邻接矩阵）==========\n");
    printf("源顶点：%s(%d)\n", graph->vertices[source], source);
    
    bool visited[MAX_VERTICES] = {false};
    
    // 初始化距离和前驱
    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INFINITY;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    
    for (int count = 0; count < graph->vertex_count; count++) {
        // 找到距离最小且未访问的顶点
        int u = -1;
        for (int v = 0; v < graph->vertex_count; v++) {
            if (!visited[v] && (u == -1 || distances[v] < distances[u])) {
                u = v;
            }
        }
        
        if (u == -1 || distances[u] == INFINITY) {
            break; // 剩余顶点不可达
        }
        
        visited[u] = true;
        printf("访问顶点：%s(%d)，距离：%d\n", 
               graph->vertices[u], u, distances[u]);
        
        // 更新相邻顶点的距离
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->adj_matrix[u][v] != NO_EDGE && !visited[v]) {
                int new_dist = distances[u] + graph->adj_matrix[u][v];
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    predecessors[v] = u;
                    printf("  更新%s(%d)距离：%d -> %d\n",
                           graph->vertices[v], v, 
                           distances[v] - graph->adj_matrix[u][v], distances[v]);
                }
            }
        }
    }
    
    printf("Dijkstra算法完成\n");
    return true;
}

/**
 * Dijkstra算法 - 邻接表版本
 * @param graph 图指针
 * @param source 源顶点
 * @param distances 距离数组
 * @param predecessors 前驱数组
 * @return 成功返回true，失败返回false
 */
bool dijkstra_list(ListGraph *graph, int source, int distances[], int predecessors[]) {
    if (!graph || source < 0 || source >= graph->vertex_count || 
        !distances || !predecessors) {
        printf("错误：参数无效\n");
        return false;
    }
    
    printf("\n========== Dijkstra算法（邻接表）==========\n");
    printf("源顶点：%s(%d)\n", graph->vertex_list[source].name, source);
    
    bool visited[MAX_VERTICES] = {false};
    
    // 初始化距离和前驱
    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INFINITY;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    
    for (int count = 0; count < graph->vertex_count; count++) {
        // 找到距离最小且未访问的顶点
        int u = -1;
        for (int v = 0; v < graph->vertex_count; v++) {
            if (!visited[v] && (u == -1 || distances[v] < distances[u])) {
                u = v;
            }
        }
        
        if (u == -1 || distances[u] == INFINITY) {
            break; // 剩余顶点不可达
        }
        
        visited[u] = true;
        printf("访问顶点：%s(%d)，距离：%d\n", 
               graph->vertex_list[u].name, u, distances[u]);
        
        // 更新相邻顶点的距离
        EdgeNode *edge = graph->vertex_list[u].first_edge;
        while (edge) {
            int v = edge->vertex;
            if (!visited[v]) {
                int new_dist = distances[u] + edge->weight;
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    predecessors[v] = u;
                    printf("  更新%s(%d)距离：%d\n",
                           graph->vertex_list[v].name, v, distances[v]);
                }
            }
            edge = edge->next;
        }
    }
    
    printf("Dijkstra算法完成\n");
    return true;
}

/**
 * Floyd算法 - 邻接矩阵版本
 * @param graph 图指针
 * @param distances 距离矩阵
 * @param predecessors 前驱矩阵
 */
void floyd_matrix(MatrixGraph *graph, int distances[][MAX_VERTICES], int predecessors[][MAX_VERTICES]) {
    if (!graph || !distances || !predecessors) {
        printf("错误：参数为空\n");
        return;
    }
    
    printf("\n========== Floyd算法（邻接矩阵）==========\n");
    
    // 初始化距离矩阵和前驱矩阵
    for (int i = 0; i < graph->vertex_count; i++) {
        for (int j = 0; j < graph->vertex_count; j++) {
            if (i == j) {
                distances[i][j] = 0;
            } else if (graph->adj_matrix[i][j] != NO_EDGE) {
                distances[i][j] = graph->adj_matrix[i][j];
            } else {
                distances[i][j] = INFINITY;
            }
            
            if (i != j && graph->adj_matrix[i][j] != NO_EDGE) {
                predecessors[i][j] = i;
            } else {
                predecessors[i][j] = -1;
            }
        }
    }
    
    // Floyd核心算法
    for (int k = 0; k < graph->vertex_count; k++) {
        printf("中间顶点：%s(%d)\n", graph->vertices[k], k);
        
        for (int i = 0; i < graph->vertex_count; i++) {
            for (int j = 0; j < graph->vertex_count; j++) {
                if (distances[i][k] != INFINITY && distances[k][j] != INFINITY &&
                    distances[i][k] + distances[k][j] < distances[i][j]) {
                    distances[i][j] = distances[i][k] + distances[k][j];
                    predecessors[i][j] = predecessors[k][j];
                    printf("  更新距离：%s(%d) -> %s(%d) = %d\n",
                           graph->vertices[i], i, graph->vertices[j], j,
                           distances[i][j]);
                }
            }
        }
    }
    
    printf("Floyd算法完成\n");
}

/**
 * Bellman-Ford算法 - 邻接矩阵版本
 * @param graph 图指针
 * @param source 源顶点
 * @param distances 距离数组
 * @param predecessors 前驱数组
 * @return 成功返回true，存在负权回路返回false
 */
bool bellman_ford_matrix(MatrixGraph *graph, int source, int distances[], int predecessors[]) {
    if (!graph || source < 0 || source >= graph->vertex_count || 
        !distances || !predecessors) {
        printf("错误：参数无效\n");
        return false;
    }
    
    printf("\n========== Bellman-Ford算法（邻接矩阵）==========\n");
    printf("源顶点：%s(%d)\n", graph->vertices[source], source);
    
    // 初始化距离和前驱
    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INFINITY;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    
    // 松弛操作（V-1次）
    for (int round = 0; round < graph->vertex_count - 1; round++) {
        printf("第%d轮松弛：\n", round + 1);
        bool updated = false;
        
        for (int u = 0; u < graph->vertex_count; u++) {
            if (distances[u] == INFINITY) continue;
            
            for (int v = 0; v < graph->vertex_count; v++) {
                if (graph->adj_matrix[u][v] != NO_EDGE) {
                    int new_dist = distances[u] + graph->adj_matrix[u][v];
                    if (new_dist < distances[v]) {
                        distances[v] = new_dist;
                        predecessors[v] = u;
                        updated = true;
                        printf("  更新%s(%d)距离：%d\n",
                               graph->vertices[v], v, distances[v]);
                    }
                }
            }
        }
        
        if (!updated) {
            printf("  没有更新，提前结束\n");
            break;
        }
    }
    
    // 检查负权回路
    printf("检查负权回路：\n");
    for (int u = 0; u < graph->vertex_count; u++) {
        if (distances[u] == INFINITY) continue;
        
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->adj_matrix[u][v] != NO_EDGE) {
                int new_dist = distances[u] + graph->adj_matrix[u][v];
                if (new_dist < distances[v]) {
                    printf("发现负权回路\n");
                    return false;
                }
            }
        }
    }
    
    printf("Bellman-Ford算法完成，无负权回路\n");
    return true;
}

/**
 * Bellman-Ford算法 - 邻接表版本
 * @param graph 图指针
 * @param source 源顶点
 * @param distances 距离数组
 * @param predecessors 前驱数组
 * @return 成功返回true，存在负权回路返回false
 */
bool bellman_ford_list(ListGraph *graph, int source, int distances[], int predecessors[]) {
    if (!graph || source < 0 || source >= graph->vertex_count || 
        !distances || !predecessors) {
        printf("错误：参数无效\n");
        return false;
    }
    
    printf("\n========== Bellman-Ford算法（邻接表）==========\n");
    printf("源顶点：%s(%d)\n", graph->vertex_list[source].name, source);
    
    // 初始化距离和前驱
    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INFINITY;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    
    // 松弛操作（V-1次）
    for (int round = 0; round < graph->vertex_count - 1; round++) {
        printf("第%d轮松弛：\n", round + 1);
        bool updated = false;
        
        for (int u = 0; u < graph->vertex_count; u++) {
            if (distances[u] == INFINITY) continue;
            
            EdgeNode *edge = graph->vertex_list[u].first_edge;
            while (edge) {
                int v = edge->vertex;
                int new_dist = distances[u] + edge->weight;
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    predecessors[v] = u;
                    updated = true;
                    printf("  更新%s(%d)距离：%d\n",
                           graph->vertex_list[v].name, v, distances[v]);
                }
                edge = edge->next;
            }
        }
        
        if (!updated) {
            printf("  没有更新，提前结束\n");
            break;
        }
    }
    
    // 检查负权回路
    printf("检查负权回路：\n");
    for (int u = 0; u < graph->vertex_count; u++) {
        if (distances[u] == INFINITY) continue;
        
        EdgeNode *edge = graph->vertex_list[u].first_edge;
        while (edge) {
            int v = edge->vertex;
            int new_dist = distances[u] + edge->weight;
            if (new_dist < distances[v]) {
                printf("发现负权回路\n");
                return false;
            }
            edge = edge->next;
        }
    }
    
    printf("Bellman-Ford算法完成，无负权回路\n");
    return true;
}

/**
 * 打印最短路径
 * @param predecessors 前驱数组
 * @param source 源顶点
 * @param destination 目标顶点
 */
void print_shortest_path(int predecessors[], int source, int destination) {
    if (!predecessors || source == destination) {
        printf("路径：%d", destination);
        return;
    }
    
    if (predecessors[destination] == -1) {
        printf("无路径到达顶点%d", destination);
        return;
    }
    
    print_shortest_path(predecessors, source, predecessors[destination]);
    printf(" -> %d", destination);
}

/**
 * 打印所有最短路径
 * @param distances 距离数组
 * @param predecessors 前驱数组
 * @param vertex_count 顶点数
 * @param source 源顶点
 */
void print_all_shortest_paths(int distances[], int predecessors[], int vertex_count, int source) {
    printf("\n========== 最短路径结果 ==========\n");
    printf("源顶点：%d\n", source);
    printf("目标顶点    距离    路径\n");
    printf("--------------------------------\n");
    
    for (int i = 0; i < vertex_count; i++) {
        if (i == source) continue;
        
        printf("%-10d  ", i);
        if (distances[i] == INFINITY) {
            printf("∞       无法到达\n");
        } else {
            printf("%-6d  ", distances[i]);
            print_shortest_path(predecessors, source, i);
            printf("\n");
        }
    }
    printf("================================\n");
}

/**
 * 打印距离数组
 * @param distances 距离数组
 * @param vertex_count 顶点数
 */
void print_distances(int distances[], int vertex_count) {
    printf("距离数组：[");
    for (int i = 0; i < vertex_count; i++) {
        if (distances[i] == INFINITY) {
            printf("∞");
        } else {
            printf("%d", distances[i]);
        }
        if (i < vertex_count - 1) printf(", ");
    }
    printf("]\n");
}

/**
 * Dijkstra算法优化版本 - 使用优先队列（邻接矩阵）
 * 时间复杂度：O((V+E)logV)，适合稀疏图
 * @param graph 图指针
 * @param source 源顶点
 * @param distances 距离数组
 * @param predecessors 前驱数组
 * @return 成功返回true，失败返回false
 */
bool dijkstra_matrix_optimized(MatrixGraph *graph, int source, int distances[], int predecessors[]) {
    if (!graph || source < 0 || source >= graph->vertex_count || 
        !distances || !predecessors) {
        printf("错误：参数无效\n");
        return false;
    }
    
    printf("\n========== Dijkstra算法优化版（邻接矩阵）==========\n");
    printf("源顶点：%s(%d)\n", graph->vertices[source], source);
    
    PriorityQueue pq;
    init_priority_queue(&pq);
    bool visited[MAX_VERTICES] = {false};
    
    // 初始化距离和前驱
    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INFINITY;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    priority_queue_insert(&pq, source, 0);
    
    while (!is_priority_queue_empty(&pq)) {
        int u, dist;
        if (!priority_queue_extract_min(&pq, &u, &dist)) break;
        
        if (visited[u]) continue;  // 已处理过的顶点
        visited[u] = true;
        
        printf("访问顶点：%s(%d)，距离：%d\n", 
               graph->vertices[u], u, distances[u]);
        
        // 更新相邻顶点的距离
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->adj_matrix[u][v] != NO_EDGE && !visited[v]) {
                int new_dist = distances[u] + graph->adj_matrix[u][v];
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    predecessors[v] = u;
                    priority_queue_insert(&pq, v, new_dist);
                    printf("  更新%s(%d)距离：%d\n",
                           graph->vertices[v], v, new_dist);
                }
            }
        }
    }
    
    printf("Dijkstra算法优化版完成\n");
    return true;
} 