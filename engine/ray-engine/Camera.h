#pragma once
#include "Config.h"

namespace Application::RayTracer {
	class Camera
	{
	public:
		Camera();
		~Camera();

	private:
		Matrix4 m_transform;
		unsigned int m_width, m_height;
		float m_fov;
	};
}
