#ifndef __IGL_H__
#define __IGL_H__
#include "geometry.h"
#include "tgaimage.h"

class Model;

namespace pipeline_impl{

class IShader
{
public:
    virtual ~IShader(){};

    virtual void setModel(Model* model) = 0;

    virtual void setModelMat(const Matrix& modMat) = 0;

    virtual void setViewMat(const Matrix& modMat) = 0;

    virtual void setProjectMat(const Matrix& modMat) = 0;

    virtual void setViewportMat(const Matrix& modMat) = 0;

    virtual Vec4f vertex(int face, int vert) = 0;

    virtual bool fragment(Vec3f bar, TGAColor& color) = 0;

    virtual Matrix getViewportMat() = 0;
};

}
#endif