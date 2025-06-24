#ifndef TABLE_UTILS_H
#define TABLE_UTILS_H

#include <stdio.h>
#include <string.h>

// 表格绘制辅助函数
void print_table_header(const char* title, int width);
void print_table_row(const char* left, const char* right, int width);
void print_table_separator(int width);
void print_table_footer(int width);

// 特殊格式输出
void print_memory_dump_header(void);
void print_register_display_header(void);
void print_performance_header(void);

#endif // TABLE_UTILS_H
