# Makefile for 排序算法 (Sort Algorithms)
# create by: zw.duan

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = 

# 目录设置
BUILD_DIR = ../../build
OBJ_DIR =$(BUILD_DIR)/obj/6.排序算法
BIN_DIR =$(BUILD_DIR)/bin
LIB_DIR =$(BUILD_DIR)/lib

# 源文件
SOURCES = sort.c sort_test.c
HEADERS = sort.h
TEST_SOURCES = test_sort.c
OBJECTS =$(SOURCES:%.c=$(OBJ_DIR)/%.o)
TEST_OBJECTS =$(TEST_SOURCES:%.c=$(OBJ_DIR)/%.o)

# 目标文件
LIBRARY =$(LIB_DIR)/libsort.a
TEST_EXECUTABLE =$(BIN_DIR)/test_sort

# 默认目标
.PHONY: all
all:$(LIBRARY)$(TEST_EXECUTABLE)

# 创建目录
$(OBJ_DIR)$(BIN_DIR)$(LIB_DIR):
	@mkdir -p$@

# 编译目标文件
$(OBJ_DIR)/%.o: %.c$(HEADERS) |$(OBJ_DIR)
	@echo "编译$<"
	@$(CC)$(CFLAGS) -c$< -o$@

# 创建静态库
$(LIBRARY):$(OBJECTS) |$(LIB_DIR)
	@echo "创建静态库$@"
	@ar rcs$@$^

# 创建测试可执行文件
$(TEST_EXECUTABLE):$(TEST_OBJECTS)$(LIBRARY) |$(BIN_DIR)
	@echo "链接测试程序$@"
	@$(CC)$(CFLAGS)$^ -o$@$(LDFLAGS)

# 运行测试
.PHONY: test
test:$(TEST_EXECUTABLE)
	@echo "运行排序算法测试..."
	@$(TEST_EXECUTABLE)

# 快速测试（只测试基本功能）
.PHONY: test-quick
test-quick:$(TEST_EXECUTABLE)
	@echo "运行快速测试..."
	@$(TEST_EXECUTABLE) | head -50

# 性能测试
.PHONY: test-performance
test-performance:$(TEST_EXECUTABLE)
	@echo "运行性能测试..."
	@$(TEST_EXECUTABLE) | grep -A 20 "性能测试"

# 题型演示
.PHONY: test-exam
test-exam:$(TEST_EXECUTABLE)
	@echo "运行408题型演示..."
	@$(TEST_EXECUTABLE) | grep -A 50 "408题型演示"

# 清理
.PHONY: clean
clean:
	@echo "清理排序算法构建文件..."
	@rm -rf$(OBJ_DIR)
	@rm -f$(LIBRARY)
	@rm -f$(TEST_EXECUTABLE)

# 完全清理
.PHONY: distclean
distclean: clean
	@rm -rf$(BUILD_DIR)

# 检查语法
.PHONY: check
check:
	@echo "检查排序算法代码语法..."
	@$(CC)$(CFLAGS) -fsyntax-only$(SOURCES)$(TEST_SOURCES)
	@echo "✅ 语法检查通过"

# 代码统计
.PHONY: stats
stats:
	@echo "排序算法代码统计："
	@echo "源文件数量:$(words$(SOURCES)$(TEST_SOURCES))"
	@echo "头文件数量:$(words$(HEADERS))"
	@wc -l$(SOURCES)$(TEST_SOURCES)$(HEADERS) 2>/dev/null || echo "无法统计行数"

# 安装
.PHONY: install
install:$(LIBRARY)
	@echo "安装排序算法库..."
	@cp$(HEADERS) /usr/local/include/ 2>/dev/null || echo "需要管理员权限安装头文件"
	@cp$(LIBRARY) /usr/local/lib/ 2>/dev/null || echo "需要管理员权限安装库文件"

# 帮助信息
.PHONY: help
help:
	@echo "排序算法 Makefile 使用说明："
	@echo ""
	@echo "构建目标："
	@echo "  all              - 构建库和测试程序（默认）"
	@echo " $(LIBRARY)       - 构建静态库"
	@echo " $(TEST_EXECUTABLE) - 构建测试程序"
	@echo ""
	@echo "测试目标："
	@echo "  test             - 运行完整测试"
	@echo "  test-quick       - 运行快速测试"
	@echo "  test-performance - 运行性能测试"
	@echo "  test-exam        - 运行408真题演示"
	@echo ""
	@echo "维护目标："
	@echo "  clean            - 清理构建文件"
	@echo "  distclean        - 完全清理"
	@echo "  check            - 语法检查"
	@echo "  stats            - 代码统计"
	@echo "  install          - 安装库文件"
	@echo "  help             - 显示此帮助信息"

# 调试信息
.PHONY: debug
debug:
	@echo "调试信息："
	@echo "CC =$(CC)"
	@echo "CFLAGS =$(CFLAGS)"
	@echo "SOURCES =$(SOURCES)"
	@echo "OBJECTS =$(OBJECTS)"
	@echo "LIBRARY =$(LIBRARY)"
	@echo "TEST_EXECUTABLE =$(TEST_EXECUTABLE)"

# 依赖关系
$(OBJ_DIR)/sort.o: sort.c sort.h
$(OBJ_DIR)/sort_test.o: sort_test.c sort.h
$(OBJ_DIR)/test_sort.o: test_sort.c sort.h 