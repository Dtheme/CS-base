/*
 * test_stack.c - 栈数据结构全面测试程序
 * create by: zw.duan
 *  数据结构专用测试
 */

#include "stack.h"
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
 * 测试顺序栈基本操作
 */
void test_sqstack_basic() {
    print_separator("顺序栈基本操作测试");
    
    SqStack S;
    int e;
    
    print_subtest("1. 初始化和状态检查");
    InitSqStack(&S);
    assert_test(SqStackEmpty(S), "初始栈为空");
    assert_test(!SqStackFull(S), "初始栈未满");
    assert_test(SqStackLength(S) == 0, "初始栈长度为0");
    
    print_subtest("2. 入栈操作测试");
    printf("依次入栈：10, 20, 30, 40, 50\n");
    assert_test(SqPush(&S, 10), "入栈10");
    assert_test(SqPush(&S, 20), "入栈20");
    assert_test(SqPush(&S, 30), "入栈30");
    assert_test(SqPush(&S, 40), "入栈40");
    assert_test(SqPush(&S, 50), "入栈50");
    
    printf("当前栈状态：");
    PrintSqStack(S);
    assert_test(SqStackLength(S) == 5, "栈长度为5");
    assert_test(!SqStackEmpty(S), "栈非空");
    
    print_subtest("3. 获取栈顶元素");
    assert_test(SqGetTop(S, &e) && e == 50, "栈顶元素为50");
    printf("栈顶元素：%d\n", e);
    
    print_subtest("4. 出栈操作测试");
    assert_test(SqPop(&S, &e) && e == 50, "出栈50");
    assert_test(SqPop(&S, &e) && e == 40, "出栈40");
    printf("连续出栈两次，当前栈状态：");
    PrintSqStack(S);
    assert_test(SqStackLength(S) == 3, "栈长度为3");
    
    print_subtest("5. 边界测试");
    // 清空栈
    while (!SqStackEmpty(S)) {
        SqPop(&S, &e);
    }
    assert_test(SqStackEmpty(S), "栈已清空");
    assert_test(!SqGetTop(S, &e), "空栈获取栈顶失败");
    assert_test(!SqPop(&S, &e), "空栈出栈失败");
    
    // 填满栈
    printf("测试栈满情况...\n");
    for (int i = 0; i < MAXSIZE; i++) {
        SqPush(&S, i);
    }
    assert_test(SqStackFull(S), "栈已满");
    assert_test(!SqPush(&S, 999), "满栈入栈失败");
    
    printf("\n✓ 顺序栈基本操作测试通过！\n");
}

/**
 * 测试链栈基本操作
 */
void test_linkstack_basic() {
    print_separator("链栈基本操作测试");
    
    LinkStack S;
    int e;
    
    print_subtest("1. 初始化和状态检查");
    InitLinkStack(&S);
    assert_test(LinkStackEmpty(S), "初始栈为空");
    assert_test(LinkStackLength(S) == 0, "初始栈长度为0");
    
    print_subtest("2. 入栈操作测试");
    printf("依次入栈：100, 200, 300\n");
    assert_test(LinkPush(&S, 100), "入栈100");
    assert_test(LinkPush(&S, 200), "入栈200");
    assert_test(LinkPush(&S, 300), "入栈300");
    
    printf("当前栈状态：");
    PrintLinkStack(S);
    assert_test(LinkStackLength(S) == 3, "栈长度为3");
    
    print_subtest("3. 获取栈顶和出栈");
    assert_test(LinkGetTop(S, &e) && e == 300, "栈顶元素为300");
    assert_test(LinkPop(&S, &e) && e == 300, "出栈300");
    assert_test(LinkPop(&S, &e) && e == 200, "出栈200");
    
    printf("出栈后状态：");
    PrintLinkStack(S);
    
    print_subtest("4. 销毁链栈");
    DestroyLinkStack(&S);
    assert_test(S == NULL, "链栈已销毁");
    
    printf("\n✓ 链栈基本操作测试通过！\n");
}

/**
 * 测试共享栈
 */
void test_shared_stack() {
    print_separator("共享栈测试");
    
    SharedStack S;
    int e;
    
    print_subtest("1. 初始化共享栈");
    InitSharedStack(&S);
    assert_test(SharedStackEmpty(S, 0), "栈0为空");
    assert_test(SharedStackEmpty(S, 1), "栈1为空");
    assert_test(!SharedStackFull(S), "共享栈未满");
    
    print_subtest("2. 向两个栈分别入栈");
    printf("栈0入栈：1, 3, 5\n");
    assert_test(SharedPush(&S, 0, 1), "栈0入栈1");
    assert_test(SharedPush(&S, 0, 3), "栈0入栈3");
    assert_test(SharedPush(&S, 0, 5), "栈0入栈5");
    
    printf("栈1入栈：2, 4, 6\n");
    assert_test(SharedPush(&S, 1, 2), "栈1入栈2");
    assert_test(SharedPush(&S, 1, 4), "栈1入栈4");
    assert_test(SharedPush(&S, 1, 6), "栈1入栈6");
    
    printf("共享栈状态：");
    PrintSharedStack(S);
    
    print_subtest("3. 获取栈顶和出栈");
    assert_test(SharedGetTop(S, 0, &e) && e == 5, "栈0栈顶为5");
    assert_test(SharedGetTop(S, 1, &e) && e == 6, "栈1栈顶为6");
    
    assert_test(SharedPop(&S, 0, &e) && e == 5, "栈0出栈5");
    assert_test(SharedPop(&S, 1, &e) && e == 6, "栈1出栈6");
    
    printf("出栈后状态：");
    PrintSharedStack(S);
    
    printf("\n✓ 共享栈测试通过！\n");
}

/* ================================
 * 应用算法测试函数
 * ================================ */

/**
 * 测试括号匹配
 */
void test_bracket_matching() {
    print_separator("括号匹配算法测试");
    
    char *testCases[] = {
        "()",
        "()[]{}",
        "([{}])",
        "((()))",
        "({[]})",
        "(",
        ")",
        "([)]",
        "(((",
        ")]",
        "{[}]",
        NULL
    };
    
    bool expected[] = {true, true, true, true, true, false, false, false, false, false, false};
    
    for (int i = 0; testCases[i] != NULL; i++) {
        bool result = BracketCheck(testCases[i]);
        printf("测试字符串：%-10s 结果：%-5s 期望：%-5s %s\n", 
               testCases[i], 
               result ? "匹配" : "不匹配",
               expected[i] ? "匹配" : "不匹配",
               (result == expected[i]) ? "✓" : "✗");
        assert_test(result == expected[i], "括号匹配测试");
    }
    
    printf("\n✓ 括号匹配测试通过！\n");
}

/**
 * 测试表达式求值
 */
void test_expression_evaluation() {
    print_separator("表达式求值测试");
    
    print_subtest("1. 中缀转后缀测试");
    char postfix[100];
    
    struct {
        char *infix;
        char *expected;
    } testCases[] = {
        {"2+3*4", "234*+"},
        {"(2+3)*4", "23+4*"},
        {"2+3*4-5", "234*+5-"},
        {"2*(3+4)", "234+*"},
        {"(2+3)*(4-1)", "23+41-*"},
        {NULL, NULL}
    };
    
    for (int i = 0; testCases[i].infix != NULL; i++) {
        bool success = InfixToPostfix(testCases[i].infix, postfix);
        printf("中缀表达式：%-12s 后缀表达式：%-10s 期望：%-10s %s\n",
               testCases[i].infix, postfix, testCases[i].expected,
               (success && strcmp(postfix, testCases[i].expected) == 0) ? "✓" : "✗");
        assert_test(success && strcmp(postfix, testCases[i].expected) == 0, "中缀转后缀");
    }
    
    print_subtest("2. 后缀表达式求值测试");
    struct {
        char *postfix;
        int expected;
    } evalCases[] = {
        {"234*+", 14},
        {"23+4*", 20},
        {"234*+5-", 9},
        {"234+*", 14},
        {"23+41-*", 15},
        {NULL, 0}
    };
    
    for (int i = 0; evalCases[i].postfix != NULL; i++) {
        int result = EvaluatePostfix(evalCases[i].postfix);
        printf("后缀表达式：%-10s 结果：%-3d 期望：%-3d %s\n",
               evalCases[i].postfix, result, evalCases[i].expected,
               (result == evalCases[i].expected) ? "✓" : "✗");
        assert_test(result == evalCases[i].expected, "后缀表达式求值");
    }
    
    print_subtest("3. 中缀表达式直接求值测试");
    struct {
        char *infix;
        int expected;
    } directCases[] = {
        {"2+3*4", 14},
        {"(2+3)*4", 20},
        {"2+3*4-5", 9},
        {"2*(3+4)", 14},
        {"(2+3)*(4-1)", 15},
        {NULL, 0}
    };
    
    for (int i = 0; directCases[i].infix != NULL; i++) {
        int result = EvaluateInfix(directCases[i].infix);
        printf("中缀表达式：%-12s 结果：%-3d 期望：%-3d %s\n",
               directCases[i].infix, result, directCases[i].expected,
               (result == directCases[i].expected) ? "✓" : "✗");
        assert_test(result == directCases[i].expected, "中缀表达式求值");
    }
    
    printf("\n✓ 表达式求值测试通过！\n");
}

/**
 * 测试进制转换
 */
void test_base_conversion() {
    print_separator("进制转换测试");
    
    struct {
        int decimal;
        int base;
        char *expected;
    } testCases[] = {
        {10, 2, "1010"},
        {255, 16, "FF"},
        {100, 8, "144"},
        {1000, 16, "3E8"},
        {15, 2, "1111"},
        {0, 0, NULL}
    };
    
    char result[100];
    for (int i = 0; testCases[i].expected != NULL; i++) {
        DecimalToBase(testCases[i].decimal, testCases[i].base, result);
        printf("十进制：%-4d -> %d进制：%-10s 期望：%-10s %s\n",
               testCases[i].decimal, testCases[i].base, result, testCases[i].expected,
               (strcmp(result, testCases[i].expected) == 0) ? "✓" : "✗");
        assert_test(strcmp(result, testCases[i].expected) == 0, "进制转换");
    }
    
    printf("\n✓ 进制转换测试通过！\n");
}

/**
 * 测试非递归阶乘
 */
void test_factorial_non_recursive() {
    print_separator("非递归阶乘测试");
    
    struct {
        int n;
        long long expected;
    } testCases[] = {
        {0, 1},
        {1, 1},
        {5, 120},
        {10, 3628800},
        {-1, -1}  // 错误情况
    };
    
    for (int i = 0; i < sizeof(testCases)/sizeof(testCases[0]); i++) {
        long long result = FactorialNonRecursive(testCases[i].n);
        printf("阶乘：%d! = %-10lld 期望：%-10lld %s\n",
               testCases[i].n, result, testCases[i].expected,
               (result == testCases[i].expected) ? "✓" : "✗");
        assert_test(result == testCases[i].expected, "非递归阶乘");
    }
    
    printf("\n✓ 非递归阶乘测试通过！\n");
}

/* ================================
 * 高级算法测试函数
 * ================================ */

/**
 * 测试用栈实现队列
 */
void test_stack_queue() {
    print_separator("用栈实现队列测试");
    
    StackQueue Q;
    int e;
    
    InitStackQueue(&Q);
    assert_test(StackQueueEmpty(Q), "初始队列为空");
    
    printf("依次入队：1, 2, 3, 4, 5\n");
    for (int i = 1; i <= 5; i++) {
        assert_test(StackEnQueue(&Q, i), "入队操作");
    }
    
    assert_test(StackGetHead(Q, &e) && e == 1, "队头元素为1");
    printf("队头元素：%d\n", e);
    
    printf("依次出队：");
    while (!StackQueueEmpty(Q)) {
        StackDeQueue(&Q, &e);
        printf("%d ", e);
    }
    printf("\n");
    
    printf("\n✓ 用栈实现队列测试通过！\n");
}

/**
 * 测试最小栈
 */
void test_min_stack() {
    print_separator("最小栈测试");
    
    MinStack S;
    int e;
    
    InitMinStack(&S);
    assert_test(MinStackEmpty(S), "初始最小栈为空");
    
    printf("依次入栈：3, 1, 4, 1, 5\n");
    int data[] = {3, 1, 4, 1, 5};
    for (int i = 0; i < 5; i++) {
        assert_test(MinPush(&S, data[i]), "入栈操作");
        printf("入栈%d后，最小值：%d\n", data[i], GetMin(S));
    }
    
    printf("\n出栈过程中的最小值变化：\n");
    while (!MinStackEmpty(S)) {
        printf("最小值：%d，", GetMin(S));
        MinPop(&S, &e);
        printf("出栈：%d\n", e);
    }
    
    printf("\n✓ 最小栈测试通过！\n");
}

/**
 * 测试下一个更大元素
 */
void test_next_greater_element() {
    print_separator("下一个更大元素测试");
    
    int nums[] = {2, 1, 2, 4, 3, 1};
    int size = sizeof(nums) / sizeof(nums[0]);
    int result[size];
    
    NextGreaterElement(nums, size, result);
    
    printf("原数组：");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");
    
    printf("结果：  ");
    for (int i = 0; i < size; i++) {
        if (result[i] == -1) {
            printf("- ");
        } else {
            printf("%d ", result[i]);
        }
    }
    printf("\n");
    
    printf("\n✓ 下一个更大元素测试通过！\n");
}

/**
 * 测试栈排序
 */
void test_stack_sort() {
    print_separator("栈排序测试");
    
    SqStack S;
    InitSqStack(&S);
    
    int data[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int size = sizeof(data) / sizeof(data[0]);
    
    printf("原始数据入栈：");
    for (int i = 0; i < size; i++) {
        SqPush(&S, data[i]);
        printf("%d ", data[i]);
    }
    printf("\n");
    
    printf("排序前栈状态：");
    PrintSqStack(S);
    
    StackSort(&S);
    
    printf("排序后栈状态：");
    PrintSqStack(S);
    
    printf("\n✓ 栈排序测试通过！\n");
}

/* ================================
 * 性能测试函数
 * ================================ */

/**
 * 栈性能测试
 */
void test_stack_performance() {
    print_separator("栈性能测试");
    
    const int TEST_SIZE = 100000;
    clock_t start, end;
    
    // 顺序栈性能测试
    print_subtest("顺序栈性能测试");
    SqStack sq_stack;
    InitSqStack(&sq_stack);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE && i < MAXSIZE; i++) {
        SqPush(&sq_stack, i);
    }
    end = clock();
    printf("顺序栈入栈%d次耗时：%.4f秒\n", 
           (TEST_SIZE < MAXSIZE) ? TEST_SIZE : MAXSIZE,
           (double)(end - start) / CLOCKS_PER_SEC);
    
    start = clock();
    int e;
    while (!SqStackEmpty(sq_stack)) {
        SqPop(&sq_stack, &e);
    }
    end = clock();
    printf("顺序栈全部出栈耗时：%.4f秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // 链栈性能测试
    print_subtest("链栈性能测试");
    LinkStack link_stack;
    InitLinkStack(&link_stack);
    
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        LinkPush(&link_stack, i);
    }
    end = clock();
    printf("链栈入栈%d次耗时：%.4f秒\n", TEST_SIZE, (double)(end - start) / CLOCKS_PER_SEC);
    
    start = clock();
    while (!LinkStackEmpty(link_stack)) {
        LinkPop(&link_stack, &e);
    }
    end = clock();
    printf("链栈全部出栈耗时：%.4f秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
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
    printf("                栈测试系统\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n");
    printf("【基本操作测试】\n");
    printf("  1. 顺序栈基本操作\n");
    printf("  2. 链栈基本操作\n");
    printf("  3. 共享栈操作\n");
    printf("\n【应用算法测试】\n");
    printf("  4. 括号匹配\n");
    printf("  5. 表达式求值\n");
    printf("  6. 进制转换\n");
    printf("  7. 非递归阶乘\n");
    printf("\n【高级算法测试】\n");
    printf("  8. 用栈实现队列\n");
    printf("  9. 最小栈\n");
    printf(" 10. 下一个更大元素\n");
    printf(" 11. 栈排序\n");
    printf("\n【综合测试】\n");
    printf(" 12. 性能测试\n");
    printf(" 13. 全部测试\n");
    printf("\n  0. 退出\n");
    for (int i = 0; i < 60; i++) printf("=");
    printf("\n请选择测试项目 (0-13): ");
}

/**
 * 执行全部测试
 */
void run_all_tests() {
    printf("\n开始执行全部栈测试...\n");
    
    test_sqstack_basic();
    test_linkstack_basic();
    test_shared_stack();
    test_bracket_matching();
    test_expression_evaluation();
    test_base_conversion();
    test_factorial_non_recursive();
    test_stack_queue();
    test_min_stack();
    test_next_greater_element();
    test_stack_sort();
    test_stack_performance();
    
    print_separator("所有栈测试完成");
    printf("✓ 全部测试通过！\n");
}

/**
 * 主函数
 */
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                栈测试用例                    ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // 自动运行所有测试，不需要用户交互
    run_all_tests();
    
    return 0;
} 