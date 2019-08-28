#version 330

layout(location = 0) in float index;

uniform vec4 base_position;
uniform float block_size = 1.0f;;
uniform int x_offset;
uniform int y_offset;
uniform int z_offset;
//uniform int index;

void main()
{

  float x = mod(index,(x_offset*4)); 
  float y = mod(index / (x_offset*4),(y_offset*4)); 
  float z = mod(index / (x_offset * y_offset * 16), z_offset*4); 

  gl_Position =  vec4(x,y,z,0.0);
}
