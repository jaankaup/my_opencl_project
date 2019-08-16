#include "Program.h"
#include "GlobalPropertyManager.h"
#include "ResourceManager.h"
#include "../Utils/log.h"
#include "../Utils/Helper.h"
#include "../Graphics/window.h"
#include "../Graphics/camera.h"
#include "../Graphics/renderer.h"
#include "../Graphics/shader.h"
#include "../OpenCL/GPU_Device.h"
#include "../OpenCL/CL_Program.h"
#include "../OpenCL/CL_Buffer.h"
#include "../OpenCL/CL_Helper.h"

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
  if (!createOpenCl()) { Log::getError().log("Failed to create opencl."); return false; }
  if (!createShaders()) { Log::getError().log("Failed to create shaders."); return false; }
  if (!createTextures()) { Log::getError().log("Failed to create textures."); return false; } 
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
    renderer.render(camera);
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

bool MainProgram::createTextures()
{
  Log::getDebug().log("CREATING TEXTURES.\n");
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::createShaders()
{
  Log::getDebug().log("CREATING SHADERS.\n");

  auto glob_manager = GlobalPropertyManager::getInstance();
  auto res_manager = ResourceManager::getInstance();

  /* The default rendering shader. vvvnnn. */
  Shader* default_shader = res_manager->create<Shader>(DEFAULT_RENDERING_SHADER);
  std::vector<std::string> src = {"shaders/default_notex.vert", "shaders/default_notex.frag"};
  default_shader->build(src);

  return true;
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

  const static int X_DIMENSION = 25;
  const static int Y_DIMENSION = 25;
  const static int Z_DIMENSION = 25;
  const static int TOTAL_SIZE = (X_DIMENSION + 2) * (Y_DIMENSION + 2) * (Z_DIMENSION + 2);
  const static float ISOVALUE = 0.0f;
  const static float CUBE_SIZE_LENGTH = 0.1f;
  const static int OUTPUT_SIZE = TOTAL_SIZE*16;

  //std::vector<cl_float3> base_points;
  //std::vector<cl_float3> base_points;
  std::vector<float> base_points;

  base_points.reserve(TOTAL_SIZE);

//  for (int x= -1; x<X_DIMENSION + 1; x++) {
//  for (int y= -1; y<Y_DIMENSION + 1; y++) {
//  for (int z= -1; z<Z_DIMENSION + 1; z++) {
//    base_points.push_back({float(x)*CUBE_SIZE_LENGTH,float(y)*CUBE_SIZE_LENGTH,float(z)*CUBE_SIZE_LENGTH});
//  }}};

  for (int i=0 ; i<TOTAL_SIZE; i++) 
  {
    base_points.push_back(float(i));
  }

  int c[] = {3};
  cl::Buffer b_Points(*(d->getContext()),CL_MEM_READ_WRITE, sizeof(cl_float)*TOTAL_SIZE);
  cl::Buffer mc_output(*(d->getContext()),CL_MEM_READ_WRITE, sizeof(float)*OUTPUT_SIZE);
  cl::Buffer counter(*(d->getContext()),CL_MEM_READ_WRITE, sizeof(int));

  auto b_PointsPtr = d->create("b_pointsit", b_Points);
  //auto b_valuesPtr = d->create("b_arvot", b_values);

  cl::CommandQueue* c_Queue = d->create<cl::CommandQueue>("mc1");
  //cl::CommandQueue* c_Queue_pass2 = d->create<cl::CommandQueue>("mc2");

  int error = c_Queue->enqueueWriteBuffer(*b_PointsPtr,CL_TRUE,0,sizeof(float)*TOTAL_SIZE,&base_points[0]);
  Log::getDebug().log("error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));

  error = c_Queue->enqueueWriteBuffer(counter,CL_TRUE,0,sizeof(int),&c);
  Log::getDebug().log("error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));

  // Load and compile the source code for evalDensity.cl and mc.cl.
  cl::Program::Sources sources;
  std::string kernel_code = Helper::loadSource("shaders/evalDencity.cl"); 
  sources.push_back({kernel_code.c_str(),kernel_code.length()});
  std::string mc_kernel_code = Helper::loadSource("shaders/mc.cl"); 
  sources.push_back({mc_kernel_code.c_str(),mc_kernel_code.length()});

  // Load and compile the source code for mc.cl.
  //cl::Program::Sources sources_mc;

  // Create a program for evalDensity.
  Log::getDebug().log("Creating the evalDencity program.");
  CL_Program program;
  if (!program.create(d,sources,"eval_density")) Log::getError().log("Failed to create the program.");

  Log::getDebug().log("Get the dimensions.");
  auto global_dim = d->getGlobalDim(TOTAL_SIZE);
  auto local_dim = d->getLocalDim();

  // EVAL_DENSITY
  Log::getDebug().log("Creating kernel and setting arguments.");
  cl::make_kernel<cl::Buffer, int, int, int, float, cl_float4, int> evalDensity_kernel(*program.getProgram(),"eval_density",&error);
  Log::getDebug().log("Creating eval_density :: error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));
  cl::EnqueueArgs eargs(*c_Queue, cl::NullRange, global_dim, local_dim);

  cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, int, int, int, float, float, int> mc_kernel(*program.getProgram(),"mc");

  Log::getDebug().log("Evaluating evalDensity.");
  cl_float4 base_pos = {0.0f,0.0f,0.0f,0.0f};
  evalDensity_kernel(eargs, *b_PointsPtr,X_DIMENSION,Y_DIMENSION,Z_DIMENSION,0.1f,base_pos,TOTAL_SIZE); //.getInfo(CL_EVENT_COMMAND_QUEUE, *c_Queue));

//  float eval_result[TOTAL_SIZE];
//  error = c_Queue->enqueueReadBuffer(*b_PointsPtr,CL_TRUE,0,sizeof(float)*TOTAL_SIZE,eval_result);

//  for (int i=0; i<TOTAL_SIZE; i++)
//  {
//     Log::getDebug().log("i == % : % ", i, eval_result[i]);
//  }

  // MC
  Log::getDebug().log("Creating MC kernel and setting arguments.");
  //cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer, int, int, int, float, float, int> mc_kernel(*program.getProgram(),"mc");

  cl::EnqueueArgs eargs2(*c_Queue, cl::NullRange, global_dim, local_dim);

  Log::getDebug().log("Evaluating MC!.");
  mc_kernel(eargs2, *b_PointsPtr,mc_output, counter, X_DIMENSION,Y_DIMENSION,Z_DIMENSION,0.1f,0.0f,TOTAL_SIZE).wait(); //.getInfo(CL_EVENT_COMMAND_QUEUE, *c_Queue));
  Log::getDebug().log("Evaluating MC done!.");

  int lkm[1] = {6};
  error = c_Queue->enqueueReadBuffer(counter,CL_TRUE,0,sizeof(int),lkm);
  Log::getDebug().log("error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));

  float bee[OUTPUT_SIZE];
  error = c_Queue->enqueueReadBuffer(mc_output,CL_TRUE,0,sizeof(glm::vec4)*(lkm[0]+1),bee);
  Log::getDebug().log("error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));


  auto vb = ResourceManager::getInstance()->create<Vertexbuffer>("hah");
  vb->init(GL_ARRAY_BUFFER,GL_STATIC_DRAW);
  std::vector<std::string> types = {"4f","4f"};
  vb->addData(&bee,sizeof(glm::vec4)*(lkm[0]+1),types);
  vb->setCount((lkm[0]+1)/2);
  Log::getError().log("lkm == %",lkm[0]+1);

  for (int i=0; i<(lkm[0]+1)/2; i++)
  {
     int off = i*8;
     glm::vec4 pos(bee[off],bee[off+1],bee[off+2],bee[off+3]);
     glm::vec4 pos1(bee[off+4],bee[off+5],bee[off+6],bee[off+7]);
     Log::getDebug().log("i == % : % ", i, pos);
     Log::getDebug().log("i == % : % ", i, pos1);
     Log::getDebug().log("*******************************");
  }

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::registerHandlers()
{

}

/////////////////////////////////////////////////////////////////////////////////////

} // namespace Program
