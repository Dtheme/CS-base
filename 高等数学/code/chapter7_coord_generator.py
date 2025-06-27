#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
第7章坐标系图片生成器
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

def generate_cylindrical_coord():
    """生成柱坐标系示意图"""
    fig = plt.figure(figsize=(16, 12))
    
    # 主图：柱坐标系3D示意
    ax1 = fig.add_subplot(2, 2, 1, projection='3d')
    
    # 绘制柱坐标系的典型点
    rho = 2.0
    phi = np.pi/3
    z = 1.5
    
    x = rho * np.cos(phi)
    y = rho * np.sin(phi)
    
    # 点P
    ax1.scatter([x], [y], [z], color='red', s=100)
    ax1.text(x + 0.2, y + 0.2, z + 0.2, f'P({x:.1f},{y:.1f},{z:.1f})', fontsize=12)
    
    # 从原点到P的投影
    ax1.plot([0, x], [0, y], [0, 0], 'b--', linewidth=2, label='ρ投影')
    ax1.plot([x, x], [y, y], [0, z], 'g--', linewidth=2, label='z坐标')
    ax1.plot([0, x], [0, y], [z, z], 'r-', linewidth=2, alpha=0.5)
    
    # 绘制圆弧表示角度φ
    phi_arc = np.linspace(0, phi, 50)
    x_arc = 0.5 * np.cos(phi_arc)
    y_arc = 0.5 * np.sin(phi_arc)
    z_arc = np.zeros_like(phi_arc)
    ax1.plot(x_arc, y_arc, z_arc, 'orange', linewidth=3)
    ax1.text(0.7, 0.3, 0, 'φ', fontsize=14, color='orange', fontweight='bold')
    
    # 绘制柱坐标网格
    # ρ = 常数的圆
    for rho_val in [1, 2, 3]:
        theta_circle = np.linspace(0, 2*np.pi, 100)
        x_circle = rho_val * np.cos(theta_circle)
        y_circle = rho_val * np.sin(theta_circle)
        z_circle = np.zeros_like(theta_circle)
        ax1.plot(x_circle, y_circle, z_circle, 'b-', alpha=0.3)
        
        # 在不同高度重复
        for z_level in [1, 2]:
            z_circle_level = z_level * np.ones_like(theta_circle)
            ax1.plot(x_circle, y_circle, z_circle_level, 'b-', alpha=0.3)
    
    # φ = 常数的射线
    for phi_val in np.linspace(0, 2*np.pi, 8, endpoint=False):
        rho_line = np.linspace(0, 3, 20)
        x_line = rho_line * np.cos(phi_val)
        y_line = rho_line * np.sin(phi_val)
        for z_level in [0, 1, 2]:
            z_line = z_level * np.ones_like(rho_line)
            ax1.plot(x_line, y_line, z_line, 'r-', alpha=0.3)
    
    # z = 常数的平面（网格）
    for z_level in [0.5, 1.0, 1.5, 2.0]:
        x_grid = np.linspace(-3, 3, 10)
        y_grid = np.linspace(-3, 3, 10)
        X_grid, Y_grid = np.meshgrid(x_grid, y_grid)
        Z_grid = z_level * np.ones_like(X_grid)
        mask = X_grid**2 + Y_grid**2 <= 9  # 限制在圆内
        ax1.plot_wireframe(X_grid, Y_grid, Z_grid, alpha=0.1, color='green')
    
    setup_3d_style(ax1, "柱坐标系")
    ax1.set_xlim(-3, 3)
    ax1.set_ylim(-3, 3)
    ax1.set_zlim(0, 3)
    
    # 坐标变换公式
    ax2 = fig.add_subplot(2, 2, 2)
    ax2.text(0.1, 0.8, '柱坐标变换公式：', fontsize=16, fontweight='bold')
    ax2.text(0.1, 0.7, r'$x = \rho \cos \phi$', fontsize=14)
    ax2.text(0.1, 0.6, r'$y = \rho \sin \phi$', fontsize=14)
    ax2.text(0.1, 0.5, r'$z = z$', fontsize=14)
    
    ax2.text(0.1, 0.35, '反变换：', fontsize=16, fontweight='bold')
    ax2.text(0.1, 0.25, r'$\rho = \sqrt{x^2 + y^2}$', fontsize=14)
    ax2.text(0.1, 0.15, r'$\phi = \arctan\frac{y}{x}$', fontsize=14)
    ax2.text(0.1, 0.05, r'$z = z$', fontsize=14)
    
    ax2.set_xlim(0, 1)
    ax2.set_ylim(0, 1)
    ax2.axis('off')
    
    # 体积元素
    ax3 = fig.add_subplot(2, 2, 3)
    ax3.text(0.1, 0.8, '体积元素：', fontsize=16, fontweight='bold', color='red')
    ax3.text(0.1, 0.6, r'$dV = \rho \, d\rho \, d\phi \, dz$', fontsize=18, color='red')
    
    ax3.text(0.1, 0.4, '积分范围：', fontsize=16, fontweight='bold')
    ax3.text(0.1, 0.3, r'$0 \leq \rho < \infty$', fontsize=14)
    ax3.text(0.1, 0.2, r'$0 \leq \phi \leq 2\pi$', fontsize=14)
    ax3.text(0.1, 0.1, r'$-\infty < z < \infty$', fontsize=14)
    
    ax3.set_xlim(0, 1)
    ax3.set_ylim(0, 1)
    ax3.axis('off')
    
    # xy平面投影
    ax4 = fig.add_subplot(2, 2, 4)
    
    # 绘制极坐标网格
    theta = np.linspace(0, 2*np.pi, 100)
    
    for r in [1, 2, 3]:
        x_circle = r * np.cos(theta)
        y_circle = r * np.sin(theta)
        ax4.plot(x_circle, y_circle, 'b-', alpha=0.5, label=f'ρ={r}' if r == 1 else "")
    
    for angle in np.linspace(0, 2*np.pi, 8, endpoint=False):
        r_line = np.linspace(0, 3, 20)
        x_line = r_line * np.cos(angle)
        y_line = r_line * np.sin(angle)
        ax4.plot(x_line, y_line, 'r-', alpha=0.5)
    
    # 标记示例点的投影
    ax4.plot(x, y, 'ro', markersize=10)
    ax4.text(x + 0.2, y + 0.2, f'({x:.1f},{y:.1f})', fontsize=12)
    
    # 绘制角度φ
    phi_arc = np.linspace(0, phi, 50)
    x_arc = 0.5 * np.cos(phi_arc)
    y_arc = 0.5 * np.sin(phi_arc)
    ax4.plot(x_arc, y_arc, 'orange', linewidth=3)
    ax4.text(0.7, 0.3, 'φ', fontsize=14, color='orange', fontweight='bold')
    
    ax4.set_xlabel('x', fontsize=12)
    ax4.set_ylabel('y', fontsize=12)
    ax4.set_title('xy平面投影（极坐标）', fontsize=14)
    ax4.grid(True, alpha=0.3)
    ax4.set_xlim(-3.5, 3.5)
    ax4.set_ylim(-3.5, 3.5)
    ax4.set_aspect('equal')
    
    plt.suptitle('柱坐标系详解', fontsize=18, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/cylindrical_coord.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_spherical_coord():
    """生成球坐标系示意图"""
    fig = plt.figure(figsize=(16, 12))
    
    # 主图：球坐标系3D示意
    ax1 = fig.add_subplot(2, 2, 1, projection='3d')
    
    # 绘制球坐标系的典型点
    r = 2.5
    theta = np.pi/4  # 方位角
    phi = np.pi/3    # 极角（从z轴正向）
    
    x = r * np.sin(phi) * np.cos(theta)
    y = r * np.sin(phi) * np.sin(theta)
    z = r * np.cos(phi)
    
    # 点P
    ax1.scatter([x], [y], [z], color='red', s=100)
    ax1.text(x + 0.2, y + 0.2, z + 0.2, f'P({x:.1f},{y:.1f},{z:.1f})', fontsize=12)
    
    # 从原点到P的向量
    ax1.plot([0, x], [0, y], [0, z], 'r-', linewidth=3, label='r')
    ax1.text(x/2, y/2, z/2 + 0.3, 'r', fontsize=14, color='red', fontweight='bold')
    
    # 投影到xy平面
    ax1.plot([0, x], [0, y], [0, 0], 'b--', linewidth=2, alpha=0.7)
    ax1.plot([x, x], [y, y], [0, z], 'g--', linewidth=2, alpha=0.7)
    
    # 绘制方位角θ (在xy平面内，从x轴到投影的角度)
    theta_arc = np.linspace(0, theta, 50)
    x_arc = 0.8 * np.cos(theta_arc)
    y_arc = 0.8 * np.sin(theta_arc)
    z_arc = np.zeros_like(theta_arc)
    ax1.plot(x_arc, y_arc, z_arc, 'blue', linewidth=3)
    ax1.text(1.0, 0.4, 0, 'θ', fontsize=14, color='blue', fontweight='bold')
    
    # 绘制极角φ (从z轴到r的角度)
    # 在包含z轴和P点的平面内
    phi_arc = np.linspace(0, phi, 50)
    x_phi = r * 0.3 * np.sin(phi_arc) * np.cos(theta)
    y_phi = r * 0.3 * np.sin(phi_arc) * np.sin(theta)
    z_phi = r * 0.3 * np.cos(phi_arc)
    ax1.plot(x_phi, y_phi, z_phi, 'green', linewidth=3)
    ax1.text(0.3, 0.3, 1.8, 'φ', fontsize=14, color='green', fontweight='bold')
    
    # 绘制球面网格
    # r = 常数的球面
    for r_val in [1, 2, 3]:
        u = np.linspace(0, 2 * np.pi, 20)
        v = np.linspace(0, np.pi, 15)
        x_sphere = r_val * np.outer(np.cos(u), np.sin(v))
        y_sphere = r_val * np.outer(np.sin(u), np.sin(v))
        z_sphere = r_val * np.outer(np.ones(np.size(u)), np.cos(v))
        ax1.plot_wireframe(x_sphere, y_sphere, z_sphere, alpha=0.2, color='gray')
    
    # θ = 常数的半平面
    for theta_val in np.linspace(0, 2*np.pi, 8, endpoint=False):
        phi_line = np.linspace(0, np.pi, 30)
        r_line = 3
        x_line = r_line * np.sin(phi_line) * np.cos(theta_val)
        y_line = r_line * np.sin(phi_line) * np.sin(theta_val)
        z_line = r_line * np.cos(phi_line)
        ax1.plot(x_line, y_line, z_line, 'b-', alpha=0.3)
    
    # φ = 常数的圆锥面
    for phi_val in [np.pi/6, np.pi/3, np.pi/2, 2*np.pi/3, 5*np.pi/6]:
        theta_cone = np.linspace(0, 2*np.pi, 50)
        r_cone = 3
        x_cone = r_cone * np.sin(phi_val) * np.cos(theta_cone)
        y_cone = r_cone * np.sin(phi_val) * np.sin(theta_cone)
        z_cone = r_cone * np.cos(phi_val) * np.ones_like(theta_cone)
        ax1.plot(x_cone, y_cone, z_cone, 'g-', alpha=0.3)
    
    setup_3d_style(ax1, "球坐标系")
    ax1.set_xlim(-3, 3)
    ax1.set_ylim(-3, 3)
    ax1.set_zlim(-3, 3)
    
    # 坐标变换公式
    ax2 = fig.add_subplot(2, 2, 2)
    ax2.text(0.1, 0.85, '球坐标变换公式：', fontsize=16, fontweight='bold')
    ax2.text(0.1, 0.75, r'$x = r \sin \phi \cos \theta$', fontsize=14)
    ax2.text(0.1, 0.65, r'$y = r \sin \phi \sin \theta$', fontsize=14)
    ax2.text(0.1, 0.55, r'$z = r \cos \phi$', fontsize=14)
    
    ax2.text(0.1, 0.4, '反变换：', fontsize=16, fontweight='bold')
    ax2.text(0.1, 0.3, r'$r = \sqrt{x^2 + y^2 + z^2}$', fontsize=12)
    ax2.text(0.1, 0.2, r'$\theta = \arctan\frac{y}{x}$', fontsize=12)
    ax2.text(0.1, 0.1, r'$\phi = \arccos\frac{z}{\sqrt{x^2+y^2+z^2}}$', fontsize=12)
    
    ax2.set_xlim(0, 1)
    ax2.set_ylim(0, 1)
    ax2.axis('off')
    
    # 体积元素
    ax3 = fig.add_subplot(2, 2, 3)
    ax3.text(0.1, 0.8, '体积元素：', fontsize=16, fontweight='bold', color='red')
    ax3.text(0.1, 0.6, r'$dV = r^2 \sin \phi \, dr \, d\phi \, d\theta$', fontsize=16, color='red')
    
    ax3.text(0.1, 0.4, '积分范围：', fontsize=16, fontweight='bold')
    ax3.text(0.1, 0.3, r'$0 \leq r < \infty$', fontsize=14)
    ax3.text(0.1, 0.2, r'$0 \leq \phi \leq \pi$', fontsize=14)
    ax3.text(0.1, 0.1, r'$0 \leq \theta \leq 2\pi$', fontsize=14)
    
    ax3.set_xlim(0, 1)
    ax3.set_ylim(0, 1)
    ax3.axis('off')
    
    # 角度说明图
    ax4 = fig.add_subplot(2, 2, 4, projection='3d')
    
    # 绘制坐标轴
    ax4.plot([0, 2], [0, 0], [0, 0], 'k-', linewidth=2)
    ax4.plot([0, 0], [0, 2], [0, 0], 'k-', linewidth=2)
    ax4.plot([0, 0], [0, 0], [0, 2], 'k-', linewidth=2)
    ax4.text(2.1, 0, 0, 'x', fontsize=12)
    ax4.text(0, 2.1, 0, 'y', fontsize=12)
    ax4.text(0, 0, 2.1, 'z', fontsize=12)
    
    # 示例向量
    r_demo = 1.5
    theta_demo = np.pi/6
    phi_demo = np.pi/4
    
    x_demo = r_demo * np.sin(phi_demo) * np.cos(theta_demo)
    y_demo = r_demo * np.sin(phi_demo) * np.sin(theta_demo)
    z_demo = r_demo * np.cos(phi_demo)
    
    ax4.plot([0, x_demo], [0, y_demo], [0, z_demo], 'r-', linewidth=3)
    ax4.scatter([x_demo], [y_demo], [z_demo], color='red', s=100)
    
    # 投影
    ax4.plot([0, x_demo], [0, y_demo], [0, 0], 'b--', linewidth=2, alpha=0.7)
    
    # 角度弧
    theta_arc = np.linspace(0, theta_demo, 30)
    x_arc = 0.5 * np.cos(theta_arc)
    y_arc = 0.5 * np.sin(theta_arc)
    z_arc = np.zeros_like(theta_arc)
    ax4.plot(x_arc, y_arc, z_arc, 'blue', linewidth=3)
    ax4.text(0.6, 0.2, 0, 'θ', fontsize=12, color='blue', fontweight='bold')
    
    phi_arc = np.linspace(0, phi_demo, 30)
    x_phi = 0.5 * np.sin(phi_arc) * np.cos(theta_demo)
    y_phi = 0.5 * np.sin(phi_arc) * np.sin(theta_demo)
    z_phi = 0.5 * np.cos(phi_arc)
    ax4.plot(x_phi, y_phi, z_phi, 'green', linewidth=3)
    ax4.text(0.2, 0.2, 0.4, 'φ', fontsize=12, color='green', fontweight='bold')
    
    ax4.set_xlim(-0.5, 2)
    ax4.set_ylim(-0.5, 2)
    ax4.set_zlim(-0.5, 2)
    ax4.set_title('角度定义', fontsize=14)
    ax4.grid(True, alpha=0.3)
    
    plt.suptitle('球坐标系详解', fontsize=18, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/spherical_coord.png', dpi=300, bbox_inches='tight')
    plt.close()

def generate_coord_comparison():
    """生成坐标系比较图"""
    fig = plt.figure(figsize=(18, 12))
    
    # 直角坐标系
    ax1 = fig.add_subplot(2, 3, 1, projection='3d')
    
    # 绘制网格
    x_grid = np.linspace(-2, 2, 5)
    y_grid = np.linspace(-2, 2, 5)
    z_grid = np.linspace(-2, 2, 5)
    
    # x = 常数的平面
    for x_val in x_grid:
        Y, Z = np.meshgrid(y_grid, z_grid)
        X = x_val * np.ones_like(Y)
        ax1.plot_wireframe(X, Y, Z, alpha=0.3, color='red')
    
    # y = 常数的平面  
    for y_val in y_grid:
        X, Z = np.meshgrid(x_grid, z_grid)
        Y = y_val * np.ones_like(X)
        ax1.plot_wireframe(X, Y, Z, alpha=0.3, color='green')
        
    # z = 常数的平面
    for z_val in z_grid:
        X, Y = np.meshgrid(x_grid, y_grid)
        Z = z_val * np.ones_like(X)
        ax1.plot_wireframe(X, Y, Z, alpha=0.3, color='blue')
    
    setup_3d_style(ax1, "直角坐标系")
    ax1.text(0, 0, 2.5, r'$dV = dx \, dy \, dz$', fontsize=12, ha='center')
    
    # 柱坐标系
    ax2 = fig.add_subplot(2, 3, 2, projection='3d')
    
    # ρ = 常数的圆柱面
    for rho in [0.5, 1.0, 1.5, 2.0]:
        theta = np.linspace(0, 2*np.pi, 50)
        z = np.linspace(-2, 2, 20)
        THETA, Z = np.meshgrid(theta, z)
        X = rho * np.cos(THETA)
        Y = rho * np.sin(THETA)
        ax2.plot_wireframe(X, Y, Z, alpha=0.3, color='red')
    
    # φ = 常数的半平面
    for phi in np.linspace(0, 2*np.pi, 8, endpoint=False):
        rho = np.linspace(0, 2, 20)
        z = np.linspace(-2, 2, 20)
        RHO, Z = np.meshgrid(rho, z)
        X = RHO * np.cos(phi)
        Y = RHO * np.sin(phi)
        ax2.plot_wireframe(X, Y, Z, alpha=0.3, color='green')
    
    # z = 常数的平面
    for z_val in [-1, 0, 1]:
        theta = np.linspace(0, 2*np.pi, 50)
        rho = np.linspace(0, 2, 20)
        THETA, RHO = np.meshgrid(theta, rho)
        X = RHO * np.cos(THETA)
        Y = RHO * np.sin(THETA)
        Z = z_val * np.ones_like(X)
        ax2.plot_wireframe(X, Y, Z, alpha=0.3, color='blue')
    
    setup_3d_style(ax2, "柱坐标系")
    ax2.text(0, 0, 2.5, r'$dV = \rho \, d\rho \, d\phi \, dz$', fontsize=12, ha='center')
    
    # 球坐标系
    ax3 = fig.add_subplot(2, 3, 3, projection='3d')
    
    # r = 常数的球面
    for r in [0.5, 1.0, 1.5, 2.0]:
        u = np.linspace(0, 2 * np.pi, 30)
        v = np.linspace(0, np.pi, 20)
        x = r * np.outer(np.cos(u), np.sin(v))
        y = r * np.outer(np.sin(u), np.sin(v))
        z = r * np.outer(np.ones(np.size(u)), np.cos(v))
        ax3.plot_wireframe(x, y, z, alpha=0.3, color='red')
    
    # θ = 常数的半平面
    for theta in np.linspace(0, 2*np.pi, 8, endpoint=False):
        phi = np.linspace(0, np.pi, 30)
        r = np.linspace(0, 2, 20)
        PHI, R = np.meshgrid(phi, r)
        X = R * np.sin(PHI) * np.cos(theta)
        Y = R * np.sin(PHI) * np.sin(theta)
        Z = R * np.cos(PHI)
        ax3.plot_wireframe(X, Y, Z, alpha=0.3, color='green')
    
    # φ = 常数的圆锥面
    for phi in [np.pi/6, np.pi/3, np.pi/2, 2*np.pi/3, 5*np.pi/6]:
        theta = np.linspace(0, 2*np.pi, 50)
        r = np.linspace(0, 2, 20)
        THETA, R = np.meshgrid(theta, r)
        X = R * np.sin(phi) * np.cos(THETA)
        Y = R * np.sin(phi) * np.sin(THETA)
        Z = R * np.cos(phi) * np.ones_like(THETA)
        ax3.plot_wireframe(X, Y, Z, alpha=0.3, color='blue')
    
    setup_3d_style(ax3, "球坐标系")
    ax3.text(0, 0, 2.5, r'$dV = r^2\sin\phi \, dr \, d\phi \, d\theta$', fontsize=12, ha='center')
    
    # 变换公式对比
    ax4 = fig.add_subplot(2, 3, (4, 6))
    
    formula_text = """
坐标变换公式对比

直角坐标系 (x, y, z)：
• 基本坐标系
• dV = dx dy dz
• 适用：长方体、简单几何体

柱坐标系 (ρ, φ, z)：
• x = ρ cos φ
• y = ρ sin φ  
• z = z
• dV = ρ dρ dφ dz
• 适用：圆柱体、旋转体

球坐标系 (r, θ, φ)：
• x = r sin φ cos θ
• y = r sin φ sin θ
• z = r cos φ
• dV = r² sin φ dr dφ dθ
• 适用：球体、球冠、球壳

选择原则：
1. 积分区域的几何特征
2. 被积函数的形式
3. 计算的简便性
"""
    
    ax4.text(0.05, 0.95, formula_text, fontsize=12, ha='left', va='top',
             transform=ax4.transAxes, 
             bbox=dict(boxstyle="round,pad=0.5", facecolor='lightblue', alpha=0.8))
    
    ax4.set_xlim(0, 1)
    ax4.set_ylim(0, 1)
    ax4.axis('off')
    
    plt.suptitle('三种坐标系统比较', fontsize=20, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/coord_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()

# 生成极坐标应用例题图
def generate_polar_application_examples():
    """生成极坐标应用例题图"""
    
    # 例题7.2：立体体积计算
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    # 左图：3D立体图
    ax1 = plt.subplot(1, 2, 1, projection='3d')
    
    # 绘制paraboloid z = x² + y²和平面z = 4的交线
    theta = np.linspace(0, 2*np.pi, 50)
    r = np.linspace(0, 2, 30)
    THETA, R = np.meshgrid(theta, r)
    
    X = R * np.cos(THETA)
    Y = R * np.sin(THETA)
    Z1 = X**2 + Y**2  # 抛物面
    Z2 = 4 * np.ones_like(X)  # 平面z=4
    
    # 只绘制z≤4的部分
    mask = Z1 <= 4
    X_masked = X.copy()
    Y_masked = Y.copy()
    Z1_masked = Z1.copy()
    X_masked[~mask] = np.nan
    Y_masked[~mask] = np.nan
    Z1_masked[~mask] = np.nan
    
    ax1.plot_surface(X_masked, Y_masked, Z1_masked, alpha=0.6, cmap='viridis', label='z = x² + y²')
    
    # 绘制平面z=4（在圆x²+y²≤4内）
    r_plane = np.linspace(0, 2, 20)
    theta_plane = np.linspace(0, 2*np.pi, 50)
    R_plane, THETA_plane = np.meshgrid(r_plane, theta_plane)
    X_plane = R_plane * np.cos(THETA_plane)
    Y_plane = R_plane * np.sin(THETA_plane)
    Z_plane = 4 * np.ones_like(X_plane)
    
    ax1.plot_surface(X_plane, Y_plane, Z_plane, alpha=0.4, color='red')
    
    setup_3d_style(ax1, "立体体积计算")
    ax1.text(0, 0, 5, r'$z = 4$', fontsize=12)
    ax1.text(1.5, 1.5, 1, r'$z = x^2 + y^2$', fontsize=12)
    
    # 右图：区域在xy平面的投影
    theta_circle = np.linspace(0, 2*np.pi, 100)
    x_circle = 2 * np.cos(theta_circle)
    y_circle = 2 * np.sin(theta_circle)
    
    ax2.fill(x_circle, y_circle, alpha=0.3, color='lightblue', label='积分区域')
    ax2.plot(x_circle, y_circle, 'b-', linewidth=3, label=r'$x^2 + y^2 = 4$')
    
    # 极坐标网格
    for r in [0.5, 1.0, 1.5, 2.0]:
        x_r = r * np.cos(theta_circle)
        y_r = r * np.sin(theta_circle)
        ax2.plot(x_r, y_r, 'k--', alpha=0.3)
        ax2.text(r + 0.1, 0, f'r={r}', fontsize=10)
    
    for angle in np.linspace(0, 2*np.pi, 8, endpoint=False):
        x_line = [0, 2.5 * np.cos(angle)]
        y_line = [0, 2.5 * np.sin(angle)]
        ax2.plot(x_line, y_line, 'k--', alpha=0.3)
    
    ax2.set_xlabel('x', fontsize=12)
    ax2.set_ylabel('y', fontsize=12)
    ax2.set_title('区域投影（极坐标）', fontsize=14)
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    ax2.set_xlim(-2.5, 2.5)
    ax2.set_ylim(-2.5, 2.5)
    ax2.set_aspect('equal')
    
    # 添加积分公式
    formula_text = r'$V = \iint_D (4 - x^2 - y^2) \, dA = \int_0^{2\pi} \int_0^2 (4 - r^2) r \, dr \, d\theta$'
    ax2.text(-2.3, -2.8, formula_text, fontsize=12, 
             bbox=dict(boxstyle="round", facecolor='lightblue', alpha=0.8))
    
    plt.suptitle('例题7.2：极坐标在立体体积计算中的应用', fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/polar_example_7_2.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 例题7.4：转动惯量计算
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    # 左图：单位圆盘
    ax1 = plt.subplot(1, 2, 1, projection='polar')
    
    theta = np.linspace(0, 2*np.pi, 100)
    r = np.ones_like(theta)
    
    ax1.fill_between(theta, 0, r, alpha=0.3, color='orange', label='单位圆盘')
    ax1.plot(theta, r, 'r-', linewidth=3, label=r'$r = 1$')
    
    # 极坐标网格
    for r_val in [0.2, 0.4, 0.6, 0.8, 1.0]:
        theta_full = np.linspace(0, 2*np.pi, 100)
        ax1.plot(theta_full, [r_val]*len(theta_full), 'k--', alpha=0.3)
    
    ax1.set_title('单位圆盘\n' + r'$D: x^2 + y^2 \leq 1$', fontsize=14, fontweight='bold')
    ax1.legend(loc='upper right')
    
    # 右图：直角坐标表示
    theta_circle = np.linspace(0, 2*np.pi, 100)
    x_circle = np.cos(theta_circle)
    y_circle = np.sin(theta_circle)
    
    ax2.fill(x_circle, y_circle, alpha=0.3, color='orange', label='单位圆盘')
    ax2.plot(x_circle, y_circle, 'r-', linewidth=3, label=r'$x^2 + y^2 = 1$')
    
    # 示例点
    ax2.plot(0.6, 0.8, 'ko', markersize=8)
    ax2.text(0.65, 0.85, r'$(x,y)$', fontsize=12)
    ax2.plot([0, 0.6], [0, 0.8], 'k--', alpha=0.5)
    ax2.text(0.3, 0.45, r'$\sqrt{x^2+y^2}$', fontsize=10, rotation=50)
    
    ax2.set_xlabel('x', fontsize=12)
    ax2.set_ylabel('y', fontsize=12)
    ax2.set_title('转动惯量计算', fontsize=14)
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    ax2.set_xlim(-1.2, 1.2)
    ax2.set_ylim(-1.2, 1.2)
    ax2.set_aspect('equal')
    
    # 添加转动惯量公式
    formula_text = r'$I_z = \iint_D (x^2 + y^2) \, dA = \int_0^{2\pi} \int_0^1 r^2 \cdot r \, dr \, d\theta = \int_0^{2\pi} \int_0^1 r^3 \, dr \, d\theta$'
    ax2.text(-1.1, -1.4, formula_text, fontsize=12, 
             bbox=dict(boxstyle="round", facecolor='orange', alpha=0.8))
    
    plt.suptitle('例题7.4：极坐标在转动惯量计算中的应用', fontsize=16, fontweight='bold')
    plt.tight_layout()
    plt.savefig(f'{output_dir}/polar_example_7_4.png', dpi=300, bbox_inches='tight')
    plt.close()

# 运行生成函数
if __name__ == "__main__":
    print("开始生成坐标系相关图片...")
    
    print("1. 生成柱坐标系示意图...")
    generate_cylindrical_coord()
    
    print("2. 生成球坐标系示意图...")
    generate_spherical_coord()
    
    print("3. 生成坐标系比较图...")
    generate_coord_comparison()
    
    print("4. 生成极坐标应用例题图...")
    generate_polar_application_examples()
    
    print("坐标系相关图片生成完成！") 