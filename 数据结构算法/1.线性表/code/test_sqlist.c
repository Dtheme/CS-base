/* test_sqlist.c - 顺序表测试程序
 * create by: zw.duan
 */

#include "sqlist.h"
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
 * 测试静态顺序表的基本操作
 */
void test_basic_operations() {
    print_separator("静态顺序表基本操作测试");
    
    SqList L;
    ElemType e;
    
    print_subtest("1. 初始化测试");
    bool init_result = InitList(&L);
    assert_test(init_result, "初始化操作");
    assert_test(ListLength(L) == 0, "初始长度为0");
    assert_test(ListEmpty(L), "初始状态为空");
    
    print_subtest("2. 插入操作测试");
    printf("依次插入元素：10, 20, 30, 15（在位置2插入15）\n");
    
    assert_test(ListInsert(&L, 1, 10), "位置1插入10");
    PrintList(L);
    
    assert_test(ListInsert(&L, 2, 20), "位置2插入20");
    PrintList(L);
    
    assert_test(ListInsert(&L, 3, 30), "位置3插入30");
    PrintList(L);
    
    assert_test(ListInsert(&L, 2, 15), "位置2插入15");
    PrintList(L);
    
    assert_test(ListLength(L) == 4, "插入后长度为4");
    assert_test(!ListEmpty(L), "插入后非空");
    
    print_subtest("3. 查找操作测试");
    
    // 按位查找测试
    bool get_result = GetElem(L, 3, &e);
    assert_test(get_result && e == 20, "第3个位置的元素为20");
    printf("第3个位置的元素：%d\n", e);
    
    // 按值查找测试
    int pos = LocateElem(L, 30);
    assert_test(pos == 4, "元素30的位置为4");
    printf("元素30的位置：%d\n", pos);
    
    pos = LocateElem(L, 99);
    assert_test(pos == 0, "元素99不存在");
    printf("元素99的位置：%d（0表示不存在）\n", pos);
    
    // 前驱后继测试
    ElemType pre, next;
    assert_test(PriorElem(L, 20, &pre) && pre == 15, "元素20的前驱为15");
    assert_test(NextElem(L, 15, &next) && next == 20, "元素15的后继为20");
    assert_test(!PriorElem(L, 10, &pre), "首元素没有前驱");
    assert_test(!NextElem(L, 30, &next), "尾元素没有后继");
    
    print_subtest("4. 删除操作测试");
    printf("删除第2个位置的元素\n");
    
    bool delete_result = ListDelete(&L, 2, &e);
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
    assert_test(!ListInsert(&L, 0, 5), "位置0插入应失败");
    assert_test(!ListInsert(&L, 10, 5), "位置超界插入应失败");
    assert_test(!ListDelete(&L, 0, &e), "位置0删除应失败");
    assert_test(!ListDelete(&L, 10, &e), "位置超界删除应失败");
    
    // 清空表测试
    ClearList(&L);
    assert_test(ListLength(L) == 0, "清空后长度为0");
    assert_test(ListEmpty(L), "清空后为空表");
    
    printf("\n✓ 基本操作测试全部通过！\n");
}

/**
 * 测试动态顺序表
 */
void test_dynamic_list() {
    print_separator("动态顺序表操作测试");
    
    DynamicList DL;
    
    print_subtest("1. 初始化动态顺序表");
    bool init_result = InitDynamicList(&DL);
    assert_test(init_result, "动态表初始化");
    PrintDynamicList(DL);
    
    print_subtest("2. 插入元素测试（验证自动扩容）");
    printf("连续插入20个元素，观察扩容过程\n");
    
    for (int i = 1; i <= 20; i++) {
        bool insert_result = DynamicListInsert(&DL, i, i * 10);
        assert_test(insert_result, "动态表插入");
        
        // 在关键点显示状态
        if (i == INIT_SIZE || i == INIT_SIZE + INCREMENT || i == 20) {
            printf("插入第%d个元素后：", i);
            PrintDynamicList(DL);
        }
    }
    
    assert_test(DL.length == 20, "动态表最终长度为20");
    
    print_subtest("3. 销毁动态顺序表");
    DestroyDynamicList(&DL);
    
    printf("\n✓ 动态表测试全部通过！\n");
}

/* ================================
 * 高级算法测试函数（ 重点）
 * ================================ */

/**
 * 测试删除所有指定值的元素
 */
void test_delete_all_x() {
    print_subtest("删除所有值为指定值的元素");
    
    SqList L;
    InitList(&L);
    
    // 构造测试数据：1, 2, 3, 2, 4, 2, 5
    int testData[] = {1, 2, 3, 2, 4, 2, 5};
    int dataSize = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        ListInsert(&L, i + 1, testData[i]);
    }
    
    printf("原始表：");
    PrintList(L);
    
    int deletedCount = DeleteAllX(&L, 2);
    printf("删除所有值为2的元素后：");
    PrintList(L);
    
    assert_test(deletedCount == 3, "删除了3个值为2的元素");
    assert_test(ListLength(L) == 4, "删除后长度为4");
    
    // 验证剩余元素
    ElemType expected[] = {1, 3, 4, 5};
    for (int i = 0; i < L.length; i++) {
        assert_test(L.data[i] == expected[i], "剩余元素正确");
    }
}

/**
 * 测试删除范围内的元素
 */
void test_delete_range() {
    print_subtest("删除范围内的元素");
    
    SqList L;
    InitList(&L);
    
    // 构造测试数据：1, 2, 3, 4, 5, 6, 7, 8, 9
    for (int i = 1; i <= 9; i++) {
        ListInsert(&L, i, i);
    }
    
    printf("原始表：");
    PrintList(L);
    
    int deletedCount = DeleteRange(&L, 3, 6);
    printf("删除范围[3,6]内的元素后：");
    PrintList(L);
    
    assert_test(deletedCount == 4, "删除了4个范围内的元素");
    assert_test(ListLength(L) == 5, "删除后长度为5");
    
    // 验证剩余元素
    ElemType expected[] = {1, 2, 7, 8, 9};
    for (int i = 0; i < L.length; i++) {
        assert_test(L.data[i] == expected[i], "剩余元素正确");
    }
}

/**
 * 测试逆转顺序表
 */
void test_reverse_list() {
    print_subtest("逆转顺序表");
    
    SqList L;
    InitList(&L);
    
    // 构造测试数据：1, 3, 5, 7, 9
    int testData[] = {1, 3, 5, 7, 9};
    int dataSize = sizeof(testData) / sizeof(testData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        ListInsert(&L, i + 1, testData[i]);
    }
    
    printf("逆转前：");
    PrintList(L);
    
    assert_test(ReverseList(&L), "逆转操作成功");
    printf("逆转后：");
    PrintList(L);
    
    // 验证逆转结果
    ElemType expected[] = {9, 7, 5, 3, 1};
    for (int i = 0; i < L.length; i++) {
        assert_test(L.data[i] == expected[i], "逆转结果正确");
    }
}

/**
 * 测试循环左移
 */
void test_left_shift() {
    print_subtest("循环左移");
    
    SqList L;
    InitList(&L);
    
    // 构造测试数据：1, 2, 3, 4, 5
    for (int i = 1; i <= 5; i++) {
        ListInsert(&L, i, i);
    }
    
    printf("左移前：");
    PrintList(L);
    
    // 左移2位
    assert_test(LeftShift(&L, 2), "左移操作成功");
    printf("左移2位后：");
    PrintList(L);
    
    // 验证左移结果：应该是 3, 4, 5, 1, 2
    ElemType expected[] = {3, 4, 5, 1, 2};
    for (int i = 0; i < L.length; i++) {
        assert_test(L.data[i] == expected[i], "左移结果正确");
    }
}

/**
 * 测试有序表操作
 */
void test_ordered_operations() {
    print_subtest("有序表操作");
    
    SqList L;
    InitList(&L);
    
    // 构造有序数据：1, 3, 5, 7, 9
    int orderedData[] = {1, 3, 5, 7, 9};
    int dataSize = sizeof(orderedData) / sizeof(orderedData[0]);
    
    for (int i = 0; i < dataSize; i++) {
        ListInsert(&L, i + 1, orderedData[i]);
    }
    
    printf("有序表：");
    PrintList(L);
    
    // 测试有序插入
    printf("插入元素4：\n");
    assert_test(InsertInOrder(&L, 4), "有序插入4");
    PrintList(L);
    
    printf("插入元素0：\n");
    assert_test(InsertInOrder(&L, 0), "有序插入0");
    PrintList(L);
    
    printf("插入元素10：\n");
    assert_test(InsertInOrder(&L, 10), "有序插入10");
    PrintList(L);
    
    // 测试二分查找
    int pos = BinarySearch(L, 5);
    assert_test(pos > 0, "二分查找找到元素5");
    printf("二分查找元素5的位置：%d\n", pos);
    
    pos = BinarySearch(L, 6);
    assert_test(pos == 0, "二分查找未找到元素6");
    printf("二分查找元素6的位置：%d（0表示未找到）\n", pos);
    
    // 测试去重（先构造有重复的有序表）
    SqList L2;
    InitList(&L2);
    int dupData[] = {1, 1, 2, 2, 2, 3, 4, 4, 5};
    int dupSize = sizeof(dupData) / sizeof(dupData[0]);
    
    for (int i = 0; i < dupSize; i++) {
        ListInsert(&L2, i + 1, dupData[i]);
    }
    
    printf("有重复的有序表：");
    PrintList(L2);
    
    int removedCount = RemoveDuplicates(&L2);
    printf("去重后：");
    PrintList(L2);
    
    assert_test(removedCount == 4, "删除了4个重复元素");
    assert_test(ListLength(L2) == 5, "去重后长度为5");
}

/**
 * 测试合并和集合运算
 */
void test_merge_and_set_operations() {
    print_subtest("合并和集合运算");
    
    SqList La, Lb, Lc;
    InitList(&La);
    InitList(&Lb);
    InitList(&Lc);
    
    // 构造两个有序表
    int dataA[] = {1, 3, 5, 7};
    int dataB[] = {2, 4, 6, 8, 9};
    
    for (int i = 0; i < 4; i++) {
        ListInsert(&La, i + 1, dataA[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        ListInsert(&Lb, i + 1, dataB[i]);
    }
    
    printf("表A：");
    PrintList(La);
    printf("表B：");
    PrintList(Lb);
    
    // 测试合并
    assert_test(MergeList(La, Lb, &Lc), "合并操作成功");
    printf("合并结果：");
    PrintList(Lc);
    
    assert_test(ListLength(Lc) == 9, "合并后长度为9");
    
    // 验证合并结果是有序的
    for (int i = 1; i < Lc.length; i++) {
        assert_test(Lc.data[i-1] <= Lc.data[i], "合并结果有序");
    }
    
    // 测试交集运算
    SqList La2, Lb2, Linter;
    InitList(&La2);
    InitList(&Lb2);
    InitList(&Linter);
    
    int dataA2[] = {1, 2, 3, 4, 5};
    int dataB2[] = {3, 4, 5, 6, 7};
    
    for (int i = 0; i < 5; i++) {
        ListInsert(&La2, i + 1, dataA2[i]);
        ListInsert(&Lb2, i + 1, dataB2[i]);
    }
    
    printf("表A2：");
    PrintList(La2);
    printf("表B2：");
    PrintList(Lb2);
    
    assert_test(Intersection(La2, Lb2, &Linter), "交集运算成功");
    printf("交集结果：");
    PrintList(Linter);
    
    assert_test(ListLength(Linter) == 3, "交集长度为3");
    
    // 测试并集运算
    SqList Lunion;
    InitList(&Lunion);
    
    assert_test(Union(La2, Lb2, &Lunion), "并集运算成功");
    printf("并集结果：");
    PrintList(Lunion);
    
    assert_test(ListLength(Lunion) == 7, "并集长度为7");
}

/**
 * 测试所有高级算法
 */
void test_advanced_operations() {
    print_separator("高级操作测试（ 重点）");
    
    test_delete_all_x();
    test_delete_range();
    test_reverse_list();
    test_left_shift();
    test_ordered_operations();
    test_merge_and_set_operations();
    
    printf("\n✓ 高级算法测试全部通过！\n");
}

/* ================================
 * 性能测试函数
 * ================================ */

/**
 * 进行性能测试
 */
void test_performance() {
    print_separator("性能测试");
    
    printf("正在进行性能测试，请稍候...\n");
    
    // 基本操作性能测试
    PerformanceTest_Basic(1000);
    
    // 高级算法性能测试
    PerformanceTest_Advanced(500);
    
    // 静态表vs动态表性能对比
    PerformanceCompare_StaticVsDynamic(1000);
    
    printf("\n✓ 性能测试完成！\n");
}

/* ================================
 * 错误处理和鲁棒性测试
 * ================================ */

/**
 * 测试错误处理和边界条件
 */
void test_error_handling() {
    print_separator("错误处理和边界条件测试");
    
    SqList L;
    ElemType e;
    
    // 初始化L以避免未初始化警告
    InitList(&L);
    
    print_subtest("1. 空指针处理测试");
    assert_test(!InitList(NULL), "空指针初始化应失败");
    assert_test(!ListInsert(NULL, 1, 10), "空指针插入应失败");
    assert_test(!ListDelete(NULL, 1, &e), "空指针删除应失败");
    assert_test(!GetElem(L, 1, NULL), "空返回指针应失败");
    
    print_subtest("2. 空表操作测试");
    InitList(&L);
    assert_test(!ListDelete(&L, 1, &e), "空表删除应失败");
    assert_test(!GetElem(L, 1, &e), "空表查找应失败");
    assert_test(LocateElem(L, 10) == 0, "空表按值查找返回0");
    
    print_subtest("3. 越界操作测试");
    ListInsert(&L, 1, 10);
    ListInsert(&L, 2, 20);
    PrintList(L);
    
    assert_test(!GetElem(L, 0, &e), "位序0查找应失败");
    assert_test(!GetElem(L, 10, &e), "位序超界查找应失败");
    assert_test(!ListDelete(&L, 0, &e), "位序0删除应失败");
    assert_test(!ListDelete(&L, 10, &e), "位序超界删除应失败");
    assert_test(!ListInsert(&L, 0, 5), "位序0插入应失败");
    assert_test(!ListInsert(&L, 10, 5), "位序超界插入应失败");
    
    print_subtest("4. 满表操作测试");
    ClearList(&L);
    
    // 填满表
    printf("尝试填满顺序表...\n");
    int insertCount = 0;
    for (int i = 1; i <= MAXSIZE + 5; i++) {
        if (ListInsert(&L, L.length + 1, i)) {
            insertCount++;
        } else {
            printf("在插入第%d个元素时失败（表已满）\n", i);
            break;
        }
    }
    printf("成功插入%d个元素\n", insertCount);
    assert_test(insertCount == MAXSIZE, "成功插入MAXSIZE个元素");
    assert_test(ListLength(L) == MAXSIZE, "表长达到最大值");
    
    // 尝试再插入一个元素
    assert_test(!ListInsert(&L, 1, 999), "满表插入应失败");
    
    print_subtest("5. 特殊参数测试");
    assert_test(DeleteRange(&L, 5, 3) == -1, "无效范围删除应失败");
    assert_test(LeftShift(&L, 0), "左移0位应成功且无变化");
    
    printf("\n✓ 错误处理测试全部通过！\n");
}

/* ================================
 * 教学演示函数
 * ================================ */

/**
 * 演示顺序表的教学用例
 */
void demo_teaching_examples() {
    print_separator("教学演示用例");
    
    print_subtest("演示1：理解位序和下标的区别");
    SqList L;
    InitList(&L);
    
    printf("插入元素：A, B, C\n");
    ListInsert(&L, 1, 'A');
    ListInsert(&L, 2, 'B');
    ListInsert(&L, 3, 'C');
    
    printf("顺序表内容：");
    for (int i = 0; i < L.length; i++) {
        printf("下标%d->位序%d: %c  ", i, i+1, L.data[i]);
    }
    printf("\n");
    
    print_subtest("演示2：插入操作的元素移动过程");
    printf("在位置2插入元素X，观察元素移动：\n");
    printf("插入前：A B C\n");
    printf("移动过程：A _ B C（B和C向右移动）\n");
    ListInsert(&L, 2, 'X');
    printf("插入后：");
    for (int i = 0; i < L.length; i++) {
        printf("%c ", L.data[i]);
    }
    printf("\n");
    
    print_subtest("演示3：时间复杂度的实际体现");
    ClearList(&L);
    
    printf("测试不同位置插入的时间差异：\n");
    
    // 构造较大的表
    for (int i = 1; i <= 50; i++) {
        ListInsert(&L, i, i);
    }
    
    clock_t start, end;
    
    // 头部插入（最坏情况）
    start = clock();
    for (int i = 0; i < 100; i++) {
        ListInsert(&L, 1, 999);
        ListDelete(&L, 1, &(ElemType){0});  // 删除以保持表长
    }
    end = clock();
    double head_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // 尾部插入（最好情况）
    start = clock();
    for (int i = 0; i < 100; i++) {
        ListInsert(&L, L.length + 1, 999);
        ListDelete(&L, L.length, &(ElemType){0});  // 删除以保持表长
    }
    end = clock();
    double tail_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("头部插入100次用时：%.6f秒\n", head_time);
    printf("尾部插入100次用时：%.6f秒\n", tail_time);
    printf("性能比值（头部/尾部）：%.2f\n", head_time / tail_time);
    
    printf("\n✓ 教学演示完成！\n");
}

/* ================================
 * 主测试函数
 * ================================ */

/**
 * 主函数：运行所有测试
 */
int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                顺序表测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("\n测试开始时间：");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    
    // 运行所有测试模块
    test_basic_operations();        // 基本操作测试
    test_dynamic_list();           // 动态表测试
    test_advanced_operations();    // 高级算法测试
    test_performance();           // 性能测试
    test_error_handling();        // 错误处理测试
    demo_teaching_examples();     // 教学演示
    
    // 测试总结
    print_separator("====== sqlist testcase finished!=======");
        
    printf("\n结束时间：");
    now = time(NULL);
    printf("%s", ctime(&now));
  
    
    return 0;
} 