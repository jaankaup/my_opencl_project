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

  const static int X_DIMENSION = 5;
  const static int Y_DIMENSION = 5;
  const static int Z_DIMENSION = 5;
  const static int TOTAL_SIZE = (X_DIMENSION + 2) * (Y_DIMENSION + 2) * (Z_DIMENSION + 2);
  const static float ISOVALUE = 0.0f;
  const static float CUBE_SIZE_LENGTH = 0.1f;

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
  cl::Buffer b_Points(*(d->getContext()),CL_MEM_READ_WRITE, sizeof(cl_float)*TOTAL_SIZE);
  cl::Buffer b_values(*(d->getContext()),CL_MEM_READ_WRITE, sizeof(cl_float4)*TOTAL_SIZE);

  auto b_PointsPtr = d->create("b_pointsit", b_Points);
  auto b_valuesPtr = d->create("b_arvot", b_values);

  cl::CommandQueue* c_Queue = d->create<cl::CommandQueue>("mc1");
  int error = c_Queue->enqueueWriteBuffer(*b_PointsPtr,CL_TRUE,0,sizeof(float)*TOTAL_SIZE,&base_points[0]);
  Log::getDebug().log("error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));

  // Load the source code.
  cl::Program::Sources sources;
  std::string kernel_code = Helper::loadSource("shaders/evalDencity.cl"); 
  sources.push_back({kernel_code.c_str(),kernel_code.length()});

  // Create a program
  Log::getDebug().log("Creating the program.");
  CL_Program program;
  if (!program.create(d,sources,"mc")) Log::getError().log("Failed to create the program.");

  Log::getDebug().log("Get the dimensions.");
  auto global_dim = d->getGlobalDim(TOTAL_SIZE);
  auto local_dim = d->getLocalDim();

//__kernel void eval_density(__global float* base_values,
//                                    int x_dimension,
//                                    int y_dimension,
//                                    int z_dimension,
//                                    float block_size,
//                                    float3 start_point,
//                                    int n)

  Log::getDebug().log("Creating kernel and setting arguments.");
  cl::make_kernel<cl::Buffer, cl::Buffer, int, int, int, float, cl_float4, int> evalDensity_kernel(cl::Kernel(*(program.getProgram()),"eval_density"));
  //cl::make_kernel<cl::Buffer, cl::Buffer, int> evalDensity_kernel(cl::Kernel(*(program.getProgram()),"eval_density"));
  cl::EnqueueArgs eargs(*c_Queue, cl::NullRange, global_dim, local_dim);

  Log::getDebug().log("Evaluating evalDensity.");
  //evalDensity_kernel(eargs, *b_PointsPtr,*b_valuesPtr,TOTAL_SIZE); //.getInfo(CL_EVENT_COMMAND_QUEUE, *c_Queue));
  Log::getDebug().log("sizeof(cl_float4) == %", sizeof(cl_float4));
  cl_float4 base_pos = {0.0f,0.0f,0.0f,0.0f};
  evalDensity_kernel(eargs, *b_PointsPtr,*b_valuesPtr,X_DIMENSION,Y_DIMENSION,Z_DIMENSION,0.1f,base_pos,TOTAL_SIZE); //.getInfo(CL_EVENT_COMMAND_QUEUE, *c_Queue));

  cl_float4 bee[TOTAL_SIZE];
  error = c_Queue->enqueueReadBuffer(*b_valuesPtr,CL_TRUE,0,sizeof(cl_float4)*TOTAL_SIZE,bee);
  Log::getDebug().log("error == CL_SUCCESS => %", error == CL_SUCCESS);
  Log::getError().log("%",errorcode_toString(error));

  for (int i=0; i<TOTAL_SIZE; i++)
  {
     Log::getDebug().log("i == % : (%,%,%,%)",i, bee[i].x, bee[i].y, bee[i].z,bee[i].w);
  }

  return true;
  //int x_offset = X_DIMENSION + 2;
  //int y_offset = (X_DIMENSION + 2) * (Y_DIMENSION + 2);
  //int z_offset = (X_DIMENSION + 2) * (Y_DIMENSION + 2) * (Z_DIMENSION + 2);
  // for (int i=0; i<TOTAL_SIZE ; i++)
  // {
  //   auto x_coord = (i % x_offset) - 1;
  //   auto y_coord = ((i / x_offset) % (Y_DIMENSION + 2)) - 1;
  //   auto z_coord = ((i / y_offset) % (Z_DIMENSION + 2)) - 1;
  //   Log::getDebug().log("f(%,%,%) = : %", x_coord, y_coord, z_coord , bee[i]);
  //}

////
////  // Create a buffers.
////  CL_Buffer counter;
////  if (!counter.create(d, CL_MEM_READ_WRITE, sizeof(int))) Log::getError().log("Failed to create counter buffer."); 
////
////  CL_Buffer b_points;
////  //if (!b_points.create(d, CL_MEM_READ_WRITE, sizeof(cl_float3)*TOTAL_SIZE)) Log::getError().log("Failed to create base_points buffer."); 
////  if (!b_points.create(d, CL_MEM_READ_ONLY, sizeof(cl_float3)*TOTAL_SIZE)) Log::getError().log("Failed to create base_points buffer."); 
////
////  CL_Buffer b_values;
////  if (!b_values.create(d, CL_MEM_READ_WRITE, sizeof(cl_float3)*TOTAL_SIZE)) Log::getError().log("Failed to create base_values buffer."); 
////
////  CL_Buffer output;
////  if (!output.create(d, CL_MEM_READ_WRITE, sizeof(cl_float3)*TOTAL_SIZE)) Log::getError().log("Failed to create output buffer."); 
////
////  b_points.addData(d,true, &base_points[0] , sizeof(cl_float3)*TOTAL_SIZE);
////  counter.addData(d,true, &c , sizeof(int));
////
////  // Load the source code.
////  cl::Program::Sources sources;
////  std::string kernel_code = Helper::loadSource("shaders/mc.cl"); 
////  sources.push_back({kernel_code.c_str(),kernel_code.length()});
////  //sources.push_back(kernel_code);
////
////  // Create a program
////  Log::getDebug().log("Creating the program.");
////  CL_Program program;
////  if (!program.create(d,sources,"mc")) Log::getError().log("Failed to create the program.");
////
//////__kernel void mc(__global float* base_points, __global const float isovalue, __global float* output,  __global int n,  __global int* counterArg){       
////  Log::getDebug().log("Setting parameters.");
////  program.setArg(0,*(b_points.getBuffer()));
////  program.setArg(1,*(b_values.getBuffer()));
////  program.setArg(2,*(output.getBuffer()));
////  program.setArg(3,*(counter.getBuffer()));
////  program.setArg(4,ISOVALUE);
////  program.setArg(5,TOTAL_SIZE);
////
////  Log::getDebug().log("Get the dimensions.");
////  auto global_dim = d->getGlobalDim(TOTAL_SIZE);
////  auto local_dim = d->getLocalDim();
////
////  auto global_dims = global_dim.dimensions();
//////  size_t x_global;
//////  size_t y_global;
//////  size_t z_global;
//////  global_dim();
////  //Log::getDebug().log("global_dim = %",global_dims);
////  
////  Log::getDebug().log("Run the kernel.");
////
////  //cl::EnqueueArgs eargs(*(d->getCommandQueue()), cl::NullRange, cl::NDRange(10), cl::NullRange);
////  //*(program.getKernel())(eargs, a,b,c,10).wait();
////  d->runKernel(&program, global_dim, local_dim);// local_dim);
////  //d->runKernel(&program, /* global_dim */ cl::NDRange(5), cl::NullRange);// local_dim);
////
////  int lkm[1];
////  cl_float3 bee[TOTAL_SIZE];
////
////  counter.getData(d,true,&lkm, sizeof(int));
////  Log::getDebug().log("LKM == %", lkm[0]);
////
////  output.getData(d,true,&bee, sizeof(cl_float3)*TOTAL_SIZE);
////
////  for (int i=0; i<TOTAL_SIZE ; i++)
////  {
////    Log::getDebug().log("%,%,%", bee[i].x, bee[i].y,bee[i].z);
////  }
//////bool CL_Buffer::getData(const bool blocking, void* dest_buffer, size_t size)
////
//////  CL_Program program;  
//////
//////  cl::Program::Sources sources;
//////
//////  std::string kernel_code = Helper::loadSource("shaders/mc.cl"); 
//////  sources.push_back({kernel_code.c_str(),kernel_code.length()});
//////
//////  program.create(d, sources);
//////  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::registerHandlers()
{

}

/////////////////////////////////////////////////////////////////////////////////////

} // namespace Program
