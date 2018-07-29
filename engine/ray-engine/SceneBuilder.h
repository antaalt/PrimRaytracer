#pragma once
#include "Scene.h"

namespace app {

	class SceneBuilder
	{
	public:
		SceneBuilder();
		~SceneBuilder();

		static Scene buildCustomScene();
	};
}

