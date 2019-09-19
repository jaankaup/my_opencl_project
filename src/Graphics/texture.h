#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include <GL/glew.h>
#include <SOIL.h>
#include "textureData.h"
#include "TextureSampler.h"
#include "../Utils/log.h"
#include "../Utils/myrandom.h"

enum class TextureType {d1,d2,d3};

class Texture
{
      friend class ResourceManager;
    public:

        /// Default constructors.
        Texture();
        /// Uses the texture object. Texture object must be initialized 
        /// before the use method. @param unit is the texture unit to use.
        void use(const GLuint unit) const;

        TextureType getTextureType() const;
        void create(const std::string& fileloc, int level);
        void create(const std::vector<std::string>& textures);
        void create3D(const TextureData& td);
        void create_tritable_texture();
        std::tuple<std::unique_ptr<float[]>,unsigned int> getTextureData();

        /// Initializes the texture object.
        void init(const TextureType t);

        void init(const TextureType t, const uint32_t width, const uint32_t height, const uint32_t depth);

        /// Binds the texture. Texture object must be initialized 
        /// before the use method.
        void bind() const;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&&) = default;
        Texture& operator=(Texture&&) = default;

        /// The destructor doesn't release the texture. Use dispose()
        /// memberfuction to destory texture object.
        ~Texture();

    private:

        /// Handle to the gl texture.
        GLuint pId = 0;

        /// Texture type.
        TextureType pType;

        /// Current texture unit.
        ///GLuint pUnit;

        bool setWidth(const uint32_t w);
        bool setHeight(const uint32_t h);
        bool setDepth(const uint32_t d);

        glm::vec3 pTextureDimension;

        unsigned int pTextureSize = 0;

        std::unique_ptr<TextureSampler> pSampler;

        /// Releases the texture object.
        //void dispose() const;
};

#endif // TEXTURE_H
