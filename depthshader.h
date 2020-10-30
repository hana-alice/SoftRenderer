#pragma once
#include "igl.h"

namespace pipeline_impl
{

class DepthShader :public IShader
{
public:
    DepthShader();

    ~DepthShader();

    virtual void setModel(Model* model) override;

    virtual void setModelMat(const Matrix& modMat) override;

    virtual void setViewMat(const Matrix& modMat) override;

    virtual void setProjectMat(const Matrix& modMat) override;

    virtual void setViewportMat(const Matrix& modMat) override;

    virtual Vec4f vertex(int face, int vert) override;

    virtual bool fragment(Vec3f bar, TGAColor& color) override;

    virtual Matrix getViewportMat() override;

private:
    Model* m_model;
    Matrix m_modelMat;
    Matrix m_viewMat;
    Matrix m_projMat;
    Matrix m_mvpMat;
    Matrix m_viewportMat;
    mat<3, 3, float> varying_tri;
};

}