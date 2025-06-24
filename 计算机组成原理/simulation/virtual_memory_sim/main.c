#include <stdio.h>
#include <stdlib.h>
#include "virtual_memory.h"

int main(void) {
    // 忽略命令行参数
    printf("========== 虚拟存储系统模拟器 ==========\n");
    printf("版本: 1.0\n");
    printf("适用: 408 - 计算机组成原理\n");
    printf("功能: 虚拟存储管理、页面置换算法仿真\n");
    printf("\n");
    
    printf("支持特性:\n");
    printf("- 分页式虚拟存储管理\n");
    printf("- FIFO、LRU、OPT、Clock页面置换算法\n");
    printf("- TLB快表加速机制\n");
    printf("- 多级页表地址转换\n");
    printf("- 详细的性能统计和分析\n");
    printf("- 408真题场景测试\n");
    printf("\n");
    
    printf("使用方法:\n");
    printf("1. 运行测试用例: make test-memory\n");
    printf("2. 查看代码文档: docs/9.存储器管理与虚拟存储.md\n");
    printf("3. 集成到项目: #include \"virtual_memory.h\"\n");
    printf("\n");
    
    printf("注意: 所有功能演示和测试都在测试用例中完成\n");
    printf("======================================\n");
    
    return 0;
} 