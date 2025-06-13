# Makefile for 第3章：树和二叉树
# create by: zw.duan

# 编译器设置
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -O2
DEBUGFLAGS = -DDEBUG -g -O0
INCLUDES = -I.

# 目录设置
BUILDDIR = ../../../build/3.树和二叉树
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin

# 源文件
SOURCES = bintree.c bst.c rbtree.c heap.c unionfind.c huffman.c btree.c
HEADERS = bintree.h bst.h rbtree.h heap.h unionfind.h huffman.h btree.h bplustree.h  
TEST_SOURCES = test_bintree.c test_bst.c test_rbtree.c test_heap.c test_unionfind.c test_huffman.c test_btree.c

# 目标文件
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:%.c=$(OBJDIR)/%.o)

# 可执行文件
EXECUTABLES = $(BINDIR)/test_bintree $(BINDIR)/test_bst $(BINDIR)/test_rbtree $(BINDIR)/test_heap $(BINDIR)/test_unionfind $(BINDIR)/test_huffman $(BINDIR)/test_btree

# 颜色定义
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m # No Color

# ============= 主要目标 =============

.PHONY: all clean help test run install debug format info

# 默认目标：构建所有测试程序
all: directories $(EXECUTABLES)
	@echo "$(GREEN)✅ 第3章所有程序编译完成！$(NC)"
	@echo "$(CYAN)📁 输出目录: $(BUILDDIR)$(NC)"

# 创建必要的目录
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# ============= 单独编译规则 =============

# 编译二叉树测试程序
$(BINDIR)/test_bintree: $(OBJDIR)/bintree.o $(OBJDIR)/test_bintree.o
	@echo "$(BLUE)🔗 链接二叉树测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_bintree 编译完成$(NC)"

# 编译二叉搜索树测试程序
$(BINDIR)/test_bst: $(OBJDIR)/bst.o $(OBJDIR)/test_bst.o
	@echo "$(BLUE)🔗 链接二叉搜索树测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_bst 编译完成$(NC)"

# 编译红黑树测试程序
$(BINDIR)/test_rbtree: $(OBJDIR)/rbtree.o $(OBJDIR)/test_rbtree.o
	@echo "$(BLUE)🔗 链接红黑树测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_rbtree 编译完成$(NC)"

# 编译堆测试程序
$(BINDIR)/test_heap: $(OBJDIR)/heap.o $(OBJDIR)/test_heap.o
	@echo "$(BLUE)🔗 链接堆测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_heap 编译完成$(NC)"

# 编译并查集测试程序
$(BINDIR)/test_unionfind: $(OBJDIR)/unionfind.o $(OBJDIR)/test_unionfind.o
	@echo "$(BLUE)🔗 链接并查集测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_unionfind 编译完成$(NC)"

# 编译哈夫曼树测试程序
$(BINDIR)/test_huffman: $(OBJDIR)/huffman.o $(OBJDIR)/test_huffman.o
	@echo "$(BLUE)🔗 链接哈夫曼树测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_huffman 编译完成$(NC)"

# 编译B树测试程序
$(BINDIR)/test_btree: $(OBJDIR)/btree.o $(OBJDIR)/test_btree.o
	@echo "$(BLUE)🔗 链接B树测试程序...$(NC)"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "$(GREEN)✅ test_btree 编译完成$(NC)"





# ============= 目标文件编译规则 =============

$(OBJDIR)/%.o: %.c $(HEADERS)
	@echo "$(YELLOW)🔨 编译 $<...$(NC)"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ============= 测试运行 =============

# 运行所有测试
test: all
	@echo "$(PURPLE)🧪 运行所有树数据结构测试...$(NC)"
	@echo "$(CYAN)==================== 二叉树测试 ====================$(NC)"
	@if [ -f $(BINDIR)/test_bintree ]; then $(BINDIR)/test_bintree; fi
	@echo "$(CYAN)================== 二叉搜索树测试 ==================$(NC)"
	@if [ -f $(BINDIR)/test_bst ]; then $(BINDIR)/test_bst; fi
	@echo "$(CYAN)==================== 红黑树测试 ====================$(NC)"
	@if [ -f $(BINDIR)/test_rbtree ]; then $(BINDIR)/test_rbtree; fi
	@echo "$(CYAN)===================== 堆测试 =====================$(NC)"
	@if [ -f $(BINDIR)/test_heap ]; then $(BINDIR)/test_heap; fi
	@echo "$(CYAN)=================== 并查集测试 ====================$(NC)"
	@if [ -f $(BINDIR)/test_unionfind ]; then $(BINDIR)/test_unionfind; fi
	@echo "$(CYAN)================== 哈夫曼树测试 ===================$(NC)"
	@if [ -f $(BINDIR)/test_huffman ]; then $(BINDIR)/test_huffman; fi
	@echo "$(CYAN)==================== B树测试 =====================$(NC)"
	@if [ -f $(BINDIR)/test_btree ]; then $(BINDIR)/test_btree; fi

# 运行单个测试
test-bintree: $(BINDIR)/test_bintree
	@echo "$(PURPLE)🧪 运行二叉树测试...$(NC)"
	$(BINDIR)/test_bintree

test-bst: $(BINDIR)/test_bst
	@echo "$(PURPLE)🧪 运行二叉搜索树测试...$(NC)"
	$(BINDIR)/test_bst

test-rbtree: $(BINDIR)/test_rbtree
	@echo "$(PURPLE)🧪 运行红黑树测试...$(NC)"
	$(BINDIR)/test_rbtree

test-heap: $(BINDIR)/test_heap
	@echo "$(PURPLE)🧪 运行堆测试...$(NC)"
	$(BINDIR)/test_heap

test-unionfind: $(BINDIR)/test_unionfind
	@echo "$(PURPLE)🧪 运行并查集测试...$(NC)"
	$(BINDIR)/test_unionfind

test-huffman: $(BINDIR)/test_huffman
	@echo "$(PURPLE)🧪 运行哈夫曼树测试...$(NC)"
	$(BINDIR)/test_huffman

test-btree: $(BINDIR)/test_btree
	@echo "$(PURPLE)🧪 运行B树测试...$(NC)"
	$(BINDIR)/test_btree

# ============= 调试版本 =============

debug: CFLAGS += $(DEBUGFLAGS)
debug: clean all
	@echo "$(YELLOW)🐛 调试版本编译完成$(NC)"

# 使用gdb调试
gdb-bintree: debug
	gdb $(BINDIR)/test_bintree

gdb-bst: debug
	gdb $(BINDIR)/test_bst

gdb-rbtree: debug
	gdb $(BINDIR)/test_rbtree

gdb-heap: debug
	gdb $(BINDIR)/test_heap

# ============= 内存检查 =============

# 使用valgrind检查内存泄漏
valgrind: all
	@echo "$(PURPLE) 使用Valgrind检查内存...$(NC)"
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all $(BINDIR)/test_bintree; \
	else \
		echo "$(RED)❌ Valgrind未安装$(NC)"; \
	fi

# ============= 性能测试 =============

# 性能基准测试
benchmark: all
	@echo "$(PURPLE)⚡ 运行性能基准测试...$(NC)"
	@echo "测试环境: $$(uname -a)"
	@echo "编译器版本: $$($(CC) --version | head -1)"
	@echo "编译选项: $(CFLAGS)"
	@echo "开始性能测试..."
	@for exec in $(EXECUTABLES); do \
		if [ -f $$exec ]; then \
			echo "$(CYAN)测试 $$exec$(NC)"; \
			time $$exec >/dev/null; \
		fi; \
	done

# ============= 代码质量 =============

# 代码格式化（如果安装了clang-format）
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		echo "$(BLUE)🎨 格式化代码...$(NC)"; \
		clang-format -i *.c *.h; \
		echo "$(GREEN)✅ 代码格式化完成$(NC)"; \
	else \
		echo "$(YELLOW)⚠️  clang-format未安装，跳过格式化$(NC)"; \
	fi

# 静态代码分析
analyze:
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "$(BLUE) 运行静态代码分析...$(NC)"; \
		cppcheck --enable=all --std=c99 *.c; \
	else \
		echo "$(YELLOW)⚠️  cppcheck未安装，跳过静态分析$(NC)"; \
	fi

# ============= 信息和帮助 =============

# 显示项目信息
info:
	@echo "$(CYAN)==================== 项目信息 ====================$(NC)"
	@echo "📂 项目:   第3章 树和二叉树"
	@echo "👨‍💻 作者: zw.duan"
	@echo "📅 创建: $$(date '+%Y-%m-%d')"
	@echo ""
	@echo "$(BLUE)📋 包含的数据结构:$(NC)"
	@echo "  • 二叉树 (bintree.c/h) - 基本二叉树操作"
	@echo "  • 二叉搜索树 (bst.c/h) - BST的各种操作"
	@echo "  • 红黑树 (rbtree.c/h) - 自平衡二叉搜索树"
	@echo "  • 堆 (heap.c/h) - 最大堆和最小堆"
	@echo "  • 并查集 (unionfind.c/h) - 路径压缩和按秩合并"
	@echo "  • 哈夫曼树 (huffman.c/h) - 最优编码树和数据压缩"
	@echo ""
	@echo "$(BLUE)🏗️  构建信息:$(NC)"
	@echo "  编译器: $(CC)"
	@echo "  编译选项: $(CFLAGS)"
	@echo "  输出目录: $(BUILDDIR)"
	@echo ""
	@echo "$(BLUE)  代码统计:$(NC)"
	@if command -v wc >/dev/null 2>&1; then \
		echo "  源文件数: $$(ls -1 *.c 2>/dev/null | wc -l)"; \
		echo "  头文件数: $$(ls -1 *.h 2>/dev/null | wc -l)"; \
		echo "  总行数: $$(cat *.c *.h 2>/dev/null | wc -l)"; \
	fi

# 显示帮助信息
help:
	@echo "$(CYAN)==================== 使用帮助 ====================$(NC)"
	@echo ""
	@echo "$(GREEN) 主要目标:$(NC)"
	@echo "  all         - 编译所有程序（默认）"
	@echo "  clean       - 清理所有生成的文件"
	@echo "  test        - 运行所有测试程序"
	@echo "  debug       - 编译调试版本"
	@echo "  help        - 显示此帮助信息"
	@echo ""
	@echo "$(GREEN)🧪 测试目标:$(NC)"
	@echo "  test-bintree   - 运行二叉树测试"
	@echo "  test-bst       - 运行二叉搜索树测试"
	@echo "  test-rbtree    - 运行红黑树测试"
	@echo "  test-heap      - 运行堆测试"
	@echo ""
	@echo "$(GREEN)🐛 调试目标:$(NC)"
	@echo "  debug          - 编译调试版本"
	@echo "  gdb-bintree    - 使用GDB调试二叉树"
	@echo "  gdb-bst        - 使用GDB调试二叉搜索树"
	@echo "  gdb-rbtree     - 使用GDB调试红黑树"
	@echo "  gdb-heap       - 使用GDB调试堆"
	@echo "  valgrind       - 使用Valgrind检查内存"
	@echo ""
	@echo "$(GREEN)⚡ 性能和质量:$(NC)"
	@echo "  benchmark      - 运行性能基准测试"
	@echo "  format         - 格式化代码"
	@echo "  analyze        - 静态代码分析"
	@echo ""
	@echo "$(GREEN)ℹ️  信息:$(NC)"
	@echo "  info           - 显示项目信息"

# ============= 清理 =============

clean:
	@echo "$(RED)🧹 清理生成的文件...$(NC)"
	@rm -rf $(BUILDDIR)
	@rm -f *.o *~ core.*
	@echo "$(GREEN)✅ 清理完成$(NC)"

# ============= 安装检查 =============

install:
	@echo "$(BLUE)🔧 检查开发环境...$(NC)"
	@echo "检查必需的工具:"
	@which $(CC) >/dev/null 2>&1 && echo "$(GREEN)✅ $(CC) 已安装$(NC)" || echo "$(RED)❌ $(CC) 未安装$(NC)"
	@which make >/dev/null 2>&1 && echo "$(GREEN)✅ make 已安装$(NC)" || echo "$(RED)❌ make 未安装$(NC)"
	@echo ""
	@echo "检查可选工具:"
	@which gdb >/dev/null 2>&1 && echo "$(GREEN)✅ gdb 已安装$(NC)" || echo "$(YELLOW)⚠️  gdb 未安装（调试功能受限）$(NC)"
	@which valgrind >/dev/null 2>&1 && echo "$(GREEN)✅ valgrind 已安装$(NC)" || echo "$(YELLOW)⚠️  valgrind 未安装（内存检查功能受限）$(NC)"
	@which clang-format >/dev/null 2>&1 && echo "$(GREEN)✅ clang-format 已安装$(NC)" || echo "$(YELLOW)⚠️  clang-format 未安装（代码格式化功能受限）$(NC)"
	@which cppcheck >/dev/null 2>&1 && echo "$(GREEN)✅ cppcheck 已安装$(NC)" || echo "$(YELLOW)⚠️  cppcheck 未安装（静态分析功能受限）$(NC)"

# ============= 依赖关系 =============

# 依赖关系声明
$(OBJDIR)/bintree.o: bintree.c bintree.h
$(OBJDIR)/bst.o: bst.c bst.h
$(OBJDIR)/rbtree.o: rbtree.c rbtree.h
$(OBJDIR)/heap.o: heap.c heap.h
$(OBJDIR)/btree.o: btree.c btree.h
$(OBJDIR)/test_bintree.o: test_bintree.c bintree.h
$(OBJDIR)/test_bst.o: test_bst.c bst.h
$(OBJDIR)/test_rbtree.o: test_rbtree.c rbtree.h
$(OBJDIR)/test_heap.o: test_heap.c heap.h
$(OBJDIR)/test_btree.o: test_btree.c btree.h

# 防止删除中间文件
.PRECIOUS: $(OBJDIR)/%.o

# 声明假目标
.PHONY: directories test test-bintree test-bst test-rbtree test-heap debug \
        gdb-bintree gdb-bst gdb-rbtree gdb-heap valgrind benchmark \
        format analyze info help clean install 