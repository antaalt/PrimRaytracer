#pragma once

#include "Config.h"

namespace Application {

	enum Attributes {
		POSITION,
		NORMAL,
		/*TANGENT,
		BITANGENT,*/
		COLOR,
		TEXCOORD0,
		TEXCOORD1,
		TEXCOORD2,
		TEXCOORD3,
		NB_ATTRIBUTES
	};

	void* offset(Attributes att);

}