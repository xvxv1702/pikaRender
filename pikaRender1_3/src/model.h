#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec3f> normal_;
	std::vector<Vec2f> uv_;
	std::vector<std::vector<int> > faces_;//顶点索引
	std::vector<std::vector<int> > uvIndex_;//纹理坐标索引
	std::vector<std::vector<int> > normIndex_;//法线索引
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Vec3f normal(int i);
	Vec2f uv(int i);
	std::vector<int> face(int idx);
	std::vector<int> uvIndex(int idx);
	std::vector<int> normIndex(int idx);
};

#endif //__MODEL_H__
