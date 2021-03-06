# pikaRender
基于c++实现的一个软光栅器，目前完成了基本框架的搭建，后期逐渐添加更多功能，欢迎指正。

项目中仅使用c++原生api，尽量减少对别的库的依赖，来还原真实的渲染管线。项目中参考了部分tinyrender教程，地址https://github.com/ssloy/tinyrenderer。

#### 目前实现的功能

**文件层**：obj格式模型读取，tga格式文件读取和保存。

**数学层**：基本的向量和矩阵运算，必要的数学运算如clamp

**平台层**：基于windows的gdi实现渲染结果的显示

**功能层**：MVP变换、三角形光栅化、深度测试、phong光照模型、基础纹理运用、法线插值、法线贴图（模型空间与切线空间）

#### **有待实现的功能：**

阴影，AO，PBR材质，可交互的3d环绕相机系统，硬件加速，光线追踪

#### 部分效果：

<img src="https://github.com/xvxv1702/pikaRender/blob/main/pikaRender1_3/asset/result/linemode.png" alt="线稿模式" style="zoom:50%;" />

<img src="https://github.com/xvxv1702/pikaRender/blob/main/pikaRender1_3/asset/result/colormode.png" alt="贴图模式" style="zoom:50%;" />

<img src="https://github.com/xvxv1702/pikaRender/blob/main/pikaRender1_3/asset/result/facemode.png" alt="面模式" style="zoom:50%;" />

<img src="https://github.com/xvxv1702/pikaRender/blob/main/pikaRender1_3/asset/result/phongmode_normlerp.png" alt="法线插值模式" style="zoom:50%;" />

<img src="https://github.com/xvxv1702/pikaRender/blob/main/pikaRender1_3/asset/result/phongmode_normmap.png" alt="法线贴图模式" style="zoom:50%;" />

<img src="https://github.com/xvxv1702/pikaRender/blob/main/pikaRender1_3/asset/result/shademode.png" alt="着色模式" style="zoom:50%;" />
