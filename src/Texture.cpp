#include "Texture.h"

#include "string.h"

namespace core
{
	Texture::Texture()
		: m_data(nullptr),
		m_height(0),
		m_width(0),
		m_glTex(0)
	{
	}

	Texture::~Texture()
	{
		dispose();
	}

	void Texture::dispose()
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
		}
		if (m_glTex != 0)
		{
			glDeleteTextures(1, &m_glTex);
			m_glTex = 0;
		}
	}

	// create OpenGL texture and fill it with black color

	void Texture::createGlTexture(int width, int height)
	{
		// make sure everything is deleted prior to this call
		assert(m_glTex == 0);
		assert(m_data == nullptr);
		// make sure width and height are positive
		assert(height > 0);
		assert(width > 0);
		// assign new width and height
		m_width = width;
		m_height = height;

		glGenTextures(1, &m_glTex);
		bindTexture();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// clear texture data and load it into VRAM (VIDEO CARD RAM)
		m_data = new GLubyte[m_width*m_height*COLORS];
		clearTexture();
		loadToVRAM();
	}

	// fill local buffer with black color
	void Texture::clearTexture() const
	{
		memset(m_data, 0, m_width * m_height * COLORS);
	}

	// load from local buffer to Video card RAM

	void Texture::loadToVRAM() const
	{
		bindTexture();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	}
}