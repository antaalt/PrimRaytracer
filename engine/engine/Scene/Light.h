#pragma once

#include "Node.h"

namespace Application {

	class Light : public Node
	{
	public:
		Light();
		~Light();

	//private:
		float intensity;
		glm::vec4 emission;
		glm::vec4 color;
	};

}