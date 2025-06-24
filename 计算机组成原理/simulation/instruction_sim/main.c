#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

int main(void) {
    // 指令系统模拟器启动界面
    printf("========== 指令系统架构模拟器 ==========\n");
    printf("版本: 2.0 Professional Edition\n");
    printf("用途: 计算机组成原理教学与研究\n");
    printf("功能: 处理器指令集架构全功能仿真\n");
    printf("\n");
    
    printf("核心特性:\n");
    printf("- 多格式指令支持: 16位/32位RISC指令集\n");
    printf("- 完整寻址方式: 8种主流寻址模式\n");
    printf("- 微架构仿真: 周期精确的流水线模拟\n");
    printf("- 性能分析: 详细的执行统计和瓶颈分析\n");
    printf("- 硬件建模: 真实处理器行为的精确建模\n");
    printf("\n");
    
    printf("应用场景:\n");
    printf("1. 计算机组成原理课程教学演示\n");
    printf("2. 处理器设计验证和性能分析\n");
    printf("3. 指令集优化和编译器后端开发\n");
    printf("4. 嵌入式系统软硬件协同设计\n");
    printf("\n");
    
    printf("使用指南:\n");
    printf("• 运行测试套件: make test\n");
    printf("• 查看技术文档: docs/architecture.md\n");
    printf("• 集成开发: #include \"instruction.h\"\n");
    printf("• 性能分析: 使用内置统计工具\n");
    printf("\n");
    
    printf("注意: 完整的演示和测试在测试用例中提供\n");
    printf("技术支持: 详见代码文档和注释说明\n");
    printf("=====================================\n");
    
    return 0;
}
