#include "tgaimage.h"
#include "model.h"
#include <limits>
#include "softgl.h"
#include "mathmatic.h"
#include "phongshader.h"
#include "depthshader.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const int width  = 800;
const int height = 800;
const int depth = 255;

Vec3f light_dir(1,1,1);
Vec3f       eye(0,0,3);
Vec3f    center(0,0,0);
Vec3f        up(0,1,0);

/*using  namespace math;

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	//err * dx * 2 to avoid float (0.5 and dy/dx) comparison
	int derr = std::abs(dy) * 2;
	int err = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}		
		err += derr;
		if (err > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			err -= dx * 2;
		}
	}	
}

void triangle(Vec2i *pts, TGAImage& image, TGAColor color)
{
	Vec2i bboxmin(image.get_width() - 1, image.get_height() - 1);
	Vec2i bboxmax(0, 0);
	Vec2i boundary(image.get_width() - 1, image.get_height() - 1);

	for (int i = 0; i < 3; i++)
	{
		bboxmin.x = std::max(0, std::min(bboxmin.x, pts[i].x));
		bboxmin.y = std::max(0, std::min(bboxmin.y, pts[i].y));
		bboxmax.x = std::min(boundary.x, std::max(bboxmax.x, pts[i].x));
		bboxmax.y = std::min(boundary.y, std::max(bboxmax.y, pts[i].y));
	}
	Vec2i p;
	for (p.x = bboxmin.x; p.x < bboxmax.x; p.x++)
	{
		for (p.y = bboxmin.y; p.y < bboxmax.y; p.y++)
		{
			Vec3f cross = barycentric(pts, p);
			if (cross.x < 0 || cross.y < 0 || cross.z < 0)
				continue;
			image.set(p.x, p.y, color);			
		}
	}
}
*/
int main(int argc, char** argv)
{
	
    Model *model = new Model("C:/Users/zli13/Documents/zeqiang.li/sr/obj/diablo3_pose/diablo3_pose.obj");
    //Model *model = new Model("C:/Users/zli13/Documents/zeqiang.li/sr/obj/4.obj");

	Matrix lookAtMat_light = math::lookAt(light_dir, center, up);
	Matrix viewport_light = math::viewport(width/8, height/8, width*3/4, height*3/4);
	Matrix projMat_light = math::projection( 0.0f );
	light_dir.normalize();

	TGAImage image(width, height, TGAImage::RGB);
	TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

	pipeline_impl::DepthShader depthShader;
	depthShader.setModel(model);
	depthShader.setModelMat(Matrix::identity());
	depthShader.setViewMat(lookAtMat_light);
	depthShader.setProjectMat(projMat_light);
	depthShader.setViewportMat(viewport_light);
	for (int i = 0; i < model->nfaces(); i++)
	{
		Vec4f screenCoords[3];
		for (int j = 0; j < 3; j++)
		{
			screenCoords[j] = depthShader.vertex(i, j);
		}
		pipeline_impl::triangle(screenCoords, depthShader, image, zbuffer, model);
	}
	//
	Matrix lookAtMat = math::lookAt(eye, center, up);
	Matrix viewport = math::viewport(width/8, height/8, width*3/4, height*3/4);
	Matrix projMat = math::projection( -1.0f / (eye-center).norm() );


	TGAImage image_front(width, height, TGAImage::RGB);
	TGAImage zbuffer_front(width, height, TGAImage::GRAYSCALE);

	pipeline_impl::PhongShader phongShader;
	phongShader.setModel(model);
	phongShader.setModelMat(Matrix::identity());
	phongShader.setViewMat(lookAtMat);
	phongShader.setProjectMat(projMat);
	phongShader.setViewportMat(viewport);
	phongShader.enableShadow(true);
	phongShader.setDepthBuffer(zbuffer.get_width(), zbuffer.get_height(), zbuffer.buffer());
	phongShader.uniform_shadowSpaceMat = viewport_light * projMat_light * lookAtMat_light;
	for (int i = 0; i < model->nfaces(); i++)
	{
		Vec4f screenCoords[3];
		for (int j = 0; j < 3; j++)
		{
			screenCoords[j] = phongShader.vertex(i, j);
		}
		pipeline_impl::triangle(screenCoords, phongShader, image_front, zbuffer_front, model);
	}
	
	image_front.write_tga_file("output.tga");
	zbuffer.write_tga_file("zbuffer.tga");
	delete model;
	return 0;
}