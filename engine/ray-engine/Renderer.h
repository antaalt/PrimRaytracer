#pragma once

#include "Config.h"
#include "Tracer.h"
#include "Camera.h"
#include "PixelBuffer.h"

#include <string>

#define TILE_WIDTH_NUMBER 30
#define TILE_HEIGHT_NUMBER 30

namespace app {

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

	namespace tracer {

		class Renderer
		{
		public:
			Renderer(unsigned int width, unsigned int height);
			~Renderer();

			bool loadScene(std::string path, Acceleration acceleration);

			void inputs(Inputs &inputs);

			bool init();

			bool updateRays();

			bool renderPreview();

			bool render();

			void setTracer(tracer::Tracer::Ptr tracer);
			void setCamera(tracer::Camera::Ptr camera);

		private:
			Scene scene;
			Camera::Ptr camera;
			Tracer::Ptr tracer;
			std::vector<tracer::Ray> rays;
			Accelerator::Ptr accelerator;
			unsigned int width, height;
			PixelBuffer output;
		};
	}
}