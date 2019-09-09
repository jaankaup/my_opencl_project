#version 330

in vec4 fUV;

out vec3 color;

uniform mat4 viewMatrix;
uniform vec3 cameraPosition;

void main(){
   
  color = vec3(0.2f,1.0f,0.9f);
}
