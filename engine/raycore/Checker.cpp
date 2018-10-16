#include "Checker.h"


namespace raycore {
	template <typename T>
	Checker<T>::Checker(T color1, T color2, unsigned int uscale, unsigned int vscale) : 
		color1(color1), 
		color2(color2), 
		uscale(uscale), 
		vscale(vscale)
	{
	}

	template <typename T>
	T Checker<T>::evaluate(const uv2 &uv = uv2(0.f)) const
	{
		unsigned int u2 = static_cast<unsigned int>(uv.x * 2.f * uscale) % 2;
		unsigned int v2 = static_cast<unsigned int>(uv.y * 2.f * vscale) % 2;
		if ((u2 == 0 && v2 == 1) || (u2 == 1 && v2 == 0))
			return color2;
		return color1;
	}
	template <typename T>
	Texture<T> * Checker<T>::clone() const
	{
		return new Checker(*this);
	}


	template class Checker<float>;
	template class Checker<colorHDR>;

}