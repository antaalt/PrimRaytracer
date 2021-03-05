#pragma once
#include <string>
#include "Scene.h"

#include <Aka/OS/FileSystem.h>

namespace prim {

class Loader {
public:
	Loader() {}
	virtual ~Loader() {}

	// Load the file and append it into the scene
	virtual bool load(const aka::Path& path, Scene &scene) = 0;
};

}


