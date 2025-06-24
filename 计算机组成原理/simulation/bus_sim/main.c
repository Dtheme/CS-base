#include <stdio.h>
#include <stdlib.h>
#include "bus.h"

int main(void) {
    // 忽略命令行参数
    printf("========== 总线系统模拟器 ==========\n");
    printf("版本: 1.0\n");
    printf("适用: 计算机组成原理\n");
    printf("功能: 总线架构、仲裁协议、传输控制仿真\n");
    printf("\n");
    
    printf("支持特性:\n");
    printf("- 系统总线、内部总线架构\n");
    printf("- 集中式、分布式仲裁\n");
    printf("- 同步、异步传输协议\n");
    printf("- 总线性能分析\n");
    printf("- 考试题型场景测试\n");
    printf("\n");
    
    printf("使用方法:\n");
    printf("1. 运行测试用例: make test\n");
    printf("2. 查看代码文档: docs/6.总线系统.md\n");
    printf("3. 集成到项目: #include \"bus.h\"\n");
    printf("\n");
    
    printf("注意: 所有功能演示和测试都在测试用例中完成\n");
    printf("==================================\n");
    
    return 0;
} 