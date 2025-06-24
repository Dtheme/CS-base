# Makefile for 计算机组成原理测试套件
# 遵循数据结构算法编码标准
# create by: zw.duan

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lm

# 目录设置
SRC_DIR = ../simulation
UNIT_TEST_DIR = unit_tests
INTEGRATION_TEST_DIR = integration_tests
PERFORMANCE_TEST_DIR = performance_tests

# 源文件路径
ALU_SRC =$(SRC_DIR)/cpu_sim/alu.c
CACHE_SRC =$(SRC_DIR)/cache_sim/cache.c
INSTRUCTION_SRC =$(SRC_DIR)/instruction_sim/instruction.c
BUS_SRC =$(SRC_DIR)/bus_sim/bus.c
CPU_SRC =$(SRC_DIR)/cpu_sim/cpu_core.c$(SRC_DIR)/cpu_sim/registers.c$(SRC_DIR)/cpu_sim/memory.c
PIPELINE_SRC =$(SRC_DIR)/pipeline_sim/pipeline.c
IO_SRC =$(SRC_DIR)/io_sim/io.c$(SRC_DIR)/io_sim/device_impl.c
VIRTUAL_MEMORY_SRC =$(SRC_DIR)/virtual_memory_sim/virtual_memory.c
COMMON_SRC =$(SRC_DIR)/common/utils.c$(SRC_DIR)/common/debug.c$(SRC_DIR)/common/test_framework.c

# 测试目标 - 新的组织结构
UNIT_TESTS = test_alu test_cache test_instruction test_bus test_cpu test_pipeline test_io test_virtual_memory
INTEGRATION_TESTS = test_cpu_integration
PERFORMANCE_TESTS = test_performance

# 所有测试目标
ALL_TESTS =$(UNIT_TESTS)$(INTEGRATION_TESTS)$(PERFORMANCE_TESTS)

# 默认目标
.PHONY: all clean help test-unit test-integration test-performance test-all
.PHONY: test-alu test-cache test-instruction test-bus test-cpu test-pipeline test-io

all:$(ALL_TESTS)
	@echo "✓ 所有测试程序编译完成"

# 单元测试编译 - 新结构
test_alu:$(UNIT_TEST_DIR)/alu/test_alu.c$(ALU_SRC)$(COMMON_SRC)
	@echo "编译ALU单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/cpu_sim$^ -o$@$(LDFLAGS)
	@echo "✓ ALU单元测试编译完成"

test_cache:$(UNIT_TEST_DIR)/cache/test_cache.c$(CACHE_SRC)$(COMMON_SRC)
	@echo "编译缓存单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/cache_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 缓存单元测试编译完成"

test_instruction:$(UNIT_TEST_DIR)/instruction/test_instruction.c$(INSTRUCTION_SRC)$(COMMON_SRC)
	@echo "编译指令系统单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/instruction_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 指令系统单元测试编译完成"

test_bus:$(UNIT_TEST_DIR)/bus/test_bus.c$(BUS_SRC)$(COMMON_SRC)
	@echo "编译总线单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/bus_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 总线单元测试编译完成"

test_cpu:$(UNIT_TEST_DIR)/cpu_core/test_cpu.c$(CPU_SRC)$(ALU_SRC)$(COMMON_SRC)
	@echo "编译CPU单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/cpu_sim$^ -o$@$(LDFLAGS)
	@echo "✓ CPU单元测试编译完成"

test_pipeline:$(UNIT_TEST_DIR)/pipeline/test_pipeline.c$(PIPELINE_SRC)$(COMMON_SRC)
	@echo "编译流水线单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/pipeline_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 流水线单元测试编译完成"

test_io:$(UNIT_TEST_DIR)/io/test_io.c$(IO_SRC)$(COMMON_SRC)
	@echo "编译I/O单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/io_sim$^ -o$@$(LDFLAGS)
	@echo "✓ I/O单元测试编译完成"

test_virtual_memory:$(UNIT_TEST_DIR)/virtual_memory/test_virtual_memory.c$(VIRTUAL_MEMORY_SRC)$(COMMON_SRC)
	@echo "编译虚拟内存单元测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/virtual_memory_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 虚拟内存单元测试编译完成"

# 集成测试编译
test_cpu_integration:$(INTEGRATION_TEST_DIR)/test_cpu_integration.c$(ALU_SRC)$(COMMON_SRC)
	@echo "编译系统集成测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/cpu_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 系统集成测试编译完成"

# 性能测试编译
test_performance:$(PERFORMANCE_TEST_DIR)/test_performance.c$(ALU_SRC)$(COMMON_SRC)
	@echo "编译性能测试..."
	$(CC)$(CFLAGS) -I$(SRC_DIR)/common -I$(SRC_DIR)/cpu_sim$^ -o$@$(LDFLAGS)
	@echo "✓ 性能测试编译完成"

# 运行测试 - 按类型分组
test-unit:$(UNIT_TESTS)
	@echo ""
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║                   运行单元测试                              ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "🧪 运行ALU单元测试..."
	./test_alu || echo "❌ ALU测试失败"
	@echo ""
	@echo "🧪 运行缓存单元测试..."
	./test_cache || echo "❌ 缓存测试失败"
	@echo ""
	@echo "🧪 运行指令系统单元测试..."
	./test_instruction || echo "❌ 指令测试失败"
	@echo ""
	@echo "🧪 运行总线单元测试..."
	./test_bus || echo "❌ 总线测试失败"
	@echo ""
	@echo "🧪 运行CPU单元测试..."
	./test_cpu || echo "❌ CPU测试失败"
	@echo ""
	@echo "🧪 运行流水线单元测试..."
	./test_pipeline || echo "❌ 流水线测试失败"
	@echo ""
	@echo "🧪 运行I/O单元测试..."
	./test_io || echo "❌ I/O测试失败"
	@echo ""
	@echo "🧪 运行虚拟内存单元测试..."
	./test_virtual_memory || echo "❌ 虚拟内存测试失败"

test-integration:$(INTEGRATION_TESTS)
	@echo ""
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║                   运行集成测试                              ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "🔗 运行CPU集成测试..."
	./test_cpu_integration || echo "❌ CPU集成测试失败"

test-performance:$(PERFORMANCE_TESTS)
	@echo ""
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║                   运行性能测试                              ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "⚡ 运行性能基准测试..."
	./test_performance || echo "❌ 性能测试失败"

test-all:$(ALL_TESTS)
	@echo ""
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║                   运行所有测试                              ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@$(MAKE) test-unit
	@$(MAKE) test-integration  
	@$(MAKE) test-performance
	@echo ""
	@echo "🎉 所有测试执行完成！"

# 单独运行特定模块测试
run-alu: test_alu
	@echo "🧪 运行ALU测试..."
	./test_alu

run-cache: test_cache
	@echo "🧪 运行缓存测试..."
	./test_cache

run-instruction: test_instruction
	@echo "🧪 运行指令测试..."
	./test_instruction

run-bus: test_bus
	@echo "🧪 运行总线测试..."
	./test_bus

run-cpu: test_cpu
	@echo "🧪 运行CPU测试..."
	./test_cpu

run-pipeline: test_pipeline
	@echo "🧪 运行流水线测试..."
	./test_pipeline

run-io: test_io
	@echo "🧪 运行I/O测试..."
	./test_io

run-virtual-memory: test_virtual_memory
	@echo "🧪 运行虚拟内存测试..."
	./test_virtual_memory

# 清理
clean:
	@echo "清理测试文件..."
	rm -f$(ALL_TESTS)
	rm -rf *.dSYM
	rm -f *.o *.a
	@echo "✓ 清理完成"

# 深度清理（包括所有临时文件）
clean-all: clean
	@echo "深度清理..."
	find . -name "*.log" -delete
	find . -name "*.tmp" -delete
	find . -name "core.*" -delete
	@echo "✓ 深度清理完成"

# 帮助信息
help:
	@echo "计算机组成原理测试套件 - 使用说明"
	@echo "=================================="
	@echo ""
	@echo "📋 编译目标："
	@echo "  all              - 编译所有测试程序"
	@echo "  test_alu         - 编译ALU单元测试"
	@echo "  test_cache       - 编译缓存单元测试"
	@echo "  test_instruction - 编译指令系统测试"
	@echo "  test_bus         - 编译总线系统测试"
	@echo "  test_cpu         - 编译CPU核心测试"
	@echo "  test_pipeline    - 编译流水线测试"
	@echo "  test_io          - 编译I/O系统测试"
	@echo "  test_virtual_memory - 编译虚拟内存测试"
	@echo ""
	@echo "🧪 运行目标："
	@echo "  test-unit        - 运行所有单元测试"
	@echo "  test-integration - 运行集成测试"
	@echo "  test-performance - 运行性能测试"
	@echo "  test-all         - 运行所有测试"
	@echo ""
	@echo "🎯 单独测试："
	@echo "  run-alu          - 运行ALU测试"
	@echo "  run-cache        - 运行缓存测试"
	@echo "  run-instruction  - 运行指令测试"
	@echo "  run-bus          - 运行总线测试"
	@echo "  run-cpu          - 运行CPU测试"
	@echo "  run-pipeline     - 运行流水线测试"
	@echo "  run-io           - 运行I/O测试"
	@echo "  run-virtual-memory - 运行虚拟内存测试"
	@echo ""
	@echo "🛠️ 其他目标："
	@echo "  clean            - 清理编译文件"
	@echo "  help             - 显示此帮助信息"
	@echo "  info             - 显示项目信息"
	@echo ""
	@echo "💡 示例用法："
	@echo "  make all         # 编译所有测试"
	@echo "  make test-all    # 运行所有测试"
	@echo "  make run-alu     # 只运行ALU测试"
	@echo "  make clean       # 清理文件"

# 项目信息
info:
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║              计算机组成原理测试套件信息                     ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo "📚 项目名称: 计算机组成原理模拟器测试套件"
	@echo "👨‍💻 创建者: zw.duan"
	@echo "📏 编码标准: 数据结构算法编码标准"
	@echo "🎯 408考研: 重点内容覆盖和真题演示"
	@echo ""
	@echo "📊 测试覆盖模块:"
	@echo "├── 🧮 ALU (算术逻辑单元)"
	@echo "├── 🚌 总线系统"
	@echo "├── 💾 缓存系统"
	@echo "├── 🖥️  CPU核心"
	@echo "├── 📋 指令系统"
	@echo "├── 🔄 流水线"
	@echo "├── 💿 I/O系统"
	@echo "├── 🔗 系统集成"
	@echo "└── ⚡ 性能基准"
	@echo ""
	@echo "📁 测试文件结构:"
	@echo "├── unit_tests/          # 单元测试"
	@echo "│   ├── alu/             # ALU测试"
	@echo "│   ├── bus/             # 总线测试"
	@echo "│   ├── cache/           # 缓存测试"
	@echo "│   ├── cpu_core/        # CPU测试"
	@echo "│   ├── instruction/     # 指令测试"
	@echo "│   ├── pipeline/        # 流水线测试"
	@echo "│   └── io/              # I/O测试"
	@echo "├── integration_tests/   # 集成测试"
	@echo "├── performance_tests/   # 性能测试"
	@echo "└── Makefile            # 构建脚本" 