#version 330

layout(location = 0) in vec4 a;
layout(location = 1) in vec4 b;
layout(location = 2) in vec4 c;
layout(location = 3) in vec4 d;

//uniform float x_res = 1024.0;
//uniform float y_res = 1024.0;

out DATA_OUT {
  vec4 a_out;
  vec4 b_out;
  vec4 c_out;
  vec4 d_out;
} vs_out;

void main(){

  vs_out.a_out = a;
  vs_out.b_out = b;
  vs_out.c_out = c;
  vs_out.d_out = d;

//  gl_Position = vec4((coords.x/x_res-0.5)*2.0, (coords.y/y_res-0.5)*2.0,0.0,1.0);
//  nCoords = normals;
  //gl_Position = vec4(coords.x, coords.y,0.0,1.0);
}
