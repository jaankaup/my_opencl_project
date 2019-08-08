#include "Program.h"
#include "GlobalPropertyManager.h"
#include "../Utils/log.h"
#include "../Graphics/window.h"
#include "../Graphics/camera.h"
#include "../Graphics/renderer.h"

namespace Program {

bool MainProgram::initialize()
{

  // Initialize InputCache.
//  auto ic = InputCache::getInstance();
//  ic->init();

  if (!createWindow()) return false;
  createGlobalProperties();
  createOpencl();
  createShaders();
  createTextures();
}

void MainProgram::start()
{
  bool running = true;

  auto ic = InputCache::getInstance();

  // Create the renderer.
  Renderer renderer;
  renderer.init();

  // Create the camera for this application.
  Camera camera = Camera(glm::vec3(0.0f,0.5f,1.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

  // Register q for stoppin the application.
  auto id1 = ic->register_lambda_function(EventType::KEYBOARD_MOUSE,[&](const InputCache* c) { Log::getDebug().log("YYY"); running = !c->isKeyReleased('q'); });

  while (running) {
    ic->pollEvents();
    camera.handleKeyInput();
    Window::getInstance()->swapBuffers();
    //renderer.render(camera);
  };
}

void MainProgram::createGlobalProperties()
{

  auto glob_manager = GlobalPropertyManager::getInstance();

  // The name for defaut_rendering shader.
  StringProperty s;
  s.set("default_shader");
  glob_manager->add("DEFAULT_SHADER",s);

//  std::cout << s.get() << std::endl;
//  auto hevonkukkuu2 = ptrManager->get<StringProperty>(std::string(std::string("kala")));
//  if (hevonkukkuu2->has_value())
//  {
//     auto val = &(hevonkukkuu2->value());
//     std::cout << val->get() << std::endl;
//  }
}

void MainProgram::createTextures()
{

}

void MainProgram::createShaders()
{

}

bool MainProgram::createWindow()
{
  // Initialize window/sdl/opengl context.
  Window* window = Window::getInstance();
  window->setTitle("Juupajooo");
}

void MainProgram::createOpencl()
{

}

void MainProgram::registerHandlers()
{

}

} // namespace Program
