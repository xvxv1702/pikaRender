#pragma once

#ifndef __MYRASTER_H__
#define __MYRASTER_H__
#define NOMINMAX //graphics.h里的min max与原来的std::min,std::max冲突
#include "model.h"
#include "geometry.h"
#include "myCamera.h"
#include<Windows.h>
#include "tgaimage.h"
#include "mymath.h"
#include <cmath>

class Raster {
private:
	Matrix get_modelMat();
	Matrix get_viewMat();
	Matrix get_projectionMat();
	Matrix get_tranMat();
	Matrix get_TBNMat(Vec3f norm,Vec3f v0, Vec3f v1, Vec3f v2, Vec2f uv0, Vec2f uv1, Vec2f uv2);
	

public:
	Model* model;
	Camera* camera;
	int width;
	int height;

	void shadeMode(HDC  memDC, TGAImage* baseTex,Vec3f ldir,float* depthBuffer);
	void shadeMode2(HDC  memDC, TGAImage* baseTex, TGAImage* normTex, Vec3f ldir, float* depthBuffer);//含法线贴图
	void shadeMode3(HDC  memDC, TGAImage* baseTex, TGAImage* tan_normTex, Vec3f ldir, float* depthBuffer);//切线空间法线贴图
	void phongMode1(HDC  memDC, Vec3f ldir, float* depthBuffer);//不含法线贴图,使用obj文件包含的法线插值
	void phongMode2(HDC  memDC, TGAImage* normTex, Vec3f ldir, float* depthBuffer);//含法线贴图
	void phongMode3(HDC  memDC, TGAImage* tan_normTex, Vec3f ldir, float* depthBuffer);//提供切线空间法线贴图
	void colorMode(HDC  memDC, TGAImage* baseTex, float* depthBuffer);
	void faceMode(HDC  memDC, Vec3f ldir, float* depthBuffer);//直接计算面的法线，半兰伯特着色
	void lineMode(HDC  memDC);
	void get_model(Model* model0);
	void get_camera(Camera* camera0);
	void get_screensize(int width0,int height0);
};


void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color);
void line(Vec3f t0, Vec3f t1, HDC  memDC, COLORREF color);
#endif
