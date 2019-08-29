#ifndef WINDOW_H
#define WINDOW_H

//#include <memory>
//#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "../Program/InputCache.h"

struct SDL_Window;
using SDL_GLContext = void*;

/* A window class using SDL2 window. */
class Window
{

  public:

    static Window* getInstance();

    // Swaps buffers.
    void swapBuffers();

    // Resize method for window.
   // void resize(InputCache* ic);
    void resize(int w, int h);

    void set_vsync(const bool enabled) const;
     
    void setTitle(const std::string& title) const;

    std::string getTitle() const; 

    // Initializes window.
    bool init(int width, int height);
   
    // Doesn't destroy the SDL2 and window. Call dispose to do this.
    ~Window();

  private:

    SDL_Window* pWindow = NULL; 
    SDL_GLContext pContext = NULL; 

    // Creates a window and initializes SDL2.
    Window();


    // Disposes the window. Destroys both window and SDL2.
    void dispose();


}; // Class window

#endif //WINDOW_H
