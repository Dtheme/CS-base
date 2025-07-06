#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
三重积分3D可视化工具

基于数学解题作图标准规范，专门用于生成三重积分相关的立体图像：
- 投影法计算过程可视化  
- 截面法计算过程立体图
- 不同坐标系体积元素对比
- 复杂区域分割示意图
- 坐标变换几何直观图
"""

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.patches as patches
from matplotlib.patches import Circle, FancyBboxPatch
import warnings
warnings.filterwarnings('ignore')

class TripleIntegral3DVisualizer:
    """三重积分3D可视化类"""
    
    def __init__(self):
        """初始化可视化工具"""
        self.setup_matplotlib()
        self.setup_colors()
        self.setup_fonts()
        
    def setup_matplotlib(self):
        """设置matplotlib基本配置"""
        plt.rcParams['figure.dpi'] = 300
        plt.rcParams['savefig.dpi'] = 300
        plt.rcParams['figure.facecolor'] = 'white'
        plt.rcParams['axes.facecolor'] = 'white'
        
    def setup_colors(self):
        """设置三重积分专用配色方案"""
        self.colors = {
            # 立体区域配色
            'region_main': '#E6F3FF',      # 主积分区域 - 淡天蓝
            'region_projection': '#FFE4E6', # 投影区域 - 淡玫瑰
            'region_section': '#F0F8E6',    # 截面区域 - 淡绿色
            'region_volume': '#FFF3E0',     # 体积元素 - 淡橙色
            
            # 边界和轮廓
            'boundary_main': '#2E5BBA',     # 主边界 - 深海蓝
            'boundary_projection': '#C8102E', # 投影边界 - 深红
            'boundary_section': '#4CAF50',   # 截面边界 - 绿色
            'boundary_axis': '#333333',      # 坐标轴 - 深灰
            
            # 文字和标注
            'text_main': '#1A237E',         # 主要文字 - 深蓝色
            'text_formula': '#D32F2F',      # 公式文字 - 深红色
            'text_annotation': '#388E3C',   # 标注文字 - 深绿色
            
            # 坐标系专用
            'cylindrical': '#4FC3F7',       # 柱坐标 - 浅蓝色
            'spherical': '#81C784',         # 球坐标 - 浅绿色
            'cartesian': '#FFB74D',         # 直角坐标 - 浅橙色
        }
        
    def setup_fonts(self):
        """设置字体配置"""
        font_candidates = [
            'SimHei', 'Microsoft YaHei', 'PingFang SC', 
            'Arial Unicode MS', 'DejaVu Sans'
        ]
        
        for font in font_candidates:
            try:
                plt.rcParams['font.sans-serif'] = [font]
                break
            except:
                continue
                
        plt.rcParams['axes.unicode_minus'] = False
        
    def create_3d_figure(self, figsize=(16, 12), projection_2d=False):
        """创建3D图形基础框架"""
        if projection_2d:
            fig = plt.figure(figsize=figsize)
            ax_3d = fig.add_subplot(2, 2, 1, projection='3d')
            ax_xy = fig.add_subplot(2, 2, 2)  
            ax_xz = fig.add_subplot(2, 2, 3)   
            ax_yz = fig.add_subplot(2, 2, 4)  
            return fig, (ax_3d, ax_xy, ax_xz, ax_yz)
        else:
            fig = plt.figure(figsize=figsize)
            ax = fig.add_subplot(111, projection='3d')
            return fig, ax
            
    def setup_3d_axes(self, ax, xlim=(-3, 3), ylim=(-3, 3), zlim=(-3, 3)):
        """设置3D坐标轴"""
        ax.set_xlim(xlim)
        ax.set_ylim(ylim)
        ax.set_zlim(zlim)
        
        ax.set_xlabel('x', fontsize=16, color=self.colors['text_main'])
        ax.set_ylabel('y', fontsize=16, color=self.colors['text_main'])
        ax.set_zlabel('z', fontsize=16, color=self.colors['text_main'])
        
        ax.grid(True, alpha=0.3, color='#E0E0E0')
        ax.view_init(elev=20, azim=45)
        
    def draw_coordinate_axes(self, ax, length=2.5):
        """绘制坐标轴"""
        ax.plot([0, length], [0, 0], [0, 0], 
               color=self.colors['boundary_axis'], linewidth=3, alpha=0.8)
        ax.text(length+0.1, 0, 0, 'x', fontsize=14, color=self.colors['text_main'])
        
        ax.plot([0, 0], [0, length], [0, 0], 
               color=self.colors['boundary_axis'], linewidth=3, alpha=0.8)
        ax.text(0, length+0.1, 0, 'y', fontsize=14, color=self.colors['text_main'])
        
        ax.plot([0, 0], [0, 0], [0, length], 
               color=self.colors['boundary_axis'], linewidth=3, alpha=0.8)
        ax.text(0, 0, length+0.1, 'z', fontsize=14, color=self.colors['text_main'])
        
    def draw_projection_process(self, region_type='sphere'):
        """绘制投影法计算过程可视化"""
        fig, axes = self.create_3d_figure(projection_2d=True)
        ax_3d, ax_xy, ax_xz, ax_yz = axes
        
        if region_type == 'sphere':
            self._draw_sphere_projection_process(ax_3d, ax_xy, ax_xz, ax_yz)
        elif region_type == 'tetrahedron':
            self._draw_tetrahedron_projection_process(ax_3d, ax_xy, ax_xz, ax_yz)
            
        self._add_projection_annotations(fig)
        return fig
        
    def _draw_sphere_projection_process(self, ax_3d, ax_xy, ax_xz, ax_yz):
        """绘制球体投影过程"""
        # 3D球体
        self.setup_3d_axes(ax_3d, (-2, 2), (-2, 2), (-2, 2))
        self.draw_coordinate_axes(ax_3d, length=1.8)
        
        u = np.linspace(0, 2 * np.pi, 50)
        v = np.linspace(0, np.pi, 50)
        r = 1.5
        x = r * np.outer(np.cos(u), np.sin(v))
        y = r * np.outer(np.sin(u), np.sin(v))
        z = r * np.outer(np.ones(np.size(u)), np.cos(v))
        
        ax_3d.plot_surface(x, y, z, alpha=0.3, color=self.colors['region_main'])
        ax_3d.plot_wireframe(x, y, z, alpha=0.5, color=self.colors['boundary_main'], linewidth=0.5)
        ax_3d.set_title('球体: $x^2+y^2+z^2 \\leq R^2$', 
                       fontsize=14, pad=20, color=self.colors['text_main'])
        
        # xy平面投影
        circle = Circle((0, 0), r, fill=True, alpha=0.4, 
                       color=self.colors['region_projection'], 
                       edgecolor=self.colors['boundary_projection'], linewidth=2)
        ax_xy.add_patch(circle)
        ax_xy.set_xlim(-2, 2)
        ax_xy.set_ylim(-2, 2)
        ax_xy.set_aspect('equal')
        ax_xy.grid(True, alpha=0.3)
        ax_xy.set_xlabel('x', fontsize=12)
        ax_xy.set_ylabel('y', fontsize=12)
        ax_xy.set_title('xy投影: $x^2+y^2 \\leq R^2$', 
                       fontsize=12, color=self.colors['text_main'])
        
        # xz平面投影
        circle_xz = Circle((0, 0), r, fill=True, alpha=0.4,
                          color=self.colors['region_projection'],
                          edgecolor=self.colors['boundary_projection'], linewidth=2)
        ax_xz.add_patch(circle_xz)
        ax_xz.set_xlim(-2, 2)
        ax_xz.set_ylim(-2, 2)
        ax_xz.set_aspect('equal')
        ax_xz.grid(True, alpha=0.3)
        ax_xz.set_xlabel('x', fontsize=12)
        ax_xz.set_ylabel('z', fontsize=12)
        ax_xz.set_title('xz投影: $x^2+z^2 \\leq R^2$', 
                       fontsize=12, color=self.colors['text_main'])
        
        # yz平面投影
        circle_yz = Circle((0, 0), r, fill=True, alpha=0.4,
                          color=self.colors['region_projection'],
                          edgecolor=self.colors['boundary_projection'], linewidth=2)
        ax_yz.add_patch(circle_yz)
        ax_yz.set_xlim(-2, 2)
        ax_yz.set_ylim(-2, 2)
        ax_yz.set_aspect('equal')
        ax_yz.grid(True, alpha=0.3)
        ax_yz.set_xlabel('y', fontsize=12)
        ax_yz.set_ylabel('z', fontsize=12)
        ax_yz.set_title('yz投影: $y^2+z^2 \\leq R^2$', 
                       fontsize=12, color=self.colors['text_main'])
                       
    def _draw_tetrahedron_projection_process(self, ax_3d, ax_xy, ax_xz, ax_yz):
        """绘制四面体投影过程"""
        self.setup_3d_axes(ax_3d, (-0.2, 1.2), (-0.2, 1.2), (-0.2, 1.2))
        self.draw_coordinate_axes(ax_3d, length=1.0)
        
        vertices = np.array([[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]])
        faces = [
            [vertices[0], vertices[1], vertices[2]],
            [vertices[0], vertices[1], vertices[3]],
            [vertices[0], vertices[2], vertices[3]],
            [vertices[1], vertices[2], vertices[3]]
        ]
        
        poly3d = [[tuple(point) for point in face] for face in faces]
        ax_3d.add_collection3d(Poly3DCollection(poly3d, alpha=0.3, 
                                               facecolor=self.colors['region_main'],
                                               edgecolor=self.colors['boundary_main']))
        
        ax_3d.set_title('四面体: $x+y+z \\leq 1$', 
                       fontsize=14, pad=20, color=self.colors['text_main'])
        
        # xy投影 - 三角形
        triangle_xy = np.array([[0, 0], [1, 0], [0, 1], [0, 0]])
        ax_xy.plot(triangle_xy[:, 0], triangle_xy[:, 1], 
                  color=self.colors['boundary_projection'], linewidth=2)
        ax_xy.fill(triangle_xy[:-1, 0], triangle_xy[:-1, 1], 
                  alpha=0.4, color=self.colors['region_projection'])
        ax_xy.set_xlim(-0.2, 1.2)
        ax_xy.set_ylim(-0.2, 1.2)
        ax_xy.set_aspect('equal')
        ax_xy.grid(True, alpha=0.3)
        ax_xy.set_xlabel('x', fontsize=12)
        ax_xy.set_ylabel('y', fontsize=12)
        ax_xy.set_title('xy投影: $x+y \\leq 1$', fontsize=12, color=self.colors['text_main'])
        
        # xz和yz投影类似处理
        for ax, labels in [(ax_xz, ('x', 'z')), (ax_yz, ('y', 'z'))]:
            triangle = np.array([[0, 0], [1, 0], [0, 1], [0, 0]])
            ax.plot(triangle[:, 0], triangle[:, 1], 
                   color=self.colors['boundary_projection'], linewidth=2)
            ax.fill(triangle[:-1, 0], triangle[:-1, 1], 
                   alpha=0.4, color=self.colors['region_projection'])
            ax.set_xlim(-0.2, 1.2)
            ax.set_ylim(-0.2, 1.2)
            ax.set_aspect('equal')
            ax.grid(True, alpha=0.3)
            ax.set_xlabel(labels[0], fontsize=12)
            ax.set_ylabel(labels[1], fontsize=12)
            ax.set_title(f'{labels[0]}{labels[1]}投影', fontsize=12, color=self.colors['text_main'])
                       
    def _add_projection_annotations(self, fig):
        """添加投影法说明文字"""
        fig.suptitle('三重积分投影法计算过程可视化', 
                    fontsize=18, y=0.95, color=self.colors['text_main'])
        
        formula_text = (
            '投影法基本公式:\n'
            '$\\iiint_\\Omega f(x,y,z) dV = \\iint_{D_{xy}} dx dy \\int_{z_1(x,y)}^{z_2(x,y)} f(x,y,z) dz$\n\n'
            '计算步骤:\n'
            '1. 确定积分区域在坐标平面上的投影\n'
            '2. 确定第三个变量的积分限\n'
            '3. 建立累次积分表达式'
        )
        
        fig.text(0.02, 0.5, formula_text, fontsize=11, 
                color=self.colors['text_formula'], 
                verticalalignment='center',
                bbox=dict(boxstyle='round,pad=0.5', 
                         facecolor='white', 
                         edgecolor=self.colors['boundary_main'],
                         alpha=0.9))
                         
    def save_figure(self, fig, filename, output_dir='../../Assets'):
        """保存图形"""
        import os
        os.makedirs(output_dir, exist_ok=True)
        
        output_path = os.path.join(output_dir, filename)
        fig.savefig(output_path, dpi=300, bbox_inches='tight', 
                   facecolor='white', edgecolor='none')
        print(f"图像已保存: {output_path}")
        
        return output_path

    def draw_slice_method(self, region_type='paraboloid'):
        """绘制截面法计算过程可视化"""
        fig = plt.figure(figsize=(18, 12))
        
        # 主3D图
        ax_main = fig.add_subplot(2, 3, (1, 4), projection='3d')
        
        # 不同z值的截面图
        ax_slice1 = fig.add_subplot(2, 3, 2)
        ax_slice2 = fig.add_subplot(2, 3, 3) 
        ax_slice3 = fig.add_subplot(2, 3, 5)
        ax_slice4 = fig.add_subplot(2, 3, 6)
        
        if region_type == 'paraboloid':
            self._draw_paraboloid_slices(ax_main, [ax_slice1, ax_slice2, ax_slice3, ax_slice4])
        elif region_type == 'ellipsoid':
            self._draw_ellipsoid_slices(ax_main, [ax_slice1, ax_slice2, ax_slice3, ax_slice4])
            
        self._add_slice_annotations(fig)
        return fig
        
    def _draw_paraboloid_slices(self, ax_main, slice_axes):
        """绘制抛物面的截面法"""
        self.setup_3d_axes(ax_main, (-2, 2), (-2, 2), (0, 3))
        self.draw_coordinate_axes(ax_main, length=1.8)
        
        # 绘制抛物面 z = x² + y²
        x = np.linspace(-2, 2, 50)
        y = np.linspace(-2, 2, 50)
        X, Y = np.meshgrid(x, y)
        Z = X**2 + Y**2
        
        # 限制z的范围
        mask = Z <= 2.5
        Z[~mask] = np.nan
        
        ax_main.plot_surface(X, Y, Z, alpha=0.3, color=self.colors['region_main'])
        ax_main.contour(X, Y, Z, levels=10, colors=self.colors['boundary_main'], alpha=0.6)
        ax_main.set_title('抛物面: $z = x^2 + y^2$ (截面法)', 
                         fontsize=14, pad=20, color=self.colors['text_main'])
        
        # 绘制不同z值的截面
        z_values = [0.5, 1.0, 1.5, 2.0]
        slice_colors = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#96CEB4']
        
        for i, (z_val, ax_slice, color) in enumerate(zip(z_values, slice_axes, slice_colors)):
            # 在3D图中绘制截面线
            theta = np.linspace(0, 2*np.pi, 100)
            r = np.sqrt(z_val)
            x_circle = r * np.cos(theta)
            y_circle = r * np.sin(theta)
            z_circle = np.full_like(x_circle, z_val)
            
            ax_main.plot(x_circle, y_circle, z_circle, 
                        color=color, linewidth=3, alpha=0.8)
            
            # 在2D截面图中绘制圆
            circle = Circle((0, 0), r, fill=True, alpha=0.4, 
                           color=color, edgecolor='black', linewidth=2)
            ax_slice.add_patch(circle)
            ax_slice.set_xlim(-2, 2)
            ax_slice.set_ylim(-2, 2)
            ax_slice.set_aspect('equal')
            ax_slice.grid(True, alpha=0.3)
            ax_slice.set_xlabel('x', fontsize=10)
            ax_slice.set_ylabel('y', fontsize=10)
            ax_slice.set_title(f'z = {z_val}: $x^2+y^2 \\leq {z_val}$', 
                              fontsize=11, color=self.colors['text_main'])
            
    def _draw_ellipsoid_slices(self, ax_main, slice_axes):
        """绘制椭球面的截面法"""
        self.setup_3d_axes(ax_main, (-2, 2), (-2, 2), (-1.5, 1.5))
        self.draw_coordinate_axes(ax_main, length=1.8)
        
        # 绘制椭球面 x²/4 + y²/1 + z²/1 = 1
        u = np.linspace(0, 2 * np.pi, 50)
        v = np.linspace(0, np.pi, 50)
        a, b, c = 2, 1, 1
        x = a * np.outer(np.cos(u), np.sin(v))
        y = b * np.outer(np.sin(u), np.sin(v))
        z = c * np.outer(np.ones(np.size(u)), np.cos(v))
        
        ax_main.plot_surface(x, y, z, alpha=0.3, color=self.colors['region_main'])
        ax_main.plot_wireframe(x, y, z, alpha=0.5, color=self.colors['boundary_main'], linewidth=0.5)
        ax_main.set_title('椭球面: $\\frac{x^2}{4} + y^2 + z^2 = 1$ (截面法)', 
                         fontsize=14, pad=20, color=self.colors['text_main'])
        
        # 不同z值的椭圆截面
        z_values = [-0.5, 0, 0.5, 0.8]
        slice_colors = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#96CEB4']
        
        for z_val, ax_slice, color in zip(z_values, slice_axes, slice_colors):
            if abs(z_val) < 1:  # 椭球内部
                # 椭圆参数
                a_slice = 2 * np.sqrt(1 - z_val**2)
                b_slice = np.sqrt(1 - z_val**2)
                
                # 绘制椭圆
                theta = np.linspace(0, 2*np.pi, 100)
                x_ellipse = a_slice * np.cos(theta)
                y_ellipse = b_slice * np.sin(theta)
                
                ax_slice.plot(x_ellipse, y_ellipse, color=color, linewidth=2)
                ax_slice.fill(x_ellipse, y_ellipse, alpha=0.4, color=color)
                
            ax_slice.set_xlim(-2.5, 2.5)
            ax_slice.set_ylim(-1.5, 1.5)
            ax_slice.set_aspect('equal')
            ax_slice.grid(True, alpha=0.3)
            ax_slice.set_xlabel('x', fontsize=10)
            ax_slice.set_ylabel('y', fontsize=10)
            ax_slice.set_title(f'z = {z_val}', fontsize=11, color=self.colors['text_main'])
            
    def _add_slice_annotations(self, fig):
        """添加截面法说明文字"""
        fig.suptitle('三重积分截面法计算过程可视化', 
                    fontsize=18, y=0.95, color=self.colors['text_main'])
        
        formula_text = (
            '截面法基本思想:\n'
            '$\\iiint_\\Omega f(x,y,z) dV = \\int_{z_1}^{z_2} dz \\iint_{D_z} f(x,y,z) dx dy$\n\n'
            '其中 $D_z$ 是立体在 $z=$ 常数平面上的截面\n\n'
            '计算步骤:\n'
            '1. 确定z的变化范围\n'
            '2. 对每个z值，确定截面区域\n'
            '3. 建立累次积分'
        )
        
        fig.text(0.02, 0.3, formula_text, fontsize=11, 
                color=self.colors['text_formula'], 
                verticalalignment='center',
                bbox=dict(boxstyle='round,pad=0.5', 
                         facecolor='white', 
                         edgecolor=self.colors['boundary_main'],
                         alpha=0.9))
                         
    def draw_coordinate_comparison(self):
        """绘制不同坐标系下的体积元素对比"""
        fig = plt.figure(figsize=(18, 12))
        
        # 直角坐标系
        ax1 = fig.add_subplot(2, 3, 1, projection='3d')
        self._draw_cartesian_volume_element(ax1)
        
        # 柱坐标系
        ax2 = fig.add_subplot(2, 3, 2, projection='3d')
        self._draw_cylindrical_volume_element(ax2)
        
        # 球坐标系
        ax3 = fig.add_subplot(2, 3, 3, projection='3d')
        self._draw_spherical_volume_element(ax3)
        
        # 坐标变换说明
        ax_info = fig.add_subplot(2, 1, 2)
        self._add_coordinate_transformation_info(ax_info)
        
        fig.suptitle('三重积分：不同坐标系体积元素对比', 
                    fontsize=18, y=0.95, color=self.colors['text_main'])
        
        return fig
        
    def _draw_cartesian_volume_element(self, ax):
        """绘制直角坐标系体积元素"""
        self.setup_3d_axes(ax, (0, 3), (0, 3), (0, 3))
        
        # 绘制立方体体积元素
        dx, dy, dz = 0.8, 0.6, 0.7
        x0, y0, z0 = 1.2, 1.2, 1.2
        
        # 立方体的8个顶点
        vertices = [
            [x0, y0, z0], [x0+dx, y0, z0], [x0+dx, y0+dy, z0], [x0, y0+dy, z0],
            [x0, y0, z0+dz], [x0+dx, y0, z0+dz], [x0+dx, y0+dy, z0+dz], [x0, y0+dy, z0+dz]
        ]
        
        # 立方体的6个面
        faces = [
            [vertices[0], vertices[1], vertices[2], vertices[3]],  # 底面
            [vertices[4], vertices[5], vertices[6], vertices[7]],  # 顶面
            [vertices[0], vertices[1], vertices[5], vertices[4]],  # 前面
            [vertices[2], vertices[3], vertices[7], vertices[6]],  # 后面
            [vertices[1], vertices[2], vertices[6], vertices[5]],  # 右面
            [vertices[0], vertices[3], vertices[7], vertices[4]]   # 左面
        ]
        
        poly3d = Poly3DCollection(faces, alpha=0.7, facecolor=self.colors['cartesian'],
                                 edgecolor='black', linewidth=1.5)
        ax.add_collection3d(poly3d)
        
        ax.set_title('直角坐标系\n$dV = dx\\,dy\\,dz$', 
                    fontsize=12, color=self.colors['text_main'])
        
        # 标注尺寸
        ax.text(x0+dx/2, y0-0.2, z0-0.2, 'dx', fontsize=10, color='red')
        ax.text(x0-0.2, y0+dy/2, z0-0.2, 'dy', fontsize=10, color='red')
        ax.text(x0-0.2, y0-0.2, z0+dz/2, 'dz', fontsize=10, color='red')
        
    def _draw_cylindrical_volume_element(self, ax):
        """绘制柱坐标系体积元素"""
        self.setup_3d_axes(ax, (-2, 2), (-2, 2), (0, 3))
        
        # 绘制柱坐标体积元素（扇形柱体）
        r, dr, dtheta, dz = 1.5, 0.3, np.pi/6, 0.5
        theta0, z0 = np.pi/4, 1.0
        
        # 内外圆弧
        theta_range = np.linspace(theta0, theta0 + dtheta, 20)
        
        # 底面扇形
        x_inner = r * np.cos(theta_range)
        y_inner = r * np.sin(theta_range)
        x_outer = (r + dr) * np.cos(theta_range)
        y_outer = (r + dr) * np.sin(theta_range)
        
        # 绘制扇形底面
        for i in range(len(theta_range)):
            x_quad = [x_inner[i], x_outer[i], x_outer[i], x_inner[i]]
            y_quad = [y_inner[i], y_outer[i], y_outer[i], y_inner[i]]
            z_quad = [z0, z0, z0+dz, z0+dz]
            
            if i < len(theta_range)-1:
                x_quad_next = [x_inner[i+1], x_outer[i+1], x_outer[i+1], x_inner[i+1]]
                y_quad_next = [y_inner[i+1], y_outer[i+1], y_outer[i+1], y_inner[i+1]]
                
                # 侧面
                vertices = [
                    [x_quad[0], y_quad[0], z_quad[0]], 
                    [x_quad[1], y_quad[1], z_quad[1]],
                    [x_quad_next[1], y_quad_next[1], z_quad[1]], 
                    [x_quad_next[0], y_quad_next[0], z_quad[0]]
                ]
                
                poly = [[tuple(v) for v in vertices]]
                poly3d = Poly3DCollection(poly, alpha=0.7, facecolor=self.colors['cylindrical'],
                                        edgecolor='black', linewidth=1)
                ax.add_collection3d(poly3d)
        
        ax.set_title('柱坐标系\n$dV = r\\,dr\\,d\\theta\\,dz$', 
                    fontsize=12, color=self.colors['text_main'])
        
    def _draw_spherical_volume_element(self, ax):
        """绘制球坐标系体积元素"""
        self.setup_3d_axes(ax, (-2, 2), (-2, 2), (-2, 2))
        
        # 绘制球坐标体积元素（球面扇形）
        r, dr = 1.5, 0.2
        theta, dtheta = np.pi/4, np.pi/8  # 方位角
        phi, dphi = np.pi/3, np.pi/8      # 极角
        
        # 生成球面元素的顶点
        r_vals = [r, r + dr]
        theta_vals = [theta, theta + dtheta]
        phi_vals = [phi, phi + dphi]
        
        vertices = []
        for r_val in r_vals:
            for theta_val in theta_vals:
                for phi_val in phi_vals:
                    x = r_val * np.sin(phi_val) * np.cos(theta_val)
                    y = r_val * np.sin(phi_val) * np.sin(theta_val)
                    z = r_val * np.cos(phi_val)
                    vertices.append([x, y, z])
        
        # 简化显示：绘制部分面
        faces = [
            [vertices[0], vertices[1], vertices[3], vertices[2]],  # 内表面
            [vertices[4], vertices[5], vertices[7], vertices[6]]   # 外表面
        ]
        
        poly3d = Poly3DCollection(faces, alpha=0.7, facecolor=self.colors['spherical'],
                                 edgecolor='black', linewidth=1)
        ax.add_collection3d(poly3d)
        
        ax.set_title('球坐标系\n$dV = r^2\\sin\\phi\\,dr\\,d\\theta\\,d\\phi$', 
                    fontsize=12, color=self.colors['text_main'])
                    
    def _add_coordinate_transformation_info(self, ax):
        """添加坐标变换信息"""
        ax.axis('off')
        
        info_text = """
坐标变换关系与体积元素：

1. 直角坐标系 → 柱坐标系:
   x = r cos θ,  y = r sin θ,  z = z
   体积元素: dV = r dr dθ dz

2. 直角坐标系 → 球坐标系:
   x = r sin φ cos θ,  y = r sin φ sin θ,  z = r cos φ
   体积元素: dV = r² sin φ dr dθ dφ

3. 选择坐标系的原则:
   • 球体、球面 → 球坐标系
   • 圆柱体、圆锥 → 柱坐标系  
   • 立方体、长方体 → 直角坐标系

4. 积分限的确定:
   • 先确定几何区域
   • 选择合适的坐标系
   • 按照积分次序确定积分限
        """
        
        ax.text(0.05, 0.95, info_text, transform=ax.transAxes,
               fontsize=12, verticalalignment='top',
               color=self.colors['text_main'],
               bbox=dict(boxstyle='round,pad=0.8', 
                        facecolor='white', 
                        edgecolor=self.colors['boundary_main'],
                        alpha=0.9))

    def draw_complex_regions(self):
        """绘制复杂三重积分区域集合"""
        fig = plt.figure(figsize=(20, 16))
        
        # 2x3布局显示6个不同的复杂区域
        regions = [
            ('sphere_cylinder', '球-圆柱相交'),
            ('paraboloid_bounded', '抛物面围成区域'),
            ('cone_plane', '圆锥-平面区域'),
            ('ellipsoid_section', '椭球截面'),
            ('torus_section', '环面截面'),
            ('complex_polyhedron', '复杂多面体')
        ]
        
        for i, (region_type, title) in enumerate(regions):
            ax = fig.add_subplot(2, 3, i+1, projection='3d')
            if region_type == 'sphere_cylinder':
                self._draw_sphere_cylinder_intersection(ax)
            elif region_type == 'paraboloid_bounded':
                self._draw_bounded_paraboloid_region(ax)
            elif region_type == 'cone_plane':
                self._draw_cone_plane_region(ax)
            elif region_type == 'ellipsoid_section':
                self._draw_ellipsoid_section(ax)
            elif region_type == 'torus_section':
                self._draw_torus_section(ax)
            elif region_type == 'complex_polyhedron':
                self._draw_complex_polyhedron(ax)
            
            ax.set_title(title, fontsize=12, color=self.colors['text_main'], pad=10)
            
        fig.suptitle('三重积分复杂区域立体图像集合', 
                    fontsize=16, y=0.95, color=self.colors['text_main'])
        
        return fig
        
    def _draw_sphere_cylinder_intersection(self, ax):
        """绘制球-圆柱相交区域"""
        self.setup_3d_axes(ax, (-2.5, 2.5), (-2.5, 2.5), (-2.5, 2.5))
        
        # 绘制球面 x² + y² + z² = 4
        u = np.linspace(0, 2 * np.pi, 50)
        v = np.linspace(0, np.pi, 50)
        r_sphere = 2
        x_sphere = r_sphere * np.outer(np.cos(u), np.sin(v))
        y_sphere = r_sphere * np.outer(np.sin(u), np.sin(v))
        z_sphere = r_sphere * np.outer(np.ones(np.size(u)), np.cos(v))
        
        ax.plot_surface(x_sphere, y_sphere, z_sphere, alpha=0.2, 
                       color=self.colors['region_main'], linewidth=0)
        
        # 绘制圆柱面 x² + y² = 1
        theta = np.linspace(0, 2*np.pi, 50)
        z_cyl = np.linspace(-2, 2, 50)
        r_cylinder = 1
        Theta, Z_cyl = np.meshgrid(theta, z_cyl)
        X_cyl = r_cylinder * np.cos(Theta)
        Y_cyl = r_cylinder * np.sin(Theta)
        
        ax.plot_surface(X_cyl, Y_cyl, Z_cyl, alpha=0.3, 
                       color=self.colors['region_projection'], linewidth=0)
        
        # 突出显示相交区域 - 圆柱内部的球面部分
        mask = X_cyl**2 + Y_cyl**2 <= 1
        z_sphere_cyl = np.sqrt(4 - X_cyl**2 - Y_cyl**2)
        z_sphere_cyl[X_cyl**2 + Y_cyl**2 > 1] = np.nan
        
        ax.plot_surface(X_cyl, Y_cyl, z_sphere_cyl, alpha=0.6, 
                       color=self.colors['boundary_main'], linewidth=0)
        ax.plot_surface(X_cyl, Y_cyl, -z_sphere_cyl, alpha=0.6, 
                       color=self.colors['boundary_main'], linewidth=0)
        
        # 添加标注
        ax.text(0, 0, 2.3, '$x^2+y^2+z^2=4$', fontsize=9, color=self.colors['text_annotation'])
        ax.text(1.2, 0, 0, '$x^2+y^2=1$', fontsize=9, color=self.colors['text_annotation'])
        
    def _draw_bounded_paraboloid_region(self, ax):
        """绘制抛物面围成区域"""
        self.setup_3d_axes(ax, (-3, 3), (-3, 3), (0, 8))
        
        x = np.linspace(-2.5, 2.5, 50)
        y = np.linspace(-2.5, 2.5, 50)
        X, Y = np.meshgrid(x, y)
        
        # 下抛物面：z = x² + y²
        Z1 = X**2 + Y**2
        mask1 = X**2 + Y**2 <= 4  # 限制在圆形区域内
        Z1[~mask1] = np.nan
        
        # 上抛物面：z = 8 - x² - y²
        Z2 = 8 - X**2 - Y**2
        mask2 = (X**2 + Y**2 <= 4) & (Z2 >= Z1)  # 确保上面在下面之上
        Z2[~mask2] = np.nan
        
        ax.plot_surface(X, Y, Z1, alpha=0.4, color=self.colors['region_main'])
        ax.plot_surface(X, Y, Z2, alpha=0.4, color=self.colors['region_section'])
        
        # 绘制交线
        theta = np.linspace(0, 2*np.pi, 100)
        r_intersection = 2
        x_int = r_intersection * np.cos(theta)
        y_int = r_intersection * np.sin(theta)
        z_int = np.full_like(x_int, 4)  # z = 4 在交线处
        
        ax.plot(x_int, y_int, z_int, color=self.colors['boundary_projection'], 
               linewidth=3, alpha=0.8)
        
        # 添加标注
        ax.text(0, 0, 1, '$z = x^2 + y^2$', fontsize=9, color=self.colors['text_annotation'])
        ax.text(0, 0, 7, '$z = 8 - x^2 - y^2$', fontsize=9, color=self.colors['text_annotation'])
        
    def _draw_cone_plane_region(self, ax):
        """绘制圆锥-平面截取区域"""
        self.setup_3d_axes(ax, (-2, 2), (-2, 2), (0, 3))
        
        # 圆锥：z = sqrt(x² + y²)
        theta = np.linspace(0, 2*np.pi, 50)
        r = np.linspace(0, 2, 30)
        Theta, R = np.meshgrid(theta, r)
        X_cone = R * np.cos(Theta)
        Y_cone = R * np.sin(Theta)
        Z_cone = R
        
        # 限制高度
        mask_cone = Z_cone <= 2.5
        X_cone[~mask_cone] = np.nan
        Y_cone[~mask_cone] = np.nan
        Z_cone[~mask_cone] = np.nan
        
        ax.plot_surface(X_cone, Y_cone, Z_cone, alpha=0.4, 
                       color=self.colors['region_main'])
        
        # 平面：z = 2
        x_plane = np.linspace(-2, 2, 20)
        y_plane = np.linspace(-2, 2, 20)
        X_plane, Y_plane = np.meshgrid(x_plane, y_plane)
        Z_plane = np.full_like(X_plane, 2)
        
        # 只显示圆锥内部的平面部分
        mask_plane = X_plane**2 + Y_plane**2 <= 4
        Z_plane[~mask_plane] = np.nan
        
        ax.plot_surface(X_plane, Y_plane, Z_plane, alpha=0.4, 
                       color=self.colors['region_projection'])
        
        # 交线：圆 x² + y² = 4, z = 2
        theta = np.linspace(0, 2*np.pi, 100)
        x_circle = 2 * np.cos(theta)
        y_circle = 2 * np.sin(theta)
        z_circle = np.full_like(x_circle, 2)
        
        ax.plot(x_circle, y_circle, z_circle, 
               color=self.colors['boundary_projection'], linewidth=3)
        
        # 添加标注
        ax.text(0, 0, 0.5, '$z = \\sqrt{x^2 + y^2}$', fontsize=9, 
               color=self.colors['text_annotation'])
        ax.text(1.5, 1.5, 2.1, '$z = 2$', fontsize=9, 
               color=self.colors['text_annotation'])
               
    def _draw_ellipsoid_section(self, ax):
        """绘制椭球截面"""
        self.setup_3d_axes(ax, (-3, 3), (-2, 2), (-1.5, 1.5))
        
        # 椭球：x²/4 + y²/1 + z²/1 = 1
        u = np.linspace(0, 2 * np.pi, 50)
        v = np.linspace(0, np.pi, 50)
        a, b, c = 2, 1, 1
        x_ellipsoid = a * np.outer(np.cos(u), np.sin(v))
        y_ellipsoid = b * np.outer(np.sin(u), np.sin(v))
        z_ellipsoid = c * np.outer(np.ones(np.size(u)), np.cos(v))
        
        ax.plot_surface(x_ellipsoid, y_ellipsoid, z_ellipsoid, alpha=0.3, 
                       color=self.colors['region_main'])
        
        # 截面：z = 0.5
        x_section = np.linspace(-2, 2, 50)
        y_section = np.linspace(-1, 1, 50)
        X_sec, Y_sec = np.meshgrid(x_section, y_section)
        
        # 椭圆截面：x²/4 + y² ≤ 1 - 0.25 = 0.75
        mask_section = (X_sec**2 / 4 + Y_sec**2) <= 0.75
        Z_sec = np.full_like(X_sec, 0.5)
        Z_sec[~mask_section] = np.nan
        
        ax.plot_surface(X_sec, Y_sec, Z_sec, alpha=0.6, 
                       color=self.colors['region_section'])
        
        # 截面边界
        theta = np.linspace(0, 2*np.pi, 100)
        a_sec = 2 * np.sqrt(0.75)  # a * sqrt(0.75)
        b_sec = np.sqrt(0.75)      # b * sqrt(0.75)
        x_boundary = a_sec * np.cos(theta)
        y_boundary = b_sec * np.sin(theta)
        z_boundary = np.full_like(x_boundary, 0.5)
        
        ax.plot(x_boundary, y_boundary, z_boundary, 
               color=self.colors['boundary_section'], linewidth=3)
               
    def _draw_torus_section(self, ax):
        """绘制环面截面"""
        self.setup_3d_axes(ax, (-3, 3), (-3, 3), (-1.5, 1.5))
        
        # 环面参数
        R = 2  # 大半径
        r = 0.8  # 小半径
        
        u = np.linspace(0, 2*np.pi, 30)
        v = np.linspace(0, 2*np.pi, 30)
        U, V = np.meshgrid(u, v)
        
        x_torus = (R + r * np.cos(V)) * np.cos(U)
        y_torus = (R + r * np.cos(V)) * np.sin(U)
        z_torus = r * np.sin(V)
        
        ax.plot_surface(x_torus, y_torus, z_torus, alpha=0.3, 
                       color=self.colors['region_main'])
        
        # z = 0 截面
        theta = np.linspace(0, 2*np.pi, 100)
        
        # 外圆
        x_outer = (R + r) * np.cos(theta)
        y_outer = (R + r) * np.sin(theta)
        z_outer = np.zeros_like(x_outer)
        
        # 内圆
        x_inner = (R - r) * np.cos(theta)
        y_inner = (R - r) * np.sin(theta)
        z_inner = np.zeros_like(x_inner)
        
        ax.plot(x_outer, y_outer, z_outer, color=self.colors['boundary_section'], linewidth=3)
        ax.plot(x_inner, y_inner, z_inner, color=self.colors['boundary_section'], linewidth=3)
        
    def _draw_complex_polyhedron(self, ax):
        """绘制复杂多面体"""
        self.setup_3d_axes(ax, (-1, 2), (-1, 2), (-1, 2))
        
        # 定义一个复杂多面体的顶点
        vertices = np.array([
            [0, 0, 0], [1, 0, 0], [1, 1, 0], [0, 1, 0],  # 底面正方形
            [0.5, 0.5, 1.5],  # 顶点
            [1.5, 0.5, 0.5], [0.5, 1.5, 0.5]  # 侧面顶点
        ])
        
        # 定义面
        faces = [
            [vertices[0], vertices[1], vertices[2], vertices[3]],  # 底面
            [vertices[0], vertices[1], vertices[4]],  # 侧面三角形
            [vertices[1], vertices[2], vertices[4]],
            [vertices[2], vertices[3], vertices[4]],
            [vertices[3], vertices[0], vertices[4]],
            [vertices[1], vertices[5], vertices[2]],  # 额外面
            [vertices[2], vertices[6], vertices[3]]
        ]
        
        colors = [self.colors['region_main'], self.colors['region_section'], 
                 self.colors['region_projection']] * 3
        
        for i, face in enumerate(faces):
            if len(face) == 4:  # 四边形
                poly = [[tuple(vertex) for vertex in face]]
            else:  # 三角形
                poly = [[tuple(vertex) for vertex in face]]
            
            poly3d = Poly3DCollection(poly, alpha=0.6, 
                                     facecolor=colors[i % len(colors)],
                                     edgecolor='black', linewidth=1)
            ax.add_collection3d(poly3d)
            
    def draw_integration_regions_comparison(self):
        """绘制不同积分区域的对比图"""
        fig = plt.figure(figsize=(20, 12))
        
        # 第一行：基础区域
        basic_regions = [
            ('sphere', '球体'),
            ('cylinder', '圆柱体'),
            ('cone', '圆锥'),
            ('ellipsoid', '椭球体')
        ]
        
        for i, (region_type, title) in enumerate(basic_regions):
            ax = fig.add_subplot(2, 4, i+1, projection='3d')
            if region_type == 'sphere':
                self._draw_basic_sphere(ax)
            elif region_type == 'cylinder':
                self._draw_basic_cylinder(ax)
            elif region_type == 'cone':
                self._draw_basic_cone(ax)
            elif region_type == 'ellipsoid':
                self._draw_basic_ellipsoid(ax)
            
            ax.set_title(f'{title}\n(基础区域)', fontsize=10, 
                        color=self.colors['text_main'])
            
        # 第二行：组合区域
        combined_regions = [
            ('sphere_plane', '球-平面'),
            ('cylinder_cone', '柱-锥'),
            ('paraboloids', '双抛物面'),
            ('revolution', '旋转体')
        ]
        
        for i, (region_type, title) in enumerate(combined_regions):
            ax = fig.add_subplot(2, 4, i+5, projection='3d')
            if region_type == 'sphere_plane':
                self._draw_sphere_plane_region(ax)
            elif region_type == 'cylinder_cone':
                self._draw_cylinder_cone_region(ax)
            elif region_type == 'paraboloids':
                self._draw_double_paraboloid(ax)
            elif region_type == 'revolution':
                self._draw_revolution_body(ax)
                
            ax.set_title(f'{title}\n(组合区域)', fontsize=10, 
                        color=self.colors['text_main'])
        
        fig.suptitle('三重积分常见区域类型对比', 
                    fontsize=16, y=0.95, color=self.colors['text_main'])
        
        return fig
        
    def _draw_basic_sphere(self, ax):
        """绘制基础球体"""
        self.setup_3d_axes(ax, (-1.5, 1.5), (-1.5, 1.5), (-1.5, 1.5))
        
        u = np.linspace(0, 2 * np.pi, 30)
        v = np.linspace(0, np.pi, 30)
        x = np.outer(np.cos(u), np.sin(v))
        y = np.outer(np.sin(u), np.sin(v))
        z = np.outer(np.ones(np.size(u)), np.cos(v))
        
        ax.plot_surface(x, y, z, alpha=0.4, color=self.colors['spherical'])
        
    def _draw_basic_cylinder(self, ax):
        """绘制基础圆柱体"""
        self.setup_3d_axes(ax, (-1.5, 1.5), (-1.5, 1.5), (0, 2))
        
        theta = np.linspace(0, 2*np.pi, 30)
        z = np.linspace(0, 2, 20)
        Theta, Z = np.meshgrid(theta, z)
        X = np.cos(Theta)
        Y = np.sin(Theta)
        
        ax.plot_surface(X, Y, Z, alpha=0.4, color=self.colors['cylindrical'])
        
        # 上下底面
        theta_cap = np.linspace(0, 2*np.pi, 30)
        r_cap = np.linspace(0, 1, 10)
        Theta_cap, R_cap = np.meshgrid(theta_cap, r_cap)
        X_cap = R_cap * np.cos(Theta_cap)
        Y_cap = R_cap * np.sin(Theta_cap)
        
        ax.plot_surface(X_cap, Y_cap, np.zeros_like(X_cap), alpha=0.4, 
                       color=self.colors['cylindrical'])
        ax.plot_surface(X_cap, Y_cap, 2*np.ones_like(X_cap), alpha=0.4, 
                       color=self.colors['cylindrical'])
        
    def _draw_basic_cone(self, ax):
        """绘制基础圆锥"""
        self.setup_3d_axes(ax, (-1.5, 1.5), (-1.5, 1.5), (0, 2))
        
        theta = np.linspace(0, 2*np.pi, 30)
        r = np.linspace(0, 1, 20)
        Theta, R = np.meshgrid(theta, r)
        X = R * np.cos(Theta)
        Y = R * np.sin(Theta)
        Z = 2 * R  # 高度与半径成比例
        
        ax.plot_surface(X, Y, Z, alpha=0.4, color=self.colors['cartesian'])
        
    def _draw_basic_ellipsoid(self, ax):
        """绘制基础椭球体"""
        self.setup_3d_axes(ax, (-2, 2), (-1.5, 1.5), (-1, 1))
        
        u = np.linspace(0, 2 * np.pi, 30)
        v = np.linspace(0, np.pi, 30)
        a, b, c = 1.5, 1, 0.8
        x = a * np.outer(np.cos(u), np.sin(v))
        y = b * np.outer(np.sin(u), np.sin(v))
        z = c * np.outer(np.ones(np.size(u)), np.cos(v))
        
        ax.plot_surface(x, y, z, alpha=0.4, color=self.colors['region_main'])
        
    def _draw_sphere_plane_region(self, ax):
        """绘制球-平面截取区域"""
        self.setup_3d_axes(ax, (-1.5, 1.5), (-1.5, 1.5), (-1.5, 1.5))
        
        # 球面
        u = np.linspace(0, 2 * np.pi, 30)
        v = np.linspace(0, np.pi, 30)
        x = np.outer(np.cos(u), np.sin(v))
        y = np.outer(np.sin(u), np.sin(v))
        z = np.outer(np.ones(np.size(u)), np.cos(v))
        
        # 只显示z >= 0的部分
        mask = z >= 0
        ax.plot_surface(x, y, z, alpha=0.4, color=self.colors['spherical'])
        
        # z = 0平面（截面）
        x_plane = np.linspace(-1, 1, 20)
        y_plane = np.linspace(-1, 1, 20)
        X_plane, Y_plane = np.meshgrid(x_plane, y_plane)
        Z_plane = np.zeros_like(X_plane)
        
        mask_plane = X_plane**2 + Y_plane**2 <= 1
        Z_plane[~mask_plane] = np.nan
        
        ax.plot_surface(X_plane, Y_plane, Z_plane, alpha=0.5, 
                       color=self.colors['region_projection'])
                       
    def _draw_cylinder_cone_region(self, ax):
        """绘制圆柱-圆锥组合区域"""
        self.setup_3d_axes(ax, (-1.5, 1.5), (-1.5, 1.5), (0, 2))
        
        # 圆柱面
        theta = np.linspace(0, 2*np.pi, 30)
        z = np.linspace(0, 1, 20)
        Theta, Z = np.meshgrid(theta, z)
        X_cyl = 0.8 * np.cos(Theta)
        Y_cyl = 0.8 * np.sin(Theta)
        
        ax.plot_surface(X_cyl, Y_cyl, Z, alpha=0.3, color=self.colors['cylindrical'])
        
        # 圆锥面
        r = np.linspace(0, 0.8, 20)
        z_cone = np.linspace(1, 1.8, 20)
        R, Z_cone = np.meshgrid(r, z_cone)
        Theta_cone = np.linspace(0, 2*np.pi, 30)
        
        for i, z_val in enumerate(z_cone):
            r_val = 0.8 * (1.8 - z_val) / 0.8  # 圆锥半径随高度变化
            x_cone = r_val * np.cos(Theta_cone)
            y_cone = r_val * np.sin(Theta_cone)
            z_cone_layer = np.full_like(x_cone, z_val)
            
            if i % 3 == 0:  # 只绘制部分层以避免过于密集
                ax.plot(x_cone, y_cone, z_cone_layer, alpha=0.5, 
                       color=self.colors['cartesian'])
                
    def _draw_double_paraboloid(self, ax):
        """绘制双抛物面区域"""
        self.setup_3d_axes(ax, (-2, 2), (-2, 2), (0, 4))
        
        x = np.linspace(-1.5, 1.5, 30)
        y = np.linspace(-1.5, 1.5, 30)
        X, Y = np.meshgrid(x, y)
        
        # 下抛物面
        Z1 = 0.5 * (X**2 + Y**2)
        mask1 = X**2 + Y**2 <= 2
        Z1[~mask1] = np.nan
        
        # 上抛物面
        Z2 = 3 - 0.5 * (X**2 + Y**2)
        mask2 = (X**2 + Y**2 <= 2) & (Z2 >= Z1)
        Z2[~mask2] = np.nan
        
        ax.plot_surface(X, Y, Z1, alpha=0.4, color=self.colors['region_main'])
        ax.plot_surface(X, Y, Z2, alpha=0.4, color=self.colors['region_section'])
        
    def _draw_revolution_body(self, ax):
        """绘制旋转体"""
        self.setup_3d_axes(ax, (-1.5, 1.5), (-1.5, 1.5), (0, 2))
        
        # 由曲线 y = sin(z) 绕z轴旋转生成的旋转体
        z = np.linspace(0, 2, 30)
        theta = np.linspace(0, 2*np.pi, 30)
        Z, Theta = np.meshgrid(z, theta)
        
        R = 0.5 + 0.3 * np.sin(2*Z)  # 半径随z变化
        X = R * np.cos(Theta)
        Y = R * np.sin(Theta)
        
        ax.plot_surface(X, Y, Z, alpha=0.4, color=self.colors['region_volume'])

def demo_triple_integral_visualizations():
    """演示三重积分可视化功能"""
    visualizer = TripleIntegral3DVisualizer()
    
    print("正在生成三重积分立体图像...")
    
    # 1. 投影法过程 - 球体
    fig1 = visualizer.draw_projection_process('sphere')
    visualizer.save_figure(fig1, 'triple_integral_projection_sphere.png')
    plt.close(fig1)
    
    # 2. 投影法过程 - 四面体
    fig2 = visualizer.draw_projection_process('tetrahedron')
    visualizer.save_figure(fig2, 'triple_integral_projection_tetrahedron.png')
    plt.close(fig2)
    
    # 3. 截面法过程 - 抛物面
    fig3 = visualizer.draw_slice_method('paraboloid')
    visualizer.save_figure(fig3, 'triple_integral_slice_paraboloid.png')
    plt.close(fig3)
    
    # 4. 截面法过程 - 椭球
    fig4 = visualizer.draw_slice_method('ellipsoid')
    visualizer.save_figure(fig4, 'triple_integral_slice_ellipsoid.png')
    plt.close(fig4)
    
    # 5. 坐标系对比
    fig5 = visualizer.draw_coordinate_comparison()
    visualizer.save_figure(fig5, 'triple_integral_coordinate_comparison.png')
    plt.close(fig5)
    
    # 6. 复杂区域集合
    fig6 = visualizer.draw_complex_regions()
    visualizer.save_figure(fig6, 'triple_integral_complex_regions.png')
    plt.close(fig6)
    
    # 7. 积分区域对比
    fig7 = visualizer.draw_integration_regions_comparison()
    visualizer.save_figure(fig7, 'triple_integral_regions_comparison.png')
    plt.close(fig7)
    
    print("三重积分立体图像生成完成!")

if __name__ == "__main__":
    demo_triple_integral_visualizations()
