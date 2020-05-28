#include "Checker.h"


namespace raycore {

template <typename T>
Checker<T>::Checker(geometry::color4<T> color1, geometry::color4<T> color2, unsigned int uscale = 1, unsigned int vscale = 1) :
	m_color1(color1), 
	m_color2(color2), 
	m_uscale(uscale), 
	m_vscale(vscale)
{
}

template <typename T>
geometry::color4<T> Checker<T>::evaluate(const geometry::uv2f &uv = geometry::uv2f(0.f)) const
{
	unsigned int u2 = static_cast<unsigned int>(uv.u * 2.f * m_uscale) % 2;
	unsigned int v2 = static_cast<unsigned int>(uv.v * 2.f * m_vscale) % 2;
	if ((u2 == 0 && v2 == 1) || (u2 == 1 && v2 == 0))
		return m_color2;
	return m_color1;
}
template <typename T>
Texture<T> * Checker<T>::clone() const
{
	// TODO copy constructor
	return new Checker(*this);
}


template class Checker<float>;

}