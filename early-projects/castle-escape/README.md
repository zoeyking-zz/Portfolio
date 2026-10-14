# 古堡逃杀｜Unity 本科游戏作品

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
- 开始界面能否自动跳转至主游戏尚未验证。若不能跳转，可尝试直接打开 `主工程/古堡逃杀.exe`。
- 当前归档未完成在本机的启动、兼容性和完整流程测试，因此不标记为“已验证可运行”。
- 文件体积、SHA-256 与包含内容见 [归档清单](archive-manifest.json)。下载页同时提供 ZIP 的 SHA-256 校验文件。

## 源码状态

2026-09-14 盘点时，提供的目录包含 `.exe`、`UnityPlayer.dll`、`GameAssembly.dll`、`_Data` 运行资源，以及 `BackUpThisFolder_ButDontShipItWithYourGame` 编译备份。未找到 `Assets/`、`Packages/`、`ProjectSettings/` 或原始 `.cs` 文件。

编译备份中的 `il2cppOutput` 是 Unity 转换生成的 C/C++，`Managed` 中的程序集与 `.pdb` 是构建和调试材料；它们不等同于原始 Unity 编辑器工程。本次发布排除了这些备份及调试符号，保留游戏运行所需的文件。

如后续找回包含 `Assets/` 和 `ProjectSettings/` 的工程，可再补充源代码、Unity 版本、依赖、场景及复现说明。现有成品与源码状态分别记录。

## 角色被场景遮挡时的可见性处理（Shader Graph）

> 以下内容依据作者对历史 Unity 工程的复盘整理。当前公开仓库仅保留 Windows 成品，未保留可复现的 `Assets/`、Shader Graph 或 C# 源工程；因此它用于说明当时的技术方案，不声称现已可重新运行或逐项验证。

### 演示图片

<img width="1608" height="852" alt="image" src="https://github.com/user-attachments/assets/4aa223ee-82b3-48fb-b4b2-aeafdf54e8bd" />

### 问题

第三人称视角下，墙壁等场景物体可能同时带来两类遮挡：

1. **视觉遮挡：** 相机与怪物之间有墙面时，怪物会被遮住，玩家难以确认怪物位置和操作状态。
2. **鼠标射线遮挡：** 怪物的 `MeshCollider` 会先命中鼠标射线，使交互/选点射线无法穿过墙抵达目标。

当时的处理将两类问题分开：用怪物 Shader 提升被遮挡时的辨识度；对不需要承载物理或交互的墙面 MeshCollider 关闭或忽略，避免它们阻断鼠标射线。

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

### 原理与取舍

- **Fresnel：** 以视角相关的边缘权重强调轮廓。它本身并不检测“是否被墙遮挡”，而是给角色提供更容易辨认的边缘表现；若需要仅在遮挡发生时启用/切换材质，仍需由相机射线、可见性逻辑或其他游戏逻辑驱动。
- **Unlit：** 不受场景光照直接影响，适合做稳定、醒目的可见性提示；代价是它不追求与原角色 PBR 光照完全一致。
- **Dither + Alpha Clip：** 通过裁剪形成半透明的视觉过渡，同时避免传统透明排序的一部分问题；但近看可能有颗粒感，阈值过高会让角色破碎或闪烁。
- **可调属性：** 将 Color 和阈值暴露给 Inspector，便于在不同场景、相机距离和美术风格下快速调整，而无需重写 Shader Graph。

### 鼠标射线处理

对于只承担视觉装饰、无需阻挡怪物或其他物理对象的墙面，当时关闭其 `MeshCollider`，让鼠标射线不再优先命中墙面。

> 边界：关闭 Collider 会同时失去该对象的物理碰撞能力。若前面仍需要物理碰撞，更稳妥的延伸方案是将其放到专用 Layer，并让鼠标射线的 LayerMask 排除该 Layer。

### 可延伸的效果

以下是基于该思路可继续实现的方向，为本项目未来可能添加的内容：

- **X-ray / 遮挡描边：** 使用相机到怪物的射线判断遮挡，仅在遮挡时启用轮廓材质或切换参数。
- **距离与遮挡渐隐：** 根据相机距离、遮挡深度或屏幕空间深度控制 Dither 阈值，使过渡更平滑。
- **深度/Stencil 方案：** 通过深度比较、Stencil 或 URP Renderer Feature 实现更严格的“透过遮挡物看怪物”效果。
- **对象分层交互：** 用 LayerMask 区分视觉装饰、可交互物和物理障碍，避免把所有场景物体都作为鼠标射线阻挡者。
- **性能与表现调优：** 按相机距离降低效果强度，限制材质切换频率，并在不同背景/光照条件下检查轮廓对比度与抖动可见性。


## 许可

此作品供求职展示，未附开源许可证。Unity 运行库及第三方美术、音频等素材的权利归原权利人；发布成品不表示这些依赖或素材由本人创作，也不授予单独提取和再分发素材的许可。
