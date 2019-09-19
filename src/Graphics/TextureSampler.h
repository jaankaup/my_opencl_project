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

        GLuint getID() { return pId; }

        ~TextureSampler() { dispose(); }

    private:

        GLuint pId = 0;

};

#endif // TEXTURE_SAMPLER_H

