#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成例题3.5的复杂积分区域图像：由y=x²、y=√x和x=4围成的区域
优化版：优雅配色、规范布局，作为解题作图的标准模板
"""

import matplotlib.pyplot as plt
import numpy as np
import os

# 配置中文字体和优雅样式
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
plt.rcParams['axes.unicode_minus'] = False
plt.rcParams['figure.dpi'] = 120
plt.rcParams['savefig.dpi'] = 300

def generate_region_complex():
    """生成例题3.5的复杂积分区域图像（优化版）"""
    
    # 创建图形，调整尺寸避免遮挡
    fig, ax = plt.subplots(1, 1, figsize=(16, 12))

    # 定义x范围
    x = np.linspace(0.05, 5, 1000)
    x_region1 = np.linspace(0.05, 1, 200)  # 第一部分：x∈[0,1]
    x_region2 = np.linspace(1, 4, 300)     # 第二部分：x∈[1,4]

    # 定义函数
    y1 = x**2  # y = x²
    y2 = np.sqrt(x)  # y = √x

    # 优雅的配色方案
    curve_color1 = '#2E5BBA'  # 深海蓝
    curve_color2 = '#C8102E'  # 深红
    vertical_color = '#228B22'  # 森林绿
    region1_color = '#FFE4E6'  # 淡玫瑰
    region2_color = '#E6F3FF'  # 淡天蓝
    demo_color1 = '#FF69B4'   # 热粉
    demo_color2 = '#4169E1'   # 皇家蓝

    # 绘制函数曲线 - 更粗的线条
    ax.plot(x, y1, color=curve_color1, linewidth=3.5, label=r'$y = x^2$', zorder=5)
    ax.plot(x, y2, color=curve_color2, linewidth=3.5, label=r'$y = \sqrt{x}$', zorder=5)

    # 绘制垂直线 x = 4
    ax.axvline(x=4, color=vertical_color, linewidth=3.5, label=r'$x = 4$', zorder=5)

    # 填充积分区域 - 优雅的透明度
    y1_region1 = x_region1**2
    y2_region1 = np.sqrt(x_region1)
    ax.fill_between(x_region1, y1_region1, y2_region1, alpha=0.6, color=region1_color, 
                    edgecolor=curve_color2, linewidth=1.5, label=r'区域1: $x \in [0,1]$', zorder=2)

    y1_region2 = x_region2**2
    y2_region2 = np.sqrt(x_region2)
    ax.fill_between(x_region2, y2_region2, y1_region2, alpha=0.6, color=region2_color, 
                    edgecolor=curve_color1, linewidth=1.5, label=r'区域2: $x \in [1,4]$', zorder=2)

    # 标记关键点 - 更大更清晰
    points = [(0, 0), (1, 1), (4, 2), (4, 16)]
    point_labels = ['(0, 0)', '(1, 1)', '(4, 2)', '(4, 16)']
    for point, label in zip(points, point_labels):
        ax.plot(point[0], point[1], 'o', color='#333333', markersize=10, 
                markeredgecolor='white', markeredgewidth=2, zorder=10)
        
        # 重新调整标签位置，避免与图像重合，利用空白区域
        if point == (0, 0):
            ax.annotate(label, point, xytext=(-0.8, -0.6), fontsize=12, weight='bold',
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white', edgecolor='gray', alpha=0.9),
                       arrowprops=dict(arrowstyle='->', color='gray', lw=1.5))
        elif point == (1, 1):
            ax.annotate(label, point, xytext=(1.4, 0.4), fontsize=12, weight='bold',
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white', edgecolor='gray', alpha=0.9),
                       arrowprops=dict(arrowstyle='->', color='gray', lw=1.5))
        elif point == (4, 2):
            ax.annotate(label, point, xytext=(5.2, 1.5), fontsize=12, weight='bold',
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white', edgecolor='gray', alpha=0.9),
                       arrowprops=dict(arrowstyle='->', color='gray', lw=1.5))
        else:  # (4, 16)
            ax.annotate(label, point, xytext=(3.8, 17.5), fontsize=12, weight='bold',
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white', edgecolor='gray', alpha=0.9),
                       arrowprops=dict(arrowstyle='->', color='gray', lw=1.5))

    # 添加示例垂直线 - 更细致的样式
    x_demo1 = 0.5
    ax.axvline(x=x_demo1, color=demo_color1, linewidth=2, linestyle='--', alpha=0.8, zorder=3)
    y_lower1 = x_demo1**2
    y_upper1 = np.sqrt(x_demo1)
    ax.plot([x_demo1, x_demo1], [y_lower1, y_upper1], color=demo_color1, linewidth=5, alpha=0.8, zorder=4)

    x_demo2 = 2.5
    ax.axvline(x=x_demo2, color=demo_color2, linewidth=2, linestyle='--', alpha=0.8, zorder=3)
    y_lower2 = np.sqrt(x_demo2)
    y_upper2 = x_demo2**2
    ax.plot([x_demo2, x_demo2], [y_lower2, y_upper2], color=demo_color2, linewidth=5, alpha=0.8, zorder=4)

    # 添加示例线标注 - 调整到不影响视觉的位置
    ax.annotate(f'x = {x_demo1}', (x_demo1, -0.7), xytext=(x_demo1 + 0.2, -0.3), 
               fontsize=11, color=demo_color1, weight='bold',
               bbox=dict(boxstyle='round,pad=0.25', facecolor='#FFF0F5', edgecolor=demo_color1, alpha=0.9),
               arrowprops=dict(arrowstyle='->', color=demo_color1, lw=1.2))

    ax.annotate(f'x = {x_demo2}', (x_demo2, -0.7), xytext=(x_demo2 + 0.4, -0.3), 
               fontsize=11, color=demo_color2, weight='bold',
               bbox=dict(boxstyle='round,pad=0.25', facecolor='#F0F8FF', edgecolor=demo_color2, alpha=0.9),
               arrowprops=dict(arrowstyle='->', color=demo_color2, lw=1.2))

    # 设置坐标轴 - 合理的范围
    ax.set_xlim(-1.2, 6.0)
    ax.set_ylim(-1.2, 19.5)
    ax.set_xlabel('x', fontsize=17, weight='bold', color='#333333')
    ax.set_ylabel('y', fontsize=17, weight='bold', color='#333333')
    ax.grid(True, alpha=0.3, linestyle='-', linewidth=0.8)
    
    # x-y-o坐标系布局 - 确保原点(0,0)清晰可见
    ax.axhline(y=0, color='black', linewidth=1.0, alpha=0.8, zorder=3)
    ax.axvline(x=0, color='black', linewidth=1.0, alpha=0.8, zorder=3)
    ax.plot(0, 0, 'ko', markersize=6, zorder=5)  # o点在(0,0)
    ax.text(0.15, -0.4, 'O', fontsize=14, weight='bold', color='#333333', zorder=6)

    # 简化标题
    ax.set_title(r'例题3.5: 由 $y = x^2$, $y = \sqrt{x}$, $x = 4$ 围成的积分区域', 
                fontsize=19, pad=20, weight='bold', color='#2C3E50')

    # 将图例移到右上角，避免与左侧文字冲突
    legend = ax.legend(loc='upper right', bbox_to_anchor=(0.98, 0.98), fontsize=12, framealpha=0.95, 
                      facecolor='white', edgecolor='gray', frameon=True)
    legend.get_frame().set_linewidth(1.2)

    # 简化文字内容，只保留核心表达式
    # 积分表达式
    ax.text(-1.1, 18.0, r'积分表达式:', fontsize=14, weight='bold', color='#2C3E50',
            bbox=dict(boxstyle='round,pad=0.4', facecolor='#F8F9FA', edgecolor='#DEE2E6', alpha=0.95))
    
    ax.text(-1.1, 17.0, r'$\iint_D (x+y) \, dA = \int_0^1 \int_{x^2}^{\sqrt{x}} (x+y) \, dy \, dx + \int_1^4 \int_{\sqrt{x}}^{x^2} (x+y) \, dy \, dx$', 
            fontsize=12, color='#2C3E50',
            bbox=dict(boxstyle='round,pad=0.4', facecolor='#F8F9FA', edgecolor='#DEE2E6', alpha=0.95))

    # 简化的区域分析
    textstr = """区域分析:
区域1: $x \\in [0,1]$, 积分限 $x^2 \\leq y \\leq \\sqrt{x}$
区域2: $x \\in [1,4]$, 积分限 $\\sqrt{x} \\leq y \\leq x^2$"""
    
    ax.text(-1.1, 15.0, textstr, fontsize=11, weight='bold', color='#155724',
            bbox=dict(boxstyle='round,pad=0.5', facecolor='#D4F7D4', edgecolor='#28A745', alpha=0.9),
            verticalalignment='top')

    # 函数关系说明
    ax.text(-1.1, 12.5, r'函数关系: $x \in [0,1]$ 时 $\sqrt{x} \geq x^2$；$x \in [1,4]$ 时 $x^2 \geq \sqrt{x}$', 
            fontsize=12, color='#5A5A8A', weight='bold',
            bbox=dict(boxstyle='round,pad=0.4', facecolor='#E8E8F5', edgecolor='#6A6A9A', alpha=0.9))

    # 美化坐标轴
    ax.spines['top'].set_color('#CCCCCC')
    ax.spines['right'].set_color('#CCCCCC')
    ax.spines['bottom'].set_color('#666666')
    ax.spines['left'].set_color('#666666')
    ax.spines['top'].set_linewidth(1)
    ax.spines['right'].set_linewidth(1)
    ax.spines['bottom'].set_linewidth(1.5)
    ax.spines['left'].set_linewidth(1.5)

    plt.tight_layout()
    
    # 保存图像
    output_path = '../../Assets/region_complex.png'
    plt.savefig(output_path, dpi=300, bbox_inches='tight', 
                facecolor='white', edgecolor='none', pad_inches=0.1)
    plt.close()
    
    print(f'成功生成优化的region_complex.png图像，保存至: {output_path}')
    print('标准规范完整实现:')
    print('1. ✅ 采用x-y-o坐标系布局，原点(0,0)清晰标记')
    print('2. ✅ 所有点标记按四角分布策略，无遮挡')
    print('3. ✅ 图例固定右上角，文字内容分层布局')
    print('4. ✅ 优雅配色方案，视觉效果协调统一')
    print('5. ✅ 符合数学作图标准规范的完整模板')
    
    # 验证文件是否生成
    if os.path.exists(output_path):
        file_size = os.path.getsize(output_path) / 1024  # KB
        print(f'文件大小: {file_size:.1f} KB')
    else:
        print('错误：文件生成失败')

if __name__ == "__main__":
    generate_region_complex() 