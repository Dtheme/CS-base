# 计算机组成原理模块 Makefile

# =============================================================================
# 基本配置
# =============================================================================

# 编译器配置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
CFLAGS_DEBUG = -Wall -Wextra -std=c99 -g -O0 -DDEBUG
CFLAGS_RELEASE = -Wall -Wextra -std=c99 -O3 -DNDEBUG

# 目录配置
MODULE_NAME = 计算机组成原理
BUILD_DIR = ../build/$(MODULE_NAME)
SRC_DIR = simulation
DOCS_DIR = docs
EXAMPLES_DIR = examples
TESTS_DIR = tests

# 包含目录
INCLUDES = -I$(SRC_DIR)/common

# 子模块目录
CPU_SIM_DIR =$(SRC_DIR)/cpu_sim
CACHE_SIM_DIR =$(SRC_DIR)/cache_sim
INSTRUCTION_SIM_DIR =$(SRC_DIR)/instruction_sim
PIPELINE_SIM_DIR =$(SRC_DIR)/pipeline_sim
# MEMORY_SIM_DIR =$(SRC_DIR)/memory_sim  # 已移除，使用virtual_memory_sim替代
VIRTUAL_MEMORY_SIM_DIR =$(SRC_DIR)/virtual_memory_sim
BUS_SIM_DIR =$(SRC_DIR)/bus_sim
IO_SIM_DIR =$(SRC_DIR)/io_sim
COMMON_DIR =$(SRC_DIR)/common

# 构建目录
BIN_DIR =$(BUILD_DIR)/bin
OBJ_DIR =$(BUILD_DIR)/obj
LIB_DIR =$(BUILD_DIR)/lib
TEST_DIR =$(BUILD_DIR)/test

# =============================================================================
# 源文件和目标文件
# =============================================================================

# 公共组件源文件
COMMON_SOURCES =$(wildcard$(COMMON_DIR)/*.c)
COMMON_OBJECTS =$(COMMON_SOURCES:$(COMMON_DIR)/%.c=$(OBJ_DIR)/common/%.o)

# CPU模拟器源文件
CPU_SOURCES =$(wildcard$(CPU_SIM_DIR)/*.c)
CPU_OBJECTS =$(CPU_SOURCES:$(CPU_SIM_DIR)/%.c=$(OBJ_DIR)/cpu_sim/%.o)
CPU_TESTS =$(filter$(CPU_SIM_DIR)/test_%.c,$(CPU_SOURCES))
CPU_LIB_SOURCES =$(filter-out$(CPU_TESTS),$(CPU_SOURCES))
CPU_LIB_OBJECTS =$(CPU_LIB_SOURCES:$(CPU_SIM_DIR)/%.c=$(OBJ_DIR)/cpu_sim/%.o)

# 缓存模拟器源文件
CACHE_SOURCES =$(wildcard$(CACHE_SIM_DIR)/*.c)
CACHE_OBJECTS =$(CACHE_SOURCES:$(CACHE_SIM_DIR)/%.c=$(OBJ_DIR)/cache_sim/%.o)
CACHE_TESTS =$(filter$(CACHE_SIM_DIR)/test_%.c,$(CACHE_SOURCES))
CACHE_LIB_SOURCES =$(filter-out$(CACHE_TESTS),$(CACHE_SOURCES))
CACHE_LIB_OBJECTS =$(CACHE_LIB_SOURCES:$(CACHE_SIM_DIR)/%.c=$(OBJ_DIR)/cache_sim/%.o)

# 指令系统模拟器源文件
INSTRUCTION_SOURCES =$(wildcard$(INSTRUCTION_SIM_DIR)/*.c)
INSTRUCTION_OBJECTS =$(INSTRUCTION_SOURCES:$(INSTRUCTION_SIM_DIR)/%.c=$(OBJ_DIR)/instruction_sim/%.o)
INSTRUCTION_TESTS =$(filter$(INSTRUCTION_SIM_DIR)/test_%.c,$(INSTRUCTION_SOURCES))
INSTRUCTION_LIB_SOURCES =$(filter-out$(INSTRUCTION_TESTS)$(INSTRUCTION_SIM_DIR)/main.c,$(INSTRUCTION_SOURCES))
INSTRUCTION_LIB_OBJECTS =$(INSTRUCTION_LIB_SOURCES:$(INSTRUCTION_SIM_DIR)/%.c=$(OBJ_DIR)/instruction_sim/%.o)

# 流水线模拟器源文件
PIPELINE_SOURCES =$(wildcard$(PIPELINE_SIM_DIR)/*.c)
PIPELINE_OBJECTS =$(PIPELINE_SOURCES:$(PIPELINE_SIM_DIR)/%.c=$(OBJ_DIR)/pipeline_sim/%.o)
PIPELINE_TESTS =$(filter$(PIPELINE_SIM_DIR)/test_%.c,$(PIPELINE_SOURCES))
PIPELINE_LIB_SOURCES =$(filter-out$(PIPELINE_TESTS),$(PIPELINE_SOURCES))
PIPELINE_LIB_OBJECTS =$(PIPELINE_LIB_SOURCES:$(PIPELINE_SIM_DIR)/%.c=$(OBJ_DIR)/pipeline_sim/%.o)

# 内存模拟器源文件 - 已移除，使用virtual_memory_sim替代
# MEMORY_SOURCES =$(wildcard$(MEMORY_SIM_DIR)/*.c)
# MEMORY_OBJECTS =$(MEMORY_SOURCES:$(MEMORY_SIM_DIR)/%.c=$(OBJ_DIR)/memory_sim/%.o)
# MEMORY_TESTS =$(filter$(MEMORY_SIM_DIR)/test_%.c,$(MEMORY_SOURCES))
# MEMORY_LIB_SOURCES =$(filter-out$(MEMORY_TESTS),$(MEMORY_SOURCES))
# MEMORY_LIB_OBJECTS =$(MEMORY_LIB_SOURCES:$(MEMORY_SIM_DIR)/%.c=$(OBJ_DIR)/memory_sim/%.o)

# 总线模拟器源文件
BUS_SOURCES =$(wildcard$(BUS_SIM_DIR)/*.c)
BUS_OBJECTS =$(BUS_SOURCES:$(BUS_SIM_DIR)/%.c=$(OBJ_DIR)/bus_sim/%.o)
BUS_TESTS =$(filter$(BUS_SIM_DIR)/test_%.c,$(BUS_SOURCES))
BUS_LIB_SOURCES =$(filter-out$(BUS_TESTS),$(BUS_SOURCES))
BUS_LIB_OBJECTS =$(BUS_LIB_SOURCES:$(BUS_SIM_DIR)/%.c=$(OBJ_DIR)/bus_sim/%.o)

# I/O模拟器源文件
IO_SOURCES =$(wildcard$(IO_SIM_DIR)/*.c)
IO_OBJECTS =$(IO_SOURCES:$(IO_SIM_DIR)/%.c=$(OBJ_DIR)/io_sim/%.o)
IO_TESTS =$(filter$(IO_SIM_DIR)/test_%.c,$(IO_SOURCES))
IO_LIB_SOURCES =$(filter-out$(IO_TESTS),$(IO_SOURCES))
IO_LIB_OBJECTS =$(IO_LIB_SOURCES:$(IO_SIM_DIR)/%.c=$(OBJ_DIR)/io_sim/%.o)

# 虚拟存储器模拟器源文件
VIRTUAL_MEMORY_SOURCES =$(wildcard$(VIRTUAL_MEMORY_SIM_DIR)/*.c)
VIRTUAL_MEMORY_OBJECTS =$(VIRTUAL_MEMORY_SOURCES:$(VIRTUAL_MEMORY_SIM_DIR)/%.c=$(OBJ_DIR)/virtual_memory_sim/%.o)
VIRTUAL_MEMORY_TESTS =$(filter$(VIRTUAL_MEMORY_SIM_DIR)/test_%.c,$(VIRTUAL_MEMORY_SOURCES))
VIRTUAL_MEMORY_LIB_SOURCES =$(filter-out$(VIRTUAL_MEMORY_TESTS),$(VIRTUAL_MEMORY_SOURCES))
VIRTUAL_MEMORY_LIB_OBJECTS =$(VIRTUAL_MEMORY_LIB_SOURCES:$(VIRTUAL_MEMORY_SIM_DIR)/%.c=$(OBJ_DIR)/virtual_memory_sim/%.o)

# 所有对象文件
ALL_OBJECTS =$(COMMON_OBJECTS)$(CPU_OBJECTS)$(CACHE_OBJECTS)$(INSTRUCTION_OBJECTS)$(PIPELINE_OBJECTS)$(VIRTUAL_MEMORY_OBJECTS)$(BUS_OBJECTS)$(IO_OBJECTS)

# 库文件
COMMON_LIB =$(LIB_DIR)/libcommon.a
CPU_LIB =$(LIB_DIR)/libcpu.a
CACHE_LIB =$(LIB_DIR)/libcache.a
INSTRUCTION_LIB =$(LIB_DIR)/libinstruction.a
PIPELINE_LIB =$(LIB_DIR)/libpipeline.a
# MEMORY_LIB =$(LIB_DIR)/libmemory.a  # 已移除
VIRTUAL_MEMORY_LIB =$(LIB_DIR)/libvirtual_memory.a
BUS_LIB =$(LIB_DIR)/libbus.a
IO_LIB =$(LIB_DIR)/libio.a

# 测试可执行文件
CPU_TEST_BINS =$(CPU_TESTS:$(CPU_SIM_DIR)/test_%.c=$(BIN_DIR)/test_cpu_%)
CACHE_TEST_BINS =$(CACHE_TESTS:$(CACHE_SIM_DIR)/test_%.c=$(BIN_DIR)/test_cache_%)
PIPELINE_TEST_BINS =$(PIPELINE_TESTS:$(PIPELINE_SIM_DIR)/test_%.c=$(BIN_DIR)/test_pipeline_%)
# MEMORY_TEST_BINS =$(MEMORY_TESTS:$(MEMORY_SIM_DIR)/test_%.c=$(BIN_DIR)/test_memory_%)  # 已移除
BUS_TEST_BINS =$(BUS_TESTS:$(BUS_SIM_DIR)/test_%.c=$(BIN_DIR)/test_bus_%)
IO_TEST_BINS =$(IO_TESTS:$(IO_SIM_DIR)/test_%.c=$(BIN_DIR)/test_io_%)

# 外部单元测试文件
ALU_TEST_BIN =$(BIN_DIR)/test_alu
BUS_UNIT_TEST_BIN =$(BIN_DIR)/test_bus_unit
IO_UNIT_TEST_BIN =$(BIN_DIR)/test_io_unit

ALL_TEST_BINS =$(CPU_TEST_BINS)$(CACHE_TEST_BINS)$(PIPELINE_TEST_BINS)$(BUS_TEST_BINS)$(IO_TEST_BINS)

# 主要可执行文件
# MAIN_EXECUTABLES =$(BIN_DIR)/cpu_sim$(BIN_DIR)/cache_sim$(BIN_DIR)/instruction_sim$(BIN_DIR)/pipeline_sim$(BIN_DIR)/memory_sim$(BIN_DIR)/bus_sim$(BIN_DIR)/io_sim
MAIN_EXECUTABLES =$(BIN_DIR)/cpu_sim$(BIN_DIR)/cache_sim$(BIN_DIR)/instruction_sim$(BIN_DIR)/pipeline_sim$(BIN_DIR)/virtual_memory_sim$(BIN_DIR)/bus_sim$(BIN_DIR)/io_sim

# =============================================================================
# 主要目标
# =============================================================================

# 默认目标
.PHONY: all
all: info build

# 显示项目信息
.PHONY: info
info:
	@echo "========================================"
	@echo "  计算机组成原理模块构建系统"
	@echo "========================================"
	@echo "模块名称:$(MODULE_NAME)"
	@echo "构建目录:$(BUILD_DIR)"
	@echo "编译器:$(CC)"
	@echo "编译选项:$(CFLAGS)"
	@echo ""
	@echo "可用目标:"
	@echo "  build          - 构建所有模拟器"
	@echo "  build-cpu      - 构建CPU模拟器"
	@echo "  build-cache    - 构建缓存模拟器"
	@echo "  build-instruction - 构建指令系统模拟器"
	@echo "  build-pipeline - 构建流水线模拟器"
	@echo "  build-memory   - 构建内存模拟器"
	@echo "  test           - 运行所有测试"
	@echo "  test-cpu       - 运行CPU测试"
	@echo "  test-cache     - 运行缓存测试"
	@echo "  test-instruction - 运行指令系统测试"
	@echo "  test-pipeline  - 运行流水线测试"
	@echo "  test-memory    - 运行内存测试"
	@echo "  clean          - 清理构建文件"
	@echo "  help           - 显示帮助信息"
	@echo "========================================"

# 构建所有组件
.PHONY: build
build:$(MAIN_EXECUTABLES)$(ALL_TEST_BINS)
	@echo "✓ 所有模拟器构建完成"

# 构建单个模拟器
.PHONY: build-cpu build-cache build-instruction build-pipeline build-memory build-bus build-io
build-cpu:$(BIN_DIR)/cpu_sim$(CPU_TEST_BINS)
	@echo "✓ CPU模拟器构建完成"

build-cache:$(BIN_DIR)/cache_sim$(CACHE_TEST_BINS)
	@echo "✓ 缓存模拟器构建完成"

build-instruction:$(BIN_DIR)/instruction_sim$(BIN_DIR)/test_instruction
	@echo "✓ 指令系统模拟器构建完成"

build-pipeline:$(BIN_DIR)/pipeline_sim$(PIPELINE_TEST_BINS)
	@echo "✓ 流水线模拟器构建完成"

build-memory:$(BIN_DIR)/memory_sim$(BIN_DIR)/test_virtual_memory
	@echo "✓ 内存模拟器构建完成"

build-bus:$(BIN_DIR)/bus_sim$(BIN_DIR)/test_bus_unit
	@echo "✓ 总线模拟器构建完成"

build-io:$(BIN_DIR)/io_sim$(BIN_DIR)/test_io_unit
	@echo "✓ I/O模拟器构建完成"

# =============================================================================
# 构建规则
# =============================================================================

# 创建必要的目录
$(BUILD_DIR)$(BIN_DIR)$(OBJ_DIR)$(LIB_DIR)$(TEST_DIR):
	@mkdir -p$@

$(OBJ_DIR)/common$(OBJ_DIR)/cpu_sim$(OBJ_DIR)/cache_sim$(OBJ_DIR)/instruction_sim$(OBJ_DIR)/pipeline_sim$(OBJ_DIR)/virtual_memory_sim$(OBJ_DIR)/bus_sim$(OBJ_DIR)/io_sim: |$(OBJ_DIR)
	@mkdir -p$@

# 公共组件编译规则
$(OBJ_DIR)/common/%.o:$(COMMON_DIR)/%.c |$(OBJ_DIR)/common
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -c$< -o$@

# CPU模拟器编译规则
$(OBJ_DIR)/cpu_sim/%.o:$(CPU_SIM_DIR)/%.c |$(OBJ_DIR)/cpu_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(CPU_SIM_DIR) -c$< -o$@

# 缓存模拟器编译规则
$(OBJ_DIR)/cache_sim/%.o:$(CACHE_SIM_DIR)/%.c |$(OBJ_DIR)/cache_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(CACHE_SIM_DIR) -c$< -o$@

# 指令系统模拟器编译规则
$(OBJ_DIR)/instruction_sim/%.o:$(INSTRUCTION_SIM_DIR)/%.c |$(OBJ_DIR)/instruction_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(INSTRUCTION_SIM_DIR) -c$< -o$@

# 流水线模拟器编译规则
$(OBJ_DIR)/pipeline_sim/%.o:$(PIPELINE_SIM_DIR)/%.c |$(OBJ_DIR)/pipeline_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(PIPELINE_SIM_DIR) -c$< -o$@

# 内存模拟器编译规则
#$(OBJ_DIR)/memory_sim/%.o:$(MEMORY_SIM_DIR)/%.c |$(OBJ_DIR)/memory_sim
#	@echo "编译$<..."
#	@$(CC)$(CFLAGS)$(INCLUDES) -I$(MEMORY_SIM_DIR) -c$< -o$@

# 总线模拟器编译规则
$(OBJ_DIR)/bus_sim/%.o:$(BUS_SIM_DIR)/%.c |$(OBJ_DIR)/bus_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(BUS_SIM_DIR) -c$< -o$@

# I/O模拟器编译规则
$(OBJ_DIR)/io_sim/%.o:$(IO_SIM_DIR)/%.c |$(OBJ_DIR)/io_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(IO_SIM_DIR) -c$< -o$@

# 虚拟存储器模拟器编译规则
$(OBJ_DIR)/virtual_memory_sim/%.o:$(VIRTUAL_MEMORY_SIM_DIR)/%.c |$(OBJ_DIR)/virtual_memory_sim
	@echo "编译$<..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(VIRTUAL_MEMORY_SIM_DIR) -c$< -o$@

# =============================================================================
# 库文件构建
# =============================================================================

# 公共库
$(COMMON_LIB):$(COMMON_OBJECTS) |$(LIB_DIR)
	@echo "创建公共库$@..."
	@if [ "$(COMMON_OBJECTS)" != "" ]; then \
		ar rcs$@$^; \
	else \
		echo "跳过公共库创建（仅头文件）"; \
		touch$@; \
	fi

# CPU库
$(CPU_LIB):$(CPU_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建CPU库$@..."
	@ar rcs$@$^

# 缓存库
$(CACHE_LIB):$(CACHE_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建缓存库$@..."
	@ar rcs$@$^

# 指令系统库
$(INSTRUCTION_LIB):$(INSTRUCTION_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建指令系统库$@..."
	@ar rcs$@$^

# 流水线库
$(PIPELINE_LIB):$(PIPELINE_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建流水线库$@..."
	@ar rcs$@$^

# 内存库
#$(MEMORY_LIB):$(MEMORY_LIB_OBJECTS) |$(LIB_DIR)
#	@echo "创建内存库$@..."
#	@ar rcs$@$^

# 总线库
$(BUS_LIB):$(BUS_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建总线库$@..."
	@ar rcs$@$^

# I/O库
$(IO_LIB):$(IO_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建I/O库$@..."
	@ar rcs$@$^

# 虚拟存储器库
$(VIRTUAL_MEMORY_LIB):$(VIRTUAL_MEMORY_LIB_OBJECTS) |$(LIB_DIR)
	@echo "创建虚拟存储器库$@..."
	@ar rcs$@$^

# =============================================================================
# 可执行文件构建
# =============================================================================

# CPU模拟器主程序
$(BIN_DIR)/cpu_sim:$(CPU_SIM_DIR)/main.c$(CPU_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接CPU模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(CPU_SIM_DIR) -o$@$< -L$(LIB_DIR) -lcpu -lcommon

# 缓存模拟器主程序
$(BIN_DIR)/cache_sim:$(CACHE_SIM_DIR)/main.c$(CACHE_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接缓存模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(CACHE_SIM_DIR) -o$@$< -L$(LIB_DIR) -lcache -lcommon

# 指令系统模拟器主程序
$(BIN_DIR)/instruction_sim:$(INSTRUCTION_SIM_DIR)/main.c$(INSTRUCTION_LIB) |$(BIN_DIR)
	@echo "编译和链接指令系统模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(INSTRUCTION_SIM_DIR) -o$@$< -L$(LIB_DIR) -linstruction

# 流水线模拟器主程序
$(BIN_DIR)/pipeline_sim:$(PIPELINE_SIM_DIR)/main.c$(PIPELINE_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接流水线模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(PIPELINE_SIM_DIR) -o$@$< -L$(LIB_DIR) -lpipeline -lcommon

# 内存模拟器主程序（使用虚拟存储模拟器）
$(BIN_DIR)/memory_sim:$(VIRTUAL_MEMORY_SIM_DIR)/main.c$(VIRTUAL_MEMORY_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "构建内存模拟器（使用虚拟内存实现）..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(VIRTUAL_MEMORY_SIM_DIR) -o$@$< -L$(LIB_DIR) -lvirtual_memory -lcommon

# 总线模拟器主程序
$(BIN_DIR)/bus_sim:$(BUS_SIM_DIR)/main.c$(BUS_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接总线模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(BUS_SIM_DIR) -o$@$< -L$(LIB_DIR) -lbus -lcommon

# I/O模拟器主程序
$(BIN_DIR)/io_sim:$(IO_SIM_DIR)/main.c$(IO_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接I/O模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(IO_SIM_DIR) -o$@$< -L$(LIB_DIR) -lio -lcommon

# 新增虚拟内存模拟器独立可执行文件
$(BIN_DIR)/virtual_memory_sim:$(VIRTUAL_MEMORY_SIM_DIR)/main.c$(VIRTUAL_MEMORY_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "构建虚拟内存模拟器..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(VIRTUAL_MEMORY_SIM_DIR) -o$@$< -L$(LIB_DIR) -lvirtual_memory -lcommon

# =============================================================================
# 测试程序构建
# =============================================================================

# CPU测试程序
$(BIN_DIR)/test_cpu_%:$(OBJ_DIR)/cpu_sim/test_%.o$(CPU_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接CPU测试程序$@..."
	@$(CC)$(CFLAGS) -o$@$< -L$(LIB_DIR) -lcpu -lcommon

# 缓存测试程序
$(BIN_DIR)/test_cache_%:$(OBJ_DIR)/cache_sim/test_%.o$(CACHE_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接缓存测试程序$@..."
	@$(CC)$(CFLAGS) -o$@$< -L$(LIB_DIR) -lcache -lcommon

# 指令系统测试程序（外部测试文件）
$(BIN_DIR)/test_instruction: tests/unit_tests/instruction/test_instruction.c$(INSTRUCTION_LIB) |$(BIN_DIR)
	@echo "编译和链接指令系统测试程序..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(INSTRUCTION_SIM_DIR) -o$@$< -L$(LIB_DIR) -linstruction

# ALU测试程序（外部测试文件）
$(BIN_DIR)/test_alu: tests/unit_tests/alu/test_alu.c$(CPU_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接ALU测试程序..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(CPU_SIM_DIR) -o$@$< -L$(LIB_DIR) -lcpu -lcommon

# 总线测试程序（外部测试文件）
$(BIN_DIR)/test_bus_unit: tests/unit_tests/bus/test_bus.c$(BUS_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接总线测试程序..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(BUS_SIM_DIR) -o$@$< -L$(LIB_DIR) -lbus -lcommon

# I/O测试程序（外部测试文件）
$(BIN_DIR)/test_io_unit: tests/unit_tests/io/test_io.c$(IO_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接I/O测试程序..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(IO_SIM_DIR) -o$@$< -L$(LIB_DIR) -lio -lcommon

# 虚拟存储器测试程序（外部测试文件）
$(BIN_DIR)/test_virtual_memory: tests/unit_tests/virtual_memory/test_virtual_memory.c$(VIRTUAL_MEMORY_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "编译和链接虚拟存储器测试程序..."
	@$(CC)$(CFLAGS)$(INCLUDES) -I$(VIRTUAL_MEMORY_SIM_DIR) -o$@$< -L$(LIB_DIR) -lvirtual_memory -lcommon

# 流水线测试程序
$(BIN_DIR)/test_pipeline_%:$(OBJ_DIR)/pipeline_sim/test_%.o$(PIPELINE_LIB)$(COMMON_LIB) |$(BIN_DIR)
	@echo "链接流水线测试程序$@..."
	@$(CC)$(CFLAGS) -o$@$< -L$(LIB_DIR) -lpipeline -lcommon

# 内存测试程序
#$(BIN_DIR)/test_memory_%:$(OBJ_DIR)/memory_sim/test_%.o$(MEMORY_LIB)$(COMMON_LIB) |$(BIN_DIR)
#	@echo "链接内存测试程序$@..."
#	@$(CC)$(CFLAGS) -o$@$< -L$(LIB_DIR) -lmemory -lcommon

# =============================================================================
# 测试目标
# =============================================================================

# 运行所有测试
.PHONY: test
test: test-cpu test-cache test-instruction test-pipeline test-memory test-virtual-memory test-alu test-bus test-io
	@echo "✓ 所有测试完成"

# 运行CPU测试
.PHONY: test-cpu
test-cpu:$(CPU_TEST_BINS)
	@echo "运行CPU模拟器测试..."
	@for test in$(CPU_TEST_BINS); do \
		echo "运行$$test..."; \
		$$test || exit 1; \
	done
	@echo "✓ CPU测试通过"

# 运行缓存测试
.PHONY: test-cache
test-cache:$(CACHE_TEST_BINS)
	@echo "运行缓存模拟器测试..."
	@for test in$(CACHE_TEST_BINS); do \
		echo "运行$$test..."; \
		$$test || exit 1; \
	done
	@echo "✓ 缓存测试通过"

# 运行指令系统测试
.PHONY: test-instruction
test-instruction:$(BIN_DIR)/test_instruction
	@echo "运行指令系统模拟器测试..."
	@$(BIN_DIR)/test_instruction || exit 1
	@echo "✓ 指令系统测试通过"

# 运行流水线测试
.PHONY: test-pipeline
test-pipeline:$(PIPELINE_TEST_BINS)
	@echo "运行流水线模拟器测试..."
	@for test in$(PIPELINE_TEST_BINS); do \
		echo "运行$$test..."; \
		$$test || exit 1; \
	done
	@echo "✓ 流水线测试通过"

# 运行内存测试
.PHONY: test-memory
test-memory:$(MEMORY_TEST_BINS)
	@echo "运行内存模拟器测试..."
	@for test in$(MEMORY_TEST_BINS); do \
		echo "运行$$test..."; \
		$$test || exit 1; \
	done
	@echo "✓ 内存测试通过"

# 运行ALU测试
.PHONY: test-alu
test-alu:$(BIN_DIR)/test_alu
	@echo "运行ALU模拟器测试..."
	@$(BIN_DIR)/test_alu || exit 1
	@echo "✓ ALU测试通过"

# 运行总线测试
.PHONY: test-bus
test-bus:$(BIN_DIR)/test_bus_unit
	@echo "运行总线模拟器测试..."
	@$(BIN_DIR)/test_bus_unit || exit 1
	@echo "✓ 总线测试通过"

# 运行I/O测试
.PHONY: test-io
test-io:$(BIN_DIR)/test_io_unit
	@echo "运行I/O模拟器测试..."
	@$(BIN_DIR)/test_io_unit || exit 1
	@echo "✓ I/O测试通过"

# 运行虚拟存储器测试
.PHONY: test-virtual-memory
test-virtual-memory:$(BIN_DIR)/test_virtual_memory
	@echo "运行虚拟存储器模拟器测试..."
	@$(BIN_DIR)/test_virtual_memory || exit 1
	@echo "✓ 虚拟存储器测试通过"

# =============================================================================
# 演示和示例目标
# =============================================================================

# 运行CPU演示
.PHONY: run-cpu-demo
run-cpu-demo:$(BIN_DIR)/cpu_sim
	@echo "运行CPU模拟器演示..."
	@if [ -f$(EXAMPLES_DIR)/simple_cpu/demo.asm ]; then \
		$(BIN_DIR)/cpu_sim -f$(EXAMPLES_DIR)/simple_cpu/demo.asm -v; \
	else \
		echo "演示文件不存在，请先创建示例程序"; \
	fi

# 运行缓存演示
.PHONY: run-cache-demo
run-cache-demo:$(BIN_DIR)/cache_sim
	@echo "运行缓存模拟器演示..."
	@if [ -f$(EXAMPLES_DIR)/cache_analysis/demo.trace ]; then \
		$(BIN_DIR)/cache_sim -t$(EXAMPLES_DIR)/cache_analysis/demo.trace; \
	else \
		echo "演示文件不存在，请先创建示例程序"; \
	fi

# 运行流水线演示
.PHONY: run-pipeline-demo
run-pipeline-demo:$(BIN_DIR)/pipeline_sim
	@echo "运行流水线模拟器演示..."
	@if [ -f$(EXAMPLES_DIR)/pipeline_demo/demo.asm ]; then \
		$(BIN_DIR)/pipeline_sim -f$(EXAMPLES_DIR)/pipeline_demo/demo.asm --show-pipeline; \
	else \
		echo "演示文件不存在，请先创建示例程序"; \
	fi

# 运行性能基准测试
.PHONY: run-benchmark
run-benchmark: build
	@echo "运行性能基准测试..."
	@if [ -d$(EXAMPLES_DIR)/benchmark ]; then \
		for bench in$(EXAMPLES_DIR)/benchmark/*; do \
			echo "运行基准测试$$bench..."; \
		done; \
	else \
		echo "基准测试不存在，请先创建基准测试程序"; \
	fi

# =============================================================================
# 开发辅助目标
# =============================================================================

# 代码静态分析
.PHONY: analyze
analyze:
	@echo "运行代码静态分析..."
	@if command -v cppcheck >/dev/null; then \
		cppcheck --enable=all --std=c99$(SRC_DIR)/; \
	else \
		echo "cppcheck未安装，跳过静态分析"; \
	fi

# 内存检查
.PHONY: memcheck
memcheck:$(ALL_TEST_BINS)
	@echo "运行内存检查..."
	@if command -v valgrind >/dev/null; then \
		for test in$(ALL_TEST_BINS); do \
			echo "内存检查$$test..."; \
			valgrind --leak-check=full --show-leak-kinds=all$$test; \
		done; \
	else \
		echo "valgrind未安装，跳过内存检查"; \
	fi

# 代码格式化
.PHONY: format
format:
	@echo "格式化代码..."
	@if command -v clang-format >/dev/null; then \
		find$(SRC_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i; \
		echo "✓ 代码格式化完成"; \
	else \
		echo "clang-format未安装，跳过代码格式化"; \
	fi

# 代码统计
.PHONY: stats
stats:
	@echo "代码统计信息:"
	@echo "源文件数量:"
	@find$(SRC_DIR) -name "*.c" | wc -l | xargs echo "  C文件:"
	@find$(SRC_DIR) -name "*.h" | wc -l | xargs echo "  头文件:"
	@echo "代码行数:"
	@find$(SRC_DIR) -name "*.c" -o -name "*.h" | xargs wc -l | tail -1

# 检查构建环境
.PHONY: check-env
check-env:
	@echo "检查构建环境..."
	@echo -n "gcc: "
	@if command -v gcc >/dev/null; then \
		gcc --version | head -1; \
	else \
		echo "未安装"; \
	fi
	@echo -n "make: "
	@if command -v make >/dev/null; then \
		make --version | head -1; \
	else \
		echo "未安装"; \
	fi
	@echo -n "gdb: "
	@if command -v gdb >/dev/null; then \
		gdb --version | head -1; \
	else \
		echo "未安装（可选）"; \
	fi
	@echo -n "valgrind: "
	@if command -v valgrind >/dev/null; then \
		valgrind --version; \
	else \
		echo "未安装（可选）"; \
	fi

# =============================================================================
# 清理目标
# =============================================================================

# 清理构建文件
.PHONY: clean
clean:
	@echo "清理构建文件..."
	@rm -rf$(BUILD_DIR)
	@echo "✓ 清理完成"

# 深度清理（包括备份文件等）
.PHONY: distclean
distclean: clean
	@echo "深度清理..."
	@find . -name "*~" -delete
	@find . -name "*.bak" -delete
	@find . -name "*.tmp" -delete
	@find . -name ".DS_Store" -delete
	@echo "✓ 深度清理完成"

# =============================================================================
# 帮助信息
# =============================================================================

.PHONY: help
help:
	@echo "计算机组成原理模块构建系统"
	@echo ""
	@echo "主要目标："
	@echo "  all            构建所有组件（默认）"
	@echo "  build          构建所有模拟器"
	@echo "  test           运行所有测试"
	@echo "  clean          清理构建文件"
	@echo ""
	@echo "模块构建："
	@echo "  build-cpu      构建CPU模拟器"
	@echo "  build-cache    构建缓存模拟器"
	@echo "  build-pipeline 构建流水线模拟器"
	@echo "  build-memory   构建内存模拟器"
	@echo ""
	@echo "测试目标："
	@echo "  test-cpu       运行CPU测试"
	@echo "  test-cache     运行缓存测试"
	@echo "  test-pipeline  运行流水线测试"
	@echo "  test-memory    运行内存测试"
	@echo ""
	@echo "演示目标："
	@echo "  run-cpu-demo   运行CPU演示"
	@echo "  run-cache-demo 运行缓存演示"
	@echo "  run-pipeline-demo 运行流水线演示"
	@echo "  run-benchmark  运行性能基准测试"
	@echo ""
	@echo "开发辅助："
	@echo "  analyze        代码静态分析"
	@echo "  memcheck       内存检查"
	@echo "  format         代码格式化"
	@echo "  stats          代码统计"
	@echo "  check-env      检查构建环境"
	@echo ""
	@echo "清理目标："
	@echo "  clean          清理构建文件"
	@echo "  distclean      深度清理"

# =============================================================================
# 特殊目标声明
# =============================================================================

# 防止文件名冲突
.PHONY: all build test clean help info
.PHONY: build-cpu build-cache build-pipeline build-memory
.PHONY: test-cpu test-cache test-pipeline test-memory
.PHONY: run-cpu-demo run-cache-demo run-pipeline-demo run-benchmark
.PHONY: analyze memcheck format stats check-env distclean

# 保持中间文件
.PRECIOUS:$(ALL_OBJECTS)

# 默认shell
SHELL = /bin/bash 