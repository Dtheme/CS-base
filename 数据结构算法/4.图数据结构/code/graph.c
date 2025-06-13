/* graph.c - 图数据结构基本操作实现
 *   图的基础功能实现
 * create by: zw.duan
 */

#include "graph.h"

// ============= 邻接矩阵图操作实现 =============

/**
 * 创建邻接矩阵图
 * @param vertex_count 顶点数量
 * @param is_directed 是否为有向图
 * @return 创建的图指针，失败返回NULL
 */
MatrixGraph* create_matrix_graph(int vertex_count, bool is_directed) {
    if (vertex_count <= 0 || vertex_count > MAX_VERTICES) {
        printf("错误：顶点数量无效 (%d)\n", vertex_count);
        return NULL;
    }
    
    MatrixGraph *graph = (MatrixGraph*)malloc(sizeof(MatrixGraph));
    if (!graph) {
        printf("错误：内存分配失败\n");
        return NULL;
    }
    
    // 初始化图结构
    graph->vertex_count = 0;
    graph->edge_count = 0;
    graph->is_directed = is_directed;
    
    // 初始化邻接矩阵为0（无边）
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph->adj_matrix[i][j] = NO_EDGE;
        }
        strcpy(graph->vertices[i], "");
    }
    
    printf("成功创建%s图，最大顶点数：%d\n", 
           is_directed ? "有向" : "无向", vertex_count);
    
    return graph;
}

/**
 * 销毁邻接矩阵图
 * @param graph 图指针
 */
void destroy_matrix_graph(MatrixGraph *graph) {
    if (graph) {
        printf("销毁邻接矩阵图，顶点数：%d，边数：%d\n", 
               graph->vertex_count, graph->edge_count);
        free(graph);
    }
}

/**
 * 添加顶点到邻接矩阵图
 * @param graph 图指针
 * @param name 顶点名称
 * @return 成功返回true，失败返回false
 */
bool add_matrix_vertex(MatrixGraph *graph, const char *name) {
    if (!graph || !name) {
        printf("错误：图指针或顶点名称为空\n");
        return false;
    }
    
    if (graph->vertex_count >= MAX_VERTICES) {
        printf("错误：顶点数量已达上限\n");
        return false;
    }
    
    // 检查顶点是否已存在
    if (find_matrix_vertex(graph, name) != -1) {
        printf("错误：顶点 '%s' 已存在\n", name);
        return false;
    }
    
    // 添加新顶点
    strcpy(graph->vertices[graph->vertex_count], name);
    graph->vertex_count++;
    
    printf("成功添加顶点：%s (索引：%d)\n", name, graph->vertex_count - 1);
    return true;
}

/**
 * 添加边到邻接矩阵图
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @param weight 边的权重
 * @return 成功返回true，失败返回false
 */
bool add_matrix_edge(MatrixGraph *graph, int from, int to, int weight) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return false;
    }
    
    if (from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        printf("错误：顶点索引越界 (%d, %d)\n", from, to);
        return false;
    }
    
    if (from == to) {
        printf("警告：添加自环边 (%d -> %d)\n", from, to);
    }
    
    // 检查边是否已存在
    if (graph->adj_matrix[from][to] != NO_EDGE) {
        printf("警告：边 (%d -> %d) 已存在，更新权重：%d -> %d\n", 
               from, to, graph->adj_matrix[from][to], weight);
        graph->adj_matrix[from][to] = weight;
        if (!graph->is_directed) {
            graph->adj_matrix[to][from] = weight;
        }
        return true;
    }
    
    // 添加新边
    graph->adj_matrix[from][to] = weight;
    graph->edge_count++;
    
    // 如果是无向图，添加反向边
    if (!graph->is_directed) {
        graph->adj_matrix[to][from] = weight;
    }
    
    printf("成功添加边：%s(%d) -> %s(%d)，权重：%d\n", 
           graph->vertices[from], from, graph->vertices[to], to, weight);
    
    return true;
}

/**
 * 删除邻接矩阵图中的边
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @return 成功返回true，失败返回false
 */
bool remove_matrix_edge(MatrixGraph *graph, int from, int to) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return false;
    }
    
    if (from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        printf("错误：顶点索引越界 (%d, %d)\n", from, to);
        return false;
    }
    
    if (graph->adj_matrix[from][to] == NO_EDGE) {
        printf("警告：边 (%d -> %d) 不存在\n", from, to);
        return false;
    }
    
    // 删除边
    graph->adj_matrix[from][to] = NO_EDGE;
    graph->edge_count--;
    
    // 如果是无向图，删除反向边
    if (!graph->is_directed) {
        graph->adj_matrix[to][from] = NO_EDGE;
    }
    
    printf("成功删除边：%s(%d) -> %s(%d)\n", 
           graph->vertices[from], from, graph->vertices[to], to);
    
    return true;
}

/**
 * 查找顶点在邻接矩阵图中的索引
 * @param graph 图指针
 * @param name 顶点名称
 * @return 顶点索引，未找到返回-1
 */
int find_matrix_vertex(MatrixGraph *graph, const char *name) {
    if (!graph || !name) return -1;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * 检查邻接矩阵图中是否存在边
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @return 存在返回true，不存在返回false
 */
bool has_matrix_edge(MatrixGraph *graph, int from, int to) {
    if (!graph || from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        return false;
    }
    return graph->adj_matrix[from][to] != NO_EDGE;
}

/**
 * 获取邻接矩阵图中边的权重
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @return 边的权重，不存在返回0
 */
int get_matrix_edge_weight(MatrixGraph *graph, int from, int to) {
    if (!has_matrix_edge(graph, from, to)) {
        return 0;
    }
    return graph->adj_matrix[from][to];
}

/**
 * 打印邻接矩阵图的基本信息
 * @param graph 图指针
 */
void print_matrix_graph(MatrixGraph *graph) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return;
    }
    
    printf("\n========== 邻接矩阵图信息 ==========\n");
    printf("图类型：%s\n", graph->is_directed ? "有向图" : "无向图");
    printf("顶点数：%d\n", graph->vertex_count);
    printf("边数：%d\n", graph->edge_count);
    
    printf("\n顶点列表：\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("  [%d] %s\n", i, graph->vertices[i]);
    }
    
    printf("\n边列表：\n");
    int edge_count = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        for (int j = 0; j < graph->vertex_count; j++) {
            if (graph->adj_matrix[i][j] != NO_EDGE) {
                printf("  %s(%d) -> %s(%d)，权重：%d\n", 
                       graph->vertices[i], i, graph->vertices[j], j, 
                       graph->adj_matrix[i][j]);
                edge_count++;
                if (!graph->is_directed && i < j) {
                    // 无向图只显示一次
                    continue;
                }
            }
        }
    }
    printf("====================================\n");
}

/**
 * 打印邻接矩阵
 * @param graph 图指针
 */
void print_matrix_adjacency_matrix(MatrixGraph *graph) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return;
    }
    
    printf("\n========== 邻接矩阵 ==========\n");
    printf("      ");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%4d", i);
    }
    printf("\n");
    
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("[%2d]  ", i);
        for (int j = 0; j < graph->vertex_count; j++) {
            if (graph->adj_matrix[i][j] == NO_EDGE) {
                printf("   -");
            } else {
                printf("%4d", graph->adj_matrix[i][j]);
            }
        }
        printf("  %s\n", graph->vertices[i]);
    }
    printf("==============================\n");
}

// ============= 邻接表图操作实现 =============

/**
 * 创建邻接表图
 * @param vertex_count 顶点数量
 * @param is_directed 是否为有向图
 * @return 创建的图指针，失败返回NULL
 */
ListGraph* create_list_graph(int vertex_count, bool is_directed) {
    if (vertex_count <= 0 || vertex_count > MAX_VERTICES) {
        printf("错误：顶点数量无效 (%d)\n", vertex_count);
        return NULL;
    }
    
    ListGraph *graph = (ListGraph*)malloc(sizeof(ListGraph));
    if (!graph) {
        printf("错误：内存分配失败\n");
        return NULL;
    }
    
    // 初始化图结构
    graph->vertex_count = 0;
    graph->edge_count = 0;
    graph->is_directed = is_directed;
    
    // 初始化顶点列表
    for (int i = 0; i < MAX_VERTICES; i++) {
        strcpy(graph->vertex_list[i].name, "");
        graph->vertex_list[i].first_edge = NULL;
    }
    
    printf("成功创建%s邻接表图，最大顶点数：%d\n", 
           is_directed ? "有向" : "无向", vertex_count);
    
    return graph;
}

/**
 * 销毁邻接表图
 * @param graph 图指针
 */
void destroy_list_graph(ListGraph *graph) {
    if (!graph) return;
    
    printf("销毁邻接表图，顶点数：%d，边数：%d\n", 
           graph->vertex_count, graph->edge_count);
    
    // 释放所有边节点
    for (int i = 0; i < graph->vertex_count; i++) {
        EdgeNode *current = graph->vertex_list[i].first_edge;
        while (current) {
            EdgeNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
    
    free(graph);
}

/**
 * 添加顶点到邻接表图
 * @param graph 图指针
 * @param name 顶点名称
 * @return 成功返回true，失败返回false
 */
bool add_list_vertex(ListGraph *graph, const char *name) {
    if (!graph || !name) {
        printf("错误：图指针或顶点名称为空\n");
        return false;
    }
    
    if (graph->vertex_count >= MAX_VERTICES) {
        printf("错误：顶点数量已达上限\n");
        return false;
    }
    
    // 检查顶点是否已存在
    if (find_list_vertex(graph, name) != -1) {
        printf("错误：顶点 '%s' 已存在\n", name);
        return false;
    }
    
    // 添加新顶点
    strcpy(graph->vertex_list[graph->vertex_count].name, name);
    graph->vertex_list[graph->vertex_count].first_edge = NULL;
    graph->vertex_count++;
    
    printf("成功添加顶点：%s (索引：%d)\n", name, graph->vertex_count - 1);
    return true;
}

/**
 * 添加边到邻接表图
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @param weight 边的权重
 * @return 成功返回true，失败返回false
 */
bool add_list_edge(ListGraph *graph, int from, int to, int weight) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return false;
    }
    
    if (from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        printf("错误：顶点索引越界 (%d, %d)\n", from, to);
        return false;
    }
    
    // 检查边是否已存在
    if (has_list_edge(graph, from, to)) {
        printf("警告：边 (%d -> %d) 已存在\n", from, to);
        return false;
    }
    
    // 创建新的边节点
    EdgeNode *new_edge = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (!new_edge) {
        printf("错误：内存分配失败\n");
        return false;
    }
    
    new_edge->vertex = to;
    new_edge->weight = weight;
    new_edge->next = graph->vertex_list[from].first_edge;
    graph->vertex_list[from].first_edge = new_edge;
    
    graph->edge_count++;
    
    // 如果是无向图，添加反向边
    if (!graph->is_directed) {
        EdgeNode *reverse_edge = (EdgeNode*)malloc(sizeof(EdgeNode));
        if (!reverse_edge) {
            printf("错误：内存分配失败\n");
            return false;
        }
        
        reverse_edge->vertex = from;
        reverse_edge->weight = weight;
        reverse_edge->next = graph->vertex_list[to].first_edge;
        graph->vertex_list[to].first_edge = reverse_edge;
    }
    
    printf("成功添加边：%s(%d) -> %s(%d)，权重：%d\n", 
           graph->vertex_list[from].name, from, 
           graph->vertex_list[to].name, to, weight);
    
    return true;
}

/**
 * 删除邻接表图中的边
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @return 成功返回true，失败返回false
 */
bool remove_list_edge(ListGraph *graph, int from, int to) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return false;
    }
    
    if (from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        printf("错误：顶点索引越界 (%d, %d)\n", from, to);
        return false;
    }
    
    // 删除正向边
    EdgeNode *current = graph->vertex_list[from].first_edge;
    EdgeNode *prev = NULL;
    
    while (current && current->vertex != to) {
        prev = current;
        current = current->next;
    }
    
    if (!current) {
        printf("警告：边 (%d -> %d) 不存在\n", from, to);
        return false;
    }
    
    if (prev) {
        prev->next = current->next;
    } else {
        graph->vertex_list[from].first_edge = current->next;
    }
    free(current);
    
    graph->edge_count--;
    
    // 如果是无向图，删除反向边
    if (!graph->is_directed) {
        current = graph->vertex_list[to].first_edge;
        prev = NULL;
        
        while (current && current->vertex != from) {
            prev = current;
            current = current->next;
        }
        
        if (current) {
            if (prev) {
                prev->next = current->next;
            } else {
                graph->vertex_list[to].first_edge = current->next;
            }
            free(current);
        }
    }
    
    printf("成功删除边：%s(%d) -> %s(%d)\n", 
           graph->vertex_list[from].name, from, 
           graph->vertex_list[to].name, to);
    
    return true;
}

/**
 * 查找顶点在邻接表图中的索引
 * @param graph 图指针
 * @param name 顶点名称
 * @return 顶点索引，未找到返回-1
 */
int find_list_vertex(ListGraph *graph, const char *name) {
    if (!graph || !name) return -1;
    
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertex_list[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * 检查邻接表图中是否存在边
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @return 存在返回true，不存在返回false
 */
bool has_list_edge(ListGraph *graph, int from, int to) {
    if (!graph || from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        return false;
    }
    
    EdgeNode *current = graph->vertex_list[from].first_edge;
    while (current) {
        if (current->vertex == to) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/**
 * 获取邻接表图中边的权重
 * @param graph 图指针
 * @param from 起始顶点索引
 * @param to 终止顶点索引
 * @return 边的权重，不存在返回0
 */
int get_list_edge_weight(ListGraph *graph, int from, int to) {
    if (!graph || from < 0 || from >= graph->vertex_count || 
        to < 0 || to >= graph->vertex_count) {
        return 0;
    }
    
    EdgeNode *current = graph->vertex_list[from].first_edge;
    while (current) {
        if (current->vertex == to) {
            return current->weight;
        }
        current = current->next;
    }
    return 0;
}

/**
 * 打印邻接表图的基本信息
 * @param graph 图指针
 */
void print_list_graph(ListGraph *graph) {
    if (!graph) {
        printf("错误：图指针为空\n");
        return;
    }
    
    printf("\n========== 邻接表图信息 ==========\n");
    printf("图类型：%s\n", graph->is_directed ? "有向图" : "无向图");
    printf("顶点数：%d\n", graph->vertex_count);
    printf("边数：%d\n", graph->edge_count);
    
    printf("\n邻接表结构：\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("[%d] %s: ", i, graph->vertex_list[i].name);
        
        EdgeNode *current = graph->vertex_list[i].first_edge;
        if (!current) {
            printf("(无邻接顶点)");
        } else {
            while (current) {
                printf("-> %s(%d)[权重:%d] ", 
                       graph->vertex_list[current->vertex].name, 
                       current->vertex, current->weight);
                current = current->next;
            }
        }
        printf("\n");
    }
    printf("==================================\n");
}

/**
 * 获取顶点的度数（邻接表图）
 * @param graph 图指针
 * @param vertex 顶点索引
 * @return 顶点的度数
 */
int get_vertex_degree(ListGraph *graph, int vertex) {
    if (!graph || vertex < 0 || vertex >= graph->vertex_count) {
        return 0;
    }
    
    int degree = 0;
    EdgeNode *current = graph->vertex_list[vertex].first_edge;
    while (current) {
        degree++;
        current = current->next;
    }
    
    return degree;
}

/**
 * 获取顶点的入度（有向图）
 * @param graph 图指针
 * @param vertex 顶点索引
 * @return 顶点的入度
 */
int get_vertex_in_degree(ListGraph *graph, int vertex) {
    if (!graph || vertex < 0 || vertex >= graph->vertex_count) {
        return 0;
    }
    
    int in_degree = 0;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (has_list_edge(graph, i, vertex)) {
            in_degree++;
        }
    }
    
    return in_degree;
}

/**
 * 获取顶点的出度（有向图）
 * @param graph 图指针
 * @param vertex 顶点索引
 * @return 顶点的出度
 */
int get_vertex_out_degree(ListGraph *graph, int vertex) {
    return get_vertex_degree(graph, vertex);
}

// ============= 辅助数据结构实现 =============

/**
 * 初始化栈
 * @param stack 栈指针
 */
void init_stack(Stack *stack) {
    if (stack) {
        stack->top = -1;
    }
}

/**
 * 检查栈是否为空
 * @param stack 栈指针
 * @return 空返回true，非空返回false
 */
bool is_stack_empty(Stack *stack) {
    return stack && stack->top == -1;
}

/**
 * 入栈操作
 * @param stack 栈指针
 * @param value 要入栈的值
 * @return 成功返回true，失败返回false
 */
bool push(Stack *stack, int value) {
    if (!stack || stack->top >= MAX_VERTICES - 1) {
        return false;
    }
    stack->data[++stack->top] = value;
    return true;
}

/**
 * 出栈操作
 * @param stack 栈指针
 * @param value 出栈值的存储位置
 * @return 成功返回true，失败返回false
 */
bool pop(Stack *stack, int *value) {
    if (!stack || is_stack_empty(stack) || !value) {
        return false;
    }
    *value = stack->data[stack->top--];
    return true;
}

/**
 * 初始化队列
 * @param queue 队列指针
 */
void init_queue(Queue *queue) {
    if (queue) {
        queue->front = queue->rear = -1;
    }
}

/**
 * 检查队列是否为空
 * @param queue 队列指针
 * @return 空返回true，非空返回false
 */
bool is_queue_empty(Queue *queue) {
    return queue && queue->front == -1;
}

/**
 * 入队操作
 * @param queue 队列指针
 * @param value 要入队的值
 * @return 成功返回true，失败返回false
 */
bool enqueue(Queue *queue, int value) {
    if (!queue) return false;
    
    if (queue->front == -1) {
        queue->front = queue->rear = 0;
        queue->data[queue->rear] = value;
    } else if ((queue->rear + 1) % MAX_VERTICES != queue->front) {
        queue->rear = (queue->rear + 1) % MAX_VERTICES;
        queue->data[queue->rear] = value;
    } else {
        return false; // 队列已满
    }
    return true;
}

/**
 * 出队操作
 * @param queue 队列指针
 * @param value 出队值的存储位置
 * @return 成功返回true，失败返回false
 */
bool dequeue(Queue *queue, int *value) {
    if (!queue || is_queue_empty(queue) || !value) {
        return false;
    }
    
    *value = queue->data[queue->front];
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1; // 队列变空
    } else {
        queue->front = (queue->front + 1) % MAX_VERTICES;
    }
    return true;
}

// ============= 图的遍历算法实现 =============

/**
 *  深度优先搜索（DFS）- 邻接矩阵版本（递归实现）
 * 
 *  算法原理：
 * 深度优先搜索是一种用于遍历或搜索树或图的算法。沿着树的深度遍历树的节点，
 * 尽可能深地搜索树的分支。当节点v的所在边都已被探寻过，搜索将回溯到发现
 * 节点v的那条边的起始节点。
 * 
 *  算法步骤：
 * 1. 访问起始顶点，标记为已访问
 * 2. 从起始顶点的邻接顶点中选择一个未访问的顶点
 * 3. 从选中的顶点开始递归执行DFS
 * 4. 如果所有邻接顶点都已访问，回溯到上一层
 * 
 *   复杂度分析：
 * - 时间复杂度：O(V²) （邻接矩阵）
 * - 空间复杂度：O(V) （递归栈 + visited数组）
 * 
 *  遍历示例：
 *     A --- B
 *     |     |
 *     D --- C
 * 
 * 从A开始DFS: A -> B -> C -> D （具体顺序取决于邻接矩阵中的索引顺序）
 * 
 *  
 * - DFS用递归实现，理解递归调用栈的作用
 * - 邻接矩阵版本时间复杂度为O(V²)，需要检查所有可能的边
 * - DFS的非递归实现需要显式使用栈数据结构
 * - DFS可用于检测环、求连通分量、拓扑排序等
 * 
 * @param graph 图指针（邻接矩阵表示）
 * @param start 起始顶点索引
 * @param visited 访问标记数组（VISITED/UNVISITED）
 */
void dfs_matrix(MatrixGraph *graph, int start, int visited[]) {
    if (!graph || start < 0 || start >= graph->vertex_count || !visited) {
        return;
    }
    
    // 步骤1：访问当前顶点，标记为已访问
    visited[start] = VISITED;
    printf("%s(%d) ", graph->vertices[start], start);
    
    // 步骤2：递归访问所有邻接的未访问顶点
    // 遍历邻接矩阵的第start行，查找所有邻接顶点
    for (int i = 0; i < graph->vertex_count; i++) {
        // 检查是否存在边且目标顶点未被访问
        if (graph->adj_matrix[start][i] != NO_EDGE && visited[i] == UNVISITED) {
            dfs_matrix(graph, i, visited);  // 递归调用
        }
    }
}

/**
 * 深度优先搜索 - 邻接矩阵版本（非递归）
 * @param graph 图指针
 * @param start 起始顶点索引
 * @param visited 访问标记数组
 */
void dfs_matrix_iterative(MatrixGraph *graph, int start, int visited[]) {
    if (!graph || start < 0 || start >= graph->vertex_count || !visited) {
        return;
    }
    
    Stack stack;
    init_stack(&stack);
    push(&stack, start);
    
    while (!is_stack_empty(&stack)) {
        int current;
        if (!pop(&stack, &current)) break;
        
        if (visited[current] == UNVISITED) {
            visited[current] = VISITED;
            printf("%s(%d) ", graph->vertices[current], current);
            
            // 将所有邻接的未访问顶点压入栈（逆序以保持访问顺序）
            for (int i = graph->vertex_count - 1; i >= 0; i--) {
                if (graph->adj_matrix[current][i] != NO_EDGE && 
                    visited[i] == UNVISITED) {
                    push(&stack, i);
                }
            }
        }
    }
}

/**
 * 深度优先搜索 - 邻接表版本（递归）
 * @param graph 图指针
 * @param start 起始顶点索引
 * @param visited 访问标记数组
 */
void dfs_list(ListGraph *graph, int start, int visited[]) {
    if (!graph || start < 0 || start >= graph->vertex_count || !visited) {
        return;
    }
    
    // 访问当前顶点
    visited[start] = VISITED;
    printf("%s(%d) ", graph->vertex_list[start].name, start);
    
    // 递归访问所有邻接的未访问顶点
    EdgeNode *current = graph->vertex_list[start].first_edge;
    while (current) {
        if (visited[current->vertex] == UNVISITED) {
            dfs_list(graph, current->vertex, visited);
        }
        current = current->next;
    }
}

/**
 * 深度优先搜索 - 邻接表版本（非递归）
 * @param graph 图指针
 * @param start 起始顶点索引
 * @param visited 访问标记数组
 */
void dfs_list_iterative(ListGraph *graph, int start, int visited[]) {
    if (!graph || start < 0 || start >= graph->vertex_count || !visited) {
        return;
    }
    
    Stack stack;
    init_stack(&stack);
    push(&stack, start);
    
    while (!is_stack_empty(&stack)) {
        int current;
        if (!pop(&stack, &current)) break;
        
        if (visited[current] == UNVISITED) {
            visited[current] = VISITED;
            printf("%s(%d) ", graph->vertex_list[current].name, current);
            
            // 将所有邻接的未访问顶点压入栈
            EdgeNode *edge = graph->vertex_list[current].first_edge;
            while (edge) {
                if (visited[edge->vertex] == UNVISITED) {
                    push(&stack, edge->vertex);
                }
                edge = edge->next;
            }
        }
    }
}

/**
 * 🌊 广度优先搜索（BFS）- 邻接矩阵版本
 * 
 *  算法原理：
 * 广度优先搜索是一种图形搜索算法。从某个顶点开始，首先访问该顶点，然后依次访问
 * 该顶点的所有未访问邻接顶点，接着对每个邻接顶点，再依次访问它们的邻接顶点。
 * 按照距离起始顶点的路径长度逐层访问。
 * 
 *  算法步骤：
 * 1. 访问起始顶点，标记为已访问，入队
 * 2. 队列不空时，取出队首顶点
 * 3. 将该顶点所有未访问的邻接顶点标记为已访问并入队
 * 4. 重复步骤2-3直到队列为空
 * 
 *   复杂度分析：
 * - 时间复杂度：O(V²) （邻接矩阵）
 * - 空间复杂度：O(V) （队列 + visited数组）
 * 
 * 🌊 遍历示例（按层次访问）：
 *     A --- B --- E
 *     |     |     |
 *     D --- C --- F
 * 
 * 从A开始BFS: 
 * 层次0: A
 * 层次1: B, D (A的邻接点)
 * 层次2: C, E (B,D的邻接点)
 * 层次3: F (C,E的邻接点)
 * 结果: A -> B -> D -> C -> E -> F
 * 
 *  队列变化过程：
 * 初始: queue = [A], visited = {A}
 * 第1步: queue = [B,D], visited = {A,B,D}  
 * 第2步: queue = [D,C,E], visited = {A,B,D,C,E}
 * 第3步: queue = [C,E,F], visited = {A,B,D,C,E,F}
 * ...
 * 
 *  
 * - BFS用队列实现，保证按层次访问
 * - BFS可用于找最短路径（无权图）、检测连通性
 * - 与DFS对比：BFS空间复杂度可能更大，但能找到最短路径
 * - 邻接矩阵实现需要检查每个顶点的所有可能邻接点
 * 
 * @param graph 图指针（邻接矩阵表示）
 * @param start 起始顶点索引
 * @param visited 访问标记数组（VISITED/UNVISITED）
 */
void bfs_matrix(MatrixGraph *graph, int start, int visited[]) {
    if (!graph || start < 0 || start >= graph->vertex_count || !visited) {
        return;
    }
    
    Queue queue;
    init_queue(&queue);
    
    // 步骤1：访问起始顶点，标记并入队
    visited[start] = VISITED;
    printf("%s(%d) ", graph->vertices[start], start);
    enqueue(&queue, start);
    
    // 步骤2：队列不空时循环处理
    while (!is_queue_empty(&queue)) {
        int current;
        if (!dequeue(&queue, &current)) break;  // 取出队首顶点
        
        // 步骤3：检查当前顶点的所有邻接顶点
        for (int i = 0; i < graph->vertex_count; i++) {
            // 如果存在边且邻接顶点未被访问
            if (graph->adj_matrix[current][i] != NO_EDGE && 
                visited[i] == UNVISITED) {
                visited[i] = VISITED;  // 标记为已访问
                printf("%s(%d) ", graph->vertices[i], i);  // 输出顶点
                enqueue(&queue, i);    // 加入队列等待处理
            }
        }
    }
}

/**
 * 广度优先搜索 - 邻接表版本
 * @param graph 图指针
 * @param start 起始顶点索引
 * @param visited 访问标记数组
 */
void bfs_list(ListGraph *graph, int start, int visited[]) {
    if (!graph || start < 0 || start >= graph->vertex_count || !visited) {
        return;
    }
    
    Queue queue;
    init_queue(&queue);
    
    visited[start] = VISITED;
    printf("%s(%d) ", graph->vertex_list[start].name, start);
    enqueue(&queue, start);
    
    while (!is_queue_empty(&queue)) {
        int current;
        if (!dequeue(&queue, &current)) break;
        
        // 访问所有邻接的未访问顶点
        EdgeNode *edge = graph->vertex_list[current].first_edge;
        while (edge) {
            if (visited[edge->vertex] == UNVISITED) {
                visited[edge->vertex] = VISITED;
                printf("%s(%d) ", graph->vertex_list[edge->vertex].name, edge->vertex);
                enqueue(&queue, edge->vertex);
            }
            edge = edge->next;
        }
    }
}

/**
 * 遍历整个邻接矩阵图（DFS，处理非连通图）
 * @param graph 图指针
 */
void traverse_all_matrix_dfs(MatrixGraph *graph) {
    if (!graph) return;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    printf("DFS遍历整个图：");
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            printf("\n连通分量 %d：", i);
            dfs_matrix(graph, i, visited);
        }
    }
    printf("\n");
}

/**
 * 遍历整个邻接矩阵图（BFS，处理非连通图）
 * @param graph 图指针
 */
void traverse_all_matrix_bfs(MatrixGraph *graph) {
    if (!graph) return;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    printf("BFS遍历整个图：");
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            printf("\n连通分量 %d：", i);
            bfs_matrix(graph, i, visited);
        }
    }
    printf("\n");
}

/**
 * 遍历整个邻接表图（DFS，处理非连通图）
 * @param graph 图指针
 */
void traverse_all_list_dfs(ListGraph *graph) {
    if (!graph) return;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    printf("DFS遍历整个图：");
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            printf("\n连通分量 %d：", i);
            dfs_list(graph, i, visited);
        }
    }
    printf("\n");
}

/**
 * 遍历整个邻接表图（BFS，处理非连通图）
 * @param graph 图指针
 */
void traverse_all_list_bfs(ListGraph *graph) {
    if (!graph) return;
    
    int visited[MAX_VERTICES];
    for (int i = 0; i < graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    
    printf("BFS遍历整个图：");
    for (int i = 0; i < graph->vertex_count; i++) {
        if (visited[i] == UNVISITED) {
            printf("\n连通分量 %d：", i);
            bfs_list(graph, i, visited);
        }
    }
    printf("\n");
} 