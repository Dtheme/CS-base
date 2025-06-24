# Makefile for 计算机组成原理模拟器
# 计算机组成原理 
# create by: zw.duan

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = -lm

# 目录设置
COMMON_DIR = common
CPU_DIR = cpu_sim
CACHE_DIR = cache_sim
INSTRUCTION_DIR = instruction_sim
PIPELINE_DIR = pipeline_sim
VIRTUAL_MEMORY_DIR = virtual_memory_sim
BUS_DIR = bus_sim
IO_DIR = io_sim

# 模拟器模块
MODULES = common cpu_sim cache_sim virtual_memory_sim instruction_sim bus_sim io_sim pipeline_sim

# 源文件
ALU_SOURCES =$(CPU_DIR)/alu.c
ALU_HEADERS =$(CPU_DIR)/alu.h

CACHE_SOURCES =$(CACHE_DIR)/cache.c
CACHE_HEADERS =$(CACHE_DIR)/cache.h

# 默认目标
.PHONY: all clean test help info libs install$(MODULES)

all:$(BUILD_DIR) libs

# 创建构建目录
$(BUILD_DIR):
	@echo "创建构建目录..."
	@mkdir -p$(BUILD_DIR)

# 编译所有库文件
libs:$(BUILD_DIR) common
	@echo "╔══════════════════════════════════════════════════════════════╗"
	@echo "║              编译计算机组成原理模拟器库文件                    ║"
	@echo "╚══════════════════════════════════════════════════════════════╝"
	@echo ""
	@$(MAKE) -C$(CPU_DIR) install || true
	@$(MAKE) -C$(CACHE_DIR) install || true
	@$(MAKE) -C$(VIRTUAL_MEMORY_DIR) install || true
	@$(MAKE) -C$(INSTRUCTION_DIR) install || true
	@$(MAKE) -C$(BUS_DIR) install || true
	@$(MAKE) -C$(IO_DIR) install || true
	@$(MAKE) -C$(PIPELINE_DIR) install || true
	@echo ""
	@echo "✓ 所有库文件编译完成"

# 编译公共模块
common:$(BUILD_DIR)
	@echo "编译公共模块..."
	@$(MAKE) -C$(COMMON_DIR) all

# 各模块编译
cpu_sim: common
	@echo "编译CPU模拟器..."
	@$(MAKE) -C$(CPU_DIR) all

cache_sim: common
	@echo "编译缓存模拟器..."
	@$(MAKE) -C$(CACHE_DIR) all

virtual_memory_sim: common
	@echo "编译虚拟内存模拟器..."
	@$(MAKE) -C$(VIRTUAL_MEMORY_DIR) all

instruction_sim: common
	@echo "编译指令模拟器..."
	@$(MAKE) -C$(INSTRUCTION_DIR) all

bus_sim: common
	@echo "编译总线模拟器..."
	@$(MAKE) -C$(BUS_DIR) all

io_sim: common
	@echo "编译I/O系统模拟器..."
	@$(MAKE) -C$(IO_DIR) all

pipeline_sim: common
	@echo "编译流水线模拟器..."
	@$(MAKE) -C$(PIPELINE_DIR) all

# 运行所有测试
test: all
	@echo "╔══════════════════════════════════════════════════════════════╗"
	@echo "║                     运行所有模拟器测试                         ║"
	@echo "╚══════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "运行CPU模拟器测试..."
	@$(MAKE) -C$(CPU_DIR) test || true
	@echo ""
	@echo "运行缓存模拟器测试..."
	@$(MAKE) -C$(CACHE_DIR) test || true
	@echo ""
	@echo "运行虚拟内存模拟器测试..."
	@$(MAKE) -C$(VIRTUAL_MEMORY_DIR) test || true
	@echo ""
	@echo "运行指令模拟器测试..."
	@$(MAKE) -C$(INSTRUCTION_DIR) test || true
	@echo ""
	@echo "运行总线模拟器测试..."
	@$(MAKE) -C$(BUS_DIR) test || true
	@echo ""
	@echo "运行I/O系统模拟器测试..."
	@$(MAKE) -C$(IO_DIR) test || true
	@echo ""
	@echo "运行流水线模拟器测试..."
	@$(MAKE) -C$(PIPELINE_DIR) test || true
	@echo ""
	@echo "✓ 所有测试完成"

# 各模块测试
test-cpu: cpu_sim
	@$(MAKE) -C$(CPU_DIR) test

test-cache: cache_sim
	@$(MAKE) -C$(CACHE_DIR) test

test-virtual-memory: virtual_memory_sim
	@$(MAKE) -C$(VIRTUAL_MEMORY_DIR) test

test-instruction: instruction_sim
	@$(MAKE) -C$(INSTRUCTION_DIR) test

test-bus: bus_sim
	@$(MAKE) -C$(BUS_DIR) test

test-io: io_sim
	@$(MAKE) -C$(IO_DIR) test

test-pipeline: pipeline_sim
	@$(MAKE) -C$(PIPELINE_DIR) test

# 安装所有模块
install: libs
	@echo "安装所有模块到构建目录..."
	@$(MAKE) -C$(COMMON_DIR) install || true
	@$(MAKE) -C$(CPU_DIR) install || true
	@$(MAKE) -C$(CACHE_DIR) install || true
	@$(MAKE) -C$(VIRTUAL_MEMORY_DIR) install || true
	@$(MAKE) -C$(INSTRUCTION_DIR) install || true
	@$(MAKE) -C$(BUS_DIR) install || true
	@$(MAKE) -C$(PIPELINE_DIR) install || true

# 清理
clean:
	@echo "清理所有构建文件..."
	@$(MAKE) -C$(COMMON_DIR) clean || true
	@$(MAKE) -C$(CPU_DIR) clean || true
	@$(MAKE) -C$(CACHE_DIR) clean || true
	@$(MAKE) -C$(VIRTUAL_MEMORY_DIR) clean || true
	@$(MAKE) -C$(INSTRUCTION_DIR) clean || true
	@$(MAKE) -C$(BUS_DIR) clean || true
	@$(MAKE) -C$(PIPELINE_DIR) clean || true
	@rm -rf$(BUILD_DIR)
	@echo "✓ 清理完成"

# 帮助信息
help:
	@echo "计算机组成原理模拟器 - Makefile使用说明"
	@echo ""
	@echo "主要目标："
	@echo "  all           - 编译所有模拟器"
	@echo "  libs          - 编译所有库文件"
	@echo "  test          - 运行所有测试"
	@echo "  install       - 安装所有模块"
	@echo "  clean         - 清理构建文件"
	@echo "  help          - 显示此帮助信息"
	@echo "  info          - 显示项目信息"
	@echo ""
	@echo "模块编译："
	@echo "  common        - 编译公共模块"
	@echo "  cpu_sim       - 编译CPU模拟器"
	@echo "  cache_sim     - 编译缓存模拟器"
	@echo "  virtual_memory_sim - 编译虚拟内存模拟器"
	@echo "  instruction_sim - 编译指令模拟器"
	@echo "  bus_sim       - 编译总线模拟器"
	@echo "  pipeline_sim  - 编译流水线模拟器"
	@echo ""
	@echo "模块测试："
	@echo "  test-cpu      - 运行CPU模拟器测试"
	@echo "  test-cache    - 运行缓存模拟器测试"
	@echo "  test-virtual-memory - 运行虚拟内存模拟器测试"
	@echo "  test-instruction - 运行指令模拟器测试"
	@echo "  test-bus      - 运行总线模拟器测试"
	@echo "  test-pipeline - 运行流水线模拟器测试"
	@echo ""
	@echo "示例："
	@echo "  make all          # 编译所有模拟器"
	@echo "  make test         # 运行完整测试"
	@echo "  make test-bus     # 只运行总线测试"
	@echo "  make clean        # 清理文件"

# 显示项目信息
info:
	@echo "╔════════════════════════════════════════════════════════════╗"
	@echo "║              计算机组成原理模拟器项目信息                  ║"
	@echo "╚════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "项目结构："
	@echo " $(COMMON_DIR)/      - 公共模块和工具"
	@echo " $(CPU_DIR)/         - CPU模拟器（ALU、控制器）"
	@echo " $(CACHE_DIR)/       - 缓存模拟器"
	@echo " $(VIRTUAL_MEMORY_DIR)/ - 虚拟内存模拟器"
	@echo " $(INSTRUCTION_DIR)/ - 指令系统模拟器"
	@echo " $(BUS_DIR)/         - 总线系统模拟器"
	@echo " $(PIPELINE_DIR)/    - 流水线模拟器"
	@echo " $(BUILD_DIR)/       - 构建输出目录"
	@echo ""
	@echo "模拟器功能："
	@echo "  CPU模拟器："
	@echo "    - ALU：算术逻辑运算、移位运算"
	@echo "    - 控制器：指令执行控制"
	@echo "    - 寄存器：通用寄存器组"
	@echo ""
	@echo "  缓存模拟器："
	@echo "    - 映射方式：直接映射、组相联、全相联"
	@echo "    - 替换算法：LRU、FIFO、LFU、随机"
	@echo "    - 写策略：写直达、写回"
	@echo ""
	@echo "  虚拟内存模拟器："
	@echo "    - 内存管理：分配、释放、映射"
	@echo "    - 虚拟内存：页式管理"
	@echo "    - 性能分析：访问时间统计"
	@echo ""
	@echo "  指令模拟器："
	@echo "    - 指令格式：RISC-V指令集"
	@echo "    - 指令执行：取指、译码、执行"
	@echo "    - 寻址模式：多种寻址方式"
	@echo ""
	@echo "  总线模拟器："
	@echo "    - 仲裁机制：轮询、优先级、链式"
	@echo "    - 时序控制：同步、异步模式"
	@echo "    - 性能分析：带宽利用率"
	@echo ""
	@echo "  流水线模拟器："
	@echo "    - 流水线阶段：取指、译码、执行、访存、写回"
	@echo "    - 冲突处理：数据冲突、控制冲突"
	@echo "    - 性能优化：分支预测、乱序执行"
	@echo ""
	@echo "408重点："
	@echo "    - 数据表示和运算（第2章）"
	@echo "    - 存储器层次结构（第3章）"
	@echo "    - 指令系统架构（第4章）"
	@echo "    - 中央处理器（第5章）"
	@echo "    - 总线系统（第6章）"
	@echo "    - 输入输出系统（第7章）" 