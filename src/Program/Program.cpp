#include "Program.h"
#include "GlobalPropertyManager.h"
#include "ResourceManager.h"
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

  // Create the resource manager.
  ResourceManager::getInstance();
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

////////////////////////////////////////////////////////////////////////////////////

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

  Log::getDebug().log("AHHAAAAA");
  int i[10] = {1,2,3,4,5,6,7,8,9,10};

  const static int X = 2;
  const static int Y = 2;
  const static int Z = 2;
  const static float ISOVALUE = 0.0f;

  std::vector<glm::vec3> base_points;

  Log::getDebug().log("YHHYYYYYYY");
  base_points.reserve(X*Y*Z*3);

  Log::getDebug().log("EHHEEEEEEE");
//  for (int x=0; x<X; x++) {
//  for (int y=0; y<Y; y++) {
//  for (int z=0; y<Z; z++) {
//    base_points.push_back(glm::vec3(x,y,z));
//  }}};

  Log::getDebug().log("JUPPPPI");
  int c[1] = {0};

  // Create a buffers.

  Log::getDebug().log("AAA");
  CL_Buffer counter;
  if (!counter.create(d, CL_MEM_READ_WRITE, sizeof(int))) Log::getError().log("Failed to create counter buffer."); 

  Log::getDebug().log("BBB");
  CL_Buffer b_points;
  if (!b_points.create(d, CL_MEM_READ_WRITE,  sizeof(glm::vec3)*X*Y*Z)) Log::getError().log("Failed to create base_points buffer."); 

  Log::getDebug().log("CCC");
  CL_Buffer output;
  if (!output.create(d, CL_MEM_READ_WRITE, sizeof(glm::vec3)*X*Y*Z*10)) Log::getError().log("Failed to create output buffer."); 

  Log::getDebug().log("DDD");
  b_points.addData(d,true, &base_points[0] , sizeof(glm::vec3)*X*Y*Z);
  counter.addData(d,true, &c , sizeof(int));

  // Load the source code.
  cl::Program::Sources sources;
  std::string kernel_code = Helper::loadSource("shaders/mc.cl"); 
  sources.push_back({kernel_code.c_str(),kernel_code.length()});
  //sources.push_back(kernel_code);

  // Create a program
  Log::getDebug().log("Creating the program.");
  CL_Program program;
  if (!program.create(d,sources,"mc")) Log::getError().log("Failed to create the program.");

//__kernel void mc(__global float* base_points, __global const float isovalue, __global float* output,  __global int n,  __global int* counterArg){       
  Log::getDebug().log("Setting parameters.");
  program.setArg(0,*(b_points.getBuffer()));
  program.setArg(1,ISOVALUE);
  program.setArg(2,*(output.getBuffer()));
  program.setArg(3,X*Y*Z);
  program.setArg(4,*(counter.getBuffer()));

  Log::getDebug().log("Get the dimensions.");
  auto global_dim = d->getGlobalDim(X*Y*Z);
  auto local_dim = d->getLocalDim();

  auto global_dims = global_dim.dimensions();
//  size_t x_global;
//  size_t y_global;
//  size_t z_global;
//  global_dim();
  //Log::getDebug().log("global_dim = %",global_dims);
  
  Log::getDebug().log("Run the kernel.");

  //cl::EnqueueArgs eargs(*(d->getCommandQueue()), cl::NullRange, cl::NDRange(10), cl::NullRange);
  //*(program.getKernel())(eargs, a,b,c,10).wait();
  d->runKernel(&program, global_dim, local_dim);// local_dim);
  //d->runKernel(&program, /* global_dim */ cl::NDRange(5), cl::NullRange);// local_dim);

  int lkm[1];
  glm::vec3 bee[X*Y*Z];

  counter.getData(d,true,&lkm, sizeof(int));
  Log::getDebug().log("LKM == %", lkm[0]);

  output.getData(d,true,&bee, sizeof(glm::vec3)*X*Y*Z);

  for (int i=0; i<X*Y*Z ; i++)
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
