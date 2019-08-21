#include "renderer.h"
#include "window.h"
#include "../Program/Program.h"
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
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    glPointSize(3);
//    glLineWidth(3);
}

void Renderer::render(const Camera& camera) {
  //Log::getDebug().log("camera % .",camera.getMatrix());
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.001f, 1000.0f);
  glm::vec3 eyePosition = camera.getPosition();
  glm::mat4 MVP = projection * camera.getMatrix() * glm::mat4(1.0f);  

  auto rm = ResourceManager::getInstance();
  auto shader = rm->get<Shader>(Program::DEFAULT_RENDERING_SHADER);
  shader->bind();
  shader->setUniform("MVP", MVP);
  shader->setUniform("normalMatrix", glm::inverseTranspose(glm::mat3(1.0)));
  shader->setUniform("M", glm::mat4(1.0f));
  shader->setUniform("lights[0].color", glm::vec3(1.0f,1.0f,1.0f));
  shader->setUniform("lights[0].ambientCoeffience", 0.25f);
  shader->setUniform("lights[0].materialSpecularColor", glm::vec3(1.0f,1.0f,1.0f));
  shader->setUniform("lights[0].materialShininess", 70.0f);
  shader->setUniform("lights[0].attentuationFactor", 0.00009f);
  shader->setUniform("cameraPosition", eyePosition);
  shader->setUniform("lights[0].position", glm::vec3(8.0f,8.0f,8.0f));/* eyePosition);*/
  shader->setUniform("sampler2D", 0);

  auto vb = rm->get<Vertexbuffer>("hah");
  vb->bind();
  glDrawArrays(GL_TRIANGLES, 0, vb->getCount());
}

void Renderer::renderModels(const Camera& camera)
{
}
