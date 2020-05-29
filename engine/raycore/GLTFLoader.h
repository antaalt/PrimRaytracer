#pragma once

#include "Loader.h"

namespace raycore {

class GLTFLoader : public Loader
{
public:
	bool load(Reader &reader, Scene &scene) override;
};

}