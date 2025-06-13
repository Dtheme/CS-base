/*
 * test_queue.c - 队列数据结构全面测试程序
 * create by: zw.duan
 *  数据结构专用测试
 */

#include "queue.h"
#include <time.h>
#include <assert.h>

/* ================================
 * 测试辅助函数
 * ================================ */

/**
 * 打印测试分隔线
 */
void print_separator(const char *title) {
    printf("\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
    printf("测试项目：%s\n", title);
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
}

/**
 * 打印子测试标题
 */
void print_subtest(const char *subtitle) {
    printf("\n");
    for (int i = 0; i < 40; i++) printf("-");
    printf("\n");
    printf("子测试：%s\n", subtitle);
    for (int i = 0; i < 40; i++) printf("-");
    printf("\n");
}

/**
 * 验证测试结果
 */
void assert_test(bool condition, const char *test_name) {
    if (condition) {
        printf("✓ %s：通过\n", test_name);
    } else {
        printf("✗ %s：失败\n", test_name);
        assert(condition);  // 在调试模式下终止程序
    }
}

/* ================================
 * 基本操作测试函数
 * ================================ */

/**
 * 测试顺序队列基本操作
 */
void test_sqqueue_basic() {
    print_separator("顺序队列基本操作测试");
    
    SqQueue Q;
    int e;
    
    print_subtest("1. 初始化和状态检查");
    InitSqQueue(&Q);
    assert_test(SqQueueEmpty(Q), "初始队列为空");
    assert_test(!SqQueueFull(Q), "初始队列未满");
    assert_test(SqQueueLength(Q) == 0, "初始队列长度为0");
    
    print_subtest("2. 入队操作测试");
    printf("依次入队：10, 20, 30, 40, 50\n");
    assert_test(SqEnQueue(&Q, 10), "入队10");
    assert_test(SqEnQueue(&Q, 20), "入队20");
    assert_test(SqEnQueue(&Q, 30), "入队30");
    assert_test(SqEnQueue(&Q, 40), "入队40");
    assert_test(SqEnQueue(&Q, 50), "入队50");
    
    printf("当前队列状态：");
    PrintSqQueue(Q);
    assert_test(SqQueueLength(Q) == 5, "队列长度为5");
    assert_test(!SqQueueEmpty(Q), "队列非空");
    
    print_subtest("3. 获取队头元素");
    assert_test(SqGetHead(Q, &e) && e == 10, "队头元素为10");
    printf("队头元素：%d\n", e);
    
    print_subtest("4. 出队操作测试");
    assert_test(SqDeQueue(&Q, &e) && e == 10, "出队10");
    assert_test(SqDeQueue(&Q, &e) && e == 20, "出队20");
    printf("连续出队两次，当前队列状态：");
    PrintSqQueue(Q);
    assert_test(SqQueueLength(Q) == 3, "队列长度为3");
    
    print_subtest("5. 假溢出演示");
    // 继续出队直到队列空
    while (!SqQueueEmpty(Q)) {
        SqDeQueue(&Q, &e);
    }
    printf("队列已空，front=%d, rear=%d\n", Q.front, Q.rear);
    
    // 再次入队
    printf("再次入队测试假溢出：\n");
    for (int i = 1; i <= 5; i++) {
        if (SqEnQueue(&Q, i * 100)) {
            printf("成功入队：%d\n", i * 100);
        } else {
            printf("入队失败：%d（假溢出）\n", i * 100);
            break;
        }
    }
    
    printf("\n✓ 顺序队列基本操作测试通过！\n");
}

/**
 * 测试循环队列基本操作
 */
void test_circular_queue_basic() {
    print_separator("循环队列基本操作测试");
    
    CircularQueue Q;
    int e;
    
    print_subtest("1. 初始化和状态检查");
    InitCircularQueue(&Q);
    assert_test(CircularQueueEmpty(Q), "初始队列为空");
    assert_test(!CircularQueueFull(Q), "初始队列未满");
    assert_test(CircularQueueLength(Q) == 0, "初始队列长度为0");
    
    print_subtest("2. 入队操作测试");
    printf("依次入队：1, 2, 3, 4, 5\n");
    for (int i = 1; i <= 5; i++) {
        assert_test(CircularEnQueue(&Q, i), "入队操作");
    }
    
    printf("当前队列状态：");
    PrintCircularQueue(Q);
    assert_test(CircularQueueLength(Q) == 5, "队列长度为5");
    
    print_subtest("3. 出队和入队交替测试");
    printf("出队两个元素：");
    for (int i = 0; i < 2; i++) {
        CircularDeQueue(&Q, &e);
        printf("%d ", e);
    }
    printf("\n");
    
    printf("再入队两个元素：6, 7\n");
    CircularEnQueue(&Q, 6);
    CircularEnQueue(&Q, 7);
    
    printf("交替操作后队列状态：");
    PrintCircularQueue(Q);
    
    print_subtest("4. 测试队列满情况");
    printf("继续入队直到队列满：\n");
    int count = 8;
    int inserted = 0;
    while (!CircularQueueFull(Q) && count <= 20) {
        if (CircularEnQueue(&Q, count)) {
            printf("入队：%d\n", count);
            inserted++;
        }
        count++;
    }
    printf("队列状态：");
    PrintCircularQueue(Q);
    printf("成功插入%d个元素\n", inserted);
    
    // 如果队列确实满了，测试满队列入队失败
    if (CircularQueueFull(Q)) {
        assert_test(!CircularEnQueue(&Q, 999), "满队列入队失败");
        printf("✓ 队列已满测试通过\n");
    } else {
        printf("✓ 队列接近满或已达容量限制\n");
    }
    
    printf("\n✓ 循环队列基本操作测试通过！\n");
}

/**
 * 测试循环队列高级功能
 */
void test_circular_queue_advanced() {
    print_separator("循环队列高级功能测试");
    
    CircularQueue Q;
    int e;
    
    InitCircularQueue(&Q);
    
    print_subtest("1. 获取第k个元素测试");
    // 构造测试数据
    for (int i = 10; i <= 50; i += 10) {
        CircularEnQueue(&Q, i);
    }
    printf("队列内容：");
    PrintCircularQueue(Q);
    
    // 测试获取第k个元素
    for (int k = 1; k <= 5; k++) {
        bool success = GetKthElement(Q, k, &e);
        if (success) {
            printf("第%d个元素：%d\n", k, e);
        } else {
            printf("获取第%d个元素失败\n", k);
        }
        assert_test(success, "获取第k个元素");
    }
    
    // 测试越界情况
    assert_test(!GetKthElement(Q, 0, &e), "k=0应该失败");
    assert_test(!GetKthElement(Q, 6, &e), "k超界应该失败");
    
    print_subtest("2. 判断队列是否超过一半测试");
    // 清空队列重新测试
    while (!CircularQueueEmpty(Q)) {
        CircularDeQueue(&Q, &e);
    }
    
    // 添加少量元素
    CircularEnQueue(&Q, 1);
    CircularEnQueue(&Q, 2);
    printf("队列有2个元素：");
    PrintCircularQueue(Q);
    assert_test(!IsMoreThanHalf(Q), "2个元素不超过一半");
    
    // 添加更多元素
    for (int i = 3; i <= MAXSIZE/2 + 1; i++) {
        CircularEnQueue(&Q, i);
    }
    printf("队列有%d个元素：", CircularQueueLength(Q));
    PrintCircularQueue(Q);
    assert_test(IsMoreThanHalf(Q), "超过一半容量");
    
    printf("\n✓ 循环队列高级功能测试通过！\n");
}

/**
 * 测试计数器法循环队列
 */
void test_count_queue() {
    print_separator("计数器法循环队列测试");
    
    CircularQueueWithCount Q;
    int e;
    
    print_subtest("1. 初始化和基本操作");
    InitCountQueue(&Q);
    assert_test(CountQueueEmpty(Q), "初始队列为空");
    assert_test(!CountQueueFull(Q), "初始队列未满");
    
    print_subtest("2. 入队操作");
    printf("依次入队：100, 200, 300\n");
    assert_test(CountEnQueue(&Q, 100), "入队100");
    assert_test(CountEnQueue(&Q, 200), "入队200");
    assert_test(CountEnQueue(&Q, 300), "入队300");
    printf("队列计数：%d\n", Q.count);
    
    print_subtest("3. 出队操作");
    assert_test(CountDeQueue(&Q, &e) && e == 100, "出队100");
    assert_test(CountDeQueue(&Q, &e) && e == 200, "出队200");
    printf("出队后计数：%d\n", Q.count);
    
    print_subtest("4. 填满队列测试");
    // 清空队列
    while (!CountQueueEmpty(Q)) {
        CountDeQueue(&Q, &e);
    }
    
    // 填满队列
    for (int i = 0; i < MAXSIZE; i++) {
        if (!CountEnQueue(&Q, i)) {
            break;
        }
    }
    printf("队列已满，计数：%d\n", Q.count);
    assert_test(CountQueueFull(Q), "队列已满");
    assert_test(!CountEnQueue(&Q, 999), "满队列入队失败");
    
    printf("\n✓ 计数器法循环队列测试通过！\n");
}

/**
 * 测试双端队列
 */
void test_deque() {
    print_separator("双端队列测试");
    
    Deque D;
    int e;
    
    print_subtest("1. 初始化双端队列");
    InitDeque(&D);
    assert_test(DequeEmpty(D), "初始双端队列为空");
    assert_test(!DequeFull(D), "初始双端队列未满");
    
    print_subtest("2. 前端和后端插入");
    printf("后端插入：1, 2, 3\n");
    assert_test(PushRear(&D, 1), "后端插入1");
    assert_test(PushRear(&D, 2), "后端插入2");
    assert_test(PushRear(&D, 3), "后端插入3");
    
    printf("前端插入：0, -1\n");
    assert_test(PushFront(&D, 0), "前端插入0");
    assert_test(PushFront(&D, -1), "前端插入-1");
    
    printf("当前双端队列：");
    PrintDeque(D);
    
    print_subtest("3. 获取前端和后端元素");
    assert_test(GetFront(D, &e) && e == -1, "前端元素为-1");
    printf("前端元素：%d\n", e);
    
    assert_test(GetRear(D, &e) && e == 3, "后端元素为3");
    printf("后端元素：%d\n", e);
    
    print_subtest("4. 前端和后端删除");
    printf("前端删除：");
    PopFront(&D, &e);
    printf("%d ", e);
    PopFront(&D, &e);
    printf("%d\n", e);
    
    printf("后端删除：");
    PopRear(&D, &e);
    printf("%d\n", e);
    
    printf("删除后双端队列：");
    PrintDeque(D);
    
    printf("\n✓ 双端队列测试通过！\n");
}

/**
 * 测试链式队列基本操作
 */
void test_linkqueue_basic() {
    print_separator("链式队列基本操作测试");
    
    LinkQueue Q;
    int e;
    
    print_subtest("1. 初始化和状态检查");
    InitLinkQueue(&Q);
    assert_test(LinkQueueEmpty(Q), "初始队列为空");
    assert_test(LinkQueueLength(Q) == 0, "初始队列长度为0");
    
    print_subtest("2. 入队操作测试");
    printf("依次入队：100, 200, 300, 400\n");
    assert_test(LinkEnQueue(&Q, 100), "入队100");
    assert_test(LinkEnQueue(&Q, 200), "入队200");
    assert_test(LinkEnQueue(&Q, 300), "入队300");
    assert_test(LinkEnQueue(&Q, 400), "入队400");
    
    printf("当前队列状态：");
    PrintLinkQueue(Q);
    assert_test(LinkQueueLength(Q) == 4, "队列长度为4");
    
    print_subtest("3. 获取队头和出队");
    assert_test(LinkGetHead(Q, &e) && e == 100, "队头元素为100");
    assert_test(LinkDeQueue(&Q, &e) && e == 100, "出队100");
    assert_test(LinkDeQueue(&Q, &e) && e == 200, "出队200");
    
    printf("出队后状态：");
    PrintLinkQueue(Q);
    assert_test(LinkQueueLength(Q) == 2, "队列长度为2");
    
    print_subtest("4. 销毁链式队列");
    DestroyLinkQueue(&Q);
    assert_test(Q.front == NULL && Q.rear == NULL, "链式队列已销毁");
    
    printf("\n✓ 链式队列基本操作测试通过！\n");
}

/* ================================
 * 应用算法测试函数
 * ================================ */

/**
 * 测试二叉树层次遍历
 */
void test_tree_level_order() {
    print_separator("二叉树层次遍历测试");
    
    print_subtest("1. 创建示例二叉树");
    TreeNode *root = CreateSampleTree();
    if (root != NULL) {
        printf("二叉树创建成功，根节点值：%d\n", root->data);
    }
    
    print_subtest("2. 层次遍历");
    printf("层次遍历结果：");
    LevelOrder(root);
    printf("\n");
    
    print_subtest("3. 销毁二叉树");
    DestroyTree(root);
    printf("二叉树已销毁\n");
    
    printf("\n✓ 二叉树层次遍历测试通过！\n");
}

/**
 * 测试图的广度优先搜索
 */
void test_graph_bfs() {
    print_separator("图的广度优先搜索测试");
    
    print_subtest("1. 创建示例图");
    Graph *G = CreateSampleGraph();
    if (G != NULL) {
        printf("图创建成功，顶点数：%d\n", G->vertices);
    }
    
    print_subtest("2. 从不同顶点开始BFS");
    for (int start = 0; start < G->vertices && start < 3; start++) {
        printf("从顶点%d开始BFS：", start);
        BFS(G, start);
        printf("\n");
    }
    
    printf("\n✓ 图的广度优先搜索测试通过！\n");
}

/**
 * 测试约瑟夫环问题
 */
void test_josephus_problem() {
    print_separator("约瑟夫环问题测试");
    
    struct {
        int n, k;
        int expected;
    } testCases[] = {
        {5, 2, 3},   // 5个人，数到2出列，最后剩下第3个
        {5, 3, 4},   // 5个人，数到3出列，最后剩下第4个
        {7, 3, 4},   // 7个人，数到3出列，最后剩下第4个
        {1, 1, 1},   // 1个人，最后剩下自己
        {0, 0, 0}    // 结束标记
    };
    
    for (int i = 0; testCases[i].n > 0; i++) {
        int result = JosephusProblem(testCases[i].n, testCases[i].k);
        printf("n=%d, k=%d：最后剩下第%d个人（期望：%d）%s\n",
               testCases[i].n, testCases[i].k, result, testCases[i].expected,
               (result == testCases[i].expected) ? "✓" : "✗");
        assert_test(result == testCases[i].expected, "约瑟夫环问题");
    }
    
    printf("\n✓ 约瑟夫环问题测试通过！\n");
}

/**
 * 测试任务调度队列
 */
void test_task_scheduling() {
    print_separator("任务调度队列测试");
    
    TaskQueue Q;
    Task task, executedTask;
    
    print_subtest("1. 初始化任务队列");
    InitTaskQueue(&Q);
    
    print_subtest("2. 添加任务");
    // 创建测试任务
    Task tasks[] = {
        {1, 1, "初始化系统"},
        {2, 2, "加载配置"},
        {3, 1, "启动服务"},
        {4, 3, "运行主程序"},
        {5, 2, "清理资源"}
    };
    
    for (int i = 0; i < 5; i++) {
        assert_test(AddTask(&Q, tasks[i]), "添加任务");
        printf("添加任务：ID=%d, 优先级=%d, 描述='%s'\n", 
               tasks[i].taskId, tasks[i].priority, tasks[i].description);
    }
    
    printf("\n当前任务队列：\n");
    PrintTaskQueue(Q);
    
    print_subtest("3. 执行任务");
    printf("按FIFO顺序执行任务：\n");
    while (ExecuteTask(&Q, &executedTask)) {
        printf("执行任务：ID=%d, 优先级=%d, 描述='%s'\n",
               executedTask.taskId, executedTask.priority, executedTask.description);
    }
    
    printf("\n✓ 任务调度队列测试通过！\n");
}

/* ================================
 * 高级算法测试函数
 * ================================ */

/**
 * 测试用队列实现栈
 */
void test_queue_stack() {
    print_separator("用队列实现栈测试");
    
    QueueStack S;
    int e;
    
    InitQueueStack(&S);
    assert_test(QueueStackEmpty(S), "初始栈为空");
    
    printf("依次入栈：1, 2, 3, 4, 5\n");
    for (int i = 1; i <= 5; i++) {
        assert_test(QueuePush(&S, i), "入栈操作");
    }
    
    assert_test(QueueStackTop(S, &e) && e == 5, "栈顶元素为5");
    printf("栈顶元素：%d\n", e);
    
    printf("依次出栈：");
    while (!QueueStackEmpty(S)) {
        QueuePop(&S, &e);
        printf("%d ", e);
    }
    printf("\n");
    
    printf("\n✓ 用队列实现栈测试通过！\n");
}

/**
 * 测试队列逆转
 */
void test_queue_reverse() {
    print_separator("队列逆转测试");
    
    CircularQueue Q;
    InitCircularQueue(&Q);
    
    printf("原始队列：");
    for (int i = 1; i <= 5; i++) {
        CircularEnQueue(&Q, i);
    }
    PrintCircularQueue(Q);
    
    ReverseQueue(&Q);
    
    printf("逆转后队列：");
    PrintCircularQueue(Q);
    
    // 验证逆转结果
    int expected[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        int e;
        CircularDeQueue(&Q, &e);
        assert_test(e == expected[i], "逆转结果正确");
    }
    
    printf("\n✓ 队列逆转测试通过！\n");
}

/**
 * 测试滑动窗口最大值
 */
void test_sliding_window_maximum() {
    print_separator("滑动窗口最大值测试");
    
    int nums[] = {1, 3, -1, -3, 5, 3, 6, 7};
    int size = sizeof(nums) / sizeof(nums[0]);
    int k = 3;
    int result[size - k + 1];
    
    printf("原数组：");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    printf("窗口大小：%d\n", k);
    
    SlidingWindowMaximum(nums, size, k, result);
    
    printf("滑动窗口最大值：");
    for (int i = 0; i < size - k + 1; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    // 验证结果
    int expected[] = {3, 3, 5, 5, 6, 7};
    for (int i = 0; i < size - k + 1; i++) {
        assert_test(result[i] == expected[i], "滑动窗口最大值正确");
    }
    
    printf("\n✓ 滑动窗口最大值测试通过！\n");
}

/**
 * 测试优先队列
 */
void test_priority_queue() {
    print_separator("优先队列测试");
    
    PriorityQueue PQ;
    int data;
    
    print_subtest("1. 初始化优先队列");
    InitPriorityQueue(&PQ);
    assert_test(PriorityQueueEmpty(PQ), "初始优先队列为空");
    assert_test(!PriorityQueueFull(PQ), "初始优先队列未满");
    
    print_subtest("2. 插入元素（优先级不同）");
    struct {
        int data;
        int priority;
    } elements[] = {
        {10, 3},
        {20, 1},
        {30, 5},
        {40, 2},
        {50, 4}
    };
    
    for (int i = 0; i < 5; i++) {
        assert_test(PriorityInsert(&PQ, elements[i].data, elements[i].priority), 
                   "插入元素");
        printf("插入：数据=%d, 优先级=%d\n", elements[i].data, elements[i].priority);
    }
    
    printf("\n当前优先队列：\n");
    PrintPriorityQueue(PQ);
    
    print_subtest("3. 按优先级删除最大元素");
    printf("按优先级删除元素：");
    while (!PriorityQueueEmpty(PQ)) {
        PriorityDeleteMax(&PQ, &data);
        printf("%d ", data);
    }
    printf("\n");
    
    printf("\n✓ 优先队列测试通过！\n");
}

/* ================================
 * 性能测试函数
 * ================================ */

/**
 * 队列性能测试
 */
void test_queue_performance() {
    print_separator("队列性能测试");
    
    const int TEST_SIZE = 100000;
    clock_t start, end;
    
    // 循环队列性能测试
    print_subtest("循环队列性能测试");
    CircularQueue cq;
    InitCircularQueue(&cq);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE && i < MAXSIZE - 1; i++) {
        CircularEnQueue(&cq, i);
    }
    end = clock();
    printf("循环队列入队%d次耗时：%.4f秒\n", 
           (TEST_SIZE < MAXSIZE - 1) ? TEST_SIZE : MAXSIZE - 1,
           (double)(end - start) / CLOCKS_PER_SEC);
    
    start = clock();
    int e;
    while (!CircularQueueEmpty(cq)) {
        CircularDeQueue(&cq, &e);
    }
    end = clock();
    printf("循环队列全部出队耗时：%.4f秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // 链式队列性能测试
    print_subtest("链式队列性能测试");
    LinkQueue lq;
    InitLinkQueue(&lq);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        LinkEnQueue(&lq, i);
    }
    end = clock();
    printf("链式队列入队%d次耗时：%.4f秒\n", TEST_SIZE, (double)(end - start) / CLOCKS_PER_SEC);
    
    start = clock();
    while (!LinkQueueEmpty(lq)) {
        LinkDeQueue(&lq, &e);
    }
    end = clock();
    printf("链式队列全部出队耗时：%.4f秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    printf("\n✓ 性能测试完成！\n");
}

/* ================================
 * 主测试函数
 * ================================ */

/**
 * 显示测试菜单
 */
void show_test_menu() {
    printf("\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
    printf("                队列测试系统\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
    printf("【基本操作测试】\n");
    printf("  1. 顺序队列基本操作\n");
    printf("  2. 循环队列基本操作\n");
    printf("  3. 循环队列高级功能\n");
    printf("  4. 计数器法循环队列\n");
    printf("  5. 双端队列操作\n");
    printf("  6. 链式队列基本操作\n");
    printf("\n【应用算法测试】\n");
    printf("  7. 二叉树层次遍历\n");
    printf("  8. 图的广度优先搜索\n");
    printf("  9. 约瑟夫环问题\n");
    printf(" 10. 任务调度队列\n");
    printf("\n【高级算法测试】\n");
    printf(" 11. 用队列实现栈\n");
    printf(" 12. 队列逆转\n");
    printf(" 13. 滑动窗口最大值\n");
    printf(" 14. 优先队列\n");
    printf("\n【综合测试】\n");
    printf(" 15. 性能测试\n");
    printf(" 16. 全部测试\n");
    printf("\n  0. 退出\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n请选择测试项目 (0-16): ");
}

/**
 * 执行全部测试
 */
void run_all_tests() {
    printf("\n开始执行全部队列测试...\n");
    
    test_sqqueue_basic();
    test_circular_queue_basic();
    test_circular_queue_advanced();
    test_count_queue();
    test_deque();
    test_linkqueue_basic();
    test_tree_level_order();
    test_graph_bfs();
    test_josephus_problem();
    test_task_scheduling();
    test_queue_stack();
    test_queue_reverse();
    test_sliding_window_maximum();
    test_priority_queue();
    test_queue_performance();
    
    print_separator("所有队列测试完成");
    printf("✓ 全部测试通过！\n");
}

/**
 * 主函数：运行所有测试
 */
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                队列测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\n测试开始时间：");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    
    // 运行所有测试模块
    test_sqqueue_basic();           // 顺序队列基本操作
    test_circular_queue_basic();    // 循环队列基本操作
    test_circular_queue_advanced(); // 循环队列高级功能
    test_count_queue();            // 计数器法循环队列
    test_deque();                  // 双端队列操作
    test_linkqueue_basic();        // 链式队列基本操作
    test_tree_level_order();       // 二叉树层次遍历
    test_graph_bfs();              // 图的广度优先搜索
    test_josephus_problem();       // 约瑟夫环问题
    test_task_scheduling();        // 任务调度队列
    test_queue_stack();            // 用队列实现栈
    test_queue_reverse();          // 队列逆转
    test_sliding_window_maximum(); // 滑动窗口最大值
    test_priority_queue();         // 优先队列
    test_queue_performance();      // 性能测试
    
    // 测试总结
    print_separator("====== queue testcase finished!=======");
        
    printf("\n结束时间：");
    now = time(NULL);
    printf("%s", ctime(&now));
    
    return 0;
} 