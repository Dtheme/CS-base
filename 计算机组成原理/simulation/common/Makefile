# Makefile for Common Module
# 计算机组成原理模拟器 - 公共模块
# create by: zw.duan

# 编译器和标志
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

# 源文件和对象文件
SOURCES = debug.c utils.c test_framework.c
OBJECTS =$(SOURCES:.c=.o)
HEADERS = debug.h utils.h test_framework.h types.h

# 构建目录
BUILD_DIR = ../../build/计算机组成原理
LIB_DIR =$(BUILD_DIR)/lib
INC_DIR =$(BUILD_DIR)/include

# 库文件
LIBRARY =$(LIB_DIR)/libcommon.a

# 默认目标
.PHONY: all clean install help library

all:$(LIBRARY)

# 创建目录
$(LIB_DIR):
	@mkdir -p$(LIB_DIR)

$(INC_DIR):
	@mkdir -p$(INC_DIR)

# 编译对象文件
%.o: %.c
	@echo "编译$<..."
	$(CC)$(CFLAGS) -c$< -o$@

# 构建库
$(LIBRARY):$(OBJECTS) |$(LIB_DIR)
	@echo "创建公共库$@..."
	ar rcs$@$(OBJECTS)

# 库构建快捷方式
library:$(LIBRARY)

# 安装到构建目录
install:$(LIBRARY) |$(INC_DIR)
	@echo "安装公共模块到构建目录..."
	@cp$(HEADERS)$(INC_DIR)/

# 清理
clean:
	@echo "清理公共模块构建文件..."
	@rm -f$(OBJECTS)$(LIBRARY) *~ core.*

# 显示帮助
help:
	@echo "公共模块 Makefile 使用说明:"
	@echo ""
	@echo "可用目标:"
	@echo "  all        - 编译所有公共模块"
	@echo "  install    - 安装到构建目录"
	@echo "  clean      - 清理构建文件"
	@echo "  help       - 显示此帮助信息"

# 文件依赖关系
debug.o: debug.c debug.h types.h
utils.o: utils.c utils.h types.h
test_framework.o: test_framework.c test_framework.h 