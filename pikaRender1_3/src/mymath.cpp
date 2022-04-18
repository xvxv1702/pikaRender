#include <vector>
#include <cmath>
#include "geometry.h"
#include "tgaimage.h"


int cross(Vec2i t0, Vec2i t1) {
    return t0.x * t1.y - t1.x * t0.y;
}
float cross(Vec2f t0, Vec2f t1) {
    return t0.x * t1.y - t1.x * t0.y;
}
Vec3f cross(Vec3f t0, Vec3f t1) {
    return Vec3f(t0.y * t1.z - t1.y * t0.z, t0.z * t1.x - t1.z * t0.x, t0.x * t1.y - t1.x * t0.y);
}
float dot(Vec3f t0, Vec3f t1) {
    return t0.x * t1.x + t0.y * t1.y + t0.z * t1.z;
}
float clamp(float min, float max, float x) {
    return std::max(min, std::min(x, max));
}
Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 0.f;
    return m;
}//把三维向量转换为齐次坐标系
Matrix p2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}//把三维点坐标转换为齐次坐标系
Vec3f m2v(Matrix m) {
    Vec3f v;
    if (m[3][0] != 0.f) {
        v.x = m[0][0] / m[3][0];
        v.y = m[1][0] / m[3][0];
        v.z = m[2][0] / m[3][0];
    }
    else {
        v.x = m[0][0];
        v.y = m[1][0];
        v.z = m[2][0];
    }
    return v;
}//把齐次坐标系转换为三维向量

Matrix get_modelMat() {
    return Matrix::identity(4);
}
Matrix get_viewMat(Vec3f camPos, Vec3f camDir, Vec3f camUp) {
    Matrix t = Matrix::identity(4);
    Matrix r = Matrix::identity(4);
    t[0][3] = camPos.x * (-1.f);
    t[1][3] = camPos.y * (-1.f);
    t[2][3] = camPos.z * (-1.f);
    Vec3f cambdir = cross(camDir, camUp);//计算相机的x轴
    r[0][0] = cambdir.x; r[0][1] = cambdir.y; r[0][2] = cambdir.z;
    r[1][0] = camUp.x; r[1][1] = camUp.y; r[1][2] = camUp.z;
    r[2][0] = -camDir.x; r[2][1] = -camDir.y; r[2][2] = -camDir.z;
    Matrix v = r * t;
    return v;
}
Matrix get_projectionMat(int width, int height, float fov, float Near, float Far) {
    const float PI = 3.1415926;
    float r, l, t, b, n, f;
    float ratio = float(width) / float(height);
    n = -Near; f = -Far;
    t = Near * tan(fov / 2.f / 180.f * PI);
    b = -t;
    r = ratio * t;
    l = -r;
    Matrix oT = Matrix::identity(4);//平移矩阵
    oT[0][3] = -(r + l) / 2.f; oT[1][3] = -(t + b) / 2.f; oT[2][3] = -(n + f) / 2.f;
    std::cout << "oT" << std::endl << oT << std::endl;
    Matrix oS = Matrix::identity(4);//缩放矩阵
    oS[0][0] = 2.f / (r - l); oS[1][1] = 2.f / (t - b); oS[2][2] = 2.f / (n - f);
    std::cout << "oS" << std::endl << oS << std::endl;
    Matrix othMat = oS * oT;//正交投影矩阵
    std::cout << "othMat" << std::endl << othMat << std::endl;
    Matrix P2O = Matrix::identity(4);//视锥变化，视锥变为矩形，便于投影
    P2O[0][0] = n; P2O[1][1] = n; P2O[2][2] = n + f; P2O[2][3] = -n * f; P2O[3][2] = 1.f; P2O[3][3] = 0.f;
    std::cout << "P2O" << std::endl << P2O << std::endl;
    Matrix proMat = othMat * P2O;//透视投影矩阵
    std::cout << "proMat" << std::endl << proMat << std::endl;

    return  proMat;
}