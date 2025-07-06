#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
坐标系可视化优化器 - 最终版本
修复字体显示问题，优化球坐标系视角
"""

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.patches as patches
import warnings
import os
import platform
warnings.filterwarnings('ignore')

def setup_chinese_font():
    """设置中文字体，确保所有文字都能正常显示"""
    # 配置中文字体 - 使用简单有效的字体设置
    plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
    plt.rcParams['axes.unicode_minus'] = False
    plt.rcParams['mathtext.fontset'] = 'stix'
    plt.rcParams['font.size'] = 14
    plt.rcParams['axes.linewidth'] = 2.0
    plt.rcParams['grid.alpha'] = 0.2
    plt.rcParams['figure.dpi'] = 150
    plt.rcParams['savefig.dpi'] = 400
    
    return True

def create_cylindrical_coordinate_system():
    """创建高清晰度的柱坐标系示意图 - 参考三坐标系对比图风格"""
    fig = plt.figure(figsize=(24, 16))
    
    # 使用GridSpec实现精确布局控制 - 参考三重积分作图规范
    gs = fig.add_gridspec(2, 4, height_ratios=[2.5, 1], width_ratios=[1.8, 1.2, 1.2, 1], 
                         hspace=0.2, wspace=0.15, left=0.02, right=0.98, top=0.93, bottom=0.05)
    
    # 主标题 - 使用规范标准字体和颜色
    fig.suptitle('柱坐标系详解：变换公式与体积元素', fontsize=22, fontweight='bold',
                color='#2F2F2F', y=0.95)
    
    # 左侧主3D图（占2行第1列）
    ax_main = fig.add_subplot(gs[:, 0], projection='3d')
    ax_main.set_facecolor('#F8F9FA')
    
    # 参考三坐标系对比图的高对比度配色方案
    colors = {
        'x_axis': '#E74C3C',    # 明亮红色
        'y_axis': '#27AE60',    # 明亮绿色  
        'z_axis': '#3498DB',    # 明亮蓝色
        'r_axis': '#E74C3C',    # r用红色（与x轴同色强调径向关系）
        'theta': '#F39C12',     # θ用橙色
        'point': '#FF1493',     # 点P用亮粉色
        'grid': '#7F8C8D',      # 网格灰色
        'background': '#F8F9FA'
    }
    
    # 定义示例点P的坐标 - 与三坐标系对比图保持完全一致
    px, py, pz = 1.0, np.sqrt(3), 2.0  # 直角坐标
    pr = 2.0  # √(1² + (√3)²) = 2，径向距离
    ptheta = np.pi/3  # arctan(√3/1) = 60°，方位角
    pz_cyl = 2.0  # z坐标不变
    
    # 设置最佳视角 - 与三坐标系对比图一致
    ax_main.view_init(elev=20, azim=45)
    
    # === 1. 绘制坐标轴 - 参考三坐标系对比图的风格 ===
    axis_length = 2.8
    # x轴（θ角的起始边）
    ax_main.quiver(0, 0, 0, axis_length, 0, 0, color=colors['x_axis'], 
                  linewidth=5, alpha=1.0, arrow_length_ratio=0.08)
    ax_main.text(axis_length+0.2, -0.1, -0.1, 'x', fontsize=20, 
                color=colors['x_axis'], fontweight='bold')
    
    # y轴
    ax_main.quiver(0, 0, 0, 0, axis_length, 0, color=colors['y_axis'], 
                  linewidth=5, alpha=1.0, arrow_length_ratio=0.08)
    ax_main.text(-0.1, axis_length+0.2, -0.1, 'y', fontsize=20, 
                color=colors['y_axis'], fontweight='bold')
    
    # z轴
    ax_main.quiver(0, 0, 0, 0, 0, axis_length, color=colors['z_axis'], 
                  linewidth=5, alpha=1.0, arrow_length_ratio=0.08)
    ax_main.text(-0.1, -0.1, axis_length+0.2, 'z', fontsize=20, 
                color=colors['z_axis'], fontweight='bold')
    
    # === 2. 绘制柱坐标系的关键要素 ===
    # 径向线r（从原点到点P在xy平面的投影）
    ax_main.quiver(0, 0, 0, px, py, 0, color=colors['r_axis'], 
                  linewidth=6, alpha=1.0, arrow_length_ratio=0.08)
    ax_main.text(px*0.6, py*0.6, -0.3, 'r', fontsize=18, 
                color=colors['r_axis'], fontweight='bold')
    
    # θ角弧线（从x轴到径向线r）
    theta_arc = np.linspace(0, ptheta, 60)
    r_arc = 0.8
    x_arc = r_arc * np.cos(theta_arc)
    y_arc = r_arc * np.sin(theta_arc)
    z_arc = np.zeros_like(theta_arc)
    ax_main.plot(x_arc, y_arc, z_arc, color=colors['theta'], linewidth=6, alpha=1.0)
    ax_main.text(0.5, 0.3, 0.15, 'θ', fontsize=18, 
                color=colors['theta'], fontweight='bold')
    
    # z方向线（从投影点到实际点P）
    ax_main.plot([px, px], [py, py], [0, pz], color=colors['z_axis'], 
                linewidth=4, alpha=0.8, linestyle='-')
    
    # === 3. 标出示例点P ===
    ax_main.scatter([px], [py], [pz], color=colors['point'], s=200, alpha=1.0, 
                   edgecolors='black', linewidth=3, zorder=10)
    ax_main.text(px+0.3, py+0.1, pz+0.3, f'P({pr:.0f}, 60°, {pz_cyl:.0f})', 
                fontsize=16, color=colors['point'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', 
                         edgecolor=colors['point'], alpha=0.9))
    
    # === 4. 绘制柱坐标网格系统（简化版，避免视觉混乱）===
    # r=常数的同心圆（仅在底面显示）
    for r_circle in [1.0, 2.0, 2.5]:
        theta_circle = np.linspace(0, 2*np.pi, 100)
        x_circle = r_circle * np.cos(theta_circle)
        y_circle = r_circle * np.sin(theta_circle)
        z_circle = np.zeros_like(theta_circle)
        ax_main.plot(x_circle, y_circle, z_circle, color=colors['grid'], 
                    linewidth=1.5, alpha=0.4, linestyle='--')
    
    # θ=常数的射线（仅在底面显示几条关键射线）
    for theta_ray in [0, np.pi/6, np.pi/3, np.pi/2, 2*np.pi/3, np.pi]:
        r_ray = np.linspace(0, 2.5, 20)
        x_ray = r_ray * np.cos(theta_ray)
        y_ray = r_ray * np.sin(theta_ray)
        z_ray = np.zeros_like(r_ray)
        ax_main.plot(x_ray, y_ray, z_ray, color=colors['grid'], 
                    linewidth=1, alpha=0.3)
    
    # 柱面轮廓（简化版，仅显示关键线条）
    # 在r=pr处绘制几条垂直线表示圆柱
    for angle in [0, np.pi/2, np.pi, 3*np.pi/2]:
        x_vert, y_vert = pr * np.cos(angle), pr * np.sin(angle)
        ax_main.plot([x_vert, x_vert], [y_vert, y_vert], [0, pz+0.5], 
                    color=colors['grid'], linewidth=1.5, alpha=0.4)
    
    # 原点标注
    ax_main.scatter([0], [0], [0], color='#000000', s=120, alpha=1.0, zorder=10)
    ax_main.text(0.2, 0.2, 0.2, 'O', fontsize=16, fontweight='bold', color='#000000')
    
    # 设置3D坐标轴范围和属性
    ax_main.set_xlim([-0.5, 3.2])
    ax_main.set_ylim([-0.5, 3.2])
    ax_main.set_zlim([-0.2, 3.2])
    ax_main.set_xticks([])
    ax_main.set_yticks([])
    ax_main.set_zticks([])
    ax_main.set_title('柱坐标系3D结构', fontsize=18, fontweight='bold',
                     color='#2F2F2F', pad=20)
    
    # 优化3D背景
    ax_main.xaxis.pane.fill = False
    ax_main.yaxis.pane.fill = False
    ax_main.zaxis.pane.fill = False
    ax_main.xaxis.pane.set_edgecolor('lightgray')
    ax_main.yaxis.pane.set_edgecolor('lightgray')
    ax_main.zaxis.pane.set_edgecolor('lightgray')
    ax_main.xaxis.pane.set_alpha(0.1)
    ax_main.yaxis.pane.set_alpha(0.1)
    ax_main.zaxis.pane.set_alpha(0.1)
    ax_main.grid(True, alpha=0.2)
    
    # === 右上：坐标变换公式详解 ===
    ax_formula = fig.add_subplot(gs[0, 1])
    ax_formula.axis('off')
    ax_formula.text(0.5, 0.98, '坐标变换公式', fontsize=18, fontweight='bold', 
                   color='#2F2F2F', ha='center', transform=ax_formula.transAxes)
    
    # 直角坐标 → 柱坐标
    ax_formula.text(0.05, 0.85, '直角坐标 → 柱坐标：', fontsize=15, fontweight='bold',
                   color=colors['r_axis'], transform=ax_formula.transAxes)
    ax_formula.text(0.05, 0.75, r'$x = r\cos\theta$', fontsize=14,
                   color='#5A6B7D', transform=ax_formula.transAxes)
    ax_formula.text(0.05, 0.65, r'$y = r\sin\theta$', fontsize=14,
                   color='#5A6B7D', transform=ax_formula.transAxes)
    ax_formula.text(0.05, 0.55, r'$z = z$', fontsize=14,
                   color='#5A6B7D', transform=ax_formula.transAxes)
    
    # 柱坐标 → 直角坐标
    ax_formula.text(0.05, 0.40, '柱坐标 → 直角坐标：', fontsize=15, fontweight='bold',
                   color=colors['theta'], transform=ax_formula.transAxes)
    ax_formula.text(0.05, 0.30, r'$r = \sqrt{x^2 + y^2}$', fontsize=14,
                   color='#5A6B7D', transform=ax_formula.transAxes)
    ax_formula.text(0.05, 0.20, r'$\theta = \arctan\left(\frac{y}{x}\right)$', fontsize=14,
                   color='#5A6B7D', transform=ax_formula.transAxes)
    ax_formula.text(0.05, 0.10, r'$z = z$', fontsize=14,
                   color='#5A6B7D', transform=ax_formula.transAxes)
    
    # === 右中：体积元素分析 ===
    ax_volume = fig.add_subplot(gs[0, 2])
    ax_volume.axis('off')
    ax_volume.text(0.5, 0.98, '体积元素分析', fontsize=18, fontweight='bold', 
                  color='#2F2F2F', ha='center', transform=ax_volume.transAxes)
    
    ax_volume.text(0.05, 0.85, '柱坐标体积元素：', fontsize=15, fontweight='bold',
                  color=colors['point'], transform=ax_volume.transAxes)
    ax_volume.text(0.05, 0.70, r'$dV = r \, dr \, d\theta \, dz$', fontsize=16,
                  color=colors['point'], transform=ax_volume.transAxes,
                  bbox=dict(boxstyle="round,pad=0.3", facecolor='#FFF8E7', 
                           edgecolor=colors['point'], alpha=0.9))
    
    ax_volume.text(0.05, 0.50, '参数范围：', fontsize=15, fontweight='bold',
                  color='#2F2F2F', transform=ax_volume.transAxes)
    ax_volume.text(0.05, 0.40, r'$r \geq 0$ （径向距离）', fontsize=13,
                  color='#5A6B7D', transform=ax_volume.transAxes)
    ax_volume.text(0.05, 0.30, r'$0 \leq \theta \leq 2\pi$ （方位角）', fontsize=13,
                  color='#5A6B7D', transform=ax_volume.transAxes)
    ax_volume.text(0.05, 0.20, r'$-\infty < z < +\infty$ （高度）', fontsize=13,
                  color='#5A6B7D', transform=ax_volume.transAxes)
    
    ax_volume.text(0.05, 0.05, 'Jacobian行列式：r', fontsize=14, fontweight='bold',
                  color=colors['r_axis'], transform=ax_volume.transAxes)
    
    # === 右右：几何意义说明 ===
    ax_geometry = fig.add_subplot(gs[0, 3])
    ax_geometry.axis('off')
    ax_geometry.text(0.5, 0.98, '几何意义', fontsize=18, fontweight='bold', 
                    color='#2F2F2F', ha='center', transform=ax_geometry.transAxes)
    
    ax_geometry.text(0.05, 0.85, '柱坐标系特点：', fontsize=15, fontweight='bold',
                    color='#2F2F2F', transform=ax_geometry.transAxes)
    ax_geometry.text(0.05, 0.75, '• r：点到z轴距离', fontsize=13,
                    color=colors['r_axis'], transform=ax_geometry.transAxes)
    ax_geometry.text(0.05, 0.65, '• θ：xy平面内角度', fontsize=13,
                    color=colors['theta'], transform=ax_geometry.transAxes)
    ax_geometry.text(0.05, 0.55, '• z：高度坐标', fontsize=13,
                    color=colors['z_axis'], transform=ax_geometry.transAxes)
    
    ax_geometry.text(0.05, 0.40, '适用场景：', fontsize=15, fontweight='bold',
                    color='#2F2F2F', transform=ax_geometry.transAxes)
    ax_geometry.text(0.05, 0.30, '• 圆柱体积分', fontsize=13,
                    color='#5A6B7D', transform=ax_geometry.transAxes)
    ax_geometry.text(0.05, 0.20, '• 旋转体积分', fontsize=13,
                    color='#5A6B7D', transform=ax_geometry.transAxes)
    ax_geometry.text(0.05, 0.10, '• 圆对称区域', fontsize=13,
                    color='#5A6B7D', transform=ax_geometry.transAxes)
    
    # === 底部：典型积分应用示例 ===
    ax_examples = fig.add_subplot(gs[1, 1:])
    ax_examples.axis('off')
    ax_examples.text(0.5, 0.90, '柱坐标系典型积分应用', fontsize=18, fontweight='bold', 
                    color='#2F2F2F', ha='center', transform=ax_examples.transAxes)
    
    # 圆柱体积分示例
    ax_examples.text(0.02, 0.70, '• 圆柱体积分：', fontsize=15, fontweight='bold',
                    color=colors['r_axis'], transform=ax_examples.transAxes)
    ax_examples.text(0.02, 0.55, r'$\iiint_{x^2+y^2 \leq a^2, \, 0 \leq z \leq h} f(x,y,z) \, dV = \int_0^{2\pi} \int_0^a \int_0^h f(r\cos\theta, r\sin\theta, z) \cdot r \, dz \, dr \, d\theta$',
                    fontsize=13, color='#5A6B7D', transform=ax_examples.transAxes)
    
    # 圆锥体积分示例
    ax_examples.text(0.02, 0.35, '• 圆锥体积分：', fontsize=15, fontweight='bold',
                    color=colors['theta'], transform=ax_examples.transAxes)
    ax_examples.text(0.02, 0.20, r'$\iiint_{x^2+y^2 \leq z^2, \, 0 \leq z \leq 1} f(x,y,z) \, dV = \int_0^{2\pi} \int_0^1 \int_0^z f(r\cos\theta, r\sin\theta, z) \cdot r \, dr \, dz \, d\theta$',
                    fontsize=13, color='#5A6B7D', transform=ax_examples.transAxes)
    
    plt.tight_layout()
    
    return fig

def create_spherical_coordinate_system():
    """创建高清晰度的球坐标系示意图 - 按照用户指示优化视角"""
    fig = plt.figure(figsize=(22, 16))
    
    # 精确的网格布局
    gs = fig.add_gridspec(1, 2, width_ratios=[1.4, 2.4], wspace=0.03, 
                         left=0.02, right=0.98, top=0.96, bottom=0.04)
    ax_text = fig.add_subplot(gs[0, 0])
    ax_main = fig.add_subplot(gs[0, 1], projection='3d')
    
    # 设置最佳视角 - 按照用户红色箭头指示的方向调整
    # 向右下方向旋转，让方位角、极角和P点都更清晰可见
    ax_main.view_init(elev=20, azim=25)  # 调整为更合适的角度
    
    # 定义参数 - 使用更适合展示的球坐标点位置，增大r值避免元素挤在一起
    r = 5.5
    phi = np.pi/3        # 60度极角 (便于展示)
    theta = np.pi/4      # 45度方位角 (便于展示)
    
    # 计算坐标
    x = r * np.sin(phi) * np.cos(theta)
    y = r * np.sin(phi) * np.sin(theta)
    z = r * np.cos(phi)
    
    # 坐标轴设置
    axis_length = 7.0
    axis_linewidth = 4.0
    
    # 绘制坐标轴
    ax_main.plot([0, axis_length], [0, 0], [0, 0], color='#B22222', linewidth=axis_linewidth, 
                alpha=1.0, solid_capstyle='round', zorder=8)
    ax_main.plot([0, 0], [0, axis_length], [0, 0], color='#006400', linewidth=axis_linewidth, 
                alpha=1.0, solid_capstyle='round', zorder=8)
    ax_main.plot([0, 0], [0, 0], [0, axis_length], color='#0000CD', linewidth=axis_linewidth, 
                alpha=1.0, solid_capstyle='round', zorder=8)
    
    # 坐标轴标签
    ax_main.text(axis_length + 0.5, -0.3, -0.3, 'x', fontsize=30, color='#B22222', 
                weight='bold', ha='center', va='center', zorder=10,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.9, edgecolor='#B22222'))
    ax_main.text(-0.3, axis_length + 0.5, -0.3, 'y', fontsize=30, color='#006400', 
                weight='bold', ha='center', va='center', zorder=10,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.9, edgecolor='#006400'))
    ax_main.text(-0.3, -0.3, axis_length + 0.5, 'z', fontsize=30, color='#0000CD', 
                weight='bold', ha='center', va='center', zorder=10,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.9, edgecolor='#0000CD'))
    
    # 示例点P
    ax_main.scatter([x], [y], [z], color='#FF4500', s=450, alpha=1.0, 
                   edgecolors='#8B0000', linewidth=6, zorder=15)
    ax_main.text(x + 0.5, y + 0.5, z + 0.5, 'P(r,φ,θ)', fontsize=16, weight='bold', 
                bbox=dict(boxstyle='round,pad=0.4', facecolor='#FFD700', alpha=0.9, 
                         edgecolor='#FF8C00', linewidth=2),
                ha='center', va='center', zorder=12)
    
    # 径向线 OP
    ax_main.plot([0, x], [0, y], [0, z], color='#000000', linewidth=8, 
                alpha=1.0, solid_capstyle='round', zorder=9)
    ax_main.text(x/2 + 0.2, y/2 + 0.2, z/2 + 0.3, 'r', fontsize=24, weight='bold', color='#000000',
                bbox=dict(boxstyle='round,pad=0.3', facecolor='#FFFFFF', alpha=0.9, 
                         edgecolor='#000000', linewidth=2),
                ha='center', va='center', zorder=11)
    
    # 投影点P'在xy平面上
    ax_main.scatter([x], [y], [0], color='#696969', s=300, alpha=1.0,
                   edgecolors='#000000', linewidth=4, zorder=10)
    ax_main.text(x + 0.1, y - 0.1, -0.4, "P'", fontsize=16, weight='bold', color='#2F4F4F',
                bbox=dict(boxstyle='round,pad=0.3', facecolor='#F5F5F5', alpha=0.9, 
                         edgecolor='#2F4F4F', linewidth=1),
                ha='center', va='center', zorder=11)
    
    # 投影线PP'
    ax_main.plot([x, x], [y, y], [0, z], color='#696969', linestyle='--', 
                linewidth=3, alpha=0.9, zorder=7)
    
    # OP'线 (P在xy平面的投影)
    ax_main.plot([0, x], [0, y], [0, 0], color='#32CD32', linewidth=6, 
                alpha=0.9, solid_capstyle='round', zorder=8)
    
    # 极角φ弧线 (从z轴到OP) - 调整位置避免遮挡
    phi_arc = np.linspace(0, phi, 120)
    arc_r_phi = 3.5
    x_phi = arc_r_phi * np.sin(phi_arc) * np.cos(theta)
    y_phi = arc_r_phi * np.sin(phi_arc) * np.sin(theta)
    z_phi = arc_r_phi * np.cos(phi_arc)
    ax_main.plot(x_phi, y_phi, z_phi, color='#0000CD', linewidth=8, 
                alpha=1.0, solid_capstyle='round', zorder=9)
    
    # 极角φ标注
    phi_mid = phi / 2
    phi_text_r = 4.0
    phi_x = phi_text_r * np.sin(phi_mid) * np.cos(theta)
    phi_y = phi_text_r * np.sin(phi_mid) * np.sin(theta)
    phi_z = phi_text_r * np.cos(phi_mid)
    ax_main.text(phi_x, phi_y, phi_z, 'φ', 
                fontsize=24, weight='bold', color='#0000CD',
                bbox=dict(boxstyle='round,pad=0.3', facecolor='#E6F3FF', alpha=0.9, 
                         edgecolor='#0000CD', linewidth=2),
                ha='center', va='center', zorder=11)
    
    # 方位角θ弧线 (在xy平面内，从x轴到OP')
    theta_arc = np.linspace(0, theta, 100)
    arc_r_theta = 3.0
    x_theta = arc_r_theta * np.cos(theta_arc)
    y_theta = arc_r_theta * np.sin(theta_arc)
    z_theta = np.zeros_like(theta_arc)
    ax_main.plot(x_theta, y_theta, z_theta, color='#DC143C', linewidth=6, 
                alpha=1.0, solid_capstyle='round', zorder=9)
    
    # 方位角θ标注
    theta_mid = theta / 2
    theta_text_r = 3.5
    ax_main.text(theta_text_r * np.cos(theta_mid), theta_text_r * np.sin(theta_mid), 0.2, 'θ', 
                fontsize=24, weight='bold', color='#DC143C',
                bbox=dict(boxstyle='round,pad=0.3', facecolor='#FFE4E1', alpha=0.9, 
                         edgecolor='#DC143C', linewidth=2),
                ha='center', va='center', zorder=11)
    
    # 添加辅助线标注角度含义 - 更清晰的线条
    # z轴延长线
    ax_main.plot([0, 0], [0, 0], [0, r+1], color='#0000CD', linewidth=3, 
                alpha=0.5, linestyle=':', zorder=6)
    
    # x轴延长线
    ax_main.plot([0, r*np.sin(phi)+1.5], [0, 0], [0, 0], color='#DC143C', linewidth=3, 
                alpha=0.5, linestyle=':', zorder=6)
    
    # 球面 - 极其透明，仅作为参考
    phi_sphere = np.linspace(0, np.pi, 30)
    theta_sphere = np.linspace(0, 2*np.pi, 60)
    Phi, Theta = np.meshgrid(phi_sphere, theta_sphere)
    R_sphere = r * np.ones_like(Phi)
    X_sphere = R_sphere * np.sin(Phi) * np.cos(Theta)
    Y_sphere = R_sphere * np.sin(Phi) * np.sin(Theta)
    Z_sphere = R_sphere * np.cos(Phi)
    ax_main.plot_surface(X_sphere, Y_sphere, Z_sphere, alpha=0.03, color='#D3D3D3', 
                        linewidth=0, antialiased=True, zorder=1)
    
    # 减少经纬线，避免视觉干扰
    for theta_line in [0, np.pi/2, np.pi]:
        phi_line = np.linspace(0, np.pi, 30)
        x_line = r * np.sin(phi_line) * np.cos(theta_line)
        y_line = r * np.sin(phi_line) * np.sin(theta_line)
        z_line = r * np.cos(phi_line)
        ax_main.plot(x_line, y_line, z_line, 'gray', alpha=0.15, linewidth=0.8, zorder=2)
    
    for phi_line in [np.pi/3, np.pi/2, 2*np.pi/3]:
        theta_line = np.linspace(0, 2*np.pi, 60)
        x_line = r * np.sin(phi_line) * np.cos(theta_line)
        y_line = r * np.sin(phi_line) * np.sin(theta_line)
        z_line = r * np.cos(phi_line) * np.ones_like(theta_line)
        ax_main.plot(x_line, y_line, z_line, 'gray', alpha=0.15, linewidth=0.8, zorder=2)
    
    # 原点
    ax_main.scatter([0], [0], [0], color='#000000', s=220, alpha=1.0, zorder=10)
    ax_main.text(0.3, 0.3, 0.3, 'O', fontsize=20, weight='bold', color='#000000', zorder=11)
    
    # 设置图形属性 - 扩大显示范围获得更好视角
    ax_main.set_xlim([-1.5, 9])
    ax_main.set_ylim([-1.5, 9])
    ax_main.set_zlim([0, 9])
    ax_main.set_xticks([])
    ax_main.set_yticks([])
    ax_main.set_zticks([])
    ax_main.set_title('球坐标系', fontsize=26, weight='bold', pad=40, color='#191970')
    
    # 优化背景
    ax_main.xaxis.pane.fill = False
    ax_main.yaxis.pane.fill = False
    ax_main.zaxis.pane.fill = False
    ax_main.xaxis.pane.set_edgecolor('lightgray')
    ax_main.yaxis.pane.set_edgecolor('lightgray')
    ax_main.zaxis.pane.set_edgecolor('lightgray')
    ax_main.xaxis.pane.set_alpha(0.03)
    ax_main.yaxis.pane.set_alpha(0.03)
    ax_main.zaxis.pane.set_alpha(0.03)
    ax_main.grid(True, alpha=0.1)
    
    # 左侧说明文字 - 修复字体问题
    ax_text.axis('off')
    
    # 分别设置每个文本，确保字体正确
    y_pos = 0.98
    line_height = 0.031
    
    # 标题
    ax_text.text(0.08, y_pos, '球坐标系特点与规则', fontsize=20, color='#191970', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 坐标变换公式
    ax_text.text(0.08, y_pos, '坐标变换公式：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'x = r sin φ cos θ', fontsize=16, color='#000000',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'y = r sin φ sin θ', fontsize=16, color='#000000',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'z = r cos φ', fontsize=16, color='#000000',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 参数范围
    ax_text.text(0.08, y_pos, '参数范围：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '0 ≤ r < +∞', fontsize=16, color='#006400',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '0 ≤ φ ≤ π', fontsize=16, color='#006400',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '0 ≤ θ ≤ 2π', fontsize=16, color='#006400',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 体积元素
    ax_text.text(0.08, y_pos, '体积元素：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'dV = r² sin φ dr dφ dθ', fontsize=18, color='#8B0000', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 适用条件
    ax_text.text(0.08, y_pos, '适用条件：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 区域具有球对称性', fontsize=14, color='#8B4513',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 被积函数含有 x²+y²+z² 项', fontsize=14, color='#8B4513',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 球体、球壳等几何体', fontsize=14, color='#8B4513',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 坐标含义
    ax_text.text(0.08, y_pos, '坐标含义：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• r: 到原点的距离', fontsize=14, color='#800080',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• φ: 极角(与z轴正向夹角)', fontsize=14, color='#800080',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• θ: 方位角(xy平面内角度)', fontsize=14, color='#800080',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 典型应用
    ax_text.text(0.08, y_pos, '典型应用：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 球体积分计算', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 引力场、电场问题', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 三维正态分布', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 量子力学原子轨道', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    

    
    plt.tight_layout()
    return fig

def main():
    """主函数"""
    # 设置中文字体
    setup_chinese_font()
    
    # 确保输出目录存在
    output_dir = "../../Assets"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    print("生成最终优化的高清中文坐标系图像...")
    
    # 生成柱坐标系图像
    print("正在生成优化柱坐标系图像...")
    fig_cyl = create_cylindrical_coordinate_system()
    cyl_path = os.path.join(output_dir, "cylindrical_coord.png")
    fig_cyl.savefig(cyl_path, dpi=400, bbox_inches='tight', 
                    facecolor='white', edgecolor='none', 
                    metadata={'Title': '柱坐标系'})
    plt.close(fig_cyl)
    print(f"优化柱坐标系图像已保存到: {cyl_path}")
    
    # 生成球坐标系图像
    print("正在生成优化球坐标系图像...")
    fig_sph = create_spherical_coordinate_system()
    sph_path = os.path.join(output_dir, "spherical_coord.png")
    fig_sph.savefig(sph_path, dpi=400, bbox_inches='tight', 
                    facecolor='white', edgecolor='none',
                    metadata={'Title': '球坐标系'})
    plt.close(fig_sph)
    print(f"优化球坐标系图像已保存到: {sph_path}")
    
    print("所有最终优化图像生成完成！")
    print("主要改进:")
    print("1. 去掉左侧说明文字的外部方框，界面更简洁")
    print("2. 球坐标系P点位置优化，避免元素拥挤：")
    print("   - P点距离：r = 4.2 → 5.5，元素分布更宽松")
    print("   - 扩大显示范围适应新的P点位置")
    print("3. 线条粗细优化，视觉更清爽：")
    print("   - 坐标轴：6px → 4px")
    print("   - 径向线：12px → 8px")
    print("   - 角度弧线：14px,10px → 8px,6px")
    print("   - 投影线：4.5px → 3px")
    print("4. 调整角度标注位置，配合新的P点位置")

if __name__ == "__main__":
    main() 