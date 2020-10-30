#pragma once
#include "igl.h"
#include <utility>

namespace pipeline_impl
{

class PhongShader :public IShader
{
public:
    PhongShader();

    ~PhongShader();

    virtual void setModel(Model* model) override;

    virtual void setModelMat(const Matrix& modMat) override;

    virtual void setViewMat(const Matrix& modMat) override;

    virtual void setProjectMat(const Matrix& modMat) override;

    virtual void setViewportMat(const Matrix& modMat) override;

    virtual Vec4f vertex(int face, int vert) override;

    virtual bool fragment(Vec3f bar, TGAColor& color) override;

    virtual Matrix getViewportMat() override;

    void enableShadow(bool en);

    void setDepthBuffer(int w, int h, uint8_t* dep);

    mat<4, 4, float>    uniform_shadowSpaceMat;
private:
    bool    m_shadowEnabled;
    Model*  m_model;
    Vec3f   varying_intensity;
    Vec3f   light_dir;
    Matrix  m_modelMat;
    Matrix  m_viewMat;
    Matrix  m_projMat;
    Matrix  m_viewportMat;
    Matrix  m_mvpMat;
    mat<2, 3, float>    varying_uv;
    mat<3, 3, float>    varying_tri;
    mat<3, 3, float>    varying_norm;
    mat<3, 3, float>    ndc_tri;
    
    uint8_t*            m_depthBuffer;
    std::pair<int, int> m_depthBufSize;
};

}