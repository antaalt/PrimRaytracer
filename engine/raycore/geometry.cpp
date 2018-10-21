#include "geometry.h"
#include <cmath>
#include <iostream>

namespace geometry {
	// -- UV2
	uv2::uv2() {}
	uv2::uv2(greal value) : u(value), v(value) {}
	uv2::uv2(greal u, greal v) : u(u), v(v) {}
	greal & uv2::operator[](size_t index) { return data[index]; }
	// --- POINT3
	point3::point3() {}
	point3::point3(greal value) : x(value), y(value), z(value) {}
	point3::point3(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	point3::point3(const norm3 & normal) : x(normal.x), y(normal.y), z(normal.z) {}
	point3::point3(const vec3 & vec) : x(vec.x), y(vec.y), z(vec.z) {}
	greal & point3::operator[](size_t index) { return data[index]; }
	// --- COLORHDR
	colorHDR::colorHDR() {}
	colorHDR::colorHDR(greal value) : r(value), g(value), b(value), a(value) {}
	colorHDR::colorHDR(const color32 & color) :
		r(powf(color.r / 255.f, 2.2f)),
		g(powf(color.g / 255.f, 2.2f)),
		b(powf(color.b / 255.f, 2.2f)),
		a(color.a / 255.f) {}
	greal & colorHDR::operator[](size_t index) { return data[index]; }
	colorHDR::colorHDR(greal r, greal g, greal b, greal a) : r(r), g(g), b(b), a(a) {}
	// --- COLOR32
	color32::color32() {}
	color32::color32(gbyte value) : r(value), g(value), b(value), a(value) {}
	color32::color32(gbyte r, gbyte g, gbyte b, gbyte a) : r(r), g(g), b(b), a(a) {}
	color32::color32(const colorHDR & color) :
		r(static_cast<gbyte>(math::saturate(powf(color.r, 1 / 2.2f)) * 255.f)),
		g(static_cast<gbyte>(math::saturate(powf(color.g, 1 / 2.2f)) * 255.f)),
		b(static_cast<gbyte>(math::saturate(powf(color.b, 1 / 2.2f)) * 255.f)),
		a(static_cast<gbyte>(math::saturate(color.a) * 255.f)) {}
	gbyte & color32::operator[](size_t index) { return data[index]; }
	// --- NORM3
	norm3::norm3() {}
	//void normalize(norm3 &norm) { float length = std::sqrtf(dot(norm, norm)); norm.x /= length; norm.y /= length; norm.z /= length; }
	norm3::norm3(greal value) : x(value), y(value), z(value) {/* normalize(*this);*/ }
	norm3::norm3(greal x, greal y, greal z) : x(x), y(y), z(z) { /*normalize(*this);*/ }
	norm3::norm3(const vec3 & vec) : x(vec.x), y(vec.y), z(vec.z) { /*normalize(*this);*/ }
	norm3::norm3(const point3 & point) : x(point.x), y(point.y), z(point.z) { /*normalize(*this);*/ }
	greal & norm3::operator[](size_t index) { return data[index]; }
	// --- VEC3
	vec3::vec3() {}
	vec3::vec3(greal value) : x(value), y(value), z(value) {}
	vec3::vec3(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	vec3::vec3(const norm3 &normal) : x(normal.x), y(normal.y), z(normal.z) {}
	vec3::vec3(const point3 &point) : x(point.x), y(point.y), z(point.z) {}
	greal & vec3::operator[](size_t index) { return data[index]; }
	const greal & vec3::operator[](size_t index) const { return data[index]; }
	// --- QUAT
	quat::quat() {}
	quat::quat(greal x, greal y, greal z, greal w) : x(x), y(y), z(z), w(w) {}
	greal & quat::operator[](size_t index) { return data[index]; }
	quat quat::identity() { return quat(0.f, 0.f, 0.f, 1.f); }
	// --- MAT4
	mat4::col::col() {}
	mat4::col::col(greal x, greal y, greal z, greal w) : x(x), y(y), z(z), w(w) {}
	mat4::col::col(norm3 vec, greal w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
	mat4::col::col(point3 vec, greal w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
	greal & mat4::col::operator[](size_t index) { return data[index]; }
	const greal & mat4::col::operator[](size_t index) const { return data[index]; }
	mat4::mat4() {}
	mat4::mat4(col x, col y, col z, col w) : cols{ x, y, z, w } {}
	mat4::mat4(const quat & quat) {}
	mat4::col & mat4::operator[](size_t index) { return cols[index]; }
	const mat4::col & mat4::operator[](size_t index) const { return cols[index]; }
	mat4 mat4::identity()
	{
		return mat4(
			mat4::col(1.f, 0.f, 0.f, 0.f),
			mat4::col(0.f, 1.f, 0.f, 0.f),
			mat4::col(0.f, 0.f, 1.f, 0.f),
			mat4::col(0.f, 0.f, 0.f, 1.f)
		);
	}
	mat4 mat4::zero()
	{
		return mat4(
			mat4::col(0.f, 0.f, 0.f, 0.f),
			mat4::col(0.f, 0.f, 0.f, 0.f),
			mat4::col(0.f, 0.f, 0.f, 0.f),
			mat4::col(0.f, 0.f, 0.f, 0.f)
		);
	}
	mat4 mat4::TRS(const vec3 & translation, const quat & rotation, const vec3 & scale)
	{
		mat4 t = mat4::identity();
		t[3] = mat4::col(translation.x, translation.y, translation.z, 1.f);
		mat4 r(rotation);
		mat4 s = mat4::identity();
		for (int i = 0; i < 3; i++)
			s[i][i] = scale[i];
		return t * (r * s);
	}
	float mat4::det() const
	{
		return
			cols[0][3] * cols[1][2] * cols[2][1] * cols[3][0] - cols[0][2] * cols[1][3] * cols[2][1] * cols[3][0] -
			cols[0][3] * cols[1][1] * cols[2][2] * cols[3][0] + cols[0][1] * cols[1][3] * cols[2][2] * cols[3][0] +
			cols[0][2] * cols[1][1] * cols[2][3] * cols[3][0] - cols[0][1] * cols[1][2] * cols[2][3] * cols[3][0] -
			cols[0][3] * cols[1][2] * cols[2][0] * cols[3][1] + cols[0][2] * cols[1][3] * cols[2][0] * cols[3][1] +
			cols[0][3] * cols[1][0] * cols[2][2] * cols[3][1] - cols[0][0] * cols[1][3] * cols[2][2] * cols[3][1] -
			cols[0][2] * cols[1][0] * cols[2][3] * cols[3][1] + cols[0][0] * cols[1][2] * cols[2][3] * cols[3][1] +
			cols[0][3] * cols[1][1] * cols[2][0] * cols[3][2] - cols[0][1] * cols[1][3] * cols[2][0] * cols[3][2] -
			cols[0][3] * cols[1][0] * cols[2][1] * cols[3][2] + cols[0][0] * cols[1][3] * cols[2][1] * cols[3][2] +
			cols[0][1] * cols[1][0] * cols[2][3] * cols[3][2] - cols[0][0] * cols[1][1] * cols[2][3] * cols[3][2] -
			cols[0][2] * cols[1][1] * cols[2][0] * cols[3][3] + cols[0][1] * cols[1][2] * cols[2][0] * cols[3][3] +
			cols[0][2] * cols[1][0] * cols[2][1] * cols[3][3] - cols[0][0] * cols[1][2] * cols[2][1] * cols[3][3] -
			cols[0][1] * cols[1][0] * cols[2][2] * cols[3][3] + cols[0][0] * cols[1][1] * cols[2][2] * cols[3][3];
	}
	mat4::col::operator point3() { return point3(x, y, z); }
	mat4::col::operator norm3() { return norm3(x, y, z); }
	// --- MAT3
	mat3::col::col() {}
	mat3::col::col(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	greal & mat3::col::operator[](size_t index) { return data[index]; }
	const greal &mat3::col::operator[](size_t index) const { return data[index]; }
	mat3::col::operator point3() { return point3(x, y, z); }
	mat3::col::operator norm3() { return norm3(x, y, z); }
	mat3::mat3() {}
	mat3::mat3(const mat4 & mat) : cols{
		mat3::col(mat[0].x, mat[0].y, mat[0].z),
		mat3::col(mat[1].x, mat[1].y, mat[1].z),
		mat3::col(mat[2].x, mat[2].y, mat[2].z)
	} {}
	mat3::mat3(col x, col y, col z) : cols{ x, y, z } {}
	mat3::col & mat3::operator[](size_t index) { return cols[index]; }
	const mat3::col & mat3::operator[](size_t index) const { return cols[index]; }
	mat3 mat3::identity()
	{
		return mat3(mat3::col(1.f, 0.f, 0.f), mat3::col(0.f, 1.f, 0.f), mat3::col(0.f, 0.f, 1.f));
	}
	mat3 mat3::zero()
	{
		return mat3(mat3::col(0.f, 0.f, 0.f), mat3::col(0.f, 0.f, 0.f), mat3::col(0.f, 0.f, 0.f));
	}
	// --- VEC3 + NORM3 + POINT3
	greal dot(const vec3 & lhs, const norm3 & rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	vec3 operator-(const vec3 &lhs, const norm3 &rhs) { return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	norm3 operator-(const norm3 &lhs, const vec3 &rhs) { return norm3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	vec3 operator+(const point3 &lhs, const vec3 &rhs) { return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	greal dot(const point3 &lhs, const norm3 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	// --- UV2
	bool operator==(const uv2 &lhs, const uv2 &rhs) { return lhs.u == rhs.u && lhs.v == rhs.v; }
	bool operator!=(const uv2 &lhs, const uv2 &rhs) { return !(lhs == rhs); }
	uv2 operator*(const uv2 &lhs, greal rhs) { return uv2(lhs.u * rhs, lhs.v * rhs); }
	uv2 operator*(greal lhs, const uv2 &rhs) { return uv2(lhs * rhs.u, lhs * rhs.v); }
	uv2 operator/(const uv2 &lhs, greal rhs) { return uv2(lhs.u / rhs, lhs.v / rhs); }
	uv2 operator+(const uv2 &lhs, const uv2 &rhs) { return uv2(lhs.u + rhs.u, lhs.v + rhs.v); }
	uv2 operator-(const uv2 &lhs, const uv2 &rhs) { return uv2(lhs.u - rhs.u, lhs.v - rhs.v); }
	uv2 operator-(const uv2 &vec) { return uv2(-vec.u, -vec.v); }
	// --- VEC3
	bool operator==(const vec3 & lhs, const vec3 & rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	bool operator!=(const vec3 &lhs, const vec3 &rhs) { return !(lhs == rhs); }
	bool operator<(const vec3 &lhs, const vec3 &rhs);
	bool operator>(const vec3 &lhs, const vec3 &rhs);
	vec3 operator*(const vec3 &lhs, greal rhs) { return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
	vec3 operator*(greal lhs, const vec3 &rhs) { return vec3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }
	vec3 operator/(const vec3 &lhs, greal rhs) { return vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
	vec3 operator+(const vec3 &lhs, const vec3 &rhs) { return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	vec3 operator-(const vec3 &lhs, const vec3 &rhs) { return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	vec3 operator-(const vec3 &vec) { return vec3(-vec.x, -vec.y, -vec.z); }
	float dot(const vec3 & lhs, const vec3 & rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	vec3 cross(const vec3 & lhs, const vec3 & rhs)
	{
		return vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}
	greal length(const vec3 &vec) { return std::sqrtf(dot(vec, vec)); }
	vec3 normalize(const vec3 &vec) { return vec / length(vec); }
	// --- POINT3
	bool operator==(const point3 &lhs, const point3 &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	bool operator!=(const point3 &lhs, const point3 &rhs) { return !(lhs == rhs); }
	point3 operator*(const point3 &lhs, greal rhs) { return point3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
	point3 operator*(greal lhs, const point3 &rhs) { return point3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }
	point3 operator/(const point3 &lhs, greal rhs) { return point3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
	point3 operator+(const point3 &lhs, const point3 &rhs) { return point3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	point3 operator-(const point3 &lhs, const point3 &rhs) { return point3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	point3 operator-(const point3 &vec) { return point3(-vec.x, -vec.y, -vec.z); }
	greal distance(const point3 &lhs, const point3 &rhs) { return length(vec3(rhs - lhs)); }
	// --- NORM3
	bool operator==(const norm3 &lhs, const norm3 &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	bool operator!=(const norm3 &lhs, const norm3 &rhs) { return !(lhs == rhs); }
	norm3 operator*(const norm3 &lhs, greal rhs) { return norm3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
	norm3 operator*(greal lhs, const norm3 &rhs) { return norm3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }
	norm3 operator/(const norm3 &lhs, greal rhs) { return norm3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
	norm3 operator+(const norm3 &lhs, const norm3 &rhs) { return norm3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	norm3 operator-(const norm3 &lhs, const norm3 &rhs) { return norm3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	norm3 operator-(const norm3 &vec) { return norm3(-vec.x, -vec.y, -vec.z); }
	greal dot(const norm3 &lhs, const norm3 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	norm3 cross(const norm3 &lhs, const norm3 &rhs)
	{
		return norm3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}
	greal length(const norm3 &vec) { return std::sqrtf(dot(vec, vec)); }
	norm3 normalize(const norm3 &vec) { return vec / length(vec); }
	// --- COLORHDR
	bool operator==(const colorHDR &lhs, const colorHDR &rhs) { return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a; }
	bool operator!=(const colorHDR &lhs, const colorHDR &rhs) { return !(lhs == rhs); }
	bool operator<(const colorHDR &lhs, const colorHDR &rhs);
	bool operator>(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator*(const colorHDR &lhs, const colorHDR &rhs) { return colorHDR(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a); }
	colorHDR operator*(const colorHDR &lhs, greal rhs) { return colorHDR(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs); }
	colorHDR operator*(greal rhs, const colorHDR &lhs) { return colorHDR(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs); }
	colorHDR operator/(const colorHDR &lhs, greal rhs) { return colorHDR(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs); }
	colorHDR operator+(const colorHDR &lhs, const colorHDR &rhs) { return colorHDR(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a); }
	colorHDR operator-(const colorHDR &lhs, const colorHDR &rhs) { return colorHDR(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a); }
	colorHDR operator-(const colorHDR &vec) { return colorHDR(-vec.r, -vec.g, -vec.b, -vec.a); }
	colorHDR &operator+=(colorHDR &lhs, const colorHDR &rhs) { lhs.r += rhs.r; lhs.g += rhs.g; lhs.b += rhs.b; lhs.a += rhs.a; return lhs; }
	colorHDR &operator-=(colorHDR &lhs, const colorHDR &rhs) { lhs.r -= rhs.r; lhs.g -= rhs.g; lhs.b -= rhs.b; lhs.a -= rhs.a; return lhs; }
	colorHDR saturate(const colorHDR &vec) { return colorHDR(math::saturate(vec.r), math::saturate(vec.g), math::saturate(vec.b), math::saturate(vec.a)); }
	bool hasNan(const colorHDR & color)
	{
		return isnan(color.r) || isnan(color.g) || isnan(color.b) || isnan(color.a);
	}
	// MAT3
	mat3 operator*(const mat3& lhs, const mat3 &rhs)
	{
		mat3 out = mat3::zero();
		for (int iCol = 0; iCol < 3; iCol++)
			for (int iRow = 0; iRow < 3; iRow++)
				for (int k = 0; k < 3; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}
	norm3 operator*(const mat3& lhs, const norm3 &rhs)
	{
		return norm3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
		);
	}
	// MAT4
	mat4 operator*(const mat4& lhs, const mat4 &rhs)
	{
		mat4 out = mat4::zero();
		for (int iCol = 0; iCol < 4; iCol++)
			for (int iRow = 0; iRow < 4; iRow++)
				for (int k = 0; k < 4; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}
	point3 operator*(const mat4& lhs, const point3 &rhs)
	{
		return point3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
		);
	}
	// PRINT
	std::ostream& operator <<(std::ostream& os, const uv2& vec) { std::cout << "uv2(" << vec.u << ", " << vec.v << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const vec3& vec) { std::cout << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const point3& vec) { std::cout << "point3(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const norm3& vec) { std::cout << "norm3(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const colorHDR& vec) { std::cout << "colorHDR(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const color32& vec) { std::cout << "color32(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const quat& vec) { std::cout << "quat(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const mat3::col& vec) { std::cout << "ma3::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const mat3& vec) { std::cout << "mat3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const mat4::col& vec) { std::cout << "mat4::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")"; return os; }
	std::ostream& operator <<(std::ostream& os, const mat4& vec) { std::cout << "mat4(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")"; return os; }
}