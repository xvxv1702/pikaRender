#pragma once
#ifndef __MYCAMERA_H__
#define __MYCAMERA_H__

#include "geometry.h"

class Camera {
	public:
		Vec3f pos;
		Vec3f dir;
		Vec3f updir;
		float fov;
		float Near;
		float Far;

		Camera(float fov0,float Near0,float Far0):fov(fov0),Near(Near0),Far(Far0) {}

		//Vec3f rotate(float angle);
		void get_pos(float x,float y,float z) {
			pos = Vec3f(x,y,z);
		}
		void get_dir(float x, float y, float z) {
			dir= Vec3f(x, y, z).normalize();
		}
		void get_dir() {
			dir = Vec3f(0.0 - pos.x, 0.0 - pos.y, 0.0 - pos.z).normalize();
		}
		void get_updir(float x, float y, float z) {
			updir = Vec3f(x, y, z).normalize();
		}

};



#endif

