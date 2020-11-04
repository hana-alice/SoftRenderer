#include "phongshader.h"
#include "model.h"
#include "mathmatic.h"

namespace pipeline_impl
{

PhongShader::PhongShader() : light_dir(1.0f, 1.0f, 3.0f), m_shadowEnabled(false), m_depthBuffer(nullptr)
{
}

PhongShader::~PhongShader()
{
}

void PhongShader::setModelMat(const Matrix& mat)
{
    m_modelMat = mat;
}

void PhongShader::setViewMat(const Matrix& mat)
{
    m_viewMat = mat;
}

void PhongShader::setProjectMat(const Matrix& mat)
{
    m_projMat = mat;
}

void PhongShader::setViewportMat(const Matrix& mat)
{
    m_viewportMat = mat;
}

void PhongShader::setModel(Model* model)
{
    m_model = model;
}

Matrix PhongShader::getViewportMat()
{
	return m_viewportMat;
}

Vec4f PhongShader::vertex(int face, int nthvert)
{
    //varying_intensity[nthvert] = std::max(0.0f, m_model->normal(face, nthvert) * light_dir);
    varying_uv.set_col(nthvert, m_model->uv(face, nthvert));
	m_mvpMat = m_projMat * m_viewMat * m_modelMat;
	varying_norm.set_col(nthvert, proj<3>(m_mvpMat.invert_transpose() * embed<4>(m_model->normal(face, nthvert),0.0f)));
	Vec4f gl_vertex = m_viewportMat * m_mvpMat * embed<4>(m_model->vert(face, nthvert));
	varying_tri.set_col(nthvert, proj<3>(gl_vertex/gl_vertex[3]));
	ndc_tri.set_col(nthvert, proj<3>(gl_vertex/gl_vertex[3]) );
    return gl_vertex;
}

void PhongShader::enableShadow(bool en)
{
    m_shadowEnabled = en;
}

void PhongShader::setDepthBuffer(int w, int h, uint8_t* depBuf)
{
    m_depthBuffer = depBuf;
    m_depthBufSize.first = w;
    m_depthBufSize.second = h;
}

bool PhongShader::fragment(Vec3f bar, TGAColor& color)
{
    #if 1
    //float intensity = varying_intensity * bar * 0.5;
    Vec2f uv = varying_uv * bar;
	#if 1 //normal
    Vec3f normal = proj<3>(m_mvpMat.invert_transpose() * embed<4>(m_model->normal(uv))).normalize();
	#else //tangent space
	Vec3f bn = (varying_norm*bar).normalize();
	mat<3, 3, float> A;
	A[0] = ndc_tri.col(1) - ndc_tri.col(0);
	A[1] = ndc_tri.col(2) - ndc_tri.col(0);
	A[2] = bn;
	mat<3, 3, float> AI = A.invert();

	Vec3f i = AI * Vec3f(varying_uv[0][1] - varying_uv[0][0], varying_uv[0][2] - varying_uv[0][0], 0);
    Vec3f j = AI * Vec3f(varying_uv[1][1] - varying_uv[1][0], varying_uv[1][2] - varying_uv[1][0], 0);

	mat<3, 3, float> B;
	B.set_col(0, i.normalize());
	B.set_col(1, j.normalize());
	B.set_col(2, bn);

	Vec3f normal = (B*m_model->normal(uv)).normalize();
	#endif

    Vec4f pt_shadow = uniform_shadowSpaceMat * (m_viewportMat * m_mvpMat).invert() * embed<4>(varying_tri * bar);
    pt_shadow = pt_shadow/pt_shadow[3];
    Vec3f light = proj<3>(m_mvpMat * embed<4>(light_dir, 0.0f)).normalize();
	Vec3f reflect = (normal * 2.0f * (normal * light) - light).normalize();
	float spec = pow(std::max(reflect.z, 0.0f), m_model->specular(uv) );
    float intensity = std::max(0.0f, normal * light);
    color = m_model->diffuse(uv);// * intensity;
	for (size_t i = 0; i < 3; i++)
	{
        if (m_shadowEnabled && m_depthBuffer)
        {
            //multi by 1.05 to avoid shadow zfighting
            bool inShadow = pt_shadow[2] * 1.05 < (m_depthBuffer[(int)pt_shadow[0] + (int)pt_shadow[1] * m_depthBufSize.first]);
            if (inShadow)
                color[i] = std::min(5.0f + color[i] * intensity * 0.4f, 255.0f);
            else
                color[i] = std::min(5.0f + color[i] * (intensity + 0.6f * spec), 255.0f);
        }
	}
    return false;
    #else
    float intensity = varying_intensity * bar * 0.5;
    if (intensity > 0.85f)
        intensity = 1.0f;
    else if(intensity > 0.60f)
        intensity = 0.8f;
    else if(intensity > 0.45f)
        intensity = 0.6f;
    else if(intensity > 0.30f)
        intensity = 0.45f;
    else if(intensity > 0.15f)
        intensity = 0.3f;
    else
        intensity = 0.0f;
    
    color = TGAColor(255, 155, 0) * intensity;
    return false;

    #endif

}
} // namespace pipeline_impl
