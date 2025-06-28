#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
坐标系图片优化
"""

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import os

# 配置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
plt.rcParams['axes.unicode_minus'] = False

# 设置输出目录为根级Assets目录
output_dir = '../../Assets'
os.makedirs(output_dir, exist_ok=True)

def setup_clean_3d_style(ax, title=""):
    """设置简洁专业的3D图形样式"""
    ax.set_xlabel('x', fontsize=14, fontweight='bold')
    ax.set_ylabel('y', fontsize=14, fontweight='bold')
    ax.set_zlabel('z', fontsize=14, fontweight='bold')
    if title:
        ax.set_title(title, fontsize=16, fontweight='bold', pad=20)
    
    # 隐藏网格和背景面板
    ax.grid(False)
    ax.xaxis.pane.fill = False
    ax.yaxis.pane.fill = False
    ax.zaxis.pane.fill = False
    ax.xaxis.pane.set_edgecolor('white')
    ax.yaxis.pane.set_edgecolor('white')
    ax.zaxis.pane.set_edgecolor('white')
    ax.xaxis.pane.set_alpha(0)
    ax.yaxis.pane.set_alpha(0)
    ax.zaxis.pane.set_alpha(0)
    
    # 设置背景为白色
    ax.xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    ax.yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
    ax.zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))

def generate_optimized_cylindrical_coord():
    """生成简洁专业的柱坐标系示意图"""
    fig = plt.figure(figsize=(16, 10))
    fig.patch.set_facecolor('white')
    
    # 主视图：柱坐标系3D示意图
    ax1 = fig.add_subplot(1, 2, 1, projection='3d')
    
    # 示例点
    r_point = 1.5
    theta_point = np.pi/3
    z_point = 1.2
    
    x_point = r_point * np.cos(theta_point)
    y_point = r_point * np.sin(theta_point)
    
    # 绘制坐标轴（简洁风格）
    ax1.plot([0, 2.5], [0, 0], [0, 0], 'k-', linewidth=3, alpha=0.8)
    ax1.plot([0, 0], [0, 2.5], [0, 0], 'k-', linewidth=3, alpha=0.8)
    ax1.plot([0, 0], [0, 0], [0, 2.5], 'k-', linewidth=3, alpha=0.8)
    
    # 添加轴标签
    ax1.text(2.7, 0, 0, 'x', fontsize=14, fontweight='bold')
    ax1.text(0, 2.7, 0, 'y', fontsize=14, fontweight='bold')
    ax1.text(0, 0, 2.7, 'z', fontsize=14, fontweight='bold')
    
    # 绘制主要圆柱体（简洁表示）
    theta_full = np.linspace(0, 2*np.pi, 100)
    r_main = 1.8
    
    # 底面圆
    x_base = r_main * np.cos(theta_full)
    y_base = r_main * np.sin(theta_full)
    z_base = np.zeros_like(theta_full)
    ax1.plot(x_base, y_base, z_base, 'royalblue', linewidth=3, alpha=0.8)
    
    # 顶面圆
    z_top = 2.0 * np.ones_like(theta_full)
    ax1.plot(x_base, y_base, z_top, 'royalblue', linewidth=3, alpha=0.8)
    
    # 圆柱体侧面（只画几条关键线）
    for theta_val in [0, np.pi/2, np.pi, 3*np.pi/2]:
        x_line = r_main * np.cos(theta_val)
        y_line = r_main * np.sin(theta_val)
        ax1.plot([x_line, x_line], [y_line, y_line], [0, 2.0], 'royalblue', 
                 alpha=0.6, linewidth=2)
    
    # 绘制几条径向线（突出r的概念）
    for theta_val in [0, np.pi/4, np.pi/2, 3*np.pi/4, np.pi, 5*np.pi/4, 3*np.pi/2, 7*np.pi/4]:
        r_line = np.linspace(0, r_main, 10)
        x_line = r_line * np.cos(theta_val)
        y_line = r_line * np.sin(theta_val)
        z_line = np.zeros_like(r_line)
        ax1.plot(x_line, y_line, z_line, 'lightcoral', alpha=0.4, linewidth=1.5)
    
    # 标记示例点
    ax1.plot([x_point], [y_point], [z_point], 'red', marker='o', markersize=12)
    
    # 从原点到点的投影连线
    ax1.plot([0, x_point], [0, y_point], [0, 0], 'red', linewidth=4, alpha=0.9)
    ax1.plot([x_point, x_point], [y_point, y_point], [0, z_point], 'green', 
             linewidth=4, alpha=0.9)
    
    # 角度弧线（θ）
    theta_arc = np.linspace(0, theta_point, 30)
    x_arc = 0.5 * np.cos(theta_arc)
    y_arc = 0.5 * np.sin(theta_arc)
    z_arc = np.zeros_like(theta_arc)
    ax1.plot(x_arc, y_arc, z_arc, 'orange', linewidth=4)
    
    # 标注
    ax1.text(x_point+0.1, y_point+0.1, z_point+0.2, 'P(r, θ, z)', 
             fontsize=14, fontweight='bold', 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='white', 
                      edgecolor='black', alpha=0.9))
    ax1.text(0.7, 0.2, -0.2, 'r', fontsize=16, color='red', fontweight='bold')
    ax1.text(0.25, 0.25, -0.15, 'θ', fontsize=16, color='orange', fontweight='bold')
    ax1.text(x_point+0.1, y_point+0.1, 0.6, 'z', fontsize=16, color='green', fontweight='bold')
    
    setup_clean_3d_style(ax1, "柱坐标系")
    ax1.set_xlim(-2.5, 2.5)
    ax1.set_ylim(-2.5, 2.5)
    ax1.set_zlim(0, 2.5)
    ax1.view_init(elev=20, azim=45)
    
    # 右侧：公式说明
    ax2 = fig.add_subplot(1, 2, 2)
    ax2.axis('off')
    ax2.set_facecolor('white')
    
    # 标题
    ax2.text(0.5, 0.95, '柱坐标变换公式', fontsize=20, fontweight='bold', 
             ha='center', transform=ax2.transAxes)
    
    # 变换公式
    ax2.text(0.1, 0.85, '坐标变换：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.78, r'$x = r\cos\theta$', fontsize=16, transform=ax2.transAxes)
    ax2.text(0.15, 0.72, r'$y = r\sin\theta$', fontsize=16, transform=ax2.transAxes)
    ax2.text(0.15, 0.66, r'$z = z$', fontsize=16, transform=ax2.transAxes)
    
    # 参数范围
    ax2.text(0.1, 0.56, '其中：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.50, r'$r \geq 0$ (径向距离)', fontsize=14, transform=ax2.transAxes, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightblue', alpha=0.8))
    ax2.text(0.15, 0.44, r'$0 \leq \theta \leq 2\pi$ (方位角)', fontsize=14, transform=ax2.transAxes)
    ax2.text(0.15, 0.38, r'$-\infty < z < +\infty$ (高度)', fontsize=14, transform=ax2.transAxes)
    
    # 体积元素
    ax2.text(0.1, 0.28, '体积元素：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.22, r'$dV = r \, dr \, d\theta \, dz$', fontsize=16, 
             transform=ax2.transAxes, color='red', fontweight='bold',
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightyellow', alpha=0.8))
    
    # Jacobian行列式
    ax2.text(0.1, 0.12, 'Jacobian行列式：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.06, r'$J = r$', fontsize=16, transform=ax2.transAxes, color='red', fontweight='bold')
    
    # 适用场合
    ax2.text(0.55, 0.56, '适用场合：', fontsize=16, fontweight='bold', transform=ax2.transAxes, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightgreen', alpha=0.8))
    ax2.text(0.55, 0.50, '• 圆柱形、圆锥形区域', fontsize=13, transform=ax2.transAxes)
    ax2.text(0.55, 0.46, '• 被积函数含有 $x^2 + y^2$', fontsize=13, transform=ax2.transAxes)
    ax2.text(0.55, 0.42, '• 关于z轴旋转对称的区域', fontsize=13, transform=ax2.transAxes)
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/cylindrical_coord.png', dpi=300, bbox_inches='tight', 
                facecolor='white', edgecolor='none')
    plt.close()

def generate_optimized_spherical_coord():
    """生成简洁专业的球坐标系示意图"""
    fig = plt.figure(figsize=(16, 10))
    fig.patch.set_facecolor('white')
    
    # 主视图：球坐标系3D示意图
    ax1 = fig.add_subplot(1, 2, 1, projection='3d')
    
    # 示例点
    r_point = 1.8
    phi_point = np.pi/3  # 极角
    theta_point = np.pi/4  # 方位角
    
    x_point = r_point * np.sin(phi_point) * np.cos(theta_point)
    y_point = r_point * np.sin(phi_point) * np.sin(theta_point)
    z_point = r_point * np.cos(phi_point)
    
    # 绘制坐标轴（简洁风格）
    ax1.plot([0, 2.5], [0, 0], [0, 0], 'k-', linewidth=3, alpha=0.8)
    ax1.plot([0, 0], [0, 2.5], [0, 0], 'k-', linewidth=3, alpha=0.8)
    ax1.plot([0, 0], [0, 0], [0, 2.5], 'k-', linewidth=3, alpha=0.8)
    
    # 添加轴标签
    ax1.text(2.7, 0, 0, 'x', fontsize=14, fontweight='bold')
    ax1.text(0, 2.7, 0, 'y', fontsize=14, fontweight='bold')
    ax1.text(0, 0, 2.7, 'z', fontsize=14, fontweight='bold')
    
    # 绘制主要球面（简洁表示）
    u = np.linspace(0, 2 * np.pi, 30)
    v = np.linspace(0, np.pi, 20)
    r_sphere = 2.0
    
    x_sphere = r_sphere * np.outer(np.cos(u), np.sin(v))
    y_sphere = r_sphere * np.outer(np.sin(u), np.sin(v))
    z_sphere = r_sphere * np.outer(np.ones(np.size(u)), np.cos(v))
    
    # 绘制球面轮廓（简洁线条）
    ax1.plot_surface(x_sphere, y_sphere, z_sphere, alpha=0.1, color='lightblue')
    
    # 绘制几条关键经线
    for theta_val in [0, np.pi/2, np.pi, 3*np.pi/2]:
        phi_line = np.linspace(0, np.pi, 50)
        x_line = r_sphere * np.cos(theta_val) * np.sin(phi_line)
        y_line = r_sphere * np.sin(theta_val) * np.sin(phi_line)
        z_line = r_sphere * np.cos(phi_line)
        ax1.plot(x_line, y_line, z_line, 'royalblue', alpha=0.6, linewidth=2)
    
    # 绘制几条关键纬线
    for phi_val in [np.pi/4, np.pi/2, 3*np.pi/4]:
        theta_line = np.linspace(0, 2*np.pi, 50)
        x_line = r_sphere * np.cos(theta_line) * np.sin(phi_val)
        y_line = r_sphere * np.sin(theta_line) * np.sin(phi_val)
        z_line = r_sphere * np.cos(phi_val) * np.ones_like(theta_line)
        ax1.plot(x_line, y_line, z_line, 'lightcoral', alpha=0.6, linewidth=2)
    
    # 标记示例点
    ax1.plot([x_point], [y_point], [z_point], 'red', marker='o', markersize=12)
    
    # 从原点到点的连线（径向线）
    ax1.plot([0, x_point], [0, y_point], [0, z_point], 'red', linewidth=4, alpha=0.9)
    
    # 在xy平面的投影
    ax1.plot([x_point], [y_point], [0], 'blue', marker='o', markersize=8)
    ax1.plot([0, x_point], [0, y_point], [0, 0], 'blue', linewidth=2, alpha=0.7, linestyle='--')
    ax1.plot([x_point, x_point], [y_point, y_point], [0, z_point], 'blue', 
             linewidth=2, alpha=0.7, linestyle='--')
    
    # 角度弧线 - 方位角θ（在xy平面）
    theta_arc = np.linspace(0, theta_point, 30)
    x_arc = 0.6 * np.cos(theta_arc)
    y_arc = 0.6 * np.sin(theta_arc)
    z_arc = np.zeros_like(theta_arc)
    ax1.plot(x_arc, y_arc, z_arc, 'orange', linewidth=4)
    
    # 角度弧线 - 极角φ（从z轴到径向线）
    phi_arc = np.linspace(0, phi_point, 30)
    x_arc_phi = 0.8 * np.sin(phi_arc) * np.cos(theta_point)
    y_arc_phi = 0.8 * np.sin(phi_arc) * np.sin(theta_point)
    z_arc_phi = 0.8 * np.cos(phi_arc)
    ax1.plot(x_arc_phi, y_arc_phi, z_arc_phi, 'purple', linewidth=4)
    
    # 标注
    ax1.text(x_point+0.1, y_point+0.1, z_point+0.2, 'P(r, φ, θ)', 
             fontsize=14, fontweight='bold',
             bbox=dict(boxstyle="round,pad=0.3", facecolor='white', 
                      edgecolor='black', alpha=0.9))
    ax1.text(0.6, 0.3, 0.5, 'r', fontsize=16, color='red', fontweight='bold')
    ax1.text(0.3, 0.3, -0.15, 'θ', fontsize=16, color='orange', fontweight='bold')
    ax1.text(0.4, 0.2, 0.6, 'φ', fontsize=16, color='purple', fontweight='bold')
    
    setup_clean_3d_style(ax1, "球坐标系")
    ax1.set_xlim(-2.5, 2.5)
    ax1.set_ylim(-2.5, 2.5)
    ax1.set_zlim(-0.5, 2.5)
    ax1.view_init(elev=20, azim=45)
    
    # 右侧：公式说明
    ax2 = fig.add_subplot(1, 2, 2)
    ax2.axis('off')
    ax2.set_facecolor('white')
    
    # 标题
    ax2.text(0.5, 0.95, '球坐标变换公式', fontsize=20, fontweight='bold', 
             ha='center', transform=ax2.transAxes)
    
    # 变换公式
    ax2.text(0.1, 0.85, '坐标变换：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.78, r'$x = r\sin\varphi\cos\theta$', fontsize=16, transform=ax2.transAxes)
    ax2.text(0.15, 0.72, r'$y = r\sin\varphi\sin\theta$', fontsize=16, transform=ax2.transAxes)
    ax2.text(0.15, 0.66, r'$z = r\cos\varphi$', fontsize=16, transform=ax2.transAxes)
    
    # 参数范围
    ax2.text(0.1, 0.56, '其中：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.50, r'$r \geq 0$ (径向距离)', fontsize=14, transform=ax2.transAxes, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightcoral', alpha=0.8))
    ax2.text(0.15, 0.44, r'$0 \leq \varphi \leq \pi$ (极角，与z轴夹角)', fontsize=14, transform=ax2.transAxes)
    ax2.text(0.15, 0.38, r'$0 \leq \theta \leq 2\pi$ (方位角)', fontsize=14, transform=ax2.transAxes)
    
    # 体积元素
    ax2.text(0.1, 0.28, '体积元素：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.22, r'$dV = r^2\sin\varphi \, dr \, d\varphi \, d\theta$', fontsize=16, 
             transform=ax2.transAxes, color='red', fontweight='bold',
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightyellow', alpha=0.8))
    
    # Jacobian行列式
    ax2.text(0.1, 0.12, 'Jacobian行列式：', fontsize=16, fontweight='bold', transform=ax2.transAxes)
    ax2.text(0.15, 0.06, r'$J = r^2\sin\varphi$', fontsize=16, transform=ax2.transAxes, color='red', fontweight='bold')
    
    # 适用场合
    ax2.text(0.55, 0.56, '适用场合：', fontsize=16, fontweight='bold', transform=ax2.transAxes, 
             bbox=dict(boxstyle="round,pad=0.3", facecolor='lightgreen', alpha=0.8))
    ax2.text(0.55, 0.50, '• 球形、球壳、锥形区域', fontsize=13, transform=ax2.transAxes)
    ax2.text(0.55, 0.46, '• 被积函数含有 $x^2 + y^2 + z^2$', fontsize=13, transform=ax2.transAxes)
    ax2.text(0.55, 0.42, '• 关于原点球对称的区域', fontsize=13, transform=ax2.transAxes)
    
    plt.tight_layout()
    plt.savefig(f'{output_dir}/spherical_coord.png', dpi=300, bbox_inches='tight',
                facecolor='white', edgecolor='none')
    plt.close()

def generate_coordinate_comparison():
    """生成简洁专业的坐标系对比图"""
    fig = plt.figure(figsize=(18, 8))
    fig.patch.set_facecolor('white')
    
    # 直角坐标系
    ax1 = fig.add_subplot(1, 3, 1, projection='3d')
    
    # 绘制坐标轴
    ax1.plot([0, 2], [0, 0], [0, 0], 'k-', linewidth=4)
    ax1.plot([0, 0], [0, 2], [0, 0], 'k-', linewidth=4)
    ax1.plot([0, 0], [0, 0], [0, 2], 'k-', linewidth=4)
    
    # 绘制简洁的立方体框架
    cube_coords = np.array([
        [0, 0, 0], [1.5, 0, 0], [1.5, 1.5, 0], [0, 1.5, 0],  # 底面
        [0, 0, 1.5], [1.5, 0, 1.5], [1.5, 1.5, 1.5], [0, 1.5, 1.5]  # 顶面
    ])
    
    # 底面
    for i in range(4):
        j = (i + 1) % 4
        ax1.plot([cube_coords[i, 0], cube_coords[j, 0]], 
                 [cube_coords[i, 1], cube_coords[j, 1]], 
                 [cube_coords[i, 2], cube_coords[j, 2]], 'b-', alpha=0.6, linewidth=2)
    
    # 顶面
    for i in range(4, 8):
        j = 4 + ((i - 4 + 1) % 4)
        ax1.plot([cube_coords[i, 0], cube_coords[j, 0]], 
                 [cube_coords[i, 1], cube_coords[j, 1]], 
                 [cube_coords[i, 2], cube_coords[j, 2]], 'b-', alpha=0.6, linewidth=2)
    
    # 侧面连接
    for i in range(4):
        ax1.plot([cube_coords[i, 0], cube_coords[i+4, 0]], 
                 [cube_coords[i, 1], cube_coords[i+4, 1]], 
                 [cube_coords[i, 2], cube_coords[i+4, 2]], 'b-', alpha=0.6, linewidth=2)
    
    # 示例点
    ax1.plot([1.2], [1.0], [1.3], 'ro', markersize=12)
    ax1.text(1.3, 1.1, 1.4, '(x, y, z)', fontsize=14, fontweight='bold',
             bbox=dict(boxstyle="round,pad=0.3", facecolor='white', 
                      edgecolor='black', alpha=0.9))
    
    setup_clean_3d_style(ax1, "直角坐标系")
    ax1.set_xlim(0, 2.2)
    ax1.set_ylim(0, 2.2)
    ax1.set_zlim(0, 2.2)
    ax1.view_init(elev=20, azim=45)
    
    # 柱坐标系
    ax2 = fig.add_subplot(1, 3, 2, projection='3d')
    
    # 绘制坐标轴
    ax2.plot([0, 2.5], [0, 0], [0, 0], 'k-', linewidth=4)
    ax2.plot([0, 0], [0, 2.5], [0, 0], 'k-', linewidth=4)
    ax2.plot([0, 0], [0, 0], [0, 2.5], 'k-', linewidth=4)
    
    # 绘制简洁的圆柱体
    theta_full = np.linspace(0, 2*np.pi, 100)
    r_cyl = 1.5
    h_cyl = 2.0
    
    # 底面圆
    x_base = r_cyl * np.cos(theta_full)
    y_base = r_cyl * np.sin(theta_full)
    z_base = np.zeros_like(theta_full)
    ax2.plot(x_base, y_base, z_base, 'royalblue', linewidth=3)
    
    # 顶面圆
    z_top = h_cyl * np.ones_like(theta_full)
    ax2.plot(x_base, y_base, z_top, 'royalblue', linewidth=3)
    
    # 圆柱体侧面线（只画关键几条）
    for theta_val in [0, np.pi/2, np.pi, 3*np.pi/2]:
        x_line = r_cyl * np.cos(theta_val)
        y_line = r_cyl * np.sin(theta_val)
        ax2.plot([x_line, x_line], [y_line, y_line], [0, h_cyl], 
                 'royalblue', linewidth=2, alpha=0.6)
    
    # 示例点
    ax2.plot([1], [1], [1.5], 'ro', markersize=12)
    ax2.text(1.1, 1.1, 1.6, '(r, θ, z)', fontsize=14, fontweight='bold',
             bbox=dict(boxstyle="round,pad=0.3", facecolor='white', 
                      edgecolor='black', alpha=0.9))
    
    setup_clean_3d_style(ax2, "柱坐标系")
    ax2.set_xlim(-2.2, 2.2)
    ax2.set_ylim(-2.2, 2.2)
    ax2.set_zlim(0, 2.2)
    ax2.view_init(elev=20, azim=45)
    
    # 球坐标系
    ax3 = fig.add_subplot(1, 3, 3, projection='3d')
    
    # 绘制坐标轴
    ax3.plot([0, 2.5], [0, 0], [0, 0], 'k-', linewidth=4)
    ax3.plot([0, 0], [0, 2.5], [0, 0], 'k-', linewidth=4)
    ax3.plot([0, 0], [0, 0], [0, 2.5], 'k-', linewidth=4)
    
    # 绘制简洁的球体
    u = np.linspace(0, 2 * np.pi, 30)
    v = np.linspace(0, np.pi, 20)
    r_sphere = 1.8
    
    x_sphere = r_sphere * np.outer(np.cos(u), np.sin(v))
    y_sphere = r_sphere * np.outer(np.sin(u), np.sin(v))
    z_sphere = r_sphere * np.outer(np.ones(np.size(u)), np.cos(v))
    
    # 绘制球面（半透明）
    ax3.plot_surface(x_sphere, y_sphere, z_sphere, alpha=0.15, color='lightblue')
    
    # 绘制几条关键经线和纬线
    for theta_val in [0, np.pi/2, np.pi, 3*np.pi/2]:
        phi_line = np.linspace(0, np.pi, 30)
        x_line = r_sphere * np.cos(theta_val) * np.sin(phi_line)
        y_line = r_sphere * np.sin(theta_val) * np.sin(phi_line)
        z_line = r_sphere * np.cos(phi_line)
        ax3.plot(x_line, y_line, z_line, 'royalblue', alpha=0.6, linewidth=2)
    
    for phi_val in [np.pi/4, np.pi/2, 3*np.pi/4]:
        theta_line = np.linspace(0, 2*np.pi, 50)
        x_line = r_sphere * np.cos(theta_line) * np.sin(phi_val)
        y_line = r_sphere * np.sin(theta_line) * np.sin(phi_val)
        z_line = r_sphere * np.cos(phi_val) * np.ones_like(theta_line)
        ax3.plot(x_line, y_line, z_line, 'lightcoral', alpha=0.6, linewidth=2)
    
    # 示例点
    ax3.plot([1], [1], [1], 'ro', markersize=12)
    ax3.text(1.1, 1.1, 1.1, '(r, φ, θ)', fontsize=14, fontweight='bold',
             bbox=dict(boxstyle="round,pad=0.3", facecolor='white', 
                      edgecolor='black', alpha=0.9))
    
    setup_clean_3d_style(ax3, "球坐标系")
    ax3.set_xlim(-2.2, 2.2)
    ax3.set_ylim(-2.2, 2.2)
    ax3.set_zlim(-0.5, 2.2)
    ax3.view_init(elev=20, azim=45)
    
    plt.suptitle('三种坐标系比较', fontsize=24, fontweight='bold', y=0.95)
    plt.tight_layout()
    plt.savefig(f'{output_dir}/coord_comparison.png', dpi=300, bbox_inches='tight',
                facecolor='white', edgecolor='none')
    plt.close()

# 运行生成函数
if __name__ == "__main__":
    print("开始生成简洁专业风格的坐标系图片...")
    print(f"输出目录: {output_dir}")
    
    print("1. 生成简洁专业的柱坐标系示意图...")
    generate_optimized_cylindrical_coord()
    
    print("2. 生成简洁专业的球坐标系示意图...")
    generate_optimized_spherical_coord()
    
    print("3. 生成简洁专业的坐标系对比图...")
    generate_coordinate_comparison()
    
    print("坐标系图片优化完成！")
    print("采用简洁专业风格：")
    print("- 去除复杂网格和背景")
    print("- 突出关键几何特征")
    print("- 采用清晰的几何体表示")
    print("- 专业的数学插图风格") 