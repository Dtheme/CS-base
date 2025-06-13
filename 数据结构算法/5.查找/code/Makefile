# Makefile for 查找算法章节
# create by: zw.duan

# 编译器设置
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -g
LDFLAGS = -lm

# 目录设置
TREE_DIR = ../../3.树和二叉树/code
BUILD_DIR = ../../build
OBJ_DIR = $(BUILD_DIR)/obj/5.查找
BIN_DIR = $(BUILD_DIR)/bin

# 源文件
SEARCH_SOURCES = search.c hashtable.c tree_search.c string_search.c
TEST_SOURCES = test_search.c test_hashtable.c test_string_search.c performance_test.c
ALL_SOURCES = $(SEARCH_SOURCES) $(TEST_SOURCES)

# 目标文件
SEARCH_OBJECTS = $(SEARCH_SOURCES:%.c=$(OBJ_DIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:%.c=$(OBJ_DIR)/%.o)
ALL_OBJECTS = $(SEARCH_OBJECTS) $(TEST_OBJECTS)

# 可执行文件
TARGETS = $(BIN_DIR)/test_search $(BIN_DIR)/test_hashtable $(BIN_DIR)/performance_test \
          $(BIN_DIR)/search_demo $(BIN_DIR)/hash_demo $(BIN_DIR)/test_string_search

# 树结构依赖库
TREE_LIBS = $(BUILD_DIR)/lib/libbst.a $(BUILD_DIR)/lib/librbtree.a \
            $(BUILD_DIR)/lib/libbtree.a $(BUILD_DIR)/lib/libbplustree.a

# 默认目标
.PHONY: all clean test install help trees libs

all: dirs trees $(TARGETS)

# 创建目录
dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/lib

# 构建依赖的树结构
trees:
	@echo "🌳 构建树结构依赖..."
	@cd $(TREE_DIR) && $(MAKE) libs

# 构建静态库
libs: $(BUILD_DIR)/lib/libsearch.a $(BUILD_DIR)/lib/libhashtable.a $(BUILD_DIR)/lib/libstring_search.a

$(BUILD_DIR)/lib/libsearch.a: $(OBJ_DIR)/search.o $(OBJ_DIR)/tree_search.o
	@echo "📚 创建查找算法静态库: $@"
	ar rcs $@ $^

$(BUILD_DIR)/lib/libhashtable.a: $(OBJ_DIR)/hashtable.o
	@echo "📚 创建哈希表静态库: $@"
	ar rcs $@ $^

$(BUILD_DIR)/lib/libstring_search.a: $(OBJ_DIR)/string_search.o
	@echo "📚 创建字符串搜索静态库: $@"
	ar rcs $@ $^

# ============= 编译规则 =============

# 编译对象文件
$(OBJ_DIR)/%.o: %.c
	@echo "🔨 编译: $< → $@"
	$(CC) $(CFLAGS) -I$(TREE_DIR) -c $< -o $@

# ============= 可执行文件构建 =============

# 查找算法综合测试
$(BIN_DIR)/test_search: $(OBJ_DIR)/test_search.o $(BUILD_DIR)/lib/libsearch.a $(TREE_LIBS)
	@echo " 构建查找算法测试程序: $@"
	$(CC) $< -L$(BUILD_DIR)/lib -lsearch -lbst -lrbtree -lbtree -lbplustree $(LDFLAGS) -o $@

# 哈希表测试
$(BIN_DIR)/test_hashtable: $(OBJ_DIR)/test_hashtable.o $(BUILD_DIR)/lib/libhashtable.a
	@echo " 构建哈希表测试程序: $@"
	$(CC) $< -L$(BUILD_DIR)/lib -lhashtable $(LDFLAGS) -o $@

# 性能测试
$(BIN_DIR)/performance_test: $(OBJ_DIR)/performance_test.o $(BUILD_DIR)/lib/libsearch.a $(BUILD_DIR)/lib/libhashtable.a $(TREE_LIBS)
	@echo " 构建性能测试程序: $@"
	$(CC) $< -L$(BUILD_DIR)/lib -lsearch -lhashtable -lbst -lrbtree -lbtree -lbplustree $(LDFLAGS) -o $@

# 查找算法演示
$(BIN_DIR)/search_demo: $(OBJ_DIR)/search_demo.o $(BUILD_DIR)/lib/libsearch.a $(TREE_LIBS)
	@echo " 构建查找算法演示程序: $@"
	$(CC) $< -L$(BUILD_DIR)/lib -lsearch -lbst -lrbtree -lbtree -lbplustree $(LDFLAGS) -o $@

# 哈希表演示
$(BIN_DIR)/hash_demo: $(OBJ_DIR)/hash_demo.o $(BUILD_DIR)/lib/libhashtable.a
	@echo " 构建哈希表演示程序: $@"
	$(CC) $< -L$(BUILD_DIR)/lib -lhashtable $(LDFLAGS) -o $@

# 字符串搜索测试
$(BIN_DIR)/test_string_search: $(OBJ_DIR)/test_string_search.o $(BUILD_DIR)/lib/libstring_search.a
	@echo " 构建字符串搜索测试程序: $@"
	$(CC) $< -L$(BUILD_DIR)/lib -lstring_search $(LDFLAGS) -o $@

# ============= 测试规则 =============

# 运行所有测试
test: all
	@echo "🧪 运行查找算法测试..."
	@echo "  基本查找算法测试："
	$(BIN_DIR)/test_search
	@echo ""
	@echo "🔗 哈希表测试："
	$(BIN_DIR)/test_hashtable
	@echo ""
	@echo " 字符串搜索测试："
	$(BIN_DIR)/test_string_search
	@echo ""
	@echo "⚡ 性能对比测试："
	$(BIN_DIR)/performance_test

# 运行基本测试
test-basic: $(BIN_DIR)/test_search
	@echo "🧪 运行基本查找算法测试..."
	$(BIN_DIR)/test_search

# 简单编译测试（不依赖树结构）
test_search_simple: search.c test_search.c
	@echo "🔨 简单编译查找算法测试..."
	$(CC) $(CFLAGS) search.c test_search.c -o test_search_simple

# 运行简单测试
run_simple: test_search_simple
	@echo "🧪 运行简单查找算法测试..."
	./test_search_simple

# 运行哈希表测试
test-hash: $(BIN_DIR)/test_hashtable
	@echo "🧪 运行哈希表测试..."
	$(BIN_DIR)/test_hashtable

# 运行字符串搜索测试
test-string: $(BIN_DIR)/test_string_search
	@echo "🧪 运行字符串搜索测试..."
	$(BIN_DIR)/test_string_search

# 运行性能测试
test-performance: $(BIN_DIR)/performance_test
	@echo "🧪 运行性能测试..."
	$(BIN_DIR)/performance_test

# 运行演示程序
demo: $(BIN_DIR)/search_demo $(BIN_DIR)/hash_demo
	@echo "🎪 运行查找算法演示..."
	$(BIN_DIR)/search_demo
	@echo ""
	@echo "🎪 运行哈希表演示..."
	$(BIN_DIR)/hash_demo

# ============= 代码验证 =============

# 语法检查
check:
	@echo " 进行代码语法检查..."
	@for file in $(ALL_SOURCES); do \
		if [ -f $$file ]; then \
			echo "检查: $$file"; \
			$(CC) $(CFLAGS) -I$(TREE_DIR) -fsyntax-only $$file; \
		fi \
	done

# 内存检查
memcheck: $(BIN_DIR)/test_search
	@echo "🧠 进行内存泄漏检查..."
	@if command -v valgrind > /dev/null; then \
		valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/test_search; \
	else \
		echo "Valgrind未安装，跳过内存检查"; \
	fi

# ============= 性能分析 =============

# 性能分析
profile: $(BIN_DIR)/performance_test
	@echo "  进行性能分析..."
	@if command -v gprof > /dev/null; then \
		$(CC) $(CFLAGS) -pg $(OBJ_DIR)/performance_test.o -L$(BUILD_DIR)/lib -lsearch -lhashtable $(LDFLAGS) -o $(BIN_DIR)/performance_test_profile; \
		$(BIN_DIR)/performance_test_profile; \
		gprof $(BIN_DIR)/performance_test_profile gmon.out > performance_analysis.txt; \
		echo "性能分析结果保存到 performance_analysis.txt"; \
	else \
		echo "gprof未安装，跳过性能分析"; \
	fi

# 基准测试
benchmark: $(BIN_DIR)/performance_test
	@echo "🏃 运行基准测试..."
	@echo "测试数据规模: 1000, 5000, 10000, 50000"
	$(BIN_DIR)/performance_test benchmark

# ============= 代码统计 =============

# 代码统计
stats:
	@echo "📈 代码统计信息："
	@echo "文件数量："
	@find . -name "*.c" -o -name "*.h" | wc -l
	@echo "代码行数："
	@find . -name "*.c" -o -name "*.h" | xargs wc -l | tail -n 1
	@echo "函数数量："
	@grep -r "^[a-zA-Z_][a-zA-Z0-9_]*\s*(" *.c *.h 2>/dev/null | wc -l

# ============= 清理规则 =============

# 清理对象文件
clean-obj:
	@echo "🧹 清理对象文件..."
	rm -rf $(OBJ_DIR)

# 清理可执行文件
clean-bin:
	@echo "🧹 清理可执行文件..."
	rm -f $(TARGETS)

# 清理库文件
clean-lib:
	@echo "🧹 清理库文件..."
	rm -f $(BUILD_DIR)/lib/libsearch.a $(BUILD_DIR)/lib/libhashtable.a

# 完全清理
clean: clean-obj clean-bin clean-lib
	@echo "🧹 清理完成"

# ============= 安装规则 =============

# 安装到系统
install: all
	@echo "📦 安装查找算法库..."
	@mkdir -p /usr/local/include/search
	@mkdir -p /usr/local/lib
	@cp search.h hashtable.h /usr/local/include/search/
	@cp $(BUILD_DIR)/lib/libsearch.a $(BUILD_DIR)/lib/libhashtable.a /usr/local/lib/
	@echo "安装完成"

# ============= 文档生成 =============

# 生成文档
docs:
	@echo "📚 生成API文档..."
	@if command -v doxygen > /dev/null; then \
		doxygen Doxyfile; \
	else \
		echo "Doxygen未安装，跳过文档生成"; \
	fi

# ============= 调试规则 =============

# 调试版本
debug: CFLAGS += -DDEBUG -O0
debug: all

# GDB调试
gdb: debug
	@echo "🐛 启动GDB调试..."
	gdb $(BIN_DIR)/test_search

# ============= 帮助信息 =============

help:
	@echo "📖 查找算法章节 Makefile 使用说明"
	@echo ""
	@echo " 主要目标："
	@echo "  all          - 构建所有程序和库"
	@echo "  libs         - 构建静态库"
	@echo "  test         - 运行所有测试"
	@echo "  demo         - 运行演示程序"
	@echo "  clean        - 清理所有生成文件"
	@echo ""
	@echo "🧪 测试目标："
	@echo "  test-basic   - 基本查找算法测试"
	@echo "  test-hash    - 哈希表测试"
	@echo "  test-performance - 性能测试"
	@echo ""
	@echo " 代码质量："
	@echo "  check        - 语法检查"
	@echo "  memcheck     - 内存泄漏检查"
	@echo "  profile      - 性能分析"
	@echo "  benchmark    - 基准测试"
	@echo ""
	@echo "  其他功能："
	@echo "  stats        - 代码统计"
	@echo "  docs         - 生成文档"
	@echo "  install      - 安装到系统"
	@echo "  debug        - 调试版本"
	@echo "  gdb          - GDB调试"

# ============= 特殊目标 =============

#  真题演示
exam-demo: $(BIN_DIR)/search_demo
	@echo "🎓  真题演示..."
	$(BIN_DIR)/search_demo exam

# 交互式演示
interactive: $(BIN_DIR)/search_demo
	@echo "🎮 交互式查找算法演示..."
	$(BIN_DIR)/search_demo interactive

# 可视化演示
visual: $(BIN_DIR)/hash_demo
	@echo "👁️ 哈希表可视化演示..."
	$(BIN_DIR)/hash_demo visual

# ============= 依赖关系 =============

# 依赖关系
$(OBJ_DIR)/search.o: search.c search.h $(TREE_DIR)/bst.h $(TREE_DIR)/rbtree.h $(TREE_DIR)/btree.h $(TREE_DIR)/bplustree.h
$(OBJ_DIR)/hashtable.o: hashtable.c hashtable.h
$(OBJ_DIR)/tree_search.o: tree_search.c search.h $(TREE_DIR)/bst.h $(TREE_DIR)/rbtree.h $(TREE_DIR)/btree.h $(TREE_DIR)/bplustree.h
$(OBJ_DIR)/string_search.o: string_search.c search.h
$(OBJ_DIR)/test_search.o: test_search.c search.h
$(OBJ_DIR)/test_hashtable.o: test_hashtable.c hashtable.h
$(OBJ_DIR)/performance_test.o: performance_test.c search.h hashtable.h

.PHONY: all clean test dirs trees libs install help stats docs debug gdb \
        test-basic test-hash test-performance demo exam-demo interactive visual \
        check memcheck profile benchmark clean-obj clean-bin clean-lib

# ================================
# 使用说明：
# make          - 构建所有程序
# make test     - 运行所有测试
# make demo     - 运行演示程序
# make help     - 显示帮助信息
# ================================ 