#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# created by Dzw 2025-06-25
#
# 极坐标图
# 


import matplotlib.pyplot as plt
import numpy as np
import os

# 设置字体为系统默认，避免中文字体问题
plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.size'] = 14
plt.rcParams['text.usetex'] = False

# 配色方案
colors = {
    'primary': '#2E86C1',
    'secondary': '#E74C3C', 
    'accent': '#27AE60'
}

def save_plot(filename, directory='../../Assets'):
    """保存图片"""
    if not os.path.exists(directory):
        os.makedirs(directory)
    filepath = os.path.join(directory, filename)
    plt.savefig(filepath, dpi=300, bbox_inches='tight', facecolor='white')
    plt.close()
    print(f"图片已保存: {filepath}")

def plot_polar_example_4_1():
    """绘制极坐标例题4.1：圆形区域x²+y²≤4"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
    
    # 左图：直角坐标表示
    theta = np.linspace(0, 2*np.pi, 100)
    x_circle = 2 * np.cos(theta)
    y_circle = 2 * np.sin(theta)
    
    ax1.fill(x_circle, y_circle, alpha=0.3, color=colors['primary'], 
             label=r'$D: x^2 + y^2 \leq 4$')
    ax1.plot(x_circle, y_circle, color=colors['primary'], linewidth=2)
    ax1.set_xlim(-3, 3)
    ax1.set_ylim(-3, 3)
    ax1.set_aspect('equal')
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('$x$', fontsize=16)
    ax1.set_ylabel('$y$', fontsize=16)
    ax1.set_title('Cartesian Coordinates', fontsize=16)
    ax1.legend()
    
    # 右图：极坐标表示
    ax2 = plt.subplot(122, projection='polar')
    theta = np.linspace(0, 2*np.pi, 100)
    ax2.fill_between(theta, 0, 2, alpha=0.3, color=colors['primary'])
    ax2.plot(theta, np.full_like(theta, 2), color=colors['primary'], linewidth=2)
    ax2.set_ylim(0, 3)
    ax2.set_title(r'Polar Coordinates: $r \leq 2$', fontsize=16, pad=20)
    ax2.grid(True)
    
    plt.tight_layout()
    save_plot('polar_example_4_1.png')

def plot_polar_example_4_2():
    """绘制极坐标例题4.2：单位圆x²+y²≤1与被积函数"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
    
    # 左上：积分区域
    theta = np.linspace(0, 2*np.pi, 100)
    x_circle = np.cos(theta)
    y_circle = np.sin(theta)
    
    ax1.fill(x_circle, y_circle, alpha=0.3, color=colors['primary'])
    ax1.plot(x_circle, y_circle, color=colors['primary'], linewidth=2)
    ax1.set_xlim(-1.5, 1.5)
    ax1.set_ylim(-1.5, 1.5)
    ax1.set_aspect('equal')
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('$x$', fontsize=14)
    ax1.set_ylabel('$y$', fontsize=14)
    ax1.set_title(r'Region: $x^2 + y^2 \leq 1$', fontsize=14)
    
    # 右上：被积函数3D图
    x = np.linspace(-1.2, 1.2, 50)
    y = np.linspace(-1.2, 1.2, 50)
    X, Y = np.meshgrid(x, y)
    Z = np.exp(-(X**2 + Y**2))
    
    # 只显示单位圆内的部分
    mask = X**2 + Y**2 <= 1
    Z_masked = np.where(mask, Z, np.nan)
    
    ax2 = fig.add_subplot(222, projection='3d')
    surf = ax2.plot_surface(X, Y, Z_masked, cmap='viridis', alpha=0.8)
    ax2.set_xlabel('$x$', fontsize=14)
    ax2.set_ylabel('$y$', fontsize=14)
    ax2.set_zlabel(r'$f(x,y) = e^{-(x^2+y^2)}$', fontsize=14)
    ax2.set_title('Integrand', fontsize=14)
    
    # 左下：极坐标区域
    ax3 = plt.subplot(223, projection='polar')
    theta = np.linspace(0, 2*np.pi, 100)
    ax3.fill_between(theta, 0, 1, alpha=0.3, color=colors['primary'])
    ax3.set_title(r'Polar: $0 \leq r \leq 1, 0 \leq \theta \leq 2\pi$', 
                  fontsize=14, pad=20)
    ax3.grid(True)
    
    # 右下：积分过程示意
    r_vals = np.linspace(0, 1, 100)
    integrand = r_vals * np.exp(-r_vals**2)
    
    ax4.plot(r_vals, integrand, color=colors['primary'], linewidth=2, 
             label=r'$r \cdot e^{-r^2}$')
    ax4.fill_between(r_vals, 0, integrand, alpha=0.3, color=colors['primary'])
    ax4.set_xlabel('$r$', fontsize=14)
    ax4.set_ylabel('Integrand', fontsize=14)
    ax4.set_title('Polar Integrand', fontsize=14)
    ax4.grid(True, alpha=0.3)
    ax4.legend()
    
    plt.tight_layout()
    save_plot('polar_example_4_2.png')

def plot_polar_example_7_2():
    """绘制例题7.2：立体体积计算"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
    
    # 左上：投影区域
    theta = np.linspace(0, 2*np.pi, 100)
    x_circle = np.sqrt(2) * np.cos(theta)
    y_circle = np.sqrt(2) * np.sin(theta)
    
    ax1.fill(x_circle, y_circle, alpha=0.3, color=colors['primary'])
    ax1.plot(x_circle, y_circle, color=colors['primary'], linewidth=2, 
             label=r'$x^2 + y^2 = 2$')
    ax1.set_xlim(-2, 2)
    ax1.set_ylim(-2, 2)
    ax1.set_aspect('equal')
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('$x$', fontsize=14)
    ax1.set_ylabel('$y$', fontsize=14)
    ax1.set_title(r'Projection: $x^2 + y^2 \leq 2$', fontsize=14)
    ax1.legend()
    
    # 右上：极坐标表示
    ax2 = plt.subplot(222, projection='polar')
    theta = np.linspace(0, 2*np.pi, 100)
    ax2.fill_between(theta, 0, np.sqrt(2), alpha=0.3, color=colors['primary'])
    ax2.plot(theta, np.full_like(theta, np.sqrt(2)), color=colors['primary'], linewidth=2)
    ax2.set_title(r'Polar: $r \leq \sqrt{2}$', fontsize=14, pad=20)
    ax2.grid(True)
    
    # 左下：立体示意图（简化的2D表示）
    x_vals = np.linspace(-np.sqrt(2), np.sqrt(2), 100)
    z_paraboloid = x_vals**2
    z_plane = np.full_like(x_vals, 2)
    
    ax3.plot(x_vals, z_paraboloid, color=colors['secondary'], linewidth=2, 
             label=r'$z = x^2 + y^2$')
    ax3.plot(x_vals, z_plane, color=colors['accent'], linewidth=2, 
             label=r'$z = 2$')
    ax3.fill_between(x_vals, z_paraboloid, z_plane, alpha=0.3, 
                     color=colors['primary'])
    ax3.set_xlabel('$x$ (cross-section)', fontsize=14)
    ax3.set_ylabel('$z$', fontsize=14)
    ax3.set_title('Solid Volume', fontsize=14)
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    
    # 右下：被积函数
    r_vals = np.linspace(0, np.sqrt(2), 100)
    height_func = 2 - r_vals**2
    
    ax4.plot(r_vals, height_func, color=colors['primary'], linewidth=2, 
             label=r'$h(r) = 2 - r^2$')
    ax4.fill_between(r_vals, 0, height_func, alpha=0.3, color=colors['primary'])
    ax4.set_xlabel('$r$', fontsize=14)
    ax4.set_ylabel('Height', fontsize=14)
    ax4.set_title('Height Function', fontsize=14)
    ax4.grid(True, alpha=0.3)
    ax4.legend()
    
    plt.tight_layout()
    save_plot('polar_example_7_2.png')

def plot_polar_example_7_4():
    """绘制例题7.4：转动惯量计算"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
    
    # 左上：单位圆盘
    theta = np.linspace(0, 2*np.pi, 100)
    x_circle = np.cos(theta)
    y_circle = np.sin(theta)
    
    ax1.fill(x_circle, y_circle, alpha=0.3, color=colors['primary'])
    ax1.plot(x_circle, y_circle, color=colors['primary'], linewidth=2)
    
    # 添加一些径向线显示转动
    for angle in np.linspace(0, 2*np.pi, 8, endpoint=False):
        x_line = [0, np.cos(angle)]
        y_line = [0, np.sin(angle)]
        ax1.plot(x_line, y_line, '--', color=colors['secondary'], alpha=0.5)
        
    ax1.scatter(0, 0, color='red', s=50, zorder=5, label='Rotation Center')
    ax1.set_xlim(-1.5, 1.5)
    ax1.set_ylim(-1.5, 1.5)
    ax1.set_aspect('equal')
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('$x$', fontsize=14)
    ax1.set_ylabel('$y$', fontsize=14)
    ax1.set_title(r'Unit Disk: $x^2 + y^2 \leq 1$', fontsize=14)
    ax1.legend()
    
    # 右上：距离函数可视化
    r_vals = np.linspace(0, 1, 6)
    ax2 = plt.subplot(222, projection='polar')
    theta = np.linspace(0, 2*np.pi, 100)
    
    for i, r_val in enumerate(r_vals):
        if r_val > 0:
            ax2.plot(theta, np.full_like(theta, r_val), '--', 
                    alpha=0.7, label=f'r = {r_val:.1f}')
    
    ax2.fill_between(theta, 0, 1, alpha=0.3, color=colors['primary'])
    ax2.set_title('Distance Circles', fontsize=14, pad=20)
    ax2.grid(True)
    
    # 左下：极坐标表示
    ax3 = plt.subplot(223, projection='polar')
    theta = np.linspace(0, 2*np.pi, 100)
    ax3.fill_between(theta, 0, 1, alpha=0.3, color=colors['primary'])
    ax3.set_title(r'Polar: $0 \leq r \leq 1$', fontsize=14, pad=20)
    ax3.grid(True)
    
    # 右下：被积函数
    r_vals = np.linspace(0, 1, 100)
    integrand = r_vals**2 * r_vals  # r² * r (Jacobian)
    
    ax4.plot(r_vals, integrand, color=colors['primary'], linewidth=2, 
             label=r'$r^2 \cdot r = r^3$')
    ax4.fill_between(r_vals, 0, integrand, alpha=0.3, color=colors['primary'])
    ax4.set_xlabel('$r$', fontsize=14)
    ax4.set_ylabel('Integrand', fontsize=14)
    ax4.set_title('Polar Integrand', fontsize=14)
    ax4.grid(True, alpha=0.3)
    ax4.legend()
    
    plt.tight_layout()
    save_plot('polar_example_7_4.png')

def main():
    print("生成极坐标例题图形...")
    
    plot_polar_example_4_1()
    print("✓ 例题4.1图形已生成")
    
    plot_polar_example_4_2()  
    print("✓ 例题4.2图形已生成")
    
    plot_polar_example_7_2()
    print("✓ 例题7.2图形已生成")
    
    plot_polar_example_7_4()
    print("✓ 例题7.4图形已生成")
    
    print("所有极坐标例题图形生成完成！")

if __name__ == "__main__":
    main() 