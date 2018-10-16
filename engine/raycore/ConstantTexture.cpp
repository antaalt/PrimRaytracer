#include "ConstantTexture.h"


namespace raycore {

	template <typename T>
	ConstantTexture<T>::ConstantTexture(T albedo) : albedo(albedo)
	{
	}

	template <typename T>
	T ConstantTexture<T>::evaluate(const uv2 &uv = uv2(0.f)) const
	{
		return albedo;
	}

	template <typename T>
	Texture<T> * ConstantTexture<T>::clone() const
	{
		return new ConstantTexture(*this);
	}

	template class ConstantTexture<float>;
	template class ConstantTexture<colorHDR>;
}