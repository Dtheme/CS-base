# 缓存模拟器Makefile
# 计算机组成原理模拟器 - 缓存模块

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -g
INCLUDES = -I../common

# 目录设置
BUILD_DIR = ../../build/计算机组成原理
OBJ_DIR =$(BUILD_DIR)/obj/cache_sim
BIN_DIR =$(BUILD_DIR)/bin
LIB_DIR =$(BUILD_DIR)/lib

# 源文件
SOURCES = cache.c
OBJECTS =$(SOURCES:%.c=$(OBJ_DIR)/%.o)

# 公共库
COMMON_LIB =$(LIB_DIR)/libcommon.a

# 目标文件
LIBRARY =$(LIB_DIR)/libcache.a

# 默认目标
all:$(LIBRARY)

# 创建目录
$(OBJ_DIR):
	@mkdir -p$(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p$(BIN_DIR)

$(LIB_DIR):
	@mkdir -p$(LIB_DIR)

# 编译规则
$(OBJ_DIR)/%.o: %.c |$(OBJ_DIR)
	@echo "编译$<..."
	$(CC)$(CFLAGS)$(INCLUDES) -c$< -o$@

# 构建库
$(LIBRARY):$(OBJECTS) |$(LIB_DIR)
	@echo "创建缓存库$@..."
	ar rcs$@$(OBJECTS)

# 清理
clean:
	rm -f$(OBJECTS)$(LIBRARY)
	rm -rf$(OBJ_DIR)

# 安装
install:$(LIBRARY)
	@echo "安装缓存模拟器库..."
	cp cache.h ../../include/ 2>/dev/null || true

# 帮助信息
help:
	@echo "可用目标："
	@echo "  all     - 构建缓存库"
	@echo "  clean   - 清理构建文件"
	@echo "  install - 安装库文件"
	@echo "  help    - 显示此帮助信息"

# 伪目标
.PHONY: all clean install help 
 
 