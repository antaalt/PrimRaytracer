#pragma once

#include "Config.h"
#include "Tracer.h"
#include "Camera.h"
#include "PixelBuffer.h"

#include <string>

#define TILE_WIDTH_NUMBER 20
#define TILE_HEIGHT_NUMBER 20

namespace app {


	namespace tracer {

		struct Tile {
			unsigned int width, height;
			unsigned int x, y;
			bool render(PixelBuffer &buffer);
			Tile(unsigned int w, unsigned int h, unsigned int x, unsigned int y);
		private:
			std::vector<Ray> rays;
		};

		class Renderer
		{
		public:
			Renderer(unsigned int width, unsigned int height);
			~Renderer();

			bool buildScene(Scene &scene, Acceleration acceleration);

			bool updateRays();

			bool renderPreview();

			bool render();

			bool renderParallel();

			void setTracer(tracer::Tracer::Ptr tracer);
			void setCamera(tracer::Camera::Ptr camera);

			const PixelBuffer &image() const;

		private:
			Scene scene;
			Camera::Ptr camera;
			Tracer::Ptr tracer;
			std::vector<Ray> rays;
			Accelerator::Ptr accelerator;
			unsigned int width, height;
			PixelBuffer output;
		};
	}
}