#pragma once
#include "Texture.h"

namespace raycore {
	template <typename T>
	class ConstantTexture : public Texture<T> {
	public:
		ConstantTexture(T albedo);
		virtual T evaluate(const uv2 &uv = uv2(0.f)) const;
		virtual Texture* clone() const;
	private:
		T albedo;
	};
}

