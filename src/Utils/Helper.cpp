#include "Helper.h"
#include "log.h"

namespace Helper {

std::string loadSource(const std::string& fileLoc)
{
    std::ifstream file(fileLoc);

    if (!file.is_open())
    {
      Log::getError().log("Shader::loadSource(%): Failed to load source.","fileLoc");
      throw std::runtime_error("Shader::loadSource: counld'n open file '" + fileLoc + "'.");
    }

    std::string str;
    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    return str;
}

bool initSDL() 
{
  uint32_t sub_systems = SDL_WasInit(SDL_INIT_EVERYTHING);

  // Video is already initialized. ok.
  if (sub_systems & SDL_INIT_VIDEO) { Log::getDebug().log("Helper::initSDL: SDL video already initialized."); return true; }

  // If initialization fails...
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    Log::getError().log("Helper::initSDL: Failed to initialize SDL.");
    Log::getError().log("Reason: %",SDL_GetError());
    return false;
  }

  // This is not necessary.
  return true;
}

};
