/* test_framework.c - 测试框架实现
 * 计算机组成原理模拟器 - 公共测试框架
 * 遵循数据结构算法编码标准
 * create by: zw.duan
 */

#include "test_framework.h"
#include <time.h>

/**
 * 测试框架实现
 * 
 * 本模块实现了完整的单元测试框架，支持：
 * 
 * 架构特点：
 * 1. 状态管理 - 全局统计信息跟踪
 * 2. 层次结构 - 套件->用例->断言的三级组织
 * 3. 时间测量 - CPU时钟的精确计时
 * 4. 结果聚合 - 实时统计和最终汇总
 * 5. 可视化输出 - 彩色格式化的结果显示
 * 
 * 数据流：
 * test_init() -> test_start_case() -> 断言执行 -> test_end_case() -> test_summary()
 */

// ==================== 全局变量 ====================

/**
 * 全局测试统计信息
 * 
 * 作用域：整个测试会话
 * 生命周期：从test_init()到test_summary()
 */
static test_stats_t g_test_stats = {0};

/**
 * 测试套件名称缓存
 * 
 * 大小限制：255字符 + null终结符
 * 用途：最终结果报告和标题显示
 */
static char g_suite_name[256] = "";

/**
 * 测试开始时间戳
 * 
 * 测量单位：CPU时钟周期
 * 精度：依赖CLOCKS_PER_SEC常量
 * 用途：计算总执行时间
 */
static clock_t g_start_time = 0;

// ==================== 测试管理函数 ====================

/**
 * 初始化测试套件
 * 
 * 初始化流程：
 * 1. 状态重置：清零所有统计信息
 * 2. 名称设置：保存套件名称或使用默认值
 * 3. 时间记录：记录测试开始时间戳
 * 4. 界面输出：显示格式化的欢迎界面
 * 
 * 界面设计：
 * - 使用Unicode框线字符绘制边框
 * - 自动计算文本居中对齐
 * - 彩色输出增强视觉效果
 * 
 * 安全特性：
 * - 字符串长度边界检查
 * - 强制null终结符
 * - 默认值兜底处理
 */
void test_init(const char *suite_name) {
    // 清零所有统计信息
    memset(&g_test_stats, 0, sizeof(test_stats_t));
    
    // 设置套件名称，安全字符串复制
    if (suite_name) {
        strncpy(g_suite_name, suite_name, sizeof(g_suite_name) - 1);
        g_suite_name[sizeof(g_suite_name) - 1] = '\0';  // 强制null终结
    } else {
        strcpy(g_suite_name, "Test Suite");  // 默认名称
    }
    
    // 记录测试开始时间
    g_start_time = clock();
    
    // 输出格式化的测试套件启动界面
    printf(ANSI_COLOR_GREEN);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                        测试套件启动                          ║\n");
    printf("║                       %s", g_suite_name);
    
    // 动态计算居中对齐所需的空格数
    int name_len = (int)strlen(g_suite_name);
    int padding = 62 - name_len;  // 总宽度62字符，减去名称长度
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf(ANSI_COLOR_RESET);
    
    printf("\n");
}

/**
 * 开始测试用例
 * 
 * 状态转换逻辑：
 * 1. 前置检查：如果已有活跃用例，先结束它
 * 2. 状态设置：标记进入测试用例状态
 * 3. 计数重置：清零当前用例的统计信息
 * 4. 名称保存：设置当前用例名称
 * 
 * 并发安全：
 * - 单线程设计，不考虑并发问题
 * - 状态检查防止嵌套用例
 * 
 * 内存管理：
 * - 字符串边界安全检查
 * - 默认名称兜底处理
 */
void test_start_case(const char *case_name) {
    // 如果已有活跃测试用例，先结束它
    if (g_test_stats.in_test_case) {
        test_end_case();
    }
    
    // 设置测试用例状态
    g_test_stats.in_test_case = true;
    g_test_stats.current_case_tests = 0;     // 重置用例测试计数
    g_test_stats.current_case_passed = 0;    // 重置用例通过计数
    
    // 保存测试用例名称
    if (case_name) {
        strncpy(g_test_stats.current_case_name, case_name, 
                sizeof(g_test_stats.current_case_name) - 1);
        g_test_stats.current_case_name[sizeof(g_test_stats.current_case_name) - 1] = '\0';
    } else {
        strcpy(g_test_stats.current_case_name, "Unknown Test Case");
    }
}

/**
 * 结束测试用例
 * 
 * 结果计算逻辑：
 * 1. 状态检查：确认当前处于测试用例中
 * 2. 成功判定：通过数 == 总测试数
 * 3. 结果输出：彩色显示用例执行结果
 * 4. 状态清理：重置用例状态标志
 * 
 * 输出格式：
 * - 成功：绿色对勾 + 用例名 + 通过比例
 * - 失败：红色叉号 + 用例名 + 通过比例
 * 
 * 返回值设计：
 * - true: 用例完全成功
 * - false: 存在失败测试
 * 
 * 边界情况：
 * - 空用例（0个测试）：视为成功
 * - 重复调用：安全返回true
 */
bool test_end_case(void) {
    // 检查是否处于测试用例中
    if (!g_test_stats.in_test_case) {
        return true;
    }
    
    // 清除测试用例状态
    g_test_stats.in_test_case = false;
    
    // 计算用例是否完全通过
    bool case_passed = (g_test_stats.current_case_passed == g_test_stats.current_case_tests);
    
    // 输出测试用例结果（仅当有测试时）
    if (g_test_stats.current_case_tests > 0) {
        if (case_passed) {
            printf(ANSI_COLOR_GREEN "[PASS] %s: %d/%d 通过" ANSI_COLOR_RESET "\n",
                   g_test_stats.current_case_name,
                   g_test_stats.current_case_passed,
                   g_test_stats.current_case_tests);
        } else {
            printf(ANSI_COLOR_RED "[FAIL] %s: %d/%d 通过" ANSI_COLOR_RESET "\n",
                   g_test_stats.current_case_name,
                   g_test_stats.current_case_passed,
                   g_test_stats.current_case_tests);
        }
    }
    
    return case_passed;
}

/**
 * 记录测试通过
 * 
 * 统计更新：
 * 1. 全局计数：总测试数和通过数各增1
 * 2. 用例计数：当前用例的测试数和通过数各增1
 * 
 * 调用时机：
 * - 断言宏中条件为真时自动调用
 * - 可在测试代码中手动调用
 * 
 * 线程安全：
 * - 简单计数操作，单线程环境下安全
 * - 多线程环境需要额外同步机制
 */
void test_pass(void) {
    // 更新全局统计
    g_test_stats.total_tests++;
    g_test_stats.passed_tests++;
    
    // 更新当前用例统计（如果在用例中）
    if (g_test_stats.in_test_case) {
        g_test_stats.current_case_tests++;
        g_test_stats.current_case_passed++;
    }
}

/**
 * 记录测试失败
 * 
 * 统计更新：
 * 1. 全局计数：总测试数和失败数各增1
 * 2. 用例计数：当前用例的测试数增1（通过数不变）
 * 
 * 参数处理：
 * - message: 失败描述信息
 * - file: 源文件名（用于调试定位）
 * - line: 源代码行号（用于调试定位）
 * 
 * 扩展能力：
 * - 当前版本只进行计数统计
 * - 可扩展为详细的失败日志记录
 * - 可添加错误分类和聚合功能
 * 
 * 参数抑制：
 * - 使用(void)参数避免编译器警告
 * - 为未来扩展保留接口兼容性
 */
void test_fail(const char *message, const char *file, int line) {
    // 更新全局统计
    g_test_stats.total_tests++;
    g_test_stats.failed_tests++;
    
    // 更新当前用例统计（如果在用例中）
    if (g_test_stats.in_test_case) {
        g_test_stats.current_case_tests++;
        // 注意：失败时通过数不增加
    }
    
    // 当前版本不使用详细参数，但保留接口兼容性
    // 未来可扩展为详细的失败日志系统
    (void)message;  // 抑制未使用变量警告
    (void)file;
    (void)line;
}

/**
 * 输出测试结果汇总
 * 
 * 汇总流程：
 * 1. 状态清理：结束任何未完成的测试用例
 * 2. 时间计算：计算总执行时间
 * 3. 统计输出：显示详细的测试统计信息
 * 4. 结果判定：根据失败数量显示最终结果
 * 
 * 时间测量：
 * - 使用CPU时钟差值计算执行时间
 * - 转换为秒单位，保留3位小数
 * - 精度依赖系统CLOCKS_PER_SEC常量
 * 
 * 输出格式：
 * - 分隔线：使用等号字符绘制视觉分隔
 * - 统计表：测试套件、总数、通过、失败、通过率、时间
 * - 颜色编码：成功绿色、失败红色、警告黄色
 * - 通过率：百分比显示，100%为绿色，其他为黄色
 * 
 * 最终结果：
 * - 全部通过：绿色成功消息
 * - 存在失败：红色失败消息 + 失败数量
 */
void test_summary(void) {
    // 确保结束当前测试用例（如果有的话）
    if (g_test_stats.in_test_case) {
        test_end_case();
    }
    
    // 计算总执行时间
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - g_start_time)) / CLOCKS_PER_SEC;
    
    // 输出格式化的汇总分隔线
    printf("\n");
    printf(ANSI_COLOR_CYAN "══════════════════════════════════════════════════════════════\n");
    printf("                          测试结果汇总\n");
    printf("══════════════════════════════════════════════════════════════" ANSI_COLOR_RESET "\n");
    
    // 输出基本统计信息
    printf("测试套件: %s\n", g_suite_name);
    printf("总测试数: %d\n", g_test_stats.total_tests);
    printf("通过数量: " ANSI_COLOR_GREEN "%d" ANSI_COLOR_RESET "\n", g_test_stats.passed_tests);
    printf("失败数量: " ANSI_COLOR_RED "%d" ANSI_COLOR_RESET "\n", g_test_stats.failed_tests);
    
    // 计算并显示通过率
    if (g_test_stats.total_tests > 0) {
        double pass_rate = (double)g_test_stats.passed_tests / g_test_stats.total_tests * 100.0;
        if (pass_rate == 100.0) {
            printf("通过率: " ANSI_COLOR_GREEN "%.1f%%" ANSI_COLOR_RESET "\n", pass_rate);
        } else {
            printf("通过率: " ANSI_COLOR_YELLOW "%.1f%%" ANSI_COLOR_RESET "\n", pass_rate);
        }
    }
    
    // 显示执行时间（精确到毫秒）
    printf("执行时间: %.3f 秒\n", elapsed_time);
    
    // 输出结束分隔线
    printf(ANSI_COLOR_CYAN "══════════════════════════════════════════════════════════════" ANSI_COLOR_RESET "\n");
    
    // 显示最终测试结果
    if (g_test_stats.failed_tests == 0) {
        printf(ANSI_COLOR_GREEN "\n[SUCCESS] 所有测试通过！\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "\n[FAILED] 有 %d 个测试失败\n" ANSI_COLOR_RESET, g_test_stats.failed_tests);
    }
}

/**
 * 获取程序退出码
 * 
 * 退出码约定：
 * - 0: 所有测试成功，程序正常退出
 * - 1: 存在测试失败，程序异常退出
 * 
 * 用途：
 * - Shell脚本中检查测试结果
 * - CI/CD流水线中的成功/失败判定
 * - 自动化测试系统的集成
 * 
 * 调用时机：
 * - 通常在main()函数末尾调用
 * - 作为return语句的返回值
 */
int test_get_exit_code(void) {
    return (g_test_stats.failed_tests == 0) ? 0 : 1;
}

/**
 * 重置测试统计信息
 * 
 * 重置范围：
 * 1. 清零所有统计计数器
 * 2. 重置测试开始时间
 * 3. 清除用例状态标志
 * 
 * 使用场景：
 * - 同一程序中运行多轮测试
 * - 测试框架的重新初始化
 * - 批量测试的中间重置
 * 
 * 注意事项：
 * - 不重置套件名称（需要test_init重新设置）
 * - 清理操作是彻底的，无法恢复
 */
void test_reset_stats(void) {
    memset(&g_test_stats, 0, sizeof(test_stats_t));
    g_start_time = clock();
} 