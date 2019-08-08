#ifndef PROGRAM_H
#define PROGRAM_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace Program {

class MainProgram
{
  public:

    bool initialize();
    void start();

  private:

    void createGlobalProperties();
    void createTextures();
    void createShaders();
    bool createWindow();
    void createOpencl();
    void registerHandlers();

};

} // namespace Program

#endif
