#include "Program.h"
#include "GlobalPropertyManager.h"
#include "ResourceManager.h"
#include "../Utils/log.h"
#include "../Utils/Helper.h"
#include "../Graphics/window.h"
#include "../Graphics/camera.h"
#include "../Graphics/renderer.h"
#include "../Graphics/shader.h"
#include "../Graphics/texture.h"
#include "../OpenCL/GPU_Device.h"
#include "../OpenCL/CL_Helper.h"
#include "Marching_Cubes_data.h"

namespace Program {

std::unique_ptr<float[]> density_values;
std::unique_ptr<glm::vec4[]> case_values;
int cube_now = Program::x_dim * Program::y_dim *  Program::z_dim * 64;//0;
float bSIZE = 0.1f;
int x_dim = 32;
int y_dim = 16;
int z_dim = 32;
float cube_float = float(x_dim * y_dim * z_dim * 64 / 3);;
glm::vec4 bPOS = glm::vec4(-5.2f,-3.0f,-5.2f,0.0f);

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
  Camera camera = Camera(glm::vec3(5.0f,10.0f,0.0f),glm::vec3(0.0f,50.0f,-20.0f),glm::vec3(0.0f,1.0f,0.0f));

  // Register q for stoppin the application.
  auto id1 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) { running = !c->isKeyReleased('q'); });
  registerHandlers();

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

  //  .
  BoolProperty show_density;
  show_density.set(false);
  glob_manager->add("show_density",show_density);

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
  float hopohopo_data[1] = {Program::cube_float};
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
         float temp;
         auto win = Window::getInstance();
         if (c->isKeyPressed('n'))
         {
           temp = Program::cube_float + 4*Program::x_dim; 
           if (temp >= 64*Program::x_dim * Program::y_dim * Program::z_dim) {}
           else {
             Program::cube_float = temp;
             glm::vec4 joop = case_values[temp];
             win->setTitle(vec_toString(joop) + " : " + std::to_string(temp));
           }
         } 
         if (c->isKeyPressed('m'))
         {
           temp = Program::cube_float - 4*Program::x_dim; 
           if (temp < 0) {}
           else {
             Program::cube_float = temp;
             glm::vec4 joop = case_values[temp];
             win->setTitle(vec_toString(joop) + " : " + std::to_string(temp));
           }
         } 
         if (c->isKeyPressed('k'))
         {
           temp = Program::cube_float - 1; 
           if (temp < 0) {}
           else {
             Program::cube_float = temp;
             glm::vec4 joop = case_values[temp];
             win->setTitle(vec_toString(joop) + " : " + std::to_string(temp));
           }
         } 
         if (c->isKeyPressed('h'))
         {
           temp = Program::cube_float + 1; 
           if (temp >= 64*Program::x_dim * Program::y_dim * Program::z_dim) {}
           else {
             Program::cube_float = temp;
             glm::vec4 joop = case_values[temp];
             win->setTitle(vec_toString(joop) + " : " + std::to_string(temp));
           }
         } 
         if (c->isKeyPressed('u'))
         {
           temp = Program::cube_float + 16*Program::x_dim * Program::y_dim; 
           if (temp >= 64*Program::x_dim * Program::y_dim * Program::z_dim) {}
           else {
             Program::cube_float = temp;
             glm::vec4 joop = case_values[temp];
             win->setTitle(vec_toString(joop) + " : " + std::to_string(temp));
           }
         }
         if (c->isKeyPressed('j'))
         {
           temp = Program::cube_float - 16*Program::x_dim * Program::y_dim; 
           if (temp < 0) {}
           else {
             Program::cube_float = temp;
             glm::vec4 joop = case_values[temp];
             win->setTitle(vec_toString(joop) + " : " + std::to_string(temp));
           }
         } 

         if (c->isKeyPressed('5'))
         {
           density_values[int(cube_float) + 1] = 1.0f;
           density_values[int(cube_float)] = 1.0f;
           density_values[int(Program::x_dim*4 + cube_float)] = 1.0f;
           density_values[int(Program::x_dim*4 + cube_float + 1)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] = -0.1f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] = -0.1f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] = -0.1f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] = -0.1f;
           this->updateScene();
         } 

         if (c->isKeyPressed('6'))
         {
           density_values[int(cube_float) + 1] = 1.0f;
           density_values[int(cube_float)] = 1.0f;
           density_values[int(Program::x_dim*4 + cube_float)] = 1.0f;
           density_values[int(Program::x_dim*4 + cube_float + 1)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] = 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] = 1.0f;
           this->updateScene();
         } 

         if (c->isKeyPressed('7'))
         {
           density_values[int(cube_float) + 1] = -0.1f;
           density_values[int(cube_float)] = -0.1f;
           density_values[int(Program::x_dim*4 + cube_float)] = -0.1f;
           density_values[int(Program::x_dim*4 + cube_float + 1)] = -0.1f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] = 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] = 1.0f;
           this->updateScene();
         } 

         if (c->isKeyPressed('8'))
         {
           density_values[int(cube_float) + 1] = 1.0f;
           density_values[int(cube_float)] = 1.0f;
           density_values[int(Program::x_dim*4 + cube_float)] = 1.0f;
           density_values[int(Program::x_dim*4 + cube_float + 1)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] = 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] = 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] = 1.0f;
           this->updateScene();
         } 

         if (c->isKeyPressed('9'))
         {
           density_values[int(cube_float) + 1] += -1.0f;
           density_values[int(cube_float)] += -1.0f;
           density_values[int(Program::x_dim*4 + cube_float)] += -1.0f;
           density_values[int(Program::x_dim*4 + cube_float + 1)] += -1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] += -1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] += -1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] += -1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] += -1.0f;
           this->updateScene();
         } 

         if (c->isKeyPressed('0'))
         {
           density_values[int(cube_float) + 1] += 1.0f;
           density_values[int(cube_float)] += 1.0f;
           density_values[int(Program::x_dim*4 + cube_float)] += 1.0f;
           density_values[int(Program::x_dim*4 + cube_float + 1)] += 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16) + 1] += 1.0f;
           density_values[int(cube_float + Program::x_dim * Program::y_dim*16)] += 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)] += 1.0f;
           density_values[int(cube_float + Program::x_dim * 4 + Program::x_dim * Program::y_dim*16)+1] += 1.0f;
           this->updateScene();
         } 

         auto vb = ResourceManager::getInstance()->get<Vertexbuffer>("hopohopo");
         //Log::getDebug().log("INDEX == %", temp);
         
         float hopohopo_data[1] = {Program::cube_float};
         vb->populate_data(&hopohopo_data,sizeof(float));
      });
}

/////////////////////////////////////////////////////////////////////////////////////

} // namespace Program
