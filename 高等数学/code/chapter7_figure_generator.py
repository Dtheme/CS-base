#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
第7章重积分图片生成器
生成高质量的中文标注图片
"""

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import os

# 配置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
plt.rcParams['axes.unicode_minus'] = False

# 创建输出目录
output_dir = '../Assets'
os.makedirs(output_dir, exist_ok=True)

def setup_3d_style(ax, title=""):
    """设置统一的3D图形样式"""
    ax.set_xlabel('x', fontsize=12)
    ax.set_ylabel('y', fontsize=12)
    ax.set_zlabel('z', fontsize=12)
    if title:
        ax.set_title(title, fontsize=14, fontweight='bold')
    ax.grid(True, alpha=0.3)

def generate_double_integral_geometry():
    """生成二重积分几何意义图"""
    fig = plt.figure(figsize=(16, 12))
    
    # 1. 二重积分的几何意义 - 曲顶柱体
    ax1 = fig.add_subplot(2, 3, 1, projection='3d')
    x = np.linspace(-2, 2, 30)
    y = np.linspace(-2, 2, 30)
    X, Y = np.meshgrid(x, y)
    # 只在圆形区域内定义函数
    mask = X**2 + Y**2 <= 1
    Z = np.zeros_like(X)
    Z[mask] = 2 - X[mask]**2 - Y[mask]**2
    Z[~mask] = np.nan
    
    ax1.plot_surface(X, Y, Z, alpha=0.7, cmap='viridis')
    # 绘制底面圆
    theta = np.linspace(0, 2*np.pi, 100)
    x_circle = np.cos(theta)
    y_circle = np.sin(theta)
    z_circle = np.zeros_like(theta)
    ax1.plot(x_circle, y_circle, z_circle, 'r-', linewidth=2)
    
    # 绘制底面区域（填充）
    theta_fill = np.linspace(0, 2*np.pi, 50)
    r_fill = np.linspace(0, 1, 20)
    THETA_fill, R_fill = np.meshgrid(theta_fill, r_fill)
    X_fill = R_fill * np.cos(THETA_fill)
    Y_fill = R_fill * np.sin(THETA_fill)
    Z_fill = np.zeros_like(X_fill)
    ax1.plot_surface(X_fill, Y_fill, Z_fill, alpha=0.3, color='yellow')
    
    setup_3d_style(ax1, "二重积分几何意义")
    ax1.text(0, 0, 2.5, r'$z = f(x,y)$', fontsize=12)
    ax1.text(0, -1.5, 0, r'区域 $D$', fontsize=12)
    
    # 2. X型区域
    ax2 = fig.add_subplot(2, 3, 2)
    x = np.linspace(0, 2, 100)
    y1 = x**2/4
    y2 = np.sqrt(x)
    ax2.plot(x, y1, 'b-', linewidth=2, label=r'$y = \frac{x^2}{4}$')
    ax2.plot(x, y2, 'r-', linewidth=2, label=r'$y = \sqrt{x}$')
    ax2.fill_between(x, y1, y2, alpha=0.3, color='lightblue')
    ax2.set_xlabel('x', fontsize=12)
    ax2.set_ylabel('y', fontsize=12)
    ax2.set_title('X型区域', fontsize=14, fontweight='bold')
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    
    # 添加积分次序说明
    ax2.text(0.5, 0.8, r'先对 $y$ 积分', fontsize=10, 
             bbox=dict(boxstyle="round", facecolor='wheat'))
    ax2.text(0.5, 0.6, r'再对 $x$ 积分', fontsize=10,
             bbox=dict(boxstyle="round", facecolor='wheat'))
    
    # 3. Y型区域
    ax3 = fig.add_subplot(2, 3, 3)
    y = np.linspace(0, 2, 100)
    x1 = y**2/4
    x2 = np.sqrt(y)
    ax3.plot(x1, y, 'b-', linewidth=2, label=r'$x = \frac{y^2}{4}$')
    ax3.plot(x2, y, 'r-', linewidth=2, label=r'$x = \sqrt{y}$')
    ax3.fill_betweenx(y, x1, x2, alpha=0.3, color='lightgreen')
    ax3.set_xlabel('x', fontsize=12)
    ax3.set_ylabel('y', fontsize=12)
    ax3.set_title('Y型区域', fontsize=14, fontweight='bold')
    ax3.grid(True, alpha=0.3)
    ax3.legend()
    
    # 添加积分次序说明
    ax3.text(0.8, 0.5, r'先对 $x$ 积分', fontsize=10,
             bbox=dict(boxstyle="round", facecolor='lightgreen'))
    ax3.text(0.6, 0.3, r'再对 $y$ 积分', fontsize=10,
             bbox=dict(boxstyle="round", facecolor='lightgreen'))
    
    # 4. 极坐标变换
    ax4 = fig.add_subplot(2, 3, 4, projection='polar')
    r = np.linspace(0, 2, 50)
    theta = np.linspace(0, 2*np.pi, 100)
    R, THETA = np.meshgrid(r, theta)
    Z = R * np.cos(THETA) + R * np.sin(THETA)
    
    # 绘制极坐标网格
    r_lines = [0.5, 1.0, 1.5, 2.0]
    theta_lines = np.linspace(0, 2*np.pi, 8, endpoint=False)
    
    for r_val in r_lines:
        theta_full = np.linspace(0, 2*np.pi, 100)
        ax4.plot(theta_full, [r_val]*len(theta_full), 'k--', alpha=0.5)
    
    for theta_val in theta_lines:
        r_full = np.linspace(0, 2, 100)
        ax4.plot([theta_val]*len(r_full), r_full, 'k--', alpha=0.5)
    
    ax4.set_title('极坐标变换\n' + r'$x = r\cos\theta, y = r\sin\theta$', 
                  fontsize=12, fontweight='bold')
    
    # 5. 积分次序选择
    ax5 = fig.add_subplot(2, 3, 5)
    # 绘制复杂区域
    x = np.linspace(-1, 1, 100)
    y1 = np.sqrt(1 - x**2)  # 上半圆
    y2 = -np.sqrt(1 - x**2)  # 下半圆
    
    ax5.plot(x, y1, 'b-', linewidth=2)
    ax5.plot(x, y2, 'b-', linewidth=2)
    ax5.fill_between(x, y1, y2, alpha=0.3, color='orange')
    
    # 添加垂直线和水平线说明
    ax5.axvline(x=0.3, color='red', linestyle='--', alpha=0.7)
    ax5.axhline(y=0.3, color='green', linestyle='--', alpha=0.7)
    
    ax5.set_xlabel('x', fontsize=12)
    ax5.set_ylabel('y', fontsize=12)
    ax5.set_title('积分次序选择', fontsize=14, fontweight='bold')
    ax5.grid(True, alpha=0.3)
    ax5.set_xlim(-1.2, 1.2)
    ax5.set_ylim(-1.2, 1.2)
    
    ax5.text(-0.8, 0.8, '选择合适的\n积分次序', fontsize=10,
             bbox=dict(boxstyle="round", facecolor='orange'))
    
    # 6. 坐标系比较
    ax6 = fig.add_subplot(2, 3, 6)
    # 直角坐标系
    ax6.plot([-1.5, 1.5], [0, 0], 'k-', linewidth=1)
    ax6.plot([0, 0], [-1.5, 1.5], 'k-', linewidth=1)
    
    # 极坐标系
    for r in [0.5, 1.0]:
        theta = np.linspace(0, 2*np.pi, 100)
        x_circle = r * np.cos(theta)
        y_circle = r * np.sin(theta)
        ax6.plot(x_circle, y_circle, 'r--', alpha=0.7)
    
    for angle in [0, np.pi/4, np.pi/2, 3*np.pi/4, np.pi, 5*np.pi/4, 3*np.pi/2, 7*np.pi/4]:
        x_line = [0, 1.2*np.cos(angle)]
        y_line = [0, 1.2*np.sin(angle)]
        ax6.plot(x_line, y_line, 'r--', alpha=0.7)
    
    ax6.set_xlabel('x', fontsize=12)
    ax6.set_ylabel('y', fontsize=12)
    ax6.set_title('坐标系比较', fontsize=14, fontweight='bold')
    ax6.grid(True, alpha=0.3)
    ax6.set_xlim(-1.5, 1.5)
    ax6.set_ylim(-1.5, 1.5)
    ax6.set_aspect('equal')
    
    ax6.text(-1.3, 1.3, '直角坐标', color='black', fontsize=10)
    ax6.text(0.8, 1.3, '极坐标', color='red', fontsize=10)
    
    plt.suptitle('二重积分理论与计算综合图', fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/double_integral_geometry.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_x_type_region():
    """生成X型区域示意图"""
    fig, ax = plt.subplots(figsize=(10, 8))
    
    x = np.linspace(0, 3, 100)
    y1 = 0.5 * x
    y2 = 2 * np.sqrt(x)
    
    ax.plot(x, y1, 'b-', linewidth=3, label=r'$y = \varphi_1(x)$')
    ax.plot(x, y2, 'r-', linewidth=3, label=r'$y = \varphi_2(x)$')
    ax.fill_between(x, y1, y2, alpha=0.3, color='lightblue', label='X型区域 D')
    
    # 添加垂直线示例
    x_example = 2
    y1_example = 0.5 * x_example
    y2_example = 2 * np.sqrt(x_example)
    ax.plot([x_example, x_example], [y1_example, y2_example], 'g--', linewidth=2)
    ax.plot([x_example, x_example], [0, y1_example], 'k:', alpha=0.5)
    
    # 添加标注
    ax.annotate('', xy=(x_example, y2_example), xytext=(x_example, y1_example),
                arrowprops=dict(arrowstyle='<->', color='green', lw=2))
    ax.text(x_example + 0.1, (y1_example + y2_example)/2, 
            r'$\varphi_1(x) \leq y \leq \varphi_2(x)$', 
            fontsize=12, rotation=90, va='center')
    
    ax.text(1.5, 0.5, r'$a \leq x \leq b$', fontsize=14, ha='center',
            bbox=dict(boxstyle="round", facecolor='wheat'))
    
    ax.set_xlabel('x', fontsize=14)
    ax.set_ylabel('y', fontsize=14)
    ax.set_title('X型区域\n' + r'$D = \{(x,y) | a \leq x \leq b, \varphi_1(x) \leq y \leq \varphi_2(x)\}$', 
                 fontsize=16, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=12)
    ax.set_xlim(-0.2, 3.2)
    ax.set_ylim(-0.2, 4.2)
    
    # 添加积分公式
    formula_text = r'$\iint_D f(x,y) \, dA = \int_a^b \left[\int_{\varphi_1(x)}^{\varphi_2(x)} f(x,y) \, dy\right] dx$'
    ax.text(0.1, 3.8, formula_text, fontsize=14, 
            bbox=dict(boxstyle="round", facecolor='lightblue', alpha=0.8))
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/x_type_region.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_y_type_region():
    """生成Y型区域示意图"""
    fig, ax = plt.subplots(figsize=(10, 8))
    
    y = np.linspace(0, 3, 100)
    x1 = 0.5 * y
    x2 = 2 * np.sqrt(y)
    
    ax.plot(x1, y, 'b-', linewidth=3, label=r'$x = \psi_1(y)$')
    ax.plot(x2, y, 'r-', linewidth=3, label=r'$x = \psi_2(y)$')
    ax.fill_betweenx(y, x1, x2, alpha=0.3, color='lightgreen', label='Y型区域 D')
    
    # 添加水平线示例
    y_example = 2
    x1_example = 0.5 * y_example
    x2_example = 2 * np.sqrt(y_example)
    ax.plot([x1_example, x2_example], [y_example, y_example], 'g--', linewidth=2)
    ax.plot([0, x1_example], [y_example, y_example], 'k:', alpha=0.5)
    
    # 添加标注
    ax.annotate('', xy=(x2_example, y_example), xytext=(x1_example, y_example),
                arrowprops=dict(arrowstyle='<->', color='green', lw=2))
    ax.text((x1_example + x2_example)/2, y_example + 0.1, 
            r'$\psi_1(y) \leq x \leq \psi_2(y)$', 
            fontsize=12, ha='center')
    
    ax.text(0.5, 1.5, r'$c \leq y \leq d$', fontsize=14, rotation=90, va='center',
            bbox=dict(boxstyle="round", facecolor='lightgreen'))
    
    ax.set_xlabel('x', fontsize=14)
    ax.set_ylabel('y', fontsize=14)
    ax.set_title('Y型区域\n' + r'$D = \{(x,y) | c \leq y \leq d, \psi_1(y) \leq x \leq \psi_2(y)\}$', 
                 fontsize=16, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=12)
    ax.set_xlim(-0.2, 4.2)
    ax.set_ylim(-0.2, 3.2)
    
    # 添加积分公式
    formula_text = r'$\iint_D f(x,y) \, dA = \int_c^d \left[\int_{\psi_1(y)}^{\psi_2(y)} f(x,y) \, dx\right] dy$'
    ax.text(0.1, 0.1, formula_text, fontsize=14, 
            bbox=dict(boxstyle="round", facecolor='lightgreen', alpha=0.8))
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/y_type_region.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_integration_order():
    """生成积分次序比较图"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    # 左图：X型区域的积分次序
    x = np.linspace(-1, 1, 100)
    y_upper = np.sqrt(1 - x**2)
    y_lower = -np.sqrt(1 - x**2)
    
    ax1.plot(x, y_upper, 'b-', linewidth=3)
    ax1.plot(x, y_lower, 'b-', linewidth=3)
    ax1.fill_between(x, y_lower, y_upper, alpha=0.3, color='lightblue')
    
    # 添加垂直线
    x_line = 0.5
    y_line_upper = np.sqrt(1 - x_line**2)
    y_line_lower = -np.sqrt(1 - x_line**2)
    ax1.plot([x_line, x_line], [y_line_lower, y_line_upper], 'r--', linewidth=2)
    
    ax1.set_xlabel('x', fontsize=14)
    ax1.set_ylabel('y', fontsize=14)
    ax1.set_title('先 y 后 x（X型区域）\n' + r'$\int_{-1}^{1} \left[\int_{-\sqrt{1-x^2}}^{\sqrt{1-x^2}} f(x,y) \, dy\right] dx$', 
                  fontsize=14, fontweight='bold')
    ax1.grid(True, alpha=0.3)
    ax1.set_xlim(-1.2, 1.2)
    ax1.set_ylim(-1.2, 1.2)
    ax1.set_aspect('equal')
    
    # 右图：Y型区域的积分次序
    y = np.linspace(-1, 1, 100)
    x_right = np.sqrt(1 - y**2)
    x_left = -np.sqrt(1 - y**2)
    
    ax2.plot(x_right, y, 'g-', linewidth=3)
    ax2.plot(x_left, y, 'g-', linewidth=3)
    ax2.fill_betweenx(y, x_left, x_right, alpha=0.3, color='lightgreen')
    
    # 添加水平线
    y_line = 0.5
    x_line_right = np.sqrt(1 - y_line**2)
    x_line_left = -np.sqrt(1 - y_line**2)
    ax2.plot([x_line_left, x_line_right], [y_line, y_line], 'r--', linewidth=2)
    
    ax2.set_xlabel('x', fontsize=14)
    ax2.set_ylabel('y', fontsize=14)
    ax2.set_title('先 x 后 y（Y型区域）\n' + r'$\int_{-1}^{1} \left[\int_{-\sqrt{1-y^2}}^{\sqrt{1-y^2}} f(x,y) \, dx\right] dy$', 
                  fontsize=14, fontweight='bold')
    ax2.grid(True, alpha=0.3)
    ax2.set_xlim(-1.2, 1.2)
    ax2.set_ylim(-1.2, 1.2)
    ax2.set_aspect('equal')
    
    plt.suptitle('积分次序比较', fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/integration_order.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_region_parabola():
    """生成抛物线区域图"""
    fig, ax = plt.subplots(figsize=(10, 8))
    
    x = np.linspace(0, 1, 100)
    y1 = x**2  # y = x²
    y2 = x     # y = x
    
    ax.plot(x, y1, 'b-', linewidth=3, label=r'$y = x^2$')
    ax.plot(x, y2, 'r-', linewidth=3, label=r'$y = x$')
    ax.fill_between(x, y1, y2, alpha=0.3, color='yellow', label='积分区域')
    
    # 标记交点
    ax.plot([0, 1], [0, 1], 'ko', markersize=8)
    ax.text(-0.05, -0.05, '(0,0)', fontsize=12, ha='right', va='top')
    ax.text(1.05, 1.05, '(1,1)', fontsize=12, ha='left', va='bottom')
    
    # 添加示例垂直线
    x_example = 0.6
    y1_example = x_example**2
    y2_example = x_example
    ax.plot([x_example, x_example], [y1_example, y2_example], 'g--', linewidth=2)
    ax.plot([x_example, x_example], [0, y1_example], 'k:', alpha=0.5)
    
    ax.set_xlabel('x', fontsize=14)
    ax.set_ylabel('y', fontsize=14)
    ax.set_title(r'由 $y = x^2$ 和 $y = x$ 围成的区域', fontsize=16, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=12)
    ax.set_xlim(-0.1, 1.2)
    ax.set_ylim(-0.1, 1.2)
    ax.set_aspect('equal')
    
    # 添加积分表达式
    formula_text = r'$\iint_D xy \, dA = \int_0^1 \left[\int_{x^2}^{x} xy \, dy\right] dx$'
    ax.text(0.1, 0.9, formula_text, fontsize=14, 
            bbox=dict(boxstyle="round", facecolor='yellow', alpha=0.8))
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/region_parabola.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_region_triangle():
    """生成三角形区域图"""
    fig, ax = plt.subplots(figsize=(10, 8))
    
    # 三角形顶点
    vertices = np.array([[0, 0], [2, 0], [1, 2], [0, 0]])
    ax.plot(vertices[:, 0], vertices[:, 1], 'b-', linewidth=3)
    ax.fill(vertices[:-1, 0], vertices[:-1, 1], alpha=0.3, color='lightcyan', label='三角形区域')
    
    # 标记顶点
    ax.plot([0, 2, 1], [0, 0, 2], 'ro', markersize=8)
    ax.text(-0.1, -0.1, '(0,0)', fontsize=12, ha='right', va='top')
    ax.text(2.1, -0.1, '(2,0)', fontsize=12, ha='left', va='top')
    ax.text(1, 2.1, '(1,2)', fontsize=12, ha='center', va='bottom')
    
    # 添加边的方程
    x1 = np.linspace(0, 1, 50)
    y1 = 2 * x1  # 左边：y = 2x
    x2 = np.linspace(1, 2, 50)
    y2 = -2 * x2 + 4  # 右边：y = -2x + 4
    
    ax.text(0.3, 0.8, r'$y = 2x$', fontsize=12, rotation=60, 
            bbox=dict(boxstyle="round", facecolor='white', alpha=0.8))
    ax.text(1.5, 0.8, r'$y = -2x + 4$', fontsize=12, rotation=-60,
            bbox=dict(boxstyle="round", facecolor='white', alpha=0.8))
    ax.text(1, -0.2, r'$y = 0$', fontsize=12, ha='center',
            bbox=dict(boxstyle="round", facecolor='white', alpha=0.8))
    
    # 示例积分线
    y_example = 1
    x_left = y_example / 2
    x_right = 2 - y_example / 2
    ax.plot([x_left, x_right], [y_example, y_example], 'g--', linewidth=2)
    ax.annotate('', xy=(x_right, y_example), xytext=(x_left, y_example),
                arrowprops=dict(arrowstyle='<->', color='green', lw=2))
    ax.text((x_left + x_right)/2, y_example + 0.1, 
            r'$\frac{y}{2} \leq x \leq 2 - \frac{y}{2}$', 
            fontsize=10, ha='center')
    
    ax.set_xlabel('x', fontsize=14)
    ax.set_ylabel('y', fontsize=14)
    ax.set_title('三角形积分区域', fontsize=16, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=12)
    ax.set_xlim(-0.3, 2.5)
    ax.set_ylim(-0.3, 2.5)
    ax.set_aspect('equal')
    
    # 添加Y型区域积分公式
    formula_text = r'$\iint_D f(x,y) \, dA = \int_0^2 \left[\int_{\frac{y}{2}}^{2-\frac{y}{2}} f(x,y) \, dx\right] dy$'
    ax.text(0.1, 2.2, formula_text, fontsize=12, 
            bbox=dict(boxstyle="round", facecolor='lightcyan', alpha=0.8))
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/region_triangle.png', dpi=300, bbox_inches='tight')
    plt.close()

# 运行生成函数
if __name__ == "__main__":
    print("开始生成第7章图片...")
    
    print("1. 生成二重积分几何意义综合图...")
    generate_double_integral_geometry()
    
    print("2. 生成X型区域示意图...")
    generate_x_type_region()
    
    print("3. 生成Y型区域示意图...")
    generate_y_type_region()
    
    print("4. 生成积分次序比较图...")
    generate_integration_order()
    
    print("5. 生成抛物线区域图...")
    generate_region_parabola()
    
    print("6. 生成三角形区域图...")
    generate_region_triangle()
    
    print("第7章图片生成完成！") 