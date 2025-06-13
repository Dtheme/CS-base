# Makefile for   栈和队列实现
# create by: zw.duan

# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

# 目标文件和可执行文件
STACK_TARGET = test_stack
QUEUE_TARGET = test_queue
OLD_TARGET = test_stack_queue

# 源文件
STACK_SOURCES = stack.c test_stack.c
QUEUE_SOURCES = queue.c test_queue.c
OLD_SOURCES = stack.c queue.c test_stack_queue.c

# 目标文件
STACK_OBJECTS = $(STACK_SOURCES:.c=.o)
QUEUE_OBJECTS = $(QUEUE_SOURCES:.c=.o)
OLD_OBJECTS = $(OLD_SOURCES:.c=.o)

# 头文件
HEADERS = stack.h queue.h

# 默认目标
.PHONY: all clean help stack queue old install

all: $(STACK_TARGET) $(QUEUE_TARGET)

# 编译栈测试程序
$(STACK_TARGET): stack.o test_stack.o
	@echo "正在链接栈测试程序..."
	$(CC) $(CFLAGS) -o $@ stack.o test_stack.o $(LDFLAGS)
	@echo "✓ 栈测试程序编译完成: $@"

# 编译队列测试程序
$(QUEUE_TARGET): queue.o test_queue.o
	@echo "正在链接队列测试程序..."
	$(CC) $(CFLAGS) -o $@ queue.o test_queue.o $(LDFLAGS)
	@echo "✓ 队列测试程序编译完成: $@"

# 编译旧版本测试程序（兼容性）
$(OLD_TARGET): $(OLD_OBJECTS)
	@echo "正在链接旧版测试程序..."
	$(CC) $(CFLAGS) -o $@ $(OLD_OBJECTS) $(LDFLAGS)
	@echo "✓ 旧版测试程序编译完成: $@"

# 编译源文件为目标文件
stack.o: stack.c stack.h
	@echo "正在编译 stack.c..."
	$(CC) $(CFLAGS) -c stack.c

queue.o: queue.c queue.h
	@echo "正在编译 queue.c..."
	$(CC) $(CFLAGS) -c queue.c

test_stack.o: test_stack.c stack.h
	@echo "正在编译 test_stack.c..."
	$(CC) $(CFLAGS) -c test_stack.c

test_queue.o: test_queue.c queue.h
	@echo "正在编译 test_queue.c..."
	$(CC) $(CFLAGS) -c test_queue.c

test_stack_queue.o: test_stack_queue.c stack.h queue.h
	@echo "正在编译 test_stack_queue.c..."
	$(CC) $(CFLAGS) -c test_stack_queue.c

# 快捷编译目标
stack: $(STACK_TARGET)
	@echo "栈测试程序已就绪，运行命令: ./$(STACK_TARGET)"

queue: $(QUEUE_TARGET)
	@echo "队列测试程序已就绪，运行命令: ./$(QUEUE_TARGET)"

old: $(OLD_TARGET)
	@echo "旧版测试程序已就绪，运行命令: ./$(OLD_TARGET)"

# 运行测试
.PHONY: run-stack run-queue run-old test

run-stack: $(STACK_TARGET)
	@echo "运行栈测试程序..."
	./$(STACK_TARGET)

run-queue: $(QUEUE_TARGET)
	@echo "运行队列测试程序..."
	./$(QUEUE_TARGET)

run-old: $(OLD_TARGET)
	@echo "运行旧版测试程序..."
	./$(OLD_TARGET)

test: all
	@echo "执行全部测试..."
	@echo "==================== 栈测试 ===================="
	./$(STACK_TARGET) || true
	@echo "=================== 队列测试 ==================="
	./$(QUEUE_TARGET) || true
	@echo "=================== 测试完成 ==================="

# 代码质量检查
.PHONY: check lint

check: all
	@echo "检查可执行文件..."
	@if [ -f $(STACK_TARGET) ]; then echo "✓ 栈测试程序: $(STACK_TARGET)"; else echo "✗ 栈测试程序编译失败"; fi
	@if [ -f $(QUEUE_TARGET) ]; then echo "✓ 队列测试程序: $(QUEUE_TARGET)"; else echo "✗ 队列测试程序编译失败"; fi

# 使用cppcheck进行静态分析（如果安装了的话）
lint:
	@echo "执行代码质量检查..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "使用 cppcheck 进行静态分析..."; \
		cppcheck --enable=all --std=c99 *.c *.h; \
	else \
		echo "cppcheck 未安装，跳过静态分析"; \
	fi

# 调试版本
.PHONY: debug

debug: CFLAGS += -DDEBUG -O0
debug: clean all
	@echo "调试版本编译完成"

# 发布版本
.PHONY: release

release: CFLAGS += -DNDEBUG -O3
release: clean all
	@echo "发布版本编译完成"

# 安装程序到系统目录（可选）
install: all
	@echo "安装程序到 /usr/local/bin （需要root权限）..."
	sudo cp $(STACK_TARGET) /usr/local/bin/
	sudo cp $(QUEUE_TARGET) /usr/local/bin/
	@echo "安装完成"

# 打包项目
.PHONY: package

package: clean
	@echo "创建项目包..."
	tar -czf stack_queue_408.tar.gz *.c *.h Makefile README.md
	@echo "项目包创建完成: stack_queue_408.tar.gz"

# 清理编译文件
clean:
	@echo "清理编译文件..."
	rm -f *.o $(STACK_TARGET) $(QUEUE_TARGET) $(OLD_TARGET) core
	rm -f stack_queue_408.tar.gz
	@echo "清理完成"

# 深度清理（包括备份文件）
.PHONY: distclean

distclean: clean
	@echo "深度清理..."
	rm -f *~ *.bak *.swp
	@echo "深度清理完成"

# 显示帮助信息
help:
	@echo "  栈和队列 Makefile 使用说明"
	@echo "=================================="
	@echo "编译目标:"
	@echo "  all        - 编译所有程序（默认）"
	@echo "  stack      - 只编译栈测试程序"
	@echo "  queue      - 只编译队列测试程序"
	@echo "  old        - 编译旧版本测试程序"
	@echo ""
	@echo "运行目标:"
	@echo "  run-stack  - 运行栈测试程序"
	@echo "  run-queue  - 运行队列测试程序"
	@echo "  run-old    - 运行旧版测试程序"
	@echo "  test       - 运行所有测试程序"
	@echo ""
	@echo "构建版本:"
	@echo "  debug      - 编译调试版本"
	@echo "  release    - 编译发布版本"
	@echo ""
	@echo "代码质量:"
	@echo "  check      - 检查编译结果"
	@echo "  lint       - 代码静态分析"
	@echo ""
	@echo "维护目标:"
	@echo "  clean      - 清理编译文件"
	@echo "  distclean  - 深度清理"
	@echo "  package    - 创建项目包"
	@echo "  install    - 安装到系统"
	@echo "  help       - 显示此帮助信息"
	@echo ""
	@echo "使用示例:"
	@echo "  make stack && ./test_stack    # 编译并运行栈测试"
	@echo "  make queue && ./test_queue    # 编译并运行队列测试"
	@echo "  make test                     # 运行全部测试"

# 防止文件名冲突
.SUFFIXES:
.SUFFIXES: .c .o

# 伪目标声明
.PHONY: all clean help stack queue old install run-stack run-queue run-old test check lint debug release package distclean