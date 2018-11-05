#pragma once
#include <cmath>
#include <iostream>

namespace math {

	/*struct Radians {
		Radians();
		Radians(const Degree &degree);
	private:
		float angle;
	};

	struct Degree {
		Degree();
		Degree(const Radians &radians);
	private:
		float angle;
	};*/

	template <typename T>
	inline T clamp(T value, T min, T max)
	{
		if (value > max) return max;
		if (value < min) return min;
		return value;
	}

	inline float saturate(float value)
	{
		return clamp<float>(value, 0.f, 1.f);
	}

	template <typename T>
	inline T lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}
	template <typename T>
	inline T max(T a, T b)
	{
		if (a > b) return a;
		return b;
	}
	template <typename T>
	inline T min(T a, T b)
	{
		if (a < b) return a;
		return b;
	}
}

namespace geometry {
	using greal = float;
	using gbyte = unsigned char;

	struct uv2;
	struct vec3;
	struct point3;
	struct norm3;
	struct colorHDR;
	struct color32;
	struct quat;
	struct mat3;
	struct mat4;

	struct uv2 {
		union {
			greal data[2];
			struct {
				greal u, v;
			};
		};
		uv2();
		explicit uv2(greal value);
		explicit uv2(greal u, greal v);
		greal &operator[](size_t index);
	};
	struct point3 {
		union {
			greal data[3];
			struct {
				greal x, y, z;
			};
		};
		point3();
		explicit point3(greal value);
		explicit point3(greal x, greal y, greal z);
		explicit point3(const norm3 &normal);
		explicit point3(const vec3 &vec);
		greal &operator[](size_t index);
	};
	struct vec3 {
		union {
			greal data[3];
			struct {
				greal x, y, z;
			};
		};
		vec3();
		explicit vec3(greal value);
		explicit vec3(greal x, greal y, greal z);
		explicit vec3(const norm3 &normal);
		explicit vec3(const point3 &point);
		greal &operator[](size_t index);
		const greal &operator[](size_t index) const;
	};
	struct norm3 {
		union {
			greal data[3];
			struct {
				greal x, y, z;
			};
		};
		norm3();
		explicit norm3(greal value);
		explicit norm3(greal x, greal y, greal z);
		explicit norm3(const vec3 &vec);
		explicit norm3(const point3 &point);
		greal &operator[](size_t index);
	};
	struct colorHDR {
		union {
			greal data[4];
			struct {
				greal r, g, b, a;
			};
		};
		colorHDR();
		explicit colorHDR(greal value);
		explicit colorHDR(greal r, greal g, greal b, greal a);
		explicit colorHDR(const color32 &color);
		greal &operator[](size_t index);
	};
	struct color32 {
		union {
			gbyte data[4];
			struct {
				gbyte r, g, b, a;
			};
		};
		color32();
		explicit color32(gbyte value);
		explicit color32(gbyte r, gbyte g, gbyte b, gbyte a);
		explicit color32(const colorHDR &color);
		gbyte &operator[](size_t index);
	};
	struct quat {
		union {
			greal data[4];
			struct {
				greal x, y, z, w;
			};
		};
		quat();
		quat(greal x, greal y, greal z, greal w);
		greal &operator[](size_t index);
		static quat identity();
	};

	struct mat3 {
		struct col {
			union {
				greal data[3];
				struct {
					greal x, y, z;
				};
			};
			col();
			col(greal value);
			col(greal x, greal y, greal z);
			greal &operator[](size_t index);
			const greal &operator[](size_t index) const;
			operator point3();
			operator norm3();
		} cols[3];
		mat3();
		mat3(greal value);
		mat3(const mat4& mat);
		mat3(col x, col y, col z);
		mat3::col &operator[](size_t index);
		const mat3::col &operator[](size_t index) const;
		static mat3 identity();
	};

	struct mat4 {
		struct col {
			union {
				greal data[4];
				struct {
					greal x, y, z, w;
				};
			};
			col();
			col(greal value);
			col(greal x, greal y, greal z, greal w);
			col(norm3 vec, greal w);
			col(vec3 vec, greal w);
			col(point3 vec, greal w);
			greal &operator[](size_t index);
			const greal &operator[](size_t index) const;
			operator point3();
			operator norm3();
		} cols[4];
		mat4();
		mat4(greal value);
		mat4(col x, col y, col z, col w);
		mat4(const quat &quat);
		mat4::col &operator[](size_t index);
		const mat4::col &operator[](size_t index) const;
		static mat4 identity();
		float det() const;
	};

	// --- VEC3 + NORM3 + POINT3
	inline greal dot(const vec3 & lhs, const norm3 & rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	inline greal dot(const point3 &lhs, const norm3 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	inline vec3 operator-(const vec3 &lhs, const norm3 &rhs) { return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline norm3 operator-(const norm3 &lhs, const vec3 &rhs) { return norm3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline vec3 operator+(const point3 &lhs, const vec3 &rhs) { return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }

	// --- UV2
	inline uv2::uv2() {}
	inline uv2::uv2(greal value) : u(value), v(value) {}
	inline uv2::uv2(greal u, greal v) : u(u), v(v) {}
	inline greal & uv2::operator[](size_t index) { return data[index]; }
	inline bool operator==(const uv2 &lhs, const uv2 &rhs) { return lhs.u == rhs.u && lhs.v == rhs.v; }
	inline bool operator!=(const uv2 &lhs, const uv2 &rhs) { return !(lhs == rhs); }
	inline uv2 operator*(const uv2 &lhs, greal rhs) { return uv2(lhs.u * rhs, lhs.v * rhs); }
	inline uv2 operator*(greal lhs, const uv2 &rhs) { return uv2(lhs * rhs.u, lhs * rhs.v); }
	inline uv2 operator/(const uv2 &lhs, greal rhs) { return uv2(lhs.u / rhs, lhs.v / rhs); }
	inline uv2 operator+(const uv2 &lhs, const uv2 &rhs) { return uv2(lhs.u + rhs.u, lhs.v + rhs.v); }
	inline uv2 operator-(const uv2 &lhs, const uv2 &rhs) { return uv2(lhs.u - rhs.u, lhs.v - rhs.v); }
	inline uv2 operator-(const uv2 &vec) { return uv2(-vec.u, -vec.v); }

	// --- VEC3
	inline vec3::vec3() {}
	inline vec3::vec3(greal value) : x(value), y(value), z(value) {}
	inline vec3::vec3(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	inline vec3::vec3(const norm3 &normal) : x(normal.x), y(normal.y), z(normal.z) {}
	inline vec3::vec3(const point3 &point) : x(point.x), y(point.y), z(point.z) {}
	inline greal & vec3::operator[](size_t index) { return data[index]; }
	inline const greal & vec3::operator[](size_t index) const { return data[index]; }
	inline bool operator==(const vec3 & lhs, const vec3 & rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	inline bool operator!=(const vec3 &lhs, const vec3 &rhs) { return !(lhs == rhs); }
	inline vec3 operator*(const vec3 &lhs, greal rhs) { return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
	inline vec3 operator*(greal lhs, const vec3 &rhs) { return vec3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }
	inline vec3 operator/(const vec3 &lhs, greal rhs) { return vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
	inline vec3 operator+(const vec3 &lhs, const vec3 &rhs) { return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	inline vec3 operator-(const vec3 &lhs, const vec3 &rhs) { return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline vec3 operator-(const vec3 &vec) { return vec3(-vec.x, -vec.y, -vec.z); }
	inline float dot(const vec3 & lhs, const vec3 & rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	inline vec3 cross(const vec3 & lhs, const vec3 & rhs) { return vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x); }
	inline greal length(const vec3 &vec) { return std::sqrtf(dot(vec, vec)); }
	inline vec3 normalize(const vec3 &vec) { return vec / length(vec); }

	// --- POINT3
	inline point3::point3() {}
	inline point3::point3(greal value) : x(value), y(value), z(value) {}
	inline point3::point3(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	inline point3::point3(const norm3 & normal) : x(normal.x), y(normal.y), z(normal.z) {}
	inline point3::point3(const vec3 & vec) : x(vec.x), y(vec.y), z(vec.z) {}
	inline greal & point3::operator[](size_t index) { return data[index]; }
	inline bool operator==(const point3 &lhs, const point3 &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	inline bool operator!=(const point3 &lhs, const point3 &rhs) { return !(lhs == rhs); }
	inline point3 operator*(const point3 &lhs, greal rhs) { return point3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
	inline point3 operator*(greal lhs, const point3 &rhs) { return point3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }
	inline point3 operator/(const point3 &lhs, greal rhs) { return point3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
	inline point3 operator+(const point3 &lhs, const point3 &rhs) { return point3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	inline point3 operator-(const point3 &lhs, const point3 &rhs) { return point3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline point3 operator-(const point3 &vec) { return point3(-vec.x, -vec.y, -vec.z); }
	inline point3 &operator+=(point3 &lhs, const point3 &rhs) { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs; }
	inline point3 &operator-=(point3 &lhs, const point3 &rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs; }
	inline point3 &operator/=(point3 &lhs, float rhs) { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }
	inline greal distance(const point3 &lhs, const point3 &rhs) { return length(vec3(rhs - lhs)); }

	// --- NORM3
	inline norm3::norm3() {}
	inline norm3::norm3(greal value) : x(value), y(value), z(value) {}
	inline norm3::norm3(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	inline norm3::norm3(const vec3 & vec) : x(vec.x), y(vec.y), z(vec.z) {}
	inline norm3::norm3(const point3 & point) : x(point.x), y(point.y), z(point.z) {}
	inline greal & norm3::operator[](size_t index) { return data[index]; }
	inline bool operator==(const norm3 &lhs, const norm3 &rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	inline bool operator!=(const norm3 &lhs, const norm3 &rhs) { return !(lhs == rhs); }
	inline norm3 operator*(const norm3 &lhs, greal rhs) { return norm3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
	inline norm3 operator*(greal lhs, const norm3 &rhs) { return norm3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs); }
	inline norm3 operator/(const norm3 &lhs, greal rhs) { return norm3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
	inline norm3 operator+(const norm3 &lhs, const norm3 &rhs) { return norm3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	inline norm3 operator-(const norm3 &lhs, const norm3 &rhs) { return norm3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	inline norm3 operator-(const norm3 &vec) { return norm3(-vec.x, -vec.y, -vec.z); }
	inline greal dot(const norm3 &lhs, const norm3 &rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	inline norm3 normalize(const norm3 &vec) { return vec / std::sqrtf(dot(vec, vec)); }
	inline norm3 faceForward(const norm3 &n, const vec3 &v) { return (dot(v, n) < 0.f) ? -n : n; }

	// --- COLOR
	inline colorHDR::colorHDR() {}
	inline colorHDR::colorHDR(greal value) : r(value), g(value), b(value), a(value) {}
	inline colorHDR::colorHDR(const color32 & color) : r(powf(color.r / 255.f, 2.2f)), g(powf(color.g / 255.f, 2.2f)), b(powf(color.b / 255.f, 2.2f)), a(color.a / 255.f) {}
	inline greal & colorHDR::operator[](size_t index) { return data[index]; }
	inline colorHDR::colorHDR(greal r, greal g, greal b, greal a) : r(r), g(g), b(b), a(a) {}
	inline bool operator==(const colorHDR &lhs, const colorHDR &rhs) { return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a; }
	inline bool operator!=(const colorHDR &lhs, const colorHDR &rhs) { return !(lhs == rhs); }
	inline colorHDR operator*(const colorHDR &lhs, const colorHDR &rhs) { return colorHDR(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a); }
	inline colorHDR operator*(const colorHDR &lhs, greal rhs) { return colorHDR(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs); }
	inline colorHDR operator*(greal rhs, const colorHDR &lhs) { return colorHDR(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs); }
	inline colorHDR operator/(const colorHDR &lhs, greal rhs) { return colorHDR(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a / rhs); }
	inline colorHDR operator+(const colorHDR &lhs, const colorHDR &rhs) { return colorHDR(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a); }
	inline colorHDR operator-(const colorHDR &lhs, const colorHDR &rhs) { return colorHDR(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a); }
	inline colorHDR operator-(const colorHDR &vec) { return colorHDR(-vec.r, -vec.g, -vec.b, -vec.a); }
	inline colorHDR &operator+=(colorHDR &lhs, const colorHDR &rhs) { lhs.r += rhs.r; lhs.g += rhs.g; lhs.b += rhs.b; lhs.a += rhs.a; return lhs; }
	inline colorHDR &operator-=(colorHDR &lhs, const colorHDR &rhs) { lhs.r -= rhs.r; lhs.g -= rhs.g; lhs.b -= rhs.b; lhs.a -= rhs.a; return lhs; }
	inline colorHDR saturate(const colorHDR &vec) { return colorHDR(math::saturate(vec.r), math::saturate(vec.g), math::saturate(vec.b), math::saturate(vec.a)); }
	inline bool hasNan(const colorHDR & color) { return isnan(color.r) || isnan(color.g) || isnan(color.b) || isnan(color.a); }

	// --- COLOR32
	inline color32::color32() {}
	inline color32::color32(gbyte value) : r(value), g(value), b(value), a(value) {}
	inline color32::color32(gbyte r, gbyte g, gbyte b, gbyte a) : r(r), g(g), b(b), a(a) {}
	inline color32::color32(const colorHDR & color) :
		r(static_cast<gbyte>(math::saturate(powf(color.r, 1 / 2.2f)) * 255.f)),
		g(static_cast<gbyte>(math::saturate(powf(color.g, 1 / 2.2f)) * 255.f)),
		b(static_cast<gbyte>(math::saturate(powf(color.b, 1 / 2.2f)) * 255.f)),
		a(static_cast<gbyte>(math::saturate(color.a) * 255.f)) {}
	inline gbyte & color32::operator[](size_t index) { return data[index]; }

	// --- QUAT
	inline quat::quat() {}
	inline quat::quat(greal x, greal y, greal z, greal w) : x(x), y(y), z(z), w(w) {}
	inline greal & quat::operator[](size_t index) { return data[index]; }
	inline quat quat::identity() { return quat(0.f, 0.f, 0.f, 1.f); }

	// --- MAT3
	inline mat3::col::col() {}
	inline mat3::col::col(greal value) : x(value), y(value), z(value) {}
	inline mat3::col::col(greal x, greal y, greal z) : x(x), y(y), z(z) {}
	inline greal & mat3::col::operator[](size_t index) { return data[index]; }
	inline const greal &mat3::col::operator[](size_t index) const { return data[index]; }
	inline mat3::col::operator point3() { return point3(x, y, z); }
	inline mat3::col::operator norm3() { return norm3(x, y, z); }
	inline mat3::mat3() {}
	inline mat3::mat3(greal value) : cols{ mat3::col(0.f), mat3::col(0.f), mat3::col(0.f) } {}
	inline mat3::mat3(const mat4 & mat) : cols{
		mat3::col(mat[0].x, mat[0].y, mat[0].z),
		mat3::col(mat[1].x, mat[1].y, mat[1].z),
		mat3::col(mat[2].x, mat[2].y, mat[2].z)
	} {}
	inline mat3::mat3(col x, col y, col z) : cols{ x, y, z } {}
	inline mat3::col & mat3::operator[](size_t index) { return cols[index]; }
	inline const mat3::col & mat3::operator[](size_t index) const { return cols[index]; }
	inline mat3 mat3::identity() { return mat3(mat3::col(1.f, 0.f, 0.f), mat3::col(0.f, 1.f, 0.f), mat3::col(0.f, 0.f, 1.f)); }
	inline mat3 operator*(const mat3& lhs, const mat3 &rhs)
	{
		mat3 out(0.f);
		for (int iCol = 0; iCol < 3; iCol++)
			for (int iRow = 0; iRow < 3; iRow++)
				for (int k = 0; k < 3; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}
	inline norm3 operator*(const mat3& lhs, const norm3 &rhs)
	{
		return norm3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
		);
	}

	// --- MAT4
	inline mat4::col::col() {}
	inline mat4::col::col(greal value) : x(value), y(value), z(value), w(value) {}
	inline mat4::col::col(greal x, greal y, greal z, greal w) : x(x), y(y), z(z), w(w) {}
	inline mat4::col::col(norm3 vec, greal w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
	inline mat4::col::col(vec3 vec, greal w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
	inline mat4::col::col(point3 vec, greal w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
	inline greal & mat4::col::operator[](size_t index) { return data[index]; }
	inline const greal & mat4::col::operator[](size_t index) const { return data[index]; }
	inline mat4::mat4() {}
	inline mat4::mat4(greal value) : cols{ col(value), col(value), col(value), col(value) } {}
	inline mat4::mat4(col x, col y, col z, col w) : cols{ x, y, z, w } {}
	inline mat4::mat4(const quat & quat) {}
	inline mat4::col & mat4::operator[](size_t index) { return cols[index]; }
	inline const mat4::col & mat4::operator[](size_t index) const { return cols[index]; }
	inline mat4 operator*(const mat4& lhs, const mat4 &rhs)
	{
		mat4 out(0.f);
		for (int iCol = 0; iCol < 4; iCol++)
			for (int iRow = 0; iRow < 4; iRow++)
				for (int k = 0; k < 4; k++)
					out[iCol][iRow] += rhs[iCol][k] * lhs[k][iRow];
		return out;
	}
	inline point3 operator*(const mat4& lhs, const point3 &rhs)
	{
		return point3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z + lhs[3].x,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z + lhs[3].y,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z + lhs[3].z
		);
	}
	inline vec3 operator*(const mat4& lhs, const vec3 &rhs)
	{
		return vec3(
			lhs[0].x * rhs.x + lhs[1].x * rhs.y + lhs[2].x * rhs.z,
			lhs[0].y * rhs.x + lhs[1].y * rhs.y + lhs[2].y * rhs.z,
			lhs[0].z * rhs.x + lhs[1].z * rhs.y + lhs[2].z * rhs.z
		);
	}
	inline mat4 mat4::identity() { return mat4(mat4::col(1.f, 0.f, 0.f, 0.f), mat4::col(0.f, 1.f, 0.f, 0.f), mat4::col(0.f, 0.f, 1.f, 0.f), mat4::col(0.f, 0.f, 0.f, 1.f)); }
	inline mat4 TRS(const vec3 & translation, const quat & rotation, const vec3 & scale)
	{
		mat4 t = mat4::identity();
		t[3] = mat4::col(translation.x, translation.y, translation.z, 1.f);
		mat4 r(rotation);
		mat4 s = mat4::identity();
		for (int i = 0; i < 3; i++)
			s[i][i] = scale[i];
		return t * (r * s);
	}
	inline mat4 inverse(const mat4 &mat)
	{
		float A2323 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];
		float A1323 = mat[2][1] * mat[3][3] - mat[2][3] * mat[3][1];
		float A1223 = mat[2][1] * mat[3][2] - mat[2][2] * mat[3][1];
		float A0323 = mat[2][0] * mat[3][3] - mat[2][3] * mat[3][0];
		float A0223 = mat[2][0] * mat[3][2] - mat[2][2] * mat[3][0];
		float A0123 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];
		float A2313 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];
		float A1313 = mat[1][1] * mat[3][3] - mat[1][3] * mat[3][1];
		float A1213 = mat[1][1] * mat[3][2] - mat[1][2] * mat[3][1];
		float A2312 = mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2];
		float A1312 = mat[1][1] * mat[2][3] - mat[1][3] * mat[2][1];
		float A1212 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
		float A0313 = mat[1][0] * mat[3][3] - mat[1][3] * mat[3][0];
		float A0213 = mat[1][0] * mat[3][2] - mat[1][2] * mat[3][0];
		float A0312 = mat[1][0] * mat[2][3] - mat[1][3] * mat[2][0];
		float A0212 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
		float A0113 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
		float A0112 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];

		float det = 1.f / mat.det();

		return mat4(
			mat4::col(
				det *   (mat[1][1] * A2323 - mat[1][2] * A1323 + mat[1][3] * A1223),
				det * -(mat[0][1] * A2323 - mat[0][2] * A1323 + mat[0][3] * A1223),
				det *   (mat[0][1] * A2313 - mat[0][2] * A1313 + mat[0][3] * A1213),
				det * -(mat[0][1] * A2312 - mat[0][2] * A1312 + mat[0][3] * A1212)
			),
			mat4::col(
				det * -(mat[1][0] * A2323 - mat[1][2] * A0323 + mat[1][3] * A0223),
				det *   (mat[0][0] * A2323 - mat[0][2] * A0323 + mat[0][3] * A0223),
				det * -(mat[0][0] * A2313 - mat[0][2] * A0313 + mat[0][3] * A0213),
				det *   (mat[0][0] * A2312 - mat[0][2] * A0312 + mat[0][3] * A0212)
			),
			mat4::col(
				det *   (mat[1][0] * A1323 - mat[1][1] * A0323 + mat[1][3] * A0123),
				det * -(mat[0][0] * A1323 - mat[0][1] * A0323 + mat[0][3] * A0123),
				det *   (mat[0][0] * A1313 - mat[0][1] * A0313 + mat[0][3] * A0113),
				det * -(mat[0][0] * A1312 - mat[0][1] * A0312 + mat[0][3] * A0112)
			),
			mat4::col(
				det * -(mat[1][0] * A1223 - mat[1][1] * A0223 + mat[1][2] * A0123),
				det *   (mat[0][0] * A1223 - mat[0][1] * A0223 + mat[0][2] * A0123),
				det * -(mat[0][0] * A1213 - mat[0][1] * A0213 + mat[0][2] * A0113),
				det *   (mat[0][0] * A1212 - mat[0][1] * A0212 + mat[0][2] * A0112)
			)
		);
	}
	inline float mat4::det() const
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
	inline mat4::col::operator point3() { return point3(x, y, z); }
	inline mat4::col::operator norm3() { return norm3(x, y, z); }
	// PRINT
	inline std::ostream& operator <<(std::ostream& os, const uv2& vec) { std::cout << "uv2(" << vec.u << ", " << vec.v << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const vec3& vec) { std::cout << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const point3& vec) { std::cout << "point3(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const norm3& vec) { std::cout << "norm3(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const colorHDR& vec) { std::cout << "colorHDR(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const color32& vec) { std::cout << "color32(" << vec.r << ", " << vec.g << ", " << vec.b << ", " << vec.a << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const quat& vec) { std::cout << "quat(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const mat3::col& vec) { std::cout << "mat3::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const mat3& vec) { std::cout << "mat3(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const mat4::col& vec) { std::cout << "mat4::col(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")"; return os; }
	inline std::ostream& operator <<(std::ostream& os, const mat4& vec) { std::cout << "mat4(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")"; return os; }

}
