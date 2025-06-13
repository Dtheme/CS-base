# ====================================================================
# 数据结构算法
# create by：zw.duan
# ====================================================================

# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = 
LIBS = -lm

# 项目基本信息
PROJECT_NAME = 数据结构算法408
VERSION = 2.1
BUILD_DIR = build
DIST_DIR = dist

# 自动发现所有章节目录
CHAPTERS := $(wildcard [0-9].*/)
CHAPTER_NAMES := $(patsubst %/,%,$(CHAPTERS))

# 颜色定义（用于美化输出）
COLOR_RESET = \033[0m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_RED = \033[31m
COLOR_BLUE = \033[34m
COLOR_CYAN = \033[36m

# 默认目标
.PHONY: all
all: info build-all

# 显示项目信息
.PHONY: info
info:
	@echo "$(COLOR_CYAN)╔════════════════════════════════════════════════════════════╗$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)║              $(PROJECT_NAME)                 ║$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)║                        版本：$(VERSION)                        ║$(COLOR_RESET)"
	@echo "$(COLOR_CYAN)╚════════════════════════════════════════════════════════════╝$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BLUE)发现的章节目录：$(COLOR_RESET)"
	@for chapter in $(CHAPTER_NAMES); do \
		echo "  $(COLOR_GREEN)✓$(COLOR_RESET) $$chapter"; \
	done
	@echo ""

# 构建所有章节
.PHONY: build-all
build-all:
	@echo "$(COLOR_YELLOW)开始构建所有章节...$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	@for chapter in $(CHAPTER_NAMES); do \
		echo "$(COLOR_BLUE)构建章节：$$chapter$(COLOR_RESET)"; \
		$(MAKE) -C "$$chapter/code" CC="$(CC)" CFLAGS="$(CFLAGS)" || exit 1; \
		echo "$(COLOR_GREEN)✓ $$chapter 构建完成$(COLOR_RESET)"; \
	done
	@echo "$(COLOR_GREEN)所有章节构建完成！$(COLOR_RESET)"

# 运行所有测试
.PHONY: test-all
test-all:
	@echo "$(COLOR_YELLOW)运行所有章节的测试...$(COLOR_RESET)"
	@for chapter in $(CHAPTER_NAMES); do \
		echo "$(COLOR_BLUE)测试章节：$$chapter$(COLOR_RESET)"; \
		$(MAKE) -C "$$chapter/code" test || echo "$(COLOR_RED)✗ $$chapter 测试失败$(COLOR_RESET)"; \
		echo "$(COLOR_GREEN)✓ $$chapter 测试完成$(COLOR_RESET)"; \
	done
	@echo "$(COLOR_GREEN)所有测试完成！$(COLOR_RESET)"

# 清理所有构建文件
.PHONY: clean-all
clean-all:
	@echo "$(COLOR_YELLOW)清理所有构建文件...$(COLOR_RESET)"
	@for chapter in $(CHAPTER_NAMES); do \
		echo "清理 $$chapter"; \
		$(MAKE) -C "$$chapter/code" clean 2>/dev/null || true; \
	done
	@rm -rf $(BUILD_DIR) $(DIST_DIR)
	@echo "$(COLOR_GREEN)清理完成！$(COLOR_RESET)"

# 特定章节操作
.PHONY: build-chapter
build-chapter:
	@if [ -z "$(CHAPTER)" ]; then \
		echo "$(COLOR_RED)错误：请指定章节名称，例如：make build-chapter CHAPTER=1.线性表$(COLOR_RESET)"; \
		exit 1; \
	fi
	@if [ ! -d "$(CHAPTER)/code" ]; then \
		echo "$(COLOR_RED)错误：章节 $(CHAPTER) 不存在或没有code目录$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_BLUE)构建章节：$(CHAPTER)$(COLOR_RESET)"
	@$(MAKE) -C "$(CHAPTER)/code" CC="$(CC)" CFLAGS="$(CFLAGS)"
	@echo "$(COLOR_GREEN)✓ $(CHAPTER) 构建完成$(COLOR_RESET)"

.PHONY: test-chapter
test-chapter:
	@if [ -z "$(CHAPTER)" ]; then \
		echo "$(COLOR_RED)错误：请指定章节名称，例如：make test-chapter CHAPTER=1.线性表$(COLOR_RESET)"; \
		exit 1; \
	fi
	@if [ ! -d "$(CHAPTER)/code" ]; then \
		echo "$(COLOR_RED)错误：章节 $(CHAPTER) 不存在或没有code目录$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_BLUE)测试章节：$(CHAPTER)$(COLOR_RESET)"
	@$(MAKE) -C "$(CHAPTER)/code" test
	@echo "$(COLOR_GREEN)✓ $(CHAPTER) 测试完成$(COLOR_RESET)"

.PHONY: clean-chapter
clean-chapter:
	@if [ -z "$(CHAPTER)" ]; then \
		echo "$(COLOR_RED)错误：请指定章节名称，例如：make clean-chapter CHAPTER=1.线性表$(COLOR_RESET)"; \
		exit 1; \
	fi
	@if [ ! -d "$(CHAPTER)/code" ]; then \
		echo "$(COLOR_RED)错误：章节 $(CHAPTER) 不存在或没有code目录$(COLOR_RESET)"; \
		exit 1; \
	fi
	@echo "$(COLOR_YELLOW)清理章节：$(CHAPTER)$(COLOR_RESET)"
	@$(MAKE) -C "$(CHAPTER)/code" clean
	@echo "$(COLOR_GREEN)✓ $(CHAPTER) 清理完成$(COLOR_RESET)"

# 创建发布包
.PHONY: dist
dist: clean-all build-all
	@echo "$(COLOR_YELLOW)创建发布包...$(COLOR_RESET)"
	@mkdir -p $(DIST_DIR)
	@tar -czf $(DIST_DIR)/$(PROJECT_NAME)-v$(VERSION).tar.gz \
		--exclude='.git*' \
		--exclude='$(BUILD_DIR)' \
		--exclude='$(DIST_DIR)' \
		--exclude='*.o' \
		--exclude='*.exe' \
		--transform 's,^,$(PROJECT_NAME)-v$(VERSION)/,' \
		*
	@echo "$(COLOR_GREEN)✓ 发布包已创建：$(DIST_DIR)/$(PROJECT_NAME)-v$(VERSION).tar.gz$(COLOR_RESET)"

# 代码格式化
.PHONY: format
format:
	@echo "$(COLOR_YELLOW)格式化代码...$(COLOR_RESET)"
	@find . -name "*.c" -o -name "*.h" | grep -v "$(BUILD_DIR)" | xargs -I {} sh -c \
		'if command -v clang-format >/dev/null 2>&1; then \
			echo "格式化：{}"; \
			clang-format -i -style="{IndentWidth: 4, TabWidth: 4, UseTab: Never}" "{}"; \
		else \
			echo "$(COLOR_YELLOW)警告：clang-format 未安装，跳过格式化$(COLOR_RESET)"; \
			exit 0; \
		fi'
	@echo "$(COLOR_GREEN)✓ 代码格式化完成$(COLOR_RESET)"

# 代码统计
.PHONY: stats
stats:
	@echo "$(COLOR_CYAN)代码统计信息：$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BLUE)文件统计：$(COLOR_RESET)"
	@find . -name "*.c" -o -name "*.h" | grep -v "$(BUILD_DIR)" | wc -l | xargs echo "  C/C++源文件："
	@find . -name "*.md" | wc -l | xargs echo "  Markdown文档："
	@find . -name "Makefile*" | wc -l | xargs echo "  Makefile："
	@echo ""
	@echo "$(COLOR_BLUE)代码行数统计：$(COLOR_RESET)"
	@if command -v cloc >/dev/null 2>&1; then \
		cloc --exclude-dir=$(BUILD_DIR),$(DIST_DIR) .; \
	else \
		echo "  $(COLOR_YELLOW)提示：安装 cloc 工具可获得详细的代码统计$(COLOR_RESET)"; \
		find . -name "*.c" -o -name "*.h" | grep -v "$(BUILD_DIR)" | xargs wc -l | tail -1 | awk '{print "  C/C++代码总行数：" $$1}'; \
	fi

# 快速验证所有代码
.PHONY: verify
verify:
	@echo "$(COLOR_YELLOW)快速验证所有代码...$(COLOR_RESET)"
	@error_count=0; \
	for chapter in $(CHAPTER_NAMES); do \
		echo "$(COLOR_BLUE)验证章节：$$chapter$(COLOR_RESET)"; \
		if [ -d "$$chapter/code" ]; then \
			cd "$$chapter/code"; \
			for c_file in *.c; do \
				if [ -f "$$c_file" ]; then \
					echo "  检查语法：$$c_file"; \
					if ! $(CC) $(CFLAGS) -fsyntax-only "$$c_file" 2>/dev/null; then \
						echo "$(COLOR_RED)  ✗ $$c_file 语法错误$(COLOR_RESET)"; \
						error_count=$$((error_count + 1)); \
					else \
						echo "$(COLOR_GREEN)  ✓ $$c_file 语法正确$(COLOR_RESET)"; \
					fi; \
				fi; \
			done; \
			cd ../..; \
		else \
			echo "$(COLOR_YELLOW)  警告：$$chapter 没有code目录$(COLOR_RESET)"; \
		fi; \
	done; \
	if [ $$error_count -eq 0 ]; then \
		echo "$(COLOR_GREEN)✓ 所有代码验证通过！$(COLOR_RESET)"; \
	else \
		echo "$(COLOR_RED)✗ 发现 $$error_count 个语法错误$(COLOR_RESET)"; \
		exit 1; \
	fi

# 安装依赖工具（仅在某些系统上有效）
.PHONY: install-tools
install-tools:
	@echo "$(COLOR_YELLOW)检查并推荐安装开发工具...$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BLUE)推荐的开发工具：$(COLOR_RESET)"
	@command -v gcc >/dev/null 2>&1 && echo "$(COLOR_GREEN)✓ gcc$(COLOR_RESET)" || echo "$(COLOR_RED)✗ gcc$(COLOR_RESET) - 请安装GCC编译器"
	@command -v make >/dev/null 2>&1 && echo "$(COLOR_GREEN)✓ make$(COLOR_RESET)" || echo "$(COLOR_RED)✗ make$(COLOR_RESET) - 请安装Make构建工具"
	@command -v gdb >/dev/null 2>&1 && echo "$(COLOR_GREEN)✓ gdb$(COLOR_RESET)" || echo "$(COLOR_YELLOW)? gdb$(COLOR_RESET) - 推荐安装GDB调试器"
	@command -v valgrind >/dev/null 2>&1 && echo "$(COLOR_GREEN)✓ valgrind$(COLOR_RESET)" || echo "$(COLOR_YELLOW)? valgrind$(COLOR_RESET) - 推荐安装Valgrind内存检测工具"
	@command -v clang-format >/dev/null 2>&1 && echo "$(COLOR_GREEN)✓ clang-format$(COLOR_RESET)" || echo "$(COLOR_YELLOW)? clang-format$(COLOR_RESET) - 推荐安装代码格式化工具"
	@command -v cloc >/dev/null 2>&1 && echo "$(COLOR_GREEN)✓ cloc$(COLOR_RESET)" || echo "$(COLOR_YELLOW)? cloc$(COLOR_RESET) - 推荐安装代码统计工具"

# 帮助信息
.PHONY: help
help:
	@echo "$(COLOR_CYAN)数据结构算法  - 统一构建系统帮助$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_BLUE)基本命令：$(COLOR_RESET)"
	@echo "  make                   显示项目信息并构建所有章节"
	@echo "  make build-all         构建所有章节"
	@echo "  make test-all          运行所有章节的测试"
	@echo "  make clean-all         清理所有构建文件"
	@echo "  make verify            快速验证所有代码语法"
	@echo ""
	@echo "$(COLOR_BLUE)单章节操作：$(COLOR_RESET)"
	@echo "  make build-chapter CHAPTER=1.线性表    构建指定章节"
	@echo "  make test-chapter CHAPTER=1.线性表     测试指定章节"
	@echo "  make clean-chapter CHAPTER=1.线性表    清理指定章节"
	@echo ""
	@echo "$(COLOR_BLUE)开发工具：$(COLOR_RESET)"
	@echo "  make format            格式化所有代码"
	@echo "  make stats             显示代码统计信息"
	@echo "  make install-tools     检查开发工具安装情况"
	@echo ""
	@echo "$(COLOR_BLUE)发布管理：$(COLOR_RESET)"
	@echo "  make dist              创建发布包"
	@echo ""
	@echo "$(COLOR_BLUE)示例用法：$(COLOR_RESET)"
	@echo "  make build-chapter CHAPTER=1.线性表"
	@echo "  make test-chapter CHAPTER=1.线性表"
	@echo "  CC=clang make build-all"
	@echo ""
	@echo "$(COLOR_BLUE)当前发现的章节：$(COLOR_RESET)"
	@for chapter in $(CHAPTER_NAMES); do \
		echo "  $(COLOR_GREEN)$(chapter)$(COLOR_RESET)"; \
	done

# 开发者快捷命令
.PHONY: dev
dev: clean-all verify build-all test-all
	@echo "$(COLOR_GREEN)✓ 开发者完整验证流程完成！$(COLOR_RESET)"

.PHONY: quick
quick: build-all
	@echo "$(COLOR_GREEN)✓ 快速构建完成！$(COLOR_RESET)"

# 默认显示帮助（当用户不知道如何使用时）
.PHONY: usage
usage: help

# 确保这些目标不会与文件名冲突
.PHONY: all info build-all test-all clean-all build-chapter test-chapter clean-chapter
.PHONY: dist format stats verify install-tools help dev quick usage 