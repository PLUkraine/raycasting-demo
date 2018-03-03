#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include "glad/glad.h"
#include "learnopengl/shader.h"
#include "Texture.h"

namespace core
{
    class ImageRenderer
    {
    private:
        GLuint m_quadVAO;
        GLuint m_quadVBO;
    public:
        ImageRenderer();
        ~ImageRenderer();
        
        void dispose();
        void render(Texture *tex, Shader *shader);
    };
}

#endif
