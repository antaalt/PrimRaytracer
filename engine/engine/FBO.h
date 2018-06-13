#pragma once

#include "Config.h"

class FBO
{
private:
	glm::ivec2 m_resolution;	// Resolution of the FBO
	GLuint m_renderBuffer;		// RenderBuffer linked to the FBO
	GLuint m_id;				// ID of the FBO
	GLenum m_status;			// status of the FBO
public:
	FBO();
	FBO(glm::ivec2 p_resolution);
	~FBO();

	void use();

	void doNotUse();

	bool isValid();

	GLuint getID();
};

