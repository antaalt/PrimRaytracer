#include "CheckerTexture.h"


namespace prim {

template <typename T>
CheckerTexture<T>::CheckerTexture(T color1, T color2, uint32_t uscale, uint32_t vscale) :
	m_color1(color1),
	m_color2(color2), 
	m_uscale(uscale), 
	m_vscale(vscale)
{
}

template <typename T>
T CheckerTexture<T>::evaluate(const uv2f &uv) const
{
	uint32_t u2 = static_cast<uint32_t>(uv.u * 2.f * m_uscale) % 2;
	uint32_t v2 = static_cast<uint32_t>(uv.v * 2.f * m_vscale) % 2;
	if (u2 != v2)
		return m_color2;
	return m_color1;
}

template class CheckerTexture<float>;
template class CheckerTexture<color4f>;

}