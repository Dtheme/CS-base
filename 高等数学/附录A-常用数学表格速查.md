# 附录A：常用数学表格速查

## 目录

### 基础篇
1. [等价无穷小表](#1-等价无穷小表)
2. [重要极限表](#2-重要极限表)
   - [2.6 常用极限结论表](#26-常用极限结论表)
3. [基本初等函数导数表](#3-基本初等函数导数表)
4. [基本积分表](#4-基本积分表)
5. [三角函数恒等式表](#5-三角函数恒等式表)

### 提高篇
6. [高阶导数表](#6-高阶导数表)
7. [常用泰勒级数展开表](#7-常用泰勒级数展开表)
8. [特殊积分表](#8-特殊积分表)

### 高级篇
9. [多元函数相关表](#9-多元函数相关表)
10. [曲线积分与曲面积分表](#10-曲线积分与曲面积分表)
11. [常微分方程解表](#11-常微分方程解表)
12. [无穷级数判定表](#12-无穷级数判定表)

###  专题篇
13. [向量运算表](#13-向量运算表)
14. [傅里叶级数表](#14-傅里叶级数表)
15. [特殊函数表](#15-特殊函数表)
16. [数值计算表](#16-数值计算表)

---

##  基础篇

## 1. 等价无穷小表

### 1.1 基础等价无穷小（ $x \to 0$）

#### **三角函数类**
| 函数 | 等价无穷小 | 记忆要点 | 重要程度 |
|:----:|:----------:|:--------|:--------:|
| $\sin x$ | $x$ | 最基础的等价无穷小 | ⭐⭐⭐⭐⭐ |
| $\tan x$ | $x$ | 当角度很小时 | ⭐⭐⭐⭐⭐ |
| $\arcsin x$ | $x$ | 反正弦函数 | ⭐⭐⭐⭐ |
| $\arctan x$ | $x$ | 反正切函数 | ⭐⭐⭐⭐ |
| $1-\cos x$ | $\frac{1}{2}x^2$ | 经典二阶公式 | ⭐⭐⭐⭐⭐ |
| $\sec x - 1$ | $\frac{1}{2}x^2$ | $\sec x = \frac{1}{\cos x}$ | ⭐⭐⭐ |
| $1 - \cos^n x$ | $\frac{n}{2}x^2$ | 一般情况， $n > 0$ | ⭐⭐⭐ |

#### **指数对数类**
| 函数 | 等价无穷小 | 记忆要点 | 重要程度 |
|:----:|:----------:|:--------|:--------:|
| $e^x-1$ | $x$ | 自然指数基础公式 | ⭐⭐⭐⭐⭐ |
| $a^x-1$ | $x\ln a$ | 一般指数， $a>0,a \neq 1$ | ⭐⭐⭐⭐ |
| $\ln(1+x)$ | $x$ | 自然对数基础公式 | ⭐⭐⭐⭐⭐ |
| $\log_a(1+x)$ | $\frac{x}{\ln a}$ | 一般对数， $a>0,a \neq 1$ | ⭐⭐⭐ |
| $(1+x)^α-1$ | $αx$ | 广义二项式， $α \neq 0$ | ⭐⭐⭐⭐ |
| $\ln(\cos x)$ | $-\frac{1}{2}x^2$ | 对数三角复合 | ⭐⭐⭐⭐ |

#### **根式函数类**
| 函数 | 等价无穷小 | 记忆要点 | 重要程度 |
|:----:|:----------:|:--------|:--------:|
| $\sqrt{1+x}-1$ | $\frac{1}{2}x$ | $n=2$ 情况 | ⭐⭐⭐⭐ |
| $\sqrt[3]{1+x}-1$ | $\frac{1}{3}x$ | $n=3$ 情况 | ⭐⭐⭐ |
| $\sqrt[n]{1+x}-1$ | $\frac{1}{n}x$ | 一般情况， $n \neq 0$ | ⭐⭐⭐ |
| $\sqrt{1+x^2}-1$ | $\frac{1}{2}x^2$ | 复合形式 | ⭐⭐⭐ |
| $\sqrt{1+x} - \sqrt{1-x}$ | $x$ | 有理化常用 | ⭐⭐⭐⭐ |

#### **双曲函数类**
| 函数 | 等价无穷小 | 记忆要点 | 重要程度 |
|:----:|:----------:|:--------|:--------:|
| $\sinh x$ | $x$ | $\sinh x = \frac{e^x - e^{-x}}{2}$ | ⭐⭐⭐ |
| $\tanh x$ | $x$ | $\tanh x = \frac{\sinh x}{\cosh x}$ | ⭐⭐⭐ |
| $\cosh x - 1$ | $\frac{1}{2}x^2$ | $\cosh x = \frac{e^x + e^{-x}}{2}$ | ⭐⭐⭐ |
| $\operatorname{arcsinh} x$ | $x$ | 反双曲正弦 | ⭐⭐ |
| $\operatorname{arctanh} x$ | $x$ | 反双曲正切， $\|x\| < 1$ | ⭐⭐ |

### 1.2 高阶等价无穷小（差值形式）

| 函数 | 等价无穷小 | 阶数 | 应用场景 |
|:----:|:----------:|:----:|:--------|
| $\sin x - x$ | $-\frac{1}{6}x^3$ | 三阶 | 一阶项相消时使用 |
| $\tan x - x$ | $\frac{1}{3}x^3$ | 三阶 | 一阶项相消时使用 |
| $x - \sin x$ | $\frac{1}{6}x^3$ | 三阶 | 注意符号 |
| $\tan x - \sin x$ | $\frac{1}{2}x^3$ | 三阶 | 常考形式 |
| $\ln(1+x) - x$ | $-\frac{1}{2}x^2$ | 二阶 | 二阶修正项 |
| $e^x - 1 - x$ | $\frac{1}{2}x^2$ | 二阶 | 二阶修正项 |
| $\arcsin x - x$ | $\frac{1}{6}x^3$ | 三阶 | 反三角函数差值 |
| $x - \arctan x$ | $\frac{1}{3}x^3$ | 三阶 | 反三角函数差值 |
| $\sec x - \cos x$ | $x^2$ | 二阶 | 三角函数差值 |
| $\arcsin x - \arctan x$ | $\frac{1}{6}x^3$ | 三阶 | 反函数差 |

### 1.3 复合函数等价无穷小

| 函数 | 等价无穷小 | 复合规律 |
|:----:|:----------:|:--------|
| $\sin(\tan x)$ | $x$ | 内外层都 $\sim x$ |
| $\tan(\sin x)$ | $x$ | 内外层都 $\sim x$ |
| $e^{\sin x} - 1$ | $x$ | $\sin x \sim x$ |
| $\ln(1 + \tan x)$ | $x$ | $\tan x \sim x$ |
| $e^{x^2} - 1$ | $x^2$ | 复合幂次 |
| $\ln(1 + x^2)$ | $x^2$ | 复合幂次 |
| $\sin(e^x - 1)$ | $x$ | $e^x - 1 \sim x$ |
| $\ln(1 + e^x - 1)$ | $x$ | $e^x - 1 \sim x$ |
| $(1 + \sin x)^{1/2} - 1$ | $\frac{1}{2}x$ | $\sin x \sim x$ |

### 1.4 乘积与比值形式

| 函数 | 等价无穷小 | 记忆要点 |
|:----:|:----------:|:--------|
| $x\cot x - 1$ | $-\frac{1}{3}x^2$ | 乘积形式 |
| $\frac{x}{\sin x} - 1$ | $\frac{1}{6}x^2$ | 比值形式 |
| $\frac{\tan x}{x} - 1$ | $\frac{1}{3}x^2$ | 比值形式 |
| $\frac{x}{\arcsin x} - 1$ | $-\frac{1}{6}x^2$ | 反函数比值 |
| $\frac{x}{\arctan x} - 1$ | $\frac{1}{3}x^2$ | 反函数比值 |

### 1.5 和形式等价无穷小（ $x \to 0$）

| 函数 | 等价无穷小 | 记忆技巧 | 重要程度 |
|:----:|:----------:|:--------|:--------:|
| $\sin x + x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐⭐ |
| $\tan x + x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐⭐ |
| $\arcsin x + x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐ |
| $\arctan x + x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐ |
| $e^x - 1 + x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐⭐ |
| $\ln(1+x) + x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐⭐ |
| $\sin x + \tan x$ | $2x$ | 两个主项相加 | ⭐⭐⭐⭐ |
| $\arcsin x + \arctan x$ | $2x$ | 两个主项相加 | ⭐⭐⭐ |
| $e^x + x$ | $1 + 2x$ | 常数项加线性项 | ⭐⭐⭐⭐ |
| $\cos x + x$ | $1 + x$ | 常数项加线性项 | ⭐⭐⭐⭐ |
| $(1+x)^α + x$ | $1 + (α+1)x$ | 广义情况 | ⭐⭐⭐ |

#### **高频考点：含系数的和形式**
| 函数 | 等价无穷小 | 考试重点 |
|:----:|:----------:|:--------|
| $a\sin x + bx$ | $(a+b)x$ | $a,b$ 为常数 |
| $\sin x + 2x$ | $3x$ | 系数求和 |
| $\tan x + 3x$ | $4x$ | 系数求和 |
| $\sin x + \cos x$ | $1 + x$ | 常数项占主导 |
| $e^x + \ln(1+x)$ | $1 + 2x$ | 不同函数组合 |
| $\sqrt{1+x} + x$ | $1 + \frac{3x}{2}$ | 根式与线性组合 |
| $\sin x + x + x^2$ | $2x$ | 忽略高阶项 |
| $e^{x^2} + \sin x$ | $1 + x$ | 复合函数组合 |

#### **特殊技巧：三角函数和形式**
| 函数 | 等价无穷小 | 技巧说明 |
|:----:|:----------:|:--------|
| $\sin x + \sin 2x$ | $3x$ | $\sin 2x \sim 2x$ |
| $\tan x + \tan 3x$ | $4x$ | $\tan 3x \sim 3x$ |
| $\sin x + \sin x^2$ | $x$ | 忽略高阶 $x^2$ 项 |
| $\cos x + \cos 2x$ | $2$ | 两个常数项 |

### 1.6 变量替换的等价无穷小

#### ** $x \to \infty$ 时的等价无穷小**
| 函数 | 等价无穷小 | 变换说明 |
|:----:|:----------:|:--------|
| $\arctan x - \frac{\pi}{2}$ | $-\frac{1}{x}$ | $x \to +\infty$ |
| $\frac{\pi}{2} - \arctan x$ | $\frac{1}{x}$ | $x \to +\infty$ |
| $x - \ln x$ | $x$ | $x \to +\infty$ |
| $\sqrt{x^2 + 1} - x$ | $\frac{1}{2x}$ | $x \to +\infty$ |

### 1.7 使用原则与技巧

> **重要提醒**：
> 1. **乘除运算**：可直接替换
> 2. **加减运算**：主项相同时可以相加，主项不同时需要保留高阶项
> 3. **复合函数**：内层等价无穷小可以向外传递
> 4. **精度匹配**：选择合适阶数的等价无穷小
> 5. **变量替换**：注意自变量趋向的值
> 6. **和形式处理**：当两个函数主项相同时，可以直接相加

#### **加法形式的处理规则**
| 情况 | 处理方法 | 举例 |
|:----:|:--------:|:-----|
| 主项相同 | 直接相加 | $\sin x + x \sim x + x = 2x$ |
| 主项不同阶 | 保留低阶项 | $x + x^2 \sim x$ （当 $x \to 0$） |
| 需要精确计算 | 用泰勒展开 | 复杂的和差形式 |

#### **常见错误与正确方法**
| 错误用法 | 正确方法 | 说明 |
|:--------:|:--------:|:-----|
| $\sin x - x \sim x - x = 0$ | $\sin x - x \sim -\frac{x^3}{6}$ | 差值不能直接替换 |
| $\ln(1+x) + x^2 \sim x + 0 = x$ | $\ln(1+x) + x^2 \sim x$ | 忽略高阶项 |
| $\tan x + \sin x \sim x + x = 2x$ | $\tan x + \sin x \sim 2x$ | 主项相同时正确 |

---

## 2. 重要极限表

### 2.1 基本重要极限

| 极限 | 值 | 类型 | 记忆技巧 | 重要程度 |
|------|----|----|----------|----------|
| $\lim_{x \to 0} \frac{\sin x}{x}$ | $1$ | $\frac{0}{0}$ 型 | 最基础的重要极限 | ⭐⭐⭐⭐⭐ |
| $\lim_{x \to \infty} \left(1+\frac{1}{x}\right)^x$ | $e$ | $1^{\infty}$ 型 | 自然对数底数定义 | ⭐⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{1-\cos x}{x^2}$ | $\frac{1}{2}$ | $\frac{0}{0}$ 型 | 余弦展开 | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{e^x-1}{x}$ | $1$ | $\frac{0}{0}$ 型 | 指数函数导数 | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{\ln(1+x)}{x}$ | $1$ | $\frac{0}{0}$ 型 | 对数函数导数 | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{(1+x)^α-1}{x}$ | $α$ | $\frac{0}{0}$ 型 | 幂函数导数 | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{a^x-1}{x}$ | $\ln a$ | $\frac{0}{0}$ 型 | 一般指数， $a>0,a≠1$ | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{\tan x}{x}$ | $1$ | $\frac{0}{0}$ 型 | 正切函数 | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{\arcsin x}{x}$ | $1$ | $\frac{0}{0}$ 型 | 反正弦函数 | ⭐⭐⭐⭐ |
| $\lim_{x \to 0} \frac{\arctan x}{x}$ | $1$ | $\frac{0}{0}$ 型 | 反正切函数 | ⭐⭐⭐⭐ |

### 2.2 数列重要极限

| 极限 | 值 | 应用场景 | 重要程度 |
|------|----|----|----------|
| $\lim_{n \to \infty} \sqrt[n]{n}$ | $1$ | 根式极限 | ⭐⭐⭐⭐ |
| $\lim_{n \to \infty} \sqrt[n]{a}$ | $1$ | $a > 0$，根式极限 | ⭐⭐⭐ |
| $\lim_{n \to \infty} \frac{a^n}{n!}$ | $0$ | 阶乘增长比指数快 | ⭐⭐⭐⭐ |
| $\lim_{n \to \infty} \frac{n^k}{a^n}$ | $0$ | $a > 1$，指数增长比幂快 | ⭐⭐⭐⭐ |
| $\lim_{n \to \infty} \frac{\ln n}{n}$ | $0$ | 对数增长比线性慢 | ⭐⭐⭐⭐ |
| $\lim_{n \to \infty} \left(1+\frac{a}{n}\right)^n$ | $e^a$ | 一般化的 $e$ 定义 | ⭐⭐⭐⭐ |
| $\lim_{n \to \infty} \left(1+\frac{1}{n}\right)^{n+a}$ | $e$ | 指数修正 | ⭐⭐⭐ |
| $\lim_{n \to \infty} \frac{\sin n}{n}$ | $0$ | 有界函数除无穷大 | ⭐⭐⭐ |

### 2.3 函数增长阶比较

| 慢增长 | $\longleftarrow$ | 增长速度 | $\longrightarrow$ | 快增长 |
|:------:|:---------------:|:--------:|:----------------:|:------:|
| $\ln(\ln x)$ | $<$ | $\ln x$ | $<$ | $x^ε$ |
| $x^ε$ | $<$ | $x$ | $<$ | $x \ln x$ |
| $x \ln x$ | $<$ | $x^a$ | $<$ | $e^x$ |
| $e^x$ | $<$ | $x!$ | $<$ | $x^x$ |

> 其中 $ε > 0$ 为任意小正数， $a > 1$

### 2.4 特殊类型极限

#### ** $1^{\infty}$ 型极限**
| 极限形式 | 计算方法 | 结果 |
|---------|----------|------|
| $\lim_{x \to 0} (1+x)^{\frac{1}{x}}$ | 直接应用定义 | $e$ |
| $\lim_{x \to 0} (1+ax)^{\frac{1}{bx}}$ | 变量替换 | $e^{\frac{a}{b}}$ |
| $\lim_{x \to a} [f(x)]^{g(x)}$ | $e^{\lim_{x \to a} g(x)\ln f(x)}$ | 指数对数法 |

#### ** $\frac{\infty}{\infty}$ 型极限**
| 极限 | 值 | 记忆要点 |
|------|----|----|
| $\lim_{x \to \infty} \frac{P_n(x)}{Q_m(x)}$ | 首项系数比（ $n=m$），0（ $n<m$）， $\infty$ （ $n>m$） | 多项式比值 |
| $\lim_{x \to \infty} \frac{a_n x^n + \cdots}{b_m x^m + \cdots}$ | $\frac{a_n}{b_m}$ （ $n=m$） | 只看最高次项 |

### 2.5 三角函数相关极限

| 极限 | 值 | 技巧 |
|------|----|----|
| $\lim_{x \to 0} \frac{\sin ax}{\sin bx}$ | $\frac{a}{b}$ | $b \neq 0$ |
| $\lim_{x \to 0} \frac{\tan ax}{\tan bx}$ | $\frac{a}{b}$ | $b \neq 0$ |
| $\lim_{x \to 0} \frac{\sin ax}{bx}$ | $\frac{a}{b}$ | $b \neq 0$ |
| $\lim_{x \to 0} \frac{1-\cos ax}{x^2}$ | $\frac{a^2}{2}$ | 二阶无穷小 |
| $\lim_{x \to 0} \frac{x - \sin x}{x^3}$ | $\frac{1}{6}$ | 高阶展开 |

### 2.6 常用极限结论表

#### **函数增长速度比较（ $x \to +\infty$）**

> **核心结论**：🚀 **"对数爬行，幂函步行，指数飞行"**

| 比较类型 | 极限结论 | 记忆要点 | 重要程度 |
|---------|----------|----------|----------|
| **对数 vs 幂** | $\lim_{x \to +\infty} \frac{(\ln x)^n}{x^m} = 0$ | 任意 $m > 0, n > 0$ | ⭐⭐⭐⭐⭐ |
| **幂 vs 指数** | $\lim_{x \to +\infty} \frac{x^n}{a^x} = 0$ | 任意 $n > 0, a > 1$ | ⭐⭐⭐⭐⭐ |
| **对数 vs 指数** | $\lim_{x \to +\infty} \frac{(\ln x)^n}{a^x} = 0$ | 任意 $n > 0, a > 1$ | ⭐⭐⭐⭐ |
| **幂 vs 阶乘** | $\lim_{n \to \infty} \frac{n^k}{n!} = 0$ | 任意 $k > 0$ | ⭐⭐⭐⭐ |
| **指数 vs 阶乘** | $\lim_{n \to \infty} \frac{a^n}{n!} = 0$ | 任意 $a > 0$ | ⭐⭐⭐⭐ |

#### **具体实例**
| 极限 | 值 | 技巧说明 |
|------|----|----|
| $\lim_{x \to +\infty} \frac{\ln x}{\sqrt{x}}$ | $0$ | 对数增长慢于任意正次幂 |
| $\lim_{x \to +\infty} \frac{x^{100}}{2^x}$ | $0$ | 指数函数增长快于任意幂函数 |
| $\lim_{x \to +\infty} \frac{(\ln x)^{10}}{x^{0.01}}$ | $0$ | 高次对数仍慢于低次幂 |
| $\lim_{x \to +\infty} x^a e^{-x}$ | $0$ | 指数衰减占主导， $a > 0$ |
| $\lim_{n \to \infty} \frac{2^n}{n!}$ | $0$ | 阶乘增长快于指数 |

#### **指数对数型极限**
| 极限 | 值 | 类型 | 解法要点 |
|------|----|----|---------|
| $\lim_{x \to 0^+} x^x$ | $1$ | $0^0$ 型 | 取对数： $\lim x \ln x = 0$ |
| $\lim_{x \to +\infty} x^{1/x}$ | $1$ | $\infty^0$ 型 | 取对数： $\lim \frac{\ln x}{x} = 0$ |
| $\lim_{x \to 0^+} (\sin x)^x$ | $1$ | $0^0$ 型 | 取对数转化 |
| $\lim_{x \to 1} x^{1/(x-1)}$ | $e$ | $1^{\infty}$ 型 | 标准 $1^{\infty}$ 型 |
| $\lim_{x \to \infty} \left(1 + \frac{a}{x}\right)^{bx}$ | $e^{ab}$ | $1^{\infty}$ 型 | 广义 $e$ 的定义 |

#### **三角函数高阶极限**
| 极限 | 值 | 阶数 | 技巧 |
|------|----|----|------|
| $\lim_{x \to 0} \frac{\tan x - \sin x}{x^3}$ | $\frac{1}{2}$ | 三阶 | 泰勒展开 |
| $\lim_{x \to 0} \frac{\sin x - x + \frac{x^3}{6}}{x^5}$ | $-\frac{1}{120}$ | 五阶 | 高阶泰勒 |
| $\lim_{x \to 0} \frac{\sin(\sin x) - x}{x^3}$ | $-\frac{1}{3}$ | 三阶 | 复合函数展开 |
| $\lim_{x \to 0} \frac{x - \arctan x}{x^3}$ | $\frac{1}{3}$ | 三阶 | 反三角函数 |
| $\lim_{x \to 0} \frac{\arcsin x - x}{x^3}$ | $\frac{1}{6}$ | 三阶 | 反三角函数 |

#### **根式与无理函数极限**
| 极限 | 值 | 技巧 |
|------|----|----|
| $\lim_{x \to +\infty} \left(\sqrt{x^2 + x} - x\right)$ | $\frac{1}{2}$ | 有理化分子 |
| $\lim_{x \to +\infty} \left(\sqrt{x^2 + ax + b} - x\right)$ | $\frac{a}{2}$ | 有理化技巧 |
| $\lim_{x \to 0} \frac{\sqrt{1+x} - \sqrt{1-x}}{x}$ | $1$ | 有理化处理 |
| $\lim_{n \to \infty} \sqrt[n]{a_1^n + a_2^n + \cdots + a_k^n}$ | $\max\{a_1, a_2, \ldots, a_k\}$ | 提取最大项 |
| $\lim_{x \to 0} \frac{\sqrt[n]{1+x} - 1}{x}$ | $\frac{1}{n}$ | 广义二项式定理 |

#### **分段函数与含参极限**
| 函数性质 | 极限结论 | 应用场景 |
|---------|----------|----------|
| **单调有界** | 必有极限 | 数列极限存在性 |
| **周期函数** | $\lim_{x \to \infty} \frac{f(x)}{x} = 0$ | $f$ 有界周期函数 |
| **间断点** | 左右极限存在但不等 | 可去、跳跃、无穷间断 |
| **渐近线** | $y = ax + b$ 当 $\lim_{x \to \infty} \frac{f(x)}{x} = a$ | 斜渐近线判定 |

#### **递推数列极限**
| 递推关系 | 极限求法 | 结果形式 |
|---------|----------|----------|
| $x_{n+1} = f(x_n)$ | 设  $\lim x_n = A$，则 $A = f(A)$ | 不动点法 |
| $x_{n+1} = ax_n + b$ | $A = \frac{b}{1-a}$ ($a \neq 1$) | 线性递推 |
| $x_{n+1} = \frac{ax_n + b}{cx_n + d}$ | 解方程  $A = \frac{aA + b}{cA + d}$ | 分式线性 |
| $x_{n+1} = \sqrt{a + x_n}$ | $A = \frac{1 + \sqrt{1+4a}}{2}$ | 根式递推 |

#### **积分相关极限**
| 极限类型 | 计算方法 | 典型例子 |
|---------|----------|----------|
| $\lim_{n \to \infty} \frac{1}{n} \sum_{k=1}^n f(\frac{k}{n})$ | $\int_0^1 f(x) dx$ | 黎曼和定义 |
| $\lim_{n \to \infty} \frac{1}{n^{p+1}} \sum_{k=1}^n k^p$ | $\frac{1}{p+1}$ | 幂和公式 |
| $\lim_{a \to 0^+} \frac{1}{a} \int_0^a f(x) dx$ | $f(0)$ | 积分中值定理 |
| $\lim_{x \to +\infty} \frac{\int_0^x f(t) dt}{x}$ | $\lim_{x \to +\infty} f(x)$ | 洛必达法则 |

---

## 3. 基本初等函数导数表

| 函数类型 | 函数 | 导数 | 定义域限制 | 记忆技巧 |
|---------|------|------|-----------|----------|
| **常数函数** | $C$ | $0$ | $\mathbb{R}$ | 常数导数为零 |
| **幂函数** | $x^α$ | $αx^{α-1}$ | $x > 0$ （当 $α \leq 0$ 时） | 指数降一次，系数为原指数 |
| **指数函数** | $e^x$ | $e^x$ | $\mathbb{R}$ | $e^x$ 求导不变 |
| | $a^x$ | $a^x \ln a$ | $\mathbb{R}$， $a > 0$， $a \neq 1$ | 多乘 $\ln a$ |
| **对数函数** | $\ln x$ | $\frac{1}{x}$ | $x > 0$ | 倒数关系 |
| | $\log_a x$ | $\frac{1}{x \ln a}$ | $x > 0$， $a > 0$， $a \neq 1$ | 多除 $\ln a$ |
| **三角函数** | $\sin x$ | $\cos x$ | $\mathbb{R}$ | 正变余 |
| | $\cos x$ | $-\sin x$ | $\mathbb{R}$ | 余变负正 |
| | $\tan x$ | $\sec^2 x = \frac{1}{\cos^2 x}$ | $x \neq \frac{\pi}{2} + k\pi$ | 正切平方加一 |
| | $\cot x$ | $-\csc^2 x = -\frac{1}{\sin^2 x}$ | $x \neq k\pi$ | 负余割平方 |
| | $\sec x$ | $\sec x \tan x$ | $x \neq \frac{\pi}{2} + k\pi$ | 自乘正切 |
| | $\csc x$ | $-\csc x \cot x$ | $x \neq k\pi$ | 负自乘余切 |
| **反三角函数** | $\arcsin x$ | $\frac{1}{\sqrt{1-x^2}}$ | $x \in (-1, 1)$ | 根号里1减平方 |
| | $\arccos x$ | $-\frac{1}{\sqrt{1-x^2}}$ | $x \in (-1, 1)$ | 负反正弦导数 |
| | $\arctan x$ | $\frac{1}{1+x^2}$ | $\mathbb{R}$ | 分母1加平方 |
| | $\operatorname{arccot} x$ | $-\frac{1}{1+x^2}$ | $\mathbb{R}$ | 负反正切导数 |
| | $\operatorname{arcsec} x$ | $\frac{1}{\|x\|\sqrt{x^2-1}}$ | $\|x\| > 1$ | 含绝对值 |
| | $\operatorname{arccsc} x$ | $-\frac{1}{\|x\|\sqrt{x^2-1}}$ | $\|x\| > 1$ | 负反正割导数 |
| **双曲函数** | $\sinh x$ | $\cosh x$ | $\mathbb{R}$ | 双曲正弦变余弦 |
| | $\cosh x$ | $\sinh x$ | $\mathbb{R}$ | 双曲余弦变正弦 |
| | $\tanh x$ | $\operatorname{sech}^2 x = \frac{1}{\cosh^2 x}$ | $\mathbb{R}$ | 双曲余弦平方倒数 |
| **反双曲函数** | $\operatorname{arcsinh} x$ | $\frac{1}{\sqrt{x^2+1}}$ | $\mathbb{R}$ | 根号里平方加1 |
| | $\operatorname{arccosh} x$ | $\frac{1}{\sqrt{x^2-1}}$ | $x > 1$ | 根号里平方减1 |
| | $\operatorname{arctanh} x$ | $\frac{1}{1-x^2}$ | $\|x\| < 1$ | 分母1减平方 |

### 记忆口诀

> **三角函数导数**："正变余，余变负正，切割方"
> 
> **反三角函数导数**："根号里1减方，分母里1加方"
> 
> **反三角函数符号**："$\arcsin$ 和$\arctan$ 为正， $\arccos$ 和$\text{arccot}$ 为负"
> 
> **双曲函数导数**："双曲正弦变余弦，双曲余弦变正弦"

---



## 4. 基本积分表

### 4.1 基本函数积分

| 函数类型 | 函数 | 不定积分 | 定义域限制 | 记忆技巧 |
|---------|------|----------|------------|----------|
| **常数与幂** | $k$ (常数) | $kx + C$ | $\mathbb{R}$ | 常数积分 |
| | $x^n$ | $\frac{x^{n+1}}{n+1} + C$ | $n \neq -1$ | 指数加1除以新指数 |
| | $\frac{1}{x^n}$ | $\frac{x^{1-n}}{1-n} + C$ | $n \neq 1, x \neq 0$ | 负指数幂 |
| **指数对数** | $e^x$ | $e^x + C$ | $\mathbb{R}$ | $e^x$ 积分不变 |
| | $a^x$ | $\frac{a^x}{\ln a} + C$ | $a > 0, a \neq 1$ | 除以 $\ln a$ |
| | $e^{ax}$ | $\frac{1}{a}e^{ax} + C$ | $a \neq 0$ | 系数倒数 |
| | $\frac{1}{x}$ | $\ln\|x\| + C$ | $x \neq 0$ | 对数积分 |

### 4.2 三角函数积分

| 函数 | 不定积分 | 定义域限制 | 技巧 |
|------|----------|------------|------|
| $\sin x$ | $-\cos x + C$ | $\mathbb{R}$ | 正弦积分负余弦 |
| $\cos x$ | $\sin x + C$ | $\mathbb{R}$ | 余弦积分正弦 |
| $\tan x$ | $-\ln\|\cos x\| + C$ | $x \neq \frac{\pi}{2}+k\pi$ | 正切积分负对数余弦 |
| $\cot x$ | $\ln\|\sin x\| + C$ | $x \neq k\pi$ | 余切积分对数正弦 |
| $\sec x$ | $\ln\|\sec x + \tan x\| + C$ | $x \neq \frac{\pi}{2}+k\pi$ | 正割积分复杂形式 |
| $\csc x$ | $-\ln\|\csc x + \cot x\| + C$ | $x \neq k\pi$ | 余割积分复杂形式 |
| $\sec^2 x$ | $\tan x + C$ | $x \neq \frac{\pi}{2}+k\pi$ | 正割平方积分正切 |
| $\csc^2 x$ | $-\cot x + C$ | $x \neq k\pi$ | 余割平方积分负余切 |
| $\sec x \tan x$ | $\sec x + C$ | $x \neq \frac{\pi}{2}+k\pi$ | 乘积形式 |
| $\csc x \cot x$ | $-\csc x + C$ | $x \neq k\pi$ | 乘积形式 |

### 4.3 反三角函数相关积分

| 函数 | 不定积分 | 定义域限制 | 记忆要点 |
|------|----------|------------|----------|
| $\frac{1}{\sqrt{1-x^2}}$ | $\arcsin x + C$ | $\|x\| < 1$ | 反正弦原型 |
| $\frac{1}{1+x^2}$ | $\arctan x + C$ | $\mathbb{R}$ | 反正切原型 |
| $\frac{1}{\sqrt{a^2-x^2}}$ | $\arcsin \frac{x}{a} + C$ | $\|x\| < a$ | 含参数反正弦 |
| $\frac{1}{a^2+x^2}$ | $\frac{1}{a}\arctan \frac{x}{a} + C$ | $a \neq 0$ | 含参数反正切 |
| $\frac{1}{x\sqrt{x^2-a^2}}$ | $\frac{1}{a}\operatorname{arcsec}\frac{\|x\|}{a} + C$ | $\|x\| > a$ | 反正割形式 |

### 4.4 根式积分

| 函数 | 不定积分 | 适用条件 | 技巧 |
|------|----------|----------|------|
| $\frac{1}{\sqrt{x^2+a^2}}$ | $\ln(x+\sqrt{x^2+a^2}) + C$ | $a \neq 0$ | 双曲正弦反函数 |
| $\frac{1}{\sqrt{x^2-a^2}}$ | $\ln\|x+\sqrt{x^2-a^2}\| + C$ | $\|x\| > a$ | 需要绝对值 |
| $\frac{1}{\sqrt{a^2-x^2}}$ | $\arcsin \frac{x}{a} + C$ | $\|x\| < a$ | 三角替换 |
| $\sqrt{a^2-x^2}$ | $\frac{x}{2}\sqrt{a^2-x^2} + \frac{a^2}{2}\arcsin \frac{x}{a} + C$ | $\|x\| \leq a$ | 分部积分 |
| $\sqrt{x^2+a^2}$ | $\frac{x}{2}\sqrt{x^2+a^2} + \frac{a^2}{2}\ln(x+\sqrt{x^2+a^2}) + C$ | $a \neq 0$ | 分部积分 |
| $\sqrt{x^2-a^2}$ | $\frac{x}{2}\sqrt{x^2-a^2} - \frac{a^2}{2}\ln\|x+\sqrt{x^2-a^2}\| + C$ | $\|x\| > a$ | 分部积分 |

### 4.5 有理函数积分

| 函数 | 不定积分 | 方法 |
|------|----------|------|
| $\frac{1}{x^2+a^2}$ | $\frac{1}{a}\arctan \frac{x}{a} + C$ | 直接公式 |
| $\frac{1}{x^2-a^2}$ | $\frac{1}{2a}\ln\left\|\frac{x-a}{x+a}\right\| + C$ | 部分分式 |
| $\frac{1}{(x+a)^n}$ | $\frac{(x+a)^{1-n}}{1-n} + C$ | $n \neq 1$ |
| $\frac{x}{x^2+a^2}$ | $\frac{1}{2}\ln(x^2+a^2) + C$ | 换元积分 |
| $\frac{1}{x(x+a)}$ | $\frac{1}{a}\ln\left\|\frac{x}{x+a}\right\| + C$ | 部分分式 |

### 4.6 双曲函数积分

| 函数 | 不定积分 | 记忆技巧 |
|------|----------|----------|
| $\sinh x$ | $\cosh x + C$ | 双曲正弦积分双曲余弦 |
| $\cosh x$ | $\sinh x + C$ | 双曲余弦积分双曲正弦 |
| $\tanh x$ | $\ln(\cosh x) + C$ | 双曲正切积分对数双曲余弦 |
| $\operatorname{coth} x$ | $\ln\|\sinh x\| + C$ | 双曲余切积分对数双曲正弦 |
| $\operatorname{sech}^2 x$ | $\tanh x + C$ | 双曲正割平方积分双曲正切 |
| $\operatorname{csch}^2 x$ | $-\operatorname{coth} x + C$ | 双曲余割平方积分负双曲余切 |

---

## 5. 三角函数恒等式表
（0基础的同学很有必要在基础篇再复习一遍三角函数相关公式，很多地方需要一眼看出来）
### 5.1 基本关系式

| 关系类型 | 恒等式 |
|---------|--------|
| **平方关系** | $\sin^2 x + \cos^2 x = 1$ |
| | $1 + \tan^2 x = \sec^2 x$ |
| | $1 + \cot^2 x = \csc^2 x$ |
| **商数关系** | $\tan x = \frac{\sin x}{\cos x}$ |
| | $\cot x = \frac{\cos x}{\sin x}$ |
| **倒数关系** | $\sec x = \frac{1}{\cos x}$ |
| | $\csc x = \frac{1}{\sin x}$ |

### 5.2 和差角公式

| 函数 | 和角公式 | 差角公式 |
|------|----------|----------|
| $\sin(A \pm B)$ | $\sin A \cos B \pm \cos A \sin B$ | 符号对应 |
| $\cos(A \pm B)$ | $\cos A \cos B \mp \sin A \sin B$ | 符号相反 |
| $\tan(A \pm B)$ | $\frac{\tan A \pm \tan B}{1 \mp \tan A \tan B}$ | 分母符号相反 |

### 5.3 和差化积公式

| 公式类型 | 恒等式 |
|---------|--------|
| **正弦和差** | $\sin A + \sin B = 2\sin\frac{A + B}{2}\cos\frac{A - B}{2}$ |
| | $\sin A - \sin B = 2\cos\frac{A + B}{2}\sin\frac{A - B}{2}$ |
| **余弦和差** | $\cos A + \cos B = 2\cos\frac{A + B}{2}\cos\frac{A - B}{2}$ |
| | $\cos A - \cos B = -2\sin\frac{A + B}{2}\sin\frac{A - B}{2}$ |

### 5.4 积化和差公式

| 积的形式 | 和差形式 |
|---------|----------|
| $\sin A \cos B$ | $\frac{1}{2}[\sin(A+B) + \sin(A-B)]$ |
| $\cos A \sin B$ | $\frac{1}{2}[\sin(A+B) - \sin(A-B)]$ |
| $\cos A \cos B$ | $\frac{1}{2}[\cos(A+B) + \cos(A-B)]$ |
| $\sin A \sin B$ | $\frac{1}{2}[\cos(A-B) - \cos(A+B)]$ |

### 5.5 倍角公式

| 函数 | 二倍角公式 | 三倍角公式 |
|------|------------|------------|
| $\sin$ | $\sin 2α = 2\sin α\cos α$ | $\sin 3α = 3\sin α - 4\sin^3 α$ |
| $\cos$ | $\cos 2α = \cos^2 α - \sin^2 α$ | $\cos 3α = 4\cos^3 α - 3\cos α$ |
| | $= 2\cos^2 α - 1$ | |
| | $= 1 - 2\sin^2 α$ | |
| $\tan$ | $\tan 2α = \frac{2\tan α}{1 - \tan^2 α}$ | $\tan 3α = \frac{3\tan α - \tan^3 α}{1 - 3\tan^2 α}$ |

### 5.6 半角公式

| 函数 | 半角公式 | 另一形式 |
|------|----------|----------|
| $\sin \frac{α}{2}$ | $\pm\sqrt{\frac{1-\cos α}{2}}$ | $\frac{1-\cos α}{\sin α} = \frac{\sin α}{1+\cos α}$ |
| $\cos \frac{α}{2}$ | $\pm\sqrt{\frac{1+\cos α}{2}}$ | |
| $\tan \frac{α}{2}$ | $\pm\sqrt{\frac{1-\cos α}{1+\cos α}}$ | $\frac{1-\cos α}{\sin α} = \frac{\sin α}{1+\cos α}$ |

> 符号由 $\frac{α}{2}$ 所在象限决定

### 5.7 万能公式（半角替换）

> 设  $t = \tan \frac{x}{2}$，则：

| 函数 | 万能公式 |
|------|----------|
| $\sin x$ | $\frac{2t}{1+t^2}$ |
| $\cos x$ | $\frac{1-t^2}{1+t^2}$ |
| $\tan x$ | $\frac{2t}{1-t^2}$ |
| $dx$ | $\frac{2}{1+t^2}dt$ |

### 5.8 其他重要公式

| 公式类型 | 恒等式 |
|---------|--------|
| **降幂公式** | $\sin^2 x = \frac{1-\cos 2x}{2}$ |
| | $\cos^2 x = \frac{1+\cos 2x}{2}$ |
| **辅助角公式** | $a\sin x + b\cos x = \sqrt{a^2+b^2}\sin(x+φ)$ |
| | 其中 $\tan φ = \frac{b}{a}$ |
| **正弦定理** | $\frac{a}{\sin A} = \frac{b}{\sin B} = \frac{c}{\sin C} = 2R$ |
| **余弦定理** | $c^2 = a^2 + b^2 - 2ab\cos C$ |

---

##  提高篇

## 6. 高阶导数表

| 函数类型 | 函数 | n阶导数 | 适用条件 | 记忆技巧 |
|---------|------|---------|----------|----------|
| **幂函数** | $x^m$ | $\frac{m!}{(m-n)!}x^{m-n}$ | $n \leq m$，否则为0 | 阶乘公式 |
| **指数函数** | $e^{ax}$ | $a^n e^{ax}$ | $n \geq 0$ | 每求导乘 $a$ |
| | $a^x$ | $a^x (\ln a)^n$ | $a > 0, a \neq 1$ | 乘 $(\ln a)^n$ |
| **对数函数** | $\ln x$ | $\frac{(-1)^{n-1}(n-1)!}{x^n}$ | $n \geq 1, x > 0$ | 符号交替，阶乘递减 |
| **三角函数** | $\sin x$ | $\sin(x + \frac{n\pi}{2})$ | $n \geq 0$ | 相位增 $\frac{n\pi}{2}$ |
| | $\cos x$ | $\cos(x + \frac{n\pi}{2})$ | $n \geq 0$ | 相位增 $\frac{n\pi}{2}$ |
| **双曲函数** | $\sinh x$ | $\sinh x$ 或$\cosh x$ | 取决于 $n$ 的奇偶性 | 周期为2 |
| | $\cosh x$ | $\cosh x$ 或$\sinh x$ | 取决于 $n$ 的奇偶性 | 周期为2 |

### 有理函数n阶导数表

| 函数类型 | 函数 | n阶导数公式 | 记忆技巧 |
|---------|------|-------------|----------|
| **基本分式** | $\frac{1}{x-a}$ | $\frac{(-1)^n n!}{(x-a)^{n+1}}$ | 符号交替，阶乘分子 |
| **幂次分式** | $\frac{1}{(x-a)^m}$ | $\frac{(-1)^n (m+n-1)!}{(m-1)!(x-a)^{m+n}}$ | 组合数形式 |
| **线性复合** | $\frac{1}{ax+b}$ | $\frac{(-1)^n n! a^n}{(ax+b)^{n+1}}$ | 多乘 $a^n$ |

---

## 7. 常用泰勒级数展开表

### 7.1 基本函数的麦克劳林级数

| 函数 | 级数展开 | 收敛半径 | 记忆要点 |
|------|----------|----------|----------|
| $e^x$ | $\sum_{n=0}^{\infty} \frac{x^n}{n!} = 1 + x + \frac{x^2}{2!} + \frac{x^3}{3!} + \cdots$ | $R = \infty$ | 系数为倒阶乘 |
| $\sin x$ | $\sum_{n=0}^{\infty} \frac{(-1)^n x^{2n+1}}{(2n+1)!} = x - \frac{x^3}{3!} + \frac{x^5}{5!} - \cdots$ | $R = \infty$ | 奇次幂，符号交替 |
| $\cos x$ | $\sum_{n=0}^{\infty} \frac{(-1)^n x^{2n}}{(2n)!} = 1 - \frac{x^2}{2!} + \frac{x^4}{4!} - \cdots$ | $R = \infty$ | 偶次幂，符号交替 |
| $(1+x)^α$ | $\sum_{n=0}^{\infty} \binom{α}{n} x^n = 1 + αx + \frac{α(α-1)}{2!}x^2 + \cdots$ | $R = 1$ | 二项式定理 |
| $\frac{1}{1-x}$ | $\sum_{n=0}^{\infty} x^n = 1 + x + x^2 + x^3 + \cdots$ | $R = 1$ | 几何级数 |
| $\frac{1}{1+x}$ | $\sum_{n=0}^{\infty} (-1)^n x^n = 1 - x + x^2 - x^3 + \cdots$ | $R = 1$ | 交替几何级数 |
| $\ln(1+x)$ | $\sum_{n=1}^{\infty} \frac{(-1)^{n-1} x^n}{n} = x - \frac{x^2}{2} + \frac{x^3}{3} - \cdots$ | $R = 1$ | 对数级数 |
| $\ln(1-x)$ | $-\sum_{n=1}^{\infty} \frac{x^n}{n} = -x - \frac{x^2}{2} - \frac{x^3}{3} - \cdots$ | $R = 1$ | 负对数级数 |
| $\arctan x$ | $\sum_{n=0}^{\infty} \frac{(-1)^n x^{2n+1}}{2n+1} = x - \frac{x^3}{3} + \frac{x^5}{5} - \cdots$ | $R = 1$ | 反正切级数 |

### 7.2 常用函数的泰勒展开（前几项）

| 函数 | 泰勒展开（ $x \to 0$） | 精度 |
|------|---------------------|------|
| $e^x$ | $1 + x + \frac{x^2}{2} + \frac{x^3}{6} + O(x^4)$ | 三阶 |
| $\sin x$ | $x - \frac{x^3}{6} + O(x^5)$ | 三阶 |
| $\cos x$ | $1 - \frac{x^2}{2} + \frac{x^4}{24} + O(x^6)$ | 四阶 |
| $\tan x$ | $x + \frac{x^3}{3} + \frac{2x^5}{15} + O(x^7)$ | 五阶 |
| $\ln(1+x)$ | $x - \frac{x^2}{2} + \frac{x^3}{3} - \frac{x^4}{4} + O(x^5)$ | 四阶 |
| $(1+x)^α$ | $1 + αx + \frac{α(α-1)}{2}x^2 + \frac{α(α-1)(α-2)}{6}x^3 + O(x^4)$ | 三阶 |
| $\arcsin x$ | $x + \frac{x^3}{6} + \frac{3x^5}{40} + O(x^7)$ | 五阶 |
| $\arctan x$ | $x - \frac{x^3}{3} + \frac{x^5}{5} + O(x^7)$ | 五阶 |
| $\sinh x$ | $x + \frac{x^3}{6} + \frac{x^5}{120} + O(x^7)$ | 五阶 |
| $\cosh x$ | $1 + \frac{x^2}{2} + \frac{x^4}{24} + O(x^6)$ | 四阶 |

### 7.3 复合函数的泰勒展开

| 函数 | 泰勒展开 | 应用 |
|------|----------|------|
| $e^{-x}$ | $1 - x + \frac{x^2}{2} - \frac{x^3}{6} + O(x^4)$ | 负指数 |
| $e^{x^2}$ | $1 + x^2 + \frac{x^4}{2} + O(x^6)$ | 复合指数 |
| $\sin(x^2)$ | $x^2 - \frac{x^6}{6} + O(x^{10})$ | 复合三角 |
| $\cos(x^2)$ | $1 - \frac{x^4}{2} + O(x^8)$ | 复合三角 |
| $\ln(1+x^2)$ | $x^2 - \frac{x^4}{2} + \frac{x^6}{3} + O(x^8)$ | 复合对数 |
| $(1+x^2)^{1/2}$ | $1 + \frac{x^2}{2} - \frac{x^4}{8} + O(x^6)$ | 复合幂函数 |

---

## 8. 特殊积分表

### 8.1 含参数积分（华里士积分）

| 积分类型 | 积分公式 | 条件 | 应用 |
|---------|----------|------|------|
| **华里士积分** | $\int_0^{\frac{\pi}{2}} \sin^n x \, dx = \int_0^{\frac{\pi}{2}} \cos^n x \, dx$ | $n \geq 0$ | 递推积分 |
| $n$ 为偶数 | $\frac{(n-1)!!}{n!!} \cdot \frac{\pi}{2}$ | $n = 2k$ | 结果含 $\pi$ |
| $n$ 为奇数 | $\frac{(n-1)!!}{n!!}$ | $n = 2k+1$ | 结果为有理数 |
| **伽马函数积分** | $\int_0^{\infty} x^{p-1} e^{-x} dx = \Gamma(p)$ | $p > 0$ | 伽马函数定义 |
| **贝塔函数积分** | $\int_0^1 x^{p-1}(1-x)^{q-1} dx = B(p,q)$ | $p,q > 0$ | 贝塔函数定义 |

### 8.2 三角函数特殊积分

| 积分 | 结果 | 条件 | 技巧 |
|------|------|------|------|
| $\int_0^{\pi} x\sin x \, dx$ | $\pi$ | 定积分 | 分部积分 |
| $\int_0^{\pi} \frac{\sin nx}{\sin x} dx$ | $\pi$ | $n$ 为正整数 | 特殊性质 |
| $\int_0^{\infty} \frac{\sin x}{x} dx$ | $\frac{\pi}{2}$ | 狄利克雷积分 | 含参积分 |
| $\int_0^{\infty} \frac{\sin^2 x}{x^2} dx$ | $\frac{\pi}{2}$ | 经典积分 | 分部积分+含参 |
| $\int_0^{\infty} e^{-x^2} dx$ | $\frac{\sqrt{\pi}}{2}$ | 高斯积分 | 极坐标变换 |

### 8.3 指数对数特殊积分

| 积分 | 结果 | 条件 | 方法 |
|------|------|------|------|
| $\int_0^1 x^p (\ln x)^n dx$ | $\frac{(-1)^n n!}{(p+1)^{n+1}}$ | $p > -1, n \geq 0$ | 分部积分 |
| $\int_0^{\infty} x^p e^{-ax} dx$ | $\frac{\Gamma(p+1)}{a^{p+1}}$ | $a > 0, p > -1$ | 伽马函数 |
| $\int_0^{\infty} \frac{e^{-ax} - e^{-bx}}{x} dx$ | $\ln\frac{b}{a}$ | $a,b > 0$ | 含参积分 |

---

##  高级篇

## 9. 多元函数相关表

### 9.1 二元函数偏导数链式法则

| 复合形式 | 偏导数公式 | 记忆要点 |
|---------|------------|----------|
| $z = f(u,v), u = u(x,y), v = v(x,y)$ | $\frac{\partial z}{\partial x} = \frac{\partial f}{\partial u}\frac{\partial u}{\partial x} + \frac{\partial f}{\partial v}\frac{\partial v}{\partial x}$ | 全微分链式法则 |
| $z = f(u,v), u = u(t), v = v(t)$ | $\frac{dz}{dt} = \frac{\partial f}{\partial u}\frac{du}{dt} + \frac{\partial f}{\partial v}\frac{dv}{dt}$ | 一元复合 |
| $z = f(x,y), y = y(x)$ | $\frac{dz}{dx} = \frac{\partial f}{\partial x} + \frac{\partial f}{\partial y}\frac{dy}{dx}$ | 隐函数求导 |

### 9.2 二阶偏导数表

| 函数类型 | 二阶偏导数 | 混合偏导数 |
|---------|------------|------------|
| $f(x,y) = x^m y^n$ | $f_{xx} = m(m-1)x^{m-2}y^n$ <br> $f_{yy} = n(n-1)x^m y^{n-2}$ | $f_{xy} = f_{yx} = mnx^{m-1}y^{n-1}$ |
| $f(x,y) = e^{ax+by}$ | $f_{xx} = a^2 e^{ax+by}$ <br> $f_{yy} = b^2 e^{ax+by}$ | $f_{xy} = f_{yx} = ab e^{ax+by}$ |
| $f(x,y) = \ln(ax+by)$ | $f_{xx} = -\frac{a^2}{(ax+by)^2}$ <br> $f_{yy} = -\frac{b^2}{(ax+by)^2}$ | $f_{xy} = f_{yx} = -\frac{ab}{(ax+by)^2}$ |

### 9.3 常用坐标变换

| 坐标系 | 变换公式 | 雅可比行列式 |
|-------|----------|-------------|
| **极坐标** | $x = r\cos θ, y = r\sin θ$ | $J = r$ |
| **柱坐标** | $x = r\cos θ, y = r\sin θ, z = z$ | $J = r$ |
| **球坐标** | $x = r\sin φ\cos θ$ <br> $y = r\sin φ\sin θ$ <br> $z = r\cos φ$ | $J = r^2\sin φ$ |

### 9.4 梯度、散度、旋度公式表

| 向量算子 | 直角坐标系 | 柱坐标系 | 球坐标系 |
|---------|------------|----------|----------|
| **梯度** $\nabla f$ | $\frac{\partial f}{\partial x}\mathbf{i} + \frac{\partial f}{\partial y}\mathbf{j} + \frac{\partial f}{\partial z}\mathbf{k}$ | $\frac{\partial f}{\partial r}\mathbf{e_r} + \frac{1}{r}\frac{\partial f}{\partial θ}\mathbf{e_θ} + \frac{\partial f}{\partial z}\mathbf{e_z}$ | $\frac{\partial f}{\partial r}\mathbf{e_r} + \frac{1}{r}\frac{\partial f}{\partial θ}\mathbf{e_θ} + \frac{1}{r\sin θ}\frac{\partial f}{\partial φ}\mathbf{e_φ}$ |
| **散度** $\nabla \cdot \mathbf{F}$ | $\frac{\partial F_x}{\partial x} + \frac{\partial F_y}{\partial y} + \frac{\partial F_z}{\partial z}$ | $\frac{1}{r}\frac{\partial(rF_r)}{\partial r} + \frac{1}{r}\frac{\partial F_θ}{\partial θ} + \frac{\partial F_z}{\partial z}$ | $\frac{1}{r^2}\frac{\partial(r^2F_r)}{\partial r} + \frac{1}{r\sin θ}\frac{\partial(\sin θ F_θ)}{\partial θ} + \frac{1}{r\sin θ}\frac{\partial F_φ}{\partial φ}$ |

---

## 10. 曲线积分与曲面积分表

### 10.1 第一类曲线积分

| 参数方程 | 弧长元素 | 积分公式 |
|---------|----------|----------|
| $\mathbf{r}(t) = (x(t), y(t)), t \in [a,b]$ | $ds = \sqrt{x'^2(t) + y'^2(t)} dt$ | $\int_L f(x,y) ds = \int_a^b f(x(t), y(t))\sqrt{x'^2(t) + y'^2(t)} dt$ |
| $y = φ(x), x \in [a,b]$ | $ds = \sqrt{1 + φ'^2(x)} dx$ | $\int_L f(x,y) ds = \int_a^b f(x, φ(x))\sqrt{1 + φ'^2(x)} dx$ |

### 10.2 第二类曲线积分

| 积分类型 | 计算公式 | 条件 |
|---------|----------|------|
| $\int_L P dx + Q dy$ | $\int_a^b [P(x(t), y(t))x'(t) + Q(x(t), y(t))y'(t)] dt$ | 参数方程 |
| **格林公式** | $\oint_L P dx + Q dy = \iint_D \left(\frac{\partial Q}{\partial x} - \frac{\partial P}{\partial y}\right) dxdy$ | $L$ 为$D$ 正向边界 |

### 10.3 第一类曲面积分

| 曲面方程 | 面积元素 | 积分公式 |
|---------|----------|----------|
| $z = z(x,y)$ | $dS = \sqrt{1 + z_x^2 + z_y^2} dxdy$ | $\iint_S f dS = \iint_D f(x,y,z(x,y))\sqrt{1 + z_x^2 + z_y^2} dxdy$ |
| 参数方程 | $dS = \|\mathbf{r_u} \times \mathbf{r_v}\| dudv$ | 叉积求模 |

### 10.4 第二类曲面积分

| 公式名称 | 公式内容 | 应用条件 |
|---------|----------|----------|
| **高斯公式** | $\oiint_S P dydz + Q dzdx + R dxdy = \iiint_Ω \left(\frac{\partial P}{\partial x} + \frac{\partial Q}{\partial y} + \frac{\partial R}{\partial z}\right) dxdydz$ | $S$ 为$Ω$ 外侧 |
| **斯托克斯公式** | $\oint_L P dx + Q dy + R dz = \iint_S \left\|\begin{array}{ccc} \mathbf{i} & \mathbf{j} & \mathbf{k} \\ \frac{\partial}{\partial x} & \frac{\partial}{\partial y} & \frac{\partial}{\partial z} \\ P & Q & R \end{array}\right\| \cdot d\mathbf{S}$ | $L$ 为$S$ 正向边界 |

---

## 11. 常微分方程解表

### 11.1 一阶微分方程

| 方程类型 | 标准形式 | 解法 | 通解 |
|---------|----------|------|------|
| **可分离变量** | $\frac{dy}{dx} = f(x)g(y)$ | 分离变量 | $\int \frac{dy}{g(y)} = \int f(x) dx + C$ |
| **齐次方程** | $\frac{dy}{dx} = F(\frac{y}{x})$ | 令 $u = \frac{y}{x}$ | 化为可分离变量 |
| **一阶线性** | $\frac{dy}{dx} + P(x)y = Q(x)$ | 积分因子 | $y = e^{-\int P dx}[\int Q e^{\int P dx} dx + C]$ |
| **伯努利方程** | $\frac{dy}{dx} + P(x)y = Q(x)y^n$ | 令 $z = y^{1-n}$ | 化为一阶线性 |
| **全微分方程** | $P(x,y)dx + Q(x,y)dy = 0$ | $\frac{\partial P}{\partial y} = \frac{\partial Q}{\partial x}$ | $\int P dx + \int Q dy = C$ |

### 11.2 二阶常系数线性微分方程

| 方程类型 | 特征方程 | 通解形式 |
|---------|----------|----------|
| $y'' + py' + qy = 0$ | $r^2 + pr + q = 0$ | |
| 两个不同实根 $r_1, r_2$ | $\Delta > 0$ | $y = C_1 e^{r_1 x} + C_2 e^{r_2 x}$ |
| 一个重根 $r$ | $\Delta = 0$ | $y = (C_1 + C_2 x) e^{rx}$ |
| 共轭复根 $α \pm βi$ | $\Delta < 0$ | $y = e^{αx}(C_1 \cos βx + C_2 \sin βx)$ |

### 11.3 二阶常系数非齐次线性方程特解

| 自由项 $f(x)$ | 特解形式 $y^*$ |
|--------------|---------------|
| $P_n(x)$ | $x^k Q_n(x)$ |
| $e^{ax} P_n(x)$ | $x^k e^{ax} Q_n(x)$ |
| $e^{ax}[P_n(x)\cos βx + Q_m(x)\sin βx]$ | $x^k e^{ax}[R_l(x)\cos βx + S_l(x)\sin βx]$ |

> 其中 $k$ 为特征根重数， $l = \max\{n,m\}$

---

## 12. 无穷级数判定表

### 12.1 正项级数判定法

| 判定法 | 条件 | 结论 | 适用情况 |
|-------|------|------|----------|
| **比较判定法** | $0 \leq u_n \leq v_n$ | $\sum v_n$ 收敛 $\Rightarrow\sum u_n$ 收敛 | 有比较对象 |
| **比值判定法** | $\lim_{n \to \infty} \frac{u_{n+1}}{u_n} = \rho$ | $\rho < 1$ 收敛， $\rho > 1$ 发散 | 含阶乘、指数 |
| **根式判定法** | $\lim_{n \to \infty} \sqrt[n]{u_n} = \rho$ | $\rho < 1$ 收敛， $\rho > 1$ 发散 | 含 $n$ 次幂 |
| **积分判定法** | $f(x)$ 单调递减正值 | $\int_1^{\infty} f(x) dx$ 与$\sum f(n)$ 同敛散 | 可积函数 |
| **拉贝判定法** | $\lim_{n \to \infty} n(1 - \frac{u_{n+1}}{u_n}) = l$ | $l > 1$ 收敛， $l < 1$ 发散 | 比值法失效时 |

### 12.2 常见级数收敛性

| 级数 | 收敛性 | 条件 |
|------|-------|------|
| $\sum_{n=1}^{\infty} \frac{1}{n^p}$ | $p > 1$ 收敛， $p \leq 1$ 发散 | $p$ 级数 |
| $\sum_{n=1}^{\infty} \frac{1}{n \ln^p n}$ | $p > 1$ 收敛， $p \leq 1$ 发散 | 对数 $p$ 级数 |
| $\sum_{n=0}^{\infty} x^n$ | $\|x\| < 1$ 收敛 | 几何级数 |
| $\sum_{n=1}^{\infty} \frac{x^n}{n}$ | $-1 < x \leq 1$ 收敛 | 对数级数 |
| $\sum_{n=0}^{\infty} \frac{x^n}{n!}$ | 对所有 $x$ 收敛 | 指数级数 |

### 12.3 交错级数判定

| 判定法 | 条件 | 结论 |
|-------|------|------|
| **莱布尼茨判定法** | $\{u_n\}$ 单调递减且 $\lim_{n \to \infty} u_n = 0$ | $\sum (-1)^n u_n$ 收敛 |
| **条件收敛** | 级数收敛但绝对值级数发散 | 可重排改变和 |
| **绝对收敛** | $\sum \|u_n\|$ 收敛 | 原级数必收敛 |

---

##   专题篇

## 13. 向量运算表

### 13.1 向量基本运算

| 运算 | 坐标表示 | 几何意义 |
|------|----------|----------|
| 向量模 | $\|\mathbf{a}\| = \sqrt{a_x^2 + a_y^2 + a_z^2}$ | 向量长度 |
| 数量积 | $\mathbf{a} \cdot \mathbf{b} = a_x b_x + a_y b_y + a_z b_z$ | $\|\mathbf{a}\|\|\mathbf{b}\|\cos θ$ |
| 向量积 | $\mathbf{a} \times \mathbf{b} = \left\|\begin{array}{ccc} \mathbf{i} & \mathbf{j} & \mathbf{k} \\ a_x & a_y & a_z \\ b_x & b_y & b_z \end{array}\right\|$ | $\|\mathbf{a}\|\|\mathbf{b}\|\sin θ$ |
| 混合积 | $(\mathbf{a}, \mathbf{b}, \mathbf{c}) = \mathbf{a} \cdot (\mathbf{b} \times \mathbf{c})$ | 平行六面体体积 |

### 13.2 向量运算性质

| 性质 | 数量积 | 向量积 |
|------|-------|-------|
| **交换律** | $\mathbf{a} \cdot \mathbf{b} = \mathbf{b} \cdot \mathbf{a}$ | $\mathbf{a} \times \mathbf{b} = -\mathbf{b} \times \mathbf{a}$ |
| **分配律** | $\mathbf{a} \cdot (\mathbf{b} + \mathbf{c}) = \mathbf{a} \cdot \mathbf{b} + \mathbf{a} \cdot \mathbf{c}$ | $\mathbf{a} \times (\mathbf{b} + \mathbf{c}) = \mathbf{a} \times \mathbf{b} + \mathbf{a} \times \mathbf{c}$ |
| **与自身** | $\mathbf{a} \cdot \mathbf{a} = \|\mathbf{a}\|^2$ | $\mathbf{a} \times \mathbf{a} = \mathbf{0}$ |

---

## 14. 傅里叶级数表

### 14.1 基本傅里叶级数

| 函数类型 | 傅里叶系数 | 级数形式 |
|---------|------------|----------|
| **一般函数** | $a_n = \frac{1}{\pi}\int_{-\pi}^{\pi} f(x)\cos(nx) dx$ <br> $b_n = \frac{1}{\pi}\int_{-\pi}^{\pi} f(x)\sin(nx) dx$ | $f(x) = \frac{a_0}{2} + \sum_{n=1}^{\infty}(a_n\cos nx + b_n\sin nx)$ |
| **奇函数** | $a_n = 0$ <br> $b_n = \frac{2}{\pi}\int_0^{\pi} f(x)\sin(nx) dx$ | $f(x) = \sum_{n=1}^{\infty} b_n\sin nx$ |
| **偶函数** | $a_n = \frac{2}{\pi}\int_0^{\pi} f(x)\cos(nx) dx$ <br> $b_n = 0$ | $f(x) = \frac{a_0}{2} + \sum_{n=1}^{\infty} a_n\cos nx$ |

### 14.2 常见函数的傅里叶级数

| 函数 | 区间 | 傅里叶级数 |
|------|------|-----------|
| $f(x) = x$ | $(-\pi, \pi)$ | $2\sum_{n=1}^{\infty} \frac{(-1)^{n+1}}{n}\sin nx$ |
| $f(x) = \|x\|$ | $(-\pi, \pi)$ | $\frac{\pi}{2} - \frac{4}{\pi}\sum_{n=1}^{\infty} \frac{\cos(2n-1)x}{(2n-1)^2}$ |
| $f(x) = x^2$ | $(-\pi, \pi)$ | $\frac{\pi^2}{3} + 4\sum_{n=1}^{\infty} \frac{(-1)^n}{n^2}\cos nx$ |

---

## 15. 特殊函数表

### 15.1 伽马函数

| 定义与性质 | 公式 | 备注 |
|------------|------|------|
| **定义** | $\Gamma(s) = \int_0^{\infty} t^{s-1} e^{-t} dt, \quad s > 0$ | 欧拉积分第二类 |
| **递推公式** | $\Gamma(s+1) = s\Gamma(s)$ | 基本递推关系 |
| **特殊值** | $\Gamma(n) = (n-1)!, \quad \Gamma(\frac{1}{2}) = \sqrt{\pi}$ | 整数和半整数 |
| **反射公式** | $\Gamma(s)\Gamma(1-s) = \frac{\pi}{\sin(\pi s)}$ | 余元公式 |
| **重要恒等式** | $\Gamma(s)\Gamma(s+\frac{1}{2}) = \frac{\sqrt{\pi}}{2^{2s-1}}\Gamma(2s)$ | Legendre重复公式 |

### 15.2 贝塔函数

| 定义与性质 | 公式 | 备注 |
|------------|------|------|
| **定义** | $B(p,q) = \int_0^1 t^{p-1}(1-t)^{q-1} dt, \quad p,q > 0$ | 欧拉积分第一类 |
| **与伽马函数关系** | $B(p,q) = \frac{\Gamma(p)\Gamma(q)}{\Gamma(p+q)}$ | 基本关系 |
| **对称性** | $B(p,q) = B(q,p)$ | 对称性质 |
| **积分表示** | $B(p,q) = 2\int_0^{\frac{\pi}{2}} \sin^{2p-1}θ \cos^{2q-1}θ \, dθ$ | 三角形式 |
| **特殊值** | $B(m,n) = \frac{(m-1)!(n-1)!}{(m+n-1)!}$ | $m,n$ 为正整数 |

### 15.3 误差函数

| 函数 | 定义 | 性质 | 应用 |
|------|------|------|------|
| **误差函数** | $\operatorname{erf}(x) = \frac{2}{\sqrt{\pi}}\int_0^x e^{-t^2} dt$ | $\operatorname{erf}(0) = 0, \operatorname{erf}(\infty) = 1$ | 概率统计 |
| **余误差函数** | $\operatorname{erfc}(x) = 1 - \operatorname{erf}(x)$ | $\operatorname{erfc}(x) = \frac{2}{\sqrt{\pi}}\int_x^{\infty} e^{-t^2} dt$ | 互补函数 |
| **导数** | $\frac{d}{dx}\operatorname{erf}(x) = \frac{2}{\sqrt{\pi}}e^{-x^2}$ | 高斯函数 | 微分方程 |

---

## 16. 数值计算表

### 16.1 常用数值

| 常数 | 近似值 | 精确表达式 |
|------|-------|------------|
| $\pi$ | $3.14159$ | $4\sum_{n=0}^{\infty} \frac{(-1)^n}{2n+1}$ |
| $e$ | $2.71828$ | $\sum_{n=0}^{\infty} \frac{1}{n!}$ |
| $\sqrt{2}$ | $1.41421$ | $\sqrt{2}$ |
| $\sqrt{3}$ | $1.73205$ | $\sqrt{3}$ |
| $\ln 2$ | $0.69315$ | $\sum_{n=1}^{\infty} \frac{(-1)^{n+1}}{n}$ |
| $\ln 10$ | $2.30259$ | 自然对数 |

### 16.2 误差分析表

| 误差类型 | 定义 | 计算公式 |
|---------|------|----------|
| **绝对误差** | $\Delta x = x^* - x$ | $x^*$ 为近似值， $x$ 为精确值 |
| **相对误差** | $\delta_x = \frac{\Delta x}{x}$ | 绝对误差除以精确值 |
| **相对误差限** | $\|\delta_x\| \leq \delta$ | 相对误差的上界 |

---

## 📋 使用说明

### 快速查找方法
> 1. **按知识点分类**：根据题目涉及的数学分支选择相应表格
> 2. **关键词搜索**：使用Ctrl+F搜索具体函数或公式
> 3. **重要程度优先**：优先掌握标注⭐⭐⭐⭐⭐的公式
> 4. **定义域检查**：注意每个公式的适用条件和定义域限制

### 学习建议
> 1. **分层掌握**：基础篇→提高篇→高级篇→专题篇的顺序学习
> 2. **理解推导**：不仅记忆结果，更要理解推导过程
> 3. **练习验证**：通过典型例题验证公式应用
> 4. **定期复习**：建立复习计划，保持熟练度
> 5. **综合运用**：练习不同章节知识点的综合应用

### 考试策略
> 1. **优先级排序**：等价无穷小 > 洛必达法则 > 泰勒展开
> 2. **基础公式**：导数积分表要达到条件反射级别
> 3. **特殊方法**：掌握各类积分的特殊技巧
> 4. **级数判断**：熟练运用各种收敛判定法
> 5. **多元函数**：重点掌握偏导数和重积分计算

### 备考重点
> **基础必备（⭐⭐⭐⭐⭐级）**：
> - 等价无穷小的12个基本公式
> - 基本初等函数导数表
> - 基本积分表
> - 重要极限公式
> - 三角函数恒等式表
> 
> **提高加分（⭐⭐⭐⭐级）**：
> - 常用泰勒级数展开
> - 高阶导数公式
> - 特殊积分计算
> - 级数收敛判定
> - 微分方程求解
> 
> **冲刺满分（⭐⭐⭐级）**：
> - 多元函数相关公式
> - 曲线曲面积分
> - 特殊函数应用
> - 数值计算方法
