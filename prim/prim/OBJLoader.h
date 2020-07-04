#pragma once

#include "Loader.h"

namespace prim {

class OBJLoader : public Loader
{
public:
	bool load(const std::string &path, Scene &scene) override;
};

}