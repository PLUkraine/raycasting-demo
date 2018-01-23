#ifndef TEXTURE_H
#define TEXTURE_H

#include <assert.h>
#include "glad/glad.h"

namespace core
{
    
	class Texture
	{
	private:
		const static int COLORS = 3;

		GLubyte *m_data;
		int m_height;
		int m_width;
		GLuint m_glTex;
	public:
		const static int RED = 0;
		const static int GREEN = 1;
		const static int BLUE = 2;

	public:
		// initialize empty texture
		Texture();
		Texture(const Texture&) = delete;
		~Texture();
		void dispose();

		inline int width() const
		{
			return m_width;
		}
		inline int height() const
		{
			return m_height;
		}
		inline int colors() const
		{
			return COLORS;
		}

		// create OpenGL texture and fill it with black color
		void createGlTexture(int width, int height);
		// fill local buffer with black color
		void clearTexture() const;
		// load from local buffer to Video card RAM
		void loadToVRAM() const;
		// bind texture as current 2d texture
		void bindTexture() const
		{
			glBindTexture(GL_TEXTURE_2D, m_glTex);
		}

		// get one byte from local buffer (read or write)
		inline GLubyte & getData(int x, int y, int color)
		{
			return m_data[y*(m_width*COLORS) + x*COLORS + color];
		}
		// get one byte from local buffer (read or write)
		inline void setPixel(int x, int y, GLubyte r, GLubyte g, GLubyte b)
		{
			GLubyte *p_data = m_data + y*(m_width*COLORS) + x*COLORS;
			*p_data = r;
			*(++p_data) = g;
			*(++p_data) = b;
		}
	};
}

#endif
