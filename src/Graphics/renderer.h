#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <chrono>
#include <GL/glew.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "shader.h"
#include "vertexbuffer.h"
#include "texture.h"
#include "camera.h"
#include "../Utils/log.h"
#include "../Utils/misc.h"

/** A simple shader class with basic shader operations. */
class Renderer
{
	public:
		Renderer();
		~Renderer();

    void init();
    void renderModels(const Camera& camera);
    void render(const Camera& camera);

	private:

};


#endif // RENDERER_H
