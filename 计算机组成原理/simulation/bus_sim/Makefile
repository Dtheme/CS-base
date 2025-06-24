# 总线模拟器Makefile
# 计算机组成原理模拟器 - 总线模块

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -g
INCLUDES = -I../common

# 目录设置
BUILD_DIR = ../../build/计算机组成原理
OBJ_DIR =$(BUILD_DIR)/obj/bus_sim
BIN_DIR =$(BUILD_DIR)/bin
LIB_DIR =$(BUILD_DIR)/lib

# 源文件
SOURCES = bus.c
OBJECTS =$(SOURCES:%.c=$(OBJ_DIR)/%.o)

# 测试文件
TEST_SOURCE = test_bus.c
TEST_OBJECT =$(OBJ_DIR)/test_bus.o

# 公共库
COMMON_LIB =$(LIB_DIR)/libcommon.a

# 目标文件
LIBRARY =$(LIB_DIR)/libbus.a
TEST_TARGET =$(BIN_DIR)/test_bus

# 默认目标
all:$(LIBRARY)$(TEST_TARGET)

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
	@echo "创建总线库$@..."
	ar rcs$@$(OBJECTS)

# 构建测试程序
$(TEST_TARGET):$(TEST_OBJECT)$(LIBRARY)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接总线测试程序$@..."
	$(CC)$(CFLAGS)$(TEST_OBJECT) -L$(LIB_DIR) -lbus -lcommon -lm -o$@

# 运行测试
test:$(TEST_TARGET)
	@echo "运行总线测试..."
	$(TEST_TARGET)

# 清理
clean:
	rm -f$(OBJECTS)$(TEST_OBJECT)$(LIBRARY)$(TEST_TARGET)
	rm -rf$(OBJ_DIR)

# 安装
install:$(LIBRARY)
	@echo "安装总线模拟器库..."
	cp bus.h ../../include/ 2>/dev/null || true

# 帮助信息
help:
	@echo "可用目标："
	@echo "  all     - 构建总线库和测试程序"
	@echo "  test    - 运行总线测试"
	@echo "  clean   - 清理构建文件"
	@echo "  install - 安装库文件"
	@echo "  help    - 显示此帮助信息"

# 伪目标
.PHONY: all test clean install help
# 性能测试
benchmark:$(TARGET)
	@echo "运行总线性能基准测试..."
	@echo "=== 总线性能测试 ==="
	./$(TARGET) 2>/dev/null | grep -E "(带宽|利用率|效率|等待时间)"

# 内存检查
memcheck:$(TARGET)
	@echo "运行内存检查..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# 安装到构建目录
install:$(TARGET)$(BUS_LIB)
	@echo "安装总线模拟器到构建目录..."
	@mkdir -p$(BUILD_DIR)
	cp$(TARGET)$(BUILD_DIR)/
	cp$(BUS_LIB)$(BUILD_DIR)/
	cp bus.h$(BUILD_DIR)/

# 清理
clean:
	@echo "清理总线模拟器构建文件..."
	rm -f$(OBJECTS)$(TARGET)$(BUS_LIB)
	rm -f *.o *~ core.*

# 深度清理
distclean: clean
	@echo "深度清理..."
	rm -rf$(BUILD_DIR)/$(TARGET)$(BUILD_DIR)/$(BUS_LIB)$(BUILD_DIR)/bus.h

# 显示帮助
help:
	@echo "总线模拟器 Makefile 使用说明:"
	@echo ""
	@echo "可用目标:"
	@echo "  all        - 编译所有程序 (默认)"
	@echo "  test       - 编译并运行测试程序"
	@echo "  run        - 运行测试程序 (test的别名)"
	@echo "  benchmark  - 运行性能基准测试"
	@echo "  memcheck   - 使用valgrind进行内存检查"
	@echo "  install    - 安装到构建目录"
	@echo "  clean      - 清理构建文件"
	@echo "  distclean  - 深度清理"
	@echo "  help       - 显示此帮助信息"
	@echo ""
	@echo "示例用法:"
	@echo "  make           # 编译程序"
	@echo "  make test      # 运行测试"
	@echo "  make benchmark # 性能测试"
	@echo "  make clean     # 清理文件"

# 文件依赖关系
bus.o: bus.c bus.h$(COMMON_DIR)/types.h$(COMMON_DIR)/debug.h
test_bus.o: test_bus.c bus.h$(COMMON_DIR)/test_framework.h
$(COMMON_DIR)/debug.o:$(COMMON_DIR)/debug.c$(COMMON_DIR)/debug.h
$(COMMON_DIR)/test_framework.o:$(COMMON_DIR)/test_framework.c$(COMMON_DIR)/test_framework.h

# 调试信息
debug-info:
	@echo "=== 构建信息 ==="
	@echo "CC:$(CC)"
	@echo "CFLAGS:$(CFLAGS)"
	@echo "INCLUDES:$(INCLUDES)"
	@echo "SOURCES:$(SOURCES)"
	@echo "OBJECTS:$(OBJECTS)"
	@echo "TARGET:$(TARGET)"
	@echo "BUS_LIB:$(BUS_LIB)" 