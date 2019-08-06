//#define CL_HPP_TARGET_OPENCL_VERSION 200
//#include <stdio.h>
//#include <CL/cl.h>
//#include <CL/cl2.hpp>
//#include <CL/cl2.hpp>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#ifdef EMSCRIPTEN
  #include <emscripten.h>
#endif

#include <math.h>
#include <vector>

#include "Graphics/window.h"
#include "Graphics/shader.h"
#include "Graphics/model.h"
#include "Graphics/vertexbuffer.h"
#include "Graphics/renderer.h"
#include "Graphics/texture.h"
#include "Graphics/camera.h"
#include "Graphics/manager.h"
#include "Graphics/model.h"
#include "Graphics/programstate.h"
#include "Graphics/modelManager.h"
#include "OpenCL/GPU_Device.h"
#include "Physics/ParticleGenerator.h"
#include "Utils/log.h"
#include "Utils/misc.h"
#include "Utils/Helper.h"
#include "Utils/static_string.h"
#include "Program/InputCache.h"

//#include <boost/asio.hpp>
//#include "boost/bind.hpp"
/**
 * Context structure that will be passed to the loop handler
 */
struct context
{
    Renderer renderer;
    Camera camera = Camera(glm::vec3(0.0f,0.5f,1.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
};

void createShaders()
{
    const std::string SCENE_SHADER = "cubeshader"; 
    const std::string TUULI_SHADER = "tuulishader"; 

    ParticleSystemManager::getInstance().create("verho", ParticleType::Verho)->init(); 
    ParticleSystemManager::getInstance().create("lumi", ParticleType::Lumi)->init(); 
    ParticleSystemManager::getInstance().create("ruohikko", ParticleType::Ruohikko)->init(); 

    Shader* shaderCube = ShaderManager::getInstance().create(SCENE_SHADER);
    std::vector<std::string> shaderSourcesCube = {"shaders/defaultPoint.vert", "shaders/defaultPoint.frag"};
    shaderCube->build(shaderSourcesCube,false);

    Shader* shader_viivat = ShaderManager::getInstance().create("jousi_particle_render");
    std::vector<std::string> shader_viivat_src = {"shaders/jousi_particle.vert", "shaders/defaultPoint.geom", "shaders/defaultPoint2.frag"};
    shader_viivat->build(shader_viivat_src,false);

    Shader* lumi = ShaderManager::getInstance().create("lumi_render");
    std::vector<std::string> lumi_src = {"shaders/lumi.vert", "shaders/lumi.geom", "shaders/defaultPoint.frag"};
    lumi->build(lumi_src,false);
//    Shader* shader_viivat_verho = ShaderManager::getInstance().create("verho_render");
//    std::vector<std::string> shader_viivat_verho_src = {"shaders/jousi_particle.vert", "shaders/default_verho.geom", "shaders/defaultPoint.frag"};
//    shader_viivat_verho->build(shader_viivat_verho_src,false);

    Shader* tuulishader = ShaderManager::getInstance().create(TUULI_SHADER);
    std::vector<std::string> tuuli_src = {"shaders/tuuliVektorit.vert", "shaders/tuuliVektorit.geom","shaders/tuuliVektorit.frag"};
    tuulishader->build(tuuli_src,false);
    
    ProgramState::getInstance().getMetadata()->meshShader = SCENE_SHADER;
}

// Initialize the marching cubes attributes.
void initializeCubeAttributes()
{
  const int BLOCK_SIZE = 16;
  const int CUBE_COUNT_X = BLOCK_SIZE;
  const int CUBE_COUNT_Y = BLOCK_SIZE;
  const int CUBE_COUNT_Z = BLOCK_SIZE;
  const float ISO_VALUE = 0.0f;

  auto metadata = ProgramState::getInstance().getMetadata();
  metadata->block_size = BLOCK_SIZE; 
  metadata->cube_count_x = CUBE_COUNT_X;
  metadata->cube_count_y = CUBE_COUNT_Y;
  metadata->cube_count_z = CUBE_COUNT_Z;
  metadata->isovalue = ISO_VALUE;
}

void createtextures()
{
  const std::string TEXTURE_NAME = "greenThingTexture"; 
  const std::string TRITABLE_NAME = "tri_table_texture"; 

  auto metadata = ProgramState::getInstance().getMetadata();

  Texture* tex3D = TextureManager::getInstance().create(TEXTURE_NAME,TextureType::d3);
  auto tex3D_data = createPerlin3D(256,256,256);
  tex3D->create3D(tex3D_data);
  metadata->texture3Dname = TEXTURE_NAME;
//  auto texData = tex3D->getTextureData();
//  auto ptr = std::get<0>(texData).get();
//  for (int i=0; i<1300 ; i++)
//  {
//    int iterator = i*4;
//    Log::getInfo().log("tex %: (%,%,%,%)", std::to_string(i),std::to_string(ptr[iterator]),std::to_string(ptr[iterator+1]),std::to_string(ptr[iterator+2]),std::to_string(ptr[iterator+3]));
//  }

  Texture* tritable = TextureManager::getInstance().create(TRITABLE_NAME,TextureType::d1);
  tritable->create_tritable_texture();
  metadata->tri_table_name = TRITABLE_NAME;
}

void loop_handler2(void *arg)
{
    context* c = static_cast<context*>(arg);
    SDL_Event e;

    ProgramState::getInstance().updateTick();
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            ProgramState::getInstance().setAppRunning(false);
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_1:
                    ProgramState::getInstance().toggleLumi();
                    break;
                case SDLK_2:
                    ProgramState::getInstance().toggleVerho();
                    break;
                case SDLK_3:
                    ProgramState::getInstance().toggleRuohikko();
                    break;
                case SDLK_4:
                    changeScene(4);
                    break;
                case SDLK_5:
                    changeScene(5);
                    break;
                case SDLK_6:
                    changeScene(6);
                    break;
                case SDLK_7:
                    changeScene(7);
                    break;
                case SDLK_8:
                    changeScene(8);
                    break;
                case SDLK_9:
                    changeScene(9);
                    break;
                case SDLK_0:
                    ProgramState::getInstance().resetWind();
                    break;
                case SDLK_KP_PLUS:
                    {
                      ProgramState::getInstance().increaseWindStrength(2.0f);
                      auto strength = ProgramState::getInstance().getWindStrength();
                      Log::getInfo().log("Wind stength: %", std::to_string(strength));
                      break;
                    }
                case SDLK_KP_MINUS:
                    {
                      ProgramState::getInstance().increaseWindStrength(-2.0f);
                      auto strength = ProgramState::getInstance().getWindStrength();
                      Log::getInfo().log("Wind stength: %", std::to_string(strength));
                      break;
                    }
                case SDLK_SPACE:
                    {
                    ProgramState::getInstance().toggleWind();
                    break;
                    }
                case SDLK_RETURN:
                    {
                    ProgramState::getInstance().toggleStopWind();
                    Log::getInfo().log("Wind stopped: %", ProgramState::getInstance().getStopWind() ? "True" : "False");
                    break;
                    }

                case SDLK_q: 
                    ProgramState::getInstance().setAppRunning(false);
                    break;
                case SDLK_ESCAPE: 
                    ProgramState::getInstance().setAppRunning(false);
                    break;
                case SDLK_t:
                    {
                    auto metadata = ProgramState::getInstance().getMetadata();
                    auto name = metadata->texture3Dname;
                    // TODO::: MODIFY ::: TODO
                    //TextureManager::getInstance().deleteTexture(name); 
                    TextureManager::getInstance().del(name); 
                    // TODO::: MODIFY ::: TODO
                    //Texture tex3D = TextureManager::getInstance().create3D(name);
                    Texture* tex3D = TextureManager::getInstance().create(name,TextureType::d3);
                    auto tex3D_data = createPerlin3D(256,256,256);
                    tex3D->create3D(tex3D_data);
                    metadata->texture3Dname = name;
                    Log::getInfo().log("Creating a new 3D texture...");
                    // Recreate scene model.
                    Log::getInfo().log("Rebuilding scene...");
                    //ModelManager::getInstance().createSceneObject();
                    break;
                    }
                case SDLK_y: 
                    {
                    auto metadata = ProgramState::getInstance().getMetadata();
                    auto name = metadata->texture3Dname;
                    // TODO::: MODIFY ::: TODO
                    //TextureManager::getInstance().deleteTexture(name); 
                    TextureManager::getInstance().del(name); 
                    // TODO::: MODIFY ::: TODO
                    //Texture tex3D = TextureManager::getInstance().create3D(name);
                    Texture* tex3D = TextureManager::getInstance().create(name,TextureType::d3);
                    auto tex3D_data = createPerlin3D_rough(256,256,256);
                    tex3D->create3D(tex3D_data);
                    metadata->texture3Dname = name;
                    Log::getInfo().log("Creating a new 3D texture with more rought noise...");
                    // Recreate scene model.
                    Log::getInfo().log("Rebuilding scene...");
                    //ModelManager::getInstance().createSceneObject();
                    break;
                    }
            }
        case SDL_WINDOWEVENT:
            switch(e.window.event)
            {
              case SDL_WINDOWEVENT_CLOSE:
                    ProgramState::getInstance().setAppRunning(false);
                  break;
              case SDL_WINDOWEVENT_RESIZED:
              {
                  int w = e.window.data1;
                  int h = e.window.data2;
                  glViewport(0,0,w,h);
                  break;
              }
            }

        /* Camera. */
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
            c->camera.handleMouseInput(e);
            break;

        }
    }
    c->camera.handleKeyInput();
    Window::getInstance().swapBuffers();
    c->renderer.render(c->camera);
    // Kasvatetaan h laskuria.
    ProgramState::getInstance().increase_h_sum(ProgramState::getInstance().getTimeStep());
}

void yhyy(const InputCache* ic) {
  Log::getDebug().log("yhyy :: just debugging.");
}

int main(int argc, char* argv[])
{
  Window window = Window::getInstance();

  auto eh = InputCache::getInstance();
  eh->init();

  bool running = true;
  //const std::function<void(const InputCache*)>* id1 = eh->register_keyboardMouse([&](const InputCache* c) { running = !c->isKeyUp('a'); });
  auto id1 = eh->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) { running = !c->isKeyUp('a'); });
  Log::getError().log("id1 == %",id1);
  //(*id1)(eh);
  //void (&eki)(const InputCache*);
  //eki = &yhyy;
  //auto* f_ptr (yhyy);
  //(*f_ptr)(eh);
  //auto f123 = [&](const InputCache* c) { running = !c->isKeyUp('a'); };
  //std::cout << type_name<decltype(eki)>() << std::endl; 
  //std::cout << type_name<decltype(f123)>() << std::endl; 
  auto id3 = eh->register_function_pointer(EventType::KEYBOARD_MOUSE,&yhyy);
  Log::getError().log("id3 == %",id3);
//  (*id3)(eh);
  //auto id3 = eh->register_function(EventType::KEYBOARD_MOUSE,eki);
//  //Log::getDebug().log("Hello::main: %", id1);
//  Log::getDebug().log("Hello::main: PAH");
//  //Log::getDebug().log("Hello::main: %", id3);
  eh->unregister(id1);
  //Log::getError().log("id1 == %",id1);
  //eh->unregister(id1);
  //auto id1 = eh->register_keyboardMouse([&](const InputCache* c) { running = !c->isKeyUp('a'); });
  int id2 = eh->register_quit([&](const InputCache* c) { running = false; });

  while (running) {
    eh->pollEvents();
  }


  //auto d = GPU_Device::getInstance();
  //d->init();
  //
  //
  //eh->registerKeysum( [&](const Uint8* keystate) {
  //                     if( keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] ) Log::getDebug().log("Ylos");
  //                     if( keystate[SDL_SCANCODE_Q]) running = false; });
  //
//  std::cout << static_string(SDLK_t) << std::endl;
//  OCL ocl;
//  ocl.init();
//  ocl.createProgram();
//
//  Helper::loadSource("erkki!!");

	//cl::Program context({default_device});

////  ParticleGenerator r;
////
////  // The program state must be created first.
////  // Laitetaan jo tassa time step.
////  //ProgramState::getInstance().setTimeStep(0.0004f);
////  //ProgramState::getInstance().setTimeStep(0.0008f);
////  ProgramState::getInstance().setTimeStep(0.0012f);
////  
////  // Initialize the base information for the marching cubes.
////  initializeCubeAttributes();
////
////  // Create the window. This also creates the opengl context.
////  Window window = Window::getInstance();
////
////  // Create all textures.
////  createtextures();
////
////  // Context creation. (webassembly).
////  context c;
////
////  // We create marching cubes shader only with native opengl.
////    
////  #ifndef EMSCRIPTEN
////
////  createShaders();
////
////  VertexBufferManager::getInstance().createExamplePoints(30, 30, 30,30.0, glm::vec3(-0.2f,-0.1f,-0.4f), "tuuli_pisteet");
////  VertexBufferManager::getInstance().createExamplePoints(40, 1, 40,50.0, glm::vec3(-0.2f,0.0f,-0.2f), "maa_pisteet");
////  #endif
////
////  // Initialize the renderer.
////  c.renderer.init();
////
////    /**
////     * Schedule the main loop handler to get 
////     * called on each animation frame
////     */
////
////    #ifdef EMSCRIPTEN
////    emscripten_set_main_loop_arg(loop_handler2, &c, -1, 1);
////    #endif
////
//////    void generateGrass(const uint32_t grassCount, const uint32_t grassHeight, const uint32_t areaWidth, const uint32_t areaheight);
////    #ifndef EMSCRIPTEN
////    while (ProgramState::getInstance().getAppRunning())
////    {
////      loop_handler2(&c);
////    }
////    #endif
////
////    return 0;
}
