# 流水线模拟器Makefile

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -g
INCLUDES = -I../common

# 目录设置
BUILD_DIR = ../../build/计算机组成原理
OBJ_DIR =$(BUILD_DIR)/obj/pipeline_sim
BIN_DIR =$(BUILD_DIR)/bin
LIB_DIR =$(BUILD_DIR)/lib

# 源文件
SOURCES = pipeline.c
OBJECTS =$(SOURCES:%.c=$(OBJ_DIR)/%.o)

# 测试文件
TEST_SOURCES = test_pipeline.c
TEST_OBJECTS =$(TEST_SOURCES:%.c=$(OBJ_DIR)/%.o)

# 主程序文件
MAIN_SOURCES = main.c
MAIN_OBJECTS =$(MAIN_SOURCES:%.c=$(OBJ_DIR)/%.o)

# 公共库
COMMON_LIB =$(LIB_DIR)/libcommon.a

# 目标文件
LIBRARY =$(LIB_DIR)/libpipeline.a
TEST_TARGET =$(BIN_DIR)/test_pipeline_pipeline
MAIN_TARGET =$(BIN_DIR)/pipeline_sim

# 默认目标
all:$(LIBRARY)$(TEST_TARGET)$(MAIN_TARGET)

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
	@echo "创建流水线库$@..."
	ar rcs$@$(OBJECTS)

# 构建测试程序
$(TEST_TARGET):$(TEST_OBJECTS)$(LIBRARY)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接流水线测试程序$@..."
	$(CC)$(CFLAGS)$(TEST_OBJECTS) -L$(LIB_DIR) -lpipeline -lcommon -lm -o$@

# 构建主程序
$(MAIN_TARGET):$(MAIN_OBJECTS)$(LIBRARY)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接流水线模拟器$@..."
	$(CC)$(CFLAGS)$(MAIN_OBJECTS) -L$(LIB_DIR) -lpipeline -lcommon -lm -o$@

# 流水线模拟器快捷方式
pipeline_sim:$(MAIN_TARGET)

# 运行测试
test:$(TEST_TARGET)
	@echo "运行流水线模拟器测试..."
	$(TEST_TARGET)

# 清理
clean:
	rm -f$(OBJECTS)$(TEST_OBJECTS)$(MAIN_OBJECTS)$(LIBRARY)$(TEST_TARGET)$(MAIN_TARGET)
	rm -rf$(OBJ_DIR)

# 安装
install:$(LIBRARY)
	@echo "安装流水线模拟器库..."
	cp pipeline.h ../../include/
	cp$(LIBRARY) ../../lib/

# 帮助信息
help:
	@echo "可用目标："
	@echo "  all     - 构建库和测试程序"
	@echo "  library - 只构建库"
	@echo "  test    - 构建并运行测试"
	@echo "  clean   - 清理构建文件"
	@echo "  install - 安装库文件"
	@echo "  help    - 显示此帮助信息"

# 伪目标
.PHONY: all test clean install help library pipeline_sim

# 库构建的快捷方式
library:$(LIBRARY) 