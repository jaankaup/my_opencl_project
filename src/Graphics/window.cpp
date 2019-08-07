#include <stdexcept>
//#include <SDL2/SDL.h>
#include "window.h"
#include "../Utils/log.h"
#include "../Utils/Helper.h"

Window& Window::getInstance()
{
    static Window instance;
    return instance;
}

Window::Window()
{
    init(1280,720);
}

Window::~Window()
{
  dispose();
}

void Window::dispose()
{
  if (pContext != NULL) { SDL_GL_DeleteContext(pContext); }
  if (pWindow != NULL) { SDL_DestroyWindow(pWindow); }
  SDL_Quit(); 
}

bool Window::init(int width, int height)
{
  bool result = Helper::initSDL();
  if (!result) return false;

  Log::getDebug().log("%","Window::Window. window created.");
    int x = SDL_WINDOWPOS_UNDEFINED;
    int y = SDL_WINDOWPOS_UNDEFINED;
    int flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
//    int flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
    pWindow = SDL_CreateWindow("", x, y, width, height, flags);

    if (pWindow == NULL)
    {
      std::string error = SDL_GetError();
      Log::getError().log("%.%","Window::init. Failed to create SDL_Window",error);
      return false;
    }


    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


    /* Luodaan opengl conteksti ikkunalle. */
    pContext = SDL_GL_CreateContext(pWindow);
    if (pContext == NULL)
    {
        std::string error = SDL_GetError();
        Log::getError().log("%.%","Window::init. Failed to create SDL_GLContext.",error);
        Log::getError().log("Perhaps your hardware doesn't support this opengl version.");
    }

    glewExperimental = GL_TRUE;


    GLenum error = glewInit();
    if (error != GLEW_NO_ERROR) {
        Log::getError().log("%","Window::init. Failed to initialize glew.");
        return false;
     }

    set_vsync(true);

    Log::getInfo().log("GL_Version: %", glGetString(GL_VERSION));
    Log::getInfo().log("Vendor: %", glGetString(GL_VENDOR));
    Log::getInfo().log("Renderer: %", glGetString(GL_RENDERER));
    SDL_GL_SetSwapInterval(true);

    // Register the resize event function.
    InputCache* ic = InputCache::getInstance();
    glViewport(0, 0, ic->get_screenWidth(), ic->get_screenHeight());
    ic->register_lambda_function(EventType::RESIZE_EVENT,
                                 [&](const InputCache* c) {  
                                 int w = ic->get_screenWidth(); 
                                 int h = ic->get_screenHeight();
                                 Log::getDebug().log("Resizing (%,%).",w,h);
                                 glViewport(0, 0, w, h);
                                 });
    return true;
}

void Window::swapBuffers()
{
  SDL_GL_SwapWindow(pWindow);
}

void Window::set_vsync(const bool enabled) const
{
  if (SDL_GL_SetSwapInterval(enabled ? 1 : 0) < 0) Log::getWarning().log("Window::set_vsync failed: %", SDL_GetError());
}

void Window::setTitle(const std::string& title) const 
{
  if (pWindow != nullptr) SDL_SetWindowTitle(pWindow, title.c_str());
}

//void Window::resize(InputCache* ic)
void Window::resize(int w, int h)
{
//    int w = ic->get_screenWidth(); 
//    int h = ic->get_screenHeight()  
    SDL_SetWindowSize(pWindow, w, h);
    glViewport(0, 0, w, h);

//    SDL_Event e;
//    e.type = SDL_WINDOWEVENT;
//    e.window.event = SDL_WINDOWEVENT_RESIZED;
//    e.window.windowID = SDL_GetWindowID(pWindow);
//    e.window.data1 = w;
//    e.window.data2 = h;
//    SDL_PushEvent(&e);
}
