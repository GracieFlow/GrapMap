#ifndef JIUJITSUMAPPER_MATH_HPP
#define JIUJITSUMAPPER_MATH_HPP

#include <cmath>
#include <iostream>
#include <array>
#include <GL/glu.h>

struct V2 { GLdouble x, y; };
struct V3 { GLdouble x, y, z; };

struct V4
{
	GLdouble x, y, z, w;

	V4(V3 v, GLdouble w)
		: x(v.x), y(v.y), z(v.z), w(w)
	{}
};

V3 const
	red{1,0,0}, blue{0.1, 0.1, 0.9}, grey{0.2, 0.2, 0.2},
	yellow{1,1,0}, green{0,1,0}, white{1,1,1}, black{0,0,0};

inline double inner_prod(V2 a, V2 b) { return a.x * b.x + a.y * b.y; }
inline double inner_prod(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

using M = std::array<double, 16>;

inline M yrot(double a)
{
	return
		{ cos(a), 0, sin(a), 0
		, 0, 1, 0, 0
		, -sin(a), 0, cos(a), 0
		, 0, 0, 0, 1 };
}

inline M xrot(double a)
{
	return
		{ 1, 0, 0, 0
		, 0, cos(a), -sin(a), 0
		, 0, sin(a), cos(a), 0
		, 0, 0, 0, 1 };
}

inline M translate(double x, double y, double z)
{
	return {
		{ 1, 0, 0, 0
		, 0, 1, 0, 0
		, 0, 0, 1, 0
		, x, y, z, 1 }};
}

inline V3 cross(V3 a, V3 b)
{
	return
		{ a.y * b.z - a.z * b.y
		, a.z * b.x - a.x * b.z
		, a.x * b.y - a.y * b.x };
}

inline V2 xy(V3 v){ return {v.x, v.y}; }
inline V2 xy(V4 v){ return {v.x, v.y}; }
inline V3 xyz(V4 v){ return {v.x, v.y, v.z}; }

inline M perspective(double fovy, double aspect, double zNear, double zFar)
{
	auto f = 1/tan(fovy*M_PI/360);

	return
		{ f/aspect, 0, 0, 0
		, 0, f, 0, 0
		, 0, 0, (zFar+zNear)/(zNear-zFar), -1
		, 0, 0, 2*zFar*zNear/(zNear-zFar), 0 };
}

inline double norm2(V2 v){ return sqrt(inner_prod(v, v)); }
inline double norm2(V3 v){ return sqrt(inner_prod(v, v)); }

inline V2 operator-(V2 a, V2 b) { return {a.x - b.x, a.y - b.y}; }
inline V3 operator-(V3 a, V3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline V3 operator-(V3 v) { return {-v.x, -v.y, -v.z}; }
inline V4 operator-(V4 a, V4 b) { return {{a.x - b.x, a.y - b.y, a.z - b.z}, a.w - b.w}; }
inline V2 operator+(V2 a, V2 b) { return {a.x + b.x, a.y + b.y}; }
inline V3 operator+(V3 a, V3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline V4 operator+(V4 a, V4 b) { return {{a.x + b.x, a.y + b.y, a.z + b.z}, a.w + b.w}; }
inline V2 operator*(V2 v, double s) { return {v.x * s, v.y * s}; }
inline V3 operator*(V3 v, double s) { return {v.x * s, v.y * s, v.z * s}; }
inline V3 & operator+=(V3 & a, V3 b) { return a = a + b; }
inline V3 & operator-=(V3 & a, V3 b) { return a = a - b; }
inline V4 & operator-=(V4 & a, V4 b) { return a = a - b; }

inline bool operator==(V3 a, V3 b) { return a.x == b.x && a.y == b.y && a.z == b.z; }

inline std::ostream & operator<<(std::ostream & o, V2 v){ return o << '{' << v.x << ',' << v.y << '}'; }
inline std::ostream & operator<<(std::ostream & o, V3 v){ return o << '{' << v.x << ',' << v.y << ',' << v.z << '}'; }

inline V2 operator/(V2 v, double s) { return {v.x / s, v.y / s}; }
inline V3 operator/(V3 v, double s) { return {v.x / s, v.y / s, v.z / s}; }
inline V3 normalize(V3 v){ return v / norm2(v); }

inline double distance(V3 from, V3 to) { return norm2(to - from); }
inline double distanceSquared(V2 from, V2 to) { return inner_prod(to - from, to - from); }
inline double distanceSquared(V3 from, V3 to) { return inner_prod(to - from, to - from); }

inline V4 operator*(M const & m, V4 const v)
{
	return
		{{ m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12]
		, m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13]
		, m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]}
		, m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15]};
}

inline M operator*(M const & a, M const & b)
{
	return
		{{ a[ 0]*b[ 0] + a[ 4]*b[ 1] + a[ 8]*b[ 2] + a[12]*b[ 3]
		,  a[ 1]*b[ 0] + a[ 5]*b[ 1] + a[ 9]*b[ 2] + a[13]*b[ 3]
		,  a[ 2]*b[ 0] + a[ 6]*b[ 1] + a[10]*b[ 2] + a[14]*b[ 3]
		,  a[ 3]*b[ 0] + a[ 7]*b[ 1] + a[11]*b[ 2] + a[15]*b[ 3]

		,  a[ 0]*b[ 4] + a[ 4]*b[ 5] + a[ 8]*b[ 6] + a[12]*b[ 7]
		,  a[ 1]*b[ 4] + a[ 5]*b[ 5] + a[ 9]*b[ 6] + a[13]*b[ 7]
		,  a[ 2]*b[ 4] + a[ 6]*b[ 5] + a[10]*b[ 6] + a[14]*b[ 7]
		,  a[ 3]*b[ 4] + a[ 7]*b[ 5] + a[11]*b[ 6] + a[15]*b[ 7]

		,  a[ 0]*b[ 8] + a[ 4]*b[ 9] + a[ 8]*b[10] + a[12]*b[11]
		,  a[ 1]*b[ 8] + a[ 5]*b[ 9] + a[ 9]*b[10] + a[13]*b[11]
		,  a[ 2]*b[ 8] + a[ 6]*b[ 9] + a[10]*b[10] + a[14]*b[11]
		,  a[ 3]*b[ 8] + a[ 7]*b[ 9] + a[11]*b[10] + a[15]*b[11]

		,  a[ 0]*b[12] + a[ 4]*b[13] + a[ 8]*b[14] + a[12]*b[15]
		,  a[ 1]*b[12] + a[ 5]*b[13] + a[ 9]*b[14] + a[13]*b[15]
		,  a[ 2]*b[12] + a[ 6]*b[13] + a[10]*b[14] + a[14]*b[15]
		,  a[ 3]*b[12] + a[ 7]*b[13] + a[11]*b[14] + a[15]*b[15] }};
}

#endif