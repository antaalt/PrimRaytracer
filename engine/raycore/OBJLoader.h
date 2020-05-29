#pragma once

#include "Loader.h"

namespace raycore {

class OBJLoader : public Loader
{
public:
	bool load(Reader &reader, Scene &scene) override;
};

}