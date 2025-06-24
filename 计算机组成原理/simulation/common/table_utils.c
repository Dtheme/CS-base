#include "table_utils.h"

void print_table_header(const char* title, int width) {
    printf("┌");
    for (int i = 0; i < width; i++) printf("─");
    printf("┐\n");
    
    int title_len = strlen(title);
    int padding = (width - title_len) / 2;
    printf("│%*s%s%*s│\n", padding, "", title, width - padding - title_len, "");
    
    printf("├");
    for (int i = 0; i < width; i++) printf("─");
    printf("┤\n");
}

void print_table_row(const char* left, const char* right, int width) {
    int left_len = strlen(left);
    int right_len = strlen(right);
    int middle_space = width - left_len - right_len - 3;
    
    printf("│ %s", left);
    for (int i = 0; i < middle_space; i++) printf(" ");
    printf("%s │\n", right);
}

void print_table_separator(int width) {
    printf("├");
    for (int i = 0; i < width; i++) printf("─");
    printf("┤\n");
}

void print_table_footer(int width) {
    printf("└");
    for (int i = 0; i < width; i++) printf("─");
    printf("┘\n");
}

void print_memory_dump_header(void) {
    printf("┌─────────────┬─────────────────────────────────────────────────┬─────────────────────┐\n");
    printf("│    地址     │                  十六进制数据                  │      ASCII字符      │\n");
    printf("├─────────────┼─────────────────────────────────────────────────┼─────────────────────┤\n");
}

void print_register_display_header(void) {
    printf("┌───────────────────────── 寄存器状态 ─────────────────────────┐\n");
    printf("│                                                               │\n");
}

void print_performance_header(void) {
    printf("┌─────────────────────── 📊 性能统计 ───────────────────────┐\n");
}
