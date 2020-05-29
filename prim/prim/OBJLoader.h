#pragma once

#include "Loader.h"

namespace prim {

class OBJLoader : public Loader
{
public:
	bool load(Reader &reader, Scene &scene) override;
};

}