#include "FBO.h"

#include <iostream>

namespace Application {
	namespace GL {
		FBO::FBO() : FBO(glm::ivec2(DEFAULT_WIDTH, DEFAULT_HEIGHT))
		{
		}

		FBO::FBO(glm::ivec2 p_resolution) :
			m_resolution(p_resolution),
			m_status(GL_FRAMEBUFFER_UNDEFINED),
			m_id(0),
			m_renderBuffer(0)
		{
			// CHECK EXTENSION WITH GLEW 
			if (!glewIsExtensionSupported("GL_ARB_framebuffer_object"))
				return;

			// Create and attach FBO
			glGenFramebuffers(1, &m_id);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
			// Create and attach a color buffer
			glGenRenderbuffers(1, &m_renderBuffer);
			// We must bind m_renderBuffer before we call glRenderbufferStorage
			glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
			// The storage format is RGBA8
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, m_resolution.x, m_resolution.y);
			// Attach color buffer to FBO
			glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBuffer);

			m_status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
			doNotUse();
			if (!isValid()) {
				std::cout << "Error while checking frame buffer status : " << m_status << std::endl;
			}
		}


		FBO::~FBO()
		{
			if (m_renderBuffer)
				glDeleteRenderbuffers(1, &m_renderBuffer);

			if (m_id)
				glDeleteFramebuffers(1, &m_id);
		}

		void FBO::use()
		{
			// Read buffer is created FBO
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
			// Draw buffer is default FBO, the one used for display
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		void FBO::doNotUse()
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		bool FBO::isValid()
		{
			return (m_status == GL_FRAMEBUFFER_COMPLETE);
		}

		GLuint FBO::getID()
		{
			return m_id;
		}
	}
}