/* test_circular.c - 循环链表测试程序
 * create by: zw.duan
 */

#include "linklist.h"
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
        assert(condition);
    }
}

/* ================================
 * 循环单链表测试
 * ================================ */

/**
 * 测试循环单链表基本操作
 */
void test_circular_list_basic() {
    print_separator("循环单链表基本操作测试");
    
    CircularList L;
    ElemType e;
    
    print_subtest("1. 初始化测试");
    L = InitCircularList();
    assert_test(L != NULL, "初始化成功");
    assert_test(CircularListEmpty(L), "初始状态为空");
    assert_test(CircularListLength(L) == 0, "初始长度为0");
    assert_test(L->next == L, "头节点指向自己形成循环");
    
    print_subtest("2. 插入操作测试");
    printf("依次插入元素：10, 20, 30, 15（在位置2插入15）\n");
    
    assert_test(CircularListInsert(L, 1, 10), "位置1插入10");
    PrintCircularList(L);
    
    assert_test(CircularListInsert(L, 2, 20), "位置2插入20");
    PrintCircularList(L);
    
    assert_test(CircularListInsert(L, 3, 30), "位置3插入30");
    PrintCircularList(L);
    
    assert_test(CircularListInsert(L, 2, 15), "位置2插入15");
    PrintCircularList(L);
    
    assert_test(CircularListLength(L) == 4, "插入后长度为4");
    assert_test(!CircularListEmpty(L), "插入后非空");
    
    print_subtest("3. 查找操作测试");
    bool get_result = CircularGetElem(L, 3, &e);
    assert_test(get_result && e == 20, "第3个位置的元素为20");
    printf("第3个位置的元素：%d\n", e);
    
    print_subtest("4. 删除操作测试");
    printf("删除第2个位置的元素\n");
    
    bool delete_result = CircularListDelete(L, 2, &e);
    assert_test(delete_result && e == 15, "删除第2个位置的元素15");
    printf("删除的元素：%d\n", e);
    PrintCircularList(L);
    assert_test(CircularListLength(L) == 3, "删除后长度为3");
    
    print_subtest("5. 遍历操作测试");
    printf("使用遍历函数打印所有元素：");
    assert_test(CircularListTraverse(L, Visit_Print), "遍历操作成功");
    printf("\n");
    
    print_subtest("6. 循环特性验证");
    // 验证循环特性：从头节点出发，能够回到头节点
    LNode *p = L->next;
    int count = 0;
    if (p != L) {
        do {
            count++;
            p = p->next;
            if (count > 10) break; // 防止无限循环
        } while (p != L);
    }
    assert_test(count == 3 && p == L, "循环特性正确");
    printf("循环验证：遍历%d个节点后回到头节点\n", count);
    
    print_subtest("7. 边界条件测试");
    assert_test(!CircularListInsert(L, 0, 5), "位置0插入应失败");
    assert_test(!CircularListInsert(L, 10, 5), "位置超界插入应失败");
    assert_test(!CircularListDelete(L, 0, &e), "位置0删除应失败");
    assert_test(!CircularListDelete(L, 10, &e), "位置超界删除应失败");
    
    // 清空表测试
    ClearCircularList(L);
    assert_test(CircularListLength(L) == 0, "清空后长度为0");
    assert_test(CircularListEmpty(L), "清空后为空表");
    assert_test(L->next == L, "清空后仍保持循环结构");
    
    DestroyCircularList(L);
    printf("\n✓ 循环单链表基本操作测试全部通过！\n");
}

/**
 * 测试循环单链表的逆转
 */
void test_circular_list_reverse() {
    print_subtest("循环单链表逆转测试");
    
    CircularList L = InitCircularList();
    
    // 构造测试数据：1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++) {
        CircularListInsert(L, i, i);
    }
    
    printf("逆转前：");
    PrintCircularList(L);
    
    assert_test(CircularListReverse(L), "逆转操作成功");
    printf("逆转后：");
    PrintCircularList(L);
    
    // 验证逆转结果
    ElemType e;
    ElemType expected[] = {5, 4, 3, 2, 1};
    for (int i = 1; i <= 5; i++) {
        CircularGetElem(L, i, &e);
        assert_test(e == expected[i-1], "逆转结果正确");
    }
    
    // 验证仍然是循环结构
    LNode *p = L->next;
    int count = 0;
    if (p != L) {
        do {
            count++;
            p = p->next;
        } while (p != L && count <= 10);
    }
    assert_test(count == 5 && p == L, "逆转后循环特性保持");
    
    DestroyCircularList(L);
}

/* ================================
 * 循环双链表测试
 * ================================ */

/**
 * 测试循环双链表基本操作
 */
void test_circular_dlist_basic() {
    print_separator("循环双链表基本操作测试");
    
    CircularDList L;
    ElemType e;
    
    print_subtest("1. 初始化测试");
    L = InitCircularDList();
    assert_test(L != NULL, "初始化成功");
    assert_test(L->next == L && L->prior == L, "头节点前驱后继都指向自己");
    
    print_subtest("2. 插入操作测试");
    printf("依次插入元素：100, 200, 300\n");
    
    assert_test(CircularDListInsert(L, 1, 100), "位置1插入100");
    PrintCircularDList(L);
    
    assert_test(CircularDListInsert(L, 2, 200), "位置2插入200");
    PrintCircularDList(L);
    
    assert_test(CircularDListInsert(L, 3, 300), "位置3插入300");
    PrintCircularDList(L);
    
    print_subtest("3. 双向循环特性验证");
    // 正向遍历
    printf("正向遍历：");
    DNode *p = L->next;
    int count = 0;
    if (p != L) {
        do {
            printf("%d ", p->data);
            p = p->next;
            count++;
        } while (p != L && count <= 10);
    }
    printf("\n");
    
    // 反向遍历
    printf("反向遍历：");
    p = L->prior;
    count = 0;
    if (p != L) {
        do {
            printf("%d ", p->data);
            p = p->prior;
            count++;
        } while (p != L && count <= 10);
    }
    printf("\n");
    
    assert_test(count == 3, "双向遍历正确");
    
    print_subtest("4. 删除操作测试");
    printf("删除第2个位置的元素\n");
    
    bool delete_result = CircularDListDelete(L, 2, &e);
    assert_test(delete_result && e == 200, "删除第2个位置的元素200");
    printf("删除的元素：%d\n", e);
    PrintCircularDList(L);
    
    // 验证删除后的双向连接
    p = L->next;
    assert_test(p->prior == L, "第一个节点的前驱指向头节点");
    assert_test(p->next->prior == p, "双向连接正确");
    
    DestroyCircularDList(L);
    printf("\n✓ 循环双链表基本操作测试全部通过！\n");
}

/* ================================
 * 循环链表高级操作测试
 * ================================ */

/**
 * 测试有序循环链表合并
 */
void test_merge_circular_lists() {
    print_separator("有序循环链表合并测试");
    
    CircularList La = InitCircularList();
    CircularList Lb = InitCircularList();
    CircularList Lc = NULL;
    
    // 构造两个有序循环链表
    int dataA[] = {1, 3, 5, 7};
    int dataB[] = {2, 4, 6, 8, 9};
    
    printf("构造两个有序循环链表：\n");
    for (int i = 0; i < 4; i++) {
        CircularListInsert(La, i + 1, dataA[i]);
    }
    printf("表A：");
    PrintCircularList(La);
    
    for (int i = 0; i < 5; i++) {
        CircularListInsert(Lb, i + 1, dataB[i]);
    }
    printf("表B：");
    PrintCircularList(Lb);
    
    // 测试合并
    assert_test(MergeCircularList(La, Lb, &Lc), "合并操作成功");
    printf("合并结果：");
    PrintCircularList(Lc);
    
    assert_test(CircularListLength(Lc) == 9, "合并后长度为9");
    
    // 验证合并结果是有序的
    ElemType prev, curr;
    CircularGetElem(Lc, 1, &prev);
    for (int i = 2; i <= CircularListLength(Lc); i++) {
        CircularGetElem(Lc, i, &curr);
        assert_test(prev <= curr, "合并结果有序");
        prev = curr;
    }
    
    // 验证合并后仍是循环结构
    LNode *p = Lc->next;
    int count = 0;
    if (p != Lc) {
        do {
            count++;
            p = p->next;
        } while (p != Lc && count <= 15);
    }
    assert_test(count == 9 && p == Lc, "合并后循环特性保持");
    
    DestroyCircularList(La);
    DestroyCircularList(Lb);
    DestroyCircularList(Lc);
    
    printf("\n✓ 循环链表合并测试全部通过！\n");
}

/* ================================
 * 性能和鲁棒性测试
 * ================================ */

/**
 * 测试循环链表的错误处理
 */
void test_circular_error_handling() {
    print_separator("循环链表错误处理测试");
    
    ElemType e;
    
    print_subtest("1. 空指针处理测试");
    assert_test(InitCircularList() != NULL, "正常初始化");
    assert_test(!CircularListInsert(NULL, 1, 10), "空指针插入应失败");
    assert_test(!CircularListDelete(NULL, 1, &e), "空指针删除应失败");
    assert_test(!CircularGetElem(NULL, 1, &e), "空指针查找应失败");
    
    print_subtest("2. 空表操作测试");
    CircularList L = InitCircularList();
    assert_test(!CircularListDelete(L, 1, &e), "空表删除应失败");
    assert_test(!CircularGetElem(L, 1, &e), "空表查找应失败");
    
    print_subtest("3. 越界操作测试");
    CircularListInsert(L, 1, 10);
    CircularListInsert(L, 2, 20);
    
    assert_test(!CircularGetElem(L, 0, &e), "位序0查找应失败");
    assert_test(!CircularGetElem(L, 10, &e), "位序超界查找应失败");
    assert_test(!CircularListDelete(L, 0, &e), "位序0删除应失败");
    assert_test(!CircularListDelete(L, 10, &e), "位序超界删除应失败");
    assert_test(!CircularListInsert(L, 0, 5), "位序0插入应失败");
    
    DestroyCircularList(L);
    
    print_subtest("4. 循环双链表错误处理");
    CircularDList DL = InitCircularDList();
    assert_test(!CircularDListDelete(DL, 1, &e), "空双表删除应失败");
    assert_test(!CircularDListInsert(NULL, 1, 10), "空指针双表插入应失败");
    
    DestroyCircularDList(DL);
    
    printf("\n✓ 错误处理测试全部通过！\n");
}

/**
 * 性能对比测试：循环链表 vs 普通链表
 */
void test_performance_comparison() {
    print_separator("性能对比测试：循环链表 vs 普通链表");
    
    const int TEST_SIZE = 1000;
    clock_t start, end;
    
    printf("测试规模：%d个元素\n", TEST_SIZE);
    
    // 循环链表性能测试
    printf("\n循环链表性能测试：\n");
    CircularList CL = InitCircularList();
    
    start = clock();
    for (int i = 1; i <= TEST_SIZE; i++) {
        CircularListInsert(CL, i, i);
    }
    end = clock();
    double circular_insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("插入%d个元素用时：%.6f秒\n", TEST_SIZE, circular_insert_time);
    
    start = clock();
    ElemType e;
    for (int i = 1; i <= TEST_SIZE; i++) {
        CircularGetElem(CL, i, &e);
    }
    end = clock();
    double circular_search_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("查找%d次用时：%.6f秒\n", TEST_SIZE, circular_search_time);
    
    DestroyCircularList(CL);
    
    // 普通链表性能测试
    printf("\n普通链表性能测试：\n");
    LinkList L = InitList();
    
    start = clock();
    for (int i = 1; i <= TEST_SIZE; i++) {
        ListInsert(L, i, i);
    }
    end = clock();
    double normal_insert_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("插入%d个元素用时：%.6f秒\n", TEST_SIZE, normal_insert_time);
    
    start = clock();
    for (int i = 1; i <= TEST_SIZE; i++) {
        GetElem(L, i, &e);
    }
    end = clock();
    double normal_search_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("查找%d次用时：%.6f秒\n", TEST_SIZE, normal_search_time);
    
    DestroyList(L);
    
    printf("\n性能比较结果：\n");
    printf("插入性能比（循环/普通）：%.2f\n", circular_insert_time / normal_insert_time);
    printf("查找性能比（循环/普通）：%.2f\n", circular_search_time / normal_search_time);
    
    printf("\n✓ 性能测试完成！\n");
}

/* ================================
 *  重点算法演示
 * ================================ */

/**
 * 演示约瑟夫环问题（循环链表经典应用）
 */
void demo_josephus_problem() {
    print_separator("约瑟夫环问题演示（ 经典）");
    
    printf("问题描述：n个人围成一圈，从第1个人开始数数，\n");
    printf("数到m的人出列，然后从下一个人开始继续数数，\n");
    printf("直到所有人都出列。求出列顺序。\n\n");
    
    int n = 7;  // 7个人
    int m = 3;  // 数到3出列
    
    printf("参数：n=%d, m=%d\n", n, m);
    
    // 构造约瑟夫环
    CircularList josephus = InitCircularList();
    for (int i = 1; i <= n; i++) {
        CircularListInsert(josephus, i, i);
    }
    
    printf("初始环：");
    PrintCircularList(josephus);
    
    printf("\n出列顺序：");
    
    LNode *current = josephus->next;  // 从第一个人开始
    int remaining = n;
    
    while (remaining > 1) {
        // 数m-1个人
        for (int i = 1; i < m; i++) {
            current = current->next;
            if (current == josephus) {  // 跳过头节点
                current = current->next;
            }
        }
        
        // 记录要出列的人
        printf("%d ", current->data);
        
        // 删除当前人
        LNode *to_delete = current;
        LNode *prev = josephus;
        
        // 找到current的前驱
        while (prev->next != current) {
            prev = prev->next;
        }
        
        // 删除current
        prev->next = current->next;
        current = current->next;
        if (current == josephus) {  // 跳过头节点
            current = current->next;
        }
        
        free(to_delete);
        remaining--;
    }
    
    // 输出最后剩下的人
    if (remaining == 1) {
        LNode *last = josephus->next;
        printf("\n最后剩下：%d\n", last->data);
    }
    
    DestroyCircularList(josephus);
    printf("\n✓ 约瑟夫环演示完成！\n");
}

/* ================================
 * 主测试函数
 * ================================ */

/**
 * 主函数：运行所有循环链表测试
 */
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                循环链表测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\n测试开始时间：");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    
    // 运行所有测试模块
    test_circular_list_basic();        // 循环单链表基本操作
    test_circular_list_reverse();      // 循环链表逆转
    test_circular_dlist_basic();       // 循环双链表基本操作
    test_merge_circular_lists();       // 循环链表合并
    test_circular_error_handling();    // 错误处理测试
    test_performance_comparison();     // 性能对比测试
    demo_josephus_problem();          // 约瑟夫环演示
    
    // 测试总结
    print_separator("====== circular list testcase finished! ======");
    
    printf("\n结束时间：");
    now = time(NULL);
    printf("%s", ctime(&now));
    
    return 0;
} 