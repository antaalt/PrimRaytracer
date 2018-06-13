#pragma once

#include "Scene\Scene.h"

#include <string>

namespace Application {

	enum KeyPosition {
		RIGHT,
		LEFT,
		MIDDLE
	};
	struct Inputs {
		struct Mouse {
			bool mouse[3];
			glm::ivec2 pos;
			glm::ivec2 wheel;
		} mouse;
		struct KeyBoard {
			bool ctrl[2];
			bool alt[2];
			bool shift[2];
			bool space;
			bool escape;
			bool printScreen;
		} keyboard;
	};

	class GLRenderer
	{
	public:
		GLRenderer();
		GLRenderer(glm::uvec2 resolution);
		~GLRenderer();

		bool loadScene(std::string path);

		void inputs(Inputs &inputs);

		bool init();

		bool render();

	private:
		World::Scene m_scene;
		glm::uvec2 m_resolution;
	};

}