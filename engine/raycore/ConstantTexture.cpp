#include "ConstantTexture.h"


namespace raycore {

template <typename T>
ConstantTexture<T>::ConstantTexture(geometry::color4<T> albedo) : albedo(albedo)
{
}

template <typename T>
geometry::color4<T> ConstantTexture<T>::evaluate(const geometry::uv2f &uv = geometry::uv2f(0.f)) const
{
	return albedo;
}

template <typename T>
Texture<T> * ConstantTexture<T>::clone() const
{
	return new ConstantTexture(*this);
}

template class ConstantTexture<float>;

}