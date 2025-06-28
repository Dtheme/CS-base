# 第7章重积分图片生成说明

本目录包含第7章（重积分）所有图片的生成脚本。

## 文件结构

### 生成脚本
- `chapter7_figure_generator.py` - 主要图片生成脚本
  - 二重积分几何意义综合图
  - X型和Y型区域示意图
  - 积分次序比较图
  - 抛物线和三角形区域图

- `coordinate_systems_optimizer.py` - 坐标系图片优化脚本（最新 - 简洁专业风格）
  - 柱坐标系示意图（简洁几何体风格）
  - 球坐标系示意图（简洁几何体风格）
  - 坐标系比较图（专业数学插图风格）
  - 去除复杂网格，突出关键几何特征
  - 采用类似教科书的简洁专业表达

- `chapter7_coord_generator.py` - 坐标系详解图片
  - 柱坐标系示意图
  - 球坐标系示意图
  - 坐标系比较图
  - 极坐标应用例题图

## 生成的图片清单

### 二重积分相关
1. `double_integral_geometry.png` - 二重积分几何意义综合图
2. `x_type_region.png` - X型区域示意图
3. `y_type_region.png` - Y型区域示意图
4. `integration_order.png` - 积分次序比较图
5. `region_parabola.png` - 抛物线区域图
6. `region_triangle.png` - 三角形区域图

### 极坐标变换相关
7. `polar_transform.png` - 极坐标变换示意图
8. `polar_example_4_1.png` - 圆形区域的极坐标积分
9. `polar_example_4_2.png` - 环形扇区的极坐标积分
10. `polar_example_7_2.png` - 立体体积的极坐标计算
11. `polar_example_7_4.png` - 单位圆盘的转动惯量计算

### 三重积分相关
12. `triple_integral_regions.png` - 三重积分区域示意图
13. `tetrahedron_example.png` - 四面体积分区域示例
14. `cylindrical_coord.png` - 柱坐标系示意图（简洁专业风格）
15. `spherical_coord.png` - 球坐标系示意图（简洁专业风格）
16. `coord_comparison.png` - 三种坐标系对比图（简洁专业风格）

## 使用说明

### 生成所有图片
```bash
# 生成基础重积分图片
python chapter7_figure_generator.py

# 生成坐标系图片（最新简洁风格）
python coordinate_systems_optimizer.py

# 生成详细坐标系图片
python chapter7_coord_generator.py
```

### 单独生成特定图片
在脚本中注释掉不需要的函数调用即可。

## 设计风格演进

### 第一版（chapter7_figure_generator.py）
- 基础的数学图形生成
- 标准的matplotlib默认风格

### 第二版（chapter7_coord_generator.py）
- 增加了复杂的网格和线框
- 详细的坐标系表示
- 丰富的视觉元素

### 第三版（coordinate_systems_optimizer.py - 当前推荐）
- **简洁专业的几何体风格**
- 去除复杂网格和背景
- 突出关键几何特征（立方体、圆柱体、球体）
- 采用教科书级别的专业数学插图风格
- 清晰的标注和配色
- 文件大小优化（更高效）

## 技术特点

- 支持中文标注
- 高分辨率输出（300 DPI）
- 白色背景，适合文档嵌入
- 标准化的图片尺寸和视角
- 专业的数学符号渲染

## 更新日志

- **2024-06-27 v3.0**: 采用简洁专业风格，去除复杂网格，突出几何特征
- **2024-06-27 v2.1**: 修复文字重合问题，优化布局
- **2024-06-27 v2.0**: 增加坐标系变换图片
- **2024-06-26 v1.0**: 初始版本，基础重积分图片 