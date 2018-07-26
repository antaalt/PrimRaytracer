#pragma once

#include "Config.h"
#include "Tracer.h"

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
			int pos[2];
			int wheel[2];
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


	class Renderer
	{
	public:
		Renderer();
		Renderer(unsigned int width, unsigned int height);
		~Renderer();

		bool loadScene(std::string path);

		void inputs(Inputs &inputs);

		bool init();

		bool render(const RayTracer::Tracer &tracer);

	private:
		RayTracer::Scene m_scene;
		RayTracer::Tracer::Ptr m_tracer;
		unsigned int m_width, m_height;
	};
}