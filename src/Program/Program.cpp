#include <exception>
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
float bSIZE = 0.5f;
int x_dim = 32;
int y_dim = 32;
int z_dim = 32;
//int cube_now = (Program::x_dim * Program::y_dim * Program::z_dim * 64) / 3;//0;
int cube_now = (32 * 16 * 32 * 64) / 3;//0;
//float cube_float = float(x_dim * y_dim * z_dim * 64 / 3);;
glm::vec4 bPOS = glm::vec4(-5.8f,-7.0f,-6.2f,0.0f);
//glm::vec4 bPOS = glm::vec4(-5.2f,-5.0f,-5.2f,0.0f);
//glm::vec4 bPOS = glm::vec4(-1.2f,-1.0f,-1.2f,0.0f);
int v0_amount = 0.0f;// THE_Tool;
int v1_amount = 0.0f;
int v2_amount = 0.0f;
int v3_amount = 0.0f;
int v4_amount = 0.0f;
int v5_amount = 0.0f;
int v6_amount = 0.0f;
int v7_amount = 0.0f;
bool rayCamera = false;

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::initialize()
{
  Log::getDebug().log("INITIALIZING PROGRAM.\n");
//  IAC::InteractiveCamera cam;

  createGlobalProperties();

  // Initialize InputCache.
  auto ic = InputCache::getInstance();
  ic->init();

  if (!createWindow()) { Log::getError().log("Failed to create window."); return false; }

  // Create the resource manager.
  ResourceManager::getInstance();
  if (!createOpenCl()) { Log::getError().log("Failed to create opencl."); return false; }
  if (!createShaders()) { Log::getError().log("Failed to create shaders."); return false; }
  if (!createTextures()) { Log::getError().log("Failed to create textures."); return false; } 

  auto vb = ResourceManager::getInstance()->create<Vertexbuffer>("rayScreen");
  vb->init(GL_ARRAY_BUFFER,GL_DYNAMIC_DRAW);
  //std::vector<std::string> types = {"4f","4f","4f","4f"};
  std::vector<std::string> types = {"4f","4f"};

  // TODO: FIXXXX
  glm::vec2 camera_resolution = GlobalPropertyManager::getInstance()->get<Vec2Property>("camera_resolution")->get();
  auto width = camera_resolution.x; // ic->get_screenWidth();
  auto height = camera_resolution.x; // ic->get_screenHeight();
  auto dummy = std::make_unique<float[]>(width*height);
  vb->addData(dummy.get(),sizeof(float)*width*height,types);
//  std::vector<glm::vec4> uvs;
//  int sw = ic->get_screenWidth();
//  int sh = ic->get_screenHeight();
//  Log::getDebug().log(" yeah "); //sw == %, sh == %", sw, sh);
//  for (int x=0; x<sw; x++) {
//  for (int y=0; y<sh; y++) {
//    uvs.push_back(glm::vec4(float(x)/float(sw),float(y)/float(sh),0.0f,0.0f));
//  }};

//  vb->addData(&uvs[0],sizeof(glm::vec4)*uvs.size(),types);
  vb->setCount(width*height);
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
  Camera camera = Camera(glm::vec3(0.0f,5.0f,3.0f),glm::vec3(0.0f,6.0f,2.0f),glm::vec3(0.0f,1.0f,0.0f));

  // Create the raycamera for this application.
  Camera ray_camera = Camera(glm::vec3(0.0f,5.0f,3.0f),glm::vec3(0.0f,6.0f,2.0f),glm::vec3(0.0f,1.0f,0.0f));
  ray_camera.isRaycamera(true);

  // Register q for stoppin the application.
  auto id1 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) { running = !c->isKeyReleased('q'); });
  registerHandlers();

  // Our state (imgui)
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  //int counter = 0;

  // Main loop.
  while (running) {
    ic->pollEvents();
    camera.handleKeyInput();
    ray_camera.handleKeyInput();
    //if (counter != 0) continue;
    rayTrace(ray_camera.getPosition(), glm::normalize(ray_camera.getFront()), glm::vec3(0.0f,1.0f,0.0f), ray_camera.getFocaldistance()); 
    renderer.render(camera,ray_camera);
    Window::getInstance()->renderImgui();
    Window::getInstance()->swapBuffers();
    //counter++;
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
  initial_screen_width.set(512);
  initial_screen_width.registerTest(width_filter);
  glob_manager->add("initial_screen_width",initial_screen_width);

  // Initial screend height.
  IntProperty initial_screen_height;
  initial_screen_height.set(512);
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

  // Property for ray tracer camera resolution.
  Vec2Property raycamera_resolution; 
  raycamera_resolution.set(glm::vec2(256.0f,256.0f));
  auto resolution_filter  = [](const glm::vec2& res) {if (res.x < 1 || res.y < 1) {Log::getError().log("Raycamera resolution (%) not supported.",res); return false;} else return true; };
  auto resolution_filter2 = [](const glm::vec2& res) {if (int(res.x) % 8 != 0  || int(res.y) % 8 != 0 ) {Log::getError().log("Raycamera resolution (%) must be modulo 8.",res); return false;} else return true; };
  raycamera_resolution.registerTest(resolution_filter);
  raycamera_resolution.registerTest(resolution_filter2);
  glob_manager->add("camera_resolution",raycamera_resolution);
}

/////////////////////////////////////////////////////////////////////////////////////

bool MainProgram::createTextures()
{
  Log::getDebug().log("CREATING TEXTURES.\n");
  ResourceManager* rm = ResourceManager::getInstance();
//  auto glob_manager = GlobalPropertyManager::getInstance();

  glm::vec2 camera_resolution = GlobalPropertyManager::getInstance()->get<Vec2Property>("camera_resolution")->get();
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

  Texture* rayRuutu = rm->create<Texture>("rayRuutu");
  rayRuutu->init(TextureType::d2,camera_resolution.x, camera_resolution.y, 1);
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

  Shader* camera_debug_shader = res_manager->create<Shader>("camera_debug_shader");
  std::vector<std::string> camera_debug_shader_src = {"shaders/camera_debug.vert", "shaders/camera_debug.geom", "shaders/camera_debug.frag"};
  camera_debug_shader->build(camera_debug_shader_src);

  Shader* ray_texture_shader = res_manager->create<Shader>("ray_texture_shader");
  std::vector<std::string> ray_texture_shader_src = {"shaders/ray_screen.vert", "shaders/ray_screen.geom", "shaders/ray_screen.frag"};
  ray_texture_shader->build(ray_texture_shader_src);

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
                                                0.4f,
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

  cl::Program::Sources raySources;
  std::string src_ray = Helper::loadSource("shaders/ray.cl"); 
  raySources.push_back({src_ray.c_str(),src_ray.length()});

  Log::getDebug().log("EHE!");
  d->createProgram("rayTrace", raySources);
  Log::getDebug().log("OHO!");

  // Load the sources for marching cubes program.
  cl::Program::Sources sources;
  std::string src_mc = Helper::loadSource("shaders/mc.cl"); 
  std::string src_eval = Helper::loadSource("shaders/evalDencity.cl"); 
//  std::string src_eval2 = Helper::loadSource("shaders/Noise2.cl"); 
  sources.push_back({src_mc.c_str(),src_mc.length()});
  sources.push_back({src_eval.c_str(),src_eval.length()});
//  sources.push_back({src_eval2.c_str(),src_eval2.length()});

  // Create the program.
  d->createProgram("mc_program", sources);
  Log::getDebug().log("12093478132904718290347");

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
           rayCamera = !rayCamera;
           //temp = Program::cube_now - 16*Program::x_dim * Program::y_dim; 
           //if (temp < 0) {}
           //else {
           //  Program::cube_now = temp;
           //  glm::vec4 joop = case_values[temp];
           //  window_title = vec_toString(joop) + " : " + std::to_string(temp);
           //  title_changed = true;
           //}
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

void MainProgram::rayTrace(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up, const float focalDistance ) {

//  Log::getDebug().log("RAY_TRACE");
  //const static glm::ivec3 LOCAL_GROUP_SIZE(4,4,4);
  //glm::ivec3 global_group_size(dimensionX, dimensionY, dimensionZ);

  // Finish with opengl.
  glFinish();

  GPU_Device* d = GPU_Device::getInstance();

  cl::CommandQueue* command = d->get<cl::CommandQueue>("command_ray");
  if (command == nullptr) command = d->create<cl::CommandQueue>("command_ray");
//  static float counter = 0.0f;
//  counter += 0.02f;
//  //float buah = 1*sin(counter); 
//  auto pos_now = glm::vec3(0.0f,1.0f,-3.0f);
//  auto target_now = glm::normalize(glm::vec3(0.0f,0.0f,-1.0f));
//  auto up_now = glm::vec3(0.0f,1.0f,0.0f);
//  Log::getDebug().log("POS :: % , TARGET :: %, UP :: %", pos_now, target_now, up_now);
//  Log::getDebug().log("POS :: % , TARGET :: %, UP :: %", pos, target, up);
// struct RayCamera {
// 	glm::vec3 position;
// 	glm::vec3 view;
// 	glm::vec3 up;
// 	glm::vec2 resolution;
// 	glm::vec2 fov;		
// 	float apertureRadius;
// 	float focalDistance;
// };
  
  // Capture opengl ray screen texture,
  //cl_mem mem = clCreateFromGLTexture(*(d->getContext())(), TODOOOOOOOOOOOOOOO 

  Texture* rayRuutu = ResourceManager::getInstance()->get<Texture>("rayRuutu");
//  Log::getDebug().log("rayRuutu == nullptr :: %", rayRuutu == nullptr);
//  Log::getDebug().log("rayRuutu.getID() == %", rayRuutu->getID());
//  cl_int error = CL_SUCCESS;
//  cl::ImageGL screen_texture(*(d->getContext()), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, rayRuutu->getID(), &error); 
//  if (error != CL_SUCCESS) print_cl_error(error);

  cl::ImageGL* screen_texture = d->get<cl::ImageGL>("screen_texture");
  if (screen_texture == nullptr) screen_texture = d->createImage("screen_texture", CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, rayRuutu->getID());

  std::vector<cl::Memory> glObjects;
  glObjects.push_back(*screen_texture);

  cl::Event ev;
  cl_int error = command->enqueueAcquireGLObjects(&glObjects, NULL, &ev);
  ev.wait();

  if (error != CL_SUCCESS) {
    Log::getDebug().log("Program::rayTrace: Failed to acquire opengl objects. ");
    print_cl_error(error);
  }

  glm::vec2 camera_resolution = GlobalPropertyManager::getInstance()->get<Vec2Property>("camera_resolution")->get();

  // Create the camera.
  cl_float3 clpos; clpos.x = pos.x; clpos.y = pos.y; clpos.z = pos.z;// = (cl_float3){pos.x,pos.y,pos.z};
  cl_float3 cltarget; cltarget.x = target.x; cltarget.y = target.y; cltarget.z = target.z; //  = (cl_float3){target.x,target.y,target.z};
  cl_float3 clup; clup.x = up.x; clup.y = up.y; clup.z = up.z; // = (cl_float3){up.x,up.y,up.z};
  //cl_float3 clpos; clpos.x = pos_now.x; clpos.y = pos_now.y; clpos.z = pos_now.z;// = (cl_float3){pos.x,pos.y,pos.z};
  //cl_float3 cltarget; cltarget.x = target_now.x; cltarget.y = target_now.y; cltarget.z = target_now.z; //  = (cl_float3){target.x,target.y,target.z};
  //cl_float3 clup; clup.x = up_now.x; clup.y = up_now.y; clup.z = up_now.z; // = (cl_float3){up.x,up.y,up.z};
  cl_float2 clresolution; clresolution.x = camera_resolution.x; clresolution.y= camera_resolution.y; // = (cl_float2){128.0f,128.0f};
  //cl_float2 clfov; clfov.x = 90.0f; clfov.y = 90.0f; // = (cl_float2){45.0f,45.0f};
  cl_float2 clfov; clfov.x = glm::radians(90.0f); clfov.y = glm::radians(90.0f); // = (cl_float2){45.0f,45.0f};
  
  //cl_float2 clfov; clfov.x = 45.0f; clfov.y = 45.0f; // = (cl_float2){45.0f,45.0f};

  RayCamera r;
  r.position = clpos;
  r.view = cltarget;
  r.up = clup;
  r.resolution = clresolution; // glm::ivec2(128);
  r.fov = clfov; //glm::vec2(45.0f,45.0f);
  r.focalDistance = focalDistance;

  cl::Buffer* rayCamera = d->get<cl::Buffer>("rayCamera");

  if (rayCamera == nullptr) rayCamera = d->createBuffer("rayCamera", sizeof(RayCamera), CL_MEM_READ_WRITE);
	error = command->enqueueWriteBuffer(*rayCamera, CL_TRUE, 0, sizeof(RayCamera), &r);
  if (error != CL_SUCCESS)
  {
    Log::getDebug().log("Program::rayTrace()");
    print_cl_error(error);
  }

  auto ic = InputCache::getInstance();

  int sw = ic->get_screenWidth();
  int sh = ic->get_screenHeight();

//  Log::getDebug().log("sw == % , wh == %", sw, sh); //sw == %, sh == %", sw, sh);
  // The global range. Screen size.
  cl::NDRange global(camera_resolution.x,camera_resolution.y,1); //sw, sh, 1);
  //cl::NDRange global(sw, sh, 1);
  cl::NDRange local(8, 8, 1);

  int theSIZE = camera_resolution.x * camera_resolution.y ; // sw * sh;

//  cl::Buffer* theScreen = d->get<cl::Buffer>("theScreen");
//  if (theScreen == nullptr) theScreen = d->createBuffer("theScreen", sizeof(int)*theSIZE, CL_MEM_READ_WRITE);

//  Log::getDebug().log("theScreen == nullptr :: %", theScreen == nullptr);
  // TODO: check if this exists.
//  cl::Buffer* density_output = d->get<cl::Buffer>("density_values");
//  assert(density_output != nullptr);

//  cl_int error = CL_SUCCESS;

  // THE KERNEL CREATION.

  // THE INDICES for both evalDensity and mc.
  cl::EnqueueArgs eargs(*command, global, local);

  // Find the rayTrace program.
  cl::Program* program = d->get<cl::Program>("rayTrace");
  if (program == nullptr)
  {
    Log::getError().log("Could't find the program."); 
  }
              
//__kernel void render_kernel(const int width, const int height, __global float3* output, __constant const Camera* cam)
  cl::make_kernel<cl::ImageGL, cl::Buffer> ray_kernel(*program,"ray_path",&error);
  if (error != CL_SUCCESS) print_cl_error(error);

  // THE EXECUTION.

  ray_kernel(eargs,*screen_texture, *rayCamera).wait();

//  auto rayOutput = std::make_unique<int[]>(theSIZE);
//  error = command->enqueueReadBuffer(*theScreen,CL_TRUE,0,sizeof(int)*theSIZE, rayOutput.get());
//  if (error != CL_SUCCESS) { print_cl_error(error); }
////  for (int i=0 ; i<theSIZE/8 ; i++) {
////    int p = i*8;
////    Log::getDebug().log("% :: (%,%,%,%,%,%,%,%)", i, rayOutput.get()[p],
////                                                    rayOutput.get()[p+1],
////                                                    rayOutput.get()[p+2],
////                                                    rayOutput.get()[p+3],
////                                                    rayOutput.get()[p+4],
////                                                    rayOutput.get()[p+5],
////                                                    rayOutput.get()[p+6],
////                                                    rayOutput.get()[p+7]);
//////    Log::getDebug().log("% :: (%,%,%,%,%,%,%,%,%,%,%,%,%,%,%,%)", i, rayOutput.get()[p],
//////                                                    rayOutput.get()[p+1],
//////                                                    rayOutput.get()[p+2],
//////                                                    rayOutput.get()[p+3],
//////                                                    rayOutput.get()[p+4],
//////                                                    rayOutput.get()[p+5],
//////                                                    rayOutput.get()[p+6],
//////                                                    rayOutput.get()[p+7],
//////                                                    rayOutput.get()[p+8],
//////                                                    rayOutput.get()[p+9],
//////                                                    rayOutput.get()[p+10],
//////                                                    rayOutput.get()[p+11],
//////                                                    rayOutput.get()[p+12],
//////                                                    rayOutput.get()[p+13],
//////                                                    rayOutput.get()[p+14],
//////                                                    rayOutput.get()[p+15]);
////    if (i>50) break;
////  }
////  throw std::runtime_error("nojoo");

//  auto vb2 = ResourceManager::getInstance()->get<Vertexbuffer>("rayScreen");
//  vb2->populate_data(rayOutput.get(),sizeof(float)*theSIZE*8);

  error = command->enqueueReleaseGLObjects(&glObjects);
  ev.wait();

  if (error != CL_SUCCESS) {
    Log::getDebug().log("Program::rayTrace: Failed to release opengl objects. ");
    print_cl_error(error);
  }

  command->finish();

//  Log::getDebug().log("RAY_TRACE END");
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
