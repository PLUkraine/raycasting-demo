#include "ImageRenderer.h"

namespace core
{
	ImageRenderer::ImageRenderer()
	{
		float vertexData[] = {
			// vertex		texture
			-1, -1, 0,		0, 0,
			-1,  1, 0,		0, 1,
			 1, -1, 0,		1, 0,

			 1, -1, 0,		1, 0,
			-1,  1, 0,		0, 1,
			 1,  1, 0,		1, 1,
		};

		glGenVertexArrays(1, &m_quadVAO);
		glGenBuffers(1, &m_quadVBO);

		glBindVertexArray(m_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// vertex array
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// UV array
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}


	ImageRenderer::~ImageRenderer()
	{
		dispose();
	}
	void ImageRenderer::dispose()
	{
		if (m_quadVAO != 0)
		{
			glDeleteVertexArrays(1, &m_quadVAO);
			m_quadVAO = 0;
		}
		if (m_quadVBO != 0)
		{
			glDeleteBuffers(1, &m_quadVBO);
			m_quadVBO = 0;
		}
	}
	void ImageRenderer::render(Texture *tex, Shader *shader)
	{
		tex->bindTexture();
		shader->use();
        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(m_quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}
