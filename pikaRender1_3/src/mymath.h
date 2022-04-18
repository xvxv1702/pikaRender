#pragma once
#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <vector>
#include <cmath>
#include "geometry.h"
#include "tgaimage.h"


int cross(Vec2i t0, Vec2i t1);
float cross(Vec2f t0, Vec2f t1);
Vec3f cross(Vec3f t0, Vec3f t1);
float dot(Vec3f t0, Vec3f t1);
float clamp(float min, float max, float x);
Matrix v2m(Vec3f v);//����ά����ת��Ϊ�������ϵ
Matrix p2m(Vec3f v);//����ά������ת��Ϊ�������ϵ
Vec3f m2v(Matrix m);//���������ϵת��Ϊ��ά����

Matrix get_modelMat();
Matrix get_viewMat(Vec3f camPos, Vec3f camDir, Vec3f camUp);
Matrix get_projectionMat(int width, int height, float fov, float Near, float Far);

#endif