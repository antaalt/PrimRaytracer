#pragma once

#include "Loader.h"

namespace raycore {

class GLTFLoader : public Loader
{
public:
	bool load(Reader &reader, prim::Scene &scene) override;
	bool write(Writer &writer, const prim::Scene &scene) override;
};

}