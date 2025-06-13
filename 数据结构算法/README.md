 #  数据结构算法  - 统一学习体系

>  **以主Makefile为核心的完整 数据结构学习项目**

## 📖 项目概述

本项目是专为 数据结构算法复习而建立的**完整学习体系**，采用**统一构建系统**管理，包含：

- 🔬 **理论文档**：每个章节的详细理论分析和考点总结
- 💻 **代码实现**：完整的C语言实现，包含详细注释
- 🧪 **测试程序**：全面的测试用例和性能分析
-  **统一构建**：一键编译、测试、验证所有章节

##  工程结构

```
数据结构算法/
├── Makefile                 #  主构建系统（核心入口）
├── README.md               # 📋 项目说明文档
├── build/                  # 📦 构建输出目录
├── 1.线性表/               #  第一章：线性表
│   ├── 1.1线性表基础理论.md
│   ├── 1.2顺序表实现与分析.md
│   ├── 1.3链表实现与分析.md
│   └── code/               # 💻 代码实现
│       ├── Makefile        # 章节构建文件
│       ├── sqlist.h/.c     # 顺序表实现
│       ├── linklist.h/.c   # 链表实现（单链表+双链表+循环链表）
│       ├── test_sqlist.c   # 顺序表测试
│       ├── test_linklist.c # 链表测试
│       └── test_circular.c # 循环链表测试
├── 2.栈和队列/             #  第二章：栈和队列
│   └── code/               # 💻 代码实现
│       ├── Makefile        # 章节构建文件
│       ├── stack.h/.c      # 栈实现
│       ├── queue.h/.c      # 队列实现
│       ├── test_stack.c    # 栈测试
│       └── test_queue.c    # 队列测试
├── 3.树和二叉树/           #  第三章：树和二叉树
│   └── code/               # 💻 代码实现
│       └── ...
├── 4.图数据结构/           #  第四章：图数据结构  NEW!
│   ├── 4.1图的基础理论.md        #  图的定义、术语、性质
│   ├── 4.2图的存储结构与实现.md  # 💾 邻接矩阵、邻接表存储
│   ├── 4.3图的遍历算法.md        #  DFS、BFS遍历算法
│   ├── 4.4图的连通性与最小生成树.md # 🌲 连通性、Prim、Kruskal
│   ├── 4.5最短路径算法.md        #  Dijkstra、Floyd算法
│   ├── 4.6其他图算法.md          #  拓扑排序、关键路径、二分图
│   └── code/               # 💻 代码实现
│       └── ...
├── 6.排序算法/              #  排序算法（已完成）
│   ├── README.md           # 📖 排序算法学习指南
│   └── code/               # 💻 代码实现
│       ├── sort.h          # 排序算法头文件
│       ├── sort.c          # 排序算法实现
│       ├── sort_test.c     # 测试和分析函数
│       ├── test_sort.c     # 主测试程序
│       └── Makefile        # 编译配置
└── ...                     #  其他章节
```

##  快速开始

### ⚡ 一键操作

```bash
# 📋 查看项目信息和章节状态
make info

# 🔨 构建所有章节（默认目标）
make

# 🧪 运行所有测试
make test-all

# 🧹 清理所有构建文件
make clean-all

#  快速验证所有代码语法
make verify
```

###  单章节操作

```bash
# 🔨 构建指定章节
make build-chapter CHAPTER=1.线性表
make build-chapter CHAPTER=2.栈和队列
make build-chapter CHAPTER=4.图数据结构

# 🧪 测试指定章节
make test-chapter CHAPTER=1.线性表
make test-chapter CHAPTER=2.栈和队列
make test-chapter CHAPTER=4.图数据结构

# 🧹 清理指定章节
make clean-chapter CHAPTER=1.线性表
```

###  开发者工具

```bash
#   查看代码统计信息
make stats

#  格式化所有代码
make format

#  检查开发工具安装情况
make install-tools

# 📦 创建发布包
make dist
```

###  完整工作流程

```bash
#  完整开发流程（清理→验证→构建→测试）
make clean-all && make verify && make build-all && make test-all

# ❓ 查看完整帮助信息
make help
```

##  使用场景示例

### 🔰 场景1：初次使用
```bash
# 1. 查看项目概况
make info

# 2. 验证开发环境
make install-tools

# 3. 构建所有章节
make

# 4. 运行测试验证
make test-all
```

### 📖 场景2：专项学习
```bash
# 专门学习线性表（包含顺序表、链表、循环链表）
make test-chapter CHAPTER=1.线性表

# 专门学习栈和队列
make test-chapter CHAPTER=2.栈和队列

# 专门学习图数据结构（图论算法完整体系）
make test-chapter CHAPTER=4.图数据结构
```

### 🔧 场景3：代码开发
```bash
# 验证代码修改（语法检查）
make verify

# 格式化代码
make format

# 构建指定章节
make build-chapter CHAPTER=1.线性表
```

###  场景4：项目维护
```bash
# 查看代码统计
make stats

# 清理重建
make clean-all
make build-all

# 创建发布包
make dist
```

##  环境配置

###  必需工具
- **gcc**: C编译器
- **make**: 构建工具

### 🔧 推荐工具（可选）
- **gdb**: 调试器
- **valgrind**: 内存检测工具
- **clang-format**: 代码格式化
- **cloc**: 代码统计工具

**一键检查工具安装**：
```bash
make install-tools
```

###  高级编译选项

**默认配置**：
- `-Wall -Wextra`: 启用所有警告
- `-std=c99`: 使用C99标准
- `-g`: 包含调试信息
- `-O2`: 优化等级2

**自定义编译**：
```bash
# 使用clang编译器
CC=clang make build-all

# 调试模式编译
CFLAGS="-Wall -g -O0 -DDEBUG" make build-all

# 4线程并行编译
make -j4 build-all
```

## 🔧 故障排除

### 常见问题解决

**🚫 构建失败**：
```bash
# 清理后重新构建
make clean-all
make build-all
```

**🔒 权限问题**：
```bash
# 确保文件可执行权限
chmod +x Makefile
find . -name "Makefile" -exec chmod +x {} \;
```

** 工具缺失**：
```bash
# 检查开发环境
make install-tools
```

**📁 路径问题**：
```bash
# 确保在项目根目录
pwd  # 应该显示 .../数据结构算法
ls   # 应该看到 Makefile README.md 等文件
```

###  调试模式
```bash
# 详细输出构建过程
make build-all VERBOSE=1

# 调试编译选项
make build-all CFLAGS="-Wall -g -O0 -DDEBUG"
```
