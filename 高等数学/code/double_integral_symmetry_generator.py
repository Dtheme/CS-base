#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
二重积分对称性图像生成器
生成轮换对称性和奇偶对称性的说明图像
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon, Circle
import matplotlib.patches as mpatches
from mpl_toolkits.mplot3d import Axes3D

# 配置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
plt.rcParams['axes.unicode_minus'] = False

def create_rotational_symmetry_figure():
    """创建轮换对称性图像"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
    
    # 图1：关于y=x对称的区域示例1 - 单位圆
    ax1.set_xlim(-1.5, 1.5)
    ax1.set_ylim(-1.5, 1.5)
    ax1.set_aspect('equal')
    
    # 绘制单位圆
    circle = Circle((0, 0), 1, fill=True, alpha=0.3, color='lightblue', 
                   edgecolor='blue', linewidth=2)
    ax1.add_patch(circle)
    
    # 绘制y=x对称轴
    x_line = np.linspace(-1.5, 1.5, 100)
    ax1.plot(x_line, x_line, 'r--', linewidth=2, label='y = x')
    
    # 标注对称点
    ax1.plot([0.8, 0.6], [0.6, 0.8], 'ro', markersize=8)
    ax1.annotate('(a, b)', xy=(0.8, 0.6), xytext=(1.0, 0.4),
                arrowprops=dict(arrowstyle='->', color='red'))
    ax1.annotate('(b, a)', xy=(0.6, 0.8), xytext=(0.4, 1.0),
                arrowprops=dict(arrowstyle='->', color='red'))
    
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_title('轮换对称区域: 单位圆', fontweight='bold')
    ax1.legend()
    
    # 图2：关于y=x对称的区域示例2 - 正方形
    ax2.set_xlim(-1.5, 1.5)
    ax2.set_ylim(-1.5, 1.5)
    ax2.set_aspect('equal')
    
    # 绘制正方形
    square = Polygon([(-1, -1), (1, -1), (1, 1), (-1, 1)], 
                    fill=True, alpha=0.3, color='lightgreen', 
                    edgecolor='green', linewidth=2)
    ax2.add_patch(square)
    
    # 绘制y=x对称轴
    ax2.plot(x_line, x_line, 'r--', linewidth=2, label='y = x')
    
    # 标注对称点
    ax2.plot([0.5, -0.3], [-0.3, 0.5], 'ro', markersize=8)
    ax2.annotate('(0.5, -0.3)', xy=(0.5, -0.3), xytext=(0.8, -0.6),
                arrowprops=dict(arrowstyle='->', color='red'))
    ax2.annotate('(-0.3, 0.5)', xy=(-0.3, 0.5), xytext=(-0.6, 0.8),
                arrowprops=dict(arrowstyle='->', color='red'))
    
    ax2.grid(True, alpha=0.3)
    ax2.set_xlabel('x')
    ax2.set_ylabel('y')
    ax2.set_title('轮换对称区域: 正方形', fontweight='bold')
    ax2.legend()
    
    # 图3：轮换对称性的积分说明
    ax3.set_xlim(-2, 2)
    ax3.set_ylim(-2, 2)
    ax3.set_aspect('equal')
    
    # 绘制椭圆
    theta = np.linspace(0, 2*np.pi, 100)
    x_ellipse = 1.5 * np.cos(theta)
    y_ellipse = 1.5 * np.sin(theta)
    ax3.fill(x_ellipse, y_ellipse, alpha=0.3, color='yellow', 
             edgecolor='orange', linewidth=2)
    
    # 绘制y=x对称轴
    ax3.plot([-2, 2], [-2, 2], 'r--', linewidth=2, label='y = x')
    
    # 添加函数说明文字
    ax3.text(-1.8, 1.5, r'∬_D f(x,y) dA = ∬_D f(y,x) dA', 
             fontsize=12, bbox=dict(boxstyle="round,pad=0.3", facecolor='white'))
    
    ax3.grid(True, alpha=0.3)
    ax3.set_xlabel('x')
    ax3.set_ylabel('y')
    ax3.set_title('轮换对称性积分性质', fontweight='bold')
    ax3.legend()
    
    # 图4：轮换对称性应用示例
    ax4.text(0.1, 0.8, '轮换对称性的应用:', fontsize=14, fontweight='bold')
    ax4.text(0.1, 0.7, '若区域D关于直线y=x对称，则:', fontsize=12)
    ax4.text(0.1, 0.6, r'∬_D [f(x,y) + f(y,x)] dA = 2∬_D f(x,y) dA', fontsize=12)
    ax4.text(0.1, 0.5, '', fontsize=12)
    ax4.text(0.1, 0.4, '例：计算 ∬_D xy dA，D: x²+y²≤1', fontsize=12)
    ax4.text(0.1, 0.3, '由于xy + yx = 2xy，利用轮换对称性：', fontsize=12)
    ax4.text(0.1, 0.2, '∬_D xy dA = ½∬_D [xy + yx] dA = ½∬_D 2xy dA = ∬_D xy dA', fontsize=12)
    ax4.text(0.1, 0.1, '但由于被积函数xy关于原点为奇函数，结果为0', fontsize=12)
    
    ax4.set_xlim(0, 1)
    ax4.set_ylim(0, 1)
    ax4.axis('off')
    
    plt.tight_layout()
    plt.savefig('../../Assets/double_integral_rotational_symmetry.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_even_odd_symmetry_figure():
    """创建奇偶对称性图像"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
    
    # 图1：关于x轴对称的区域
    ax1.set_xlim(-2, 2)
    ax1.set_ylim(-1.5, 1.5)
    ax1.set_aspect('equal')
    
    # 绘制椭圆（关于x轴对称）
    theta = np.linspace(0, 2*np.pi, 100)
    x_ellipse = 1.5 * np.cos(theta)
    y_ellipse = 1.0 * np.sin(theta)
    ax1.fill(x_ellipse, y_ellipse, alpha=0.3, color='lightcoral', 
             edgecolor='red', linewidth=2)
    
    # 绘制x轴对称轴
    ax1.axhline(y=0, color='black', linewidth=2, label='x轴')
    ax1.axvline(x=0, color='gray', linestyle=':', alpha=0.7)
    
    # 标注对称点
    ax1.plot([1, 1], [0.6, -0.6], 'bo', markersize=8)
    ax1.annotate('(x, y)', xy=(1, 0.6), xytext=(1.3, 0.8),
                arrowprops=dict(arrowstyle='->', color='blue'))
    ax1.annotate('(x, -y)', xy=(1, -0.6), xytext=(1.3, -0.8),
                arrowprops=dict(arrowstyle='->', color='blue'))
    
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_title('关于x轴对称的区域', fontweight='bold')
    ax1.legend()
    
    # 图2：关于y轴对称的区域
    ax2.set_xlim(-1.5, 1.5)
    ax2.set_ylim(-2, 2)
    ax2.set_aspect('equal')
    
    # 绘制椭圆（关于y轴对称）
    x_ellipse = 1.0 * np.cos(theta)
    y_ellipse = 1.5 * np.sin(theta)
    ax2.fill(x_ellipse, y_ellipse, alpha=0.3, color='lightgreen', 
             edgecolor='green', linewidth=2)
    
    # 绘制y轴对称轴
    ax2.axvline(x=0, color='black', linewidth=2, label='y轴')
    ax2.axhline(y=0, color='gray', linestyle=':', alpha=0.7)
    
    # 标注对称点
    ax2.plot([0.6, -0.6], [1, 1], 'mo', markersize=8)
    ax2.annotate('(x, y)', xy=(0.6, 1), xytext=(0.8, 1.3),
                arrowprops=dict(arrowstyle='->', color='magenta'))
    ax2.annotate('(-x, y)', xy=(-0.6, 1), xytext=(-0.8, 1.3),
                arrowprops=dict(arrowstyle='->', color='magenta'))
    
    ax2.grid(True, alpha=0.3)
    ax2.set_xlabel('x')
    ax2.set_ylabel('y')
    ax2.set_title('关于y轴对称的区域', fontweight='bold')
    ax2.legend()
    
    # 图3：奇函数的对称性
    ax3.set_xlim(-3, 3)
    ax3.set_ylim(-2, 2)
    
    # 绘制奇函数 f(x) = x³
    x = np.linspace(-2.5, 2.5, 100)
    y = x**3 / 5  # 缩放以适应图形
    ax3.plot(x, y, 'r-', linewidth=3, label=r'f(x) = x³/5')
    ax3.axhline(y=0, color='black', linewidth=1)
    ax3.axvline(x=0, color='black', linewidth=1)
    
    # 标注奇函数性质
    ax3.plot([1.5, -1.5], [1.5**3/5, -(1.5**3/5)], 'ro', markersize=8)
    ax3.annotate('f(a)', xy=(1.5, 1.5**3/5), xytext=(2.0, 1.2),
                arrowprops=dict(arrowstyle='->', color='red'))
    ax3.annotate('-f(a)', xy=(-1.5, -(1.5**3/5)), xytext=(-2.5, -1.2),
                arrowprops=dict(arrowstyle='->', color='red'))
    
    # 阴影区域表示积分区域
    x_fill = np.linspace(-2, 2, 100)
    y_upper = np.ones_like(x_fill) * 1.5
    y_lower = np.ones_like(x_fill) * (-1.5)
    ax3.fill_between(x_fill, y_lower, y_upper, alpha=0.2, color='blue')
    
    ax3.grid(True, alpha=0.3)
    ax3.set_xlabel('x')
    ax3.set_ylabel('y')
    ax3.set_title('奇函数: f(-x) = -f(x)', fontweight='bold')
    ax3.legend()
    ax3.text(-2.8, 1.7, r'∬_D f(x,y) dA = 0', fontsize=12, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='yellow'))
    
    # 图4：偶函数的对称性
    ax4.set_xlim(-3, 3)
    ax4.set_ylim(-0.5, 2.5)
    
    # 绘制偶函数 f(x) = x²
    y_even = x**2 / 3  # 缩放以适应图形
    ax4.plot(x, y_even, 'b-', linewidth=3, label=r'f(x) = x²/3')
    ax4.axhline(y=0, color='black', linewidth=1)
    ax4.axvline(x=0, color='black', linewidth=1)
    
    # 标注偶函数性质
    ax4.plot([1.5, -1.5], [1.5**2/3, 1.5**2/3], 'bo', markersize=8)
    ax4.annotate('f(a)', xy=(1.5, 1.5**2/3), xytext=(2.0, 1.5),
                arrowprops=dict(arrowstyle='->', color='blue'))
    ax4.annotate('f(a)', xy=(-1.5, 1.5**2/3), xytext=(-2.5, 1.5),
                arrowprops=dict(arrowstyle='->', color='blue'))
    
    # 阴影区域表示积分区域
    ax4.fill_between(x_fill, np.zeros_like(x_fill), y_upper, alpha=0.2, color='green')
    
    ax4.grid(True, alpha=0.3)
    ax4.set_xlabel('x')
    ax4.set_ylabel('y')
    ax4.set_title('偶函数: f(-x) = f(x)', fontweight='bold')
    ax4.legend()
    ax4.text(-2.8, 2.2, r'∬_D f(x,y) dA = 2∬_{D₁} f(x,y) dA', fontsize=10, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightgreen'))
    
    plt.tight_layout()
    plt.savefig('../../Assets/double_integral_even_odd_symmetry.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_symmetry_applications_figure():
    """创建对称性应用的综合示例图"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(14, 12))
    
    # 图1：例题6.1的积分区域（单位圆）
    ax1.set_xlim(-1.5, 1.5)
    ax1.set_ylim(-1.5, 1.5)
    ax1.set_aspect('equal')
    
    # 绘制单位圆
    circle = Circle((0, 0), 1, fill=True, alpha=0.3, color='lightblue', 
                   edgecolor='blue', linewidth=2)
    ax1.add_patch(circle)
    
    # 分割上下半圆
    theta_upper = np.linspace(0, np.pi, 50)
    theta_lower = np.linspace(np.pi, 2*np.pi, 50)
    x_upper = np.cos(theta_upper)
    y_upper = np.sin(theta_upper)
    x_lower = np.cos(theta_lower)
    y_lower = np.sin(theta_lower)
    
    ax1.fill(x_upper, y_upper, alpha=0.5, color='red', label='上半圆')
    ax1.fill(x_lower, y_lower, alpha=0.5, color='blue', label='下半圆')
    
    # 绘制坐标轴
    ax1.axhline(y=0, color='black', linewidth=2)
    ax1.axvline(x=0, color='black', linewidth=2)
    
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_title(r'例题6.1: ∬_D (x³ + xy²) dA', fontweight='bold')
    ax1.legend()
    
    # 图2：对称性分析
    ax2.text(0.05, 0.9, '对称性分析:', fontsize=14, fontweight='bold')
    ax2.text(0.05, 0.8, '• 区域D: x² + y² ≤ 1（关于x轴和y轴对称）', fontsize=11)
    ax2.text(0.05, 0.7, '• 被积函数: f(x,y) = x³ + xy² = x(x² + y²)', fontsize=11)
    ax2.text(0.05, 0.6, '• 关于y的奇偶性: f(x,-y) = x(x² + y²) = f(x,y)', fontsize=11)
    ax2.text(0.05, 0.5, '• 关于x的奇偶性: f(-x,y) = -x(x² + y²) = -f(x,y)', fontsize=11)
    ax2.text(0.05, 0.4, '', fontsize=11)
    ax2.text(0.05, 0.3, '结论:', fontsize=12, fontweight='bold', color='red')
    ax2.text(0.05, 0.2, '由于区域关于y轴对称，被积函数关于x为奇函数', fontsize=11)
    ax2.text(0.05, 0.1, '所以 ∬_D (x³ + xy²) dA = 0', fontsize=11, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='yellow'))
    
    ax2.set_xlim(0, 1)
    ax2.set_ylim(0, 1)
    ax2.axis('off')
    
    # 图3：复合对称性示例
    ax3.set_xlim(-2, 2)
    ax3.set_ylim(-2, 2)
    ax3.set_aspect('equal')
    
    # 绘制正方形区域
    square = Polygon([(-1.5, -1.5), (1.5, -1.5), (1.5, 1.5), (-1.5, 1.5)], 
                    fill=True, alpha=0.3, color='lightgreen', 
                    edgecolor='green', linewidth=2)
    ax3.add_patch(square)
    
    # 绘制对称轴
    ax3.axhline(y=0, color='black', linewidth=2, label='x轴')
    ax3.axvline(x=0, color='black', linewidth=2, label='y轴')
    ax3.plot([-2, 2], [-2, 2], 'r--', linewidth=2, label='y = x')
    
    # 分割四个象限
    ax3.fill([0, 1.5, 1.5, 0], [0, 0, 1.5, 1.5], alpha=0.3, color='red', label='第一象限')
    ax3.fill([-1.5, 0, 0, -1.5], [0, 0, 1.5, 1.5], alpha=0.3, color='blue', label='第二象限')
    ax3.fill([-1.5, 0, 0, -1.5], [-1.5, -1.5, 0, 0], alpha=0.3, color='yellow', label='第三象限')
    ax3.fill([0, 1.5, 1.5, 0], [-1.5, -1.5, 0, 0], alpha=0.3, color='purple', label='第四象限')
    
    ax3.grid(True, alpha=0.3)
    ax3.set_xlabel('x')
    ax3.set_ylabel('y')
    ax3.set_title('复合对称性区域', fontweight='bold')
    ax3.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    
    # 图4：对称性应用总结
    ax4.text(0.05, 0.95, '二重积分对称性应用总结', fontsize=14, fontweight='bold')
    
    ax4.text(0.05, 0.85, '1. 轮换对称性（区域关于y=x对称）:', fontsize=12, fontweight='bold')
    ax4.text(0.08, 0.8, '∬_D f(x,y) dA = ∬_D f(y,x) dA', fontsize=11)
    ax4.text(0.08, 0.75, '∬_D [f(x,y) + f(y,x)] dA = 2∬_D f(x,y) dA', fontsize=11)
    
    ax4.text(0.05, 0.65, '2. x轴对称性:', fontsize=12, fontweight='bold')
    ax4.text(0.08, 0.6, '• f(x,-y) = -f(x,y) (奇) → ∬_D f(x,y) dA = 0', fontsize=11)
    ax4.text(0.08, 0.55, '• f(x,-y) = f(x,y) (偶) → ∬_D f(x,y) dA = 2∬_{D₁} f(x,y) dA', fontsize=11)
    
    ax4.text(0.05, 0.45, '3. y轴对称性:', fontsize=12, fontweight='bold')
    ax4.text(0.08, 0.4, '• f(-x,y) = -f(x,y) (奇) → ∬_D f(x,y) dA = 0', fontsize=11)
    ax4.text(0.08, 0.35, '• f(-x,y) = f(x,y) (偶) → ∬_D f(x,y) dA = 2∬_{D₁} f(x,y) dA', fontsize=11)
    
    ax4.text(0.05, 0.25, '4. 应用策略:', fontsize=12, fontweight='bold')
    ax4.text(0.08, 0.2, '• 先判断积分区域的对称性', fontsize=11)
    ax4.text(0.08, 0.15, '• 再分析被积函数的奇偶性', fontsize=11)
    ax4.text(0.08, 0.1, '• 合理利用对称性简化计算', fontsize=11)
    ax4.text(0.08, 0.05, '• 注意复合对称性的应用', fontsize=11)
    
    ax4.set_xlim(0, 1)
    ax4.set_ylim(0, 1)
    ax4.axis('off')
    
    plt.tight_layout()
    plt.savefig('../../Assets/double_integral_symmetry_applications.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_3d_symmetry_visualization():
    """创建3D对称性可视化图"""
    fig = plt.figure(figsize=(15, 5))
    
    # 创建三个3D子图
    ax1 = fig.add_subplot(131, projection='3d')
    ax2 = fig.add_subplot(132, projection='3d')
    ax3 = fig.add_subplot(133, projection='3d')
    
    # 创建网格
    x = np.linspace(-2, 2, 30)
    y = np.linspace(-2, 2, 30)
    X, Y = np.meshgrid(x, y)
    
    # 图1：奇函数 f(x,y) = xy
    Z1 = X * Y
    # 只显示单位圆内的部分
    mask = X**2 + Y**2 <= 2
    Z1[~mask] = np.nan
    
    ax1.plot_surface(X, Y, Z1, alpha=0.7, cmap='RdBu_r')
    ax1.set_xlabel('x')
    ax1.set_ylabel('y')
    ax1.set_zlabel('z')
    ax1.set_title('奇函数: f(x,y) = xy\n积分值为0')
    
    # 图2：偶函数 f(x,y) = x² + y²
    Z2 = X**2 + Y**2
    Z2[~mask] = np.nan
    
    ax2.plot_surface(X, Y, Z2, alpha=0.7, cmap='viridis')
    ax2.set_xlabel('x')
    ax2.set_ylabel('y')
    ax2.set_zlabel('z')
    ax2.set_title('偶函数: f(x,y) = x² + y²\n可利用对称性')
    
    # 图3：轮换对称函数 f(x,y) = x + y
    Z3 = X + Y
    Z3[~mask] = np.nan
    
    ax3.plot_surface(X, Y, Z3, alpha=0.7, cmap='plasma')
    ax3.set_xlabel('x')
    ax3.set_ylabel('y')
    ax3.set_zlabel('z')
    ax3.set_title('轮换函数: f(x,y) = x + y\n可利用轮换对称性')
    
    plt.tight_layout()
    plt.savefig('../../Assets/double_integral_3d_symmetry.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

if __name__ == "__main__":
    print("正在生成二重积分对称性图像...")
    
    # 生成各类对称性图像
    create_rotational_symmetry_figure()
    print("✓ 轮换对称性图像已生成")
    
    create_even_odd_symmetry_figure()
    print("✓ 奇偶对称性图像已生成")
    
    create_symmetry_applications_figure()
    print("✓ 对称性应用图像已生成")
    
    create_3d_symmetry_visualization()
    print("✓ 3D对称性可视化图像已生成")
    
    print("\n所有图像已保存到 ../../Assets/ 目录:")
    print("• double_integral_rotational_symmetry.png")
    print("• double_integral_even_odd_symmetry.png") 
    print("• double_integral_symmetry_applications.png")
    print("• double_integral_3d_symmetry.png") 