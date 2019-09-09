#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include "Program.h"
#include "GlobalPropertyManager.h"
#include "ResourceManager.h"
#include "../Utils/log.h"
#include "../Utils/Helper.h"
#include "../Graphics/window.h"
#include "../Graphics/camera.h"
#include "../Graphics/renderer.h"
#include "../Graphics/shader.h"
#include "../Graphics/IACamera.h"
#include "../Graphics/texture.h"
#include "../OpenCL/GPU_Device.h"
#include "../OpenCL/CL_Helper.h"
#include "Marching_Cubes_data.h"

namespace Program {

std::unique_ptr<float[]> density_values;
std::unique_ptr<glm::vec4[]> case_values;
float bSIZE = 0.1f;
int x_dim = 32;
int y_dim = 16;
int z_dim = 32;
//int cube_now = (Program::x_dim * Program::y_dim * Program::z_dim * 64) / 3;//0;
int cube_now = (32 * 16 * 32 * 64) / 3;//0;
//float cube_float = float(x_dim * y_dim * z_dim * 64 / 3);;
glm::vec4 bPOS = glm::vec4(-5.2f,-3.0f,-5.2f,0.0f);
int v0_amount = 0.0f;// THE_Tool;
int v1_amount = 0.0f;
int v2_amount = 0.0f;
int v3_amount = 0.0f;
int v4_amount = 0.0f;
int v5_amount = 0.0f;
int v6_amount = 0.0f;
int v7_amount = 0.0f;

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::initialize()
{
  Log::getDebug().log("INITIALIZING PROGRAM.\n");
//  IAC::InteractiveCamera cam;

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

//  auto vb = ResourceManager::getInstance()->create<Vertexbuffer>("uvs");
//  vb->init(GL_ARRAY_BUFFER,GL_STATIC_DRAW);
//  std::vector<std::string> types = {"4f"};
//  std::vector<glm::vec4> uvs;
//  int sw = ic->get_screenWidth();
//  int sh = ic->get_screenHeight();
//  Log::getDebug().log(" yeah "); //sw == %, sh == %", sw, sh);
//  for (int x=0; x<sw; x++) {
//  for (int y=0; y<sh; y++) {
//    uvs.push_back(glm::vec4(float(x)/float(sw),float(y)/float(sh),0.0f,0.0f));
//  }};

//  vb->addData(&uvs[0],sizeof(glm::vec4)*uvs.size(),types);
//  vb->setCount(uvs.size());
  return true;
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
  Camera camera = Camera(glm::vec3(5.0f,2.0f,0.0f),glm::vec3(0.0f,10.0f,-10.0f),glm::vec3(0.0f,1.0f,0.0f));

  // Register q for stoppin the application.
  auto id1 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) { running = !c->isKeyReleased('q'); });
  registerHandlers();

  // Our state (imgui)
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop.
  while (running) {
    ic->pollEvents();

    camera.handleKeyInput();
    Window::getInstance()->swapBuffers();
    //rayTrace(camera.getPosition(), camera.getTarget(), glm::vec3(0.0f,1.0f,0.0f)); 
    renderer.render(camera);
    Window::getInstance()->renderImgui();
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

  // Property for showing the density values.  
  BoolProperty show_density;
  show_density.set(false);
  glob_manager->add("show_density",show_density);

  // Property for showing the scene values.  
  BoolProperty show_scene;
  show_scene.set(true);
  glob_manager->add("show_scene",show_scene);
}

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::createTextures()
{
  Log::getDebug().log("CREATING TEXTURES.\n");
  ResourceManager* rm = ResourceManager::getInstance();
//  std::vector<std::string> texture_names;
//  texture_names.push_back("textures/grass.png");
//  texture_names.push_back("textures/mountain.jpg");
  Texture* t = rm->create<Texture>("ruohikko");
  t->init(TextureType::d2);
  t->create("textures/grass.png",0);
  //t->use(0);
//  t->create(texture_names);

  Texture* t2 = rm->create<Texture>("kallio");
  t2->init(TextureType::d2);
  t2->create("textures/mountain.jpg",1);
  //t2->use(1);
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

  Shader* cube_wire = res_manager->create<Shader>("cube_wire");
  std::vector<std::string> src_wire = {"shaders/cube_wire.vert", "shaders/cube_wire.geom", "shaders/cube_wire.frag"};
  cube_wire->build(src_wire);

  Shader* density_points = res_manager->create<Shader>("density_shader");
  std::vector<std::string> src_density_points = {"shaders/density_points.vert", "shaders/density_points.geom", "shaders/density_points.frag"};
  density_points->build(src_density_points);

  Shader* raymarch_shader = res_manager->create<Shader>("march_shader");
  std::vector<std::string> raymarch_shader_src = {"shaders/raymarch.vert",  "shaders/raymarch.frag"};
  raymarch_shader->build(raymarch_shader_src);

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

void MainProgram::updateScene()
{
  Marching_Cubes_Data mcd;
  int lkm = 0;
  std::unique_ptr<glm::vec4[]> result  = mcd.create("mc_program",
                                                x_dim,
                                                y_dim,
                                                z_dim,
                                                bSIZE,
                                                0.0f,
                                                bPOS,
                                                &lkm);

  // Create the vertex buffer for opengl and populate the marching cubes data.
  ResourceManager::getInstance()->del<Vertexbuffer>("hah");
  auto vb = ResourceManager::getInstance()->create<Vertexbuffer>("hah");
  vb->init(GL_ARRAY_BUFFER,GL_STATIC_DRAW);
  std::vector<std::string> types = {"4f","4f","4f"};
  vb->addData(result.get(),sizeof(glm::vec4)*lkm,types);
  vb->setCount(lkm/3);
  auto vb2 = ResourceManager::getInstance()->get<Vertexbuffer>("density_points");
  vb2->populate_data(Program::density_values.get(),sizeof(float)*Program::x_dim * Program::y_dim * Program::z_dim * 64);

}

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::createOpenCl()
{
  Log::getDebug().log("CREATING OPENCL DEVICE/CONTEXT. RUN THE MARCHING CUBES ALGORITHM.\n");
  auto d = GPU_Device::getInstance();
  if (!d->init()) return false;

  // Load the sources for marching cubes program.
  cl::Program::Sources sources;
  std::string src_mc = Helper::loadSource("shaders/mc.cl"); 
  std::string src_eval = Helper::loadSource("shaders/evalDencity.cl"); 
  sources.push_back({src_mc.c_str(),src_mc.length()});
  sources.push_back({src_eval.c_str(),src_eval.length()});

  // Create the program.
  d->createProgram("mc_program", sources);

  // Run the marching cubes algoritm an get the result of it.
  Marching_Cubes_Data mcd;
  int lkm = 0;
  std::unique_ptr<glm::vec4[]> result  = mcd.create("mc_program",
                                                x_dim,
                                                y_dim,
                                                z_dim,
                                                bSIZE,
                                                0.0f,
                                                bPOS,
                                                &lkm);

  // Create the vertex buffer for opengl and populate the marching cubes data.
  auto vb = ResourceManager::getInstance()->create<Vertexbuffer>("hah");
  vb->init(GL_ARRAY_BUFFER,GL_STATIC_DRAW);
  std::vector<std::string> types = {"4f","4f","4f"};
  vb->addData(result.get(),sizeof(glm::vec4)*lkm,types);
  vb->setCount(lkm/3);

  auto vb2 = ResourceManager::getInstance()->create<Vertexbuffer>("hopohopo");
  vb2->init(GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);
  float hopohopo_data[1] = {float(Program::cube_now)};
  std::vector<std::string> types2 = {"1f"};
  vb2->addData(&hopohopo_data,sizeof(float),types2);
  vb2->setCount(1);

  auto vb3 = ResourceManager::getInstance()->create<Vertexbuffer>("density_points");
  vb3->init(GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);
  std::vector<std::string> types3 = {"1f"};
  vb3->addData(Program::density_values.get(),sizeof(float)*Program::x_dim * Program::y_dim * Program::z_dim * 64,types3);
  vb3->setCount(Program::x_dim * Program::y_dim * Program::z_dim * 64);
//  for (int i=0 ; i<lkm ; i++) {
//    Log::getDebug().log("% . cube-case == % ",i, Program::case_values.get()[i]);
//  }

  cl::Program::Sources raySources;
  std::string src_ray = Helper::loadSource("shaders/rayTrace.cl"); 
  sources.push_back({src_ray.c_str(),src_ray.length()});

  d->createProgram("rayTrace", raySources);
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::registerHandlers()
{
  
  auto ic = InputCache::getInstance();
  auto id2 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[](const InputCache* c) {
         if (!c->isKeyReleased('f')) return; 
         auto glob_manager = GlobalPropertyManager::getInstance();
         auto property = glob_manager->get<BoolProperty>("show_density");
         property->set(!property->get());
      });

  auto show_scene_handler = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[](const InputCache* c) {
         if (!c->isKeyReleased('g')) return; 
         auto glob_manager = GlobalPropertyManager::getInstance();
         auto show_scene = glob_manager->get<BoolProperty>("show_scene");
         show_scene->set(!show_scene->get());
      });

  auto id3 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) {
         int temp;
         auto win = Window::getInstance();
         std::string window_title;

         bool title_changed = false;

         if (c->isKeyPressed('n'))
         {
           temp = Program::cube_now + 4*Program::x_dim; 
           if (temp >= 64*Program::x_dim * Program::y_dim * Program::z_dim) {}
           else {
             Program::cube_now = temp;
             glm::vec4 joop = case_values[temp];
             window_title = vec_toString(joop) + " : " + std::to_string(temp);
             title_changed = true;
           }
         } 
         if (c->isKeyPressed('m'))
         {
           temp = Program::cube_now - 4*Program::x_dim; 
           if (temp < 0) {}
           else {
             Program::cube_now = temp;
             glm::vec4 joop = case_values[temp];
             window_title = vec_toString(joop) + " : " + std::to_string(temp);
             title_changed = true;
           }
         } 
         if (c->isKeyPressed('k'))
         {
           temp = Program::cube_now - 1; 
           if (temp < 0) {}
           else {
             Program::cube_now = temp;
             glm::vec4 joop = case_values[temp];
             window_title = vec_toString(joop) + " : " + std::to_string(temp);
             title_changed = true;
           }
         } 
         if (c->isKeyPressed('h'))
         {
           temp = Program::cube_now + 1; 
           if (temp >= 64*Program::x_dim * Program::y_dim * Program::z_dim) {}
           else {
             Program::cube_now = temp;
             glm::vec4 joop = case_values[temp];
             window_title = vec_toString(joop) + " : " + std::to_string(temp);
             title_changed = true;
           }
         } 
         if (c->isKeyPressed('u'))
         {
           temp = Program::cube_now + 16*Program::x_dim * Program::y_dim; 
           if (temp >= 64*Program::x_dim * Program::y_dim * Program::z_dim) {}
           else {
             Program::cube_now = temp;
             glm::vec4 joop = case_values[temp];
             window_title = vec_toString(joop) + " : " + std::to_string(temp);
             title_changed = true;
           }
         }
         if (c->isKeyPressed('j'))
         {
           temp = Program::cube_now - 16*Program::x_dim * Program::y_dim; 
           if (temp < 0) {}
           else {
             Program::cube_now = temp;
             glm::vec4 joop = case_values[temp];
             window_title = vec_toString(joop) + " : " + std::to_string(temp);
             title_changed = true;
           }
         } 

         if (c->isKeyPressed('1')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v0_amount -= 1; else v0_amount += 1; title_changed = true; }
         if (c->isKeyPressed('2')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v1_amount -= 1; else v1_amount += 1; title_changed = true; }
         if (c->isKeyPressed('3')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v2_amount -= 1; else v2_amount += 1; title_changed = true; }
         if (c->isKeyPressed('4')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v3_amount -= 1; else v3_amount += 1; title_changed = true; }
         if (c->isKeyPressed('5')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v4_amount -= 1; else v4_amount += 1; title_changed = true; }
         if (c->isKeyPressed('6')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v5_amount -= 1; else v5_amount += 1; title_changed = true; }
         if (c->isKeyPressed('7')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v6_amount -= 1; else v6_amount += 1; title_changed = true; }
         if (c->isKeyPressed('8')) { if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) v7_amount -= 1; else v7_amount += 1; title_changed = true; }

         if (c->isKeyPressed('9'))
         { 
           float kerroin = 1.0f;
           if (c->isKeyDown(SDL_SCANCODE_LSHIFT)) kerroin = -1.0f; 
           density_values[cube_now + 1]                                        += v0_amount * 0.1f * kerroin; // v0
           density_values[cube_now]                                            += v3_amount * 0.1f * kerroin; // v3 
           density_values[Program::x_dim*4 + cube_now]                         += v2_amount * 0.1f * kerroin; // v1 
           density_values[Program::x_dim*4 + cube_now + 1]                     += v1_amount * 0.1f * kerroin; // v2 
           density_values[cube_now + Program::x_dim * Program::y_dim*16 + 1]   += v4_amount * 0.1f * kerroin; // v4
           density_values[cube_now + Program::x_dim * Program::y_dim*16 + Program::x_dim * 4]       += v6_amount * 0.1f * kerroin; // v6
           density_values[cube_now + Program::x_dim * Program::y_dim*16 + Program::x_dim * 4 + 1]   += v5_amount * 0.1f * kerroin; // v6
           density_values[cube_now + Program::x_dim * Program::y_dim*16 ]      += v7_amount * 0.1f * kerroin; // v7
           this->updateScene();
         } 
//         } 

//        v5                        v6
//         +------------------------+
//        /|                       /|
//       / |                      / |
//      /  |                     /  |
//     /   |                    /   |  
//    /    |                   /    |
//v1 +------------------------+ v2  |
//   |     |                  |     |
//   |     |                  |     |
//   |     |                  |     |
//   |  v4 +------------------|-----+ v7
//   |    /                   |    /
//   |   /                    |   /
//   |  /                     |  /    
//   | /                      | /
//   |/                       |/
//   +------------------------+
//  v0                       v3

         if (c->isKeyPressed('0'))
         {
           density_values[cube_now + 1]                                        = v0_amount * 0.1f; // v0
           density_values[cube_now]                                            = v3_amount * 0.1f; // v3 
           density_values[Program::x_dim*4 + cube_now]                         = v2_amount * 0.1f; // v1 
           density_values[Program::x_dim*4 + cube_now + 1]                     = v1_amount * 0.1f; // v2 
           density_values[cube_now + Program::x_dim * Program::y_dim*16 + 1]   = v4_amount * 0.1f; // v4
           density_values[cube_now + Program::x_dim * Program::y_dim*16 + Program::x_dim * 4]       = v6_amount * 0.1f; // v6
           density_values[cube_now + Program::x_dim * Program::y_dim*16 + Program::x_dim * 4 + 1]   = v5_amount * 0.1f; // v6
           density_values[cube_now + Program::x_dim * Program::y_dim*16 ]      = v7_amount * 0.1f; // v7
           this->updateScene();
         } 

         if (c->isKeyPressed('r'))
         { 
           float* fPtr = Program::density_values.get();
           for (int i=0; i<Program::x_dim * Program::y_dim * Program::z_dim*64; ++i) fPtr[i] = 1.0f; 
//           memset(Program::density_values.get(), 0.0f, sizeof(float)*Program::x_dim * Program::y_dim * Program::z_dim*64);
           auto vb2 = ResourceManager::getInstance()->get<Vertexbuffer>("density_points");
           vb2->populate_data(Program::density_values.get(),sizeof(float)*Program::x_dim * Program::y_dim * Program::z_dim * 64);
           this->updateScene();
         } 

         if (title_changed) {
            std::string final_title = window_title
                + " (v0:" + std::to_string(Program::v0_amount*0.1f) + "," +            
                + "v1:"   + std::to_string(Program::v1_amount*0.1f) + "," +            
                + "v2:"   + std::to_string(Program::v2_amount*0.1f) + "," +            
                + "v3:"   + std::to_string(Program::v3_amount*0.1f) + "," +            
                + "v4:"   + std::to_string(Program::v4_amount*0.1f) + "," +            
                + "v5:"   + std::to_string(Program::v5_amount*0.1f) + "," +            
                + "v6:"   + std::to_string(Program::v6_amount*0.1f) + "," +            
                + "v7:"   + std::to_string(Program::v7_amount*0.1f) + ")";
            win->setTitle(final_title.c_str());            
         }


//         if (c->isKeyPressed('0'))
//         {
//           density_values[int(cube_float) + 1] += 1.0f;
//           density_values[int(cube_float)] += 1.0f;
//           density_values[int(Program::x_dim*4 + cube_float)] += 1.0f;
//           density_values[int(Program::x_dim*4 + cube_float + 1)] += 1.0f;
//           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] += 1.0f;
//           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] += 1.0f;
//           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] += 1.0f;
//           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] += 1.0f;
//           this->updateScene();
//         } 

         auto vb = ResourceManager::getInstance()->get<Vertexbuffer>("hopohopo");
         //Log::getDebug().log("INDEX == %", temp);
         
         float hopohopo_data[1] = {float(Program::cube_now)};
         vb->populate_data(&hopohopo_data,sizeof(float));
      });
}

/////////////////////////////////////////////////////////////////////////////////////

void MainProgram::rayTrace(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up ) {

  //const static glm::ivec3 LOCAL_GROUP_SIZE(4,4,4);
  //glm::ivec3 global_group_size(dimensionX, dimensionY, dimensionZ);
  GPU_Device* d = GPU_Device::getInstance();

  cl::CommandQueue* command = d->get<cl::CommandQueue>("command_ray");
  if (d == nullptr) command = d->create<cl::CommandQueue>("command_ray");

// struct RayCamera {
// 	glm::vec3 position;
// 	glm::vec3 view;
// 	glm::vec3 up;
// 	glm::vec2 resolution;
// 	glm::vec2 fov;		
// 	float apertureRadius;
// 	float focalDistance;
// };
  // Create the camera.

  RayCamera r;
  r.position = pos;
  r.view = target;
  r.up = up;
  r.resolution;
  r.fov = glm::vec2(45.0f,45.0f);

  cl::Buffer* rayCamera = d->get<cl::Buffer>("rayCamera");

  if (rayCamera == nullptr) rayCamera = d->createBuffer("rayCamera", sizeof(RayCamera), CL_MEM_READ_WRITE);
	command->enqueueWriteBuffer(*rayCamera, CL_TRUE, 0, sizeof(RayCamera), &r);

  auto ic = InputCache::getInstance();

  int sw = ic->get_screenWidth();
  int sh = ic->get_screenHeight();

  // The global range. Screen size.
  cl::NDRange global(sw, sh, 1);
  cl::NDRange local(8, 8, 1);

  int theSIZE = sw * sh;

  cl::Buffer* theScreen = d->get<cl::Buffer>("theScreen");
  if (theScreen == nullptr) d->createBuffer("theScreen", sizeof(glm::vec3)*theSIZE, CL_MEM_READ_WRITE);

  // TODO: check if this exists.
  cl::Buffer* density_output = d->get<cl::Buffer>("density_values");
  assert(density_output != nullptr);

  cl_int error = CL_SUCCESS;

  // THE KERNEL CREATION.

  // THE INDICES for both evalDensity and mc.
  cl::EnqueueArgs eargs(*command, global, local);

  cl::Program* program = d->get<cl::Program>("rayTrace");
  assert(program != nullptr);

//__kernel void render_kernel(const int width, const int height, __global float3* output, __constant const Camera* cam)
  cl::make_kernel<int, int, cl::Buffer, cl::Buffer> ray_kernel(*program,"render_kernel",&error);
  if (error != CL_SUCCESS) print_cl_error(error);

  // THE EXECUTION.

  ray_kernel(eargs, sw, sh, *theScreen, *rayCamera).wait();

//  float eval_result[theSIZE];
//  error = command->enqueueReadBuffer(*density_output,CL_TRUE,0,sizeof(float)*theSIZE,eval_result);
//
//  for (int i=0; i<theSIZE; i++)
//  {
//     Log::getDebug().log("i == % : % ", i, eval_result[i]);
//  }

  // The marching cubes output count. The total count of the float4 values
  // (vvvv nnnn vvvv nnnn vvvv nnn.....) vvvv :: float4, nnnn :: float4
////  int lkm[1] = {0};
////  error = command->enqueueReadBuffer(*counter,CL_TRUE,0,sizeof(int),lkm);
////  if (error != CL_SUCCESS) { print_cl_error(error); }
////  
////  Log::getDebug().log("lkm[0] == %", lkm[0]);
////  
////  // Create the array for maching cubes output.
////  auto result = std::make_unique<glm::vec4[]>(lkm[0]);
////
////  // Create the array for maching cubes output.
////  if (!update) {
////    auto d_values = std::make_unique<float[]>(theSIZE);
////    error = command->enqueueReadBuffer(*density_output,CL_TRUE,0,sizeof(float)*theSIZE, d_values.get());
////    if (error != CL_SUCCESS) { print_cl_error(error); }
////    Program::density_values = std::move(d_values);
////  }
////
////  auto c_values = std::make_unique<glm::vec4[]>(theSIZE);
////  error = command->enqueueReadBuffer(*cubecase_output,CL_TRUE,0,sizeof(glm::vec4)*theSIZE, c_values.get());
////  if (error != CL_SUCCESS) { print_cl_error(error); }
////  Program::case_values = std::move(c_values);
////
////  // Copy the result of marching cubes.
////  error = command->enqueueReadBuffer(*mc_output,CL_TRUE,0,sizeof(glm::vec4)*lkm[0], result.get());
////  if (error != CL_SUCCESS) { print_cl_error(error); }
////
////  //for (int i=0; i< lkm[0]; i++)
////  //{
////  //   Log::getDebug().log("i == % : % ", i, c_values.get()[i]); // result.get()[i]);
////  //}
////
//////  for (int i=0; i< lkm[0]; i++)
//////  {
//////     Log::getDebug().log("i == % : % ", i, Program::density_values.get()[i]); // result.get()[i]);
//////  }
////
////  // Store the output count of marching cubes. This is the count of all float4
////  // created in marching cubes shader. We need so we can store and draw 
////  // the mc output with opengl.
////  *total_count = lkm[0];
////
////  return std::move(result);
}

/////////////////////////////////////////////////////////////////////////////////////

} // namespace Program
