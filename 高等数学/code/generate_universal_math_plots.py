#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# created by Dzw 2025-06-25
#
# 绘制高数相关插图 
# 
 
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Polygon, Circle, Ellipse
import matplotlib.patches as patches
from matplotlib import rcParams
from mpl_toolkits.mplot3d import Axes3D
import sys
import platform
import os
from typing import Dict, Tuple, List, Optional, Union

class UniversalMathPlotter:
    """通用数学绘图器"""
    
    def __init__(self, language='en', style='academic'):
        """
        Initialize plotter - English only to avoid encoding issues
        
        Args:
            language: 'en' English (default and only supported)
            style: 'academic' academic style, 'modern' modern style
        """
        self.language = 'en'  # Force English only
        self.style = style
        self.colors = self._create_color_scheme()
        self.texts = self._load_text_library()
        self._setup_matplotlib()
    
    def _setup_matplotlib(self):
        """Configure matplotlib style - English only"""
        # Font configuration - English only
        rcParams['font.family'] = 'serif'
        rcParams['font.serif'] = ['Times New Roman', 'DejaVu Serif', 'serif']
        rcParams['axes.unicode_minus'] = False
        
        rcParams['font.size'] = 13
        rcParams['text.usetex'] = False
        
        # 图形质量配置
        rcParams['figure.facecolor'] = 'white'
        rcParams['axes.facecolor'] = 'white'
        rcParams['savefig.facecolor'] = 'white'
        rcParams['savefig.edgecolor'] = 'none'
        rcParams['savefig.dpi'] = 300
        rcParams['figure.dpi'] = 100
        
        # 线条和标记配置
        rcParams['lines.linewidth'] = 2.5
        rcParams['lines.markersize'] = 8
        rcParams['axes.linewidth'] = 1.2
        
        print(f"System: {platform.system()}")
        print(f"Language: {self.language}, Style: {self.style}")
        print("Universal mathematical plotter initialized")
    
    def _create_color_scheme(self) -> Dict[str, str]:
        """创建配色方案"""
        if self.style == 'academic':
            return {
                'primary': '#2E86C1',      # 主蓝色
                'secondary': '#E74C3C',     # 辅助红色
                'accent': '#27AE60',      # 强调绿色
                'light_blue': '#AED6F1',        # 浅蓝色填充
                'light_green': '#A9DFBF',       # 浅绿色填充
                'light_coral': '#F1948A',       # 浅珊瑚色填充
                'dark_gray': '#2C3E50',         # 深灰色文字
                'light_gray': '#BDC3C7',        # 浅灰色网格
                'warning_yellow': '#F39C12',    # 警告黄色
                'background': '#FEFEFE',        # 背景色
                'purple': '#8E44AD',            # 紫色
                'orange': '#E67E22'             # 橙色
            }
        else:  # modern style
            return {
                'primary': '#3498DB',
                'secondary': '#E74C3C',
                'accent': '#2ECC71',
                'light_blue': '#EBF5FB',
                'light_green': '#EAFAF1',
                'light_coral': '#FADBD8',
                'dark_gray': '#34495E',
                'light_gray': '#D5DBDB',
                'warning_yellow': '#F1C40F',
                'background': '#FFFFFF',
                'purple': '#9B59B6',
                'orange': '#FF7F50'
            }
    
    def _load_text_library(self) -> Dict[str, Dict[str, str]]:
        """加载文本库"""
        return {
            'common': {
                'zh': {
                    'x_axis': 'x',
                    'y_axis': 'y', 
                    'z_axis': 'z',
                    'volume': '体积',
                    'area': '面积',
                    'region': '区域',
                    'boundary': '边界',
                    'integration': '积分',
                    'coordinate_system': '坐标系'
                },
                'en': {
                    'x_axis': 'x',
                    'y_axis': 'y',
                    'z_axis': 'z', 
                    'volume': 'Volume',
                    'area': 'Area',
                    'region': 'Region',
                    'boundary': 'Boundary',
                    'integration': 'Integration',
                    'coordinate_system': 'Coordinate System'
                }
            },
            'double_integral': {
                'zh': {
                    'x_type_region': 'X型区域',
                    'y_type_region': 'Y型区域',
                    'upper_boundary': '上边界',
                    'lower_boundary': '下边界',
                    'left_boundary': '左边界',
                    'right_boundary': '右边界',
                    'integration_line': '积分线',
                    'polar_transform': '极坐标变换'
                },
                'en': {
                    'x_type_region': 'X-Type Region',
                    'y_type_region': 'Y-Type Region', 
                    'upper_boundary': 'Upper Boundary',
                    'lower_boundary': 'Lower Boundary',
                    'left_boundary': 'Left Boundary',
                    'right_boundary': 'Right Boundary',
                    'integration_line': 'Integration Line',
                    'polar_transform': 'Polar Coordinate Transform'
                }
            },
            'triple_integral': {
                'zh': {
                    'rectangular_coord': '直角坐标系',
                    'cylindrical_coord': '柱坐标系',
                    'spherical_coord': '球坐标系',
                    'projection_method': '投影法',
                    'cross_section_method': '截面法',
                    'coordinate_transform': '坐标变换',
                    'volume_element': '体积元素'
                },
                'en': {
                    'rectangular_coord': 'Rectangular Coordinates',
                    'cylindrical_coord': 'Cylindrical Coordinates', 
                    'spherical_coord': 'Spherical Coordinates',
                    'projection_method': 'Projection Method',
                    'cross_section_method': 'Cross Section Method',
                    'coordinate_transform': 'Coordinate Transform',
                    'volume_element': 'Volume Element'
                }
            }
        }
    
    def get_text(self, category: str, key: str) -> str:
        """获取文本"""
        return self.texts.get(category, {}).get(self.language, {}).get(key, key)
    
    def save_plot(self, filename: str, directory: str = '../../Assets'):
        """保存图片"""
        if not os.path.exists(directory):
            os.makedirs(directory)
        filepath = os.path.join(directory, filename)
        plt.tight_layout()
        plt.savefig(filepath, dpi=300, bbox_inches='tight', 
                   facecolor='white', edgecolor='none')
        plt.close()
        print(f"Plot saved: {filepath}")
    
    # === 二重积分相关图形 ===
    
    def plot_x_type_region(self, save_name: str = 'x_type_region.png'):
        """绘制X型区域图"""
        fig, ax = plt.subplots(figsize=(14, 10))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 定义区域边界
        x = np.linspace(0, 4, 200)
        y_upper = 2.2 + 0.4*np.sin(1.8*x)
        y_lower = 0.6 + 0.25*np.sin(1.2*x)
        
        # 填充X型区域
        ax.fill_between(x, y_lower, y_upper, alpha=0.25, color=self.colors['light_blue'], 
                       label=self.get_text('double_integral', 'x_type_region'), 
                       edgecolor=self.colors['primary'], linewidth=0.5)
        
        # 绘制边界曲线
        ax.plot(x, y_upper, color=self.colors['primary'], linewidth=3.5, 
               label=r'$y = \varphi_2(x)$ (' + self.get_text('double_integral', 'upper_boundary') + ')')
        ax.plot(x, y_lower, color=self.colors['secondary'], linewidth=3.5, 
               label=r'$y = \varphi_1(x)$ (' + self.get_text('double_integral', 'lower_boundary') + ')')
        
        # 添加垂直线示例
        x_sample = 2.5
        y_sample_lower = 0.6 + 0.25*np.sin(1.2*x_sample)
        y_sample_upper = 2.2 + 0.4*np.sin(1.8*x_sample)
        
        ax.axvline(x=x_sample, color=self.colors['accent'], linestyle='--', 
                  alpha=0.9, linewidth=3)
        ax.plot([x_sample, x_sample], [y_sample_lower, y_sample_upper], 
               'o-', color=self.colors['accent'], linewidth=5, markersize=10, 
               markerfacecolor=self.colors['warning_yellow'], markeredgecolor=self.colors['accent'])
        
        # 标注
        annotation_x = x_sample + 0.4
        annotation_y = (y_sample_lower + y_sample_upper) / 2
        ax.annotate(f'{self.get_text("double_integral", "integration_line")}\nx = {x_sample}\n' + 
                   r'$\varphi_1(x) \leq y \leq \varphi_2(x)$',
                   xy=(x_sample, annotation_y), 
                   xytext=(annotation_x, annotation_y + 0.3),
                   fontsize=12, ha='left', va='center',
                   bbox=dict(boxstyle="round,pad=0.4", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', connectionstyle='arc3,rad=0.1', 
                                  color=self.colors['accent'], lw=2))
        
        # 设置坐标轴
        ax.set_xlim(-0.3, 4.8)
        ax.set_ylim(0, 3.2)
        ax.set_xlabel(self.get_text('common', 'x_axis'), fontsize=18, fontweight='bold', color=self.colors['dark_gray'])
        ax.set_ylabel(self.get_text('common', 'y_axis'), fontsize=18, fontweight='bold', color=self.colors['dark_gray'])
        
        title_text = self.get_text('double_integral', 'x_type_region')
        if self.language == 'en':
            title_text += ': Vertical Lines Intersect Boundary at Two Points'
        else:
            title_text += '：垂直线与边界相交于两点'
            
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=25, color=self.colors['dark_gray'])
        
        # 添加积分公式
        formula_text = r'$\iint_D f(x,y) \, dA = \int_a^b dx \int_{\varphi_1(x)}^{\varphi_2(x)} f(x,y) \, dy$'
        ax.text(0.4, 2.9, formula_text, fontsize=15, 
               bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                        alpha=0.95, edgecolor=self.colors['primary'], linewidth=2))
        
        # 图例和网格
        legend = ax.legend(fontsize=13, loc='upper right', frameon=True, 
                          fancybox=True, shadow=True, framealpha=0.95)
        legend.get_frame().set_facecolor('white')
        legend.get_frame().set_edgecolor(self.colors['light_gray'])
        
        ax.grid(True, alpha=0.3, color=self.colors['light_gray'], linewidth=1)
        ax.tick_params(axis='both', which='major', labelsize=13, colors=self.colors['dark_gray'])
        
        self.save_plot(save_name)
    
    def plot_y_type_region(self, save_name: str = 'y_type_region.png'):
        """绘制Y型区域图"""
        fig, ax = plt.subplots(figsize=(14, 10))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 定义区域边界
        y = np.linspace(0.2, 3, 200)
        x_left = 0.6 + 0.3*np.sin(1.5*y)
        x_right = 3.2 + 0.3*np.cos(1.2*y)
        
        # 填充Y型区域
        ax.fill_betweenx(y, x_left, x_right, alpha=0.25, color=self.colors['light_green'], 
                        label=self.get_text('double_integral', 'y_type_region'), 
                        edgecolor=self.colors['accent'], linewidth=0.5)
        
        # 绘制边界曲线
        ax.plot(x_left, y, color=self.colors['secondary'], linewidth=3.5, 
               label=r'$x = \psi_1(y)$ (' + self.get_text('double_integral', 'left_boundary') + ')')
        ax.plot(x_right, y, color=self.colors['primary'], linewidth=3.5, 
               label=r'$x = \psi_2(y)$ (' + self.get_text('double_integral', 'right_boundary') + ')')
        
        # 添加水平线示例
        y_sample = 1.8
        x_sample_left = 0.6 + 0.3*np.sin(1.5*y_sample)
        x_sample_right = 3.2 + 0.3*np.cos(1.2*y_sample)
        
        ax.axhline(y=y_sample, color=self.colors['accent'], linestyle='--', 
                  alpha=0.9, linewidth=3)
        ax.plot([x_sample_left, x_sample_right], [y_sample, y_sample], 
               'o-', color=self.colors['accent'], linewidth=5, markersize=10,
               markerfacecolor=self.colors['warning_yellow'], markeredgecolor=self.colors['accent'])
        
        # 标注
        annotation_x = (x_sample_left + x_sample_right) / 2
        annotation_y = y_sample + 0.5
        ax.annotate(f'{self.get_text("double_integral", "integration_line")}\ny = {y_sample}\n' + 
                   r'$\psi_1(y) \leq x \leq \psi_2(y)$',
                   xy=(annotation_x, y_sample), 
                   xytext=(annotation_x, annotation_y),
                   fontsize=12, ha='center', va='bottom',
                   bbox=dict(boxstyle="round,pad=0.4", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', connectionstyle='arc3,rad=0', 
                                  color=self.colors['accent'], lw=2))
        
        # 设置坐标轴
        ax.set_xlim(0, 4.2)
        ax.set_ylim(0, 3.5)
        ax.set_xlabel(self.get_text('common', 'x_axis'), fontsize=18, fontweight='bold', color=self.colors['dark_gray'])
        ax.set_ylabel(self.get_text('common', 'y_axis'), fontsize=18, fontweight='bold', color=self.colors['dark_gray'])
        
        title_text = self.get_text('double_integral', 'y_type_region')
        if self.language == 'en':
            title_text += ': Horizontal Lines Intersect Boundary at Two Points'
        else:
            title_text += '：水平线与边界相交于两点'
            
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=25, color=self.colors['dark_gray'])
        
        # 添加积分公式
        formula_text = r'$\iint_D f(x,y) \, dA = \int_c^d dy \int_{\psi_1(y)}^{\psi_2(y)} f(x,y) \, dx$'
        ax.text(0.2, 3.15, formula_text, fontsize=15, 
               bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                        alpha=0.95, edgecolor=self.colors['primary'], linewidth=2))
        
        # 图例和网格
        legend = ax.legend(fontsize=13, loc='lower right', frameon=True, 
                          fancybox=True, shadow=True, framealpha=0.95)
        legend.get_frame().set_facecolor('white')
        legend.get_frame().set_edgecolor(self.colors['light_gray'])
        
        ax.grid(True, alpha=0.3, color=self.colors['light_gray'], linewidth=1)
        ax.tick_params(axis='both', which='major', labelsize=13, colors=self.colors['dark_gray'])
        
        self.save_plot(save_name)
    
    # === 三重积分相关图形 ===
    
    def plot_cylindrical_coordinates(self, save_name: str = 'cylindrical_coord.png'):
        """绘制柱坐标系示意图"""
        fig = plt.figure(figsize=(14, 10))
        ax = fig.add_subplot(111, projection='3d')
        
        # 绘制坐标轴
        ax.plot([0, 3], [0, 0], [0, 0], 'k-', linewidth=3, alpha=0.8)
        ax.plot([0, 0], [0, 3], [0, 0], 'k-', linewidth=3, alpha=0.8)
        ax.plot([0, 0], [0, 0], [0, 4], 'k-', linewidth=3, alpha=0.8)
        
        # 标注坐标轴
        ax.text(3.2, 0, 0, self.get_text('common', 'x_axis'), fontsize=16, fontweight='bold')
        ax.text(0, 3.2, 0, self.get_text('common', 'y_axis'), fontsize=16, fontweight='bold')
        ax.text(0, 0, 4.2, self.get_text('common', 'z_axis'), fontsize=16, fontweight='bold')
        
        # 绘制点P
        r = 2
        theta = np.pi/4
        z = 2.5
        x = r * np.cos(theta)
        y = r * np.sin(theta)
        
        ax.scatter([x], [y], [z], color=self.colors['secondary'], s=100, label='P(r,θ,z)')
        
        # 绘制从原点到点P的线
        ax.plot([0, x], [0, y], [0, 0], '--', color=self.colors['primary'], linewidth=2, alpha=0.7)
        ax.plot([0, x], [0, y], [z, z], '--', color=self.colors['primary'], linewidth=2, alpha=0.7)
        ax.plot([x, x], [y, y], [0, z], '--', color=self.colors['accent'], linewidth=2, alpha=0.7)
        
        # 绘制圆柱面网格
        theta_grid = np.linspace(0, 2*np.pi, 20)
        z_grid = np.linspace(0, 3, 10)
        Theta, Z = np.meshgrid(theta_grid, z_grid)
        X = r * np.cos(Theta)
        Y = r * np.sin(Theta)
        ax.plot_surface(X, Y, Z, alpha=0.1, color=self.colors['light_blue'])
        
        # 绘制底面圆
        theta_circle = np.linspace(0, 2*np.pi, 100)
        x_circle = r * np.cos(theta_circle)
        y_circle = r * np.sin(theta_circle)
        z_circle = np.zeros_like(theta_circle)
        ax.plot(x_circle, y_circle, z_circle, color=self.colors['accent'], linewidth=2)
        
        # 标注
        ax.text(x/2, y/2, 0.2, 'r', fontsize=16, fontweight='bold', color=self.colors['primary'])
        ax.text(0.3, 0.3, 0.2, 'θ', fontsize=16, fontweight='bold', color=self.colors['primary'])
        ax.text(x+0.2, y+0.2, z/2, 'z', fontsize=16, fontweight='bold', color=self.colors['accent'])
        
        # 设置标题
        title_text = self.get_text('triple_integral', 'cylindrical_coord')
        if self.language == 'en':
            title_text += ' System\nx = r cos θ, y = r sin θ, z = z'
        else:
            title_text += '\nx = r cos θ, y = r sin θ, z = z'
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=20)
        
        # 设置视角和范围
        ax.set_xlim(0, 3)
        ax.set_ylim(0, 3)
        ax.set_zlim(0, 4)
        ax.view_init(elev=20, azim=45)
        
        self.save_plot(save_name)
    
    def plot_spherical_coordinates(self, save_name: str = 'spherical_coord.png'):
        """绘制球坐标系示意图"""
        fig = plt.figure(figsize=(14, 10))
        ax = fig.add_subplot(111, projection='3d')
        
        # 绘制坐标轴
        ax.plot([0, 3], [0, 0], [0, 0], 'k-', linewidth=3, alpha=0.8)
        ax.plot([0, 0], [0, 3], [0, 0], 'k-', linewidth=3, alpha=0.8)
        ax.plot([0, 0], [0, 0], [0, 3], 'k-', linewidth=3, alpha=0.8)
        
        # 标注坐标轴
        ax.text(3.2, 0, 0, self.get_text('common', 'x_axis'), fontsize=16, fontweight='bold')
        ax.text(0, 3.2, 0, self.get_text('common', 'y_axis'), fontsize=16, fontweight='bold')
        ax.text(0, 0, 3.2, self.get_text('common', 'z_axis'), fontsize=16, fontweight='bold')
        
        # 绘制点P
        r = 2.5
        phi = np.pi/3
        theta = np.pi/4
        x = r * np.sin(phi) * np.cos(theta)
        y = r * np.sin(phi) * np.sin(theta)
        z = r * np.cos(phi)
        
        ax.scatter([x], [y], [z], color=self.colors['secondary'], s=100, label='P(r,φ,θ)')
        
        # 绘制从原点到点P的线
        ax.plot([0, x], [0, y], [0, z], color=self.colors['primary'], linewidth=3, alpha=0.8)
        
        # 绘制球面网格
        phi_grid = np.linspace(0, np.pi, 15)
        theta_grid = np.linspace(0, 2*np.pi, 20)
        Phi, Theta = np.meshgrid(phi_grid, theta_grid)
        X = r * np.sin(Phi) * np.cos(Theta)
        Y = r * np.sin(Phi) * np.sin(Theta)
        Z = r * np.cos(Phi)
        ax.plot_surface(X, Y, Z, alpha=0.1, color=self.colors['light_blue'])
        
        # 绘制经线和纬线
        # 经线
        phi_line = np.linspace(0, np.pi, 50)
        x_meridian = r * np.sin(phi_line) * np.cos(theta)
        y_meridian = r * np.sin(phi_line) * np.sin(theta)
        z_meridian = r * np.cos(phi_line)
        ax.plot(x_meridian, y_meridian, z_meridian, color=self.colors['accent'], linewidth=2)
        
        # 纬线
        theta_line = np.linspace(0, 2*np.pi, 50)
        x_parallel = r * np.sin(phi) * np.cos(theta_line)
        y_parallel = r * np.sin(phi) * np.sin(theta_line)
        z_parallel = r * np.cos(phi) * np.ones_like(theta_line)
        ax.plot(x_parallel, y_parallel, z_parallel, color=self.colors['purple'], linewidth=2)
        
        # 投影线
        ax.plot([0, x], [0, y], [0, 0], '--', color=self.colors['orange'], linewidth=2, alpha=0.7)
        ax.plot([x, x], [y, y], [0, z], '--', color=self.colors['accent'], linewidth=2, alpha=0.7)
        
        # 标注
        ax.text(x/2, y/2, z/2, 'r', fontsize=16, fontweight='bold', color=self.colors['primary'])
        ax.text(0.3, 0.3, 0.7, 'φ', fontsize=16, fontweight='bold', color=self.colors['purple'])
        ax.text(0.5, 0.2, 0.1, 'θ', fontsize=16, fontweight='bold', color=self.colors['orange'])
        
        # 设置标题
        title_text = self.get_text('triple_integral', 'spherical_coord')
        if self.language == 'en':
            title_text += ' System\nx = r sin φ cos θ, y = r sin φ sin θ, z = r cos φ'
        else:
            title_text += '\nx = r sin φ cos θ, y = r sin φ sin θ, z = r cos φ'
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=20)
        
        # 设置视角和范围
        ax.set_xlim(-1, 3)
        ax.set_ylim(-1, 3)
        ax.set_zlim(0, 3)
        ax.view_init(elev=20, azim=45)
        
        self.save_plot(save_name)
    
    def plot_triple_integral_region(self, save_name: str = 'triple_region.png'):
        """绘制三重积分区域示意图"""
        fig = plt.figure(figsize=(14, 10))
        ax = fig.add_subplot(111, projection='3d')
        
        # 绘制坐标轴
        ax.plot([0, 4], [0, 0], [0, 0], 'k-', linewidth=2, alpha=0.8)
        ax.plot([0, 0], [0, 4], [0, 0], 'k-', linewidth=2, alpha=0.8)
        ax.plot([0, 0], [0, 0], [0, 3], 'k-', linewidth=2, alpha=0.8)
        
        # 标注坐标轴
        ax.text(4.2, 0, 0, self.get_text('common', 'x_axis'), fontsize=16, fontweight='bold')
        ax.text(0, 4.2, 0, self.get_text('common', 'y_axis'), fontsize=16, fontweight='bold')
        ax.text(0, 0, 3.2, self.get_text('common', 'z_axis'), fontsize=16, fontweight='bold')
        
        # 绘制立体区域（例如：抛物面与平面围成的区域）
        u = np.linspace(0, 2, 30)
        v = np.linspace(0, 2*np.pi, 30)
        U, V = np.meshgrid(u, v)
        
        # 抛物面 z = x² + y²
        X1 = U * np.cos(V)
        Y1 = U * np.sin(V)
        Z1 = X1**2 + Y1**2
        
        # 平面 z = 4
        X2 = U * np.cos(V)
        Y2 = U * np.sin(V)
        Z2 = 4 * np.ones_like(X2)
        
        # 绘制曲面
        ax.plot_surface(X1, Y1, Z1, alpha=0.6, color=self.colors['light_blue'], 
                       edgecolor=self.colors['primary'], linewidth=0.5)
        ax.plot_surface(X2, Y2, Z2, alpha=0.4, color=self.colors['light_coral'], 
                       edgecolor=self.colors['secondary'], linewidth=0.5)
        
        # 绘制边界曲线
        theta = np.linspace(0, 2*np.pi, 100)
        r = 2
        x_boundary = r * np.cos(theta)
        y_boundary = r * np.sin(theta)
        z_boundary = 4 * np.ones_like(theta)
        ax.plot(x_boundary, y_boundary, z_boundary, color=self.colors['secondary'], linewidth=3)
        
        # 绘制投影
        x_proj = r * np.cos(theta)
        y_proj = r * np.sin(theta)
        z_proj = np.zeros_like(theta)
        ax.plot(x_proj, y_proj, z_proj, '--', color=self.colors['accent'], linewidth=2, alpha=0.7)
        
        # 添加样本点和积分线
        x_sample, y_sample = 1.0, 1.0
        z_lower = x_sample**2 + y_sample**2
        z_upper = 4
        ax.plot([x_sample, x_sample], [y_sample, y_sample], [z_lower, z_upper], 
               'o-', color=self.colors['warning_yellow'], linewidth=4, markersize=8)
        
        # 标注
        ax.text(x_sample+0.2, y_sample+0.2, (z_lower+z_upper)/2, 
               f'{self.get_text("double_integral", "integration_line")}', 
               fontsize=12, fontweight='bold', color=self.colors['warning_yellow'])
        
        # 设置标题
        title_text = self.get_text('triple_integral', 'projection_method')
        if self.language == 'en':
            title_text += ': Volume between Paraboloid and Plane'
        else:
            title_text += '：抛物面与平面间的体积'
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=20)
        
        # 设置视角和范围
        ax.set_xlim(-2.5, 2.5)
        ax.set_ylim(-2.5, 2.5)
        ax.set_zlim(0, 5)
        ax.view_init(elev=25, azim=45)
        
        self.save_plot(save_name)
    
    def plot_coordinate_comparison(self, save_name: str = 'coord_comparison.png'):
        """绘制坐标系比较图"""
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 直角坐标系 (2D)
        x = np.linspace(-2, 2, 100)
        y = np.linspace(-2, 2, 100)
        X, Y = np.meshgrid(x, y)
        
        # 绘制网格
        for i in range(-2, 3):
            ax1.axhline(y=i, color=self.colors['light_gray'], alpha=0.5)
            ax1.axvline(x=i, color=self.colors['light_gray'], alpha=0.5)
        
        # 绘制点
        points_x = [1, -1, 0, 1.5]
        points_y = [1, 1, -1.5, -0.5]
        ax1.scatter(points_x, points_y, color=self.colors['secondary'], s=50)
        
        ax1.set_xlim(-2.5, 2.5)
        ax1.set_ylim(-2.5, 2.5)
        ax1.set_xlabel(self.get_text('common', 'x_axis'), fontsize=16)
        ax1.set_ylabel(self.get_text('common', 'y_axis'), fontsize=16)
        ax1.set_title(self.get_text('triple_integral', 'rectangular_coord'), fontsize=16, fontweight='bold')
        ax1.grid(True, alpha=0.3)
        ax1.set_aspect('equal')
        
        # 极坐标系 (2D)
        theta = np.linspace(0, 2*np.pi, 100)
        for r in [0.5, 1, 1.5, 2]:
            x_circle = r * np.cos(theta)
            y_circle = r * np.sin(theta)
            ax2.plot(x_circle, y_circle, color=self.colors['light_gray'], alpha=0.5)
        
        for angle in np.linspace(0, 2*np.pi, 8, endpoint=False):
            x_line = [0, 2*np.cos(angle)]
            y_line = [0, 2*np.sin(angle)]
            ax2.plot(x_line, y_line, color=self.colors['light_gray'], alpha=0.5)
        
        # 同样的点在极坐标下
        for px, py in zip(points_x, points_y):
            ax2.scatter(px, py, color=self.colors['secondary'], s=50)
        
        ax2.set_xlim(-2.5, 2.5)
        ax2.set_ylim(-2.5, 2.5)
        ax2.set_xlabel(self.get_text('common', 'x_axis'), fontsize=16)
        ax2.set_ylabel(self.get_text('common', 'y_axis'), fontsize=16)
        ax2.set_title(self.get_text('double_integral', 'polar_transform'), fontsize=16, fontweight='bold')
        ax2.set_aspect('equal')
        
        # 柱坐标系表示 (3D投影到2D)
        # 显示不同高度的圆
        for z_level in [0, 0.5, 1, 1.5]:
            alpha = 1 - z_level/2
            for r in [0.5, 1, 1.5]:
                x_circle = r * np.cos(theta)
                y_circle = r * np.sin(theta)
                ax3.plot(x_circle, y_circle, color=self.colors['primary'], alpha=alpha, linewidth=2)
        
        ax3.set_xlim(-2, 2)
        ax3.set_ylim(-2, 2)
        ax3.set_xlabel('r cos θ', fontsize=16)
        ax3.set_ylabel('r sin θ', fontsize=16)
        ax3.set_title(self.get_text('triple_integral', 'cylindrical_coord'), fontsize=16, fontweight='bold')
        ax3.set_aspect('equal')
        
        # 球坐标系表示 (3D投影到2D)
        # 绘制不同半径的球在xy平面的投影
        for r in [0.5, 1, 1.5, 2]:
            # 球在不同φ角度的投影圆
            for phi in [np.pi/4, np.pi/3, np.pi/2]:
                r_proj = r * np.sin(phi)
                x_circle = r_proj * np.cos(theta)
                y_circle = r_proj * np.sin(theta)
                alpha = np.sin(phi)
                ax4.plot(x_circle, y_circle, color=self.colors['purple'], alpha=alpha, linewidth=2)
        
        ax4.set_xlim(-2, 2)
        ax4.set_ylim(-2, 2)
        ax4.set_xlabel('r sin φ cos θ', fontsize=16)
        ax4.set_ylabel('r sin φ sin θ', fontsize=16)
        ax4.set_title(self.get_text('triple_integral', 'spherical_coord'), fontsize=16, fontweight='bold')
        ax4.set_aspect('equal')
        
        plt.tight_layout()
        self.save_plot(save_name)
    
    def plot_example_region_parabola(self, save_name: str = 'region_parabola.png'):
        """绘制例题区域：y=x²和y=x围成的区域"""
        fig, ax = plt.subplots(figsize=(14, 10))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 定义函数
        x = np.linspace(-0.5, 1.5, 200)
        y1 = x**2  # y = x²
        y2 = x     # y = x
        
        # 找交点
        x_intersect = [0, 1]
        y_intersect = [0, 1]
        
        # 绘制曲线
        ax.plot(x, y1, color=self.colors['primary'], linewidth=3.5, label=r'$y = x^2$')
        ax.plot(x, y2, color=self.colors['secondary'], linewidth=3.5, label=r'$y = x$')
        
        # 填充积分区域
        x_fill = np.linspace(0, 1, 100)
        y1_fill = x_fill**2
        y2_fill = x_fill
        ax.fill_between(x_fill, y1_fill, y2_fill, alpha=0.25, color=self.colors['light_green'], 
                       label=self.get_text('common', 'region') + r' $(\sigma)$',
                       edgecolor=self.colors['accent'], linewidth=1)
        
        # 标注交点
        for i, (xi, yi) in enumerate(zip(x_intersect, y_intersect)):
            ax.plot(xi, yi, 'o', color=self.colors['warning_yellow'], markersize=12, 
                   markeredgecolor=self.colors['dark_gray'], markeredgewidth=2)
            ax.annotate(f'({xi}, {yi})', xy=(xi, yi), xytext=(xi+0.1, yi+0.1),
                       fontsize=12, fontweight='bold', color=self.colors['dark_gray'])
        
        # 添加积分线示例
        x_sample = 0.5
        y_sample_lower = x_sample**2
        y_sample_upper = x_sample
        
        ax.plot([x_sample, x_sample], [y_sample_lower, y_sample_upper], 
               'o-', color=self.colors['accent'], linewidth=4, markersize=8,
               markerfacecolor=self.colors['warning_yellow'])
        
        # 标注积分线
        ax.annotate(f'x = {x_sample}\n' + r'$x^2 \leq y \leq x$',
                   xy=(x_sample, (y_sample_lower + y_sample_upper)/2),
                   xytext=(x_sample + 0.3, (y_sample_lower + y_sample_upper)/2),
                   fontsize=12, ha='left', va='center',
                   bbox=dict(boxstyle="round,pad=0.4", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', color=self.colors['accent'], lw=2))
        
        # 设置坐标轴
        ax.set_xlim(-0.3, 1.4)
        ax.set_ylim(-0.2, 1.3)
        ax.set_xlabel(self.get_text('common', 'x_axis'), fontsize=18, fontweight='bold')
        ax.set_ylabel(self.get_text('common', 'y_axis'), fontsize=18, fontweight='bold')
        
        title_text = f"{self.get_text('common', 'region')} $\\sigma$: $y = x^2$ and $y = x$"
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=25)
        
        # 添加积分公式
        formula_text = r'$\iint_\sigma \frac{y^2}{x^2} d\sigma = \int_0^1 dx \int_{x^2}^x \frac{y^2}{x^2} dy$'
        ax.text(0.05, 1.15, formula_text, fontsize=16, 
               bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                        alpha=0.95, edgecolor=self.colors['primary'], linewidth=2))
        
        # 图例和网格
        legend = ax.legend(fontsize=13, loc='upper left', frameon=True, 
                          fancybox=True, shadow=True, framealpha=0.95)
        legend.get_frame().set_facecolor('white')
        
        ax.grid(True, alpha=0.3, color=self.colors['light_gray'], linewidth=1)
        ax.tick_params(axis='both', which='major', labelsize=13)
        
        self.save_plot(save_name)

    def plot_example_region_triangle(self, save_name: str = 'region_triangle.png'):
        """绘制例题区域：y=x, y=1, x=0围成的三角形区域"""
        fig, ax = plt.subplots(figsize=(14, 10))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 定义区域边界
        x_line = np.linspace(0, 1, 100)
        y_line = x_line  # y = x
        
        # 绘制边界线
        ax.plot(x_line, y_line, color=self.colors['primary'], linewidth=3.5, label=r'$y = x$')
        ax.axhline(y=1, color=self.colors['secondary'], linewidth=3.5, label=r'$y = 1$', xmin=0, xmax=1)
        ax.axvline(x=0, color=self.colors['accent'], linewidth=3.5, label=r'$x = 0$', ymin=0, ymax=1)
        
        # 填充三角形区域
        triangle_x = [0, 1, 0, 0]
        triangle_y = [0, 1, 1, 0]
        ax.fill(triangle_x, triangle_y, alpha=0.25, color=self.colors['light_coral'], 
               label=self.get_text('common', 'region') + r' $(\sigma)$',
               edgecolor=self.colors['orange'], linewidth=2)
        
        # 标注顶点
        vertices = [(0, 0), (1, 1), (0, 1)]
        vertex_labels = ['(0, 0)', '(1, 1)', '(0, 1)']
        
        for (xi, yi), label in zip(vertices, vertex_labels):
            ax.plot(xi, yi, 'o', color=self.colors['warning_yellow'], markersize=12, 
                   markeredgecolor=self.colors['dark_gray'], markeredgewidth=2)
            offset_x = 0.1 if xi != 0 else -0.1
            offset_y = 0.1 if yi != 1 else -0.1
            ax.annotate(label, xy=(xi, yi), xytext=(xi+offset_x, yi+offset_y),
                       fontsize=12, fontweight='bold', color=self.colors['dark_gray'])
        
        # 添加积分线示例 (Y型区域)
        y_sample = 0.6
        x_sample_range = [0, y_sample]  # 从x=0到x=y
        
        ax.plot(x_sample_range, [y_sample, y_sample], 
               'o-', color=self.colors['purple'], linewidth=4, markersize=8,
               markerfacecolor=self.colors['warning_yellow'])
        
        # 标注积分线
        ax.annotate(f'y = {y_sample}\n' + r'$0 \leq x \leq y$',
                   xy=(y_sample/2, y_sample),
                   xytext=(y_sample/2 + 0.2, y_sample + 0.15),
                   fontsize=12, ha='center', va='bottom',
                   bbox=dict(boxstyle="round,pad=0.4", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', color=self.colors['purple'], lw=2))
        
        # 设置坐标轴
        ax.set_xlim(-0.2, 1.3)
        ax.set_ylim(-0.2, 1.3)
        ax.set_xlabel(self.get_text('common', 'x_axis'), fontsize=18, fontweight='bold')
        ax.set_ylabel(self.get_text('common', 'y_axis'), fontsize=18, fontweight='bold')
        
        title_text = f"{self.get_text('common', 'region')} $\\sigma$: $y = x$, $y = 1$, $x = 0$"
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=25)
        
        # 添加积分公式
        formula_text = r'$\iint_\sigma e^{y^2} d\sigma = \int_0^1 dy \int_0^y e^{y^2} dx$'
        ax.text(0.05, 1.15, formula_text, fontsize=16, 
               bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                        alpha=0.95, edgecolor=self.colors['primary'], linewidth=2))
        
        # 图例和网格
        legend = ax.legend(fontsize=13, loc='upper right', frameon=True, 
                          fancybox=True, shadow=True, framealpha=0.95)
        legend.get_frame().set_facecolor('white')
        
        ax.grid(True, alpha=0.3, color=self.colors['light_gray'], linewidth=1)
        ax.tick_params(axis='both', which='major', labelsize=13)
        
        self.save_plot(save_name)

    def plot_double_integral_3d_geometric(self, save_name: str = 'double_integral_3d.png'):
        """绘制二重积分的3D几何意义图"""
        fig = plt.figure(figsize=(16, 12))
        ax = fig.add_subplot(111, projection='3d')
        
        # 创建网格
        x = np.linspace(0, 2, 30)
        y = np.linspace(0, 2, 30)
        X, Y = np.meshgrid(x, y)
        
        # 定义函数 z = f(x,y) = x + y + 1
        Z = X + Y + 1
        
        # 绘制曲面
        surf = ax.plot_surface(X, Y, Z, alpha=0.7, cmap='viridis', 
                              edgecolor=self.colors['light_gray'], linewidth=0.3)
        
        # 绘制积分区域的投影
        x_region = np.array([0, 2, 2, 0, 0])
        y_region = np.array([0, 0, 2, 2, 0])
        z_region = np.zeros_like(x_region)
        
        ax.plot(x_region, y_region, z_region, color=self.colors['secondary'], linewidth=4)
        
        # Create 3D polygon for the base region
        from mpl_toolkits.mplot3d.art3d import Poly3DCollection
        verts = [list(zip(x_region[:-1], y_region[:-1], z_region[:-1]))]
        ax.add_collection3d(Poly3DCollection(verts, alpha=0.3, facecolor=self.colors['light_coral']))
        
        # 绘制几个垂直线表示积分概念
        for xi in [0.5, 1.0, 1.5]:
            for yi in [0.5, 1.0, 1.5]:
                zi = xi + yi + 1
                ax.plot([xi, xi], [yi, yi], [0, zi], '--', 
                       color=self.colors['accent'], alpha=0.6, linewidth=2)
        
        # 设置标签和标题
        ax.set_xlabel('x', fontsize=16, fontweight='bold')
        ax.set_ylabel('y', fontsize=16, fontweight='bold')
        ax.set_zlabel('z = f(x,y)', fontsize=16, fontweight='bold')
        ax.set_title('Double Integral Geometric Meaning\n' + 
                    r'Volume = $\iint_D f(x,y) \, dA$', 
                    fontsize=16, fontweight='bold', pad=20)
        
        # 设置视角
        ax.view_init(elev=25, azim=45)
        
        # 添加颜色条
        fig.colorbar(surf, ax=ax, shrink=0.6, aspect=20)
        
        self.save_plot(save_name)

    def plot_integration_order_comparison(self, save_name: str = 'integration_order.png'):
        """绘制积分次序比较图 - 基于例题3.1的y=x和y=x²区域"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 8))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 左图：dydx次序 (X型区域)
        x = np.linspace(0, 1, 100)
        y_upper = x  # y = x
        y_lower = x**2  # y = x²
        
        ax1.fill_between(x, y_lower, y_upper, alpha=0.25, color=self.colors['light_blue'], 
                        label='Integration Region D', edgecolor=self.colors['primary'], linewidth=1)
        ax1.plot(x, y_upper, color=self.colors['primary'], linewidth=3, label=r'$y = x$')
        ax1.plot(x, y_lower, color=self.colors['secondary'], linewidth=3, label=r'$y = x^2$')
        
        # 添加垂直积分线
        x_sample = 0.6
        y_lower_sample = x_sample**2
        y_upper_sample = x_sample
        ax1.plot([x_sample, x_sample], [y_lower_sample, y_upper_sample], 
                'o-', color=self.colors['accent'], linewidth=4, markersize=8,
                markerfacecolor=self.colors['warning_yellow'])
        
        # 标注积分线
        ax1.annotate(f'Fixed x = {x_sample}\n' + r'$x^2 \leq y \leq x$',
                   xy=(x_sample, (y_lower_sample + y_upper_sample)/2),
                   xytext=(x_sample + 0.2, (y_lower_sample + y_upper_sample)/2),
                   fontsize=11, ha='left', va='center',
                   bbox=dict(boxstyle="round,pad=0.3", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', color=self.colors['accent'], lw=2))
        
        # 标注交点
        ax1.plot([0, 1], [0, 1], 'o', color=self.colors['warning_yellow'], markersize=10, 
               markeredgecolor=self.colors['dark_gray'], markeredgewidth=2)
        ax1.annotate('(0, 0)', xy=(0, 0), xytext=(-0.1, -0.1),
                   fontsize=10, fontweight='bold', color=self.colors['dark_gray'])
        ax1.annotate('(1, 1)', xy=(1, 1), xytext=(1.05, 1.05),
                   fontsize=10, fontweight='bold', color=self.colors['dark_gray'])
        
        ax1.set_xlim(-0.15, 1.3)
        ax1.set_ylim(-0.15, 1.2)
        ax1.set_xlabel('x', fontsize=16, fontweight='bold')
        ax1.set_ylabel('y', fontsize=16, fontweight='bold')
        ax1.set_title('X-type Region: dy dx order\n' + 
                     r'$\int_0^1 dx \int_{x^2}^x f(x,y) dy$', 
                     fontsize=14, fontweight='bold')
        ax1.grid(True, alpha=0.3)
        ax1.legend(fontsize=11, loc='upper left')
        
        # 右图：dxdy次序 (Y型区域)
        y = np.linspace(0, 1, 100)
        # 对于Y型区域：从y=x²得到x=√y，从y=x得到x=y
        x_left = np.sqrt(y)  # 从 y = x² 得到 x = √y (右边界)
        x_right = y  # 从 y = x 得到 x = y (左边界)
        
        ax2.fill_betweenx(y, x_left, x_right, alpha=0.25, 
                         color=self.colors['light_green'], label='Integration Region D',
                         edgecolor=self.colors['accent'], linewidth=1)
        ax2.plot(x_left, y, color=self.colors['secondary'], linewidth=3, label=r'$x = \sqrt{y}$')
        ax2.plot(x_right, y, color=self.colors['primary'], linewidth=3, label=r'$x = y$')
        
        # 添加水平积分线
        y_sample = 0.4
        x_left_sample = np.sqrt(y_sample)
        x_right_sample = y_sample
        ax2.plot([x_left_sample, x_right_sample], [y_sample, y_sample], 
                'o-', color=self.colors['purple'], linewidth=4, markersize=8,
                markerfacecolor=self.colors['warning_yellow'])
        
        # 标注积分线
        ax2.annotate(f'Fixed y = {y_sample}\n' + r'$\sqrt{y} \leq x \leq y$',
                   xy=((x_left_sample + x_right_sample)/2, y_sample),
                   xytext=((x_left_sample + x_right_sample)/2, y_sample + 0.2),
                   fontsize=11, ha='center', va='bottom',
                   bbox=dict(boxstyle="round,pad=0.3", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', color=self.colors['purple'], lw=2))
        
        # 标注交点
        ax2.plot([0, 1], [0, 1], 'o', color=self.colors['warning_yellow'], markersize=10, 
               markeredgecolor=self.colors['dark_gray'], markeredgewidth=2)
        ax2.annotate('(0, 0)', xy=(0, 0), xytext=(-0.1, -0.1),
                   fontsize=10, fontweight='bold', color=self.colors['dark_gray'])
        ax2.annotate('(1, 1)', xy=(1, 1), xytext=(1.05, 1.05),
                   fontsize=10, fontweight='bold', color=self.colors['dark_gray'])
        
        ax2.set_xlim(-0.15, 1.3)
        ax2.set_ylim(-0.15, 1.2)
        ax2.set_xlabel('x', fontsize=16, fontweight='bold')
        ax2.set_ylabel('y', fontsize=16, fontweight='bold')
        ax2.set_title('Y-type Region: dx dy order\n' + 
                     r'$\int_0^1 dy \int_{\sqrt{y}}^y f(x,y) dx$', 
                     fontsize=14, fontweight='bold')
        ax2.grid(True, alpha=0.3)
        ax2.legend(fontsize=11, loc='upper left')
        
        plt.tight_layout()
        self.save_plot(save_name)

    def plot_polar_coordinate_transform(self, save_name: str = 'polar_transform.png'):
        """绘制极坐标变换对比图"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 9))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 左图：直角坐标系下的圆形区域
        theta = np.linspace(0, 2*np.pi, 100)
        r = 2
        x_circle = r * np.cos(theta)
        y_circle = r * np.sin(theta)
        
        ax1.fill(x_circle, y_circle, alpha=0.25, color=self.colors['light_blue'], 
                label=r'$x^2 + y^2 \leq 4$', edgecolor=self.colors['primary'], linewidth=2)
        ax1.plot(x_circle, y_circle, color=self.colors['primary'], linewidth=3)
        
        # 添加网格线
        for i in range(-2, 3):
            ax1.axhline(y=i, color=self.colors['light_gray'], alpha=0.5, linewidth=1)
            ax1.axvline(x=i, color=self.colors['light_gray'], alpha=0.5, linewidth=1)
        
        ax1.set_xlim(-3, 3)
        ax1.set_ylim(-3, 3)
        ax1.set_xlabel('x', fontsize=16, fontweight='bold')
        ax1.set_ylabel('y', fontsize=16, fontweight='bold')
        ax1.set_title('Rectangular Coordinates\n' + r'$\iint_D f(x,y) \, dx \, dy$', 
                     fontsize=16, fontweight='bold')
        ax1.grid(True, alpha=0.3)
        ax1.set_aspect('equal')
        ax1.legend(fontsize=12)
        
        # 右图：极坐标系
        # 绘制极坐标网格
        for r_grid in [0.5, 1, 1.5, 2]:
            x_grid = r_grid * np.cos(theta)
            y_grid = r_grid * np.sin(theta)
            ax2.plot(x_grid, y_grid, color=self.colors['light_gray'], alpha=0.6, linewidth=1)
        
        # 绘制射线
        for angle in np.linspace(0, 2*np.pi, 8, endpoint=False):
            x_ray = [0, 2*np.cos(angle)]
            y_ray = [0, 2*np.sin(angle)]
            ax2.plot(x_ray, y_ray, color=self.colors['light_gray'], alpha=0.6, linewidth=1)
        
        # 高亮积分区域
        ax2.fill(x_circle, y_circle, alpha=0.25, color=self.colors['light_coral'], 
                label=r'$0 \leq r \leq 2, 0 \leq \theta \leq 2\pi$', 
                edgecolor=self.colors['secondary'], linewidth=2)
        ax2.plot(x_circle, y_circle, color=self.colors['secondary'], linewidth=3)
        
        ax2.set_xlim(-3, 3)
        ax2.set_ylim(-3, 3)
        ax2.set_xlabel('x = r cos θ', fontsize=16, fontweight='bold')
        ax2.set_ylabel('y = r sin θ', fontsize=16, fontweight='bold')
        ax2.set_title('Polar Coordinates\n' + r'$\iint_D f(r\cos\theta, r\sin\theta) \cdot r \, dr \, d\theta$', 
                     fontsize=16, fontweight='bold')
        ax2.set_aspect('equal')
        ax2.legend(fontsize=12)
        
        plt.tight_layout()
        self.save_plot(save_name)

    def plot_polar_example_4_1(self, save_name='polar_circle.png'):
        """绘制极坐标例题4.1：圆形区域x²+y²≤4"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))
        
        # 左图：直角坐标表示
        theta = np.linspace(0, 2*np.pi, 100)
        x_circle = 2 * np.cos(theta)
        y_circle = 2 * np.sin(theta)
        
        ax1.fill(x_circle, y_circle, alpha=0.3, color=self.colors['primary'], label=r'$D: x^2 + y^2 \leq 4$')
        ax1.plot(x_circle, y_circle, color=self.colors['primary'], linewidth=2)
        ax1.set_xlim(-3, 3)
        ax1.set_ylim(-3, 3)
        ax1.set_aspect('equal')
        ax1.grid(True, alpha=0.3)
        ax1.set_xlabel('$x$', fontsize=16)
        ax1.set_ylabel('$y$', fontsize=16)
        ax1.set_title('Rectangular Coordinates', fontsize=16)
        ax1.legend()
        
        # 右图：极坐标表示
        r = np.linspace(0, 2, 50)
        theta = np.linspace(0, 2*np.pi, 100)
        R, THETA = np.meshgrid(r, theta)
        X = R * np.cos(THETA)
        Y = R * np.sin(THETA)
        
        ax2 = plt.subplot(122, projection='polar')
        ax2.fill_between(theta, 0, 2, alpha=0.3, color=self.colors['primary'])
        ax2.plot(theta, np.full_like(theta, 2), color=self.colors['primary'], linewidth=2)
        ax2.set_ylim(0, 3)
        ax2.set_title(r'Polar Coordinates: $r \leq 2$', fontsize=16, pad=20)
        ax2.grid(True)
        
        plt.tight_layout()
        self.save_plot(save_name)
        
    def plot_polar_example_4_2(self, save_name='polar_exponential.png'):
        """绘制极坐标例题4.2：单位圆x²+y²≤1与被积函数"""
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
        
        # Top left: Integration region
        theta = np.linspace(0, 2*np.pi, 100)
        x_circle = np.cos(theta)
        y_circle = np.sin(theta)
        
        ax1.fill(x_circle, y_circle, alpha=0.3, color=self.colors['primary'])
        ax1.plot(x_circle, y_circle, color=self.colors['primary'], linewidth=2)
        ax1.set_xlim(-1.5, 1.5)
        ax1.set_ylim(-1.5, 1.5)
        ax1.set_aspect('equal')
        ax1.grid(True, alpha=0.3)
        ax1.set_xlabel('$x$', fontsize=16)
        ax1.set_ylabel('$y$', fontsize=16)
        ax1.set_title(r'Integration Region: $x^2 + y^2 \leq 1$', fontsize=16)
        
        # Top right: 3D integrand function
        x = np.linspace(-1.2, 1.2, 50)
        y = np.linspace(-1.2, 1.2, 50)
        X, Y = np.meshgrid(x, y)
        Z = np.exp(-(X**2 + Y**2))
        
        # 只显示单位圆内的部分
        mask = X**2 + Y**2 <= 1
        Z_masked = np.where(mask, Z, np.nan)
        
        ax2 = fig.add_subplot(222, projection='3d')
        surf = ax2.plot_surface(X, Y, Z_masked, cmap='viridis', alpha=0.8)
        ax2.set_xlabel('$x$', fontsize=16)
        ax2.set_ylabel('$y$', fontsize=16)
        ax2.set_zlabel('$f(x,y) = e^{-(x^2+y^2)}$', fontsize=16)
        ax2.set_title('Integrand Function', fontsize=16)
        
        # Left bottom: Polar coordinates representation
        ax3 = plt.subplot(223, projection='polar')
        theta = np.linspace(0, 2*np.pi, 100)
        ax3.fill_between(theta, 0, 1, alpha=0.3, color=self.colors['primary'])
        
        # Show equidistant circles
        for r_val in [0.2, 0.4, 0.6, 0.8, 1.0]:
            ax3.plot(theta, np.full_like(theta, r_val), '--', 
                    color=self.colors['secondary'], alpha=0.7)
                    
        ax3.set_title(r'Polar Coordinates: $0 \leq r \leq 1$', fontsize=16, pad=20)
        ax3.grid(True)
        
        # Bottom right: Integrand function
        r_vals = np.linspace(0, 1, 100)
        integrand = r_vals * np.exp(-r_vals**2)
        
        ax4.plot(r_vals, integrand, color=self.colors['primary'], linewidth=2, 
                label=r'$r \cdot e^{-r^2}$')
        ax4.fill_between(r_vals, 0, integrand, alpha=0.3, color=self.colors['primary'])
        ax4.set_xlabel('$r$', fontsize=16)
        ax4.set_ylabel('Integrand', fontsize=16)
        ax4.set_title(r'Polar Form: $r \cdot e^{-r^2}$', fontsize=16)
        ax4.grid(True, alpha=0.3)
        ax4.legend()
        
        plt.tight_layout()
        self.save_plot(save_name)
        
    def plot_polar_example_7_2(self, save_name='polar_volume.png'):
        """Plot Example 7.2: 3D Volume Calculation"""
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
        
        # Top left: Projection region
        theta = np.linspace(0, 2*np.pi, 100)
        x_circle = np.sqrt(2) * np.cos(theta)
        y_circle = np.sqrt(2) * np.sin(theta)
        
        ax1.fill(x_circle, y_circle, alpha=0.3, color=self.colors['primary'])
        ax1.plot(x_circle, y_circle, color=self.colors['primary'], linewidth=2, 
                label=r'$x^2 + y^2 = 2$')
        ax1.set_xlim(-2, 2)
        ax1.set_ylim(-2, 2)
        ax1.set_aspect('equal')
        ax1.grid(True, alpha=0.3)
        ax1.set_xlabel('$x$', fontsize=16)
        ax1.set_ylabel('$y$', fontsize=16)
        ax1.set_title(r'Projection Region: $x^2 + y^2 \leq 2$', fontsize=16)
        ax1.legend()
        
        # Top right: 3D solid
        x = np.linspace(-1.5, 1.5, 30)
        y = np.linspace(-1.5, 1.5, 30)
        X, Y = np.meshgrid(x, y)
        
        # 只在投影区域内
        mask = X**2 + Y**2 <= 2
        Z_bottom = np.where(mask, X**2 + Y**2, np.nan)
        Z_top = np.where(mask, 2, np.nan)
        
        ax2 = fig.add_subplot(222, projection='3d')
        ax2.plot_surface(X, Y, Z_bottom, alpha=0.6, color=self.colors['secondary'], label='$z = x^2 + y^2$')
        ax2.plot_surface(X, Y, Z_top, alpha=0.6, color=self.colors['accent'], label='$z = 2$')
        ax2.set_xlabel('$x$', fontsize=16)
        ax2.set_ylabel('$y$', fontsize=16)
        ax2.set_zlabel('$z$', fontsize=16)
        ax2.set_title('3D Solid', fontsize=16)
        
        # Bottom left: Polar coordinates representation
        ax3 = plt.subplot(223, projection='polar')
        theta = np.linspace(0, 2*np.pi, 100)
        ax3.fill_between(theta, 0, np.sqrt(2), alpha=0.3, color=self.colors['primary'])
        ax3.plot(theta, np.full_like(theta, np.sqrt(2)), color=self.colors['primary'], linewidth=2)
        ax3.set_title(r'Polar Coordinates: $r \leq \sqrt{2}$', fontsize=16, pad=20)
        ax3.grid(True)
        
        # Bottom right: Integrand function
        r_vals = np.linspace(0, np.sqrt(2), 100)
        height_func = 2 - r_vals**2
        
        ax4.plot(r_vals, height_func, color=self.colors['primary'], linewidth=2, 
                label=r'$h(r) = 2 - r^2$')
        ax4.fill_between(r_vals, 0, height_func, alpha=0.3, color=self.colors['primary'])
        ax4.set_xlabel('$r$', fontsize=16)
        ax4.set_ylabel('Height', fontsize=16)
        ax4.set_title('Height Function of Solid', fontsize=16)
        ax4.grid(True, alpha=0.3)
        ax4.legend()
        
        plt.tight_layout()
        self.save_plot(save_name)
        
    def plot_polar_example_7_4(self, save_name='polar_moment.png'):
        """Plot Example 7.4: Moment of Inertia Calculation"""
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))
        
        # Top left: Unit disk
        theta = np.linspace(0, 2*np.pi, 100)
        x_circle = np.cos(theta)
        y_circle = np.sin(theta)
        
        ax1.fill(x_circle, y_circle, alpha=0.3, color=self.colors['primary'])
        ax1.plot(x_circle, y_circle, color=self.colors['primary'], linewidth=2)
        
        # 添加一些径向线显示转动
        for angle in np.linspace(0, 2*np.pi, 8, endpoint=False):
            x_line = [0, np.cos(angle)]
            y_line = [0, np.sin(angle)]
            ax1.plot(x_line, y_line, '--', color=self.colors['secondary'], alpha=0.5)
            
        ax1.scatter(0, 0, color='red', s=50, zorder=5, label='Rotation Center')
        ax1.set_xlim(-1.5, 1.5)
        ax1.set_ylim(-1.5, 1.5)
        ax1.set_aspect('equal')
        ax1.grid(True, alpha=0.3)
        ax1.set_xlabel('$x$', fontsize=16)
        ax1.set_ylabel('$y$', fontsize=16)
        ax1.set_title(r'Unit Disk: $x^2 + y^2 \leq 1$', fontsize=16)
        ax1.legend()
        
        # Top right: Distance function
        x = np.linspace(-1, 1, 30)
        y = np.linspace(-1, 1, 30)
        X, Y = np.meshgrid(x, y)
        R_squared = X**2 + Y**2
        
        # 只在单位圆内
        mask = R_squared <= 1
        R_squared_masked = np.where(mask, R_squared, np.nan)
        
        ax2 = fig.add_subplot(222, projection='3d')
        surf = ax2.plot_surface(X, Y, R_squared_masked, cmap='viridis', alpha=0.8)
        ax2.set_xlabel('$x$', fontsize=16)
        ax2.set_ylabel('$y$', fontsize=16)
        ax2.set_zlabel('$x^2 + y^2$', fontsize=16)
        ax2.set_title('Distance Squared Function', fontsize=16)
        
        # Left bottom: Polar coordinates representation
        ax3 = plt.subplot(223, projection='polar')
        theta = np.linspace(0, 2*np.pi, 100)
        ax3.fill_between(theta, 0, 1, alpha=0.3, color=self.colors['primary'])
        
        # Show equidistant circles
        for r_val in [0.2, 0.4, 0.6, 0.8, 1.0]:
            ax3.plot(theta, np.full_like(theta, r_val), '--', 
                    color=self.colors['secondary'], alpha=0.7)
                    
        ax3.set_title(r'Polar Coordinates: $0 \leq r \leq 1$', fontsize=16, pad=20)
        ax3.grid(True)
        
        # Bottom right: Integrand function
        r_vals = np.linspace(0, 1, 100)
        integrand = r_vals**2 * r_vals  # r² * r (Jacobian)
        
        ax4.plot(r_vals, integrand, color=self.colors['primary'], linewidth=2, 
                label=r'$r^2 \cdot r = r^3$')
        ax4.fill_between(r_vals, 0, integrand, alpha=0.3, color=self.colors['primary'])
        ax4.set_xlabel('$r$', fontsize=16)
        ax4.set_ylabel('Integrand', fontsize=16)
        ax4.set_title(r'Polar Form: $r^2 \cdot r \cdot dr$', fontsize=16)
        ax4.grid(True, alpha=0.3)
        ax4.legend()
        
        plt.tight_layout()
        self.save_plot(save_name)

    def generate_all_plots(self, prefix: str = ''):
        """Generate all plots"""
        print("Starting to generate all mathematical plots...")
        
        # Double integral plots
        self.plot_x_type_region(f'{prefix}x_type_region.png')
        self.plot_y_type_region(f'{prefix}y_type_region.png')
        self.plot_example_region_parabola(f'{prefix}region_parabola.png')
        self.plot_example_region_triangle(f'{prefix}region_triangle.png')
        self.plot_double_integral_3d_enhanced(f'{prefix}double_integral_3d.png')
        self.plot_integration_order_comparison(f'{prefix}integration_order.png')
        self.plot_polar_coordinate_transform(f'{prefix}polar_transform.png')
        
        # Complex example regions (7.1 additional examples)
        self.plot_example_region_complex_1(f'{prefix}region_symmetric.png')
        self.plot_example_region_complex_2(f'{prefix}region_complex.png')
        
        # Triple integral plots
        self.plot_cylindrical_coordinates(f'{prefix}cylindrical_coord.png')
        self.plot_spherical_coordinates(f'{prefix}spherical_coord.png')
        self.plot_triple_integral_region(f'{prefix}triple_region.png')
        self.plot_coordinate_comparison(f'{prefix}coord_comparison.png')
        self.plot_tetrahedron_example(f'{prefix}tetrahedron_example.png')
        self.plot_sphere_cylinder_intersection(f'{prefix}sphere_cylinder.png')
        
        # Polar coordinate examples
        self.plot_polar_example_4_1(f'{prefix}polar_circle.png')
        self.plot_polar_example_4_2(f'{prefix}polar_exponential.png')
        self.plot_polar_example_7_2(f'{prefix}polar_volume.png')
        self.plot_polar_example_7_4(f'{prefix}polar_moment.png')
        
        print("All plots generated successfully!")
    
    def create_custom_plot(self, plot_type: str, **kwargs):
        """Create custom plot"""
        plot_methods = {
            'x_type': self.plot_x_type_region,
            'y_type': self.plot_y_type_region,
            'cylindrical': self.plot_cylindrical_coordinates,
            'spherical': self.plot_spherical_coordinates,
            'triple_region': self.plot_triple_integral_region,
            'coord_comparison': self.plot_coordinate_comparison
        }
        
        if plot_type in plot_methods:
            plot_methods[plot_type](**kwargs)
        else:
            print(f"Unknown plot type: {plot_type}")
            print(f"Available types: {list(plot_methods.keys())}")

    def plot_double_integral_3d_enhanced(self, save_name: str = 'double_integral_3d_enhanced.png'):
        """Enhanced 3D visualization of double integral geometric meaning"""
        fig = plt.figure(figsize=(20, 14))
        
        # Create layout with main plot and three subplots
        gs = fig.add_gridspec(2, 3, height_ratios=[3, 1], width_ratios=[1, 1, 1], 
                             hspace=0.25, wspace=0.15)
        
        # Main 3D plot
        ax_main = fig.add_subplot(gs[0, :], projection='3d')
        
        # Create finer mesh
        x = np.linspace(0, 2, 50)
        y = np.linspace(0, 2, 50)
        X, Y = np.meshgrid(x, y)
        
        # Define function z = f(x,y) = 2 + 0.5*sin(πx) + 0.5*cos(πy)
        Z = 2 + 0.5*np.sin(np.pi*X) + 0.5*np.cos(np.pi*Y)
        
        # Draw semi-transparent surface
        surf = ax_main.plot_surface(X, Y, Z, alpha=0.7, cmap='viridis', 
                                   edgecolor='none', linewidth=0, antialiased=True)
        
        # Draw integration region boundary on bottom
        x_region = np.array([0, 2, 2, 0, 0])
        y_region = np.array([0, 0, 2, 2, 0])
        z_bottom = np.zeros_like(x_region)
        
        # Bottom region boundary (emphasized)
        ax_main.plot(x_region, y_region, z_bottom, color=self.colors['secondary'], 
                    linewidth=5, label='Integration Region D')
        
        # Create bottom fill using 3D polygon
        from mpl_toolkits.mplot3d.art3d import Poly3DCollection
        verts = [list(zip(x_region[:-1], y_region[:-1], z_bottom[:-1]))]
        ax_main.add_collection3d(Poly3DCollection(verts, alpha=0.4, facecolor=self.colors['light_coral']))
        
        # Draw vertical lines to represent differential elements
        x_samples = np.linspace(0.3, 1.7, 8)
        y_samples = np.linspace(0.3, 1.7, 8)
        
        for xi in x_samples:
            for yi in y_samples:
                zi = 2 + 0.5*np.sin(np.pi*xi) + 0.5*np.cos(np.pi*yi)
                # Gradient colored vertical lines
                color_intensity = (zi - Z.min()) / (Z.max() - Z.min())
                line_color = plt.cm.viridis(color_intensity)
                ax_main.plot([xi, xi], [yi, yi], [0, zi], 
                           color=line_color, alpha=0.8, linewidth=2.5)
                # Top points
                ax_main.scatter([xi], [yi], [zi], color=line_color, s=50, alpha=0.9)
        
        # Draw cross-sections to show integration process
        x_cut = 1.0
        y_cut_line = np.linspace(0, 2, 30)
        z_cut_line = 2 + 0.5*np.sin(np.pi*x_cut) + 0.5*np.cos(np.pi*y_cut_line)
        ax_main.plot([x_cut]*len(y_cut_line), y_cut_line, z_cut_line, 
                    color=self.colors['accent'], linewidth=4, alpha=0.9, label=f'Cross-section x={x_cut}')
        
        y_cut = 1.0
        x_cut_line = np.linspace(0, 2, 30)
        z_cut_line2 = 2 + 0.5*np.sin(np.pi*x_cut_line) + 0.5*np.cos(np.pi*y_cut)
        ax_main.plot(x_cut_line, [y_cut]*len(x_cut_line), z_cut_line2, 
                    color=self.colors['warning_yellow'], linewidth=4, alpha=0.9, label=f'Cross-section y={y_cut}')
        
        # Set main plot labels
        ax_main.set_xlabel('x', fontsize=18, fontweight='bold', labelpad=15)
        ax_main.set_ylabel('y', fontsize=18, fontweight='bold', labelpad=15)
        ax_main.set_zlabel('z = f(x,y)', fontsize=18, fontweight='bold', labelpad=15)
        
        title_text = ('Double Integral Geometric Meaning: Volume of Curved-Top Cylinder\n' +
                     r'$V = \iint_D f(x,y) \, dA = \iint_D f(x,y) \, dx \, dy$')
        ax_main.set_title(title_text, fontsize=20, fontweight='bold', pad=30)
        
        # Set viewing angle
        ax_main.view_init(elev=20, azim=45)
        ax_main.set_xlim(0, 2)
        ax_main.set_ylim(0, 2)
        ax_main.set_zlim(0, Z.max()*1.1)
        ax_main.legend(loc='upper left', fontsize=12, framealpha=0.8)
        
        # Left subplot: Function value distribution
        ax1 = fig.add_subplot(gs[1, 0])
        x_2d = np.linspace(0, 2, 100)
        y_2d = np.linspace(0, 2, 100)
        X_2d, Y_2d = np.meshgrid(x_2d, y_2d)
        Z_2d = 2 + 0.5*np.sin(np.pi*X_2d) + 0.5*np.cos(np.pi*Y_2d)
        
        contour = ax1.contourf(X_2d, Y_2d, Z_2d, levels=15, cmap='viridis', alpha=0.8)
        ax1.contour(X_2d, Y_2d, Z_2d, levels=15, colors='white', alpha=0.6, linewidths=0.5)
        ax1.plot(x_region, y_region, color=self.colors['secondary'], linewidth=3)
        ax1.set_xlabel('x', fontsize=14, fontweight='bold')
        ax1.set_ylabel('y', fontsize=14, fontweight='bold')
        ax1.set_title('Function Values\nover Region D', fontsize=12, fontweight='bold')
        ax1.set_aspect('equal')
        cbar1 = plt.colorbar(contour, ax=ax1, shrink=0.8)
        cbar1.set_label('f(x,y)', fontsize=12)
        
        # Middle subplot: Differential elements
        ax2 = fig.add_subplot(gs[1, 1])
        dx = 0.25
        dy = 0.25
        for i in np.arange(0, 2+dx, dx):
            ax2.axvline(x=i, color=self.colors['light_gray'], alpha=0.7, linewidth=1)
        for j in np.arange(0, 2+dy, dy):
            ax2.axhline(y=j, color=self.colors['light_gray'], alpha=0.7, linewidth=1)
        
        # Highlight some differential elements
        highlight_x = [0.5, 1.0, 1.5]
        highlight_y = [0.5, 1.0, 1.5]
        for hx in highlight_x:
            for hy in highlight_y:
                rect = patches.Rectangle((hx-dx/2, hy-dy/2), dx, dy, 
                                       linewidth=2, edgecolor=self.colors['accent'], 
                                       facecolor=self.colors['light_green'], alpha=0.7)
                ax2.add_patch(rect)
        
        ax2.plot(x_region, y_region, color=self.colors['secondary'], linewidth=3)
        ax2.set_xlabel('x', fontsize=14, fontweight='bold')
        ax2.set_ylabel('y', fontsize=14, fontweight='bold')
        ax2.set_title('Differential\nElements dA', fontsize=12, fontweight='bold')
        ax2.set_xlim(0, 2)
        ax2.set_ylim(0, 2)
        ax2.set_aspect('equal')
        
        # Right subplot: Integration concept
        ax3 = fig.add_subplot(gs[1, 2])
        x_section = np.linspace(0, 2, 100)
        y_fixed = 1.0
        z_section = 2 + 0.5*np.sin(np.pi*x_section) + 0.5*np.cos(np.pi*y_fixed)
        
        ax3.plot(x_section, z_section, color=self.colors['primary'], linewidth=3, 
                label=f'f(x, {y_fixed})')
        ax3.fill_between(x_section, 0, z_section, alpha=0.3, color=self.colors['light_blue'])
        
        ax3.text(1, z_section.max()*0.7, r'$\int_0^2 f(x,y_0) dx$', 
                fontsize=14, fontweight='bold', ha='center',
                bbox=dict(boxstyle="round,pad=0.3", facecolor=self.colors['light_coral'], alpha=0.8))
        
        ax3.set_xlabel('x', fontsize=14, fontweight='bold')
        ax3.set_ylabel('z = f(x,y)', fontsize=14, fontweight='bold')
        ax3.set_title('Integration along x\n(fixed y)', fontsize=12, fontweight='bold')
        ax3.grid(True, alpha=0.3)
        ax3.set_xlim(0, 2)
        ax3.legend(fontsize=10)
        
        # Add overall explanation
        fig.text(0.5, 0.02, 
                'Double Integral = Volume = Sum of products f(x,y)·dA over region D',
                ha='center', fontsize=14, fontweight='bold', 
                bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                         edgecolor=self.colors['dark_gray'], linewidth=2))
        
        plt.tight_layout()
        self.save_plot(save_name)

    def plot_example_region_complex_1(self, save_name: str = 'region_symmetric.png'):
        """绘制例题3.3区域：y=x²和y=2-x²围成的区域"""
        fig, ax = plt.subplots(figsize=(14, 10))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 定义函数
        x = np.linspace(-1.5, 1.5, 200)
        y1 = x**2  # y = x²
        y2 = 2 - x**2  # y = 2 - x²
        
        # 找交点
        x_intersect = [-1, 1]
        y_intersect = [1, 1]
        
        # 绘制曲线
        ax.plot(x, y1, color=self.colors['primary'], linewidth=3.5, label=r'$y = x^2$')
        ax.plot(x, y2, color=self.colors['secondary'], linewidth=3.5, label=r'$y = 2 - x^2$')
        
        # 填充积分区域
        x_fill = np.linspace(-1, 1, 100)
        y1_fill = x_fill**2
        y2_fill = 2 - x_fill**2
        ax.fill_between(x_fill, y1_fill, y2_fill, alpha=0.25, color=self.colors['light_coral'], 
                       label='Integration Region D',
                       edgecolor=self.colors['accent'], linewidth=1)
        
        # 标注交点
        for i, (xi, yi) in enumerate(zip(x_intersect, y_intersect)):
            ax.plot(xi, yi, 'o', color=self.colors['warning_yellow'], markersize=12, 
                   markeredgecolor=self.colors['dark_gray'], markeredgewidth=2)
            ax.annotate(f'({xi}, {yi})', xy=(xi, yi), xytext=(xi+0.1, yi+0.1),
                       fontsize=12, fontweight='bold', color=self.colors['dark_gray'])
        
        # 添加积分线示例
        x_sample = 0.5
        y_sample_lower = x_sample**2
        y_sample_upper = 2 - x_sample**2
        
        ax.plot([x_sample, x_sample], [y_sample_lower, y_sample_upper], 
               'o-', color=self.colors['accent'], linewidth=4, markersize=8,
               markerfacecolor=self.colors['warning_yellow'])
        
        # 标注积分线
        ax.annotate(f'x = {x_sample}\n' + r'$x^2 \leq y \leq 2-x^2$',
                   xy=(x_sample, (y_sample_lower + y_sample_upper)/2),
                   xytext=(x_sample + 0.3, (y_sample_lower + y_sample_upper)/2),
                   fontsize=12, ha='left', va='center',
                   bbox=dict(boxstyle="round,pad=0.4", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', color=self.colors['accent'], lw=2))
        
        # 设置坐标轴
        ax.set_xlim(-1.5, 1.5)
        ax.set_ylim(-0.3, 2.3)
        ax.set_xlabel('x', fontsize=18, fontweight='bold')
        ax.set_ylabel('y', fontsize=18, fontweight='bold')
        
        title_text = r"Example 3.3: Region bounded by $y = x^2$ and $y = 2 - x^2$"
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=25)
        
        # 添加积分公式
        formula_text = r'$\iint_D x \, dA = \int_{-1}^1 dx \int_{x^2}^{2-x^2} x \, dy = 0$ (by symmetry)'
        ax.text(-1.4, 2.1, formula_text, fontsize=14, 
               bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                        alpha=0.95, edgecolor=self.colors['primary'], linewidth=2))
        
        # 图例和网格
        legend = ax.legend(fontsize=13, loc='upper right', frameon=True, 
                          fancybox=True, shadow=True, framealpha=0.95)
        legend.get_frame().set_facecolor('white')
        
        ax.grid(True, alpha=0.3, color=self.colors['light_gray'], linewidth=1)
        ax.tick_params(axis='both', which='major', labelsize=13)
        
        self.save_plot(save_name)

    def plot_example_region_complex_2(self, save_name: str = 'region_complex.png'):
        """绘制例题3.4区域：y=x², y=2x-1和x=2围成的区域"""
        fig, ax = plt.subplots(figsize=(14, 10))
        fig.patch.set_facecolor(self.colors['background'])
        
        # 定义函数
        x1 = np.linspace(0.5, 2.5, 200)
        y1 = x1**2  # y = x²
        y2 = 2*x1 - 1  # y = 2x - 1
        
        # 区域边界：x从1到2
        x_fill = np.linspace(1, 2, 100)
        y1_fill = x_fill**2  # 上边界 y = x²
        y2_fill = 2*x_fill - 1  # 下边界 y = 2x - 1
        
        # 绘制曲线
        ax.plot(x1, y1, color=self.colors['primary'], linewidth=3.5, label=r'$y = x^2$')
        ax.plot(x1, y2, color=self.colors['secondary'], linewidth=3.5, label=r'$y = 2x - 1$')
        ax.axvline(x=2, color=self.colors['accent'], linewidth=3.5, label=r'$x = 2$', 
                  ymin=0.1, ymax=0.9)
        
        # 填充积分区域
        ax.fill_between(x_fill, y2_fill, y1_fill, alpha=0.25, color=self.colors['light_green'], 
                       label='Integration Region D',
                       edgecolor=self.colors['orange'], linewidth=2)
        
        # 标注关键点
        key_points = [(1, 1), (2, 4), (2, 3)]
        point_labels = ['(1, 1)', '(2, 4)', '(2, 3)']
        
        for (xi, yi), label in zip(key_points, point_labels):
            ax.plot(xi, yi, 'o', color=self.colors['warning_yellow'], markersize=12, 
                   markeredgecolor=self.colors['dark_gray'], markeredgewidth=2)
            offset_x = 0.1 if xi == 1 else -0.2
            offset_y = 0.1 if yi == 1 else 0.1
            ax.annotate(label, xy=(xi, yi), xytext=(xi+offset_x, yi+offset_y),
                       fontsize=12, fontweight='bold', color=self.colors['dark_gray'])
        
        # 添加积分线示例
        x_sample = 1.5
        y_sample_lower = 2*x_sample - 1
        y_sample_upper = x_sample**2
        
        ax.plot([x_sample, x_sample], [y_sample_lower, y_sample_upper], 
               'o-', color=self.colors['purple'], linewidth=4, markersize=8,
               markerfacecolor=self.colors['warning_yellow'])
        
        # 标注积分线
        ax.annotate(f'x = {x_sample}\n' + r'$2x-1 \leq y \leq x^2$',
                   xy=(x_sample, (y_sample_lower + y_sample_upper)/2),
                   xytext=(x_sample - 0.4, (y_sample_lower + y_sample_upper)/2),
                   fontsize=12, ha='center', va='center',
                   bbox=dict(boxstyle="round,pad=0.4", facecolor=self.colors['warning_yellow'], 
                            alpha=0.9, edgecolor=self.colors['dark_gray']),
                   arrowprops=dict(arrowstyle='->', color=self.colors['purple'], lw=2))
        
        # 设置坐标轴
        ax.set_xlim(0.5, 2.7)
        ax.set_ylim(-0.5, 4.5)
        ax.set_xlabel('x', fontsize=18, fontweight='bold')
        ax.set_ylabel('y', fontsize=18, fontweight='bold')
        
        title_text = r"Example 3.4: Region bounded by $y = x^2$, $y = 2x-1$, $x = 2$"
        ax.set_title(title_text, fontsize=16, fontweight='bold', pad=25)
        
        # 添加积分公式
        formula_text = r'$\iint_D xy \, dA = \int_1^2 dx \int_{2x-1}^{x^2} xy \, dy = \frac{37}{12}$'
        ax.text(0.6, 4.1, formula_text, fontsize=14, 
               bbox=dict(boxstyle="round,pad=0.5", facecolor='white', 
                        alpha=0.95, edgecolor=self.colors['primary'], linewidth=2))
        
        # 图例和网格
        legend = ax.legend(fontsize=13, loc='upper left', frameon=True, 
                          fancybox=True, shadow=True, framealpha=0.95)
        legend.get_frame().set_facecolor('white')
        
        ax.grid(True, alpha=0.3, color=self.colors['light_gray'], linewidth=1)
        ax.tick_params(axis='both', which='major', labelsize=13)
        
        self.save_plot(save_name)

    def plot_tetrahedron_example(self, save_name: str = 'tetrahedron_example.png'):
        """绘制四面体例题区域图 - 例题2.1"""
        fig = plt.figure(figsize=(16, 10))
        ax = fig.add_subplot(111, projection='3d')
        
        # 绘制坐标轴
        ax.plot([0, 1.2], [0, 0], [0, 0], 'k-', linewidth=3, alpha=0.8)
        ax.plot([0, 0], [0, 1.2], [0, 0], 'k-', linewidth=3, alpha=0.8)
        ax.plot([0, 0], [0, 0], [0, 1.2], 'k-', linewidth=3, alpha=0.8)
        
        # 标注坐标轴
        ax.text(1.3, 0, 0, 'x', fontsize=18, fontweight='bold')
        ax.text(0, 1.3, 0, 'y', fontsize=18, fontweight='bold')
        ax.text(0, 0, 1.3, 'z', fontsize=18, fontweight='bold')
        
        # 四面体的四个顶点
        vertices = np.array([[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]])
        
        # 绘制四面体的边
        edges = [(0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)]
        for edge in edges:
            p1, p2 = vertices[edge[0]], vertices[edge[1]]
            ax.plot([p1[0], p2[0]], [p1[1], p2[1]], [p1[2], p2[2]], 
                   color=self.colors['primary'], linewidth=3, alpha=0.8)
        
        # 绘制四面体的面
        from mpl_toolkits.mplot3d.art3d import Poly3DCollection
        
        # 四个三角形面
        faces = [
            [vertices[0], vertices[1], vertices[2]],  # 底面
            [vertices[0], vertices[1], vertices[3]],  # xz面
            [vertices[0], vertices[2], vertices[3]],  # yz面
            [vertices[1], vertices[2], vertices[3]]   # 斜面
        ]
        
        colors = [self.colors['light_green'], self.colors['light_blue'], 
                 self.colors['light_coral'], '#E8DAEF']
        
        for i, face in enumerate(faces):
            ax.add_collection3d(Poly3DCollection([face], alpha=0.3, 
                                               facecolor=colors[i], 
                                               edgecolor=self.colors['primary'], 
                                               linewidth=2))
        
        # 标注顶点
        labels = ['O(0,0,0)', 'A(1,0,0)', 'B(0,1,0)', 'C(0,0,1)']
        offsets = [(0.05, 0.05, 0.05), (0.05, -0.1, 0.05), (-0.1, 0.05, 0.05), (0.05, 0.05, 0.05)]
        
        for i, (vertex, label, offset) in enumerate(zip(vertices, labels, offsets)):
            ax.scatter(*vertex, color=self.colors['secondary'], s=100, alpha=1)
            ax.text(vertex[0]+offset[0], vertex[1]+offset[1], vertex[2]+offset[2], 
                   label, fontsize=14, fontweight='bold', color=self.colors['dark_gray'])
        
        # 添加平面方程
        ax.text(0.5, 0.3, 0.7, 'x + y + z = 1', fontsize=16, fontweight='bold',
               bbox=dict(boxstyle="round,pad=0.3", facecolor='#FEF9E7', alpha=0.8),
               color=self.colors['dark_gray'])
        
        # 添加样本积分线
        x_sample, y_sample = 0.3, 0.2
        z_upper = 1 - x_sample - y_sample
        if z_upper > 0:
            ax.plot([x_sample, x_sample], [y_sample, y_sample], [0, z_upper], 
                   'o-', color=self.colors['warning_yellow'], linewidth=4, markersize=8)
            ax.text(x_sample+0.05, y_sample+0.05, z_upper/2, 
                   'Integration\nLine', fontsize=12, fontweight='bold', 
                   color=self.colors['warning_yellow'])
        
        # 绘制xy平面投影
        x_proj = np.array([0, 1, 0, 0])
        y_proj = np.array([0, 0, 1, 0])
        z_proj = np.zeros_like(x_proj)
        ax.plot(x_proj, y_proj, z_proj, '--', color=self.colors['accent'], 
               linewidth=2, alpha=0.7, label='Projection on xy-plane')
        
        # 设置标题
        title_text = ('Tetrahedron Region: Example 2.1\n' +
                     r'$\Omega = \{(x,y,z): x \geq 0, y \geq 0, z \geq 0, x+y+z \leq 1\}$')
        ax.set_title(title_text, fontsize=18, fontweight='bold', pad=30)
        
        # 设置视角和范围
        ax.set_xlim(0, 1.2)
        ax.set_ylim(0, 1.2)
        ax.set_zlim(0, 1.2)
        ax.view_init(elev=25, azim=45)
        
        # 添加图例
        ax.legend(loc='upper right', fontsize=12, framealpha=0.8)
        
        self.save_plot(save_name)
    
    def plot_sphere_cylinder_intersection(self, save_name: str = 'sphere_cylinder.png'):
        """绘制球与圆柱交集的复杂区域 - 例题10.4"""
        fig = plt.figure(figsize=(16, 10))
        ax = fig.add_subplot(111, projection='3d')
        
        # 绘制坐标轴
        ax.plot([-3, 3], [0, 0], [0, 0], 'k-', linewidth=2, alpha=0.6)
        ax.plot([0, 0], [-3, 3], [0, 0], 'k-', linewidth=2, alpha=0.6)
        ax.plot([0, 0], [0, 0], [-3, 3], 'k-', linewidth=2, alpha=0.6)
        
        # 标注坐标轴
        ax.text(3.2, 0, 0, 'x', fontsize=16, fontweight='bold')
        ax.text(0, 3.2, 0, 'y', fontsize=16, fontweight='bold')
        ax.text(0, 0, 3.2, 'z', fontsize=16, fontweight='bold')
        
        # 绘制球面 x² + y² + z² = 4
        u = np.linspace(0, 2*np.pi, 30)
        v = np.linspace(0, np.pi, 20)
        U, V = np.meshgrid(u, v)
        X_sphere = 2 * np.sin(V) * np.cos(U)
        Y_sphere = 2 * np.sin(V) * np.sin(U)
        Z_sphere = 2 * np.cos(V)
        
        ax.plot_surface(X_sphere, Y_sphere, Z_sphere, alpha=0.3, 
                       color=self.colors['light_blue'], 
                       edgecolor=self.colors['primary'], linewidth=0.5)
        
        # 绘制圆柱面 x² + y² = 1
        theta = np.linspace(0, 2*np.pi, 50)
        z_cyl = np.linspace(-2, 2, 30)
        Theta, Z_cyl = np.meshgrid(theta, z_cyl)
        X_cylinder = np.cos(Theta)
        Y_cylinder = np.sin(Theta)
        
        ax.plot_surface(X_cylinder, Y_cylinder, Z_cyl, alpha=0.4, 
                       color=self.colors['light_coral'], 
                       edgecolor=self.colors['secondary'], linewidth=0.5)
        
        # 绘制交集区域边界
        # 球与圆柱的交线在 r=1 处
        theta_boundary = np.linspace(0, 2*np.pi, 100)
        x_boundary = np.cos(theta_boundary)
        y_boundary = np.sin(theta_boundary)
        z_boundary_upper = np.sqrt(4 - 1)  # z = √(4-r²) = √3
        z_boundary_lower = -np.sqrt(4 - 1)
        
        ax.plot(x_boundary, y_boundary, z_boundary_upper * np.ones_like(theta_boundary), 
               color=self.colors['accent'], linewidth=4, alpha=0.9, label='Upper intersection')
        ax.plot(x_boundary, y_boundary, z_boundary_lower * np.ones_like(theta_boundary), 
               color=self.colors['accent'], linewidth=4, alpha=0.9, label='Lower intersection')
        
        # 在xy平面绘制投影圆
        ax.plot(x_boundary, y_boundary, np.zeros_like(theta_boundary), 
               '--', color=self.colors['warning_yellow'], linewidth=3, alpha=0.8, 
               label='Projection on xy-plane')
        
        # 填充交集区域（在柱坐标下）
        theta_fill = np.linspace(0, 2*np.pi, 20)
        r_fill = np.linspace(0, 1, 10)
        Theta_fill, R_fill = np.meshgrid(theta_fill, r_fill)
        X_fill = R_fill * np.cos(Theta_fill)
        Y_fill = R_fill * np.sin(Theta_fill)
        Z_fill_upper = np.sqrt(4 - R_fill**2)
        Z_fill_lower = -np.sqrt(4 - R_fill**2)
        
        # 绘制上下边界面
        ax.plot_surface(X_fill, Y_fill, Z_fill_upper, alpha=0.6, 
                       color=self.colors['light_green'], 
                       edgecolor=self.colors['accent'], linewidth=0.3)
        ax.plot_surface(X_fill, Y_fill, Z_fill_lower, alpha=0.6, 
                       color=self.colors['light_green'], 
                       edgecolor=self.colors['accent'], linewidth=0.3)
        
        # 添加样本积分线
        r_sample = 0.5
        x_sample = r_sample * np.cos(np.pi/4)
        y_sample = r_sample * np.sin(np.pi/4)
        z_lower = -np.sqrt(4 - r_sample**2)
        z_upper = np.sqrt(4 - r_sample**2)
        
        ax.plot([x_sample, x_sample], [y_sample, y_sample], [z_lower, z_upper], 
               'o-', color=self.colors['purple'], linewidth=5, markersize=10, alpha=0.9)
        ax.text(x_sample+0.2, y_sample+0.2, 0, 
               f'Integration line\nr = {r_sample}', fontsize=12, fontweight='bold', 
               color=self.colors['purple'],
               bbox=dict(boxstyle="round,pad=0.3", facecolor='#FEF9E7', alpha=0.8))
        
        # 添加方程标注
        ax.text(-1, -1, 2.5, 'Sphere: x² + y² + z² = 4', fontsize=14, fontweight='bold',
               bbox=dict(boxstyle="round,pad=0.3", facecolor=self.colors['light_blue'], alpha=0.8),
               color=self.colors['dark_gray'])
        
        ax.text(1.5, 1.5, -2.5, 'Cylinder: x² + y² = 1', fontsize=14, fontweight='bold',
               bbox=dict(boxstyle="round,pad=0.3", facecolor=self.colors['light_coral'], alpha=0.8),
               color=self.colors['dark_gray'])
        
        # 设置标题
        title_text = ('Complex Region: Sphere ∩ Cylinder (Example 10.4)\n' +
                     r'$\Omega = \{(x,y,z): x^2+y^2 \leq 1, x^2+y^2+z^2 \leq 4\}$')
        ax.set_title(title_text, fontsize=18, fontweight='bold', pad=30)
        
        # 添加积分表达式
        ax.text(0, 0, -3.5, 
               r'Volume = $\int_0^{2\pi} \int_0^1 \int_{-\sqrt{4-r^2}}^{\sqrt{4-r^2}} r \, dz \, dr \, d\theta$',
               fontsize=14, fontweight='bold', ha='center',
               bbox=dict(boxstyle="round,pad=0.5", facecolor='#FEF9E7', alpha=0.9),
               color=self.colors['dark_gray'])
        
        # 设置视角和范围
        ax.set_xlim(-2.5, 2.5)
        ax.set_ylim(-2.5, 2.5)
        ax.set_zlim(-3, 3)
        ax.view_init(elev=20, azim=45)
        
        # 添加图例
        ax.legend(loc='upper left', fontsize=12, framealpha=0.8)
        
        self.save_plot(save_name)

def main():
    """Main function"""
    print("Universal mathematical plotting tool starting...")
    
    # Create English plotter
    plotter_en = UniversalMathPlotter(language='en', style='academic')
    
    # Generate all plots with simplified naming (no prefix)
    plotter_en.generate_all_plots('')
    
    # Can also generate specific plots individually
    # plotter_en.create_custom_plot('cylindrical', save_name='my_cylindrical.png')
    
    print("Task completed!")


if __name__ == "__main__":
    main() 