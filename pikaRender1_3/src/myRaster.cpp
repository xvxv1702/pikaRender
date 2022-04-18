#include "myRaster.h"
using namespace std;

void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color) {
    bool steep = false;
    if (std::abs(t0.x - t1.x) < std::abs(t0.y - t1.y)) {
        std::swap(t0.x, t0.y);
        std::swap(t1.x, t1.y);
        steep = true;
    }
    if (t0.x > t1.x) {
        std::swap(t0.x, t1.x);
        std::swap(t0.y, t1.y);
    }

    for (int x = t0.x; x <= t1.x; x++) {
        float t = (x - t0.x) / (float)(t1.x - t0.x);
        int y = t0.y * (1. - t) + t1.y * t;
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
    }
}
void line(Vec3f t0, Vec3f t1, HDC  memDC, COLORREF color) {
    bool steep = false;
    if (std::abs(t0.x - t1.x) < std::abs(t0.y - t1.y)) {
        std::swap(t0.x, t0.y);
        std::swap(t1.x, t1.y);
        steep = true;
    }
    if (t0.x > t1.x) {
        std::swap(t0.x, t1.x);
        std::swap(t0.y, t1.y);
    }

    for (int x = t0.x; x <= t1.x; x++) {
        float t = (x - t0.x) / (float)(t1.x - t0.x);
        int y = t0.y * (1. - t) + t1.y * t;
        if (steep) {
            SetPixel(memDC, y, x, color);
        }
        else {
            SetPixel(memDC, x, y, color);
        }
    }
}
Matrix Raster::get_modelMat() {
	return Matrix::identity(4);
}
Matrix Raster::get_viewMat() {
    Matrix t = Matrix::identity(4);
    Matrix r = Matrix::identity(4);
    t[0][3] = camera->pos.x * (-1.f);
    t[1][3] = camera->pos.y * (-1.f);
    t[2][3] = camera->pos.z * (-1.f);
    Vec3f cambdir = (camera->dir)^ (camera->updir);//计算相机的x轴
    r[0][0] = cambdir.x; r[0][1] = cambdir.y; r[0][2] = cambdir.z;
    r[1][0] = camera->updir.x; r[1][1] = camera->updir.y; r[1][2] = camera->updir.z;
    r[2][0] = (camera->dir.x) * (-1.f); r[2][1] = (camera->dir.y) * (-1.f); r[2][2] = (camera->dir.z) * (-1.f);
    Matrix v = r * t;
    return v;
}
Matrix Raster::get_projectionMat() {
    const float PI = 3.1415926;
    float r, l, t, b, n, f;
    float ratio = float(this->width) / float(this->height);
    n = -(this->camera->Near); f = -(this->camera->Far);
    t = (this->camera->Near) * tan((this->camera->fov) / 2.f / 180.f * PI);
    b = -t;
    r = ratio * t;
    l = -r;
    Matrix oT = Matrix::identity(4);//平移矩阵
    oT[0][3] = -(r + l) / 2.f; oT[1][3] = -(t + b) / 2.f; oT[2][3] = -(n + f) / 2.f;
    //std::cout << "oT" << std::endl << oT << std::endl;
    Matrix oS = Matrix::identity(4);//缩放矩阵
    oS[0][0] = 2.f / (r - l); oS[1][1] = 2.f / (t - b); oS[2][2] = 2.f / (n - f);
    //std::cout << "oS" << std::endl << oS << std::endl;
    Matrix othMat = oS * oT;//正交投影矩阵
    //std::cout << "othMat" << std::endl << othMat << std::endl;
    Matrix P2O = Matrix::identity(4);//视锥变化，视锥变为矩形，便于投影
    P2O[0][0] = n; P2O[1][1] = n; P2O[2][2] = n + f; P2O[2][3] = -n * f; P2O[3][2] = 1.f; P2O[3][3] = 0.f;
    //std::cout << "P2O" << std::endl << P2O << std::endl;
    Matrix proMat = othMat * P2O;//透视投影矩阵
    //std::cout << "proMat" << std::endl << proMat << std::endl;

    return  proMat;
}
Matrix Raster::get_tranMat() {
    Matrix modelMat= get_modelMat();
    Matrix viewMat = get_viewMat();
    Matrix projectionMat = get_projectionMat();
    return projectionMat * viewMat * modelMat;
    //return viewMat * modelMat;
    //return modelMat;
}
Matrix Raster::get_TBNMat(Vec3f norm,Vec3f v0, Vec3f v1, Vec3f v2, Vec2f uv0, Vec2f uv1, Vec2f uv2) {
    Vec3f edge1 = v1 - v0; Vec3f edge2 = v2 - v0;
    Vec3f tangent, bitangent;
    Vec2f deltaUV1 = uv1 - uv0; Vec2f deltaUV2 = uv2 - uv0;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent.normalize();

    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent.normalize();

    Matrix modelMat = get_modelMat();
    tangent = m2v(modelMat * v2m(tangent)) * (-1);
    bitangent = m2v(modelMat * v2m(bitangent)) * (-1);
    norm= m2v(modelMat * v2m(norm)) * (-1);

    Matrix TBNMat = Matrix::identity(4);
    TBNMat[0][0] = tangent.x; TBNMat[0][1] = bitangent.x; TBNMat[0][2] = norm.x;
    TBNMat[1][0] = tangent.y; TBNMat[1][1] = bitangent.y; TBNMat[1][2] = norm.y;
    TBNMat[2][0] = tangent.z; TBNMat[2][1] = bitangent.z; TBNMat[2][2] = norm.z;

    return TBNMat;
}
void Raster::get_model(Model* model0) {
    model = model0;
}
void Raster::get_camera(Camera* camera0) {
    camera = camera0;
}
void Raster::get_screensize(int width0, int height0) {
    width = width0;
    height = height0;
}
void Raster::shadeMode(HDC  memDC, TGAImage* baseTex, Vec3f ldir,float * depthBuffer) {
    //初始化深度缓存区
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            depthBuffer[i * width + j] = std::numeric_limits<float>::max() * (-1);
        }
    }
    Matrix tranMat = get_tranMat();
    //ofstream log("./renderlog.txt");
    //log << tranMat << endl;
    //log.close();

    for (int i = 0; i < model->nfaces(); i++) {
        Vec3f v0= model->vert(model->face(i)[0]); Vec3f v1 = model->vert(model->face(i)[1]); Vec3f v2 = model->vert(model->face(i)[2]);
        Vec2f uv0 = model->uv(model->uvIndex(i)[0]); Vec2f uv1 = model->uv(model->uvIndex(i)[1]); Vec2f uv2 = model->uv(model->uvIndex(i)[2]);
        Vec3f norm = (v1 - v0)^(v2 - v0) ;
        norm.normalize();
        float ndotl = ldir*norm;//光照计算
        ndotl = clamp(0., 1., ndotl);
        //模型mvpv变换
        v0= m2v(tranMat * p2m(v0)); v1= m2v(tranMat * p2m(v1)); v2= m2v(tranMat * p2m(v2));
        v0= Vec3f((v0.x + 1.0) * 0.5 * float(width), float(height) - (v0.y + 1.0) * 0.5 * float(height), v0.z);
        v1 = Vec3f((v1.x + 1.0) * 0.5 * float(width), float(height) - (v1.y + 1.0) * 0.5 * float(height), v1.z);
        v2 = Vec3f((v2.x + 1.0) * 0.5 * float(width), float(height) - (v2.y + 1.0) * 0.5 * float(height), v2.z);
        Vec3f normScreen = m2v(tranMat * v2m(norm)) * (-1);
        if (normScreen.z >= 0) {

            int xmax = std::max(v0.x, std::max(v1.x, v2.x));
            int xmin = std::min(v0.x, std::min(v1.x, v2.x));
            int ymax = std::max(v0.y, std::max(v1.y, v2.y));
            int ymin = std::min(v0.y, std::min(v1.y, v2.y));

            for (int x = xmin; x <= xmax; x++) {
                for (int y = ymin; y <= ymax; y++) {
                    float alpha, beta, gamma;
                    //求重心坐标
                    alpha = (-(x - v1.x) * (v2.y - v1.y) + (y - v1.y) * (v2.x - v1.x)) / (-(v0.x - v1.x) * (v2.y - v1.y) + (v0.y - v1.y) * (v2.x - v1.x));
                    beta = (-(x - v2.x) * (v0.y - v2.y) + (y - v2.y) * (v0.x - v2.x)) / (-(v1.x - v2.x) * (v0.y - v2.y) + (v1.y - v2.y) * (v0.x - v2.x));
                    gamma = 1.f - alpha - beta;

                    if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                        float depth = alpha * v0.z + beta * v1.z + gamma * v2.z;
                        if (depth > depthBuffer[x*width+y]) {
                            Vec2f uv = uv0 * alpha + uv1 * beta + uv2 * gamma;
                            TGAColor texcol = baseTex->get(int(baseTex->get_width() * uv.x), int(baseTex->get_height() * (1.f - uv.y)));
                            COLORREF texturecolor = RGB(texcol.r , texcol.g , texcol.b );
                            COLORREF normcol = RGB(norm.x*255, norm.y*255, norm.z*255);
                            COLORREF lightcol = RGB(texcol.r * ndotl, texcol.g * ndotl, texcol.b * ndotl);
                            COLORREF phong = RGB(255 * ndotl, 255 * ndotl,255* ndotl);
                            SetPixel(memDC, x, y, phong);
                            depthBuffer[x * width + y] = depth;
                        }
                    }
                }
            }
        }
    }
}
void Raster::lineMode(HDC  memDC) {
    Matrix tranMat = get_tranMat();
    for (int i = 0; i < model->nfaces(); i++) {
        Vec3f v0 = model->vert(model->face(i)[0]); Vec3f v1 = model->vert(model->face(i)[1]); Vec3f v2 = model->vert(model->face(i)[2]);
        Vec3f norm = (v2 - v0) ^ (v1 - v0);
        //模型mvpv变换
        v0 = m2v(tranMat * p2m(v0)); v1 = m2v(tranMat * p2m(v1)); v2 = m2v(tranMat * p2m(v2));
        v0 = Vec3f((v0.x + 1.0) * 0.5 * float(width), float(height) - (v0.y + 1.0) * 0.5 * float(height), v0.z);
        v1 = Vec3f((v1.x + 1.0) * 0.5 * float(width), float(height) - (v1.y + 1.0) * 0.5 * float(height), v1.z);
        v2 = Vec3f((v2.x + 1.0) * 0.5 * float(width), float(height) - (v2.y + 1.0) * 0.5 * float(height), v2.z);
        Vec3f normScreen = m2v(tranMat * v2m(norm)) * (-1);
        if (normScreen.z >= 0) {
            COLORREF testcolor = RGB(255, 255, 255);
            line(v0, v1, memDC, testcolor);
            line(v1, v2, memDC, testcolor);
            line(v2, v0, memDC, testcolor);
        }
    }
}
void Raster::shadeMode(HDC  memDC, TGAImage* baseTex, TGAImage* normTex, Vec3f ldir, float* depthBuffer) {
    //初始化深度缓存区
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            depthBuffer[i * width + j] = std::numeric_limits<float>::max() * (-1);
        }
    }
    Matrix tranMat = get_tranMat();
    //ofstream log("./renderlog.txt");
    //log << tranMat << endl;
    //log.close();

    for (int i = 0; i < model->nfaces(); i++) {
        Vec3f v0 = model->vert(model->face(i)[0]); Vec3f v1 = model->vert(model->face(i)[1]); Vec3f v2 = model->vert(model->face(i)[2]);
        Vec2f uv0 = model->uv(model->uvIndex(i)[0]); Vec2f uv1 = model->uv(model->uvIndex(i)[1]); Vec2f uv2 = model->uv(model->uvIndex(i)[2]);
        Vec3f norm = ( (v1 - v0) ^ (v2 - v0)).normalize();
        //模型mvpv变换
        v0 = m2v(tranMat * p2m(v0)); v1 = m2v(tranMat * p2m(v1)); v2 = m2v(tranMat * p2m(v2));
        v0 = Vec3f((v0.x + 1.0) * 0.5 * float(width), float(height) - (v0.y + 1.0) * 0.5 * float(height), v0.z);
        v1 = Vec3f((v1.x + 1.0) * 0.5 * float(width), float(height) - (v1.y + 1.0) * 0.5 * float(height), v1.z);
        v2 = Vec3f((v2.x + 1.0) * 0.5 * float(width), float(height) - (v2.y + 1.0) * 0.5 * float(height), v2.z);
        Vec3f normScreen = m2v(tranMat * v2m(norm)) * (-1);
        if (normScreen.z >= 0) {

            int xmax = std::max(v0.x, std::max(v1.x, v2.x));
            int xmin = std::min(v0.x, std::min(v1.x, v2.x));
            int ymax = std::max(v0.y, std::max(v1.y, v2.y));
            int ymin = std::min(v0.y, std::min(v1.y, v2.y));

            for (int x = xmin; x <= xmax; x++) {
                for (int y = ymin; y <= ymax; y++) {
                    float alpha, beta, gamma;
                    //求重心坐标
                    alpha = (-(x - v1.x) * (v2.y - v1.y) + (y - v1.y) * (v2.x - v1.x)) / (-(v0.x - v1.x) * (v2.y - v1.y) + (v0.y - v1.y) * (v2.x - v1.x));
                    beta = (-(x - v2.x) * (v0.y - v2.y) + (y - v2.y) * (v0.x - v2.x)) / (-(v1.x - v2.x) * (v0.y - v2.y) + (v1.y - v2.y) * (v0.x - v2.x));
                    gamma = 1.f - alpha - beta;

                    if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                        float depth = alpha * v0.z + beta * v1.z + gamma * v2.z;
                        if (depth > depthBuffer[x * width + y]) {
                            Vec2f uv = uv0 * alpha + uv1 * beta + uv2 * gamma;
                            TGAColor texcol = baseTex->get(int(baseTex->get_width() * uv.x), int(baseTex->get_height() * (1.f - uv.y)));
                            TGAColor normtexcol = normTex->get(int(normTex->get_width() * uv.x), int(normTex->get_height() * (1.f - uv.y)));
                            norm = Vec3f(normtexcol.r, normtexcol.g, normtexcol.b).normalize();
                            float ndotl = ldir * norm;//光照计算
                            ndotl = clamp(0., 1., ndotl);
                            //COLORREF texturecolor = RGB(texcol.r, texcol.g, texcol.b);
                            //COLORREF normcol = RGB(normtexcol.r, normtexcol.g, normtexcol.b);
                            COLORREF lightcol = RGB(texcol.r * ndotl, texcol.g * ndotl, texcol.b * ndotl);
                            //COLORREF phong = RGB(255 * ndotl, 255 * ndotl, 255 * ndotl);
                            SetPixel(memDC, x, y, lightcol);
                            depthBuffer[x * width + y] = depth;
                        }
                    }
                }
            }
        }
    }

}
void Raster::shadeMode(HDC  memDC, TGAImage* baseTex, TGAImage* normTex, TGAImage* tan_normTex, Vec3f ldir, float* depthBuffer) {
    //初始化深度缓存区
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            depthBuffer[i * width + j] = std::numeric_limits<float>::max() * (-1);
        }
    }
    Matrix tranMat = get_tranMat();
    //ofstream log("./renderlog.txt");
    //log << tranMat << endl;
    //log.close();

    for (int i = 0; i < model->nfaces(); i++) {
        Vec3f v0 = model->vert(model->face(i)[0]); Vec3f v1 = model->vert(model->face(i)[1]); Vec3f v2 = model->vert(model->face(i)[2]);
        Vec2f uv0 = model->uv(model->uvIndex(i)[0]); Vec2f uv1 = model->uv(model->uvIndex(i)[1]); Vec2f uv2 = model->uv(model->uvIndex(i)[2]);
        Vec3f norm = ((v1 - v0) ^ (v2 - v0)).normalize();
        //模型mvpv变换
        v0 = m2v(tranMat * p2m(v0)); v1 = m2v(tranMat * p2m(v1)); v2 = m2v(tranMat * p2m(v2));
        v0 = Vec3f((v0.x + 1.0) * 0.5 * float(width), float(height) - (v0.y + 1.0) * 0.5 * float(height), v0.z);
        v1 = Vec3f((v1.x + 1.0) * 0.5 * float(width), float(height) - (v1.y + 1.0) * 0.5 * float(height), v1.z);
        v2 = Vec3f((v2.x + 1.0) * 0.5 * float(width), float(height) - (v2.y + 1.0) * 0.5 * float(height), v2.z);
        Vec3f normScreen = m2v(tranMat * v2m(norm)) * (-1);
        if (normScreen.z >= 0) {

            int xmax = std::max(v0.x, std::max(v1.x, v2.x));
            int xmin = std::min(v0.x, std::min(v1.x, v2.x));
            int ymax = std::max(v0.y, std::max(v1.y, v2.y));
            int ymin = std::min(v0.y, std::min(v1.y, v2.y));

            for (int x = xmin; x <= xmax; x++) {
                for (int y = ymin; y <= ymax; y++) {
                    float alpha, beta, gamma;
                    //求重心坐标
                    alpha = (-(x - v1.x) * (v2.y - v1.y) + (y - v1.y) * (v2.x - v1.x)) / (-(v0.x - v1.x) * (v2.y - v1.y) + (v0.y - v1.y) * (v2.x - v1.x));
                    beta = (-(x - v2.x) * (v0.y - v2.y) + (y - v2.y) * (v0.x - v2.x)) / (-(v1.x - v2.x) * (v0.y - v2.y) + (v1.y - v2.y) * (v0.x - v2.x));
                    gamma = 1.f - alpha - beta;

                    if (alpha >= 0 && beta >= 0 && gamma >= 0) {
                        float depth = alpha * v0.z + beta * v1.z + gamma * v2.z;
                        if (depth > depthBuffer[x * width + y]) {
                            Vec2f uv = uv0 * alpha + uv1 * beta + uv2 * gamma;
                            TGAColor texcol = baseTex->get(int(baseTex->get_width() * uv.x), int(baseTex->get_height() * (1.f - uv.y)));
                            TGAColor normtexcol = normTex->get(int(normTex->get_width() * uv.x), int(normTex->get_height() * (1.f - uv.y)));
                            norm = Vec3f(normtexcol.r, normtexcol.g, normtexcol.b).normalize();
                            TGAColor tan_normcol = tan_normTex->get(int(tan_normTex->get_width() * uv.x), int(tan_normTex->get_height() * (1.f - uv.y)));
                            Vec3f tan_norm = Vec3f(tan_normcol.r, tan_normcol.g, tan_normcol.b).normalize();
                            
                            Matrix TBNMat = get_TBNMat(norm, v0, v1, v2, uv0, uv1, uv2);
                            norm = m2v(TBNMat * v2m(tan_norm)) * (-1.0);
                            norm.normalize();

                            float ndotl = ldir * norm;//光照计算
                            ndotl = clamp(0., 1., ndotl);
                            COLORREF texturecolor = RGB(texcol.r, texcol.g, texcol.b);
                            //COLORREF normcol = RGB(normtexcol.r, normtexcol.g, normtexcol.b);
                            COLORREF lightcol = RGB(texcol.r * ndotl, texcol.g * ndotl, texcol.b * ndotl);
                            //COLORREF phong = RGB(255 * ndotl, 255 * ndotl, 255 * ndotl);
                            SetPixel(memDC, x, y, RGB(255,255,255));
                            depthBuffer[x * width + y] = depth;
                        }
                    }
                }
            }
        }
    }
}



