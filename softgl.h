#ifndef __SOFTGL_H__
#define __SOFTGL_H__
#include "igl.h"

static const int width  = 800;
static const int height = 800;
namespace pipeline_impl
{

void triangle(Vec4f* pts, IShader& shader, TGAImage& image, float* zbuffer, Model* md);

}
#endif