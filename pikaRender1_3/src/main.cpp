#define NOMINMAX //graphics.h���min max��ԭ����std::min,std::max��ͻ
#pragma comment(linker, "/ENTRY:wWinMainCRTStartup")// ������ڵ���wWinMainCRTStartup�������������ǾͿ���ʹ��wWinMain�������������main����
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")// ����Windows���򣬾��ǲ���С�ڿ��ͼ�ν������
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// �����ﴦ�����д�����Ϣ
	switch (msg)
	{
	case WM_DESTROY:
		// ����������ʱ�˳�Ӧ�ó���
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
//��ں���
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	//-------------------------------------------------------------------------------------------------------------
	// ��ʼ��������ṹ��
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));//���ڱ���ɫ
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = TEXT("mainWindow");//�����������
	// ע�ᴰ����
	RegisterClassEx(&wc);
	// �������ڲ���ʾ����
	HWND hWnd = CreateWindowEx(0, wc.lpszClassName, TEXT("pikaRender"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, //����λ�úͳߴ�
		nullptr, nullptr, nullptr, nullptr);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	HDC hDC = GetDC(hWnd);//���ϵͳ��ͼ�豸

	//---------------------------------------------------------------------------------------------------------------
	//��������
	Vec3f ldir = Vec3f(-1., 1., 1.0).normalize();
	//�������
	Camera Cam(70.0, 1.0, 3.0);
	Cam.get_pos(0.0, 0.0, 2.0);
	Cam.get_dir();
	Cam.get_updir(0., 1., 0.);

	//����ģ��
	Model* model = NULL;
	model = new Model("./asset/manhead/african_head.obj");
	TGAImage tex, normtex,tan_normtex;
	if (tex.read_tga_file("./asset/manhead/african_head_diffuse.tga")) std::cout << "tex imported" << std::endl;
	if (normtex.read_tga_file("./asset/manhead/african_head_nm.tga")) std::cout << "normtex imported" << std::endl;
	//if (tan_normtex.read_tga_file("./asset/manhead/african_head_nm_tangent.tga")) std::cout << "normtex imported" << std::endl;

	//������դ����
	Raster raster;
	raster.get_camera(&Cam);
	raster.get_model(model);
	raster.get_screensize(width,height);

	//---------------------------------------------------------------------------------------------------------------
	//��ʼ����
	while (1) {
		//Cam.pos.z += 0.05;
		ldir.x += 0.1;
		ldir.normalize();
		HDC memDC = CreateCompatibleDC(0);//����������ͼ�豸
		HBITMAP bmpBack = CreateCompatibleBitmap(hDC, width, height);//��������λͼ��������
		SelectObject(memDC, bmpBack);//������������ͼ�豸��
		//raster.shadeMode(memDC, &tex,  ldir, depthBuffer);//���ṩ������ͼ����������ɫ
		//raster.shadeMode(memDC, &tex, &normtex, ldir, depthBuffer);//�ṩ������ͼ
		//raster.shadeMode(memDC, &tex, &normtex, &tan_normtex, ldir, depthBuffer);//�ṩ������ͼ�����߿ռ䷨����ͼ������������̫����
		raster.lineMode(memDC);

		BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);//���Ƶ�ϵͳ�豸����ʾ
		//����ı�
		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkColor(hDC, RGB(0, 255, 0));
		SetBkMode(hDC, TRANSPARENT);
		TCHAR text[] = TEXT("color mode");
		TextOut(hDC, 10, 10, text, ARRAYSIZE(text) - 1);//hdc����x���꣬y���꣬���֣����ֳ���
		ReleaseDC(hWnd, memDC);//�ͷŸ�����ͼ��
	}
	//-----------------------------------------------------------------------------------------------------------------------
	// ������Ϣѭ��
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);
	ReleaseDC(hWnd, hDC);//�ͷ�ϵͳ��ͼ��

	delete model;
}