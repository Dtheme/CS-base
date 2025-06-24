# Makefile for I/O System Simulator
# 计算机组成原理模拟器 - I/O系统模块

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -O2
INCLUDES = -I../common
LDFLAGS = 
LIBS = -lm

# 源文件
SOURCES = io.c device_impl.c
OBJECTS =$(SOURCES:.c=.o)
HEADERS = io.h

# 测试文件
TEST_SOURCES = test_io.c
TEST_OBJECTS =$(TEST_SOURCES:.c=.o)
TEST_TARGET = test_io

# 公共模块
COMMON_DIR = ../common
COMMON_OBJECTS =$(COMMON_DIR)/utils.o$(COMMON_DIR)/debug.o$(COMMON_DIR)/test_framework.o

# 目标文件
TARGET = libio.a

# 默认目标
.PHONY: all clean test help install

all:$(TARGET)

# 静态库
$(TARGET):$(OBJECTS)
	@echo "创建I/O系统静态库..."
	ar rcs$@$^
	@echo "✓ 静态库创建完成:$@"

# 测试程序
$(TEST_TARGET):$(TEST_OBJECTS)$(TARGET)$(COMMON_OBJECTS)
	@echo "链接I/O系统测试程序..."
	$(CC)$(LDFLAGS) -o$@$^$(LIBS)
	@echo "✓ 测试程序构建完成:$@"

# 编译规则
%.o: %.c$(HEADERS)
	@echo "编译$<..."
	$(CC)$(CFLAGS)$(INCLUDES) -c$< -o$@

# 公共模块依赖
$(COMMON_OBJECTS):
	@echo "构建公共模块..."
	$(MAKE) -C$(COMMON_DIR)

# 运行测试
test:$(TEST_TARGET)
	@echo "运行I/O系统测试..."
	./$(TEST_TARGET)

# 安装目标
install:$(TARGET)
	@echo "安装I/O系统库完成"

# 清理
clean:
	@echo "清理I/O系统模块..."
	rm -f$(OBJECTS)$(TEST_OBJECTS)
	rm -f$(TARGET)$(TEST_TARGET)
	@echo "✓ 清理完成"

# 深度清理（包括公共模块）
distclean: clean
	$(MAKE) -C$(COMMON_DIR) clean

# 帮助信息
help:
	@echo "I/O系统模拟器 Makefile"
	@echo ""
	@echo "可用目标:"
	@echo "  all        - 构建所有目标"
	@echo " $(TARGET)     - 构建静态库"
	@echo " $(TEST_TARGET)     - 构建测试程序"
	@echo "  test       - 运行测试"
	@echo "  clean      - 清理构建文件"
	@echo "  distclean  - 深度清理"
	@echo "  install    - 安装库文件"
	@echo "  help       - 显示此帮助"

# 依赖关系
$(OBJECTS):$(HEADERS)
$(TEST_OBJECTS):$(HEADERS) 