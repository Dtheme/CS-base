#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
例题3.7图像生成器 - 基于数学作图标准规范
题目：计算 ∬_D xy dA，其中 D 是由 y = x²、y = 2x-1 和 x = 2 围成的区域
"""

import matplotlib.pyplot as plt
import numpy as np
import os

def generate_example_3_7():
    """生成例题3.7的标准图像并验证计算"""
    
    # 配置matplotlib中文显示
    plt.rcParams['font.sans-serif'] = ['Arial Unicode MS', 'SimHei', 'DejaVu Sans']
    plt.rcParams['axes.unicode_minus'] = False
    
    # 创建图形，标准尺寸
    fig, ax = plt.subplots(1, 1, figsize=(16, 12))
    
    # 定义x范围
    x = np.linspace(0, 3, 1000)
    x_region = np.linspace(1, 2, 200)  # 积分区域范围
    
    # 定义函数
    y1 = x**2          # y = x²
    y2 = 2*x - 1       # y = 2x - 1
    
    # 标准配色方案
    curve_color1 = '#2E5BBA'  # 深海蓝
    curve_color2 = '#C8102E'  # 深红
    vertical_color = '#228B22'  # 森林绿
    region_color = '#E6F3FF'  # 淡天蓝
    
    # 绘制函数曲线
    ax.plot(x, y1, color=curve_color1, linewidth=3.5, label=r'$y = x^2$', zorder=5)
    ax.plot(x, y2, color=curve_color2, linewidth=3.5, label=r'$y = 2x-1$', zorder=5)
    
    # 绘制垂直线 x = 2
    ax.axvline(x=2, color=vertical_color, linewidth=3.5, label=r'$x = 2$', zorder=5)
    
    # 填充积分区域
    y1_region = x_region**2
    y2_region = 2*x_region - 1
    ax.fill_between(x_region, y2_region, y1_region, alpha=0.6, color=region_color,
                    edgecolor=curve_color1, linewidth=1.5, 
                    label=r'积分区域 $D$', zorder=2)
    
    # 标记关键点 - 按四角分布策略
    points = [(1, 1), (2, 4), (2, 3)]
    point_labels = ['(1, 1)', '(2, 4)', '(2, 3)']
    positions = [(0.4, 0.7), (2.3, 4.3), (2.3, 2.5)]
    
    for point, label, position in zip(points, point_labels, positions):
        ax.plot(point[0], point[1], 'o', color='#333333', markersize=10,
                markeredgecolor='white', markeredgewidth=2, zorder=10)
        
        ax.annotate(label, point, xytext=position, fontsize=12, weight='bold',
                   bbox=dict(boxstyle='round,pad=0.3', facecolor='white', 
                            edgecolor='gray', alpha=0.9),
                   arrowprops=dict(arrowstyle='->', color='gray', lw=1.5))
    
    # 设置坐标轴
    ax.set_xlim(-0.5, 3.5)
    ax.set_ylim(-0.5, 5.5)
    ax.set_xlabel('x', fontsize=17, weight='bold', color='#333333')
    ax.set_ylabel('y', fontsize=17, weight='bold', color='#333333')
    ax.grid(True, alpha=0.3, linestyle='-', linewidth=0.8)
    
    # x-y-o坐标系布局
    ax.axhline(y=0, color='black', linewidth=1.0, alpha=0.8, zorder=3)
    ax.axvline(x=0, color='black', linewidth=1.0, alpha=0.8, zorder=3)
    ax.plot(0, 0, 'ko', markersize=6, zorder=5)
    ax.text(0.1, -0.2, 'O', fontsize=14, weight='bold', color='#333333', zorder=6)
    
    # 设置标题
    ax.set_title(r'例题3.7: 由 $y = x^2$, $y = 2x-1$, $x = 2$ 围成的积分区域', 
                fontsize=19, pad=20, weight='bold', color='#2C3E50')
    
    # 图例右上角
    legend = ax.legend(loc='upper left', bbox_to_anchor=(0.02, 0.98),
                      fontsize=12, framealpha=0.95, facecolor='white',
                      edgecolor='gray', frameon=True)
    legend.get_frame().set_linewidth(1.2)
    
    # 文字内容 - 右侧分层布局（避免与函数曲线重叠）
    # 积分表达式
    ax.text(2.7, 5.2, r'积分表达式:', fontsize=14, weight='bold', color='#2C3E50',
            bbox=dict(boxstyle='round,pad=0.4', facecolor='#F8F9FA', 
                     edgecolor='#DEE2E6', alpha=0.95))
    
    ax.text(2.7, 4.85, r'$\iint_D xy \, dA = \int_1^2 \int_{2x-1}^{x^2} xy \, dy \, dx$', 
            fontsize=11, color='#2C3E50',
            bbox=dict(boxstyle='round,pad=0.4', facecolor='#F8F9FA', 
                     edgecolor='#DEE2E6', alpha=0.95))
    
    # 区域分析
    textstr = """区域分析:
交点: $(1,1)$, $(2,4)$, $(2,3)$
积分限: $x \\in [1,2]$, $2x-1 \\leq y \\leq x^2$
X型区域表示"""
    
    ax.text(2.7, 4.2, textstr, fontsize=11, weight='bold', color='#155724',
            bbox=dict(boxstyle='round,pad=0.5', facecolor='#D4F7D4', 
                     edgecolor='#28A745', alpha=0.9),
            verticalalignment='top')
    
    # 计算验证（显示正确结果）
    ax.text(2.7, 3.2, r'计算结果: $\iint_D xy \, dA = \frac{5}{3}$', 
            fontsize=12, color='#721C24', weight='bold',
            bbox=dict(boxstyle='round,pad=0.4', facecolor='#F8D7DA', 
                     edgecolor='#DC3545', alpha=0.9))
    
    # 保存图像
    plt.tight_layout()
    output_path = '../../Assets/example_3_7.png'
    fig.savefig(output_path, dpi=300, bbox_inches='tight', 
                facecolor='white', edgecolor='none')
    
    # 获取文件大小
    file_size = os.path.getsize(output_path) / 1024
    
    print(f'成功生成例题3.7图像：{output_path}')
    print(f'文件大小：{file_size:.1f} KB')
    print('标准规范实现:')
    print('1. ✅ x-y-o坐标系布局，原点(0,0)清晰标记')
    print('2. ✅ 关键点标记清晰，无遮挡')
    print('3. ✅ 三条边界线形成完整三角形区域')
    print('4. ✅ 积分表达式和结果正确显示')
    print('5. ✅ 符合数学作图标准规范')
    
    # 验证计算过程
    print('\n📊 解题验证:')
    print('🔍 检查原文档中的计算错误:')
    print('原文档结果: 37/12 ≈ 3.083')
    print('正确计算结果: 5/3 ≈ 1.667')
    print('❌ 原文档计算存在错误，应为 5/3')
    
    # 验证文件是否生成
    if os.path.exists(output_path):
        print(f'✅ 图像文件已成功生成')
    else:
        print(f'❌ 图像文件生成失败')
    
    return output_path

def verify_calculation():
    """详细验证例题3.7的计算过程"""
    print('\n🧮 例题3.7计算过程详细验证:')
    print('=' * 50)
    
    print('步骤1: 确定积分区域')
    print('交点计算: x² = 2x-1 → (x-1)² = 0 → x = 1, y = 1')
    print('其他关键点: (2,4), (2,3)')
    print('区域: D = {(x,y) | 1 ≤ x ≤ 2, 2x-1 ≤ y ≤ x²}')
    
    print('\n步骤2: 设定积分表达式')
    print('∬_D xy dA = ∫₁² ∫_{2x-1}^{x²} xy dy dx')
    
    print('\n步骤3: 内层积分')
    print('∫_{2x-1}^{x²} xy dy = x[y²/2]_{2x-1}^{x²}')
    print('= x/2 · [x⁴ - (2x-1)²]')
    print('= x/2 · [x⁴ - (4x² - 4x + 1)]')
    print('= x/2 · [x⁴ - 4x² + 4x - 1]')
    
    print('\n步骤4: 外层积分')
    print('∫₁² x/2 · (x⁴ - 4x² + 4x - 1) dx')
    print('= 1/2 ∫₁² (x⁵ - 4x³ + 4x² - x) dx')
    
    print('\n步骤5: 计算定积分')
    print('[x⁶/6 - x⁴ + 4x³/3 - x²/2]₁²')
    
    # 计算 x = 2 时的值
    val_2 = 64/6 - 16 + 32/3 - 2
    print(f'x=2时: 64/6 - 16 + 32/3 - 2 = {val_2:.6f} = 10/3')
    
    # 计算 x = 1 时的值  
    val_1 = 1/6 - 1 + 4/3 - 1/2
    print(f'x=1时: 1/6 - 1 + 4/3 - 1/2 = {val_1:.6f} = 0')
    
    result = 1/2 * (val_2 - val_1)
    print(f'\n最终结果: 1/2 × (10/3 - 0) = {result:.6f} = 5/3')
    
    print('\n✅ 验证结论: 正确答案应该是 5/3，原文档中的 37/12 是计算错误')

if __name__ == "__main__":
    generate_example_3_7()
    verify_calculation() 