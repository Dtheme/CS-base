/* test_huffman.c - 哈夫曼树完整功能测试程序
 * create by: zw.duan
 *   第3章 树和二叉树
 */

#include "huffman.h"
#include <time.h>
#include <assert.h>

void PrintTestHeader(const char *testName) {
    printf("\n==================================================\n");
    printf("测试项目: %s\n", testName);
    printf("==================================================\n");
}

void PrintTestResult(const char *testName, bool passed) {
    printf("[%s] %s\n", passed ? "通过" : "失败", testName);
}

void TestHuffmanBasicOperations() {
    PrintTestHeader("哈夫曼树基本操作测试");
    
    // 测试节点创建
    HuffmanNode *node1 = CreateHuffmanNode('A', 5, true);
    HuffmanNode *node2 = CreateHuffmanNode('B', 3, true);
    assert(node1 != NULL && node2 != NULL);
    assert(node1->data == 'A' && node1->weight == 5);
    assert(node2->data == 'B' && node2->weight == 3);
    PrintTestResult("节点创建测试", true);
    
    // 测试内部节点创建
    HuffmanNode *internal = CreateInternalNode(node1, node2);
    assert(internal != NULL);
    assert(internal->weight == 8);
    assert(internal->left == node1 && internal->right == node2);
    assert(!internal->isLeaf);
    PrintTestResult("内部节点创建测试", true);
    
    // 测试树初始化
    HuffmanTree tree;
    assert(InitHuffmanTree(&tree) == true);
    assert(tree.root == NULL);
    assert(tree.nodeCount == 0);
    assert(tree.leafCount == 0);
    PrintTestResult("树初始化测试", true);
    
    // 清理
    DestroyHuffmanNode(internal);
    DestroyHuffmanTree(&tree);
    PrintTestResult("内存清理测试", true);
}

void TestCharacterFrequencyAnalysis() {
    PrintTestHeader("字符频率分析测试");
    
    const char *testText = "aabbcccddddeeeee";
    int count;
    CharFrequency *frequencies = CountCharacterFrequencies(testText, &count);
    
    assert(frequencies != NULL);
    assert(count == 5);  // a, b, c, d, e
    
    printf("测试文本: \"%s\"\n", testText);
    PrintCharacterFrequencies(frequencies, count);
    
    // 验证频率统计正确性
    bool foundA = false, foundE = false;
    for (int i = 0; i < count; i++) {
        if (frequencies[i].character == 'a') {
            assert(frequencies[i].frequency == 2);
            foundA = true;
        }
        if (frequencies[i].character == 'e') {
            assert(frequencies[i].frequency == 5);
            foundE = true;
        }
    }
    assert(foundA && foundE);
    
    PrintTestResult("字符频率统计", true);
    
    // 测试排序
    SortFrequenciesByWeight(frequencies, count);
    printf("\n排序后的频率:\n");
    PrintCharacterFrequencies(frequencies, count);
    
    // 验证排序正确性
    for (int i = 0; i < count - 1; i++) {
        assert(frequencies[i].frequency <= frequencies[i + 1].frequency);
    }
    PrintTestResult("频率排序测试", true);
    
    free(frequencies);
}

void TestHuffmanTreeConstruction() {
    PrintTestHeader("哈夫曼树构建测试");
    
    // 创建测试频率数据
    CharFrequency frequencies[] = {
        {'A', 5}, {'B', 2}, {'C', 3}, {'D', 1}, {'E', 8}
    };
    int count = 5;
    
    printf("输入字符频率:\n");
    PrintCharacterFrequencies(frequencies, count);
    
    // 构建哈夫曼树
    HuffmanTree *tree = BuildHuffmanTree(frequencies, count);
    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(tree->leafCount == 5);
    
    printf("\n构建的哈夫曼树:\n");
    PrintHuffmanTree(tree);
    
    // 计算并验证WPL
    int wpl = CalculateWPL(tree);
    printf("\n带权路径长度(WPL): %d\n", wpl);
    assert(wpl > 0);
    
    PrintTestResult("哈夫曼树构建", true);
    
    // 测试树的深度
    int depth = GetHuffmanTreeDepth(tree);
    printf("树的深度: %d\n", depth);
    assert(depth > 0);
    
    PrintTestResult("树深度计算", true);
    
    DestroyHuffmanTree(tree);
    free(tree);
}

void TestHuffmanCoding() {
    PrintTestHeader("哈夫曼编码测试");
    
    const char *testText = "ABCABCABC";
    printf("测试文本: \"%s\"\n", testText);
    
    // 从字符串构建哈夫曼树
    HuffmanTree *tree = BuildHuffmanTreeFromString(testText);
    assert(tree != NULL);
    
    printf("\n构建的哈夫曼树:\n");
    PrintHuffmanTree(tree);
    
    // 生成编码表
    HuffmanCodeTable codeTable;
    assert(InitHuffmanCodeTable(&codeTable, tree->leafCount) == true);
    
    GenerateHuffmanCodes(tree, &codeTable);
    
    printf("\n生成的编码表:\n");
    PrintHuffmanCodeTable(&codeTable);
    
    PrintTestResult("编码表生成", true);
    
    // 测试编码
    char *encoded = EncodeString(testText, &codeTable);
    assert(encoded != NULL);
    
    printf("\n原始文本: \"%s\"\n", testText);
    printf("编码结果: \"%s\"\n", encoded);
    printf("原始长度: %lu 字符 (%lu 位)\n", strlen(testText), strlen(testText) * 8);
    printf("编码长度: %lu 位\n", strlen(encoded));
    
    double compressionRatio = (double)strlen(encoded) / (strlen(testText) * 8);
    printf("压缩比: %.2f%%\n", compressionRatio * 100);
    
    PrintTestResult("文本编码", true);
    
    // 测试解码
    char *decoded = DecodeString(encoded, tree);
    assert(decoded != NULL);
    
    printf("\n解码结果: \"%s\"\n", decoded);
    assert(strcmp(testText, decoded) == 0);
    
    PrintTestResult("文本解码", true);
    
    // 清理
    free(encoded);
    free(decoded);
    DestroyHuffmanCodeTable(&codeTable);
    DestroyHuffmanTree(tree);
    free(tree);
}

void TestHuffmanEdgeCases() {
    PrintTestHeader("哈夫曼树边界测试");
    
    // 测试单字符情况
    const char *singleChar = "AAAA";
    HuffmanTree *tree1 = BuildHuffmanTreeFromString(singleChar);
    assert(tree1 != NULL);
    assert(tree1->leafCount == 1);
    
    HuffmanCodeTable table1;
    InitHuffmanCodeTable(&table1, 1);
    GenerateHuffmanCodes(tree1, &table1);
    
    printf("单字符测试:\n");
    printf("原文: \"%s\"\n", singleChar);
    PrintHuffmanCodeTable(&table1);
    
    char *encoded1 = EncodeString(singleChar, &table1);
    char *decoded1 = DecodeString(encoded1, tree1);
    assert(strcmp(singleChar, decoded1) == 0);
    
    PrintTestResult("单字符编码", true);
    
    // 清理
    free(encoded1);
    free(decoded1);
    DestroyHuffmanCodeTable(&table1);
    DestroyHuffmanTree(tree1);
    free(tree1);
    
    // 测试空字符串
    HuffmanTree *tree2 = BuildHuffmanTreeFromString("");
    assert(tree2 == NULL);
    
    PrintTestResult("空字符串处理", true);
    
    // 测试两字符情况
    const char *twoChars = "AABB";
    HuffmanTree *tree3 = BuildHuffmanTreeFromString(twoChars);
    assert(tree3 != NULL);
    assert(tree3->leafCount == 2);
    
    HuffmanCodeTable table3;
    InitHuffmanCodeTable(&table3, 2);
    GenerateHuffmanCodes(tree3, &table3);
    
    printf("\n双字符测试:\n");
    printf("原文: \"%s\"\n", twoChars);
    PrintHuffmanCodeTable(&table3);
    
    char *encoded3 = EncodeString(twoChars, &table3);
    char *decoded3 = DecodeString(encoded3, tree3);
    assert(strcmp(twoChars, decoded3) == 0);
    
    PrintTestResult("双字符编码", true);
    
    // 清理
    free(encoded3);
    free(decoded3);
    DestroyHuffmanCodeTable(&table3);
    DestroyHuffmanTree(tree3);
    free(tree3);
}

void TestHuffmanCompressionEfficiency() {
    PrintTestHeader("哈夫曼压缩效率测试");
    
    // 测试不同类型的文本
    const char *texts[] = {
        "AAAAAAAAAA",                              // 高重复性
        "ABCDEFGHIJ",                              // 均匀分布
        "AAAAABBBCCDDEFGHIJKLMNOPQRSTUVWXYZ",      // 混合分布
        "Hello, World! This is a test message.",   // 英文文本
        "The quick brown fox jumps over the lazy dog."  // 经典测试文本
    };
    
    const char *descriptions[] = {
        "高重复性文本",
        "均匀分布文本", 
        "混合分布文本",
        "英文文本1",
        "英文文本2"
    };
    
    int textCount = sizeof(texts) / sizeof(texts[0]);
    
    printf("%-15s %-40s %8s %8s %8s %8s\n", 
           "文本类型", "内容", "原长度", "编码长", "压缩比", "WPL");
    printf("%-15s %-40s %8s %8s %8s %8s\n", 
           "--------", "----", "------", "------", "------", "---");
    
    for (int i = 0; i < textCount; i++) {
        HuffmanTree *tree = BuildHuffmanTreeFromString(texts[i]);
        if (tree == NULL) continue;
        
        HuffmanCodeTable table;
        InitHuffmanCodeTable(&table, tree->leafCount);
        GenerateHuffmanCodes(tree, &table);
        
        char *encoded = EncodeString(texts[i], &table);
        if (encoded != NULL) {
            int originalBits = strlen(texts[i]) * 8;
            int encodedBits = strlen(encoded);
            double ratio = (double)encodedBits / originalBits;
            int wpl = CalculateWPL(tree);
            
            printf("%-15s %-40s %8d %8d %7.2f%% %8d\n",
                   descriptions[i], texts[i], originalBits, encodedBits, 
                   ratio * 100, wpl);
            
            free(encoded);
        }
        
        DestroyHuffmanCodeTable(&table);
        DestroyHuffmanTree(tree);
        free(tree);
    }
    
    PrintTestResult("压缩效率分析", true);
}

void TestPriorityQueue() {
    PrintTestHeader("优先队列测试");
    
    HuffmanPriorityQueue pq;
    assert(InitPriorityQueue(&pq, 10) == true);
    assert(IsQueueEmpty(&pq) == true);
    assert(GetQueueSize(&pq) == 0);
    
    // 创建测试节点
    HuffmanNode *nodes[5];
    int weights[] = {5, 2, 8, 1, 3};
    char chars[] = {'A', 'B', 'C', 'D', 'E'};
    
    for (int i = 0; i < 5; i++) {
        nodes[i] = CreateHuffmanNode(chars[i], weights[i], true);
        assert(EnqueueNode(&pq, nodes[i]) == true);
    }
    
    assert(GetQueueSize(&pq) == 5);
    assert(IsQueueEmpty(&pq) == false);
    
    printf("入队顺序 (权重): ");
    for (int i = 0; i < 5; i++) {
        printf("%c(%d) ", chars[i], weights[i]);
    }
    printf("\n");
    
    printf("出队顺序 (权重): ");
    while (!IsQueueEmpty(&pq)) {
        HuffmanNode *node = DequeueNode(&pq);
        assert(node != NULL);
        printf("%c(%d) ", node->data, node->weight);
        DestroyHuffmanNode(node);
    }
    printf("\n");
    
    DestroyPriorityQueue(&pq);
    PrintTestResult("优先队列操作", true);
}

void TestHuffmanPerformance() {
    PrintTestHeader("哈夫曼树性能测试");
    
    // 生成不同长度的测试文本
    int textLengths[] = {100, 1000, 10000};
    int numTests = sizeof(textLengths) / sizeof(textLengths[0]);
    
    printf("%-10s %-12s %-12s %-12s %-12s\n", 
           "文本长度", "构建时间", "编码时间", "解码时间", "总时间");
    printf("%-10s %-12s %-12s %-12s %-12s\n", 
           "--------", "--------", "--------", "--------", "--------");
    
    for (int i = 0; i < numTests; i++) {
        int length = textLengths[i];
        
        // 生成测试文本
        char *testText = (char *)malloc((length + 1) * sizeof(char));
        srand((unsigned int)time(NULL));
        
        for (int j = 0; j < length; j++) {
            testText[j] = 'A' + (rand() % 10);  // 随机生成A-J的字符
        }
        testText[length] = '\0';
        
        clock_t start, end;
        double buildTime, encodeTime, decodeTime;
        
        // 测试树构建时间
        start = clock();
        HuffmanTree *tree = BuildHuffmanTreeFromString(testText);
        end = clock();
        buildTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        
        if (tree == NULL) {
            free(testText);
            continue;
        }
        
        // 生成编码表
        HuffmanCodeTable table;
        InitHuffmanCodeTable(&table, tree->leafCount);
        GenerateHuffmanCodes(tree, &table);
        
        // 测试编码时间
        start = clock();
        char *encoded = EncodeString(testText, &table);
        end = clock();
        encodeTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        
        // 测试解码时间
        start = clock();
        char *decoded = DecodeString(encoded, tree);
        end = clock();
        decodeTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
        
        double totalTime = buildTime + encodeTime + decodeTime;
        
        printf("%-10d %-12.2f %-12.2f %-12.2f %-12.2f\n",
               length, buildTime, encodeTime, decodeTime, totalTime);
        
        // 验证正确性
        assert(decoded != NULL && strcmp(testText, decoded) == 0);
        
        // 清理
        free(testText);
        free(encoded);
        free(decoded);
        DestroyHuffmanCodeTable(&table);
        DestroyHuffmanTree(tree);
        free(tree);
    }
    
    PrintTestResult("性能测试", true);
}

void TestHuffmanApplications() {
    PrintTestHeader("哈夫曼树应用测试");
    
    // 测试经典例题：根据编码表推导哈夫曼树
    printf(" 经典题目测试:\n");
    printf("已知字符频率: A=5, B=3, C=2, D=1\n");
    
    CharFrequency examFreqs[] = {
        {'D', 1}, {'C', 2}, {'B', 3}, {'A', 5}
    };
    
    HuffmanTree *examTree = BuildHuffmanTree(examFreqs, 4);
    assert(examTree != NULL);
    
    printf("\n构建的哈夫曼树:\n");
    PrintHuffmanTree(examTree);
    
    HuffmanCodeTable examTable;
    InitHuffmanCodeTable(&examTable, 4);
    GenerateHuffmanCodes(examTree, &examTable);
    
    printf("\n生成的编码表:\n");
    PrintHuffmanCodeTable(&examTable);
    
    int wpl = CalculateWPL(examTree);
    printf("\nWPL = %d\n", wpl);
    
    // 计算平均码长
    int totalFreq = 5 + 3 + 2 + 1;
    double avgCodeLength = (double)wpl / totalFreq;
    printf("平均码长 = %.2f\n", avgCodeLength);
    
    // 与等长编码比较
    double fixedCodeLength = 2.0;  // 4个字符需要2位固定编码
    printf("等长编码长度 = %.2f\n", fixedCodeLength);
    printf("编码效率提升 = %.2f%%\n", 
           (fixedCodeLength - avgCodeLength) / fixedCodeLength * 100);
    
    PrintTestResult("经典例题验证", true);
    
    DestroyHuffmanCodeTable(&examTable);
    DestroyHuffmanTree(examTree);
    free(examTree);
}

int main() {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                哈夫曼树测试用例                 ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    TestHuffmanBasicOperations();
    TestCharacterFrequencyAnalysis();
    TestPriorityQueue();
    TestHuffmanTreeConstruction();
    TestHuffmanCoding();
    TestHuffmanEdgeCases();
    TestHuffmanCompressionEfficiency();
    TestHuffmanApplications();
    TestHuffmanPerformance();
    
    printf("\n============================================================\n");
    printf("哈夫曼树完整功能测试完成！\n");
    printf("============================================================\n");
   
    
    return 0;
} 