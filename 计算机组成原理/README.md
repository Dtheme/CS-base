# 计算机组成原理模拟器

408考研学习笔记，代码部分实现了CPU、内存、缓存、总线、I/O等计算机系统核心组件的仿真。文档部分是理论相关知识，以及考试相关题目考察方式。

## 项目概述

包含以下内容：

- **CPU模拟器** - 完整的CPU执行引擎
- **指令系统** - 支持多种寻址方式和指令类型
- **ALU运算器** - 算术逻辑运算单元
- **内存系统** - 内存管理和地址翻译
- **缓存系统** - 多级缓存模拟
- **虚拟存储** - 页面置换算法实现
- **流水线** - 指令流水线执行
- **总线系统** - 总线仲裁和数据传输
- **I/O系统** - 设备控制和调度算法

## GO!
> readme是针对代码部分运行环境要求和怎么跑测试用例等进行解释

### 环境要求

- **操作系统**: macOS/Linux/Windows (WSL)
- **编译器**: GCC 4.8+ 或 Clang 3.5+
- **构建工具**: Make
- **内存**: 至少 1GB 可用内存

### 编译项目

```bash
# 克隆或下载项目
clone 项目
cd 计算机组成原理

# 编译所有模块
make

# 或者编译特定模块
cd simulation && make cpu_sim        # 编译CPU模拟器
cd simulation && make cache_sim      # 编译缓存模拟器
cd simulation && make io_sim         # 编译I/O模拟器
```

### 运行测试

```bash
# 运行所有测试
make test

# 运行特定模块测试
cd tests && make run-cpu         # CPU模拟器测试
cd tests && make run-cache       # 缓存模拟器测试
cd tests && make run-instruction # 指令系统测试
cd tests && make run-virtual-memory # 虚拟存储测试
cd tests && make run-alu         # ALU模拟器测试
cd tests && make run-bus         # 总线模拟器测试
cd tests && make run-io          # I/O系统测试
cd tests && make run-pipeline    # 流水线测试
```

### 清理构建文件

```bash
make clean
```

##  项目结构

```
计算机组成原理/
├── simulation/              # 核心模拟器代码
│   ├── common/             # 公共组件
│   │   ├── types.h         # 数据类型定义
│   │   ├── debug.h/.c      # 调试功能
│   │   ├── utils.h/.c      # 工具函数
│   │   └── test_framework.h/.c  # 测试框架
│   ├── cpu_sim/            # CPU模拟器
│   ├── instruction_sim/    # 指令系统
│   ├── cache_sim/          # 缓存模拟器
│   ├── virtual_memory_sim/ # 虚拟存储
│   ├── pipeline_sim/       # 流水线模拟器
│   ├── bus_sim/            # 总线模拟器
│   ├── io_sim/             # I/O系统模拟器
│   └── Makefile           # 构建配置
├── tests/                  # 测试套件
│   ├── unit_tests/         # 单元测试
│   ├── integration_tests/  # 集成测试
│   ├── performance_tests/  # 性能测试
│   └── Makefile           # 测试构建配置
├── docs/                   # 文档
├── build/                  # 构建输出目录
└── README.md              # 项目说明文档
```

##  测试用例说明

### 测试架构

项目采用分层测试架构：

1. **单元测试** - 测试单个模块的功能
2. **集成测试** - 测试模块间的协作
3. **性能测试** - 测试系统性能指标

### 单元测试详情

#### CPU模拟器测试 (`tests/unit_tests/cpu_core/`)
```bash
cd tests && make run-cpu
```
测试内容：
- CPU初始化和重置
- 寄存器读写操作
- 内存访问机制
- ALU运算功能
- 异常处理机制

#### 指令系统测试 (`tests/unit_tests/instruction/`)
```bash
cd tests && make run-instruction
```
测试内容：
- 8种寻址方式：立即、寄存器、直接、间接、基址、变址、相对寻址
- 指令类型：数据传送、算术运算、逻辑运算、程序控制
- 指令执行流程和程序运行
- 408考研真题场景模拟

#### 缓存模拟器测试 (`tests/unit_tests/cache/`)
```bash
cd tests && make run-cache
```
测试内容：
- 缓存初始化和配置
- 直接映射、组相联、全相联
- LRU、FIFO、Random替换算法
- 缓存命中率和性能统计
- 多级缓存层次结构

#### 虚拟存储测试 (`tests/unit_tests/virtual_memory/`)
```bash
cd tests && make run-virtual-memory
```
测试内容：
- 地址翻译机制 (虚拟地址 → 物理地址)
- TLB (Translation Lookaside Buffer)
- 页面置换算法：FIFO、LRU、OPT、Clock
- 缺页中断处理
- 408考研典型算法比较

#### ALU模拟器测试 (`tests/unit_tests/alu/`)
```bash
cd tests && make run-alu
```
测试内容：
- 算术运算：加减乘除、增减、取反
- 逻辑运算：与或非异或、移位、循环移位
- 比较运算和标志位设置
- 溢出检测和异常处理
- 408真题演示

#### 总线模拟器测试 (`tests/unit_tests/bus/`)
```bash
cd tests && make run-bus
```
测试内容：
- 总线初始化和配置
- 设备管理和仲裁算法
- 链式查询、优先级、轮询仲裁
- 总线操作和时序模拟
- 性能分析和统计

#### I/O系统测试 (`tests/unit_tests/io/`)
```bash
cd tests && make run-io
```
测试内容：
- I/O控制方式：程序查询、中断、DMA、通道
- 设备控制器模拟
- I/O调度算法：FCFS、优先级、SSTF电梯算法
- 中断控制器和DMA控制器
- 408考研场景：中断响应时间计算

#### 流水线测试 (`tests/unit_tests/pipeline/`)
```bash
cd tests && make run-pipeline
```
测试内容：
- 流水线初始化和配置
- 指令流水线执行
- 数据冲突和控制冲突
- 流水线性能分析

### 集成测试

```bash
cd tests && make test-integration
```

测试整个系统的协同工作，包括：
- CPU与内存系统交互
- 缓存与虚拟存储协作
- I/O与中断系统配合

### 性能测试

```bash
cd tests && make test-performance
```

评估系统性能指标：
- 指令执行速度
- 缓存命中率
- 内存访问延迟
- I/O吞吐量

## 使用示例

### 1. CPU指令执行示例

```c
#include "simulation/instruction_sim/instruction.h"

// 初始化指令模拟器
instruction_simulator_t sim;
instruction_sim_init(&sim, 4096, INST_FORMAT_32BIT);

// 创建简单程序：计算 100 + 200
instruction_t program[] = {
    {OP_MOV, ADDR_REGISTER, ADDR_IMMEDIATE, 0, 1, 100, 0, ADDR_TWO, 4},  // MOV R1, #100
    {OP_MOV, ADDR_REGISTER, ADDR_IMMEDIATE, 0, 2, 200, 0, ADDR_TWO, 4},  // MOV R2, #200
    {OP_ADD, ADDR_REGISTER, ADDR_REGISTER, 0, 1, 2, 0, ADDR_TWO, 4},     // ADD R1, R2
    {OP_HLT, 0, 0, 0, 0, 0, 0, ADDR_ZERO, 4}                            // HALT
};

// 加载并运行程序
instruction_sim_load_program(&sim, program, 4);
instruction_sim_run(&sim);

// 查看结果
printf("结果: R1 = %u\n", sim.registers.general[1]);  // 输出: 300
```

### 2. 缓存性能测试示例

```c
#include "simulation/cache_sim/cache.h"

// 初始化缓存：32KB, 8路组相联, 64B缓存行
cache_t cache;
cache_config_t config = {
    .size = 32768,
    .line_size = 64,
    .associativity = 8,
    .replacement_policy = CACHE_LRU
};
cache_init(&cache, &config);

// 模拟内存访问
for (int i = 0; i < 1000; i++) {
    u32 addr = i * 64;  // 访问不同缓存行
    cache_access(&cache, addr, CACHE_READ);
}

// 查看缓存统计
cache_stats_t stats = cache_get_stats(&cache);
printf("命中率: %.2f%%\n", stats.hit_rate * 100);
```

### 3. 虚拟存储页面置换示例

```c
#include "simulation/virtual_memory_sim/virtual_memory.h"

// 初始化虚拟存储系统：3个页框, LRU算法
virtual_memory_t vm;
vm_init(&vm, VM_MODE_PAGING, 3, VM_REPLACE_LRU);

// 模拟408考研经典序列
u32 sequence[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
for (int i = 0; i < 12; i++) {
    u32 vaddr = sequence[i] * VM_PAGE_SIZE;
    u32 paddr;
    vm_translate_address(&vm, vaddr, &paddr);
}

// 查看缺页统计
printf("缺页次数: %u\n", vm.stats.page_faults);
```

## 重点

本模拟器特别针对408考研计算机组成原理部分，包含：

### 考试重点内容
1. **指令系统** - 寻址方式、指令格式、程序执行
2. **CPU设计** - 数据通路、控制器、流水线
3. **存储系统** - 缓存、虚拟存储、存储层次
4. **I/O系统** - 控制方式、中断、DMA
5. **总线系统** - 仲裁、时序、性能
 

## 🔧 开发说明

### 代码规范与开发环境

#### 技术栈与标准
- **标准**: 遵循C99标准
- **编译器支持**: GCC 4.8+ / Clang 3.5+ / MSVC 2015+
- **构建系统**: GNU Make 3.8+
- **调试工具**: GDB, Valgrind, AddressSanitizer
- **代码质量**: 静态分析工具支持 (cppcheck, clang-analyzer)

#### 运行环境要求
- **运行环境**: 
  - macOS 10.12+ (推荐 macOS 12+)
  - Linux发行版: Ubuntu 16.04+, CentOS 7+, Debian 9+
  - Windows 10+ (需要WSL 2.0或MinGW-w64)
- **CPU架构**: x86-64, ARM64 (Apple Silicon)
- **内存要求**: 最小512MB，推荐2GB+
- **磁盘空间**: 编译需要100MB，完整测试需要200MB

#### 编译器配置
```bash
# GCC编译标志
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
CFLAGS += -fPIC -pthread                    # 位置无关代码和线程支持
CFLAGS += -D_GNU_SOURCE                     # GNU扩展特性
CFLAGS += -march=native                     # CPU指令集优化

# 调试版本
CFLAGS_DEBUG = -Wall -Wextra -std=c99 -g -O0
CFLAGS_DEBUG += -DDEBUG -fsanitize=address  # 内存检查
CFLAGS_DEBUG += -fno-omit-frame-pointer     # 调试信息保留

# 链接器配置
LDFLAGS = -lm -lpthread                     # 数学库和线程库
```

#### 代码风格规范
- **命名约定**:
  - 结构体: `snake_case_t` (如: `cpu_core_t`, `cache_config_t`)
  - 函数: `snake_case` (如: `cpu_init`, `cache_access`)
  - 宏定义: `UPPER_CASE` (如: `CPU_SUCCESS`, `MAX_REGISTERS`)
  - 局部变量: `snake_case` (如: `result_value`, `temp_addr`)
  - 全局变量: `g_` 前缀 (如: `g_debug_level`)

- **文件组织**:
  - 头文件: `.h` 扩展名，包含接口声明
  - 源文件: `.c` 扩展名，包含实现代码
  - 测试文件: `test_*.c` 命名格式
  - 每个模块独立目录，避免循环依赖

#### 开发工具链
```bash
# 代码格式化
clang-format -style=file -i *.c *.h

# 静态分析
cppcheck --enable=all --std=c99 src/

# 内存检查
valgrind --tool=memcheck --leak-check=full ./test_program

# 性能分析
perf record -g ./test_program
perf report
```

####  文档表格规范
本项目已全面优化表格显示效果，统一使用markdown标准表格格式：

**表格优化亮点**：
-  **替代ASCII表格**：所有ASCII绘制的表格已优化为markdown格式
-  **统一样式规范**：遵循 `docs/表格样式指南.md` 中的标准
-  **响应式设计**：支持不同屏幕尺寸和移动端查看
-  **增强可读性**：清晰的表头、合理的对齐、一致的格式

**查看表格指南**：
```bash
# 查看表格样式规范
cat docs/表格样式指南.md

# 示例：优化后的中断向量表格
# | 中断号 | 向量地址 | 中断类型 | 处理程序 |
# |--------|----------|----------|----------|  
# | 0      | 0x0000   | 除零异常 | div_zero_handler |
```

**支持的表格类型**：
-  指令格式表格（操作码、地址码结构）
-  性能对比表格（时间、频率、带宽分析）  
-  功能对比表格（特性、应用场景对比）
-  配置参数表格（系统设置、技术规格）
-  数据结构表格（寄存器、内存组织）

### 扩展开发
如需添加新功能：

1. 在对应的模拟器目录添加源码
2. 在tests目录添加测试用例
3. 更新Makefile构建配置
4. 运行测试确保功能正确

### 调试技巧
```bash
# 启用调试模式编译
make DEBUG=1

# 使用GDB调试
gdb ./tests/test_cpu
```


## 贡献

欢迎提交Issue和Pull Request来改进项目！

## 许可证

本项目仅用于学习目的，没有添加遵循任何许可证，可以自由使用。

## 联系方式

如有问题或建议，请通过以下方式联系：
- 项目Issues页面
- 邮件咨询

---

 
