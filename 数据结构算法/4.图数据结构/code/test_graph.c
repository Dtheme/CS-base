/* test_graph.c - 图数据结构测试程序
 *   图算法综合测试
 * create by: zw.duan
 */

#include "graph.h"

// 测试函数声明
void test_basic_operations(void);
void test_graph_traversal(void);
void test_minimum_spanning_tree(void);
void test_shortest_path(void);
void test_topological_sort(void);
void test_connectivity(void);
void test_advanced_algorithms(void);
void print_test_header(const char *testName);
void print_test_result(const char *testName, bool passed);

// 辅助函数实现
void print_test_header(const char *testName) {
    printf("\n==================================================\n");
    printf("测试项目: %s\n", testName);
    printf("==================================================\n");
}

void print_test_result(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

// 创建测试图
MatrixGraph* create_test_matrix_graph(void) {
    MatrixGraph *graph = create_matrix_graph(6, false);
    
    // 添加顶点
    add_matrix_vertex(graph, "A");
    add_matrix_vertex(graph, "B");
    add_matrix_vertex(graph, "C");
    add_matrix_vertex(graph, "D");
    add_matrix_vertex(graph, "E");
    add_matrix_vertex(graph, "F");
    
    // 添加边（无向图）
    add_matrix_edge(graph, 0, 1, 2);  // A-B: 2
    add_matrix_edge(graph, 0, 2, 4);  // A-C: 4
    add_matrix_edge(graph, 1, 2, 1);  // B-C: 1
    add_matrix_edge(graph, 1, 3, 7);  // B-D: 7
    add_matrix_edge(graph, 2, 4, 3);  // C-E: 3
    add_matrix_edge(graph, 3, 4, 2);  // D-E: 2
    add_matrix_edge(graph, 3, 5, 1);  // D-F: 1
    add_matrix_edge(graph, 4, 5, 5);  // E-F: 5
    
    return graph;
}

ListGraph* create_test_list_graph(void) {
    ListGraph *graph = create_list_graph(6, false);
    
    // 添加顶点
    add_list_vertex(graph, "A");
    add_list_vertex(graph, "B");
    add_list_vertex(graph, "C");
    add_list_vertex(graph, "D");
    add_list_vertex(graph, "E");
    add_list_vertex(graph, "F");
    
    // 添加边（无向图）
    add_list_edge(graph, 0, 1, 2);  // A-B: 2
    add_list_edge(graph, 0, 2, 4);  // A-C: 4
    add_list_edge(graph, 1, 2, 1);  // B-C: 1
    add_list_edge(graph, 1, 3, 7);  // B-D: 7
    add_list_edge(graph, 2, 4, 3);  // C-E: 3
    add_list_edge(graph, 3, 4, 2);  // D-E: 2
    add_list_edge(graph, 3, 5, 1);  // D-F: 1
    add_list_edge(graph, 4, 5, 5);  // E-F: 5
    
    return graph;
}

MatrixGraph* create_directed_test_graph(void) {
    MatrixGraph *graph = create_matrix_graph(6, true);
    
    // 添加顶点
    add_matrix_vertex(graph, "V0");
    add_matrix_vertex(graph, "V1");
    add_matrix_vertex(graph, "V2");
    add_matrix_vertex(graph, "V3");
    add_matrix_vertex(graph, "V4");
    add_matrix_vertex(graph, "V5");
    
    // 添加有向边（用于拓扑排序测试）
    add_matrix_edge(graph, 0, 1, 1);  // V0->V1
    add_matrix_edge(graph, 0, 2, 1);  // V0->V2
    add_matrix_edge(graph, 1, 3, 1);  // V1->V3
    add_matrix_edge(graph, 2, 3, 1);  // V2->V3
    add_matrix_edge(graph, 2, 4, 1);  // V2->V4
    add_matrix_edge(graph, 3, 5, 1);  // V3->V5
    add_matrix_edge(graph, 4, 5, 1);  // V4->V5
    
    return graph;
}


void interactive_menu() {
    int choice;
    
    while (1) {
        printf("\n========== 图算法测试菜单 ==========\n");
        printf("1. 基本操作测试\n");
        printf("2. 图遍历测试\n");
        printf("3. 最小生成树测试\n");
        printf("4. 最短路径测试\n");
        printf("5. 拓扑排序测试\n");
        printf("6. 连通性测试\n");
        printf("7. 高级算法测试\n");
        printf("0. 退出程序\n");
        printf("==================================\n");
        printf("请选择测试项目 (0-7): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字!\n");
            while (getchar() != '\n'); // 清除输入缓冲区
            continue;
        }
        
        switch (choice) {
            case 1:
                test_basic_operations();
                break;
            case 2:
                test_graph_traversal();
                break;
            case 3:
                test_minimum_spanning_tree();
                break;
            case 4:
                test_shortest_path();
                break;
            case 5:
                test_topological_sort();
                break;
            case 6:
                test_connectivity();
                break;
            case 7:
                test_advanced_algorithms();
                break;
            case 0:
                printf("感谢使用图算法测试程序!\n");
                return;
            default:
                printf("无效选择，请重新输入!\n");
                break;
        }
        
        printf("\n按任意键继续...");
        getchar(); // 清除换行符
        getchar(); // 等待用户输入
    }
}

void test_basic_operations(void) {
    print_test_header("图基本操作测试");
    
    // 测试邻接矩阵图
    printf("\n--- 邻接矩阵图测试 ---\n");
    MatrixGraph *matrix_graph = create_test_matrix_graph();
    print_matrix_graph(matrix_graph);
    print_matrix_adjacency_matrix(matrix_graph);
    
    // 测试邻接表图
    printf("\n--- 邻接表图测试 ---\n");
    ListGraph *list_graph = create_test_list_graph();
    print_list_graph(list_graph);
    
    // 测试边操作
    printf("\n--- 边操作测试 ---\n");
    printf("检查边 A-B 是否存在：%s\n", 
           has_matrix_edge(matrix_graph, 0, 1) ? "是" : "否");
    printf("获取边 A-B 的权重：%d\n", 
           get_matrix_edge_weight(matrix_graph, 0, 1));
    
    printf("删除边 A-B...\n");
    remove_matrix_edge(matrix_graph, 0, 1);
    printf("检查边 A-B 是否存在：%s\n", 
           has_matrix_edge(matrix_graph, 0, 1) ? "是" : "否");
    
    // 清理内存
    destroy_matrix_graph(matrix_graph);
    destroy_list_graph(list_graph);
    
    print_test_result("图基本操作测试", true);
}

void test_graph_traversal(void) {
    print_test_header("图遍历算法测试");
    
    MatrixGraph *matrix_graph = create_test_matrix_graph();
    ListGraph *list_graph = create_test_list_graph();
    
    int visited[MAX_VERTICES];
    
    // 邻接矩阵图遍历
    printf("\n--- 邻接矩阵图遍历 ---\n");
    
    // DFS递归
    for (int i = 0; i < matrix_graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    printf("DFS遍历（递归）：");
    dfs_matrix(matrix_graph, 0, visited);
    printf("\n");
    
    // DFS非递归
    for (int i = 0; i < matrix_graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    printf("DFS遍历（非递归）：");
    dfs_matrix_iterative(matrix_graph, 0, visited);
    printf("\n");
    
    // BFS
    for (int i = 0; i < matrix_graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    printf("BFS遍历：");
    bfs_matrix(matrix_graph, 0, visited);
    printf("\n");
    
    // 邻接表图遍历
    printf("\n--- 邻接表图遍历 ---\n");
    
    // DFS递归
    for (int i = 0; i < list_graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    printf("DFS遍历（递归）：");
    dfs_list(list_graph, 0, visited);
    printf("\n");
    
    // BFS
    for (int i = 0; i < list_graph->vertex_count; i++) {
        visited[i] = UNVISITED;
    }
    printf("BFS遍历：");
    bfs_list(list_graph, 0, visited);
    printf("\n");
    
    // 清理内存
    destroy_matrix_graph(matrix_graph);
    destroy_list_graph(list_graph);
    
    print_test_result("图遍历算法测试", true);
}

void test_minimum_spanning_tree(void) {
    print_test_header("最小生成树算法测试");
    
    MatrixGraph *matrix_graph = create_test_matrix_graph();
    ListGraph *list_graph = create_test_list_graph();
    
    Edge mst[MAX_VERTICES];
    
    // 邻接矩阵图MST测试
    printf("\n--- 邻接矩阵图MST ---\n");
    
    // Prim算法
    int prim_count = prim_matrix(matrix_graph, mst);
    int prim_weight = calculate_mst_weight(mst, prim_count);
    print_mst(mst, prim_count, prim_weight);
    validate_mst(mst, prim_count, matrix_graph->vertex_count);
    
    // Kruskal算法
    int kruskal_count = kruskal_matrix(matrix_graph, mst);
    int kruskal_weight = calculate_mst_weight(mst, kruskal_count);
    print_mst(mst, kruskal_count, kruskal_weight);
    validate_mst(mst, kruskal_count, matrix_graph->vertex_count);
    
    // 邻接表图MST测试
    printf("\n--- 邻接表图MST ---\n");
    
    // Prim算法
    prim_count = prim_list(list_graph, mst);
    prim_weight = calculate_mst_weight(mst, prim_count);
    print_mst(mst, prim_count, prim_weight);
    
    // Kruskal算法
    kruskal_count = kruskal_list(list_graph, mst);
    kruskal_weight = calculate_mst_weight(mst, kruskal_count);
    print_mst(mst, kruskal_count, kruskal_weight);
    
    // 清理内存
    destroy_matrix_graph(matrix_graph);
    destroy_list_graph(list_graph);
    
    print_test_result("最小生成树算法测试", true);
}

void test_shortest_path(void) {
    print_test_header("最短路径算法测试");
    
    MatrixGraph *matrix_graph = create_test_matrix_graph();
    ListGraph *list_graph = create_test_list_graph();
    
    int distances[MAX_VERTICES];
    int predecessors[MAX_VERTICES];
    
    // Dijkstra算法测试
    printf("\n--- Dijkstra算法测试 ---\n");
    
    // 邻接矩阵版本
    if (dijkstra_matrix(matrix_graph, 0, distances, predecessors)) {
        print_distances(distances, matrix_graph->vertex_count);
        print_all_shortest_paths(distances, predecessors, 
                                matrix_graph->vertex_count, 0);
    }
    
    // 邻接表版本
    if (dijkstra_list(list_graph, 0, distances, predecessors)) {
        print_distances(distances, list_graph->vertex_count);
        print_all_shortest_paths(distances, predecessors, 
                                list_graph->vertex_count, 0);
    }
    
    // Floyd算法测试
    printf("\n--- Floyd算法测试 ---\n");
    int floyd_dist[MAX_VERTICES][MAX_VERTICES];
    int floyd_pred[MAX_VERTICES][MAX_VERTICES];
    
    floyd_matrix(matrix_graph, floyd_dist, floyd_pred);
    
    printf("Floyd距离矩阵：\n");
    for (int i = 0; i < matrix_graph->vertex_count; i++) {
        for (int j = 0; j < matrix_graph->vertex_count; j++) {
            if (floyd_dist[i][j] == INFINITY) {
                printf("  ∞");
            } else {
                printf("%3d", floyd_dist[i][j]);
            }
        }
        printf("\n");
    }
    
    // Bellman-Ford算法测试
    printf("\n--- Bellman-Ford算法测试 ---\n");
    if (bellman_ford_matrix(matrix_graph, 0, distances, predecessors)) {
        print_distances(distances, matrix_graph->vertex_count);
        print_all_shortest_paths(distances, predecessors, 
                                matrix_graph->vertex_count, 0);
    }
    
    // 清理内存
    destroy_matrix_graph(matrix_graph);
    destroy_list_graph(list_graph);
    
    print_test_result("最短路径算法测试", true);
}

void test_topological_sort(void) {
    print_test_header("拓扑排序算法测试");
    
    MatrixGraph *directed_graph = create_directed_test_graph();
    
    print_matrix_graph(directed_graph);
    
    int result[MAX_VERTICES];
    
    // Kahn算法测试
    if (topological_sort_kahn_matrix(directed_graph, result)) {
        printf("Kahn算法拓扑排序结果：");
        for (int i = 0; i < directed_graph->vertex_count; i++) {
            printf("%s(%d) ", directed_graph->vertices[result[i]], result[i]);
        }
        printf("\n");
    }
    
    // DFS算法测试
    if (topological_sort_dfs_matrix(directed_graph, result)) {
        printf("DFS算法拓扑排序结果：");
        for (int i = 0; i < directed_graph->vertex_count; i++) {
            printf("%s(%d) ", directed_graph->vertices[result[i]], result[i]);
        }
        printf("\n");
    }
    
    // 环检测测试
    printf("环检测结果：%s\n", 
           has_cycle_matrix(directed_graph) ? "存在环" : "无环");
    
    // 清理内存
    destroy_matrix_graph(directed_graph);
    
    print_test_result("拓扑排序算法测试", true);
}

void test_connectivity(void) {
    print_test_header("图连通性算法测试");
    
    MatrixGraph *matrix_graph = create_test_matrix_graph();
    ListGraph *list_graph = create_test_list_graph();
    
    // 连通性测试
    printf("邻接矩阵图连通性：%s\n", 
           is_connected_matrix(matrix_graph) ? "连通" : "不连通");
    printf("邻接表图连通性：%s\n", 
           is_connected_list(list_graph) ? "连通" : "不连通");
    
    // 连通分量数测试
    printf("邻接矩阵图连通分量数：%d\n", 
           count_connected_components_matrix(matrix_graph));
    printf("邻接表图连通分量数：%d\n", 
           count_connected_components_list(list_graph));
    
    // 路径存在性测试
    printf("A到E的路径：%s\n", 
           has_path_matrix(matrix_graph, 0, 4) ? "存在" : "不存在");
    printf("B到F的路径：%s\n", 
           has_path_list(list_graph, 1, 5) ? "存在" : "不存在");
    
    // 清理内存
    destroy_matrix_graph(matrix_graph);
    destroy_list_graph(list_graph);
    
    print_test_result("图连通性算法测试", true);
}

void test_advanced_algorithms(void) {
    print_test_header("高级图算法测试");
    
    // 测试关键路径算法（AOE网络）
    printf("\n--- AOE网络关键路径测试 ---\n");
    MatrixGraph *aoe_graph = create_matrix_graph(6, true);
    
    // 构建AOE网络示例
    add_matrix_vertex(aoe_graph, "V0");
    add_matrix_vertex(aoe_graph, "V1"); 
    add_matrix_vertex(aoe_graph, "V2");
    add_matrix_vertex(aoe_graph, "V3");
    add_matrix_vertex(aoe_graph, "V4");
    add_matrix_vertex(aoe_graph, "V5");
    
    // 添加活动（边）
    add_matrix_edge(aoe_graph, 0, 1, 3);  // a1: 3天
    add_matrix_edge(aoe_graph, 0, 2, 2);  // a2: 2天
    add_matrix_edge(aoe_graph, 1, 3, 4);  // a3: 4天
    add_matrix_edge(aoe_graph, 1, 4, 3);  // a4: 3天
    add_matrix_edge(aoe_graph, 2, 3, 2);  // a5: 2天
    add_matrix_edge(aoe_graph, 2, 5, 3);  // a6: 3天
    add_matrix_edge(aoe_graph, 3, 5, 2);  // a7: 2天
    add_matrix_edge(aoe_graph, 4, 5, 1);  // a8: 1天
    
    CriticalPath *cp = find_critical_path_matrix(aoe_graph);
    if (cp) {
        print_critical_path(cp, aoe_graph);
        destroy_critical_path(cp);
    }
    destroy_matrix_graph(aoe_graph);
    
    // 测试强连通分量
    printf("\n--- 强连通分量测试 ---\n");
    MatrixGraph *scc_graph = create_matrix_graph(5, true);
    
    add_matrix_vertex(scc_graph, "A");
    add_matrix_vertex(scc_graph, "B");
    add_matrix_vertex(scc_graph, "C");
    add_matrix_vertex(scc_graph, "D");
    add_matrix_vertex(scc_graph, "E");
    
    // 构建有强连通分量的图
    add_matrix_edge(scc_graph, 0, 1, 1);  // A->B
    add_matrix_edge(scc_graph, 1, 2, 1);  // B->C
    add_matrix_edge(scc_graph, 2, 0, 1);  // C->A (形成强连通分量)
    add_matrix_edge(scc_graph, 1, 3, 1);  // B->D
    add_matrix_edge(scc_graph, 3, 4, 1);  // D->E
    
    StronglyConnectedComponents *scc = find_scc_matrix(scc_graph);
    if (scc) {
        printf("强连通分量数量: %d\n", scc->component_count);
        destroy_scc(scc);
    }
    destroy_matrix_graph(scc_graph);
    
    // 测试二分图检测
    printf("\n--- 二分图检测测试 ---\n");
    MatrixGraph *bipartite_graph = create_matrix_graph(4, false);
    
    add_matrix_vertex(bipartite_graph, "A");
    add_matrix_vertex(bipartite_graph, "B");
    add_matrix_vertex(bipartite_graph, "C");
    add_matrix_vertex(bipartite_graph, "D");
    
    // 构建二分图
    add_matrix_edge(bipartite_graph, 0, 2, 1);  // A-C
    add_matrix_edge(bipartite_graph, 0, 3, 1);  // A-D
    add_matrix_edge(bipartite_graph, 1, 2, 1);  // B-C
    add_matrix_edge(bipartite_graph, 1, 3, 1);  // B-D
    
    int coloring[MAX_VERTICES];
    if (is_bipartite_matrix(bipartite_graph, coloring)) {
        printf("该图是二分图\n");
        printf("顶点染色情况:\n");
        for (int i = 0; i < bipartite_graph->vertex_count; i++) {
            printf("%s: 颜色%d\n", bipartite_graph->vertices[i], coloring[i]);
        }
    } else {
        printf("该图不是二分图\n");
    }
    
    destroy_matrix_graph(bipartite_graph);
    
    print_test_result("高级图算法测试", true);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                图数据结构测试用例              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    test_basic_operations();
    test_graph_traversal();
    test_minimum_spanning_tree();
    test_shortest_path();
    test_topological_sort();
    test_connectivity();
    test_advanced_algorithms();
    
    printf("\n============================================================\n");
    printf("图数据结构完整功能测试完成！\n");
    printf("============================================================\n");
    
    return 0;
}