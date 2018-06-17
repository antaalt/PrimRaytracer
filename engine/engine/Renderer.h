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

	namespace GL {
		class Renderer
		{
		public:
			Renderer();
			Renderer(glm::uvec2 resolution);
			~Renderer();

			bool loadScene(std::string path);

			void inputs(Inputs &inputs);

			bool init();

			bool render();

		private:
			Scene m_scene;
			glm::uvec2 m_resolution;
		};
	}
}