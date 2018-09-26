#pragma once
#include "Scene.h"

namespace app {

	class SceneBuilder
	{
	public:
		SceneBuilder();
		~SceneBuilder();

		static bool buildCustomScene(Scene &scene);
		static bool buildCustomSceneTri(Scene &scene);
	};
}

