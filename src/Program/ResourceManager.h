#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include "../Graphics/shader.h"
#include "../Graphics/texture.h"
#include "../Graphics/vertexbuffer.h"

class Shader;
class Texture;
class Vertexbuffer;

class ResourceManager
{
  public:

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ~ResourceManager() {}

    /// Get the instance to ResourceManager object.
    static auto* getInstance() { static ResourceManager instance; return &instance; }

    template<typename T>
    T* create(const std::string& key, T value)
    {
      if constexpr (std::is_same<T,Shader>::value) { Shader s; pShaders[key] = s; }
      if constexpr (std::is_same<T,Texture>::value) {Texture t; pTextures[key] = t; }
      if constexpr (std::is_same<T,Vertexbuffer>::value) { Vertexbuffer vb; pVertexBuffers[key] = vb; }
    }

    template<typename T>
    T* del(const std::string& key)
    {
      if constexpr (std::is_same<T,Shader>::value) { pShaders.erase(key); }
      if constexpr (std::is_same<T,Texture>::value) {pTextures.erase(key); }
      if constexpr (std::is_same<T,Vertexbuffer>::value) { pVertexBuffers.erase(key);}
    }

    template<typename T>
    T* get(const std::string& key)
    {
      if constexpr (std::is_same<T,Shader>::value) { auto f =  pShaders.find(key); if (f != pShaders.end()) return &f->second; else return nullptr; }
      if constexpr (std::is_same<T,Texture>::value) {auto f =  pTextures.find(key); if (f != pTextures.end()) return &f->second; else return nullptr; }
      if constexpr (std::is_same<T,Vertexbuffer>::value) {auto f =  pVertexBuffers.find(key); if (f != pVertexBuffers.end()) return &f->second; else return nullptr; }
    }

  private:

    ResourceManager() {}

    std::unordered_map<std::string,Shader> pShaders;
    std::unordered_map<std::string,Texture> pTextures;
    std::unordered_map<std::string,Vertexbuffer> pVertexBuffers;

};

#endif // RESOURCEMANAGER_H
