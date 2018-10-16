#pragma once
#include "Texture.h"

namespace raycore {


	template <typename T>
	class Checker : public Texture<T> {
	public:
		Checker(T color1, T color2, unsigned int x = 1, unsigned int y = 1);
		virtual T evaluate(const uv2 &uv = uv2(0.f)) const;
		virtual Texture* clone() const;
	private:
		T color1, color2;
		unsigned int uscale, vscale;
	};

}