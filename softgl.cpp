#include  "softgl.h"
#include "model.h"
#include "mathmatic.h"

namespace pipeline_impl
{

void triangle(Vec4f* pts, IShader& shader, TGAImage& image, float* zbuffer, Model* md)
{
	std::for_each(pts, pts + 3, [&](Vec4f& pts) { pts = pts / pts[3]; });

	Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	Vec2f boundary(image.get_width() - 1, image.get_height() - 1);

	auto f_x = [=](const Vec4f& a, const Vec4f& b) {return a[0] < b[0]; };
	auto f_y = [=](const Vec4f& a, const Vec4f& b) {return a[1] < b[1]; };
	Vec4f minXVert = std::min({pts[0], pts[1], pts[2]}, f_x);
	Vec4f maxXVert = std::max({pts[0], pts[1], pts[2]}, f_x);
	Vec4f minYVert = std::min({pts[0], pts[1], pts[2]}, f_y);
	Vec4f maxYVert = std::max({pts[0], pts[1], pts[2]}, f_y);

	minXVert[0] = math::clamp<float>(minXVert[0], 0, image.get_width());
	minXVert[1] = math::clamp<float>(minXVert[1], 0, image.get_height());
	maxXVert[0] = math::clamp<float>(maxXVert[0], 0, image.get_width());
	maxXVert[1] = math::clamp<float>(maxXVert[1], 0, image.get_height());

	minYVert[0] = math::clamp<float>(minYVert[0], 0, image.get_width());
	minYVert[1] = math::clamp<float>(minYVert[1], 0, image.get_height());
	maxYVert[0] = math::clamp<float>(maxYVert[0], 0, image.get_width());
	maxYVert[1] = math::clamp<float>(maxYVert[1], 0, image.get_height());

	Vec2i p;
	for (p.x = minXVert[0]; p.x <= maxXVert[0]; p.x++)
	{
		for (p.y = minYVert[1]; p.y <= maxYVert[1]; p.y++)
		{
			Vec3f cross = math::barycentric(proj<2>(pts[0]/pts[0][3]), proj<2>(pts[1]/pts[1][3]), proj<2>(pts[2]/pts[2][3]), proj<2>(p));
            float z = pts[0][2]*cross.x + pts[1][2]*cross.y + pts[2][2]*cross.z;
            float w = pts[0][3]*cross.x + pts[1][3]*cross.y + pts[2][3]*cross.z;
            float depth = std::max(0, std::min(255, int(z/w + 0.5)));
			if (cross.x < 0 || cross.y < 0 || cross.z < 0 || zbuffer[p.x + p.y * width] > depth)
				continue;
			/*
            p.z = 0;
			p.z += pts[0].coord.z * cross.x;
			p.z += pts[1].coord.z * cross.y;
			p.z += pts[2].coord.z * cross.z;


            //uv enable
            
			int u = 0, v = 0;
			u += pts[0].uv.x * cross.x;
			u += pts[1].uv.x * cross.y;
			u += pts[2].uv.x * cross.z;

			v += pts[0].uv.y * cross.x;
			v += pts[1].uv.y * cross.y;
			v += pts[2].uv.y * cross.z;
            */

            TGAColor color;
            bool discard = shader.fragment(cross, color);
		
			//u = std::abs(maxXVert[0] - minXVert[0]) == 0 ? 0.0 :
			//	(p.x - minXVert[0])/(maxXVert[0] - minXVert[0]) * (maxXVert.uv.x - minXVert.uv.x) + minXVert.uv.x;
			//v = std::abs(maxYVert[1] - minYVert[1]) == 0 ? 0.0 :
			//	(p.y - minYVert[1])/(maxYVert[1] - minYVert[1]) * (maxYVert.uv.y - minYVert.uv.y) + minYVert.uv.y;


			if (!discard)
			{
				TGAColor color;
				shader.fragment(cross, color);
				zbuffer[p.x + p.y*width] = depth;
				image.set(p.x, p.y, color);
			}			
		}
	}

}

}