#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("开始B树调试...\n");
    
    BTree tree;
    printf("初始化B树...\n");
    InitBTree(&tree, 3);
    printf("B树初始化完成\n");
    
    printf("检查空树...\n");
    bool empty = BTreeEmpty(&tree);
    printf("空树检查结果: %s\n", empty ? "是" : "否");
    
    printf("插入一些元素...\n");
    int data[] = {10, 20, 5, 6, 12, 30, 7, 17, 15, 25};
    int size = sizeof(data) / sizeof(data[0]);
    
    for (int i = 0; i < size; i++) {
        printf("插入元素 %d...\n", data[i]);
        bool success = BTreeInsert(&tree, data[i]);
        printf("插入结果: %s\n", success ? "成功" : "失败");
        
        if (i == 2) {  // 插入3个元素后测试状态
            printf("当前树状态:\n");
            printf("高度: %d\n", BTreeHeight(&tree));
            printf("关键字数: %d\n", BTreeKeyCount(&tree));
        }
    }
    
    printf("最终树状态:\n");
    printf("高度: %d\n", BTreeHeight(&tree));
    printf("关键字数: %d\n", BTreeKeyCount(&tree));
    
    printf("测试中序遍历...\n");
    BTreeInOrder(&tree);
    
    printf("测试查找...\n");
    BTreeSearchResult result = BTreeSearch(&tree, 15);
    printf("查找15: %s\n", result.found ? "找到" : "未找到");
    
    printf("清理资源...\n");
    DestroyBTree(&tree);
    
    printf("调试完成\n");
    return 0;
} 