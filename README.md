# pikaRender
基于c++实现的一个软光栅器，目前完成了基本框架的搭建，后期逐渐添加更多功能，欢迎指正。

项目中仅使用c++原生api，尽量减少对别的库的依赖，来还原真实的渲染管线。项目中参考了部分tinyrender教程，地址https://github.com/ssloy/tinyrenderer。

#### 目前实现的功能

**文件层：**obj格式模型读取，tga格式文件读取和保存。

**数学层**：基本的向量和矩阵运算，必要的数学运算如clamp

**平台层：**基于windows的gdi实现渲染结果的显示

**功能层：**MVP变换、三角形光栅化、深度测试、phong光照模型、基础纹理运用

#### **有待实现的功能：**

阴影，AO，PBR材质，可交互的3d环绕相机系统，硬件加速，光线追踪，法线贴图

#### 部分效果：

![线稿模式](https://github.com/xvxv1702/pikaRender/blob/xvxv1702-patch-1/pikaRender1_3/asset/manhead/%E7%9D%80%E8%89%B2%E6%A8%A1%E5%BC%8F.png)

![phong模型](https://github.com/xvxv1702/pikaRender/blob/xvxv1702-patch-1/pikaRender1_3/asset/manhead/%E7%9D%80%E8%89%B2%E6%A8%A1%E5%BC%8F.png)

![着色模式](https://github.com/xvxv1702/pikaRender/blob/xvxv1702-patch-1/pikaRender1_3/asset/manhead/%E7%9D%80%E8%89%B2%E6%A8%A1%E5%BC%8F.png)
