#pragma once
#ifndef __MYCAMERA_H__
#define __MYCAMERA_H__

#include "geometry.h"
#include <cmath>
#include "mymath.h"

class Camera {
	public:
		Vec3f pos;
		Vec3f dir;
		Vec3f updir;
		float fov;
		float Near;
		float Far;

		Camera(float fov0,float Near0,float Far0):fov(fov0),Near(Near0),Far(Far0) {}

		void rotate(float angle) {
			Matrix ro = Matrix::identity(4);
			ro[0][0] = cos(angle); ro[0][2] = sin(angle);
			ro[2][0] = -sin(angle); ro[2][2] = cos(angle);

			pos = m2v(ro * p2m(this->pos));
			get_dir();
		}
		void get_pos(float x,float y,float z) {
			pos = Vec3f(x,y,z);
		}
		void get_dir(float x, float y, float z) {
			dir= Vec3f(x, y, z).normalize();
		}
		void get_dir() {
			dir = Vec3f(0.0 - pos.x, 0.0 - pos.y, 0.0 - pos.z).normalize();//指向原点
		}
		void get_updir(float x, float y, float z) {
			updir = Vec3f(x, y, z).normalize();
		}

};



#endif

