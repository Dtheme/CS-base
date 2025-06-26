#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# created by Dzw 2025-06-25
#
# demo usage

from generate_universal_math_plots import UniversalMathPlotter

def main():
    """主函数：演示各种使用方法"""
    
    print("=== 通用数学绘图工具使用示例 ===\n")
    
    # 示例1：创建英文学术风格的绘图器
    print("1. 创建英文学术风格绘图器...")
    plotter_en = UniversalMathPlotter(language='en', style='academic')
    
    # 生成特定图形
    print("   - 生成柱坐标系图形")
    plotter_en.plot_cylindrical_coordinates('example_cylindrical.png')
    
    print("   - 生成球坐标系图形")
    plotter_en.plot_spherical_coordinates('example_spherical.png')
    
    # 示例2：创建中文现代风格的绘图器
    print("\n2. 创建中文现代风格绘图器...")
    plotter_zh = UniversalMathPlotter(language='zh', style='modern')
    
    # 使用自定义方法
    print("   - 生成X型区域图形（中文版）")
    plotter_zh.create_custom_plot('x_type', save_name='x型区域_中文.png')
    
    # 示例3：批量生成所有图形
    print("\n3. 批量生成所有图形...")
    plotter_batch = UniversalMathPlotter(language='en', style='academic')
    plotter_batch.generate_all_plots(prefix='demo_')
    
    # 示例4：展示不同的自定义选项
    print("\n4. 演示所有可用的图形类型...")
    
    # 所有可用的图形类型
    plot_types = {
        'x_type': 'X型区域',
        'y_type': 'Y型区域',
        'cylindrical': '柱坐标系',
        'spherical': '球坐标系',
        'triple_region': '三重积分区域',
        'coord_comparison': '坐标系比较'
    }
    
    plotter_demo = UniversalMathPlotter(language='en', style='academic')
    
    for plot_type, description in plot_types.items():
        print(f"   - 生成{description}图形: {plot_type}")
        try:
            plotter_demo.create_custom_plot(plot_type, save_name=f'demo_{plot_type}.png')
            print(f"     ✅ 成功生成 demo_{plot_type}.png")
        except Exception as e:
            print(f"     ❌ 生成失败: {e}")
    
    print("\n=== 使用示例完成 ===")
    print("所有生成的图片都保存在 ../../Assets/ 目录下")
    
    # 示例5：展示如何在实际项目中使用
    print("\n5. 实际项目中的使用模式...")
    
    def generate_chapter_plots(chapter_name, language='en'):
        """为特定章节生成图形的函数"""
        plotter = UniversalMathPlotter(language=language, style='academic')
        
        if 'double_integral' in chapter_name:
            plotter.plot_x_type_region(f'{chapter_name}_x_type.png')
            plotter.plot_y_type_region(f'{chapter_name}_y_type.png')
        elif 'triple_integral' in chapter_name:
            plotter.plot_cylindrical_coordinates(f'{chapter_name}_cylindrical.png')
            plotter.plot_spherical_coordinates(f'{chapter_name}_spherical.png')
            plotter.plot_triple_integral_region(f'{chapter_name}_region.png')
        
        print(f"   ✅ 已为章节 {chapter_name} 生成相关图形")
    
    # 为不同章节生成图形
    generate_chapter_plots('7.1_double_integral', 'en')
    generate_chapter_plots('7.2_triple_integral', 'en')
    
    print("\n📝 提示：")
    print("1. 在实际使用中，建议根据需要选择合适的语言和风格")
    print("2. 生成的图片默认为300 DPI高质量，适合打印和文档使用")
    print("3. 如需自定义图形，可以继承UniversalMathPlotter类并添加新方法")
    print("4. 所有图形都采用学术友好的配色方案，确保专业性")

if __name__ == "__main__":
    main() 