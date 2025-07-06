#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
三重积分可视化系统V2.0 - 优雅柔和版
- 基于极坐标作图规范的高标准
- 完美中文字体支持
- 优雅柔和的配色方案
- 简化体积分割显示
- 强化细节与美观度

作者：数学课程图像标准化项目组
版本：2.0 Elegant
日期：2024
"""

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.patches as patches
import warnings
warnings.filterwarnings('ignore')

class TripleIntegralVisualizerV2Elegant:
    """三重积分可视化系统V2.0 - 优雅版"""
    
    def __init__(self):
        self.setup_matplotlib()
        self.setup_elegant_colors()
        self.setup_fonts()
        
    def setup_matplotlib(self):
        """设置matplotlib基础配置 - 完全参考region_complex_generator.py"""
        # 中文字体配置 - 直接复制region_complex_generator.py的配置
        plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
        plt.rcParams['axes.unicode_minus'] = False
        plt.rcParams['figure.dpi'] = 120
        plt.rcParams['savefig.dpi'] = 300
        
        # 额外的配置
        plt.rcParams['figure.facecolor'] = 'white'
        plt.rcParams['savefig.bbox'] = 'tight'
        plt.rcParams['savefig.facecolor'] = 'white'
        plt.rcParams['savefig.pad_inches'] = 0.1
        
        print("✓ 中文字体配置完成（使用region_complex_generator.py的配置）")
        
    def setup_elegant_colors(self):
        """设置优雅柔和的配色方案"""
        self.colors = {
            # 主要颜色 - 优雅柔和
            'region_main': '#F8F4F0',        # 温暖米白 - 主积分区域
            'region_secondary': '#F0F8FF',   # 淡雪蓝 - 辅助区域
            'region_highlight': '#FFF8E7',   # 淡香草 - 高亮区域
            
            # 边界颜色 - 温和深色
            'boundary_main': '#5D7AAE',      # 温和蓝 - 主边界
            'boundary_aux': '#A67B5B',       # 温和棕 - 辅助边界
            'boundary_highlight': '#6B8E6B', # 温和绿 - 高亮边界
            
            # 3D曲面颜色 - 柔和渐变
            'surface_main': '#E8DDD4',       # 柔和米色 - 主曲面
            'surface_aux': '#D4E8E8',        # 柔和青色 - 辅助曲面
            'surface_highlight': '#E8E8D4',  # 柔和黄色 - 特殊曲面
            
            # 网格和轴 - 低对比度
            'grid_major': '#E5E5E5',         # 浅灰 - 主网格
            'grid_minor': '#F0F0F0',         # 更浅灰 - 次网格
            'axis_main': '#666666',          # 中灰 - 坐标轴
            
            # 文字颜色 - 温和深色
            'text_main': '#4A4A4A',          # 温和深灰 - 主文字
            'text_formula': '#5A6B7D',       # 温和蓝灰 - 公式文字
            'text_highlight': '#8B5A3C',     # 温和棕色 - 高亮文字
            
            # 体积元素颜色 - 典型元素突出
            'volume_element': '#C8A882',     # 温和金棕 - 体积元素
            'volume_edge': '#8B7355',        # 深金棕 - 体积元素边界
        }
        
    def setup_fonts(self):
        """测试并验证中文字体支持"""
        import matplotlib.font_manager as fm
        
        # 获取当前字体列表
        current_font = plt.rcParams['font.sans-serif'][0]
        
        # 测试中文字体
        available_fonts = [f.name for f in fm.fontManager.ttflist]
        chinese_fonts = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans', 'WenQuanYi Micro Hei']
        
        found_fonts = []
        for font in chinese_fonts:
            if font in available_fonts:
                found_fonts.append(font)
        
        if found_fonts:
            print(f"✓ 中文字体配置完成，可用字体: {found_fonts}")
            print(f"✓ 当前使用字体: {current_font}")
        else:
            print("⚠️ 警告：未找到推荐的中文字体，可能会有显示问题")
            print(f"可用字体数量: {len(available_fonts)}")
            
        # 设置默认字体大小和样式
        plt.rcParams['font.size'] = 12
        plt.rcParams['xtick.labelsize'] = 11
        plt.rcParams['ytick.labelsize'] = 11
        
    def enhance_3d_axes(self, ax, x_range=(-2, 2), y_range=(-2, 2), z_range=(-2, 2)):
        """增强3D坐标轴的美观度和细节 - 柔和版"""
        # 设置坐标轴范围
        ax.set_xlim(x_range)
        ax.set_ylim(y_range)
        ax.set_zlim(z_range)
        
        # 添加坐标轴标签 - 柔和色调
        ax.set_xlabel('x', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax.set_ylabel('y', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax.set_zlabel('z', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        
        # 美化网格 - 更轻柔
        ax.grid(True, alpha=0.2, linewidth=0.6, color=self.colors['grid_major'])
        
        # 美化坐标轴面板 - 完全透明
        ax.xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
        ax.yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
        ax.zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
        
        # 设置坐标轴线条样式 - 柔和
        ax.xaxis._axinfo["grid"]['color'] = self.colors['grid_major']
        ax.yaxis._axinfo["grid"]['color'] = self.colors['grid_major']
        ax.zaxis._axinfo["grid"]['color'] = self.colors['grid_major']
        
        # 绘制原点 - 柔和标记
        ax.scatter([0], [0], [0], color=self.colors['axis_main'], s=60, zorder=10, alpha=0.7)
        
    def create_elegant_cube(self, ax, x0, y0, z0, dx, dy, dz, color_type='main', alpha=0.5):
        """创建优雅的立方体"""
        # 立方体的8个顶点
        vertices = np.array([
            [x0, y0, z0], [x0+dx, y0, z0], [x0+dx, y0+dy, z0], [x0, y0+dy, z0],
            [x0, y0, z0+dz], [x0+dx, y0, z0+dz], [x0+dx, y0+dy, z0+dz], [x0, y0+dy, z0+dz]
        ])
        
        # 立方体的6个面
        faces = [
            [vertices[0], vertices[1], vertices[5], vertices[4]],  # 前面
            [vertices[7], vertices[6], vertices[2], vertices[3]],  # 后面
            [vertices[0], vertices[3], vertices[7], vertices[4]],  # 左面
            [vertices[1], vertices[2], vertices[6], vertices[5]],  # 右面
            [vertices[0], vertices[1], vertices[2], vertices[3]],  # 底面
            [vertices[4], vertices[5], vertices[6], vertices[7]]   # 顶面
        ]
        
        # 选择颜色
        if color_type == 'volume':
            face_color = self.colors['volume_element']
            edge_color = self.colors['volume_edge']
        else:
            face_color = self.colors['region_main']
            edge_color = self.colors['boundary_main']
        
        # 绘制立方体面 - 柔和效果
        cube = Poly3DCollection(faces, alpha=alpha, 
                              facecolors=face_color,
                              edgecolors=edge_color, 
                              linewidths=1.2)
        ax.add_collection3d(cube)

    def create_elegant_basic_concept_figure(self):
        """创建优雅的基础概念图：三重积分的几何意义"""
        fig = plt.figure(figsize=(18, 12))
        fig.suptitle('三重积分的几何意义与计算方法', fontsize=18, fontweight='bold', 
                    color=self.colors['text_main'])
        
        # 左图：体积元素概念 - 简化版
        ax1 = fig.add_subplot(131, projection='3d')
        
        # 绘制一个完整的积分区域（椭球体）
        u = np.linspace(0, 2*np.pi, 30)
        v = np.linspace(0, np.pi, 20)
        U, V = np.meshgrid(u, v)
        a, b, c = 1.5, 1.2, 1.0
        X = a * np.sin(V) * np.cos(U)
        Y = b * np.sin(V) * np.sin(U)
        Z = c * np.cos(V)
        
        # 椭球面 - 优雅渲染
        ax1.plot_surface(X, Y, Z, alpha=0.3, color=self.colors['surface_main'],
                        linewidth=0.1, antialiased=True, shade=True)
        
        # 在椭球内部添加一个典型的体积元素dx dy dz
        dx, dy, dz = 0.3, 0.3, 0.3
        x0, y0, z0 = 0.2, 0.2, 0.2
        self.create_elegant_cube(ax1, x0, y0, z0, dx, dy, dz, 'volume', alpha=0.8)
        
        # 添加体积元素标注 - 参考标准模板，放在外面空白区域
        ax1.text2D(0.02, 0.95, '$dx \\, dy \\, dz$', transform=ax1.transAxes,
                  fontsize=14, color=self.colors['text_highlight'], 
                  fontweight='bold', ha='left',
                  bbox=dict(boxstyle="round,pad=0.3", facecolor='#FFF8E7', 
                           edgecolor=self.colors['text_highlight'], alpha=0.9))
        
        # 添加坐标轴向量指示
        ax1.quiver(x0, y0, z0, dx, 0, 0, color='#8B4513', linewidth=2.5, alpha=0.8)
        ax1.quiver(x0, y0, z0, 0, dy, 0, color='#228B22', linewidth=2.5, alpha=0.8)
        ax1.quiver(x0, y0, z0, 0, 0, dz, color='#4169E1', linewidth=2.5, alpha=0.8)
        
        # 坐标轴标注 - 参考标准模板的箭头指向方式
        # dx标注
        ax1.text2D(0.02, 0.85, '$dx$：x方向长度元素', transform=ax1.transAxes,
                  fontsize=11, color='#8B4513', fontweight='bold',
                  bbox=dict(boxstyle="round,pad=0.25", facecolor='#FFF0F0', 
                           edgecolor='#8B4513', alpha=0.9))
        
        # dy标注  
        ax1.text2D(0.02, 0.78, '$dy$：y方向长度元素', transform=ax1.transAxes,
                  fontsize=11, color='#228B22', fontweight='bold',
                  bbox=dict(boxstyle="round,pad=0.25", facecolor='#F0FFF0', 
                           edgecolor='#228B22', alpha=0.9))
        
        # dz标注
        ax1.text2D(0.02, 0.71, '$dz$：z方向长度元素', transform=ax1.transAxes,
                  fontsize=11, color='#4169E1', fontweight='bold',
                  bbox=dict(boxstyle="round,pad=0.25", facecolor='#F0F8FF', 
                           edgecolor='#4169E1', alpha=0.9))
        
        self.enhance_3d_axes(ax1, (-2, 2), (-1.5, 1.5), (-1.2, 1.5))
        ax1.set_title('体积元素分割\n$dV = dx \\, dy \\, dz$', fontsize=13, 
                     fontweight='bold', pad=15, color=self.colors['text_main'])
        ax1.view_init(25, 40)
        
        # 中图：投影法示意 - 优雅版
        ax2 = fig.add_subplot(132, projection='3d')
        
        # 绘制更精细的抛物面
        theta = np.linspace(0, 2*np.pi, 40)
        r = np.linspace(0, 1, 25)
        T, R = np.meshgrid(theta, r)
        X = R * np.cos(T)
        Y = R * np.sin(T)
        Z = X**2 + Y**2
        
        # 抛物面 - 柔和渲染
        ax2.plot_surface(X, Y, Z, alpha=0.6, color=self.colors['surface_main'],
                        linewidth=0.1, antialiased=True, shade=True)
        
        # 上边界平面 - 柔和效果
        Z_top = np.ones_like(Z) * 1.2
        ax2.plot_surface(X, Y, Z_top, alpha=0.4, color=self.colors['surface_aux'],
                        linewidth=0.1, antialiased=True)
        
        # 投影到底面 - 柔和边界
        theta_proj = np.linspace(0, 2*np.pi, 100)
        x_proj = np.cos(theta_proj)
        y_proj = np.sin(theta_proj)
        z_proj = np.zeros_like(x_proj)
        ax2.plot(x_proj, y_proj, z_proj, color=self.colors['boundary_main'], 
                linewidth=3, alpha=0.8)
        
        # 添加少量投影线示例 - 不过密
        for i in range(0, len(theta_proj), 20):
            x_p, y_p = x_proj[i], y_proj[i]
            z_bottom = x_p**2 + y_p**2
            ax2.plot([x_p, x_p], [y_p, y_p], [z_bottom, 1.2], 
                    color=self.colors['boundary_aux'], linewidth=1.5, alpha=0.5)
        
        self.enhance_3d_axes(ax2, (-1.3, 1.3), (-1.3, 1.3), (0, 1.5))
        ax2.set_title('投影法原理\n$\\iint_{D_{xy}} \\int_{z_1}^{z_2} f \\, dz \\, dx \\, dy$', 
                     fontsize=13, fontweight='bold', pad=15, color=self.colors['text_main'])
        ax2.view_init(20, 55)
        
        # 右图：公式说明 - 无乱码优雅版
        ax3 = fig.add_subplot(133)
        ax3.axis('off')
        
        # 分段添加文字，避免乱码和重叠
        # 标题
        ax3.text(0.1, 0.95, '三重积分的基本概念', transform=ax3.transAxes, 
                fontsize=16, fontweight='bold', color=self.colors['text_main'],
                bbox=dict(boxstyle="round,pad=0.3", facecolor='#F0F8FF', 
                         edgecolor=self.colors['boundary_main'], alpha=0.9))
        
        # 步骤1
        ax3.text(0.1, 0.82, '1. 体积元素分割:', transform=ax3.transAxes, 
                fontsize=13, fontweight='bold', color=self.colors['text_highlight'])
        ax3.text(0.15, 0.76, r'将区域$\Omega$分割成n个小体积元素$\Delta V_i$', 
                transform=ax3.transAxes, fontsize=11, color=self.colors['text_formula'])
        
        # 步骤2
        ax3.text(0.1, 0.68, '2. 积分和:', transform=ax3.transAxes, 
                fontsize=13, fontweight='bold', color=self.colors['text_highlight'])
        ax3.text(0.15, 0.62, r'$S_n = \sum_{i=1}^{n} f(\xi_i, \eta_i, \zeta_i) \Delta V_i$', 
                transform=ax3.transAxes, fontsize=11, color=self.colors['text_formula'])
        
        # 步骤3
        ax3.text(0.1, 0.54, '3. 取极限:', transform=ax3.transAxes, 
                fontsize=13, fontweight='bold', color=self.colors['text_highlight'])
        ax3.text(0.15, 0.48, r'$\iiint_{\Omega} f(x,y,z) \, dV = \lim_{\lambda \to 0} S_n$', 
                transform=ax3.transAxes, fontsize=11, color=self.colors['text_formula'])
        
        # 步骤4
        ax3.text(0.1, 0.40, '4. 投影法公式:', transform=ax3.transAxes, 
                fontsize=13, fontweight='bold', color=self.colors['text_highlight'])
        ax3.text(0.15, 0.32, r'$\iiint_{\Omega} f(x,y,z) \, dV =$', 
                transform=ax3.transAxes, fontsize=11, color=self.colors['text_formula'])
        ax3.text(0.15, 0.26, r'$\iint_{D_{xy}} \left[ \int_{z_1(x,y)}^{z_2(x,y)} f(x,y,z) \, dz \right] dx \, dy$', 
                transform=ax3.transAxes, fontsize=11, color=self.colors['text_formula'])
        
        # 说明
        ax3.text(0.1, 0.18, r'其中$D_{xy}$是区域在xy平面的投影', 
                transform=ax3.transAxes, fontsize=10, color=self.colors['text_main'],
                bbox=dict(boxstyle="round,pad=0.2", facecolor='#FFF8E7', alpha=0.8))
        
        plt.tight_layout()
        return fig

    def create_elegant_coordinate_comparison(self):
        """创建清晰的坐标系对比图 - 突出线条和角度，含具体点示例"""
        fig = plt.figure(figsize=(24, 12))
        fig.suptitle('三重积分的三种坐标系对比 - 以点P为例', fontsize=22, fontweight='bold',
                    color='#2F2F2F', y=0.93)
        
        # 高对比度颜色方案
        colors = {
            'x_axis': '#E74C3C',    # 明亮红色
            'y_axis': '#27AE60',    # 明亮绿色  
            'z_axis': '#3498DB',    # 明亮蓝色
            'r_axis': '#E74C3C',    # r用红色
            'theta': '#F39C12',     # θ用橙色
            'phi': '#9B59B6',       # φ用紫色
            'point': '#FF1493',     # 点P用亮粉色
            'background': '#F8F9FA'
        }
        
        # 定义示例点P的坐标 - 选择数值优美的点
        # 直角坐标
        px, py, pz = 1.0, np.sqrt(3), 2.0
        # 柱坐标 (r, θ, z)
        pr = 2.0  # √(1² + (√3)²) = 2
        ptheta = np.pi/3  # arctan(√3/1) = 60°
        pz_cyl = 2.0
        # 球坐标 (ρ, φ, θ)
        prho = 2*np.sqrt(2)  # √(1² + (√3)² + 2²) = 2√2
        pphi = np.pi/4  # arccos(2/(2√2)) = 45°
        ptheta_sph = np.pi/3  # 60°
        
        # 直角坐标系 - 简化清晰版
        ax1 = fig.add_subplot(131, projection='3d')
        ax1.set_facecolor(colors['background'])
        
        # 简化的立方体框架 - 仅显示边线
        edges = [
            [(0,0,0), (1.5,0,0)], [(0,0,0), (0,1,0)], [(0,0,0), (0,0,1.2)],
            [(1.5,0,0), (1.5,1,0)], [(1.5,0,0), (1.5,0,1.2)],
            [(0,1,0), (1.5,1,0)], [(0,1,0), (0,1,1.2)],
            [(0,0,1.2), (1.5,0,1.2)], [(0,0,1.2), (0,1,1.2)],
            [(1.5,1,0), (1.5,1,1.2)], [(1.5,0,1.2), (1.5,1,1.2)], [(0,1,1.2), (1.5,1,1.2)]
        ]
        
        for edge in edges:
            ax1.plot3D(*zip(*edge), color='#BDC3C7', linewidth=1.5, alpha=0.6)
        
        # 突出显示的坐标轴 - 加粗
        ax1.quiver(0, 0, 0, 1.8, 0, 0, color=colors['x_axis'], linewidth=5, 
                  arrow_length_ratio=0.1, alpha=1.0)
        ax1.quiver(0, 0, 0, 0, 1.2, 0, color=colors['y_axis'], linewidth=5, 
                  arrow_length_ratio=0.1, alpha=1.0)
        ax1.quiver(0, 0, 0, 0, 0, 1.5, color=colors['z_axis'], linewidth=5, 
                  arrow_length_ratio=0.1, alpha=1.0)
        
        # 清晰的轴标注
        ax1.text(2.0, 0, 0, 'x', fontsize=18, color=colors['x_axis'], fontweight='bold')
        ax1.text(0, 1.4, 0, 'y', fontsize=18, color=colors['y_axis'], fontweight='bold')
        ax1.text(0, 0, 1.7, 'z', fontsize=18, color=colors['z_axis'], fontweight='bold')
        
        # 标出示例点P
        ax1.scatter([px], [py], [pz], color=colors['point'], s=180, alpha=1.0, 
                   edgecolors='black', linewidth=2)
        ax1.text(px+0.2, py+0.1, pz+0.2, f'P({px:.0f}, {py:.1f}, {pz:.0f})', 
                fontsize=13, color=colors['point'], fontweight='bold', 
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        
        # 绘制坐标线（虚线）
        ax1.plot([0, px], [py, py], [pz, pz], 'k--', alpha=0.5, linewidth=1)
        ax1.plot([px, px], [0, py], [pz, pz], 'k--', alpha=0.5, linewidth=1)
        ax1.plot([px, px], [py, py], [0, pz], 'k--', alpha=0.5, linewidth=1)
        
        self.enhance_3d_axes(ax1, (-0.2, 2.2), (-0.2, 2.0), (-0.2, 2.5))
        ax1.set_title('直角坐标系\n$dV = dx \\, dy \\, dz$', fontsize=16, 
                     fontweight='bold', pad=25, color='#2F2F2F')
        ax1.view_init(elev=20, azim=45)
        
        # 柱坐标系 - 清晰版
        ax2 = fig.add_subplot(132, projection='3d')
        ax2.set_facecolor(colors['background'])
        
        # 简化的圆柱轮廓 - 仅显示关键线条
        theta_circle = np.linspace(0, 2*np.pi, 100)
        x_circle = np.cos(theta_circle)
        y_circle = np.sin(theta_circle)
        
        # 底圆和顶圆
        ax2.plot(x_circle, y_circle, 0, color='#7F8C8D', linewidth=2, alpha=0.6)
        ax2.plot(x_circle, y_circle, 1.5, color='#7F8C8D', linewidth=2, alpha=0.6)
        
        # 几条垂直线表示圆柱
        for angle in [0, np.pi/2, np.pi, 3*np.pi/2]:
            x, y = np.cos(angle), np.sin(angle)
            ax2.plot([x, x], [y, y], [0, 1.5], color='#7F8C8D', linewidth=1.5, alpha=0.6)
        
        # 柱坐标系关键要素 - 清晰的角度关系
        r_val = 0.9
        theta_val = np.pi/3  # 60度角，便于观察
        x_point = r_val * np.cos(theta_val)
        y_point = r_val * np.sin(theta_val)
        z_point = 0.8
        
        # 1. 显示x轴正方向（θ角的起始边）
        ax2.quiver(0, 0, z_point, 0.8, 0, 0, color='#2C3E50', linewidth=4, 
                  alpha=0.8, arrow_length_ratio=0.1, linestyle='--')
        ax2.text(0.9, -0.1, z_point, 'x', fontsize=16, color='#2C3E50', fontweight='bold')
        
        # 2. 显示径向线r（θ角的终止边）
        ax2.quiver(0, 0, z_point, x_point, y_point, 0, color=colors['r_axis'], 
                  linewidth=6, alpha=1.0, arrow_length_ratio=0.1)
        ax2.text(x_point*1.1, y_point*1.1, z_point+0.1, 'r', fontsize=18, 
                color=colors['r_axis'], fontweight='bold')
        
        # 3. θ角弧线（连接x轴和r轴）
        theta_arc = np.linspace(0, theta_val, 50)
        r_arc = 0.35
        x_arc = r_arc * np.cos(theta_arc)
        y_arc = r_arc * np.sin(theta_arc)
        z_arc = np.full_like(theta_arc, z_point)
        ax2.plot(x_arc, y_arc, z_arc, color=colors['theta'], linewidth=5, alpha=1.0)
        
        # 4. z方向
        ax2.quiver(0, 0, 0, 0, 0, 1.5, color=colors['z_axis'], 
                  linewidth=6, alpha=1.0, arrow_length_ratio=0.1)
        ax2.text(-0.1, -0.1, 1.6, 'z', fontsize=18, 
                color=colors['z_axis'], fontweight='bold')
        
        # 5. 角度标注（位置优化）
        ax2.text(0.2, 0.12, z_point+0.15, 'θ', fontsize=16, 
                color=colors['theta'], fontweight='bold')
        
        # 标出示例点P（同一个点在柱坐标系中）
        ax2.scatter([px], [py], [pz], color=colors['point'], s=180, alpha=1.0, 
                   edgecolors='black', linewidth=2)
        ax2.text(px+0.2, py-0.1, pz+0.3, f'P({pr:.0f}, 60°, {pz_cyl:.0f})', 
                fontsize=13, color=colors['point'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        
        # 显示柱坐标的几何关系
        # 从原点到点在xy平面的投影
        ax2.plot([0, px], [0, py], [pz, pz], color=colors['point'], linewidth=3, alpha=0.8)
        # 从投影点到实际点的垂直线
        ax2.plot([px, px], [py, py], [0, pz], color=colors['point'], linewidth=3, alpha=0.8)
        
        self.enhance_3d_axes(ax2, (-1.5, 1.5), (-1.5, 2.0), (-0.2, 2.5))
        ax2.set_title('柱坐标系\n$dV = r \\, dr \\, d\\theta \\, dz$', fontsize=16, 
                     fontweight='bold', pad=25, color='#2F2F2F')
        ax2.view_init(elev=20, azim=45)
        
        # 球坐标系 - 清晰版
        ax3 = fig.add_subplot(133, projection='3d')
        ax3.set_facecolor(colors['background'])
        
        # 简化的球面轮廓 - 仅显示关键大圆
        theta_sphere = np.linspace(0, 2*np.pi, 100)
        
        # xy平面大圆
        x_circle = np.cos(theta_sphere)
        y_circle = np.sin(theta_sphere)
        z_circle = np.zeros_like(x_circle)
        ax3.plot(x_circle, y_circle, z_circle, color='#7F8C8D', linewidth=2, alpha=0.6)
        
        # xz平面大圆
        x_circle2 = np.cos(theta_sphere)
        y_circle2 = np.zeros_like(x_circle2)
        z_circle2 = np.sin(theta_sphere)
        ax3.plot(x_circle2, y_circle2, z_circle2, color='#7F8C8D', linewidth=2, alpha=0.6)
        
        # 球坐标系关键要素 - 优化视角和布局
        r_val = 0.9
        phi_val = np.pi/4   # 天顶角 45度 (与点P一致)
        theta_val = np.pi/3  # 方位角 60度 (与点P一致)
        
        x_point = r_val * np.sin(phi_val) * np.cos(theta_val)
        y_point = r_val * np.sin(phi_val) * np.sin(theta_val)
        z_point = r_val * np.cos(phi_val)
        
        # 1. 显示z轴正方向（φ角的起始边）- 清晰适中
        ax3.quiver(0, 0, 0, 0, 0, 1.0, color='#2C3E50', linewidth=3, 
                  alpha=1.0, arrow_length_ratio=0.08)
        
        # 2. 显示径向线r（φ角的终止边）- 主要向量
        ax3.quiver(0, 0, 0, x_point, y_point, z_point, color=colors['r_axis'], 
                  linewidth=4, alpha=1.0, arrow_length_ratio=0.08)
        
        # 3. 显示x轴正方向（θ角的起始边）
        ax3.quiver(0, 0, 0, 1.0, 0, 0, color='#2C3E50', linewidth=3, 
                  alpha=1.0, arrow_length_ratio=0.08)
        
        # 4. 显示r在xy平面的投影（θ角的终止边）- 适中显示
        x_proj = r_val * np.sin(phi_val) * np.cos(theta_val)
        y_proj = r_val * np.sin(phi_val) * np.sin(theta_val)
        ax3.plot([0, x_proj], [0, y_proj], [0, 0], color='#34495E', 
                linewidth=2.5, alpha=0.8, linestyle='-')
        
        # 5. φ角弧线（从z轴到径向线r）- 清晰展示
        phi_arc = np.linspace(0, phi_val, 50)
        r_arc = 0.5
        x_phi = r_arc * np.sin(phi_arc) * np.cos(theta_val)
        y_phi = r_arc * np.sin(phi_arc) * np.sin(theta_val)
        z_phi = r_arc * np.cos(phi_arc)
        ax3.plot(x_phi, y_phi, z_phi, color=colors['phi'], linewidth=3, alpha=1.0)
        
        # 6. θ角弧线（从x轴到r投影）- 清晰展示
        theta_arc = np.linspace(0, theta_val, 50)
        r_theta = 0.6
        x_theta = r_theta * np.cos(theta_arc)
        y_theta = r_theta * np.sin(theta_arc)
        z_theta = np.zeros_like(theta_arc)
        ax3.plot(x_theta, y_theta, z_theta, color=colors['theta'], linewidth=3, alpha=1.0)
        
        # 7. 外置文字标注 - 用适中指示线连接
        # z轴标注
        ax3.text(-0.4, -0.7, 1.3, 'z轴', fontsize=16, color='#2C3E50', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        ax3.plot([0, -0.25], [0, -0.5], [1.0, 1.2], color='#2C3E50', linewidth=1.5, alpha=0.6)
        
        # x轴标注
        ax3.text(1.3, -0.5, -0.3, 'x轴', fontsize=16, color='#2C3E50', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        ax3.plot([1.0, 1.2], [0, -0.3], [0, -0.2], color='#2C3E50', linewidth=1.5, alpha=0.6)
        
        # r向量标注
        ax3.text(x_point+0.4, y_point+0.3, z_point+0.2, 'r向量', fontsize=16, 
                color=colors['r_axis'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        ax3.plot([x_point, x_point+0.2], [y_point, y_point+0.15], 
                [z_point, z_point+0.1], color=colors['r_axis'], linewidth=1.5, alpha=0.6)
        
        # φ角标注
        phi_mid_x = 0.3 * np.sin(phi_val/2) * np.cos(theta_val)
        phi_mid_y = 0.3 * np.sin(phi_val/2) * np.sin(theta_val)
        phi_mid_z = 0.3 * np.cos(phi_val/2)
        ax3.text(phi_mid_x-0.3, phi_mid_y-0.4, phi_mid_z+0.4, 'φ角', fontsize=16, 
                color=colors['phi'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        ax3.plot([phi_mid_x, phi_mid_x-0.15], [phi_mid_y, phi_mid_y-0.2], 
                [phi_mid_z, phi_mid_z+0.2], color=colors['phi'], linewidth=1.5, alpha=0.6)
        
        # θ角标注
        theta_mid_x = 0.4 * np.cos(theta_val/2)
        theta_mid_y = 0.4 * np.sin(theta_val/2)
        ax3.text(theta_mid_x+0.3, theta_mid_y+0.4, -0.4, 'θ角', fontsize=16, 
                color=colors['theta'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.9))
        ax3.plot([theta_mid_x, theta_mid_x+0.15], [theta_mid_y, theta_mid_y+0.2], 
                [0, -0.2], color=colors['theta'], linewidth=1.5, alpha=0.6)
        
        # 标出示例点P（同一个点在球坐标系中）
        ax3.scatter([px], [py], [pz], color=colors['point'], s=200, alpha=1.0, 
                   edgecolors='black', linewidth=3)
        
        # P点标注 - 外置避免重叠
        ax3.text(px-0.8, py+0.6, pz+0.4, f'P(2√2, 45°, 60°)', 
                fontsize=14, color=colors['point'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='yellow', alpha=0.9))
        # P点指示线
        ax3.plot([px, px-0.4], [py, py+0.3], [pz, pz+0.2], 
                color=colors['point'], linewidth=1.5, alpha=0.8)
        
        # 显示球坐标的几何关系 - 适中显示
        # 从原点到点的径向线（已通过quiver显示）
        # 在xy平面的投影线
        ax3.plot([0, px], [0, py], [0, 0], color='#34495E', linewidth=2.5, 
                alpha=0.8, linestyle='-')
        # 从投影点到实际点的垂直连线
        ax3.plot([px, px], [py, py], [0, pz], color=colors['point'], linewidth=2, 
                alpha=0.7, linestyle='--')
        
        self.enhance_3d_axes(ax3, (-1.8, 1.8), (-1.8, 1.8), (-1.5, 1.8))
        ax3.set_title('球坐标系\n$dV = r^2 \\sin\\phi \\, dr \\, d\\phi \\, d\\theta$', 
                     fontsize=16, fontweight='bold', pad=15, color='#2F2F2F')
        ax3.view_init(elev=20, azim=25)  # 优化视角：更好观察角度关系
        
        # 在图的底部添加简化的坐标转换关系 - 压缩空白
        fig.text(0.17, 0.04, r'$P(x, y, z)$', 
                fontsize=15, ha='center', color='#2F2F2F', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='#F0F8FF', alpha=0.8))
        
        fig.text(0.5, 0.04, r'$x = r\cos\theta, \; y = r\sin\theta, \; z = z$', 
                fontsize=15, ha='center', color='#2F2F2F', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='#FFF8DC', alpha=0.8))
        
        fig.text(0.83, 0.04, r'$x = r\sin\phi\cos\theta, \; y = r\sin\phi\sin\theta, \; z = r\cos\phi$', 
                fontsize=15, ha='center', color='#2F2F2F', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.2", facecolor='#F5F0FF', alpha=0.8))
        
        plt.tight_layout()
        plt.subplots_adjust(bottom=0.08, top=0.88, left=0.02, right=0.98)  # 大幅减少空白
        return fig

    def create_elegant_method_comparison(self):
        """创建优雅的计算方法对比图 - 深度优化版（布局优化）"""
        fig = plt.figure(figsize=(22, 18))
        fig.suptitle('三重积分计算方法深度对比：投影法 vs 截面法', fontsize=20, fontweight='bold',
                    color=self.colors['text_main'], y=0.96)
        
        # 使用GridSpec来更精细地控制布局 - 增加文字区域宽度
        from matplotlib.gridspec import GridSpec
        gs = GridSpec(3, 4, figure=fig, height_ratios=[2, 2, 0.3], width_ratios=[1.3, 1.4, 1.3, 1], 
                     hspace=0.25, wspace=0.25)
        
        # 投影法 - 3D视图（左上，稍微缩小）
        ax1 = fig.add_subplot(gs[0, 0], projection='3d')
        
        # 绘制更精细的椭球
        u = np.linspace(0, 2*np.pi, 50)
        v = np.linspace(0, np.pi, 30)
        U, V = np.meshgrid(u, v)
        a, b, c = 2, 1.5, 1.2
        X = a * np.sin(V) * np.cos(U)
        Y = b * np.sin(V) * np.sin(U)
        Z = c * np.cos(V)
        
        # 椭球面 - 高质量渐变效果
        ax1.plot_surface(X, Y, Z, alpha=0.4, color=self.colors['surface_main'],
                        linewidth=0.1, antialiased=True, shade=True, rstride=2, cstride=2)
        
        # 投影椭圆 - 突出显示
        theta = np.linspace(0, 2*np.pi, 100)
        x_proj = a * np.cos(theta)
        y_proj = b * np.sin(theta)
        z_proj = np.zeros_like(x_proj) - 1.4  # 稍微下移到z轴底部
        ax1.plot(x_proj, y_proj, z_proj, color='#8B0000', 
                linewidth=4.5, alpha=0.9, label='投影曲线')
        
        # 投影平面 - 半透明展示
        x_plane = np.linspace(-2.5, 2.5, 20)
        y_plane = np.linspace(-2, 2, 20)
        X_plane, Y_plane = np.meshgrid(x_plane, y_plane)
        Z_plane = np.full_like(X_plane, -1.4)
        ax1.plot_surface(X_plane, Y_plane, Z_plane, alpha=0.15, color='#F0F0F0')
        
        # 添加精选投影线 - 从椭球到投影面
        projection_indices = [0, 20, 40, 60, 80]  # 精选几条清晰的投影线
        for i in projection_indices:
            xi, yi = x_proj[i], y_proj[i]
            # 椭球上对应点
            zi_top = c * np.sqrt(1 - (xi/a)**2 - (yi/b)**2) if (xi/a)**2 + (yi/b)**2 <= 1 else 0
            zi_bottom = -zi_top
            # 投影线
            ax1.plot([xi, xi], [yi, yi], [zi_top, -1.4], 
                    color='#DC143C', linewidth=2.5, alpha=0.7, linestyle='--')
            ax1.plot([xi, xi], [yi, yi], [zi_bottom, -1.4], 
                    color='#DC143C', linewidth=2.5, alpha=0.7, linestyle='--')
        
        # 移除3D图内的文字说明，稍后放到专门区域
        
        self.enhance_3d_axes(ax1, (-2.5, 2.5), (-2, 2), (-1.4, 1.4))
        ax1.set_title('投影法：椭球投影到 xy 平面', fontsize=15, fontweight='bold', 
                     color='#8B0000', pad=15)
        ax1.view_init(25, 45)
        
        # 投影区域详图（右上）
        ax2 = fig.add_subplot(gs[0, 2:4])
        # 填充投影椭圆
        ax2.fill(x_proj, y_proj, color='#FFE4E1', alpha=0.7, label='投影区域 $D_{xy}$')
        ax2.plot(x_proj, y_proj, color='#8B0000', linewidth=3.5, alpha=0.9)
        
        # 坐标轴
        ax2.axhline(0, color=self.colors['axis_main'], linewidth=1.2, alpha=0.7)
        ax2.axvline(0, color=self.colors['axis_main'], linewidth=1.2, alpha=0.7)
        
        # 添加网格点示例
        x_sample = np.array([-1.5, 0, 1.5])
        y_sample = np.array([-1, 0, 1])
        for xs in x_sample:
            for ys in y_sample:
                if (xs/a)**2 + (ys/b)**2 <= 1:  # 在椭圆内
                    ax2.plot(xs, ys, 'o', color='#8B0000', markersize=8, alpha=0.8)
        
        # 箭头指向关键区域
        ax2.annotate('投影边界\n椭圆曲线', xy=(1.8, 0.8), xytext=(2.8, 1.5),
                    arrowprops=dict(arrowstyle='->', color='#8B0000', lw=2),
                    fontsize=11, color='#8B0000', fontweight='bold', ha='center')
        
        ax2.set_xlim(-3, 3.5)
        ax2.set_ylim(-2.5, 2.5)
        ax2.set_aspect('equal')
        ax2.grid(True, alpha=0.3, color=self.colors['grid_major'])
        ax2.set_title('投影区域分析：$D_{xy}$ 平面', fontsize=15, fontweight='bold',
                     color='#8B0000', pad=15)
        ax2.set_xlabel('x', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax2.set_ylabel('y', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax2.legend(fontsize=11, loc='upper right')
        
        # 截面法 - 3D视图（左下）
        ax3 = fig.add_subplot(gs[1, 0], projection='3d')
        
        # 同样的椭球，但更透明以突出截面
        ax3.plot_surface(X, Y, Z, alpha=0.25, color=self.colors['surface_main'],
                        linewidth=0.1, antialiased=True, rstride=2, cstride=2)
        
        # 绘制多个截面 - 渐变色彩系统
        z_levels = [-0.8, -0.4, 0, 0.4, 0.8]
        colors = ['#8B4513', '#CD853F', '#DDA0DD', '#87CEEB', '#4169E1']  # 温暖到冷色调
        alphas = [0.6, 0.7, 0.8, 0.7, 0.6]  # 中间更突出
        
        for i, z_level in enumerate(z_levels):
            if abs(z_level) <= c:
                # 椭圆截面
                factor = np.sqrt(1 - (z_level/c)**2)
                x_section = a * factor * np.cos(theta)
                y_section = b * factor * np.sin(theta)
                z_section = np.full_like(x_section, z_level)
                
                ax3.plot(x_section, y_section, z_section, 
                        color=colors[i], linewidth=4, alpha=alphas[i],
                        label=f'z = {z_level:.1f}')
                
                # 填充截面 - 增强对比
                vertices = list(zip(x_section, y_section, z_section))
                poly = [vertices]
                section_poly = Poly3DCollection(poly, alpha=0.3, facecolors=colors[i],
                                              edgecolors=colors[i], linewidths=1)
                ax3.add_collection3d(section_poly)
        
        # 移除3D图内的文字说明，稍后放到专门区域
        
        self.enhance_3d_axes(ax3, (-2.5, 2.5), (-2, 2), (-1.4, 1.4))
        ax3.set_title('截面法：平行 z 截面族', fontsize=15, fontweight='bold',
                     color='#4169E1', pad=15)
        ax3.view_init(35, 25)
        ax3.legend(bbox_to_anchor=(1.05, 1), loc='upper left', fontsize=10)
        
        # 典型截面详图（右下）
        ax4 = fig.add_subplot(gs[1, 2:4])
        z_demo = 0.4
        factor_demo = np.sqrt(1 - (z_demo/c)**2)
        x_demo = a * factor_demo * np.cos(theta)
        y_demo = b * factor_demo * np.sin(theta)
        
        # 填充截面区域
        ax4.fill(x_demo, y_demo, color='#E6F3FF', alpha=0.8, label=f'截面 $D({z_demo})$')
        ax4.plot(x_demo, y_demo, color='#4169E1', linewidth=3.5, alpha=0.9)
        
        # 坐标轴
        ax4.axhline(0, color=self.colors['axis_main'], linewidth=1.2, alpha=0.7)
        ax4.axvline(0, color=self.colors['axis_main'], linewidth=1.2, alpha=0.7)
        
        # 添加示例积分点
        x_sample = np.array([-1.2, 0, 1.2])
        y_sample = np.array([-0.8, 0, 0.8])
        for xs in x_sample:
            for ys in y_sample:
                if (xs/(a*factor_demo))**2 + (ys/(b*factor_demo))**2 <= 1:  # 在截面椭圆内
                    ax4.plot(xs, ys, 's', color='#4169E1', markersize=8, alpha=0.8)
        
        # 箭头指向关键区域
        ax4.annotate(f'z = {z_demo} 处\n截面边界', xy=(1.2, 0.6), xytext=(2.3, 1.3),
                    arrowprops=dict(arrowstyle='->', color='#4169E1', lw=2),
                    fontsize=11, color='#4169E1', fontweight='bold', ha='center')
        
        # 添加面积公式
        ax4.text(0.02, 0.95, f'截面面积：', transform=ax4.transAxes,
                fontsize=12, fontweight='bold', color='#4169E1')
        ax4.text(0.02, 0.88, f'$S(z) = \\pi ab (1 - z^2/c^2)$', transform=ax4.transAxes,
                fontsize=11, color='#4169E1')
        
        ax4.set_xlim(-2.5, 2.8)
        ax4.set_ylim(-2, 2)
        ax4.set_aspect('equal')
        ax4.grid(True, alpha=0.3, color=self.colors['grid_major'])
        ax4.set_title(f'典型截面分析：z = {z_demo}', fontsize=15, fontweight='bold',
                     color='#4169E1', pad=15)
        ax4.set_xlabel('x', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax4.set_ylabel('y', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax4.legend(fontsize=11, loc='upper right')
        
        # 在中间列添加方法特点说明
        ax_text1 = fig.add_subplot(gs[0, 1])
        ax_text1.axis('off')
        ax_text1.text(0.5, 0.95, '投影法特点', fontsize=14, fontweight='bold', 
                     color='#8B0000', ha='center', transform=ax_text1.transAxes)
        ax_text1.text(0.1, 0.75, '• 先投影到坐标面', fontsize=11, 
                     color='#8B0000', transform=ax_text1.transAxes)
        ax_text1.text(0.1, 0.65, '• 确定积分区域边界', fontsize=11, 
                     color='#8B0000', transform=ax_text1.transAxes)
        ax_text1.text(0.1, 0.55, '• 建立三重积分', fontsize=11, 
                     color='#8B0000', transform=ax_text1.transAxes)
        
        # 投影法积分表达式 - 紧凑布局
        ax_text1.text(0.5, 0.35, '积分表达式：', fontsize=12, fontweight='bold', 
                     color=self.colors['text_main'], ha='center', transform=ax_text1.transAxes)
        ax_text1.text(0.5, 0.25, r'$\iiint_{\Omega} f(x,y,z) \, dV$', fontsize=13, 
                     color=self.colors['text_main'], ha='center', transform=ax_text1.transAxes)
        ax_text1.text(0.5, 0.12, r'$= \iint_{D_{xy}} \left[ \int_{z_1(x,y)}^{z_2(x,y)} f(x,y,z) \, dz \right] \, dx \, dy$', 
                     fontsize=11, color=self.colors['text_main'], ha='center', transform=ax_text1.transAxes)
        
        # 截面法特点说明
        ax_text2 = fig.add_subplot(gs[1, 1])
        ax_text2.axis('off')
        ax_text2.text(0.5, 0.95, '截面法特点', fontsize=14, fontweight='bold', 
                     color='#4169E1', ha='center', transform=ax_text2.transAxes)
        ax_text2.text(0.1, 0.75, '• 平行截面族分割', fontsize=11, 
                     color='#4169E1', transform=ax_text2.transAxes)
        ax_text2.text(0.1, 0.65, '• 每个截面独立计算', fontsize=11, 
                     color='#4169E1', transform=ax_text2.transAxes)
        ax_text2.text(0.1, 0.55, '• 沿截面方向积分', fontsize=11, 
                     color='#4169E1', transform=ax_text2.transAxes)
        
        # 截面法积分表达式 - 紧凑布局
        ax_text2.text(0.5, 0.35, '积分表达式：', fontsize=12, fontweight='bold', 
                     color=self.colors['text_main'], ha='center', transform=ax_text2.transAxes)
        ax_text2.text(0.5, 0.25, r'$\iiint_{\Omega} f(x,y,z) \, dV$', fontsize=13, 
                     color=self.colors['text_main'], ha='center', transform=ax_text2.transAxes)
        ax_text2.text(0.5, 0.12, r'$= \int_{z_1}^{z_2} \left[ \iint_{D(z)} f(x,y,z) \, dx \, dy \right] \, dz$', 
                     fontsize=11, color=self.colors['text_main'], ha='center', transform=ax_text2.transAxes)
        
        # 底部方法选择指导 - 利用增宽的空间
        ax_summary = fig.add_subplot(gs[2, :])
        ax_summary.axis('off')
        ax_summary.text(0.5, 0.7, '方法选择指导', fontsize=16, fontweight='bold', 
                       color=self.colors['text_main'], ha='center', transform=ax_summary.transAxes)
        ax_summary.text(0.25, 0.25, '投影法：适合投影边界简单的区域（如椭球、抛物面等）', 
                       fontsize=13, color='#8B0000', ha='center', transform=ax_summary.transAxes)
        ax_summary.text(0.75, 0.25, '截面法：适合截面形状规律的区域（如旋转体、柱体等）', 
                       fontsize=13, color='#4169E1', ha='center', transform=ax_summary.transAxes)
        
        plt.tight_layout()
        return fig

    def create_elegant_cylindrical_coord(self):
        """创建优雅的柱坐标系图像 - 参考三坐标系对比图风格的高质量版"""
        fig = plt.figure(figsize=(24, 16))  # 高质量尺寸
        fig.suptitle('柱坐标系详解：变换公式与体积元素', fontsize=22, fontweight='bold',
                    color='#2F2F2F', y=0.95)
        
        # 使用GridSpec实现精确布局控制
        from matplotlib.gridspec import GridSpec
        gs = GridSpec(2, 4, figure=fig, height_ratios=[2.5, 1], width_ratios=[1.8, 1.2, 1.2, 1], 
                     hspace=0.2, wspace=0.15)
        
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
        
        # 定义示例点P的坐标 - 与三坐标系对比图保持一致
        px, py, pz = 1.0, np.sqrt(3), 2.0  # 直角坐标
        pr = 2.0  # √(1² + (√3)²) = 2，径向距离
        ptheta = np.pi/3  # arctan(√3/1) = 60°，方位角
        pz_cyl = 2.0  # z坐标不变
        
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
        
        # === 4. 绘制柱坐标网格系统 ===
        # r=常数的同心圆（在不同z层）
        for r_circle in [1.0, 2.0, 2.5]:
            theta_circle = np.linspace(0, 2*np.pi, 100)
            x_circle = r_circle * np.cos(theta_circle)
            y_circle = r_circle * np.sin(theta_circle)
            
            # 在z=0和z=pz层绘制圆
            for z_level in [0, pz]:
                z_circle = np.full_like(theta_circle, z_level)
                ax_main.plot(x_circle, y_circle, z_circle, color=colors['grid'], 
                            linewidth=1.5, alpha=0.4, linestyle='--')
        
        # θ=常数的射线（在不同z层）
        for theta_ray in np.linspace(0, 2*np.pi, 8, endpoint=False):
            r_ray = np.linspace(0, 2.5, 20)
            x_ray = r_ray * np.cos(theta_ray)
            y_ray = r_ray * np.sin(theta_ray)
            
            for z_level in [0, pz]:
                z_ray = np.full_like(r_ray, z_level)
                ax_main.plot(x_ray, y_ray, z_ray, color=colors['grid'], 
                            linewidth=1, alpha=0.3)
        
        # === 5. 绘制体积元素dV = r dr dθ dz ===
        # 在点P附近绘制一个典型的柱坐标体积元素
        dr, dtheta, dz_vol = 0.25, np.pi/12, 0.3
        r_base, theta_base, z_base = pr - dr/2, ptheta - dtheta/2, pz - dz_vol/2
        
        # 绘制扇形柱体的关键边线
        theta_vol = np.linspace(theta_base, theta_base + dtheta, 20)
        
        # 内外弧线（底面和顶面）
        for z_level in [z_base, z_base + dz_vol]:
            # 内弧
            x_inner = r_base * np.cos(theta_vol)
            y_inner = r_base * np.sin(theta_vol)
            z_inner = np.full_like(theta_vol, z_level)
            ax_main.plot(x_inner, y_inner, z_inner, color=self.colors['volume_edge'], 
                        linewidth=3, alpha=0.8)
            
            # 外弧
            x_outer = (r_base + dr) * np.cos(theta_vol)
            y_outer = (r_base + dr) * np.sin(theta_vol)
            z_outer = np.full_like(theta_vol, z_level)
            ax_main.plot(x_outer, y_outer, z_outer, color=self.colors['volume_edge'], 
                        linewidth=3, alpha=0.8)
        
        # 径向边线
        for theta_edge in [theta_base, theta_base + dtheta]:
            x_rad = [r_base * np.cos(theta_edge), (r_base + dr) * np.cos(theta_edge)]
            y_rad = [r_base * np.sin(theta_edge), (r_base + dr) * np.sin(theta_edge)]
            for z_level in [z_base, z_base + dz_vol]:
                z_rad = [z_level, z_level]
                ax_main.plot(x_rad, y_rad, z_rad, color=self.colors['volume_edge'], 
                            linewidth=3, alpha=0.8)
        
        # 垂直边线
        for r_edge in [r_base, r_base + dr]:
            for theta_edge in [theta_base, theta_base + dtheta]:
                x_vert = r_edge * np.cos(theta_edge)
                y_vert = r_edge * np.sin(theta_edge)
                z_vert = [z_base, z_base + dz_vol]
                ax_main.plot([x_vert, x_vert], [y_vert, y_vert], z_vert, 
                            color=self.colors['volume_edge'], linewidth=3, alpha=0.8)
        
        # 设置3D坐标轴
        self.enhance_3d_axes(ax_main, (-0.5, 3.2), (-0.5, 3.2), (-0.5, 3.2))
        ax_main.set_title('柱坐标系3D结构与体积元素', fontsize=18, fontweight='bold',
                         color='#2F2F2F', pad=20)
        ax_main.view_init(elev=20, azim=45)
        
        # === 右上：坐标变换公式详解 ===
        ax_formula = fig.add_subplot(gs[0, 1])
        ax_formula.axis('off')
        ax_formula.text(0.5, 0.98, '坐标变换公式', fontsize=18, fontweight='bold', 
                       color='#2F2F2F', ha='center', transform=ax_formula.transAxes)
        
        # 直角坐标 → 柱坐标
        ax_formula.text(0.05, 0.85, '直角坐标 → 柱坐标：', fontsize=15, fontweight='bold',
                       color=colors['r_axis'], transform=ax_formula.transAxes)
        ax_formula.text(0.05, 0.75, r'$x = r\cos\theta$', fontsize=14,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.05, 0.65, r'$y = r\sin\theta$', fontsize=14,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.05, 0.55, r'$z = z$', fontsize=14,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        
        # 柱坐标 → 直角坐标
        ax_formula.text(0.05, 0.40, '柱坐标 → 直角坐标：', fontsize=15, fontweight='bold',
                       color=colors['theta'], transform=ax_formula.transAxes)
        ax_formula.text(0.05, 0.30, r'$r = \sqrt{x^2 + y^2}$', fontsize=14,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.05, 0.20, r'$\theta = \arctan\left(\frac{y}{x}\right)$', fontsize=14,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.05, 0.10, r'$z = z$', fontsize=14,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        
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
                      color=self.colors['text_formula'], transform=ax_volume.transAxes)
        ax_volume.text(0.05, 0.30, r'$0 \leq \theta \leq 2\pi$ （方位角）', fontsize=13,
                      color=self.colors['text_formula'], transform=ax_volume.transAxes)
        ax_volume.text(0.05, 0.20, r'$-\infty < z < +\infty$ （高度）', fontsize=13,
                      color=self.colors['text_formula'], transform=ax_volume.transAxes)
        
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
                        color=self.colors['text_formula'], transform=ax_geometry.transAxes)
        ax_geometry.text(0.05, 0.20, '• 旋转体积分', fontsize=13,
                        color=self.colors['text_formula'], transform=ax_geometry.transAxes)
        ax_geometry.text(0.05, 0.10, '• 圆对称区域', fontsize=13,
                        color=self.colors['text_formula'], transform=ax_geometry.transAxes)
        
        # === 底部：典型积分应用示例 ===
        ax_examples = fig.add_subplot(gs[1, 1:])
        ax_examples.axis('off')
        ax_examples.text(0.5, 0.90, '柱坐标系典型积分应用', fontsize=18, fontweight='bold', 
                        color='#2F2F2F', ha='center', transform=ax_examples.transAxes)
        
        # 圆柱体积分示例
        ax_examples.text(0.02, 0.70, '• 圆柱体积分：', fontsize=15, fontweight='bold',
                        color=colors['r_axis'], transform=ax_examples.transAxes)
        ax_examples.text(0.02, 0.55, r'$\iiint_{x^2+y^2 \leq a^2, \, 0 \leq z \leq h} f(x,y,z) \, dV = \int_0^{2\pi} \int_0^a \int_0^h f(r\cos\theta, r\sin\theta, z) \cdot r \, dz \, dr \, d\theta$',
                        fontsize=13, color=self.colors['text_formula'], transform=ax_examples.transAxes)
        
        # 圆锥体积分示例
        ax_examples.text(0.02, 0.35, '• 圆锥体积分：', fontsize=15, fontweight='bold',
                        color=colors['theta'], transform=ax_examples.transAxes)
        ax_examples.text(0.02, 0.20, r'$\iiint_{x^2+y^2 \leq z^2, \, 0 \leq z \leq 1} f(x,y,z) \, dV = \int_0^{2\pi} \int_0^1 \int_0^z f(r\cos\theta, r\sin\theta, z) \cdot r \, dr \, dz \, d\theta$',
                        fontsize=13, color=self.colors['text_formula'], transform=ax_examples.transAxes)
        
        plt.tight_layout()
        
        return fig
    
    def create_elegant_spherical_coord(self):
        """创建优雅的球坐标系图像 - 高质量版"""
        fig = plt.figure(figsize=(22, 16))  # 提升到高质量尺寸
        fig.suptitle('球坐标系详解：变换公式与体积元素', fontsize=20, fontweight='bold',
                    color=self.colors['text_main'], y=0.96)
        
        # 使用GridSpec实现复杂布局
        from matplotlib.gridspec import GridSpec
        gs = GridSpec(2, 3, figure=fig, height_ratios=[2, 1], width_ratios=[1.5, 1.2, 1.3], 
                     hspace=0.25, wspace=0.25)
        
        # 主3D图（左上，占两行）
        ax = fig.add_subplot(gs[:, 0], projection='3d')
        
        # 设置球坐标参数 - 优化视角
        r_val = 2.2
        phi_val = np.pi/4  # 极角45度，便于观察
        theta_val = np.pi/3  # 方位角60度，便于观察
        
        x_p = r_val * np.sin(phi_val) * np.cos(theta_val)
        y_p = r_val * np.sin(phi_val) * np.sin(theta_val)
        z_p = r_val * np.cos(phi_val)
        
        # 1. 坐标轴 - 清晰适中
        ax.quiver(0, 0, 0, 0, 0, 3.2, color='#2C3E50', 
                linewidth=3, alpha=1.0, arrow_length_ratio=0.05)
        ax.quiver(0, 0, 0, 3.2, 0, 0, color='#2C3E50', 
                linewidth=3, alpha=1.0, arrow_length_ratio=0.05)
        ax.quiver(0, 0, 0, 0, 3.2, 0, color='#2C3E50', 
                linewidth=3, alpha=0.8, arrow_length_ratio=0.05)
        
        # 2. 径向线（从原点到点P）- 主要向量
        ax.quiver(0, 0, 0, x_p, y_p, z_p, color=self.colors['boundary_main'], 
                linewidth=4, alpha=1.0, arrow_length_ratio=0.08)
        
        # 3. 极角弧线（φ角）- 清晰展示
        phi_arc = np.linspace(0, phi_val, 60)
        r_arc = 1.2
        x_phi = r_arc * np.sin(phi_arc) * np.cos(theta_val)
        y_phi = r_arc * np.sin(phi_arc) * np.sin(theta_val) 
        z_phi = r_arc * np.cos(phi_arc)
        ax.plot(x_phi, y_phi, z_phi, color=self.colors['boundary_aux'], 
                linewidth=3, alpha=1.0)
        
        # 4. 方位角弧线（θ角）- 清晰展示
        theta_arc = np.linspace(0, theta_val, 60)
        r_arc = 1.5
        x_theta = r_arc * np.cos(theta_arc)
        y_theta = r_arc * np.sin(theta_arc)
        z_theta = np.zeros_like(theta_arc)
        ax.plot(x_theta, y_theta, z_theta, color=self.colors['boundary_highlight'], 
                linewidth=3, alpha=1.0)
        
        # 5. 显示r在xy平面的投影
        x_proj = r_val * np.sin(phi_val) * np.cos(theta_val)
        y_proj = r_val * np.sin(phi_val) * np.sin(theta_val)
        ax.plot([0, x_proj], [0, y_proj], [0, 0], color='#34495E', 
                linewidth=2.5, alpha=0.8, linestyle='-')
        
        # 6. 球面网格（简化，避免视觉混乱）
        phi_grid = np.linspace(0, np.pi, 15)
        theta_grid = np.linspace(0, 2*np.pi, 25)
        
        # 画几条关键经线
        for theta in [0, np.pi/3, np.pi/2, np.pi, 3*np.pi/2]:
            x_meridian = r_val * np.sin(phi_grid) * np.cos(theta)
            y_meridian = r_val * np.sin(phi_grid) * np.sin(theta)
            z_meridian = r_val * np.cos(phi_grid)
            ax.plot(x_meridian, y_meridian, z_meridian, 
                   color=self.colors['grid_major'], linewidth=1, alpha=0.3)
        
        # 画几条关键纬线
        for phi in [np.pi/6, np.pi/4, np.pi/3, np.pi/2, 2*np.pi/3]:
            x_parallel = r_val * np.sin(phi) * np.cos(theta_grid)
            y_parallel = r_val * np.sin(phi) * np.sin(theta_grid)
            z_parallel = r_val * np.cos(phi) * np.ones_like(theta_grid)
            ax.plot(x_parallel, y_parallel, z_parallel, 
                   color=self.colors['grid_major'], linewidth=1, alpha=0.3)
        
        # 6. 绘制点P
        ax.scatter([x_p], [y_p], [z_p], color=self.colors['text_highlight'], 
                  s=200, alpha=1.0, zorder=10, edgecolors='black', linewidth=3)
        
        # 7. 球坐标体积元素
        dr, dphi, dtheta = 0.25, np.pi/15, np.pi/10
        self.draw_spherical_volume_element(ax, r_val-dr/2, phi_val, theta_val, 
                                         dr, dphi, dtheta)
        
        # 8. 外置文字标注 - 用适中指示线，清晰展示
        # z轴标注
        ax.text(-1.5, -2.0, 3.8, 'z轴', fontsize=16, color='#2C3E50', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.9))
        ax.plot([0, -0.8], [0, -1.2], [3.2, 3.5], color='#2C3E50', linewidth=1.5, alpha=0.7)
        
        # x轴标注
        ax.text(4.0, -1.0, -0.8, 'x轴', fontsize=16, color='#2C3E50', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.9))
        ax.plot([3.2, 3.6], [0, -0.5], [0, -0.4], color='#2C3E50', linewidth=1.5, alpha=0.7)
        
        # y轴标注
        ax.text(-0.8, 4.0, -0.8, 'y轴', fontsize=16, color='#2C3E50', fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.9))
        ax.plot([0, -0.4], [3.2, 3.6], [0, -0.4], color='#2C3E50', linewidth=1.5, alpha=0.7)
        
        # r向量标注
        ax.text(x_p+1.0, y_p+0.8, z_p+0.5, 'r向量', fontsize=16, 
                color=self.colors['boundary_main'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.9))
        ax.plot([x_p, x_p+0.5], [y_p, y_p+0.4], [z_p, z_p+0.25], 
                color=self.colors['boundary_main'], linewidth=1.5, alpha=0.7)
        
        # P点标注
        ax.text(x_p-1.5, y_p+1.2, z_p+1.0, r'P(r,φ,θ)', fontsize=16, 
                color=self.colors['text_highlight'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='yellow', alpha=0.9))
        ax.plot([x_p, x_p-0.8], [y_p, y_p+0.6], [z_p, z_p+0.5], 
                color=self.colors['text_highlight'], linewidth=1.5, alpha=0.7)
        
        # φ角标注
        phi_mid_x = 0.8 * np.sin(phi_val/2) * np.cos(theta_val)
        phi_mid_y = 0.8 * np.sin(phi_val/2) * np.sin(theta_val)
        phi_mid_z = 0.8 * np.cos(phi_val/2)
        ax.text(phi_mid_x-1.2, phi_mid_y-1.5, phi_mid_z+1.2, 'φ角(极角)', fontsize=16, 
                color=self.colors['boundary_aux'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.9))
        ax.plot([phi_mid_x, phi_mid_x-0.6], [phi_mid_y, phi_mid_y-0.8], 
                [phi_mid_z, phi_mid_z+0.6], color=self.colors['boundary_aux'], linewidth=1.5, alpha=0.7)
        
        # θ角标注
        theta_mid_x = 1.0 * np.cos(theta_val/2)
        theta_mid_y = 1.0 * np.sin(theta_val/2)
        ax.text(theta_mid_x+1.0, theta_mid_y+1.5, -1.2, 'θ角(方位角)', fontsize=16, 
                color=self.colors['boundary_highlight'], fontweight='bold',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.9))
        ax.plot([theta_mid_x, theta_mid_x+0.5], [theta_mid_y, theta_mid_y+0.8], 
                [0, -0.6], color=self.colors['boundary_highlight'], linewidth=1.5, alpha=0.7)
        
        # 设置坐标轴 - 扩大范围容纳外置标注
        self.enhance_3d_axes(ax, (-4, 4), (-4, 4), (-2, 4))
        ax.set_title('球坐标系3D结构详解', fontsize=16, fontweight='bold',
                    color=self.colors['text_main'], pad=15)
        
        # 优化视角，确保所有元素清晰可见
        ax.view_init(elev=22, azim=40)
        
        # 右上：坐标变换公式详解
        ax_formula = fig.add_subplot(gs[0, 1])
        ax_formula.axis('off')
        ax_formula.text(0.5, 0.95, '坐标变换公式', fontsize=16, fontweight='bold', 
                       color=self.colors['text_main'], ha='center', transform=ax_formula.transAxes)
        
        ax_formula.text(0.1, 0.80, '直角坐标 → 球坐标：', fontsize=14, fontweight='bold',
                       color=self.colors['boundary_main'], transform=ax_formula.transAxes)
        ax_formula.text(0.1, 0.70, r'$x = r\sin\varphi\cos\theta$', fontsize=13,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.1, 0.62, r'$y = r\sin\varphi\sin\theta$', fontsize=13,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.1, 0.54, r'$z = r\cos\varphi$', fontsize=13,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        
        ax_formula.text(0.1, 0.40, '球坐标 → 直角坐标：', fontsize=14, fontweight='bold',
                       color=self.colors['boundary_aux'], transform=ax_formula.transAxes)
        ax_formula.text(0.1, 0.30, r'$r = \sqrt{x^2 + y^2 + z^2}$', fontsize=13,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.1, 0.22, r'$\varphi = \arccos(\frac{z}{\sqrt{x^2 + y^2 + z^2}})$', fontsize=11,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        ax_formula.text(0.1, 0.14, r'$\theta = \arctan(\frac{y}{x})$', fontsize=13,
                       color=self.colors['text_formula'], transform=ax_formula.transAxes)
        
        # 右上：体积元素详解  
        ax_volume = fig.add_subplot(gs[0, 2])
        ax_volume.axis('off')
        ax_volume.text(0.5, 0.95, '体积元素分析', fontsize=16, fontweight='bold', 
                      color=self.colors['text_main'], ha='center', transform=ax_volume.transAxes)
        
        ax_volume.text(0.1, 0.80, '球坐标体积元素：', fontsize=14, fontweight='bold',
                      color=self.colors['text_highlight'], transform=ax_volume.transAxes)
        ax_volume.text(0.1, 0.68, r'$dV = r^2\sin\varphi \, dr \, d\varphi \, d\theta$', fontsize=13,
                      color=self.colors['text_highlight'], transform=ax_volume.transAxes)
        
        ax_volume.text(0.1, 0.50, '参数范围：', fontsize=14, fontweight='bold',
                      color=self.colors['text_main'], transform=ax_volume.transAxes)
        ax_volume.text(0.1, 0.40, r'$r \geq 0$ （径向距离）', fontsize=12,
                      color=self.colors['text_formula'], transform=ax_volume.transAxes)
        ax_volume.text(0.1, 0.32, r'$0 \leq \varphi \leq \pi$ （极角）', fontsize=12,
                      color=self.colors['text_formula'], transform=ax_volume.transAxes)
        ax_volume.text(0.1, 0.24, r'$0 \leq \theta \leq 2\pi$ （方位角）', fontsize=12,
                      color=self.colors['text_formula'], transform=ax_volume.transAxes)
        
        ax_volume.text(0.1, 0.08, r'Jacobian行列式：$r^2\sin\varphi$', fontsize=13, fontweight='bold',
                      color=self.colors['boundary_highlight'], transform=ax_volume.transAxes)
        
        # 底部：积分应用示例
        ax_example = fig.add_subplot(gs[1, 1:])
        ax_example.axis('off')
        ax_example.text(0.5, 0.90, '球坐标系典型应用', fontsize=16, fontweight='bold', 
                       color=self.colors['text_main'], ha='center', transform=ax_example.transAxes)
        
        ax_example.text(0.05, 0.70, '• 球体积分：', fontsize=14, fontweight='bold',
                       color=self.colors['boundary_main'], transform=ax_example.transAxes)
        ax_example.text(0.05, 0.55, r'$\iiint_{x^2+y^2+z^2 \leq a^2} f(x,y,z) \, dV = \int_0^{2\pi} \int_0^\pi \int_0^a f(r\sin\varphi\cos\theta, r\sin\varphi\sin\theta, r\cos\varphi) \cdot r^2\sin\varphi \, dr \, d\varphi \, d\theta$',
                       fontsize=11, color=self.colors['text_formula'], transform=ax_example.transAxes)
        
        ax_example.text(0.05, 0.35, '• 锥体积分：', fontsize=14, fontweight='bold',
                       color=self.colors['boundary_aux'], transform=ax_example.transAxes)
        ax_example.text(0.05, 0.20, r'$\iiint_{x^2+y^2+z^2 \leq 1, z \geq \frac{\sqrt{3}}{2}} f(x,y,z) \, dV = \int_0^{2\pi} \int_0^{\pi/6} \int_0^1 f(r\sin\varphi\cos\theta, r\sin\varphi\sin\theta, r\cos\varphi) \cdot r^2\sin\varphi \, dr \, d\varphi \, d\theta$',
                       fontsize=10, color=self.colors['text_formula'], transform=ax_example.transAxes)
        
        plt.tight_layout()
        
        return fig
    
    def create_elegant_tetrahedron_example(self):
        """创建优雅的四面体例题图像 - 高质量版"""
        fig = plt.figure(figsize=(22, 16))  # 提升到高质量尺寸
        fig.suptitle(r'四面体积分区域详解：$x \geq 0, y \geq 0, z \geq 0, x+y+z \leq 1$', 
                    fontsize=20, fontweight='bold', color=self.colors['text_main'], y=0.96)
        
        # 使用GridSpec实现复杂布局
        from matplotlib.gridspec import GridSpec
        gs = GridSpec(2, 3, figure=fig, height_ratios=[2, 1], width_ratios=[1.5, 1.2, 1.3], 
                     hspace=0.25, wspace=0.25)
        
        # 主3D图（左上，占两行）
        ax = fig.add_subplot(gs[:, 0], projection='3d')
        
        # 四面体的四个顶点
        vertices = np.array([
            [0, 0, 0],  # 原点
            [1, 0, 0],  # x轴上的点
            [0, 1, 0],  # y轴上的点
            [0, 0, 1]   # z轴上的点
        ])
        
        # 四面体的四个面
        faces = [
            [vertices[0], vertices[1], vertices[2]],  # xy平面上的三角形
            [vertices[0], vertices[1], vertices[3]],  # xz平面上的三角形
            [vertices[0], vertices[2], vertices[3]],  # yz平面上的三角形
            [vertices[1], vertices[2], vertices[3]]   # 斜面
        ]
        
        # 绘制四面体面
        tetrahedron = Poly3DCollection(faces, alpha=0.3, 
                                     facecolors=self.colors['region_main'],
                                     edgecolors=self.colors['boundary_main'], 
                                     linewidths=2.5)
        ax.add_collection3d(tetrahedron)
        
        # 绘制顶点
        ax.scatter(vertices[:, 0], vertices[:, 1], vertices[:, 2], 
                  color=self.colors['text_highlight'], s=80, alpha=0.9, zorder=10)
        
        # 标注顶点
        labels = ['O(0,0,0)', 'A(1,0,0)', 'B(0,1,0)', 'C(0,0,1)']
        offsets = [(0.05, 0.05, 0.05), (0.05, 0, 0), (0, 0.05, 0), (0, 0, 0.05)]
        
        for i, (vertex, label, offset) in enumerate(zip(vertices, labels, offsets)):
            ax.text(vertex[0] + offset[0], vertex[1] + offset[1], vertex[2] + offset[2],
                   label, fontsize=11, color=self.colors['text_highlight'], 
                   fontweight='bold')
        
        # 绘制坐标轴和网格
        ax.plot([0, 1.2], [0, 0], [0, 0], color=self.colors['axis_main'], 
                linewidth=2, alpha=0.7)
        ax.plot([0, 0], [0, 1.2], [0, 0], color=self.colors['axis_main'], 
                linewidth=2, alpha=0.7)
        ax.plot([0, 0], [0, 0], [0, 1.2], color=self.colors['axis_main'], 
                linewidth=2, alpha=0.7)
        
        # 添加典型体积元素
        dx, dy, dz = 0.15, 0.15, 0.15
        x0, y0, z0 = 0.2, 0.2, 0.2
        if x0 + y0 + z0 + dx + dy + dz <= 1:  # 确保在四面体内
            self.create_elegant_cube(ax, x0, y0, z0, dx, dy, dz, 'volume', alpha=0.7)
        
        # 设置坐标轴
        ax.set_xlim(-0.1, 1.3)
        ax.set_ylim(-0.1, 1.3)
        ax.set_zlim(-0.1, 1.3)
        
        ax.set_xlabel('x', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax.set_ylabel('y', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax.set_zlabel('z', fontsize=14, fontweight='bold', color=self.colors['axis_main'])
        ax.set_title('四面体3D结构', fontsize=16, fontweight='bold',
                    color=self.colors['text_main'], pad=15)
        
        ax.view_init(elev=25, azim=45)
        
        # 右上：约束条件和性质
        ax_properties = fig.add_subplot(gs[0, 1])
        ax_properties.axis('off')
        ax_properties.text(0.5, 0.95, '区域约束与性质', fontsize=16, fontweight='bold', 
                          color=self.colors['text_main'], ha='center', transform=ax_properties.transAxes)
        
        ax_properties.text(0.1, 0.80, '约束条件：', fontsize=14, fontweight='bold',
                          color=self.colors['boundary_main'], transform=ax_properties.transAxes)
        ax_properties.text(0.1, 0.70, r'$x \geq 0, y \geq 0, z \geq 0$', fontsize=13,
                          color=self.colors['text_formula'], transform=ax_properties.transAxes)
        ax_properties.text(0.1, 0.62, r'$x + y + z \leq 1$', fontsize=13,
                          color=self.colors['text_formula'], transform=ax_properties.transAxes)
        
        ax_properties.text(0.1, 0.45, '几何性质：', fontsize=14, fontweight='bold',
                          color=self.colors['boundary_aux'], transform=ax_properties.transAxes)
        ax_properties.text(0.1, 0.35, r'• 体积：$V = \frac{1}{6}$', fontsize=12,
                          color=self.colors['text_formula'], transform=ax_properties.transAxes)
        ax_properties.text(0.1, 0.27, r'• 质心：$(\frac{1}{4}, \frac{1}{4}, \frac{1}{4})$', fontsize=12,
                          color=self.colors['text_formula'], transform=ax_properties.transAxes)
        ax_properties.text(0.1, 0.19, '• 关于三变量轮换对称', fontsize=12,
                          color=self.colors['text_formula'], transform=ax_properties.transAxes)
        ax_properties.text(0.1, 0.11, r'• 边长均为 $\sqrt{2}$ 的正四面体', fontsize=12,
                          color=self.colors['text_formula'], transform=ax_properties.transAxes)
        
        # 右上：积分次序分析
        ax_integration = fig.add_subplot(gs[0, 2])
        ax_integration.axis('off')
        ax_integration.text(0.5, 0.95, '积分次序分析', fontsize=16, fontweight='bold', 
                           color=self.colors['text_main'], ha='center', transform=ax_integration.transAxes)
        
        ax_integration.text(0.1, 0.80, '次序1 (dz dy dx)：', fontsize=14, fontweight='bold',
                           color=self.colors['text_highlight'], transform=ax_integration.transAxes)
        ax_integration.text(0.1, 0.70, r'$0 \leq z \leq 1-x-y$', fontsize=12,
                           color=self.colors['text_formula'], transform=ax_integration.transAxes)
        ax_integration.text(0.1, 0.62, r'$0 \leq y \leq 1-x$', fontsize=12,
                           color=self.colors['text_formula'], transform=ax_integration.transAxes)
        ax_integration.text(0.1, 0.54, r'$0 \leq x \leq 1$', fontsize=12,
                           color=self.colors['text_formula'], transform=ax_integration.transAxes)
        
        ax_integration.text(0.1, 0.38, '次序2 (dx dy dz)：', fontsize=14, fontweight='bold',
                           color=self.colors['boundary_main'], transform=ax_integration.transAxes)
        ax_integration.text(0.1, 0.28, r'$0 \leq x \leq 1-y-z$', fontsize=12,
                           color=self.colors['text_formula'], transform=ax_integration.transAxes)
        ax_integration.text(0.1, 0.20, r'$0 \leq y \leq 1-z$', fontsize=12,
                           color=self.colors['text_formula'], transform=ax_integration.transAxes)
        ax_integration.text(0.1, 0.12, r'$0 \leq z \leq 1$', fontsize=12,
                           color=self.colors['text_formula'], transform=ax_integration.transAxes)
        
        # 底部：应用示例
        ax_examples = fig.add_subplot(gs[1, 1:])
        ax_examples.axis('off')
        ax_examples.text(0.5, 0.90, '四面体积分典型应用', fontsize=16, fontweight='bold', 
                        color=self.colors['text_main'], ha='center', transform=ax_examples.transAxes)
        
        ax_examples.text(0.05, 0.70, '• 体积计算：', fontsize=14, fontweight='bold',
                        color=self.colors['boundary_main'], transform=ax_examples.transAxes)
        ax_examples.text(0.05, 0.55, r'$V = \iiint_{\Omega} 1 \, dV = \int_0^1 \int_0^{1-x} \int_0^{1-x-y} 1 \, dz \, dy \, dx = \frac{1}{6}$',
                        fontsize=12, color=self.colors['text_formula'], transform=ax_examples.transAxes)
        
        ax_examples.text(0.05, 0.35, '• 质心计算：', fontsize=14, fontweight='bold',
                        color=self.colors['boundary_aux'], transform=ax_examples.transAxes)
        ax_examples.text(0.05, 0.20, r'$\bar{x} = \frac{1}{V}\iiint_{\Omega} x \, dV = \frac{1}{\frac{1}{6}} \int_0^1 \int_0^{1-x} \int_0^{1-x-y} x \, dz \, dy \, dx = \frac{1}{4}$',
                        fontsize=12, color=self.colors['text_formula'], transform=ax_examples.transAxes)
        
        plt.tight_layout()
        
        return fig
    
    def draw_cylindrical_volume_element(self, ax, r, theta, z, dr, dtheta, dz):
        """绘制柱坐标体积元素"""
        # 简化：只绘制关键边
        # 内弧
        theta_arc = np.linspace(theta, theta+dtheta, 20)
        x_inner = r * np.cos(theta_arc)
        y_inner = r * np.sin(theta_arc)
        z_inner = np.full_like(theta_arc, z)
        ax.plot(x_inner, y_inner, z_inner, color=self.colors['volume_edge'], 
                linewidth=2, alpha=0.8)
        
        # 外弧
        x_outer = (r+dr) * np.cos(theta_arc)
        y_outer = (r+dr) * np.sin(theta_arc)
        z_outer = np.full_like(theta_arc, z)
        ax.plot(x_outer, y_outer, z_outer, color=self.colors['volume_edge'], 
                linewidth=2, alpha=0.8)
        
        # 径向边
        x_rad1 = [r*np.cos(theta), (r+dr)*np.cos(theta)]
        y_rad1 = [r*np.sin(theta), (r+dr)*np.sin(theta)]
        z_rad1 = [z, z]
        ax.plot(x_rad1, y_rad1, z_rad1, color=self.colors['volume_edge'], 
                linewidth=2, alpha=0.8)
        
        x_rad2 = [r*np.cos(theta+dtheta), (r+dr)*np.cos(theta+dtheta)]
        y_rad2 = [r*np.sin(theta+dtheta), (r+dr)*np.sin(theta+dtheta)]
        z_rad2 = [z, z]
        ax.plot(x_rad2, y_rad2, z_rad2, color=self.colors['volume_edge'], 
                linewidth=2, alpha=0.8)
    
    def draw_spherical_volume_element(self, ax, r, phi, theta, dr, dphi, dtheta):
        """绘制球坐标体积元素"""
        # 简化：绘制球坐标体积元素的关键特征线
        
        # φ方向的弧线
        phi_arc = np.linspace(phi, phi+dphi, 20)
        x_phi = r * np.sin(phi_arc) * np.cos(theta)
        y_phi = r * np.sin(phi_arc) * np.sin(theta)
        z_phi = r * np.cos(phi_arc)
        ax.plot(x_phi, y_phi, z_phi, color=self.colors['volume_edge'], 
                linewidth=1.5, alpha=0.8)
        
        # θ方向的弧线
        theta_arc = np.linspace(theta, theta+dtheta, 20)
        x_theta = r * np.sin(phi) * np.cos(theta_arc)
        y_theta = r * np.sin(phi) * np.sin(theta_arc)
        z_theta = r * np.cos(phi) * np.ones_like(theta_arc)
        ax.plot(x_theta, y_theta, z_theta, color=self.colors['volume_edge'], 
                linewidth=1.5, alpha=0.8)
        
        # r方向的直线
        r_line = np.linspace(r, r+dr, 20)
        x_r = r_line * np.sin(phi) * np.cos(theta)
        y_r = r_line * np.sin(phi) * np.sin(theta)
        z_r = r_line * np.cos(phi)
        ax.plot(x_r, y_r, z_r, color=self.colors['volume_edge'], 
                linewidth=1.5, alpha=0.8)

    def test_font_display(self):
        """测试字体显示效果"""
        print("🔤 测试中文字体显示...")
        
        # 创建简单测试图
        fig, ax = plt.subplots(figsize=(8, 6))
        ax.text(0.5, 0.8, '三重积分测试', fontsize=16, ha='center', transform=ax.transAxes)
        ax.text(0.5, 0.6, r'数学公式: $\iiint_{\Omega} f(x,y,z) \, dV$', fontsize=14, ha='center', transform=ax.transAxes)
        ax.text(0.5, 0.4, '希腊字母: α β γ δ θ φ ψ ω', fontsize=12, ha='center', transform=ax.transAxes)
        ax.text(0.5, 0.2, '特殊符号: ∑ ∫ ∂ ∇ ∞ ≤ ≥', fontsize=12, ha='center', transform=ax.transAxes)
        ax.axis('off')
        
        test_path = '../../Assets/font_test.png'
        fig.savefig(test_path, dpi=150, bbox_inches='tight')
        plt.close(fig)
        print(f"  字体测试图像已保存: {test_path}")

    def generate_all_elegant_figures(self):
        """生成所有优雅的三重积分图像"""
        print("🚀 开始生成三重积分图像系统V2.0 Elegant...")
        
        # 测试字体
        self.test_font_display()
        
        # 删除旧图像
        import os
        old_files = [
            'triple_integral_v2_basic_concept_enhanced.png',
            'triple_integral_v2_coordinates_enhanced.png',
            'triple_integral_v2_methods_enhanced.png',
            'triple_integral_v2_basic_concept_elegant.png'  # 删除旧的elegant版本重新生成
        ]
        
        print("🗑️ 清理旧图像文件...")
        for file in old_files:
            file_path = f'../../Assets/{file}'
            if os.path.exists(file_path):
                os.remove(file_path)
                print(f"  已删除: {file}")
        
        print("📚 生成无乱码优雅版图像...")
        
        # 基础概念图 - 无乱码优雅版
        print("  1. 生成无乱码基础概念图...")
        fig1 = self.create_elegant_basic_concept_figure()
        fig1.savefig('../../Assets/triple_integral_v2_basic_concept_elegant.png', 
                    dpi=300, bbox_inches='tight', facecolor='white', edgecolor='none')
        plt.close(fig1)
        
        # 坐标系对比图 - 优雅版
        print("  2. 生成优雅版坐标系对比图...")
        fig2 = self.create_elegant_coordinate_comparison()
        fig2.savefig('../../Assets/triple_integral_v2_coordinates_elegant.png', 
                    dpi=300, bbox_inches='tight', facecolor='white', edgecolor='none')
        plt.close(fig2)
        
        # 方法对比图 - 优雅版
        print("  3. 生成优雅版方法对比图...")
        fig3 = self.create_elegant_method_comparison()
        fig3.savefig('../../Assets/triple_integral_v2_methods_elegant.png', 
                    dpi=300, bbox_inches='tight', facecolor='white', edgecolor='none')
        plt.close(fig3)
        
        # 柱坐标系图 - 优雅版（替换原有图像）
        print("  4. 生成优雅版柱坐标系图...")
        fig4 = self.create_elegant_cylindrical_coord()
        fig4.savefig('../../Assets/cylindrical_coord.png', 
                    dpi=300, bbox_inches='tight', facecolor='white', edgecolor='none')
        plt.close(fig4)
        
        # 球坐标系图 - 优雅版（替换原有图像）
        print("  5. 生成优雅版球坐标系图...")
        fig5 = self.create_elegant_spherical_coord()
        fig5.savefig('../../Assets/spherical_coord.png', 
                    dpi=300, bbox_inches='tight', facecolor='white', edgecolor='none')
        plt.close(fig5)
        
        # 四面体例题图 - 优雅版（替换原有图像）
        print("  6. 生成优雅版四面体例题图...")
        fig6 = self.create_elegant_tetrahedron_example()
        fig6.savefig('../../Assets/tetrahedron_example.png', 
                    dpi=300, bbox_inches='tight', facecolor='white', edgecolor='none')
        plt.close(fig6)
        
        print("✅ 三重积分图像系统V2.0 Elegant全面重构完成！")
        print("📁 所有优雅版图像已保存到Assets/目录")
        print("🎯 完整图像列表：")
        print("  1. triple_integral_v2_basic_concept_elegant.png - 基础概念图")
        print("  2. triple_integral_v2_coordinates_elegant.png - 坐标系对比图")
        print("  3. triple_integral_v2_methods_elegant.png - 方法对比图")
        print("  4. cylindrical_coord.png - 柱坐标系图（新优化）")
        print("  5. spherical_coord.png - 球坐标系图（新优化）")
        print("  6. tetrahedron_example.png - 四面体例题图（新优化）")
        print("🎨 主要优化：")
        print("  • 解决了中文乱码和文字重叠问题")
        print("  • 使用分段式LaTeX公式布局，避免特殊字符乱码")
        print("  • 优雅柔和的配色方案，视觉效果温和舒适")
        print("  • 简化体积分割显示，用典型dx dy dz元素展示概念")
        print("  • 完美的中文字体支持和清晰的文字排版")
        print("  • 文字外置布局，符合数学作图标准规范")
        print("  • 所有图像质量高，文件大小优化合理")

if __name__ == "__main__":
    visualizer = TripleIntegralVisualizerV2Elegant()
    visualizer.generate_all_elegant_figures()
