# 指令模拟器Makefile
# 计算机组成原理模拟器 - 指令模块

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -g
INCLUDES = -I../common

# 目录设置
BUILD_DIR = ../../build/计算机组成原理
OBJ_DIR =$(BUILD_DIR)/obj/instruction_sim
BIN_DIR =$(BUILD_DIR)/bin
LIB_DIR =$(BUILD_DIR)/lib

# 源文件
SOURCES = instruction.c
OBJECTS =$(SOURCES:%.c=$(OBJ_DIR)/%.o)

# 主程序
MAIN_SOURCE = main.c
MAIN_OBJECT =$(OBJ_DIR)/main.o

# 公共库
COMMON_LIB =$(LIB_DIR)/libcommon.a

# 目标文件
LIBRARY =$(LIB_DIR)/libinstruction.a
MAIN_TARGET =$(BIN_DIR)/instruction_sim

# 默认目标
all:$(LIBRARY)$(MAIN_TARGET)

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
	@echo "创建指令库$@..."
	ar rcs$@$(OBJECTS)

# 构建主程序
$(MAIN_TARGET):$(MAIN_OBJECT)$(LIBRARY)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接指令模拟器$@..."
	$(CC)$(CFLAGS)$(MAIN_OBJECT) -L$(LIB_DIR) -linstruction -lcommon -lm -o$@

# 运行程序
run:$(MAIN_TARGET)
	@echo "运行指令模拟器..."
	$(MAIN_TARGET)

# 清理
clean:
	rm -f$(OBJECTS)$(MAIN_OBJECT)$(LIBRARY)$(MAIN_TARGET)
	rm -rf$(OBJ_DIR)

# 安装
install:$(LIBRARY)
	@echo "安装指令模拟器库..."
	cp instruction.h ../../include/ 2>/dev/null || true

# 帮助信息
help:
	@echo "可用目标："
	@echo "  all     - 构建指令库和主程序"
	@echo "  run     - 运行指令模拟器"
	@echo "  clean   - 清理构建文件"
	@echo "  install - 安装库文件"
	@echo "  help    - 显示此帮助信息"

# 伪目标
.PHONY: all run clean install help 
 
 