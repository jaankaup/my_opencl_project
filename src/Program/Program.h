#ifndef PROGRAM_H
#define PROGRAM_H

namespace Program {

class MainProgram
{
  public:

    void initialize();
    void start();

  private:

    void createGlobalProperties();
    void createTextures();
    void createShaders();
    void createWindow();
    void createOpencl();
    void registerHandlers();

};

} // namespace Program

#endif
