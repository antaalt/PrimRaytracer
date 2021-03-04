#pragma once

#include "../../Core/Loader.h"

namespace prim {

class OBJLoader : public Loader
{
public:
	bool load(const aka::Path &path, Scene &scene) override;
};

}