#include "Enum.h"

void * app::offset(Attributes att)
{
	switch (att)
	{
	case Attributes::POSITION:
		return 0;
	case Attributes::NORMAL:
		return (void*)(sizeof(glm::vec3));
	case Attributes::COLOR:
		return (void*)(2 * sizeof(glm::vec3));
	case Attributes::TEXCOORD0:
		return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4));
	case Attributes::TEXCOORD1:
		return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec2));
	case Attributes::TEXCOORD2:
		return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4) + 2 * sizeof(glm::vec2));
	case Attributes::TEXCOORD3:
		return (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec4) + 3 * sizeof(glm::vec2));
	default:
		return 0;
	}
}
