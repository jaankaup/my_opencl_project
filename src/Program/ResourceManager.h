#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include "../Graphics/shader.h"
#include "../Graphics/texture.h"
#include "../Graphics/vertexbuffer.h"

class Shader;
class Texture;
class Vertexbuffer;

/**
 * A class for storing resources. Now Shader, Texture and Vertexbuffer are
 * supported.
 */
class ResourceManager
{
  public:

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ~ResourceManager() {}

    static auto* getInstance() { static ResourceManager instance; return &instance; }

    /**
     * Create a new resource with a given key. TODO: solve what happens if the
     * resource already exists.
     * @param key The hash key.
     * @return A pointer to the created resource.
     */
    template<typename T>
    T* create(const std::string& key)
    {
      if constexpr (std::is_same<T,Shader>::value) { Shader s; pShaders[key] = std::move(s); auto sh = &pShaders[key]; sh->init(); return sh; }
      if constexpr (std::is_same<T,Texture>::value) {pTextures[key] = Texture();}
      if constexpr (std::is_same<T,Vertexbuffer>::value) { Vertexbuffer vb; pVertexBuffers[key] = vb; return &pVertexBuffers[key];}
    }

    /**
     * Delete resource with a given key. 
     * @param key The hash key to the resource.
     */
    template<typename T>
    void del(const std::string& key)
    {
      if constexpr (std::is_same<T,Shader>::value) { pShaders.erase(key); }
      if constexpr (std::is_same<T,Texture>::value) {pTextures.erase(key); }
      if constexpr (std::is_same<T,Vertexbuffer>::value) { pVertexBuffers.erase(key);}
    }

    /**
     * Get a resource with a given key. 
     * @param key The hash key to the resource.
     * @param return A pointer to the resource of nullptr is resource can not
     * be found.
     */
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
