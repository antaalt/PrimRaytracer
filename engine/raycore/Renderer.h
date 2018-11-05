#pragma once

#include "Config.h"
#include "Tracer.h"
#include "Camera.h"
#include "PixelBuffer.h"

#include <string>

#define TILE_WIDTH_NUMBER 20
#define TILE_HEIGHT_NUMBER 20
#define PARALLEL_RENDERING

namespace raycore {

	namespace tracer {

		struct index2D {
			index2D(unsigned int x, unsigned int y) : x(x), y(y) {}
			index2D operator-(const index2D &rhs) { return index2D(x - rhs.x, y - rhs.y); }
			index2D operator+(const index2D &rhs) { return index2D(x + rhs.x, y + rhs.y); }
			index2D operator*(float scalar) { return index2D(static_cast<unsigned int>(x * scalar), static_cast<unsigned int>(y * scalar)); }
			unsigned int x, y;
		};

		struct Tile {
			Tile(const index2D &min, const index2D &max): min(min), max(max) {}

			unsigned int width() { return max.x - min.x; }
			unsigned int height() { return max.y - min.y; }
			index2D center() { return (max - min) * 0.5f; }

			index2D min;
			index2D max;
		};

		struct Settings {
			unsigned int tileSize;
			unsigned int samplesX;
			unsigned int samplesY;
			RaySampler raySamplerX;
			RaySampler raySamplerY;
		};

		class Renderer
		{
		public:
			Renderer(unsigned int width, unsigned int height, const Settings &settings);
			~Renderer();
			Renderer(const Renderer& other) = delete;
			Renderer& operator=(const Renderer &other) = delete;

			bool buildScene(prim::Scene &&scene, prim::Acceleration acceleration);

			bool updateRays();

			bool renderPreview();

			bool render();

			void resize(unsigned int width, unsigned int height);

			void buildTiles(unsigned int tileSize);

			void setTracer(tracer::Tracer* tracer);
			void setCamera(tracer::Camera* camera);

			const PixelBuffer &image() const;

		private:
			Camera* camera;
			Tracer* tracer; // TODO add a secondary quick tracer (whitted for example) for display
			prim::Scene scene;

			std::vector<Tile> tiles;
			unsigned int tileSize;
			unsigned int width, height;
			unsigned int samples;
			unsigned int subSamplesX, subSamplesY;
			RaySampler raySamplerX, raySamplerY;
			PixelBuffer output;
		};
	}
}