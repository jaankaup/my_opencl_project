#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace Program {


const static std::string DEFAULT_RENDERING_SHADER = "default_shader";

extern std::unique_ptr<float[]> density_values;
extern std::unique_ptr<glm::vec4[]> case_values;
extern int cube_now;
extern float cube_float;
extern float bSIZE;
extern int x_dim;
extern int y_dim;
extern int z_dim;
extern glm::vec4 bPOS;

/**
 * The main program.
 */
class MainProgram
{
  public:

    /**
     * An initialization member function.
     * This member function initializes all the resources that are needed for
     * running the application.
     * @return the result of the initialization.
     */
    bool initialize();

    /**
     * A method for running the program.
     * Initialize method must be called succesfully before calling this method.
     */
    void start();

  private:

    /** 
     * This method creates most of the global variables for the program.
     */
    void createGlobalProperties();

    /**
     * This methdod creates the initial textures for the program.
     * @param return Returns true if textures was initialized, false
     * otherwise.
     */
    bool createTextures();

    /**
     * This method creates the initial shaders for the program.
     * @param return Returns true if shaders was initialized, false
     * otherwise.
     */
    bool createShaders();

    /**
     * This method creates the main window for the system.
     * @param return Returns true if window created succesfully, false
     * otherwise.
     */
    bool createWindow();

    /**
     * Creates a GPU_Device object for application.
     * @param return Returns true if opencl was initialized, false
     * otherwise.
     */
    bool createOpenCl();

    /**
     * This method creates some ininital event handler labmda-functions 
     * for the application.
     */
    void registerHandlers();
};

} // namespace Program

#endif
