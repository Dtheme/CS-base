#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
数学解题作图标准模板
基于例题3.5优化经验制定的通用模板

使用方法：
1. 导入此模板：from 数学作图标准模板 import MathFigureTemplate
2. 创建实例：template = MathFigureTemplate()
3. 定制化配置
4. 生成图像

版本：1.0
更新：基于例题3.5优化经验
"""

import matplotlib.pyplot as plt
import matplotlib.font_manager as fm
import numpy as np
import os

class MathFigureTemplate:
    """数学解题作图标准模板类"""
    
    def __init__(self):
        """初始化模板配置"""
        self.setup_matplotlib()
        self.setup_colors()
        self.setup_layout()
        
    def setup_matplotlib(self):
        """配置matplotlib中文显示"""
        plt.rcParams['font.sans-serif'] = ['Arial Unicode MS', 'SimHei', 'DejaVu Sans']
        plt.rcParams['axes.unicode_minus'] = False
        
    def setup_colors(self):
        """设置标准配色方案"""
        # 主色调（优雅柔和但对比清晰）
        self.colors = {
            'curve1': '#2E5BBA',      # 深海蓝 - 主曲线
            'curve2': '#C8102E',      # 深红 - 辅助曲线  
            'vertical': '#228B22',    # 森林绿 - 垂直线
            'region1': '#FFE4E6',     # 淡玫瑰 - 区域1
            'region2': '#E6F3FF',     # 淡天蓝 - 区域2
            'demo1': '#FF69B4',       # 热粉 - 示例线1
            'demo2': '#4169E1',       # 皇家蓝 - 示例线2
            'point': '#333333',       # 深灰 - 关键点
        }
        
        # 文字框配色
        self.text_colors = {
            'expression': {'face': '#F8F9FA', 'edge': '#DEE2E6'},
            'analysis': {'face': '#D4F7D4', 'edge': '#28A745'}, 
            'relation': {'face': '#E8E8F5', 'edge': '#6A6A9A'},
        }
        
    def setup_layout(self):
        """设置布局参数"""
        self.layout = {
            'figsize': (16, 12),
            'xlim': (-1.2, 6.0),
            'ylim': (-1.2, 19.5),
            'title_pad': 20,
            'title_fontsize': 19,
            'axis_fontsize': 17,
            'text_fontsize': {
                'main': 14,
                'expression': 12,
                'analysis': 11,
                'relation': 12
            }
        }
        
        # 关键点标记位置模板（四角分布策略）
        self.point_positions = {
            'left_bottom': (-0.8, -0.6),    # 左下角
            'right_middle': (1.4, 0.4),     # 右侧中部
            'right_space': (5.2, 1.5),      # 右侧空白
            'left_top': (3.8, 17.5),        # 左上方
        }
        
        # 文字分层位置
        self.text_positions = {
            'expression_title': (-1.1, 18.0),
            'expression_content': (-1.1, 17.0),
            'analysis': (-1.1, 15.0),
            'relation': (-1.1, 12.5),
        }
    
    def create_figure(self, with_origin=True):
        """创建标准图形和坐标系
        
        Args:
            with_origin: 是否显示x-y-o坐标系布局
        """
        fig, ax = plt.subplots(1, 1, figsize=self.layout['figsize'])
        
        # 设置坐标轴
        ax.set_xlim(self.layout['xlim'])
        ax.set_ylim(self.layout['ylim'])
        ax.set_xlabel('x', fontsize=self.layout['axis_fontsize'], 
                     weight='bold', color='#333333')
        ax.set_ylabel('y', fontsize=self.layout['axis_fontsize'], 
                     weight='bold', color='#333333')
        
        # 设置网格
        ax.grid(True, alpha=0.3, linestyle='-', linewidth=0.8)
        
        # x-y-o坐标系布局
        if with_origin:
            self.setup_origin_layout(ax)
        
        return fig, ax
    
    def setup_origin_layout(self, ax):
        """设置x-y-o坐标系布局，确保原点(0,0)清晰可见"""
        # x轴和y轴
        ax.axhline(y=0, color='black', linewidth=1.0, alpha=0.8, zorder=3)
        ax.axvline(x=0, color='black', linewidth=1.0, alpha=0.8, zorder=3)
        
        # 原点标记
        ax.plot(0, 0, 'ko', markersize=6, zorder=5)
        
        # 原点标签
        ax.text(0.15, -0.4, 'O', fontsize=14, weight='bold', 
               color='#333333', zorder=6)
        
        # 坐标轴箭头（可选）
        xlim = ax.get_xlim()
        ylim = ax.get_ylim()
        
        if xlim[1] > 1:  # 只有当x轴范围足够大时才添加箭头
            ax.annotate('', xy=(xlim[1]*0.95, 0), xytext=(xlim[1]*0.85, 0),
                       arrowprops=dict(arrowstyle='->', color='black', lw=1.2))
        
        if ylim[1] > 1:  # 只有当y轴范围足够大时才添加箭头
            ax.annotate('', xy=(0, ylim[1]*0.95), xytext=(0, ylim[1]*0.85),
                       arrowprops=dict(arrowstyle='->', color='black', lw=1.2))
        
    def plot_curves(self, ax, x_data, functions, labels):
        """绘制函数曲线
        
        Args:
            ax: matplotlib轴对象
            x_data: x坐标数据
            functions: 函数列表 [y1_data, y2_data, ...]
            labels: 标签列表 ['label1', 'label2', ...]
        """
        curve_colors = [self.colors['curve1'], self.colors['curve2']]
        
        for i, (y_data, label) in enumerate(zip(functions, labels)):
            color = curve_colors[i % len(curve_colors)]
            ax.plot(x_data, y_data, color=color, linewidth=3.5, 
                   label=label, zorder=5)
    
    def plot_vertical_line(self, ax, x_value, label):
        """绘制垂直线"""
        ax.axvline(x=x_value, color=self.colors['vertical'], 
                  linewidth=3.5, label=label, zorder=5)
    
    def fill_regions(self, ax, regions_data):
        """填充积分区域
        
        Args:
            ax: matplotlib轴对象
            regions_data: 区域数据列表，每个元素包含:
                {'x': x_range, 'y_lower': y_lower, 'y_upper': y_upper, 
                 'color_key': 'region1', 'label': 'label'}
        """
        region_colors = [self.colors['region1'], self.colors['region2']]
        edge_colors = [self.colors['curve2'], self.colors['curve1']]
        
        for i, region in enumerate(regions_data):
            color = region_colors[i % len(region_colors)]
            edge_color = edge_colors[i % len(edge_colors)]
            
            ax.fill_between(region['x'], region['y_lower'], region['y_upper'],
                           alpha=0.6, color=color, edgecolor=edge_color,
                           linewidth=1.5, label=region['label'], zorder=2)
    
    def add_key_points(self, ax, points_data):
        """添加关键点标记
        
        Args:
            ax: matplotlib轴对象  
            points_data: 点数据列表，每个元素包含:
                {'point': (x, y), 'label': 'label', 'position_key': 'left_bottom'}
        """
        for point_data in points_data:
            point = point_data['point']
            label = point_data['label']
            position = self.point_positions[point_data['position_key']]
            
            # 绘制点
            ax.plot(point[0], point[1], 'o', color=self.colors['point'],
                   markersize=10, markeredgecolor='white', 
                   markeredgewidth=2, zorder=10)
            
            # 添加标注
            ax.annotate(label, point, xytext=position, fontsize=12, 
                       weight='bold',
                       bbox=dict(boxstyle='round,pad=0.3', facecolor='white',
                                edgecolor='gray', alpha=0.9),
                       arrowprops=dict(arrowstyle='->', color='gray', lw=1.5))
    
    def add_demo_lines(self, ax, demo_data):
        """添加示例线
        
        Args:
            ax: matplotlib轴对象
            demo_data: 示例线数据列表，每个元素包含:
                {'x': x_value, 'y_range': (y_lower, y_upper), 'color_key': 'demo1'}
        """
        demo_colors = [self.colors['demo1'], self.colors['demo2']]
        light_colors = ['#FFF0F5', '#F0F8FF']
        
        for i, demo in enumerate(demo_data):
            color = demo_colors[i % len(demo_colors)]
            light_color = light_colors[i % len(light_colors)]
            x_val = demo['x']
            y_lower, y_upper = demo['y_range']
            
            # 虚线
            ax.axvline(x=x_val, color=color, linewidth=2, 
                      linestyle='--', alpha=0.8, zorder=3)
            
            # 积分区间线段
            ax.plot([x_val, x_val], [y_lower, y_upper], 
                   color=color, linewidth=5, alpha=0.8, zorder=4)
            
            # 底部标注
            ax.annotate(f'x = {x_val}', (x_val, -0.7), 
                       xytext=(x_val + 0.2 + i*0.2, -0.3),
                       fontsize=11, color=color, weight='bold',
                       bbox=dict(boxstyle='round,pad=0.25', 
                                facecolor=light_color, 
                                edgecolor=color, alpha=0.9),
                       arrowprops=dict(arrowstyle='->', color=color, lw=1.2))
    
    def set_title(self, ax, title):
        """设置标题"""
        ax.set_title(title, fontsize=self.layout['title_fontsize'],
                    pad=self.layout['title_pad'], weight='bold', 
                    color='#2C3E50')
    
    def set_legend(self, ax):
        """设置图例"""
        legend = ax.legend(loc='upper right', bbox_to_anchor=(0.98, 0.98),
                          fontsize=12, framealpha=0.95, facecolor='white',
                          edgecolor='gray', frameon=True)
        legend.get_frame().set_linewidth(1.2)
    
    def add_text_content(self, ax, text_data):
        """添加文字内容
        
        Args:
            ax: matplotlib轴对象
            text_data: 文字数据字典，包含:
                {'expression': {'title': str, 'content': str},
                 'analysis': str,
                 'relation': str}
        """
        # 积分表达式
        if 'expression' in text_data:
            expr = text_data['expression']
            ax.text(*self.text_positions['expression_title'], 
                   expr['title'], fontsize=self.layout['text_fontsize']['main'],
                   weight='bold', color='#2C3E50',
                   bbox=dict(boxstyle='round,pad=0.4', 
                            facecolor=self.text_colors['expression']['face'],
                            edgecolor=self.text_colors['expression']['edge'],
                            alpha=0.95))
            
            ax.text(*self.text_positions['expression_content'],
                   expr['content'], fontsize=self.layout['text_fontsize']['expression'],
                   color='#2C3E50',
                   bbox=dict(boxstyle='round,pad=0.4',
                            facecolor=self.text_colors['expression']['face'],
                            edgecolor=self.text_colors['expression']['edge'],
                            alpha=0.95))
        
        # 区域分析
        if 'analysis' in text_data:
            ax.text(*self.text_positions['analysis'],
                   text_data['analysis'], 
                   fontsize=self.layout['text_fontsize']['analysis'],
                   weight='bold', color='#155724',
                   bbox=dict(boxstyle='round,pad=0.5',
                            facecolor=self.text_colors['analysis']['face'],
                            edgecolor=self.text_colors['analysis']['edge'],
                            alpha=0.9),
                   verticalalignment='top')
        
        # 函数关系
        if 'relation' in text_data:
            ax.text(*self.text_positions['relation'],
                   text_data['relation'],
                   fontsize=self.layout['text_fontsize']['relation'],
                   color='#5A5A8A', weight='bold',
                   bbox=dict(boxstyle='round,pad=0.4',
                            facecolor=self.text_colors['relation']['face'],
                            edgecolor=self.text_colors['relation']['edge'],
                            alpha=0.9))
    
    def save_figure(self, fig, filename, output_dir='../../Assets'):
        """保存图像"""
        output_path = os.path.join(output_dir, filename)
        fig.tight_layout()
        fig.savefig(output_path, dpi=300, bbox_inches='tight', 
                   facecolor='white', edgecolor='none')
        
        # 获取文件大小
        file_size = os.path.getsize(output_path) / 1024  # KB
        
        print(f'成功生成数学图像：{output_path}')
        print(f'文件大小：{file_size:.1f} KB')
        
        return output_path
    
    def quality_check(self):
        """质量检查清单"""
        checklist = [
            "所有文字、图例、标注无遮挡",
            "关键点标记位置合理，不与曲线重叠", 
            "图例位置固定在右上角",
            "文字内容分层清晰，重点突出",
            "配色协调统一，对比清晰",
            "坐标轴范围合理，预留足够空白",
            "数学表达式格式正确",
            "整体布局美观协调"
        ]
        
        print("\n📋 质量检查清单：")
        for i, item in enumerate(checklist, 1):
            print(f"{i}. [ ] {item}")
        
        print("\n💡 优化建议：")
        print("- 优先使用空白区域放置辅助信息")
        print("- 保持图像内容简洁，专注解题思路") 
        print("- 定期检查不同分辨率下的显示效果")
        print("- 统一使用标准配色方案")


def demo_usage():
    """演示模板使用方法"""
    print("🎨 数学作图标准模板演示")
    print("=" * 50)
    
    # 创建模板实例
    template = MathFigureTemplate()
    
    # 创建图形
    fig, ax = template.create_figure()
    
    # 示例数据
    x = np.linspace(0.05, 5, 1000)
    y1 = x**2
    y2 = np.sqrt(x)
    
    # 绘制曲线
    template.plot_curves(ax, x, [y1, y2], [r'$y = x^2$', r'$y = \sqrt{x}$'])
    
    # 绘制垂直线
    template.plot_vertical_line(ax, 4, r'$x = 4$')
    
    # 填充区域
    x1 = np.linspace(0.05, 1, 200)
    x2 = np.linspace(1, 4, 300)
    regions = [
        {'x': x1, 'y_lower': x1**2, 'y_upper': np.sqrt(x1), 'label': r'区域1: $x \in [0,1]$'},
        {'x': x2, 'y_lower': np.sqrt(x2), 'y_upper': x2**2, 'label': r'区域2: $x \in [1,4]$'}
    ]
    template.fill_regions(ax, regions)
    
    # 添加关键点
    points = [
        {'point': (0, 0), 'label': '(0, 0)', 'position_key': 'left_bottom'},
        {'point': (1, 1), 'label': '(1, 1)', 'position_key': 'right_middle'},
        {'point': (4, 2), 'label': '(4, 2)', 'position_key': 'right_space'},
        {'point': (4, 16), 'label': '(4, 16)', 'position_key': 'left_top'}
    ]
    template.add_key_points(ax, points)
    
    # 添加示例线
    demo_lines = [
        {'x': 0.5, 'y_range': (0.5**2, np.sqrt(0.5)), 'color_key': 'demo1'},
        {'x': 2.5, 'y_range': (np.sqrt(2.5), 2.5**2), 'color_key': 'demo2'}
    ]
    template.add_demo_lines(ax, demo_lines)
    
    # 设置标题
    template.set_title(ax, r'数学作图标准模板演示：由 $y = x^2$, $y = \sqrt{x}$, $x = 4$ 围成的积分区域')
    
    # 设置图例
    template.set_legend(ax)
    
    # 添加文字内容
    text_content = {
        'expression': {
            'title': r'积分表达式:',
            'content': r'$\iint_D (x+y) \, dA = \int_0^1 \int_{x^2}^{\sqrt{x}} (x+y) \, dy \, dx + \int_1^4 \int_{\sqrt{x}}^{x^2} (x+y) \, dy \, dx$'
        },
        'analysis': """区域分析:
区域1: $x \\in [0,1]$, 积分限 $x^2 \\leq y \\leq \\sqrt{x}$
区域2: $x \\in [1,4]$, 积分限 $\\sqrt{x} \\leq y \\leq x^2$""",
        'relation': r'函数关系: $x \in [0,1]$ 时 $\sqrt{x} \geq x^2$；$x \in [1,4]$ 时 $x^2 \geq \sqrt{x}$'
    }
    template.add_text_content(ax, text_content)
    
    # 保存图像
    output_path = template.save_figure(fig, 'template_demo.png')
    
    # 质量检查
    template.quality_check()
    
    return output_path


if __name__ == "__main__":
    demo_usage() 