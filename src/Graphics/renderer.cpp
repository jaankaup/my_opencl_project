#include "renderer.h"
#include "window.h"
#include "../Program/Program.h"
#include "../Program/GlobalPropertyManager.h"
#include "../Program/ResourceManager.h"
Renderer::Renderer()
{
}
	
Renderer::~Renderer()
{

}

void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);
    glLineWidth(5);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::render(const Camera& camera) {
  //Log::getDebug().log("camera % .",camera.getMatrix());
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.001f, 1000.0f);
  glm::vec3 eyePosition = camera.getPosition();
  glm::mat4 MVP = projection * camera.getMatrix() * glm::mat4(1.0f);  

  auto glob_manager = Program::GlobalPropertyManager::getInstance();

  auto rm = ResourceManager::getInstance();
  auto shader = rm->get<Shader>(Program::DEFAULT_RENDERING_SHADER);
  shader->bind();

  Texture* ruohikko = rm->get<Texture>("ruohikko");
  ruohikko->use(0);
  shader->setUniform("diffuseTexture", 0);

  Texture* kallio = rm->get<Texture>("kallio");
  kallio->use(1);
//  ruohikko->use(1);
  shader->setUniform("diffuseTexture2", 1);

//  Texture* kallio = rm->get<Texture>("kallio");
//  kallio->use(1);
//  shader->setUniform("diffuseTexure2", 1);

  auto show_scene = glob_manager->get<Program::BoolProperty>("show_scene")->get();

  shader->setUniform("MVP", MVP);
  shader->setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(1.0)));
  shader->setUniform("M", glm::mat4(1.0f));
  shader->setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader->setUniform("lights[0].ambientCoeffience", 0.25f);
  shader->setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader->setUniform("lights[0].materialShininess", 70.0f);
  shader->setUniform("lights[0].attentuationFactor", 0.00009f);
  shader->setUniform("cameraPosition", eyePosition);
  shader->setUniform("lights[0].position", glm::vec3(8.0f,18.0f,8.0f));/* eyePosition);*/
  shader->setUniform("diffuseTexure2", 1);
  //shader->setUniform("sampler2D", 1);

  if (show_scene) {
    auto vb = rm->get<Vertexbuffer>("hah");
    vb->bind();
    glDrawArrays(GL_TRIANGLES, 0, vb->getCount());
  }

  glFrontFace(GL_CW);
  auto shader_wire = rm->get<Shader>("cube_wire");
  shader_wire->bind();
  shader_wire->setUniform("MVP", MVP);
  shader_wire->setUniform("base_position", Program::bPOS);
  shader_wire->setUniform("x_offset", Program::x_dim);
  shader_wire->setUniform("y_offset", Program::y_dim);
  shader_wire->setUniform("z_offset", Program::z_dim);
  shader_wire->setUniform("block_size", Program::bSIZE);
//  shader_wire->setUniform("index", int(Program::cube_float));

  auto vb_wire = rm->get<Vertexbuffer>("hopohopo");
  vb_wire->bind();
  glDrawArrays(GL_POINTS, 0, 1 /* vb_wire->getCount() */);
  //Program::cube_now +=1;
  //Program::cube_float += 10.0f;
  //  Log::getDebug().log("Program::cube_now == %", Program::cube_now);
  if (Program::cube_now > Program::x_dim * Program::y_dim * Program::z_dim * 64)
  {
    Program::cube_now = 0;
    Log::getDebug().log("Program::cube_now == %", Program::cube_now);
  }

  glFrontFace(GL_CCW);

  
  auto show_den = glob_manager->get<Program::BoolProperty>("show_density")->get();
  if (!show_den) return;
  
  auto shader_density_points = rm->get<Shader>("density_shader");
  shader_density_points->bind();
  shader_density_points->setUniform("MVP", MVP);
  shader_density_points->setUniform("base_position", Program::bPOS);
  shader_density_points->setUniform("x_offset", Program::x_dim);
  shader_density_points->setUniform("y_offset", Program::y_dim);
  shader_density_points->setUniform("z_offset", Program::z_dim);
  shader_density_points->setUniform("block_size", Program::bSIZE);
  auto vb_density_points = ResourceManager::getInstance()->get<Vertexbuffer>("density_points");
  vb_density_points->bind();
  glDrawArrays(GL_POINTS, 0, vb_density_points->getCount());

}

void Renderer::renderModels(const Camera& camera)
{
}
