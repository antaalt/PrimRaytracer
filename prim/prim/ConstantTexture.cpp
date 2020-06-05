#include "ConstantTexture.h"


namespace prim {

template <typename T>
ConstantTexture<T>::ConstantTexture(T albedo) : m_albedo(albedo)
{
}

template <typename T>
T ConstantTexture<T>::evaluate(const uv2f &uv = uv2f(0.f)) const
{
	return m_albedo;
}

template class ConstantTexture<float>;
template class ConstantTexture<color4f>;

}