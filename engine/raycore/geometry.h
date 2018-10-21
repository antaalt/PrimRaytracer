#pragma once
#include <ostream>

namespace math {
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
	struct uv2;
	struct vec3;
	struct point3;
	struct norm3;
	struct colorHDR;
	struct color32;
	struct quat;
	struct mat3;
	struct mat4;

	using greal = float;
	using gbyte = unsigned char;

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
			col(greal x, greal y, greal z);
			greal &operator[](size_t index);
			const greal &operator[](size_t index) const;
			operator point3();
			operator norm3();
		} cols[3];
		mat3();
		mat3(const mat4& mat);
		mat3(col x, col y, col z);
		mat3::col &operator[](size_t index);
		const mat3::col &operator[](size_t index) const;
		static mat3 identity();
		static mat3 zero();
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
			col(greal x, greal y, greal z, greal w);
			col(norm3 vec, greal w);
			col(point3 vec, greal w);
			greal &operator[](size_t index);
			const greal &operator[](size_t index) const;
			operator point3();
			operator norm3();
		} cols[4];
		mat4();
		mat4(col x, col y, col z, col w);
		mat4(const quat &quat);
		mat4::col &operator[](size_t index);
		const mat4::col &operator[](size_t index) const;
		static mat4 identity();
		static mat4 zero();
		static mat4 TRS(const vec3 &translation, const quat &rotation, const vec3 &scale);
		float det() const;
	};

	// --- VEC3 + NORM3
	greal dot(const vec3 &lhs, const norm3 &rhs);
	norm3 cross(const norm3 &lhs, const norm3 &rhs);
	vec3 operator-(const vec3 &lhs, const norm3 &rhs);
	norm3 operator-(const norm3 &lhs, const vec3 &rhs);

	// --- VEC3 + POINT3
	vec3 operator+(const point3 &lhs, const vec3 &rhs);
	greal dot(const point3 &lhs, const norm3 &rhs);

	// --- UV2
	bool operator==(const uv2 &lhs, const uv2 &rhs);
	bool operator!=(const uv2 &lhs, const uv2 &rhs);

	uv2 operator*(const uv2 &lhs, greal rhs);
	uv2 operator*(greal lhs, const uv2 &rhs);
	uv2 operator/(const uv2 &lhs, greal rhs);
	uv2 operator+(const uv2 &lhs, const uv2 &rhs);
	uv2 operator-(const uv2 &lhs, const uv2 &rhs);
	uv2 operator-(const uv2 &vec);

	// --- VEC3
	bool operator==(const vec3 &lhs, const vec3 &rhs);
	bool operator!=(const vec3 &lhs, const vec3 &rhs);
	bool operator<(const vec3 &lhs, const vec3 &rhs);
	bool operator>(const vec3 &lhs, const vec3 &rhs);

	vec3 operator*(const vec3 &lhs, greal rhs);
	vec3 operator*(greal lhs, const vec3 &rhs);
	vec3 operator/(const vec3 &lhs, greal rhs);
	vec3 operator+(const vec3 &lhs, const vec3 &rhs);
	vec3 operator-(const vec3 &lhs, const vec3 &rhs);
	vec3 operator-(const vec3 &vec);

	greal dot(const vec3 &lhs, const vec3 &rhs);
	vec3 cross(const vec3 &lhs, const vec3 &rhs);
	greal length(const vec3 &vec);
	vec3 normalize(const vec3 &vec);

	// --- POINT3
	bool operator==(const point3 &lhs, const point3 &rhs);
	bool operator!=(const point3 &lhs, const point3 &rhs);

	point3 operator*(const point3 &lhs, greal rhs);
	point3 operator*(greal rhs, const point3 &lhs);
	point3 operator/(const point3 &lhs, greal rhs);
	point3 operator+(const point3 &lhs, const point3 &rhs);
	point3 operator-(const point3 &lhs, const point3 &rhs);
	point3 operator-(const point3 &vec);

	greal distance(const point3 &lhs, const point3 &rhs);

	// --- NORM3
	bool operator==(const norm3 &lhs, const norm3 &rhs);
	bool operator!=(const norm3 &lhs, const norm3 &rhs);

	norm3 operator*(const norm3 &lhs, greal rhs);
	norm3 operator*(greal rhs, const norm3 &lhs);
	norm3 operator/(const norm3 &lhs, greal rhs);
	norm3 operator+(const norm3 &lhs, const norm3 &rhs);
	norm3 operator-(const norm3 &lhs, const norm3 &rhs);
	norm3 operator-(const norm3 &vec);
	greal length(const norm3 &vec);
	norm3 normalize(const norm3 &vec);

	greal dot(const norm3 &lhs, const norm3 &rhs);
	norm3 cross(const norm3 &lhs, const norm3 &rhs);


	// --- COLOR
	bool operator==(const colorHDR &lhs, const colorHDR &rhs);
	bool operator!=(const colorHDR &lhs, const colorHDR &rhs);
	bool operator<(const colorHDR &lhs, const colorHDR &rhs);
	bool operator>(const colorHDR &lhs, const colorHDR &rhs);

	colorHDR operator*(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator*(const colorHDR &lhs, greal rhs);
	colorHDR operator*(greal rhs, const colorHDR &lhs);
	colorHDR operator/(const colorHDR &lhs, greal rhs);
	colorHDR operator+(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator-(const colorHDR &lhs, const colorHDR &rhs);
	colorHDR operator-(const colorHDR &vec);
	colorHDR &operator+=(colorHDR &lhs, const colorHDR &rhs);
	colorHDR &operator-=(colorHDR &lhs, const colorHDR &rhs);

	colorHDR saturate(const colorHDR &vec);
	bool hasNan(const colorHDR &color);


	// --- MAT3
	mat3 operator*(const mat3& lhs, const mat3 &rhs);
	norm3 operator*(const mat3& lhs, const norm3 &rhs);

	// --- MAT4
	mat4 operator*(const mat4& lhs, const mat4 &rhs);
	point3 operator*(const mat4& lhs, const point3 &rhs);

	// --- PRINT
	std::ostream& operator <<(std::ostream& os, const uv2& vec);
	std::ostream& operator <<(std::ostream& os, const vec3& vec);
	std::ostream& operator <<(std::ostream& os, const point3& vec);
	std::ostream& operator <<(std::ostream& os, const norm3& vec);
	std::ostream& operator <<(std::ostream& os, const colorHDR& vec);
	std::ostream& operator <<(std::ostream& os, const color32& vec);
	std::ostream& operator <<(std::ostream& os, const quat& vec);
	std::ostream& operator <<(std::ostream& os, const mat3& vec);
	std::ostream& operator <<(std::ostream& os, const mat4& vec);
}