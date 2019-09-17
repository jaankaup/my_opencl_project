#version 330

layout(location = 0) in vec4 coords;
layout(location = 1) in vec4 normals;

uniform float x_res = 1024.0;
uniform float y_res = 1024.0;
out vec4 fCoords;
out vec4 nCoords;

void main(){
  gl_Position = vec4((coords.x/x_res-0.5)*2.0, (coords.y/y_res-0.5)*2.0,0.0,1.0);
  nCoords = normals;
  //gl_Position = vec4(coords.x, coords.y,0.0,1.0);
}
