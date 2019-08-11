#include "Program.h"
#include "GlobalPropertyManager.h"
#include "../Utils/log.h"
#include "../Utils/Helper.h"
#include "../Graphics/window.h"
#include "../Graphics/camera.h"
#include "../Graphics/renderer.h"
#include "../OpenCL/GPU_Device.h"
#include "../OpenCL/CL_Program.h"
#include "../OpenCL/CL_Buffer.h"

namespace Program {

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::initialize()
{
  Log::getDebug().log("INITIALIZING PROGRAM.\n");

  // Initialize InputCache.
  auto ic = InputCache::getInstance();
  ic->init();

  createGlobalProperties();

  if (!createWindow()) { Log::getError().log("Failed to create window."); return false; }
  createOpenCl();
  createShaders();
  createTextures();
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::start()
{
  bool running = true;

  auto ic = InputCache::getInstance();
  ic->init();

  // Create the renderer.
  Renderer renderer;
  renderer.init();

  // Create the camera for this application.
  Camera camera = Camera(glm::vec3(0.0f,0.5f,1.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

  // Register q for stoppin the application.
  auto id1 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) { running = !c->isKeyReleased('q'); });

  // Main loop.
  while (running) {
    ic->pollEvents();
    camera.handleKeyInput();
    Window::getInstance()->swapBuffers();
    //renderer.render(camera);
  };
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::createGlobalProperties()
{
  Log::getDebug().log("CREATING GLOBAL PROPERTIES.\n");
  auto glob_manager = GlobalPropertyManager::getInstance();

  // The name for defaut_rendering shader.
  StringProperty s;
  s.set("default_shader");
  glob_manager->add("DEFAULT_SHADER",s);

  // Screen size filters.
  auto width_filter  = [](int x) {if (x < 0 || x > 1920) {Log::getError().log("Screen width % is not supported.",x); return false;} else return true; };
  auto height_filter = [](int x) {if (x < 0 || x > 1080) {Log::getError().log("Screen height % is not supported.",x); return false;} else return true; };

  // Initial screend width.
  IntProperty initial_screen_width;
  initial_screen_width.set(1280);
  initial_screen_width.registerTest(width_filter);
  glob_manager->add("initial_screen_width",initial_screen_width);

  // Initial screend height.
  IntProperty initial_screen_height;
  initial_screen_height.set(720);
  initial_screen_height.registerTest(height_filter);
  glob_manager->add("initial_screen_height",initial_screen_height);

}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::createTextures()
{
  Log::getDebug().log("CREATING TEXTURES.\n");

}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::createShaders()
{
  Log::getDebug().log("CREATING SHADERS.\n");

}

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::createWindow()
{
  Log::getDebug().log("CREATING WINDOW.\n");
  // Initialize window/sdl/opengl context.
  Window* window = Window::getInstance();
  auto glob_manager = GlobalPropertyManager::getInstance();
  auto w = glob_manager->get<IntProperty>("initial_screen_width");
  auto h = glob_manager->get<IntProperty>("initial_screen_height");

  if (w == nullptr || h == nullptr || !window->init(w->get(),h->get())) return false;
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::createOpenCl()
{
  Log::getDebug().log("CREATING OPENCL DEVICE/CONTEXT.\n");
  auto d = GPU_Device::getInstance();
  if (!d->init()) return false;

  int i[10] = {1,2,3,4,5,6,7,8,9,10};

  // Create a buffer.
  CL_Buffer b;
  if (!b.create(d, CL_MEM_READ_WRITE, sizeof(int)*10)) Log::getError().log("Failed to create buffer."); 

  // Add data to the buffer.
  b.addData(&i, sizeof(int)*10);

  // Load the source code.
  cl::Program::Sources sources;
  std::string kernel_code = Helper::loadSource("shaders/example.cl"); 
  sources.push_back({kernel_code.c_str(),kernel_code.length()});
  //sources.push_back(kernel_code);

  // Create a program
  Log::getDebug().log("Creating the program.");
  CL_Program program;
  if (!program.create(d,sources,"add_one")) Log::getError().log("Failed to create the program.");

  // Set parameter to the first argument.
  Log::getDebug().log("Setting parameters.");
  Log::getDebug().log("Parameter 0.");
  program.setArg(0,*(b.getBuffer()));
  Log::getDebug().log("Parameter 1.");
  program.setArg(1,10);

  Log::getDebug().log("Get the dimensions.");
  auto global_dim = d->getGlobalDim(10);
  auto local_dim = d->getLocalDim();
  
  Log::getDebug().log("Run the kernel.");
  d->runKernel(&program, global_dim, local_dim);

  int bee[10];

  b.getData(true,&bee, sizeof(int)*10);

  for (int i=0; i<10 ; i++)
  {
    Log::getDebug().log("%", bee[i]);
  }
//bool CL_Buffer::getData(const bool blocking, void* dest_buffer, size_t size)

//  CL_Program program;  
//
//  cl::Program::Sources sources;
//
//  std::string kernel_code = Helper::loadSource("shaders/mc.cl"); 
//  sources.push_back({kernel_code.c_str(),kernel_code.length()});
//
//  program.create(d, sources);
//  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::registerHandlers()
{

}

/////////////////////////////////////////////////////////////////////////////////////

} // namespace Program
