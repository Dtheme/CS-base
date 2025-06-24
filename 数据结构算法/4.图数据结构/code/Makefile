# ====================================================================
# 图数据结构 - Makefile
# 
# 编译和测试图数据结构与算法的实现
# 支持多个编译目标和测试选项
# 
# Author:  数据结构项目组
# ====================================================================

# 编译器配置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2 -pedantic
LDFLAGS = 
INCLUDES = -I.

# 文件定义
HEADERS = graph.h
SOURCES = graph.c graph_algorithms.c graph_advanced.c
OBJECTS =$(SOURCES:.c=.o)
TEST_SOURCES = test_graph.c
TEST_OBJECTS =$(TEST_SOURCES:.c=.o)

# 目标程序
MAIN_TARGET = test_graph
ALL_TARGETS =$(MAIN_TARGET)

# 默认目标
.PHONY: all
all:$(ALL_TARGETS)

# 编译主测试程序
$(MAIN_TARGET):$(OBJECTS)$(TEST_OBJECTS)
	@echo "正在链接图测试程序..."
	$(CC)$(CFLAGS)$(INCLUDES) -o$@$^$(LDFLAGS)
	@echo "图测试程序编译完成:$@"

# 编译对象文件
%.o: %.c$(HEADERS)
	@echo "正在编译:$<"
	$(CC)$(CFLAGS)$(INCLUDES) -c$< -o$@

# 运行测试
.PHONY: test
test:$(MAIN_TARGET)
	@echo "=========================================="
	@echo "        运行图数据结构测试程序"
	@echo "=========================================="
	./$(MAIN_TARGET)

# 快速测试（只运行基本功能）
.PHONY: quick-test
quick-test:$(MAIN_TARGET)
	@echo "=========================================="
	@echo "        运行图数据结构快速测试"
	@echo "=========================================="
	@echo "运行基本操作测试..."
	@echo "q" | ./$(MAIN_TARGET) || true

# 内存检查（如果安装了valgrind）
.PHONY: memcheck
memcheck:$(MAIN_TARGET)
	@echo "=========================================="
	@echo "        内存泄漏检查"
	@echo "=========================================="
	@which valgrind > /dev/null 2>&1 && \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(MAIN_TARGET) || \
		echo "未安装valgrind，跳过内存检查"

# 调试版本
.PHONY: debug
debug: CFLAGS += -DDEBUG -O0
debug: clean$(MAIN_TARGET)
	@echo "调试版本编译完成"

# 发布版本
.PHONY: release
release: CFLAGS += -DNDEBUG -O3
release: clean$(MAIN_TARGET)
	@echo "发布版本编译完成"

# 代码分析
.PHONY: analyze
analyze:
	@echo "=========================================="
	@echo "        代码静态分析"
	@echo "=========================================="
	@which cppcheck > /dev/null 2>&1 && \
		cppcheck --enable=all --std=c99 --suppress=missingIncludeSystem$(SOURCES)$(TEST_SOURCES) || \
		echo "未安装cppcheck，跳过静态分析"

# 代码格式化
.PHONY: format
format:
	@echo "=========================================="
	@echo "        代码格式化"
	@echo "=========================================="
	@which clang-format > /dev/null 2>&1 && \
		clang-format -i -style="{BasedOnStyle: K&R, IndentWidth: 4, TabWidth: 4, UseTab: Never}" \
		$(HEADERS)$(SOURCES)$(TEST_SOURCES) && \
		echo "代码格式化完成" || \
		echo "未安装clang-format，跳过代码格式化"

# 生成依赖关系
.PHONY: deps
deps:
	@echo "=========================================="
	@echo "        生成依赖关系"
	@echo "=========================================="
	$(CC)$(CFLAGS)$(INCLUDES) -MM$(SOURCES)$(TEST_SOURCES) > .depend

# 包含依赖关系（如果存在）
-include .depend

# 清理编译文件
.PHONY: clean
clean:
	@echo "清理编译文件..."
	rm -f$(OBJECTS)$(TEST_OBJECTS)$(ALL_TARGETS)
	rm -f *.o *~ core *.core
	rm -f .depend
	@echo "清理完成"

# 深度清理（包括备份文件等）
.PHONY: distclean
distclean: clean
	@echo "深度清理..."
	rm -f *.bak *.tmp *.log
	rm -rf *.dSYM/
	@echo "深度清理完成"

# 显示项目信息
.PHONY: info
info:
	@echo "=========================================="
	@echo "        图数据结构项目信息"
	@echo "=========================================="
	@echo "项目名称: 图数据结构与算法实现"
	@echo "编译器:$(CC)"
	@echo "编译选项:$(CFLAGS)"
	@echo "头文件:$(HEADERS)"
	@echo "源文件:$(SOURCES)"
	@echo "测试文件:$(TEST_SOURCES)"
	@echo "目标程序:$(ALL_TARGETS)"
	@echo ""
	@echo "可用目标:"
	@echo "  all        - 编译所有程序"
	@echo "  test       - 运行完整测试"
	@echo "  quick-test - 运行快速测试"
	@echo "  debug      - 编译调试版本"
	@echo "  release    - 编译发布版本"
	@echo "  memcheck   - 内存泄漏检查"
	@echo "  analyze    - 静态代码分析"
	@echo "  format     - 代码格式化"
	@echo "  clean      - 清理编译文件"
	@echo "  distclean  - 深度清理"
	@echo "  info       - 显示项目信息"
	@echo "=========================================="

# 安装程序（可选）
.PHONY: install
install:$(MAIN_TARGET)
	@echo "安装程序到系统目录..."
	@mkdir -p$(HOME)/bin
	cp$(MAIN_TARGET)$(HOME)/bin/
	@echo "程序已安装到$(HOME)/bin/"

# 创建代码统计报告
.PHONY: stats
stats:
	@echo "=========================================="
	@echo "        代码统计报告"
	@echo "=========================================="
	@echo "文件统计:"
	@echo "  头文件:$(words$(HEADERS)) 个"
	@echo "  源文件:$(words$(SOURCES)) 个"
	@echo "  测试文件:$(words$(TEST_SOURCES)) 个"
	@echo ""
	@echo "代码行数统计:"
	@wc -l$(HEADERS)$(SOURCES)$(TEST_SOURCES) 2>/dev/null || true
	@echo ""
	@echo "文件大小统计:"
	@ls -lh$(HEADERS)$(SOURCES)$(TEST_SOURCES) 2>/dev/null || true

# 创建示例演示
.PHONY: demo
demo:$(MAIN_TARGET)
	@echo "=========================================="
	@echo "        图算法演示程序"
	@echo "=========================================="
	@echo "运行图数据结构演示..."
	@echo "注意：这是一个交互式演示，请按照提示操作"
	./$(MAIN_TARGET)

# 基准测试
.PHONY: benchmark
benchmark: release
	@echo "=========================================="
	@echo "        性能基准测试"
	@echo "=========================================="
	@echo "运行性能测试..."
	time ./$(MAIN_TARGET) > /dev/null 2>&1 || true
	@echo "基准测试完成"

# 帮助信息
.PHONY: help
help: info

# 错误：目标不存在时的提示
%:
	@echo "错误: 未知目标 '$@'"
	@echo "运行 'make help' 查看可用目标"
	@false

# ====================================================================
# 特殊目标配置
# ====================================================================

# 阻止删除中间文件
.PRECIOUS: %.o

# 设置 SHELL
SHELL = /bin/bash

# 设置默认目标
.DEFAULT_GOAL := all

# ====================================================================
# 编译优化选项
# ====================================================================

# 针对不同情况的优化选项
ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG -O0 -ggdb3
else
    CFLAGS += -DNDEBUG -O2
endif

# 启用更多警告（GCC特定）
ifeq ($(CC), gcc)
    CFLAGS += -Wformat=2 -Wformat-security -Wformat-nonliteral
    CFLAGS += -Wcast-align -Wcast-qual -Wconversion
    CFLAGS += -Wshadow -Wstrict-prototypes -Wmissing-prototypes
endif

# ====================================================================
# 平台特定配置
# ====================================================================

# macOS特定配置
UNAME_S :=$(shell uname -s)
ifeq ($(UNAME_S), Darwin)
    CFLAGS += -Wno-deprecated-declarations
endif

# Linux特定配置
ifeq ($(UNAME_S), Linux)
    CFLAGS += -D_GNU_SOURCE
    LDFLAGS += -lm
endif 