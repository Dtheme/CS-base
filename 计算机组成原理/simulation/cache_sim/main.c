#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

int main(void) {
    // 忽略命令行参数
    printf("========== 缓存系统模拟器 ==========\n");
    printf("版本: 1.0\n");
    printf("适用: 40 - 计算机组成原理\n");
    printf("功能: 高速缓存设计、映射方式、替换算法仿真\n");
    printf("\n");
    
    printf("支持特性:\n");
    printf("- 直接映射、组相联、全相联缓存\n");
    printf("- LRU、FIFO、Random替换算法\n");
    printf("- 写回、写通策略\n");
    printf("- 详细的性能统计\n");
    printf("- 考试题型场景测试\n");
    printf("\n");
    
    printf("使用方法:\n");
    printf("1. 运行测试用例: make test-cache\n");
    printf("2. 查看代码文档: docs/3.存储器层次结构.md\n");
    printf("3. 集成到项目: #include \"cache.h\"\n");
    printf("\n");
    
    printf("注意: 所有功能演示和测试都在测试用例中完成\n");
    printf("=====================================\n");
    
    return 0;
}  