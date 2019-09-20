#ifndef TEXTURE_SAMPLER_H
#define TEXTURE_SAMPLER_H

#include <GL/glew.h>

class TextureSampler
{
    public:

        TextureSampler() { init(); }

        void init() {
          glGenSamplers(1, &pId);
        }

        void dispose() {
          glGenSamplers(1, &pId);
        }

        void set_raytracing_params() {
          glTexParameteri(pId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(pId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri(pId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri(pId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        GLuint getID() { return pId; }

        ~TextureSampler() { dispose(); }

    private:

        GLuint pId = 0;

};

#endif // TEXTURE_SAMPLER_H

