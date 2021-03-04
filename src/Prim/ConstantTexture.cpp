#include "ConstantTexture.h"


namespace prim {

template <typename T>
ConstantTexture<T>::ConstantTexture(T albedo) : m_albedo(albedo)
{
}

template <typename T>
T ConstantTexture<T>::evaluate(const uv2f &uv) const
{
	return m_albedo;
}

template class ConstantTexture<float>;
template class ConstantTexture<color4f>;

}