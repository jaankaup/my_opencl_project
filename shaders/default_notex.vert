#version 330

layout(location = 0) in vec4 vert_in;
layout(location = 1) in vec4 normal_in;
layout(location = 2) in vec4 normal_in_flat;

out vec3 vFrag_in;
out vec3 nFrag_in;
out vec3 nFrag_in_flat;

uniform mat4 MVP;
uniform float normal_mode = 1.0;

void main(){

  gl_Position =  MVP * vec4(vert_in.xyz,1.0);
  vFrag_in = vert_in.xyz;
  nFrag_in = normal_in.xyz;
  nFrag_in_flat = normal_in_flat.xyz;
//  if (normal_mode == 0.0) nFrag_in = normal_in.xyz;
//  else nFrag_in = normal_in_flat.xyz;
}
