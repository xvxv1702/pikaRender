#define NOMINMAX //graphics.h里的min max与原来的std::min,std::max冲突
#pragma comment(linker, "/ENTRY:wWinMainCRTStartup")// 程序入口点是wWinMainCRTStartup函数，这样我们就可以使用wWinMain函数替代常见的main函数
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")// 生成Windows程序，就是不带小黑框的图形界面程序
#include "myCamera.h"
#include<iostream>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include<iostream>
#include"mymath.h"
#include<Windows.h>
#include "myRaster.h"

const int width = 800;
const int height = 800;
float depthBuffer[width*height];
const float PI = 3.1415926;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 在这里处理所有窗口消息
	switch (msg)
	{
	case WM_DESTROY:
		// 当窗口销毁时退出应用程序
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
//入口函数
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	//-------------------------------------------------------------------------------------------------------------
	// 初始化窗口类结构体
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));//窗口背景色
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = TEXT("mainWindow");//窗口类的名称
	// 注册窗口类
	RegisterClassEx(&wc);
	// 创建窗口并显示窗口
	HWND hWnd = CreateWindowEx(0, wc.lpszClassName, TEXT("pikaRender"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, //窗口位置和尺寸
		nullptr, nullptr, nullptr, nullptr);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	HDC hDC = GetDC(hWnd);//获得系统绘图设备

	//---------------------------------------------------------------------------------------------------------------
	//创建光照
	Vec3f ldir = Vec3f(3.0, 0., 2.0).normalize();
	//创建相机
	Camera Cam(70.0, 1.0, 3.0);
	Cam.get_pos(0.0, 0.0,2.0);
	Cam.get_dir();
	Cam.get_updir(0., 1., 0.);

	//导入模型
	Model* model = NULL;
	if(model = new Model("./asset/crab/crab1.obj")) std::cout<<"model imported"<<std::endl;
	TGAImage tex, normtex,tan_normtex;
	if (tex.read_tga_file("./asset/crab/crab_diffuse.tga")) std::cout << "tex imported" << std::endl;
	if (normtex.read_tga_file("./asset/crab/african_head_nm.tga")) std::cout << "normtex imported" << std::endl;
	if (tan_normtex.read_tga_file("./asset/crab/crab_normal.tga")) std::cout << "normtex imported" << std::endl;

	//创建光栅化器
	Raster raster;
	raster.get_camera(&Cam);
	raster.get_model(model);
	raster.get_screensize(width,height);

	//---------------------------------------------------------------------------------------------------------------
	//开始绘制
	
	//准备画布
	HDC memDC = CreateCompatibleDC(0);//创建辅助绘图设备
	HBITMAP bmpBack = CreateCompatibleBitmap(hDC, width, height);//创建掩码位图（画布）
	SelectObject(memDC, bmpBack);//将画布贴到绘图设备上

	//Cam.rotate(PI / 18);
	//ldir.x += 0.1;
	//ldir.normalize();

	//开始绘制
	//raster.shadeMode(memDC, &tex,  ldir, depthBuffer);//不提供法线贴图，phong模型
	//raster.shadeMode2(memDC, &tex, &normtex, ldir, depthBuffer);//提供模型空间法线贴图，phong模型
	//raster.shadeMode3(memDC, &tex, &tan_normtex, ldir, depthBuffer);//提供切线空间法线贴图,phong模型
	//raster.phongMode1(memDC, ldir, depthBuffer);//使用模型记录的顶点法线插值，半兰伯特着色
	//raster.phongMode2(memDC,&normtex, ldir, depthBuffer);//提供模型空间法线贴图
	//raster.phongMode3(memDC,&tan_normtex, ldir, depthBuffer);//提供切线空间法线贴图，半兰伯特着色
	//raster.faceMode(memDC, ldir, depthBuffer);//直接计算面的法线，半兰伯特着色
	//raster.colorMode(memDC, &tex, depthBuffer);//贴图着色，无光照
	raster.lineMode(memDC);

	BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);//复制到系统设备上显示
	//输出文本
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, RGB(0, 255, 0));
	SetBkMode(hDC, TRANSPARENT);
	TCHAR text[] = TEXT("line mode");
	TextOut(hDC, 10, 10, text, ARRAYSIZE(text) - 1);//hdc名，x坐标，y坐标，文字，文字长度
	ReleaseDC(hWnd, memDC);//释放辅助绘图区
	
	//-----------------------------------------------------------------------------------------------------------------------
	// 进入消息循环
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
	ReleaseDC(hWnd, hDC);//释放系统绘图区

	delete model;
}