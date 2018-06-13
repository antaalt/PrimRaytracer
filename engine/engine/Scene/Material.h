#pragma once

#include "../GLProgram.h"

// PBR material
// @see https://www.allegorithmic.com/pbr-guide
namespace World {

	class Material
	{
	public:
		Material();
		~Material();

		void setProgram(Application::GLProgram *glProgram);
		Application::GLProgram *getProgram();

	private:
		Application::GLProgram *m_program;
	};

}