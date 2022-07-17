#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v.raw[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;//顶点索引
            std::vector<int> uv;//uv索引
            std::vector<int> n;//法向量索引
            int idx, iuv, in;
            iss >> trash;
            while (iss >> idx >> trash >> iuv >> trash >> in) {//f后面由v/vt/vn组成
                idx--; // in wavefront obj all indices start at 1, not zero
                iuv--;
                in--;
                f.push_back(idx);
                uv.push_back(iuv);
                n.push_back(in);//写入法线索引
            }
            faces_.push_back(f);
            uvIndex_.push_back(uv);
            normIndex_.push_back(n);
        }
        else if (!line.compare(0, 2, "vn")) {
            iss >> trash;
            iss >> trash;
            Vec3f n;
            for (int i = 0; i < 3; i++) iss >> n.raw[i];
            normal_.push_back(n);
        }
        else if (!line.compare(0, 2, "vt")) {
            iss >> trash;
            iss >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; i++) iss >> uv.raw[i];
            for (int i = 0; i < 2; i++) {//设置如果uv的范围超过0-1，就重复使用贴图
                while (uv.raw[i] > 1.0)uv.raw[i] -= 1.0;
            }
            uv_.push_back(uv);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}
std::vector<int> Model::uvIndex(int idx) {
    return uvIndex_[idx];
}
std::vector<int> Model::normIndex(int idx) {
    return normIndex_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::normal(int i) {
    return normal_[i];
}

Vec2f Model::uv(int i) {
    return uv_[i];
}