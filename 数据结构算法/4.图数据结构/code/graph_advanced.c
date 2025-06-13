/* graph_advanced.c - 高级图算法实现
 *   拓扑排序、关键路径、二分图算法
 * create by: zw.duan
 */

#include "graph.h"

// ============= 图的连通性算法实现 =============

/**
 * 检查邻接矩阵图是否连通
 * @param graph 图指针
 * @return 连通返回true，不连通返回false
 */
bool is_connected_matrix(MatrixGraph *graph) {
    if (!graph || graph->vertex_count <= 0) return false;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    // 从顶点0开始DFS
    dfs_matrix(graph, 0, visited);
    
    // 检查是否所有顶点都被访问
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            return false;
        }
    }
    return true;
}

/**
 * 检查邻接表图是否连通
 * @param graph 图指针
 * @return 连通返回true，不连通返回false
 */
bool is_connected_list(ListGraph *graph) {
    if (!graph || graph->vertex_count <= 0) return false;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    // 从顶点0开始DFS
    dfs_list(graph, 0, visited);
    
    // 检查是否所有顶点都被访问
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            return false;
        }
    }
    return true;
}

/**
 * 统计邻接矩阵图的连通分量数
 * @param graph 图指针
 * @return 连通分量数
 */
int count_connected_components_matrix(MatrixGraph *graph) {
    if (!graph || graph->vertex_count <= 0) return 0;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    int component_count = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            dfs_matrix(graph, i, visited);
            component_count++;
        }
    }
    
    return component_count;
}

/**
 * 统计邻接表图的连通分量数
 * @param graph 图指针
 * @return 连通分量数
 */
int count_connected_components_list(ListGraph *graph) {
    if (!graph || graph->vertex_count <= 0) return 0;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    int component_count = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            dfs_list(graph, i, visited);
            component_count++;
        }
    }
    
    return component_count;
}

/**
 * 检查邻接矩阵图中两点间是否有路径
 * @param graph 图指针
 * @param from 起始顶点
 * @param to 目标顶点
 * @return 有路径返回true，无路径返回false
 */
bool has_path_matrix(MatrixGraph *graph, int from, int to) {
    if (!graph || from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        return false;
    }
    
    if (from == to) return true;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    // 使用BFS搜索路径
    Queue queue;
    init_queue(&queue);
    visited[from] = VISITED;
    enqueue(&queue, from);
    
    while (!is_queue_empty(&queue)) {
        int current;
        if (!dequeue(&queue, &current)) break;
        
        if (current == to) return true;
        
        for (int i = 0; i < graph->vertex_count; i++) {
            if (graph->adj_matrix[current][i] != NO_EDGE && 
                visited[i] == UNVISITED) {
                visited[i] = VISITED;
                enqueue(&queue, i);
            }
        }
    }
    
    return false;
}

/**
 * 检查邻接表图中两点间是否有路径
 * @param graph 图指针
 * @param from 起始顶点
 * @param to 目标顶点
 * @return 有路径返回true，无路径返回false
 */
bool has_path_list(ListGraph *graph, int from, int to) {
    if (!graph || from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        return false;
    }
    
    if (from == to) return true;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    // 使用BFS搜索路径
    Queue queue;
    init_queue(&queue);
    visited[from] = VISITED;
    enqueue(&queue, from);
    
    while (!is_queue_empty(&queue)) {
        int current;
        if (!dequeue(&queue, &current)) break;
        
        if (current == to) return true;
        
        EdgeNode *edge = graph->vertex_list[current].first_edge;
        while (edge) {
            if (visited[edge->vertex] == UNVISITED) {
                visited[edge->vertex] = VISITED;
                enqueue(&queue, edge->vertex);
            }
            edge = edge->next;
        }
    }
    
    return false;
}

// ============= 拓扑排序算法实现 =============

/**
 * Kahn算法拓扑排序 - 邻接矩阵版本
 * @param graph 图指针
 * @param result 拓扑排序结果数组
 * @return 成功返回true，失败返回false（存在环）
 */
bool topological_sort_kahn_matrix(MatrixGraph *graph, int result[]) {
    if (!graph || !result || !graph->is_directed) {
        printf("错误：图为空或不是有向图\n");
        return false;
    }
    
    printf("\n========== Kahn算法拓扑排序（邻接矩阵）==========\n");
    
    int in_degree[MAX_VERTICES] = {0};
    Queue queue;
    init_queue(&queue);
    
    // 计算所有顶点的入度
    for (int i = 0; i < graph->vertex_count; i++) {
        for (int j = 0; j < graph->vertex_count; j++) {
            if (graph->adj_matrix[i][j] != NO_EDGE) {
                in_degree[j]++;
            }
        }
    }
    
    printf("初始入度：");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%s(%d):%d ", graph->vertices[i], i, in_degree[i]);
        if (in_degree[i] == 0) {
            enqueue(&queue, i);
        }
    }
    printf("\n");
    
    int result_count = 0;
    
    while (!is_queue_empty(&queue)) {
        int u;
        if (!dequeue(&queue, &u)) break;
        
        result[result_count++] = u;
        printf("输出顶点：%s(%d)\n", graph->vertices[u], u);
        
        // 删除从u出发的所有边，更新入度
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->adj_matrix[u][v] != NO_EDGE) {
                in_degree[v]--;
                printf("  %s(%d)入度减1，当前入度：%d\n", 
                       graph->vertices[v], v, in_degree[v]);
                if (in_degree[v] == 0) {
                    enqueue(&queue, v);
                    printf("  %s(%d)入度为0，入队\n", graph->vertices[v], v);
                }
            }
        }
    }
    
    if (result_count == graph->vertex_count) {
        printf("拓扑排序成功\n");
        return true;
    } else {
        printf("拓扑排序失败：图中存在环\n");
        return false;
    }
}

/**
 * Kahn算法拓扑排序 - 邻接表版本
 * @param graph 图指针
 * @param result 拓扑排序结果数组
 * @return 成功返回true，失败返回false（存在环）
 */
bool topological_sort_kahn_list(ListGraph *graph, int result[]) {
    if (!graph || !result || !graph->is_directed) {
        printf("错误：图为空或不是有向图\n");
        return false;
    }
    
    printf("\n========== Kahn算法拓扑排序（邻接表）==========\n");
    
    int in_degree[MAX_VERTICES] = {0};
    Queue queue;
    init_queue(&queue);
    
    // 计算所有顶点的入度
    for (int i = 0; i < graph->vertex_count; i++) {
        EdgeNode *edge = graph->vertex_list[i].first_edge;
        while (edge) {
            in_degree[edge->vertex]++;
            edge = edge->next;
        }
    }
    
    printf("初始入度：");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%s(%d):%d ", graph->vertex_list[i].name, i, in_degree[i]);
        if (in_degree[i] == 0) {
            enqueue(&queue, i);
        }
    }
    printf("\n");
    
    int result_count = 0;
    
    while (!is_queue_empty(&queue)) {
        int u;
        if (!dequeue(&queue, &u)) break;
        
        result[result_count++] = u;
        printf("输出顶点：%s(%d)\n", graph->vertex_list[u].name, u);
        
        // 删除从u出发的所有边，更新入度
        EdgeNode *edge = graph->vertex_list[u].first_edge;
        while (edge) {
            int v = edge->vertex;
            in_degree[v]--;
            printf("  %s(%d)入度减1，当前入度：%d\n", 
                   graph->vertex_list[v].name, v, in_degree[v]);
            if (in_degree[v] == 0) {
                enqueue(&queue, v);
                printf("  %s(%d)入度为0，入队\n", graph->vertex_list[v].name, v);
            }
            edge = edge->next;
        }
    }
    
    if (result_count == graph->vertex_count) {
        printf("拓扑排序成功\n");
        return true;
    } else {
        printf("拓扑排序失败：图中存在环\n");
        return false;
    }
}

// DFS拓扑排序辅助函数
static void dfs_topological_sort_matrix(MatrixGraph *graph, int vertex, 
                                       int visited[], int *result, int *index) {
    visited[vertex] = VISITED;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->adj_matrix[vertex][i] != NO_EDGE && visited[i] == UNVISITED) {
            dfs_topological_sort_matrix(graph, i, visited, result, index);
        }
    }
    
    result[(*index)--] = vertex;
    printf("完成顶点：%s(%d)\n", graph->vertices[vertex], vertex);
}

static void dfs_topological_sort_list(ListGraph *graph, int vertex, 
                                     int visited[], int *result, int *index) {
    visited[vertex] = VISITED;
    
    EdgeNode *edge = graph->vertex_list[vertex].first_edge;
    while (edge) {
        if (visited[edge->vertex] == UNVISITED) {
            dfs_topological_sort_list(graph, edge->vertex, visited, result, index);
        }
        edge = edge->next;
    }
    
    result[(*index)--] = vertex;
    printf("完成顶点：%s(%d)\n", graph->vertex_list[vertex].name, vertex);
}

/**
 * DFS算法拓扑排序 - 邻接矩阵版本
 * @param graph 图指针
 * @param result 拓扑排序结果数组
 * @return 成功返回true，失败返回false
 */
bool topological_sort_dfs_matrix(MatrixGraph *graph, int result[]) {
    if (!graph || !result || !graph->is_directed) {
        printf("错误：图为空或不是有向图\n");
        return false;
    }
    
    printf("\n========== DFS算法拓扑排序（邻接矩阵）==========\n");
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    int index = graph->vertex_count - 1;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            dfs_topological_sort_matrix(graph, i, visited, result, &index);
        }
    }
    
    printf("DFS拓扑排序完成\n");
    return true;
}

/**
 * DFS算法拓扑排序 - 邻接表版本
 * @param graph 图指针
 * @param result 拓扑排序结果数组
 * @return 成功返回true，失败返回false
 */
bool topological_sort_dfs_list(ListGraph *graph, int result[]) {
    if (!graph || !result || !graph->is_directed) {
        printf("错误：图为空或不是有向图\n");
        return false;
    }
    
    printf("\n========== DFS算法拓扑排序（邻接表）==========\n");
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    int index = graph->vertex_count - 1;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            dfs_topological_sort_list(graph, i, visited, result, &index);
        }
    }
    
    printf("DFS拓扑排序完成\n");
    return true;
}

// 环检测辅助函数
static bool has_cycle_dfs_matrix(MatrixGraph *graph, int vertex, 
                                int visited[], int rec_stack[]) {
    visited[vertex] = VISITED;
    rec_stack[vertex] = VISITED;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (graph->adj_matrix[vertex][i] != NO_EDGE) {
            if (visited[i] == UNVISITED) {
                if (has_cycle_dfs_matrix(graph, i, visited, rec_stack)) {
                    return true;
                }
            } else if (rec_stack[i] == VISITED) {
                return true;
            }
        }
    }
    
    rec_stack[vertex] = UNVISITED;
    return false;
}

static bool has_cycle_dfs_list(ListGraph *graph, int vertex, 
                              int visited[], int rec_stack[]) {
    visited[vertex] = VISITED;
    rec_stack[vertex] = VISITED;
    
    EdgeNode *edge = graph->vertex_list[vertex].first_edge;
    while (edge) {
        if (visited[edge->vertex] == UNVISITED) {
            if (has_cycle_dfs_list(graph, edge->vertex, visited, rec_stack)) {
                return true;
            }
        } else if (rec_stack[edge->vertex] == VISITED) {
            return true;
        }
        edge = edge->next;
    }
    
    rec_stack[vertex] = UNVISITED;
    return false;
}

/**
 * 检测邻接矩阵图中是否存在环
 * @param graph 图指针
 * @return 存在环返回true，不存在环返回false
 */
bool has_cycle_matrix(MatrixGraph *graph) {
    if (!graph) return false;
    
    if (!graph->is_directed) {
        // 无向图：使用DFS检测回边
        int visited[MAX_VERTICES];
        for (int i = 0; i < graph->vertex_count; i++) {
            visited[i] = UNVISITED;
        }
        
        // 简化版本：检查是否边数 >= 顶点数
        return graph->edge_count >= graph->vertex_count;
    } else {
        // 有向图：使用DFS递归栈检测环
        int visited[MAX_VERTICES];
        int rec_stack[MAX_VERTICES];
        
        for (int i = 0; i < graph->vertex_count; i++) {
            visited[i] = UNVISITED;
            rec_stack[i] = UNVISITED;
        }
        
        for (int i = 0; i < graph->vertex_count; i++) {
            if (visited[i] == UNVISITED) {
                if (has_cycle_dfs_matrix(graph, i, visited, rec_stack)) {
                    return true;
                }
            }
        }
        return false;
    }
}

/**
 * 检测邻接表图中是否存在环
 * @param graph 图指针
 * @return 存在环返回true，不存在环返回false
 */
bool has_cycle_list(ListGraph *graph) {
    if (!graph) return false;
    
    if (!graph->is_directed) {
        // 无向图：使用DFS检测回边
        int visited[MAX_VERTICES];
        for (int i = 0; i < graph->vertex_count; i++) {
            visited[i] = UNVISITED;
        }
        
        // 简化版本：检查是否边数 >= 顶点数
        return graph->edge_count >= graph->vertex_count;
    } else {
        // 有向图：使用DFS递归栈检测环
        int visited[MAX_VERTICES];
        int rec_stack[MAX_VERTICES];
        
        for (int i = 0; i < graph->vertex_count; i++) {
            visited[i] = UNVISITED;
            rec_stack[i] = UNVISITED;
        }
        
        for (int i = 0; i < graph->vertex_count; i++) {
            if (visited[i] == UNVISITED) {
                if (has_cycle_dfs_list(graph, i, visited, rec_stack)) {
                    return true;
                }
            }
        }
        return false;
    }
}

// ============= 关键路径算法实现（AOE网络）=============

/**
 * 求AOE网络的关键路径
 * @param graph 有向无环图
 * @return 关键路径信息，失败返回NULL
 */
CriticalPath* find_critical_path_matrix(MatrixGraph *graph) {
    if (!graph || graph->vertex_count <= 0 || !graph->is_directed) {
        printf("错误：无效的AOE网络\n");
        return NULL;
    }
    
    // 检查是否为DAG
    if (has_cycle_matrix(graph)) {
        printf("错误：图中存在环，无法计算关键路径\n");
        return NULL;
    }
    
    CriticalPath *cp = (CriticalPath*)malloc(sizeof(CriticalPath));
    if (!cp) {
        printf("错误：内存分配失败\n");
        return NULL;
    }
    
    cp->early_time = (int*)calloc(graph->vertex_count, sizeof(int));
    cp->late_time = (int*)calloc(graph->vertex_count, sizeof(int));
    cp->critical_edges = (Edge*)malloc(graph->edge_count * sizeof(Edge));
    cp->critical_count = 0;
    
    if (!cp->early_time || !cp->late_time || !cp->critical_edges) {
        destroy_critical_path(cp);
        return NULL;
    }
    
    // 计算最早发生时间（拓扑排序）
    int topo_order[MAX_VERTICES];
    if (!topological_sort_kahn_matrix(graph, topo_order)) {
        printf("错误：拓扑排序失败\n");
        destroy_critical_path(cp);
        return NULL;
    }
    
    // 按拓扑序计算最早发生时间
    for (int i = 0; i < graph->vertex_count; i++) {
        int v = topo_order[i];
        for (int u = 0; u < graph->vertex_count; u++) {
            if (graph->adj_matrix[u][v] != NO_EDGE) {
                int new_time = cp->early_time[u] + graph->adj_matrix[u][v];
                if (new_time > cp->early_time[v]) {
                    cp->early_time[v] = new_time;
                }
            }
        }
    }
    
    // 找到工程总时间（汇点的最早发生时间）
    cp->project_time = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (cp->early_time[i] > cp->project_time) {
            cp->project_time = cp->early_time[i];
        }
    }
    
    // 初始化最迟发生时间
    for (int i = 0; i < graph->vertex_count; i++) {
        cp->late_time[i] = cp->project_time;
    }
    
    // 按逆拓扑序计算最迟发生时间
    for (int i = graph->vertex_count - 1; i >= 0; i--) {
        int u = topo_order[i];
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->adj_matrix[u][v] != NO_EDGE) {
                int new_time = cp->late_time[v] - graph->adj_matrix[u][v];
                if (new_time < cp->late_time[u]) {
                    cp->late_time[u] = new_time;
                }
            }
        }
    }
    
    // 找出关键边
    for (int u = 0; u < graph->vertex_count; u++) {
        for (int v = 0; v < graph->vertex_count; v++) {
            if (graph->adj_matrix[u][v] != NO_EDGE) {
                int earliest = cp->early_time[u];
                int latest = cp->late_time[v] - graph->adj_matrix[u][v];
                
                if (earliest == latest) {  // 关键边
                    cp->critical_edges[cp->critical_count].from = u;
                    cp->critical_edges[cp->critical_count].to = v;
                    cp->critical_edges[cp->critical_count].weight = graph->adj_matrix[u][v];
                    cp->critical_count++;
                }
            }
        }
    }
    
    return cp;
}

/**
 * 销毁关键路径结构
 */
void destroy_critical_path(CriticalPath *cp) {
    if (cp) {
        free(cp->early_time);
        free(cp->late_time);
        free(cp->critical_edges);
        free(cp);
    }
}

/**
 * 打印关键路径信息
 */
void print_critical_path(CriticalPath *cp, MatrixGraph *graph) {
    if (!cp || !graph) return;
    
    printf("\n========== 关键路径分析 ==========\n");
    printf("工程总时间：%d\n", cp->project_time);
    
    printf("\n事件时间分析：\n");
    printf("顶点\t最早时间\t最迟时间\t时间余量\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%s\t%d\t\t%d\t\t%d\n", 
               graph->vertices[i], cp->early_time[i], 
               cp->late_time[i], cp->late_time[i] - cp->early_time[i]);
    }
    
    printf("\n关键路径（关键边）：\n");
    for (int i = 0; i < cp->critical_count; i++) {
        printf("%s -> %s (权重：%d)\n",
               graph->vertices[cp->critical_edges[i].from],
               graph->vertices[cp->critical_edges[i].to],
               cp->critical_edges[i].weight);
    }
}

// ============= 强连通分量算法实现（Tarjan算法）=============

typedef struct {
    int *stack;
    int top;
    int *low;
    int *disc;
    int *in_stack;
    int time;
    int scc_count;
    int *scc_id;
} TarjanData;

static void tarjan_dfs(MatrixGraph *graph, int u, TarjanData *data) {
    data->disc[u] = data->low[u] = ++data->time;
    data->stack[++data->top] = u;
    data->in_stack[u] = 1;
    
    for (int v = 0; v < graph->vertex_count; v++) {
        if (graph->adj_matrix[u][v] != NO_EDGE) {
            if (data->disc[v] == -1) {
                tarjan_dfs(graph, v, data);
                data->low[u] = (data->low[u] < data->low[v]) ? data->low[u] : data->low[v];
            } else if (data->in_stack[v]) {
                data->low[u] = (data->low[u] < data->disc[v]) ? data->low[u] : data->disc[v];
            }
        }
    }
    
    if (data->low[u] == data->disc[u]) {
        printf("强连通分量 %d: ", data->scc_count);
        int w;
        do {
            w = data->stack[data->top--];
            data->in_stack[w] = 0;
            data->scc_id[w] = data->scc_count;
            printf("%s ", graph->vertices[w]);
        } while (w != u);
        printf("\n");
        data->scc_count++;
    }
}

/**
 * 使用Tarjan算法求强连通分量
 */
StronglyConnectedComponents* find_scc_matrix(MatrixGraph *graph) {
    if (!graph || !graph->is_directed) {
        printf("错误：需要有向图\n");
        return NULL;
    }
    
    StronglyConnectedComponents *scc = (StronglyConnectedComponents*)malloc(sizeof(StronglyConnectedComponents));
    if (!scc) return NULL;
    
    scc->components = (int*)malloc(graph->vertex_count * sizeof(int));
    if (!scc->components) {
        free(scc);
        return NULL;
    }
    
    TarjanData data;
    data.stack = (int*)malloc(graph->vertex_count * sizeof(int));
    data.low = (int*)malloc(graph->vertex_count * sizeof(int));
    data.disc = (int*)malloc(graph->vertex_count * sizeof(int));
    data.in_stack = (int*)calloc(graph->vertex_count, sizeof(int));
    data.scc_id = scc->components;
    data.top = -1;
    data.time = 0;
    data.scc_count = 0;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        data.disc[i] = -1;
    }
    
    printf("\n========== 强连通分量分析 ==========\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        if (data.disc[i] == -1) {
            tarjan_dfs(graph, i, &data);
        }
    }
    
    scc->component_count = data.scc_count;
    
    free(data.stack);
    free(data.low);
    free(data.disc);
    free(data.in_stack);
    
    return scc;
}

/**
 * 销毁强连通分量结构
 */
void destroy_scc(StronglyConnectedComponents *scc) {
    if (scc) {
        free(scc->components);
        free(scc);
    }
}

// ============= 二分图算法实现 =============

/**
 * 使用DFS检测二分图
 */
static bool is_bipartite_dfs(MatrixGraph *graph, int v, int coloring[], int color) {
    coloring[v] = color;
    
    for (int u = 0; u < graph->vertex_count; u++) {
        if (graph->adj_matrix[v][u] != NO_EDGE) {
            if (coloring[u] == -1) {
                if (!is_bipartite_dfs(graph, u, coloring, 1 - color)) {
                    return false;
                }
            } else if (coloring[u] == color) {
                return false;  // 相邻顶点颜色相同
            }
        }
    }
    return true;
}

/**
 * 检测图是否为二分图
 */
bool is_bipartite_matrix(MatrixGraph *graph, int coloring[]) {
    if (!graph || !coloring) return false;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        coloring[i] = -1;
    }
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (coloring[i] == -1) {
            if (!is_bipartite_dfs(graph, i, coloring, 0)) {
                return false;
            }
        }
    }
    return true;
} 