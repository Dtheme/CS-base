/* graph.h - 图数据结构头文件
 *   图的完整实现
 * create by: zw.duan
 * 
 * 1. 图的两种存储方式：邻接矩阵 vs 邻接表
 * 2. 图的遍历：DFS和BFS的实现原理
 * 3. 最小生成树：Prim和Kruskal算法区别
 * 4. 最短路径：Dijkstra、Floyd、Bellman-Ford适用场景
 * 5. 拓扑排序：有向无环图的线性排序
 * 6. 关键路径：AOE网络的项目管理应用
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// ============= 基本常量定义 =============
#define MAX_VERTICES 100        // 最大顶点数（ 通常100以内）
#define MAX_VERTEX_NAME 20      // 顶点名称最大长度
#define INFINITY INT_MAX        // 无穷大（表示无边连接）
#define NO_EDGE 0              // 无边标记（邻接矩阵中）
#define VISITED 1              // 已访问标记（遍历算法中）
#define UNVISITED 0            // 未访问标记

// ============= 边的定义 =============
/**
 * 边结构体 - 用于表示图中的一条边
 * 
 * 在算法中的应用：
 * - 最小生成树算法：存储MST的边集合
 * - 最短路径算法：存储路径上的边
 * - 关键路径算法：存储关键活动边
 */
typedef struct {
    int from;          // 起始顶点索引
    int to;            // 终止顶点索引  
    int weight;        // 边的权重（距离、成本、时间等）
} Edge;

// ============= 邻接表节点定义 =============
/**
 * 邻接表中的边节点
 * 
 * 链表结构图示：
 * vertex[0] -> [1|2|next] -> [3|5|next] -> NULL
 *              ↑             ↑
 *           边(0,1)权重2   边(0,3)权重5
 */
typedef struct EdgeNode {
    int vertex;                 // 邻接顶点的索引
    int weight;                 // 边的权重
    struct EdgeNode *next;      // 指向下一个邻接顶点的指针
} EdgeNode;

/**
 * 邻接表中的顶点节点
 */
typedef struct {
    char name[MAX_VERTEX_NAME]; // 顶点名称（如：A, B, C或V0, V1, V2）
    EdgeNode *first_edge;       // 指向第一条出边的指针
} VertexNode;

// ============= 图的存储结构 =============

/**
 * 邻接矩阵表示法
 * 
 *  适用场景：稠密图，边数接近V²
 * ⏰ 空间复杂度：O(V²)
 * ⚡ 查询边：O(1)，添加/删除边：O(1)
 * 
 * 矩阵示例（无向图）：
 *     A  B  C  D
 * A [ 0  2  ∞  4 ]
 * B [ 2  0  1  ∞ ]
 * C [ ∞  1  0  3 ]
 * D [ 4  ∞  3  0 ]
 * 
 * 矩阵示例（有向图）：
 *     A  B  C  D
 * A [ 0  2  ∞  ∞ ]
 * B [ ∞  0  1  ∞ ]
 * C [ ∞  ∞  0  3 ]
 * D [ 4  ∞  ∞  0 ]
 */
typedef struct {
    char vertices[MAX_VERTICES][MAX_VERTEX_NAME];   // 顶点名称数组
    int adj_matrix[MAX_VERTICES][MAX_VERTICES];     // 邻接矩阵
    int vertex_count;                               // 当前顶点数
    int edge_count;                                 // 当前边数
    bool is_directed;                               // 是否为有向图
} MatrixGraph;

/**
 * 邻接表表示法
 * 
 *  适用场景：稀疏图，边数远小于V²
 * ⏰ 空间复杂度：O(V + E)
 * ⚡ 查询边：O(度数)，添加边：O(1)，删除边：O(度数)
 * 
 * 邻接表示例：
 * [0] A: -> B(2) -> D(4) -> NULL
 * [1] B: -> A(2) -> C(1) -> NULL  
 * [2] C: -> B(1) -> D(3) -> NULL
 * [3] D: -> A(4) -> C(3) -> NULL
 * 
 * 其中：B(2)表示到顶点B的边权重为2
 */
typedef struct {
    VertexNode vertex_list[MAX_VERTICES];  // 顶点数组（每个顶点含邻接链表）
    int vertex_count;                      // 当前顶点数
    int edge_count;                        // 当前边数  
    bool is_directed;                      // 是否为有向图
} ListGraph;

// ============= 图的基本操作函数声明 =============

// 邻接矩阵图操作
MatrixGraph* create_matrix_graph(int vertex_count, bool is_directed);
void destroy_matrix_graph(MatrixGraph *graph);
bool add_matrix_vertex(MatrixGraph *graph, const char *name);
bool add_matrix_edge(MatrixGraph *graph, int from, int to, int weight);
bool remove_matrix_edge(MatrixGraph *graph, int from, int to);
int find_matrix_vertex(MatrixGraph *graph, const char *name);
bool has_matrix_edge(MatrixGraph *graph, int from, int to);
int get_matrix_edge_weight(MatrixGraph *graph, int from, int to);
void print_matrix_graph(MatrixGraph *graph);
void print_matrix_adjacency_matrix(MatrixGraph *graph);

// 邻接表图操作
ListGraph* create_list_graph(int vertex_count, bool is_directed);
void destroy_list_graph(ListGraph *graph);
bool add_list_vertex(ListGraph *graph, const char *name);
bool add_list_edge(ListGraph *graph, int from, int to, int weight);
bool remove_list_edge(ListGraph *graph, int from, int to);
int find_list_vertex(ListGraph *graph, const char *name);
bool has_list_edge(ListGraph *graph, int from, int to);
int get_list_edge_weight(ListGraph *graph, int from, int to);
void print_list_graph(ListGraph *graph);
int get_vertex_degree(ListGraph *graph, int vertex);
int get_vertex_in_degree(ListGraph *graph, int vertex);
int get_vertex_out_degree(ListGraph *graph, int vertex);

// ============= 图的遍历算法 =============

// 深度优先搜索
void dfs_matrix(MatrixGraph *graph, int start, int visited[]);
void dfs_matrix_iterative(MatrixGraph *graph, int start, int visited[]);
void dfs_list(ListGraph *graph, int start, int visited[]);
void dfs_list_iterative(ListGraph *graph, int start, int visited[]);

// 广度优先搜索
void bfs_matrix(MatrixGraph *graph, int start, int visited[]);
void bfs_list(ListGraph *graph, int start, int visited[]);

// 遍历整个图（处理非连通图）
void traverse_all_matrix_dfs(MatrixGraph *graph);
void traverse_all_matrix_bfs(MatrixGraph *graph);
void traverse_all_list_dfs(ListGraph *graph);
void traverse_all_list_bfs(ListGraph *graph);

// ============= 图的连通性 =============
bool is_connected_matrix(MatrixGraph *graph);
bool is_connected_list(ListGraph *graph);
int count_connected_components_matrix(MatrixGraph *graph);
int count_connected_components_list(ListGraph *graph);
bool has_path_matrix(MatrixGraph *graph, int from, int to);
bool has_path_list(ListGraph *graph, int from, int to);

// ============= 最小生成树算法 =============

// Prim算法
int prim_matrix(MatrixGraph *graph, Edge mst[]);
int prim_list(ListGraph *graph, Edge mst[]);

// Kruskal算法
int kruskal_matrix(MatrixGraph *graph, Edge mst[]);
int kruskal_list(ListGraph *graph, Edge mst[]);

// MST辅助函数
int calculate_mst_weight(Edge mst[], int edge_count);
void print_mst(Edge mst[], int edge_count, int total_weight);
bool validate_mst(Edge mst[], int edge_count, int vertex_count);

// ============= 最短路径算法 =============

// Dijkstra算法（单源最短路径）
bool dijkstra_matrix(MatrixGraph *graph, int source, int distances[], int predecessors[]);
bool dijkstra_list(ListGraph *graph, int source, int distances[], int predecessors[]);
bool dijkstra_matrix_optimized(MatrixGraph *graph, int source, int distances[], int predecessors[]);

// Floyd算法（多源最短路径）
void floyd_matrix(MatrixGraph *graph, int distances[][MAX_VERTICES], int predecessors[][MAX_VERTICES]);

// Bellman-Ford算法（处理负权边）
bool bellman_ford_matrix(MatrixGraph *graph, int source, int distances[], int predecessors[]);
bool bellman_ford_list(ListGraph *graph, int source, int distances[], int predecessors[]);

// 路径重构和输出
void print_shortest_path(int predecessors[], int source, int destination);
void print_all_shortest_paths(int distances[], int predecessors[], int vertex_count, int source);
void print_distances(int distances[], int vertex_count);

// ============= 拓扑排序 =============

// Kahn算法（基于入度）
bool topological_sort_kahn_matrix(MatrixGraph *graph, int result[]);
bool topological_sort_kahn_list(ListGraph *graph, int result[]);

// DFS算法
bool topological_sort_dfs_matrix(MatrixGraph *graph, int result[]);
bool topological_sort_dfs_list(ListGraph *graph, int result[]);

// 检测环
bool has_cycle_matrix(MatrixGraph *graph);
bool has_cycle_list(ListGraph *graph);

// ============= 关键路径算法 =============

// AOE网络关键路径
typedef struct {
    int *early_time;    // 事件最早发生时间
    int *late_time;     // 事件最迟发生时间
    Edge *critical_edges; // 关键边
    int critical_count; // 关键边数量
    int project_time;   // 工程总时间
} CriticalPath;

CriticalPath* find_critical_path_matrix(MatrixGraph *graph);
CriticalPath* find_critical_path_list(ListGraph *graph);
void destroy_critical_path(CriticalPath *cp);
void print_critical_path(CriticalPath *cp, MatrixGraph *graph);

// ============= 二分图算法 =============

// 二分图检测
bool is_bipartite_matrix(MatrixGraph *graph, int coloring[]);
bool is_bipartite_list(ListGraph *graph, int coloring[]);

// 二分图最大匹配（匈牙利算法）
int max_bipartite_matching_matrix(MatrixGraph *graph, int matching[]);
int max_bipartite_matching_list(ListGraph *graph, int matching[]);

// ============= 其他图算法 =============

// 强连通分量（Tarjan算法）
typedef struct {
    int *components;    // 每个顶点所属的强连通分量
    int component_count; // 强连通分量数量
} StronglyConnectedComponents;

StronglyConnectedComponents* find_scc_matrix(MatrixGraph *graph);
StronglyConnectedComponents* find_scc_list(ListGraph *graph);
void destroy_scc(StronglyConnectedComponents *scc);

// 欧拉路径和回路
bool has_euler_path_matrix(MatrixGraph *graph);
bool has_euler_circuit_matrix(MatrixGraph *graph);
bool has_euler_path_list(ListGraph *graph);
bool has_euler_circuit_list(ListGraph *graph);
void find_euler_path_matrix(MatrixGraph *graph, int path[], int *path_length);
void find_euler_path_list(ListGraph *graph, int path[], int *path_length);

// 哈密顿路径（回溯算法）
bool has_hamilton_path_matrix(MatrixGraph *graph);
bool has_hamilton_circuit_matrix(MatrixGraph *graph);
void find_hamilton_path_matrix(MatrixGraph *graph, int path[], int *path_length);

// ============= 图的转换 =============
ListGraph* matrix_to_list(MatrixGraph *matrix_graph);
MatrixGraph* list_to_matrix(ListGraph *list_graph);

// ============= 图的输入输出 =============
MatrixGraph* read_matrix_graph_from_file(const char *filename);
ListGraph* read_list_graph_from_file(const char *filename);
bool write_matrix_graph_to_file(MatrixGraph *graph, const char *filename);
bool write_list_graph_to_file(ListGraph *graph, const char *filename);

// DOT格式输出（用于Graphviz可视化）
void export_matrix_graph_to_dot(MatrixGraph *graph, const char *filename);
void export_list_graph_to_dot(ListGraph *graph, const char *filename);

// ============= 辅助数据结构 =============

// 栈（用于DFS）
typedef struct {
    int data[MAX_VERTICES];
    int top;
} Stack;

void init_stack(Stack *stack);
bool is_stack_empty(Stack *stack);
bool push(Stack *stack, int value);
bool pop(Stack *stack, int *value);

// 队列（用于BFS）
typedef struct {
    int data[MAX_VERTICES];
    int front, rear;
} Queue;

void init_queue(Queue *queue);
bool is_queue_empty(Queue *queue);
bool enqueue(Queue *queue, int value);
bool dequeue(Queue *queue, int *value);

// 优先队列（用于Dijkstra和Prim）
typedef struct {
    int vertex;
    int priority;
} PriorityNode;

typedef struct {
    PriorityNode data[MAX_VERTICES];
    int size;
} PriorityQueue;

void init_priority_queue(PriorityQueue *pq);
bool is_priority_queue_empty(PriorityQueue *pq);
void priority_queue_insert(PriorityQueue *pq, int vertex, int priority);
bool priority_queue_extract_min(PriorityQueue *pq, int *vertex, int *priority);
void priority_queue_decrease_key(PriorityQueue *pq, int vertex, int new_priority);

// 并查集（用于Kruskal算法）
typedef struct {
    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
} UnionFindSet;

void init_union_find(UnionFindSet *uf, int size);
int find_set(UnionFindSet *uf, int x);
bool union_sets(UnionFindSet *uf, int x, int y);

// ============= 性能测试和分析 =============
void performance_test_graph_creation(int vertex_count);
void performance_test_graph_traversal(int vertex_count);
void performance_test_shortest_path(int vertex_count);
void performance_test_mst(int vertex_count);

// 内存使用统计
typedef struct {
    size_t matrix_graph_size;
    size_t list_graph_size;
    size_t total_memory_used;
} MemoryUsage;

MemoryUsage calculate_graph_memory_usage(int vertex_count, int edge_count);
void print_memory_usage(MemoryUsage usage);

// ============= 调试和验证函数 =============
bool validate_matrix_graph(MatrixGraph *graph);
bool validate_list_graph(ListGraph *graph);
void print_graph_statistics(MatrixGraph *matrix_graph, ListGraph *list_graph);
void debug_print_adjacency_info(MatrixGraph *matrix_graph, ListGraph *list_graph);

#endif // GRAPH_H

/* ================================
 * 编译建议：
 * gcc -std=c99 -Wall -Wextra -O2 -g graph.c graph_algorithms.c graph_advanced.c test_graph.c -o test_graph
 * 
 *  重点：
 * 1. 图的两种存储方式：邻接矩阵和邻接表
 * 2. 图的遍历：DFS和BFS的递归与非递归实现
 * 3. 最小生成树：Prim和Kruskal算法
 * 4. 最短路径：Dijkstra、Floyd、Bellman-Ford算法
 * 5. 拓扑排序：Kahn算法和DFS算法
 * 6. 关键路径：AOE网络的关键路径求解
 * 7. 连通性：强连通分量、二分图判定
 * ================================ */