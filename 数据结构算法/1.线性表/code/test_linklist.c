/* test_linklist.c - 链表测试程序
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
        assert(condition);  // 在调试模式下终止程序
    }
}

/* ================================
 * 链表基本操作测试
 * ================================ */

/**
 * 测试链表的基本操作
 */
void test_basic_operations() {
    print_separator("单链表基本操作测试");
    
    LinkList L;
    ElemType e;
    
    print_subtest("1. 初始化和基本状态测试");
    L = InitList();
    assert_test(L != NULL, "链表初始化");
    assert_test(ListEmpty(L), "初始状态为空");
    assert_test(ListLength(L) == 0, "初始长度为0");
    
    print_subtest("2. 插入操作测试");
    printf("依次插入元素：10, 20, 30, 15（在位置2插入15）\n");
    
    assert_test(ListInsert(L, 1, 10), "位置1插入10");
    PrintList(L);
    
    assert_test(ListInsert(L, 2, 20), "位置2插入20");
    PrintList(L);
    
    assert_test(ListInsert(L, 3, 30), "位置3插入30");
    PrintList(L);
    
    assert_test(ListInsert(L, 2, 15), "位置2插入15");
    PrintList(L);
    
    assert_test(ListLength(L) == 4, "插入后长度为4");
    assert_test(!ListEmpty(L), "插入后非空");
    
    print_subtest("3. 查找操作测试");
    
    // 按位查找测试
    bool get_result = GetElem(L, 3, &e);
    assert_test(get_result && e == 20, "第3个位置的元素为20");
    printf("第3个位置的元素：%d\n", e);
    
    // 按值查找测试
    int pos = LocateElemPos(L, 30);
    assert_test(pos == 4, "元素30的位置为4");
    printf("元素30的位置：%d\n", pos);
    
    pos = LocateElemPos(L, 99);
    assert_test(pos == 0, "元素99不存在");
    printf("元素99的位置：%d（0表示不存在）\n", pos);
    
    // 指针查找测试
    LNode *ptr = LocateElem(L, 15);
    assert_test(ptr != NULL && ptr->data == 15, "指针查找元素15");
    
    print_subtest("4. 删除操作测试");
    printf("删除第2个位置的元素\n");
    
    bool delete_result = ListDelete(L, 2, &e);
    assert_test(delete_result && e == 15, "删除第2个位置的元素15");
    printf("删除的元素：%d\n", e);
    PrintList(L);
    assert_test(ListLength(L) == 3, "删除后长度为3");
    
    print_subtest("5. 遍历操作测试");
    printf("使用遍历函数打印所有元素：");
    assert_test(ListTraverse(L, Visit_Print), "遍历操作成功");
    printf("\n");
    
    print_subtest("6. 边界条件测试");
    printf("测试非法操作：\n");
    assert_test(!ListInsert(L, 0, 5), "位置0插入应失败");
    assert_test(!ListInsert(L, 10, 5), "位置超界插入应失败");
    assert_test(!ListDelete(L, 0, &e), "位置0删除应失败");
    assert_test(!ListDelete(L, 10, &e), "位置超界删除应失败");
    
    // 清空表测试
    ClearList(L);
    assert_test(ListLength(L) == 0, "清空后长度为0");
    assert_test(ListEmpty(L), "清空后为空表");
    
    // 销毁链表
    DestroyList(L);
    
    printf("\n✓ 基本操作测试全部通过！\n");
}

/* ================================
 * 建表算法测试
 * ================================ */

/**
 * 测试头插法建表
 */
void test_create_list_head() {
    print_subtest("头插法建表测试");
    
    LinkList L;
    printf("模拟头插法建表：输入序列 1 2 3 4 5\n");
    
    // 模拟头插法（手动插入避免输入）
    L = InitList();
    for (int i = 1; i <= 5; i++) {
        ListInsert(L, 1, i);  // 总是在第1个位置插入
    }
    
    printf("头插法建表结果：");
    PrintList(L);
    printf("说明：元素顺序与输入顺序相反\n");
    
    // 验证结果：应该是 5 4 3 2 1
    ElemType expected[] = {5, 4, 3, 2, 1};
    for (int i = 1; i <= 5; i++) {
        ElemType e;
        GetElem(L, i, &e);
        assert_test(e == expected[i-1], "头插法结果正确");
    }
    
    DestroyList(L);
}

/**
 * 测试尾插法建表
 */
void test_create_list_tail() {
    print_subtest("尾插法建表测试");
    
    LinkList L;
    printf("模拟尾插法建表：输入序列 1 2 3 4 5\n");
    
    // 模拟尾插法（手动插入避免输入）
    L = InitList();
    for (int i = 1; i <= 5; i++) {
        ListInsert(L, ListLength(L) + 1, i);  // 总是在尾部插入
    }
    
    printf("尾插法建表结果：");
    PrintList(L);
    printf("说明：元素顺序与输入顺序相同\n");
    
    // 验证结果：应该是 1 2 3 4 5
    for (int i = 1; i <= 5; i++) {
        ElemType e;
        GetElem(L, i, &e);
        assert_test(e == i, "尾插法结果正确");
    }
    
    DestroyList(L);
}

/**
 * 测试从数组建表
 */
void test_create_from_array() {
    print_subtest("从数组建表测试");
    
    ElemType arr[] = {10, 20, 30, 40, 50};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    LinkList L;
    bool result = CreateListFromArray(&L, arr, n);
    assert_test(result, "从数组建表成功");
    
    printf("数组：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    printf("建表结果：");
    PrintList(L);
    
    // 验证结果
    for (int i = 1; i <= n; i++) {
        ElemType e;
        GetElem(L, i, &e);
        assert_test(e == arr[i-1], "从数组建表结果正确");
    }
    
    DestroyList(L);
}

/* ================================
 *  经典算法测试
 * ================================ */

/**
 * 测试链表逆转算法
 */
void test_reverse_list() {
    print_subtest("链表逆转算法测试");
    
    // 创建测试链表：1 -> 2 -> 3 -> 4 -> 5
    ElemType arr[] = {1, 2, 3, 4, 5};
    LinkList L;
    CreateListFromArray(&L, arr, 5);
    
    printf("逆转前：");
    PrintList(L);
    
    bool result = ReverseList(L);
    assert_test(result, "链表逆转操作成功");
    
    printf("逆转后：");
    PrintList(L);
    
    // 验证逆转结果：应该是 5 4 3 2 1
    ElemType expected[] = {5, 4, 3, 2, 1};
    for (int i = 1; i <= 5; i++) {
        ElemType e;
        GetElem(L, i, &e);
        assert_test(e == expected[i-1], "逆转结果正确");
    }
    
    DestroyList(L);
}

/**
 * 测试有序链表合并
 */
void test_merge_list() {
    print_subtest("有序链表合并测试");
    
    // 创建两个有序链表
    ElemType arr1[] = {1, 3, 5, 7};
    ElemType arr2[] = {2, 4, 6, 8, 9};
    
    LinkList La, Lb, Lc;
    CreateListFromArray(&La, arr1, 4);
    CreateListFromArray(&Lb, arr2, 5);
    
    printf("链表A：");
    PrintList(La);
    printf("链表B：");
    PrintList(Lb);
    
    bool result = MergeList(La, Lb, &Lc);
    assert_test(result, "有序链表合并成功");
    
    printf("合并结果：");
    PrintList(Lc);
    
    // 验证合并结果长度
    assert_test(ListLength(Lc) == 9, "合并后长度正确");
    
    // 验证合并结果是有序的
    for (int i = 1; i < ListLength(Lc); i++) {
        ElemType e1, e2;
        GetElem(Lc, i, &e1);
        GetElem(Lc, i + 1, &e2);
        assert_test(e1 <= e2, "合并结果有序");
    }
    
    DestroyList(Lc);
    // 注意：La和Lb已在MergeList中释放
}

/**
 * 测试删除重复元素
 */
void test_remove_duplicates() {
    print_subtest("删除重复元素测试");
    
    // 创建有重复的有序链表：1 1 2 2 2 3 4 4 5
    LinkList L = InitList();
    int data[] = {1, 1, 2, 2, 2, 3, 4, 4, 5};
    for (int i = 0; i < 9; i++) {
        ListInsert(L, i + 1, data[i]);
    }
    
    printf("去重前：");
    PrintList(L);
    
    int removed_count = RemoveDuplicates(L);
    printf("去重后：");
    PrintList(L);
    
    assert_test(removed_count == 4, "删除了4个重复元素");
    assert_test(ListLength(L) == 5, "去重后长度为5");
    
    // 验证去重结果：应该是 1 2 3 4 5
    for (int i = 1; i <= 5; i++) {
        ElemType e;
        GetElem(L, i, &e);
        assert_test(e == i, "去重结果正确");
    }
    
    DestroyList(L);
}

/**
 * 测试删除所有指定值的节点
 */
void test_remove_all_x() {
    print_subtest("删除所有指定值节点测试");
    
    // 创建测试链表：1 2 3 2 4 2 5
    LinkList L = InitList();
    int data[] = {1, 2, 3, 2, 4, 2, 5};
    for (int i = 0; i < 7; i++) {
        ListInsert(L, i + 1, data[i]);
    }
    
    printf("删除前：");
    PrintList(L);
    
    int removed_count = RemoveAllX(L, 2);
    printf("删除所有值为2的节点后：");
    PrintList(L);
    
    assert_test(removed_count == 3, "删除了3个值为2的节点");
    assert_test(ListLength(L) == 4, "删除后长度为4");
    
    // 验证剩余元素：应该是 1 3 4 5
    ElemType expected[] = {1, 3, 4, 5};
    for (int i = 1; i <= 4; i++) {
        ElemType e;
        GetElem(L, i, &e);
        assert_test(e == expected[i-1], "删除结果正确");
    }
    
    DestroyList(L);
}

/**
 * 测试有序插入
 */
void test_insert_in_order() {
    print_subtest("有序插入测试");
    
    // 创建有序链表：1 3 5 7 9
    ElemType arr[] = {1, 3, 5, 7, 9};
    LinkList L;
    CreateListFromArray(&L, arr, 5);
    
    printf("原有序链表：");
    PrintList(L);
    
    // 测试插入不同位置
    printf("插入元素4：\n");
    assert_test(InsertInOrder(L, 4), "有序插入4");
    PrintList(L);
    
    printf("插入元素0：\n");
    assert_test(InsertInOrder(L, 0), "有序插入0");
    PrintList(L);
    
    printf("插入元素10：\n");
    assert_test(InsertInOrder(L, 10), "有序插入10");
    PrintList(L);
    
    // 验证插入后仍然有序
    for (int i = 1; i < ListLength(L); i++) {
        ElemType e1, e2;
        GetElem(L, i, &e1);
        GetElem(L, i + 1, &e2);
        assert_test(e1 <= e2, "插入后仍保持有序");
    }
    
    DestroyList(L);
}

/**
 * 测试环检测算法
 */
void test_has_cycle() {
    print_subtest("环检测算法测试");
    
    // 创建无环链表
    ElemType arr[] = {1, 2, 3, 4, 5};
    LinkList L;
    CreateListFromArray(&L, arr, 5);
    
    printf("测试无环链表：");
    PrintList(L);
    bool has_cycle = HasCycle(L);
    assert_test(!has_cycle, "无环链表检测正确");
    printf("检测结果：%s\n", has_cycle ? "有环" : "无环");
    
    // 人工创建环（仅用于测试，实际使用中要小心）
    LNode *tail = L->next;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    LNode *second = L->next->next;
    tail->next = second;  // 创建环：tail -> second
    
    printf("人工创建环后检测：\n");
    has_cycle = HasCycle(L);
    assert_test(has_cycle, "有环链表检测正确");
    printf("检测结果：%s\n", has_cycle ? "有环" : "无环");
    
    // 破除环，避免DestroyList死循环
    tail->next = NULL;
    DestroyList(L);
}

/**
 * 测试找中间节点
 */
void test_find_middle() {
    print_subtest("找中间节点测试");
    
    // 测试奇数个节点
    ElemType arr1[] = {1, 2, 3, 4, 5};
    LinkList L1;
    CreateListFromArray(&L1, arr1, 5);
    
    printf("奇数个节点：");
    PrintList(L1);
    
    LNode *middle = FindMiddleNode(L1);
    assert_test(middle != NULL && middle->data == 3, "奇数个节点中间值为3");
    printf("中间节点值：%d\n", middle->data);
    
    DestroyList(L1);
    
    // 测试偶数个节点
    ElemType arr2[] = {1, 2, 3, 4};
    LinkList L2;
    CreateListFromArray(&L2, arr2, 4);
    
    printf("偶数个节点：");
    PrintList(L2);
    
    middle = FindMiddleNode(L2);
    assert_test(middle != NULL && middle->data == 3, "偶数个节点中间值为3");
    printf("中间节点值：%d\n", middle->data);
    
    DestroyList(L2);
}

/* ================================
 * 性能测试
 * ================================ */

/**
 * 链表性能测试
 */
void test_performance() {
    print_separator("链表性能测试");
    
    printf("正在进行性能测试，请稍候...\n");
    
    // 基本操作性能测试
    PerformanceTest_LinkList(1000);
    
    // 链表vs顺序表性能对比
    PerformanceCompare_ListVsArray(1000);
    
    // 高级算法性能测试
    PerformanceTest_Advanced(500);
    
    printf("\n✓ 性能测试完成！\n");
}

/* ================================
 * 主测试函数
 * ================================ */

/**
 * 运行所有链表算法测试
 */
void test_advanced_algorithms() {
    print_separator(" 经典算法测试");
    
    test_reverse_list();
    test_merge_list();
    test_remove_duplicates();
    test_remove_all_x();
    test_insert_in_order();
    test_has_cycle();
    test_find_middle();
    
    printf("\n✓ 经典算法测试全部通过！\n");
}

/**
 * 运行所有建表算法测试
 */
void test_create_algorithms() {
    print_separator("建表算法测试");
    
    test_create_list_head();
    test_create_list_tail();
    test_create_from_array();
    
    printf("\n✓ 建表算法测试全部通过！\n");
}

/**
 * 主函数：运行所有测试
 */
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                单链表测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\n测试开始时间：");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    
    // 运行所有测试模块
    test_basic_operations();        // 基本操作测试
    test_create_algorithms();       // 建表算法测试
    test_advanced_algorithms();     // 高级算法测试
    test_performance();            // 性能测试
    
    // 测试总结
    print_separator("====== linklist testcase finished!=======");

    printf("\n结束时间：");
    now = time(NULL);
    printf("%s", ctime(&now));
    
    
    return 0;
} 