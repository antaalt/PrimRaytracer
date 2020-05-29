#pragma once

#include "Loader.h"

namespace prim {

class GLTFLoader : public Loader
{
public:
	bool load(Reader &reader, Scene &scene) override;
};

}