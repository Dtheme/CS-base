# ============================================================
#   第1章线性表 构建系统
# 作者: zw.duan
# 版本: 2.0 (参考第2章栈和队列优化标准)
# 日期: 2025年
# ============================================================

# 编译器配置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

# 颜色定义（增强用户体验）
RED    = \033[0;31m
GREEN  = \033[0;32m
YELLOW = \033[0;33m
BLUE   = \033[0;34m
PURPLE = \033[0;35m
CYAN   = \033[0;36m
WHITE  = \033[0;37m
NC     = \033[0m # No Color

# 目标文件定义
SQLIST_TARGET = test_sqlist
LINKLIST_TARGET = test_linklist
CIRCULAR_TARGET = test_circular

# 顺序表相关文件
SQLIST_OBJS = sqlist.o test_sqlist.o
SQLIST_SOURCES = sqlist.c sqlist.h test_sqlist.c

# 链表相关文件  
LINKLIST_OBJS = linklist.o test_linklist.o
LINKLIST_SOURCES = linklist.c linklist.h test_linklist.c

# 循环链表相关文件
CIRCULAR_OBJS = linklist.o test_circular.o
CIRCULAR_SOURCES = linklist.c linklist.h test_circular.c

# 所有目标文件
ALL_TARGETS =$(SQLIST_TARGET)$(LINKLIST_TARGET)$(CIRCULAR_TARGET)
ALL_OBJS =$(SQLIST_OBJS)$(LINKLIST_OBJS) test_circular.o

# ============================================================
# 主要构建目标
# ============================================================

# 默认目标：编译所有程序
all:$(ALL_TARGETS)
	@echo "$(GREEN)✓ 所有线性表程序编译完成！$(NC)"
	@echo "$(CYAN)使用 'make help' 查看可用命令$(NC)"

# 顺序表程序编译
sqlist:$(SQLIST_TARGET)

$(SQLIST_TARGET):$(SQLIST_OBJS)
	@echo "$(YELLOW)正在链接顺序表测试程序...$(NC)"
	@$(CC)$(CFLAGS) -o$(SQLIST_TARGET)$(SQLIST_OBJS)$(LDFLAGS)
	@echo "$(GREEN)✓ 顺序表测试程序编译完成:$(SQLIST_TARGET)$(NC)"
	@echo "$(CYAN)顺序表程序已就绪，运行命令: ./$(SQLIST_TARGET)$(NC)"

# 链表程序编译
linklist:$(LINKLIST_TARGET)

$(LINKLIST_TARGET):$(LINKLIST_OBJS)
	@echo "$(YELLOW)正在链接链表测试程序...$(NC)"
	@$(CC)$(CFLAGS) -o$(LINKLIST_TARGET)$(LINKLIST_OBJS)$(LDFLAGS)
	@echo "$(GREEN)✓ 链表测试程序编译完成:$(LINKLIST_TARGET)$(NC)"
	@echo "$(CYAN)链表程序已就绪，运行命令: ./$(LINKLIST_TARGET)$(NC)"

# 循环链表程序编译
circular:$(CIRCULAR_TARGET)

$(CIRCULAR_TARGET):$(CIRCULAR_OBJS)
	@echo "$(YELLOW)正在链接循环链表测试程序...$(NC)"
	@$(CC)$(CFLAGS) -o$(CIRCULAR_TARGET)$(CIRCULAR_OBJS)$(LDFLAGS)
	@echo "$(GREEN)✓ 循环链表测试程序编译完成:$(CIRCULAR_TARGET)$(NC)"
	@echo "$(CYAN)循环链表程序已就绪，运行命令: ./$(CIRCULAR_TARGET)$(NC)"

# ============================================================
# 单独编译规则
# ============================================================

# 顺序表对象文件
sqlist.o: sqlist.c sqlist.h
	@echo "$(YELLOW)正在编译 sqlist.c...$(NC)"
	@$(CC)$(CFLAGS) -c sqlist.c

test_sqlist.o: test_sqlist.c sqlist.h
	@echo "$(YELLOW)正在编译 test_sqlist.c...$(NC)"
	@$(CC)$(CFLAGS) -c test_sqlist.c

# 链表对象文件
linklist.o: linklist.c linklist.h
	@echo "$(YELLOW)正在编译 linklist.c...$(NC)"
	@$(CC)$(CFLAGS) -c linklist.c

test_linklist.o: test_linklist.c linklist.h
	@echo "$(YELLOW)正在编译 test_linklist.c...$(NC)"
	@$(CC)$(CFLAGS) -c test_linklist.c

# 循环链表对象文件
test_circular.o: test_circular.c linklist.h
	@echo "$(YELLOW)正在编译 test_circular.c...$(NC)"
	@$(CC)$(CFLAGS) -c test_circular.c

# ============================================================
# 测试运行目标
# ============================================================

# 运行所有测试
test: all
	@echo "$(PURPLE)======================================$(NC)"
	@echo "$(PURPLE)    开始运行线性表完整测试套件   $(NC)"
	@echo "$(PURPLE)======================================$(NC)"
	@echo "$(BLUE) 运行顺序表测试...$(NC)"
	@./$(SQLIST_TARGET)
	@echo ""
	@echo "$(BLUE) 运行链表测试...$(NC)"
	@./$(LINKLIST_TARGET)
	@echo ""
	@echo "$(BLUE) 运行循环链表测试...$(NC)"
	@./$(CIRCULAR_TARGET)
	@echo "$(GREEN)✅ 所有线性表测试完成！$(NC)"

# 单独运行顺序表测试
run-sqlist:$(SQLIST_TARGET)
	@echo "$(BLUE) 运行顺序表测试程序...$(NC)"
	@./$(SQLIST_TARGET)

# 单独运行链表测试
run-linklist:$(LINKLIST_TARGET)
	@echo "$(BLUE) 运行链表测试程序...$(NC)"
	@./$(LINKLIST_TARGET)

# 单独运行循环链表测试
run-circular:$(CIRCULAR_TARGET)
	@echo "$(BLUE) 运行循环链表测试程序...$(NC)"
	@./$(CIRCULAR_TARGET)

# ============================================================
# 开发与调试目标
# ============================================================

# 调试模式编译
debug: CFLAGS += -DDEBUG -O0
debug: all
	@echo "$(YELLOW)⚠️  调试模式编译完成$(NC)"

# 发布模式编译  
release: CFLAGS += -DNDEBUG -O3
release: all
	@echo "$(GREEN)🚀 发布模式编译完成$(NC)"

# 代码检查
check: all
	@echo "$(BLUE) 进行代码静态检查...$(NC)"
	@which cppcheck > /dev/null && cppcheck --enable=all --suppress=missingIncludeSystem$(SQLIST_SOURCES)$(LINKLIST_SOURCES)$(CIRCULAR_SOURCES) || echo "$(YELLOW)⚠️  cppcheck未安装，跳过静态检查$(NC)"

# 代码格式化
lint: 
	@echo "$(BLUE)🎨 格式化代码...$(NC)"
	@which clang-format > /dev/null && clang-format -i *.c *.h || echo "$(YELLOW)⚠️  clang-format未安装，跳过格式化$(NC)"

# 内存检查（需要valgrind）
memcheck-sqlist:$(SQLIST_TARGET)
	@echo "$(BLUE) 顺序表内存检查...$(NC)"
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(SQLIST_TARGET) || echo "$(YELLOW)⚠️  valgrind未安装$(NC)"

memcheck-linklist:$(LINKLIST_TARGET)
	@echo "$(BLUE) 链表内存检查...$(NC)"
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(LINKLIST_TARGET) || echo "$(YELLOW)⚠️  valgrind未安装$(NC)"

memcheck-circular:$(CIRCULAR_TARGET)
	@echo "$(BLUE) 循环链表内存检查...$(NC)"
	@which valgrind > /dev/null && valgrind --leak-check=full --show-leak-kinds=all ./$(CIRCULAR_TARGET) || echo "$(YELLOW)⚠️  valgrind未安装$(NC)"

memcheck: memcheck-sqlist memcheck-linklist memcheck-circular

# ============================================================
# 清理和维护目标
# ============================================================

# 清理编译文件
clean:
	@echo "$(YELLOW)🧹 清理编译文件...$(NC)"
	@rm -f$(ALL_OBJS)$(ALL_TARGETS) core
	@rm -f linear_list_408.tar.gz
	@echo "$(GREEN)✓ 清理完成$(NC)"

# 完全重新编译
rebuild: clean all

# ============================================================
# 打包和分发目标
# ============================================================

# 创建项目包
package: clean
	@echo "$(BLUE)📦 创建项目包...$(NC)"
	@tar -czf linear_list_408.tar.gz *.c *.h Makefile README.md 2>/dev/null || tar -czf linear_list_408.tar.gz *.c *.h Makefile
	@echo "$(GREEN)✓ 项目包已创建: linear_list_408.tar.gz$(NC)"

# ============================================================
# 信息和帮助目标  
# ============================================================

# 显示项目信息
info:
	@echo "$(CYAN)╔════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║                第1章线性表                ║$(NC)"
	@echo "$(CYAN)║                          版本：2.0                          ║$(NC)"
	@echo "$(CYAN)║                        日期：2025年                         ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(WHITE)📁 项目文件统计:$(NC)"
	@echo "   源文件:$(words$(wildcard *.c)) 个"
	@echo "   头文件:$(words$(wildcard *.h)) 个"
	@echo "   总代码行数:$$(cat *.c *.h | wc -l) 行"
	@echo ""
	@echo "$(WHITE)  知识点覆盖:$(NC)"
	@echo "   ✓ 顺序表的定义、初始化、插入、删除"
	@echo "   ✓ 动态顺序表的扩容机制"
	@echo "   ✓ 单链表、双链表的建立、遍历、逆转"
	@echo "   ✓ 循环链表及约瑟夫环问题"
	@echo "   ✓ 有序表合并、去重算法"
	@echo "   ✓ 经典算法：删除、查找、集合运算"

# 显示详细帮助信息
help:
	@echo "$(CYAN)╔════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║                    线性表构建系统帮助                        ║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "$(WHITE)📋 构建目标:$(NC)"
	@echo "  $(YELLOW)all$(NC)              - 编译所有程序（默认目标）"
	@echo "  $(YELLOW)sqlist$(NC)           - 只编译顺序表程序"
	@echo "  $(YELLOW)linklist$(NC)         - 只编译链表程序"
	@echo "  $(YELLOW)circular$(NC)         - 只编译循环链表程序"
	@echo ""
	@echo "$(WHITE) 测试目标:$(NC)"
	@echo "  $(YELLOW)test$(NC)             - 运行所有测试程序"
	@echo "  $(YELLOW)run-sqlist$(NC)       - 运行顺序表测试"
	@echo "  $(YELLOW)run-linklist$(NC)     - 运行链表测试"
	@echo "  $(YELLOW)run-circular$(NC)     - 运行循环链表测试"
	@echo ""
	@echo "$(WHITE)🛠️  开发目标:$(NC)"
	@echo "  $(YELLOW)debug$(NC)            - 调试模式编译"
	@echo "  $(YELLOW)release$(NC)          - 发布模式编译"
	@echo "  $(YELLOW)check$(NC)            - 代码静态检查"
	@echo "  $(YELLOW)lint$(NC)             - 代码格式化"
	@echo "  $(YELLOW)memcheck$(NC)         - 内存泄漏检查"
	@echo ""
	@echo "$(WHITE)🧹 维护目标:$(NC)"
	@echo "  $(YELLOW)clean$(NC)            - 清理编译文件"
	@echo "  $(YELLOW)rebuild$(NC)          - 完全重新编译"
	@echo "  $(YELLOW)package$(NC)          - 创建项目包"
	@echo ""
	@echo "$(WHITE)ℹ️  信息目标:$(NC)"
	@echo "  $(YELLOW)info$(NC)             - 显示项目信息"
	@echo "  $(YELLOW)help$(NC)             - 显示此帮助信息"
	@echo ""
	@echo "$(WHITE)💡 使用示例:$(NC)"
	@echo "  $(GREEN)make sqlist && ./test_sqlist$(NC)     # 编译并运行顺序表测试"
	@echo "  $(GREEN)make test$(NC)                        # 运行所有测试"
	@echo "  $(GREEN)make debug run-linklist$(NC)          # 调试模式编译并运行链表测试"
	@echo "  $(GREEN)make circular && ./test_circular$(NC) # 编译并运行循环链表测试"

# ============================================================
# 特殊目标声明
# ============================================================

# 声明伪目标
.PHONY: all sqlist linklist circular test run-sqlist run-linklist run-circular debug release check lint
.PHONY: memcheck memcheck-sqlist memcheck-linklist clean rebuild package info help

# 禁止删除中间文件
.PRECIOUS:$(ALL_OBJS)

# 默认目标
.DEFAULT_GOAL := all 