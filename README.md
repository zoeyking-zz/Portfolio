# Technical Art & Real-Time Graphics — Selected Work

个人技术美术、角色动画工具与实时图形作品集。近期项目放在仓库顶层，本科及早期项目统一收录于 [early-projects](early-projects/)。

## 近期项目

| 项目 | 内容 | 入口 |
|---|---|---|
| [Gen3D Inspector](gen3d-inspector/) | 浏览器本地多格式 3D 资产检查器；支持 Graphdeco Gaussian Splat 真渲染与自适应视口 | [在线演示](https://zoeyking-zz.github.io/Portfolio/) · [完整源码](gen3d-inspector/) |
| [Gen3D Asset Factory](gen3d-asset-factory/) | Blender 静态资产检查、规范命名、GLB 导出与重新导入核对；0.2.0 预览版 | [安装包](gen3d-asset-factory/dist/gen3d_factory-0.2.0.zip) · [测试记录](gen3d-asset-factory/docs/testing.md) |

## 本科与早期项目

| 项目 | 技术关键词 | 当前材料 | 状态 |
|---|---|---|---|
| [古堡逃杀](early-projects/castle-escape/) | Unity、C#、角色交互、UI、氛围表现 | [Windows 历史成品下载](https://github.com/zoeyking-zz/Portfolio/releases/tag/castle-escape-archive-2026-09-14) | 包含开始界面和主游戏；尚未找回原始 Unity 工程 |
| [Blender 实时动捕与虚拟角色工具](early-projects/blender-realtime-mocap/) | Blender、Python、Motion Capture、Facial Animation | 案例说明、演示关键画面；完整视频可按需提供 | 可作为案例展示；源码已无法找回 |
| [Unity 布料模拟](early-projects/unity-cloth-simulation/) | Unity、C#、Mesh、Constraint、Collision、Shader | 核心脚本与 Shader | 代码样本；场景与贴图未公开 |
| [OpenGL 光照与相机交互](early-projects/opengl-lighting-demo/) | C++、OpenGL、GLSL、Phong、GLFW、GLEW | 源码、Shader、GIF | 早期课程项目；依赖与贴图未打包 |
| [UNO](early-projects/uno/) | C++、SFML、卡牌规则、Windows 桌面程序 | 源码、素材与已验证启动文件 | 可在 Windows 本地运行 |
| [扫雷](early-projects/minesweeper/) | C++、SFML、网格交互、Windows 桌面程序 | 源码、素材与已验证启动文件 | 可在 Windows 本地运行 |
| [贪吃蛇](early-projects/snake/) | C++、SFML、动画、音效、Windows 桌面程序 | 源码、素材与已验证启动文件 | 可在 Windows 本地运行 |
| [植物大战僵尸](early-projects/plants-vs-zombies/) | JavaScript、CreateJS、Adobe Animate | 可在线运行的网页导出、贴图与设计文档 | GitHub Pages 部署后可直接游玩 |
| [Unity 正弦波流体模拟](early-projects/unity-fluid-simulation/) | Unity、C#、Mesh、正弦位移、实时顶点变形 | Windows 历史成品、GIF 与实验报告 | 原始 Unity 工程与源码缺失 |

## 文件组织

```text
technical-art-portfolio-staging/
├── README.md
├── gen3d-inspector/           # 近期项目：在线 3D 资产检查器完整源码
├── gen3d-asset-factory/       # 近期项目：源码、文档、安装包与演示
├── early-projects/           # 本科及早期作品统一归档
│   ├── README.md
│   ├── castle-escape/
│   ├── blender-realtime-mocap/
│   ├── unity-cloth-simulation/
│   ├── unity-fluid-simulation/
│   ├── opengl-lighting-demo/
│   ├── uno/
│   ├── minesweeper/
│   ├── snake/
│   └── plants-vs-zombies/
└── GAME_EXPERIENCE.md
```
