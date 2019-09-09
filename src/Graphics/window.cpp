#include <stdexcept>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
//#include <SDL2/SDL.h>
#include "window.h"
#include "../Utils/log.h"
#include "../Utils/Helper.h"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{

  std::string the_source;
  switch (source) {
    case GL_DEBUG_SOURCE_API : the_source = "DEBUG_SOURCE_API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM : the_source = "DEBUG_SOURCE_WINDOW_SYSTEM"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER : the_source = "DEBUG_SOURCE_SHADER_COMPILER"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY : the_source = "DEBUG_SOURCE_THIRD_PARTY"; break;
    case GL_DEBUG_SOURCE_APPLICATION : the_source = "DEBUG_SOURCE_APPLICATION"; break;
    case GL_DEBUG_SOURCE_OTHER : the_source = "DEBUG_SOURCE_OTHER"; break;
    default: the_source = "UNKNOW_SOURCE!!!";
  }

  std::string the_type;
  switch (type) {
    case GL_DEBUG_TYPE_ERROR : the_type = "DEBUG_TYPE_ERROR"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR : the_type = "DEBUG_TYPE_DEPRECATED_BEHAVIOR"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR : the_type = "DEBUG_TYPE_UNDEFINED_BEHAVIOR";break;
    case GL_DEBUG_TYPE_PORTABILITY : the_type = "DEBUG_TYPE_PORTABILITY";break;
    case GL_DEBUG_TYPE_PERFORMANCE : the_type = "DEBUG_TYPE_PERFORMANCE";break;
    case GL_DEBUG_TYPE_MARKER : the_type = "DEBUG_TYPE_MARKER";break;
    case GL_DEBUG_TYPE_PUSH_GROUP : the_type = "DEBUG_TYPE_PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP : the_type = "DEBUG_TYPE_POP_GROUP";break;
    case GL_DEBUG_TYPE_OTHER : the_type = "DEBUG_TYPE_OTHER";break;
    default: the_type = "UNKNOW_TYPE!!!";
  }

  std::string the_severity;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH : the_severity = "DEBUG_SEVERITY_HIGH"; break;
    case GL_DEBUG_SEVERITY_MEDIUM : the_severity = "DEBUG_SEVERITY_MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW : the_severity = "DEBUG_SEVERITY_LOW"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION : the_severity = "DEBUG_SEVERITY_NOTIFICATION"; break;
    default: the_type = "UNKNOW_SEVERITY!!!";
  }
  if (severity == GL_DEBUG_SEVERITY_HIGH) {
    Log::getError().log("GL CALLBACK: source == %, type == %, id == %, severity == %, length ==  %, message == %, userParam == %)",
        the_source, the_type, id, the_severity, length, message, userParam);
  }
  else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
    Log::getWarning().log("GL CALLBACK: source == %, type == %, id == %, severity == %, length ==  %, message == %, userParam == %)",
        the_source, the_type, id, the_severity, length, message, userParam);
  }
  else if (severity == GL_DEBUG_SEVERITY_LOW) {
    Log::getDebug().log("GL CALLBACK: source == %, type == %, id == %, severity == %, length ==  %, message == %, userParam == %)",
        the_source, the_type, id, the_severity, length, message, userParam);  
  }
  else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
    Log::getInfo().log("GL CALLBACK: source == %, type == %, id == %, severity == %, length ==  %, message == %, userParam == %)",
        the_source, the_type, id, the_severity, length, message, userParam);  
    }
}

Window* Window::getInstance()
{
    static Window instance;
    return &instance;
}

Window::Window()
{
    //init(1280,720);
}

Window::~Window()
{
  dispose();
}

void Window::dispose()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  if (pContext != NULL) { SDL_GL_DeleteContext(pContext); }
  if (pWindow != NULL) { SDL_DestroyWindow(pWindow); }

  SDL_Quit(); 
}

bool Window::init(int width, int height)
{
  bool result = Helper::initSDL();
  if (!result) return false;

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);


    /* Luodaan opengl conteksti ikkunalle. */
    pContext = SDL_GL_CreateContext(pWindow);
    if (pContext == NULL) {
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

        // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(pWindow, pContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);


    // Register the resize event function.
    InputCache* ic = InputCache::getInstance();
    glViewport(0, 0, width, height);
    ic->register_lambda_function(EventType::RESIZE_EVENT,
                                 [&](const InputCache* c) {  
                                 int w = c->get_screenWidth(); 
                                 int h = c->get_screenHeight();
                                 SDL_SetWindowSize(pWindow, w, h);
                                 glViewport(0, 0, w, h);
                                 });

    //glEnable              ( GL_DEBUG_OUTPUT );
    //glDebugMessageCallback( MessageCallback, 0 );

    return true;
}

void Window::swapBuffers()
{
  SDL_GL_SwapWindow(pWindow);
}

void Window::renderImgui()
{
      // Our state
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
          // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(pWindow);
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

         // Rendering
        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //SDL_GL_SwapWindow(pWindow);

}

void Window::set_vsync(const bool enabled) const
{
  if (SDL_GL_SetSwapInterval(enabled ? 1 : 0) < 0) Log::getWarning().log("Window::set_vsync failed: %", SDL_GetError());
}

void Window::setTitle(const std::string& title) const 
{
  if (pWindow != nullptr) SDL_SetWindowTitle(pWindow, title.c_str());
}

std::string Window::getTitle() const 
{
  if (pWindow != nullptr) SDL_GetWindowTitle(pWindow);
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
