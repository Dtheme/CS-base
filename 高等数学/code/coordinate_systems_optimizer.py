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
    """创建高清晰度的柱坐标系示意图"""
    fig = plt.figure(figsize=(22, 16))
    
    # 更精确的网格布局
    gs = fig.add_gridspec(1, 2, width_ratios=[1.4, 2.4], wspace=0.03, 
                         left=0.02, right=0.98, top=0.96, bottom=0.04)
    ax_text = fig.add_subplot(gs[0, 0])
    ax_main = fig.add_subplot(gs[0, 1], projection='3d')
    
    # 设置最佳视角
    ax_main.view_init(elev=22, azim=42)
    
    # 定义参数 - 优化角度以更好展示
    r = 2.8
    theta = np.pi * 3/4  # 135度，更好展示角度
    z = 3.5
    
    # 计算坐标
    x = r * np.cos(theta)
    y = r * np.sin(theta)
    
    # 坐标轴设置 - 进一步加粗
    axis_length = 5.0
    axis_linewidth = 6.0
    
    # 绘制坐标轴 - 使用更鲜明的颜色
    ax_main.plot([0, axis_length], [0, 0], [0, 0], color='#B22222', linewidth=axis_linewidth, 
                alpha=1.0, solid_capstyle='round', zorder=8)
    ax_main.plot([0, 0], [0, axis_length], [0, 0], color='#006400', linewidth=axis_linewidth, 
                alpha=1.0, solid_capstyle='round', zorder=8)
    ax_main.plot([0, 0], [0, 0], [0, axis_length], color='#0000CD', linewidth=axis_linewidth, 
                alpha=1.0, solid_capstyle='round', zorder=8)
    
    # 坐标轴标签 - 增大字体和对比度
    ax_main.text(axis_length + 0.5, -0.3, -0.3, 'x', fontsize=30, color='#B22222', 
                weight='bold', ha='center', va='center', zorder=10,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.9, edgecolor='#B22222'))
    ax_main.text(-0.3, axis_length + 0.5, -0.3, 'y', fontsize=30, color='#006400', 
                weight='bold', ha='center', va='center', zorder=10,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.9, edgecolor='#006400'))
    ax_main.text(-0.3, -0.3, axis_length + 0.5, 'z', fontsize=30, color='#0000CD', 
                weight='bold', ha='center', va='center', zorder=10,
                bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.9, edgecolor='#0000CD'))
    
    # 绘制示例点P - 更大更突出
    ax_main.scatter([x], [y], [z], color='#FF4500', s=300, alpha=1.0, 
                   edgecolors='#8B0000', linewidth=5, zorder=15)
    ax_main.text(x + 0.5, y + 0.5, z + 0.5, 'P(r,θ,z)', fontsize=22, weight='bold', 
                bbox=dict(boxstyle='round,pad=0.6', facecolor='#FFD700', alpha=0.95, 
                         edgecolor='#FF8C00', linewidth=3),
                ha='center', va='center', zorder=12)
    
    # 径向线 - 加粗并增强对比
    ax_main.plot([0, x], [0, y], [0, 0], color='#000000', linewidth=8, 
                alpha=1.0, solid_capstyle='round', zorder=9)
    ax_main.text(x/2 - 0.3, y/2 + 0.5, -0.7, 'r', fontsize=32, weight='bold', color='#000000',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='#FFFFFF', alpha=0.95, 
                         edgecolor='#000000', linewidth=3),
                ha='center', va='center', zorder=11)
    
    # 高度线 - 加粗并增强颜色
    ax_main.plot([x, x], [y, y], [0, z], color='#0000CD', linewidth=8, 
                alpha=1.0, solid_capstyle='round', zorder=9)
    ax_main.text(x + 0.4, y + 0.4, z/2, 'z', fontsize=32, weight='bold', color='#0000CD',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='#E6F3FF', alpha=0.95, 
                         edgecolor='#0000CD', linewidth=3),
                ha='center', va='center', zorder=11)
    
    # 角度弧线 - 显著加粗和增强颜色
    theta_arc = np.linspace(0, theta, 100)  # 增加点数使弧线更平滑
    arc_r = 1.6
    x_arc = arc_r * np.cos(theta_arc)
    y_arc = arc_r * np.sin(theta_arc)
    z_arc = np.zeros_like(theta_arc)
    ax_main.plot(x_arc, y_arc, z_arc, color='#DC143C', linewidth=10, 
                alpha=1.0, solid_capstyle='round', zorder=9)
    
    # 角度标注 - 增大字体和对比度
    theta_mid = theta / 2
    theta_text_r = 2.1
    ax_main.text(theta_text_r * np.cos(theta_mid), theta_text_r * np.sin(theta_mid), 0.3, 'θ', 
                fontsize=36, weight='bold', color='#DC143C',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='#FFE4E1', alpha=0.98, 
                         edgecolor='#DC143C', linewidth=3),
                ha='center', va='center', zorder=11)
    
    # 投影点 - 增大并增强对比
    ax_main.scatter([x], [y], [0], color='#696969', s=200, alpha=1.0,
                   edgecolors='#000000', linewidth=4, zorder=10)
    ax_main.text(x + 0.15, y - 0.15, -0.5, "P'", fontsize=22, weight='bold', color='#2F4F4F',
                bbox=dict(boxstyle='round,pad=0.4', facecolor='#F5F5F5', alpha=0.95, 
                         edgecolor='#2F4F4F', linewidth=2),
                ha='center', va='center', zorder=11)
    
    # 投影虚线 - 加粗
    ax_main.plot([x, x], [y, y], [z, 0], color='#696969', linestyle='--', 
                linewidth=4.5, alpha=0.9, zorder=7)
    
    # 圆柱面 - 增强可见性
    theta_cylinder = np.linspace(0, 2*np.pi, 200)
    z_cylinder = np.linspace(0, 4.5, 30)
    Theta_cyl, Z_cyl = np.meshgrid(theta_cylinder, z_cylinder)
    R_cyl = r * np.ones_like(Theta_cyl)
    X_cyl = R_cyl * np.cos(Theta_cyl)
    Y_cyl = R_cyl * np.sin(Theta_cyl)
    ax_main.plot_surface(X_cyl, Y_cyl, Z_cyl, alpha=0.12, color='#00CED1', 
                        linewidth=0, antialiased=True, zorder=1)
    
    # 底面圆 - 加粗虚线
    theta_circle = np.linspace(0, 2*np.pi, 200)
    x_circle = r * np.cos(theta_circle)
    y_circle = r * np.sin(theta_circle)
    z_circle = np.zeros_like(theta_circle)
    ax_main.plot(x_circle, y_circle, z_circle, color='#00CED1', linestyle='--', 
                linewidth=4.5, alpha=0.9, zorder=6)
    
    # 原点 - 增大
    ax_main.scatter([0], [0], [0], color='#000000', s=150, alpha=1.0, zorder=10)
    ax_main.text(0.25, 0.25, 0.25, 'O', fontsize=22, weight='bold', color='#000000', zorder=11)
    
    # 设置图形属性
    ax_main.set_xlim([-0.8, 5.5])
    ax_main.set_ylim([-0.8, 5.5])
    ax_main.set_zlim([0, 5.5])
    ax_main.set_xticks([])
    ax_main.set_yticks([])
    ax_main.set_zticks([])
    ax_main.set_title('柱坐标系', fontsize=26, weight='bold', pad=40, color='#191970')
    
    # 优化背景
    ax_main.xaxis.pane.fill = False
    ax_main.yaxis.pane.fill = False
    ax_main.zaxis.pane.fill = False
    ax_main.xaxis.pane.set_edgecolor('lightgray')
    ax_main.yaxis.pane.set_edgecolor('lightgray')
    ax_main.zaxis.pane.set_edgecolor('lightgray')
    ax_main.xaxis.pane.set_alpha(0.05)
    ax_main.yaxis.pane.set_alpha(0.05)
    ax_main.zaxis.pane.set_alpha(0.05)
    ax_main.grid(True, alpha=0.2)
    
    # 左侧说明文字 - 修复字体问题
    ax_text.axis('off')
    
    # 分别设置每个文本，确保字体正确
    y_pos = 0.98
    line_height = 0.032
    
    # 标题
    ax_text.text(0.08, y_pos, '柱坐标系特点与规则', fontsize=20, color='#191970', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 坐标变换公式
    ax_text.text(0.08, y_pos, '坐标变换公式：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'x = r cos θ', fontsize=16, color='#000000',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'y = r sin θ', fontsize=16, color='#000000',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'z = z', fontsize=16, color='#000000',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 参数范围
    ax_text.text(0.08, y_pos, '参数范围：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '0 ≤ r < +∞', fontsize=16, color='#006400',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '0 ≤ θ ≤ 2π', fontsize=16, color='#006400',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '-∞ < z < +∞', fontsize=16, color='#006400',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 体积元素
    ax_text.text(0.08, y_pos, '体积元素：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, 'dV = r dr dθ dz', fontsize=18, color='#8B0000', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 适用条件
    ax_text.text(0.08, y_pos, '适用条件：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 区域关于z轴旋转对称', fontsize=14, color='#8B4513',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 被积函数含有 x²+y² 项', fontsize=14, color='#8B4513',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 圆柱、圆锥等几何体', fontsize=14, color='#8B4513',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 坐标含义
    ax_text.text(0.08, y_pos, '坐标含义：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• r: 到z轴的距离', fontsize=14, color='#800080',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• θ: 在xy平面内的角度', fontsize=14, color='#800080',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• z: 高度坐标(保持不变)', fontsize=14, color='#800080',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height * 2
    
    # 典型应用
    ax_text.text(0.08, y_pos, '典型应用：', fontsize=18, color='#000080', weight='bold',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 圆柱体积分计算', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 旋转体相关问题', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    y_pos -= line_height
    ax_text.text(0.08, y_pos, '• 场论中的柱对称问题', fontsize=14, color='#483D8B',
                transform=ax_text.transAxes, verticalalignment='top')
    

    
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