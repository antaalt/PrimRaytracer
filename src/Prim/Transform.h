#pragma once
#include "Config.h"

#include <Aka/Core/Geometry.h>
#include "BoundingBox.h"
#include "Intersection.h"

namespace prim {

// World coordinates have z as up
// Screen coordinates have y as up to fit screen
// All coordinates are left handed
struct Transform {
	explicit Transform() : Transform(mat4f::identity()) {}
	explicit Transform(const mat4f &matrix) : Transform(matrix, mat4f::inverse(matrix)) {}
	explicit Transform(const mat4f &matrix, const mat4f &inverse) : m_matrix(matrix), m_inverse(inverse) {}

	const mat4f &getMatrix() const { return m_matrix; }
	const mat4f &getInverseMatrix() const { return m_inverse; }

	Transform inverse() const;

	template <typename T>
	vec3<T> operator()(const vec3<T> &) const;
	template <typename T>
	vec4<T> operator()(const vec4<T> &) const;
	template <typename T>
	point3<T> operator()(const point3<T> &) const;
	template <typename T>
	norm3<T> operator()(const norm3<T> &) const;

	Ray operator()(const Ray &) const;

	BoundingBox operator()(const BoundingBox &) const;

	bool swapHandedness() const;

	Transform operator*(const Transform &rhs) const;
	Transform &operator*=(const Transform &rhs);
private:
	mat4f m_matrix;
	mat4f m_inverse;
};

// Is Onb needed now as we have transform ?
struct Onb {
	Onb(const norm3f &n);
	vec3f operator()(const vec3f &n);
	norm3f operator()(const norm3f &n);
private:
	vec3f m_normal;
	vec3f m_tangent;
	vec3f m_bitangent;
};

template<typename T>
inline vec3<T> Transform::operator()(const vec3<T>& vec) const
{
	return vec3<T>(
		m_matrix[0].x * vec.x + m_matrix[1].x * vec.y + m_matrix[2].x * vec.z,
		m_matrix[0].y * vec.x + m_matrix[1].y * vec.y + m_matrix[2].y * vec.z,
		m_matrix[0].z * vec.x + m_matrix[1].z * vec.y + m_matrix[2].z * vec.z
	);
}

template<typename T>
inline vec4<T> Transform::operator()(const vec4<T>& vec) const
{
	return vec4<T>(
		m_matrix[0].x * vec.x + m_matrix[1].x * vec.y + m_matrix[2].x * vec.z + m_matrix[3].x * vec.w,
		m_matrix[0].y * vec.x + m_matrix[1].y * vec.y + m_matrix[2].y * vec.z + m_matrix[3].y * vec.w,
		m_matrix[0].z * vec.x + m_matrix[1].z * vec.y + m_matrix[2].z * vec.z + m_matrix[3].z * vec.w,
		m_matrix[0].w * vec.x + m_matrix[1].w * vec.y + m_matrix[2].w * vec.z + m_matrix[3].w * vec.w
	);
}

template<typename T>
inline point3<T> Transform::operator()(const point3<T>& point) const
{
	return point3<T>(
		m_matrix[0].x * point.x + m_matrix[1].x * point.y + m_matrix[2].x * point.z + m_matrix[3].x,
		m_matrix[0].y * point.x + m_matrix[1].y * point.y + m_matrix[2].y * point.z + m_matrix[3].y,
		m_matrix[0].z * point.x + m_matrix[1].z * point.y + m_matrix[2].z * point.z + m_matrix[3].z
	);
}

template<typename T>
inline norm3<T> Transform::operator()(const norm3<T>& norm) const
{
	// We compute normal with transpose of inverse.
	return norm3<T>(
		m_inverse[0].x * norm.x + m_inverse[0].y * norm.y + m_inverse[0].z * norm.z,
		m_inverse[1].x * norm.x + m_inverse[1].y * norm.y + m_inverse[1].z * norm.z,
		m_inverse[2].x * norm.x + m_inverse[2].y * norm.y + m_inverse[2].z * norm.z
	);
}

}