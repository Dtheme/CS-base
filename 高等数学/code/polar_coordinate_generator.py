#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
极坐标图像生成器
基于数学解题作图标准规范

功能：
1. 极坐标变换示意图
2. 例题4.1：圆形区域的极坐标变换
3. 例题4.2：指数函数的极坐标积分
4. 其他极坐标相关图像

版本：1.0
基于标准作图规范优化
"""

import matplotlib.pyplot as plt
import numpy as np
import os
from 数学作图标准模板 import MathFigureTemplate

class PolarCoordinateGenerator(MathFigureTemplate):
    """极坐标图像生成器类"""
    
    def __init__(self):
        super().__init__()
        self.setup_polar_layout()
    
    def setup_polar_layout(self):
        """设置极坐标专用布局参数"""
        # 极坐标图像的特殊布局
        self.polar_layout = {
            'figsize': (16, 12),
            'xlim': (-3.5, 4.5),
            'ylim': (-3.5, 4.5),
            'grid_density': 20,
            'angle_lines': 8,  # 角度线数量
            'circle_radii': [1, 2, 3],  # 圆周半径
        }
        
        # 极坐标专用配色
        self.polar_colors = {
            'grid_circle': '#E8E8E8',     # 极径圆
            'grid_angle': '#E8E8E8',      # 角度线
            'axis': '#333333',            # 主轴
            'region': '#FFE4E6',          # 积分区域
            'boundary': '#C8102E',        # 边界
            'transform_arrow': '#2E5BBA', # 变换箭头
        }
    
    def draw_polar_grid(self, ax, max_r=3):
        """绘制极坐标网格"""
        # 绘制极径圆
        for r in np.linspace(0.5, max_r, 6):
            circle = plt.Circle((0, 0), r, fill=False, 
                              color=self.polar_colors['grid_circle'], 
                              linewidth=1, alpha=0.6)
            ax.add_patch(circle)
        
        # 绘制角度线
        angles = np.linspace(0, 2*np.pi, self.polar_layout['angle_lines'], endpoint=False)
        for angle in angles:
            x_end = max_r * np.cos(angle)
            y_end = max_r * np.sin(angle)
            ax.plot([0, x_end], [0, y_end], 
                   color=self.polar_colors['grid_angle'], 
                   linewidth=1, alpha=0.6)
        
        # 标记主要角度
        main_angles = [0, np.pi/4, np.pi/2, 3*np.pi/4, np.pi, 5*np.pi/4, 3*np.pi/2, 7*np.pi/4]
        angle_labels = ['0', 'π/4', 'π/2', '3π/4', 'π', '5π/4', '3π/2', '7π/4']
        
        for angle, label in zip(main_angles, angle_labels):
            x = (max_r + 0.3) * np.cos(angle)
            y = (max_r + 0.3) * np.sin(angle)
            ax.text(x, y, label, fontsize=10, ha='center', va='center',
                   bbox=dict(boxstyle='round,pad=0.2', facecolor='white', alpha=0.8))
    
    def generate_polar_transform_diagram(self):
        """生成极坐标变换示意图"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 9))
        
        # 左图：直角坐标系
        ax1.set_xlim(-0.5, 4.5)
        ax1.set_ylim(-0.5, 4.5)
        ax1.set_title('直角坐标系 (x, y)', fontsize=16, weight='bold', pad=20)
        ax1.grid(True, alpha=0.3)
        ax1.set_xlabel('x', fontsize=14, weight='bold')
        ax1.set_ylabel('y', fontsize=14, weight='bold')
        
        # x-y-o坐标系
        ax1.axhline(y=0, color='black', linewidth=1.5)
        ax1.axvline(x=0, color='black', linewidth=1.5)
        ax1.plot(0, 0, 'ko', markersize=8)
        ax1.text(0.1, -0.3, 'O', fontsize=12, weight='bold')
        
        # 示例点和向量
        x_point, y_point = 3, 2.5
        ax1.plot(x_point, y_point, 'o', color=self.colors['curve1'], markersize=12)
        ax1.plot([0, x_point], [0, y_point], color=self.colors['curve1'], linewidth=3)
        
        # 坐标标注
        ax1.plot([x_point, x_point], [0, y_point], '--', color=self.colors['demo1'], linewidth=2)
        ax1.plot([0, x_point], [y_point, y_point], '--', color=self.colors['demo1'], linewidth=2)
        ax1.text(x_point + 0.1, y_point + 0.1, f'P({x_point:.1f}, {y_point:.1f})', 
                fontsize=12, weight='bold', color=self.colors['curve1'])
        ax1.text(x_point/2 - 0.3, -0.4, 'x', fontsize=12, weight='bold', color=self.colors['demo1'])
        ax1.text(-0.4, y_point/2, 'y', fontsize=12, weight='bold', color=self.colors['demo1'])
        
        # 右图：极坐标系
        ax2.set_xlim(-4.5, 4.5)
        ax2.set_ylim(-4.5, 4.5)
        ax2.set_title('极坐标系 (r, θ)', fontsize=16, weight='bold', pad=20)
        ax2.set_xlabel('x = r cos θ', fontsize=14, weight='bold')
        ax2.set_ylabel('y = r sin θ', fontsize=14, weight='bold')
        ax2.set_aspect('equal')
        
        # 极坐标网格
        self.draw_polar_grid(ax2, max_r=4)
        
        # 原点和主轴
        ax2.axhline(y=0, color='black', linewidth=2)
        ax2.axvline(x=0, color='black', linewidth=2)
        ax2.plot(0, 0, 'ko', markersize=8)
        ax2.text(0.2, -0.3, 'O', fontsize=12, weight='bold')
        
        # 同一点在极坐标中的表示
        r = np.sqrt(x_point**2 + y_point**2)
        theta = np.arctan2(y_point, x_point)
        
        ax2.plot(x_point, y_point, 'o', color=self.colors['curve1'], markersize=12)
        ax2.plot([0, x_point], [0, y_point], color=self.colors['curve1'], linewidth=3)
        
        # 角度弧线
        arc_angles = np.linspace(0, theta, 50)
        arc_r = 1.0
        arc_x = arc_r * np.cos(arc_angles)
        arc_y = arc_r * np.sin(arc_angles)
        ax2.plot(arc_x, arc_y, color=self.colors['curve2'], linewidth=3)
        
        # 标注
        ax2.text(x_point + 0.2, y_point + 0.2, f'P(r={r:.1f}, θ={theta:.2f})', 
                fontsize=12, weight='bold', color=self.colors['curve1'])
        ax2.text(1.3, 0.3, 'θ', fontsize=14, weight='bold', color=self.colors['curve2'])
        ax2.text(x_point/2 + 0.2, y_point/2 + 0.2, 'r', fontsize=14, weight='bold', color=self.colors['curve1'])
        
        # 变换公式
        transform_text = """变换公式:
x = r cos θ
y = r sin θ
dA = r dr dθ"""
        
        ax1.text(0.5, 4.0, transform_text, fontsize=13, weight='bold',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='#F8F9FA', 
                         edgecolor='#DEE2E6', alpha=0.95),
                verticalalignment='top')
        
        # 雅可比行列式
        jacobian_text = """雅可比行列式:
J = |∂(x,y)/∂(r,θ)| = r"""
        
        ax2.text(-4.0, 4.0, jacobian_text, fontsize=13, weight='bold',
                bbox=dict(boxstyle='round,pad=0.5', facecolor='#E8E8F5', 
                         edgecolor='#6A6A9A', alpha=0.95),
                verticalalignment='top')
        
        plt.tight_layout()
        output_path = self.save_figure(fig, 'polar_transform.png')
        return output_path
    
    def generate_example_4_1(self):
        """生成例题4.1：圆形区域的极坐标变换"""
        fig, ax = self.create_figure()
        
        # 调整坐标轴范围，减少上方空白
        ax.set_xlim(-3.0, 3.5)
        ax.set_ylim(-3.0, 3.5)
        ax.set_aspect('equal')
        
        # 重新设置文字位置，适应圆形区域
        self.text_positions = {
            'expression_title': (-2.8, 3.2),
            'expression_content': (-2.8, 2.8),
            'analysis': (-2.8, 2.2),
            'relation': (-2.8, 1.6),
        }
        
        # 绘制简化的极坐标网格（不要过于复杂）
        # 只绘制必要的极径圆
        for r in [1, 2]:
            circle = plt.Circle((0, 0), r, fill=False, 
                              color='#E0E0E0', linewidth=1, alpha=0.7)
            ax.add_patch(circle)
        
        # 绘制主要角度线
        main_angles = [0, np.pi/2, np.pi, 3*np.pi/2]
        for angle in main_angles:
            x_end = 2.5 * np.cos(angle)
            y_end = 2.5 * np.sin(angle)
            ax.plot([0, x_end], [0, y_end], 
                   color='#E0E0E0', linewidth=1, alpha=0.7)
        
        # 主要积分区域：圆盘 x² + y² ≤ 4
        theta = np.linspace(0, 2*np.pi, 100)
        radius = 2
        x_circle = radius * np.cos(theta)
        y_circle = radius * np.sin(theta)
        
        # 填充积分区域
        ax.fill(x_circle, y_circle, alpha=0.6, color=self.colors['region1'], 
               edgecolor=self.colors['curve1'], linewidth=3, label='积分区域: $x^2 + y^2 \\leq 4$')
        
        # 关键点标记 - 使用四角分布策略
        key_points = [
            {'point': (0, 0), 'label': 'O(0,0)', 'position_key': 'left_bottom'},
            {'point': (2, 0), 'label': '(2,0)', 'position_key': 'right_space'},
            {'point': (0, 2), 'label': '(0,2)', 'position_key': 'left_top'},
        ]
        
        # 调整关键点位置避免遮挡
        self.point_positions['left_bottom'] = (-2.7, -2.7)
        self.point_positions['right_space'] = (2.5, -0.3)
        self.point_positions['left_top'] = (-0.3, 2.5)
        
        self.add_key_points(ax, key_points)
        
        # 添加半径标注
        ax.plot([0, 2], [0, 0], 'k--', linewidth=2, alpha=0.8)
        ax.text(1, -0.4, 'r = 2', fontsize=12, weight='bold', ha='center',
               bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.9))
        
        # 设置标题
        self.set_title(ax, r'例题4.1：计算 $\iint_D (x^2 + y^2) dA$，其中 $D: x^2 + y^2 \leq 4$')
        
        # 图例位置调整
        legend = ax.legend(loc='upper right', bbox_to_anchor=(0.98, 0.98),
                          fontsize=12, framealpha=0.95, facecolor='white',
                          edgecolor='gray', frameon=True)
        legend.get_frame().set_linewidth(1.2)
        
        # 文字内容 - 调整为极坐标专用内容
        text_content = {
            'expression': {
                'title': r'极坐标变换:',
                'content': r'$\iint_D (x^2 + y^2) \, dA = \int_0^{2\pi} \int_0^2 r^3 \, dr \, d\theta$'
            },
            'analysis': """区域分析:
极坐标: $0 \\leq r \\leq 2, 0 \\leq \\theta \\leq 2\\pi$
变换: $x^2 + y^2 = r^2$""",
            'relation': r'结果: $\int_0^{2\pi} \int_0^2 r^3 \, dr \, d\theta = 8\pi$'
        }
        self.add_text_content(ax, text_content)
        
        output_path = self.save_figure(fig, 'polar_example_4_1.png')
        return output_path
    
    def generate_example_4_2(self):
        """生成例题4.2：指数函数的极坐标积分"""
        fig, ax = self.create_figure()
        
        # 调整坐标轴范围，减少空白
        ax.set_xlim(-2.0, 2.5)
        ax.set_ylim(-2.0, 2.5)
        ax.set_aspect('equal')
        
        # 重新设置文字位置，适应单位圆区域
        self.text_positions = {
            'expression_title': (-1.9, 2.2),
            'expression_content': (-1.9, 1.9),
            'analysis': (-1.9, 1.4),
            'relation': (-1.9, 1.0),
        }
        
        # 绘制简化的极坐标网格
        # 只绘制必要的极径圆
        for r in [0.5, 1]:
            circle = plt.Circle((0, 0), r, fill=False, 
                              color='#E0E0E0', linewidth=1, alpha=0.7)
            ax.add_patch(circle)
        
        # 绘制主要角度线
        main_angles = [0, np.pi/2, np.pi, 3*np.pi/2]
        for angle in main_angles:
            x_end = 1.5 * np.cos(angle)
            y_end = 1.5 * np.sin(angle)
            ax.plot([0, x_end], [0, y_end], 
                   color='#E0E0E0', linewidth=1, alpha=0.7)
        
        # 主要积分区域：单位圆盘 x² + y² ≤ 1
        theta = np.linspace(0, 2*np.pi, 100)
        radius = 1
        x_circle = radius * np.cos(theta)
        y_circle = radius * np.sin(theta)
        
        # 填充积分区域
        ax.fill(x_circle, y_circle, alpha=0.6, color=self.colors['region2'], 
               edgecolor=self.colors['curve2'], linewidth=3, label='积分区域: $x^2 + y^2 \\leq 1$')
        
        # 添加简化的等高线显示e^{-(x²+y²)}的变化
        x_contour = np.linspace(-1.2, 1.2, 50)
        y_contour = np.linspace(-1.2, 1.2, 50)
        X, Y = np.meshgrid(x_contour, y_contour)
        Z = np.exp(-(X**2 + Y**2))
        
        # 只在单位圆内显示少量等高线
        mask = X**2 + Y**2 <= 1
        Z_masked = np.where(mask, Z, np.nan)
        
        contours = ax.contour(X, Y, Z_masked, levels=3, colors=['#666666'], alpha=0.5, linewidths=1)
        
        # 关键点标记 - 使用四角分布策略
        key_points = [
            {'point': (0, 0), 'label': 'O(0,0)', 'position_key': 'left_bottom'},
            {'point': (1, 0), 'label': '(1,0)', 'position_key': 'right_space'},
            {'point': (0, 1), 'label': '(0,1)', 'position_key': 'left_top'},
        ]
        
        # 调整关键点位置避免遮挡
        self.point_positions['left_bottom'] = (-1.8, -1.8)
        self.point_positions['right_space'] = (1.3, -0.2)
        self.point_positions['left_top'] = (-0.2, 1.3)
        
        self.add_key_points(ax, key_points)
        
        # 添加半径标注
        ax.plot([0, 1], [0, 0], 'k--', linewidth=2, alpha=0.8)
        ax.text(0.5, -0.25, 'r = 1', fontsize=12, weight='bold', ha='center',
               bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.9))
        
        # 设置标题
        self.set_title(ax, r'例题4.2：计算 $\iint_D e^{-(x^2+y^2)} dA$，其中 $D: x^2 + y^2 \leq 1$')
        
        # 图例位置调整
        legend = ax.legend(loc='upper right', bbox_to_anchor=(0.98, 0.98),
                          fontsize=12, framealpha=0.95, facecolor='white',
                          edgecolor='gray', frameon=True)
        legend.get_frame().set_linewidth(1.2)
        
        # 文字内容 - 调整为极坐标专用内容
        text_content = {
            'expression': {
                'title': r'极坐标变换:',
                'content': r'$\iint_D e^{-(x^2+y^2)} \, dA = \int_0^{2\pi} \int_0^1 e^{-r^2} \cdot r \, dr \, d\theta$'
            },
            'analysis': """区域分析:
极坐标: $0 \\leq r \\leq 1, 0 \\leq \\theta \\leq 2\\pi$
变换: $e^{-(x^2+y^2)} = e^{-r^2}$""",
            'relation': r'结果: $\pi(1 - \frac{1}{e}) \approx 1.986$'
        }
        self.add_text_content(ax, text_content)
        
        output_path = self.save_figure(fig, 'polar_example_4_2.png')
        return output_path
    
    def generate_example_7_2(self):
        """生成例题7.2：立体体积的极坐标计算"""
        fig, ax = self.create_figure()
        
        # 调整坐标轴范围
        ax.set_xlim(-2.5, 2.5)
        ax.set_ylim(-2.5, 2.5)
        ax.set_aspect('equal')
        
        # 重新设置文字位置
        self.text_positions = {
            'expression_title': (-2.3, 2.2),
            'expression_content': (-2.3, 1.9),
            'analysis': (-2.3, 1.4),
            'relation': (-2.3, 1.0),
        }
        
        # 绘制简化的极坐标网格
        for r in [1, np.sqrt(2)]:
            circle = plt.Circle((0, 0), r, fill=False, 
                              color='#E0E0E0', linewidth=1, alpha=0.7)
            ax.add_patch(circle)
        
        # 绘制主要角度线
        main_angles = [0, np.pi/2, np.pi, 3*np.pi/2]
        for angle in main_angles:
            x_end = 2.2 * np.cos(angle)
            y_end = 2.2 * np.sin(angle)
            ax.plot([0, x_end], [0, y_end], 
                   color='#E0E0E0', linewidth=1, alpha=0.7)
        
        # 主要积分区域：投影区域 x² + y² ≤ 2
        theta = np.linspace(0, 2*np.pi, 100)
        radius = np.sqrt(2)
        x_circle = radius * np.cos(theta)
        y_circle = radius * np.sin(theta)
        
        # 填充投影区域
        ax.fill(x_circle, y_circle, alpha=0.6, color=self.colors['region1'], 
               edgecolor=self.colors['curve1'], linewidth=3, label='投影区域: $x^2 + y^2 \\leq 2$')
        
        # 关键点标记
        key_points = [
            {'point': (0, 0), 'label': 'O(0,0)', 'position_key': 'left_bottom'},
            {'point': (np.sqrt(2), 0), 'label': f'({np.sqrt(2):.2f},0)', 'position_key': 'right_space'},
            {'point': (0, np.sqrt(2)), 'label': f'(0,{np.sqrt(2):.2f})', 'position_key': 'left_top'},
        ]
        
        # 调整关键点位置
        self.point_positions['left_bottom'] = (-2.2, -2.2)
        self.point_positions['right_space'] = (1.8, -0.2)
        self.point_positions['left_top'] = (-0.2, 1.8)
        
        self.add_key_points(ax, key_points)
        
        # 添加半径标注
        ax.plot([0, np.sqrt(2)], [0, 0], 'k--', linewidth=2, alpha=0.8)
        ax.text(np.sqrt(2)/2, -0.3, f'r = √2 ≈ {np.sqrt(2):.2f}', fontsize=12, weight='bold', ha='center',
               bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.9))
        
        # 设置标题
        self.set_title(ax, r'例题7.2：计算由 $z = x^2 + y^2$ 与 $z = 2$ 围成的立体体积')
        
        # 图例
        legend = ax.legend(loc='upper right', bbox_to_anchor=(0.98, 0.98),
                          fontsize=12, framealpha=0.95, facecolor='white',
                          edgecolor='gray', frameon=True)
        legend.get_frame().set_linewidth(1.2)
        
        # 文字内容
        text_content = {
            'expression': {
                'title': r'极坐标积分:',
                'content': r'$V = \int_0^{2\pi} \int_0^{\sqrt{2}} (2 - r^2) \cdot r \, dr \, d\theta$'
            },
            'analysis': """区域分析:
投影区域: $x^2 + y^2 \\leq 2$
极坐标: $0 \\leq r \\leq \\sqrt{2}, 0 \\leq \\theta \\leq 2\\pi$""",
            'relation': r'结果: $V = 2\pi$'
        }
        self.add_text_content(ax, text_content)
        
        output_path = self.save_figure(fig, 'polar_example_7_2.png')
        return output_path
    
    def generate_example_7_4(self):
        """生成例题7.4：单位圆盘的转动惯量计算"""
        fig, ax = self.create_figure()
        
        # 调整坐标轴范围
        ax.set_xlim(-1.8, 1.8)
        ax.set_ylim(-1.8, 1.8)
        ax.set_aspect('equal')
        
        # 重新设置文字位置
        self.text_positions = {
            'expression_title': (-1.7, 1.6),
            'expression_content': (-1.7, 1.3),
            'analysis': (-1.7, 0.9),
            'relation': (-1.7, 0.5),
        }
        
        # 绘制简化的极坐标网格
        for r in [0.5, 1]:
            circle = plt.Circle((0, 0), r, fill=False, 
                              color='#E0E0E0', linewidth=1, alpha=0.7)
            ax.add_patch(circle)
        
        # 绘制主要角度线
        main_angles = [0, np.pi/2, np.pi, 3*np.pi/2]
        for angle in main_angles:
            x_end = 1.4 * np.cos(angle)
            y_end = 1.4 * np.sin(angle)
            ax.plot([0, x_end], [0, y_end], 
                   color='#E0E0E0', linewidth=1, alpha=0.7)
        
        # 主要积分区域：单位圆盘
        theta = np.linspace(0, 2*np.pi, 100)
        radius = 1
        x_circle = radius * np.cos(theta)
        y_circle = radius * np.sin(theta)
        
        # 填充积分区域
        ax.fill(x_circle, y_circle, alpha=0.6, color=self.colors['region2'], 
               edgecolor=self.colors['curve2'], linewidth=3, label='单位圆盘: $x^2 + y^2 \\leq 1$')
        
        # 添加密度可视化（同心圆表示r²）
        for r in [0.3, 0.6, 0.9]:
            circle = plt.Circle((0, 0), r, fill=False, 
                              color=self.colors['curve2'], linewidth=1.5, alpha=0.8, linestyle='--')
            ax.add_patch(circle)
        
        # 关键点标记
        key_points = [
            {'point': (0, 0), 'label': 'O(0,0)', 'position_key': 'left_bottom'},
            {'point': (1, 0), 'label': '(1,0)', 'position_key': 'right_space'},
            {'point': (0, 1), 'label': '(0,1)', 'position_key': 'left_top'},
        ]
        
        # 调整关键点位置
        self.point_positions['left_bottom'] = (-1.6, -1.6)
        self.point_positions['right_space'] = (1.2, -0.15)
        self.point_positions['left_top'] = (-0.15, 1.2)
        
        self.add_key_points(ax, key_points)
        
        # 添加半径标注
        ax.plot([0, 1], [0, 0], 'k--', linewidth=2, alpha=0.8)
        ax.text(0.5, -0.2, 'r = 1', fontsize=12, weight='bold', ha='center',
               bbox=dict(boxstyle='round,pad=0.3', facecolor='white', alpha=0.9))
        
        # 设置标题
        self.set_title(ax, r'例题7.4：计算单位圆盘对原点的转动惯量（密度 $\rho = 1$）')
        
        # 图例
        legend = ax.legend(loc='upper right', bbox_to_anchor=(0.98, 0.98),
                          fontsize=12, framealpha=0.95, facecolor='white',
                          edgecolor='gray', frameon=True)
        legend.get_frame().set_linewidth(1.2)
        
        # 文字内容
        text_content = {
            'expression': {
                'title': r'转动惯量积分:',
                'content': r'$I_0 = \int_0^{2\pi} \int_0^1 r^2 \cdot r \, dr \, d\theta$'
            },
            'analysis': """物理意义:
转动惯量: $I_0 = \\iint_D (x^2 + y^2) \\rho \\, dA$
极坐标: $x^2 + y^2 = r^2, \\rho = 1$""",
            'relation': r'结果: $I_0 = \frac{\pi}{2}$'
        }
        self.add_text_content(ax, text_content)
        
        output_path = self.save_figure(fig, 'polar_example_7_4.png')
        return output_path
    
    def generate_polar_region_types(self):
        """生成极坐标常见区域类型图"""
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        fig.suptitle('极坐标系中的常见积分区域类型', fontsize=18, weight='bold', y=0.95)
        
        # 类型1：圆盘
        ax1 = axes[0, 0]
        ax1.set_xlim(-2.5, 2.5)
        ax1.set_ylim(-2.5, 2.5)
        ax1.set_aspect('equal')
        ax1.set_title('圆盘：0 ≤ r ≤ R', fontsize=14, weight='bold')
        
        theta = np.linspace(0, 2*np.pi, 100)
        R = 2
        x = R * np.cos(theta)
        y = R * np.sin(theta)
        ax1.fill(x, y, alpha=0.6, color=self.colors['region1'], 
                edgecolor=self.colors['curve1'], linewidth=2)
        ax1.grid(True, alpha=0.3)
        ax1.axhline(y=0, color='black', linewidth=1)
        ax1.axvline(x=0, color='black', linewidth=1)
        
        # 类型2：扇形
        ax2 = axes[0, 1]
        ax2.set_xlim(-2.5, 2.5)
        ax2.set_ylim(-2.5, 2.5)
        ax2.set_aspect('equal')
        ax2.set_title('扇形：0 ≤ r ≤ R, α ≤ θ ≤ β', fontsize=14, weight='bold')
        
        alpha, beta = np.pi/6, np.pi/3
        theta_sector = np.linspace(alpha, beta, 50)
        r_max = 2
        
        # 扇形区域
        theta_fill = np.concatenate([[alpha], theta_sector, [beta], [alpha]])
        r_fill = np.concatenate([[0], np.full_like(theta_sector, r_max), [0], [0]])
        x_fill = r_fill * np.cos(theta_fill)
        y_fill = r_fill * np.sin(theta_fill)
        ax2.fill(x_fill, y_fill, alpha=0.6, color=self.colors['region2'],
                edgecolor=self.colors['curve2'], linewidth=2)
        ax2.grid(True, alpha=0.3)
        ax2.axhline(y=0, color='black', linewidth=1)
        ax2.axvline(x=0, color='black', linewidth=1)
        
        # 类型3：环形
        ax3 = axes[1, 0]
        ax3.set_xlim(-2.5, 2.5)
        ax3.set_ylim(-2.5, 2.5)
        ax3.set_aspect('equal')
        ax3.set_title('环形：R₁ ≤ r ≤ R₂', fontsize=14, weight='bold')
        
        R1, R2 = 1, 2
        theta_ring = np.linspace(0, 2*np.pi, 100)
        x_outer = R2 * np.cos(theta_ring)
        y_outer = R2 * np.sin(theta_ring)
        x_inner = R1 * np.cos(theta_ring)
        y_inner = R1 * np.sin(theta_ring)
        
        ax3.fill(x_outer, y_outer, alpha=0.6, color=self.colors['region1'])
        ax3.fill(x_inner, y_inner, alpha=1.0, color='white')
        ax3.plot(x_outer, y_outer, color=self.colors['curve1'], linewidth=2)
        ax3.plot(x_inner, y_inner, color=self.colors['curve1'], linewidth=2)
        ax3.grid(True, alpha=0.3)
        ax3.axhline(y=0, color='black', linewidth=1)
        ax3.axvline(x=0, color='black', linewidth=1)
        
        # 类型4：花瓣形（玫瑰线）
        ax4 = axes[1, 1]
        ax4.set_xlim(-2.5, 2.5)
        ax4.set_ylim(-2.5, 2.5)
        ax4.set_aspect('equal')
        ax4.set_title('特殊区域：r = f(θ)', fontsize=14, weight='bold')
        
        theta_rose = np.linspace(0, 2*np.pi, 1000)
        r_rose = 2 * np.cos(2 * theta_rose)
        r_rose = np.maximum(r_rose, 0)  # 只取正值
        x_rose = r_rose * np.cos(theta_rose)
        y_rose = r_rose * np.sin(theta_rose)
        
        ax4.fill(x_rose, y_rose, alpha=0.6, color=self.colors['region2'],
                edgecolor=self.colors['curve2'], linewidth=2)
        ax4.grid(True, alpha=0.3)
        ax4.axhline(y=0, color='black', linewidth=1)
        ax4.axvline(x=0, color='black', linewidth=1)
        ax4.text(0, -2.2, 'r = 2cos(2θ)', fontsize=12, weight='bold', ha='center')
        
        plt.tight_layout()
        output_path = self.save_figure(fig, 'polar_region_types.png')
        return output_path
    
    def generate_all_polar_figures(self):
        """生成所有极坐标相关图像"""
        print("🎨 开始生成极坐标系列图像...")
        print("=" * 50)
        
        # 生成各种图像
        paths = []
        
        print("1. 生成极坐标变换示意图...")
        paths.append(self.generate_polar_transform_diagram())
        
        print("2. 生成例题4.1图像...")
        paths.append(self.generate_example_4_1())
        
        print("3. 生成例题4.2图像...")
        paths.append(self.generate_example_4_2())
        
        print("4. 生成例题7.2图像...")
        paths.append(self.generate_example_7_2())
        
        print("5. 生成例题7.4图像...")
        paths.append(self.generate_example_7_4())
        
        print("6. 生成极坐标区域类型图...")
        paths.append(self.generate_polar_region_types())
        
        print(f"\n✅ 成功生成 {len(paths)} 个极坐标图像")
        for i, path in enumerate(paths, 1):
            print(f"{i}. {path}")
        
        # 质量检查
        print("\n" + "="*50)
        self.quality_check()
        
        return paths


def main():
    """主函数"""
    generator = PolarCoordinateGenerator()
    paths = generator.generate_all_polar_figures()
    
    print(f"\n🎯 极坐标图像生成完成！")
    print(f"📁 输出目录：../../Assets/")
    print(f"📊 生成文件数：{len(paths)}")


if __name__ == "__main__":
    main() 