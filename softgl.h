#ifndef __SOFTGL_H__
#define __SOFTGL_H__
#include "igl.h"

namespace pipeline_impl
{

void triangle(Vec4f* pts, IShader& shader, TGAImage& image, TGAImage& zbuffer, Model* md);

}
#endif