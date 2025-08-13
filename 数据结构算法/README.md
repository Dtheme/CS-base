 #  数据结构算法 

参考书目: 
1. 算法导论
2. 王道论坛-数据结构
 
## 关于代码

### 一键操作

```bash
#  查看项目信息和章节状态
make info

# 构建所有章节（默认目标）
make

# 运行所有测试
make test-all

# 清理所有构建文件
make clean-all

# 快速验证所有代码语法
make verify
```

###  单章节操作

```bash
# 构建指定章节
make build-chapter CHAPTER=1.线性表
make build-chapter CHAPTER=2.栈和队列
make build-chapter CHAPTER=4.图数据结构

#  测试指定章节
make test-chapter CHAPTER=1.线性表
make test-chapter CHAPTER=2.栈和队列
make test-chapter CHAPTER=4.图数据结构

# 清理指定章节
make clean-chapter CHAPTER=1.线性表
``` 
##  使用场景示例

###  场景1：初次使用
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

###  使用某个章节
```bash
# 专门学习线性表（包含顺序表、链表、循环链表）
make test-chapter CHAPTER=1.线性表

# 专门学习栈和队列
make test-chapter CHAPTER=2.栈和队列

# 专门学习图数据结构（图论算法完整体系）
make test-chapter CHAPTER=4.图数据结构
```

###  添加代码
```bash
# 验证代码修改（语法检查）
make verify

# 格式化代码
make format

# 构建指定章节
make build-chapter CHAPTER=1.线性表
```
 

##  环境

- **gcc**: C编译器
- **make**: 构建工具

### 可选工具
- **gdb**: 调试器
- **valgrind**: 内存检测工具
- **clang-format**: 代码格式化
- **cloc**: 代码统计工具

**一键检查工具安装**：
```bash
make install-tools
```

### 编译选项

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

## 常见问题

** 构建失败**：
```bash
# 清理后重新构建
make clean-all
make build-all
```

** 权限问题**：
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

** 路径问题**：
```bash
# 确保在项目根目录
pwd  # 应该显示 .../数据结构算法
ls   # 应该看到 Makefile README.md 等文件
```

### debug模式
```bash
# 详细输出构建过程
make build-all VERBOSE=1

# 调试编译选项
make build-all CFLAGS="-Wall -g -O0 -DDEBUG"
```
