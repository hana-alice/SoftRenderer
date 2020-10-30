#include "DepthShader.h"
#include "model.h"
#include "mathmatic.h"

namespace pipeline_impl
{

DepthShader::DepthShader()
{
}

DepthShader::~DepthShader()
{
}

void DepthShader::setModelMat(const Matrix& mat)
{
    m_modelMat = mat;
}

void DepthShader::setViewMat(const Matrix& mat)
{
    m_viewMat = mat;
}

void DepthShader::setProjectMat(const Matrix& mat)
{
    m_projMat = mat;
}

void DepthShader::setModel(Model* model)
{
    m_model = model;
}

void DepthShader::setViewportMat(const Matrix& mat)
{
    m_viewportMat = mat;
}

Matrix DepthShader::getViewportMat()
{
	return m_viewportMat;
}

Vec4f DepthShader::vertex(int face, int nthvert)
{
    //varying_intensity[nthvert] = std::max(0.0f, m_model->normal(face, nthvert) * light_dir);
    m_mvpMat = m_projMat * m_viewMat * m_modelMat;
	Vec4f gl_vertex = m_viewportMat * m_mvpMat * embed<4>(m_model->vert(face, nthvert));
	varying_tri.set_col(nthvert, proj<3>(gl_vertex/gl_vertex[3]));
    return gl_vertex;
}

bool DepthShader::fragment(Vec3f bar, TGAColor& color)
{
    Vec3f pt = varying_tri * bar;
    color = TGAColor(255,255,255) * (pt.z/255.0f);
    return false;

}
} // namespace pipeline_impl
