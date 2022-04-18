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
Matrix v2m(Vec3f v);//把三维向量转换为齐次坐标系
Matrix p2m(Vec3f v);//把三维点坐标转换为齐次坐标系
Vec3f m2v(Matrix m);//把齐次坐标系转换为三维向量

Matrix get_modelMat();
Matrix get_viewMat(Vec3f camPos, Vec3f camDir, Vec3f camUp);
Matrix get_projectionMat(int width, int height, float fov, float Near, float Far);

#endif