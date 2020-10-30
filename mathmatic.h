#ifndef __MATHEMATIC_H__
#define __MATHEMATIC_H__
#include "geometry.h"

namespace math{

static const float global_depth = 255.0f;

template <typename T>
T clamp(const T& val, const T& min, const T& max)
{
	return val < min ? min : (val > max ? max : val);
}

inline Vec3f cross(Vec3f a, Vec3f b)
{
	return Vec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Matrix viewport(int x, int y, int w, int h)
{
    Matrix mat = Matrix::identity();
    mat[0][3] = x + w/2.0f;
    mat[1][3] = y + h/2.0f;
    mat[2][3] = global_depth/2.0f;
    mat[0][0] = w/2.0f;
    mat[1][1] = h/2.0f;
    mat[2][2] = global_depth/2.0f;
    return mat;
}

inline Matrix projection(float coeff)
{
    Matrix mat = Matrix::identity();
    mat[3][2] = coeff;
    return mat;
}

inline Matrix lookAt(Vec3f eye, Vec3f center, Vec3f up)
{
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();

    Matrix mat = Matrix::identity();
    for (size_t i = 0; i < 3; i++)
    {
        mat[0][i] = x[i];
        mat[1][i] = y[i];
        mat[2][i] = z[i];
        mat[i][3] = -center[i];
    }
    return mat;    
}

inline Vec3f barycentric(Vec2i *pts, Vec2i p)
{
	Vec3f u = cross(Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x), Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y));
	//interior angle will never greater than 180 so u[2] should always greater than 0
	if (std::abs(u.z) < 1)
	{
		return Vec3f(-1, 1, 1);
	}
	return Vec3f(1.0f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z);
}

inline Vec3f barycentric(Vertex *pts, Vec3f p)
{
	Vec3f u = cross(Vec3f(pts[2].coord.x - pts[0].coord.x, pts[1].coord.x - pts[0].coord.x, pts[0].coord.x - p.x), 
					Vec3f(pts[2].coord.y - pts[0].coord.y, pts[1].coord.y - pts[0].coord.y, pts[0].coord.y - p.y));
	//interior angle will never greater than 180 so u[2] should always greater than 0
	if (std::abs(u.z) < 0.000001)
	{
		return Vec3f(-1, 1, 1);
	}
	return Vec3f(1.0f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z);
}

inline Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

}
#endif