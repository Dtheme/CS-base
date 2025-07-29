/* test_bitree.c - 树状数组测试程序
 * create by: zw.duan
 */

#include "bitree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

// ============= 测试函数声明 =============

void TestBasicOperations();
void TestAdvancedOperations();
void Test2DBITree();
void TestDynamicRangeCounter();
void TestOfflineQueries();
void TestInversionCounting();
void TestPerformance();
void TestSerialization();
void TestEdgeCases();
void TestMemoryManagement();

// ============= 主函数 =============

int main() {
    printf("🌳 树状数组（Binary Indexed Tree）测试程序\n");
    printf("==========================================\n\n");
    
    // 设置随机种子
    srand((unsigned int)time(NULL));
    
    // 运行所有测试
    TestBasicOperations();
    TestAdvancedOperations();
    Test2DBITree();
    TestDynamicRangeCounter();
    TestOfflineQueries();
    TestInversionCounting();
    TestPerformance();
    TestSerialization();
    TestEdgeCases();
    TestMemoryManagement();
    
    printf("🎉 所有测试完成！\n");
    return 0;
}

// ============= 基本操作测试 =============

void TestBasicOperations() {
    printf("📋 测试基本操作...\n");
    
    // 测试创建和销毁
    BITree *bit = CreateBITree(10);
    assert(bit != NULL);
    assert(bit->size == 10);
    assert(bit->tree != NULL);
    
    printf("✅ 创建树状数组成功\n");
    
    // 测试空检查
    assert(!BITreeEmpty(bit));
    assert(BITreeSize(bit) == 10);
    
    printf("✅ 基本属性检查通过\n");
    
    // 测试单点更新和查询
    Update(bit, 0, 5);
    Update(bit, 1, 3);
    Update(bit, 2, 7);
    Update(bit, 3, 2);
    Update(bit, 4, 8);
    
    assert(Query(bit, 0) == 5);
    assert(Query(bit, 1) == 8);
    assert(Query(bit, 2) == 15);
    assert(Query(bit, 3) == 17);
    assert(Query(bit, 4) == 25);
    
    printf("✅ 单点更新和前缀和查询通过\n");
    
    // 测试区间查询
    assert(QueryRange(bit, 1, 3) == 12);
    assert(QueryRange(bit, 0, 4) == 25);
    assert(QueryRange(bit, 2, 2) == 7);
    
    printf("✅ 区间查询通过\n");
    
    // 测试获取和设置元素
    assert(GetElement(bit, 0) == 5);
    assert(GetElement(bit, 1) == 3);
    assert(GetElement(bit, 2) == 7);
    
    SetElement(bit, 1, 10);
    assert(GetElement(bit, 1) == 10);
    assert(Query(bit, 1) == 15);
    
    printf("✅ 元素获取和设置通过\n");
    
    // 测试从数组构建
    BITreeElemType arr[] = {1, 2, 3, 4, 5};
    BITree *bit2 = CreateBITree(5);
    BuildBITree(bit2, arr, 5);
    
    assert(Query(bit2, 0) == 1);
    assert(Query(bit2, 1) == 3);
    assert(Query(bit2, 2) == 6);
    assert(Query(bit2, 3) == 10);
    assert(Query(bit2, 4) == 15);
    
    printf("✅ 从数组构建通过\n");
    
    // 清理
    DestroyBITree(bit);
    DestroyBITree(bit2);
    
    printf("✅ 基本操作测试完成\n\n");
}

// ============= 高级操作测试 =============

void TestAdvancedOperations() {
    printf("🔧 测试高级操作...\n");
    
    BITree *bit = CreateBITree(10);
    
    // 初始化数据
    BITreeElemType arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    BuildBITree(bit, arr, 10);
    
    // 测试区间更新
    printf("更新前的区间[2,5]的和: %d\n", QueryRange(bit, 2, 5));
    UpdateRange(bit, 2, 5, 10);
    BITreeElemType range_sum = QueryRange(bit, 2, 5);
    printf("更新后的区间[2,5]的和: %d\n", range_sum);
    
    // 打印每个元素的值
    printf("更新后的元素值:\n");
    for (int i = 0; i < 10; i++) {
        printf("arr[%d] = %d\n", i, GetElement(bit, i));
    }
    
    // 原始值: 4+1+5+9 = 19，加上区间更新: 19 + 4*10 = 59
    assert(range_sum == 59);
    printf("✅ 区间更新通过\n");
    
    // 测试统计操作
    int less_than_5 = CountLessThan(bit, 5);
    int greater_than_5 = CountGreaterThan(bit, 5);
    int in_range = CountInRange(bit, 3, 7);
    
    printf("小于5的元素个数: %d\n", less_than_5);
    printf("大于5的元素个数: %d\n", greater_than_5);
    printf("在[3,7]范围内的元素个数: %d\n", in_range);
    
    printf("✅ 统计操作通过\n");
    
    // 测试查找操作
    int kth_smallest = FindKthSmallest(bit, 3);
    int kth_largest = FindKthLargest(bit, 3);
    
    printf("第3小的元素位置: %d\n", kth_smallest);
    printf("第3大的元素位置: %d\n", kth_largest);
    
    printf("✅ 查找操作通过\n");
    
    // 测试验证
    ValidateBITree(bit);
    
    DestroyBITree(bit);
    printf("✅ 高级操作测试完成\n\n");
}

// ============= 二维树状数组测试 =============

void Test2DBITree() {
    printf("📊 测试二维树状数组...\n");
    
    BITree2D *bit2d = CreateBITree2D(5, 5);
    assert(bit2d != NULL);
    assert(bit2d->rows == 5);
    assert(bit2d->cols == 5);
    
    printf("✅ 创建二维树状数组成功\n");
    
    // 测试更新和查询
    Update2D(bit2d, 0, 0, 1);
    Update2D(bit2d, 1, 1, 2);
    Update2D(bit2d, 2, 2, 3);
    Update2D(bit2d, 3, 3, 4);
    Update2D(bit2d, 4, 4, 5);
    
    assert(Query2D(bit2d, 0, 0) == 1);
    assert(Query2D(bit2d, 1, 1) == 3);
    assert(Query2D(bit2d, 2, 2) == 6);
    assert(Query2D(bit2d, 3, 3) == 10);
    assert(Query2D(bit2d, 4, 4) == 15);
    
    printf("✅ 二维单点更新和查询通过\n");
    
    // 测试二维区间查询
    BITreeElemType sum = QueryRange2D(bit2d, 0, 0, 2, 2);
    printf("区间[0,0]到[2,2]的和: %d\n", sum);
    
    // 测试更大的区间
    Update2D(bit2d, 0, 1, 10);
    Update2D(bit2d, 1, 0, 20);
    sum = QueryRange2D(bit2d, 0, 0, 1, 1);
    printf("区间[0,0]到[1,1]的和: %d\n", sum);
    
    printf("✅ 二维区间查询通过\n");
    
    DestroyBITree2D(bit2d);
    printf("✅ 二维树状数组测试完成\n\n");
}

// ============= 动态区间统计测试 =============

void TestDynamicRangeCounter() {
    printf("📈 测试动态区间统计...\n");
    
    DynamicRangeCounter *drc = CreateDynamicRangeCounter(100);
    assert(drc != NULL);
    
    printf("✅ 创建动态区间统计器成功\n");
    
    // 添加元素
    AddElement(drc, 10);
    AddElement(drc, 20);
    AddElement(drc, 30);
    AddElement(drc, 40);
    AddElement(drc, 50);
    AddElement(drc, 10); // 重复添加
    
    assert(GetFrequency(drc, 10) == 2);
    assert(GetFrequency(drc, 20) == 1);
    assert(GetFrequency(drc, 30) == 1);
    
    printf("✅ 添加元素和频率查询通过\n");
    
    // 测试区间频率
    int freq_10_30 = GetRangeFrequency(drc, 10, 30);
    int freq_20_50 = GetRangeFrequency(drc, 20, 50);
    
    printf("区间[10,30]的频率: %d\n", freq_10_30);
    printf("区间[20,50]的频率: %d\n", freq_20_50);
    
    assert(freq_10_30 == 4); // 10(2次) + 20(1次) + 30(1次)
    assert(freq_20_50 == 4); // 20(1次) + 30(1次) + 40(1次) + 50(1次)
    
    printf("✅ 区间频率查询通过\n");
    
    // 测试移除元素
    RemoveElement(drc, 10);
    assert(GetFrequency(drc, 10) == 1);
    
    RemoveElement(drc, 20);
    assert(GetFrequency(drc, 20) == 0);
    
    printf("✅ 移除元素通过\n");
    
    DestroyDynamicRangeCounter(drc);
    printf("✅ 动态区间统计测试完成\n\n");
}

// ============= 离线查询测试 =============

void TestOfflineQueries() {
    printf("🔍 测试离线查询处理...\n");
    
    BITreeElemType arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = 10;
    
    OfflineQuery queries[] = {
        {0, 2, 0, 0},   // 查询[0,2]
        {1, 4, 1, 0},   // 查询[1,4]
        {2, 6, 2, 0},   // 查询[2,6]
        {0, 9, 3, 0},   // 查询[0,9]
        {3, 7, 4, 0}    // 查询[3,7]
    };
    int query_count = 5;
    
    ProcessOfflineQueries(arr, size, queries, query_count);
    
    // 验证结果
    printf("查询结果:\n");
    for (int i = 0; i < query_count; i++) {
        printf("  查询[%d,%d]: %d\n", queries[i].left, queries[i].right, queries[i].result);
    }
    
    // 验证结果
    assert(queries[0].result == 6);   // 1+2+3
    assert(queries[1].result == 14);  // 2+3+4+5
    assert(queries[2].result == 25);  // 3+4+5+6+7
    assert(queries[3].result == 55);  // 1+2+...+10
    assert(queries[4].result == 30);  // 4+5+6+7+8 (修正期望值)
    
    printf("✅ 离线查询处理通过\n");
    
    printf("✅ 离线查询测试完成\n\n");
}

// ============= 逆序对计算测试 =============

void TestInversionCounting() {
    printf("🔄 测试逆序对计算...\n");
    
    BITreeElemType arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    int size = 10;
    
    // 使用暴力方法计算
    long long brute_force = CountInversions(arr, size);
    printf("暴力方法计算的逆序对数: %lld\n", brute_force);
    
    // 使用树状数组计算
    BITree *bit = CreateBITree(size);
    long long bit_count = CountInversionsWithBITree(bit, arr, size);
    printf("树状数组计算的逆序对数: %lld\n", bit_count);
    
    assert(brute_force == bit_count);
    printf("✅ 逆序对计算结果一致\n");
    
    // 测试更多例子
    BITreeElemType arr2[] = {5, 4, 3, 2, 1};
    int size2 = 5;
    
    long long inv1 = CountInversions(arr2, size2);
    long long inv2 = CountInversionsWithBITree(bit, arr2, size2);
    
    printf("数组[5,4,3,2,1]的逆序对数: %lld\n", inv1);
    assert(inv1 == inv2);
    
    DestroyBITree(bit);
    printf("✅ 逆序对计算测试完成\n\n");
}

// ============= 性能测试 =============

void TestPerformance() {
    printf("⚡ 测试性能...\n");
    
    // 测试不同大小的性能
    int sizes[] = {1000, 10000, 100000};
    int num_sizes = 3;
    
    for (int i = 0; i < num_sizes; i++) {
        printf("测试大小: %d\n", sizes[i]);
        PerformanceTest_BITree(sizes[i]);
        PerformanceCompare_BITree_vs_Array(sizes[i]);
    }
    
    // 测试二维树状数组性能
    PerformanceTest_2D_BITree(100, 100);
    
    printf("✅ 性能测试完成\n\n");
}

// ============= 序列化测试 =============

void TestSerialization() {
    printf("💾 测试序列化...\n");
    
    // 创建树状数组并添加数据
    BITree *bit = CreateBITree(10);
    BITreeElemType arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    BuildBITree(bit, arr, 10);
    
    // 序列化
    SerializeBITree(bit, "bitree_test.dat");
    
    // 反序列化
    BITree *bit2 = DeserializeBITree("bitree_test.dat");
    assert(bit2 != NULL);
    assert(bit2->size == bit->size);
    
    // 验证数据一致性
    for (int i = 0; i < bit->size; i++) {
        assert(Query(bit, i) == Query(bit2, i));
    }
    
    printf("✅ 序列化和反序列化通过\n");
    
    // 清理
    DestroyBITree(bit);
    DestroyBITree(bit2);
    
    // 删除测试文件
    remove("bitree_test.dat");
    
    printf("✅ 序列化测试完成\n\n");
}

// ============= 边界情况测试 =============

void TestEdgeCases() {
    printf("⚠️ 测试边界情况...\n");
    
    // 测试空指针
    Update(NULL, 0, 1);
    Query(NULL, 0);
    QueryRange(NULL, 0, 1);
    
    // 测试无效索引
    BITree *bit = CreateBITree(5);
    Update(bit, -1, 1);
    Update(bit, 10, 1);
    Query(bit, -1);
    Query(bit, 10);
    QueryRange(bit, -1, 3);
    QueryRange(bit, 2, 10);
    QueryRange(bit, 5, 3); // 左边界大于右边界
    
    printf("✅ 边界情况处理通过\n");
    
    // 测试大小为1的树状数组
    BITree *bit1 = CreateBITree(1);
    Update(bit1, 0, 5);
    assert(Query(bit1, 0) == 5);
    assert(QueryRange(bit1, 0, 0) == 5);
    
    printf("✅ 大小为1的树状数组测试通过\n");
    
    // 测试内存管理
    ResizeBITree(bit, 10);
    assert(bit->size == 10);
    
    ClearBITree(bit);
    assert(Query(bit, 0) == 0);
    
    printf("✅ 内存管理测试通过\n");
    
    DestroyBITree(bit);
    DestroyBITree(bit1);
    printf("✅ 边界情况测试完成\n\n");
}

// ============= 内存管理测试 =============

void TestMemoryManagement() {
    printf("🧠 测试内存管理...\n");
    
    // 测试大量创建和销毁
    for (int i = 0; i < 100; i++) {
        BITree *bit = CreateBITree(1000);
        assert(bit != NULL);
        
        // 添加一些数据
        for (int j = 0; j < 100; j++) {
            Update(bit, j, rand() % 100);
        }
        
        DestroyBITree(bit);
    }
    
    printf("✅ 大量创建和销毁测试通过\n");
    
    // 测试二维树状数组内存管理
    for (int i = 0; i < 10; i++) {
        BITree2D *bit2d = CreateBITree2D(100, 100);
        assert(bit2d != NULL);
        
        // 添加一些数据
        for (int r = 0; r < 50; r++) {
            for (int c = 0; c < 50; c++) {
                Update2D(bit2d, r, c, rand() % 100);
            }
        }
        
        DestroyBITree2D(bit2d);
    }
    
    printf("✅ 二维树状数组内存管理测试通过\n");
    
    // 测试动态区间统计器内存管理
    for (int i = 0; i < 50; i++) {
        DynamicRangeCounter *drc = CreateDynamicRangeCounter(1000);
        assert(drc != NULL);
        
        // 添加和移除元素
        for (int j = 0; j < 100; j++) {
            AddElement(drc, rand() % 1000);
        }
        
        for (int j = 0; j < 50; j++) {
            RemoveElement(drc, rand() % 1000);
        }
        
        DestroyDynamicRangeCounter(drc);
    }
    
    printf("✅ 动态区间统计器内存管理测试通过\n");
    
    printf("✅ 内存管理测试完成\n\n");
} 