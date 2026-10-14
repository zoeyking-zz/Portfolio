# 古堡逃杀｜Unity 第三人称恐怖向动作游戏

本科阶段的 Unity 游戏作品。当前公开材料为历史 Windows 成品：开始界面与主游戏分别保留在两个目录中。
视频演示：[bilibili](https://www.bilibili.com/video/BV1aVpR6aEKL/?spm_id_from=333.1387.homepage.video_card.click&vd_source=81196c19ce83679c20e55a5cf9b5a4c3)

## 项目内容

根据个人经历记录，项目涵盖概念设计、原型迭代、功能开发、测试与打包，涉及角色移动、近战与远程攻击、武器切换、简单技能与装备，以及主菜单、血量、背包和设置等 UI；结合动画事件、受击反馈、镜头、音效、光照与粒子营造战斗和场景氛围。

以上为历史工作记录。本次整理没有重新验证全部玩法，也没有把编译产物当作原始 C# 实现。


## 下载与启动

前往 [GitHub Release：Windows 历史成品](https://github.com/zoeyking-zz/technical-art-portfolio-staging/releases/tag/castle-escape-archive-2026-09-14)，下载 `castle-escape-windows-archive.zip`，完整解压后使用。

```text
开始界面工程/
├── 古堡逃杀_开始界面.exe
├── 古堡逃杀_开始界面_Data/
├── UnityPlayer.dll
└── GameAssembly.dll 等运行依赖
主工程/
├── 古堡逃杀.exe
├── 古堡逃杀_Data/
├── UnityPlayer.dll
└── GameAssembly.dll 等运行依赖
```

- 可分别启动开始界面与主游戏的 `.exe`；请保持各自的 `_Data` 目录与 DLL 在原位置。
- 文件体积、SHA-256 与包含内容见 [归档清单](archive-manifest.json)。下载页同时提供 ZIP 的 SHA-256 校验文件。

### 源码状态

2026-09-14 盘点时，提供的目录包含 `.exe`、`UnityPlayer.dll`、`GameAssembly.dll`、`_Data` 运行资源，以及 `BackUpThisFolder_ButDontShipItWithYourGame` 编译备份。未找到 `Assets/`、`Packages/`、`ProjectSettings/` 或原始 `.cs` 文件。

编译备份中的 `il2cppOutput` 是 Unity 转换生成的 C/C++，`Managed` 中的程序集与 `.pdb` 是构建和调试材料；它们不等同于原始 Unity 编辑器工程。本次发布排除了这些备份及调试符号，保留游戏运行所需的文件。

如后续找回包含 `Assets/` 和 `ProjectSettings/` 的工程，可再补充源代码、Unity 版本、依赖、场景及复现说明。现有成品与源码状态分别记录。


## Shader Graph 剔除遮挡：怪物被墙面遮挡时的可见性处理

### 问题

第三人称视角下，墙壁等场景物体可能同时带来两类遮挡：

1. **视觉遮挡：** 相机与怪物之间有墙面时，怪物会被遮住，玩家难以确认怪物位置和操作状态。
2. **鼠标射线遮挡：** 怪物的 `MeshCollider` 会先命中鼠标射线，使交互/选点射线无法穿过墙抵达目标。

当时的处理将两类问题分开：用怪物 Shader 提升被遮挡时的辨识度；对不需要承载物理或交互的墙面 MeshCollider 关闭或忽略，避免它们阻断鼠标射线。

### 结果演示图片

<img width="1608" height="852" alt="image" src="https://github.com/user-attachments/assets/4aa223ee-82b3-48fb-b4b2-aeafdf54e8bd" />

<img width="1683" height="858" alt="image" src="https://github.com/user-attachments/assets/a883596b-41ed-43f9-aab9-fe39322e959f" />


### Shader Graph 实现

使用 **URP Unlit Shader Graph** 创建怪物的遮挡可见性材质；该材质替代角色原有材质，以 Fresnel 边缘光和抖动透明度提供更强的轮廓提示。

1. 在 `Material` 目录创建 **URP Unlit Shader Graph**，并基于该 Graph 创建材质（历史命名：`Occlusion`）。
2. 在 Graph 中新增 **Fresnel Effect** 节点。Fresnel 按表面法线与视线方向的夹角输出边缘权重，用于让角色轮廓比正面区域更显眼。
3. 在 Blackboard 新建可从 Inspector 调整的 **Color** 属性；将其拖入 Graph。
4. 使用 **Multiply** 将 Fresnel 输出与 Color 相乘，再将结果连接到 Fragment 的 **Base Color**。这样可以在材质层面独立调整轮廓颜色，而不必改动 Graph。
5. 添加 **Dither** 节点，利用抖动图案让透明/裁剪边缘不呈现整齐硬切线。
6. 添加可调 **Float** 属性，将 Dither 的输出接入 Fragment 的 **Alpha**；再以 Float 值控制 **Alpha Clip Threshold**。阈值决定哪些抖动像素保留、哪些被裁剪，从而控制遮挡可见性效果的强弱和过渡。
7. 在材质/Graph 的 Surface 设置中启用与 Alpha Clip 对应的裁剪配置，并在 Inspector 中调节 Color 与阈值，观察角色轮廓与透明过渡。

逻辑可概括为：

```text
Fresnel Effect ──┐
                 ├─ Multiply ──> Fragment / Base Color
Color (Property) ┘

Dither + Float (Property) ──> Fragment / Alpha
Float (Property) ───────────> Alpha Clip Threshold
```

#### 原理与取舍

- **Fresnel：** 以视角相关的边缘权重强调轮廓。它本身并不检测“是否被墙遮挡”，而是给角色提供更容易辨认的边缘表现；若需要仅在遮挡发生时启用/切换材质，仍需由相机射线、可见性逻辑或其他游戏逻辑驱动。
- **Unlit：** 不受场景光照直接影响，适合做稳定、醒目的可见性提示；代价是它不追求与原角色 PBR 光照完全一致。
- **Dither + Alpha Clip：** 通过裁剪形成半透明的视觉过渡，同时避免传统透明排序的一部分问题；但近看可能有颗粒感，阈值过高会让角色破碎或闪烁。
- **可调属性：** 将 Color 和阈值暴露给 Inspector，便于在不同场景、相机距离和美术风格下快速调整，而无需重写 Shader Graph。

#### 鼠标射线处理

对于只承担视觉装饰、无需阻挡怪物或其他物理对象的墙面，当时关闭其 `MeshCollider`，让鼠标射线不再优先命中墙面。

> 边界：关闭 Collider 会同时失去该对象的物理碰撞能力。若前面仍需要物理碰撞，更稳妥的延伸方案是将其放到专用 Layer，并让鼠标射线的 LayerMask 排除该 Layer。

## Shader Graph：程序化传送门视觉

在场景中以 Quad 作为传送门表面，使用 **URP Lit + Transparent Shader Graph** 构建可参数化的旋涡能量视觉。该效果的目标不是依赖固定帧动画，而是用 UV 变形和程序化纹理持续生成流动图案；通过材质参数可快速调整颜色、流速、纹理密度与对比度。

### Graph 与材质参数

保留的 Shader Graph 截图显示，材质公开了以下可调参数：

| 参数 | 用途 |
|---|---|
| `Speed` | 控制随时间变化的 UV 扭曲/流动速度 |
| `Strength` | 控制最终图案或颜色表现的强度 |
| `Mask` | 圆形/径向遮罩，将效果约束在传送门轮廓内 |
| `Color` | 控制传送门颜色，可在材质 Inspector 中直接调节 |
| `CellScale` | 控制 Voronoi 纹理的细胞密度与细节尺度 |
| `PowerScale` | 控制程序化图案的明暗对比与亮部收束程度 |

节点链的核心思路为：

```text
UV0
 ↓
Twirl（Time × Speed 驱动）
 ↓
Voronoi（CellScale 控制细节）
 ↓
Power（PowerScale 强化对比）
 ↓
与 Mask、Color、Strength 多次 Multiply
 ↓
透明传送门表面
```

### 关键实现与原理

- **动画 UV 扭曲：** `Time` 与 `Speed` 相乘后驱动 `Twirl`。Twirl 将平面 UV 围绕中心卷曲，使静态程序化纹理形成持续旋转、向中心汇聚的运动感。
- **程序化能量纹理：** `Voronoi` 生成细胞状的连续变化图案；在扭曲后的 UV 上采样时，图案会由规则细胞转化为旋涡式能量流，而非一张重复滚动的静态贴图。
- **对比控制：** `Power` 对 Voronoi 的数值分布做非线性调整。通过 `PowerScale` 可控制亮部是否更集中、暗部是否更干净，从而在“柔和雾状”和“锐利能量条纹”之间取舍。
- **形状与颜色：** `Mask` 的径向衰减将程序化纹理裁切为圆形/椭圆形传送门，避免效果暴露 Quad 的矩形边界；`Color` 与 `Strength` 参与末端叠加，提供可快速迭代的颜色与强度控制。
- **透明表面：** Graph 设置为 Transparent，使 Mask 边缘可自然淡出。透明效果需要结合具体场景检查排序、背景对比度和边缘颗粒感，避免在复杂半透明物体附近产生视觉问题。

### 设计价值

该实现将“旋转速度、能量密度、对比度、形状和颜色”从节点逻辑中抽成材质参数。即使不修改 Graph，美术或关卡使用者也能为不同区域、阵营或剧情状态制作不同传送门预设；程序化纹理避免了依赖一套固定动画贴图的重复感。

需要区分的是：该 Shader 负责**传送门的视觉表面**，实际的角色移动、入口/出口配对、触发器、冷却时间和场景切换应由独立的 Collider/Trigger 与游戏脚本处理。

### 演示与证据状态

> 待补充：Shader Graph 完整截图、Game View 运行效果、不同 `Color / Speed / CellScale / PowerScale` 组合的对比图或 GIF。

当前公开仓库保留的是历史 Windows 成品和 Graph 截图，未包含原始 Unity `Assets/`、Shader Graph 资产或 C# 源工程。因此本节仅陈述截图可见的参数、节点类型和可解释的高层处理链，不将未保留的精确端口连接、实际传送脚本、性能数据或完整复现能力表述为已验证事实。

## 许可

此作品供求职展示，未附开源许可证。Unity 运行库及第三方美术、音频等素材的权利归原作者。
