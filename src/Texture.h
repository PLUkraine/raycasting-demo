#ifndef TEXTURE_H
#define TEXTURE_H

#include <assert.h>
#include "glad/glad.h"

namespace core
{
    
    class ImageBase
    {
    protected:
        int m_width, m_height, m_colors;
        GLubyte *m_data;
        
    public:
        ImageBase();
        virtual ~ImageBase() {}
        virtual void dispose()=0;
        
        inline GLubyte& getData(int x, int y, int color)
        {
            return m_data[y*(m_width*m_colors) + x*m_colors + color];
        }
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
            return m_colors;
        }
        
    };
    
    class Image : public ImageBase
    {
    public:
        const static int RED = 0;
        const static int GREEN = 1;
        const static int BLUE = 2;
        
    public:
        ~Image();
        
        void loadFromFile(const char *file);
        void dispose();
    };
    
    class Texture : public ImageBase
    {
    private:
        const static int COLORS = 3;
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
        
        // create OpenGL texture and fill it with black color
        void createGlTexture(int width, int height);
        // fill local buffer with black color
        void clearTexture() const;
        // load from local buffer to Video card RAM
        void loadToVRAM() const;
        
        // bind texture as current 2d texture
        inline void bindTexture() const
        {
            glBindTexture(GL_TEXTURE_2D, m_glTex);
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
